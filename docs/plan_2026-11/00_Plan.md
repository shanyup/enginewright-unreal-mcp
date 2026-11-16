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

## Çıktılar

- [x] 00_Plan.md
- [x] 01_Rakip_ve_Emsal_Karsilastirma.md
- [x] 02_Oyun_Ihtiyaclari.md
- [x] 03_Yol_Haritasi.md
- [x] 04_Kesfedilebilirlik_SEO.md
- [x] 05_Duyuru_ve_Yildiz_Buyume_Plani.md
- [x] SORULAR.md
- [x] OZET.md

## Notlar

- **Araç sayısı (kaynak sayımı, 16.11):** `Plugins/**/Tools/*.cpp` içinde 133 `Register/RegisterAsync` kaydı (129 düz desen + `game_run`, `game_run_status`, `loc_gather`, `loc_gather_status`). README 0.4.0 "124 tool" dönemi; fark = `mrq_render`, `mrq_status`, `perf_capture`, `perf_scene`, `perf_gpu_profile` + sayım farkı. Yayımlanmış 0.4.0 zip'inde bu 5 araç yok.
- **Dal durumu:** `main` yerelde `origin/main`'den 4 commit önde (perf, MRQ, derin rakip raporları, 14.11 roadmap). Bu commit'lerde Türkçe iç belgeler var. Push edilirse public olur → `SORULAR.md` S-1.
- **GP ve RagnaCook kaynakları:** ikisinin İçerik Sistemi kartı bitmedi. İhtiyaçlar eski belgelerden çıkarıldı. Kartlar bitince `02` yeniden gözden geçirilmeli.
- Rakip verisi tarihi: 16.11.2026 (GitHub REST API). 10.11 derin raporları kaynak kodu okumasıdır; özellik hücreleri oradan, sayılar bugünden.
