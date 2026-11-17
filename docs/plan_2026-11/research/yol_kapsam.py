# -*- coding: utf-8 -*-
"""08 kapsam denetimi: her ihtiyaç (06) hangi sürüm bölümünde geçiyor?
tpl_08.md → 08_Yol_Haritasi_v2.md (YOL_KAPSAM_TABLOSU yerine tablo koyar).
Kısmen/Yok ihtiyaç hiçbir sürümde yoksa çıkış kodu 1.
"""
import json, os, re, sys
H = os.path.dirname(os.path.abspath(__file__))
P = os.path.dirname(H)
tpl = open(os.path.join(H, "tpl_08.md"), encoding="utf-8").read()
need = json.load(open(os.path.join(H, "ihtiyac_esleme.json"), encoding="utf-8"))

# yalnız sürüm bölümleri (## 2 … ## 9); §1 akış, §10, §11 sayılmaz
bolum = {}
for m in re.finditer(r"^## (\d+)\. (.+)$", tpl, re.M):
    bolum[int(m.group(1))] = (m.start(), m.group(2))
sira = sorted(bolum)
ad = {2: "0.4.1", 3: "0.5", 4: "0.6", 5: "0.7", 6: "0.8", 7: "0.9", 8: "1.0", 9: "Sonrası"}
kap = {}
for i, n in enumerate(sira):
    if n not in ad:
        continue
    bas = bolum[n][0]
    son = bolum[sira[i + 1]][0] if i + 1 < len(sira) else len(tpl)
    for idn in set(re.findall(r"EW-\d{3}", tpl[bas:son])):
        kap.setdefault(idn, []).append(ad[n])

o = ["| ID | İhtiyaç (kısa) | 07 durumu | Sürüm(ler) |", "|---|---|---|---|"]
eksik = []
for n in need:
    s = kap.get(n["id"], [])
    if not s and n["durum"] != "Var":
        eksik.append(n["id"])
    o.append(f"| {n['id']} | {n['ihtiyac'][:60]}{'…' if len(n['ihtiyac'])>60 else ''} | {n['durum']} | {', '.join(s) if s else ('— (Var; 1.0 kanıt koşusu)' if n['durum']=='Var' else '**YOK**')} |")
ozet = (f"\nSonuç: {len(need)} ihtiyaçtan {sum(1 for n in need if n['id'] in kap)} tanesi en az bir sürüm bölümünde geçiyor. "
        f"Kısmen/Yok olup sürümsüz kalan: {len(eksik)} {eksik if eksik else ''}.\n"
        "\"Var\" olup sürümde geçmeyenler 1.0 kanıt koşusunda doğrulanır (§8).\n")
out = tpl.replace("YOL_KAPSAM_TABLOSU", "\n".join(o) + "\n" + ozet)
open(os.path.join(P, "08_Yol_Haritasi_v2.md"), "w", encoding="utf-8").write(out)
print(ozet.strip())
sys.exit(1 if eksik else 0)
