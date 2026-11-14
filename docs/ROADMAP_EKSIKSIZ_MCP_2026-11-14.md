# Enginewright — Eksiksiz MCP Yol Haritası (derin rakip analizi + git geçmişi)

Tarih: 2026-11-14 · Yazar: Hermes (GLM 5.3) · Durum: taslak, Talha onayı bekliyor
Amaç: Enginewright'ı "1.0.0" etiketiyle değil, **zorlandığımız her alanda eksiksiz, rakiplerden fazlasını sunan** bir MCP'ye dönüştürmek.

## 1. Yöntem ve kaynaklar

- Enginewright git geçmişi: v0.1.0 → v0.4.0 + Unreleased (CHANGELOG.md, release-notes/*)
- Güvenilirlik süpürgesi: 0.4.0'da RagnaCook üzerinde 330 gerçek çağrı (docs/RELIABILITY_0.4.md)
- EngineWire 0.6.0 saha testi: RagnaCook'ta D0, A1–A11, B1–B18, E1–E16 (docs/ENGINEWIRE_TEST.md)
- Epic UE 5.8 MCP envanteri: 870 araç / 59 sınıf (docs/epic_ue58_inventory.json, git dışı)
- Eksik listesi: docs/EKSIKLER_VE_ONCELIK.md (A: güvenilirlik, B: olmayan yetenekler, C: Epic kapsamı, D: altyapı, E: iş akışı)
- Acı kayıtları: RagnaCook TaskList, 13.11 oturumu (kamera ensure çökmesi, çift editör, MRQ precision-loss, doku binding kopması, LFS push gürültüsü, kanban kota duvarı)

## 2. Enginewright evrimi (git bazlı)

| Sürüm | Eklenen ana alanlar | Çözülen sorun |
|---|---|---|
| v0.1.0 | Editör içi MCP sunucusu, temel asset/actor araçları | Dış sunucu/proxy gereksinimi |
| v0.2.0 | Hazır UE 5.8 zip, client config'ler (Claude/Codex/Cursor/VS Code/Hermes), screenshot + screenshot_ab, PIE start (1-4 oyuncu), pie_input, log_wait, live coding, editor_restart, BP/Material/mesh/texture araçları | Ajanın elle derleme/dönüş döngüsü |
| v0.3.0 | UMG widget CRUD + render, CommonUI runtime (ui_state/ui_click/...), lokalizasyon stringtable, game_run + image_compose, gameplay tags, pie_actors/pie_net_emulation, class_measure, screen_coverage | UI ve MP testlerinin elle yapılması |
| v0.3.1 | Durum çubuğu + Enginewright paneli + doctor, proje ayarları, editor_capture | Editör sağlık kontrolünün elle yapılması (E13) |
| v0.4.0 | loc_gather tek çağrıda gather→export→import→compile→check, font_coverage, game_run culture/cvars/shotNames, ui_key/pie_walk_to/pie_travel, savegame get/set, scene_dress/actor_preview, MVVM plugin, image_stats; 11 kritik hata fix (datatable Name ezilmesi, level_save C:/Game'a yazması, asset_delete bağlı set, tag ini artığı...) | 330 çağrılık sweep'te bulunan 11 gerçek hata |
| Unreleased (0.4.1) | **Sanat hattı**: FBX import (LOD, collision, pivot/ölçek), iskelet bind, IK retarget, soketler, Physics Asset, modüler karakter önizleme, master material şablonu, doku import ayarları, anim import/montage/blendspace/notify, asset bütçe kontrolü | RagnaCook Y2 hattının elle betiklerle yapılması |
| (son commitler) | mrq_render/mrq_status (MRQ PIE executor + ffmpeg), perf_capture/perf_scene/perf_gpu_profile | MRQ'nun ensure açması, performansın elle ölçülmesi |

## 3. Derin karşılaştırma (alan bazlı)

| Alan | Epic 5.8 MCP | EngineWire 0.6.0 | Enginewright şimdi | Hedef durum |
|---|---|---|---|---|
| Güvenilir yazma (katı mod, readback) | 🟡 sessiz hata var | 🟡 sessiz alan yok sayımı | ✅ 0.4.0 fix'li | Koru + testle |
| PIE durum kapısı | ❌ yanıltıcı | 🟡 kısmi | ✅ | Koru |
| Konsol komutu + log_wait | ✅ | ✅ | ✅ | Koru |
| Oyun kamerası görüntüsü | 🟡 editör kamerası | ✅ game_viewport/full | ✅ screenshot PIE + mrq | Koru |
| Çok oyunculu PIE + girdi | 🟡 | 🟡 (girdi var, N-istemci yok) | ✅ pie_start 1-4 + pie_input + net_emulation | ✅ E1'i de ekle: senaryolu bot playtest |
| Tam UBT derleme + restart | 🟡 live coding | 🟡 restart var, UBT yok | ✅ live_coding + editor_restart + build_status | UBT tam derleme ekle (B4) |
| Güvenli rename/delete | 🟡 | ✅ | ✅ + dryRun + text/ID raporu | Koru |
| Undo/transaction | ❌ | ✅ | 🟡 yok | **Ekle (B7)** |
| Batch + rollback | ❌ | 🟡 spawn'da var | 🟡 yok | **Ekle (B8)** |
| Import (editör açıkken) | 🟡 commandlet | ✅ | ✅ 0.4.1 sanat hattı | Koru |
| Enhanced Input yardımcıları | 🟡 | ✅ | ✅ input_map_list/ui_key | Koru |
| Doğrulama (BP derle, map check, validation) | ✅ | ✅ | 🟡 kısmi | **Tamamla (B11)** |
| Ölçüm (bounds, ekran kaplama, FPS) | 🟡 | ✅ | ✅ class_measure/screen_coverage + perf_* | ✅ + build ölçümü (E3) |
| AnimBP / BT / StateTree yazma | 🟡 okuma | ✅ | 🟡 yok | **Ekle (B13/B14)** |
| Navmesh + ışık build | ❌ | ✅ | 🟡 yok | **Ekle (B15)** |
| Lokalizasyon | ✅ | ❌ | ✅✅ loc_gather + compare + font_coverage | Koru, fazlası bizde |
| Paketleme/cook + headless | 🟡 | 🟡/❌ headless | 🟡 game_run var, cook yok | **Ekle (B17/B18)** |
| Headless mod (editör kapalı) | ❌ | ❌ | ❌ | **Ekle (B18)** |
| Asset metadata/lisans kaydı | ❌ | ✅ set_metadata | 🟡 yok | **Ekle (E14)** — Steam AI beyanı için |
| A/B görsel onay + kolaj | ❌ | ❌ | ✅ screenshot_ab + image_compose + image_stats | Koru, fazlası bizde |
| Devlog/klip kaydı | 🟡 sequencer | 🟡 | ✅ mrq_render (ffmpeg mp4) | Koru |
| Sağlık kontrolü (doctor) | ❌ | ❌ | ✅ doctor + panel | Koru, fazlası bizde |
| MVVM | ✅ | ❌ | ✅ ayrı plugin | Koru |
| Proje tarifleri (skill) | 🟡 5.8'e bağlı | ❌ | ❌ | **Ekle (B19)** |

## 4. Acıdan öğrenilenler → karşılığı

| Olay (13.11 ve öncesi) | Eksik yetenek | Karşılık |
|---|---|---|
| Kamera ensure çökmesi (OriginX<=OriginMax), MRQ precision-loss | Oyun kamerası + log iddiası + MRQ executor | screenshot/PIE + log_wait ✅, mrq_render ✅ (yeni) |
| Aynı anda 2 editör (işçi karışması) | doctor + editör kilidi | doctor ✅; kilit dosyası proje dışı → **MCP `editor_lock` aracı ekle** |
| Tripo modellerinin dokusu oyunda gri | malzeme parametre readback + import doğrulama | 0.4.1 sanat hattı ✅; `material_param_get` readback ✅ |
| Işık/gölge dengesizliği elle bulundu | görüntü istatistiği + sahne perf | image_stats ✅, perf_scene ✅ |
| Kanban kota duvarı (ajandan bağımsız) | — (altyapı dışı) | Not: MCP değil, Hermes katmanı |
| LFS push gürültüsü | — | git katmanı, MCP dışı |

## 5. Yol haritası: eksiksiz + fazlası

Her fazın kabulü: RagnaCook üzerinde gerçek projede sweep (RELIABILITY_0.4 standardı: her araç en az 1 gerçek çağrı + hatalı girdi + PIE sırasındaki yazma + temizlik kontrolü).

- **Faz 1 — Kalan çekirdek eksikler:** B7 undo/transaction, B8 batch+rollback, B13 AnimBP yazma, B14 BT/StateTree yazma, B15 nav+ışık build, B4 tam UBT, B11 tam doğrulama, B17 cook/paketleme, B18 headless commandlet modu, B19 proje tarifleri, E14 asset metadata/lisans, E1 bot playtest senaryoları, E3 build ölçümü, E10 build arşivi+TEST.md, `editor_lock` aracı.
- **Faz 2 — Sanat hattının olgunlaşması (0.4.1 devamı):** FBX LOD/collision doğrulama katılaştır, modüler karakter önizleme → tam wardrobe önizleme, doku import preset'leri, anim notify düzenleme.
- **Faz 3 — Fazlası (bizde olan, rakiplerde olmayan):** doctor+panel, screenshot_ab/image_compose/image_stats, loc_gather tek çağrı, font_coverage, mrq_render ffmpeg, perf_*, MVVM plugin, savegame araçları, ui_click akıllı eşleme. Bunları **RELEASE_NOTES'ta "Enginewright fazlası" bölümü** olarak pazarla.
- **Faz 4 — Sürümleme politikası:** "1.0.0" etiketi yok; sürüm = kapsam kapıhamed OTT: her faz bitince dahili "tam kapsam" kontrol listesi (EKSIKLER A/B/C/D/E tabloları tamamen ✅) işaretlenir, açık madde kalmadıkça etiket çıkmaz.

## 6. Doğrulama ve geri alınabilirlik

- Her araç: birim testi + smoke + RagnaCook sweep; CHANGELOG'a girer.
- Her faz ayrı dalda, merge sonrası `tests/` tümü yeşil; RagnaCook'ta kirli working tree bırakmaz (0.4.0 standardı).
- Bu döküman: `docs/ROADMAP_EKSIKSIZ_MCP_2026-11-14.md`; her faz kapandığında tablo güncellenir.
