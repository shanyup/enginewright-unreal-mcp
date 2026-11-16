# 02 — Oyun İhtiyaçları (ARPG + GunpointParole + RagnaCook)

Tarih: 17.11.2026. Her satır: hangi oyun, hangi iş, bugün nasıl, Enginewright karşılığı, eksik/yeni araç, saat kazancı (tek geliştirici elle yapmaya göre **tahmin**). EW-xx = ARPG §12.3 numarası; GP = `EW-GP-xx` (GP 08_Icerik_Uretim_Sistemi.md §6.3); RC = `EW-RC-xx` (Demo_Plan/03 + TaskList + tools/art/README + Devir_Notu_16-11).

## Kaynaklar (kart kuralı: hangisinden alındığı yazılır)

| Oyun | Kart durumu | Kaynak belgeler |
|---|---|---|
| ARPG | **BİTTİ** (t_1ac37fc7, onaylandı) | `Game/ARPG/Docs/ARPG_Tasarim_Kararlari.md` **§12.3** (EW-1..11, gerçek liste) + `Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` + `09_Is_Listesi.md` — bitmiş kart çıktısı |
| GunpointParole | ÇALIŞIYOR (t_e6c46ed7) | `docs/Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` **§6.2–6.3** (EW-GP-01..09, hızlanma tablosu + GP-özel eksikler) + `docs/ParoleLoop_TaskList.md` (P2-11 Steam 2 makine, replikasyon, fizik, Synty→kendi asset) + `docs/GDD_v2_ParoleLoop.md` — **eski belgeler + çalışan kart çıktısı** |
| RagnaCook | ÇALIŞIYOR (t_d9c54eb5) | `Docs/Demo_Plan/03_Yol_Haritasi.md` (M0–M9 paketler), `Docs/RagnaCook_TaskList.md`, `tools/art/README.md` (sanat hattı), `Docs/Demo_Plan/Gorsel_Inceleme.md`, `Docs/Devir_Notu_16-11.md` — **eski belgeler + çalışan kart çıktısı** |

> GP ve RagnaCook kartları bitince bu tablo yeniden gözden geçirilmeli (bkz `00_Plan.md` Notlar).

---

## İhtiyaç tablosu (ARPG EW-1..11 §12.3 sırası + GP + RC)

| No | Oyun | İş | Bugün nasıl | Enginewright karşılığı (mevcut araç) | Eksik / yeni araç | Saat kazancı (tahmin) | Kaç oyuna yarar |
|---|---|---|---|---|---|---|---|
| **EW-1** | ARPG | **Sanat hattı 0.4.1** (iskeletli import + anim, IK retarget, soket, Physics Asset, montage/blendspace/notify, bütçe) | Elle, betikler, editör sırası | `asset_import` (meshType), `mesh_info`, `material_*` | **YENİ** `skeletal_import` (hedef skeleton + anim), `socket_create`, `physics_asset_create`, `ik_retarget`, `anim_montage_create`, `anim_blendspace_create`, `anim_notify_edit`, `art_budget_check` | 6–8 kart-gün (§12.3) | ARPG, RC, GP |
| **EW-2** | ARPG | **Malzeme varyant aracı** (master → MI, slot eşleme, çoklu önizleme panosu) | Elle Python betikleri | `material_instance_create`, `material_instance_set` | **YENİ** `material_variants` (master MI → slot MI toplu, grid önizleme) | 2 kart-gün (§12.3 E24/E34 açık) | ARPG, RC |
| **EW-3** | ARPG | **StateTree / BT yazma** (22 aile davranışı + boss fazları) | Elle, editörde | `gameplay_tags_list` (sadece tag) | **YENİ** `statetree_create`, `statetree_node_add`, `behaviortree_create`, `behaviortree_task_add`, `blackboard_key_add` | 4–5 kart-gün (§12.3 v0.5) | ARPG, RC, GP |
| **EW-4** | ARPG | **AnimBP state machine** (iskelet ailesi başına 1 şablon, 5 aile) | Elle, kopya-özelleştir | — | **YENİ** `animbp_statemachine_template` (iskelet → şablon + parametre) | 3–4 kart-gün (§12.3 v0.5) | ARPG, RC |
| **EW-5** | ARPG | **GAS araçları** (Ability/Effect/Attribute/Cue oluştur, oku, test) — 36 skill, 108 mutasyon, affix'ler = GameplayEffect | Elle, Blueprint | `gameplay_tag_*` (sadece tag okuma/yazma) | **YENİ** `gas_ability_create`, `gas_effect_create`, `gas_attribute_create`, `gas_cue_create`, `gas_test_run` (simülasyon) | 5–6 kart-gün (§12.3 Epic'te 14 okuma) | ARPG, RC |
| **EW-6** | ARPG | **Niagara parametre/varyant** (kopyala, parametre, renk/element varyantı, önizleme) | Elle, Niagara editörü | — | **YENİ** `niagara_clone`, `niagara_param_set`, `niagara_variant_grid` (element/renk), `niagara_preview` | 4 kart-gün (§12.3 1.0) | ARPG, RC |
| **EW-7** | ARPG | **Oda parçası + kat doğrulayıcı** (bağlantı noktaları, ızgara, navmesh build, erişilebilirlik) | Elle, PIE'de test | `scene_dress` (toplu yerleştirme) | **YENİ** `room_piece_validate` (port/grid/nav), `dungeon_flow_test` (A→B→C erişilebilirlik) | 4–5 kart-gün (§12.3 B15 açık) | ARPG |
| **EW-8** | ARPG | **Bot playtest + denge koşusu** (bot zindan temizler: TTK, ölüm, drop, "ilk unique'e süre") | Elle oynama | `pie_input`, `pie_walk_to` | **YENİ** `bot_scenario` (girdi dizisi + iddialar), `balance_sim` (parametre tarama + metrik), `dungeon_autoplay` | 5 kart-gün (§12.3 E1/E2/E18) | ARPG, GP, RC |
| **EW-9** | ARPG | **Headless mod** (editör kapalı import/veri işi) — tek editör kuyruğunu açar | Elle commandlet | `game_run` (paketli oyun), `editor_restart` | **YENİ** `headless_import`, `headless_datatable`, `headless_batch` (commandlet MCP sunucusu) | 6–8 kart-gün (§12.3 1.0) | ARPG, RC, GP |
| **EW-10** | ARPG | **Asset metadata / AI beyanı** (Steam AI içerik beyanı) | Elle tablo | — | **YENİ** `asset_metadata_set` (kaynak/lisans/AI bayrağı), `steam_ai_declaration_report` | 1 kart-gün (§12.3 E14) | ARPG, RC, GP |
| **EW-11** | ARPG | **Sequencer temel** (akt ara sahnesi ~8) | Elle, Python | — | **YENİ** `sequence_create`, `sequence_bind_actor`, `sequence_camera_cut`, `sequence_key_transform` | 3 kart-gün (§12.3 1.0) | ARPG, RC |
| **EW-GP-01** | GP | **Ağlı Playtest Otomasyonu** (2–4 kişi listen-server + P2P Steam testleri, hata raporu toplama) | 2 fizik makine, elle (P2-11) | `pie_net_emulation`, `game_run` ×2 | **YENİ** `net_test_2pc` (2 instance + gecikme/paket kaybı senaryosu + log merge) | 4 saat/test → 15 dk | GP |
| **EW-GP-02** | GP | **Fizik Simülasyonu Doğrulayıcısı** (ganimet fiziği yuvarlanma/kırılma/sekme, ragdoll + tokmak etkileşimi) | Elle PIE | `pie_input`, `pie_walk_to` | **YENİ** `physics_test` (senaryo + sonuç raporu) | 1 saat → 10 dk | GP |
| **EW-GP-03** | GP | **Synty → Kendi Asset Geçiş Aracı** (K-70: Synty yer tutucu → Kendi modelleri Blender pipeline otomatik değiştirme) | Elle import/rename | `asset_import`, `asset_rename` | **YENİ** `asset_import_batch` (klasör → import + rename kuralı + hedef path + referans kontrol) | 3 saat → 10 dk | GP, RC |
| **EW-GP-04** | GP | **Seamless Travel Test Botu** (Hub↔İş↔Avlu travel sırası, PlayerState/Pawn/Controller koruma doğrulama) | Elle PIE | `pie_travel` | **YENİ** `travel_test_suite` (harita sırası + state doğrulama) | 2 saat → 15 dk | GP |
| **EW-GP-05** | GP | **Steam Rich Presence / Lobby Test Aracı** (Davet, Katıl, Oyun İçi Lobby, Avlu Eşleşmesi) | Elle Steam | — | **YENİ** `steam_lobby_test` (davet/katıl/eşleşme akışı) | — | GP |
| **EW-GP-06** | GP | **Cebe Atma / Avlu Bahsi Hile Tespiti** (Runtime memory scan + Server-authoritative validation logları) | Elle log | — | **YENİ** `anti_cheat_audit` (memory pattern + server log doğrulama) | — | GP |
| **EW-GP-07** | GP | **Prosedürel Hayvan Animasyon Önizleyici** (Croc/Ostrich/Peacock spline+IK önizleme Editor içinde) | Elle Blender | — | **YENİ** `procedural_anim_preview` (spline/IK parametreleri) | — | GP |
| **EW-GP-08** | GP | **Koleksiyon Defteri Görselleştirici** (48 kayıt görsel grid, altın varyant vurgulama) | Elle UI | `widget_render` | **YENİ** `collection_grid_capture` (grid + varyant vurgusu) | — | GP |
| **EW-GP-09** | GP | **Sabıka/Dövme Görsel Uygulama** (Karakter üzerine procedural dövme texture baking) | Elle materyal | `material_instance_set` | **YENİ** `tattoo_bake` (procedural texture + UV) | — | GP |
| **EW-RC-01** | RC | **Demo build paketleme** (RunUAT tek komut, log özeti) | Elle RunUAT | `game_run` (paketli exe çalıştırır) | **YENİ** `package_build` (platform + config + out_dir + server?, log özeti + süre) | Kurulum+çalıştırma ~1 gün → 10 dk | RC, GP, ARPG |
| **EW-RC-02** | RC | **Co-op test (2 oyuncu, aynı save)** | Elle 2 instance | `game_run` ×2 + `pie_net_emulation` | **YENİ** `coop_test` (2 instance senkron start/stop + save doğrulama) | 3 saat → 15 dk | RC, GP |
| **EW-RC-03** | RC | **Sahne sahne çekim** (kamerayı gez, N screenshot + manifest) | Elle her sahne tek tek | `screenshot`, `batch`, `pie_travel` | **YENİ** `screenshot_tour` (waypoint[] → N shot + manifest.json) | 1 gün çekim → 30 dk | RC |
| **EW-RC-04** | RC | **Toplu görsel inceleme** (N screenshot grid'de gör, anomali bul) | Elle klasörde tek tek | `image_stats`, `image_compose` | **YENİ** `visual_review` (grid + otomatik anomali notu + `image_diff`) | 2 saat → 10 dk | RC |
| **EW-RC-05** | RC | **Elle asset aktarımı** (art pipeline: klasör → import → rename → yerleştir + referans kontrol) | Elle (tools/art/README akışı) | `asset_import`, `asset_rename`, `asset_duplicate` | **YENİ** `asset_pipeline` (kaynak klasör → import + ad kuralı + hedef path + referans kontrol + rapor) | 4 saat/gün → 20 dk | RC, GP |

---

## Birleştirme (tekilleştirme)

| Araç grubu | İçeren numaralar | Tekilleştirilmiş ad | Not |
|---|---|---|---|
| Paketleme | EW-10 (ARPG §12.3'de yok, RC/GP ortak), EW-RC-01, GP-3 | `package_build` | Tek araç, server target opsiyonel |
| Ağ test | EW-GP-01, EW-RC-02 | `net_test_2pc` / `coop_test` | Senaryo katmanı ayrı (`pie_scenario` bazlı) |
| Asset import toplu | EW-GP-03, EW-RC-05 | `asset_import_batch` / `asset_pipeline` | Pipeline = import_batch + rename + referans kontrol |
| Görsel QA | EW-RC-03, EW-RC-04 | `screenshot_tour` + `visual_review` | `image_diff` (SSIM/rms) ortak alt araç |
| Bot/denge | EW-8, EW-GP-04, EW-GP-05 | `bot_scenario` + `balance_sim` + `travel_test_suite` | Ayrı senaryo katmanları |

---

## Kaç oyuna yarar (özet)

| Araç grubu | ARPG | GP | RC |
|---|---|---|---|
| Sanat hattı (EW-1, EW-2) | ✅ | 🟡 | ✅ |
| AI/StateTree/AnimBP (EW-3, EW-4) | ✅ | 🟡 | ✅ |
| GAS/Niagara (EW-5, EW-6) | ✅ | ❌ | ✅ |
| Kat/oda doğrulama (EW-7) | ✅ | ❌ | ❌ |
| Bot/denge/headless (EW-8, EW-9) | ✅ | ✅ | ✅ |
| Metadata/Sequencer (EW-10, EW-11) | ✅ | 🟡 | ✅ |
| Ağlı test (GP-01, RC-02) | 🟡 | ✅ | ✅ |
| Fizik/hile (GP-02, GP-06) | ❌ | ✅ | ❌ |
| Asset pipeline (GP-03, RC-05) | 🟡 | ✅ | ✅ |
| Çekim/QA (RC-03, RC-04) | 🟡 | 🟡 | ✅ |

**Toplam yeni araç sayısı (tekilleştirilmiş): 19** — her biri sürüme dağıtılacak (bkz `03_Yol_Haritasi.md`).