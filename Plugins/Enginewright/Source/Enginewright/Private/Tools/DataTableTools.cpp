#include "EnginewrightTool.h"
#include "EWCommon.h"
#include "EWProps.h"
#include "EWTypes.h"
#include "StructUtils/UserDefinedStruct.h"

#include "AssetToolsModule.h"
#include "DataTableEditorUtils.h"
#include "Engine/DataTable.h"
#include "Factories/DataTableFactory.h"
#include "IAssetTools.h"
#include "Misc/PackageName.h"
#include "ScopedTransaction.h"
#include "UObject/Package.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/UnrealType.h"

namespace
{
	UDataTable* LoadTable(const TSharedPtr<FJsonObject>& Args, FEWResult& OutErr)
	{
		FString Resolved, Err;
		UObject* Obj = EW::ResolveObject(EW::Str(Args, TEXT("path")), false, Resolved, Err);
		UDataTable* DT = Cast<UDataTable>(Obj);
		if (!DT)
		{
			OutErr = FEWResult::Error(TEXT("TABLE_NOT_FOUND"), Obj ? FString::Printf(TEXT("'%s' is a %s, not a DataTable."), *Resolved, *Obj->GetClass()->GetName()) : Err);
			return nullptr;
		}
		if (!DT->GetRowStruct())
		{
			OutErr = FEWResult::Error(TEXT("TABLE_HAS_NO_STRUCT"), TEXT("The table has no row struct (its C++ struct may be missing)."));
			return nullptr;
		}
		return DT;
	}

	TArray<FString> RowNames(const UDataTable* DT)
	{
		TArray<FString> Out;
		for (const FName& N : DT->GetRowNames()) { Out.Add(N.ToString()); }
		return Out;
	}

	const UScriptStruct* FindRowStruct(const FString& In)
	{
		FString Err;
		return EW::FindStruct(In, Err);
	}

	/** CSV text -> rows of cells. Handles quotes, "" escapes, commas and line breaks inside quotes, a BOM and blank lines. */
	bool ParseCsv(FString Text, TArray<TArray<FString>>& OutRows, FString& OutError)
	{
		if (Text.Len() > 0 && Text[0] == 0xFEFF) { Text.RemoveAt(0); }
		TArray<FString> Row;
		FString Cell;
		bool bQuoted = false, bAnyInRow = false;
		auto EndRow = [&]()
		{
			if (bAnyInRow || Row.Num() > 0 || !Cell.IsEmpty())
			{
				Row.Add(Cell);
				bool bBlank = true;
				for (const FString& C : Row) { if (!C.TrimStartAndEnd().IsEmpty()) { bBlank = false; break; } }
				if (!bBlank) { OutRows.Add(MoveTemp(Row)); }
			}
			Row.Reset(); Cell.Reset(); bAnyInRow = false;
		};
		for (int32 i = 0; i < Text.Len(); ++i)
		{
			const TCHAR C = Text[i];
			if (bQuoted)
			{
				if (C == '"')
				{
					if (i + 1 < Text.Len() && Text[i + 1] == '"') { Cell.AppendChar('"'); ++i; }
					else { bQuoted = false; }
				}
				else { Cell.AppendChar(C); }
				continue;
			}
			if (C == '"' && Cell.IsEmpty()) { bQuoted = true; bAnyInRow = true; }
			else if (C == ',') { Row.Add(Cell); Cell.Reset(); bAnyInRow = true; }
			else if (C == '\r') { /* ignored; \n ends the row */ }
			else if (C == '\n') { EndRow(); }
			else { Cell.AppendChar(C); bAnyInRow = true; }
		}
		if (bQuoted) { OutError = TEXT("The CSV ends inside a quoted cell (a quote is not closed)."); return false; }
		EndRow();
		return true;
	}

	/** Turns one CSV cell into a JSON value for the property. Empty cells of non-text fields are skipped (field keeps its value). */
	TSharedPtr<FJsonValue> CellToJson(const FProperty* P, const FString& Raw, bool& bOutSkip, FString& OutError)
	{
		bOutSkip = false;
		const FString Cell = Raw.TrimStartAndEnd();
		const bool bText = P->IsA<FStrProperty>() || P->IsA<FNameProperty>() || P->IsA<FTextProperty>();
		if (bText) { return MakeShared<FJsonValueString>(Raw); }
		if (Cell.IsEmpty()) { bOutSkip = true; return nullptr; }
		if (Cell[0] == '{' || Cell[0] == '[')
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TEXT("{\"v\":") + Cell + TEXT("}"));
			TSharedPtr<FJsonObject> Wrap;
			if (!FJsonSerializer::Deserialize(Reader, Wrap) || !Wrap.IsValid()) { OutError = TEXT("The cell looks like JSON but does not parse."); return nullptr; }
			return Wrap->TryGetField(TEXT("v"));
		}
		if (P->IsA<FBoolProperty>())
		{
			if (Cell.Equals(TEXT("true"), ESearchCase::IgnoreCase) || Cell == TEXT("1") || Cell.Equals(TEXT("yes"), ESearchCase::IgnoreCase)) { return MakeShared<FJsonValueBoolean>(true); }
			if (Cell.Equals(TEXT("false"), ESearchCase::IgnoreCase) || Cell == TEXT("0") || Cell.Equals(TEXT("no"), ESearchCase::IgnoreCase)) { return MakeShared<FJsonValueBoolean>(false); }
			OutError = FString::Printf(TEXT("'%s' is not a bool (use true or false)."), *Cell);
			return nullptr;
		}
		const FByteProperty* ByteP = CastField<FByteProperty>(P);
		const bool bEnum = P->IsA<FEnumProperty>() || (ByteP && ByteP->Enum);
		if (!bEnum && P->IsA<FNumericProperty>())
		{
			if (!Cell.IsNumeric()) { OutError = FString::Printf(TEXT("'%s' is not a number."), *Cell); return nullptr; }
			return MakeShared<FJsonValueNumber>(FCString::Atod(*Cell));
		}
		return MakeShared<FJsonValueString>(Cell);
	}

	/** The file must sit inside the project folder. Returns the full path or sets OutError. */
	FString ProjectFile(const FString& In, FString& OutError)
	{
		const FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
		FString Full = FPaths::IsRelative(In) ? FPaths::ConvertRelativePathToFull(ProjectDir, In) : FPaths::ConvertRelativePathToFull(In);
		FPaths::NormalizeFilename(Full);
		FPaths::CollapseRelativeDirectories(Full);
		if (!FPaths::IsUnderDirectory(Full, ProjectDir))
		{
			OutError = FString::Printf(TEXT("'%s' is outside the project folder. Put the file under %s (for example Saved/Enginewright/import/)."), *In, *ProjectDir);
			return FString();
		}
		return Full;
	}

	struct FImportRecord
	{
		FString Row;
		TSharedPtr<FJsonObject> Values = MakeShared<FJsonObject>();
	};

	TSharedRef<FJsonValueObject> ChangeJson(const TSharedPtr<FJsonValue>& Before, const TSharedPtr<FJsonValue>& After)
	{
		TSharedRef<FJsonObject> C = MakeShared<FJsonObject>();
		C->SetField(TEXT("before"), Before);
		C->SetField(TEXT("after"), After);
		return MakeShared<FJsonValueObject>(C);
	}

	FEWResult SaveTable(UDataTable* DT, bool bSave, FEWResult Res)
	{
		DT->MarkPackageDirty();
		bool bDeferred = false, bSaved = false;
		if (bSave)
		{
			bSaved = EW::SaveOrDefer(DT->GetOutermost(), bDeferred);
			if (!bSaved) { Res.Warnings.Add(TEXT("Save failed; the change is in memory only.")); }
		}
		if (!Res.Data.IsValid()) { Res.Data = MakeShared<FJsonObject>(); }
		Res.Data->SetBoolField(TEXT("saved"), bSaved && !bDeferred);
		return Res;
	}
}

void RegisterDataTableTools(FEWRegistry& R)
{
	R.Register(TEXT("datatable_create"),
		TEXT("Create a DataTable for a row struct (a C++ struct derived from FTableRowBase, a Blueprint struct asset path, or /Script/Module.Struct)."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"New asset path, e.g. /Game/Data/DT_Items."},
			"rowStruct":{"type":"string","description":"ItemDefinition, FItemDefinition or /Script/MyGame.ItemDefinition."}},
			"required":["path","rowStruct"]})"),
		true,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			const FString Path = EW::Str(Args, TEXT("path"));
			const UScriptStruct* RS = FindRowStruct(EW::Str(Args, TEXT("rowStruct")));
			if (!RS || !(RS->IsChildOf(FTableRowBase::StaticStruct()) || RS->IsA<UUserDefinedStruct>()))
			{
				return FEWResult::Error(TEXT("UNKNOWN_ROW_STRUCT"), FString::Printf(TEXT("'%s' is not a Blueprint struct or a C++ struct derived from FTableRowBase."), *EW::Str(Args, TEXT("rowStruct"))));
			}
			if (FPackageName::DoesPackageExist(Path)) { return FEWResult::Error(TEXT("ASSET_EXISTS"), FString::Printf(TEXT("'%s' already exists."), *Path)); }
			UDataTableFactory* Factory = NewObject<UDataTableFactory>();
			Factory->Struct = const_cast<UScriptStruct*>(RS);
			IAssetTools& AT = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get();
			UObject* Asset = AT.CreateAsset(FPackageName::GetShortName(Path), FPackageName::GetLongPackagePath(Path), UDataTable::StaticClass(), Factory);
			UDataTable* DT = Cast<UDataTable>(Asset);
			if (!DT) { return FEWResult::Error(TEXT("CREATE_FAILED"), TEXT("The engine could not create the table (check the folder path).")); }
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("path"), DT->GetOutermost()->GetName());
			Data->SetStringField(TEXT("rowStruct"), RS->GetPathName());
			return SaveTable(DT, true, FEWResult::Ok(Data));
		}, /*bTransactional*/ false);

	R.Register(TEXT("datatable_get"),
		TEXT("Read a DataTable: its row struct, the row names, and the values of the rows asked for (or all rows when rows is omitted)."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"DataTable asset path, e.g. /Game/Data/DT_Items."},
			"rows":{"type":"array","items":{"type":"string"},"description":"Row names to read. Omit for all."},
			"fields":{"type":"array","items":{"type":"string"},"description":"Field names to read. Omit for all."},
			"namesOnly":{"type":"boolean","description":"Return only the row names and the field list."}},
			"required":["path"]})"),
		false,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FEWResult Err;
			UDataTable* DT = LoadTable(Args, Err);
			if (!DT) { return Err; }
			const UScriptStruct* RS = DT->GetRowStruct();

			TArray<FString> Fields, Wanted;
			const TArray<TSharedPtr<FJsonValue>>* List = nullptr;
			if (Args->TryGetArrayField(TEXT("fields"), List)) { for (const auto& V : *List) { Fields.Add(V->AsString()); } }
			if (Args->TryGetArrayField(TEXT("rows"), List)) { for (const auto& V : *List) { Wanted.Add(V->AsString()); } }
			bool bNamesOnly = false;
			Args->TryGetBoolField(TEXT("namesOnly"), bNamesOnly);

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetStringField(TEXT("rowStruct"), RS->GetPathName());
			TArray<TSharedPtr<FJsonValue>> Names, FieldNames;
			for (const FString& N : RowNames(DT)) { Names.Add(MakeShared<FJsonValueString>(N)); }
			for (const FString& F : EW::PropertyNames(RS)) { FieldNames.Add(MakeShared<FJsonValueString>(F)); }
			Data->SetArrayField(TEXT("rowNames"), Names);
			Data->SetArrayField(TEXT("fields"), FieldNames);
			if (bNamesOnly) { return FEWResult::Ok(Data); }

			if (Wanted.Num() == 0) { Wanted = RowNames(DT); }
			TSharedRef<FJsonObject> Rows = MakeShared<FJsonObject>();
			for (const FString& Row : Wanted)
			{
				const uint8* Mem = DT->FindRowUnchecked(FName(*Row));
				if (!Mem)
				{
					return FEWResult::Error(TEXT("ROW_NOT_FOUND"), FString::Printf(TEXT("No row '%s'."), *Row), EW::Suggest(Row, RowNames(DT)));
				}
				TSharedPtr<FJsonObject> Values;
				FEWResult R2 = EW::ReadValues(RS, Mem, Fields, Values);
				if (!R2.bOk) { return R2; }
				Rows->SetObjectField(Row, Values);
			}
			Data->SetObjectField(TEXT("rows"), Rows);
			return FEWResult::Ok(Data);
		});

	R.Register(TEXT("datatable_set_rows"),
		TEXT("Add or update DataTable rows in one step: {rowName: {field: value}}. Fields are checked like object_set (unknown field, type, enum = error). Partial rows keep their other fields. If any row fails, nothing is written."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"DataTable asset path, e.g. /Game/Data/DT_Items."},
			"rows":{"type":"object","description":"Row name -> object of field values."},
			"mode":{"type":"string","enum":["upsert","add","update"],"description":"upsert (default): add missing rows, update existing; add: fail if a row exists; update: fail if a row is missing."},
			"save":{"type":"boolean","description":"Save afterwards (default true)."}},
			"required":["path","rows"]})"),
		true,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FEWResult Err;
			UDataTable* DT = LoadTable(Args, Err);
			if (!DT) { return Err; }
			const UScriptStruct* RS = DT->GetRowStruct();
			const TSharedPtr<FJsonObject>* Rows = nullptr;
			if (!Args->TryGetObjectField(TEXT("rows"), Rows) || (*Rows)->Values.Num() == 0)
			{
				return FEWResult::Error(TEXT("INVALID_ARGUMENT"), TEXT("rows must be a non-empty object."));
			}
			FString Mode = EW::Str(Args, TEXT("mode"));
			if (Mode.IsEmpty()) { Mode = TEXT("upsert"); }
			bool bSave = true;
			Args->TryGetBoolField(TEXT("save"), bSave);

			for (const auto& Pair : (*Rows)->Values)
			{
				const FString Row(*Pair.Key);
				const bool bExists = DT->FindRowUnchecked(FName(*Row)) != nullptr;
				if (Mode == TEXT("add") && bExists) { return FEWResult::Error(TEXT("ROW_EXISTS"), FString::Printf(TEXT("Row '%s' already exists (mode add). Nothing was written."), *Row)); }
				if (Mode == TEXT("update") && !bExists)
				{
					return FEWResult::Error(TEXT("ROW_NOT_FOUND"), FString::Printf(TEXT("No row '%s' (mode update). Nothing was written."), *Row), EW::Suggest(Row, RowNames(DT)));
				}
				if (Pair.Value->Type != EJson::Object) { return FEWResult::Error(TEXT("INVALID_ARGUMENT"), FString::Printf(TEXT("Row '%s' must be an object of fields."), *Row)); }
			}

			// 1) Validate first: write every row into a temporary buffer and keep it. Nothing touches the table.
			struct FPendingRow
			{
				FString Row;
				uint8* Buf = nullptr;
				FEWResult Result;
			};
			TArray<FPendingRow> Pending;
			Pending.Reserve((*Rows)->Values.Num());
			for (const auto& Pair : (*Rows)->Values)
			{
				FPendingRow P;
				P.Row = FString(*Pair.Key);
				P.Buf = (uint8*)FMemory::Malloc(RS->GetStructureSize(), RS->GetMinAlignment());
				RS->InitializeStruct(P.Buf);
				if (const uint8* Existing = DT->FindRowUnchecked(FName(*P.Row))) { RS->CopyScriptStruct(P.Buf, Existing); }
				P.Result = EW::WriteValues(RS, P.Buf, nullptr, Pair.Value->AsObject());
				if (!P.Result.bOk)
				{
					FEWResult Fail = P.Result;
					for (FPendingRow& X : Pending) { RS->DestroyStruct(X.Buf); FMemory::Free(X.Buf); }
					RS->DestroyStruct(P.Buf);
					FMemory::Free(P.Buf);
					Fail.Message = FString::Printf(TEXT("Row '%s': %s Nothing was written."), *P.Row, *Fail.Message);
					return Fail;
				}
				Pending.Add(MoveTemp(P));
			}

			// 2) Every row passed: write them now in one transaction.
			FScopedTransaction Tx(FText::FromString(TEXT("Enginewright datatable_set_rows")));
			DT->Modify();
			FDataTableEditorUtils::BroadcastPreChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowList);

			FEWResult Res = FEWResult::Ok();
			TSharedRef<FJsonObject> AllChanges = MakeShared<FJsonObject>();
			TArray<TSharedPtr<FJsonValue>> Added;
			for (FPendingRow& P : Pending)
			{
				const FName Row(*P.Row);
				uint8* Mem = DT->FindRowUnchecked(Row);
				if (!Mem)
				{
					Mem = FDataTableEditorUtils::AddRow(DT, Row);
					Added.Add(MakeShared<FJsonValueString>(Row.ToString()));
				}
				if (!Mem)
				{
					for (FPendingRow& X : Pending) { RS->DestroyStruct(X.Buf); FMemory::Free(X.Buf); }
					FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowList);
					EW::CancelUnlessBatch(Tx);
					return FEWResult::Error(TEXT("ADD_ROW_FAILED"), FString::Printf(TEXT("Row '%s': Could not add the row. Nothing was written."), *P.Row));
				}
				RS->CopyScriptStruct(Mem, P.Buf);
				Res.Warnings.Append(P.Result.Warnings);
				AllChanges->SetObjectField(P.Row, P.Result.Data->GetObjectField(TEXT("changes")));
			}
			FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowList);
			for (FPendingRow& P : Pending) { RS->DestroyStruct(P.Buf); FMemory::Free(P.Buf); }

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetObjectField(TEXT("changes"), AllChanges);
			Data->SetArrayField(TEXT("addedRows"), Added);
			Res.Data = Data;
			return SaveTable(DT, bSave, Res);
		});

	R.Register(TEXT("datatable_remove_rows"),
		TEXT("Remove DataTable rows. All names must exist, otherwise nothing is removed."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"DataTable asset path, e.g. /Game/Data/DT_Items."},
			"rows":{"type":"array","items":{"type":"string"},"description":"Row names to remove, e.g. [\"Sword\"]."},
			"save":{"type":"boolean","description":"Save afterwards (default true)."}},
			"required":["path","rows"]})"),
		true,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FEWResult Err;
			UDataTable* DT = LoadTable(Args, Err);
			if (!DT) { return Err; }
			const TArray<TSharedPtr<FJsonValue>>* List = nullptr;
			Args->TryGetArrayField(TEXT("rows"), List);
			for (const auto& V : *List)
			{
				if (!DT->FindRowUnchecked(FName(*V->AsString())))
				{
					return FEWResult::Error(TEXT("ROW_NOT_FOUND"), FString::Printf(TEXT("No row '%s'. Nothing was removed."), *V->AsString()), EW::Suggest(V->AsString(), RowNames(DT)));
				}
			}
			FScopedTransaction Tx(FText::FromString(TEXT("Enginewright datatable_remove_rows")));
			for (const auto& V : *List) { FDataTableEditorUtils::RemoveRow(DT, FName(*V->AsString())); }
			bool bSave = true;
			Args->TryGetBoolField(TEXT("save"), bSave);
			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetNumberField(TEXT("removed"), List->Num());
			return SaveTable(DT, bSave, FEWResult::Ok(Data));
		});

	R.Register(TEXT("datatable_rename_row"),
		TEXT("Rename one DataTable row. Note: other assets that store the old name as an ID are not changed; search them with asset text search first."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"DataTable asset path, e.g. /Game/Data/DT_Items."},
			"from":{"type":"string","description":"Current row name."},
			"to":{"type":"string","description":"New row name."},
			"save":{"type":"boolean","description":"Save afterwards (default true)."}},
			"required":["path","from","to"]})"),
		true,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FEWResult Err;
			UDataTable* DT = LoadTable(Args, Err);
			if (!DT) { return Err; }
			const FString From = EW::Str(Args, TEXT("from")), To = EW::Str(Args, TEXT("to"));
			if (!DT->FindRowUnchecked(FName(*From))) { return FEWResult::Error(TEXT("ROW_NOT_FOUND"), FString::Printf(TEXT("No row '%s'."), *From), EW::Suggest(From, RowNames(DT))); }
			if (DT->FindRowUnchecked(FName(*To))) { return FEWResult::Error(TEXT("ROW_EXISTS"), FString::Printf(TEXT("Row '%s' already exists."), *To)); }
			FScopedTransaction Tx(FText::FromString(TEXT("Enginewright datatable_rename_row")));
			if (!FDataTableEditorUtils::RenameRow(DT, FName(*From), FName(*To))) { EW::CancelUnlessBatch(Tx); return FEWResult::Error(TEXT("RENAME_FAILED"), TEXT("The engine refused the rename.")); }
			bool bSave = true;
			Args->TryGetBoolField(TEXT("save"), bSave);
			return SaveTable(DT, bSave, FEWResult::Ok());
		});

	R.Register(TEXT("datatable_import"),
		TEXT("Import a CSV or JSON file into a DataTable (upsert) and report what changes: added rows, changed rows (field before/after), removed rows (only with deleteMissing), unchanged count. Use dryRun:true first to see the diff without writing. ")
		TEXT("Strict: an unknown column/field, a bad value, a duplicate row name or an empty row rejects the whole file and nothing is written (errors list every problem). Writes are checked like datatable_set_rows and read back after saving. ")
		TEXT("CSV: first line is the header; the row-name column is the first column (or rowNameColumn); other columns are struct fields. Cells of struct/array/map fields hold JSON text; an empty cell of a non-text field keeps the current value (an empty text cell sets empty text). ")
		TEXT("JSON: an object {rowName:{field:value}} or an array of objects that each carry the row name (key Name, RowName, name or id, or rowNameColumn). The file must be inside the project folder. To export first use datatable_get. Search words: csv import, json import, bulk rows, spreadsheet, dry run."),
		TEXT(R"({"type":"object","properties":{
			"path":{"type":"string","description":"DataTable asset path, e.g. /Game/Data/DT_Cards."},
			"file":{"type":"string","description":"CSV or JSON file, relative to the project folder or absolute inside it, e.g. Saved/Enginewright/import/cards.csv."},
			"format":{"type":"string","enum":["auto","csv","json"],"description":"Default auto: by file extension (.json = json, else csv)."},
			"dryRun":{"type":"boolean","description":"True: only report the diff, write nothing (default false)."},
			"deleteMissing":{"type":"boolean","description":"Remove table rows that are not in the file (default false: they stay)."},
			"rowNameColumn":{"type":"string","description":"CSV column or JSON key holding the row name. Default: first CSV column; JSON Name/RowName/name/id."},
			"maxList":{"type":"integer","description":"Most rows listed per group in the reply (default 200); counts are always complete."},
			"save":{"type":"boolean","description":"Save the table afterwards (default true; ignored by dryRun)."}},
			"required":["path","file"]})"),
		true,
		[](const TSharedPtr<FJsonObject>& Args) -> FEWResult
		{
			FEWResult Err;
			UDataTable* DT = LoadTable(Args, Err);
			if (!DT) { return Err; }
			const UScriptStruct* RS = DT->GetRowStruct();
			const bool bDry = EW::Bool(Args, TEXT("dryRun"), false);
			const bool bDelete = EW::Bool(Args, TEXT("deleteMissing"), false);
			const int32 MaxList = FMath::Max(1, (int32)EW::Num(Args, TEXT("maxList"), 200));
			const FString RowCol = EW::Str(Args, TEXT("rowNameColumn")).TrimStartAndEnd();

			FString PathErr;
			const FString File = ProjectFile(EW::Str(Args, TEXT("file")), PathErr);
			if (File.IsEmpty()) { return FEWResult::Error(TEXT("INVALID_PATH"), PathErr); }
			FString Text;
			if (!FFileHelper::LoadFileToString(Text, *File)) { return FEWResult::Error(TEXT("FILE_NOT_FOUND"), FString::Printf(TEXT("Cannot read '%s'."), *File)); }
			FString Format = EW::Str(Args, TEXT("format"));
			if (Format.IsEmpty() || Format == TEXT("auto")) { Format = FPaths::GetExtension(File).Equals(TEXT("json"), ESearchCase::IgnoreCase) ? TEXT("json") : TEXT("csv"); }

			// 1) File -> records. Every problem is collected; one problem rejects the file.
			TArray<FImportRecord> Records;
			TArray<FString> Problems;
			TArray<FString> FieldSuggest;
			TSet<FString> Seen;
			auto AddRecord = [&](FImportRecord&& Rec, int32 Line)
			{
				if (Rec.Row.TrimStartAndEnd().IsEmpty()) { Problems.Add(FString::Printf(TEXT("Entry %d: the row name is empty."), Line)); return; }
				if (Seen.Contains(Rec.Row)) { Problems.Add(FString::Printf(TEXT("Row '%s' appears twice in the file."), *Rec.Row)); return; }
				Seen.Add(Rec.Row);
				Records.Add(MoveTemp(Rec));
			};
			auto CheckField = [&](const FString& Col) -> FProperty*
			{
				FProperty* P = EW::FindPropertyLoose(RS, Col);
				if (!P)
				{
					Problems.Add(FString::Printf(TEXT("Unknown field '%s' (row struct %s)."), *Col, *RS->GetName()));
					for (const FString& S : EW::Suggest(Col, EW::PropertyNames(RS))) { FieldSuggest.AddUnique(S); }
				}
				return P;
			};

			if (Format == TEXT("csv"))
			{
				TArray<TArray<FString>> Rows;
				FString CsvErr;
				if (!ParseCsv(Text, Rows, CsvErr)) { return FEWResult::Error(TEXT("INVALID_FILE"), CsvErr); }
				if (Rows.Num() == 0) { return FEWResult::Error(TEXT("INVALID_FILE"), TEXT("The CSV is empty (it needs a header line).")); }
				const TArray<FString>& Head = Rows[0];
				int32 NameIdx = 0;
				if (!RowCol.IsEmpty())
				{
					NameIdx = Head.IndexOfByPredicate([&](const FString& H) { return H.TrimStartAndEnd().Equals(RowCol, ESearchCase::IgnoreCase); });
					if (NameIdx == INDEX_NONE) { return FEWResult::Error(TEXT("INVALID_FILE"), FString::Printf(TEXT("The CSV has no column '%s'."), *RowCol), EW::Suggest(RowCol, Head)); }
				}
				TArray<FProperty*> Props;
				TSet<FString> Cols;
				for (int32 c = 0; c < Head.Num(); ++c)
				{
					const FString H = Head[c].TrimStartAndEnd();
					if (c == NameIdx) { Props.Add(nullptr); continue; }
					if (H.IsEmpty()) { Problems.Add(FString::Printf(TEXT("Column %d has an empty header."), c + 1)); Props.Add(nullptr); continue; }
					if (Cols.Contains(H.ToLower())) { Problems.Add(FString::Printf(TEXT("Column '%s' appears twice."), *H)); Props.Add(nullptr); continue; }
					Cols.Add(H.ToLower());
					Props.Add(CheckField(H));
				}
				for (int32 r = 1; r < Rows.Num(); ++r)
				{
					if (Rows[r].Num() != Head.Num())
					{
						Problems.Add(FString::Printf(TEXT("CSV line %d has %d cells, the header has %d."), r + 1, Rows[r].Num(), Head.Num()));
						continue;
					}
					FImportRecord Rec;
					Rec.Row = Rows[r][NameIdx].TrimStartAndEnd();
					for (int32 c = 0; c < Head.Num(); ++c)
					{
						if (c == NameIdx || !Props[c]) { continue; }
						bool bSkip = false;
						FString CellErr;
						TSharedPtr<FJsonValue> V = CellToJson(Props[c], Rows[r][c], bSkip, CellErr);
						if (!CellErr.IsEmpty()) { Problems.Add(FString::Printf(TEXT("Row '%s', field '%s': %s"), *Rec.Row, *Head[c].TrimStartAndEnd(), *CellErr)); continue; }
						if (!bSkip) { Rec.Values->SetField(Head[c].TrimStartAndEnd(), V); }
					}
					AddRecord(MoveTemp(Rec), r + 1);
				}
			}
			else
			{
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Text);
				TSharedPtr<FJsonValue> Root;
				if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) { return FEWResult::Error(TEXT("INVALID_FILE"), TEXT("The file is not valid JSON.")); }
				auto TakeFields = [&](FImportRecord& Rec, const TSharedPtr<FJsonObject>& O, const FString& SkipKey)
				{
					for (const auto& Pair : O->Values)
					{
						const FString Key(*Pair.Key);
						if (!SkipKey.IsEmpty() && Key.Equals(SkipKey, ESearchCase::IgnoreCase)) { continue; }
						if (CheckField(Key)) { Rec.Values->SetField(Key, Pair.Value); }
					}
				};
				if (Root->Type == EJson::Array)
				{
					int32 i = 0;
					for (const TSharedPtr<FJsonValue>& V : Root->AsArray())
					{
						++i;
						const TSharedPtr<FJsonObject>* O = nullptr;
						if (!V->TryGetObject(O)) { Problems.Add(FString::Printf(TEXT("Entry %d is not an object."), i)); continue; }
						FString Key = RowCol;
						if (Key.IsEmpty())
						{
							for (const TCHAR* Cand : { TEXT("Name"), TEXT("RowName"), TEXT("name"), TEXT("id") })
							{
								if ((*O)->HasField(Cand)) { Key = Cand; break; }
							}
						}
						FImportRecord Rec;
						if (Key.IsEmpty() || !(*O)->TryGetStringField(Key, Rec.Row)) { Problems.Add(FString::Printf(TEXT("Entry %d has no row name (key Name, RowName, name, id or rowNameColumn)."), i)); continue; }
						TakeFields(Rec, *O, Key);
						AddRecord(MoveTemp(Rec), i);
					}
				}
				else if (Root->Type == EJson::Object)
				{
					int32 i = 0;
					for (const auto& Pair : Root->AsObject()->Values)
					{
						++i;
						const TSharedPtr<FJsonObject>* O = nullptr;
						if (!Pair.Value->TryGetObject(O)) { Problems.Add(FString::Printf(TEXT("Row '%s' is not an object of fields."), *FString(*Pair.Key))); continue; }
						FImportRecord Rec;
						Rec.Row = FString(*Pair.Key);
						TakeFields(Rec, *O, FString());
						AddRecord(MoveTemp(Rec), i);
					}
				}
				else { return FEWResult::Error(TEXT("INVALID_FILE"), TEXT("The JSON must be an object {row:{field:value}} or an array of row objects.")); }
			}
			if (Records.Num() == 0 && Problems.Num() == 0) { Problems.Add(TEXT("The file has no rows.")); }
			for (const FImportRecord& Rec : Records)
			{
				if (Rec.Values->Values.Num() == 0) { Problems.Add(FString::Printf(TEXT("Row '%s' has no field values."), *Rec.Row)); }
			}

			// 2) Diff: write each row into a scratch copy of the existing row (or a default row) and compare.
			TArray<TSharedPtr<FJsonValue>> Added, Changed;
			TSharedRef<FJsonObject> ChangedRows = MakeShared<FJsonObject>(); // only rows that change, for the real write
			int32 AddedCount = 0, ChangedCount = 0, Unchanged = 0;
			if (Problems.Num() == 0)
			{
				for (const FImportRecord& Rec : Records)
				{
					uint8* Buf = (uint8*)FMemory::Malloc(RS->GetStructureSize(), RS->GetMinAlignment());
					RS->InitializeStruct(Buf);
					const uint8* Existing = DT->FindRowUnchecked(FName(*Rec.Row));
					if (Existing) { RS->CopyScriptStruct(Buf, Existing); }
					FEWResult W = EW::WriteValues(RS, Buf, nullptr, Rec.Values);
					RS->DestroyStruct(Buf);
					FMemory::Free(Buf);
					if (!W.bOk)
					{
						Problems.Add(FString::Printf(TEXT("Row '%s': %s"), *Rec.Row, *W.Message));
						for (const FString& S : W.Suggestions) { FieldSuggest.AddUnique(S); }
						continue;
					}
					TSharedRef<FJsonObject> RowChanges = MakeShared<FJsonObject>();
					const TSharedPtr<FJsonObject>* Ch = nullptr;
					if (W.Data.IsValid() && W.Data->TryGetObjectField(TEXT("changes"), Ch))
					{
						for (const auto& F : (*Ch)->Values)
						{
							const TSharedPtr<FJsonObject>* One = nullptr;
							if (!F.Value->TryGetObject(One)) { continue; }
							const TSharedPtr<FJsonValue> B = (*One)->TryGetField(TEXT("before")), A = (*One)->TryGetField(TEXT("after"));
							const bool bSame = B.IsValid() && A.IsValid() && FJsonValue::CompareEqual(*B, *A);
							if (!bSame || !Existing) { RowChanges->SetField(F.Key, ChangeJson(B, A)); }
						}
					}
					if (!Existing)
					{
						++AddedCount;
						ChangedRows->SetObjectField(Rec.Row, Rec.Values);
						if (Added.Num() < MaxList) { Added.Add(MakeShared<FJsonValueString>(Rec.Row)); }
					}
					else if (RowChanges->Values.Num() > 0)
					{
						++ChangedCount;
						ChangedRows->SetObjectField(Rec.Row, Rec.Values);
						if (Changed.Num() < MaxList)
						{
							TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
							J->SetStringField(TEXT("row"), Rec.Row);
							J->SetObjectField(TEXT("fields"), RowChanges);
							Changed.Add(MakeShared<FJsonValueObject>(J));
						}
					}
					else { ++Unchanged; }
				}
			}
			if (Problems.Num() > 0)
			{
				TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
				TArray<TSharedPtr<FJsonValue>> List;
				for (int32 i = 0; i < Problems.Num() && i < 100; ++i) { List.Add(MakeShared<FJsonValueString>(Problems[i])); }
				Data->SetArrayField(TEXT("errors"), List);
				Data->SetNumberField(TEXT("errorCount"), Problems.Num());
				TArray<FString> Sug = FieldSuggest;
				if (Sug.Num() > 3) { Sug.SetNum(3); }
				FEWResult Fail = FEWResult::Error(TEXT("IMPORT_INVALID"), FString::Printf(TEXT("%d problem(s) in the file; nothing was written. First: %s"), Problems.Num(), *Problems[0]), Sug);
				Fail.Data = Data;
				return Fail;
			}

			TArray<FString> Removed;
			if (bDelete)
			{
				for (const FString& N : RowNames(DT)) { if (!Seen.Contains(N)) { Removed.Add(N); } }
			}

			TSharedRef<FJsonObject> Data = MakeShared<FJsonObject>();
			Data->SetBoolField(TEXT("dryRun"), bDry);
			Data->SetStringField(TEXT("table"), DT->GetOutermost()->GetName());
			Data->SetStringField(TEXT("file"), File);
			Data->SetStringField(TEXT("format"), Format);
			Data->SetNumberField(TEXT("fileRows"), Records.Num());
			Data->SetNumberField(TEXT("addedCount"), AddedCount);
			Data->SetNumberField(TEXT("changedCount"), ChangedCount);
			Data->SetNumberField(TEXT("removedCount"), Removed.Num());
			Data->SetNumberField(TEXT("unchangedCount"), Unchanged);
			TArray<TSharedPtr<FJsonValue>> RemovedJson;
			for (int32 i = 0; i < Removed.Num() && i < MaxList; ++i) { RemovedJson.Add(MakeShared<FJsonValueString>(Removed[i])); }
			Data->SetArrayField(TEXT("added"), Added);
			Data->SetArrayField(TEXT("changed"), Changed);
			Data->SetArrayField(TEXT("removed"), RemovedJson);
			Data->SetStringField(TEXT("summary"), FString::Printf(TEXT("%s%d added, %d changed, %d removed, %d unchanged."), bDry ? TEXT("dryRun: ") : TEXT(""), AddedCount, ChangedCount, Removed.Num(), Unchanged));

			if (bDry) { return FEWResult::Ok(Data); }

			// 3) Apply with the existing write tools (all-or-nothing, undo step), then read back.
			FEWRegistry& Reg = FEWRegistry::Get();
			FEWResult Res = FEWResult::Ok();
			if (ChangedRows->Values.Num() > 0)
			{
				TSharedRef<FJsonObject> A = MakeShared<FJsonObject>();
				A->SetStringField(TEXT("path"), DT->GetOutermost()->GetName());
				A->SetObjectField(TEXT("rows"), ChangedRows);
				A->SetStringField(TEXT("mode"), TEXT("upsert"));
				A->SetBoolField(TEXT("save"), false);
				const FEWResult W = Reg.Call(TEXT("datatable_set_rows"), A);
				if (!W.bOk) { return W; }
				Res.Warnings.Append(W.Warnings);
			}
			if (Removed.Num() > 0)
			{
				TSharedRef<FJsonObject> A = MakeShared<FJsonObject>();
				A->SetStringField(TEXT("path"), DT->GetOutermost()->GetName());
				TArray<TSharedPtr<FJsonValue>> Names;
				for (const FString& N : Removed) { Names.Add(MakeShared<FJsonValueString>(N)); }
				A->SetArrayField(TEXT("rows"), Names);
				A->SetBoolField(TEXT("save"), false);
				const FEWResult W = Reg.Call(TEXT("datatable_remove_rows"), A);
				if (!W.bOk) { return W; }
			}
			// Read back: every imported row now equals the file, removed rows are gone.
			int32 Mismatch = 0;
			for (const FImportRecord& Rec : Records)
			{
				const uint8* Mem = DT->FindRowUnchecked(FName(*Rec.Row));
				if (!Mem) { ++Mismatch; continue; }
				uint8* Buf = (uint8*)FMemory::Malloc(RS->GetStructureSize(), RS->GetMinAlignment());
				RS->InitializeStruct(Buf);
				RS->CopyScriptStruct(Buf, Mem);
				const FEWResult W = EW::WriteValues(RS, Buf, nullptr, Rec.Values);
				RS->DestroyStruct(Buf);
				FMemory::Free(Buf);
				const TSharedPtr<FJsonObject>* Ch = nullptr;
				if (W.bOk && W.Data.IsValid() && W.Data->TryGetObjectField(TEXT("changes"), Ch))
				{
					for (const auto& F : (*Ch)->Values)
					{
						const TSharedPtr<FJsonObject>* One = nullptr;
						if (F.Value->TryGetObject(One) && !FJsonValue::CompareEqual(*(*One)->TryGetField(TEXT("before")), *(*One)->TryGetField(TEXT("after")))) { ++Mismatch; break; }
					}
				}
			}
			for (const FString& N : Removed) { if (DT->FindRowUnchecked(FName(*N))) { ++Mismatch; } }
			if (Mismatch > 0)
			{
				return FEWResult::Error(TEXT("READBACK_MISMATCH"), FString::Printf(TEXT("%d row(s) differ from the file after the import. Undo with Ctrl+Z and report this."), Mismatch));
			}
			Data->SetBoolField(TEXT("readBack"), true);
			Res.Data = Data;
			return SaveTable(DT, EW::Bool(Args, TEXT("save"), true), Res);
		});
}
