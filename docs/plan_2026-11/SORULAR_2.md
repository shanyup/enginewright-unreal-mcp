# SORULAR_2 — Talha kararları (yol haritası v2)

Kart: `t_ae3a845a` · 18.11.2026. Her soruda **önerim ilk sırada** + gerekçe.
Kapsam dışı (isim, SEO, yıldız/duyuru, Fab, Discord, lisans, Pro katman → `SORULAR.md` S-0…S-10) burada sorulmaz.
Cevap biçimi: "S2-01: A" yeterli.

---

## Sıra ve kapsam

**S2-01. Sürüm sırası 0.4.1 (RC) → 0.5 (GP YZ-0) → 0.6 (Demo) → 0.7–0.9 (ARPG) olsun mu?** (08 §1.1)
- **A (öneri):** Evet, bu sıra. Gerekçe: 0.4.1 küçük (≈ 35 sa TAHMİN) ve RC aktif; GP kuralı "EW-GP-01…08 YZ-0'dan önce zorunlu" (GP16 kural 1); ARPG T4 varsayımı RC demosundan sonra.
- B: 0.5 (GP) önce, 0.4.1 sonra. GP YZ-0 hemen başlayacaksa.
- C: 0.4.1 + 0.6'nın RC kısmı birlikte (RC Demo önce), GP 0.5 sonra. RC demo tarihi sıkışırsa.

**S2-02. GP "araç önce" kuralı: GP YZ-0 kodu yokken araçların GP kabulü nasıl olsun?** (08 §3 risk 1)
- **A (öneri):** Araç Enginewright test projesinde kabul edilir (sahte `metrics.json`, küçük StateTree), GP kanıtı YZ-0'ın ilk haftasında. Gerekçe: GP16 kural 1'i bozmaz; tavuk-yumurta kırılır.
- B: Önce GP'de YZ-0 iskeleti (test modülü, `metrics.json` biçimi), sonra araçlar. GP kuralını değiştirir.
- C: G0 araçları (EW-011, EW-014) önce, G1–G3 araçları YZ-0 ile paralel.

**S2-07. 0.6 büyük (≈ 52 sa TAHMİN). Bölünsün mü?** (08 §4 risk 3)
- **A (öneri):** RC Demo zamanına göre karar: yaklaşırsa 0.6a (`package_build`, `pie_scenario`, `bot_run`, beyan) / 0.6b (gerisi). Gerekçe: 0.6a RC IS-31/35'i açar; gerisi beklenebilir.
- B: Hep tek sürüm.
- C: Şimdiden ikiye böl.

**S2-10. 03_Yol_Haritasi'nda olup üç oyunun güncel listesinde karşılığı olmayan araçlar ne olsun?** (`scene_snapshot/diff`, `world_inventory`, `log_analyze`, `level_flow_test`, `travel_test_suite`, `mutable_tools`; 08 §9, §11)
- **A (öneri):** "Sonrası" listesinde kalsın, oyun isterse sürüme alınsın. Gerekçe: ilke "oyun ihtiyacı önce"; 06'da hiçbirine ihtiyaç satırı yok.
- B: Genel değerli olanlar (`scene_diff`, `log_analyze`) 1.0 cilasına girsin.
- C: Listeden tamamen çıksın.

**S2-13. `scenario_place` (EW-019) 0.5'e girsin mi?** (GP SORULAR S11, GP16 EW-GP-09 "isteğe bağlı")
- **A (öneri):** Evet, 0.5'in sonunda (5 sa TAHMİN). Gerekçe: 0.6'da GP GI-03 yerleşim seti modu bunun üstüne kurulur; S1–S3 haritalarını elle kurmak tekrar iştir.
- B: 0.6'ya kaydır (GI-03 ile birlikte).
- C: Hiç yapma; GP senaryoları elle.

## ARPG varsayımları (T1 / T3 / T4 hâlâ AÇIK)

**S2-12. 06/08 şu varsayımla yazıldı: T1 = UE 5.8, T3 = 4 akt, T4 = şimdi yalnız ön-prodüksiyon (tam prodüksiyon RC demosundan sonra). Onay?** (06 §4, AT §2, §12.4)
- **A (öneri):** Onayla (üçü de ARPG belgesinin kendi önerisi). Gerekçe: 0.7–0.9 sürüm gerekçesi buna dayanıyor; ARPG kararı netleşince yalnız sıra kayar, araçlar aynı (RC/GP de kullanır).
- B: T1 açık kalsın; 0.7–0.9 "ARPG kararından sonra" diye işaretlensin, RC/GP payı (EW-071 retarget, EW-084, EW-100) öne çekilsin.
- C: T1 = Unity → 0.7–0.9 küçülür: yalnız RC/GP payı kalır (iskelet import, fizik testi, Sequencer).

## Numara ve oyun belgeleri

**S2-03. GP'deki iki "EW-GP" listesi (GP08 eski EW-GP-1…5, GP16 yeni EW-GP-01…09) oyun belgelerinde nasıl düzelsin?** (06 §3)
- **A (öneri):** GP belgelerinde EW-xxx'e geç: GP16 ve GP08 tablolarına "Enginewright ID" sütunu eklensin (ayrı GP kartı; bu kart oyun reposuna yazmaz). Gerekçe: tek numara 06'da; iki taraf aynı ID'yi görür.
- B: GP08 eski listeye "EW-GP-E1…E5" diye yeni ad ver, GP16 aynı kalsın.
- C: Dokunma; 06'daki eşleme yeter.

**S2-04. Oyun belgelerindeki eski durum notları düzeltilsin mi?** (07 §6: RC12 EW-R20 "B11 yok" → `validate` var; GP16 EW-GP-01 `automation_run` "eksik" → editör içinde var)
- **A (öneri):** Evet, ilgili oyunun bir sonraki belge kartında "07 §6'ya göre" notuyla. Gerekçe: oyun ajanları yanlış "yok" görüp gereksiz iş açabilir.
- B: Hayır; 07 tek doğru kaynak.

**S2-05. StateTree yazma aracı (EW-080) kimin için?** — ✅ **KAPANDI (genel araç, Talha 18.11)**
- **Karar:** StateTree yazma aracı genel araçtır; her oyun kullanabilir. Seri 0.8.x "Oynanış yazarlığı (GAS, StateTree, BT, AnimBP)".
- İç not: GP StateTree kullanıyor (`Game/GunpointParole/docs/YZ_v2/02_Karar_Sistemi_Karsilastirma.md` §3); şu an yalnız `statetree_debug_dump` (EW-017, 0.5.x) istedi. İlk yazma kullanıcısı ARPG F0→F1 (06 iç eşleme).
- Eski seçenekler (A: yalnız ARPG / B: GP de sahip) genel MCP kuralıyla geçersiz: araç sahibi oyun değil, yetenektir.

## Araç tasarımı

**S2-06. Oyuna özgü araç adları genelleşsin mi?** (`ai_metrics_read` → `metrics_read`; 09 §1.2 S10) — ✅ **KAPANDI = A (Talha 18.11)**
- **A (seçildi):** Genel ad + oyunun dosya biçimi parametre (`metrics_read` `format`). Araç adı, açıklaması ve sürüm başlıklarında oyun adı yok; oyun bağlantısı yalnız "(iç)" alanlarda. 08 §3, 09 S10 güncellendi.
- B: (reddedildi) GP16 adları aynen kalsın.

**S2-11. 09'daki "ÖNERİ" kuralları (S2, S4, S7–S10, H1–H6, T1–T5, belge şablonu) yeni araçlar için zorunlu olsun mu?**
- **A (öneri):** Evet, 0.4.1'den itibaren yeni araçlarda zorunlu; eski 129 araca dokunulmaz (yalnız T-xx borç listesi). Gerekçe: dondurma (Y9) öncesi tutarlılık ucuz; sonradan değiştirmek kırıcı.
- B: Yalnız 0.9 dondurma adayında uygula.
- C: Yalnız öneri olarak kalsın.

**S2-14. Oyun kanıt kayıtları nerede tutulsun?** (09 §3.2 T3)
- **A (öneri):** Enginewright reposunda `docs/oyun_kosulari/<OYUN>_<sürüm>.md`. Gerekçe: araç sürümüyle birlikte durur; oyun repoları kirlenmez. Oyun adı/sırrı içerirse dosya yayında `.gitignore`'a alınabilir (yayın işi).
- B: Her oyunun kendi reposunda (`Docs/Enginewright_Kosulari.md`).

## Cila şeridi

**S2-08. CI nasıl olsun?** (09 §3.4)
- **A (öneri):** Şimdi A (statik CI: envanter + kapsam betikleri, GitHub'da), 1.0 öncesi C (elle `run_all.py` + RELIABILITY turu, sürüm kontrol listesinde). Gerekçe: GitHub makinelerinde Unreal yok; kendi koşucu (B) PC'nin açık kalmasını ve güvenlik ayarını ister. Not: GitHub'a iş akışı eklemek bu kartın işi değil (push yok).
- B: Self-hosted koşucu (Talha PC) ile derleme + canlı test.
- C: Hiç CI yok, yalnız elle tur.

**S2-09. Çoklu UE sürümü (Y7) hangi sürümler?** (README "Supported versions": yalnız 5.8 Win64; kaynakta sürüm koruması 0, T-11)
- **A (öneri):** 5.8 + bir önceki (5.7); Mac/Linux 1.0 sonrası. Gerekçe: üç oyun 5.8'de (ARPG VARSAYIM); RC eski etiket 5.6'da kaldı ama aktif değil. İki sürüm derleme yükünü sınırlı tutar.
- B: 5.6 + 5.7 + 5.8.
- C: Yalnız 5.8 (1.0'a kadar).

---

Toplam: **14 soru** (S2-01 … S2-14).


## Talha kararları (18.11.2026)

| # | Karar |
|---|---|
| S2-01 | A — Sıra: 0.4.1 (RC) → 0.5 (GP YZ-0) → 0.6 (Demo) → 0.7–0.9 (ARPG). |
| S2-02 | A — Araç Enginewright test projesinde kabul; GP kanıtı YZ-0'ın ilk haftasında. |
| S2-12 | A — ARPG varsayımları onaylı: T1 UE 5.8, T3 4 akt, T4 şimdi ön-prodüksiyon (tam prodüksiyon RC demosundan sonra). |
| Not | Talha: "İlerlemeyi tamamen ara versiyonlar hâlinde yaz; iş yaparken ona göre yapacağız." → 08 ara sürümlere bölünecek (S2-07 bu kapsamda). |

---

## Ek soru (kart `t_035891ab`, 10_Ara_Surumler)

**S2-15. Ara sürüm numaraları (0.4.1 … 1.0.0, 55 adım) dışarıya nasıl görünsün?** (10 §1, §8)
- **A (öneri):** Her ara sürüm yerel git etiketi + CHANGELOG'da başlık; GitHub sürümü (zip) yalnız seri sonlarında (0.4.4, 0.5.6, 0.6.6, 0.7.5, 0.8.7, 0.9.16, 1.0.0). Gerekçe: iş sırası adım adım izlenir; kullanıcıya 55 zip yerine 7 sürüm gider. Etiket/push kararı yayın işiyle birlikte (bu kart push yapmaz).
- B: Her ara sürüm GitHub sürümü (zip). Sık sürüm "canlı proje" görüntüsü verir ama her biri RELIABILITY + paket ister.
- C: Ara sürümler yalnız iç plan (etiket yok); dışarıda yalnız 0.5, 0.6 … 1.0.

Talha 18.11: S2-05, S2-06 genel MCP kuralıyla kapandı.

## Talha kararları — kalan 10 soru (18.11.2026)

Hepsi **A** (önerilen seçenek). Belgeler zaten önerilen seçenekle yazılmıştı; "S2-xx cevabına bağlı" işaretleri artık kesin karar sayılır.

| # | Karar |
|---|---|
| S2-03 | A — Oyun belgelerine "Enginewright ID" (EW-xxx) sütunu; ayrı oyun belge kartında (bu repo oyun reposuna yazmaz). |
| S2-04 | A — Oyun belgelerindeki eski "araç yok" notları, ilgili oyunun sonraki belge kartında 07 §6'ya göre düzeltilir. |
| S2-07 | A — Ara sürümlerle kapandı (10). |
| S2-08 | A — Şimdi statik CI (envanter + kapsam betikleri); 1.0 öncesi elle `run_all.py` + RELIABILITY turu. GitHub'a şimdi bir şey eklenmez. |
| S2-09 | A — UE 5.8 + 5.7; Mac/Linux 1.0 sonrası. |
| S2-10 | A — 03_Yol_Haritasi'nın sahipsiz araçları "Sonrası" listesinde; bir oyun isterse sürüme alınır. |
| S2-11 | A — 09 kuralları 0.4.1'den itibaren yeni araçlarda zorunlu; eski araçlar yalnız T-xx borç listesiyle. |
| S2-13 | A — `scenario_place` 0.5.x sonunda. |
| S2-14 | A — Oyun kanıt kayıtları `docs/oyun_kosulari/` (klasör ilk kanıtta açılır). |
| S2-15 | A — Her ara sürüm yerel git etiketi + CHANGELOG başlığı; GitHub sürümü yalnız seri sonlarında; push kararı yayın işinde. |

SORULAR_2'de açık soru kalmadı.
