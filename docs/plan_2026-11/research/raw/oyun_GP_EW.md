# GP GDD_v2 §16
## 16. YZ-0 öncesi Enginewright araçları (EW-GP-01…09) — 17.11, K-131

Talha kararı (S10 = 2, öneriden farklı): **dokuz aracın hepsi YZ-0'dan önce yazılır.** Kaynak: [YZ_v2/05 §8](YZ_v2/05_Otomatik_YZ_Test_Sistemi.md). Araçlar ayrı repo işidir (`enginewright`); bu belge yalnız listeler. "İlk kullanıldığı kapı" sütunu 05'teki eski öncelik sütunudur; artık sıra değil, aracın ilk işe yaradığı yeri gösterir.

Mevcut EW araçları: `game_run`, `smoke`, `log_wait`, `perf_capture` (E19), `actor_spawn`.

| ID | Araç | Ne yapar | Neden | İlk kullanıldığı kapı |
|---|---|---|---|---|
| **EW-GP-01** | `automation_run` | `Automation RunTests <filtre>` başsız koşar, sonuç XML/json'u ayrıştırır (geçti/kaldı/süre) | Senaryo koşusu tek çağrı | G0 |
| **EW-GP-02** | `ai_metrics_read` | `Saved/AITest/<son>/metrics.json` + `ozet.md`'yi okur, kapıya göre özet döner | Ajan ham json okumadan karar verir | G0 |
| **EW-GP-03** | `vislog_extract` | `.bvlog`'tan belirli aktörün belirli zaman aralığını metin/PNG'ye döker | Kırmızı olayın nedeni | G1 |
| **EW-GP-04** | `nav_build_check` | NavMesh build + kopuk ada / ulaşılamaz siper noktası raporu | M1 takılma | G1 |
| **EW-GP-05** | `gdt_snapshot` | Gameplay Debugger kategorisini seçili YZ için açar, ekran görüntüsü + metin döker | Utility puanı/rol kanıtı | G1 |
| **EW-GP-06** | `perf_capture` + `scope` | E19'a `GPAI_*` stat grubu filtresi ve `-game` modu (E22) | M9 host maliyeti | G1 |
| **EW-GP-07** | `soak_run` | Paket `-game -nullrhi` uzun koşu; 60 sn'de bir bellek/ms/takılma satırı; çökme yakalama | 30 dk soak | G3 |
| **EW-GP-08** | `statetree_debug_dump` | StateTree hata ayıklayıcı izini metne çevirir | M12 titreme nedeni | G2 |
| **EW-GP-09** | `scenario_place` | Senaryo haritasına YZ/bot/siper Smart Object'ini veri listesinden yerleştirir | Talha'nın harita kurma saati | G0 (isteğe bağlı) |

- **Sonuç:** YZ-0 kartı, dokuz araç enginewright'ta çalışır olmadan açılmaz (§13 "EW Önhazırlık" satırı).
- **Açık:** EW kartının ajan saati ve "isteğe bağlı" EW-GP-09'un bu kurala dahil olup olmadığı SORULAR S11'de.

---


# GP YZ_v2/05 §8
## 8. Enginewright'a gereken araçlar (EW-GP-xx)

Mevcut EW araçları: `game_run`, `smoke`, `log_wait`, `perf_capture` (E19), `actor_spawn`. Eksikler:

| ID | Araç | Ne yapar | Neden | Öncelik |
|---|---|---|---|---|
| **EW-GP-01** | `automation_run` | `Automation RunTests <filtre>` başsız koşar, sonuç XML/json'u ayrıştırır (geçti/kaldı/süre) | Senaryo koşusunu ajan tek çağrıyla yapar (EKSIKLER C2 "Automation test") | G0 |
| **EW-GP-02** | `ai_metrics_read` | `Saved/AITest/<son>/metrics.json` + ozet.md'yi okur, kapıya göre özet döner | Ajan ham json'u bağlama okumadan karar verir | G0 |
| **EW-GP-03** | `vislog_extract` | `.bvlog`'tan belirli aktörün belirli zaman aralığını metin/PNG'ye döker | Kırmızı olayın nedenini görmek (Visual Logger UI'sı ajan için okunmaz) | G1 |
| **EW-GP-04** | `nav_build_check` | NavMesh build + kopuk ada / ulaşılamaz siper noktası raporu | M1 takılmanın %'si navmesh hatasından gelir (EKSIKLER B15) | G1 |
| **EW-GP-05** | `gdt_snapshot` | Gameplay Debugger kategorisini seçili YZ için açar, ekran görüntüsü + metin döker | Utility puanı/rol görsel kanıtı | G1 |
| **EW-GP-06** | `perf_capture` + `scope` | E19'a `GPAI_*` stat grubu filtresi ve `-game` modu (E22) | M9 host maliyeti | G1 |
| **EW-GP-07** | `soak_run` | Paket `-game -nullrhi` uzun koşu; 60 sn'de bir bellek/ms/takılma satırı; çökme yakalama | 30 dk soak (G3) | G3 |
| **EW-GP-08** | `statetree_debug_dump` | StateTree hata ayıklayıcı izini (aktif durum geçmişi) metne çevirir | M12 titreme nedeni | G2 |
| **EW-GP-09** | `scenario_place` | Senaryo haritasına YZ/bot/siper Smart Object'i veri listesinden yerleştirir | Talha'nın harita kurma saatini düşürür | G0 (isteğe bağlı) |

EW araçları ayrı repo işidir (enginewright); bu belge yalnız listeler, kart açmaz.
# SORULAR S11
**S11. S10 = 2 kararının kapsamı: EW önhazırlık kartı nasıl açılsın?** (05 §8, GDD §16, K-131)
1. **EW-GP-01…08 YZ-0'dan önce zorunlu; EW-GP-09 (`scenario_place`, 05'te "isteğe bağlı") YZ-0 ile paralel; tek enginewright kartı, ajan saati kart açılırken tahmin edilir** (öneri — 09 yalnız Talha'nın harita saatini azaltır, ölçümü engellemez).
2. Dokuzu da istisnasız YZ-0'dan önce.
3. Her araç ayrı kart, sıra 01→09.

**S12. Fırsatçı Nişancı rolü (TLOU OpportunisticShooter) GP'de olsun mu?** (01 §4, GDD §6.1 [AÇIK])
1. **Ayrı rol yok; isabet jetonu modeli (K-123: herkes ateş eder, isabeti jeton kapılar) aynı işi görür. GDD §6.1 satırı "karşılandı → K-123" diye kapanır** (öneri — fazladan rol, M13 çakışma riskini artırır).
2. Ayrı rol: oyuncu başına 1 jetonsuz YZ, açıkta kalan oyuncuya fırsat atışı (isabetli) yapar; isabet jetonu tavanına sayılır.
3. YZ-2 his testinden sonra karar ver.
# Icerik_Sistemi/08 §7
## 7. Enginewright eksikleri (ARPG §12.3 EW-1..11'e ek, GP'ye özel)

ARPG'nin EW listesi (bkz. `ARPG/Docs/ARPG_Tasarim_Kararlari.md` §12) + GP ekleri:

| # | Eksik | Neden kritik |
|---|---|---|
| EW-GP-1 | Ağ / çok oyunculu test (listen-server, RPC) | tüm hasar/ekonomi server'da (K-11); PIE tek başına yeterli değil |
| EW-GP-2 | Fizik/ragdoll doğrulama (ganimet taşıma, sekmeler) | K-106 sekme deterministik; ragdoll eşikleri elitlerde |
| EW-GP-3 | Synty → kendi asset geçişi (K-70 yer tutucu) | production'da yer tutucu kalmasın |
| EW-GP-4 | DataAsset toplu yazım (`gp.py data`) okuma/doğrulama | K-48; 100+ asset elle yazılmaz |
| EW-GP-5 | Sim ↔ gerçek veri köprüsü (GI-29 denge turu) | ekonomi_sim ↔ parole_data senkronu |
