# 02 — Oyun İhtiyaçları (ARPG + Gunpoint Parole + RagnaCook)

> **18.11.2026 — Yerini aldı:** ihtiyaç listesi → `06_Ihtiyac_Birlesik.md` (tek numara EW-xxx; bu belgedeki A-/G-/R-/K ID'leri 06 §6'da eşlendi); "mevcut araç" sütunu → `07_Arac_Esleme.md`. Bu belge silinmedi, tarihçe için duruyor.

Tarih: 17.11.2026 (v2). Kart: `t_3d650d3a`. Yalnız belge.
v1'den fark: GP ve RagnaCook İçerik Sistemi belgeleri artık var; ihtiyaçlar oradan alındı. Kaynakta dayanağı olmayan v1 satırları (dövme, koleksiyon defteri, hile tespiti, hayvan animasyonu, Steam lobby) çıkarıldı.

## 0. Kaynaklar (hangi ihtiyaç hangi dosyadan)

| Oyun | Kart durumu | Okunan dosyalar | Satır kodu |
|---|---|---|---|
| ARPG | BİTTİ | `Game/ARPG/Docs/ARPG_Tasarim_Kararlari.md` §12.1–12.5 (EW-1..EW-11) · `Docs/Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` §1 (boru hattı) · `Docs/Icerik_Sistemi/09_Is_Listesi.md` (IC-xx saatleri) | `A-xx` |
| Gunpoint Parole | ÇALIŞIYOR (t_e6c46ed7) | `Game/GunpointParole/docs/Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` §1, §3, §5, §7 (EW-GP-1..5) · `docs/Icerik_Sistemi/09_Is_Listesi.md` (GI-07) · `docs/ParoleLoop_TaskList.md` (P2-11, T-03b satır 150, P1-20) | `G-xx` |
| RagnaCook | ÇALIŞIYOR (t_d9c54eb5) | `UE/RagnaCook/Docs/Icerik_Sistemi/12_Icerik_Uretim_Sistemi.md` §2, §5, §6, §7, §9, §10, §11 · `13_Is_Listesi.md` · `13_Ek_Is_ve_Esleme.md` satır 210, 215 (F0-11) · `09_Dengeleme_ve_Test_Plani.md` satır 63 · `14_Magaza_Trailer_ve_Yol_Haritasi.md` §5 | `R-xx` |
| Enginewright (mevcut araç) | — | `Plugins/**/Tools/*.cpp` + `EWRegistry.cpp` `Register`/`RegisterAsync` kayıtları (129 ad, 17.11 sayımı) · `docs/EKSIKLER_VE_ONCELIK.md` (A1, B2…E34 kodları) · `CHANGELOG.md` [Unreleased] | — |

Not: GP ve RagnaCook kartları sürüyor. Kartlar bitince bu tablo yeniden gözden geçirilmeli.

**İşaretler** (Enginewright sütununda):
- ✅ **VAR** — araç bugün kodda var; ihtiyacı karşılıyor.
- 🟡 **KISMİ** — bir kısmı var; eksik kısım yazılı.
- ❌ **YOK** — kayıtlı araç yok.

**Öncelik:** P0 = prototip/demo kapısından önce şart · P1 = demo içinde · P2 = EA/1.0 · P3 = sonra · "kullan" = araç var, yalnız oyunun hattına bağlanacak.

**Saat kuralı:** "kaynak" = sayı belgeden alındı. "tahmin" = benim hesabım; gerekçesi parantezde. Kazanılan saat = o işin elle yapılmasına göre, oyunun 1.0'ına kadar toplam.

---

## 1. Ana tablo

### 1.1 ARPG

| No | Oyun | İş | Bugün nasıl yapılıyor | Enginewright'ta ne lazım (araç/özellik) | Kazanılan saat (kaynak/tahmin) | Öncelik |
|---|---|---|---|---|---|---|
| A-01 | ARPG | **EW-1 Sanat hattı:** iskeletli import + anim, IK retarget, soket, Physics Asset, montage/blendspace/notify, bütçe (22 aile + 8 boss + oyuncu) — kaynak: T §12.3 | Betik + elle (Tripo hattı; rig yok, her asset 4 kapı — T §12.1) | 🟡 KISMİ: `asset_import` (`meshType: skeletal`), `mesh_info`, `perf_scene` var. YOK: retarget, soket, Physics Asset, montage, blendspace, notify, bütçe kontrolü (CHANGELOG [Unreleased] 0.4.1 = plan) | ≈ 60 sa (tahmin: 31 rigli asset × ~2 sa; animasyon hızlanması 1,5–2× — T §12.2) | P0 (T §12.3 sıra 1) |
| A-02 | ARPG | **EW-2 Malzeme varyantı:** master → MI, slot eşleme, çoklu önizleme panosu (66 düşman varyantı) — kaynak: T §12.3 | Elle Python betikleri (EKSIKLER E24, E34) | 🟡 KISMİ: `material_instance_create`, `material_instance_set`, `mesh_set_material`, `actor_preview`, `image_compose`. YOK: slot adı → MI toplu eşleme (E24), `actor_preview` için `material_overrides` + varyant panosu (E34) | ≈ 25 sa (tahmin: 66 varyant × ~0,4 sa elle iş) | P0 (sıra 2) |
| A-03 | ARPG | **EW-5 GAS araçları:** Ability/Effect/Attribute/Cue oluştur, oku, test (36 skill, 108 mutasyon, affix = GameplayEffect) — kaynak: T §12.3 | Elle, Blueprint (T §12.3 "Yok") | 🟡 KISMİ: yalnız `gameplay_tag_add/remove`, `gameplay_tags_list`. YOK: GAS asset oluşturma/okuma/test | ≈ 45 sa (tahmin: 144 GAS asset × ~0,5 sa; araçla ~3× hızlı) | P0 (sıra 3) |
| A-04 | ARPG | **EW-3 StateTree / BT yazma:** 22 aile davranışı + boss fazları — kaynak: T §12.3 | Elle, editörde | ❌ YOK: StateTree/BT/Blackboard oluşturma ve düğüm ekleme | ≈ 45 sa (tahmin: 30 ağaç × ~3 sa; araçla ~2×) | P0 (sıra 4) |
| A-05 | ARPG | **EW-4 AnimBP state machine:** iskelet ailesi başına 1 şablon (5 aile) — kaynak: T §12.3 | Elle, kopyala-özelleştir | ❌ YOK (`blueprint_*` AnimBP state machine yazmıyor) | ≈ 20 sa (tahmin: 5 şablon × ~8 sa; araçla ~2×) | P1 |
| A-06 | ARPG | **EW-7 Oda parçası + kat doğrulayıcı:** port, ızgara, navmesh build, erişilebilirlik (7 kit × ~30 oda) — kaynak: T §12.3 | Elle, PIE'de yürüyerek | 🟡 KISMİ: `scene_dress`, `world_trace`, `pie_walk_to` (navmesh varsa yol izler). YOK: navmesh/ışık build (EKSIKLER B15), port/ızgara kontrolü, A→B erişilebilirlik testi | ≈ 50 sa (tahmin: ~210 oda × ~0,25 sa elle kontrol) | P1 |
| A-07 | ARPG | **EW-8 Bot playtest + denge koşusu:** TTK, ölüm, drop, "ilk unique'e süre" — kaynak: T §12.3; denge turları IC-43 24 sa, IC-51 30 sa, IC-69 36 sa (kaynak: 09) | Elle oynama + Python sim (`denge_sim.py`, 08 §1) + ölçüm CSV (IC-12) | 🟡 KISMİ: `pie_input`, `pie_walk_to`, `pie_state`, `log_wait`, `automation_run`. YOK: girdi senaryosu + iddia oynatıcı, metrik çıkışı (EKSIKLER E2, E18) | ≈ 10–20 sa (tahmin: 90 sa denge işinin sayım kısmı; bot yalnız sayıyı hızlandırır 1–1,3× — T §12.2) | P1 |
| A-08 | ARPG | **EW-10 Asset metadata / AI beyanı** (Steam AI içerik beyanı) — kaynak: T §12.3, §12.5 | Elle tablo | ❌ YOK (EKSIKLER E14 açık) | ≈ 10 sa (tahmin: ~240 ikon + modeller için elle satır yazma) | P1 |
| A-09 | ARPG | **EW-6 Niagara parametre/varyant** (36 skill × 5 element + elit auraları) — kaynak: T §12.3 | Elle, Niagara editörü | ❌ YOK (Niagara yalnız `perf_scene` sayımında geçiyor) | ≈ 30–60 sa (tahmin: ~180 varyant; VFX hızlanması 1,5–2× — T §12.2) | P2 |
| A-10 | ARPG | **EW-9 Headless mod** (editör kapalı import/veri işi) — kaynak: T §12.3 | Elle commandlet | ❌ YOK (`game_run` oyunu `-game` ile açar; commandlet MCP sunucusu yok) | Saat değil takvim kazancı: tek editör kuyruğu açılır (kaynak: T §12.1, §12.4 "darboğaz editör sırası") | P2 |
| A-11 | ARPG | **EW-11 Sequencer temel** (~8 akt ara sahnesi) — kaynak: T §12.3 | Elle, Python | 🟡 KISMİ: `mrq_render` mevcut sequence'ı render eder. YOK: sequence oluşturma, aktör bağlama, kamera kesmesi, key | ≈ 10 sa (tahmin: 8 sahne × ~1,5 sa) | P3 |
| A-12 | ARPG | **Veri içe aktarma:** `Data/src/*.csv` → DataTable/DataAsset (ör. `DT_Affix.csv` 357 satır) — kaynak: 08 §1 | Betik (`affix_tablosu.py`, `dogrula.py`); içe aktarma "Enginewright batch" olarak planlı (08 §1) | ✅ VAR: `datatable_create`, `datatable_set_rows` (bilinmeyen alan = hata, hepsi ya da hiçbiri), `batch` | Veri işi ~10× (kaynak: T §12.2); saat: ≥ 50 sa (tahmin: 09'daki veri işleri yüzlerce saat) | kullan (IC-01) |
| A-13 | ARPG | **Doğrulayıcı + İçerik Testi haritası** (IC-68, 38 sa — kaynak: 09) | Betik (`dogrula.py`, 0 hata — 08 §1) | 🟡 KISMİ: `validate` (BP derleme, data validation, kırık referans, map check), `id_audit`. YOK: satır başına otomatik duman testi | ≈ 10 sa (tahmin: IC-68'in UE tarafı) | P2 |

### 1.2 Gunpoint Parole

| No | Oyun | İş | Bugün nasıl yapılıyor | Enginewright'ta ne lazım (araç/özellik) | Kazanılan saat (kaynak/tahmin) | Öncelik |
|---|---|---|---|---|---|---|
| G-01 | GP | **EW-GP-1 Ağ / çok oyunculu test** (listen-server, RPC; hasar/ekonomi server'da, K-11) — kaynak: GP 08 §7; GI-07 tezgâh ağ koşulu 4 sa (kaynak: 09) | Elle: PIE 2 oyuncu (TaskList P1 kabulleri) | 🟡 KISMİ: `pie_start` (1–4 oyuncu, listen/client), `pie_net_emulation`, `pie_input`, `pie_actors` VAR. YOK: replikasyon iddiası (server ↔ client değer karşılaştırma) | ≈ 15 sa (tahmin: P2'de ~20 ağ testi turu × ~45 dk) | P0 |
| G-02 | GP | **2 makine Steam testi:** kaydet → kapat → "Devam", cep/parçalar SteamID ile döner — kaynak: TaskList P2-11 | Yok (bekliyor; 2 makine + arkadaş gerekir) | 🟡 KISMİ: `game_run` paketli exe'yi açar. YOK: paketleme (bkz. R-14), aynı makinede 2 süreç senkron başlatma + log birleştirme | ≈ 3 sa/test (tahmin: kurulum + 2 kişi koordinasyonu) | P1 |
| G-03 | GP | **EW-GP-2 Fizik/ragdoll doğrulama** (ganimet taşıma, sekme deterministik K-106, elit ragdoll eşikleri) — kaynak: GP 08 §7; TaskList P1-20 kabulü "vurulur, ragdoll olur" | Elle PIE | 🟡 KISMİ: `pie_actors`, `object_get`, `world_trace`, `pie_input`. YOK: senaryo (itme/atış) + sonuç kaydı + tekrar karşılaştırma | ≈ 8 sa (tahmin: ~16 fizik kabulü × ~30 dk) | P1 |
| G-04 | GP | **EW-GP-3 Synty → kendi asset geçişi** (K-70 yer tutucu; production'da kalmasın) — kaynak: GP 08 §7; TaskList satır 150 | Betik: `synty_import.py` + `synty_relocate.py` (5 paket, ≥ 1.867 asset — kaynak: TaskList satır 150) | 🟡 KISMİ: `asset_import`, `asset_rename` (referans + redirector + metin izi), `asset_references`, `asset_find`. YOK: "A'yı kullanan her yerde B'ye geç" toplu değiştirme + kalan Synty referansı raporu | ≈ 25 sa (tahmin: ~300 kullanılan asset × ~5 dk elle değişim) | P1 |
| G-05 | GP | **EW-GP-4 DataAsset toplu yazım okuma/doğrulama** (`gp.py data`, K-48; 100+ asset) — kaynak: GP 08 §1, §7 | Betik: `parole_data.py` SPECS → `gp.py data` → `make_parole_data.py` | 🟡 KISMİ: `object_get`/`object_set`, `validate`, `id_audit`. YOK: DataAsset oluşturma (sınıf + yol), SPECS'e karşı toplu okuma/diff | ≈ 10 sa (tahmin: 100+ asset elle kontrol yerine tek rapor) | P1 |
| G-06 | GP | **EW-GP-5 Sim ↔ gerçek veri köprüsü** (`ekonomi_sim` ↔ `parole_data`, GI-29) — kaynak: GP 08 §7 | Elle port: `veri.py` → `parole_data.py` ~1 gün (kaynak: 08 §1) | 🟡 KISMİ: `object_get` asset değerini okur. YOK: JSON ↔ DataAsset diff aracı | ≈ 8 sa ilk port (kaynak: 08 §1 "~1 gün") + her denge turunda ~1 sa (tahmin) | P2 |
| G-07 | GP | **Yerleşim seti** (9/18/27 set; yeni iş = tür saati + 12 sa veri+set) — kaynak: GP 08 §3, §5 | Betik + elle (`patch_maps.py`, TaskList) | 🟡 KISMİ: `scene_dress`, `actor_spawn`, `actor_find`, `level_save`. YOK: set şablonu (nokta listesi → seviyeye yerleştir) | ≈ 27 sa (tahmin: 27 set × ~1 sa) | P2 |

### 1.3 RagnaCook

| No | Oyun | İş | Bugün nasıl yapılıyor | Enginewright'ta ne lazım (araç/özellik) | Kazanılan saat (kaynak/tahmin) | Öncelik |
|---|---|---|---|---|---|---|
| R-01 | RC | **Veri aktarımı (A1 katı `set_rows`):** 28 yemek, 48 kart, 58 kilit — kaynak: RC 12 §2, §8, §11 sıra 1 | Betik (YENİ `sim/aktar.py` → JSON) | ✅ VAR: `datatable_set_rows` (bilinmeyen alan/tip/enum = hata; bir satır patlarsa hiçbiri yazılmaz) | Veri ~10× (kaynak: RC 12 §9.2); saat: ≈ 10 sa (tahmin: 134 satır elle giriş) | kullan |
| R-02 | RC | **B9 Editör açıkken import** (155 asset + 186 ses) — kaynak: RC 12 §7, §11 sıra 2 | Commandlet döngüsü (kaynak: 12 §11 "olmazsa") | ✅ VAR: `asset_import` (doku, ses, font, mesh; diyalogsuz) | ≈ 15 sa (tahmin: ~340 dosya × ~3 dk commandlet döngüsü) | kullan |
| R-03 | RC | **Sanat hattı + MI ataması:** Tripo → Blender → UE import → `M_RC_Boya` MI (155 asset, 219,8 sa AI'lı — kaynak: RC 12 §7) — kaynak: RC 12 §6, §9 | Betik (Tripo + Blender + `tools/art/on_kontrol.py`) + elle | 🟡 KISMİ: `asset_import`, `material_instance_create/set`, `mesh_set_material`, `mesh_info`. YOK: tek çağrıda import + MI + kayıt (asset başına hat) | ≈ 15 sa (tahmin: 155 asset × ~6 dk editör adımı) | P0 |
| R-04 | RC | **B3 Oyun kamerası karesi** (G2, G3, G6; altın kare) — kaynak: RC 12 §5.2, §11 sıra 3 | Elle | ✅ VAR: `screenshot` (source `game`, UI dahil, parlaklık uyarısı) | ≈ 5 sa (tahmin) | kullan |
| R-05 | RC | **E5 Okunabilirlik:** yemek ekranda ≥ 28 px — kaynak: RC 12 §5.2 G3, §11 sıra 4 | Geçici spawn betiği | ✅ VAR: `screen_coverage` (px kutusu, ekran payı) | ≈ 5 sa (tahmin: 28 yemek × ~10 dk) | kullan |
| R-06 | RC | **B5 Güvenli taşıma:** yer tutucuları `_Kullanilmayan/`'a taşı (Q4: oyuncu yolunda referans = 0) — kaynak: RC 12 §6, §10, §11 sıra 5 | Elle (kırık referans riski) | ✅ VAR: `asset_rename` (referans güncelleme, redirector temizliği, metin izi raporu, `dryRun`), `asset_references` | ≈ 6 sa (tahmin) | kullan |
| R-07 | RC | **E6 ID bütünlük** (V1–V3'ün UE tarafı) — kaynak: RC 12 §5.1, §11 sıra 6 | İkili dosyada grep | ✅ VAR: `id_audit` (tanımlı ama kullanılmayan / kullanılan ama tanımsız) | ≈ 4 sa (tahmin) | kullan |
| R-08 | RC | **E8 Önce/sonra kolaj** (Q2: her 10 asset, Telegram) — kaynak: RC 12 §10, §11 sıra 7 | Elle PIL | ✅ VAR: `screenshot_ab`, `image_compose` | ≈ 5 sa (tahmin: ~16 kolaj × ~20 dk) | kullan |
| R-09 | RC | **E14 Asset kaynağı / AI beyanı** (`Docs/Asset_Kaydi.csv`) — kaynak: RC 12 §2, §6, §11 sıra 8 | Elle CSV | ❌ YOK | ≈ 8 sa (tahmin: 155 asset + 186 ses satırı) | P1 |
| R-10 | RC | **E2 Bot playtest** (sipariş akışı; `RC.BotRun` Enginewright ile başlatılır) — kaynak: RC 12 §11 sıra 9; 09 satır 63 | Yalnız Python sim | 🟡 KISMİ: `game_run` (args ile `-ExecCmds`), `pie_input`, `pie_walk_to`, `log_wait`. YOK: senaryo oynatıcı + iddia + `BUGS.md` çıktısı | ≈ 10 sa (tahmin) | P1 |
| R-11 | RC | **Co-op test, tek makine** (B2: N istemci PIE; ölçek testi) — kaynak: RC 12 §11 sıra 10 | 2 makine gerekir (kaynak: 12 §11) | ✅ VAR: `pie_start` (1–4 oyuncu, listen/client), `pie_net_emulation`. Kayıt doğrulamalı akış için bkz. K3 | ≈ 3 sa/test (tahmin) | kullan |
| R-12 | RC | **Co-op test, paketli + 2 makine Steam** — kaynak: `13_Ek_Is_ve_Esleme.md` satır 215 (F0-11 → IS-33) | Elle, ertelendi | 🟡 KISMİ: `game_run` (paketli exe). YOK: paketleme (R-14), 2 süreç senkron | ≈ 3 sa/test (tahmin) | P1 |
| R-13 | RC | **B11 Toplu doğrulama** (demo kapısı) — kaynak: RC 12 §11 sıra 11 | Hata oyunda çıkıyor (kaynak: 12 §11) | ✅ VAR: `validate` | ≈ 4 sa (tahmin) | kullan |
| R-14 | RC | **Paketleme:** demo build (Next Fest Haziran 2027 — kaynak: RC 13 §5) — kaynak: 13_Ek satır 210, 215 | Elle RunUAT / `unreal-packaging` skill (betik) | ❌ YOK (Plugins'te `BuildCookRun`/`RunUAT` geçmiyor) | ≈ 8 sa (tahmin: ~20 build × ~25 dk komut + log okuma) | P1 |
| R-15 | RC | **Sahne sahne çekim:** 8 mağaza ekran görüntüsü (≥ 1920×1080) + altın kare — kaynak: RC 14 §5; 12 §10 Q1 | Elle, sahne sahne | 🟡 KISMİ: `screenshot`, `pie_travel`, `game_run` (`shotNames`, çoklu çözünürlük, `variants`), `mrq_render`. YOK: kamera/nokta listesi → N kare + manifest tek çağrıda | ≈ 6 sa (tahmin: ~4 çekim turu × ~1,5 sa) | P2 |
| R-16 | RC | **Toplu görsel inceleme:** G1–G6 ölçümü (palet, bölge L*/S, gürültü) 155 asset — kaynak: RC 12 §5.2 | Betik (`stil_olcu.py`, Blender `rapor.json`) | 🟡 KISMİ: `image_stats` (luma, histogram, doygunluk, referans farkı), `image_compose`. YOK: bölge maskeli ölçüm + eşik raporu (stil_olcu dışarıda kalabilir) | ≈ 5 sa (tahmin) | P2 |

---

## 2. Ortak ihtiyaçlar (kümeler)

"Kaynakta yok" = o oyunun okunan belgelerinde bu ihtiyaç geçmiyor.

### 2.1 Üç oyunda ortak

| Küme | ARPG | GP | RC | Enginewright bugün | Eksik araç (öneri ad) | Toplam kazanç (tahmin, satır toplamı) | Öncelik |
|---|---|---|---|---|---|---|---|
| K1 Veri → DataTable/DataAsset | A-12 | G-05, G-06 | R-01 | ✅ `datatable_set_rows`, `datatable_create`, `batch` | DataAsset oluşturma + JSON ↔ asset diff (`dataasset_write`, `data_diff`) | ≈ 80 sa (tahmin) | kullan; eksik kısım P1 |
| K2 Sanat import + malzeme varyantı | A-01, A-02 | G-04 | R-02, R-03 | 🟡 `asset_import` (skeletal dahil), `material_instance_*`, `mesh_set_material` | 0.4.1 sanat hattı + slot→MI toplu + varyant panosu + toplu referans değiştirme | ≈ 140 sa (tahmin) | P0 |
| K3 Bot playtest / senaryo testi | A-07 | G-01, G-03 | R-10 | 🟡 `pie_input`, `pie_walk_to`, `pie_state`, `log_wait`, `automation_run` | Senaryo oynatıcı: girdi dizisi + iddia + metrik/BUGS çıktısı (`pie_scenario`) | ≈ 43–53 sa (tahmin) | P1 |
| K4 Doğrulama + ID bütünlüğü | A-13 | G-05 | R-07, R-13 | ✅ `validate`, `id_audit` | Satır başına duman testi (ARPG IC-68) | ≈ 28 sa (tahmin; G-05 K1'de de sayılı) | kullan; eksik kısım P2 |
| K5 Paketleme + çok makine test | kaynakta yok | G-02 | R-12, R-14 | 🟡 `game_run` (paketli exe) | `package_build` (BuildCookRun + log özeti) + 2 süreç senkron başlatma | ≈ 8 sa + test başına ~3 sa (tahmin) | P1 |
| K6 Asset kaynağı / AI beyanı | A-08 | kaynakta yok | R-09 | ❌ | `asset_metadata_set` + beyan raporu (E14) | ≈ 18 sa (tahmin) | P1 |

Not: K5 ve K6 iki oyunun belgesinde geçiyor. Üçüncü oyuna da gerektiği **tahmindir** (her Steam oyunu paket ve AI beyanı ister). Bu yüzden üç oyun kümesinde tutuldu ve "kaynakta yok" diye işaretlendi.

### 2.2 İki oyunda ortak

| Küme | Oyunlar | Satırlar | Enginewright bugün | Eksik | Öncelik |
|---|---|---|---|---|---|
| K7 Ağ / co-op test (tek makine) | GP, RC | G-01, R-11 | ✅ `pie_start` (1–4, listen/client), `pie_net_emulation` | Replikasyon iddiası (server ↔ client değer) | P0 |
| K8 Yer tutucu temizliği | GP, RC | G-04, R-06 | ✅ `asset_rename`, `asset_references` | Kalan yer tutucu referansı taraması (RC Q4 = 0 raporu) | P1 |
| K9 Görsel QA (kare + kolaj + ölçüm) | RC (+ ARPG T §12.5 "toplu panolar") | R-04, R-05, R-08, R-15, R-16 | ✅ `screenshot`, `screen_coverage`, `screenshot_ab`, `image_stats`, `image_compose` | `screenshot_tour` (nokta listesi → N kare + manifest) | P2 |
| K10 Seviye yerleştirme / doğrulama | ARPG, GP | A-06, G-07 | 🟡 `scene_dress`, `world_trace`, `pie_walk_to` | Navmesh build (B15), oda/set şablonu, erişilebilirlik testi | P1 |

### 2.3 Tek oyuna özel

| Satır | Oyun | Neden yalnız orada |
|---|---|---|
| A-03 GAS, A-04 StateTree/BT, A-05 AnimBP | ARPG | RC 12 §11: "EW-3/4/5 RagnaCook'ta gerekmez". GP StateTree kullanıyor (TaskList P1-19) ama araç ihtiyacı GP 08 §7'de yok |
| A-09 Niagara, A-10 Headless, A-11 Sequencer | ARPG | Diğer iki oyunun okunan belgelerinde geçmiyor |
| G-03 Fizik/ragdoll | GP | Ganimet fiziği GP'ye özgü (GP 08 §7) |

---

## 3. Mevcut araç eşleşmesi (sayım)

Sayım bu belgenin 36 satırından (ARPG 13, GP 7, RC 16).

| Durum | Sayı | Satırlar |
|---|---|---|
| ✅ VAR (yalnız kullan) | 10 | A-12, R-01, R-02, R-04, R-05, R-06, R-07, R-08, R-11, R-13 |
| 🟡 KISMİ | 19 | A-01, A-02, A-03, A-06, A-07, A-11, A-13, G-01…G-07, R-03, R-10, R-12, R-15, R-16 |
| ❌ YOK | 7 | A-04, A-05, A-08, A-09, A-10, R-09, R-14 |

Sonuç:
- RagnaCook'un 11 maddelik listesinin (RC 12 §11) 9'u bugün araçla karşılanıyor. Açık kalanlar: E14 (R-09) ve E2 (R-10).
- ARPG'nin EW-1..11 listesinin hiçbiri tam karşılanmıyor. Yol haritasını en çok ARPG belirler.
- GP'nin 5 maddesinin (EW-GP-1..5) hepsi KISMİ. En büyük boşluk: replikasyon iddiası ve toplu referans değiştirme.

---

## 4. Yapım maliyeti (yalnız ARPG kaynağı verir)

| EW | Kart-gün (kaynak: ARPG T §12.3) |
|---|---|
| EW-1 | 6–8 |
| EW-2 | 2 |
| EW-3 | 4–5 |
| EW-4 | 3–4 |
| EW-5 | 5–6 |
| EW-6 | 4 |
| EW-7 | 4–5 |
| EW-8 | 5 |
| EW-9 | 6–8 |
| EW-10 | 1 |
| EW-11 | 3 |
| **Toplam** | **≈ 43–51** |

GP ve RC eklerinin yapım maliyeti kaynakta yok. Bunları `03_Yol_Haritasi.md` tahmin eder.
