"""level_audit tests (EW-076): hidden actor + idle static mesh + clean actor -> 2 findings; filters, patterns, saved map.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Writes only under /Game/_EWTest; the actors it adds are removed again.
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import ROOT, call as c, check, delete_if_exists, done, ensure_stopped

ensure_stopped()
IDLE, PROP, MAP = ROOT + "/BP_EWAuditIdle", ROOT + "/BP_EWAuditProp", ROOT + "/MAP_EWAudit"
CUBE = "/Engine/BasicShapes/Cube.Cube"
delete_if_exists(IDLE, PROP, MAP)

for bp, extra in ((IDLE, False), (PROP, True)):
    r = c("blueprint_create", {"path": bp, "parent": "Actor"})
    check(f"create {bp.split('/')[-1]}", r["ok"], r)
    r = c("blueprint_add_component", {"path": bp, "class": "StaticMeshComponent", "name": "Body", "values": {"StaticMesh": CUBE}})
    check("  with a static mesh", r["ok"], r)
    if extra:
        r = c("blueprint_add_component", {"path": bp, "class": "BoxComponent", "name": "Trigger"})
        check("  with a collision box (gameplay component)", r["ok"], r)
    c("blueprint_compile", {"path": bp})

labels = []


def spawn(cls, label):
    r = c("actor_spawn", {"class": cls, "label": label, "location": [0, 0, 100 * len(labels)]})
    check(f"spawn {label}", r["ok"], r)
    labels.append(label)
    return r["data"]


hid = spawn(IDLE + "." + IDLE.split("/")[-1] + "_C", "EW_Hidden")
spawn(IDLE + "." + IDLE.split("/")[-1] + "_C", "EW_Idle")
spawn(PROP + "." + PROP.split("/")[-1] + "_C", "EW_Clean")
r = c("object_set", {"path": hid["path"], "values": {"bHidden": True}, "save": False})
check("hide one actor in game", r["ok"], r)

try:
    r = c("level_audit", {"meshPatterns": [], "class": "Actor"})
    d = r.get("data") or {}
    ours = lambda k: [x["label"] for x in d.get(k, []) if x["label"].startswith("EW_")]
    check("level_audit runs on the open level", r["ok"], r)
    check("hidden: EW_Hidden", ours("hidden") == ["EW_Hidden"], d.get("hidden"))
    check("idle static mesh: EW_Idle only (the clean actor has a collision box, the hidden one is not double counted)", ours("idleMeshes") == ["EW_Idle"], d.get("idleMeshes"))
    check("2 findings of ours (1 hidden + 1 idle), clean actor not listed", sum(1 for k in ("hidden", "idleMeshes", "placeholders") for x in d[k] if x["label"].startswith("EW_")) == 2
          and "EW_Clean" not in str(d), d)
    check("counts and verdict", d["hiddenCount"] >= 1 and d["idleMeshCount"] >= 1 and d["findingCount"] >= 2 and d["verdict"] == "findings" and d["actorsScanned"] >= 3, d)
    check("actor path listed", any(x["path"].endswith("EW_Hidden") or "EW_Hidden" in x["path"] or x["label"] == "EW_Hidden" for x in d["hidden"]), d["hidden"])

    r = c("level_audit", {"meshPatterns": ["*Cube*"]})
    check("pattern *Cube* finds the visible cube actors (idle + clean), not the hidden one", r["ok"] and sorted(x["label"] for x in r["data"]["placeholders"] if x["label"].startswith("EW_")) == ["EW_Clean", "EW_Idle"], r)
    r = c("level_audit", {})
    check("default patterns include engine basic shapes", r["ok"] and r["data"]["placeholderCount"] >= 2 and "/Engine/BasicShapes/*" in r["data"]["meshPatterns"], r)

    r = c("level_audit", {"class": PROP + "." + PROP.split("/")[-1] + "_C", "meshPatterns": []})
    check("class filter: only the clean Blueprint, no findings", r["ok"] and r["data"]["actorsScanned"] == 1 and r["data"]["findingCount"] == 0 and r["data"]["verdict"] == "clean", r)
    r = c("level_audit", {"class": "NoSuchClassXyz"})
    check("unknown class -> UNKNOWN_CLASS", not r["ok"] and r["errorCode"] == "UNKNOWN_CLASS", r)
    r = c("level_audit", {"tag": "NoSuchTag"})
    check("tag filter with no match -> 0 scanned", r["ok"] and r["data"]["actorsScanned"] == 0, r)

    # a saved copy that is NOT the open map
    r = c("level_save", {"path": MAP})
    check("save a copy of the level", r["ok"], r)
    open_map = c("editor_state")["data"]["map"]
    if r["ok"] and open_map != MAP:
        r = c("level_audit", {"map": MAP, "meshPatterns": [], "class": "Actor"})
        d = r.get("data") or {}
        check("level_audit on a map that is not open: same findings, open level unchanged",
              r["ok"] and d["openLevel"] is False and [x["label"] for x in d["hidden"] if x["label"].startswith("EW_")] == ["EW_Hidden"]
              and [x["label"] for x in d["idleMeshes"] if x["label"].startswith("EW_")] == ["EW_Idle"] and c("editor_state")["data"]["map"] == open_map, r)
    else:
        check("saved copy is not the open map (skipped: editor moved to the copy)", True)
    r = c("level_audit", {"map": ROOT + "/MAP_Nope"})
    check("missing map -> MAP_NOT_FOUND", not r["ok"] and r["errorCode"] == "MAP_NOT_FOUND", r)
    r = c("level_audit", {"map": "not a path"})
    check("bad map path -> INVALID_PATH", not r["ok"] and r["errorCode"] == "INVALID_PATH", r)
finally:
    left = c("actor_find", {"label": "EW_*"})["data"]["actors"]
    if left:
        c("actor_delete", {"actors": [a["label"] for a in left]})
    delete_if_exists(IDLE, PROP, MAP)
done()
