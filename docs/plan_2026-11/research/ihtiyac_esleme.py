# -*- coding: utf-8 -*-
"""Birleşik ihtiyaç listesi (06) + araç eşlemesi (07) için tek veri kaynağı.

Girdi : arac_test_kapsami.json (arac_envanteri.py + arac_test_kapsami.py üretir; kaynak koddan)
Çıktı : ihtiyac_esleme.json, gen_06_tablo.md, gen_07_tablo.md, gen_07_ters.md
Çalıştır: python ihtiyac_esleme.py   (bu klasörde)

Kısaltmalar (kaynak belgeler, hepsi YALNIZ okundu, 18.11.2026):
  RC12  = UE/RagnaCook/Docs/Icerik_Sistemi/12_Icerik_Uretim_Sistemi.md  (§11.1 = EW-R01..R22)
  RC13E = UE/RagnaCook/Docs/Icerik_Sistemi/13_Ek_Is_ve_Esleme.md         (IS/EK/LOC işleri, aşama)
  RC20  = UE/RagnaCook/Docs/Icerik_Sistemi/20_Lokalizasyon_Sistemi.md
  GP16  = Game/GunpointParole/docs/GDD_v2_DusmanYZ.md §16               (EW-GP-01..09, YENİ)
  GP05  = Game/GunpointParole/docs/YZ_v2/05_Otomatik_YZ_Test_Sistemi.md §8
  GP06  = Game/GunpointParole/docs/YZ_v2/06_Uygulama_Plani.md §2 (YZ-0..YZ-5, kapı G0..G5)
  GP08  = Game/GunpointParole/docs/Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md §7 (EW-GP-1..5, ESKİ)
  GP09  = Game/GunpointParole/docs/Icerik_Sistemi/09_Is_Listesi.md (GI-xx)
  AT    = Game/ARPG/Docs/ARPG_Tasarim_Kararlari.md §12.3 (EW-1..11)
  A09   = Game/ARPG/Docs/Icerik_Sistemi/09_Is_Listesi.md (IC-xx, aşama)
  EW02  = enginewright/docs/plan_2026-11/02_Oyun_Ihtiyaclari.md (A-xx, G-xx, R-xx; 17.11)
  EKS   = enginewright/docs/EKSIKLER_VE_ONCELIK.md (A1.., B1.., E1..)
"""
import json, os, re, collections

HERE = os.path.dirname(os.path.abspath(__file__))
TOOLS = {t["name"]: t for t in json.load(open(os.path.join(HERE, "arac_test_kapsami.json"), encoding="utf-8"))}

KUMELER = {
    "VERİ": "Veri → DataTable/DataAsset + doğrulama",
    "TEST": "Otomatik test, bot, ölçüm okuma, YZ hata ayıklama",
    "AĞ": "Co-op / çok oyunculu test",
    "PERF": "Performans + paketleme",
    "LOC": "Lokalizasyon",
    "GÖRSEL": "Görsel QA (kare, ölçüm, kolaj)",
    "SANAT": "Sanat hattı (import, iskelet, malzeme, kaynak kaydı)",
    "YZ": "Oynanış yazarlığı (StateTree/BT, AnimBP, GAS, Niagara, fizik)",
    "SEVİYE": "Seviye / navmesh / yerleşim",
    "MEDYA": "Sinematik / çekim",
    "ALTYAPI": "Çalışma biçimi (headless, girdi denetimi)",
}

# id, küme, ihtiyaç, oyunlar, eski numaralar, oyundaki iş/belge, ihtiyaç anı, durum, araçlar, eksik kısım, EKSIKLER kodu
N = [
 # ---------------- VERİ ----------------
 ("EW-001","VERİ","DataTable satırlarını katı denetimle toplu yazma (bilinmeyen alan = hata, hepsi ya da hiç)","RC, ARPG, GP",
  "EW-R01, EW-R02 · R-01 · A-12 · K1","RC: IS-10 kart/tanrı/mod aktarımı (48 kart + 4 lanet + 21 sunak), IS-23 `DT_Unlocks` 58 satır (RC12 §11.1) · ARPG: IC-01 veri fabrikası (A09) · GP: `gp.py data` (GP08 §6)",
  "RC Demo D1 ilk kod işi (IS-10, bağımlılık yok) · ARPG Prototip/F1 IC-01 · GP içerik (GI-xx)","Var",
  ["datatable_set_rows","datatable_create","datatable_get","datatable_remove_rows","datatable_rename_row","type_info"],
  "Oyunda ilk deneme koşusu yapılmadı (RC12 §11.1 \"Var = davranış RagnaCook'ta denenmedi\")","A1 ✅"),
 ("EW-002","VERİ","CSV/JSON dosyasından şemaya göre upsert + fark raporu (eklenen/değişen/silinen)","RC, ARPG",
  "EW-R01 (eksik notu) · A-12 · E17","RC: IS-10 `aktar.py` (RC12 §11.1 \"CSV/JSON'dan toplu upsert + fark raporu yok\") · ARPG: `DT_Affix.csv` 357 satır, IC-01/IC-38/IC-53 (EW02 A-12)",
  "RC Demo D1 IS-10 (betikle geçilebilir) · ARPG Prototip/F1 IC-01","Kısmen",
  ["datatable_set_rows","batch"],
  "Dosyadan okuma ve fark raporu yok; ajan dosyayı JSON'a çevirip `datatable_set_rows` çağırıyor","E17"),
 ("EW-003","VERİ","DataAsset toplu oluşturma / okuma / doğrulama (100+ asset)","GP, ARPG",
  "EW-GP-4 (ESKİ, GP08) · G-05 · K1","GP: `gp.py data` SPECS → DataAsset, K-48 (GP08 §7) · ARPG: içe aktarma → DataTable + DataAsset (ARPG 08 §1)",
  "GP içerik GI-02…GI-27 (Demo) · ARPG F1","Kısmen",
  ["object_get","object_set","asset_duplicate","asset_find","batch"],
  "DataAsset oluşturma aracı yok (yalnız var olanı `object_set` ile yazma); toplu \"şablondan N asset\" yok",""),
 ("EW-004","VERİ","Sim ↔ oyun verisi köprüsü: oyundaki veriyi dışa aktar, sim çıktısıyla karşılaştır","GP, ARPG, RC",
  "EW-GP-5 (ESKİ, GP08) · G-06 · E18","GP: GI-29 denge turu \"sim'i gerçek veriye bağla\" (GP09) · ARPG: IC-12 ölçüm kaydı → CSV, IC-43 sim kalibrasyonu (A09) · RC: IS-32 denge turu (RC13E)",
  "GP Demo GI-29 · ARPG Prototip/F1 IC-12 · RC Demo IS-32","Kısmen",
  ["datatable_get","object_get"],
  "DataTable/asset → CSV/JSON dışa aktarma ve sim çıktısıyla fark raporu yok","E18"),
 ("EW-005","VERİ","ID bütünlüğü: tanımlı ↔ kullanılan ID'ler (DataTable, BP, data asset)","RC, ARPG",
  "EW-R03 · R-07 · A-13 · K4","RC: IS-10, IS-34 V1–V3 (RC12 §11.1) · ARPG: IC-68 doğrulayıcı (A09)",
  "RC Demo D1 IS-10 → IS-34 kapı · ARPG 1.0 IC-68","Var",
  ["id_audit","asset_search_text"],
  "BP içindeki sabit FName'lerin kapsandığı RC'de denenmedi (RC12 §11.1 EW-R03)","E6 ✅"),
 ("EW-006","VERİ","Tek çağrıda proje doğrulaması: bütün BP derle, Data Validation, kırık referans, map check","RC, ARPG",
  "EW-R20 · R-13 · A-13 · K4 · B11","RC: IS-34 D1 demo kapısı (RC12 §11.1) · ARPG: IC-68 doğrulayıcı + İçerik Testi haritası (A09)",
  "RC Demo D1 IS-34 (kapı) · ARPG 1.0 IC-68","Var",
  ["validate","blueprint_compile","asset_references","build_status"],
  "RC12 §11.1 EW-R20 'Kısmen / B11 yok' diyor; kodda `validate` (ImportValidationTools.cpp:235: blueprints, data, references, map) var → RC belgesi eski (oyun reposu salt okunur: SORULAR_2'de not)","B11 ✅"),
 ("EW-007","VERİ","İçerik duman testi: her veri satırını oyunda spawn edip hatasız mı bak (İçerik Testi haritası)","ARPG",
  "A-13","ARPG: IC-68 \"Doğrulayıcının tamamı + İçerik Testi haritası\" (A09)",
  "ARPG 1.0 IC-68","Yok",
  [],
  "Satır başına spawn + log iddiası döngüsü yok (bugün `actor_spawn` + `log_wait` ile ajan döngüsü)",""),
 # ---------------- TEST ----------------
 ("EW-010","TEST","Automation testlerini koşup sonucu ayrıştırma (geçti/kaldı/süre)","GP, RC",
  "EW-GP-01 (YENİ) · EW-R04","GP: YZ-0 test modülü, `AGPAITestScenario` (GP06 §2) · RC: EK-03 kural kancası öz-testi `RC_RunSelfTests` (RC12 §11.1)",
  "GP YZ-0 öncesi (GP16: \"dokuzu YZ-0'dan önce\") · RC Demo D1 EK-03","Var",
  ["automation_run","automation_list","console_command","log_wait"],
  "GP16 EW-GP-01'i \"eksik\" sayıyor; araç var (editör içinde). Başsız koşu ayrı satır: EW-011",""),
 ("EW-011","TEST","Başsız automation koşusu (`-game -nullrhi` / paket) + XML/json rapor ayrıştırma","GP, RC",
  "EW-GP-01 (YENİ, \"başsız\") · E18","GP: G0 \"3 senaryo hatasız biter\" (GP06 §2) · RC: 09 bot koşusu `-nullrhi -unattended -ExecCmds=\"RC.BotRun…\"` (RC 09_Dengeleme_ve_Test_Plani.md:87)",
  "GP YZ-0 öncesi · RC Demo IS-32","Kısmen",
  ["game_run","game_run_status"],
  "`game_run` `args`/`cvars`/`pass`/`fail` ile başsız süreç başlatır; automation rapor dosyasını ayrıştırma yok",""),
 ("EW-012","TEST","PIE senaryo oynatıcı: adım listesi (girdi → bekle → durum iddiası) tek çağrıda","RC, ARPG",
  "EW-R04 · EW-R05 · EW-R24 · E2 (parça)","RC: EK-03 kart başına senaryo \"kartı ver → olay tetikle → sonuç oku\", C49 Kazan Atışı (IS-62, T-06b) (RC12 §11.1) · ARPG: senaryo testleri (EW02 K3)",
  "RC Demo D1 IS-21 (kart kuralları) · RC 1.0 IS-62 · ARPG F1","Kısmen",
  ["pie_start","pie_input","pie_walk_to","pie_actors","pie_state","log_wait","console_command"],
  "Hazır kalıp yok; zamanlı girdi dizisi (yön + güç) yok; adımlar ajan döngüsüyle bağlanıyor","E2"),
 ("EW-013","TEST","Bot playtest + denge koşusu: oyunun kendi botunu başlat, metrikleri topla","RC, ARPG, GP",
  "EW-R17 · R-10 · EW-8 (ARPG) · A-07 · K3 · E2/E18","RC: IS-32 playtest, IS-52 bot gece koşusu (RC12 §11.1) · ARPG: IC-43/IC-51/IC-69 denge turları (A09) · GP: YZ bot profilleri (GP06 §2)",
  "RC Demo IS-32 (P2) · GP YZ-0 G0 · ARPG Demo IC-43","Kısmen",
  ["game_run","pie_input","pie_walk_to","console_command","log_wait"],
  "Bot oyunun içinde (C++). EW tarafı: başlat + bekle + metrik dosyasını oku. Okuma kısmı yok (EW-014)","E2, E18"),
 ("EW-014","TEST","Oyunun yazdığı ölçüm dosyasını (json/csv) okuyup kapıya göre özet","GP, ARPG, RC",
  "EW-GP-02 (YENİ)","GP: `Saved/AITest/<son>/metrics.json` + `ozet.md` (GP16) · ARPG: IC-12 TTK/ölüm/item CSV (A09) · RC: bot koşu çıktısı (RC 09)",
  "GP YZ-0 öncesi (G0) · ARPG Prototip/F1 IC-12","Yok",
  [],
  "Dosya okuma + eşik karşılaştırma aracı yok (ajan ham dosyayı bağlama okuyor)",""),
 ("EW-015","TEST","Visual Logger dökümü: aktör + zaman aralığı → metin/PNG","GP",
  "EW-GP-03 (YENİ)","GP: G1 kırmızı olayın nedeni (GP16)","GP YZ-0 öncesi (ilk kullanım G1)","Yok",[],"`.bvlog` okuma yok",""),
 ("EW-016","TEST","Gameplay Debugger anlık görüntü (seçili YZ, kategori, ekran + metin)","GP",
  "EW-GP-05 (YENİ)","GP: utility puanı / rol kanıtı (GP16)","GP YZ-0 öncesi (ilk kullanım G1)","Kısmen",
  ["console_command","screenshot"],
  "Kategori seçimi + metin dökümü yok; ekran görüntüsü alınabiliyor",""),
 ("EW-017","TEST","StateTree hata ayıklayıcı izini metne çevirme","GP, ARPG",
  "EW-GP-08 (YENİ)","GP: M12 titreme nedeni (GP16) · ARPG: 22 aile StateTree (AT EW-3, dolaylı)","GP YZ-0 öncesi (ilk kullanım G2)","Yok",[],"StateTree okuma/iz aracı yok",""),
 ("EW-018","TEST","Soak koşusu: paket `-game -nullrhi` uzun koşu, 60 sn'de bellek/ms/takılma satırı, çökme yakalama","GP, RC",
  "EW-GP-07 (YENİ)","GP: 30 dk soak, G3 (GP16) · RC: IS-31 paket + performans (RC13E)",
  "GP YZ-0 öncesi (ilk kullanım G3) · RC Demo IS-31","Kısmen",
  ["game_run","game_run_status"],
  "Uzun koşu + log iddiası var; periyodik bellek/ms örnekleme ve çökme özeti yok",""),
 ("EW-019","TEST","Senaryo haritasına YZ / bot / Smart Object'i veri listesinden yerleştirme","GP, ARPG",
  "EW-GP-09 (YENİ, \"isteğe bağlı\")","GP: S1 Koridor / S2 / S3 test haritaları (GP06 §2) · ARPG: İçerik Testi haritası IC-68",
  "GP YZ-0 ile (S11 açık) · ARPG 1.0","Kısmen",
  ["actor_spawn","scene_dress","batch","level_save"],
  "Veri listesinden toplu spawn + Smart Object ayarı yok (tek tek `actor_spawn`)",""),
 ("EW-020","TEST","Kayıt testi ve göçü: kayıt alanı yaz/oku, demo → EA kaydı","RC, GP",
  "EW-R14 · G-02 (parça)","RC: IS-54 T-15 `DemoImportState` KS-8 (RC12 §11.1) · GP: P2-11 kaydet → kapat → Devam (EW02 G-02)",
  "RC EA IS-54 · GP Demo","Var",
  ["savegame_get","savegame_set","game_run"],
  "İki ayrı paket (demo ve EA) arası akış elle (RC12 §11.1)",""),
 # ---------------- AĞ ----------------
 ("EW-030","AĞ","Tek makinede N istemcili co-op testi + kötü ağ emülasyonu","RC, GP",
  "EW-R16 · R-11 · EW-GP-1 (ESKİ, GP08) · G-01 · K7 · B2/E1","RC: IS-33 MP testleri T-09…T-11 (RC12 §11.1) · GP: hasar/ekonomi server'da K-11, GI-07 tezgâh ağ koşulu (GP08 §7, GP09)",
  "RC Demo IS-33 · GP P2 GI-07","Var",
  ["pie_start","pie_net_emulation","pie_state","pie_travel","pie_actors"],
  "Host kopması senaryosu elle (RC12 §11.1); ana menüden 2 oyuncu `TIMEOUT` (RELIABILITY_0.4.md:140 NOTE)","B2 ✅"),
 ("EW-031","AĞ","Replikasyon iddiası: server ↔ istemci değer karşılaştırması tek çağrıda","GP, RC",
  "EW-GP-1 (ESKİ) · K7","GP: tüm hasar/ekonomi server'da (GP08 §7) · RC: IS-33",
  "GP P2 GI-07 · RC Demo IS-33","Kısmen",
  ["pie_actors"],
  "`pie_actors` her dünyadaki değeri listeler; fark hesabı/iddia ajan tarafında",""),
 ("EW-032","AĞ","Paketli build ile 2 makine / Steam co-op testi","GP, RC",
  "G-02 · R-12 · K5","GP: P2-11 2 makine Steam testi (EW02 G-02) · RC: IS-33 (13_Ek satır 215, F0-11)",
  "GP Demo · RC Demo IS-33","Kısmen",
  ["game_run"],
  "Tek makinede paketli exe başlatılabiliyor; ikinci makine ve Steam oturumu kapsam dışı (TAHMİN: araçla çözülmez, elle)",""),
 # ---------------- PERF ----------------
 ("EW-040","PERF","Editör/PIE kare süresi, sahne maliyeti, GPU geçiş profili","GP, RC",
  "EW-GP-06 (taban) · EW-R15 (parça) · E19–E21","GP: M9 host maliyeti (GP16) · RC: IS-31 (RC12 §11.1)",
  "GP YZ-0 öncesi (G1) · RC Demo IS-31","Var",
  ["perf_capture","perf_scene","perf_gpu_profile"],
  "Üç araç da testsiz ve RELIABILITY tablosunda yok (arac_test_kapsami.json)","E19–E21 ✅"),
 ("EW-041","PERF","Stat grubu filtresi (ör. `GPAI_*` scope) ile ölçüm","GP",
  "EW-GP-06 (YENİ, \"scope\")","GP: M9 YZ host maliyeti (GP16)","GP YZ-0 öncesi (G1)","Yok",[],"`perf_capture`'da stat grubu seçimi yok",""),
 ("EW-042","PERF","Paketli / `-game` oyunda FPS örnekleme (Steam Deck 800p dahil)","RC, GP",
  "EW-R15 · EW-GP-06 (\"-game modu\") · E22/E3","RC: IS-31 paket + Deck, IS-52 (RC12 §11.1) · GP: M9 (GP16)",
  "RC Demo IS-31 · GP YZ-0 öncesi (G1)","Yok",
  [],
  "`game_run` oyunu açar ama kare süresi örneklemez (E22 açık); Deck cihazı ölçümü elle","E22"),
 ("EW-043","PERF","Paketleme / cook + durum takibi (demo build, dedicated server)","RC, GP, ARPG",
  "EW-R15 (B17) · R-14 · K5","RC: IS-31, IS-35 Next Fest build (RC13E) · GP: dedicated server + 2 makine (EW02 G-02) · ARPG: Demo (A09 G3)",
  "RC Demo IS-31 → IS-35 · GP Demo · ARPG Demo","Yok",
  [],
  "Elle `.bat` / RunUAT (EKS B17)","B17"),
 # ---------------- LOC ----------------
 ("EW-050","LOC","String Table toplu yazma (kaynak EN + TR)","RC","EW-R06","RC: LOC-1, LOC-2 (RC12 §11.1, RC20)","RC Demo LOC-1","Var",
  ["stringtable_set","stringtable_get","stringtable_list"],"CSV'den yüklenen tablo salt okunur → RC asset tablo kullanmalı (RC12 §11.1)",""),
 ("EW-051","LOC","Çeviri eksik / bozuk kontrolü (eksik anahtar, `{x}`, çevrilmemiş)","RC","EW-R07","RC: LOC-2, RC20 §5.4","RC Demo LOC-2","Var",
  ["stringtable_compare"],"—",""),
 ("EW-052","LOC","Lokalizasyon gather / export / import / compile (.po)","RC","EW-R08 · B16/E15","RC: LOC-2, LOC-5, LOC-8, LOC-9","RC Demo LOC-2 → 1.0 LOC-9","Var",
  ["loc_gather","loc_gather_status"],"—","B16 ✅"),
 ("EW-053","LOC","Glif testi (CJK, Tay, Arap fontlarında eksik karakter)","RC","EW-R09","RC: LOC-3 (Demo), LOC-6 (EA), LOC-6b (1.0)","RC Demo LOC-3","Var",
  ["font_coverage","game_run"],"Arapça bağlı harf şekli yalnız ekran görüntüsüyle (RC12 §11.1)",""),
 ("EW-054","LOC","Dil × ekran görüntüsü döngüsü (her dilde bütün UI ekranları, tek çağrı)","RC","EW-R10","RC: LOC-4, 09 T-16","RC Demo LOC-4 (LOC-5'ten önce)","Kısmen",
  ["loc_culture","widget_render","screenshot","ui_open","ui_state","game_run"],"\"Her dil × her ekran\" tek çağrı yok; ajan döngüsü",""),
 ("EW-055","LOC","Ekran taşma testi: kesilen / taşan metin sayısı (TextBlock istenen > ayrılan)","RC","EW-R11","RC: LOC-4 T-16 (RC12 §11.1: \"tek P0 eksik araç\")","RC Demo LOC-4 → LOC-5 → IS-32","Yok",
  [],"`text_overflow_audit` yok; olmazsa ekran görüntüsü + göz (RC12 §11.1)",""),
 # ---------------- GÖRSEL ----------------
 ("EW-060","GÖRSEL","Oyun kamerasından kare (altın kare, mağaza/trailer kareleri)","RC","EW-R12 · R-04 · K9 · B3","RC: IS-03, IS-32, IS-53 (RC12 §11.1)","RC Stil kilidi IS-03 (★ kritik yol)","Var",
  ["screenshot","screenshot_ab","image_compose","image_stats"],"—","B3 ✅"),
 ("EW-061","GÖRSEL","Okunabilirlik: nesne ekranda kaç px (yemek ≥ 28 px)","RC","EW-R13 · R-05 · K9 · E5","RC: IS-03, IS-13 (RC12 §11.1)","RC Stil kilidi → Demo IS-13","Var",
  ["screen_coverage","class_measure","actor_bounds"],"—","E5 ✅"),
 ("EW-062","GÖRSEL","Önce/sonra kolaj + sayısal görsel fark (SSIM)","RC, ARPG","R-08 · K9 · E8","RC: Q2 her 10 asset onayı (RC12 §10) · ARPG: toplu panolar (AT §12.5)","RC Demo IS-13","Var",
  ["screenshot_ab","image_compose","image_stats"],"—","E8 ✅"),
 ("EW-063","GÖRSEL","Toplu stil ölçümü: bölge maskesiyle palet / L* / gürültü (G1–G6)","RC","R-16 · K9","RC: IS-03 `stil_olcu.py` (RC13E)","RC Stil kilidi IS-03","Kısmen",
  ["image_stats"],"Bölge maskesi ve palet ölçümü yok (RC betiği yapıyor)",""),
 ("EW-064","GÖRSEL","Kamera turu: N konumdan sırayla kare + manifest","RC","R-15 · K9","RC: 8 mağaza görüntüsü (RC 14 §5), IS-53","RC Demo IS-35 · EA IS-53","Kısmen",
  ["screenshot","game_run"],"Waypoint listesinden tur yok (`game_run` `shotNames` oyun kendi çekerse)",""),
 ("EW-065","GÖRSEL","Varyant önizleme panosu: malzeme override + animasyon karesi","ARPG, RC","EW-2 (parça) · E25/E34","ARPG: 66 düşman varyantı (AT EW-2) · RC: tipleme önizleme (EKS E34)","ARPG F0","Kısmen",
  ["actor_preview","image_compose"],"`actor_preview`'da `material_overrides` ve `anim/time` yok; Character çerçevesi gevşek (RELIABILITY_0.4.md:61 NOTE, 0.4.1'e)","E25, E34"),
 # ---------------- SANAT ----------------
 ("EW-070","SANAT","Editör açıkken toplu içe aktarma (mesh, doku, ses, font)","RC, ARPG, GP","EW-R18 · R-02 · K2 · B9","RC: 155 asset + 186 ses, IS-13/25/41/51/63 (RC12 §11.1) · ARPG: kit/oda/aile assetleri · GP: kendi asset geçişi",
  "RC Demo IS-13 (★) · ARPG F0 · GP Demo","Var",
  ["asset_import","batch","mesh_info","texture_info"],"Klasör tarama + ad kuralı yok (dosya listesi verilir); Türkçe dosya adı uyarısız (RELIABILITY_0.4.md:68 NOTE, 0.4.1'e)","B9 ✅"),
 ("EW-071","SANAT","İskeletli mesh + animasyon import, IK retarget, soket, Physics Asset, montage/blendspace/notify","ARPG, RC, GP","EW-1 (ARPG) · A-01 · EW-R18 (Garmr) · E23","ARPG: 22 aile + 8 boss + oyuncu (AT EW-1, 6–8 kart-gün) · RC: yalnız Garmr (RC12 §11) · GP: YZ-2 retarget + Physical Animation (GP06:22)",
  "ARPG F0 (AT §12.3: \"ilk dördü prototipten önce\") · GP YZ-2 · RC EA","Kısmen",
  ["asset_import","mesh_info"],"`asset_import` `meshType: skeletal` var; animasyon, retarget, soket, Physics Asset, montage yok (E23)","E23"),
 ("EW-072","SANAT","Malzeme varyantı: master → MI, FBX slotu başına MI + doku bağlama","ARPG, RC","EW-2 · A-02 · R-03 · E24","ARPG: 66 düşman varyantı (AT EW-2, 2 kart-gün) · RC: 155 asset `M_RC_Boya` MI (EW02 R-03)",
  "RC Demo IS-13 (★) · ARPG F0","Kısmen",
  ["material_instance_create","material_instance_set","mesh_set_material"],"Slot başına toplu MI (`material_instances_for_mesh`) yok (E24)","E24"),
 ("EW-073","SANAT","Sanat bütçesi kontrolü (üçgen, doku boyutu) ","ARPG, RC","EW-1 (\"bütçe\") · ROADMAP 0.4.1","ARPG: AT EW-1 · RC: 155 asset (RC12 §7)","ARPG F0 · RC Demo IS-13","Kısmen",
  ["mesh_info","texture_info"],"Tek asset okunuyor; klasör + eşik raporu yok",""),
 ("EW-074","SANAT","Güvenli taşıma + kalan yer tutucu taraması (oyuncu yolunda referans = 0)","RC, GP","R-06 · EW-R18 · EW-GP-3 (ESKİ, GP08) · G-04 · K8 · B5","RC: IS-30 ★ `_Kullanilmayan` taşıma (RC13E) · GP: K-70 Synty yer tutucu (GP08 §7)",
  "RC Demo IS-30 (★) · GP Demo","Var",
  ["asset_rename","asset_references","asset_delete","asset_search_text"],"\"Kalan yer tutucu\" toplu raporu yok (klasör → referans sayısı)","B5 ✅"),
 ("EW-075","SANAT","Asset kaynağı / lisans / AI beyanı metadata + beyan raporu","RC, ARPG","EW-R19 · R-09 · EW-10 (ARPG) · A-08 · K6 · E14","RC: `Asset_Kaydi.csv`, IS-35 demo sayfası, IS-53 (RC12 §11.1) · ARPG: Steam AI beyanı (AT EW-10)",
  "RC Demo IS-35 · ARPG 1.0","Yok",[],"Metadata yazma/okuma ve rapor yok","E14"),
 ("EW-076","SANAT","Harita denetimi: gizli aktör, görünür ama işlevsiz prop, yer tutucu / kit mesh sayısı (harita başına)","RC","EW-R23","RC: Temel Hat TH-1/TH-4 \"gizli 0, işlevsiz 0\" (RC12 §11.1 EW-R23, Talha 18.11)",
  "RC Demo EK-10","Yok",[],"actor_find yalnız arama; denetim raporu yok","—"),
 # ---------------- YZ ----------------
 ("EW-080","YZ","StateTree / Behavior Tree yazma","ARPG, GP","EW-3 (ARPG) · A-04 · B14","ARPG: 22 aile + boss fazları (AT EW-3, 4–5 kart-gün) · GP: YZ-0 StateTree geçişi (GP06:12, :20); GP16 StateTree yazma aracı istemiyor, yalnız iz (EW-GP-08)",
  "ARPG F0 (prototipten önce) · GP YZ-0 (belgelenmemiş ihtiyaç, TAHMİN)","Yok",[],"Okuma da yazma da yok","B14"),
 ("EW-081","YZ","AnimBP state machine şablonu (iskelet ailesi başına)","ARPG","EW-4 · A-05 · B13","ARPG: 5 iskelet ailesi (AT EW-4, 3–4 kart-gün)","ARPG F0 (AT §12.3 sıra önerisinde 5., \"ilk dört\"ten sonra)","Yok",
  ["blueprint_graph_read"],"AnimGraph yazma yok","B13"),
 ("EW-082","YZ","GAS: Ability / Effect / Attribute / Cue oluştur, oku, test","ARPG","EW-5 · A-03","ARPG: 36 skill, 108 mutasyon, affix = GameplayEffect (AT EW-5, 5–6 kart-gün); IC-02 attribute set (A09)","ARPG F0 (prototipten önce) → Prototip/F1 IC-02","Yok",[],"Hiç yok",""),
 ("EW-083","YZ","Niagara parametre / renk-element varyantı / önizleme","ARPG","EW-6 · A-09","ARPG: skill + element + aura VFX (AT EW-6, 4 kart-gün); IC-16 gerçek VFX","ARPG Dikey dilim/F2 IC-16","Yok",[],"Hiç yok",""),
 ("EW-084","YZ","Fizik / ragdoll doğrulama (ganimet taşıma, sekme determinizmi)","GP","EW-GP-2 (ESKİ, GP08) · G-03","GP: K-106 sekme, elit ragdoll eşikleri (GP08 §7); YZ-1 ragdoll ölüm (GP06:21); \"ragdoll belirlenimci değil\" riski (GP06:50)","GP YZ-1 · Demo","Kısmen",
  ["pie_actors","world_trace","console_command"],"Fizik senaryosu + sonuç ölçümü yok",""),
 # ---------------- SEVİYE ----------------
 ("EW-090","SEVİYE","NavMesh build + kopuk ada / ulaşılamaz nokta raporu","GP, ARPG","EW-GP-04 (YENİ) · EW-7 (parça) · A-06 · B15","GP: M1 takılma (GP16) · ARPG: kat erişilebilirliği (AT EW-7)","GP YZ-0 öncesi (G1) · ARPG Prototip/F1 IC-10","Yok",
  ["pie_walk_to","world_trace"],"Build ve erişilebilirlik raporu yok (`pie_walk_to` STUCK ile dolaylı)","B15"),
 ("EW-091","SEVİYE","Oda parçası + kat doğrulayıcı (bağlantı noktası, ızgara)","ARPG","EW-7 · A-06 · K10","ARPG: 7 kit × ~30 oda, prosedürel kat (AT EW-7, 4–5 kart-gün)","ARPG Prototip/F1 IC-10 (tek kit gri kutu)","Yok",
  ["actor_bounds","world_trace","mesh_info"],"Port/ızgara kuralı yok",""),
 ("EW-092","SEVİYE","Yerleşim seti: iş noktası grupları + tohum (veri → seviye)","GP","G-07 · K10","GP: GI-03 yerleşim seti, GI-26/27 B1 mekânları (GP09)","GP P2 GI-03 → Demo GI-26","Kısmen",
  ["scene_dress","actor_spawn","batch"],"Veri listesinden grup yerleştirme yok",""),
 # ---------------- MEDYA ----------------
 ("EW-100","MEDYA","Sequencer kurma (Level Sequence, kamera, kesme, anahtar)","ARPG, RC","EW-11 · A-11 · EW-R22 · E29","ARPG: ~8 akt ara sahnesi (AT EW-11, P2) · RC: IS-53 trailer (RC12 §11.1)","RC EA IS-53 · ARPG 1.0","Yok",[],"Hiç yok (EKS E29: bugün Python ile)","E29"),
 ("EW-101","MEDYA","Movie Render Queue çalıştırma","RC","EW-R22","RC: IS-53 trailer / Reel","RC EA IS-53","Var",["mrq_render","mrq_status"],"İki araç da testsiz (arac_test_kapsami.json)","E30 ✅"),
 ("EW-102","MEDYA","Oynanış kaydı (Take Recorder → sekans → MRQ)","RC","EW-R22 · E32","RC: IS-53 (RC12 §11.1)","RC EA IS-53","Yok",[],"Hiç yok","E32"),
 # ---------------- ALTYAPI ----------------
 ("EW-110","ALTYAPI","Headless mod: aynı araçlar editör kapalıyken (import, veri)","ARPG","EW-9 · A-10 · B18","ARPG: tek editör kuyruğunu açar (AT EW-9, 6–8 kart-gün)","ARPG F3 prodüksiyon (AT §12.3 sıra önerisinde 10.)","Yok",
  ["loc_gather"],"Yalnız `loc_gather` commandlet ile çalışıyor","B18"),
 ("EW-111","ALTYAPI","Sabit tuş denetimi + girdi istemleri","RC","EW-R21 · E7","RC: IS-34 ekip tuş kuralı","RC Demo IS-34","Var",["key_audit","ui_input_audit","ui_prompts"],"—","E7 ✅"),
]

KOD_RE = re.compile(r"EW-R\d+|EW-GP-\d+|EW-\d+|[ARG]-\d+|K\d+|[ABE]\d+")

def kod_kaynak(k):
    if k.startswith("EW-R"): return "RagnaCook RC12 §11.1"
    if re.fullmatch(r"EW-GP-0\d", k): return "GP YENİ liste GP16 (+ GP05 §8)"
    if re.fullmatch(r"EW-GP-\d", k): return "GP ESKİ liste GP08 §7"
    if k.startswith("EW-"): return "ARPG AT §12.3"
    if k[0] in "ARG" and "-" in k: return {"A":"EW02 §ARPG","G":"EW02 §GP","R":"EW02 §RC"}[k[0]]
    if k.startswith("K"): return "EW02 ortak küme"
    return "EKSIKLER_VE_ONCELIK"

def eski_esleme():
    rows = collections.defaultdict(list)
    for n in N:
        for k in KOD_RE.findall(n[4]):
            if n[0] not in rows[k]:
                rows[k].append(n[0])
    def anahtar(k):
        m = re.match(r"([A-Z-]+?)-?(\d+)$", k)
        return (kod_kaynak(k), m.group(1) if m else k, int(m.group(2)) if m else 0)
    return {k: rows[k] for k in sorted(rows, key=anahtar)}

def eski_tablo():
    o = ["| Eski numara | Kaynak liste | Yeni ID(ler) |", "|---|---|---|"]
    for k, v in eski_esleme().items():
        o.append(f"| {k} | {kod_kaynak(k)} | {', '.join(v)} |")
    return "\n".join(o) + "\n"

def arac_ref(name):
    t = TOOLS.get(name)
    if not t:
        return f"`{name}` (KAYITTA YOK)"
    return f"`{name}` ({os.path.basename(t['file'])}:{t['line']})"

def guv(name):
    t = TOOLS[name]
    r = t.get("reliability") or "-"
    r = "tabloda yok" if r == "-" else r
    nt = len(t["tests"])
    return f"{name}: {r}, test {nt}" if nt else f"{name}: {r}, **testsiz**"

def main():
    # doğrulama: her araç kayıtta olmalı
    eksik = sorted({a for n in N for a in n[8] if a not in TOOLS})
    assert not eksik, eksik
    ids = [n[0] for n in N]
    assert len(ids) == len(set(ids))
    json.dump([dict(zip(["id","kume","ihtiyac","oyunlar","eski","oyun_is","an","durum","araclar","eksik","eks_kod"], n)) for n in N],
              open(os.path.join(HERE, "ihtiyac_esleme.json"), "w", encoding="utf-8"), ensure_ascii=False, indent=1)
    # 06 tablo
    o = ["| ID | İhtiyaç | Oyun(lar) | Oyundaki iş / belge | Oyunun ihtiyaç anı (sıra/bağımlılık) | Ortak küme | Eski numaralar |", "|---|---|---|---|---|---|---|"]
    for n in N:
        o.append(f"| {n[0]} | {n[2]} | {n[3]} | {n[5]} | {n[6]} | {n[1]} ({len(n[3].split(','))} oyun) | {n[4]} |")
    open(os.path.join(HERE, "gen_06_tablo.md"), "w", encoding="utf-8").write("\n".join(o) + "\n")
    # 07 tablo
    o = ["| ID | İhtiyaç (kısa) | Durum | Araç (kaynak dosya:satır) | Eksik kısım | Güvenilirlik (RELIABILITY_0.4 + test dosyası sayısı) |", "|---|---|---|---|---|---|"]
    for n in N:
        ar = ", ".join(arac_ref(a) for a in n[8]) or "—"
        gv = "; ".join(guv(a) for a in n[8]) or "—"
        o.append(f"| {n[0]} | {n[2][:70]}{'…' if len(n[2])>70 else ''} | **{n[7]}** | {ar} | {n[9]} | {gv} |")
    open(os.path.join(HERE, "gen_07_tablo.md"), "w", encoding="utf-8").write("\n".join(o) + "\n")
    # ters tablo
    rev = collections.defaultdict(list)
    for n in N:
        for a in n[8]:
            rev[a].append(n[0])
    o = ["| Araç | Kategori | Kaynak | Hizmet ettiği ihtiyaç sayısı | İhtiyaçlar | RELIABILITY | Test dosyası |", "|---|---|---|---|---|---|---|"]
    for a, l in sorted(rev.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        t = TOOLS[a]
        o.append(f"| `{a}` | {t['category']} | {os.path.basename(t['file'])}:{t['line']} | {len(l)} | {', '.join(l)} | {t.get('reliability') or '-'} | {len(t['tests'])} |")
    unused = sorted(set(TOOLS) - set(rev))
    open(os.path.join(HERE, "gen_07_ters.md"), "w", encoding="utf-8").write("\n".join(o) + "\n")
    open(os.path.join(HERE, "gen_06_eski.md"), "w", encoding="utf-8").write(eski_tablo())
    c = collections.Counter(n[7] for n in N)
    k = collections.Counter(n[1] for n in N)
    games = collections.Counter(len(n[3].split(",")) for n in N)
    stats = {"ihtiyac": len(N), "durum": dict(c), "kume": dict(k), "oyun_sayisi": dict(games),
             "kullanilan_arac": len(rev), "toplam_arac": len(TOOLS), "kullanilmayan": unused,
             "eski_esleme": eski_esleme()}
    json.dump(stats, open(os.path.join(HERE, "ihtiyac_esleme_ozet.json"), "w", encoding="utf-8"), ensure_ascii=False, indent=1)
    print(json.dumps({k2: v for k2, v in stats.items() if k2 not in ("eski_esleme", "kullanilmayan")}, ensure_ascii=False))
    print("kullanılmayan araç:", len(unused))

if __name__ == "__main__":
    main()
