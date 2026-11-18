
## 7. İhtiyaç sırası — ilk kullanan oyun açısından, tarih yok (iç eşleme)

Oyunun ilk kullandığı ana göre. Ayrıntılı sürüm sırası 08'de.

```
GP YZ-0 öncesi (GP16: "hepsi YZ-0'dan önce")         RC Stil kilidi + Demo D1 (★ yol)            ARPG F0 (ön-prodüksiyon, VARSAYIM T4)
  G0: EW-010/011 automation, EW-014 metrik oku           IS-03: EW-060/061/063 kare + ölçüm           EW-071 iskelet+anim (AT EW-1)
      EW-019 senaryo yerleşimi (isteğe bağlı, S11)       IS-10: EW-001/002/005 veri + ID              EW-072 malzeme varyantı (EW-2)
  G1: EW-015 vislog, EW-016 GDT, EW-090 navmesh,         IS-13: EW-070/072/073 import + MI            EW-082 GAS (EW-5)
      EW-040/041/042 perf + scope + -game                EK-03/IS-21: EW-010/012 kural testleri       EW-080 StateTree (EW-3)
  G2: EW-017 StateTree izi                               LOC-1…4: EW-050…055 (EW-055 tek "Yok" P0)    EW-081 AnimBP (EW-4)
  G3: EW-018 soak                                        IS-30: EW-074 yer tutucu ★                 ARPG Prototip (F1)
GP Demo (GP09)                                           IS-31/33: EW-042/043, EW-030/032             EW-001/002/004 veri + sim, EW-091/090 oda+nav
  GI-07: EW-030/031 ağ                                   IS-32: EW-013 bot, EW-004 denge              EW-013/014 bot + ölçüm
  GI-29: EW-004 sim köprüsü                              IS-34: EW-006 doğrulama, EW-111 tuş        ARPG Dikey dilim → Demo → 1.0
                                                         IS-35: EW-075 beyan                          EW-083 Niagara, EW-075 beyan, EW-007, EW-110, EW-100
                                                       RC EA / 1.0: EW-020, EW-064, EW-100…102, EW-071 (Garmr)
```

Ortak yükler (birden çok oyunun ilk anında): EW-001 veri, EW-013/014 bot + ölçüm, EW-042 paketli FPS, EW-070 import, EW-090 navmesh.

## 8. Açık noktalar (SORULAR_2'ye taşındı)

- RC12 §11.1 EW-R20 "Kısmen / B11 yok" diyor; kodda `validate` var (ImportValidationTools.cpp:235). RC belgesi eski → S2-04.
- GP16 EW-GP-01 `automation_run`'ı "eksik" sayıyor; araç var (editör içi). Eksik kısım başsız koşu + XML ayrıştırma (EW-011) → S2-04.
- EW-080 StateTree yazma: genel araçtır, her oyun kullanabilir. GP StateTree kullanıyor (YZ_v2/02 §3); şu an yalnız `statetree_debug_dump` (EW-017) istedi. **S2-05 kapandı (genel araç, Talha 18.11).**
- EW-032 iki makine / Steam testi araçla çözülmez (TAHMİN); elle kalır.
