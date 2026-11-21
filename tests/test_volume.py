"""actor_spawn volume tests (EW-094): volumes get a box brush, so they have an area and work.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. The actors it adds are removed again; the level is not saved.
"""
import os, sys, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import call as c, check, done, ensure_stopped

ensure_stopped()
added = []


def near(a, b, tol=2.0):
    return all(abs(x - y) <= tol for x, y in zip(a, b))


def spawn(args):
    r = c("actor_spawn", args)
    if r["ok"]:
        added.append(r["data"]["label"])
    return r


def size_of(label):
    return c("actor_bounds", {"actor": label})["data"]["size"]


def find(cls):
    r = c("actor_find", {"class": cls})
    return [a["label"] for a in r["data"]["actors"]] if r["ok"] else []


nav_before = find("RecastNavMesh")

try:
    # Default box, as Place Actors builds it.
    r = spawn({"class": "TriggerVolume", "label": "EW_VolTrigger", "location": [0, 0, 100]})
    check("TriggerVolume placed with a brush, default 200 cm", r["ok"] and r["data"].get("brushSize") == [200, 200, 200], r)
    check("  actor_bounds: 200 x 200 x 200 (not 0)", r["ok"] and near(size_of("EW_VolTrigger"), [200, 200, 200]), r)

    # Size and scale together: world size = size x scale.
    r = spawn({"class": "PostProcessVolume", "label": "EW_VolPost", "location": [0, 0, 600], "size": [400, 300, 100], "scale": [2, 1, 1]})
    check("PostProcessVolume with size + scale", r["ok"] and near(r["data"].get("worldSize", [0, 0, 0]), [800, 300, 100]), r)
    check("  actor_bounds agrees", r["ok"] and near(size_of("EW_VolPost"), [800, 300, 100]), r)

    # The case that broke a level: a nav mesh bounds volume without a brush built no nav mesh.
    r = spawn({"class": "NavMeshBoundsVolume", "label": "EW_VolNav", "location": [0, 0, 0], "size": [3000, 3000, 600]})
    check("NavMeshBoundsVolume placed with a 3000 x 3000 x 600 brush", r["ok"] and near(size_of("EW_VolNav"), [3000, 3000, 600]), r)
    nav = []
    for _ in range(20):
        nav = find("RecastNavMesh")
        if nav:
            break
        time.sleep(0.5)
    check("  the level gets nav mesh data (RecastNavMesh)", len(nav) > 0, {"before": nav_before, "after": nav})

    # Errors: nothing is left behind.
    count = len(find("Volume"))
    r = c("actor_spawn", {"class": "TriggerVolume", "label": "EW_VolBad", "size": [0, 100, 100]})
    check("size 0 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
    check("  nothing placed", len(find("Volume")) == count, count)
    r = c("actor_spawn", {"class": "Brush", "label": "EW_VolBsp"})
    check("BSP Brush -> NOT_SPAWNABLE, nothing placed", not r["ok"] and r["errorCode"] == "NOT_SPAWNABLE" and len(find("Volume")) == count, r)

    # Not a volume: size is ignored with a warning, the actor is placed as before.
    r = spawn({"class": "PointLight", "label": "EW_VolLight", "size": [500, 500, 500]})
    check("size on a non-volume -> placed, with a warning", r["ok"] and any("volumes only" in w for w in r.get("warnings", [])), r)

finally:
    if added:
        c("actor_delete", {"actors": added})
    extra = [n for n in find("RecastNavMesh") if n not in nav_before]
    if extra:
        c("actor_delete", {"actors": extra})

done()
