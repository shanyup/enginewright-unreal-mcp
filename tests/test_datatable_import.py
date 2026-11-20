"""datatable_import tests (EW-002): CSV/JSON upsert with diff, dryRun, strict field check.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Writes only under /Game/_EWTest and <project>/Saved/Enginewright/import.
"""
import json, os, pathlib, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import PROJ, ROOT, call as c, check, delete_if_exists, done, ensure_stopped, fixtures

ensure_stopped()
fx = fixtures()
DT = ROOT + "/DT_EWImport"
delete_if_exists(DT)
IMP = PROJ / "Saved/Enginewright/import"
IMP.mkdir(parents=True, exist_ok=True)

r = c("datatable_create", {"path": DT, "rowStruct": fx["struct"]})
check("table created", r["ok"], r)
r = c("datatable_set_rows", {"path": DT, "rows": {"Apple": {"DisplayName": "Apple", "Weight": 1.5, "Category": "RawIngredient"},
                                                  "Bread": {"DisplayName": "Bread", "Weight": 2.0, "Category": "Dish"}}})
check("seed 2 rows", r["ok"], r)


def rows():
    return c("datatable_get", {"path": DT})["data"]["rows"]


def write(name, text):
    f = IMP / name
    f.write_text(text, encoding="utf-8")
    return str(f)


before = rows()
csv1 = write("a.csv", 'Name,DisplayName,Weight,Category\nApple,Apple,2.5,RawIngredient\nBread,Bread,2,Dish\nPear,"Pear, green",0.75,RawIngredient\n')
r = c("datatable_import", {"path": DT, "file": csv1, "dryRun": True})
d = r.get("data") or {}
check("dryRun: 1 added, 1 changed, 1 unchanged, 0 removed", r["ok"] and d["addedCount"] == 1 and d["changedCount"] == 1 and d["unchangedCount"] == 1 and d["removedCount"] == 0, r)
check("dryRun lists the row and the field before/after", d.get("added") == ["Pear"] and d["changed"][0]["row"] == "Apple"
      and d["changed"][0]["fields"]["Weight"]["before"] == 1.5 and d["changed"][0]["fields"]["Weight"]["after"] == 2.5, d)
check("dryRun wrote nothing", rows() == before, rows())

r = c("datatable_import", {"path": DT, "file": csv1})
d = r.get("data") or {}
check("apply: same counts, read back ok", r["ok"] and d["addedCount"] == 1 and d["changedCount"] == 1 and d.get("readBack") is True, r)
check("apply: no 'did not change' warning for the fields a changed row kept",
      not any("did not change" in w for w in r.get("warnings", [])), r.get("warnings"))
after = rows()
check("datatable_get shows what dryRun promised (Pear with a comma in the text, Apple 2.5)",
      set(after) == {"Apple", "Bread", "Pear"} and after["Apple"]["Weight"] == 2.5 and "Pear, green" in json.dumps(after["Pear"]["DisplayName"]), after)
r = c("datatable_import", {"path": DT, "file": csv1, "dryRun": True})
check("second import: everything unchanged", r["ok"] and r["data"]["unchangedCount"] == 3 and r["data"]["addedCount"] == 0 and r["data"]["changedCount"] == 0, r)

# strict checks: nothing written
snap = rows()
bad = write("bad_field.csv", "Name,Weigth\nApple,3\n")
r = c("datatable_import", {"path": DT, "file": bad})
check("unknown column -> IMPORT_INVALID with a suggestion, nothing written", not r["ok"] and r["errorCode"] == "IMPORT_INVALID" and "Weight" in r.get("suggestions", []) and rows() == snap, r)
bad = write("bad_value.csv", "Name,Weight\nApple,heavy\nPear,9\n")
r = c("datatable_import", {"path": DT, "file": bad})
check("bad number -> IMPORT_INVALID, the good row is not written either", not r["ok"] and r["errorCode"] == "IMPORT_INVALID" and rows() == snap, r)
bad = write("bad_enum.csv", "Name,Category\nApple,Fruit\n")
r = c("datatable_import", {"path": DT, "file": bad, "dryRun": True})
check("bad enum value -> IMPORT_INVALID (dryRun says so too)", not r["ok"] and r["errorCode"] == "IMPORT_INVALID", r)
bad = write("dup.csv", "Name,Weight\nApple,1\nApple,2\n")
r = c("datatable_import", {"path": DT, "file": bad, "dryRun": True})
check("duplicate row name -> IMPORT_INVALID", not r["ok"] and r["errorCode"] == "IMPORT_INVALID", r)
bad = write("short.csv", "Name,Weight\nApple\n")
r = c("datatable_import", {"path": DT, "file": bad, "dryRun": True})
check("row with too few cells -> IMPORT_INVALID", not r["ok"] and r["errorCode"] == "IMPORT_INVALID", r)

# deleteMissing + JSON
js = write("b.json", json.dumps([{"Name": "Apple", "Weight": 2.5}, {"RowName": "x"}]))
r = c("datatable_import", {"path": DT, "file": js, "dryRun": True})
check("JSON entry without a row name key -> IMPORT_INVALID", not r["ok"], r)
js = write("c.json", json.dumps([{"Name": "Apple", "Weight": 9.0}, {"Name": "Kiwi", "DisplayName": "Kiwi", "Weight": 0.1}]))
r = c("datatable_import", {"path": DT, "file": js, "dryRun": True, "deleteMissing": True})
d = r.get("data") or {}
check("JSON + deleteMissing dryRun: Kiwi added, Apple changed, Bread+Pear removed", r["ok"] and d["added"] == ["Kiwi"] and d["changedCount"] == 1
      and sorted(d["removed"]) == ["Bread", "Pear"] and rows() == snap, r)
r = c("datatable_import", {"path": DT, "file": js, "deleteMissing": True})
check("apply with deleteMissing", r["ok"] and set(rows()) == {"Apple", "Kiwi"} and rows()["Apple"]["Weight"] == 9.0, r)
js = write("d.json", json.dumps({"Apple": {"Weight": 1.5}}))
r = c("datatable_import", {"path": DT, "file": js})
check("JSON object form {row:{field:value}}; other rows stay", r["ok"] and set(rows()) == {"Apple", "Kiwi"} and rows()["Apple"]["Weight"] == 1.5, r)

# arguments
r = c("datatable_import", {"path": DT, "file": "C:/Windows/win.ini"})
check("file outside the project -> INVALID_PATH", not r["ok"] and r["errorCode"] == "INVALID_PATH", r)
r = c("datatable_import", {"path": DT, "file": "Saved/Enginewright/import/missing.csv"})
check("missing file -> FILE_NOT_FOUND", not r["ok"] and r["errorCode"] == "FILE_NOT_FOUND", r)
r = c("datatable_import", {"path": ROOT + "/DT_Nope", "file": csv1})
check("missing table -> TABLE_NOT_FOUND", not r["ok"] and r["errorCode"] == "TABLE_NOT_FOUND", r)
r = c("datatable_import", {"path": DT, "file": csv1, "bogus": 1})
check("unknown argument rejected", not r["ok"] and r["errorCode"] == "UNKNOWN_ARGUMENT", r)

delete_if_exists(DT)
done()
