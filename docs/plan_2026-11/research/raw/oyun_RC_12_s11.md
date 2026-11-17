# RagnaCook 12 §11-11.1 (satır 214-264)
## 11. Enginewright eksikleri (RagnaCook için)

Kaynak: `enginewright/docs/EKSIKLER_VE_ONCELIK.md` (17.11 okundu). Sıra RagnaCook ihtiyacına göre.

| Sıra | Madde | RagnaCook'ta kullanım | Olmazsa |
|---|---|---|---|
| 1 | A1 katı `set_rows` | 28 yemek, 48 kart, 48 kilit aktarımı | Yanlış alan sessizce boş kalır |
| 2 | B9 editör açıkken mesh/doku/ses içe aktarma | 155 asset + 186 ses dosyası | Commandlet döngüsü, saat kaybı |
| 3 | B3 oyun kamerası karesi | G2, G3, G6; altın kare | Ölçüm elle |
| 4 | E5 okunabilirlik (ekranda px) | Yemek ≥ 28 px | Geçici spawn betiği |
| 5 | B5 güvenli yeniden adlandırma / taşıma | Yer tutucuları `_Kullanilmayan`'a taşıma | Kırık referans |
| 6 | E6 ID bütünlük kontrolü | V1–V3'ün UE tarafı | İkili dosyada grep |
| 7 | E8 önce/sonra kolaj | Q2, Talha onayı | Elle PIL |
| 8 | E14 asset kaynağı / AI beyanı | `Asset_Kaydi.csv` | Steam beyanı elle |
| 9 | E2 bot playtest | 09 bot testi (sipariş akışı) | Yalnız Python sim |
| 10 | B2 PIE kontrolü (N istemci) | Co-op ölçek testi | 2 makine gerekir |
| 11 | B11 toplu doğrulama | Demo kapısı | Hata oyunda çıkar |

ARPG EW-1 (iskeletli import) yalnız Garmr ve karakterler için gerekir; EW-3/4/5 (BT, AnimBP, GAS) RagnaCook'ta gerekmez.

### 11.1 RagnaCook → Enginewright ihtiyaç listesi (18.11, t_6fc5c13a; bütün kararlarla)

Yukarıdaki tablo 17.11 hâliyle durur. Bu tablo onun yerine geçer (değişti → Q4). Kaynak: Enginewright kodundaki araç kayıtları (`Plugins/Enginewright/Source`, depo 348c9bc, 17.11; **yalnız okundu**). "Var" = araç bugün kayıtlı; davranış RagnaCook'ta denenmedi. Öncelik: **P0** demo kapısından önce şart · **P1** demo içinde çok saat kazandırır · **P2** EA ve sonrası.

| # | İhtiyaç | IS | Öncelik | Mevcut araç | Eksik / not |
|---|---|---|---|---|---|
| EW-R01 | DataTable kart aktarımı: 48 kart + 4 lanet + 21 sunak, `veri_kart.py` → `DT_Cards` (katı alan denetimi, hepsi ya da hiç) | IS-10, IS-21, IS-48, IS-62 | P0 | **Var:** `datatable_set_rows` (bilinmeyen alan = hata, satır hatasında hiç yazmaz), `datatable_create`, `datatable_get`, `datatable_remove_rows`, `datatable_rename_row` | CSV/JSON'dan toplu upsert + fark raporu yok (EKSİK E17); betik tarafında JSON üretilir. `CIKTI` aşamalı satır (C05) aktarılmaz |
| EW-R02 | Kilit + kademe aktarımı: `DT_Unlocks` 58 satır = 48 kilit + 10 kademe (07 §5) | IS-23, IS-49 | P0 | **Var:** `datatable_set_rows` | — |
| EW-R03 | ID bütünlüğü: kart / tarif / malzeme / kilit ID'leri DataTable ve BP arasında | IS-10, IS-34 (V1–V3) | P0 | **Var:** `id_audit` | BP içindeki sabit FName'ler kapsanıyor mu: denenmeli |
| EW-R04 | Kural kancası testleri (EK-03): her kural kartı için birim test + `RC_RunSelfTests` | EK-03, IS-21, IS-48 | P0 | **Var:** `automation_list`, `automation_run`; `console_command` + `log_wait` (öz-test komutu ve log iddiası) | Kart başına senaryo (kartı ver → olay tetikle → sonuç oku) için `pie_input` + `pie_actors` birleşimi; hazır kalıp yok |
| EW-R05 | C49 Kazan Atışı ve fırlatma testleri: PIE'de at → kap → doğranmış kimlik + pişme ×1,25 | IS-62, 09 T-06b | P2 | **Kısmen:** `pie_start`, `pie_input`, `pie_actors`, `log_wait` | Fırlatma hedefleme (yön + güç) girdi dizisi yok |
| EW-R06 | String Table toplu yazma (kaynak EN + TR) | LOC-1, LOC-2 | P0 | **Var:** `stringtable_set`, `stringtable_get`, `stringtable_list` | CSV'den yüklenen tablolar burada salt okunur (araç notu) → RagnaCook asset tablo kullanmalı |
| EW-R07 | Çeviri eksik / bozuk kontrolü (eksik anahtar, `{x}` argümanı, çevrilmemiş) | LOC-2, 20 §5.4 | P0 | **Var:** `stringtable_compare` | — |
| EW-R08 | Lokalizasyon gather / export / import / compile (.po) | LOC-2, LOC-5, LOC-8, LOC-9 | P0 | **Var:** `loc_gather`, `loc_gather_status` | — |
| EW-R09 | Glif testi: CJK (F-1), Tay (F-2), Arap (F-3) fontları eksik karakter | LOC-3, LOC-6, LOC-6b | P0 (L1) · P2 (L2–L6) | **Var:** `font_coverage` (tofu kontrolü) | Arapça bağlı harf şekillendirme görsel kontrolü yalnız ekran görüntüsüyle |
| EW-R10 | Dil başına ekran görüntüsü: her dilde bütün UI ekranları | LOC-4, 09 T-16 | P0 | **Var:** `loc_culture` (dil değiştir) + `widget_render` (PIE'siz widget PNG) + `screenshot` (PIE, UI dahil); `ui_open`, `ui_state` | Tek çağrıda "her dil × her ekran" döngüsü yok (ajan döngüsüyle yapılır) |
| EW-R11 | Ekran taşma testi: kesilen / taşan metin sayısı | LOC-4, 09 T-16 | P0 | **Yok.** Kodda taşma ölçen araç bulunmadı | EKSİK: widget içindeki TextBlock istenen boyut > ayrılan alan raporu (`text_overflow_audit`). Olmazsa: ekran görüntüsü + göz |
| EW-R12 | Oyun kamerasından kare alma (altın kare, G2/G3/G6, trailer kareleri) | IS-03, IS-32, IS-53 | P1 | **Var:** `screenshot` (oyuncu görüşü, UI dahil), `screenshot_ab` (önce / sonra yan yana), `image_compose`, `image_stats` | — (17.11 tablosundaki B3 kapandı) |
| EW-R13 | Okunabilirlik: yemek ≥ 28 px | IS-03, IS-13 | P1 | **Var:** `screen_coverage`, `class_measure`, `actor_bounds` | — (E5 kapandı) |
| EW-R14 | Kayıt göç testi: demo kaydı → EA, `DemoImportState` (KS-8), sürümlü kayıt KS-1…KS-8 | IS-54, 07 §11, 09 T-15 | P1 | **Var:** `savegame_get`, `savegame_set` (alan doğrulamalı, diskten geri okur) | Demo ve EA build'i arası (iki ayrı paket) akışı elle; `game_run` ile iki ayrı exe koşulabilir |
| EW-R15 | Paket / Steam Deck performansı: paketli build FPS, 800p | IS-31, IS-52 | P1 | **Kısmen:** `perf_capture`, `perf_scene`, `perf_gpu_profile` (PIE / editör); `game_run` (paketli exe başlatır, log iddiası) | Paketli oyunda FPS örnekleme yok (EKSİK E22). Paketleme / cook aracı yok (EKSİK B17). Deck cihazı ölçümü elle |
| EW-R16 | Co-op testi: 2–4 istemci, kötü ağ (T-09…T-11) | IS-33 | P1 | **Var:** `pie_start` (N istemci), `pie_net_emulation`, `pie_state`, `pie_travel` | Host kopması senaryosu elle |
| EW-R17 | Bot playtest (sipariş akışı, kuyruk) | IS-32, IS-52 | P2 | **Kısmen:** `pie_input`, `pie_walk_to` | Senaryo oynatıcı + oyun durumu iddiası (EKSİK E2) |
| EW-R18 | Asset / ses içe aktarma (155 asset + 186 ses) | IS-13, IS-25, IS-30 (taşıma), IS-41, IS-51, IS-63 | P1 | **Var:** `asset_import`; `asset_rename` (yer tutucu taşıma) | İskeletli FBX animasyon (EKSİK E23) yalnız Garmr |
| EW-R19 | Asset kaynağı / AI beyanı (Steam) | IS-35, IS-53 | P1 | **Yok** (EKSİK E14) | `Asset_Kaydi.csv` elle |
| EW-R20 | Toplu doğrulama: BP derle, kırık referans (demo kapısı) | IS-34 | P0 | **Kısmen:** `blueprint_compile`, `asset_references`, `build_status` | Tek çağrıda bütün proje doğrulaması yok (EKSİK B11) |
| EW-R21 | Sabit tuş denetimi + girdi istemleri (ekip tuş kuralı) | IS-34 | P2 | **Var:** `key_audit`, `ui_input_audit`, `ui_prompts` | — |
| EW-R22 | Trailer / Reel çekimi | IS-53 | P2 | **Kısmen:** `mrq_render`, `mrq_status` | Sequencer kurma (E29), oynanış kaydı (E32) yok |

**En kısa Enginewright sırası (RagnaCook demo için):** EW-R11 taşma testi (tek **yok** P0) → EW-R15 paketli FPS (E22) → EW-R20 toplu doğrulama (B11) → EW-R17 bot playtest (E2). Diğer P0'ların aracı var; RagnaCook'ta ilk kullanım bir deneme koşusu ister.

