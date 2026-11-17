# -*- coding: utf-8 -*-
"""Enginewright araç envanteri: Plugins/ kaynak kodundan araç kayıtlarını çıkarır.

Kullanım (repo kökünden):  python docs/plan_2026-11/research/arac_envanteri.py
Çıktı: docs/plan_2026-11/research/arac_envanteri.json + arac_envanteri.md
UE açılmaz, derleme yok; yalnız metin okuma.
"""
import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
SRC = os.path.join(ROOT, "Plugins")
OUT = os.path.dirname(os.path.abspath(__file__))

# Kategori sırası: EnginewrightModule.cpp StartupModule
MODULE = os.path.join(SRC, "Enginewright", "Source", "Enginewright", "Private", "EnginewrightModule.cpp")

REG_RE = re.compile(r'\b(?:R|Reg)\s*\.\s*(Register|RegisterAsync)\s*\(\s*TEXT\("([a-z0-9_]+)"\)\s*,\s*(TEXT\((?:\s*"(?:[^"\\]|\\.)*")+\s*\))', re.S)
FUNC_RE = re.compile(r'^void\s+(Register\w+)\s*\(\s*FEWRegistry', re.M)


def category_map():
    txt = open(MODULE, encoding="utf-8", errors="replace").read()
    body = txt[txt.index("StartupModule()"):]
    cat = None
    res = {}
    for m in re.finditer(r'SetCategory\(TEXT\("(\w+)"\)\)|\b(Register\w+)\(Reg\)', body):
        if m.group(1):
            cat = m.group(1)
        elif m.group(2):
            res.setdefault(m.group(2), cat)
    return res


def main():
    cats = category_map()
    tools = []
    for dirpath, dirs, files in os.walk(SRC):
        if "Intermediate" in dirpath or "Binaries" in dirpath:
            continue
        for f in files:
            if not f.endswith(".cpp"):
                continue
            p = os.path.join(dirpath, f)
            txt = open(p, encoding="utf-8", errors="replace").read()
            funcs = [(m.start(), m.group(1)) for m in FUNC_RE.finditer(txt)]
            for m in REG_RE.finditer(txt):
                line = txt.count("\n", 0, m.start()) + 1
                desc = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(3)))
                desc = desc.replace('\\"', '"')
                owner = None
                for pos, name in funcs:
                    if pos < m.start():
                        owner = name
                rest = txt[m.end():m.end() + 8000]
                se = rest.find(')")')
                rest2 = rest[se + 3:] if se >= 0 else rest
                # bWrites = şemadan sonraki ilk bool argüman
                wb = re.match(r'\s*,\s*(true|false)\b', rest2)
                if not wb:
                    wb = re.search(r'\b(true|false)\b', rest2)
                tools.append({
                    "name": m.group(2),
                    "async": m.group(1) == "RegisterAsync",
                    "writes": (wb.group(1) == "true") if wb else None,
                    "file": os.path.relpath(p, ROOT).replace("\\", "/"),
                    "line": line,
                    "register_fn": owner,
                    "category": cats.get(owner, "?"),
                    "desc": desc[:220],
                })
    tools.sort(key=lambda t: (t["category"], t["file"], t["line"]))
    names = [t["name"] for t in tools]
    dup = sorted({n for n in names if names.count(n) > 1})
    json.dump({"count": len(tools), "duplicates": dup, "tools": tools},
              open(os.path.join(OUT, "arac_envanteri.json"), "w", encoding="utf-8"),
              ensure_ascii=False, indent=1)
    with open(os.path.join(OUT, "arac_envanteri.md"), "w", encoding="utf-8") as fo:
        fo.write("# Araç envanteri (kaynak koddan, otomatik)\n\n")
        fo.write("Üreten: `docs/plan_2026-11/research/arac_envanteri.py`. UE açılmadan, yalnız `Plugins/**/*.cpp` okunarak.\n")
        fo.write("Not: `writes` sütunu Register çağrısındaki ilk true/false'tan sezgisel okunur; şüphede kaynak satırına bak.\n\n")
        fo.write(f"Toplam kayıt: **{len(tools)}** · Yinelenen ad: {dup or 'yok'}\n\n")
        by = {}
        for t in tools:
            by.setdefault(t["category"], []).append(t)
        fo.write("| Kategori | Araç sayısı |\n|---|---|\n")
        for c, ts in by.items():
            fo.write(f"| {c} | {len(ts)} |\n")
        fo.write("\n| # | Araç | Kategori | Async | Yazar | Kaynak | Açıklama (ilk 220 kr) |\n|---|---|---|---|---|---|---|\n")
        for i, t in enumerate(tools, 1):
            d = t["desc"].replace("|", "/")
            fo.write(f"| {i} | `{t['name']}` | {t['category']} | {'E' if t['async'] else ''} | {'E' if t['writes'] else ''} | `{t['file'].split('/')[-1]}:{t['line']}` | {d} |\n")
    print(len(tools), "araç; yinelenen:", dup)


if __name__ == "__main__":
    sys.exit(main())
