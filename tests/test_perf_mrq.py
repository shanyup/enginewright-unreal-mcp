"""Tests for the tools that had none before 0.4.1: perf_capture, perf_scene, perf_gpu_profile, mrq_render, mrq_status,
asset_references, asset_save.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Writes only under /Game/_EWTest; the actor it adds is removed again.
Movie Render Queue is only checked up to its argument and asset checks (a real render takes minutes).
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import ROOT, call as c, check, delete_if_exists, done, ensure_stopped

ensure_stopped()
BP = ROOT + "/BP_EWPerfProbe"
delete_if_exists(BP)
CUBE = "/Engine/BasicShapes/Cube.Cube"

r = c("blueprint_create", {"path": BP, "parent": "Actor"})
check("probe Blueprint created", r["ok"], r)
r = c("blueprint_add_component", {"path": BP, "class": "StaticMeshComponent", "name": "Body", "values": {"StaticMesh": CUBE}})
check("probe has a cube mesh", r["ok"], r)

# --- asset_references / asset_save
r = c("asset_references", {"path": BP, "direction": "dependencies"})
check("asset_references (dependencies) lists the cube mesh", r["ok"] and CUBE.split(".")[0] in str(r["data"]), r)
r = c("asset_references", {"path": CUBE.split(".")[0], "direction": "referencers"})
check("asset_references (referencers) of the cube names the probe", r["ok"] and "BP_EWPerfProbe" in str(r["data"]), r)
r = c("asset_references", {"path": ROOT + "/Nope"})
check("asset_references on a missing asset -> ASSET_NOT_FOUND", not r["ok"] and r["errorCode"] == "ASSET_NOT_FOUND", r)
r = c("asset_references", {"path": BP, "direction": "sideways"})
check("asset_references bad direction -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("asset_save", {"paths": [BP]})
check("asset_save saves the probe", r["ok"], r)
r = c("asset_save", {"paths": [ROOT + "/Nope"]})
check("asset_save on a missing asset -> ASSET_NOT_FOUND", not r["ok"] and r["errorCode"] == "ASSET_NOT_FOUND", r)

# --- perf_scene
r = c("actor_spawn", {"class": BP + ".BP_EWPerfProbe_C", "label": "EW_PerfProbe", "location": [0, 0, 0]})
check("probe actor placed", r["ok"], r)
try:
    r = c("perf_scene", {"top": 3})
    d = r.get("data") or {}
    check("perf_scene counts the editor level", r["ok"] and d["world"] == "editor" and d["actors"] >= 1 and "lights" in d and "settings" in d, r)
    check("perf_scene sees the cube (visible primitive, triangles)", d.get("visiblePrimitives", 0) >= 1 and d.get("staticMeshComponents", 0) >= 1, d)
    check("perf_scene lists the heaviest meshes, at most top", len(d.get("heaviestMeshes", [])) <= 3 and any("Cube" in str(m) for m in d.get("heaviestMeshes", [])), d.get("heaviestMeshes"))
finally:
    c("actor_delete", {"actors": ["EW_PerfProbe"]})

# --- perf_capture / perf_gpu_profile (editor level, no PIE)
r = c("perf_capture", {"seconds": 1, "warmup": 0})
d = r.get("data") or {}
check("perf_capture: frames, fps and frame-time spread", r["ok"] and d["frames"] > 5 and d["fpsAvg"] > 0 and {"avg", "p50", "p95", "max"} <= set(d["frameMs"]), r)
check("perf_capture says which side bounds the frame", d.get("boundBy") in ("game", "render", "gpu"), d)
r = c("perf_capture", {"seconds": 1, "bogus": 1})
check("perf_capture unknown argument -> UNKNOWN_ARGUMENT", not r["ok"] and r["errorCode"] == "UNKNOWN_ARGUMENT", r)
r = c("perf_gpu_profile", {"top": 3})
d = r.get("data") or {}
check("perf_gpu_profile: total GPU ms and at most 3 passes", r["ok"] and d["totalGpuMs"] > 0 and 1 <= len(d["top"]) <= 3 and "ms" in d["top"][0], r)

# --- mrq_status / mrq_render (arguments and asset checks only)
r = c("mrq_status")
check("mrq_status: idle when nothing renders", r["ok"] and r["data"]["state"] == "idle" and r["data"]["framesExpected"] == 0, r)
r = c("mrq_render", {})
check("mrq_render without sequence -> MISSING_ARGUMENT", not r["ok"] and r["errorCode"] == "MISSING_ARGUMENT", r)
r = c("mrq_render", {"sequence": ROOT + "/LS_DoesNotExist"})
check("mrq_render with a missing sequence is refused, nothing started", not r["ok"] and c("mrq_status")["data"]["state"] == "idle", r)

delete_if_exists(BP)
done()
