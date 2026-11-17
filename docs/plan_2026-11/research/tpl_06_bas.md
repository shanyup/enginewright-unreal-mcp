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

