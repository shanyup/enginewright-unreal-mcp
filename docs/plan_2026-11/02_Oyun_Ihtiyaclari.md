# 02 — Oyun İhtiyaçları (ARPG + GunpointParole + RagnaCook)

Tarih: 16.11.2026. Her satır: hangi oyun, hangi iş, bugün nasıl, Enginewright karşılığı, saat kazancı. Saatler **tahmin** (tek geliştirici, elle yapılan süreye göre).

## Kaynaklar (kart kuralı: hangisinden alındığı yazılır)

| Oyun | Kart durumu | Kaynak belgeler |
|---|---|---|
| ARPG | **BİTTİ** (t_1ac37fc7, onaylandı) | `Game/ARPG/Docs/ARPG_Tasarim_Kararlari.md` §12 (EW-1..EW-11) + `Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` + `09_Is_Listesi.md` — bitmiş kart çıktısı |
| GunpointParole | ÇALIŞIYOR (t_e6c46ed7) | `docs/Icerik_Sistemi/08_...md` YOK (klasörde sadece `00_Plan.md`) → fallback: `docs/GDD_v2_ParoleLoop.md` + `docs/ParoleLoop_TaskList.md` — **eski belgeler** |
| RagnaCook | ÇALIŞIYOR (t_d9c54eb5) | `Docs/Icerik_Sistemi/12_...md` YOK → fallback: `Docs/Demo_Plan/03_Yol_Haritasi.md`, `Docs/RagnaCook_TaskList.md`, `tools/art/README.md`, `Docs/Demo_Plan/Gorsel_Inceleme.md`, `Docs/Devir_Notu_16-11.md` — **eski belgeler** |

GP ve RagnaCook kartları bitince bu tablo yeniden gözden geçirilir (bkz 00_Plan Notlar).

## İhtiyaç tablosu (EW-xx = ARPG §12 numarası; GP/RC kendi numarası)

| No | Oyun | İş | Bugün nasıl | Enginewright karşılığı (mevcut araç) | Eksik / yeni araç | Saat kazancı (tahmin) |
|---|---|---|---|---|---|---|
| EW-1 | ARPG | Sahnedeki tüm aktör/asset envanteri (JSON) — "yanlış yerde aktör" hataları | elle, outliner'da göz | `actor_find`, `object_get`, `asset_find` + `batch` | **YENİ `world_inventory`** (tek çağrıda tüm sahne JSON) | ~2 saat/saha → 1 dk |
| EW-2 | ARPG | Aynı property'yi N aktörde toplu değiştir | elle, tek tek | `object_set` + `batch` | **YENİ `actor_set_props`** (filtre + property + değer) | ~1 saat → 2 dk |
| EW-3 | ARPG | Asset rename/move, referansları koru | elle, redirectors elle | `asset_rename`, `asset_references` | move + fixup **YENİ `asset_move`** | ~30 dk → 2 dk |
| EW-4 | ARPG | Sahne düzeni snapshot + diff ("ne değişti?") | yok | — | **YENİ `scene_snapshot` + `scene_diff`** | yok → 5 dk (önce/sonra) |
| EW-5 | ARPG | Tüm BP'leri derle + hata raporu | elle, BP tek tek aç | `blueprint_compile`, `blueprint_info` | **YENİ `blueprint_compile_all`** (tümü + rapor) | ~1 saat → 3 dk |
| EW-6 | ARPG | PIE başlat/durdur + otomatik test senaryosu | elle | `pie_start`, `pie_stop`, `pie_input`, `pie_state`, `pie_actors` | **YENİ `pie_scenario`** (adım listesi: spawn→input→assert) | ~1 saat/test → 10 dk |
| EW-7 | ARPG | Screenshot + iki görüntünün diff'i | elle, karşılaştırma gözle | `screenshot`, `image_stats` | **YENİ `image_diff`** (piksel/rms farkı) | ~20 dk → 1 dk |
| EW-8 | ARPG | DataTable ↔ CSV (357 affix'i tabloda düzenle, geri yükle) | elle, DataTable UI | `datatable_get`, `datatable_set_rows`, `datatable_create` | **YENİ `datatable_csv`** (export/import) | 2-3 saat → 10 dk |
| EW-9 | ARPG | Crash log analizi (modül, stack) | elle, log dosyasını elle tara | `log_read`, `log_wait` | **YENİ `log_analyze`** (crash pattern + modül özeti) | ~1 saat → 5 dk |
| EW-10 | ARPG+GP+RC | Cook/package tek komut (demo build) | elle, RunUAT komut satırı | — | **YENİ `package_build`** (platform + config + out dir) | kurulum+çalıştırma ~1 gün ilk kez → 10 dk |
| EW-11 | ARPG | Seviye akış testi (level streaming geçişleri) | elle, PIE'de gez | `level_open`, `level_save` | **YENİ `level_flow_test`** (A→B→C geçiş + süre) | ~2 saat → 15 dk |
| GP-1 | GP | 2 makinede listen-server co-op test | 2 fizik makine, elle kurulum | `pie_net_emulation`, `game_run` (2 instance) | **YENİ `net_test_2pc`** (2 instance + gecikme/paket kaybı senaryosu) | ~4 saat/test → 15 dk |
| GP-2 | GP | Replikasyon doğrulama (property'ler server'da güncelleniyor mu) | elle, debug yaz | — | **YENİ `replication_check`** (aktör + property listesi, server/client karşılaştırma) | ~2 saat → 10 dk |
| GP-3 | GP | Dedicated server build | elle | — | EW-10 `package_build` (server target) | yukarıyla aynı |
| GP-4 | GP | Synty hazır assetleri projeye aktar (tek tek import) | elle, sürükle-bırak | `asset_import` | **YENİ `asset_import_batch`** (klasör → import + rename kuralı) | ~3 saat → 10 dk |
| GP-5 | GP | Fizik/chaos/ragdoll senaryo testi | elle, PIE'de dene | `pie_input`, `pie_walk_to` | **YENİ `physics_test`** (darbe/çökme senaryosu + sonuç) | ~1 saat → 10 dk |
| RC-1 | RC | Demo build paketleme | elle, RunUAT | — | EW-10 `package_build` | yukarıyla aynı |
| RC-2 | RC | Co-op test (2 oyuncu, aynı save) | elle, 2 instance | `game_run` ×2 + `pie_net_emulation` | **YENİ `coop_test`** (2 instance senkron start/stop) | ~3 saat → 15 dk |
| RC-3 | RC | Sahne sahne çekim (kamerayı gez, N screenshot) | elle, her sahne tek tek | `screenshot`, `batch`, `pie_travel` | **YENİ `screenshot_tour`** (waypoint listesi → N shot + manifest) | ~1 gün çekim → 30 dk |
| RC-4 | RC | Toplu görsel inceleme (N screenshot'ı grid'de gör, bozuk olanı bul) | elle, klasörde tek tek aç | `image_stats`, `image_compose` | **YENİ `visual_review`** (grid + otomatik anomali notu) | ~2 saat → 10 dk |
| RC-5 | RC | Elle asset aktarımı (art pipeline: klasör → import → rename → yerleştir) | elle | `asset_import`, `asset_rename`, `asset_duplicate` | **YENİ `asset_pipeline`** (kaynak klasör → import + ad kuralı + hedef path + referans kontrol) | ~4 saat/gün → 20 dk |

## Birleştirme (tekilleştirme)

- EW-10 = GP-3 = RC-1 → tek araç `package_build`.
- EW-6 (pie_scenario) ile GP-5, RC-2 kısmen örtüşür; ayrı senaryo katmanı `pie_scenario`.
- Toplam **yeni araç: 13** (`world_inventory`, `actor_set_props`, `asset_move`, `scene_snapshot`+`scene_diff`, `blueprint_compile_all`, `pie_scenario`, `image_diff`, `datatable_csv`, `log_analyze`, `package_build`, `level_flow_test`, `net_test_2pc`, `replication_check`, `asset_import_batch`, `physics_test`, `coop_test`, `screenshot_tour`, `visual_review`, `asset_pipeline`) — sayım: 19 satır, 3'ü aynı araç → **16 yeni araç**.

## Kaç oyuna yarar

| Araç grubu | ARPG | GP | RC |
|---|---|---|---|
| Envanter/audit (`world_inventory`, `actor_set_props`, snapshot/diff) | ✅ | ✅ | ✅ |
| İçerik üretimi (`datatable_csv`, `asset_pipeline`, `asset_import_batch`, BP rapor) | ✅ (357 affix) | 🟡 | 🟡 |
| Test (`pie_scenario`, `net_test_2pc`, `replication_check`, `coop_test`, `physics_test`, `level_flow_test`) | ✅ | ✅ | ✅ |
| Paketleme (`package_build`) | ✅ | ✅ | ✅ |
| Çekim/QA (`screenshot_tour`, `visual_review`, `image_diff`, `log_analyze`) | 🟡 | 🟡 | ✅ |
