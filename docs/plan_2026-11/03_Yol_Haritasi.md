# 03 — Yol Haritası (0.4.1 → 0.5 → 0.6 → 1.0 → sonrası)

Tarih: 16.11.2026. Saatler **tahmin** (tek geliştirici + AI asistan, haftada ~15-20 saat). Her sürüm: tema, araçlar (ad + kısa sözleşme), kabul ölçütü, doğrulama oyunu, oyun takvimi bağımlılığı. "Yapmayacaklarımız" sonda.

Mevcut durum: v0.4.0 yayında, **129 araç** (kod sayımı 16.11), 10★, MIT, public.

## v0.4.1 — "Sanat hattı kapat" (2 hafta, ~30 saat)

| Alan | Araçlar (sözleşme) | Kabul ölçütü | Test oyunu |
|---|---|---|---|
| Asset pipeline | `asset_pipeline(kaynak_klasör, hedef_path, ad_kuralı)` → import + rename + referans kontrol, rapor döner | RagnaCook'ta bir klasörlik Synty/yerel asset tek komutla giriyor, collision yok | RagnaCook (art hattı, `tools/art/README.md`) |
| Toplu import | `asset_import_batch(klasör, format_filtresi)` → N asset, rapor | 50 asset < 5 dk | RagnaCook |
| Görsel diff | `image_diff(a, b)` → rms + fark görseli | aynı sahnenin 2 shot'u %0 fark; değişik sahne > eşik | RagnaCook `Gorsel_Inceleme.md` akışı |

Oyun bağımlılığı: RagnaCook demo çekimi yakın (bkz Devir_Notu_16-11).

## v0.5 — "Demo ve çekim otomasyonu" (3-4 hafta, ~50 saat)

| Araç | Sözleşme | Kabul ölçütü | Test oyunu |
|---|---|---|---|
| `package_build(platform, config, out_dir, server?)` | RunUAT tek komut; log özeti + süre döner | RC demo build < 30 dk, sıfır elle adım | RagnaCook + GP (dedicated server) |
| `screenshot_tour(waypoints[], shot_sayısı)` | kamera gezer, N screenshot + manifest.json | RC sahnesi 20 shot < 10 dk | RagnaCook `03_Yol_Haritasi` çekim kalemleri |
| `visual_review(görsel_klasörü)` → grid + anomali notu | 20 shot tek grid, `image_compose` ile | Gorsel_Inceleme.md'deki elle inceleme yerine geçer | RagnaCook |
| `coop_test(instance_sayısı=2, senaryo)` | 2 game_run instance senkron start/stop + log merge | GP/RC co-op senaryosu otomatik | GP (t_e6c46ed7) + RC |
| `pie_scenario(adımlar[])` | spawn→input→assert zinciri, PIE içinde | ARPG EW-6 senaryosu tek çağrıda | ARPG |

Oyun bağımlılığı: GP kartı (ağ testleri) ve RC demo takvimi bu sürümü çeker.

## v0.6 — "İçerik üretimi ve ağ" (4-5 hafta, ~60 saat)

| Araç | Sözleşme | Kabul ölçütü | Test oyunu |
|---|---|---|---|
| `datatable_csv(table, yön)` | export/import, kolon eşleme raporu | ARPG 357 affix CSV roundtrip, 0 kayıp | ARPG (t_1ac37fc7 çıktısı) |
| `blueprint_compile_all()` → rapor (BP, hata, satır) | tüm BP'ler, JSON rapor | ARPG EW-5 elle yapılanla aynı rapor | ARPG |
| `world_inventory()` → tüm sahne JSON | aktör+asset+property dökümü | EW-1 DioceseBug tipi hata 1 dk'da bulunur | ARPG |
| `actor_set_props(filtre, property, değer)` | N aktör tek çağrı | EW-2: 50 aktör < 2 dk | ARPG |
| `scene_snapshot()` + `scene_diff(a,b)` | düzen snapshot + fark listesi | EW-4: iki düzenleme arası fark net | tümü |
| `net_test_2pc(gecikme, paket_kaybı, senaryo)` | 2 instance + ağ emülasyonu | GP-1: replikasyon senaryosu otomatik | GP |
| `replication_check(aktörler, propertyler)` | server/client property karşılaştırma | GP-2 elle debug yerine | GP |
| `log_analyze(log_path)` → crash pattern + modül özeti | crash log özet raporu | EW-9: elle tarama yerine | tümü |
| `level_flow_test(seviye_sırası)` | A→B→C geçiş + süre raporu | EW-11 | ARPG |
| `physics_test(senaryo)` | chaos/ragdoll senaryosu + sonuç | GP-5 | GP |

Bu sürüm sonunda **API freeze**: araç adları ve şemalar sabitlenir, 1.0'a kadar sadece bugfix + doküman.

## 1.0 — "Launch" (2-3 hafta, ~40 saat + duyuru işi 05'te)

| İş | Kabul ölçütü |
|---|---|
| SEO paketi: açıklama (≤350 krk), 20 topic, sosyal önizleme 1280x640 (<1 MB), release notları, tag v1.0.0 | 04'teki checklist %100 |
| Resmi MCP registry yayını (`server.json`, `io.github.shanyup/enginewright`, `mcp-publisher publish`) | registry'de listelenir, introspection geçer |
| GitHub Pages dokümantasyon sitesi (EN) + anahtar kelimeler | site yayında, 10 anahtar kelime dokümanda |
| awesome-mcp-servers PR (Glama score badge ile) | PR merge |
| Demo videoları (30-60 sn, 5 adet) | YouTube/X'te yayında |
| Issue şablonları + good-first-issue seti (`docs/good-first-issues.md` hazır) | ilk 5 issue açılabilir |

## Sonrası (1.0+) — tek geliştirici kapasitesini aşanlar "Sonra"

| İş | Not |
|---|---|
| Fab'a free code plugin listing (Marketplace kuralı 1.2.4.b: free sadece code plugin; .uplugin zip, her UE sürümü ayrı; Fab otomatik build eder, ≤15 GB) | UE 5.8 sürümüyle başla; SORULAR S-2 |
| UE 5.9/5.10 destek | Epic resmi MCP büyüdükçe takip (01 §5) |
| hosted/remote mod | şu an localhost-only; sonra değerlendir |
| Discord | SORULAR S-6 (öneri: hayır, GitHub Discussions) |
| Ücretli katman | SORULAR S-9 (öneri: hayır, saf OSS) |

## Eski roadmap dosyalarıyla eşleme (satır silinmez)

| Eski satır | Durum | Yeni karşılığı |
|---|---|---|
| `ROADMAP.md` v0.4.1 "art import pipeline" | **devam** | v0.4.1 `asset_pipeline` + `asset_import_batch` |
| `ROADMAP.md` v0.5 "RagnaCook demo needs" | **devam** | v0.5 `package_build`, `screenshot_tour`, `visual_review`, `coop_test` |
| `ROADMAP.md` v0.6 "API freeze" | **değişti** (içerik eklendi) | v0.6 + içerik/ağ araçları, sonra freeze |
| `ROADMAP.md` 1.0.0 "launch update" | **devam** | 1.0 launch + SEO/registry (04/05) |
| `ROADMAP_1.0_TASLAK.md` Adım 2 rakip analizi | **tamamlandı** | bu dosya 01 |
| `ROADMAP_1.0_TASLAK.md` Adım 3 aşama→sürüm | **devam** | yukarıdaki sürüm tabloları |
| `ROADMAP_EKSIKSIZ_MCP_2026-11-14.md` §5 yol haritası | **devam** | 02'deki oyun ihtiyaçlarıyla beslendi |
| `EKSIKLER_VE_ONCELIK.md` A (güvenilirlik) | **devam** | v0.6 `validate`/audit genişletme, `log_analyze` |
| `EKSIKLER_VE_ONCELIK.md` B (Epic'te olmayanlar) | **devam** | 02 tablo: cook, snapshot/diff, görsel QA, ağ test |
| `EKSIKLER_VE_ONCELIK.md` E (RagnaCook+GP eksikleri) | **devam** | v0.4.1/v0.5 araçları (çekim, sanat hattı, paketleme) |

## Yapmayacaklarımız

1. Epic'in 870 aracının kopyası olmak — oyun iş akışlarına odaklanıyoruz (01 §5).
2. Remote/hosted sunucu (güvenlik yükü, tek geliştirici için "Sonra").
3. Ücretli katman / telemetri (OSS saf; blender-mcp'in telemetri itirazı dersi).
4. Screenshot-tabanlı vision otomasyonu — structured state + targeted screenshot (Playwright dersi).
5. Birden fazla UE ana sürümünü aynı anda desteklemek — tek sürüm (5.8), sonra 5.9.
6. Otomatik star/upvote, spam, astroturf — 05 "YAPMA" listesi.
