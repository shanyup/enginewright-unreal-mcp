# 06 — Birleşik ihtiyaç listesi (3 oyun → tek numara EW-xxx)

Kart: `t_ae3a845a` · 18.11.2026 · Yalnız belge. Oyun repoları yalnız okundu.
Veri kaynağı: `research/ihtiyac_esleme.py` (tek tablo, 06 ve 07 bundan üretilir). Tablo değişirse betiği düzelt, yeniden çalıştır.
Alıntılar: `research/raw/oyun_RC_12_s11.md`, `oyun_GP_EW.md`, `oyun_ARPG_EW.md` (kaynak satırların kopyası).

## 1. Kaynaklar ve kısaltmalar

| Kısa | Belge | Ne verir |
|---|---|---|
| RC12 | `UE/RagnaCook/Docs/Icerik_Sistemi/12_Icerik_Uretim_Sistemi.md` §11.1 | EW-R01…R22 (22 satır, öncelik P0/P1/P2, 18.11 son) |
| RC13E | `…/13_Ek_Is_ve_Esleme.md` §1 | IS / EK / LOC işleri, aşama (Stil kilidi → Demo D1 → EA → 1.0), bağımlılık |
| RC20 | `…/20_Lokalizasyon_Sistemi.md` | LOC-1…9 ayrıntısı |
| GP16 | `Game/GunpointParole/docs/GDD_v2_DusmanYZ.md` §16 | **YENİ** EW-GP-01…09 (17.11, K-131; "hepsi YZ-0'dan önce") |
| GP05 | `…/docs/YZ_v2/05_Otomatik_YZ_Test_Sistemi.md` §8 | GP16 ile aynı 9 satır + kapı (G0…G3) |
| GP06 | `…/docs/YZ_v2/06_Uygulama_Plani.md` §2 | YZ-0…YZ-5 adımları, kapılar |
| GP08 | `…/docs/Icerik_Sistemi/08_Icerik_Uretim_Sistemi.md` §7 | **ESKİ** EW-GP-1…5 (içerik üretimi) |
| GP09 | `…/docs/Icerik_Sistemi/09_Is_Listesi.md` | GI-xx işleri (P2 / Demo) |
| AT | `Game/ARPG/Docs/ARPG_Tasarim_Kararlari.md` §12.3–12.4 | ARPG EW-1…11 + sıra önerisi + fazlar F0…F6 |
| A09 | `Game/ARPG/Docs/Icerik_Sistemi/09_Is_Listesi.md` | IC-xx işleri (Prototip / Dikey dilim / Demo / 1.0) |
| EW02 | `docs/plan_2026-11/02_Oyun_Ihtiyaclari.md` (v2, 17.11) | A-01…13, G-01…07, R-01…16, ortak küme K1…K10 |
| EKS | `docs/EKSIKLER_VE_ONCELIK.md` | A/B/E maddeleri |

## 2. Sayılar (betik çıktısı: `research/ihtiyac_esleme_ozet.json`)

| Ölçü | Değer |
|---|---|
| Birleşik ihtiyaç | **56** (EW-001…EW-111, kümeye göre onluk blok) |
| Üç oyunun ortak ihtiyacı | 7 |
| İki oyunun ortak ihtiyacı | 25 |
| Tek oyunun ihtiyacı | 24 |
| Eşlenen eski numara | 123 (EW-R 22/22, GP YENİ 9/9, GP ESKİ 5/5, ARPG EW 11/11, A 13/13, G 7/7, R 16/16, K 10/10, EKS 30) |
| Mevcut araç durumu (ayrıntı 07) | Var 18 · Kısmen 20 · Yok 18 |

Kümeler (ID bloğu): VERİ 001–007 (7) · TEST 010–020 (11) · AĞ 030–032 (3) · PERF 040–043 (4) · LOC 050–055 (6) · GÖRSEL 060–065 (6) · SANAT 070–075 (6) · YZ 080–084 (5) · SEVİYE 090–092 (3) · MEDYA 100–102 (3) · ALTYAPI 110–111 (2).

Numara kuralı: ID'ler sabit kalır. Yeni ihtiyaç bloğun boş numarasını alır (ör. VERİ'de EW-008). Silinen ihtiyacın numarası tekrar kullanılmaz.

## 3. GP numara çakışması — çözüm

GP'de aynı ön ekle iki ayrı liste var:

```
GP08 §7 (ESKİ, içerik üretimi)      GP16 / GP05 §8 (YENİ, YZ testi, 17.11)
EW-GP-1  Ağ testi            ──┐    EW-GP-01 automation_run (başsız)
EW-GP-2  Fizik/ragdoll         │    EW-GP-02 ai_metrics_read
EW-GP-3  Yer tutucu geçişi     │    EW-GP-03 vislog_extract
EW-GP-4  DataAsset toplu       │    EW-GP-04 nav_build_check
EW-GP-5  Sim ↔ veri köprüsü  ──┘    … EW-GP-09 scenario_place
        Tek hane = ESKİ              İki hane (0 ile) = YENİ
```

- İki liste **ayrı satırlarla** eşlendi (§5 tablosu, "Kaynak liste" sütunu). Biri diğerinin yerine geçmez; konuları örtüşmüyor.
- Ayırt etme kuralı: `EW-GP-<tek hane>` = GP08 ESKİ, `EW-GP-0<hane>` = GP16 YENİ. Bu belgeden sonra ikisi de yalnız eşleme sütununda geçer; yeni ad EW-xxx.
- GP belgelerinde yeniden adlandırma bu kartın işi değil (oyun reposu salt okunur). Soru: SORULAR_2 S2-03.

## 4. ARPG varsayımları (T1 / T3 / T4 AÇIK)

AT §2 "Temel kararlar"da T1 Motor, T3 Kapsam, T4 Başlama zamanı "[AÇIK]". Bu belge şu varsayımla yazıldı; **VARSAYIM** olarak işaretlidir:

| Karar | Varsayım | Etkisi bu belgede |
|---|---|---|
| T1 Motor | **UE 5.8** (AT önerisi) | ARPG satırları (EW-071, 080–083, 091, 110) geçerli. Unity seçilirse bu satırlar ARPG'den düşer; RC/GP payı kalır. |
| T3 Kapsam | **4 akt** (AT önerisi) | Sayılar (22 aile, 66 varyant, ~8 ara sahne) AT §12.3'ten; 3 akt seçilirse yaklaşık %25 az içerik, araç listesi aynı. |
| T4 Başlama | **Şimdi yalnız ön-prodüksiyon** (AT önerisi); tam prodüksiyon RC demosundan sonra (AT §12.4) | ARPG'nin "ihtiyaç anı" F0 (ön-prodüksiyon + EW ARPG paketi) ve sonrası; RC/GP'den sonra sıralanır. |

## 5. Birleşik tablo

"Oyunun ihtiyaç anı": oyunun kendi iş listesindeki aşama + ilk kullanan iş. RC aşamaları RC13E §4 (Stil kilidi → Demo D1 → EA → 1.0); GP aşamaları GP06 (YZ-0…YZ-5, kapı G0…G5) ve GP09 (P2 → Demo); ARPG fazları AT §12.4 (F0…F6) ve A09 aşaması. ★ = oyunun kritik yolu (RC13E §3).

| ID | İhtiyaç | Oyun(lar) | Oyundaki iş / belge | Oyunun ihtiyaç anı (sıra/bağımlılık) | Ortak küme | Eski numaralar |
|---|---|---|---|---|---|---|
| EW-001 | DataTable satırlarını katı denetimle toplu yazma (bilinmeyen alan = hata, hepsi ya da hiç) | RC, ARPG, GP | RC: IS-10 kart/tanrı/mod aktarımı (48 kart + 4 lanet + 21 sunak), IS-23 `DT_Unlocks` 58 satır (RC12 §11.1) · ARPG: IC-01 veri fabrikası (A09) · GP: `gp.py data` (GP08 §6) | RC Demo D1 ilk kod işi (IS-10, bağımlılık yok) · ARPG Prototip/F1 IC-01 · GP içerik (GI-xx) | VERİ (3 oyun) | EW-R01, EW-R02 · R-01 · A-12 · K1 |
| EW-002 | CSV/JSON dosyasından şemaya göre upsert + fark raporu (eklenen/değişen/silinen) | RC, ARPG | RC: IS-10 `aktar.py` (RC12 §11.1 "CSV/JSON'dan toplu upsert + fark raporu yok") · ARPG: `DT_Affix.csv` 357 satır, IC-01/IC-38/IC-53 (EW02 A-12) | RC Demo D1 IS-10 (betikle geçilebilir) · ARPG Prototip/F1 IC-01 | VERİ (2 oyun) | EW-R01 (eksik notu) · A-12 · E17 |
| EW-003 | DataAsset toplu oluşturma / okuma / doğrulama (100+ asset) | GP, ARPG | GP: `gp.py data` SPECS → DataAsset, K-48 (GP08 §7) · ARPG: içe aktarma → DataTable + DataAsset (ARPG 08 §1) | GP içerik GI-02…GI-27 (Demo) · ARPG F1 | VERİ (2 oyun) | EW-GP-4 (ESKİ, GP08) · G-05 · K1 |
| EW-004 | Sim ↔ oyun verisi köprüsü: oyundaki veriyi dışa aktar, sim çıktısıyla karşılaştır | GP, ARPG, RC | GP: GI-29 denge turu "sim'i gerçek veriye bağla" (GP09) · ARPG: IC-12 ölçüm kaydı → CSV, IC-43 sim kalibrasyonu (A09) · RC: IS-32 denge turu (RC13E) | GP Demo GI-29 · ARPG Prototip/F1 IC-12 · RC Demo IS-32 | VERİ (3 oyun) | EW-GP-5 (ESKİ, GP08) · G-06 · E18 |
| EW-005 | ID bütünlüğü: tanımlı ↔ kullanılan ID'ler (DataTable, BP, data asset) | RC, ARPG | RC: IS-10, IS-34 V1–V3 (RC12 §11.1) · ARPG: IC-68 doğrulayıcı (A09) | RC Demo D1 IS-10 → IS-34 kapı · ARPG 1.0 IC-68 | VERİ (2 oyun) | EW-R03 · R-07 · A-13 · K4 |
| EW-006 | Tek çağrıda proje doğrulaması: bütün BP derle, Data Validation, kırık referans, map check | RC, ARPG | RC: IS-34 D1 demo kapısı (RC12 §11.1) · ARPG: IC-68 doğrulayıcı + İçerik Testi haritası (A09) | RC Demo D1 IS-34 (kapı) · ARPG 1.0 IC-68 | VERİ (2 oyun) | EW-R20 · R-13 · A-13 · K4 · B11 |
| EW-007 | İçerik duman testi: her veri satırını oyunda spawn edip hatasız mı bak (İçerik Testi haritası) | ARPG | ARPG: IC-68 "Doğrulayıcının tamamı + İçerik Testi haritası" (A09) | ARPG 1.0 IC-68 | VERİ (1 oyun) | A-13 |
| EW-010 | Automation testlerini koşup sonucu ayrıştırma (geçti/kaldı/süre) | GP, RC | GP: YZ-0 test modülü, `AGPAITestScenario` (GP06 §2) · RC: EK-03 kural kancası öz-testi `RC_RunSelfTests` (RC12 §11.1) | GP YZ-0 öncesi (GP16: "dokuzu YZ-0'dan önce") · RC Demo D1 EK-03 | TEST (2 oyun) | EW-GP-01 (YENİ) · EW-R04 |
| EW-011 | Başsız automation koşusu (`-game -nullrhi` / paket) + XML/json rapor ayrıştırma | GP, RC | GP: G0 "3 senaryo hatasız biter" (GP06 §2) · RC: 09 bot koşusu `-nullrhi -unattended -ExecCmds="RC.BotRun…"` (RC 09_Dengeleme_ve_Test_Plani.md:87) | GP YZ-0 öncesi · RC Demo IS-32 | TEST (2 oyun) | EW-GP-01 (YENİ, "başsız") · E18 |
| EW-012 | PIE senaryo oynatıcı: adım listesi (girdi → bekle → durum iddiası) tek çağrıda | RC, ARPG | RC: EK-03 kart başına senaryo "kartı ver → olay tetikle → sonuç oku", C49 Kazan Atışı (IS-62, T-06b) (RC12 §11.1) · ARPG: senaryo testleri (EW02 K3) | RC Demo D1 IS-21 (kart kuralları) · RC 1.0 IS-62 · ARPG F1 | TEST (2 oyun) | EW-R04 · EW-R05 · E2 (parça) |
| EW-013 | Bot playtest + denge koşusu: oyunun kendi botunu başlat, metrikleri topla | RC, ARPG, GP | RC: IS-32 playtest, IS-52 bot gece koşusu (RC12 §11.1) · ARPG: IC-43/IC-51/IC-69 denge turları (A09) · GP: YZ bot profilleri (GP06 §2) | RC Demo IS-32 (P2) · GP YZ-0 G0 · ARPG Demo IC-43 | TEST (3 oyun) | EW-R17 · R-10 · EW-8 (ARPG) · A-07 · K3 · E2/E18 |
| EW-014 | Oyunun yazdığı ölçüm dosyasını (json/csv) okuyup kapıya göre özet | GP, ARPG, RC | GP: `Saved/AITest/<son>/metrics.json` + `ozet.md` (GP16) · ARPG: IC-12 TTK/ölüm/item CSV (A09) · RC: bot koşu çıktısı (RC 09) | GP YZ-0 öncesi (G0) · ARPG Prototip/F1 IC-12 | TEST (3 oyun) | EW-GP-02 (YENİ) |
| EW-015 | Visual Logger dökümü: aktör + zaman aralığı → metin/PNG | GP | GP: G1 kırmızı olayın nedeni (GP16) | GP YZ-0 öncesi (ilk kullanım G1) | TEST (1 oyun) | EW-GP-03 (YENİ) |
| EW-016 | Gameplay Debugger anlık görüntü (seçili YZ, kategori, ekran + metin) | GP | GP: utility puanı / rol kanıtı (GP16) | GP YZ-0 öncesi (ilk kullanım G1) | TEST (1 oyun) | EW-GP-05 (YENİ) |
| EW-017 | StateTree hata ayıklayıcı izini metne çevirme | GP, ARPG | GP: M12 titreme nedeni (GP16) · ARPG: 22 aile StateTree (AT EW-3, dolaylı) | GP YZ-0 öncesi (ilk kullanım G2) | TEST (2 oyun) | EW-GP-08 (YENİ) |
| EW-018 | Soak koşusu: paket `-game -nullrhi` uzun koşu, 60 sn'de bellek/ms/takılma satırı, çökme yakalama | GP, RC | GP: 30 dk soak, G3 (GP16) · RC: IS-31 paket + performans (RC13E) | GP YZ-0 öncesi (ilk kullanım G3) · RC Demo IS-31 | TEST (2 oyun) | EW-GP-07 (YENİ) |
| EW-019 | Senaryo haritasına YZ / bot / Smart Object'i veri listesinden yerleştirme | GP, ARPG | GP: S1 Koridor / S2 / S3 test haritaları (GP06 §2) · ARPG: İçerik Testi haritası IC-68 | GP YZ-0 ile (S11 açık) · ARPG 1.0 | TEST (2 oyun) | EW-GP-09 (YENİ, "isteğe bağlı") |
| EW-020 | Kayıt testi ve göçü: kayıt alanı yaz/oku, demo → EA kaydı | RC, GP | RC: IS-54 T-15 `DemoImportState` KS-8 (RC12 §11.1) · GP: P2-11 kaydet → kapat → Devam (EW02 G-02) | RC EA IS-54 · GP Demo | TEST (2 oyun) | EW-R14 · G-02 (parça) |
| EW-030 | Tek makinede N istemcili co-op testi + kötü ağ emülasyonu | RC, GP | RC: IS-33 MP testleri T-09…T-11 (RC12 §11.1) · GP: hasar/ekonomi server'da K-11, GI-07 tezgâh ağ koşulu (GP08 §7, GP09) | RC Demo IS-33 · GP P2 GI-07 | AĞ (2 oyun) | EW-R16 · R-11 · EW-GP-1 (ESKİ, GP08) · G-01 · K7 · B2/E1 |
| EW-031 | Replikasyon iddiası: server ↔ istemci değer karşılaştırması tek çağrıda | GP, RC | GP: tüm hasar/ekonomi server'da (GP08 §7) · RC: IS-33 | GP P2 GI-07 · RC Demo IS-33 | AĞ (2 oyun) | EW-GP-1 (ESKİ) · K7 |
| EW-032 | Paketli build ile 2 makine / Steam co-op testi | GP, RC | GP: P2-11 2 makine Steam testi (EW02 G-02) · RC: IS-33 (13_Ek satır 215, F0-11) | GP Demo · RC Demo IS-33 | AĞ (2 oyun) | G-02 · R-12 · K5 |
| EW-040 | Editör/PIE kare süresi, sahne maliyeti, GPU geçiş profili | GP, RC | GP: M9 host maliyeti (GP16) · RC: IS-31 (RC12 §11.1) | GP YZ-0 öncesi (G1) · RC Demo IS-31 | PERF (2 oyun) | EW-GP-06 (taban) · EW-R15 (parça) · E19–E21 |
| EW-041 | Stat grubu filtresi (ör. `GPAI_*` scope) ile ölçüm | GP | GP: M9 YZ host maliyeti (GP16) | GP YZ-0 öncesi (G1) | PERF (1 oyun) | EW-GP-06 (YENİ, "scope") |
| EW-042 | Paketli / `-game` oyunda FPS örnekleme (Steam Deck 800p dahil) | RC, GP | RC: IS-31 paket + Deck, IS-52 (RC12 §11.1) · GP: M9 (GP16) | RC Demo IS-31 · GP YZ-0 öncesi (G1) | PERF (2 oyun) | EW-R15 · EW-GP-06 ("-game modu") · E22/E3 |
| EW-043 | Paketleme / cook + durum takibi (demo build, dedicated server) | RC, GP, ARPG | RC: IS-31, IS-35 Next Fest build (RC13E) · GP: dedicated server + 2 makine (EW02 G-02) · ARPG: Demo (A09 G3) | RC Demo IS-31 → IS-35 · GP Demo · ARPG Demo | PERF (3 oyun) | EW-R15 (B17) · R-14 · K5 |
| EW-050 | String Table toplu yazma (kaynak EN + TR) | RC | RC: LOC-1, LOC-2 (RC12 §11.1, RC20) | RC Demo LOC-1 | LOC (1 oyun) | EW-R06 |
| EW-051 | Çeviri eksik / bozuk kontrolü (eksik anahtar, `{x}`, çevrilmemiş) | RC | RC: LOC-2, RC20 §5.4 | RC Demo LOC-2 | LOC (1 oyun) | EW-R07 |
| EW-052 | Lokalizasyon gather / export / import / compile (.po) | RC | RC: LOC-2, LOC-5, LOC-8, LOC-9 | RC Demo LOC-2 → 1.0 LOC-9 | LOC (1 oyun) | EW-R08 · B16/E15 |
| EW-053 | Glif testi (CJK, Tay, Arap fontlarında eksik karakter) | RC | RC: LOC-3 (Demo), LOC-6 (EA), LOC-6b (1.0) | RC Demo LOC-3 | LOC (1 oyun) | EW-R09 |
| EW-054 | Dil × ekran görüntüsü döngüsü (her dilde bütün UI ekranları, tek çağrı) | RC | RC: LOC-4, 09 T-16 | RC Demo LOC-4 (LOC-5'ten önce) | LOC (1 oyun) | EW-R10 |
| EW-055 | Ekran taşma testi: kesilen / taşan metin sayısı (TextBlock istenen > ayrılan) | RC | RC: LOC-4 T-16 (RC12 §11.1: "tek P0 eksik araç") | RC Demo LOC-4 → LOC-5 → IS-32 | LOC (1 oyun) | EW-R11 |
| EW-060 | Oyun kamerasından kare (altın kare, mağaza/trailer kareleri) | RC | RC: IS-03, IS-32, IS-53 (RC12 §11.1) | RC Stil kilidi IS-03 (★ kritik yol) | GÖRSEL (1 oyun) | EW-R12 · R-04 · K9 · B3 |
| EW-061 | Okunabilirlik: nesne ekranda kaç px (yemek ≥ 28 px) | RC | RC: IS-03, IS-13 (RC12 §11.1) | RC Stil kilidi → Demo IS-13 | GÖRSEL (1 oyun) | EW-R13 · R-05 · K9 · E5 |
| EW-062 | Önce/sonra kolaj + sayısal görsel fark (SSIM) | RC, ARPG | RC: Q2 her 10 asset onayı (RC12 §10) · ARPG: toplu panolar (AT §12.5) | RC Demo IS-13 | GÖRSEL (2 oyun) | R-08 · K9 · E8 |
| EW-063 | Toplu stil ölçümü: bölge maskesiyle palet / L* / gürültü (G1–G6) | RC | RC: IS-03 `stil_olcu.py` (RC13E) | RC Stil kilidi IS-03 | GÖRSEL (1 oyun) | R-16 · K9 |
| EW-064 | Kamera turu: N konumdan sırayla kare + manifest | RC | RC: 8 mağaza görüntüsü (RC 14 §5), IS-53 | RC Demo IS-35 · EA IS-53 | GÖRSEL (1 oyun) | R-15 · K9 |
| EW-065 | Varyant önizleme panosu: malzeme override + animasyon karesi | ARPG, RC | ARPG: 66 düşman varyantı (AT EW-2) · RC: tipleme önizleme (EKS E34) | ARPG F0 | GÖRSEL (2 oyun) | EW-2 (parça) · E25/E34 |
| EW-070 | Editör açıkken toplu içe aktarma (mesh, doku, ses, font) | RC, ARPG, GP | RC: 155 asset + 186 ses, IS-13/25/41/51/63 (RC12 §11.1) · ARPG: kit/oda/aile assetleri · GP: kendi asset geçişi | RC Demo IS-13 (★) · ARPG F0 · GP Demo | SANAT (3 oyun) | EW-R18 · R-02 · K2 · B9 |
| EW-071 | İskeletli mesh + animasyon import, IK retarget, soket, Physics Asset, montage/blendspace/notify | ARPG, RC, GP | ARPG: 22 aile + 8 boss + oyuncu (AT EW-1, 6–8 kart-gün) · RC: yalnız Garmr (RC12 §11) · GP: YZ-2 retarget + Physical Animation (GP06:22) | ARPG F0 (AT §12.3: "ilk dördü prototipten önce") · GP YZ-2 · RC EA | SANAT (3 oyun) | EW-1 (ARPG) · A-01 · EW-R18 (Garmr) · E23 |
| EW-072 | Malzeme varyantı: master → MI, FBX slotu başına MI + doku bağlama | ARPG, RC | ARPG: 66 düşman varyantı (AT EW-2, 2 kart-gün) · RC: 155 asset `M_RC_Boya` MI (EW02 R-03) | RC Demo IS-13 (★) · ARPG F0 | SANAT (2 oyun) | EW-2 · A-02 · R-03 · E24 |
| EW-073 | Sanat bütçesi kontrolü (üçgen, doku boyutu)  | ARPG, RC | ARPG: AT EW-1 · RC: 155 asset (RC12 §7) | ARPG F0 · RC Demo IS-13 | SANAT (2 oyun) | EW-1 ("bütçe") · ROADMAP 0.4.1 |
| EW-074 | Güvenli taşıma + kalan yer tutucu taraması (oyuncu yolunda referans = 0) | RC, GP | RC: IS-30 ★ `_Kullanilmayan` taşıma (RC13E) · GP: K-70 Synty yer tutucu (GP08 §7) | RC Demo IS-30 (★) · GP Demo | SANAT (2 oyun) | R-06 · EW-R18 · EW-GP-3 (ESKİ, GP08) · G-04 · K8 · B5 |
| EW-075 | Asset kaynağı / lisans / AI beyanı metadata + beyan raporu | RC, ARPG | RC: `Asset_Kaydi.csv`, IS-35 demo sayfası, IS-53 (RC12 §11.1) · ARPG: Steam AI beyanı (AT EW-10) | RC Demo IS-35 · ARPG 1.0 | SANAT (2 oyun) | EW-R19 · R-09 · EW-10 (ARPG) · A-08 · K6 · E14 |
| EW-080 | StateTree / Behavior Tree yazma | ARPG, GP | ARPG: 22 aile + boss fazları (AT EW-3, 4–5 kart-gün) · GP: YZ-0 StateTree geçişi (GP06:12, :20); GP16 StateTree yazma aracı istemiyor, yalnız iz (EW-GP-08) | ARPG F0 (prototipten önce) · GP YZ-0 (belgelenmemiş ihtiyaç, TAHMİN) | YZ (2 oyun) | EW-3 (ARPG) · A-04 · B14 |
| EW-081 | AnimBP state machine şablonu (iskelet ailesi başına) | ARPG | ARPG: 5 iskelet ailesi (AT EW-4, 3–4 kart-gün) | ARPG F0 (AT §12.3 sıra önerisinde 5., "ilk dört"ten sonra) | YZ (1 oyun) | EW-4 · A-05 · B13 |
| EW-082 | GAS: Ability / Effect / Attribute / Cue oluştur, oku, test | ARPG | ARPG: 36 skill, 108 mutasyon, affix = GameplayEffect (AT EW-5, 5–6 kart-gün); IC-02 attribute set (A09) | ARPG F0 (prototipten önce) → Prototip/F1 IC-02 | YZ (1 oyun) | EW-5 · A-03 |
| EW-083 | Niagara parametre / renk-element varyantı / önizleme | ARPG | ARPG: skill + element + aura VFX (AT EW-6, 4 kart-gün); IC-16 gerçek VFX | ARPG Dikey dilim/F2 IC-16 | YZ (1 oyun) | EW-6 · A-09 |
| EW-084 | Fizik / ragdoll doğrulama (ganimet taşıma, sekme determinizmi) | GP | GP: K-106 sekme, elit ragdoll eşikleri (GP08 §7); YZ-1 ragdoll ölüm (GP06:21); "ragdoll belirlenimci değil" riski (GP06:50) | GP YZ-1 · Demo | YZ (1 oyun) | EW-GP-2 (ESKİ, GP08) · G-03 |
| EW-090 | NavMesh build + kopuk ada / ulaşılamaz nokta raporu | GP, ARPG | GP: M1 takılma (GP16) · ARPG: kat erişilebilirliği (AT EW-7) | GP YZ-0 öncesi (G1) · ARPG Prototip/F1 IC-10 | SEVİYE (2 oyun) | EW-GP-04 (YENİ) · EW-7 (parça) · A-06 · B15 |
| EW-091 | Oda parçası + kat doğrulayıcı (bağlantı noktası, ızgara) | ARPG | ARPG: 7 kit × ~30 oda, prosedürel kat (AT EW-7, 4–5 kart-gün) | ARPG Prototip/F1 IC-10 (tek kit gri kutu) | SEVİYE (1 oyun) | EW-7 · A-06 · K10 |
| EW-092 | Yerleşim seti: iş noktası grupları + tohum (veri → seviye) | GP | GP: GI-03 yerleşim seti, GI-26/27 B1 mekânları (GP09) | GP P2 GI-03 → Demo GI-26 | SEVİYE (1 oyun) | G-07 · K10 |
| EW-100 | Sequencer kurma (Level Sequence, kamera, kesme, anahtar) | ARPG, RC | ARPG: ~8 akt ara sahnesi (AT EW-11, P2) · RC: IS-53 trailer (RC12 §11.1) | RC EA IS-53 · ARPG 1.0 | MEDYA (2 oyun) | EW-11 · A-11 · EW-R22 · E29 |
| EW-101 | Movie Render Queue çalıştırma | RC | RC: IS-53 trailer / Reel | RC EA IS-53 | MEDYA (1 oyun) | EW-R22 |
| EW-102 | Oynanış kaydı (Take Recorder → sekans → MRQ) | RC | RC: IS-53 (RC12 §11.1) | RC EA IS-53 | MEDYA (1 oyun) | EW-R22 · E32 |
| EW-110 | Headless mod: aynı araçlar editör kapalıyken (import, veri) | ARPG | ARPG: tek editör kuyruğunu açar (AT EW-9, 6–8 kart-gün) | ARPG F3 prodüksiyon (AT §12.3 sıra önerisinde 10.) | ALTYAPI (1 oyun) | EW-9 · A-10 · B18 |
| EW-111 | Sabit tuş denetimi + girdi istemleri | RC | RC: IS-34 ekip tuş kuralı | RC Demo IS-34 | ALTYAPI (1 oyun) | EW-R21 · E7 |


## 6. Eski numara → yeni ID

Her eski numara en az bir yeni ID'ye bağlı. Bir eski numara birden çok yeni ID'ye bölündüyse (ör. EW-R15 → performans, paketli FPS, paketleme) hepsi yazılı.
GP satırları "Kaynak liste" sütunuyla ayrılır: GP08 ESKİ (tek hane) ve GP16 YENİ (iki hane).

| Eski numara | Kaynak liste | Yeni ID(ler) |
|---|---|---|
| EW-1 | ARPG AT §12.3 | EW-071, EW-073 |
| EW-2 | ARPG AT §12.3 | EW-065, EW-072 |
| EW-3 | ARPG AT §12.3 | EW-080 |
| EW-4 | ARPG AT §12.3 | EW-081 |
| EW-5 | ARPG AT §12.3 | EW-082 |
| EW-6 | ARPG AT §12.3 | EW-083 |
| EW-7 | ARPG AT §12.3 | EW-090, EW-091 |
| EW-8 | ARPG AT §12.3 | EW-013 |
| EW-9 | ARPG AT §12.3 | EW-110 |
| EW-10 | ARPG AT §12.3 | EW-075 |
| EW-11 | ARPG AT §12.3 | EW-100 |
| B2 | EKSIKLER_VE_ONCELIK | EW-030 |
| B3 | EKSIKLER_VE_ONCELIK | EW-060 |
| B5 | EKSIKLER_VE_ONCELIK | EW-074 |
| B9 | EKSIKLER_VE_ONCELIK | EW-070 |
| B11 | EKSIKLER_VE_ONCELIK | EW-006 |
| B13 | EKSIKLER_VE_ONCELIK | EW-081 |
| B14 | EKSIKLER_VE_ONCELIK | EW-080 |
| B15 | EKSIKLER_VE_ONCELIK | EW-090 |
| B16 | EKSIKLER_VE_ONCELIK | EW-052 |
| B17 | EKSIKLER_VE_ONCELIK | EW-043 |
| B18 | EKSIKLER_VE_ONCELIK | EW-110 |
| E1 | EKSIKLER_VE_ONCELIK | EW-030 |
| E2 | EKSIKLER_VE_ONCELIK | EW-012, EW-013 |
| E3 | EKSIKLER_VE_ONCELIK | EW-042 |
| E5 | EKSIKLER_VE_ONCELIK | EW-061 |
| E7 | EKSIKLER_VE_ONCELIK | EW-111 |
| E8 | EKSIKLER_VE_ONCELIK | EW-062 |
| E14 | EKSIKLER_VE_ONCELIK | EW-075 |
| E15 | EKSIKLER_VE_ONCELIK | EW-052 |
| E17 | EKSIKLER_VE_ONCELIK | EW-002 |
| E18 | EKSIKLER_VE_ONCELIK | EW-004, EW-011, EW-013 |
| E19 | EKSIKLER_VE_ONCELIK | EW-040 |
| E21 | EKSIKLER_VE_ONCELIK | EW-040 |
| E22 | EKSIKLER_VE_ONCELIK | EW-042 |
| E23 | EKSIKLER_VE_ONCELIK | EW-071 |
| E24 | EKSIKLER_VE_ONCELIK | EW-072 |
| E25 | EKSIKLER_VE_ONCELIK | EW-065 |
| E29 | EKSIKLER_VE_ONCELIK | EW-100 |
| E32 | EKSIKLER_VE_ONCELIK | EW-102 |
| E34 | EKSIKLER_VE_ONCELIK | EW-065 |
| K1 | EW02 ortak küme | EW-001, EW-003 |
| K2 | EW02 ortak küme | EW-070 |
| K3 | EW02 ortak küme | EW-013 |
| K4 | EW02 ortak küme | EW-005, EW-006 |
| K5 | EW02 ortak küme | EW-032, EW-043 |
| K6 | EW02 ortak küme | EW-075 |
| K7 | EW02 ortak küme | EW-030, EW-031 |
| K8 | EW02 ortak küme | EW-074 |
| K9 | EW02 ortak küme | EW-060, EW-061, EW-062, EW-063, EW-064 |
| K10 | EW02 ortak küme | EW-091, EW-092 |
| A-01 | EW02 §ARPG | EW-071 |
| A-02 | EW02 §ARPG | EW-072 |
| A-03 | EW02 §ARPG | EW-082 |
| A-04 | EW02 §ARPG | EW-080 |
| A-05 | EW02 §ARPG | EW-081 |
| A-06 | EW02 §ARPG | EW-090, EW-091 |
| A-07 | EW02 §ARPG | EW-013 |
| A-08 | EW02 §ARPG | EW-075 |
| A-09 | EW02 §ARPG | EW-083 |
| A-10 | EW02 §ARPG | EW-110 |
| A-11 | EW02 §ARPG | EW-100 |
| A-12 | EW02 §ARPG | EW-001, EW-002 |
| A-13 | EW02 §ARPG | EW-005, EW-006, EW-007 |
| G-01 | EW02 §GP | EW-030 |
| G-02 | EW02 §GP | EW-020, EW-032 |
| G-03 | EW02 §GP | EW-084 |
| G-04 | EW02 §GP | EW-074 |
| G-05 | EW02 §GP | EW-003 |
| G-06 | EW02 §GP | EW-004 |
| G-07 | EW02 §GP | EW-092 |
| R-01 | EW02 §RC | EW-001 |
| R-02 | EW02 §RC | EW-070 |
| R-03 | EW02 §RC | EW-072 |
| R-04 | EW02 §RC | EW-060 |
| R-05 | EW02 §RC | EW-061 |
| R-06 | EW02 §RC | EW-074 |
| R-07 | EW02 §RC | EW-005 |
| R-08 | EW02 §RC | EW-062 |
| R-09 | EW02 §RC | EW-075 |
| R-10 | EW02 §RC | EW-013 |
| R-11 | EW02 §RC | EW-030 |
| R-12 | EW02 §RC | EW-032 |
| R-13 | EW02 §RC | EW-006 |
| R-14 | EW02 §RC | EW-043 |
| R-15 | EW02 §RC | EW-064 |
| R-16 | EW02 §RC | EW-063 |
| EW-GP-1 | GP ESKİ liste GP08 §7 | EW-030, EW-031 |
| EW-GP-2 | GP ESKİ liste GP08 §7 | EW-084 |
| EW-GP-3 | GP ESKİ liste GP08 §7 | EW-074 |
| EW-GP-4 | GP ESKİ liste GP08 §7 | EW-003 |
| EW-GP-5 | GP ESKİ liste GP08 §7 | EW-004 |
| EW-GP-01 | GP YENİ liste GP16 (+ GP05 §8) | EW-010, EW-011 |
| EW-GP-02 | GP YENİ liste GP16 (+ GP05 §8) | EW-014 |
| EW-GP-03 | GP YENİ liste GP16 (+ GP05 §8) | EW-015 |
| EW-GP-04 | GP YENİ liste GP16 (+ GP05 §8) | EW-090 |
| EW-GP-05 | GP YENİ liste GP16 (+ GP05 §8) | EW-016 |
| EW-GP-06 | GP YENİ liste GP16 (+ GP05 §8) | EW-040, EW-041, EW-042 |
| EW-GP-07 | GP YENİ liste GP16 (+ GP05 §8) | EW-018 |
| EW-GP-08 | GP YENİ liste GP16 (+ GP05 §8) | EW-017 |
| EW-GP-09 | GP YENİ liste GP16 (+ GP05 §8) | EW-019 |
| EW-R01 | RagnaCook RC12 §11.1 | EW-001, EW-002 |
| EW-R02 | RagnaCook RC12 §11.1 | EW-001 |
| EW-R03 | RagnaCook RC12 §11.1 | EW-005 |
| EW-R04 | RagnaCook RC12 §11.1 | EW-010, EW-012 |
| EW-R05 | RagnaCook RC12 §11.1 | EW-012 |
| EW-R06 | RagnaCook RC12 §11.1 | EW-050 |
| EW-R07 | RagnaCook RC12 §11.1 | EW-051 |
| EW-R08 | RagnaCook RC12 §11.1 | EW-052 |
| EW-R09 | RagnaCook RC12 §11.1 | EW-053 |
| EW-R10 | RagnaCook RC12 §11.1 | EW-054 |
| EW-R11 | RagnaCook RC12 §11.1 | EW-055 |
| EW-R12 | RagnaCook RC12 §11.1 | EW-060 |
| EW-R13 | RagnaCook RC12 §11.1 | EW-061 |
| EW-R14 | RagnaCook RC12 §11.1 | EW-020 |
| EW-R15 | RagnaCook RC12 §11.1 | EW-040, EW-042, EW-043 |
| EW-R16 | RagnaCook RC12 §11.1 | EW-030 |
| EW-R17 | RagnaCook RC12 §11.1 | EW-013 |
| EW-R18 | RagnaCook RC12 §11.1 | EW-070, EW-071, EW-074 |
| EW-R19 | RagnaCook RC12 §11.1 | EW-075 |
| EW-R20 | RagnaCook RC12 §11.1 | EW-006 |
| EW-R21 | RagnaCook RC12 §11.1 | EW-111 |
| EW-R22 | RagnaCook RC12 §11.1 | EW-100, EW-101, EW-102 |


## 7. İhtiyaç sırası (oyunlar açısından, tarih yok)

Oyunun ilk kullandığı ana göre. Ayrıntılı sürüm sırası 08'de.

```
GP YZ-0 öncesi (GP16: "hepsi YZ-0'dan önce")         RC Stil kilidi + Demo D1 (★ yol)            ARPG F0 (ön-prodüksiyon, VARSAYIM T4)
  G0: EW-010/011 automation, EW-014 metrik oku           IS-03: EW-060/061/063 kare + ölçüm           EW-071 iskelet+anim (AT EW-1)
      EW-019 senaryo yerleşimi (isteğe bağlı, S11)       IS-10: EW-001/002/005 veri + ID              EW-072 malzeme varyantı (EW-2)
  G1: EW-015 vislog, EW-016 GDT, EW-090 navmesh,         IS-13: EW-070/072/073 import + MI            EW-082 GAS (EW-5)
      EW-040/041/042 perf + scope + -game                EK-03/IS-21: EW-010/012 kural testleri       EW-080 StateTree (EW-3)
  G2: EW-017 StateTree izi                               LOC-1…4: EW-050…055 (EW-055 tek "Yok" P0)    EW-081 AnimBP (EW-4)
  G3: EW-018 soak                                        IS-30: EW-074 yer tutucu ★                 ARPG Prototip (F1)
GP Demo (GP09)                                           IS-31/33: EW-042/043, EW-030/032             EW-001/002/004 veri + sim, EW-091/090 oda+nav
  GI-07: EW-030/031 ağ                                   IS-32: EW-013 bot, EW-004 denge              EW-013/014 bot + ölçüm
  GI-29: EW-004 sim köprüsü                              IS-34: EW-006 doğrulama, EW-111 tuş        ARPG Dikey dilim → Demo → 1.0
                                                         IS-35: EW-075 beyan                          EW-083 Niagara, EW-075 beyan, EW-007, EW-110, EW-100
                                                       RC EA / 1.0: EW-020, EW-064, EW-100…102, EW-071 (Garmr)
```

Ortak yükler (birden çok oyunun ilk anında): EW-001 veri, EW-013/014 bot + ölçüm, EW-042 paketli FPS, EW-070 import, EW-090 navmesh.

## 8. Açık noktalar (SORULAR_2'ye taşındı)

- RC12 §11.1 EW-R20 "Kısmen / B11 yok" diyor; kodda `validate` var (ImportValidationTools.cpp:235). RC belgesi eski → S2-04.
- GP16 EW-GP-01 `automation_run`'ı "eksik" sayıyor; araç var (editör içi). Eksik kısım başsız koşu + XML ayrıştırma (EW-011) → S2-04.
- EW-080 StateTree yazma: GP YZ-0 StateTree geçişini elle (C++) yapıyor; GP belgeleri yazma aracı istemiyor. GP'yi bu ihtiyaca ekleme TAHMİN → S2-05.
- EW-032 iki makine / Steam testi araçla çözülmez (TAHMİN); elle kalır.
