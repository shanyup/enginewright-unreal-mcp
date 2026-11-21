"""pie_cursor tests (EW-104): the mouse cursor moved onto an actor, a world point or a pixel, as the game sees it.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT whose open level has a player start or default pawn.
"""
import math, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import ROOT, call as c, check, done, ensure_stopped, exists

ensure_stopped()
CUBE = ROOT + "/BP_EWMeasure"
LABEL = "EW_CursorCube"
if not exists(CUBE):
    r = c("blueprint_create", {"path": CUBE, "parent": "Actor"})
    assert r["ok"], r
    r = c("blueprint_add_component", {"path": CUBE, "class": "StaticMeshComponent", "name": "Body",
                                      "values": {"StaticMesh": "/Engine/BasicShapes/Cube.Cube"}})
    assert r["ok"], r
c("actor_delete", {"actors": [LABEL]})

r = c("pie_cursor", {"screen": [10, 10]})
check("pie_cursor without PIE -> NO_PLAYER", not r["ok"] and r["errorCode"] == "NO_PLAYER", r)

# A cube 5 m in front of the player camera (the camera is read in a first PIE run).
r = c("pie_start", {"timeout": 90})
cam = r["data"]["worlds"][0]["players"][0] if r["ok"] else None
c("pie_stop")
check("pie_start for camera position", cam is not None, r)
pitch, yaw = math.radians(cam["cameraRotation"][0]), math.radians(cam["cameraRotation"][1])
fwd = [math.cos(pitch) * math.cos(yaw), math.cos(pitch) * math.sin(yaw), math.sin(pitch)]
loc = [cam["cameraLocation"][i] + fwd[i] * 500 for i in range(3)]
behind = [cam["cameraLocation"][i] - fwd[i] * 500 for i in range(3)]
s = c("actor_spawn", {"class": CUBE, "label": LABEL, "location": loc})
check("spawn a cube 5 m in front of the player camera", s["ok"], s)
r = c("pie_start", {"timeout": 90})
check("pie_start", r["ok"], r)

r = c("pie_cursor", {})
check("no target -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_cursor", {"actor": LABEL, "screen": [1, 1]})
check("two targets -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_cursor", {"location": [1, 2]})
check("location with 2 numbers -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_cursor", {"screen": [5, 5], "offset": [0, 0, 10]})
check("offset with screen -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_cursor", {"actor": "EW_CursorCub"})
check("unknown actor -> ACTOR_NOT_FOUND with a suggestion", not r["ok"] and r["errorCode"] == "ACTOR_NOT_FOUND" and LABEL in r.get("suggestions", []), r)
r = c("pie_cursor", {"location": behind})
check("point behind the camera -> OFF_SCREEN", not r["ok"] and r["errorCode"] == "OFF_SCREEN", r)
r = c("pie_cursor", {"screen": [-5, 10]})
check("pixel outside the view -> OFF_SCREEN with the point", not r["ok"] and r["errorCode"] == "OFF_SCREEN" and r.get("data", {}).get("screen") == [-5, 10], r)
r = c("pie_cursor", {"actor": LABEL, "click": "Up"})
check("bad click button -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)

r = c("pie_cursor", {"actor": LABEL})
d = r.get("data", {})
vw, vh = d.get("viewport", [0, 0])
check("cursor on the cube: near the screen centre (the cube is straight ahead)", r["ok"] and abs(d["screen"][0] - vw / 2) < vw * 0.05
      and abs(d["screen"][1] - vh / 2) < vh * 0.05, d)
check("a click there would hit the cube (onTarget, hitActor)", r["ok"] and d.get("onTarget") is True and d.get("hitActor") == LABEL and 300 < d.get("hitDistance", 0) < 500, d)
check("the game reads the cursor where it was put", r["ok"] and "gameCursor" in d and abs(d["gameCursor"][0] - d["screen"][0]) <= 3
      and abs(d["gameCursor"][1] - d["screen"][1]) <= 3, r)
check("no UI over the cursor", r["ok"] and d.get("uiOver") is False, d)

r = c("pie_cursor", {"actor": LABEL, "offset": [0, 0, 300]})
d = r.get("data", {})
check("offset above the cube: higher on screen, not on the cube, warned", r["ok"] and d.get("onTarget") is False and r.get("warnings"), r)

r = c("pie_cursor", {"location": loc, "click": "Left"})
d = r.get("data", {})
check("location + click: click sent, cube under the cursor", r["ok"] and d.get("click") == "Left" and "clickHandled" in d and d.get("hitActor") == LABEL, d)

r = c("pie_cursor", {"screen": [40, 30]})
d = r.get("data", {})
check("screen pixel: game cursor at that pixel", r["ok"] and d["screen"] == [40, 30] and abs(d["gameCursor"][0] - 40) <= 3 and abs(d["gameCursor"][1] - 30) <= 3, r)

c("pie_stop")
c("actor_delete", {"actors": [LABEL]})
done()
