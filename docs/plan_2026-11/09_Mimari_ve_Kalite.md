# 09 — Mimari ve kalite: yeni araçlar için ortak desenler, teknik borç, güvenilirlik

Kart: `t_ae3a845a` · 18.11.2026 · Yalnız belge. Kod değişmedi. Genel ad düzeltmesi: kart `t_5f61e99c` (18.11, genel MCP kuralı).
Kaynak: kaynak kod okuması (dosya:satır yazılı), `research/arac_test_kapsami.json`, `docs/RELIABILITY_0.4.md`.
08'deki bütün yeni araçlar bu belgeye uyar. Kural yeni değilse "mevcut" yazılı; yeni kural "ÖNERİ" (Talha onayı: SORULAR_2 S2-11).

---

## 1. Araç sözleşmesi

### 1.1 Mevcut yapı (kaynaktan)

| Öğe | Nerede | Ne yapar |
|---|---|---|
| `FEWTool` | `Public/EnginewrightTool.h:28-43` | `Name`, `Description`, `InputSchema` (JSON Schema), `bWrites`, `bTransactional`, `Category`, `Run` ya da `RunAsync` |
| `FEWResult` | `EnginewrightTool.h:7-23` | `bOk`, `ErrorCode`, `Message`, `Data` (JSON nesnesi) |
| Kayıt | `EnginewrightTool.h:73-78` | `Register(ad, açıklama, şema, bWrites, Run, bTransactional=true)`, `RegisterAsync(…)` (async = transactional değil, `EWRegistry.cpp:84`) |
| Kategori | `EnginewrightTool.h:59` `SetCategory` | Ayarlardan kategori kapatılır → `TOOL_DISABLED` (`EWRegistry.cpp:249`) |
| Argüman denetimi | `EWRegistry.cpp:195-247` (`Check`) | Bilinmeyen argüman → `UNKNOWN_ARGUMENT` + öneri; tür ve `enum` → `INVALID_ARGUMENT`; `required` → `MISSING_ARGUMENT` |
| Salt okunur mod | `EWRegistry.cpp:252-` | `bWrites` ya da yan etkili araç → `READ_ONLY` |
| Çağrı kaydı | `EWRegistry.cpp:114-135` (`Record`) | Son 100 çağrı (ad, ok, hata kodu, ms); log düzeyine göre `UE_LOG` |
| İstemci yüzü | README "Tools" | İstemci 3 meta araç görür: `search`, `describe`, `call` |

Sonuç: sözleşme **zaten merkezi**. Yeni araç yalnız şema + `Run` yazar; denetim ve kayıt kayıt defterinde.

### 1.2 Yeni araç kuralları

| # | Kural | Durum | Neden |
|---|---|---|---|
| S1 | Ad `alan_fiil` ya da `alan_nesne`: küçük harf, alt çizgi, mevcut önek ailesine katıl (`datatable_*`, `pie_*`, `asset_*`, `perf_*`) | mevcut desen | `search` aracı önekle bulur; 129 aracın tümü bu biçimde (`arac_envanteri.json`) |
| S2 | Yeni kategori açma; mevcut `SetCategory` değerine ekle (07 §4 listesi) | ÖNERİ | Ayarlardaki kategori kapatma anlamlı kalır |
| S3 | Şemada her alan `type` + kısa `description`; seçenekler `enum` | mevcut desen | `Check` bunları denetler (`EWRegistry.cpp:210-240`) |
| S4 | Yazan araçta `dryRun` (değişiklik listesini döner, yazmaz) | ÖNERİ (bugün 3 araç dosyasında var: AssetTools, GameRunTools, LocGatherTools) | 08'deki toplu yazanlar (`datatable_import`, `dataasset_create`, `scenario_place`, `asset_metadata_set`) için zorunlu |
| S5 | Yazan araç yazdığını geri okur; fark → `READBACK_MISMATCH` | mevcut desen (10 araç dosyasında) | RELIABILITY 0.4 turunun ana dersi: sessiz yazma hatası |
| S6 | Uzun iş (> 10 sn TAHMİN) async: `RegisterAsync` + ayrı `*_status` aracı | mevcut desen (`game_run` + `game_run_status`, `loc_gather` + `_status`, `mrq_render` + `_status`) | Editör donmaz; `package_build`, `soak_run`, `bot_run` bu kalıpla |
| S7 | Çıktı kısa: özet önce (`summary` alanı), ayrıntı dosyaya (`Saved/Enginewright/runs/<araç>/<zaman>/`), yanıtta dosya yolu | ÖNERİ | GP16 EW-GP-02 gerekçesi: "ajan ham json'u bağlama okumadan karar verir" |
| S8 | Dosya okuyan araç (`metrics_read`, `datatable_import`, `vislog_extract`) yalnız proje klasörü altını okur | ÖNERİ | Yerel MCP; yol kaçışı (`..`) → `INVALID_PATH` (kod zaten 8 yerde) |
| S9 | Headless uyum bayrağı: araç editör altsistemine (Slate, viewport, PIE) bağlıysa açıklamada "editor only" | ÖNERİ | 0.9 headless (Y8) hangi aracın commandlet'te çalışacağını bilmeli |
| S10 | Oyuna özgü ad yok (`ai_metrics_read` → `metrics_read`); oyun dosya biçimi parametre. Sürüm başlığı, hedef, araç açıklaması, kabul ölçütü ve şema etiketinde de oyun adı yok; oyun bağlantısı yalnız "(iç)" alanlarda | **KARAR** (Talha 18.11; S2-06 = A) | Genel MCP, herkes için geliştiriliyor; oyunlar geliştirilirken destek olur. Denetim: `research/oyun_adi_denetim.py` |

## 2. Hata modeli

### 2.1 Mevcut (kaynak sayımı)

Kaynakta 195 ayrı büyük harfli sabit dizgi var; çoğu hata kodu (`grep "\"[A-Z][A-Z_]{4,}\""`, Private/**.cpp). En sık:

| Kod | Kullanım sayısı | Anlam |
|---|---|---|
| `INVALID_ARGUMENT` | 160 | Argüman var ama değer geçersiz |
| `INVALID_VALUE` | 20 | Özellik değeri yazılamadı |
| `READBACK_MISMATCH` | 19 | Yazılan ≠ okunan |
| `NO_PLAYER` | 17 | PIE oyuncusu yok |
| `UNKNOWN_CLASS` | 15 | Sınıf bulunamadı (öneriyle) |
| `TIMEOUT` | 11 | Bekleme süresi doldu |
| `*_NOT_FOUND` | 22 farklı kod | `ASSET_`, `ACTOR_`, `TABLE_`, `ROW_`, `WIDGET_`… + düz `NOT_FOUND` |

### 2.2 Yeni araç kuralları

| # | Kural | Neden |
|---|---|---|
| H1 | Önce mevcut kodu kullan. Yeni kod yalnız ajan farklı davranacaksa (ör. `NAV_NOT_BUILT` → önce build) | 195 kod zaten çok |
| H2 | Bulunamadı: `<NESNE>_NOT_FOUND`; düz `NOT_FOUND` kullanma | 5 yerde düz `NOT_FOUND` var → teknik borç T-07 |
| H3 | Mesaj üç parça: ne oldu · neden · ne yapmalı (öneri listesi `EW::Suggest` ile) | `UNKNOWN_ARGUMENT` örneği (`EWRegistry.cpp:205-207`) |
| H4 | Kısmi başarı hata değildir: `bOk=true` + `Data.failed[]` + `summary` | Toplu araçlar (import, 100 satır) tek kötü satırda her şeyi atmamalı; `dryRun` hariç "hepsi ya da hiç" yalnız `batch` ve katı `datatable_*` |
| H5 | Oyun sonucu ile araç hatası ayrı: test kaldıysa `bOk=true`, `verdict:"failed"`; araç çalışamadıysa `bOk=false` | `game_run` deseni (log kalıbıyla karar) — `automation_run`, `metrics_read`, `soak_run` aynı |
| H6 | Yeni kodlar RELIABILITY tablosunda listelenir | Hata kodu sözlüğü dağınık (T-08) |

## 3. Test standardı

### 3.1 Mevcut

- `tests/` = 19 `test_*.py` dosyası + `ewtest.py` (ortak yardımcı), `run_all.py`, `smoke.py`; toplam 4.086 satır (`wc -l tests/*.py`).
- Testler canlı editöre HTTP ile bağlanır (`ewtest.py:1-8`: `ENGINEWRIGHT_TEST_PROJECT`, `127.0.0.1:3100/mcp`). Yazılar yalnız `/Game/_EWTest` altına.
- Kapsam: 129 araçtan 122'si en az bir test dosyasında; 7'si testsiz (`asset_references`, `asset_save`, `mrq_render`, `mrq_status`, `perf_capture`, `perf_scene`, `perf_gpu_profile`).
- Güvenilirlik turu: `RELIABILITY_0.4.md` (her araç OK / FIXED / NOTE + süre).

### 3.2 Yeni araç için zorunlu (Tanım: "bitti")

| # | Koşul | Kanıt |
|---|---|---|
| T1 | `tests/test_<alan>.py` içinde en az: 1 başarılı çağrı, 1 hata yolu (yanlış argüman ya da bulunamadı), yazan araçta geri okuma + `dryRun` | test dosyası |
| T2 | Async araçta: başlat → `*_status` → bitti; zaman aşımı yolu | test dosyası |
| T3 | Oyun doğrulaması (iç): ihtiyacı ilk kullanan gerçek projede 1 koşu (çağrı, süre, sonuç). Kabul ölçütü bundan bağımsız, genel test projesi üzerindendir (T1/T2) | `docs/oyun_kosulari/<OYUN>_<sürüm>.md` (ÖNERİ, yeni klasör) |
| T4 | RELIABILITY_<sürüm>.md satırı | satır |
| T5 | Envanter betiği yeni aracı görür; test kapsamı betiği "testsiz" demez | `arac_test_kapsami.py` çıktısı |

### 3.3 Betiklerin yeri

`research/arac_envanteri.py` ve `arac_test_kapsami.py` bu kartta yazıldı (yalnız okur). 0.6'da (Y3) `tools/` altına taşınır ve sürüm öncesi denetim olur: "kayıtlı araç sayısı = README tablosundaki araç sayısı = test kapsamı". Bu kartta `tools/` değişmedi.

### 3.4 CI seçenekleri (karar: S2-08)

`.github/workflows` yok (yalnız `ISSUE_TEMPLATE`, `PULL_REQUEST_TEMPLATE.md`). GitHub'ın barındırdığı makinelerde Unreal yok.

| Seçenek | Ne koşar | Artı | Eksi |
|---|---|---|---|
| A. Statik CI (GitHub) | envanter + kapsam betikleri, Python test sözdizimi, belge bağlantıları | Ücretsiz, hızlı | Derleme ve canlı test yok |
| B. Talha'nın PC'sinde kendi koşucu (self-hosted) | A + derleme + `run_all.py` | Gerçek kanıt | PC açık olmalı; güvenlik (yalnız özel dallar) |
| C. Elle sürüm öncesi koşu + kayıt | `run_all.py` + RELIABILITY | Bugünkü yol | İnsan unutur |

## 4. Belge şablonu (her araç)

Bugün araç belgesi yalnız README "Tools" tablosunda tek satır + araç açıklaması (`Description`). `docs/EWTOOL.md` araç belgesi değil, `ewtool.py` (editör kapalı derleme) belgesi.
ÖNERİ: araç açıklaması (istemcinin `describe` ile gördüğü) tek kaynak; README satırı ondan kısaltılır.

Açıklama şablonu (İngilizce, kaynak koddaki `Description`):

```
<Bir cümle: ne yapar.> <İkinci cümle: ne zaman kullanılır / hangi araçla birlikte.>
Returns: <özet alanları>. Writes: <neyi yazar | nothing>. Async: <evet → *_status>.
Editor only | Headless ok. Errors: <en olası 2-3 kod>.
```

Belge kontrol listesi (araç başına):
1. Açıklama şablona uyar.
2. README "Tools" tablosunda satır var.
3. CHANGELOG girişinde ad + bir cümle.
4. Örnek çağrı (`tests/` dosyasındaki ilk çağrı) README ya da tarif belgesinde.
5. Oyun doğrulaması (iç) kaydına bağlantı (T3).

## 5. Teknik borç ve güvenilirlik listesi

Öncelik: **Y** = gerçek proje işini engelliyor, **O** = kalite, **D** = düşük. Parantez içi = iç not (etkilenen oyun işi). Sürüm = 10'daki ara sürüm.

| # | Borç | Kanıt | Öncelik (iç not) | Sürüm |
|---|---|---|---|---|
| T-01 | 7 araç testsiz | arac_test_kapsami.json | Y (perf ×3 GP G1'de kullanılacak) | 0.4.1 |
| T-02 | 5 araç RELIABILITY tablosunda yok (perf ×3, mrq ×2) | arac_test_kapsami.json | O | 0.4.1 |
| T-03 | `asset_find` olmayan klasöre sessiz boş liste | RELIABILITY_0.4.md:67 | O | 0.4.1 |
| T-04 | ASCII dışı asset adı uyarısız (`asset_import`, `blueprint_create`) | RELIABILITY_0.4.md:68, :81 | Y (RC 155 asset + 186 ses, Türkçe dosya adları) | 0.4.1 |
| T-05 | `game_run` koşu klasörü sınırsız büyür (gerçek projede 145 koşu / 243 MB; iç: RC) | RELIABILITY_0.4.md §Kept for 0.4.1 | O (0.5 soak + bot ile hızlanır) | 0.4.1 |
| T-06 | `actor_preview` kamera kollu Character'ı gevşek çerçeveler | RELIABILITY_0.4.md:61 | O (ARPG varyant panosu) | 0.4.1 |
| T-07 | Düz `NOT_FOUND` (5 yer) ve 22 farklı `*_NOT_FOUND` | kaynak grep | D | 0.9 (dondurma öncesi) |
| T-08 | Hata kodu sözlüğü yok (195 sabit dizgi) | kaynak grep | O | 0.9 (Y9 ile) |
| T-09 | `pie_start` oyunun ilk açılış süresine bağlı; ağır ana menü haritasında listen 2 oyuncu `TIMEOUT` (iç: RC) | RELIABILITY_0.4.md:140 | Y (RC IS-33, GP GI-07) | 0.4.1 (bekleme parametresi + belge) |
| T-10 | PIE okuması `input_map_list` oyunun kare takılmasıyla 0,1–8 sn (en yavaş 12,75 sn) | RELIABILITY_0.4.md:116, §Kept | D (motor/oyun kaynaklı) | belge notu |
| T-11 | Motor sürümü koruması yok: kaynakta `ENGINE_MAJOR/MINOR_VERSION` 0 kez; yalnız UE 5.8 Win64 denenmiş | kaynak grep, README "Supported versions" | O (Y7 çoklu UE'de ilk iş) | Y7 |
| T-12 | CI yok | `.github/` içeriği | O | Y4 |
| T-13 | Araç belgesi tek satır (README) + açıklama; şablon yok | README "Tools", §4 | O | Y5 (0.6) |
| T-14 | Oyun belgelerinde eski durum (RC12 EW-R20 "B11 yok"; GP16 EW-GP-01 "eksik") | 07 §6 | D (oyun reposu; yalnız not) | S2-04 |
| T-15 | 16.11 notunda "133 araç" (ROADMAP_EKSIKSIZ); kaynak 129 | 00_Plan, arac_envanteri | D | bu kartta not edildi |
| T-16 | Çağrı kaydı yalnız bellekte son 100 (`EWRegistry.cpp:122`); uzun oturumda (soak, bot) iz kaybolur | kaynak | O | 0.5 (soak ile) |

## 6. Güvenilirlik turu (her sürüm sonunda)

0.4 turu deseni korunur (`RELIABILITY_0.4.md`): her araç gerçek projede çağrılır, durum + süre yazılır.
ÖNERİ: tur dosyası sürüm başına (`RELIABILITY_0.5.md` …); "Kept for" bölümü bir sonraki sürümün Y1 iş listesi olur (08 §2.2 deseni).
