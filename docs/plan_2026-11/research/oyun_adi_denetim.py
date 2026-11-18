# -*- coding: utf-8 -*-
"""Oyun adı denetimi (genel MCP kuralı, Talha 18.11 · kart t_5f61e99c).

Kural: sürüm/ara sürüm başlıkları, hedef cümleleri, araç adları ve açıklamaları,
kabul ölçütleri, kilometre taşları ve şema (Mermaid/ASCII) etiketlerinde oyun adı geçmez.
Oyun bağlantısı yalnız "(iç)" etiketli iç planlama alanlarında kalır.

Denetlenen satır türleri:
  başlık  : `#` ile başlayan satırlar
  şema    : ``` çitli blok içindeki satırlar (Mermaid + ASCII)
  madde   : `- **Hedef:**`, `- **Araç / düzeltme:**`, `- **Kabul ölçütü…:**`, `**Tema:**` maddeleri
  tablo   : tablo başlık satırı + başlığı KAPSAM'a uyan sütunların hücreleri
            (başlığında "(iç)" geçen sütun hariç)
  muaf    : başlığında "(iç" geçen `##` bölümünün şema satırları (06 iç eşleme bölümleri;
            başlığın kendisi yine denetlenir)
Yalnız okur. Çalıştır: python research/oyun_adi_denetim.py  (çıkış kodu: ihlal varsa 1)
"""
import re, sys, pathlib

KOK = pathlib.Path(__file__).resolve().parent.parent
BELGELER = ["00_Plan.md", "06_Ihtiyac_Birlesik.md", "07_Arac_Esleme.md", "08_Yol_Haritasi_v2.md",
            "09_Mimari_ve_Kalite.md", "10_Ara_Surumler.md", "OZET_2.md"]
OYUN = re.compile(r"RagnaCook|Gunpoint|Parole|ARPG|(?<![A-Za-z0-9_])RC(?![A-Za-z])|(?<![A-Za-z0-9_])GP(?![A-Za-z])")
KAPSAM = re.compile(r"^(Sürüm|Sürüm\(ler\)|Ara sürüm|Seri|Ad|Tema|Hedef|Araç.*|Yeni araç.*|Kabul.*|Kilometre taşı.*|Sıra|İş|Nerede|Sözleşme.*|Cila)$")
MADDE = re.compile(r"^\s*(- )?\*\*(Hedef|Araç / düzeltme|Kabul ölçütü[^*]*|Tema):?\*\*")


def hucreler(satir):
    return [h.strip() for h in satir.strip().strip("|").split("|")]


def denetle(yol):
    L = yol.read_text(encoding="utf-8").split("\n")
    say = {"başlık": 0, "şema": 0, "madde": 0, "tablo": 0}
    satir_say = {"başlık": 0, "şema": 0, "madde": 0, "tablo": 0}
    ihlal = []
    kod = False
    basl = None  # aktif tablo başlığı
    ic = False   # başlığında "(iç" geçen bölüm: şema/tablo satırları muaf (06 iç eşleme)
    for i, s in enumerate(L):
        n = i + 1
        if s.lstrip().startswith("```"):
            kod = not kod
            continue
        if kod and ic:
            continue
        if kod:
            satir_say["şema"] += 1
            k = len(OYUN.findall(s))
            if k:
                say["şema"] += k; ihlal.append(("şema", n, s))
            continue
        if s.startswith("#"):
            if s.startswith("## ") or s.startswith("# "):
                ic = "(iç" in s
            satir_say["başlık"] += 1
            k = len(OYUN.findall(s))
            if k:
                say["başlık"] += k; ihlal.append(("başlık", n, s))
            continue
        if MADDE.match(s):
            satir_say["madde"] += 1
            k = len(OYUN.findall(s))
            if k:
                say["madde"] += k; ihlal.append(("madde", n, s))
            continue
        if s.startswith("|"):
            if i + 1 < len(L) and re.match(r"^\|\s*:?-{3}", L[i + 1]):
                basl = hucreler(s)
                satir_say["tablo"] += 1
                k = len(OYUN.findall(s))
                if k:
                    say["tablo"] += k; ihlal.append(("tablo", n, s))
                continue
            if re.match(r"^\|\s*:?-{3}", s) or basl is None:
                continue
            hs = hucreler(s)
            for j, h in enumerate(hs):
                b = basl[j] if j < len(basl) else ""
                if "(iç)" in b or not KAPSAM.match(b):
                    continue
                satir_say["tablo"] += 1
                k = len(OYUN.findall(h))
                if k:
                    say["tablo"] += k; ihlal.append(("tablo", n, f"[{b}] {h}"))
        else:
            basl = None
    return say, satir_say, ihlal


def main():
    # --once <commit>: aynı denetimi o commit'teki belgelere uygular (dedektör kanıtı)
    once = sys.argv[sys.argv.index("--once") + 1] if "--once" in sys.argv else None
    if once:
        import subprocess, tempfile, os
        git = os.environ.get("GIT", "git")
        tmp = pathlib.Path(tempfile.mkdtemp())
        for b in BELGELER:
            out = subprocess.run([git, "-C", str(KOK), "show", f"{once}:./{b}"], capture_output=True)
            (tmp / b).write_bytes(out.stdout if out.returncode == 0 else b"")
        kok = tmp
        print(f"Denetim: commit {once} (kart başı, karşılaştırma)\n")
    else:
        kok = KOK
        print("Denetim: çalışma ağacı\n")
    toplam = 0
    print("| Belge | Başlık | Şema | Madde (hedef/araç/kabul) | Tablo (kapsam sütunları) | Denetlenen satır/hücre | Oyun adı toplam |")
    print("|---|---|---|---|---|---|---|")
    tum = []
    for b in BELGELER:
        say, ss, ih = denetle(kok / b)
        t = sum(say.values())
        toplam += t
        tum += [(b,) + x for x in ih]
        print(f"| {b} | {say['başlık']} | {say['şema']} | {say['madde']} | {say['tablo']} | {sum(ss.values())} | **{t}** |")
    print()
    print(f"TOPLAM oyun adı (kapsam satırlarında): {toplam}")
    for b, tur, n, s in tum:
        print(f"  {b}:{n} [{tur}] {s[:160]}")
    return 1 if toplam else 0


if __name__ == "__main__":
    sys.exit(main())
