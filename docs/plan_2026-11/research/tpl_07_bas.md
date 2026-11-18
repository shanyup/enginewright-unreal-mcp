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

