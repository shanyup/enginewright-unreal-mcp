# 07 — İhtiyaç → mevcut araç eşlemesi

Kart: `t_ae3a845a` · 18.11.2026 · Yalnız belge. UE açılmadı, araç çağrılmadı. Genel ad düzeltmesi: kart `t_5f61e99c` (18.11).
**Genel MCP kuralı (Talha 18.11):** araç adı ve açıklamalarında oyun adı geçmez. Oyun bağlantısı yalnız ihtiyaç kaynağı ve **(iç)** etiketli sütunlarda (iç eşleme).
**Bu belge `03_Yol_Haritasi.md` §araç eşleme kısmının ve `02_Oyun_Ihtiyaclari.md` "mevcut araç" sütununun yerini alır.**

Kaynak (hepsi otomatik, `research/`):
- Araç listesi: `arac_envanteri.py` kaynak koddan okur (`Plugins/Enginewright/Source/Enginewright/Private/**/*.cpp`). **129 araç** = 109 `R.Register(` + 16 `R.RegisterAsync(` + 4 `Reg.RegisterAsync(`; çift ad yok (grep sayımı ile aynı).
- Güvenilirlik: `arac_test_kapsami.py` → `docs/RELIABILITY_0.4.md` durumu (OK / FIXED / NOTE) + `tests/*.py` içinde aracı çağıran dosya sayısı.
- İhtiyaç → araç: `ihtiyac_esleme.py` (betik, adı kayıtta olmayan aracı reddeder).

## 1. Özet

| Durum | Sayı | Anlamı |
|---|---|---|
| **Var** | 18 | İş bugünkü araçlarla tek ya da birkaç çağrıda yapılır. Gerçek projede ilk deneme koşusu çoğunda yapılmadı (iç: RC12 §11.1 notu). |
| **Kısmen** | 20 | Parçalar var; eksik kısım ajan döngüsüyle ya da oyun betiğiyle kapanıyor. |
| **Yok** | 18 | Araç yok. Yol haritasında yeni araç (08). |

Kümeye göre (Var / Kısmen / Yok):

| Küme | Var | Kısmen | Yok | Yorum (iç: hangi oyun bekliyor) |
|---|---|---|---|---|
| VERİ (7) | 3 | 3 | 1 | Çekirdek güçlü; dosyadan upsert (E17) ve dışa aktarma eksik |
| TEST (11) | 2 | 6 | 3 | En büyük boşluk YZ test ve ölçüm araçları (metrik, vislog, StateTree izi); iç: GP YZ-0 |
| AĞ (3) | 1 | 2 | 0 | Tek makine co-op var; replikasyon iddiası ajan tarafında |
| PERF (4) | 1 | 0 | 3 | PIE ölçümü var; paketli FPS (E22), stat filtresi, paketleme yok |
| LOC (6) | 4 | 1 | 1 | RC'nin tek "Yok" P0'ı burada: EW-055 taşma testi |
| GÖRSEL (6) | 3 | 3 | 0 | Hazır; toplu tur / bölge maskesi / varyant panosu kısmen |
| SANAT (6) | 2 | 3 | 1 | İskelet + anim (E23) ve slot başına MI (E24) eksik; iç: ARPG |
| YZ (5) | 0 | 1 | 4 | Oynanış yazarlığı: StateTree, AnimBP, GAS, Niagara hiç yok; iç: ARPG ana yükü |
| SEVİYE (3) | 0 | 1 | 2 | NavMesh (B15) ve oda doğrulama eksik; iç: GP + ARPG ortak |
| MEDYA (3) | 1 | 0 | 2 | MRQ var; Sequencer ve kayıt yok |
| ALTYAPI (2) | 1 | 0 | 1 | Headless (B18) yok |

Kaynak: `research/ihtiyac_esleme_ozet.json`; küme sayıları §2 tablosundan sayıldı.

## 2. İhtiyaç bazlı eşleme

Sütun "Güvenilirlik": her araç için RELIABILITY_0.4 durumu ve aracı çağıran test dosyası sayısı. **testsiz** = `tests/` içinde aracı çağıran dosya yok. "tabloda yok" = RELIABILITY_0.4'te satır yok (araç 0.4 turundan sonra eklendi).

| ID | İhtiyaç (kısa) | Durum | Araç (kaynak dosya:satır) | Eksik kısım | Güvenilirlik (RELIABILITY_0.4 + test dosyası sayısı) |
|---|---|---|---|---|---|
| EW-001 | DataTable satırlarını katı denetimle toplu yazma (bilinmeyen alan = ha… | **Var** | `datatable_set_rows` (DataTableTools.cpp:144), `datatable_create` (DataTableTools.cpp:66), `datatable_get` (DataTableTools.cpp:94), `datatable_remove_rows` (DataTableTools.cpp:248), `datatable_rename_row` (DataTableTools.cpp:279), `type_info` (TypeTools.cpp:218) | Oyunda ilk deneme koşusu yapılmadı (RC12 §11.1 "Var = davranış RagnaCook'ta denenmedi") | datatable_set_rows: FIXED, test 2; datatable_create: OK, test 4; datatable_get: FIXED, test 2; datatable_remove_rows: FIXED, test 1; datatable_rename_row: FIXED, test 1; type_info: OK, test 1 |
| EW-002 | CSV/JSON dosyasından şemaya göre upsert + fark raporu (eklenen/değişen… | **Kısmen** | `datatable_set_rows` (DataTableTools.cpp:144), `batch` (BatchTool.cpp:26) | Dosyadan okuma ve fark raporu yok; ajan dosyayı JSON'a çevirip `datatable_set_rows` çağırıyor | datatable_set_rows: FIXED, test 2; batch: OK, test 4 |
| EW-003 | DataAsset toplu oluşturma / okuma / doğrulama (100+ asset) | **Kısmen** | `object_get` (ObjectTools.cpp:12), `object_set` (ObjectTools.cpp:45), `asset_duplicate` (AssetTools.cpp:300), `asset_find` (AssetTools.cpp:211), `batch` (BatchTool.cpp:26) | DataAsset oluşturma aracı yok (yalnız var olanı `object_set` ile yazma); toplu "şablondan N asset" yok | object_get: OK, test 6; object_set: OK, test 7; asset_duplicate: FIXED, test 1; asset_find: NOTE, test 3; batch: OK, test 4 |
| EW-004 | Sim ↔ oyun verisi köprüsü: oyundaki veriyi dışa aktar, sim çıktısıyla … | **Kısmen** | `datatable_get` (DataTableTools.cpp:94), `object_get` (ObjectTools.cpp:12) | DataTable/asset → CSV/JSON dışa aktarma ve sim çıktısıyla fark raporu yok | datatable_get: FIXED, test 2; object_get: OK, test 6 |
| EW-005 | ID bütünlüğü: tanımlı ↔ kullanılan ID'ler (DataTable, BP, data asset) | **Var** | `id_audit` (AuditTools.cpp:170), `asset_search_text` (AuditTools.cpp:142) | BP içindeki sabit FName'lerin kapsandığı RC'de denenmedi (RC12 §11.1 EW-R03) | id_audit: FIXED, test 1; asset_search_text: FIXED, test 1 |
| EW-006 | Tek çağrıda proje doğrulaması: bütün BP derle, Data Validation, kırık … | **Var** | `validate` (ImportValidationTools.cpp:235), `blueprint_compile` (BlueprintTools.cpp:450), `asset_references` (AssetTools.cpp:273), `build_status` (BuildTools.cpp:158) | RC12 §11.1 EW-R20 'Kısmen / B11 yok' diyor; kodda `validate` (ImportValidationTools.cpp:235: blueprints, data, references, map) var → RC belgesi eski (oyun reposu salt okunur: SORULAR_2'de not) | validate: FIXED, test 1; blueprint_compile: FIXED, test 1; asset_references: FIXED, **testsiz**; build_status: OK, test 1 |
| EW-007 | İçerik duman testi: her veri satırını oyunda spawn edip hatasız mı bak… | **Yok** | — | Satır başına spawn + log iddiası döngüsü yok (bugün `actor_spawn` + `log_wait` ile ajan döngüsü) | — |
| EW-010 | Automation testlerini koşup sonucu ayrıştırma (geçti/kaldı/süre) | **Var** | `automation_run` (BuildTools.cpp:210), `automation_list` (BuildTools.cpp:280), `console_command` (EditorTools.cpp:29), `log_wait` (SettingsLogTools.cpp:139) | GP16 EW-GP-01'i "eksik" sayıyor; araç var (editör içinde). Başsız koşu ayrı satır: EW-011 | automation_run: FIXED, test 1; automation_list: FIXED, test 1; console_command: OK, test 6; log_wait: FIXED, test 2 |
| EW-011 | Başsız automation koşusu (`-game -nullrhi` / paket) + XML/json rapor a… | **Kısmen** | `game_run` (GameRunTools.cpp:991), `game_run_status` (GameRunTools.cpp:1261) | `game_run` `args`/`cvars`/`pass`/`fail` ile başsız süreç başlatır; automation rapor dosyasını ayrıştırma yok | game_run: FIXED, test 2; game_run_status: FIXED, test 1 |
| EW-012 | PIE senaryo oynatıcı: adım listesi (girdi → bekle → durum iddiası) tek… | **Kısmen** | `pie_start` (PieTools.cpp:233), `pie_input` (PieTools.cpp:305), `pie_walk_to` (PieTools.cpp:360), `pie_actors` (MultiplayerTools.cpp:123), `pie_state` (PieTools.cpp:299), `log_wait` (SettingsLogTools.cpp:139), `console_command` (EditorTools.cpp:29) | Hazır kalıp yok; zamanlı girdi dizisi (yön + güç) yok; adımlar ajan döngüsüyle bağlanıyor | pie_start: NOTE, test 7; pie_input: OK, test 2; pie_walk_to: OK, test 1; pie_actors: OK, test 2; pie_state: OK, test 1; log_wait: FIXED, test 2; console_command: OK, test 6 |
| EW-013 | Bot playtest + denge koşusu: oyunun kendi botunu başlat, metrikleri to… | **Kısmen** | `game_run` (GameRunTools.cpp:991), `pie_input` (PieTools.cpp:305), `pie_walk_to` (PieTools.cpp:360), `console_command` (EditorTools.cpp:29), `log_wait` (SettingsLogTools.cpp:139) | Bot oyunun içinde (C++). EW tarafı: başlat + bekle + metrik dosyasını oku. Okuma kısmı yok (EW-014) | game_run: FIXED, test 2; pie_input: OK, test 2; pie_walk_to: OK, test 1; console_command: OK, test 6; log_wait: FIXED, test 2 |
| EW-014 | Oyunun yazdığı ölçüm dosyasını (json/csv) okuyup kapıya göre özet | **Yok** | — | Dosya okuma + eşik karşılaştırma aracı yok (ajan ham dosyayı bağlama okuyor) | — |
| EW-015 | Visual Logger dökümü: aktör + zaman aralığı → metin/PNG | **Yok** | — | `.bvlog` okuma yok | — |
| EW-016 | Gameplay Debugger anlık görüntü (seçili YZ, kategori, ekran + metin) | **Kısmen** | `console_command` (EditorTools.cpp:29), `screenshot` (PieTools.cpp:574) | Kategori seçimi + metin dökümü yok; ekran görüntüsü alınabiliyor | console_command: OK, test 6; screenshot: FIXED, test 3 |
| EW-017 | StateTree hata ayıklayıcı izini metne çevirme | **Yok** | — | StateTree okuma/iz aracı yok | — |
| EW-018 | Soak koşusu: paket `-game -nullrhi` uzun koşu, 60 sn'de bellek/ms/takı… | **Kısmen** | `game_run` (GameRunTools.cpp:991), `game_run_status` (GameRunTools.cpp:1261) | Uzun koşu + log iddiası var; periyodik bellek/ms örnekleme ve çökme özeti yok | game_run: FIXED, test 2; game_run_status: FIXED, test 1 |
| EW-019 | Senaryo haritasına YZ / bot / Smart Object'i veri listesinden yerleşti… | **Kısmen** | `actor_spawn` (SceneTools.cpp:142), `scene_dress` (SceneDressTools.cpp:115), `batch` (BatchTool.cpp:26), `level_save` (EditorTools.cpp:94) | Veri listesinden toplu spawn + Smart Object ayarı yok (tek tek `actor_spawn`) | actor_spawn: FIXED, test 5; scene_dress: FIXED, test 1; batch: OK, test 4; level_save: FIXED, test 1 |
| EW-020 | Kayıt testi ve göçü: kayıt alanı yaz/oku, demo → EA kaydı | **Var** | `savegame_get` (SaveGameTools.cpp:110), `savegame_set` (SaveGameTools.cpp:153), `game_run` (GameRunTools.cpp:991) | İki ayrı paket (demo ve EA) arası akış elle (RC12 §11.1) | savegame_get: OK, test 1; savegame_set: OK, test 1; game_run: FIXED, test 2 |
| EW-030 | Tek makinede N istemcili co-op testi + kötü ağ emülasyonu | **Var** | `pie_start` (PieTools.cpp:233), `pie_net_emulation` (MultiplayerTools.cpp:196), `pie_state` (PieTools.cpp:299), `pie_travel` (PieTools.cpp:480), `pie_actors` (MultiplayerTools.cpp:123) | Host kopması senaryosu elle (RC12 §11.1); ana menüden 2 oyuncu `TIMEOUT` (RELIABILITY_0.4.md:140 NOTE) | pie_start: NOTE, test 7; pie_net_emulation: OK, test 2; pie_state: OK, test 1; pie_travel: OK, test 1; pie_actors: OK, test 2 |
| EW-031 | Replikasyon iddiası: server ↔ istemci değer karşılaştırması tek çağrıd… | **Kısmen** | `pie_actors` (MultiplayerTools.cpp:123) | `pie_actors` her dünyadaki değeri listeler; fark hesabı/iddia ajan tarafında | pie_actors: OK, test 2 |
| EW-032 | Paketli build ile 2 makine / Steam co-op testi | **Kısmen** | `game_run` (GameRunTools.cpp:991) | Tek makinede paketli exe başlatılabiliyor; ikinci makine ve Steam oturumu kapsam dışı (TAHMİN: araçla çözülmez, elle) | game_run: FIXED, test 2 |
| EW-040 | Editör/PIE kare süresi, sahne maliyeti, GPU geçiş profili | **Var** | `perf_capture` (PerfTools.cpp:105), `perf_scene` (PerfTools.cpp:180), `perf_gpu_profile` (PerfTools.cpp:303) | Üç araç da testsiz ve RELIABILITY tablosunda yok (arac_test_kapsami.json) | perf_capture: tabloda yok, **testsiz**; perf_scene: tabloda yok, **testsiz**; perf_gpu_profile: tabloda yok, **testsiz** |
| EW-041 | Stat grubu filtresi (ör. `GPAI_*` scope) ile ölçüm | **Yok** | — | `perf_capture`'da stat grubu seçimi yok | — |
| EW-042 | Paketli / `-game` oyunda FPS örnekleme (Steam Deck 800p dahil) | **Yok** | — | `game_run` oyunu açar ama kare süresi örneklemez (E22 açık); Deck cihazı ölçümü elle | — |
| EW-043 | Paketleme / cook + durum takibi (demo build, dedicated server) | **Yok** | — | Elle `.bat` / RunUAT (EKS B17) | — |
| EW-050 | String Table toplu yazma (kaynak EN + TR) | **Var** | `stringtable_set` (LocalizationTools.cpp:347), `stringtable_get` (LocalizationTools.cpp:299), `stringtable_list` (LocalizationTools.cpp:271) | CSV'den yüklenen tablo salt okunur → RC asset tablo kullanmalı (RC12 §11.1) | stringtable_set: OK, test 3; stringtable_get: OK, test 2; stringtable_list: OK, test 2 |
| EW-051 | Çeviri eksik / bozuk kontrolü (eksik anahtar, `{x}`, çevrilmemiş) | **Var** | `stringtable_compare` (LocalizationTools.cpp:444) | — | stringtable_compare: FIXED, test 2 |
| EW-052 | Lokalizasyon gather / export / import / compile (.po) | **Var** | `loc_gather` (LocGatherTools.cpp:449), `loc_gather_status` (LocGatherTools.cpp:549) | — | loc_gather: OK, test 1; loc_gather_status: OK, test 1 |
| EW-053 | Glif testi (CJK, Tay, Arap fontlarında eksik karakter) | **Var** | `font_coverage` (LocalizationTools.cpp:655), `game_run` (GameRunTools.cpp:991) | Arapça bağlı harf şekli yalnız ekran görüntüsüyle (RC12 §11.1) | font_coverage: OK, test 2; game_run: FIXED, test 2 |
| EW-054 | Dil × ekran görüntüsü döngüsü (her dilde bütün UI ekranları, tek çağrı… | **Kısmen** | `loc_culture` (LocalizationTools.cpp:806), `widget_render` (UMGTools.cpp:433), `screenshot` (PieTools.cpp:574), `ui_open` (UIRuntimeTools.cpp:961), `ui_state` (UIRuntimeTools.cpp:720), `game_run` (GameRunTools.cpp:991) | "Her dil × her ekran" tek çağrı yok; ajan döngüsü | loc_culture: OK, test 2; widget_render: OK, test 2; screenshot: FIXED, test 3; ui_open: OK, test 3; ui_state: OK, test 2; game_run: FIXED, test 2 |
| EW-055 | Ekran taşma testi: kesilen / taşan metin sayısı (TextBlock istenen > a… | **Yok** | — | `text_overflow_audit` yok; olmazsa ekran görüntüsü + göz (RC12 §11.1) | — |
| EW-060 | Oyun kamerasından kare (altın kare, mağaza/trailer kareleri) | **Var** | `screenshot` (PieTools.cpp:574), `screenshot_ab` (PieTools.cpp:628), `image_compose` (ImageTools.cpp:159), `image_stats` (ImageTools.cpp:376) | — | screenshot: FIXED, test 3; screenshot_ab: FIXED, test 1; image_compose: OK, test 1; image_stats: OK, test 1 |
| EW-061 | Okunabilirlik: nesne ekranda kaç px (yemek ≥ 28 px) | **Var** | `screen_coverage` (MeasureTools.cpp:104), `class_measure` (MeasureTools.cpp:37), `actor_bounds` (SceneTools.cpp:229) | — | screen_coverage: OK, test 2; class_measure: OK, test 3; actor_bounds: FIXED, test 2 |
| EW-062 | Önce/sonra kolaj + sayısal görsel fark (SSIM) | **Var** | `screenshot_ab` (PieTools.cpp:628), `image_compose` (ImageTools.cpp:159), `image_stats` (ImageTools.cpp:376) | — | screenshot_ab: FIXED, test 1; image_compose: OK, test 1; image_stats: OK, test 1 |
| EW-063 | Toplu stil ölçümü: bölge maskesiyle palet / L* / gürültü (G1–G6) | **Kısmen** | `image_stats` (ImageTools.cpp:376) | Bölge maskesi ve palet ölçümü yok (RC betiği yapıyor) | image_stats: OK, test 1 |
| EW-064 | Kamera turu: N konumdan sırayla kare + manifest | **Kısmen** | `screenshot` (PieTools.cpp:574), `game_run` (GameRunTools.cpp:991) | Waypoint listesinden tur yok (`game_run` `shotNames` oyun kendi çekerse) | screenshot: FIXED, test 3; game_run: FIXED, test 2 |
| EW-065 | Varyant önizleme panosu: malzeme override + animasyon karesi | **Kısmen** | `actor_preview` (ActorPreviewTools.cpp:147), `image_compose` (ImageTools.cpp:159) | `actor_preview`'da `material_overrides` ve `anim/time` yok; Character çerçevesi gevşek (RELIABILITY_0.4.md:61 NOTE, 0.4.1'e) | actor_preview: NOTE, test 1; image_compose: OK, test 1 |
| EW-070 | Editör açıkken toplu içe aktarma (mesh, doku, ses, font) | **Var** | `asset_import` (ImportValidationTools.cpp:82), `batch` (BatchTool.cpp:26), `mesh_info` (MeshTextureTools.cpp:71), `texture_info` (MeshTextureTools.cpp:172) | Klasör tarama + ad kuralı yok (dosya listesi verilir); Türkçe dosya adı uyarısız (RELIABILITY_0.4.md:68 NOTE, 0.4.1'e) | asset_import: NOTE, test 1; batch: OK, test 4; mesh_info: FIXED, test 1; texture_info: FIXED, test 1 |
| EW-071 | İskeletli mesh + animasyon import, IK retarget, soket, Physics Asset, … | **Kısmen** | `asset_import` (ImportValidationTools.cpp:82), `mesh_info` (MeshTextureTools.cpp:71) | `asset_import` `meshType: skeletal` var; animasyon, retarget, soket, Physics Asset, montage yok (E23) | asset_import: NOTE, test 1; mesh_info: FIXED, test 1 |
| EW-072 | Malzeme varyantı: master → MI, FBX slotu başına MI + doku bağlama | **Kısmen** | `material_instance_create` (MaterialTools.cpp:486), `material_instance_set` (MaterialTools.cpp:533), `mesh_set_material` (MeshTextureTools.cpp:124) | Slot başına toplu MI (`material_instances_for_mesh`) yok (E24) | material_instance_create: FIXED, test 1; material_instance_set: FIXED, test 1; mesh_set_material: FIXED, test 1 |
| EW-073 | Sanat bütçesi kontrolü (üçgen, doku boyutu)  | **Kısmen** | `mesh_info` (MeshTextureTools.cpp:71), `texture_info` (MeshTextureTools.cpp:172) | Tek asset okunuyor; klasör + eşik raporu yok | mesh_info: FIXED, test 1; texture_info: FIXED, test 1 |
| EW-074 | Güvenli taşıma + kalan yer tutucu taraması (oyuncu yolunda referans = … | **Var** | `asset_rename` (AssetTools.cpp:324), `asset_references` (AssetTools.cpp:273), `asset_delete` (AssetTools.cpp:414), `asset_search_text` (AuditTools.cpp:142) | "Kalan yer tutucu" toplu raporu yok (klasör → referans sayısı) | asset_rename: OK, test 2; asset_references: FIXED, **testsiz**; asset_delete: FIXED, test 3; asset_search_text: FIXED, test 1 |
| EW-075 | Asset kaynağı / lisans / AI beyanı metadata + beyan raporu | **Yok** | — | Metadata yazma/okuma ve rapor yok | — |
| EW-080 | StateTree / Behavior Tree yazma | **Yok** | — | Okuma da yazma da yok | — |
| EW-081 | AnimBP state machine şablonu (iskelet ailesi başına) | **Yok** | `blueprint_graph_read` (BlueprintTools.cpp:754) | AnimGraph yazma yok | blueprint_graph_read: FIXED, test 1 |
| EW-082 | GAS: Ability / Effect / Attribute / Cue oluştur, oku, test | **Yok** | — | Hiç yok | — |
| EW-083 | Niagara parametre / renk-element varyantı / önizleme | **Yok** | — | Hiç yok | — |
| EW-084 | Fizik / ragdoll doğrulama (ganimet taşıma, sekme determinizmi) | **Kısmen** | `pie_actors` (MultiplayerTools.cpp:123), `world_trace` (SceneTools.cpp:249), `console_command` (EditorTools.cpp:29) | Fizik senaryosu + sonuç ölçümü yok | pie_actors: OK, test 2; world_trace: FIXED, test 2; console_command: OK, test 6 |
| EW-090 | NavMesh build + kopuk ada / ulaşılamaz nokta raporu | **Yok** | `pie_walk_to` (PieTools.cpp:360), `world_trace` (SceneTools.cpp:249) | Build ve erişilebilirlik raporu yok (`pie_walk_to` STUCK ile dolaylı) | pie_walk_to: OK, test 1; world_trace: FIXED, test 2 |
| EW-091 | Oda parçası + kat doğrulayıcı (bağlantı noktası, ızgara) | **Yok** | `actor_bounds` (SceneTools.cpp:229), `world_trace` (SceneTools.cpp:249), `mesh_info` (MeshTextureTools.cpp:71) | Port/ızgara kuralı yok | actor_bounds: FIXED, test 2; world_trace: FIXED, test 2; mesh_info: FIXED, test 1 |
| EW-092 | Yerleşim seti: iş noktası grupları + tohum (veri → seviye) | **Kısmen** | `scene_dress` (SceneDressTools.cpp:115), `actor_spawn` (SceneTools.cpp:142), `batch` (BatchTool.cpp:26) | Veri listesinden grup yerleştirme yok | scene_dress: FIXED, test 1; actor_spawn: FIXED, test 5; batch: OK, test 4 |
| EW-100 | Sequencer kurma (Level Sequence, kamera, kesme, anahtar) | **Yok** | — | Hiç yok (EKS E29: bugün Python ile) | — |
| EW-101 | Movie Render Queue çalıştırma | **Var** | `mrq_render` (CinematicTools.cpp:134), `mrq_status` (CinematicTools.cpp:257) | İki araç da testsiz (arac_test_kapsami.json) | mrq_render: tabloda yok, **testsiz**; mrq_status: tabloda yok, **testsiz** |
| EW-102 | Oynanış kaydı (Take Recorder → sekans → MRQ) | **Yok** | — | Hiç yok | — |
| EW-110 | Headless mod: aynı araçlar editör kapalıyken (import, veri) | **Yok** | `loc_gather` (LocGatherTools.cpp:449) | Yalnız `loc_gather` commandlet ile çalışıyor | loc_gather: OK, test 1 |
| EW-111 | Sabit tuş denetimi + girdi istemleri | **Var** | `key_audit` (AuditTools.cpp:236), `ui_input_audit` (UIRuntimeTools.cpp:1132), `ui_prompts` (UIRuntimeTools.cpp:538) | — | key_audit: FIXED, test 1; ui_input_audit: OK, test 2; ui_prompts: OK, test 2 |


## 3. Araç bazlı ters tablo

Hangi araç kaç ihtiyaca hizmet ediyor. Sıra: ihtiyaç sayısı (çok → az). 72 araç en az bir ihtiyaca bağlı; 57 araç hiçbir oyun ihtiyacında geçmiyor (§4).

| Araç | Kategori | Kaynak | Hizmet ettiği ihtiyaç sayısı | İhtiyaçlar | RELIABILITY | Test dosyası |
|---|---|---|---|---|---|---|
| `game_run` | Play | GameRunTools.cpp:991 | 8 | EW-011, EW-013, EW-018, EW-020, EW-032, EW-053, EW-054, EW-064 | FIXED | 2 |
| `batch` | Editor | BatchTool.cpp:26 | 5 | EW-002, EW-003, EW-019, EW-070, EW-092 | OK | 4 |
| `console_command` | Editor | EditorTools.cpp:29 | 5 | EW-010, EW-012, EW-013, EW-016, EW-084 | OK | 6 |
| `mesh_info` | Assets | MeshTextureTools.cpp:71 | 4 | EW-070, EW-071, EW-073, EW-091 | FIXED | 1 |
| `pie_actors` | Multiplayer | MultiplayerTools.cpp:123 | 4 | EW-012, EW-030, EW-031, EW-084 | OK | 2 |
| `screenshot` | Play | PieTools.cpp:574 | 4 | EW-016, EW-054, EW-060, EW-064 | FIXED | 3 |
| `image_compose` | Images | ImageTools.cpp:159 | 3 | EW-060, EW-062, EW-065 | OK | 1 |
| `image_stats` | Images | ImageTools.cpp:376 | 3 | EW-060, EW-062, EW-063 | OK | 1 |
| `log_wait` | Editor | SettingsLogTools.cpp:139 | 3 | EW-010, EW-012, EW-013 | FIXED | 2 |
| `pie_walk_to` | Play | PieTools.cpp:360 | 3 | EW-012, EW-013, EW-090 | OK | 1 |
| `world_trace` | Level | SceneTools.cpp:249 | 3 | EW-084, EW-090, EW-091 | FIXED | 2 |
| `actor_bounds` | Level | SceneTools.cpp:229 | 2 | EW-061, EW-091 | FIXED | 2 |
| `actor_spawn` | Level | SceneTools.cpp:142 | 2 | EW-019, EW-092 | FIXED | 5 |
| `asset_import` | Assets | ImportValidationTools.cpp:82 | 2 | EW-070, EW-071 | NOTE | 1 |
| `asset_references` | Assets | AssetTools.cpp:273 | 2 | EW-006, EW-074 | FIXED | 0 |
| `asset_search_text` | Audit | AuditTools.cpp:142 | 2 | EW-005, EW-074 | FIXED | 1 |
| `datatable_get` | Data | DataTableTools.cpp:94 | 2 | EW-001, EW-004 | FIXED | 2 |
| `datatable_set_rows` | Data | DataTableTools.cpp:144 | 2 | EW-001, EW-002 | FIXED | 2 |
| `game_run_status` | Play | GameRunTools.cpp:1261 | 2 | EW-011, EW-018 | FIXED | 1 |
| `loc_gather` | Localization | LocGatherTools.cpp:449 | 2 | EW-052, EW-110 | OK | 1 |
| `object_get` | Objects | ObjectTools.cpp:12 | 2 | EW-003, EW-004 | OK | 6 |
| `pie_input` | Play | PieTools.cpp:305 | 2 | EW-012, EW-013 | OK | 2 |
| `pie_start` | Play | PieTools.cpp:233 | 2 | EW-012, EW-030 | NOTE | 7 |
| `pie_state` | Play | PieTools.cpp:299 | 2 | EW-012, EW-030 | OK | 1 |
| `scene_dress` | Level | SceneDressTools.cpp:115 | 2 | EW-019, EW-092 | FIXED | 1 |
| `screenshot_ab` | Play | PieTools.cpp:628 | 2 | EW-060, EW-062 | FIXED | 1 |
| `texture_info` | Assets | MeshTextureTools.cpp:172 | 2 | EW-070, EW-073 | FIXED | 1 |
| `actor_preview` | Level | ActorPreviewTools.cpp:147 | 1 | EW-065 | NOTE | 1 |
| `asset_delete` | Assets | AssetTools.cpp:414 | 1 | EW-074 | FIXED | 3 |
| `asset_duplicate` | Assets | AssetTools.cpp:300 | 1 | EW-003 | FIXED | 1 |
| `asset_find` | Assets | AssetTools.cpp:211 | 1 | EW-003 | NOTE | 3 |
| `asset_rename` | Assets | AssetTools.cpp:324 | 1 | EW-074 | OK | 2 |
| `automation_list` | Build | BuildTools.cpp:280 | 1 | EW-010 | FIXED | 1 |
| `automation_run` | Build | BuildTools.cpp:210 | 1 | EW-010 | FIXED | 1 |
| `blueprint_compile` | Blueprints | BlueprintTools.cpp:450 | 1 | EW-006 | FIXED | 1 |
| `blueprint_graph_read` | Blueprints | BlueprintTools.cpp:754 | 1 | EW-081 | FIXED | 1 |
| `build_status` | Build | BuildTools.cpp:158 | 1 | EW-006 | OK | 1 |
| `class_measure` | Level | MeasureTools.cpp:37 | 1 | EW-061 | OK | 3 |
| `datatable_create` | Data | DataTableTools.cpp:66 | 1 | EW-001 | OK | 4 |
| `datatable_remove_rows` | Data | DataTableTools.cpp:248 | 1 | EW-001 | FIXED | 1 |
| `datatable_rename_row` | Data | DataTableTools.cpp:279 | 1 | EW-001 | FIXED | 1 |
| `font_coverage` | Localization | LocalizationTools.cpp:655 | 1 | EW-053 | OK | 2 |
| `id_audit` | Audit | AuditTools.cpp:170 | 1 | EW-005 | FIXED | 1 |
| `key_audit` | Audit | AuditTools.cpp:236 | 1 | EW-111 | FIXED | 1 |
| `level_save` | Editor | EditorTools.cpp:94 | 1 | EW-019 | FIXED | 1 |
| `loc_culture` | Localization | LocalizationTools.cpp:806 | 1 | EW-054 | OK | 2 |
| `loc_gather_status` | Localization | LocGatherTools.cpp:549 | 1 | EW-052 | OK | 1 |
| `material_instance_create` | Materials | MaterialTools.cpp:486 | 1 | EW-072 | FIXED | 1 |
| `material_instance_set` | Materials | MaterialTools.cpp:533 | 1 | EW-072 | FIXED | 1 |
| `mesh_set_material` | Assets | MeshTextureTools.cpp:124 | 1 | EW-072 | FIXED | 1 |
| `mrq_render` | Cinematics | CinematicTools.cpp:134 | 1 | EW-101 | - | 0 |
| `mrq_status` | Cinematics | CinematicTools.cpp:257 | 1 | EW-101 | - | 0 |
| `object_set` | Objects | ObjectTools.cpp:45 | 1 | EW-003 | OK | 7 |
| `perf_capture` | Performance | PerfTools.cpp:105 | 1 | EW-040 | - | 0 |
| `perf_gpu_profile` | Performance | PerfTools.cpp:303 | 1 | EW-040 | - | 0 |
| `perf_scene` | Performance | PerfTools.cpp:180 | 1 | EW-040 | - | 0 |
| `pie_net_emulation` | Multiplayer | MultiplayerTools.cpp:196 | 1 | EW-030 | OK | 2 |
| `pie_travel` | Play | PieTools.cpp:480 | 1 | EW-030 | OK | 1 |
| `savegame_get` | Play | SaveGameTools.cpp:110 | 1 | EW-020 | OK | 1 |
| `savegame_set` | Play | SaveGameTools.cpp:153 | 1 | EW-020 | OK | 1 |
| `screen_coverage` | Level | MeasureTools.cpp:104 | 1 | EW-061 | OK | 2 |
| `stringtable_compare` | Localization | LocalizationTools.cpp:444 | 1 | EW-051 | FIXED | 2 |
| `stringtable_get` | Localization | LocalizationTools.cpp:299 | 1 | EW-050 | OK | 2 |
| `stringtable_list` | Localization | LocalizationTools.cpp:271 | 1 | EW-050 | OK | 2 |
| `stringtable_set` | Localization | LocalizationTools.cpp:347 | 1 | EW-050 | OK | 3 |
| `type_info` | Objects | TypeTools.cpp:218 | 1 | EW-001 | OK | 1 |
| `ui_input_audit` | UI | UIRuntimeTools.cpp:1132 | 1 | EW-111 | OK | 2 |
| `ui_open` | UI | UIRuntimeTools.cpp:961 | 1 | EW-054 | OK | 3 |
| `ui_prompts` | UI | UIRuntimeTools.cpp:538 | 1 | EW-111 | OK | 2 |
| `ui_state` | UI | UIRuntimeTools.cpp:720 | 1 | EW-054 | OK | 2 |
| `validate` | Audit | ImportValidationTools.cpp:235 | 1 | EW-006 | FIXED | 1 |
| `widget_render` | UI | UMGTools.cpp:433 | 1 | EW-054 | OK | 2 |


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
