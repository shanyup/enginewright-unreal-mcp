# OZET — Enginewright Plan Paketi (1 Sayfa)

**Tarih:** 17.11.2026 **Repo:** github.com/shanyup/enginewright **v0.4.0** **10★** **MIT** **Public**

---

## Konumlandırma Cümlesi

**Enginewright = "Playwright for Unreal Engine"** — Unreal Engine 5.8 editörü içinde çalışan, **strict / verified / undoable** MCP sunucusu. Python/Node gerektirmez. AI ajanlara (Claude Code, Codex, Cursor, VS Code, Hermes) 129 araç verir: PIE kontrolü, paketli oyun çalıştırma (`game_run`), çok oyunculu test, görsel QA (SSIM + compose), lokalizasyon (PO/locres + font coverage), asset pipeline, atomik batch rollback, tek komutla client config. Gerçek oyun projelerinde (RagnaCook, ARPG, GunpointParole) doğrulanıyor.

---

## İlk 10 İş (Yol Haritası + Duyuru Karışık, Sıralı)

| # | İş | Ait Olduğu Dosya | Süre / Hedef |
|---:|---|---|---|
| 1 | **v0.4.1 "Sanat hattı kapat"**: `asset_pipeline`, `asset_import_batch`, `skeletal_import`, `image_diff`, `material_variants` → RagnaCook M1-5/M2-5b demo çekimi | `03_Yol_Haritasi.md` v0.4.1 | 2 hafta (~30 saat) |
| 2 | **Repo rename**: `enginewright` → `enginewright-unreal-mcp` + GitHub yönlendirme; 20 topic ekle; sosyal önizleme; açıklama 348 krk | `04_Kesfedilebilirlik_SEO.md` §2–4 | v1.0 lansmanında |
| 3 | **README hero + demo GIF + 6 video senaryosu** (PIE MP, game_run, A/B görsel, Loc QA, Sanat hattı, Batch rollback) | `04_Kesfedilebilirlik_SEO.md` §5, `05_Duyuru_ve_Yildiz_Buyume_Plani.md` §3 | Lansman öncesi |
| 4 | **Registry / Liste kayıtları**: MCP Registry → awesome-mcp-servers (Glama badge) → Glama/Smithery/mcp.so | `04_Kesfedilebilirlik_SEO.md` §7 | Lansman günü + 1 hafta |
| 5 | **GitHub Pages dokümantasyon sitesi** (EN, 10 anahtar kelime, tools/guides/changelog) | `04_Kesfedilebilirlik_SEO.md` §8 | v1.0 Release PR |
| 6 | **v0.5 "Demo/Çekim/AI"**: `package_build`, `screenshot_tour`, `visual_review`, `coop_test`, `pie_scenario`, `statetree_*`, `bt_*`, `animbp_template`, `travel_test_suite` | `03_Yol_Haritasi.md` v0.5 | 3–4 hafta (~55 saat) |
| 7 | **90 günlük duyuru takvimi başlat**: HN Show HN, r/unrealengine, r/gamedev, X thread, YouTube Shorts, dev.to, Product Hunt, 3 showcase dalgası (RagnaCook, ARPG, v1.1) | `05_Duyuru_ve_Yildiz_Buyume_Plani.md` §2 | Lansman gününden itibaren |
| 8 | **v0.6 "İçerik/Ağ/API freeze"**: `datatable_csv`, `world_inventory`, `actor_set_props`, `scene_snapshot/diff`, `net_test_2pc`, `replication_check`, `log_analyze`, `level_flow_test`, `physics_test`, `headless_import`, `asset_metadata_set` | `03_Yol_Haritasi.md` v0.6 | 4–5 hafta (~65 saat) |
| 9 | **v1.0 Launch + Fab listing UE 5.8**: Release zip, CHANGELOG, GitHub Pages canlı, Fab free code plugin PR, issue templates + good-first-issue, Discussions aç | `03_Yol_Haritasi.md` 1.0, `04_Kesfedilebilirlik_SEO.md` §9 | v0.6 sonrası 2–3 hafta |
| 10 | **SORULAR.md 10 kararın Talha onayı** → ilgili dosyaları kilitle (repo adı, Fab, headless, Python, multi-UE, Discord, Pages, lisans, telemetri, ücretli, v0.4.1 takvim) | `SORULAR.md` | Lansman öncesi |

---

## Sürüm Takvimi (Tahmini)

| Sürüm | Tema | Ana Araçlar | Tahmini Süre | Test Oyunu |
|---|---|---|---|---|
| **v0.4.1** | Sanat hattı | `asset_pipeline`, `asset_import_batch`, `skeletal_import`, `image_diff`, `material_variants` | **2 hafta** | RagnaCook |
| **v0.5** | Demo/Çekim/AI | `package_build`, `screenshot_tour`, `visual_review`, `coop_test`, `pie_scenario`, `statetree_*`, `bt_*`, `animbp_template`, `travel_test_suite` | **3–4 hafta** | GP + RC |
| **v0.6** | İçerik/Ağ/Freeze | `datatable_csv`, `world_inventory`, `actor_set_props`, `scene_snapshot/diff`, `net_test_2pc`, `replication_check`, `log_analyze`, `level_flow_test`, `physics_test`, `headless_import`, `asset_metadata_set` | **4–5 hafta** | ARPG + GP |
| **1.0** | Launch + SEO | Release, Registry, Pages, Fab, Demo videoları, Duyuru takvimi | **2–3 hafta** | Tümü (showcase) |
| **Sonra** | 5.4–5.8, 5.9/6.0, Hosted, Discord?, Pro? | `SORULAR.md` S-1..S-9 | Kapasiteye göre | — |

---

## Yıldız Hedefleri (Rakip Verisine Dayalı)

| Aşama | Gün | Hedef ★ | Gerekçe |
|---|---:|---:|---|
| **Lansman patlaması** | 30 | **80–150** | db-lyon 9 ay 505★ (ay 56), ChiR24 14 ay 1.043★ (ay 74); demo videoları + registry + HN + Reddit |
| **Organik büyüme** | 90 | **300–600** | 3 showcase dalgası (RagnaCook, ARPG, v1.1) + sürekli release |
| **Niş liderliği** | 180 | **800–1.500** | ChiR24 seviyesine yakınlaşma; Epic resmi MCP ile farklılaşma (oyun iş akışları) |

**Ölçüm:** star-history.com, GitHub Insights Traffic/Referrers, haftalık `gh api` stargazer çekimi.

---

## Kritik Kararlar (SORULAR.md — Talha Onayı Bekliyor)

| Soru | Opus Önerisi | Etki |
|---:|---|---|
| **S-0 Repo adı** | `enginewright-unreal-mcp` | Keşfedilebilirlik ↑↑ |
| **S-1 Fab listing** | Evet, UE 5.8 | Yeni kitle ↑ |
| **S-2 Headless** | v0.6 başlangıç, 1.0 tam | ARPG EW-9 erken |
| **S-3 Python exec** | Hayır | Güvenlik ↑ |
| **S-4 Multi-UE** | 1.0: sadece 5.8 | Odak ↑ |
| **S-5 Discord** | Hayır, Discussions | SEO ↑, yük ↓ |
| **S-6 Pages** | v1.0'da canlı | SEO ↑ |
| **S-7 Lisans** | MIT sabit | Topluluk ↑ |
| **S-8 Telemetri** | Hayır | Güven ↑ |
| **S-9 Ücretli** | Hayır, saf OSS | Basitlik ↑ |
| **S-10 v0.4.1 takvim** | 2 hafta tam set | RagnaCook demo kalite ↑ |

---

## Veri Kaynakları ve Doğrulama

- **Rakip verisi:** GitHub REST API 17.11.2026 (`$TMPDIR/ewplan/repos.txt`, canlı arama, fork milestone'ları).
- **Emsal büyüme:** star-history.com (blender-mcp 10k★ May 2025, playwright-mcp 25k+★ ilk yıl), Simon Willison blog, VS Code blog, fork tarihi dağılımı.
- **ARPG ihtiyaçları:** Bitmiş kart çıktısı (t_1ac37fc7) → `ARPG_Tasarim_Kararlari.md` §12.3 (EW-1..11) + `Icerik_Sistemi/08-09`.
- **GP ihtiyaçları:** Çalışan kart (t_e6c46ed7) → `Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` §6.3 (EW-GP-01..09) + `ParoleLoop_TaskList.md` + `GDD_v2_ParoleLoop.md`.
- **RC ihtiyaçları:** Çalışan kart (t_d9c54eb5) → `Demo_Plan/03_Yol_Haritasi.md` + `RagnaCook_TaskList.md` + `tools/art/README.md` + `Gorsel_Inceleme.md` + `Devir_Notu_16-11.md`.
- **Mevcut araç sayımı:** 129 tekil `Register(TEXT("..."))` grep (17.11), `Plugins/` altındaki kayıtlar.
- **Epic UE 5.8 envanteri:** `epic_ue58_inventory.json` → 55 toolset / 870 araç.

---

## Çıktı Dosyaları (8 belge, `docs/plan_2026-11/`)

| Dosya | İçerik |
|---|---|
| `00_Plan.md` | Okuma listesi + her çıktı için checkbox + Notlar (dayanıklılık) |
| `01_Rakip_ve_Emsal_Karsilastirma.md` | 25 repo tablo + emsal olayları + özellik matrisi + önde/geride/kimsede-yok + Epic risk/farklılaşma + fork proxy büyüme |
| `02_Oyun_Ihtiyaclari.md` | ARPG EW-1..11 + GP EW-GP-01..09 + RC EW-RC-01..05 = 19 tekilleştirilmiş araç, saat kazancı, kaç oyuna yarar |
| `03_Yol_Haritasi.md` | v0.4.1→v0.5→v0.6→1.0→Sonra: araç sözleşmeleri, saatler, kabul ölçütü, test oyunu, eski roadmap eşleme, yapmayacaklar |
| `04_Kesfedilebilirlik_SEO.md` | Repo adı S-0, açıklama 348 krk, 20 topic hacimle, README hero EN, sosyal önizleme, registry/liste kayıtları, Pages, Fab, Google keywordler |
| `05_Duyuru_ve_Yildiz_Buyume_Plani.md` | 90 gün takvim (kanal/kural/başlık EN), 6 video saniye saniye, hedefler 80-150/300-600/800-1500, topluluk, ölçüm, YAPMA listesi |
| `SORULAR.md` | 10 karar sorusu (S-0..S-9), 2–3 seçenek, ilk öneri, gerekçe+etki |
| `OZET.md` | Bu dosya — 1 sayfa konumlandırma, ilk 10 iş, sürüm takvimi, yıldız hedefleri, kritik kararlar |

---

## Sonraki Adım

`kanban_request_review(reviewer="default", artifacts=[OZET.md, 03_Yol_Haritasi.md, 05_Duyuru_ve_Yildiz_Buyume_Plani.md mutlak yolları])`

Opus inceler → Talha karar verir → Onaylanan SORULAR işlenir → v0.4.1 kod çalışmasına geçilir (ayrı kartlar).