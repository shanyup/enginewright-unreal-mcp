"""0.4.1 polish tests (cila): asset_find folder warning, non-ASCII name warnings, game_run run retention,
actor_preview of a Character with a camera boom, pie_start timeout.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Writes only under /Game/_EWTest and <project>/Saved/Enginewright/runs
(game_run keepRuns deletes the OLDEST run folders there: use a test project).
"""
import os, pathlib, sys, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import PROJ, ROOT, call as c, check, delete_if_exists, done, ensure_stopped

ensure_stopped()
HERE = pathlib.Path(__file__).resolve().parent
CUBE = "/Engine/BasicShapes/Cube.Cube"

# --- asset_find: a folder that does not exist warns
r = c("asset_find", {"folder": "/Game/_EWNoSuchFolder", "limit": 5})
check("asset_find: missing folder -> empty list + warning that names the folder", r["ok"] and r["data"]["total"] == 0
      and any("_EWNoSuchFolder" in w and "does not exist" in w for w in r.get("warnings", [])), r)
r = c("asset_find", {"folder": ROOT, "limit": 5})
check("asset_find: existing folder -> no folder warning", r["ok"] and not any("does not exist" in w for w in r.get("warnings", [])), r)
r = c("asset_find", {"limit": 1})
check("asset_find: default folder /Game -> no folder warning", r["ok"] and not any("does not exist" in w for w in r.get("warnings", [])), r)

# --- non-ASCII asset names warn (blueprint_create, asset_import)
TR = ROOT + "/BP_EWÇaydanlık"
ASCII = ROOT + "/BP_EWAsciiOnly"
delete_if_exists(ASCII)
r = c("blueprint_create", {"path": ASCII, "parent": "Actor"})
check("blueprint_create ASCII name -> no warning", r["ok"] and not r.get("warnings"), r)
delete_if_exists(ASCII)
r = c("blueprint_create", {"path": TR, "parent": "Actor"})
check("blueprint_create BP_EWÇaydanlık -> created, warning names the letters and suggests an ASCII name",
      r["ok"] and any("Ç" in w and "ı" in w and "BP_EWCaydanlik" in w for w in r.get("warnings", [])), r)
delete_if_exists(TR)

png = PROJ / "Saved/Enginewright/import/EWÇay.png"
png.parent.mkdir(parents=True, exist_ok=True)
sys.path.insert(0, str(HERE))
import ewpng
ewpng.write_png(str(png), 4, 4, bytes([200, 100, 50, 255]) * 16)
TEX = ROOT + "/EWÇay"
delete_if_exists(TEX)
r = c("asset_import", {"files": [str(png)], "folder": ROOT})
check("asset_import of a file with a Turkish name -> warning", r["ok"] and any("EWÇay" in w and "ASCII" in w for w in r.get("warnings", [])), r)
delete_if_exists(TEX)
png.unlink(missing_ok=True)

# --- game_run keeps only the newest keepRuns run folders (default folder)
FAKE = str(HERE / "fakegame.py")
# The stand-in game runs windowless (pythonw.exe) so no console window opens and stays on screen.
_PYW = os.path.join(os.path.dirname(sys.executable), "pythonw.exe")
PY = _PYW if os.path.exists(_PYW) else sys.executable
runs = PROJ / "Saved/Enginewright/runs"


def fake_run(keep):
    return c("game_run", {"exe": PY, "exeArgs": [FAKE], "url": "/Game/Maps/Test?Mode=pass?Shots=0", "pass": ["PASSED"],
                          "keepRuns": keep, "timeout": 60}, timeout=300)


def run_dirs():
    return sorted(d.name for d in runs.glob("run-*") if d.is_dir()) if runs.exists() else []


for i in range(3):
    r = fake_run(2)
    check(f"fake run {i + 1} ok", r.get("ok"), r)
    time.sleep(1.2)  # run folders are named by the second
dirs = run_dirs()
check("keepRuns 2: after 3 runs exactly 2 run folders remain", len(dirs) == 2, dirs)
r = fake_run(0)
check("keepRuns 0 keeps everything (the folder count grows by one)", r.get("ok") and len(run_dirs()) == 3, run_dirs())
time.sleep(1.2)
r = fake_run(2)
check("N+1th run: oldest deleted, reply says so", r.get("ok") and len(run_dirs()) == 2 and any("Deleted" in w for w in r.get("warnings", [])), (run_dirs(), r.get("warnings")))
r = c("game_run", {"exe": PY, "exeArgs": [FAKE], "url": "/Game/Maps/Test?Mode=pass?Shots=0", "pass": ["PASSED"], "keepRuns": -1, "dryRun": True})
check("dryRun deletes nothing", r.get("ok") and len(run_dirs()) == 2, r)

# --- actor_preview: Character with a camera boom is framed on the body
B = ROOT + "/BP_EWBoomChar"
delete_if_exists(B)
r = c("blueprint_create", {"path": B, "parent": "Character"})
check("Character Blueprint", r["ok"], r)
r1 = c("blueprint_add_component", {"path": B, "class": "StaticMeshComponent", "name": "Body", "values": {"StaticMesh": CUBE}})
r2 = c("blueprint_add_component", {"path": B, "class": "SpringArmComponent", "name": "Boom", "values": {"TargetArmLength": 900}})
r3 = c("blueprint_add_component", {"path": B, "class": "CameraComponent", "name": "Cam", "parent": "Boom"})
check("body + spring arm + camera added", r1["ok"] and r2["ok"] and r3["ok"], (r1, r2, r3))
r = c("actor_preview", {"class": B + ".BP_EWBoomChar_C", "inline": False, "width": 640, "height": 360})
d = r.get("data") or {}
check("actor_preview: frame = the 100 cm body, not the 900 cm boom", r["ok"] and all(abs(v - 100) < 1 for v in d.get("boundsSize", [0])) and d["drawnPercent"] > 1, r)
check("preview cleaned up", d.get("cleanedUp") is True, d)
delete_if_exists(B)

# --- pie_start timeout
r = c("pie_start", {"timeout": 0})
check("pie_start timeout 0 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_start", {"timeout": 5000})
check("pie_start timeout 5000 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_start", {"timeout": 1})
if not r["ok"]:
    check("pie_start timeout 1: TIMEOUT message names the wait and says the session is not stopped",
          r["errorCode"] == "TIMEOUT" and "within 1 s" in r["message"] and "not stopped" in r["message"], r)
else:
    check("pie_start timeout 1 (the map opened within 1 s: ok)", True)
c("pie_stop")
ensure_stopped()
r = c("pie_start", {"timeout": 120})
check("pie_start with a longer timeout waits for the pawn instead of TIMEOUT", r["ok"], r)
c("pie_stop")
ensure_stopped()
done()
