# 00 — Plan (Enginewright yol haritası + emsal + SEO + duyuru)

Kart: `t_1e95fc46` · Başlangıç: 16.11.2026 · Yazan: Fable 5.2 · Yalnız belge.
Yeniden başlatılırsan: ilk boş kutudan devam et. Her çıktı tek başına yerel commit. **Push yok.**

## Okuma listesi

- [x] `README.md`, `CHANGELOG.md`, `docs/ROADMAP.md`, `ROADMAP_1.0_TASLAK.md`, `ROADMAP_EKSIKSIZ_MCP_2026-11-14.md`, `EKSIKLER_VE_ONCELIK.md`
- [x] `docs/YAYIN.md`, `YAYIN_HAZIRLIK.md`, `good-first-issues.md`, `SUPPORT.md` (baş kısmı)
- [x] `docs/arastirma/derin/01–04` (başlık + künye tabloları; 10.11 tarihli, kaynak doğrulamalı)
- [x] `Plugins/` araç kayıtları (sayım: Notlar)
- [x] ARPG: `ARPG_Tasarim_Kararlari.md` §12, `Icerik_Sistemi/08`, `09`
- [x] Gunpoint Parole: `docs/Icerik_Sistemi/` yalnız `00_Plan.md` içeriyor (kart sürüyor) → `ParoleLoop_TaskList.md` + GDD
- [x] RagnaCook: `Docs/Icerik_Sistemi/` yalnız `00_Plan.md` içeriyor (kart sürüyor) → `Demo_Plan/03_Yol_Haritasi.md`, `tools/art/README.md`, `Gorsel_Inceleme.md`, `Devir_Notu_16-11.md`
- [x] Web + GitHub API: rakip ve emsal sayıları (16.11.2026)
- [x] **İnceleme Turu 1 düzeltmeleri** (17.11): 01 rakip tablosu 25 repo + dolu hücreler + fork proxy büyüme; 02 ARPG EW-1..11 gerçek §12.3 listesi + GP/RC kaynakları yazılı; 03 araç sözleşmeleri saatlerle + eski roadmap eşleme; 04 canlı repo verisi + 20 topic hacimle + README hero EN + repo adı S-0; 05 90 gün takvim + 6 video senaryosu + hedefler 80-150/300-600/800-1500 + YAPMA listesi; SORULAR 10 soru; OZET 1 sayfa.

## Çıktılar

- [x] 00_Plan.md
- [x] 01_Rakip_ve_Emsal_Karsilastirma.md
- [x] 02_Oyun_Ihtiyaclari.md (v2, 17.11 — İçerik Sistemi belgelerinden)
- [x] 03_Yol_Haritasi.md
- [x] 04_Kesfedilebilirlik_SEO.md
- [x] 05_Duyuru_ve_Yildiz_Buyume_Plani.md
- [x] SORULAR.md
- [x] OZET.md

## Notlar

- **Araç sayısı (kaynak sayımı, 16.11):** `Plugins/**/Tools/*.cpp` içinde 133 `Register/RegisterAsync` kaydı (129 düz desen + `game_run`, `game_run_status`, `loc_gather`, `loc_gather_status`). README 0.4.0 "124 tool" dönemi; fark = `mrq_render`, `mrq_status`, `perf_capture`, `perf_scene`, `perf_gpu_profile` + sayım farkı. Yayımlanmış 0.4.0 zip'inde bu 5 araç yok.
- **Dal durumu:** `main` yerelde `origin/main`'den 8 commit önde (8 plan belgesi commit'leri). Bu commit'lerde yalnız `docs/plan_2026-11/` değişiyor. Push edilirse public olur → `SORULAR.md` S-1.
- **GP ve RagnaCook kaynakları:** ikisinin İçerik Sistemi kartı bitmedi. İhtiyaçlar eski belgelerden çıkarıldı. Kartlar bitince `02` yeniden gözden geçirilmeli.
- **02 v2 (17.11, kart t_3d650d3a):** GP `08_Icerik_Uretim_Sistemi.md` §7 ve RC `12_Icerik_Uretim_Sistemi.md` §11 artık var; `02` bunlardan yeniden yazıldı. 36 satır (ARPG 13, GP 7, RC 16), istenen 7 sütun, ortak kümeler K1–K10, mevcut araç işareti (✅ 10 / 🟡 19 / ❌ 7). Kanıtsız v1 GP satırları çıkarıldı.
- **Rakip verisi tarihi:** 16.11.2026 (GitHub REST API). 10.11 derin raporları kaynak kodu okumasıdır; özellik hücreleri oradan, sayılar bugünden.
- **İnceleme Turu 1 tamamlandı:** 8 dosya düzeltilip yerel commitlendi (36cc938, 4eaedf9, 9551f31, f3348ec, 6ea3b60, a96d108, 12eef68). Push yok. Sonraki adım: `kanban_request_review`.

---

## Kart t_ae3a845a — Tam sistem yol haritası (18.11.2026)

Yalnız belge. Oyun repoları salt okunur. Push yok. Alt kart yok. Yeni dosyalar 06+; eski belgeler silinmez.

### Araştırma (research/)

- [x] `arac_envanteri.py` → 129 araç (109 `R.Register` + 16 `R.RegisterAsync` + 4 `Reg.RegisterAsync`; çift ad yok). 16.11 notundaki "133" fazla sayım: `game_run`, `game_run_status`, `loc_gather`, `loc_gather_status` zaten 4 `Reg.RegisterAsync` içinde.
- [x] `arac_test_kapsami.py` → araç başına test dosyası + RELIABILITY_0.4 durumu
- [x] `raw/oyun_*.md` → üç oyunun EW listesi alıntıları
- [x] `ihtiyac_esleme.py` → 56 ihtiyaç, 06/07 tabloları; `belge_yap.py` belgeleri birleştirir

### Çıktılar

- [x] 06_Ihtiyac_Birlesik.md
- [x] 07_Arac_Esleme.md
- [x] 08_Yol_Haritasi_v2.md (kapsam: 56/56, Kısmen/Yok sürümsüz 0 — `research/yol_kapsam.py`)
- [x] 09_Mimari_ve_Kalite.md
- [x] SORULAR_2.md (14 soru)
- [x] OZET_2.md
- [x] Eski belgelere "yerini aldı" notu: 02 (→06/07), 03_Yol_Haritasi (→08/07), OZET (→OZET_2). 03_Emsal ve 03b değişmedi (yerini alan yok).

---

## Kart t_035891ab — Ara sürümler (18.11.2026)

Yalnız belge, yalnız `docs/plan_2026-11/`. Push yok. Alt kart yok. Silme yok.

- [x] `research/ara_surum.py` (üretici + denetim: ≤ 12 sa, EW kapsamı, T-01…T-16, Y1–Y9)
- [x] 10_Ara_Surumler.md (55 ara sürüm, sıralı tablo, tam liste, kritik yol şeması, kilometre taşları)
- [x] 08 ve OZET_2'ye "ara sürüm listesi: 10" notu
- [x] SORULAR_2: ek soru S2-15 (ara sürüm numaraları dışarıda nasıl görünür)

---

## Kart t_5f61e99c — Genel MCP kuralı: oyun adları sürüm dilinden çıktı (18.11.2026)

Talha 18.11: "Temel seviyenin sürüm kodlarında oyunlarımızın adları olmasın. Bu genel bir MCP, herkes için geliştiriliyor. Oyunlar geliştirilirken buna destek oluyorlar." Yalnız belge, yalnız `docs/plan_2026-11/`. Push yok. Alt kart yok. Silme yok.

Seri adları (Talha onaylı): 0.4.x Kararlılık + veri aktarımı ve UI metin denetimi · 0.5.x YZ test ve ölçüm paketi · 0.6.x Paketleme, senaryo ve bot testi · 0.7.x İskelet, animasyon ve malzeme hattı · 0.8.x Oynanış yazarlığı (GAS, StateTree, BT, AnimBP) · 0.9.x Seviye, VFX, Sequencer + headless · 1.0.x Dondurma + cila.

- [x] 10 (`research/ara_surum.py` yeniden yazıldı ve koşuldu): başlık, hedef, kabul genel; "İlk kullanan oyun / açtığı oyun işi (iç)" + "Oyun doğrulaması (iç)"
- [x] 08 (+ `research/tpl_08.md`, `yol_kapsam.py` sürüm etiketi): seri başlıkları, hedef satırları, Mermaid/ASCII, kabul sütunu genel test projesi + ayrı "Oyun doğrulaması (iç)" sütunu
- [x] 07 (şablon `tpl_07_*`): küme yorumu ve güvenilirlik notları genel; oyun yalnız "iç:" notunda
- [x] 06 (şablon `tpl_06_*`): iç eşleme işareti; başlıklar oyunsuz; kaynaklar ve oyun belgesi referansları aynen
- [x] 09: S10 genel ad kuralı KARAR; T3 = oyun doğrulaması (iç)
- [x] OZET_2: seri tablosu + kritik yol genel; oyun işi (iç) sütunu
- [x] SORULAR_2: S2-05 kapandı (genel araç), S2-06 kapandı = A; kapanış satırı
- [x] Denetim: `research/oyun_adi_denetim.py` → `research/oyun_adi_denetim_cikti.md` (kapsam satırlarında oyun adı = 0)

---

## Kart t_c6e43c15 — Yayın hazırlığı: repo adı + konumlandırma + SEO v2 + duyuru v2 (18.11.2026)

Talha 18.11: marka adı Enginewright değişmez; depo adı `enginewright-unreal-mcp` (bu kart yeniden adlandırmaz, yalnız plan); büyük duyuru 0.6.x seri sonu; oyunlar yalnız vitrin. Yalnız belge, yalnız `docs/plan_2026-11/`. GitHub'da değişiklik yok. Push yok. Alt kart yok. Silme yok.

- [x] 03b_Buyume_Hikayeleri commit (önceki karttan commit'siz kalmıştı)
- [x] `research/yayin_hazirlik_kaynaklar_2026-11-18.md` (web notları K1–K6)
- [x] 11_Konumlandirma.md (tek cümle EN + TR, fark tablosu, 3 kanıt, kim için)
- [x] 12_SEO_v2.md (depo adı geçişi 0.4.4, açıklama 342/322/151 kr, 20 topic ölçümlü, README ilk ekran EN, sosyal görsel, 7 dizin + 2 yapılmaz, doküman sitesi 1.0)
- [x] 13_Duyuru_v2.md (hazırlık H1–H18, küçük paylaşım 0.4.4/0.5.6, duyuru günü 10 sıra, D1 45 sn + D2 + D3 saniye saniye, vitrin kuralı, 90 gün, YAPMA 14, yıldız 3 senaryo TAHMİN, ölçüm)
- [x] SORULAR notu (S-0 kapandı; S-2, S-4, S-10 geçersiz; diğerleri taşındı) + SORULAR_3.md (15 soru, öneri ilk sırada)
- [x] 04 / 05 "yerini 12/13 aldı" notu + OZET_2 satırı
