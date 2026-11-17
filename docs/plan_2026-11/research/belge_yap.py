# -*- coding: utf-8 -*-
"""06 ve 07 belgelerini şablon + üretilmiş tablolardan birleştirir.
Önce: python ihtiyac_esleme.py   Sonra: python belge_yap.py
"""
import os
H = os.path.dirname(os.path.abspath(__file__))
P = os.path.dirname(H)
r = lambda f: open(os.path.join(H, f), encoding="utf-8").read()

parcalar = {
    "06_Ihtiyac_Birlesik.md": ["tpl_06_bas.md", "gen_06_tablo.md", "tpl_06_orta.md", "gen_06_eski.md", "tpl_06_son.md"],
    "07_Arac_Esleme.md": ["tpl_07_bas.md", "gen_07_tablo.md", "tpl_07_orta.md", "gen_07_ters.md", "tpl_07_son.md"],
}
for hedef, liste in parcalar.items():
    if not all(os.path.exists(os.path.join(H, f)) for f in liste):
        print("atlandı (şablon eksik):", hedef)
        continue
    open(os.path.join(P, hedef), "w", encoding="utf-8").write("\n".join(r(f).rstrip("\n") + "\n" for f in liste))
    print("yazıldı:", hedef)
