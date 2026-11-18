# OZET_2 — Enginewright tam sistem yol haritası (1 sayfa)

18.11.2026 · Kart `t_ae3a845a` · Yalnız belge. Yerini aldığı: `OZET.md` yol haritası kısmı (yayın kısmı OZET'te kalır).
**Genel MCP kuralı (Talha 18.11, kart `t_5f61e99c`):** sürüm adları genel yetenek adıdır; oyun adı yalnız "(iç)" sütunlarda. Denetim: `research/oyun_adi_denetim.py` → 0.
Ayrıntı: 06 ihtiyaç · 07 eşleme · 08 yol haritası · 09 mimari/kalite · SORULAR_2.

> **Ara sürüm listesi: `10_Ara_Surumler.md`** (18.11). 55 ara sürüm (0.4.1 → 1.0.0), her biri ≤ 12 sa TAHMİN, toplam ≈ 538 sa. İş sırası artık 10'dan alınır; aşağıdaki "İlk 10 iş" 10'daki 0.4.1–0.5.2 ile aynı işleri kapsar (10'da ek olarak 0.5.1 `perf_capture` + `scope`).

## Sayılar

| | |
|---|---|
| Araç (kaynak koddan) | **129** (109 `R.Register` + 16 `R.RegisterAsync` + 4 `Reg.RegisterAsync`); 122 testli, 7 testsiz |
| Üç oyunun birleşik ihtiyacı | **56** (EW-001…111); 7 ortak (3 oyun), 25 ikili, 24 tekil |
| Bugünkü durum | **Var 18 · Kısmen 20 · Yok 18** |
| Eski numara eşlemesi | 123 (EW-R 22, GP yeni 9 + eski 5, ARPG 11, A/G/R 36, K 10, EKSİK 30) |
| Kapsam denetimi | Kısmen/Yok ihtiyaçların hepsi bir sürümde (`research/yol_kapsam.py`: sürümsüz 0) |

## Sürümler (tarih yok, sıra var; efor TAHMİN)

| Seri | Tema | İlk kullanan oyun / açtığı oyun işi (iç) | TAHMİN |
|---|---|---|---|
| 0.4.x | Kararlılık + veri aktarımı ve UI metin denetimi | RC Demo D1 (IS-10, LOC-4/5) | ≈ 35 sa |
| 0.5.x | YZ test ve ölçüm paketi (9 araç) | GP YZ-0 kapı G0…G3 | ≈ 55 sa |
| 0.6.x | Paketleme, senaryo ve bot testi (paket, senaryo, bot, ağ, beyan) | RC IS-31…35, GP Demo | ≈ 52 sa |
| 0.7.x | İskelet, animasyon ve malzeme hattı | ARPG F0 (VARSAYIM) | ≈ 8–10 kart-gün |
| 0.8.x | Oynanış yazarlığı (GAS, StateTree, BT, AnimBP) | ARPG F1; GP StateTree kullanabilir | ≈ 12–15 kart-gün |
| 0.9.x | Seviye, VFX, Sequencer + headless | ARPG F1→F3 | ≈ 18–21 kart-gün |
| 1.0.x | Dondurma + cila; **YAYIN İŞİ** yalnız kilometre taşı (kapsam dışı) | Gerçek projelerde kanıt koşusu (RC, GP, ARPG) | ≈ 40 sa |

Cila şeridi (ayrı): Y1 kararlılık → Y2 test standardı → Y3 betikler → Y4 CI → Y5 belge → Y6 örnek → Y7 çoklu UE → Y8 headless → Y9 API dondurma → Y10 yayın işi.

## İlk 10 iş (sırayla)

| # | İş | İhtiyaç / borç | İlk kullanan oyun / açtığı oyun işi (iç) | TAHMİN |
|---|---|---|---|---|
| 1 | `text_overflow_audit` (UI metin taşması) | EW-055 (tek "Yok" P0) | RC LOC-4 → LOC-5 | 8 sa |
| 2 | `datatable_import` (CSV/JSON upsert + dryRun) | EW-002 | RC IS-10 | 6 sa |
| 3 | Gerçek proje deneme koşusu: "Var" araçları gerçek veriyle | EW-001, 005, 006, 050–053, 060, 061, 111 | RC Demo D1 güveni | 4 sa |
| 4 | ASCII dışı ad uyarısı + `pie_start` bekleme | T-04, T-09 | RC import, IS-33 | 4 sa |
| 5 | 7 testsiz araca test + 5 RELIABILITY satırı | T-01, T-02 | GP G1 perf güveni | 7 sa |
| 6 | `game_run` saklama, `asset_find` uyarısı, `actor_preview` çerçeve | T-05, T-03, T-06 | hepsi | 6 sa |
| 7 | `automation_run` headless + rapor | EW-011 | GP G0 | 5 sa |
| 8 | `metrics_read` (eşik + kapı özeti) | EW-014 | GP G0, RC IS-32, ARPG IC-12 | 4 sa |
| 9 | `nav_build_check` | EW-090 | GP G1, ARPG IC-10 | 6 sa |
| 10 | `vislog_extract` | EW-015 | GP G1 | 8 sa |

## Kritik yol

```
0.4.x (1–6) ─► 0.5.x test kapısı (7,8) ─► vislog, gdt, nav, scope, paketli FPS ─► StateTree izi ─► soak ─► YZ test paketi tam
                                        └─► 0.6.x package_build ─► paketli demo build
0.6.x ─► 0.7.x skeletal_import ─► 0.8.x GAS + StateTree ─► oynanış yazarlığı tam
(iç: YZ test paketi tam = GP YZ-0 başlar · paketli demo build = RC IS-31/IS-35 Next Fest · 0.8.x sonu = ARPG F1 prototip, T1/T4 VARSAYIM)
```

En büyük riskler: GP YZ-0 kodu yokken araç kabulü (S2-02); iskelet/retarget ve StateTree editör API'leri sürüme bağlı (0.7/0.8, TAHMİN); `package_build` editörü kilitleyebilir (0.6).

## Açık sorular (SORULAR_2, 14 soru — öneri ilk sırada)

Önce cevaplanmalı: **S2-01** sürüm sırası · **S2-02** GP araç kabulü · **S2-12** ARPG T1/T3/T4 varsayımları.
Diğerleri: S2-03 GP numaraları · S2-04 oyun belgesi eski notları · S2-05 ✅ kapandı (genel araç) · S2-06 ✅ kapandı (A, genel ad) · S2-07 0.6 bölme · S2-08 CI · S2-09 UE sürümleri · S2-10 03'ten kalan araçlar · S2-11 09 kuralları · S2-13 `scenario_place` · S2-14 kanıt kayıtları yeri.
