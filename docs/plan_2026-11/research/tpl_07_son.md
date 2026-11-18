
## 4. Hiçbir oyun ihtiyacında geçmeyen 57 araç

Bunlar gereksiz değil. Çoğu genel editör işi (Blueprint düzenleme, UI/MVVM, girdi eşleme, malzeme grafiği). Oyunların İçerik Sistemi belgeleri bu işleri "araç ihtiyacı" diye yazmadı, çünkü bugün çalışıyorlar.

| Kategori | Sayı | Araçlar |
|---|---|---|
| UI (widget + MVVM + ui_*) | 15 | widget_create/add/get/set/remove/tree, viewmodel_add/bind/list/remove/unbind, ui_click, ui_close, ui_input_type, ui_key |
| Blueprints | 11 | blueprint_create/info, blueprint_add_component/node/variables, blueprint_connect, blueprint_remove_component/node/variables, blueprint_set_component_defaults, blueprint_set_pin |
| Editor | 8 | client_config, doctor, editor_capture, editor_state, level_open, log_read, settings_get, settings_set |
| Play | 7 | input_action_create, input_context_create, input_context_info, input_map, input_map_list, input_unmap, pie_stop |
| Materials | 4 | material_create, material_add_expression, material_connect, material_info |
| Level | 3 | actor_delete, actor_find, actor_set_transform |
| Data | 3 | gameplay_tag_add, gameplay_tag_remove, gameplay_tags_list |
| Assets / Objects / Build | 2 + 2 + 2 | asset_exists, asset_save · enum_create, struct_create · editor_restart, live_coding_compile |

Kategori adları kaynak koddaki `SetCategory` değeridir (`arac_envanteri.json`). Liste: `research/ihtiyac_esleme_ozet.json` → `kullanilmayan`.

Sonuç: 1.0 öncesi temizlik ya da birleştirme için aday değil (oyunlar dolaylı kullanıyor). Yeni araç eklerken **mevcut kategoriye ekle**, yeni kategori açma (09 §1).

## 5. Güvenilirlik riskleri (oyun ihtiyacına hizmet eden araçlarda)

Kaynak: `RELIABILITY_0.4.md` satırları + `arac_test_kapsami.json`. Tümü: 129 araçtan 61 OK, 56 FIXED, 7 NOTE, 5 tabloda yok; 7 araç testsiz.

| Araç | Sorun | Etkilenen ihtiyaç (iç: ilk kullanan oyun) | Kaynak |
|---|---|---|---|
| `perf_capture`, `perf_scene`, `perf_gpu_profile` | Testsiz + RELIABILITY'de yok (0.4 turundan sonra eklendi) | EW-040 (GP G1 M9, RC IS-31) | arac_test_kapsami.json |
| `mrq_render`, `mrq_status` | Testsiz + RELIABILITY'de yok | EW-101 (RC IS-53) | arac_test_kapsami.json |
| `asset_references` | Testsiz (RELIABILITY'de satırı var) | EW-006, EW-074 (RC IS-30 ★, IS-34) | arac_test_kapsami.json |
| `pie_start` | NOTE: ağır ana menü haritasında ilk açılış 103 s; 2 oyunculu listen orada `TIMEOUT` (iç: RC) | EW-012, EW-030 (RC IS-33, GP GI-07) | RELIABILITY_0.4.md:140, §Kept for 0.4.1 |
| `asset_import` | NOTE: Türkçe dosya adı uyarısız Türkçe asset adı olur (0.4.1) | EW-070, EW-071 (RC 155 asset + 186 ses) | RELIABILITY_0.4.md:68 |
| `actor_preview` | NOTE: kamera kollu Character gevşek çerçeve (0.4.1) | EW-065 (ARPG 66 varyant panosu) | RELIABILITY_0.4.md:61 |
| `asset_find` | NOTE: olmayan klasör uyarısız boş liste (0.4.1) | EW-003 | RELIABILITY_0.4.md:67 |
| `game_run` | FIXED; 8 ihtiyaçla en yüklü araç. Gerçek projede 145 koşu / 243 MB, saklama kuralı yok (0.4.x; iç: RC) | EW-011, 013, 018, 020, 032, 053, 054, 064 | RELIABILITY_0.4.md §Kept for 0.4.1 |

Bu tablo 09 §5 teknik borç listesine girer; 08'de 0.4.x sürümü bunları kapatır.

## 6. Oyun belgelerindeki eski durum notları

Oyun repoları salt okunur. Aşağıdaki farklar yalnız burada kayıtlı; oyun belgesi düzeltmesi Talha'nın kararı (SORULAR_2 S2-04).

| Oyun belgesi | Belgedeki durum | Kaynak koddaki durum |
|---|---|---|
| RC12 §11.1 EW-R20 | "Kısmen: tek çağrıda bütün proje doğrulaması yok (EKSİK B11)" | `validate` var (ImportValidationTools.cpp:235; blueprints, data, references, map; RELIABILITY FIXED) → **Var** |
| GP16 / GP05 §8 EW-GP-01 | `automation_run` eksik araç olarak listeli | `automation_run`, `automation_list` var (editör içi). Eksik: başsız koşu + XML ayrıştırma → **Kısmen** (EW-011) |
| GP16 EW-GP-06 | `perf_capture` + `scope` | `perf_capture` var (PerfTools.cpp:105); `scope` ve `-game` yok → EW-040 Var, EW-041/042 Yok |
