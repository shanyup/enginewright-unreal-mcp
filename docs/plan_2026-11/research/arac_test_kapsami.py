"""Araç envanterine test ve RELIABILITY kapsamını ekler (yalnız metin okur).

Girdi: research/arac_envanteri.json, tests/*.py, docs/RELIABILITY_0.4.md
Çıktı: research/arac_test_kapsami.json (+ ekrana özet)
"""
import glob, json, os, re

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))

inv = json.load(open(os.path.join(HERE, "arac_envanteri.json"), encoding="utf-8"))
tools = inv["tools"] if isinstance(inv, dict) else inv
tests = {os.path.basename(p): open(p, encoding="utf-8", errors="replace").read()
         for p in glob.glob(os.path.join(ROOT, "tests", "*.py"))}
rel = open(os.path.join(ROOT, "docs", "RELIABILITY_0.4.md"), encoding="utf-8").read()

rel_status = {}
for m in re.finditer(r"^\| `([a-z0-9_]+)` \| (OK|FIXED|NOTE|0\.4\.1) \|", rel, re.M):
    rel_status[m.group(1)] = m.group(2)

out = []
for t in tools:
    n = t["name"]
    pat = re.compile(r"[\"']" + re.escape(n) + r"[\"']")
    hits = sorted(f for f, s in tests.items() if pat.search(s))
    out.append({**t, "tests": hits, "reliability": rel_status.get(n, "-")})

json.dump(out, open(os.path.join(HERE, "arac_test_kapsami.json"), "w", encoding="utf-8"),
          ensure_ascii=False, indent=1)

cats = {}
for t in out:
    c = cats.setdefault(t["category"], [0, 0, 0, 0])
    c[0] += 1
    c[1] += bool(t["tests"])
    c[2] += t["reliability"] != "-"
    c[3] += bool(t.get("writes"))
print("toplam", len(out), "| testli", sum(bool(t["tests"]) for t in out),
      "| RELIABILITY'de", sum(t["reliability"] != "-" for t in out))
for k, v in cats.items():
    print(f"{k:14s} arac={v[0]:3d} testli={v[1]:3d} rel={v[2]:3d} yazar={v[3]:3d}")
print("TESTSIZ:", " ".join(t["name"] for t in out if not t["tests"]))
print("REL DISI:", " ".join(t["name"] for t in out if t["reliability"] == "-"))
