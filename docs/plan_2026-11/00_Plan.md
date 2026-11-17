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
- [ ] SORULAR_2.md
- [ ] OZET_2.md
- [ ] Eski belgelere "yerini 06/07/08 aldı" notu (02, 03_Yol_Haritasi, 03b)
