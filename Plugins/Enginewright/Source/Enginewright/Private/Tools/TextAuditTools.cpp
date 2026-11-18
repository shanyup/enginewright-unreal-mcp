// UI text overflow audit: for every TextBlock of a screen, the width the text wants against the width its slot gives it, per culture.
#include "EnginewrightTool.h"
#include "EWCommon.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Editor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/TextLocalizationManager.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RenderingThread.h"
#include "Rendering/SlateRenderer.h"
#include "Slate/WidgetRenderer.h"
#include "TextureResource.h"
#include "UObject/UObjectIterator.h"
#include "WidgetBlueprint.h"
#include "Widgets/Text/STextBlock.h"

namespace
{
	FEWResult Bad(const FString& Code, const FString& Msg, const TArray<FString>& Sug = {}) { return FEWResult::Error(Code, Msg, Sug); }

	struct FTextRow
	{
		FString Widget, Name, Text;
		double Desired = 0, Allotted = 0, LongestWord = 0;
		bool bWrap = false, bScaled = false, bOverflow = false;
		FString Verdict, Reason;
	};

	struct FCultureReport
	{
		FString Culture;
		bool bGameLocalization = true;
		int32 Total = 0, Hidden = 0, NotLaidOut = 0;
		TArray<FTextRow> Rows;
		FString Screenshot;
	};

	bool HasScaleAncestor(UWidget* W)
	{
		for (UWidget* P = W ? W->GetParent() : nullptr; P; P = P->GetParent())
		{
			if (Cast<UScaleBox>(P)) { return true; }
		}
		return false;
	}

	void CollectText(UUserWidget* Owner, const FString& Prefix, bool bOuterScaled, int32 Depth, TFunctionRef<void(UTextBlock*, const FString&, bool)> Visit)
	{
		if (!Owner || !Owner->WidgetTree || Depth > 6) { return; }
		TArray<UWidget*> All;
		Owner->WidgetTree->GetAllWidgets(All);
		for (UWidget* W : All)
		{
			if (UTextBlock* TB = Cast<UTextBlock>(W)) { Visit(TB, Prefix, bOuterScaled || HasScaleAncestor(TB)); }
			else if (UUserWidget* Child = Cast<UUserWidget>(W)) { CollectText(Child, Prefix + Child->GetName() + TEXT("/"), bOuterScaled || HasScaleAncestor(Child), Depth + 1, Visit); }
		}
	}

	double LineWidth(const TSharedRef<FSlateFontMeasure>& Svc, const FString& Line, const FSlateFontInfo& Font)
	{
		return Line.IsEmpty() ? 0.0 : (double)Svc->Measure(FStringView(Line), Font).X;
	}

	/** Measures one TextBlock after a layout pass. Returns false when it is hidden or has no layout (counted, not listed). */
	bool MeasureOne(UTextBlock* TB, const FString& Prefix, bool bScaled, double Tol, FCultureReport& Rep, FTextRow& Out)
	{
		++Rep.Total;
		if (!TB->IsVisible()) { ++Rep.Hidden; return false; }
		const TSharedPtr<SWidget> SW = TB->GetCachedWidget();
		const double Allotted = (double)TB->GetCachedGeometry().GetLocalSize().X;
		if (!SW.IsValid() || Allotted < 1.0) { ++Rep.NotLaidOut; return false; }

		Out.Widget = Prefix + TB->GetName();
		Out.Name = TB->GetName();
		Out.Text = TB->GetText().ToString();
		Out.Allotted = Allotted;
		Out.bScaled = bScaled;
		const bool bAuto = TB->GetAutoWrapText();
		const double WrapAt = TB->GetWrapTextAt();
		Out.bWrap = bAuto || WrapAt > 0.0;

		const TSharedRef<FSlateFontMeasure> Svc = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		FString Str = Out.Text;
		switch (TB->GetTextTransformPolicy())
		{
		case ETextTransformPolicy::ToUpper: Str = Str.ToUpper(); break;
		case ETextTransformPolicy::ToLower: Str = Str.ToLower(); break;
		default: break;
		}
		TArray<FString> Lines;
		Str.ParseIntoArrayLines(Lines, false);
		double Widest = 0.0, Word = 0.0;
		for (const FString& L : Lines)
		{
			Widest = FMath::Max(Widest, LineWidth(Svc, L, TB->GetFont()));
			TArray<FString> Words;
			L.ParseIntoArrayWS(Words);
			for (const FString& Wd : Words) { Word = FMath::Max(Word, LineWidth(Svc, Wd, TB->GetFont())); }
		}
		// Without wrapping, Slate's own desired width is the truth (it includes transform, letter spacing, margin).
		Out.Desired = Out.bWrap ? Widest : FMath::Max(Widest, (double)SW->GetDesiredSize().X);
		Out.LongestWord = Word;

		if (Out.Desired <= Allotted + Tol) { Out.Verdict = TEXT("fits"); return true; }
		if (bScaled)
		{
			// A ScaleBox gives its child the child's own desired size and shrinks the result: the text is not cut off.
			Out.Verdict = TEXT("scaled");
			Out.Reason = TEXT("inside a ScaleBox: the text is scaled to fit, not cut off");
			return true;
		}
		if (Out.bWrap)
		{
			const double WrapWidth = bAuto ? (WrapAt > 0.0 ? FMath::Min(WrapAt, Allotted) : Allotted) : WrapAt;
			if (Word > Allotted + Tol)
			{
				Out.bOverflow = true;
				Out.Verdict = TEXT("overflow");
				Out.Reason = FString::Printf(TEXT("the longest word is %.0f px wide, the slot gives %.0f px (wrapping cannot break a word)"), Word, Allotted);
			}
			else if (WrapWidth > Allotted + Tol)
			{
				Out.bOverflow = true;
				Out.Verdict = TEXT("overflow");
				Out.Reason = FString::Printf(TEXT("WrapTextAt %.0f px is wider than the slot (%.0f px); turn on Auto Wrap Text or lower it"), WrapWidth, Allotted);
			}
			else
			{
				Out.Verdict = TEXT("wraps");
				Out.Reason = TEXT("wraps onto more lines inside the slot");
			}
			return true;
		}
		Out.bOverflow = true;
		Out.Verdict = TEXT("overflow");
		Out.Reason = FString::Printf(TEXT("the text wants %.0f px, the slot gives %.0f px and the TextBlock does not wrap"), Out.Desired, Allotted);
		return true;
	}

	TSharedRef<FJsonObject> RowJson(const FTextRow& R)
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		J->SetStringField(TEXT("widget"), R.Widget);
		J->SetStringField(TEXT("text"), R.Text.Len() > 80 ? R.Text.Left(80) + TEXT("...") : R.Text);
		J->SetNumberField(TEXT("desiredWidth"), FMath::RoundToDouble(R.Desired * 10) / 10);
		J->SetNumberField(TEXT("allottedWidth"), FMath::RoundToDouble(R.Allotted * 10) / 10);
		J->SetNumberField(TEXT("overBy"), FMath::RoundToDouble(FMath::Max(0.0, R.Desired - R.Allotted) * 10) / 10);
		J->SetBoolField(TEXT("wrap"), R.bWrap);
		J->SetBoolField(TEXT("scaled"), R.bScaled);
		J->SetStringField(TEXT("verdict"), R.Verdict);
		if (!R.Reason.IsEmpty()) { J->SetStringField(TEXT("reason"), R.Reason); }
		return J;
	}

	TSharedRef<FJsonObject> ReportJson(const FCultureReport& C, bool bAll)
	{
		TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
		J->SetStringField(TEXT("culture"), C.Culture);
		J->SetBoolField(TEXT("hasGameLocalization"), C.bGameLocalization);
		int32 Over = 0, Wraps = 0, Scaled = 0, Fits = 0;
		TArray<TSharedPtr<FJsonValue>> Overflows, AllRows;
		for (const FTextRow& R : C.Rows)
		{
			if (R.bOverflow) { ++Over; Overflows.Add(MakeShared<FJsonValueObject>(RowJson(R))); }
			else if (R.Verdict == TEXT("wraps")) { ++Wraps; }
			else if (R.Verdict == TEXT("scaled")) { ++Scaled; }
			else { ++Fits; }
			if (bAll) { AllRows.Add(MakeShared<FJsonValueObject>(RowJson(R))); }
		}
		J->SetNumberField(TEXT("textBlocks"), C.Total);
		J->SetNumberField(TEXT("measured"), C.Rows.Num());
		J->SetNumberField(TEXT("fit"), Fits);
		J->SetNumberField(TEXT("wrapped"), Wraps);
		J->SetNumberField(TEXT("scaled"), Scaled);
		J->SetNumberField(TEXT("overflowCount"), Over);
		J->SetNumberField(TEXT("hidden"), C.Hidden);
		J->SetNumberField(TEXT("notLaidOut"), C.NotLaidOut);
		J->SetArrayField(TEXT("overflows"), Overflows);
		if (bAll) { J->SetArrayField(TEXT("all"), AllRows); }
		if (!C.Screenshot.IsEmpty()) { J->SetStringField(TEXT("screenshot"), C.Screenshot); }
		return J;
	}
}

void RegisterTextAuditTools(FEWRegistry& R)
{
	R.Register(TEXT("text_overflow_audit"),
		TEXT("Find UI text that does not fit its slot. For each visible TextBlock of a Widget Blueprint (or C++ UserWidget class) it lays the screen out at a given size and reports the width the text wants against the width its slot gives it, once per culture, and lists the overflows (widget, text, desired vs allotted width, why). ")
		TEXT("No Play In Editor needed. Without path it audits the UserWidgets that are open in the viewport during Play In Editor, in the current language only. Wrapping text is an overflow only when one word is wider than the slot or WrapTextAt is wider than the slot (Auto Wrap Text and ScaleBox are not false alarms); text inside a ScaleBox is reported as scaled. ")
		TEXT("Translated text comes from the game localization data: a culture without it keeps the source text (the reply says so). The editor language is restored afterwards. Set screenshots:true to save one PNG per culture. Search words: text overflow, truncated text, localization, translation too long, layout."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"Widget Blueprint path (/Game/UI/WBP_Menu) or a UserWidget class (/Script/MyGame.MyMenuPage, short name, Blueprint class path). Omit while Play In Editor runs to audit the open UI."},
			"cultures":{"type":"array","items":{"type":"string"},"description":"Languages to check, e.g. [\"en\",\"de\",\"ru\"] (at most 8). Default: the current editor language. Not allowed without path."},
			"width":{"type":"integer","description":"Screen width in px (default 1920)."},"height":{"type":"integer","description":"Screen height in px (default 1080)."},
			"tolerance":{"type":"number","description":"Pixels of slack before text counts as too wide (default 1)."},
			"includeFits":{"type":"boolean","description":"List every measured TextBlock, not only the overflows (default false)."},
			"screenshots":{"type":"boolean","description":"Save a PNG of the screen for each culture under Saved/Enginewright/Screenshots (default false; not with Play In Editor mode)."},
			"nativeInit":{"type":"boolean","description":"Run NativeOnInitialized of C++ widgets whose tree is empty (default true), like widget_render."}}})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			const FString Path = EW::Str(Args, TEXT("path")).TrimStartAndEnd();
			const int32 W = (int32)EW::Num(Args, TEXT("width"), 1920), H = (int32)EW::Num(Args, TEXT("height"), 1080);
			if (W < 16 || H < 16 || W > 4096 || H > 4096) { return Bad(TEXT("INVALID_ARGUMENT"), TEXT("width and height must be 16 to 4096.")); }
			const double Tol = FMath::Max(0.0, EW::Num(Args, TEXT("tolerance"), 1.0));
			const bool bAll = EW::Bool(Args, TEXT("includeFits"), false);
			const bool bShots = EW::Bool(Args, TEXT("screenshots"), false);
			TArray<FString> Cultures = EW::StrList(Args, TEXT("cultures"));
			if (Cultures.Num() > 8) { return Bad(TEXT("INVALID_ARGUMENT"), TEXT("At most 8 cultures per call.")); }
			FInternationalization& I18N = FInternationalization::Get();
			const FString OrigLanguage = I18N.GetCurrentLanguage()->GetName();
			TArray<FString> GameCultures = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
			TArray<FCultureReport> Reports;
			FString Source, ClassPath;
			TArray<FString> Warnings;

			// ---- open UI (Play In Editor) ----
			if (Path.IsEmpty())
			{
				if (Cultures.Num() > 0) { return Bad(TEXT("INVALID_ARGUMENT"), TEXT("cultures needs a widget path: the open UI is measured in the current language. Pass path, or omit cultures.")); }
				if (bShots) { return Bad(TEXT("INVALID_ARGUMENT"), TEXT("screenshots needs a widget path (the open UI is not redrawn). Use screenshot tools for the live screen.")); }
				UWorld* PW = GEditor ? GEditor->PlayWorld.Get() : nullptr;
				if (!PW) { return Bad(TEXT("NOT_PLAYING"), TEXT("No path given and Play In Editor is not running. Pass a widget path, or start PIE and open the screen first (pie_start, ui_open).")); }
				FCultureReport Rep;
				Rep.Culture = OrigLanguage;
				Rep.bGameLocalization = true;
				int32 Roots = 0;
				for (TObjectIterator<UUserWidget> It; It; ++It)
				{
					UUserWidget* UW = *It;
					if (!UW || UW->GetWorld() != PW || !UW->IsInViewport() || UW->IsTemplate()) { continue; }
					++Roots;
					CollectText(UW, UW->GetName() + TEXT("/"), false, 0, [&](UTextBlock* TB, const FString& Prefix, bool bScaled)
					{
						FTextRow Row;
						if (MeasureOne(TB, Prefix, bScaled, Tol, Rep, Row)) { Rep.Rows.Add(MoveTemp(Row)); }
					});
				}
				if (Roots == 0) { return Bad(TEXT("NO_OPEN_UI"), TEXT("Play In Editor runs but no UserWidget is in the viewport. Open a screen first (ui_open, or the game's own flow).")); }
				Source = TEXT("openUi");
				ClassPath = FString::Printf(TEXT("%d widget(s) in the viewport"), Roots);
				Reports.Add(MoveTemp(Rep));
			}
			else
			{
				// ---- widget asset / class ----
				UClass* Cls = nullptr;
				UWidgetBlueprint* WB = nullptr;
				if (!Path.StartsWith(TEXT("/Script/")) && Path.StartsWith(TEXT("/")))
				{
					FString Ignored;
					WB = EW::LoadAs<UWidgetBlueprint>(Path, Ignored);
				}
				if (WB)
				{
					if (!WB->GeneratedClass || WB->Status == BS_Error) { return Bad(TEXT("COMPILE_ERROR"), TEXT("The Widget Blueprint does not compile.")); }
					Cls = WB->GeneratedClass;
					Source = TEXT("widgetBlueprint");
				}
				else
				{
					FString CErr;
					TArray<FString> Sug;
					Cls = EW::FindClass(Path, CErr, &Sug);
					if (!Cls) { return Bad(TEXT("ASSET_NOT_FOUND"), CErr + TEXT(" text_overflow_audit takes a Widget Blueprint path (/Game/UI/WBP_Menu) or a UserWidget class."), Sug); }
					if (!Cls->IsChildOf(UUserWidget::StaticClass())) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("'%s' is not a UserWidget class."), *Cls->GetPathName())); }
					if (Cls->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists)) { return Bad(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("'%s' is abstract or outdated; audit a concrete subclass."), *Cls->GetPathName())); }
					Source = Cls->ClassGeneratedBy ? TEXT("blueprintClass") : TEXT("nativeClass");
				}
				ClassPath = Cls->GetPathName();
				UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
				if (!World) { return Bad(TEXT("NO_LEVEL"), TEXT("No editor world.")); }
				APlayerController* PC = (GEditor && GEditor->PlayWorld) ? GEditor->PlayWorld->GetFirstPlayerController() : nullptr;

				if (Cultures.Num() == 0) { Cultures.Add(OrigLanguage); }
				for (const FString& C : Cultures)
				{
					if (!I18N.GetCulture(C).IsValid())
					{
						TArray<FString> All;
						I18N.GetCultureNames(All);
						return Bad(TEXT("UNKNOWN_CULTURE"), FString::Printf(TEXT("'%s' is not a known culture name."), *C), EW::Suggest(C, All));
					}
				}
				const FString ShotDir = FPaths::ProjectSavedDir() / TEXT("Enginewright/Screenshots");
				FString Failure;
				for (const FString& C : Cultures)
				{
					if (!I18N.SetCurrentLanguage(C)) { Failure = FString::Printf(TEXT("The engine refused language '%s'."), *C); break; }
					FCultureReport Rep;
					Rep.Culture = I18N.GetCurrentLanguage()->GetName();
					Rep.bGameLocalization = GameCultures.Contains(Rep.Culture) || GameCultures.ContainsByPredicate([&](const FString& G) { return Rep.Culture.StartsWith(G + TEXT("-")) || G.StartsWith(Rep.Culture + TEXT("-")); });
					if (!Rep.bGameLocalization) { Warnings.Add(FString::Printf(TEXT("No compiled game localization for '%s': its text stays in the source language, so the widths equal the source widths."), *Rep.Culture)); }

					UUserWidget* Inst = PC ? CreateWidget<UUserWidget>(PC, TSubclassOf<UUserWidget>(Cls)) : CreateWidget<UUserWidget>(World, TSubclassOf<UUserWidget>(Cls));
					if (!Inst) { Failure = TEXT("Could not create the widget."); break; }
					if (!PC && EW::Bool(Args, TEXT("nativeInit"), true) && Inst->WidgetTree && !Inst->WidgetTree->RootWidget)
					{
						struct FAccess : UUserWidget { static void (UUserWidget::*Fn())() { return &FAccess::NativeOnInitialized; } };
						(Inst->*FAccess::Fn())();
					}
					TSharedRef<SWidget> Slate = Inst->TakeWidget();
					FWidgetRenderer* Renderer = new FWidgetRenderer(/*bUseGammaCorrection*/ true);
					UTextureRenderTarget2D* RT = FWidgetRenderer::CreateTargetFor(FVector2D(W, H), TF_Bilinear, true);
					RT->ClearColor = FLinearColor::Black;
					RT->UpdateResourceImmediate(true);
					Renderer->DrawWidget(RT, Slate, FVector2D(W, H), 0.f);
					Renderer->DrawWidget(RT, Slate, FVector2D(W, H), 0.016f); // second pass: layout settled
					FlushRenderingCommands();

					CollectText(Inst, FString(), false, 0, [&](UTextBlock* TB, const FString& Prefix, bool bScaled)
					{
						FTextRow Row;
						if (MeasureOne(TB, Prefix, bScaled, Tol, Rep, Row)) { Rep.Rows.Add(MoveTemp(Row)); }
					});

					if (bShots)
					{
						TArray<FColor> Pixels;
						FTextureRenderTargetResource* Res = RT->GameThread_GetRenderTargetResource();
						TArray64<uint8> Png;
						if (Res && Res->ReadPixels(Pixels) && Pixels.Num() == W * H)
						{
							for (FColor& Px : Pixels) { Px.A = 255; }
							if (EW::EncodePng(Pixels, W, H, Png))
							{
								FString Name = Cls->GetName();
								Name.RemoveFromEnd(TEXT("_C"));
								const FString File = FPaths::ConvertRelativePathToFull(ShotDir / FString::Printf(TEXT("%s_text_%s_%s.png"), *Name, *Rep.Culture, *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S_%s"))));
								if (FFileHelper::SaveArrayToFile(Png, *File)) { Rep.Screenshot = File; }
							}
						}
						if (Rep.Screenshot.IsEmpty()) { Warnings.Add(FString::Printf(TEXT("Could not save the screenshot for '%s'."), *Rep.Culture)); }
					}
					BeginCleanup(Renderer);
					Inst->MarkAsGarbage();
					Reports.Add(MoveTemp(Rep));
				}
				if (I18N.GetCurrentLanguage()->GetName() != OrigLanguage) { I18N.SetCurrentLanguage(OrigLanguage); }
				if (!Failure.IsEmpty()) { return Bad(TEXT("CAPTURE_FAILED"), Failure + TEXT(" The editor language was restored.")); }
			}

			int32 Total = 0;
			TArray<TSharedPtr<FJsonValue>> Cs;
			TArray<FString> Short;
			for (const FCultureReport& C : Reports)
			{
				int32 Over = 0;
				for (const FTextRow& Row : C.Rows) { if (Row.bOverflow) { ++Over; } }
				Total += Over;
				Short.Add(FString::Printf(TEXT("%s: %d of %d"), *C.Culture, Over, C.Rows.Num()));
				Cs.Add(MakeShared<FJsonValueObject>(ReportJson(C, bAll)));
			}
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("source"), Source);
			Data->SetStringField(TEXT("widget"), ClassPath);
			Data->SetNumberField(TEXT("width"), W);
			Data->SetNumberField(TEXT("height"), H);
			Data->SetNumberField(TEXT("tolerance"), Tol);
			Data->SetArrayField(TEXT("cultures"), Cs);
			Data->SetNumberField(TEXT("overflowTotal"), Total);
			Data->SetStringField(TEXT("verdict"), Total > 0 ? TEXT("overflow") : TEXT("clean"));
			Data->SetStringField(TEXT("summary"), FString::Printf(TEXT("%d overflow(s). Per culture (overflows of measured TextBlocks): %s."), Total, *FString::Join(Short, TEXT("; "))));
			FEWResult Out = FEWResult::Ok(Data);
			Out.Warnings = Warnings;
			return Out;
		});
}
