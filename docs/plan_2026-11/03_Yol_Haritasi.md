# 03 — Yol Haritası (0.4.1 → 0.5 → 0.6 → 1.0 → sonrası)

> **18.11.2026 — Yerini aldı:** `08_Yol_Haritasi_v2.md` (sürüm planı) ve `07_Arac_Esleme.md` (araç eşleme). Bu belgedeki her aracın yeni yeri 08 §11'de. Bu belge silinmedi, tarihçe için duruyor.

Tarih: 17.11.2026. Saatler **tahmin** (tek geliştirici + AI asistan, haftada ~15–20 saat). Her sürüm: tema, araçlar (ad + kısa sözleşme: girdi/çıktı), kabul ölçütü, doğrulama oyunu, oyun takvimi bağımlılığı. "Yapmayacaklarımız" sonda.

**Mevcut durum:** v0.4.0 yayında, **129 araç** (kod sayımı 17.11, 129 tekil `Register(TEXT("..."))`), 10★, MIT, public.

---

## v0.4.1 — "Sanat hattı kapat" (2 hafta, ~30 saat)

**Tema:** RagnaCook sanat hattı (M0-2, M0-3, M1-1…M1-5) için FBX → UE akışını tek komuta indir.

| Araç | Sözleşme (girdi → çıktı) | Saat (tahmin) | Kabul ölçütü | Test oyunu |
|---|---|---|---|---|
| `asset_pipeline(kaynak_klasör, hedef_path, ad_kuralı, {lod,ucx,skeleton,physics,material})` | klasör yolu + seçenekler → import raporu (asset listesi, collision doğrulama, referans kontrol) | 10 | RagnaCook'ta bir klasörlik Synty/yerel asset tek komutla giriyor, LOD/UCX/skeleton/physics doğrulaması geçiyor, `PlateStack` soketi çalışıyor | RagnaCook (art hattı, `tools/art/README.md`) |
| `asset_import_batch(klasör, format_filtresi, {rename_template, hedef_path})` | klasör + filtre → N asset + rapor (başarılı/başarısız, süre) | 6 | 50 asset < 5 dk, rename template uygulanıyor, hedef path'te referansları bozmadan yerleşiyor | RagnaCook (Dalga 1, `M1-2_Ref_Paketi.md`) |
| `skeletal_import(fbx, hedef_skeleton, {anim_klasörü, socket_list, physics_asset})` | FBX + hedef iskelet → `SkeletalMesh` + `PhysicsAsset` + `AnimSequence` listesi + soketler | 8 | KayKit `Rig_Medium` hedefi: Tripo FBX → hedef skeleton bind + IK retarget + Physics Asset + 3 soket (Muzzle, Magazine, Ejection) < 10 dk | RagnaCook (Viking karakter, `Devir_Notu_16-11.md`) |
| `image_diff(a_path, b_path, {eşik_rms, maske})` → `{rms, fark_piksel_oranı, diff_görseli_path}` | iki PNG → sayısal diff + görsel | 4 | Aynı sahnenin 2 shot'u RMS < 0.01; değişik sahne RMS > eşik, diff görseli `image_compose` ile yan yana | RagnaCook `Gorsel_Inceleme.md` akışı |
| `material_variants(master_MI, slot_adı[], parametre_setleri[])` → grid önizleme PNG + MI listesi | master MI + slot parametreleri → varyant MI'lar + grid shot | 2 | 66 düşman varyantı master MI'dan üretiliyor, 8×8 grid PNG `actor_preview` ile üretiliyor | ARPG (EW-2) |

**Oyun bağımlılığı:** RagnaCook demo çekimi yakın (bkz `Devir_Notu_16-11.md` M1-5, M2-5b). GP `EW-GP-03` asset import batch bu sürümden yararlanır.

---

## v0.5 — "Demo ve çekim otomasyonu + AI/Animasyon" (3–4 hafta, ~55 saat)

**Tema:** BT/StateTree/AnimBP/Mutable yazma + paketleme/çekim/co-op test araçları.

| Araç | Sözleşme | Saat | Kabul ölçütü | Test oyunu |
|---|---|---|---|---|
| `package_build(platform, config, out_dir, server?)` | platform/config/out_dir (+ server target) → zip yolu + log özeti + süre | 8 | RC demo build < 30 dk, sıfır elle adım, `CleanBuild.bat` eşdeğeri | RagnaCook + GP (dedicated server) |
| `screenshot_tour(waypoints[], shot_sayısı, {çözünürlük, format})` → `manifest.json` + PNG'ler | kamera waypoint listesi → N screenshot + manifest (pozisyon, FOV, zaman) | 6 | RC sahnesi 20 shot < 10 dk, manifest `visual_review` girdisi | RagnaCook `03_Yol_Haritasi` çekim kalemleri |
| `visual_review(görsel_klasörü, {grid_sütun, eşik_diff})` → grid PNG + anomali raporu | klasör → `image_compose` grid + `image_diff` anomalleri | 4 | `Gorsel_Inceleme.md` elle inceleme yerine geçir; 20 shot tek grid, anomali sayısı 0 | RagnaCook |
| `coop_test(instance_sayısı=2, senaryo, {sync_start, save_path})` → merged log + verdik | 2 `game_run` instance senkron → log birleştirme + save doğrulama | 6 | GP/RC co-op senaryosu otomatik: Hub→İş→Rapor, save yükleniyor, cep/kasa eşit | GP (t_e6c46ed7) + RC |
| `pie_scenario(adımlar[{araç, girdi, bekleme, iddialı}])` → sonuç raporu | adım listesi → PIE içinde zincirleme çalıştırma + iddiá doğrulama | 6 | ARPG EW-6 senaryosu (spawn→input→assert) tek çağrıda, rapor JSON | ARPG |
| `statetree_create(ad, şema_json)` + `statetree_node_add` + `behaviortree_create` + `bt_task_add` | JSON şema → StateTree/BT asset + node'lar | 12 | 22 düşman arketipi StateTree şablonundan üretiliyor, boss fazları BT ile, `validate` geçiyor | ARPG (EW-3) |
| `animbp_statemachine_template(hedef_skeleton, parametreler)` → AnimBP asset | iskelet + parametreler → state machine şablonu (5 aile) | 8 | 5 iskelet ailesi için şablon üretilip, varyantlara parametreyle uyanıyor | ARPG (EW-4) |
| `mutable_tools` (opsiyonel, RC/GP'ye gerekirse) | Customizable Object create/edit/compile | 6 | RC karakter özelleştirme (M4-3 Gardırop) Mutable ile yapılıyor, doğrulama geçiyor | RC (M4-3) |
| `travel_test_suite(harita_sırası[], {net_mode, oyuncu_sayısı})` → state doğrulama raporu | harita dizisi → seamless travel + PlayerState/Pawn/Controller koruma kontrolü | 4 | GP Hub↔İş↔Avlu akışı 3 travel, state kaybı 0 | GP (EW-GP-04) |

**Oyun bağımlılığı:** GP kartı (ağ testleri, `ParoleLoop_TaskList.md` P2-11) ve RC demo takvimi bu sürümü çeker.

---

## v0.6 — "İçerik üretimi ve ağ + API freeze" (4–5 hafta, ~65 saat)

**Tema:** Veri/İçerik araçları (DataTable CSV, Blueprint rapor, world inventory, actor toplu set, scene snapshot/diff) + ağ testleri derinleşme + log analizi + API freeze.

| Araç | Sözleşme | Saat | Kabul ölçütü | Test oyunu |
|---|---|---|---|---|
| `datatable_csv(table, yön, {kolon_eşleme, rapor})` | export: DataTable → CSV + rapor; import: CSV → DataTable + kolon eşleme raporu | 8 | ARPG 357 affix CSV roundtrip, 0 kayıp, `dogrula.py` 0 hata | ARPG (t_1ac37fc7 çıktısı) |
| `blueprint_compile_all({filtre, rapor_formatı})` → JSON rapor (BP, hata, satır, süre) | tüm BP'ler derle → JSON (path, errors[], warnings[], süre) | 6 | ARPG EW-5 elle yapılanla aynı rapor, `automation_run` ile entegre | ARPG |
| `world_inventory({sahne, filtre})` → JSON (aktörler + assetler + property özet) | sahne yolu/filtre → tüm sahne envanteri JSON | 6 | EW-1 DioceseBug tipi "yanlış yerde aktör" hatası 1 dk'da bulunuyor | ARPG |
| `actor_set_props(filtre, property, değer, {dryRun})` → etkilenen sayısı + değişiklik listesi | filtre + property + değer → N aktör tek çağrıda + rapor | 4 | EW-2: 50 aktör < 2 dk, `dryRun` önizleme doğru | ARPG |
| `scene_snapshot()` + `scene_diff(a_json, b_json)` → fark listesi (eklenen/silinen/değişen) | snapshot JSON → diff raporu | 4 | EW-4: iki düzenleme arası fark net, `image_diff` ile görsel korelasyon | Tümü |
| `net_test_2pc(gecikme_ms, paket_kaybı_pct, senaryo)` → replikasyon raporu + log merge | 2 instance + ağ emülasyon profili → replikasyon doğrulama | 8 | GP-1: replikasyon senaryosu otomatik, `pie_actors` karşılaştırma raporu | GP |
| `replication_check(aktörler[], propertyler[])` → server/client karşılaştırma raporu | aktör + property listesi → server vs client değer farkı | 4 | GP-2: elle debug yerine tek çağrı, fark 0 | GP |
| `log_analyze(log_path, {patternler})` → crash pattern + modül özeti + stack trace | log dosyası → özet rapor (modül, fonksiyon, hata tipi) | 4 | EW-9: elle tarama yerine, crash log 30 sn'de özet | Tümü |
| `level_flow_test(seviye_sırası[], {geçiş_süreleri})` → A→B→C geçiş + süre raporu | seviye dizisi → streaming geçiş süreleri + hata yok | 4 | EW-11: ARPG zindan akışı 3 geçiş < 30 sn toplam | ARPG |
| `physics_test(senaryo, {beklenen_sonuçlar})` → sonuç raporu (başarılı/başarısız, metrikler) | caos/ragdoll senaryosu → metrikler (yüklenen kuvvet, süre, durum) | 4 | GP-5: tokmak + ganimet fiziği senaryosu otomatik doğrulama | GP |
| `headless_import(kaynak_klasör, hedef_proje)` → import raporu (commandlet MCP) | commandlet'te MCP sunucusu + import_batch | 12 | Editör kapalıyken 100 asset import, rapor `asset_pipeline` formatında | RC (EW-9 başlangıcı) |
| `asset_metadata_set(asset_path, {kaynak, lisans, ai_bayrağı, notlar})` → metadata yazılıp okundu | asset + metadata dict → `AssetMetadata` tag'lerine yaz + readback doğrulama | 2 | Steam AI beyanı için 90 unique + 66 düşman varyantı metadata'sı dolu | ARPG, RC (EW-10) |

**Bu sürüm sonunda API freeze:** araç adları ve şemalar sabitlenir, 1.0'a kadar sadece bugfix + doküman.

---

## 1.0 — "Launch" (2–3 hafta, ~40 saat + duyuru işi 05'te)

**Tema:** SEO/Registry/Demo videoları + stabilite vaadi + showcase.

| İş | Kabul ölçütü |
|---|---|
| SEO paketi: açıklama (≤350 krk), 20 topic, sosyal önizleme 1280×640 (<1 MB), release notları, tag v1.0.0 | `04_Kesfedilebilirlik_SEO.md` checklist %100 |
| Resmi MCP registry yayını (`server.json`, `io.github.shanyup/enginewright`, `mcp-publisher publish`) | registry'de listelenir, introspection geçer |
| GitHub Pages dokümantasyon sitesi (EN) + 10 anahtar kelime dokümanda | site yayında, `unreal engine mcp`, `ue5 ai agent`, `claude unreal` vb. |
| awesome-mcp-servers PR (Glama score badge ile) | PR merge, badge README'da |
| Demo videoları (30–60 sn, 6 adet — bkz 05 §3) | YouTube/X'te yayında, her video 05'teki senaryosuna uygun |
| Issue şablonları + good-first-issue seti (`docs/good-first-issues.md` hazır) | İlk 5 issue açılabilir, etiketli |
| `CHANGELOG.md` v1.0.0 girişi + GitHub Release (zip Win64 UE 5.8) | Release sayfasında zip indirilebilir |

---

## Sonrası (1.0+) — tek geliştirici kapasitesini aşanlar "Sonra"

| İş | Not |
|---|---|
| Fab'a free code plugin listing (Marketplace kuralı 1.2.4.b: free sadece code plugin; .uplugin zip, her UE sürümü ayrı; Fab otomatik build eder, ≤15 GB) | UE 5.8 sürümüyle başla; `SORULAR.md` S-2 |
| UE 5.9/5.10 destek | Epic resmi MCP büyüdükçe takip (01 §5) |
| Hosted/remote mod | Şu an localhost-only; sonra değerlendir |
| Discord | `SORULAR.md` S-6 (öneri: hayır, GitHub Discussions) |
| Ücretli katman | `SORULAR.md` S-9 (öneri: hayır, saf OSS) |

---

## Eski roadmap dosyalarıyla eşleme (satır silinmez — devam/değişti/iptal)

| Eski satır (kaynak) | Durum | Yeni karşılık (bu dosya) | Not |
|---|---|---|---|
| `ROADMAP.md` v0.4.1 "art import pipeline" | **devam** | v0.4.1 `asset_pipeline` + `asset_import_batch` + `skeletal_import` + `image_diff` + `material_variants` | Genişletildi |
| `ROADMAP.md` v0.5 "RagnaCook demo needs" (BT/StateTree/AnimBP/Mutable) | **devam** | v0.5 `statetree_*`, `behaviortree_*`, `animbp_statemachine_template`, `mutable_tools` + `package_build`, `screenshot_tour`, `visual_review`, `coop_test`, `pie_scenario` | Çekim/co-op eklendi |
| `ROADMAP.md` v0.6 "API freeze" | **değişti** (içerik eklendi) | v0.6 + veri/ağ araçları (`datatable_csv`, `world_inventory`, `actor_set_props`, `scene_snapshot/diff`, `net_test_2pc`, `replication_check`, `log_analyze`, `level_flow_test`, `physics_test`, `headless_import`, `asset_metadata_set`), **sonra** freeze | Kapsam büyüdü |
| `ROADMAP.md` 1.0.0 "launch update" (headless, Niagara, Sequencer, UE 5.4–5.8, agent skill, showcase) | **devam** | 1.0 launch + SEO/registry (04/05); headless/Niagara/Sequencer/5.4–5.8/agent skill → **Sonra** | Gerçekçi kapsam |
| `ROADMAP_1.0_TASLAK.md` Adım 2 rakip analizi | **tamamlandı** | `01_Rakip_ve_Emsal_Karsilastirma.md` | |
| `ROADMAP_1.0_TASLAK.md` Adım 3 aşama→sürüm | **devam** | Yukarıdaki sürüm tabloları | |
| `ROADMAP_EKSIKSIZ_MCP_2026-11-14.md` §5 yol haritası (Faz 1–4) | **devam** | v0.4.1/v0.5/v0.6/1.0 + Sonra eşleşiyor; Faz 1→v0.4.1+v0.5, Faz 2→v0.6, Faz 3→1.0, Faz 4→Sonra | |
| `EKSIKLER_VE_ONCELIK.md` A (güvenilirlik A1–A11) | **devam** | v0.6 `validate` genişletme, `log_analyze`, `batch` zaten v0.4.0 | A8 (yeniden bağlanma) v0.5 `editor_restart` |
| `EKSIKLER_VE_ONCELIK.md` B (Epic'te olmayanlar B1–B19) | **devam** | B1 konsol✅, B2 PIE✅, B3 kamera✅, B4 derleme✅, B5 rename✅, B6 silme✅, B7 undo→**v0.6/yok**, B8 batch✅, B9 import✅, B10 input✅, B11 doğrulama→v0.6, B12 ölçüm✅, B13 AnimBP→v0.5, B14 BT→v0.5, B15 nav/ışık→Sonra, B16 loc✅, B17 cook→1.0 `package_build`, B18 headless→v0.6 başlangıcı, B19 skill→Sonra | |
| `EKSIKLER_VE_ONCELIK.md` E (RagnaCook+GP eksikleri E1–E27) | **devam** | E1 MP test→v0.5 `coop_test`+`net_test_2pc`, E2 bot→v0.6 `bot_scenario`, E3 build ölçüm→v0.6 `package_build` log, E4 kamera→v0.4.1 `image_diff`/`actor_preview`, E5 okunabilirlik→`screen_coverage`✅, E6 ID audit→`id_audit`✅, E7 tuş audit→`key_audit`✅, E8 A/B görsel→`screenshot_ab`+`image_compose`✅, E9 devlog→`mrq_render`✅, E10 build arşivi→1.0, E11 değişiklik raporu→v0.6 `log_analyze`, E12 header döngüsü→v0.5 `editor_restart`, E13 doctor→`doctor`✅, E14 metadata→v0.6 `asset_metadata_set`, E15 loc→`loc_*`✅, E16 cheat→`console_command`✅, E17 toplu veri→`datatable_csv` v0.6, E18 sim→v0.6 `balance_sim` | |

---

## Yapmayacaklarımız

1. Epic'in 870 aracının kopyası olmak — oyun iş akışlarına odaklanıyoruz (01 §5).
2. Remote/hosted sunucu (güvenlik yükü, tek geliştirici için "Sonra").
3. Ücretli katman / telemetri (OSS saf; blender-mcp telemetri itirazı dersi).
4. Screenshot-tabanlı vision otomasyonu — structured state + targeted screenshot (Playwright dersi).
5. Birden fazla UE ana sürümünü aynı anda desteklemek — tek sürüm (5.8), sonra 5.9.
6. Otomatik star/upvote, spam, astroturf — 05 "YAPMA" listesi.
7. Python exec aracı (bilinçli tercih; `console_command` + `editor_restart` için C++ yolu var).
8. MetaHuman / Control Rig / PCG / Landscape / Water yazma (Epic/rakip yoğun, oyun iş akışımızda değil).
9. Source control araçları (Git CLI yeterli).
10. 3D asset üretimi (Tripo/Meshy entegrasyonu kapsam dışı; import doğrulama ile tamamlayıcı).