"""text_overflow_audit tests (EW-055): a text that overflows and texts that must NOT be reported (Auto Wrap, ScaleBox, fits).

Needs a running editor on ENGINEWRIGHT_TEST_PROJECT. Writes only under /Game/_EWTest.
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ewtest import ROOT, call as c, check, delete_if_exists, done, ensure_stopped

ensure_stopped()
W = ROOT + "/WBP_EWTextAudit"
delete_if_exists(W)
LONG = "A very long caption that cannot fit into the box"

r = c("widget_create", {"path": W, "root": "VerticalBox"})
check("widget_create vertical root", r["ok"], r)
root = r["data"]["root"] if r["ok"] else ""


def boxed(name, text, extra=None, scale=False):
    """SizeBox 160 px wide, left aligned; optionally a ScaleBox inside; one TextBlock."""
    r1 = c("widget_add", {"path": W, "class": "SizeBox", "name": name + "Box", "parent": root,
                          "properties": {"WidthOverride": 160, "bOverride_WidthOverride": True}, "slot": {"HorizontalAlignment": "HAlign_Left"}})
    parent = name + "Box"
    if scale:
        c("widget_add", {"path": W, "class": "ScaleBox", "name": name + "Scale", "parent": parent})
        parent = name + "Scale"
    props = {"Text": text}
    props.update(extra or {})
    r2 = c("widget_add", {"path": W, "class": "TextBlock", "name": name, "parent": parent, "properties": props})
    return r1["ok"] and r2["ok"], (r1, r2)


for name, text, extra, scale in [("Cut", LONG, None, False), ("Wrapped", LONG, {"AutoWrapText": True}, False),
                                 ("Scaled", LONG, None, True), ("Short", "OK", None, False),
                                 ("LongWord", "Supercalifragilisticexpialidocious", {"AutoWrapText": True}, False)]:
    ok, d = boxed(name, text, extra, scale)
    check(f"build {name}", ok, d)

r = c("text_overflow_audit", {"path": W, "includeFits": True})
d = r.get("data") or {}
check("audit runs, one culture", r["ok"] and len(d.get("cultures", [])) == 1, r)
rows = {x["widget"]: x for x in (d["cultures"][0]["all"] if r["ok"] else [])}
check("5 TextBlocks measured", len(rows) == 5, list(rows))
check("non-wrapping long text -> overflow", rows.get("Cut", {}).get("verdict") == "overflow" and rows["Cut"]["overBy"] > 50, rows.get("Cut"))
check("Auto Wrap text -> wraps, NOT an overflow (no false positive)", rows.get("Wrapped", {}).get("verdict") == "wraps", rows.get("Wrapped"))
check("text in a ScaleBox -> scaled, NOT an overflow (no false positive)", rows.get("Scaled", {}).get("verdict") in ("fits", "scaled") and rows["Scaled"]["scaled"], rows.get("Scaled"))
check("short text fits", rows.get("Short", {}).get("verdict") == "fits", rows.get("Short"))
check("a word wider than the slot overflows even with Auto Wrap", rows.get("LongWord", {}).get("verdict") == "overflow", rows.get("LongWord"))
check("overflow list = Cut + LongWord, verdict overflow", sorted(x["widget"] for x in d["cultures"][0]["overflows"]) == ["Cut", "LongWord"]
      and d["overflowTotal"] == 2 and d["verdict"] == "overflow", d.get("cultures"))

LANG0 = c("loc_culture")["data"]["language"]
# 1 screen x 2 languages, with screenshots to compare with the report
r = c("text_overflow_audit", {"path": W, "cultures": ["en", "de"], "screenshots": True})
d = r.get("data") or {}
cs = d.get("cultures", [])
check("2 cultures reported, each with a screenshot file", r["ok"] and [x["culture"].split("-")[0] for x in cs] == ["en", "de"]
      and all(os.path.isfile(x.get("screenshot", "")) for x in cs), r)
check("both cultures count the same overflows when no translation exists", len(cs) == 2 and cs[0]["overflowCount"] == cs[1]["overflowCount"] == 2, cs)
check("language restored afterwards", c("loc_culture")["data"]["language"] == LANG0, LANG0)

# errors
r = c("text_overflow_audit", {"path": W, "cultures": ["xx-nonsense"]})
check("unknown culture -> UNKNOWN_CULTURE", not r["ok"] and r["errorCode"] == "UNKNOWN_CULTURE", r)
r = c("text_overflow_audit", {"path": ROOT + "/WBP_Nope"})
check("missing widget -> ASSET_NOT_FOUND", not r["ok"] and r["errorCode"] == "ASSET_NOT_FOUND", r)
r = c("text_overflow_audit", {})
check("no path and no PIE -> NOT_PLAYING", not r["ok"] and r["errorCode"] == "NOT_PLAYING", r)
r = c("text_overflow_audit", {"path": W, "width": 4})
check("bad size -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)
r = c("text_overflow_audit", {"path": W, "cultures": ["en"] * 9})
check("more than 8 cultures -> INVALID_ARGUMENT", not r["ok"] and r["errorCode"] == "INVALID_ARGUMENT", r)

delete_if_exists(W)
done()
