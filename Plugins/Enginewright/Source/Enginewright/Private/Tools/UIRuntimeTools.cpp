// Runtime UI in Play In Editor: what a player's screen shows (UMG and CommonUI: activatable widget stacks,
// buttons with labels and positions) and clicking a button like a player. CommonUI is used through
// reflection, so projects without the CommonUI plugin still work.
#include "EnginewrightTool.h"
#include "EWCommon.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/HitResult.h"
#include "Slate/SceneViewport.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputAction.h"
#include "Misc/PackageName.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UObject/StructOnScope.h"
#include "WidgetBlueprint.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "Framework/Application/SlateUser.h"
#include "Layout/WidgetPath.h"
#include "UObject/UObjectIterator.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SViewport.h"
#include "Widgets/SWindow.h"

namespace
{
	FEWResult Bad(const FString& Code, const FString& Msg, const TArray<FString>& Sug = {}) { return FEWResult::Error(Code, Msg, Sug); }

	UClass* CommonClass(const TCHAR* Name) { return FindObject<UClass>(nullptr, *FString::Printf(TEXT("/Script/CommonUI.%s"), Name)); }

	bool CallBool(UObject* Obj, const TCHAR* Fn)
	{
		UFunction* F = Obj ? Obj->FindFunction(FName(Fn)) : nullptr;
		if (!F || F->ParmsSize > 64) { return false; }
		uint8 Buf[64] = {};
		Obj->ProcessEvent(F, Buf);
		const FBoolProperty* Ret = CastField<FBoolProperty>(F->GetReturnProperty());
		return Ret && Ret->GetPropertyValue_InContainer(Buf);
	}

	/** Call a UFunction whose first input parameter is an object or class; returns its object return value. */
	UObject* CallWithObject(UObject* Obj, const TCHAR* Fn, UObject* Arg, bool* bCalled = nullptr)
	{
		UFunction* F = Obj ? Obj->FindFunction(FName(Fn)) : nullptr;
		if (bCalled) { *bCalled = F != nullptr; }
		if (!F) { return nullptr; }
		FStructOnScope Parms(F);
		for (TFieldIterator<FProperty> It(F); It && It->HasAnyPropertyFlags(CPF_Parm); ++It)
		{
			if (It->HasAnyPropertyFlags(CPF_ReturnParm)) { continue; }
			if (FObjectPropertyBase* OP = CastField<FObjectPropertyBase>(*It)) { OP->SetObjectPropertyValue_InContainer(Parms.GetStructMemory(), Arg); }
			break;
		}
		Obj->ProcessEvent(F, Parms.GetStructMemory());
		const FObjectPropertyBase* Ret = CastField<FObjectPropertyBase>(F->GetReturnProperty());
		return Ret ? Ret->GetObjectPropertyValue_InContainer(Parms.GetStructMemory()) : nullptr;
	}

	/** The integer value of an enum (or byte) property. */
	int64 EnumValue(const FProperty* P, const void* Container)
	{
		if (const FEnumProperty* EP = CastField<FEnumProperty>(P)) { return EP->GetUnderlyingProperty()->GetSignedIntPropertyValue(EP->ContainerPtrToValuePtr<void>(Container)); }
		if (const FNumericProperty* NP = CastField<FNumericProperty>(P)) { return NP->GetSignedIntPropertyValue(NP->ContainerPtrToValuePtr<void>(Container)); }
		return -1;
	}

	/** CommonInput's per-player subsystem (null without the CommonUI plugin). */
	UObject* CommonInputSubsystem(APlayerController* PC)
	{
		UClass* C = FindObject<UClass>(nullptr, TEXT("/Script/CommonInput.CommonInputSubsystem"));
		ULocalPlayer* LP = PC ? PC->GetLocalPlayer() : nullptr;
		return (C && LP) ? (UObject*)LP->GetSubsystemBase(TSubclassOf<ULocalPlayerSubsystem>(C)) : nullptr;
	}

	UEnum* CommonInputTypeEnum() { return FindObject<UEnum>(nullptr, TEXT("/Script/CommonInput.ECommonInputType")); }

	/** Current CommonInput input type name (MouseAndKeyboard, Gamepad, Touch), empty without CommonInput. */
	FString InputTypeName(APlayerController* PC)
	{
		UObject* Sub = CommonInputSubsystem(PC);
		UFunction* F = Sub ? Sub->FindFunction(TEXT("GetCurrentInputType")) : nullptr;
		UEnum* E = CommonInputTypeEnum();
		if (!F || !E) { return FString(); }
		FStructOnScope Parms(F);
		Sub->ProcessEvent(F, Parms.GetStructMemory());
		return E->GetNameStringByValue(EnumValue(F->GetReturnProperty(), Parms.GetStructMemory()));
	}

	/** Activatable widget containers (stacks / queues) in this player's world. */
	TArray<UWidget*> Containers(APlayerController* PC)
	{
		TArray<UWidget*> Out;
		UClass* Container = CommonClass(TEXT("CommonActivatableWidgetContainerBase"));
		if (!Container) { return Out; }
		for (TObjectIterator<UWidget> It; It; ++It)
		{
			UWidget* C = *It;
			if (!IsValid(C) || !C->IsA(Container) || C->GetWorld() != PC->GetWorld() || !C->GetCachedWidget().IsValid()) { continue; }
			UUserWidget* Owner = C->GetTypedOuter<UUserWidget>();
			if (Owner && Owner->GetOwningPlayer() && Owner->GetOwningPlayer() != PC) { continue; }
			Out.Add(C);
		}
		return Out;
	}

	TArray<UObject*> StackList(UWidget* C)
	{
		TArray<UObject*> Out;
		if (const FArrayProperty* AP = CastField<FArrayProperty>(C->GetClass()->FindPropertyByName(TEXT("WidgetList"))))
		{
			FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(C));
			const FObjectPropertyBase* Inner = CastField<FObjectPropertyBase>(AP->Inner);
			for (int32 i = 0; Inner && i < H.Num(); ++i) { Out.Add(Inner->GetObjectPropertyValue(H.GetRawPtr(i))); }
		}
		return Out;
	}

	/** True when the widget and all its Slate ancestors are visible and it has a size. */
	bool OnScreen(UWidget* W)
	{
		TSharedPtr<SWidget> S = W ? W->GetCachedWidget() : nullptr;
		if (!S.IsValid() || S->GetPaintSpaceGeometry().GetLocalSize().IsNearlyZero()) { return false; }
		for (TSharedPtr<SWidget> P = S; P.IsValid(); P = P->GetParentWidget())
		{
			if (!P->GetVisibility().IsVisible()) { return false; }
		}
		return true;
	}

	TArray<SWidget*> Chain(SWidget* W)
	{
		TArray<SWidget*> Out;
		for (TSharedPtr<SWidget> P = W ? W->AsShared() : TSharedPtr<SWidget>(); P.IsValid(); P = P->GetParentWidget()) { Out.Insert(P.Get(), 0); }
		return Out; // root first
	}

	/** True when A is painted after (on top of) B: compare their branches under the closest common Slate parent. */
	bool PaintedAfter(SWidget* A, SWidget* B)
	{
		const TArray<SWidget*> CA = Chain(A), CB = Chain(B);
		int32 i = 0;
		while (i < CA.Num() && i < CB.Num() && CA[i] == CB[i]) { ++i; }
		if (i == 0 || i >= CA.Num() || i >= CB.Num()) { return false; } // unrelated, or one contains the other
		FChildren* Kids = CA[i - 1]->GetChildren();
		int32 IA = INDEX_NONE, IB = INDEX_NONE;
		for (int32 k = 0; Kids && k < Kids->Num(); ++k)
		{
			const SWidget* K = &Kids->GetChildAt(k).Get();
			if (K == CA[i]) { IA = k; }
			if (K == CB[i]) { IB = k; }
		}
		return IA != INDEX_NONE && IB != INDEX_NONE && IA > IB;
	}

	/** A hit-testable widget drawn over Point on top of Target (UMG level, works without a fresh paint). */
	UWidget* Blocker(UWorld* World, UWidget* Target, const FVector2D& Point)
	{
		SWidget* T = Target->GetCachedWidget().Get();
		for (TObjectIterator<UWidget> It; It; ++It)
		{
			UWidget* W = *It;
			if (!IsValid(W) || W == Target || W->GetWorld() != World || W->GetVisibility() != ESlateVisibility::Visible || !OnScreen(W)) { continue; }
			SWidget* S = W->GetCachedWidget().Get();
			const FGeometry G = S->GetPaintSpaceGeometry();
			const FVector2D TL = G.GetAbsolutePosition(), BR = TL + G.GetAbsoluteSize();
			if (Point.X < TL.X || Point.Y < TL.Y || Point.X > BR.X || Point.Y > BR.Y) { continue; }
			if (PaintedAfter(S, T)) { return W; }
		}
		return nullptr;
	}

	/** The SButton (or subclass, e.g. CommonUI's) at or under W. */
	TSharedPtr<SButton> FindSButton(const TSharedPtr<SWidget>& W, int32 Depth = 0)
	{
		if (!W.IsValid() || Depth > 8) { return nullptr; }
		if (W->GetType().ToString().EndsWith(TEXT("Button"))) { return StaticCastSharedPtr<SButton>(W); }
		FChildren* Kids = W->GetChildren();
		for (int32 k = 0; Kids && k < Kids->Num(); ++k)
		{
			if (TSharedPtr<SButton> B = FindSButton(Kids->GetChildAt(k), Depth + 1)) { return B; }
		}
		return nullptr;
	}

	/** Every text of the widget in drawing order: TextBlocks (CommonTextBlock too) under UMG panels, buttons and user widgets. */
	void CollectTexts(UWidget* W, TArray<FString>& Out, int32 Depth = 0)
	{
		if (!W || Depth > 12) { return; }
		if (UTextBlock* T = Cast<UTextBlock>(W)) { const FString S = T->GetText().ToString(); if (!S.IsEmpty()) { Out.Add(S); } return; }
		if (UPanelWidget* P = Cast<UPanelWidget>(W))
		{
			for (int32 i = 0; i < P->GetChildrenCount(); ++i) { CollectTexts(P->GetChildAt(i), Out, Depth + 1); }
		}
		if (UUserWidget* U = Cast<UUserWidget>(W)) { if (U->WidgetTree) { CollectTexts(U->WidgetTree->RootWidget, Out, Depth + 1); } }
	}

	/** Fallback for widgets whose texts are not in a UMG tree: the STextBlocks of the built Slate widget. */
	void CollectSlateTexts(const TSharedPtr<SWidget>& W, TArray<FString>& Out, int32 Depth = 0)
	{
		if (!W.IsValid() || Depth > 16) { return; }
		if (W->GetType() == FName(TEXT("STextBlock"))) { const FString S = StaticCastSharedPtr<STextBlock>(W)->GetText().ToString(); if (!S.IsEmpty()) { Out.Add(S); } }
		FChildren* Kids = W->GetChildren();
		for (int32 k = 0; Kids && k < Kids->Num(); ++k) { CollectSlateTexts(Kids->GetChildAt(k), Out, Depth + 1); }
	}

	bool HasLetterOrDigit(const FString& S)
	{
		for (const TCHAR C : S) { if (FChar::IsAlnum(C)) { return true; } }
		return false;
	}

	/** The label of a button: the first text with a letter or digit. An icon or glyph character (a gear, a play triangle) is used only when there is nothing else. */
	FString PickLabel(const TArray<FString>& Texts)
	{
		for (const FString& T : Texts) { if (HasLetterOrDigit(T)) { return T; } }
		return Texts.Num() ? Texts[0] : FString();
	}

	TArray<FString> AllTexts(UWidget* W)
	{
		TArray<FString> Out;
		CollectTexts(W, Out);
		if (Out.Num() == 0 && W) { CollectSlateTexts(W->GetCachedWidget(), Out); }
		// Styled buttons draw a text several times (shadow, outline, copies): list each text once, in first-seen order.
		TArray<FString> Unique;
		for (const FString& T : Out) { Unique.AddUnique(T); }
		return Unique;
	}

	/** The tooltip text set on the widget (or on the UButton inside it). */
	FString Tooltip(UWidget* W, UWidget* Inner = nullptr)
	{
		if (W && !W->ToolTipText.IsEmpty()) { return W->ToolTipText.ToString(); }
		if (Inner && !Inner->ToolTipText.IsEmpty()) { return Inner->ToolTipText.ToString(); }
		return FString();
	}

	/** The SViewport geometry of the player's game viewport (to turn Slate positions into viewport pixels). */
	bool ViewportGeometry(APlayerController* PC, FGeometry& Out)
	{
		UWorld* World = PC ? PC->GetWorld() : nullptr;
		UGameViewportClient* GVC = World ? World->GetGameViewport() : nullptr;
		TSharedPtr<SViewport> VP = GVC ? GVC->GetGameViewportWidget() : nullptr;
		if (!VP.IsValid()) { return false; }
		Out = VP->GetPaintSpaceGeometry();
		return true;
	}

	struct FButtonInfo
	{
		UWidget* Widget = nullptr;   // UButton or CommonButtonBase
		UButton* Clickable = nullptr; // the UButton that fires the click (CommonButtonBase: its internal button)
		FString Label;                // the first text with a letter or digit
		TArray<FString> Texts;        // every text inside the button, in drawing order (an icon glyph may come first)
		FString Tooltip;
		FString Owner;
		FString Layer = TEXT("viewport"); // the activatable stack that holds the page, or "viewport" for a plain widget
		bool bTop = true;                 // false: the page is deactivated or sits below another page of its stack
		FString HiddenReason;             // why it is not top (for includeHidden and error messages)
	};

	/**
	 * Where a widget sits: Layer = the CommonUI stack that holds its page ("viewport" for a plain UMG widget);
	 * bTop = false when a page above it is not activated, or another page of the same stack is drawn over it.
	 */
	void Classify(UWidget* W, const TArray<UWidget*>& Stacks, FString& Layer, bool& bTop, FString& Reason)
	{
		Layer = TEXT("viewport");
		bTop = true;
		UClass* Act = CommonClass(TEXT("CommonActivatableWidget"));
		if (!Act) { return; }
		for (UObject* O = W; O; O = O->GetOuter())
		{
			if (!O->IsA(Act)) { continue; }
			if (!CallBool(O, TEXT("IsActivated")))
			{
				bTop = false;
				if (Reason.IsEmpty()) { Reason = FString::Printf(TEXT("page %s is not activated"), *O->GetClass()->GetName()); }
			}
			for (UWidget* S : Stacks)
			{
				const TArray<UObject*> List = StackList(S);
				if (!List.Contains(O)) { continue; }
				Layer = S->GetName();
				if (List.Last() != O)
				{
					bTop = false;
					if (Reason.IsEmpty()) { Reason = FString::Printf(TEXT("page %s is below %s on stack %s"), *O->GetClass()->GetName(), List.Last() ? *List.Last()->GetClass()->GetName() : TEXT("?"), *S->GetName()); }
				}
			}
		}
	}

	/** Buttons on this player's screen. A CommonButtonBase is reported once, not with its internal UButton.
	 *  Off-screen buttons are skipped unless bIncludeOffscreen. */
	TArray<FButtonInfo> Buttons(APlayerController* PC, bool bIncludeOffscreen = false)
	{
		TArray<FButtonInfo> Out;
		UClass* CBB = CommonClass(TEXT("CommonButtonBase"));
		const TArray<UWidget*> Stacks = Containers(PC);
		TSet<UWidget*> Seen;
		for (TObjectIterator<UButton> It; It; ++It)
		{
			UButton* B = *It;
			if (!IsValid(B) || B->GetWorld() != PC->GetWorld() || (!bIncludeOffscreen && !OnScreen(B))) { continue; }
			if (bIncludeOffscreen && !B->GetCachedWidget().IsValid()) { continue; }
			UUserWidget* OwnerWidget = B->GetTypedOuter<UUserWidget>();
			if (OwnerWidget && OwnerWidget->GetOwningPlayer() && OwnerWidget->GetOwningPlayer() != PC) { continue; }
			FButtonInfo I;
			I.Clickable = B;
			I.Widget = (CBB && OwnerWidget && OwnerWidget->IsA(CBB)) ? (UWidget*)OwnerWidget : (UWidget*)B;
			if (Seen.Contains(I.Widget)) { continue; }
			Seen.Add(I.Widget);
			I.Texts = AllTexts(I.Widget);
			I.Label = PickLabel(I.Texts);
			I.Tooltip = Tooltip(I.Widget, I.Clickable);
			UUserWidget* Page = I.Widget->GetTypedOuter<UUserWidget>();
			I.Owner = Page ? Page->GetClass()->GetName() : FString();
			Classify(I.Widget, Stacks, I.Layer, I.bTop, I.HiddenReason);
			if (!OnScreen(I.Widget) && I.HiddenReason.IsEmpty()) { I.HiddenReason = TEXT("not drawn (hidden, collapsed or zero size)"); }
			Out.Add(I);
		}
		return Out;
	}

	/** Visible and on the top page of its stack: what the player can use right now. */
	bool IsUsable(const FButtonInfo& B) { return B.bTop && OnScreen(B.Widget); }

	bool ParseKeyName(const FString& Name, FKey& Out)
	{
		Out = FKey(FName(*Name));
		if (Out.IsValid()) { return true; }
		TArray<FKey> All;
		EKeys::GetAllKeys(All);
		for (const FKey& K : All) { if (K.GetFName().ToString().Equals(Name, ESearchCase::IgnoreCase)) { Out = K; return true; } }
		return false;
	}

	TArray<FString> SuggestKeys(const FString& Name)
	{
		TArray<FKey> All;
		EKeys::GetAllKeys(All);
		TArray<FString> Names;
		for (const FKey& K : All) { Names.Add(K.GetFName().ToString()); }
		return EW::Suggest(Name, Names);
	}

	/** What has user focus: the Slate widget type and the UMG widget (and page) that owns it. */
	TSharedRef<FJsonObject> FocusInfo(APlayerController* PC, int32 UserIndex)
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		TSharedPtr<SWidget> F = FSlateApplication::IsInitialized() ? FSlateApplication::Get().GetUserFocusedWidget(UserIndex) : nullptr;
		J->SetBoolField(TEXT("focused"), F.IsValid());
		J->SetStringField(TEXT("slateType"), F.IsValid() ? F->GetType().ToString() : TEXT("none"));
		if (!F.IsValid()) { return J; }
		TMap<const SWidget*, UWidget*> Owners;
		for (TObjectIterator<UWidget> It; It; ++It)
		{
			UWidget* W = *It;
			if (IsValid(W) && W->GetWorld() == PC->GetWorld() && W->GetCachedWidget().IsValid()) { Owners.Add(W->GetCachedWidget().Get(), W); }
		}
		for (TSharedPtr<SWidget> P = F; P.IsValid(); P = P->GetParentWidget())
		{
			if (UWidget* const* W = Owners.Find(P.Get()))
			{
				J->SetStringField(TEXT("widget"), (*W)->GetName());
				J->SetStringField(TEXT("class"), (*W)->GetClass()->GetName());
				UUserWidget* Page = (*W)->GetTypedOuter<UUserWidget>();
				if (UUserWidget* Self = Cast<UUserWidget>(*W)) { Page = Self; }
				if (Page) { J->SetStringField(TEXT("page"), Page->GetClass()->GetName()); }
				break;
			}
		}
		return J;
	}

	TArray<TSharedPtr<FJsonValue>> Rect(UWidget* W, const FGeometry& VP)
	{
		const FGeometry G = W->GetCachedWidget()->GetPaintSpaceGeometry();
		const FVector2D TL = VP.AbsoluteToLocal(G.GetAbsolutePosition());
		const FVector2D BR = VP.AbsoluteToLocal(G.GetAbsolutePosition() + G.GetAbsoluteSize());
		return { MakeShared<FJsonValueNumber>(FMath::RoundToDouble(TL.X)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(TL.Y)),
			MakeShared<FJsonValueNumber>(FMath::RoundToDouble(BR.X)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(BR.Y)) };
	}

	/** Lists only what the player can use now (visible, top page); bIncludeHidden adds the rest with the reason. */
	TSharedRef<FJsonObject> UiState(APlayerController* PC, bool bIncludeHidden = false)
	{
		TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
		FGeometry VP;
		const bool bVP = ViewportGeometry(PC, VP);
		UWorld* World = PC->GetWorld();

		TArray<TSharedPtr<FJsonValue>> Screens;
		int32 HiddenWidgets = 0, HiddenButtons = 0;
		const TArray<UWidget*> StackWidgets = Containers(PC);
		for (TObjectIterator<UUserWidget> It; It; ++It)
		{
			UUserWidget* U = *It;
			if (!IsValid(U) || U->GetWorld() != World || !U->IsInViewport()) { continue; }
			if (U->GetOwningPlayer() && U->GetOwningPlayer() != PC) { continue; }
			TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
			J->SetStringField(TEXT("name"), U->GetName());
			J->SetStringField(TEXT("class"), U->GetClass()->GetName());
			const bool bVisible = OnScreen(U);
			J->SetBoolField(TEXT("visible"), bVisible);
			if (!bVisible && !bIncludeHidden) { ++HiddenWidgets; continue; }
			Screens.Add(MakeShared<FJsonValueObject>(J));
		}
		Data->SetArrayField(TEXT("viewportWidgets"), Screens);

		UClass* Activatable = CommonClass(TEXT("CommonActivatableWidget"));
		UClass* Container = CommonClass(TEXT("CommonActivatableWidgetContainerBase"));
		Data->SetBoolField(TEXT("commonUI"), Activatable != nullptr);
		if (Activatable)
		{
			TArray<TSharedPtr<FJsonValue>> Acts;
			for (TObjectIterator<UUserWidget> It; It; ++It)
			{
				UUserWidget* U = *It;
				if (!IsValid(U) || !U->IsA(Activatable) || U->GetWorld() != World) { continue; }
				if (U->GetOwningPlayer() && U->GetOwningPlayer() != PC) { continue; }
				const bool bActive = CallBool(U, TEXT("IsActivated"));
				const bool bVisible = OnScreen(U);
				if (!bActive && !bVisible) { continue; }
				FString Layer, Reason;
				bool bTop = true;
				Classify(U, StackWidgets, Layer, bTop, Reason);
				TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
				J->SetStringField(TEXT("name"), U->GetName());
				J->SetStringField(TEXT("class"), U->GetClass()->GetName());
				J->SetBoolField(TEXT("activated"), bActive);
				J->SetBoolField(TEXT("visible"), bVisible);
				J->SetStringField(TEXT("layer"), Layer);
				J->SetBoolField(TEXT("top"), bTop);
				if (!Reason.IsEmpty()) { J->SetStringField(TEXT("hiddenReason"), Reason); }
				if (!bIncludeHidden && !(bActive && bVisible && bTop)) { ++HiddenWidgets; continue; }
				Acts.Add(MakeShared<FJsonValueObject>(J));
			}
			Data->SetArrayField(TEXT("activatableWidgets"), Acts);
		}
		const FString InputType = InputTypeName(PC);
		if (!InputType.IsEmpty()) { Data->SetStringField(TEXT("inputType"), InputType); }
		if (Container)
		{
			TArray<TSharedPtr<FJsonValue>> Stacks;
			for (UWidget* C : Containers(PC))
			{
				TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
				J->SetStringField(TEXT("name"), C->GetName());
				J->SetStringField(TEXT("class"), C->GetClass()->GetName());
				if (UUserWidget* Owner = C->GetTypedOuter<UUserWidget>()) { J->SetStringField(TEXT("owner"), Owner->GetClass()->GetName()); }
				TArray<TSharedPtr<FJsonValue>> List;
				for (UObject* W : StackList(C)) { List.Add(MakeShared<FJsonValueString>(W ? W->GetClass()->GetName() : TEXT("null"))); }
				J->SetArrayField(TEXT("widgets"), List);
				if (List.Num()) { J->SetStringField(TEXT("top"), List.Last()->AsString()); }
				Stacks.Add(MakeShared<FJsonValueObject>(J));
			}
			Data->SetArrayField(TEXT("stacks"), Stacks);
		}

		TArray<TSharedPtr<FJsonValue>> Btns;
		for (const FButtonInfo& B : Buttons(PC, bIncludeHidden))
		{
			if (!IsUsable(B) && !bIncludeHidden) { ++HiddenButtons; continue; }
			TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
			J->SetStringField(TEXT("name"), B.Widget->GetName());
			J->SetStringField(TEXT("class"), B.Widget->GetClass()->GetName());
			J->SetStringField(TEXT("label"), B.Label);
			TArray<TSharedPtr<FJsonValue>> TextJ;
			for (const FString& T : B.Texts) { TextJ.Add(MakeShared<FJsonValueString>(T)); }
			J->SetArrayField(TEXT("text"), TextJ);
			if (!B.Tooltip.IsEmpty()) { J->SetStringField(TEXT("tooltip"), B.Tooltip); }
			J->SetStringField(TEXT("page"), B.Owner);
			J->SetStringField(TEXT("layer"), B.Layer);
			J->SetBoolField(TEXT("top"), B.bTop);
			J->SetBoolField(TEXT("visible"), OnScreen(B.Widget));
			if (!B.HiddenReason.IsEmpty()) { J->SetStringField(TEXT("hiddenReason"), B.HiddenReason); }
			J->SetBoolField(TEXT("enabled"), B.Widget->GetIsEnabled() && B.Clickable->GetIsEnabled());
			if (B.Widget != B.Clickable && B.Widget->FindFunction(TEXT("GetSelected"))) { J->SetBoolField(TEXT("selected"), CallBool(B.Widget, TEXT("GetSelected"))); }
			if (B.Widget != B.Clickable && B.Widget->FindFunction(TEXT("IsInteractionEnabled"))) { J->SetBoolField(TEXT("interactionEnabled"), CallBool(B.Widget, TEXT("IsInteractionEnabled"))); }
			J->SetStringField(TEXT("path"), B.Widget->GetPathName());
			if (bVP && B.Widget->GetCachedWidget().IsValid()) { J->SetArrayField(TEXT("rect"), Rect(B.Widget, VP)); }
			Btns.Add(MakeShared<FJsonValueObject>(J));
		}
		Data->SetArrayField(TEXT("buttons"), Btns);
		Data->SetBoolField(TEXT("includeHidden"), bIncludeHidden);
		if (!bIncludeHidden) { Data->SetNumberField(TEXT("hiddenButtons"), HiddenButtons); Data->SetNumberField(TEXT("hiddenWidgets"), HiddenWidgets); }
		return Data;
	}
	FString DeviceOf(const FKey& K) { return K.IsGamepadKey() ? TEXT("gamepad") : K.IsTouch() ? TEXT("touch") : TEXT("keyboardMouse"); }

	/** A prompt text names a key: the key name (B, SpaceBar), its display names (Space), or the Xbox letter of a gamepad face button. */
	bool KeyMatchesText(const FKey& K, const FString& Text)
	{
		const FString T = Text.TrimStartAndEnd();
		if (T.IsEmpty()) { return false; }
		if (K.GetFName().ToString().Equals(T, ESearchCase::IgnoreCase) || K.GetDisplayName(false).ToString().Equals(T, ESearchCase::IgnoreCase) || K.GetDisplayName(true).ToString().Equals(T, ESearchCase::IgnoreCase)) { return true; }
		const FString N = K.GetFName().ToString();
		if (N.EndsWith(TEXT("FaceButton_Bottom"))) { return T.Equals(TEXT("A"), ESearchCase::IgnoreCase); }
		if (N.EndsWith(TEXT("FaceButton_Right"))) { return T.Equals(TEXT("B"), ESearchCase::IgnoreCase); }
		if (N.EndsWith(TEXT("FaceButton_Left"))) { return T.Equals(TEXT("X"), ESearchCase::IgnoreCase); }
		if (N.EndsWith(TEXT("FaceButton_Top"))) { return T.Equals(TEXT("Y"), ESearchCase::IgnoreCase); }
		return false;
	}

	FString CallText(UObject* Obj, const TCHAR* Fn)
	{
		UFunction* F = Obj ? Obj->FindFunction(FName(Fn)) : nullptr;
		if (!F || F->ParmsSize > 256) { return FString(); }
		FStructOnScope Parms(F);
		Obj->ProcessEvent(F, Parms.GetStructMemory());
		const FTextProperty* TP = CastField<FTextProperty>(F->GetReturnProperty());
		return TP ? TP->GetPropertyValue_InContainer(Parms.GetStructMemory()).ToString() : FString();
	}

	TArray<TSharedPtr<FJsonValue>> StrArray(const TArray<FString>& In)
	{
		TArray<TSharedPtr<FJsonValue>> Out;
		for (const FString& S : In) { Out.Add(MakeShared<FJsonValueString>(S)); }
		return Out;
	}
}

void RegisterUIRuntimeTools(FEWRegistry& R)
{
	R.Register(TEXT("ui_prompts"),
		TEXT("Check the input prompts a Play In Editor player sees against what the input is really bound to (read-only). Lists the CommonUI action widgets on screen (their shown text, the Enhanced Input action or the input action rows, and the keys that action has for the current input type) and the on-screen texts that look like a key name (B, E, Space) with the actions that key triggers now. This finds a hint that shows a key the action does not use (for example a hint 'B' for an action bound to P). check = a list of {prompt, action}: each entry says OK, PROMPT_NOT_SHOWN (no such text on screen), ACTION_NOT_BOUND or MISMATCH, with the keys the action has and the actions the shown key triggers. The input type decides which keys count (MouseAndKeyboard, Gamepad, Touch; ui_input_type switches it, or pass device). Use input_map_list for the full mapping and ui_input_audit for the widget assets."),
		TEXT(R"({"type":"object","properties":{
			"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."},
			"device":{"type":"string","enum":["keyboardMouse","gamepad","touch"],"description":"Which keys count. Default: from the current CommonUI input type."},
			"check":{"type":"array","description":"Prompts to verify: [{\"prompt\":\"B\",\"action\":\"IA_Secondary\"}]. action is an Input Action name or asset path (wildcards allowed).","items":{"type":"object"}}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { return Bad(TEXT("NO_PLAYER"), Err); }

			// Validate the checks first.
			struct FCheck { FString Prompt, Action; };
			TArray<FCheck> Checks;
			const TArray<TSharedPtr<FJsonValue>>* CheckArr = nullptr;
			if (Args->TryGetArrayField(TEXT("check"), CheckArr))
			{
				for (int32 i = 0; i < CheckArr->Num(); ++i)
				{
					const TSharedPtr<FJsonObject>* O = nullptr;
					if (!(*CheckArr)[i]->TryGetObject(O)) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("check[%d] must be an object {prompt, action}."), i)); }
					for (const auto& KV : (*O)->Values)
					{
						if (KV.Key != TEXT("prompt") && KV.Key != TEXT("action")) { return Bad(TEXT("UNKNOWN_ARGUMENT"), FString::Printf(TEXT("check[%d] has unknown key '%s' (allowed: prompt, action)."), i, *KV.Key)); }
					}
					FCheck C;
					(*O)->TryGetStringField(TEXT("prompt"), C.Prompt);
					(*O)->TryGetStringField(TEXT("action"), C.Action);
					if (C.Prompt.TrimStartAndEnd().IsEmpty() || C.Action.TrimStartAndEnd().IsEmpty()) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("check[%d] needs both prompt and action."), i)); }
					Checks.Add(C);
				}
			}

			const FString InputType = InputTypeName(PC);
			FString Device = EW::Str(Args, TEXT("device"));
			if (Device.IsEmpty()) { Device = InputType == TEXT("Gamepad") ? TEXT("gamepad") : InputType == TEXT("Touch") ? TEXT("touch") : TEXT("keyboardMouse"); }

			// action name -> keys of the chosen device
			TMap<FString, TArray<FKey>> Bound;
			TMap<FString, FString> ActionPath;
			TArray<FEnhancedActionKeyMapping> Live;
			EW::PlayerInputMappings(PC, Live);
			for (const FEnhancedActionKeyMapping& M : Live)
			{
				if (!M.Action || DeviceOf(M.Key) != Device) { continue; }
				TArray<FKey>& Keys = Bound.FindOrAdd(M.Action->GetName());
				Keys.AddUnique(M.Key);
				ActionPath.Add(M.Action->GetName(), M.Action->GetPathName());
			}
			auto KeysJson = [](const TArray<FKey>& Keys)
			{
				TArray<FString> Names;
				for (const FKey& K : Keys) { Names.Add(K.GetFName().ToString()); }
				return StrArray(Names);
			};
			auto ActionsForText = [&Bound](const FString& Text)
			{
				TArray<FString> Out;
				for (const TPair<FString, TArray<FKey>>& P : Bound) { for (const FKey& K : P.Value) { if (KeyMatchesText(K, Text)) { Out.AddUnique(P.Key); break; } } }
				Out.Sort();
				return Out;
			};

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			FEWResult Res = FEWResult::Ok(Data);
			Data->SetStringField(TEXT("device"), Device);
			if (!InputType.IsEmpty()) { Data->SetStringField(TEXT("inputType"), InputType); }
			Data->SetNumberField(TEXT("boundActions"), Bound.Num());
			if (Bound.Num() == 0) { Res.Warnings.Add(FString::Printf(TEXT("This player has no %s Enhanced Input mappings (no active mapping context, or the game uses legacy input). Prompts cannot be compared."), *Device)); }

			// CommonUI action widgets on screen.
			TArray<TSharedPtr<FJsonValue>> Widgets;
			UWorld* World = PC->GetWorld();
			if (UClass* AW = CommonClass(TEXT("CommonActionWidget")))
			{
				for (TObjectIterator<UWidget> It; It; ++It)
				{
					UWidget* W = *It;
					if (!IsValid(W) || !W->IsA(AW) || W->GetWorld() != World || !OnScreen(W)) { continue; }
					UUserWidget* Page = W->GetTypedOuter<UUserWidget>();
					if (Page && Page->GetOwningPlayer() && Page->GetOwningPlayer() != PC) { continue; }
					TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
					J->SetStringField(TEXT("name"), W->GetName());
					if (Page) { J->SetStringField(TEXT("page"), Page->GetClass()->GetName()); }
					const FString Shown = CallText(W, TEXT("GetDisplayText"));
					J->SetStringField(TEXT("text"), Shown);
					if (UObject* IA = CallWithObject(W, TEXT("GetEnhancedInputAction"), nullptr))
					{
						J->SetStringField(TEXT("enhancedAction"), IA->GetName());
						const TArray<FKey>* Keys = Bound.Find(IA->GetName());
						J->SetArrayField(TEXT("keys"), Keys ? KeysJson(*Keys) : TArray<TSharedPtr<FJsonValue>>());
						if (!Shown.IsEmpty() && Keys) { bool bAny = false; for (const FKey& K : *Keys) { bAny |= KeyMatchesText(K, Shown); } J->SetBoolField(TEXT("textMatchesKey"), bAny); }
					}
					TArray<FString> Rows;
					if (const FArrayProperty* AP = CastField<FArrayProperty>(W->GetClass()->FindPropertyByName(TEXT("InputActions"))))
					{
						const FStructProperty* SP = CastField<FStructProperty>(AP->Inner);
						FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(W));
						for (int32 i = 0; SP && SP->Struct == FDataTableRowHandle::StaticStruct() && i < H.Num(); ++i) { Rows.Add(reinterpret_cast<const FDataTableRowHandle*>(H.GetRawPtr(i))->RowName.ToString()); }
					}
					if (Rows.Num()) { J->SetArrayField(TEXT("inputActionRows"), StrArray(Rows)); }
					Widgets.Add(MakeShared<FJsonValueObject>(J));
				}
			}
			Data->SetArrayField(TEXT("actionWidgets"), Widgets);

			// On-screen texts and where they come from.
			TArray<TSharedPtr<FJsonValue>> KeyTexts;
			TSet<FString> ShownTexts;
			for (TObjectIterator<UTextBlock> It; It; ++It)
			{
				UTextBlock* T = *It;
				if (!IsValid(T) || T->GetWorld() != World || !OnScreen(T)) { continue; }
				UUserWidget* Page = T->GetTypedOuter<UUserWidget>();
				if (Page && Page->GetOwningPlayer() && Page->GetOwningPlayer() != PC) { continue; }
				const FString Text = T->GetText().ToString().TrimStartAndEnd();
				if (Text.IsEmpty()) { continue; }
				ShownTexts.Add(Text.ToLower());
				if (Text.Len() > 14) { continue; }
				const TArray<FString> Acts = ActionsForText(Text);
				if (Acts.Num() == 0) { continue; }
				TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
				J->SetStringField(TEXT("text"), Text);
				J->SetStringField(TEXT("widget"), T->GetName());
				if (Page) { J->SetStringField(TEXT("page"), Page->GetClass()->GetName()); }
				J->SetArrayField(TEXT("actions"), StrArray(Acts));
				KeyTexts.Add(MakeShared<FJsonValueObject>(J));
			}
			Data->SetArrayField(TEXT("keyTexts"), KeyTexts);
			TSet<FString> WidgetTexts;
			for (const TSharedPtr<FJsonValue>& V : Widgets) { WidgetTexts.Add(V->AsObject()->GetStringField(TEXT("text")).ToLower()); }

			// Verify the requested prompts.
			if (Checks.Num())
			{
				TArray<TSharedPtr<FJsonValue>> Out;
				int32 Problems = 0;
				for (const FCheck& C : Checks)
				{
					TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
					J->SetStringField(TEXT("prompt"), C.Prompt);
					J->SetStringField(TEXT("action"), C.Action);
					const FString P = C.Prompt.TrimStartAndEnd();
					const bool bShown = ShownTexts.Contains(P.ToLower()) || WidgetTexts.Contains(P.ToLower());
					J->SetBoolField(TEXT("shown"), bShown);
					TArray<FString> Hit;
					for (const TPair<FString, TArray<FKey>>& B : Bound)
					{
						if (B.Key.Equals(C.Action, ESearchCase::IgnoreCase) || B.Key.MatchesWildcard(C.Action) || ActionPath[B.Key].Equals(C.Action, ESearchCase::IgnoreCase) || FPackageName::ObjectPathToPackageName(ActionPath[B.Key]).Equals(C.Action, ESearchCase::IgnoreCase)) { Hit.Add(B.Key); }
					}
					FString Status;
					if (Hit.Num() == 0)
					{
						Status = TEXT("ACTION_NOT_BOUND");
						TArray<FString> All;
						Bound.GetKeys(All);
						J->SetArrayField(TEXT("boundActionNames"), StrArray(EW::Suggest(C.Action, All)));
					}
					else
					{
						TArray<FString> KeyNames;
						bool bMatch = false;
						for (const FString& A : Hit) { for (const FKey& K : Bound[A]) { KeyNames.AddUnique(K.GetFName().ToString()); bMatch |= KeyMatchesText(K, P); } }
						J->SetArrayField(TEXT("actionKeys"), StrArray(KeyNames));
						J->SetArrayField(TEXT("matchedActions"), StrArray(Hit));
						J->SetArrayField(TEXT("promptKeyTriggers"), StrArray(ActionsForText(P)));
						Status = !bShown ? TEXT("PROMPT_NOT_SHOWN") : bMatch ? TEXT("OK") : TEXT("MISMATCH");
					}
					if (Status == TEXT("PROMPT_NOT_SHOWN") && Hit.Num() == 0) { Status = TEXT("ACTION_NOT_BOUND"); }
					J->SetStringField(TEXT("status"), Status);
					if (Status != TEXT("OK")) { ++Problems; Res.Warnings.Add(FString::Printf(TEXT("Prompt '%s' for %s: %s."), *C.Prompt, *C.Action, *Status)); }
					Out.Add(MakeShared<FJsonValueObject>(J));
				}
				Data->SetArrayField(TEXT("checks"), Out);
				Data->SetNumberField(TEXT("problems"), Problems);
				Data->SetBoolField(TEXT("allOk"), Problems == 0);
			}
			return Res;
		});

	R.Register(TEXT("ui_state"),
		TEXT("What a Play In Editor player's UI shows right now: widgets in the viewport, CommonUI activatable widgets and activatable stacks with their widget list and top, and the buttons (UMG Button or CommonButtonBase) with label (the first text with a letter or digit, so a gear or play glyph does not hide the word), text (every text inside the button in drawing order, glyphs included), tooltip, page, layer (the stack holding the page), enabled and rectangle in viewport pixels. By default it lists only what the player can use: visible, on the top page of its stack, page activated. A closed or covered page (for example the old Back/Save after Settings closed) is left out and counted in hiddenButtons / hiddenWidgets. includeHidden lists everything, with top, visible, layer and hiddenReason per item."),
		TEXT(R"({"type":"object","properties":{
			"player":{"type":"integer","description":"Local player index, default 0."},
			"includeHidden":{"type":"boolean","description":"Also list widgets and buttons that are hidden, covered by a higher page, or on a deactivated page (each with layer, top, visible and hiddenReason). Default false."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { return Bad(TEXT("NO_PLAYER"), Err); }
			return FEWResult::Ok(UiState(PC, EW::Bool(Args, TEXT("includeHidden"), false)));
		});

	R.RegisterAsync(TEXT("ui_click"),
		TEXT("Click a button the player can use in Play In Editor (visible, on the top page of its stack), found by widget name, label, any text inside the button (a glyph such as a gear works too) or tooltip (wildcards allowed). method click (default) first checks that no other hit-testable widget is drawn over the button center (else COVERED), then presses the Slate button (press + click, as a pointer or gamepad accept does); method event fires the UMG OnClicked directly, even when covered. Returns the UI state after settleSeconds."),
		TEXT(R"({"type":"object","properties":{
			"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."},
			"button":{"type":"string","description":"Widget name, label, any text inside the button or its tooltip, e.g. Play or *Settings*."},
			"method":{"type":"string","enum":["click","event"],"description":"click (default): checked like a real click, refused when covered; event: fire OnClicked directly."},
			"settleSeconds":{"type":"number","description":"Wait after the click before reading the UI, default 0.3."}},
			"required":["button"]})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { Done(Bad(TEXT("NO_PLAYER"), Err)); return; }
			const FString Key = EW::Str(Args, TEXT("button"));
			const FString Method = Args->HasField(TEXT("method")) ? EW::Str(Args, TEXT("method")).ToLower() : TEXT("click");
			if (Method != TEXT("click") && Method != TEXT("event")) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("method must be click or event."))); return; }
			const float Settle = FMath::Clamp((float)EW::Num(Args, TEXT("settleSeconds"), 0.3), 0.f, 10.f);

			// Only buttons the player can use now (visible, top page of its stack) are candidates; a button on a
			// closed or covered page is reported as such instead of being clicked.
			const TArray<FButtonInfo> All = Buttons(PC, /*bIncludeOffscreen*/ true);
			auto Matches = [&Key](const FButtonInfo& B)
			{
				if (B.Widget->GetName() == Key || B.Label.Equals(Key, ESearchCase::IgnoreCase) || B.Label.MatchesWildcard(Key) || B.Widget->GetName().MatchesWildcard(Key)) { return true; }
				if (!B.Tooltip.IsEmpty() && (B.Tooltip.Equals(Key, ESearchCase::IgnoreCase) || B.Tooltip.MatchesWildcard(Key))) { return true; }
				for (const FString& T : B.Texts) { if (T.Equals(Key, ESearchCase::IgnoreCase) || T.MatchesWildcard(Key)) { return true; } }
				return false;
			};
			auto Describe = [](const FButtonInfo& B) { return FString::Printf(TEXT("%s (page %s, layer %s)"), *B.Widget->GetName(), *B.Owner, *B.Layer); };
			TArray<FButtonInfo> Hits, Unusable;
			TArray<FString> Names;
			for (const FButtonInfo& B : All)
			{
				if (!IsUsable(B)) { if (Matches(B)) { Unusable.Add(B); } continue; }
				Names.Add(B.Label.IsEmpty() ? B.Widget->GetName() : B.Label);
				if (Matches(B)) { Hits.Add(B); }
			}
			if (Hits.Num() == 0)
			{
				if (Unusable.Num() > 0)
				{
					TArray<FString> Where;
					for (const FButtonInfo& B : Unusable) { Where.Add(FString::Printf(TEXT("%s: %s"), *Describe(B), *B.HiddenReason)); }
					Done(Bad(TEXT("BUTTON_NOT_FOUND"), FString::Printf(TEXT("'%s' exists only on a page the player cannot use now (see ui_state includeHidden)."), *Key), Where));
					return;
				}
				Done(Bad(TEXT("BUTTON_NOT_FOUND"), FString::Printf(TEXT("No visible button '%s' on player %d's screen."), *Key, (int32)EW::Num(Args, TEXT("player"), 0)), EW::Suggest(Key, Names)));
				return;
			}
			if (Hits.Num() > 1)
			{
				TArray<FString> Which;
				for (const FButtonInfo& B : Hits) { Which.Add(Describe(B)); }
				Done(Bad(TEXT("AMBIGUOUS"), FString::Printf(TEXT("%d buttons match '%s'; pass the widget name. Candidates are listed as name (page, layer)."), Hits.Num(), *Key), Which));
				return;
			}
			const FButtonInfo B = Hits[0];
			if (!B.Widget->GetIsEnabled() || !B.Clickable->GetIsEnabled()) { Done(Bad(TEXT("BUTTON_DISABLED"), FString::Printf(TEXT("'%s' is disabled."), *B.Widget->GetName()))); return; }

			TSharedRef<FJsonObject> Clicked = MakeShared<FJsonObject>();
			Clicked->SetStringField(TEXT("name"), B.Widget->GetName());
			Clicked->SetStringField(TEXT("label"), B.Label);
			Clicked->SetStringField(TEXT("method"), Method);
			TWeakObjectPtr<APlayerController> WeakPC = PC;
			auto Finish = [Done, Clicked, WeakPC, Settle]()
			{
				EW::PollUntil(Settle, []() { return false; }, [Done, Clicked, WeakPC]()
				{
					if (!WeakPC.IsValid()) { Done(Bad(TEXT("NO_PLAYER"), TEXT("The player went away after the click."))); return; }
					TSharedRef<FJsonObject> Data = UiState(WeakPC.Get());
					Data->SetObjectField(TEXT("clicked"), Clicked);
					Done(FEWResult::Ok(Data));
				});
			};

			if (Method == TEXT("event"))
			{
				B.Clickable->OnClicked.Broadcast();
				Finish();
				return;
			}

			TSharedPtr<SWidget> Slate = B.Widget->GetCachedWidget();
			const FGeometry G = Slate->GetPaintSpaceGeometry();
			const FVector2D Center = G.GetAbsolutePosition() + G.GetAbsoluteSize() * 0.5f;
			if (UWidget* Over = Blocker(PC->GetWorld(), B.Widget, Center))
			{
				UUserWidget* OverPage = Over->GetTypedOuter<UUserWidget>();
				Done(Bad(TEXT("COVERED"), FString::Printf(TEXT("'%s' is covered by %s (%s%s) at its center. Use method event to fire it anyway."), *B.Widget->GetName(), *Over->GetName(), *Over->GetClass()->GetName(), OverPage ? *(TEXT(" in ") + OverPage->GetClass()->GetName()) : TEXT(""))));
				return;
			}
			// Press the Slate button itself (press, then click: the same path as a pointer or gamepad accept).
			TSharedPtr<SButton> Btn = FindSButton(B.Clickable->GetCachedWidget());
			if (!Btn.IsValid()) { Btn = FindSButton(Slate); }
			if (!Btn.IsValid()) { Done(Bad(TEXT("CLICK_FAILED"), FString::Printf(TEXT("'%s' has no Slate button to press."), *B.Widget->GetName()))); return; }
			Btn->SimulateClick();
			Finish();
		});

	R.RegisterAsync(TEXT("ui_key"),
		TEXT("Send a key or a mouse button to a Play In Editor player's UI through Slate, the same path a keyboard or mouse takes: the key goes to the widget that has user focus (UMG NativeOnKeyDown / OnKeyDown, CommonUI input, text boxes) or, with no focused widget, to the game viewport. Use it for panels that wait for 'press any key' or for a focused widget that ui_click cannot reach. key (e.g. Enter, SpaceBar, E, Escape, Gamepad_FaceButton_Bottom) with event tap (default: press, hold, release), press or release; or mouseButton (Left, Right, Middle) at x, y in viewport pixels. focus first moves user focus to a widget by name or class (wildcards allowed) and fails with FOCUS_FAILED when the widget cannot take focus. Returns what had focus before and after, whether Slate handled the event, and the UI state after settleSeconds. Use pie_input instead for gameplay input (Enhanced Input actions)."),
		TEXT(R"({"type":"object","properties":{
			"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."},
			"key":{"type":"string","description":"Key name, e.g. Enter, SpaceBar, E, Escape, Gamepad_FaceButton_Bottom."},
			"event":{"type":"string","enum":["tap","press","release"],"description":"tap (default): press, hold for seconds, release."},
			"seconds":{"type":"number","minimum":0,"maximum":5,"description":"Hold time for tap, default 0.05. Max 5."},
			"mouseButton":{"type":"string","enum":["Left","Right","Middle"],"description":"Send a mouse button instead of a key. Needs x and y."},
			"x":{"type":"number","description":"Mouse x in viewport pixels (as in ui_state rect)."},
			"y":{"type":"number","description":"Mouse y in viewport pixels."},
			"focus":{"type":"string","description":"Optional: widget name or class (wildcards allowed) to give user focus before the input."},
			"settleSeconds":{"type":"number","description":"Wait after the input before reading the UI, default 0.3."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { Done(Bad(TEXT("NO_PLAYER"), Err)); return; }
			if (!FSlateApplication::IsInitialized()) { Done(Bad(TEXT("NO_SLATE"), TEXT("Slate is not available."))); return; }
			const FString KeyName = EW::Str(Args, TEXT("key"));
			const FString MouseName = EW::Str(Args, TEXT("mouseButton"));
			if (KeyName.IsEmpty() == MouseName.IsEmpty()) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("Pass exactly one of key or mouseButton."))); return; }
			const FString Event = Args->HasField(TEXT("event")) ? EW::Str(Args, TEXT("event")).ToLower() : TEXT("tap");
			if (Event != TEXT("tap") && Event != TEXT("press") && Event != TEXT("release")) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("event must be tap, press or release."))); return; }
			const float Hold = FMath::Clamp((float)EW::Num(Args, TEXT("seconds"), 0.05), 0.f, 5.f);
			const float Settle = FMath::Clamp((float)EW::Num(Args, TEXT("settleSeconds"), 0.3), 0.f, 10.f);
			const int32 UserIndex = (PC->GetLocalPlayer() && PC->GetLocalPlayer()->GetSlateUser().IsValid()) ? PC->GetLocalPlayer()->GetSlateUser()->GetUserIndex() : 0;

			FKey Key;
			if (!KeyName.IsEmpty() && !ParseKeyName(KeyName, Key)) { Done(Bad(TEXT("UNKNOWN_KEY"), FString::Printf(TEXT("No key '%s'."), *KeyName), SuggestKeys(KeyName))); return; }
			FKey Mouse;
			FVector2D MouseAbs = FVector2D::ZeroVector;
			TSharedPtr<SWindow> MouseWindow;
			if (!MouseName.IsEmpty())
			{
				Mouse = MouseName == TEXT("Left") ? EKeys::LeftMouseButton : MouseName == TEXT("Right") ? EKeys::RightMouseButton : EKeys::MiddleMouseButton;
				if (!Args->HasField(TEXT("x")) || !Args->HasField(TEXT("y"))) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("mouseButton needs x and y (viewport pixels)."))); return; }
				FGeometry VP;
				if (!ViewportGeometry(PC, VP)) { Done(Bad(TEXT("NO_VIEWPORT"), TEXT("The player has no game viewport."))); return; }
				MouseAbs = VP.LocalToAbsolute(FVector2D(EW::Num(Args, TEXT("x"), 0), EW::Num(Args, TEXT("y"), 0)));
				UGameViewportClient* GVC = PC->GetWorld()->GetGameViewport();
				MouseWindow = GVC && GVC->GetGameViewportWidget().IsValid() ? FSlateApplication::Get().FindWidgetWindow(GVC->GetGameViewportWidget().ToSharedRef()) : nullptr;
				if (!MouseWindow.IsValid() || !MouseWindow->GetNativeWindow().IsValid()) { Done(Bad(TEXT("NO_VIEWPORT"), TEXT("The game viewport has no window."))); return; }
			}

			// Optional focus move.
			const FString FocusKey = EW::Str(Args, TEXT("focus"));
			if (!FocusKey.IsEmpty())
			{
				TArray<FString> Names;
				UWidget* Target = nullptr;
				for (TObjectIterator<UWidget> It; It; ++It)
				{
					UWidget* W = *It;
					if (!IsValid(W) || W->GetWorld() != PC->GetWorld() || !W->GetCachedWidget().IsValid()) { continue; }
					const FString Cls = W->GetClass()->GetName();
					Names.Add(W->GetName());
					if (!Target && (W->GetName() == FocusKey || Cls == FocusKey || Cls == FocusKey + TEXT("_C") || W->GetName().MatchesWildcard(FocusKey) || Cls.MatchesWildcard(FocusKey))) { Target = W; }
				}
				if (!Target) { Done(Bad(TEXT("WIDGET_NOT_FOUND"), FString::Printf(TEXT("No widget '%s' on player's screen."), *FocusKey), EW::Suggest(FocusKey, Names))); return; }
				Target->SetUserFocus(PC);
				TSharedPtr<SWidget> Now = FSlateApplication::Get().GetUserFocusedWidget(UserIndex);
				bool bInside = false;
				for (TSharedPtr<SWidget> P = Now; P.IsValid(); P = P->GetParentWidget()) { if (P == Target->GetCachedWidget()) { bInside = true; break; } }
				if (!bInside) { Done(Bad(TEXT("FOCUS_FAILED"), FString::Printf(TEXT("%s did not take user focus. A UserWidget needs bIsFocusable (or a focusable child such as a button); the focus is on %s."), *Target->GetName(), *FocusInfo(PC, UserIndex)->GetStringField(TEXT("slateType"))))); return; }
			}

			TSharedRef<FJsonObject> Sent = MakeShared<FJsonObject>();
			Sent->SetObjectField(TEXT("focusBefore"), FocusInfo(PC, UserIndex));
			Sent->SetStringField(TEXT("event"), Event);
			TWeakObjectPtr<APlayerController> WeakPC = PC;

			auto Finish = [Done, Sent, WeakPC, Settle, UserIndex]()
			{
				EW::PollUntil(Settle, []() { return false; }, [Done, Sent, WeakPC, UserIndex]()
				{
					if (!WeakPC.IsValid()) { Done(Bad(TEXT("NO_PLAYER"), TEXT("The player went away after the input."))); return; }
					Sent->SetObjectField(TEXT("focusAfter"), FocusInfo(WeakPC.Get(), UserIndex));
					TSharedRef<FJsonObject> Data = UiState(WeakPC.Get());
					Data->SetObjectField(TEXT("sent"), Sent);
					Done(FEWResult::Ok(Data));
				});
			};

			if (!KeyName.IsEmpty())
			{
				Sent->SetStringField(TEXT("key"), Key.GetFName().ToString());
				const uint32* KeyCode = nullptr; const uint32* CharCode = nullptr;
				FInputKeyManager::Get().GetCodesFromKey(Key, KeyCode, CharCode);
				const uint32 KC = KeyCode ? *KeyCode : 0, CC = CharCode ? *CharCode : 0;
				const FKeyEvent Down(Key, FModifierKeysState(), (uint32)UserIndex, false, CC, KC);
				const FKeyEvent Up(Key, FModifierKeysState(), (uint32)UserIndex, false, CC, KC);
				if (Event != TEXT("release"))
				{
					Sent->SetBoolField(TEXT("handled"), FSlateApplication::Get().ProcessKeyDownEvent(Down));
					if (CC >= 32 && CC != 127) { FSlateApplication::Get().ProcessKeyCharEvent(FCharacterEvent((TCHAR)CC, FModifierKeysState(), (uint32)UserIndex, false)); }
				}
				if (Event == TEXT("press")) { Finish(); return; }
				if (Event == TEXT("release")) { Sent->SetBoolField(TEXT("handled"), FSlateApplication::Get().ProcessKeyUpEvent(Up)); Finish(); return; }
				EW::PollUntil(Hold, []() { return false; }, [Up, Finish]()
				{
					FSlateApplication::Get().ProcessKeyUpEvent(Up);
					Finish();
				});
				return;
			}

			Sent->SetStringField(TEXT("mouseButton"), MouseName);
			Sent->SetNumberField(TEXT("x"), EW::Num(Args, TEXT("x"), 0));
			Sent->SetNumberField(TEXT("y"), EW::Num(Args, TEXT("y"), 0));
			const TSharedPtr<FGenericWindow> Native = MouseWindow->GetNativeWindow();
			const FVector2D AbsPos = MouseAbs;
			FSlateApplication& Slate = FSlateApplication::Get();
			Slate.ProcessMouseMoveEvent(FPointerEvent(0, AbsPos, AbsPos, TSet<FKey>(), FKey(), 0.f, FModifierKeysState()), /*bIsSynthetic*/ true);
			const TSet<FKey> Pressed({ Mouse });
			const FPointerEvent DownEv(0, AbsPos, AbsPos, Pressed, Mouse, 0.f, FModifierKeysState());
			const FPointerEvent UpEv(0, AbsPos, AbsPos, TSet<FKey>(), Mouse, 0.f, FModifierKeysState());
			if (Event != TEXT("release")) { Sent->SetBoolField(TEXT("handled"), Slate.ProcessMouseButtonDownEvent(Native, DownEv)); }
			if (Event == TEXT("press")) { Finish(); return; }
			if (Event == TEXT("release")) { Sent->SetBoolField(TEXT("handled"), Slate.ProcessMouseButtonUpEvent(UpEv)); Finish(); return; }
			EW::PollUntil(Hold, []() { return false; }, [UpEv, Finish]()
			{
				FSlateApplication::Get().ProcessMouseButtonUpEvent(UpEv);
				Finish();
			});
		});

	R.Register(TEXT("ui_open"),
		TEXT("Open a widget class (Widget Blueprint or C++ UserWidget) on a Play In Editor player's screen: creates it for that player and adds it to the viewport; a CommonUI activatable widget is also activated. With stack (name of a running CommonActivatableWidgetStack / Queue, see ui_state stacks) the activatable widget is pushed onto that stack instead, as the game's own UI flow would do. Use it to test one screen in isolation."),
		TEXT(R"({"type":"object","properties":{
			"class":{"type":"string","description":"Widget Blueprint path or class name."},
			"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."},
			"stack":{"type":"string","description":"Optional: push onto this activatable widget stack (widget name or owner class name)."},
			"zOrder":{"type":"integer","description":"Default 10."}},
			"required":["class"]})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { return Bad(TEXT("NO_PLAYER"), Err); }
			TArray<FString> Sug;
			UClass* C = EW::FindClass(EW::Str(Args, TEXT("class")), Err, &Sug);
			if (!C) { return Bad(TEXT("UNKNOWN_CLASS"), Err, Sug); }
			if (!C->IsChildOf(UUserWidget::StaticClass()) || C->HasAnyClassFlags(CLASS_Abstract)) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("%s is not a concrete UserWidget class."), *C->GetName())); }
			UClass* Act = CommonClass(TEXT("CommonActivatableWidget"));

			const FString StackKey = EW::Str(Args, TEXT("stack"));
			if (!StackKey.IsEmpty())
			{
				if (!Act) { return Bad(TEXT("NO_COMMONUI"), TEXT("stack needs the CommonUI plugin, which is not loaded.")); }
				if (!C->IsChildOf(Act)) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("%s is not a CommonActivatableWidget; only those can be pushed onto a stack."), *C->GetName())); }
				TArray<UWidget*> Matches;
				TArray<FString> Names;
				for (UWidget* S : Containers(PC))
				{
					UUserWidget* Owner = S->GetTypedOuter<UUserWidget>();
					const FString OwnerClass = Owner ? Owner->GetClass()->GetName() : FString();
					Names.Add(S->GetName());
					if (S->GetName() == StackKey || S->GetName().MatchesWildcard(StackKey) || OwnerClass == StackKey || OwnerClass == StackKey + TEXT("_C")) { Matches.Add(S); }
				}
				if (Matches.Num() == 0) { return Bad(TEXT("STACK_NOT_FOUND"), FString::Printf(TEXT("No running activatable widget stack '%s' for this player."), *StackKey), Names.Num() ? EW::Suggest(StackKey, Names) : TArray<FString>{ TEXT("ui_state lists stacks; open the screen that owns the stack first.") }); }
				if (Matches.Num() > 1) { TArray<FString> Which; for (UWidget* S : Matches) { Which.Add(S->GetName()); } return Bad(TEXT("AMBIGUOUS"), FString::Printf(TEXT("%d stacks match '%s'; pass the widget name."), Matches.Num(), *StackKey), Which); }
				UWidget* Stack = Matches[0];
				const int32 Before = StackList(Stack).Num();
				bool bCalled = false;
				UObject* Pushed = CallWithObject(Stack, TEXT("BP_AddWidget"), C, &bCalled);
				if (!bCalled) { return Bad(TEXT("PUSH_FAILED"), FString::Printf(TEXT("%s has no BP_AddWidget (unsupported CommonUI version)."), *Stack->GetClass()->GetName())); }
				const TArray<UObject*> After = StackList(Stack);
				if (!Pushed || !After.Contains(Pushed)) { return Bad(TEXT("PUSH_FAILED"), FString::Printf(TEXT("%s did not take the widget (stack had %d, now %d)."), *Stack->GetName(), Before, After.Num())); }
				TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
				Data->SetStringField(TEXT("name"), Pushed->GetName());
				Data->SetStringField(TEXT("class"), C->GetName());
				Data->SetStringField(TEXT("stack"), Stack->GetName());
				Data->SetNumberField(TEXT("stackSize"), After.Num());
				Data->SetBoolField(TEXT("activated"), CallBool(Pushed, TEXT("IsActivated")));
				return FEWResult::Ok(Data);
			}

			UUserWidget* W = CreateWidget<UUserWidget>(PC, TSubclassOf<UUserWidget>(C));
			if (!W) { return Bad(TEXT("CREATE_FAILED"), TEXT("Could not create the widget.")); }
			W->AddToViewport((int32)EW::Num(Args, TEXT("zOrder"), 10));
			bool bActivated = false;
			if (Act)
			{
				if (W->IsA(Act))
				{
					if (UFunction* F = W->FindFunction(TEXT("ActivateWidget"))) { W->ProcessEvent(F, nullptr); }
					bActivated = CallBool(W, TEXT("IsActivated"));
				}
			}
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("name"), W->GetName());
			Data->SetStringField(TEXT("class"), C->GetName());
			Data->SetBoolField(TEXT("inViewport"), W->IsInViewport());
			Data->SetBoolField(TEXT("activated"), bActivated);
			if (!W->IsInViewport()) { return Bad(TEXT("OPEN_FAILED"), TEXT("The widget was created but is not in the viewport.")); }
			return FEWResult::Ok(Data);
		});

	R.Register(TEXT("ui_close"),
		TEXT("Remove a widget from a Play In Editor player's screen by widget name (as listed by ui_state viewportWidgets) or class name."),
		TEXT(R"({"type":"object","properties":{"widget":{"type":"string","description":"Widget name or class name (wildcards allowed), as listed by ui_state."},"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."}},"required":["widget"]})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { return Bad(TEXT("NO_PLAYER"), Err); }
			const FString Key = EW::Str(Args, TEXT("widget"));
			TArray<FString> Names, Closed;
			for (TObjectIterator<UUserWidget> It; It; ++It)
			{
				UUserWidget* U = *It;
				if (!IsValid(U) || U->GetWorld() != PC->GetWorld() || !U->IsInViewport()) { continue; }
				if (U->GetOwningPlayer() && U->GetOwningPlayer() != PC) { continue; }
				Names.Add(U->GetName());
				if (U->GetName() == Key || U->GetClass()->GetName() == Key || U->GetClass()->GetName() == Key + TEXT("_C"))
				{
					if (UClass* Act = CommonClass(TEXT("CommonActivatableWidget")))
					{
						if (U->IsA(Act)) { if (UFunction* F = U->FindFunction(TEXT("DeactivateWidget"))) { U->ProcessEvent(F, nullptr); } }
					}
					U->RemoveFromParent();
					Closed.Add(U->GetName());
				}
			}
			// Widgets inside an activatable stack: remove them from their stack (the stack deactivates them).
			for (UWidget* S : Containers(PC))
			{
				for (UObject* W : StackList(S))
				{
					if (!W) { continue; }
					Names.Add(W->GetName());
					if (W->GetName() == Key || W->GetClass()->GetName() == Key || W->GetClass()->GetName() == Key + TEXT("_C"))
					{
						CallWithObject(S, TEXT("RemoveWidget"), W);
						// The top widget leaves the list after the stack's transition; it is deactivated right away.
						if (!StackList(S).Contains(W) || !CallBool(W, TEXT("IsActivated"))) { Closed.Add(W->GetName()); }
						else { return Bad(TEXT("CLOSE_FAILED"), FString::Printf(TEXT("%s is still active on %s after RemoveWidget."), *W->GetName(), *S->GetName())); }
					}
				}
			}
			if (Closed.Num() == 0) { return Bad(TEXT("WIDGET_NOT_FOUND"), FString::Printf(TEXT("No widget '%s' in player's viewport."), *Key), EW::Suggest(Key, Names)); }
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			TArray<TSharedPtr<FJsonValue>> J;
			for (const FString& N : Closed) { J.Add(MakeShared<FJsonValueString>(N)); }
			Data->SetArrayField(TEXT("closed"), J);
			return FEWResult::Ok(Data);
		});

	R.Register(TEXT("ui_input_type"),
		TEXT("Read or switch a Play In Editor player's CommonUI input type (MouseAndKeyboard, Gamepad, Touch), as if the player picked up a gamepad. Use it to check gamepad glyphs, focus and button hints without a controller. Needs the CommonUI plugin."),
		TEXT(R"({"type":"object","properties":{
			"player":{"type":"integer","description":"Play In Editor player index (0 = first, default 0)."},
			"type":{"type":"string","enum":["MouseAndKeyboard","Gamepad","Touch"],"description":"Omit to only read the current type."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FString Err;
			APlayerController* PC = EW::PiePlayer((int32)EW::Num(Args, TEXT("player"), 0), Err);
			if (!PC) { return Bad(TEXT("NO_PLAYER"), Err); }
			UObject* Sub = CommonInputSubsystem(PC);
			UEnum* E = CommonInputTypeEnum();
			if (!Sub || !E) { return Bad(TEXT("NO_COMMONUI"), TEXT("The CommonUI plugin (CommonInput subsystem) is not active for this player.")); }
			const FString Before = InputTypeName(PC);
			const FString Want = EW::Str(Args, TEXT("type"));
			if (!Want.IsEmpty())
			{
				const int64 V = E->GetValueByNameString(Want);
				if (V == INDEX_NONE || Want == TEXT("Count") || Want == TEXT("MAX")) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("Unknown input type '%s'."), *Want), { TEXT("MouseAndKeyboard"), TEXT("Gamepad"), TEXT("Touch") }); }
				UFunction* F = Sub->FindFunction(TEXT("SetCurrentInputType"));
				if (!F) { return Bad(TEXT("UNSUPPORTED"), TEXT("CommonInputSubsystem has no SetCurrentInputType.")); }
				FStructOnScope Parms(F);
				for (TFieldIterator<FProperty> It(F); It && It->HasAnyPropertyFlags(CPF_Parm); ++It)
				{
					void* Ptr = It->ContainerPtrToValuePtr<void>(Parms.GetStructMemory());
					if (FEnumProperty* EP = CastField<FEnumProperty>(*It)) { EP->GetUnderlyingProperty()->SetIntPropertyValue(Ptr, V); }
					else if (FNumericProperty* NP = CastField<FNumericProperty>(*It)) { NP->SetIntPropertyValue(Ptr, V); }
					break;
				}
				Sub->ProcessEvent(F, Parms.GetStructMemory());
			}
			const FString Now = InputTypeName(PC);
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("inputType"), Now);
			if (!Want.IsEmpty())
			{
				Data->SetStringField(TEXT("before"), Before);
				if (Now != Want)
				{
					// CommonInput refuses types the platform does not support (see CommonInputSettings / platform traits).
					return Bad(TEXT("NOT_APPLIED"), FString::Printf(TEXT("Input type is still %s after asking for %s; this platform's CommonInput settings may not allow it."), *Now, *Want));
				}
			}
			return FEWResult::Ok(Data);
		});

	R.Register(TEXT("ui_input_audit"),
		TEXT("Check CommonUI input wiring in Widget Blueprints (read-only): CommonButtonBase triggering input actions whose DataTable row does not exist, Enhanced Input actions on buttons while CommonInput's Enhanced Input support is off (they never fire), two buttons in one widget triggering the same action, and a missing CommonUI input data asset (no default click / back action). Returns issues with asset, widget and a fix hint."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"Content folder to scan, default /Game."},
			"maxAssets":{"type":"integer","description":"Default 500."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			UClass* CBB = CommonClass(TEXT("CommonButtonBase"));
			if (!CBB) { return Bad(TEXT("NO_COMMONUI"), TEXT("The CommonUI plugin is not loaded; nothing to check.")); }
			FString Folder = EW::Str(Args, TEXT("path"));
			if (Folder.IsEmpty()) { Folder = TEXT("/Game"); }
			if (!Folder.StartsWith(TEXT("/"))) { return Bad(TEXT("INVALID_ARGUMENT"), TEXT("path must be a content folder like /Game/UI.")); }
			if (!FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get().PathExists(FName(*Folder)))
			{
				return Bad(TEXT("FOLDER_NOT_FOUND"), FString::Printf(TEXT("The content folder '%s' does not exist."), *Folder));
			}
			const int32 MaxAssets = FMath::Clamp((int32)EW::Num(Args, TEXT("maxAssets"), 500), 1, 5000);

			bool bEnhanced = false, bInputData = false;
			if (UClass* SettingsClass = FindObject<UClass>(nullptr, TEXT("/Script/CommonInput.CommonInputSettings")))
			{
				UObject* S = SettingsClass->GetDefaultObject();
				if (const FBoolProperty* P = CastField<FBoolProperty>(SettingsClass->FindPropertyByName(TEXT("bEnableEnhancedInputSupport")))) { bEnhanced = P->GetPropertyValue_InContainer(S); }
				if (const FSoftClassProperty* P = CastField<FSoftClassProperty>(SettingsClass->FindPropertyByName(TEXT("InputData")))) { bInputData = !P->GetPropertyValue_InContainer(S).IsNull(); }
			}

			TArray<TSharedPtr<FJsonValue>> Issues;
			auto Issue = [&Issues](const FString& Code, const FString& Asset, const FString& Widget, const FString& Msg, const FString& Fix)
			{
				TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
				J->SetStringField(TEXT("code"), Code);
				J->SetStringField(TEXT("asset"), Asset);
				if (!Widget.IsEmpty()) { J->SetStringField(TEXT("widget"), Widget); }
				J->SetStringField(TEXT("message"), Msg);
				J->SetStringField(TEXT("fix"), Fix);
				Issues.Add(MakeShared<FJsonValueObject>(J));
			};

			const FStructProperty* RowProp = CastField<FStructProperty>(CBB->FindPropertyByName(TEXT("TriggeringInputAction")));
			const FObjectPropertyBase* IAProp = CastField<FObjectPropertyBase>(CBB->FindPropertyByName(TEXT("TriggeringEnhancedInputAction")));
			int32 Scanned = 0, ButtonCount = 0, WithAction = 0;
			TArray<FString> Assets = EW::FindAssets(Folder, UWidgetBlueprint::StaticClass());
			const bool bTruncated = Assets.Num() > MaxAssets;
			if (bTruncated) { Assets.SetNum(MaxAssets); }
			for (const FString& Path : Assets)
			{
				FString Err;
				UWidgetBlueprint* WBP = EW::LoadAs<UWidgetBlueprint>(Path, Err);
				if (!WBP || !WBP->GeneratedClass) { continue; }
				++Scanned;
				// The button templates placed in this widget, plus the class defaults when the Blueprint itself is a button.
				TArray<TPair<FString, UObject*>> Candidates;
				if (WBP->GeneratedClass->IsChildOf(CBB)) { Candidates.Emplace(TEXT("(class defaults)"), WBP->GeneratedClass->GetDefaultObject()); }
				if (WBP->WidgetTree)
				{
					TArray<UWidget*> All;
					WBP->WidgetTree->GetAllWidgets(All);
					for (UWidget* W : All) { if (W && W->IsA(CBB)) { Candidates.Emplace(W->GetName(), W); } }
				}
				TMap<FString, FString> SeenAction; // action key -> first widget
				for (const TPair<FString, UObject*>& C : Candidates)
				{
					++ButtonCount;
					FString ActionKey;
					if (RowProp && RowProp->Struct == FDataTableRowHandle::StaticStruct())
					{
						const FDataTableRowHandle* H = RowProp->ContainerPtrToValuePtr<FDataTableRowHandle>(C.Value);
						if (!H->RowName.IsNone() || H->DataTable)
						{
							ActionKey = FString::Printf(TEXT("%s:%s"), H->DataTable ? *H->DataTable->GetName() : TEXT("None"), *H->RowName.ToString());
							if (!H->DataTable) { Issue(TEXT("ROW_WITHOUT_TABLE"), Path, C.Key, FString::Printf(TEXT("TriggeringInputAction names row '%s' but no DataTable."), *H->RowName.ToString()), TEXT("Pick the input action DataTable or clear the row.")); }
							else if (H->RowName.IsNone() || !H->DataTable->GetRowMap().Contains(H->RowName))
							{
								TArray<FString> Rows; for (const FName& N : H->DataTable->GetRowNames()) { Rows.Add(N.ToString()); }
								const TArray<FString> Near = EW::Suggest(H->RowName.ToString(), Rows);
								Issue(TEXT("ROW_MISSING"), Path, C.Key, FString::Printf(TEXT("TriggeringInputAction row '%s' is not in %s."), *H->RowName.ToString(), *H->DataTable->GetPathName()),
									Near.Num() ? FString::Printf(TEXT("Did you mean %s?"), *FString::Join(Near, TEXT(", "))) : TEXT("Add the row or pick an existing one."));
							}
						}
					}
					if (IAProp)
					{
						if (UObject* IA = IAProp->GetObjectPropertyValue_InContainer(C.Value))
						{
							ActionKey = IA->GetPathName();
							if (!bEnhanced) { Issue(TEXT("ENHANCED_INPUT_OFF"), Path, C.Key, FString::Printf(TEXT("TriggeringEnhancedInputAction is %s but CommonInput Enhanced Input support is off, so it never triggers."), *IA->GetName()), TEXT("Turn on Project Settings > Common Input > Enable Enhanced Input Support, or use TriggeringInputAction (DataTable row).")); }
						}
					}
					if (ActionKey.IsEmpty()) { continue; }
					++WithAction;
					if (const FString* First = SeenAction.Find(ActionKey))
					{
						if (*First != C.Key) { Issue(TEXT("DUPLICATE_ACTION"), Path, C.Key, FString::Printf(TEXT("Triggers the same input action as %s (%s); only one of them will react."), **First, *ActionKey), TEXT("Give each button its own action, or show only one at a time.")); }
					}
					else { SeenAction.Add(ActionKey, C.Key); }
				}
			}
			if (ButtonCount > 0 && !bInputData) { Issue(TEXT("NO_INPUT_DATA"), TEXT("Project Settings > Common Input"), FString(), TEXT("CommonInputSettings.InputData is not set: CommonUI has no default click / back action, so gamepad accept and back do nothing on CommonUI buttons and pages."), TEXT("Create a CommonUIInputData Blueprint with click and back actions and set it in Project Settings > Common Input > Input Data.")); }

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("path"), Folder);
			Data->SetNumberField(TEXT("widgetBlueprints"), Scanned);
			Data->SetNumberField(TEXT("commonButtons"), ButtonCount);
			Data->SetNumberField(TEXT("buttonsWithAction"), WithAction);
			Data->SetBoolField(TEXT("enhancedInputSupport"), bEnhanced);
			Data->SetBoolField(TEXT("inputDataSet"), bInputData);
			Data->SetBoolField(TEXT("truncated"), bTruncated);
			Data->SetArrayField(TEXT("issues"), Issues);
			Data->SetNumberField(TEXT("issueCount"), Issues.Num());
			return FEWResult::Ok(Data);
		});

	R.RegisterAsync(TEXT("pie_cursor"),
		TEXT("Move a Play In Editor player's mouse cursor onto an actor, a world point or a screen pixel, the way a mouse does (through Slate, so the game viewport and the UI both see it: hover, UMG OnMouseEnter, the cursor position the game reads). actor/class aims at the centre of the actor's visible bounds (offset moves the aim point in world units, e.g. [0,0,50] for the head); location is a world point; screen is [x, y] in viewport pixels. click (Left, Right, Middle) then presses and releases there. Returns the screen point, whether it is on screen, what a click would hit there (the actor under the cursor by the Visibility trace, onTarget when it is the aimed actor), whether a UI widget is under the cursor (uiOver: game clicks would land on the UI), and the cursor position the game itself reads (gameCursor) after settleSeconds. Errors: OFF_SCREEN (behind the camera or outside the view, with the projected point), ACTOR_NOT_FOUND, NO_PLAYER. Use ui_click for buttons by name."),
		TEXT(R"({"type":"object","properties":{
			"actor":{"type":"string","description":"Actor label or name in the play world (wildcards allowed); the first match is used."},
			"class":{"type":"string","description":"Or: the first actor of this class (with actor: the first of this class that matches)."},
			"location":{"type":"array","items":{"type":"number"},"description":"Or: a world point [x, y, z]."},
			"screen":{"type":"array","items":{"type":"number"},"description":"Or: viewport pixels [x, y] (0,0 = top left)."},
			"offset":{"type":"array","items":{"type":"number"},"description":"World offset [x, y, z] added to the actor or location aim point."},
			"click":{"type":"string","enum":["Left","Right","Middle"],"description":"Optional: press and release this mouse button at the cursor."},
			"player":{"type":"integer","minimum":0,"description":"Local player index (0 = first / host). Default 0."},
			"settleSeconds":{"type":"number","minimum":0,"maximum":10,"description":"Hold the cursor there this long (re-sent every frame), then read the result; default 0.2."},
			"realCursor":{"type":"boolean","description":"Also move the real Windows mouse pointer there, so the cursor stays after the call (default false: Slate puts the cursor back where the real mouse is after settleSeconds)."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args, FEWDone Done)
		{
			FString Err;
			const int32 PlayerIndex = (int32)EW::Num(Args, TEXT("player"), 0);
			APlayerController* PC = EW::PiePlayer(PlayerIndex, Err);
			if (!PC) { Done(Bad(TEXT("NO_PLAYER"), Err)); return; }
			const FString Key = EW::Str(Args, TEXT("actor"));
			const FString ClassArg = EW::Str(Args, TEXT("class"));
			const TArray<TSharedPtr<FJsonValue>>* LocArr = nullptr;
			const TArray<TSharedPtr<FJsonValue>>* ScrArr = nullptr;
			const TArray<TSharedPtr<FJsonValue>>* OffArr = nullptr;
			const bool bLoc = Args->TryGetArrayField(TEXT("location"), LocArr);
			const bool bScr = Args->TryGetArrayField(TEXT("screen"), ScrArr);
			const bool bActor = !Key.IsEmpty() || !ClassArg.IsEmpty();
			if ((int32)bActor + (int32)bLoc + (int32)bScr != 1) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("Pass exactly one target: actor/class, location or screen."))); return; }
			if (bLoc && LocArr->Num() != 3) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("location must be [x, y, z]."))); return; }
			if (bScr && ScrArr->Num() != 2) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("screen must be [x, y]."))); return; }
			FVector Offset = FVector::ZeroVector;
			if (Args->TryGetArrayField(TEXT("offset"), OffArr))
			{
				if (bScr) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("offset is in world units; it cannot be used with screen."))); return; }
				if (OffArr->Num() != 3) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("offset must be [x, y, z]."))); return; }
				Offset = FVector((*OffArr)[0]->AsNumber(), (*OffArr)[1]->AsNumber(), (*OffArr)[2]->AsNumber());
			}
			const FString ClickName = EW::Str(Args, TEXT("click"));
			if (!ClickName.IsEmpty() && ClickName != TEXT("Left") && ClickName != TEXT("Right") && ClickName != TEXT("Middle")) { Done(Bad(TEXT("INVALID_ARGUMENT"), TEXT("click must be Left, Right or Middle."))); return; }
			const float Settle = FMath::Clamp((float)EW::Num(Args, TEXT("settleSeconds"), 0.2), 0.f, 10.f);
			const bool bReal = EW::Bool(Args, TEXT("realCursor"), false);

			ULocalPlayer* LP = PC->GetLocalPlayer();
			FViewport* V = LP && LP->ViewportClient ? LP->ViewportClient->Viewport : nullptr;
			FGeometry VPGeo;
			if (!V || !ViewportGeometry(PC, VPGeo)) { Done(Bad(TEXT("NO_VIEWPORT"), TEXT("The player has no game viewport."))); return; }
			const FIntPoint Size = V->GetSizeXY();
			if (Size.X <= 0 || Size.Y <= 0) { Done(Bad(TEXT("NO_VIEWPORT"), TEXT("The game viewport has no size (minimized?)."))); return; }

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			AActor* Target = nullptr;
			FVector2D Pixel;
			if (bScr) { Pixel = FVector2D((*ScrArr)[0]->AsNumber(), (*ScrArr)[1]->AsNumber()); }
			else
			{
				FVector World;
				if (bActor)
				{
					UClass* Class = nullptr;
					if (!ClassArg.IsEmpty())
					{
						TArray<FString> Sug;
						Class = EW::FindClass(ClassArg, Err, &Sug);
						if (!Class) { Done(Bad(TEXT("UNKNOWN_CLASS"), Err, Sug)); return; }
					}
					TArray<FString> Names;
					for (TActorIterator<AActor> It(PC->GetWorld(), Class ? Class : AActor::StaticClass()); It; ++It)
					{
						if (!IsValid(*It)) { continue; }
						if (Names.Num() < 400) { Names.Add(It->GetActorLabel()); }
						if (!Target && (Key.IsEmpty() || It->GetActorLabel() == Key || It->GetName() == Key || It->GetActorLabel().MatchesWildcard(Key) || It->GetName().MatchesWildcard(Key))) { Target = *It; }
					}
					if (!Target) { Done(Bad(TEXT("ACTOR_NOT_FOUND"), FString::Printf(TEXT("No actor matching '%s' in player %d's world."), *(Key.IsEmpty() ? ClassArg : Key), PlayerIndex), EW::Suggest(Key, Names))); return; }
					FVector Origin, Extent;
					Target->GetActorBounds(/*bOnlyColliding*/ false, Origin, Extent, /*bIncludeFromChildActors*/ true);
					World = Extent.IsNearlyZero() ? Target->GetActorLocation() : Origin;
					Data->SetStringField(TEXT("actor"), Target->GetActorLabel());
				}
				else { World = FVector((*LocArr)[0]->AsNumber(), (*LocArr)[1]->AsNumber(), (*LocArr)[2]->AsNumber()); }
				World += Offset;
				Data->SetArrayField(TEXT("aimAt"), { MakeShared<FJsonValueNumber>(FMath::RoundToDouble(World.X)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(World.Y)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(World.Z)) });
				const bool bFront = PC->ProjectWorldLocationToScreen(World, Pixel, /*bPlayerViewportRelative*/ true);
				if (!bFront)
				{
					Done(Bad(TEXT("OFF_SCREEN"), TEXT("The point is behind the camera. Turn the camera or walk closer (pie_walk_to) first.")));
					return;
				}
			}
			Data->SetArrayField(TEXT("screen"), { MakeShared<FJsonValueNumber>(FMath::RoundToDouble(Pixel.X)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(Pixel.Y)) });
			Data->SetArrayField(TEXT("viewport"), { MakeShared<FJsonValueNumber>(Size.X), MakeShared<FJsonValueNumber>(Size.Y) });
			if (Pixel.X < 0 || Pixel.Y < 0 || Pixel.X >= Size.X || Pixel.Y >= Size.Y)
			{
				FEWResult R = Bad(TEXT("OFF_SCREEN"), FString::Printf(TEXT("The point projects to (%.0f, %.0f), outside the %dx%d view."), Pixel.X, Pixel.Y, Size.X, Size.Y));
				R.Data = Data;
				Done(R);
				return;
			}

			// Viewport pixels -> desktop space through the geometry the scene viewport receives mouse events in
			// (cursor pixel = local * scale there). The game's own reading is then checked and corrected once,
			// so DPI scale, tab borders or a stale layout cannot leave the cursor off by an offset.
			UGameViewportClient* GVC = PC->GetWorld()->GetGameViewport();
			FSceneViewport* SV = GVC ? GVC->GetGameViewport() : nullptr;
			TSharedPtr<SWindow> Window = GVC && GVC->GetGameViewportWidget().IsValid() ? FSlateApplication::Get().FindWidgetWindow(GVC->GetGameViewportWidget().ToSharedRef()) : nullptr;
			if (!SV || !Window.IsValid() || !Window->GetNativeWindow().IsValid()) { Done(Bad(TEXT("NO_VIEWPORT"), TEXT("The game viewport has no window."))); return; }
			const FGeometry& G = SV->GetCachedGeometry();
			const float Scale = G.Scale > 0 ? G.Scale : 1.f;
			FVector2D Abs = FVector2D(G.LocalToAbsolute(FVector2D(Pixel.X / Scale, Pixel.Y / Scale)));
			const int32 UserIndex = (LP->GetSlateUser().IsValid()) ? LP->GetSlateUser()->GetUserIndex() : 0;
			FSlateApplication& Slate = FSlateApplication::Get();
			auto MoveTo = [&Slate, UserIndex](const FVector2D& To)
			{
				const FVector2D Prev = Slate.GetCursorPos();
				Slate.ProcessMouseMoveEvent(FPointerEvent(UserIndex, 0, To, Prev, TSet<FKey>(), FKey(), 0.f, FModifierKeysState()), /*bIsSynthetic*/ false);
			};
			MoveTo(Abs);
			for (int32 Try = 0; Try < 2; ++Try)
			{
				FIntPoint Got(-1, -1);
				SV->GetMousePos(Got, /*bLocalPosition*/ true);
				const FVector2D Miss(Pixel.X - Got.X, Pixel.Y - Got.Y);
				if (Got.X < 0 || Got.Y < 0 || Miss.GetAbsMax() <= 1.0) { break; }
				// Desktop units per cursor pixel: the absolute geometry scale over the local one.
				const double Per = G.GetAccumulatedLayoutTransform().GetScale() / Scale;
				Abs += Miss * (Per > 0 ? Per : 1.0);
				MoveTo(Abs);
			}
			if (bReal) { Slate.SetCursorPos(Abs); }
			Data->SetBoolField(TEXT("realCursor"), bReal);

			if (!ClickName.IsEmpty())
			{
				const FKey Mouse = ClickName == TEXT("Left") ? EKeys::LeftMouseButton : ClickName == TEXT("Right") ? EKeys::RightMouseButton : EKeys::MiddleMouseButton;
				const TSet<FKey> Pressed({ Mouse });
				Data->SetBoolField(TEXT("clickHandled"), Slate.ProcessMouseButtonDownEvent(Window->GetNativeWindow(), FPointerEvent(UserIndex, 0, Abs, Abs, Pressed, Mouse, 0.f, FModifierKeysState())));
				Slate.ProcessMouseButtonUpEvent(FPointerEvent(UserIndex, 0, Abs, Abs, TSet<FKey>(), Mouse, 0.f, FModifierKeysState()));
				Data->SetStringField(TEXT("click"), ClickName);
			}

			TWeakObjectPtr<APlayerController> WeakPC = PC;
			TWeakObjectPtr<AActor> WeakTarget = Target;
			TSharedPtr<SWidget> GameVP = GVC ? StaticCastSharedPtr<SWidget>(GVC->GetGameViewportWidget()) : nullptr;
			// Slate re-sends the real mouse position every frame; keep sending ours while the game settles.
			const FVector2D Held = Abs;
			EW::PollUntil(Settle, [Held, UserIndex]()
			{
				FSlateApplication& S = FSlateApplication::Get();
				S.ProcessMouseMoveEvent(FPointerEvent(UserIndex, 0, Held, Held, TSet<FKey>(), FKey(), 0.f, FModifierKeysState()), /*bIsSynthetic*/ false);
				return false;
			}, [Done, Data, WeakPC, WeakTarget, Pixel, Abs, Window, UserIndex, bActor, GameVP]()
			{
				APlayerController* P = WeakPC.Get();
				if (!P) { Done(Bad(TEXT("NO_PLAYER"), TEXT("The player went away."))); return; }
				FHitResult Hit;
				if (P->GetHitResultAtScreenPosition(Pixel, ECC_Visibility, /*bTraceComplex*/ false, Hit) && Hit.GetActor())
				{
					Data->SetStringField(TEXT("hitActor"), Hit.GetActor()->GetActorLabel());
					if (Hit.GetComponent()) { Data->SetStringField(TEXT("hitComponent"), Hit.GetComponent()->GetName()); }
					Data->SetNumberField(TEXT("hitDistance"), FMath::RoundToDouble(Hit.Distance));
				}
				if (bActor)
				{
					const bool bOn = WeakTarget.IsValid() && Hit.GetActor() && (Hit.GetActor() == WeakTarget.Get() || Hit.GetActor()->IsAttachedTo(WeakTarget.Get()) || Hit.GetActor()->GetOwner() == WeakTarget.Get());
					Data->SetBoolField(TEXT("onTarget"), bOn);
				}
				// The top widget under the cursor: anything but the viewport itself means the UI would take the click.
				FString TopType;
				if (Window.IsValid())
				{
					const FWidgetPath Path = FSlateApplication::Get().LocateWindowUnderMouse(Abs, { Window.ToSharedRef() }, false, UserIndex);
					// The path ends at the topmost hit-testable widget; the game viewport itself (or its layer manager)
					// there means no UI is under the cursor.
					if (Path.IsValid())
					{
						const TSharedRef<SWidget> Top = Path.Widgets.Last().Widget;
						const FName T = Top->GetType();
						const bool bViewport = GameVP.IsValid() && &Top.Get() == GameVP.Get();
						if (!bViewport && T != TEXT("SViewport") && T != TEXT("SPIEViewport") && T != TEXT("SGameLayerManager") && T != TEXT("SWindow")) { TopType = Top->GetTypeAsString(); }
					}
				}
				Data->SetBoolField(TEXT("uiOver"), !TopType.IsEmpty());
				if (!TopType.IsEmpty()) { Data->SetStringField(TEXT("uiWidget"), TopType); }
				float MX = 0, MY = 0;
				if (P->GetMousePosition(MX, MY)) { Data->SetArrayField(TEXT("gameCursor"), { MakeShared<FJsonValueNumber>(FMath::RoundToDouble(MX)), MakeShared<FJsonValueNumber>(FMath::RoundToDouble(MY)) }); }
				Data->SetBoolField(TEXT("showMouseCursor"), P->bShowMouseCursor);
				FEWResult R = FEWResult::Ok(Data, FString::Printf(TEXT("Cursor at (%.0f, %.0f)%s."), Pixel.X, Pixel.Y,
					Data->HasField(TEXT("hitActor")) ? *FString::Printf(TEXT(", over %s"), *Data->GetStringField(TEXT("hitActor"))) : TEXT("")));
				if (!Data->HasField(TEXT("gameCursor"))) { R.Warnings.Add(TEXT("The game does not read a cursor position from this viewport (mouse not attached or the viewport has no focus); hover in the game may not react.")); }
				else if (FMath::Abs(MX - Pixel.X) > 3 || FMath::Abs(MY - Pixel.Y) > 3) { R.Warnings.Add(FString::Printf(TEXT("The game reads the cursor at (%.0f, %.0f), not at the aimed (%.0f, %.0f)."), MX, MY, Pixel.X, Pixel.Y)); }
				if (!Data->GetBoolField(TEXT("realCursor"))) { R.Warnings.Add(TEXT("The cursor returns to the real mouse position after this call; use realCursor true (or a longer settleSeconds) when the game must see it longer.")); }
				if (bActor && !Data->GetBoolField(TEXT("onTarget"))) { R.Warnings.Add(TEXT("Something else is under the cursor (see hitActor): the actor is covered, or has no Visibility collision. Use offset to aim at another part.")); }
				Done(R);
			});
		});
}
