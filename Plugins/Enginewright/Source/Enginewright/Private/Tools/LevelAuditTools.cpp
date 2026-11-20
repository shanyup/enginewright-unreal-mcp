// Level audit: hidden actors, visible static-mesh actors that do nothing, placeholder / kit meshes left in a map.
#include "EnginewrightTool.h"
#include "EWCommon.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_Comment.h"
#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/Brush.h"
#include "Engine/LevelStreaming.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/WorldSettings.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UnrealType.h"

namespace
{
	FEWResult Bad(const FString& Code, const FString& Msg, const TArray<FString>& Sug = {}) { return FEWResult::Error(Code, Msg, Sug); }

	/** USceneComponent::bHiddenInGame, read through reflection so a header change in the engine cannot break the build. */
	bool IsHiddenInGame(const USceneComponent* C)
	{
		static const FBoolProperty* P = CastField<FBoolProperty>(USceneComponent::StaticClass()->FindPropertyByName(TEXT("bHiddenInGame")));
		return P && P->GetPropertyValue_InContainer(C);
	}

	/** Components that only draw or place something. Anything else (collision shape, audio, script component, light) counts as gameplay. */
	bool IsPassive(const UActorComponent* C)
	{
		if (C->GetClass() == USceneComponent::StaticClass()) { return true; }
		return C->IsA<UStaticMeshComponent>() || C->IsA<UBillboardComponent>() || C->IsA<UArrowComponent>();
	}

	/** Nodes that do something: enabled, not a comment. A new Actor Blueprint holds three disabled event nodes that do not count. */
	int32 ActiveNodes(const UEdGraph* G)
	{
		int32 N = 0;
		if (!G) { return 0; }
		for (const UEdGraphNode* Node : G->Nodes)
		{
			if (Node && !Node->IsA<UEdGraphNode_Comment>() && Node->IsNodeEnabled()) { ++N; }
		}
		return N;
	}

	/** True when the Blueprint holds logic: an enabled node in an event graph, or a user function graph (the construction script does not count). */
	bool BlueprintHasLogic(const UBlueprint* BP)
	{
		for (const UEdGraph* G : BP->UbergraphPages) { if (ActiveNodes(G) > 0) { return true; } }
		for (const UEdGraph* G : BP->FunctionGraphs)
		{
			if (G && G->GetFName() != TEXT("UserConstructionScript") && G->Nodes.Num() > 0) { return true; }
		}
		return false;
	}

	/** Own code behind the actor: a C++ class outside the engine, or a Blueprint in the class chain with logic. */
	bool HasOwnCode(const UClass* Class, FString& OutWhy)
	{
		for (const UClass* C = Class; C; C = C->GetSuperClass())
		{
			if (const UBlueprint* BP = Cast<UBlueprint>(C->ClassGeneratedBy))
			{
				if (BlueprintHasLogic(BP)) { OutWhy = BP->GetName() + TEXT(" has graph logic"); return true; }
				continue;
			}
			const FString Pkg = C->GetOutermost()->GetName();
			if (!Pkg.StartsWith(TEXT("/Script/Engine")) && !Pkg.StartsWith(TEXT("/Script/CoreUObject"))) { OutWhy = C->GetName() + TEXT(" is a ") + Pkg + TEXT(" class"); return true; }
			break;
		}
		return false;
	}

	UWorld* ResolveWorld(const FString& MapArg, FString& OutName, FString& OutError, FEWResult& OutErr, bool& bOutLoaded)
	{
		bOutLoaded = false;
		UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
		if (MapArg.IsEmpty())
		{
			if (!EditorWorld) { OutErr = Bad(TEXT("NO_LEVEL"), TEXT("No editor level is open. Pass map.")); return nullptr; }
			OutName = EditorWorld->GetOutermost()->GetName();
			return EditorWorld;
		}
		FString Pkg = MapArg;
		const int32 Dot = Pkg.Find(TEXT("."));
		if (Dot != INDEX_NONE) { Pkg = Pkg.Left(Dot); }
		if (!FPackageName::IsValidLongPackageName(Pkg, false))
		{
			OutErr = Bad(TEXT("INVALID_PATH"), FString::Printf(TEXT("'%s' is not a map path like /Game/Maps/MAP_Test."), *MapArg));
			return nullptr;
		}
		OutName = Pkg;
		if (EditorWorld && EditorWorld->GetOutermost()->GetName() == Pkg) { return EditorWorld; }
		if (!FPackageName::DoesPackageExist(Pkg))
		{
			TArray<FString> Maps;
			for (const FString& M : EW::FindAssets(TEXT("/Game"), UWorld::StaticClass())) { Maps.Add(M); }
			OutErr = Bad(TEXT("MAP_NOT_FOUND"), FString::Printf(TEXT("No map '%s'."), *Pkg), EW::Suggest(Pkg, Maps));
			return nullptr;
		}
		UPackage* Package = FindPackage(nullptr, *Pkg);
		bOutLoaded = Package == nullptr;
		if (!Package) { Package = LoadPackage(nullptr, *Pkg, LOAD_None); }
		UWorld* World = Package ? UWorld::FindWorldInPackage(Package) : nullptr;
		if (!World) { OutErr = Bad(TEXT("MAP_NOT_FOUND"), FString::Printf(TEXT("'%s' is not a map (no world in the package)."), *Pkg)); return nullptr; }
		return World;
	}

	struct FFinding
	{
		FString Label, Path, Class, Mesh, Reason;
	};

	TSharedRef<FJsonValueObject> FindingJson(const FFinding& F)
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		J->SetStringField(TEXT("label"), F.Label);
		J->SetStringField(TEXT("path"), F.Path);
		J->SetStringField(TEXT("class"), F.Class);
		if (!F.Mesh.IsEmpty()) { J->SetStringField(TEXT("mesh"), F.Mesh); }
		J->SetStringField(TEXT("reason"), F.Reason);
		return MakeShared<FJsonValueObject>(J);
	}

	TArray<TSharedPtr<FJsonValue>> ListJson(const TArray<FFinding>& L, int32 Max)
	{
		TArray<TSharedPtr<FJsonValue>> Out;
		for (int32 i = 0; i < L.Num() && i < Max; ++i) { Out.Add(FindingJson(L[i])); }
		return Out;
	}
}

void RegisterLevelAuditTools(FEWRegistry& R)
{
	R.Register(TEXT("level_audit"),
		TEXT("Audit a map (or the open level) for leftovers: hidden actors (hidden in game, or every mesh component hidden/invisible), visible static-mesh actors that do nothing (only passive components such as the static mesh itself, no collision shape, audio, script component, and no C++ or Blueprint logic), and actors whose static mesh matches placeholder / kit name patterns. ")
		TEXT("Read-only: it counts and lists actor paths and never changes the map. A map that is not open is loaded into memory only (the editor level does not change); streaming levels that are not loaded are not scanned (the reply says so). ")
		TEXT("Filter with class (subclasses match) and tag (actor tag). meshPatterns are wildcards matched against the mesh path and name, case-insensitive; the default finds engine basic shapes and names with 'placeholder'; pass [] to skip that check. Search words: level cleanup, hidden actors, placeholder meshes, kit pieces, dead decoration, map audit."),
		TEXT(R"({"type":"object","properties":{
			"map":{"type":"string","description":"Map asset path, e.g. /Game/Maps/MAP_Test. Omit to audit the open editor level."},
			"class":{"type":"string","description":"Only actors of this class (short name, /Script path or Blueprint path; subclasses match)."},
			"tag":{"type":"string","description":"Only actors with this actor tag."},
			"meshPatterns":{"type":"array","items":{"type":"string"},"description":"Wildcards for placeholder / kit meshes, e.g. [\"SM_Kit_*\",\"*Placeholder*\",\"/Engine/BasicShapes/*\"]. Default: engine basic shapes and *placeholder*. [] turns the check off."},
			"maxList":{"type":"integer","description":"Most actors listed per group (default 200); counts are always complete."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			const int32 MaxList = FMath::Max(1, (int32)EW::Num(Args, TEXT("maxList"), 200));
			UClass* Filter = AActor::StaticClass();
			const FString ClassArg = EW::Str(Args, TEXT("class")).TrimStartAndEnd();
			if (!ClassArg.IsEmpty())
			{
				FString CErr;
				TArray<FString> Sug;
				Filter = EW::FindClass(ClassArg, CErr, &Sug);
				if (!Filter) { return Bad(TEXT("UNKNOWN_CLASS"), CErr, Sug); }
				if (!Filter->IsChildOf(AActor::StaticClass())) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("'%s' is not an Actor class."), *Filter->GetPathName())); }
			}
			const FString TagArg = EW::Str(Args, TEXT("tag")).TrimStartAndEnd();
			TArray<FString> Patterns;
			if (Args->HasField(TEXT("meshPatterns"))) { Patterns = EW::StrList(Args, TEXT("meshPatterns")); }
			else { Patterns = { TEXT("/Engine/BasicShapes/*"), TEXT("*placeholder*") }; }

			FString MapName, Unused;
			FEWResult Err;
			bool bLoadedHere = false;
			UWorld* World = ResolveWorld(EW::Str(Args, TEXT("map")).TrimStartAndEnd(), MapName, Unused, Err, bLoadedHere);
			if (!World) { return Err; }

			TArray<ULevel*> Levels;
			for (ULevel* L : World->GetLevels()) { if (L) { Levels.AddUnique(L); } }
			if (World->PersistentLevel) { Levels.AddUnique(World->PersistentLevel); }
			int32 UnloadedStreaming = 0;
			for (const ULevelStreaming* S : World->GetStreamingLevels()) { if (S && !S->GetLoadedLevel()) { ++UnloadedStreaming; } }

			TArray<FFinding> Hidden, Idle, Placeholder;
			TSet<const AActor*> Flagged;
			int32 Scanned = 0, MeshActors = 0;
			for (const ULevel* L : Levels)
			{
				for (AActor* A : L->Actors)
				{
					if (!IsValid(A) || A->IsTemplate() || A->IsA<AWorldSettings>() || A->GetClass() == ABrush::StaticClass()) { continue; }
					if (!A->IsA(Filter)) { continue; }
					if (!TagArg.IsEmpty() && !A->ActorHasTag(FName(*TagArg))) { continue; }
					++Scanned;

					TArray<UMeshComponent*> Meshes;
					A->GetComponents<UMeshComponent>(Meshes);
					TArray<UStaticMeshComponent*> StaticMeshes;
					A->GetComponents<UStaticMeshComponent>(StaticMeshes);
					const UStaticMesh* FirstMesh = nullptr;
					for (const UStaticMeshComponent* SM : StaticMeshes) { if (SM->GetStaticMesh()) { FirstMesh = SM->GetStaticMesh(); break; } }
					if (Meshes.Num() > 0 || FirstMesh) { ++MeshActors; }

					FFinding F;
					F.Label = A->GetActorLabel();
					F.Path = A->GetPathName();
					F.Class = A->GetClass()->GetPathName();
					F.Mesh = FirstMesh ? FirstMesh->GetPathName() : FString();

					// 1) hidden
					FString HiddenWhy;
					if (A->IsHidden()) { HiddenWhy = TEXT("actor is hidden in game"); }
					else if (A->GetRootComponent() && IsHiddenInGame(A->GetRootComponent()) && Meshes.Num() > 0) { HiddenWhy = TEXT("root component is hidden in game"); }
					else if (Meshes.Num() > 0)
					{
						bool bAnyShown = false;
						for (const UMeshComponent* M : Meshes) { if (M->IsVisible() && !IsHiddenInGame(M)) { bAnyShown = true; break; } }
						if (!bAnyShown) { HiddenWhy = TEXT("every mesh component is hidden or invisible"); }
					}
					if (!HiddenWhy.IsEmpty())
					{
						FFinding H = F;
						H.Reason = HiddenWhy;
						Hidden.Add(H);
						Flagged.Add(A);
						continue; // a hidden actor is not also reported as idle or placeholder
					}

					// 2) visible static mesh that does nothing
					if (FirstMesh)
					{
						FString Why;
						bool bGameplay = HasOwnCode(A->GetClass(), Why);
						if (!bGameplay)
						{
							TInlineComponentArray<UActorComponent*> All;
							A->GetComponents(All);
							for (const UActorComponent* C : All)
							{
								if (!IsPassive(C)) { bGameplay = true; break; }
							}
						}
						if (!bGameplay)
						{
							FFinding I = F;
							I.Reason = TEXT("visible static mesh with no gameplay component (only passive components, no own code)");
							Idle.Add(I);
							Flagged.Add(A);
						}
					}

					// 3) placeholder / kit mesh
					if (FirstMesh && Patterns.Num() > 0)
					{
						const FString MeshPath = FirstMesh->GetPathName(), MeshName = FirstMesh->GetName();
						for (const FString& P : Patterns)
						{
							if (MeshPath.MatchesWildcard(P, ESearchCase::IgnoreCase) || MeshName.MatchesWildcard(P, ESearchCase::IgnoreCase))
							{
								FFinding PH = F;
								PH.Reason = FString::Printf(TEXT("mesh matches pattern '%s'"), *P);
								Placeholder.Add(PH);
								Flagged.Add(A);
								break;
							}
						}
					}
				}
			}

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("map"), MapName);
			Data->SetBoolField(TEXT("openLevel"), GEditor && GEditor->GetEditorWorldContext().World() == World);
			Data->SetNumberField(TEXT("levelsScanned"), Levels.Num());
			Data->SetNumberField(TEXT("actorsScanned"), Scanned);
			Data->SetNumberField(TEXT("meshActors"), MeshActors);
			Data->SetNumberField(TEXT("hiddenCount"), Hidden.Num());
			Data->SetNumberField(TEXT("idleMeshCount"), Idle.Num());
			Data->SetNumberField(TEXT("placeholderCount"), Placeholder.Num());
			Data->SetNumberField(TEXT("findingCount"), Flagged.Num());
			Data->SetArrayField(TEXT("hidden"), ListJson(Hidden, MaxList));
			Data->SetArrayField(TEXT("idleMeshes"), ListJson(Idle, MaxList));
			Data->SetArrayField(TEXT("placeholders"), ListJson(Placeholder, MaxList));
			TArray<TSharedPtr<FJsonValue>> Pats;
			for (const FString& P : Patterns) { Pats.Add(MakeShared<FJsonValueString>(P)); }
			Data->SetArrayField(TEXT("meshPatterns"), Pats);
			Data->SetStringField(TEXT("verdict"), Flagged.Num() > 0 ? TEXT("findings") : TEXT("clean"));
			Data->SetStringField(TEXT("summary"), FString::Printf(TEXT("%d actor(s) flagged of %d scanned: %d hidden, %d idle static mesh, %d placeholder mesh."), Flagged.Num(), Scanned, Hidden.Num(), Idle.Num(), Placeholder.Num()));
			FEWResult Out = FEWResult::Ok(Data);
			if (UnloadedStreaming > 0) { Out.Warnings.Add(FString::Printf(TEXT("%d streaming level(s) are not loaded and were not scanned."), UnloadedStreaming)); }
			if (bLoadedHere) { Out.Warnings.Add(TEXT("The map was loaded into memory for this audit only; the open level did not change.")); }
			return Out;
		});
}
