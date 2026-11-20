// Performance tools: frame-time capture (game / render / GPU split), a scene cost census with plain-language
// hints, and a one-frame GPU pass profile. Work in Play In Editor (first PIE world) or the editor level.
#include "EnginewrightTool.h"
#include "EWCommon.h"

#include "Components/LightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BackgroundBlur.h"
#include "Blueprint/UserWidget.h"
#include "Editor.h"
#include "Editor/EditorPerformanceSettings.h"
#include "Engine/Engine.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInterface.h"
#include "Misc/App.h"
#include "RenderCore.h"
#include "RHI.h"
#include "RHIStats.h"
#include "StaticMeshResources.h"
#include "UObject/UObjectIterator.h"

namespace
{
	double R1(double V) { return FMath::RoundToDouble(V * 10) / 10; }
	double R2(double V) { return FMath::RoundToDouble(V * 100) / 100; }

	UWorld* TargetWorld(FString& OutWhere)
	{
		const TArray<UWorld*> Pie = EW::PieWorlds();
		if (Pie.Num() > 0) { OutWhere = TEXT("pie"); return Pie[0]; }
		OutWhere = TEXT("editor");
		return GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	}

	double Pct(TArray<double> V, double P)
	{
		if (V.Num() == 0) { return 0; }
		V.Sort();
		const int32 I = FMath::Clamp(FMath::FloorToInt(P * (V.Num() - 1) + 0.5), 0, V.Num() - 1);
		return V[I];
	}

	double Avg(const TArray<double>& V)
	{
		double S = 0;
		for (double X : V) { S += X; }
		return V.Num() ? S / V.Num() : 0;
	}

	TSharedRef<FJsonObject> Series(const TArray<double>& V)
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		J->SetNumberField(TEXT("avg"), R2(Avg(V)));
		J->SetNumberField(TEXT("p50"), R2(Pct(V, 0.5)));
		J->SetNumberField(TEXT("p95"), R2(Pct(V, 0.95)));
		J->SetNumberField(TEXT("max"), R2(Pct(V, 1.0)));
		return J;
	}

	struct FPerfSamples
	{
		TArray<double> Frame, Game, Render, Rhi, Gpu, Draws, Prims;
		double Start = 0;
	};

	FString CVarStr(const TCHAR* Name)
	{
		IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(Name);
		return V ? V->GetString() : FString(TEXT("(none)"));
	}
	int32 CVarInt(const TCHAR* Name, int32 Default = -1)
	{
		IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(Name);
		return V ? V->GetInt() : Default;
	}

	TSharedRef<FJsonObject> RenderSettings()
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		const TCHAR* Names[] = {
			TEXT("r.DynamicGlobalIlluminationMethod"), TEXT("r.ReflectionMethod"), TEXT("r.Shadow.Virtual.Enable"),
			TEXT("r.AntiAliasingMethod"), TEXT("r.ScreenPercentage"), TEXT("r.DynamicRes.OperationMode"), TEXT("t.MaxFPS"),
			TEXT("r.VSync"), TEXT("r.Nanite"), TEXT("r.MotionBlurQuality"), TEXT("r.BloomQuality"), TEXT("r.DepthOfFieldQuality"),
			TEXT("r.Lumen.ScreenProbeGather.ScreenTraces"), TEXT("r.Lumen.HardwareRayTracing"), TEXT("r.VolumetricFog"),
			TEXT("sg.ResolutionQuality"), TEXT("sg.ViewDistanceQuality"), TEXT("sg.ShadowQuality"), TEXT("sg.GlobalIlluminationQuality"),
			TEXT("sg.ReflectionQuality"), TEXT("sg.PostProcessQuality"), TEXT("sg.TextureQuality"), TEXT("sg.EffectsQuality"),
			TEXT("sg.FoliageQuality"), TEXT("sg.ShadingQuality") };
		for (const TCHAR* N : Names) { J->SetStringField(N, CVarStr(N)); }
		return J;
	}
}

void RegisterPerfTools(FEWRegistry& R)
{
	R.RegisterAsync(TEXT("perf_capture"),
		TEXT("Measure frame time for some seconds in Play In Editor (or the editor level when PIE is off): frame, game thread, render thread, RHI thread and GPU milliseconds (avg / p50 / p95 / max), FPS, hitches, draw calls and triangles per frame, and which side bounds the frame (game, render or GPU). Editor background throttling is switched off during the capture so an unfocused editor still measures real numbers."),
		TEXT(R"({"type":"object","properties":{
			"seconds":{"type":"number","minimum":0.5,"maximum":60,"description":"Capture length, default 5, max 60."},
			"warmup":{"type":"number","description":"Seconds to skip first (shader compile / streaming spikes), default 1."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			FString Where;
			if (!TargetWorld(Where)) { Done(FEWResult::Error(TEXT("NO_LEVEL"), TEXT("No PIE world and no editor level."))); return; }
			const double Seconds = FMath::Clamp(EW::Num(Args, TEXT("seconds"), 5), 0.5, 60.0);
			const double Warmup = FMath::Clamp(EW::Num(Args, TEXT("warmup"), 1), 0.0, 30.0);
			UEditorPerformanceSettings* Perf = GetMutableDefault<UEditorPerformanceSettings>();
			const bool bThrottle = Perf->bThrottleCPUWhenNotForeground;
			Perf->bThrottleCPUWhenNotForeground = false;
			TSharedRef<FPerfSamples> S = MakeShared<FPerfSamples>();
			S->Start = FPlatformTime::Seconds();
			auto Finish = [S, Done, Where, bThrottle, Perf]()
			{
				Perf->bThrottleCPUWhenNotForeground = bThrottle;
				if (S->Frame.Num() == 0) { Done(FEWResult::Error(TEXT("NO_FRAMES"), TEXT("No frames were captured."))); return; }
				TSharedRef<FJsonObject> D = MakeShared<FJsonObject>();
				D->SetStringField(TEXT("world"), Where);
				D->SetNumberField(TEXT("frames"), S->Frame.Num());
				const double AvgFrame = Avg(S->Frame);
				D->SetNumberField(TEXT("fpsAvg"), R1(AvgFrame > 0 ? 1000.0 / AvgFrame : 0));
				D->SetNumberField(TEXT("fps1PercentLow"), R1(1000.0 / FMath::Max(0.01, Pct(S->Frame, 0.99))));
				D->SetObjectField(TEXT("frameMs"), Series(S->Frame));
				D->SetObjectField(TEXT("gameMs"), Series(S->Game));
				D->SetObjectField(TEXT("renderMs"), Series(S->Render));
				D->SetObjectField(TEXT("rhiMs"), Series(S->Rhi));
				D->SetObjectField(TEXT("gpuMs"), Series(S->Gpu));
				D->SetObjectField(TEXT("drawCalls"), Series(S->Draws));
				D->SetObjectField(TEXT("trianglesK"), Series(S->Prims));
				const double Med = Pct(S->Frame, 0.5);
				int32 Hitches = 0;
				for (double F : S->Frame) { if (F > Med * 2.0 && F > 20.0) { ++Hitches; } }
				D->SetNumberField(TEXT("hitches"), Hitches);
				const double G = Pct(S->Game, 0.5), Rt = Pct(S->Render, 0.5), Gp = Pct(S->Gpu, 0.5);
				FString Bound = TEXT("gpu");
				if (G >= Rt && G >= Gp) { Bound = TEXT("game"); }
				else if (Rt >= Gp) { Bound = TEXT("render"); }
				// A frame far longer than every thread = waiting on a frame cap or VSync.
				if (Med > FMath::Max3(G, Rt, Gp) * 1.4 && Med > 5.0) { Bound = TEXT("capped (t.MaxFPS / VSync / editor idle)"); }
				D->SetStringField(TEXT("boundBy"), Bound);
				D->SetObjectField(TEXT("settings"), RenderSettings());
				TArray<FString> Hints;
				if (Bound == TEXT("gpu")) { Hints.Add(TEXT("GPU bound: run perf_gpu_profile to see the costly passes (Lumen, shadows, translucency, post).")); }
				if (Bound == TEXT("game")) { Hints.Add(TEXT("Game thread bound: look for heavy Tick (Blueprint/C++), widget rebuilds or many actors; perf_scene lists actor counts.")); }
				if (Bound == TEXT("render")) { Hints.Add(TEXT("Render thread bound: too many draw calls / primitives; merge meshes, use instancing, cut dynamic shadow casters.")); }
				if (CVarInt(TEXT("t.MaxFPS"), 0) <= 0 && CVarInt(TEXT("r.VSync"), 0) == 0) { Hints.Add(TEXT("No frame cap and VSync off: menus spin the GPU at 100% (fans, heat). Cap menus (e.g. t.MaxFPS 60) or enable VSync.")); }
				if (Where == TEXT("editor")) { Hints.Add(TEXT("Measured the editor viewport, not the game. Start PIE for game numbers; the editor UI itself costs frame time.")); }
				TArray<TSharedPtr<FJsonValue>> HJ;
				for (const FString& H : Hints) { HJ.Add(MakeShared<FJsonValueString>(H)); }
				D->SetArrayField(TEXT("hints"), HJ);
				Done(FEWResult::Ok(D, FString::Printf(TEXT("%.1f FPS avg (%s), p95 frame %.1f ms, bound by %s."), AvgFrame > 0 ? 1000.0 / AvgFrame : 0, *Where, Pct(S->Frame, 0.95), *Bound)));
			};
			EW::PollUntil(Seconds + Warmup + 5.0,
				[S, Seconds, Warmup, Finish]() -> bool
				{
					const double T = FPlatformTime::Seconds() - S->Start;
					if (T < Warmup) { return false; }
					S->Frame.Add(FApp::GetDeltaTime() * 1000.0);
					S->Game.Add(FPlatformTime::ToMilliseconds(GGameThreadTime));
					S->Render.Add(FPlatformTime::ToMilliseconds(GRenderThreadTime));
					S->Rhi.Add(FPlatformTime::ToMilliseconds(GRHIThreadTime));
					S->Gpu.Add(FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0)));
					S->Draws.Add((double)GNumDrawCallsRHI[0]);
					S->Prims.Add((double)GNumPrimitivesDrawnRHI[0] / 1000.0);
					if (T >= Warmup + Seconds) { Finish(); return true; }
					return false;
				},
				[Finish]() { Finish(); });
		});

	R.Register(TEXT("perf_scene"),
		TEXT("Cost census of the PIE world (or editor level): actors, visible primitives, static mesh triangles (LOD0, top meshes), skeletal meshes, lights by type with shadow casters and mobility, translucent materials, Niagara and post process volumes, widgets on screen and background blurs, plus the render settings that cost the most (Lumen, virtual shadows, AA, frame cap) and plain-language hints."),
		TEXT(R"({"type":"object","properties":{
			"top":{"type":"integer","description":"How many of the heaviest meshes to list, default 10."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Where;
			UWorld* World = TargetWorld(Where);
			if (!World) { return FEWResult::Error(TEXT("NO_LEVEL"), TEXT("No PIE world and no editor level.")); }
			const int32 Top = FMath::Clamp((int32)EW::Num(Args, TEXT("top"), 10), 1, 50);
			int32 Actors = 0, Prims = 0, Hidden = 0, SM = 0, ISMInst = 0, SK = 0, Niagara = 0, PPV = 0, Translucent = 0, ShadowPrims = 0;
			int32 LDir = 0, LPoint = 0, LSpot = 0, LOther = 0, LShadow = 0, LMovable = 0;
			double Tris = 0;
			TMap<FString, double> MeshTris;
			TSet<UMaterialInterface*> Mats;
			for (TActorIterator<AActor> It(World); It; ++It)
			{
				AActor* A = *It;
				if (!IsValid(A)) { continue; }
				++Actors;
				if (A->IsA<APostProcessVolume>()) { ++PPV; }
				TArray<UActorComponent*> Comps;
				A->GetComponents(Comps);
				for (UActorComponent* C : Comps)
				{
					if (!C || !C->IsRegistered()) { continue; }
					if (C->GetClass()->GetName().Contains(TEXT("Niagara")) && C->IsActive()) { ++Niagara; }
					if (ULightComponent* L = Cast<ULightComponent>(C))
					{
						if (!L->IsVisible()) { continue; }
						if (L->IsA<UDirectionalLightComponent>()) { ++LDir; }
						else if (L->IsA<USpotLightComponent>()) { ++LSpot; }
						else if (L->IsA<UPointLightComponent>()) { ++LPoint; }
						else { ++LOther; }
						if (L->CastShadows) { ++LShadow; }
						if (L->Mobility == EComponentMobility::Movable) { ++LMovable; }
						continue;
					}
					UPrimitiveComponent* P = Cast<UPrimitiveComponent>(C);
					if (!P || P->IsEditorOnly()) { continue; }
					if (!P->IsVisible() || P->bHiddenInGame) { ++Hidden; continue; }
					++Prims;
					if (P->CastShadow) { ++ShadowPrims; }
					TArray<UMaterialInterface*> Used;
					P->GetUsedMaterials(Used);
					for (UMaterialInterface* M : Used)
					{
						if (!M) { continue; }
						Mats.Add(M);
						if (IsTranslucentBlendMode(M->GetBlendMode())) { ++Translucent; }
					}
					if (UStaticMeshComponent* S = Cast<UStaticMeshComponent>(P))
					{
						UStaticMesh* Mesh = S->GetStaticMesh();
						if (!Mesh || !Mesh->GetRenderData() || Mesh->GetRenderData()->LODResources.Num() == 0) { continue; }
						int32 Count = 1;
						if (UInstancedStaticMeshComponent* I = Cast<UInstancedStaticMeshComponent>(S)) { Count = I->GetInstanceCount(); ISMInst += Count; }
						const double T = (double)Mesh->GetRenderData()->LODResources[0].GetNumTriangles() * Count;
						Tris += T; ++SM;
						MeshTris.FindOrAdd(Mesh->GetPathName()) += T;
					}
					else if (P->IsA<USkeletalMeshComponent>()) { ++SK; }
				}
			}
			int32 Widgets = 0, Blurs = 0;
			for (TObjectIterator<UUserWidget> It; It; ++It)
			{
				if (IsValid(*It) && It->GetWorld() == World && It->IsInViewport()) { ++Widgets; }
			}
			for (TObjectIterator<UBackgroundBlur> It; It; ++It)
			{
				if (IsValid(*It) && It->GetWorld() == World && It->GetCachedWidget().IsValid() && It->GetBlurStrength() > 0) { ++Blurs; }
			}
			MeshTris.ValueSort([](double A, double B) { return A > B; });
			TArray<TSharedPtr<FJsonValue>> TopJ;
			for (const TPair<FString, double>& P : MeshTris)
			{
				if (TopJ.Num() >= Top) { break; }
				TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
				J->SetStringField(TEXT("mesh"), P.Key);
				J->SetNumberField(TEXT("trianglesK"), R1(P.Value / 1000.0));
				TopJ.Add(MakeShared<FJsonValueObject>(J));
			}
			TSharedRef<FJsonObject> D = MakeShared<FJsonObject>();
			D->SetStringField(TEXT("world"), Where);
			D->SetStringField(TEXT("map"), World->GetMapName());
			D->SetNumberField(TEXT("actors"), Actors);
			D->SetNumberField(TEXT("visiblePrimitives"), Prims);
			D->SetNumberField(TEXT("hiddenPrimitives"), Hidden);
			D->SetNumberField(TEXT("shadowCastingPrimitives"), ShadowPrims);
			D->SetNumberField(TEXT("staticMeshComponents"), SM);
			D->SetNumberField(TEXT("instances"), ISMInst);
			D->SetNumberField(TEXT("staticTrianglesK"), R1(Tris / 1000.0));
			D->SetNumberField(TEXT("skeletalMeshes"), SK);
			D->SetNumberField(TEXT("uniqueMaterials"), Mats.Num());
			D->SetNumberField(TEXT("translucentMaterialSlots"), Translucent);
			D->SetNumberField(TEXT("niagaraActive"), Niagara);
			D->SetNumberField(TEXT("postProcessVolumes"), PPV);
			TSharedRef<FJsonObject> LJ = MakeShared<FJsonObject>();
			LJ->SetNumberField(TEXT("directional"), LDir); LJ->SetNumberField(TEXT("point"), LPoint);
			LJ->SetNumberField(TEXT("spot"), LSpot); LJ->SetNumberField(TEXT("other"), LOther);
			LJ->SetNumberField(TEXT("shadowCasting"), LShadow); LJ->SetNumberField(TEXT("movable"), LMovable);
			D->SetObjectField(TEXT("lights"), LJ);
			D->SetNumberField(TEXT("widgetsOnScreen"), Widgets);
			D->SetNumberField(TEXT("backgroundBlurs"), Blurs);
			D->SetArrayField(TEXT("heaviestMeshes"), TopJ);
			D->SetObjectField(TEXT("settings"), RenderSettings());
			TArray<FString> Hints;
			if (CVarInt(TEXT("r.DynamicGlobalIlluminationMethod"), 0) == 1) { Hints.Add(TEXT("Lumen GI is on: the most expensive feature on low/mid GPUs. For a small stylized scene or a menu, baked/ambient light or Lumen only on High+ scalability is much cheaper.")); }
			if (CVarInt(TEXT("r.ReflectionMethod"), 0) == 1) { Hints.Add(TEXT("Lumen reflections are on: rarely visible in a stylized game; screen space reflections or none saves GPU time.")); }
			if (CVarInt(TEXT("r.Shadow.Virtual.Enable"), 0) == 1) { Hints.Add(TEXT("Virtual shadow maps are on: costly with many movable lights or skeletal meshes.")); }
			if (LShadow > 2) { Hints.Add(FString::Printf(TEXT("%d shadow-casting lights: each one renders the scene again; keep 1-2 (sun + one key)."), LShadow)); }
			if (Blurs > 0) { Hints.Add(FString::Printf(TEXT("%d BackgroundBlur widget(s) on screen: each blurs the full frame every frame; prefer a pre-blurred image or a cheap tint."), Blurs)); }
			if (Translucent > 30) { Hints.Add(TEXT("Many translucent material slots: overdraw is expensive; use masked or opaque where possible.")); }
			if (Prims > 2000) { Hints.Add(TEXT("Over 2000 visible primitives: draw calls add up; merge or instance repeated props.")); }
			if (CVarInt(TEXT("t.MaxFPS"), 0) <= 0 && CVarInt(TEXT("r.VSync"), 0) == 0) { Hints.Add(TEXT("No frame cap and VSync off: the menu renders as fast as it can and keeps the GPU at 100%.")); }
			TArray<TSharedPtr<FJsonValue>> HJ;
			for (const FString& H : Hints) { HJ.Add(MakeShared<FJsonValueString>(H)); }
			D->SetArrayField(TEXT("hints"), HJ);
			return FEWResult::Ok(D, FString::Printf(TEXT("%s: %d actors, %d visible primitives, %.0fK static triangles, %d lights (%d shadowed)."), *Where, Actors, Prims, Tris / 1000.0, LDir + LPoint + LSpot + LOther, LShadow));
		});

	R.RegisterAsync(TEXT("perf_gpu_profile"),
		TEXT("Profile one rendered frame on the GPU (the engine's ProfileGPU, without its window) and return the costliest passes in milliseconds, e.g. Lumen, shadow depths, base pass, translucency, post processing. Works best in Play In Editor."),
		TEXT(R"({"type":"object","properties":{
			"top":{"type":"integer","minimum":1,"maximum":100,"description":"How many passes to return, default 20."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			FString Where;
			UWorld* World = TargetWorld(Where);
			if (!World || !GEngine) { Done(FEWResult::Error(TEXT("NO_LEVEL"), TEXT("No PIE world and no editor level."))); return; }
			const int32 Top = FMath::Clamp((int32)EW::Num(Args, TEXT("top"), 20), 1, 100);
			IConsoleVariable* ShowUI = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ProfileGPU.ShowUI"));
			const int32 OldShow = ShowUI ? ShowUI->GetInt() : 1;
			if (ShowUI) { ShowUI->Set(0, ECVF_SetByCode); }
			UEditorPerformanceSettings* Perf = GetMutableDefault<UEditorPerformanceSettings>();
			const bool bThrottle = Perf->bThrottleCPUWhenNotForeground;
			Perf->bThrottleCPUWhenNotForeground = false;
			const uint64 Seq = EW::LogSeq();
			GEngine->Exec(World, TEXT("ProfileGPU"));
			TSharedRef<double> Start = MakeShared<double>(FPlatformTime::Seconds());
			auto Parse = [Seq, Top, Where, Done, ShowUI, OldShow, Perf, bThrottle](bool bTimeout)
			{
				if (ShowUI) { ShowUI->Set(OldShow, ECVF_SetByCode); }
				Perf->bThrottleCPUWhenNotForeground = bThrottle;
				const TArray<FString> Lines = EW::LogLinesSince(Seq);
				struct FPass { FString Name; double Ms; double Incl; int32 Depth; };
				TArray<FPass> Passes;
				double Total = 0;
				bool bGraphics = false;
				auto MsOf = [](const FString& Col) -> double
				{
					// "... │ 0.181 ms " -> 0.181 (last number before "ms")
					int32 P = Col.Find(TEXT(" ms"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
					if (P == INDEX_NONE) { return -1; }
					int32 B = P - 1;
					while (B >= 0 && (FChar::IsDigit(Col[B]) || Col[B] == TEXT('.'))) { --B; }
					return FCString::Atod(*Col.Mid(B + 1, P - B - 1));
				};
				for (const FString& L : Lines)
				{
					if (L.Contains(TEXT("Graphics pipeline"))) { bGraphics = true; continue; }
					if (L.Contains(TEXT("Compute pipeline")) || L.Contains(TEXT("Copy pipeline"))) { bGraphics = false; continue; }
					if (!bGraphics) { continue; }
					const int32 FT = L.Find(TEXT("Frame Time"));
					if (FT != INDEX_NONE)
					{
						int32 C = L.Find(TEXT(":"), ESearchCase::CaseSensitive, ESearchDir::FromStart, FT);
						if (C != INDEX_NONE) { Total = FCString::Atod(*L.Mid(C + 1).TrimStart()); }
						continue;
					}
					// UE 5.6+ table row: "┃ excl cols ┃ incl cols ┃ <indent>Name"
					TArray<FString> Parts;
					L.ParseIntoArray(Parts, TEXT("\u2503"), false);  // heavy vertical box line
					if (Parts.Num() < 4) { continue; }
					const double Ex = MsOf(Parts[1]), In = MsOf(Parts[2]);
					if (Ex < 0 || In < 0) { continue; }
					FString Name = Parts[3];
					int32 Depth = 0;
					while (Depth < Name.Len() && Name[Depth] == TEXT(' ')) { ++Depth; }
					Name.TrimStartAndEndInline();
					if (Name.IsEmpty() || Name == TEXT("<root>")) { continue; }
					Passes.Add({ Name, Ex, In, Depth });
				}
				if (Passes.Num() == 0)
				{
					FEWResult Res = FEWResult::Error(TEXT("NO_PROFILE"), bTimeout ? TEXT("ProfileGPU wrote no pass rows in time (is the viewport rendering? try in PIE).") : TEXT("Could not read the ProfileGPU rows."));
					Res.Data = MakeShared<FJsonObject>();
					TArray<TSharedPtr<FJsonValue>> Raw;
					for (int32 i = 0; i < FMath::Min(40, Lines.Num()); ++i) { Raw.Add(MakeShared<FJsonValueString>(Lines[i])); }
					Res.Data->SetArrayField(TEXT("logLines"), Raw);
					Done(Res);
					return;
				}
				Passes.Sort([](const FPass& A, const FPass& B) { return A.Ms > B.Ms; });  // exclusive time: where the GPU really spends it
				TArray<TSharedPtr<FJsonValue>> PJ;
				for (int32 i = 0; i < FMath::Min(Top, Passes.Num()); ++i)
				{
					TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
					J->SetStringField(TEXT("pass"), Passes[i].Name);
					J->SetNumberField(TEXT("ms"), R2(Passes[i].Ms));
					J->SetNumberField(TEXT("inclusiveMs"), R2(Passes[i].Incl));
					J->SetNumberField(TEXT("indent"), Passes[i].Depth);
					PJ.Add(MakeShared<FJsonValueObject>(J));
				}
				TSharedRef<FJsonObject> D = MakeShared<FJsonObject>();
				D->SetStringField(TEXT("world"), Where);
				D->SetNumberField(TEXT("totalGpuMs"), R2(Total));
				D->SetNumberField(TEXT("passesParsed"), Passes.Num());
				D->SetArrayField(TEXT("top"), PJ);
				Done(FEWResult::Ok(D, FString::Printf(TEXT("GPU frame %.2f ms; costliest: %s %.2f ms."), Total, *Passes[0].Name, Passes[0].Ms)));
			};
			TSharedRef<uint64> LastCount = MakeShared<uint64>(0);
			TSharedRef<double> LastChange = MakeShared<double>(FPlatformTime::Seconds());
			EW::PollUntil(8.0,
				[Seq, Parse, LastCount, LastChange]() -> bool
				{
					const uint64 Now = EW::LogSeq();
					if (Now != *LastCount) { *LastCount = Now; *LastChange = FPlatformTime::Seconds(); return false; }
					// Rows have arrived and the log has been quiet for 0.5 s: the dump is complete.
					if (Now > Seq + 5 && FPlatformTime::Seconds() - *LastChange > 0.5) { Parse(false); return true; }
					return false;
				},
				[Parse]() { Parse(true); });
		});
}
