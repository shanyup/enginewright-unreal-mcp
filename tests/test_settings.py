"""Settings and editor UI tests: Enginewright's own settings (read-only mode, tool categories, log level), their
read-back from the ini file, describe's category, doctor's server block, client tracking and editor_capture.

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Every setting it changes is put back at the end.
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import PROJ, ROOT, call as c, check, done, rpc

S = "EnginewrightSettings"
INI = PROJ / "Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini"


def put_back():
    c("settings_set", {"class": S, "values": {"bReadOnly": False, "DisabledCategories": [], "LogLevel": "Quiet", "AllowedOrigins": []}})


put_back()

# --- Origin: a web page on another site cannot reach the editor (DNS rebinding / drive-by); clients without Origin can
def with_origin(origin, tok=None):
    return rpc("ping", {}, tok=tok, headers={"Origin": origin} if origin is not None else None)[0]

check("no Origin header: allowed", with_origin(None) == 200)
check("empty Origin: allowed", with_origin("") == 200)
for o in ["http://localhost", "http://localhost:5173", "https://127.0.0.1:8080", "http://[::1]:3000", "HTTP://LOCALHOST:1"]:
    check(f"loopback Origin {o}: allowed", with_origin(o) == 200, o)
for o in ["https://evil.example", "http://localhost.evil.example", "http://127.0.0.1.evil.example", "null",
          "http://evil.example@localhost", "file://", "http://[::1].evil.example"]:
    check(f"foreign Origin {o}: refused with 403", with_origin(o) == 403, o)
check("foreign Origin is refused before the token check (wrong token still 403, not 401)", with_origin("https://evil.example", tok="wrong") == 403)
check("loopback Origin with a wrong token: 401", with_origin("http://localhost", tok="wrong") == 401)
r = c("settings_set", {"class": S, "values": {"AllowedOrigins": ["https://tools.example/"]}})
check("settings_set AllowedOrigins", r["ok"], r)
check("listed Origin: allowed (trailing slash in the setting ignored)", with_origin("https://tools.example") == 200)
check("unlisted Origin still refused", with_origin("https://other.example") == 403)
c("settings_set", {"class": S, "values": {"AllowedOrigins": []}})
check("listed Origin refused again after removal", with_origin("https://tools.example") == 403)

# --- read the settings
r = c("settings_get", {"class": S})
v = r.get("data", {}).get("values", {})
check("settings_get EnginewrightSettings", r["ok"] and v.get("bServerEnabled") is True and isinstance(v.get("Port"), int)
      and v.get("bReadOnly") is False and v.get("DisabledCategories") == [], r)

# --- a client that sends clientInfo is listed by doctor
s, init = rpc("initialize", {"protocolVersion": "2025-06-18", "capabilities": {}, "clientInfo": {"name": "ew-settings-test", "version": "1"}})
check("initialize ok", s == 200 and init["result"]["serverInfo"]["version"], init)
r = c("doctor")
srv = r.get("data", {}).get("server", {})
check("doctor reports the server: version, url, clients, settings", r["ok"] and srv.get("version", "").count(".") == 2
      and srv.get("url", "").endswith("/mcp") and srv.get("clients", 0) >= 1 and srv.get("readOnly") is False, r)

# --- describe shows the category
s, d = rpc("tools/call", {"name": "describe", "arguments": {"name": "widget_tree"}})
dd = d["result"]["structuredContent"]["data"]
check("describe: category UI", dd.get("category") == "UI" and "disabled" not in dd, dd)
s, d = rpc("tools/call", {"name": "describe", "arguments": {"name": "actor_spawn"}})
check("describe: category Level", d["result"]["structuredContent"]["data"].get("category") == "Level", d)

# --- turn a category off
r = c("settings_set", {"class": S, "values": {"DisabledCategories": ["UI"]}})
check("settings_set DisabledCategories [UI]", r["ok"], r)
r = c("settings_get", {"class": S, "properties": ["DisabledCategories"]})
check("read-back: DisabledCategories == [UI]", r["ok"] and r["data"]["values"]["DisabledCategories"] == ["UI"], r)
r = c("widget_tree", {"path": ROOT + "/WBP_Nope"})
check("a UI tool is refused: TOOL_DISABLED", not r["ok"] and r["errorCode"] == "TOOL_DISABLED" and "UI" in r["message"], r)
s, d = rpc("tools/call", {"name": "describe", "arguments": {"name": "widget_tree"}})
check("describe marks it disabled", d["result"]["structuredContent"]["data"].get("disabled") is True, d)
r = c("asset_exists", {"path": ROOT})
check("other categories still work", r["ok"], r)
r = c("settings_set", {"class": S, "values": {"DisabledCategories": ["Editor"]}})
r2 = c("doctor")
check("the Editor category cannot be turned off", r2["ok"], [r, r2])
c("settings_set", {"class": S, "values": {"DisabledCategories": []}})
r = c("widget_tree", {"path": ROOT + "/WBP_Nope"})
check("back on: the UI tool runs again (its own error, not TOOL_DISABLED)", r.get("errorCode") != "TOOL_DISABLED", r)

# --- read-only mode
r = c("settings_set", {"class": S, "values": {"bReadOnly": True}})
check("settings_set bReadOnly true", r["ok"], r)
text = INI.read_text(encoding="utf-8", errors="replace") if INI.exists() else ""
check("saved to EditorPerProjectUserSettings.ini", "[/Script/Enginewright.EnginewrightSettings]" in text and "bReadOnly=True" in text, str(INI))
r = c("actor_spawn", {"class": "PointLight"})
check("a writing tool is refused: READ_ONLY", not r["ok"] and r["errorCode"] == "READ_ONLY", r)
r = c("console_command", {"command": "stat fps"})
check("console_command is refused too (side effects)", not r["ok"] and r["errorCode"] == "READ_ONLY", r)
r = c("asset_find", {"folder": ROOT})
check("reading tools still work", r["ok"], r)
r = c("settings_set", {"class": S, "values": {"bReadOnly": False}})
check("settings_set on Enginewright's own settings is allowed in read-only mode", r["ok"], r)
r = c("settings_get", {"class": S, "properties": ["bReadOnly"]})
check("read-back: bReadOnly false", r["ok"] and r["data"]["values"]["bReadOnly"] is False, r)

# --- log level: one line per call
r = c("settings_set", {"class": S, "values": {"LogLevel": "Calls"}})
check("settings_set LogLevel Calls", r["ok"], r)
c("asset_exists", {"path": ROOT})
r = c("log_read", {"pattern": "Enginewright: call asset_exists -> ok", "limit": 5})
check("Calls level writes 'Enginewright: call <tool> -> ok (n ms)'", r["ok"] and len(r["data"]["lines"]) >= 1, r)
c("settings_set", {"class": S, "values": {"LogLevel": "Quiet"}})
r = c("log_read", {"limit": 1})
seq = r["data"].get("lastSeq", 0)
c("asset_exists", {"path": ROOT})
r = c("log_read", {"pattern": "Enginewright: call asset_exists", "since": seq})
check("Quiet level writes no call lines", r["ok"] and len(r["data"]["lines"]) == 0, r)
# 0.5.2: colour-change calls had no level bits and came back as "Log" lines through every minVerbosity filter.
r = c("log_read", {"minVerbosity": "Warning", "limit": 200})
check("log_read minVerbosity Warning returns only Warning/Error/Fatal lines",
      r["ok"] and all(l.get("verbosity") in ("Warning", "Error", "Fatal") for l in r["data"]["lines"]), r)
# 0.5.2: number bounds from the schema are enforced (negative maxWidth went through).
r = c("editor_capture", {"target": "panel", "inline": False, "maxWidth": -5})
check("editor_capture negative maxWidth -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)

# --- editor UI capture: panel and main window (with the status bar)
for target in ("panel", "window"):
    r = c("editor_capture", {"target": target, "inline": False}, timeout=60)
    f = r.get("data", {}).get("file", "")
    check(f"editor_capture {target}: PNG written", r["ok"] and os.path.isfile(f) and r["data"]["width"] > 200 and r["data"]["height"] > 100, r)
    if os.path.isfile(f):
        os.remove(f)
r = c("editor_capture", {"target": "dock"})
check("editor_capture bad target -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] in ("INVALID_ARGUMENT", "INVALID_VALUE"), r)

put_back()
r = c("settings_get", {"class": S})
v = r["data"]["values"]
check("settings put back", v["bReadOnly"] is False and v["DisabledCategories"] == [] and v["LogLevel"] == "Quiet", v)
done()
