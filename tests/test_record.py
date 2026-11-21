"""pie_record tests (EW-103): a short MP4 of a Play In Editor player, with timed steps, a contact sheet and
honest frame counts.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT whose open level has a player start or default pawn, and
ffmpeg (PATH or WinGet). Videos land in <project>/Saved/Enginewright/runs/pie_record/.
"""
import json, os, subprocess, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import call as c, call_raw, check, done, ensure_stopped

exists = os.path.isfile


def probe(path):
    """Duration and frame count of a video via ffprobe (next to ffmpeg); None when ffprobe is missing."""
    try:
        out = subprocess.run(["ffprobe", "-v", "error", "-select_streams", "v:0", "-count_frames", "-show_entries",
                              "stream=nb_read_frames,width,height,codec_name:format=duration", "-of", "json", path],
                             capture_output=True, text=True, timeout=60).stdout
        j = json.loads(out)
        s = j["streams"][0]
        return {"duration": float(j["format"]["duration"]), "frames": int(s["nb_read_frames"]), "codec": s["codec_name"],
                "width": int(s["width"]), "height": int(s["height"])}
    except Exception:
        return None


ensure_stopped()
r = c("pie_record", {"seconds": 1})
check("pie_record without PIE -> NO_PLAYER", not r["ok"] and r["errorCode"] == "NO_PLAYER", r)

r = c("pie_start", {"timeout": 90})
check("pie_start for recording", r["ok"], r)

r = c("pie_record", {"seconds": 0.1})
check("seconds below 0.5 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"seconds": 31})
check("seconds above 30 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"fps": 60})
check("fps above 30 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"maxWidth": 50})
check("maxWidth 50 -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"player": 3})
check("missing player index -> NO_PLAYER", not r["ok"] and r["errorCode"] == "NO_PLAYER", r)
r = c("pie_record", {"steps": [{"tool": "pie_inptu", "args": {"key": "W"}}]})
check("unknown step tool -> UNKNOWN_TOOL with a suggestion", not r["ok"] and r["errorCode"] == "UNKNOWN_TOOL"
      and "pie_input" in r.get("suggestions", []), r)
r = c("pie_record", {"steps": [{"tool": "pie_stop"}]})
check("pie_stop as a step -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"seconds": 2, "steps": [{"tool": "pie_input", "at": 5}]})
check("step 'at' after the end -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("pie_record", {"steps": [{"tool": "pie_input", "when": 1}]})
check("unknown step key -> INVALID_ARGUMENT naming it", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT" and "when" in r["message"], r)
r = c("pie_record", {"steps": ["pie_input"]})
check("step that is not an object -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)

# The real recording: 3 s at 15 fps, walking forward from 0.5 s, so frames must differ.
raw = call_raw("pie_record", {"seconds": 3, "fps": 15, "steps": [
    {"tool": "pie_input", "args": {"key": "W", "seconds": 1.5}, "at": 0.5},
    {"tool": "console_command", "args": {"command": "stat fps"}, "at": 0},
    {"tool": "console_command", "args": {"command": "stat fps"}, "at": 2.5}]}, timeout=120)
r = raw["structuredContent"] if isinstance(raw, dict) and "structuredContent" in raw else raw
d = r.get("data", {})
check("pie_record returns an MP4 path that exists", r["ok"] and exists(d.get("video", "")) and d["video"].endswith(".mp4"), r)
check("frames near seconds x fps (at least 60 %), real fps reported", r["ok"] and d["frames"] >= 27 and d["fps"] > 8 and 2.8 <= d["seconds"] <= 4.0, d)
check("walking makes distinct frames, not frozen", r["ok"] and d["distinctFrames"] >= 5 and d["frozen"] is False, d)
check("three steps ran and all ok, at their times", r["ok"] and len(d["steps"]) == 3 and all(s.get("ok") for s in d["steps"])
      and not d["failed"] and abs(d["steps"][1]["at"] - 0.5) < 0.25, d.get("steps"))
check("contact sheet written and four moments listed", r["ok"] and exists(d["sheet"]) and len(d["sheetAt"]) == 4, d)
imgs = [x for x in (raw.get("content", []) if isinstance(raw, dict) else []) if x.get("type") == "image"]
check("contact sheet returned as an image", len(imgs) == 1 and imgs[0]["mimeType"] == "image/png", [x.get("type") for x in raw.get("content", [])] if isinstance(raw, dict) else raw)
check("frames deleted after a good MP4 (keepFrames false)", r["ok"] and "framesDir" not in d and not os.path.isdir(os.path.join(d["runDir"], "frames")), d)
check("width limited to maxWidth 960", r["ok"] and 0 < d["width"] <= 960, d)
p = probe(d.get("video", "")) if r["ok"] else None
if p is not None:
    check("ffprobe: H.264, duration close to the recording (real speed)", p["codec"] == "h264" and abs(p["duration"] - d["seconds"]) < 0.5, {"probe": p, "seconds": d["seconds"]})
    # The whole view is in the picture: the bottom-right quarter of the last frame is not black (a capture buffer
    # that is not scaled leaves the picture in the top-left corner only).
    raw = subprocess.run(["ffmpeg", "-v", "error", "-sseof", "-0.3", "-i", d["video"], "-frames:v", "1", "-vf", "crop=iw/4:ih/4:iw*3/4:ih*3/4,scale=16:16",
                          "-f", "rawvideo", "-pix_fmt", "gray", "-"], capture_output=True, timeout=60).stdout
    check("the picture fills the frame (bottom-right corner not black)", len(raw) == 256 and sum(raw) / 256 > 20, {"mean": sum(raw) / max(1, len(raw)), "bytes": len(raw)})

# A still camera: nothing moves, so the picture count must say so.
c("console_command", {"command": "slomo 0.0001"})
r = c("pie_record", {"seconds": 1.5, "fps": 10, "ui": False, "keepFrames": True, "maxWidth": 320}, timeout=120)
c("console_command", {"command": "slomo 1"})
d = r.get("data", {})
check("ui false + keepFrames: frames folder kept, small width", r["ok"] and d.get("ui") is False and os.path.isdir(d.get("framesDir", "")) and d["width"] <= 320, d)
check("still scene -> few distinct frames", r["ok"] and d["distinctFrames"] <= 3, d)

# PIE stops in the middle: the video is shorter, said so.
r = c("pie_record", {"seconds": 4, "steps": [{"tool": "console_command", "args": {"command": "exit"}, "at": 1}]}, timeout=120)
d = r.get("data", {})
check("PIE ending mid-recording -> shorter video, stoppedEarly true, a warning",
      r["ok"] and d.get("stoppedEarly") is True and d["seconds"] < 3.5 and any("shorter" in w for w in r.get("warnings", [])), r)
ensure_stopped()
done()
