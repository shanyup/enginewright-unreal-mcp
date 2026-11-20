// Cinematic render tools: Movie Render Queue from an agent. mrq_render starts a render of a Level Sequence
// (PIE executor) with a resolution / anti-aliasing / format preset, keeps the editor unthrottled until the
// render ends, and optionally encodes an MP4 with ffmpeg. mrq_status reports progress of the last render.
#include "EnginewrightTool.h"
#include "EWCommon.h"

#include "Editor.h"
#include "Editor/EditorPerformanceSettings.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Containers/Ticker.h"
#include "LevelSequence.h"
#include "MovieScene.h"

#include "MoviePipelineQueueSubsystem.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelinePrimaryConfig.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"

namespace
{
	/** State of the last render started by mrq_render (one at a time, like the MRQ subsystem itself). */
	struct FEWRender
	{
		bool bActive = false;
		bool bFinished = false;
		bool bSuccess = false;
		bool bOldThrottle = true;
		bool bWantMp4 = false;
		FString OutDir, Sequence, Map, Format, Mp4, Mp4Error;
		FIntPoint Resolution = FIntPoint::ZeroValue;
		int32 Fps = 60, ExpectedFrames = 0, Aa = 1;
		double StartSeconds = 0, EndSeconds = 0;
		TWeakObjectPtr<UMoviePipelineExecutorBase> Executor;
		FTSTicker::FDelegateHandle Ticker;
	};
	FEWRender GRender;

	int32 CountFrames(const FString& Dir)
	{
		TArray<FString> Files;
		IFileManager::Get().FindFiles(Files, *(Dir / TEXT("frame.*.png")), true, false);
		return Files.Num();
	}

	FString FindFfmpeg()
	{
		FString Path;
		if (FPlatformMisc::GetEnvironmentVariable(TEXT("PATH")).Len() > 0)
		{
			TArray<FString> Dirs;
			FPlatformMisc::GetEnvironmentVariable(TEXT("PATH")).ParseIntoArray(Dirs, TEXT(";"), true);
			for (const FString& D : Dirs)
			{
				const FString C = FPaths::Combine(D, TEXT("ffmpeg.exe"));
				if (FPaths::FileExists(C)) { return C; }
			}
		}
		// winget installs ffmpeg under the user's package folder without always updating the editor's PATH.
		const FString Local = FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA")) / TEXT("Microsoft/WinGet/Packages");
		TArray<FString> Found;
		IFileManager::Get().FindFilesRecursive(Found, *Local, TEXT("ffmpeg.exe"), true, false);
		return Found.Num() ? Found[0] : FString();
	}

	void EncodeMp4()
	{
		const FString Ff = FindFfmpeg();
		if (Ff.IsEmpty()) { GRender.Mp4Error = TEXT("ffmpeg not found (PATH or WinGet packages). Frames are kept as PNG."); return; }
		GRender.Mp4 = GRender.OutDir + TEXT(".mp4");
		const FString Args = FString::Printf(TEXT("-y -framerate %d -start_number 0 -i \"%s/frame.%%04d.png\" -c:v libx264 -preset medium -crf 16 -pix_fmt yuv420p -movflags +faststart \"%s\""),
			GRender.Fps, *GRender.OutDir, *GRender.Mp4);
		int32 Code = -1; FString Out, Err;
		FPlatformProcess::ExecProcess(*Ff, *Args, &Code, &Out, &Err);
		if (Code != 0 || !FPaths::FileExists(GRender.Mp4))
		{
			GRender.Mp4Error = FString::Printf(TEXT("ffmpeg exit %d: %s"), Code, *Err.Right(300));
			GRender.Mp4.Empty();
		}
	}

	void FinishRender(bool bSuccess)
	{
		if (!GRender.bActive) { return; }
		GRender.bActive = false;
		GRender.bFinished = true;
		GRender.bSuccess = bSuccess;
		GRender.EndSeconds = FPlatformTime::Seconds();
		GetMutableDefault<UEditorPerformanceSettings>()->bThrottleCPUWhenNotForeground = GRender.bOldThrottle;
		if (GRender.Ticker.IsValid()) { FTSTicker::GetCoreTicker().RemoveTicker(GRender.Ticker); GRender.Ticker.Reset(); }
		if (bSuccess && GRender.bWantMp4) { EncodeMp4(); }
	}

	TSharedRef<FJsonObject> StatusJson()
	{
		TSharedRef<FJsonObject> D = MakeShared<FJsonObject>();
		const int32 Frames = GRender.OutDir.IsEmpty() ? 0 : CountFrames(GRender.OutDir);
		const double Now = GRender.bFinished ? GRender.EndSeconds : FPlatformTime::Seconds();
		const double Elapsed = GRender.StartSeconds > 0 ? Now - GRender.StartSeconds : 0;
		D->SetStringField(TEXT("state"), GRender.bActive ? TEXT("rendering") : (GRender.bFinished ? (GRender.bSuccess ? TEXT("done") : TEXT("failed")) : TEXT("idle")));
		D->SetStringField(TEXT("sequence"), GRender.Sequence);
		D->SetStringField(TEXT("map"), GRender.Map);
		D->SetStringField(TEXT("outputDir"), GRender.OutDir);
		D->SetNumberField(TEXT("framesWritten"), Frames);
		D->SetNumberField(TEXT("framesExpected"), GRender.ExpectedFrames);
		D->SetNumberField(TEXT("progress"), GRender.ExpectedFrames > 0 ? FMath::Min(1.0, (double)Frames / GRender.ExpectedFrames) : 0.0);
		D->SetNumberField(TEXT("elapsedSeconds"), FMath::RoundToDouble(Elapsed));
		D->SetNumberField(TEXT("secondsPerFrame"), Frames > 0 ? FMath::RoundToDouble(Elapsed / Frames * 100) / 100 : 0.0);
		if (!GRender.Mp4.IsEmpty()) { D->SetStringField(TEXT("mp4"), GRender.Mp4); }
		if (!GRender.Mp4Error.IsEmpty()) { D->SetStringField(TEXT("mp4Error"), GRender.Mp4Error); }
		return D;
	}

	FIntPoint ResolutionFor(const FString& Format, const TSharedPtr<FJsonObject>& Args)
	{
		const int32 W = (int32)EW::Num(Args, TEXT("width"), 0), H = (int32)EW::Num(Args, TEXT("height"), 0);
		if (W > 0 && H > 0) { return FIntPoint(W, H); }
		if (Format == TEXT("vertical")) { return FIntPoint(1080, 1920); }
		if (Format == TEXT("square")) { return FIntPoint(1080, 1080); }
		if (Format == TEXT("4k")) { return FIntPoint(3840, 2160); }
		return FIntPoint(1920, 1080);
	}
}

void RegisterCinematicTools(FEWRegistry& R)
{
	R.RegisterAsync(TEXT("mrq_render"),
		TEXT("Render a Level Sequence with Movie Render Queue (PIE executor) to a PNG frame sequence, optionally encoded to MP4 (H.264, social-media ready) with ffmpeg. Presets: format horizontal 1920x1080, vertical 1080x1920 (TikTok/Reels/Shorts), square 1080x1080, 4k; or width+height. aa = temporal samples (1 fast preview, 8 final). The editor is kept unthrottled until the render ends, so a background editor renders at full speed. Returns at once (poll mrq_status) unless wait is true. Needs the MovieRenderPipeline plugin."),
		TEXT(R"({"type":"object","properties":{
			"sequence":{"type":"string","description":"Level Sequence asset path, e.g. /Game/Marketing/LS_Shot."},
			"map":{"type":"string","description":"Map to render in; default the open editor level."},
			"format":{"type":"string","enum":["horizontal","vertical","square","4k"],"description":"Resolution preset, default horizontal."},
			"width":{"type":"integer","description":"Custom width in px (use with height); overrides the format preset."},"height":{"type":"integer","description":"Custom height in px (use with width); overrides the format preset."},
			"aa":{"type":"integer","description":"Temporal samples per frame, 1-32, default 8."},
			"warmup":{"type":"integer","description":"Engine warm-up frames before the first output frame, default 32."},
			"outputDir":{"type":"string","description":"Absolute folder; default <Project>/Saved/Renders/<sequence>_<format>_<time>."},
			"mp4":{"type":"boolean","description":"Encode <outputDir>.mp4 with ffmpeg when done, default true."},
			"wait":{"type":"boolean","description":"Wait for the render to finish (up to timeout), default false."},
			"timeout":{"type":"number","description":"Seconds to wait when wait is true, default 600, max 1800."}},
			"required":["sequence"]})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			if (!GEditor) { Done(FEWResult::Error(TEXT("NO_EDITOR"), TEXT("No editor."))); return; }
			if (EW::IsPlaying()) { Done(FEWResult::Error(TEXT("PIE_RUNNING"), TEXT("Stop Play In Editor first (pie_stop); MRQ starts its own PIE session."))); return; }
			UMoviePipelineQueueSubsystem* Q = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>();
			if (!Q) { Done(FEWResult::Error(TEXT("NO_MRQ"), TEXT("Movie Render Queue subsystem missing; enable the MovieRenderPipeline plugin."))); return; }
			if (Q->IsRendering() || GRender.bActive) { Done(FEWResult::Error(TEXT("BUSY"), TEXT("A render is already running; see mrq_status."))); return; }

			// A given map that does not exist used to start a render that could never load it (tur 0.5). Checked before the
			// sequence so the reply names the first problem; the open editor level (default) may be unsaved, so it is not checked.
			const FString GivenMap = EW::Str(Args, TEXT("map"));
			if (!GivenMap.IsEmpty() && !FPackageName::DoesPackageExist(FPackageName::ObjectPathToPackageName(GivenMap)))
			{
				Done(FEWResult::Error(TEXT("MAP_NOT_FOUND"), FString::Printf(TEXT("No map '%s'. Give a package path like /Game/Maps/MyMap."), *GivenMap))); return;
			}

			FString Err;
			const FString SeqPath = EW::Str(Args, TEXT("sequence"));
			ULevelSequence* Seq = EW::LoadAs<ULevelSequence>(SeqPath, Err);
			if (!Seq) { Done(FEWResult::Error(TEXT("NOT_FOUND"), Err)); return; }
			FString MapPath = EW::Str(Args, TEXT("map"));
			if (MapPath.IsEmpty())
			{
				UWorld* W = GEditor->GetEditorWorldContext().World();
				MapPath = W ? W->GetOutermost()->GetName() : FString();
			}
			if (MapPath.IsEmpty()) { Done(FEWResult::Error(TEXT("NO_MAP"), TEXT("No map given and no editor level open."))); return; }

			FString Format = EW::Str(Args, TEXT("format"));
			if (Format.IsEmpty()) { Format = TEXT("horizontal"); }
			const FIntPoint Res = ResolutionFor(Format, Args);
			const int32 Aa = FMath::Clamp((int32)EW::Num(Args, TEXT("aa"), 8), 1, 32);
			const int32 Warm = FMath::Clamp((int32)EW::Num(Args, TEXT("warmup"), 32), 0, 512);
			FString Out = EW::Str(Args, TEXT("outputDir"));
			if (Out.IsEmpty())
			{
				Out = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()) / TEXT("Renders") /
					FString::Printf(TEXT("%s_%s_%s"), *Seq->GetName(), *Format, *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
			}
			IFileManager::Get().MakeDirectory(*Out, true);

			UMoviePipelineQueue* Queue = Q->GetQueue();
			Queue->DeleteAllJobs();
			UMoviePipelineExecutorJob* Job = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			Job->JobName = FString::Printf(TEXT("EW_%s"), *Seq->GetName());
			Job->SetSequence(FSoftObjectPath(Seq));
			Job->Map = FSoftObjectPath(MapPath + TEXT(".") + FPackageName::GetShortName(MapPath));
			UMoviePipelinePrimaryConfig* Cfg = Job->GetConfiguration();

			UMoviePipelineOutputSetting* O = Cast<UMoviePipelineOutputSetting>(Cfg->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
			O->OutputDirectory.Path = Out;
			O->FileNameFormat = TEXT("frame.{frame_number}");
			O->OutputResolution = Res;
			O->bUseCustomFrameRate = false;
			O->bOverrideExistingOutput = true;
			Cfg->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
			Cfg->FindOrAddSettingByClass(UMoviePipelineImageSequenceOutput_PNG::StaticClass());
			UMoviePipelineAntiAliasingSetting* AA = Cast<UMoviePipelineAntiAliasingSetting>(Cfg->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass()));
			AA->SpatialSampleCount = 1;
			AA->TemporalSampleCount = Aa;
			AA->bOverrideAntiAliasing = true;
			AA->AntiAliasingMethod = Aa > 1 ? EAntiAliasingMethod::AAM_None : EAntiAliasingMethod::AAM_TemporalAA;
			AA->RenderWarmUpCount = 32;
			AA->EngineWarmUpCount = Warm;
			UMoviePipelineGameOverrideSetting* G = Cast<UMoviePipelineGameOverrideSetting>(Cfg->FindOrAddSettingByClass(UMoviePipelineGameOverrideSetting::StaticClass()));
			G->bCinematicQualitySettings = true;
			G->TextureStreaming = EMoviePipelineTextureStreamingMethod::Disabled;

			// expected frame count from the sequence playback range at its display rate
			const UMovieScene* MS = Seq->GetMovieScene();
			const TRange<FFrameNumber> PR = MS->GetPlaybackRange();
			const FFrameRate Tick = MS->GetTickResolution(), Disp = MS->GetDisplayRate();
			const int32 Frames = FMath::Max(0, (int32)FFrameRate::TransformTime(FFrameTime(PR.Size<FFrameNumber>()), Tick, Disp).RoundToFrame().Value);

			GRender = FEWRender();
			GRender.bActive = true;
			GRender.OutDir = Out; GRender.Sequence = Seq->GetPathName(); GRender.Map = MapPath; GRender.Format = Format;
			GRender.Resolution = Res; GRender.Aa = Aa; GRender.ExpectedFrames = Frames;
			GRender.Fps = FMath::Max(1, FMath::RoundToInt(Disp.AsDecimal()));
			GRender.bWantMp4 = !Args->HasField(TEXT("mp4")) || Args->GetBoolField(TEXT("mp4"));
			GRender.StartSeconds = FPlatformTime::Seconds();
			UEditorPerformanceSettings* Perf = GetMutableDefault<UEditorPerformanceSettings>();
			GRender.bOldThrottle = Perf->bThrottleCPUWhenNotForeground;
			Perf->bThrottleCPUWhenNotForeground = false;

			UMoviePipelineExecutorBase* Ex = Q->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
			if (!Ex) { FinishRender(false); Done(FEWResult::Error(TEXT("START_FAILED"), TEXT("Movie Render Queue did not start (see log)."))); return; }
			GRender.Executor = Ex;
			Ex->OnExecutorFinished().AddLambda([](UMoviePipelineExecutorBase*, bool bOk) { FinishRender(bOk); });
			// safety net: if the finished delegate never fires (executor destroyed), stop when MRQ is idle
			GRender.Ticker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float) -> bool
			{
				if (!GRender.bActive) { return false; }
				UMoviePipelineQueueSubsystem* Qs = GEditor ? GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>() : nullptr;
				if (Qs && !Qs->IsRendering() && FPlatformTime::Seconds() - GRender.StartSeconds > 5.0)
				{
					FinishRender(GRender.ExpectedFrames > 0 && CountFrames(GRender.OutDir) >= GRender.ExpectedFrames);
					return false;
				}
				return true;
			}), 1.0f);

			const bool bWait = Args->HasField(TEXT("wait")) && Args->GetBoolField(TEXT("wait"));
			const FString Msg = FString::Printf(TEXT("Rendering %s (%dx%d, aa %d, %d frames) to %s."), *Seq->GetName(), Res.X, Res.Y, Aa, Frames, *Out);
			if (!bWait) { Done(FEWResult::Ok(StatusJson(), Msg + TEXT(" Poll mrq_status."))); return; }
			const float Timeout = FMath::Clamp((float)EW::Num(Args, TEXT("timeout"), 600), 5.f, 1800.f);
			EW::PollUntil(Timeout,
				[Done]() -> bool
				{
					if (GRender.bActive) { return false; }
					Done(GRender.bSuccess ? FEWResult::Ok(StatusJson(), TEXT("Render finished.")) : FEWResult::Error(TEXT("RENDER_FAILED"), TEXT("Render failed; see log_read MoviePipeline.")));
					return true;
				},
				[Done]() { Done(FEWResult::Ok(StatusJson(), TEXT("Still rendering after the wait timeout; poll mrq_status."))); });
		});

	R.Register(TEXT("mrq_status"),
		TEXT("Progress of the last mrq_render: state (rendering / done / failed / idle), frames written vs expected, seconds per frame, output folder and MP4 path."),
		TEXT(R"({"type":"object","properties":{}})"), false,
		[](const TSharedPtr<FJsonObject>&)
		{
			TSharedRef<FJsonObject> D = StatusJson();
			return FEWResult::Ok(D, FString::Printf(TEXT("%s, %d/%d frames."), *D->GetStringField(TEXT("state")),
				(int32)D->GetNumberField(TEXT("framesWritten")), GRender.ExpectedFrames));
		});
}
