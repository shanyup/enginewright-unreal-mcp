# 13 — Duyuru ve yıldız planı v2

Tarih: 18.11.2026 · Kart `t_c6e43c15` · YALNIZ BELGE. **05_Duyuru_ve_Yildiz_Buyume_Plani.md'nin yerini alır** (05 silinmedi; üstüne not düşüldü).
Girdiler: 05, 03b (§0–§7), 10 (§1.1, §3, §4), 11, 12, SORULAR, SORULAR_2 (S2-15).
Web notları: [`research/yayin_hazirlik_kaynaklar_2026-11-18.md`](research/yayin_hazirlik_kaynaklar_2026-11-18.md).

Talha kararları (18.11): **büyük duyuru 0.6.x seri sonunda** (0.6.6: paketleme + senaryo + bot testi hazır). Demo "ajan kurdu, test etti, paketledi", 30–60 sn. Öncesinde seri sonlarında küçük paylaşım olabilir (§2 netleştirir). Oyunlar yalnız vitrin (§5).
"S3-xx" = [SORULAR_3](SORULAR_3.md) sorusu; cevap gelene kadar öneri (ilk seçenek) varsayılır. Takvim tarih değil **ara sürüm** bağlıdır; 0.4.1 → 0.6.6 toplam ≈ 166 sa TAHMİN (10 §1.1: 38 + 66 + 62).

---

## 0. Neden 05 değişti

| Konu | 05 (17.11) | 13 (18.11) | Dayanak |
|---|---|---|---|
| Büyük duyuru | v1.0 lansmanı | **0.6.6** | Talha 18.11 |
| Ana kanal | HN + Reddit + X aynı gün | **X'te tek video** ana kanal; Reddit iş akışı gönderisi; HN 1.0'a kalır | 03b §0 #2, §7 #1, #5 |
| Duyuru süresi | 1 gün + 90 gün takvim | Duyuru **3 haftaya yayılır** (her 3–4 günde yeni demo) | 03b §7 #2 |
| Mesaj | "strict, verified, undoable" | **"test and ship, not just edit"**; verified destek kanıtı | 11 §1, K3 |
| Dizinler | Duyuru günü | **Duyurudan önce** (0.5.6 ve 0.6.6 yayını) | 03b §7 #4, 12 §6 |
| Discord | Kendi sunucusu yok | Aynı (S3-09); başka sunucularda yalnız #showcase | SORULAR S-5 |
| Yıldız hedefi | 30 g 80–150, 90 g 300–600 | Üç senaryo, emsal eğrisiyle (§9) | 03b §1, §3 |

---

## 1. Duyuru öncesi hazırlık listesi (0.6.6'ya kadar)

Her satır bir ara sürüme bağlı. "Kart" = ayrı kanban kartı (bu kart açmaz). "Talha" = elle yapılacak iş.

| # | İş | Ara sürüm | Kim | Bitti ölçütü |
|---:|---|---|---|---|
| H1 | Reddit hesabı geçmişi: r/unrealengine, r/ClaudeAI, r/gamedev'de yardım yorumları (9:1 kılavuzu) | **Hemen → 0.6.6** (sürekli) | Talha | Duyurudan önce ≥ 4 hafta düzenli katkı (03b §5, Reddit satırları) |
| H2 | X hesabı seçimi + biyografi "Playwright for Unreal" + depo linki | 0.4.4'ten önce | Talha (S3-15) | Hesap hazır, sabit gönderi yok |
| H3 | Push öncesi iç belge ayrımı | 0.4.4 | Kart (S3-01) | Açık dalda Türkçe iç plan yok |
| H4 | **Depo adı → `enginewright-unreal-mcp`**, açıklama (ara metin), 20 topic | 0.4.4 yayını | Kart + Talha (12 §1, §2, §3) | 12 §1.4 adım 7 doğrulaması geçti |
| H5 | Küçük paylaşım 1 (§2) | 0.4.4 yayını | Talha | §2 |
| H6 | Unreal forumu (Epic Developer Community) "Work in Progress" kalıcı thread'i | 0.4.4 yayını | Talha | Thread açık; sonraki her yayında aynı thread'e yanıt (03b §5) |
| H7 | Glama kaydı + rozet; punkpeye/awesome-mcp-servers PR (Gaming, `🌊 🏠 🪟`) | 0.5.6 yayını | Talha (12 §6) | Glama sayfası açık; PR açık (birleşme beklenir) |
| H8 | Küçük paylaşım 2 (§2) | 0.5.6 yayını | Talha | §2 |
| H9 | GitHub Discussions aç (Q&A, Show and tell, Ideas) | 0.5.6 yayını | Talha | 3 kategori + karşılama gönderisi |
| H10 | Registry köprüsü kararı uygulanır, `server.json` taslağı | 0.6.0–0.6.2 | Kart (S3-03) | `mcp-publisher` kuru koşusu geçer |
| H11 | **Demo ham kayıtları** D1 (paket zinciri) | 0.6.1 kabulünden sonra | Talha + ajan | §4 D1 her sahne bir kez gerçek koşuda alındı |
| H12 | D2 ham kaydı (çok oyunculu + replikasyon) | 0.6.3 kabulünden sonra | Talha + ajan | §4 D2 |
| H13 | Sosyal görsel kareleri (`screenshot_tour`) + D3 ham kaydı | 0.6.5 | Talha + ajan (12 §5) | 1280×640 PNG + D3 |
| H14 | Video kurgu: D1 45 sn, D2 30 sn, D3 30 sn, D1 uzun sürüm 3–5 dk (YouTube) | 0.6.5 – 0.6.6 | Talha | MP4 1080p, altyazılı (sessiz izlenir) |
| H15 | README ilk ekran v2 + `docs/showcase.md` + sürüm notu | 0.6.6 | Kart (12 §4) | Yeni kullanıcı kuru testi: README'den 5 dk'da kurulum |
| H16 | Kuru kurulum testi: temiz UE 5.8 projesi, yalnız README ile | 0.6.6 | Talha (ya da ajan, yeni proje) | `doctor` temiz; D1 adımları tekrar edilebilir |
| H17 | Registry yayını + mcp.so / mcp.directory / MCP Market / LobeHub formları | 0.6.6 yayını (duyurudan 1–3 gün önce) | Talha (12 §6) | Formlar gönderildi; Registry'de görünür |
| H18 | Duyuru metinleri (§3 taslaklar) son hâli; her subreddit kuralı elle okunur | Duyuru − 1 gün | Talha | Metinler hazır, kurallar not edildi |

**Kapı:** 0.6.6 güvenilirlik turu (10 §4) geçmeden büyük duyuru yapılmaz. Turda kırmızı varsa duyuru 0.6.7 düzeltmesine kayar; **0.7.0'a başlamadan** duyuru yapılır (yeni seri dikkati dağıtır, TAHMİN).

---

## 2. Seri sonu küçük paylaşımları (0.4.4, 0.5.6)

Amaç: hesap geçmişi, ilk kullanıcı geri bildirimi, dizinlerin taranması. **Amaç yıldız patlaması değildir.** Büyük demo saklanır (§8 Y-6).

| | 0.4.4 küçük paylaşım | 0.5.6 küçük paylaşım |
|---|---|---|
| Ne | Depo adı değişti + 0.4 özeti | 0.5 "YZ test ve ölçüm paketi" özeti |
| Klip | 15–20 sn: ajan bir Widget'ta taşan metni `text_overflow_audit` ile bulur, düzeltir, `widget_render` ile gösterir | 20–25 sn: ajan bir bot koşusunda takılmayı ölçer, Visual Logger kaydını metne çevirip nedeni söyler |
| Kanallar | Unreal forumu WIP thread'i · X (tek gönderi) | Aynı WIP thread'ine yanıt · X (tek gönderi) · Glama + awesome PR (H7) |
| Kullanılmaz | Reddit, HN, Discord sunucuları, YouTube | Reddit, HN, YouTube (Discord #showcase yalnız S3-04 B seçilirse) |
| Cümle | 11 §1.1 ara cümle | 11 §1.1 ara cümle |
| Oyun klibi | Yok | En fazla 1 vitrin klibi (§5) |

Seçenekler ve gerekçe: **S3-04** (öneri A: tabloda yazılı hâli).

---

## 3. Büyük duyuru günü (0.6.6 yayını + 1–3 gün)

Gün: **Salı ya da Çarşamba** (TAHMİN: ABD sabahı X ve Reddit için en iyi pencere; 03b §5 X ve Reddit satırları). Saatler TSİ. Talha o gün **6 saat** yorumlara cevap verir (03b §5: HN/Reddit'te yazarın yorumlarda kalması).

| Sıra | Saat (TSİ) | Kanal | Ne | Başlık / ilk satır taslağı (EN) |
|---:|---|---|---|---|
| 0 | Gün −3…−1 | MCP dizinleri | H17: Registry yayını + formlar; awesome PR birleşti mi bakılır | (12 §2 kısa metin) |
| 1 | 15:00 | **YouTube** | D1 uzun sürüm (3–5 dk) **public** + D1 Shorts. Önce yüklenir ki diğer kanallar link verebilsin | *Your AI agent can now test and ship your Unreal game (Enginewright 0.6, MCP)* |
| 2 | 16:00 | **X** (ana kanal) | Tek gönderi + gömülü **D1 45 sn**. Linkler ilk yanıtta. Thread yok (03b §5 X satırı) | *My AI agent just set up a test level, played it in 2-player PIE, packaged the game and ran a bot through it. Unreal Engine MCP, free and MIT: Enginewright 0.6.* |
| 3 | 16:30 | **r/unrealengine** | "I built" self-post + D1 video + teknik ayrıntı; uygun flair (sidebar elle okunur) | *I built an MCP plugin that lets AI agents test and package Unreal games, not just edit them (free, MIT, works next to Epic's MCP)* |
| 4 | 17:00 | **Unreal forumu** | WIP thread'ine 0.6 yanıtı + yeni **Showcase** thread'i | *Enginewright 0.6: AI agents that test and ship your Unreal game* |
| 5 | 18:00 | **Discord toplulukları** | Yalnız #showcase / #self-promo kanalları; her sunucuda tek mesaj, farklı metin; önce sunucu kuralı okunur. Aday: Unreal Source, Claude/Anthropic topluluk sunucusu, MCP topluluk sunucusu (adları duyurudan önce doğrulanır) | Kısa: 11 §1 cümlesi + D1 GIF + link |
| 6 | Gün +1, 16:00 | **r/ClaudeAI** | Ürün değil **iş akışı**: "Claude Code paketli UE oyunumu test etti" (03b §5) | *Workflow: Claude Code packages my Unreal game, runs a bot through it and reports what broke* |
| 7 | Gün +2 | **r/mcp** | Tek açıklamalı gönderi, showcase etiketi | *Enginewright: an in-editor Unreal Engine MCP server focused on testing and packaging* |
| 8 | Gün +3 … +7 | **r/gamedev** | Haftalık thread ya da postmortem self-post: "AI ajanıyla UE'de QA otomasyonu: ne çalıştı, ne çalışmadı" (03b §5) | *What worked (and what didn't) automating Unreal QA with an AI agent* |
| 9 | Gün +4, +8, +12 | X + YouTube Shorts | **D2** (çok oyunculu), **D3** (görsel/dil QA), vitrin klibi (§5). Her 3–4 günde bir (03b §7 #2) | — |
| — | **1.0.0'da** | **HN (Show HN)** | 0.6.6'da **yok**. 1.0'da bir kez, Pazar 19:00 ET civarı (03b §5 HN satırı, §7 #5; S3-13) | *Show HN: Enginewright – Playwright for Unreal Engine (MCP)* |

Kural: Her kanala **farklı metin** yazılır; aynı metin kopyalanmaz (§8 Y-2). Her gönderide bağlantı açıkça "I built this" diye belirtilir.

---

## 4. Demo video senaryoları (saniye saniye)

Ortak kurallar:
- Hepsi **genel test projesinde** çekilir (oyun projesi değil). Vitrin oyun klibi yalnız §5 kuralıyla.
- Ekranda iki bölme: solda ajan terminali (Claude Code), sağda Unreal editörü / oyun. Hızlandırma ekranda "×4" etiketiyle gösterilir; kesme gizlenmez.
- Sessiz izlenir: her sahnede 1 satır altyazı (EN). Ses: hafif müzik ya da yok (S3-14).
- Her araç çağrısı ekranda gerçek adıyla görünür. Uydurma çıktı yok; her sahne gerçek bir koşudan alınır (H11–H13).

### D1 — "Set up, tested, shipped" (45 sn, ana video) · hazır: 0.6.1 sonrası, çekim H11

| Saniye | Görüntü | Araç / kanıt | Altyazı (EN) |
|---|---|---|---|
| 0–3 | Siyah zemin, başlık kartı | — | *Can an AI agent ship an Unreal game?* |
| 3–8 | Terminalde tek istem: "Set up a test arena with 3 enemies, test it in 2-player, package it and run a bot." | — | *One prompt in Claude Code.* |
| 8–15 | Editörde düzey kurulur: test haritası, düşmanlar ve senaryo noktaları yerleşir (×4) | `batch` + `scenario_place` (0.5.6) | *It sets up the level in one undoable batch.* |
| 15–23 | İki PIE penceresi yan yana (listen server + client), ajan girdi gönderir | `pie_start` (2 oyuncu), `pie_input`, `pie_scenario` (0.6.1) | *It plays it as two players.* |
| 23–31 | Paket ilerleme çubuğu (×8), editör kilitlenmeden çalışmaya devam eder | `package_build`, `package_status` (0.6.0) | *It packages a Win64 build in the background.* |
| 31–39 | Paketli oyun açılır, bot yürür, köşede FPS grafiği | `game_run`, `bot_run` (0.6.1), `game_perf_sample` (0.5.3) | *It runs the packaged game with a bot and measures FPS.* |
| 39–43 | Terminalde rapor: "1 issue: enemy health not replicated on client" + ekran görüntüsü | `screenshot_ab` / ajan özeti | *And reports what broke, with proof.* |
| 43–45 | Son kart: Enginewright işareti + `github.com/shanyup/enginewright-unreal-mcp` | — | *Enginewright. Free, MIT. Works next to Epic's MCP.* |

Not: 39–43'teki hata test projesine **kasıtlı konulur**. Videoda bu söylenir: altyazıda *"(planted bug)"*. Gizlemek güveni bozar (§8 Y-4).

### D2 — "It tests multiplayer" (30 sn) · hazır: 0.6.3 sonrası, çekim H12

| Saniye | Görüntü | Araç | Altyazı (EN) |
|---|---|---|---|
| 0–3 | Başlık | — | *Multiplayer bugs, found by your agent.* |
| 3–8 | İstem: "Start 2 players with 150 ms lag and check that score replicates." | — | *One prompt.* |
| 8–15 | İki PIE penceresi; ağ emülasyonu paneli 150 ms | `pie_start`, `pie_net_emulation` | *Listen server + client, 150 ms lag.* |
| 15–22 | Client'ta skor 0, server'da 3; ajan iddiayı koşar | `replication_check` (0.6.3) | *Score is 3 on the server, 0 on the client.* |
| 22–27 | Ajan değişkene `Replicated` ekler, tekrar dener: iki taraf 3 | `batch`, `replication_check` | *It fixes it and checks again.* |
| 27–30 | Son kart | — | *Enginewright · Unreal Engine MCP* |

### D3 — "It checks what the player sees" (30 sn) · hazır: 0.6.5 sonrası, çekim H13

| Saniye | Görüntü | Araç | Altyazı (EN) |
|---|---|---|---|
| 0–3 | Başlık | — | *Every language. Every screen.* |
| 3–8 | İstem: "Check all menus in all languages for overflow." | — | *One prompt.* |
| 8–17 | Hızlı tur: 4 dil × 3 ekran ızgarası, taşan kutular kırmızı çerçeve | `loc_screen_sweep` (0.6.3), `text_overflow_audit` (0.4.1) | *It renders every screen in every language.* |
| 17–24 | Kamera turu karesi önce/sonra, fark haritası | `screenshot_tour` (0.6.5), `screenshot_ab`, `image_stats` | *And compares against the last good build.* |
| 24–28 | Toplu düzeltme sonrası tek komutla geri alma | `batch` rollback | *Every change can be undone.* |
| 28–30 | Son kart | — | *Enginewright · Unreal Engine MCP* |

### D1 uzun (YouTube, 3–5 dk) · H14

1. 0:00–0:20 D1'in ilk 20 sn'si (kanca).
2. 0:20–1:30 Kurulum: zip → `Plugins/` → `doctor` → istemci ayarı (README adımları, hızlandırmasız).
3. 1:30–3:30 D1 akışı hızlandırmasız, ajanın her çağrısı açıklanır.
4. 3:30–4:00 Epic MCP ile birlikte kullanım (iki sunucu aynı istemcide).
5. 4:00–4:30 Vitrin: "Built while making real games" (§5) + link.

---

## 5. Vitrin oyun klipleri kuralı

1. **Yalnız gerçek kullanım.** Klipte Enginewright'ın o oyunda gerçekten yaptığı iş görünür (ör. paketli build testi, dil taraması). Sahne kurgusu yok.
2. **Ayrı yerde.** Oyun klibi ana demonun (D1–D3) içine girmez. Yalnız: D1 uzun sürümün son 30 sn'si, `docs/showcase.md`, ayrı X/Shorts gönderisi (§3 sıra 9).
3. **Süre:** tek klip ≤ 10 sn; vitrin gönderisi ≤ 20 sn.
4. **Ad nerede geçer:** klip başlığı, altyazı, showcase sayfası. **Geçmez:** sürüm adı, CHANGELOG, araç adı/açıklaması, `server.json`, topic, README ilk ekranı (09 S10).
5. **Onay:** Her klip yayından önce Talha onayından geçer (oyun başına Telegram mesajı). Yayınlanmamış içerik (hikâye, boss, yeni harita) onaysız gösterilmez.
6. **Oran:** 90 günde vitrin gönderisi, araç gönderilerinin en fazla **1/3**'ü (TAHMİN: ürün mesajı oyun tanıtımına dönmesin).
7. **Bağlantı:** Mağaza linki (ör. Steam sayfası) yalnız showcase sayfasında ve vitrin gönderisinin yanıtında. Ana duyuruda yok.
8. **Dil:** "Used to build", "Built with". **"Made by AI"** denmez. Gerekçe (TAHMİN): oyun topluluklarında "AI ile yapılmış oyun" tepkisi oyunlara zarar verir.

---

## 6. HN (Show HN) kuralı

- 0.6.6'da yapılmaz. **1.0.0'da bir kez** (03b §7 #5; S3-13).
- Kural (K4): denenebilir ürün, kayıt engeli yok; sürüm notu Show HN değildir; oy istemek yasak; yazar yorumlarda.
- Zaman: Pazar 19:00 ET civarı (03b §5: 2021+ verisinde %10,8; kaynak tek blog → TAHMİN).
- Başlık: *Show HN: Enginewright – Playwright for Unreal Engine (MCP)*. İlk yorum: neden yapıldı, Epic MCP ile fark (11 §2), sınırlar (Win64, UE 5.8/5.7).

---

## 7. Sonraki 90 gün (duyuru günü = G0)

0.7.x serisi aynı dönemde sürer (10 §3: 0.7.0 … 0.7.5, ≈ 64 sa TAHMİN). Talha zamanı: **~2 sa/hafta** tanıtım (05 §7).

| Hafta | Kanal | İş | Ara sürüm bağı |
|---|---|---|---|
| G0 – H1 | Hepsi | §3 sırası; her yoruma 24 saatte cevap | 0.6.6 |
| H1 | X + Shorts | D2 (G+4), D3 (G+8) | 0.6.3, 0.6.5 |
| H2 | X + Shorts | Vitrin klibi 1 (G+12) | — |
| H2 | GitHub | İlk 10 issue/discussion'a cevap; "good first issue" etiketleri güncel | — |
| H3 | Reddit | r/gamedev postmortem (yapılmadıysa) | — |
| H3–H4 | wong2/awesome-mcp-servers | PR (CONTRIBUTING önce okunur) | — |
| H4 | Ölçüm | 30 gün raporu (§10) → SORULAR'a "devam / değiştir" sorusu | — |
| H5–H8 | YouTube | Kurulum videosu (5 dk, ayrı) + 2 Shorts (0.7 sanat hattı ilk klipleri) | 0.7.0 – 0.7.3 |
| H6 | Unreal forumu | Showcase thread'ine 0.7 ara notu | 0.7.x |
| H8 | X | 0.7.5 seri sonu gönderisi (küçük paylaşım biçimi, §2) | 0.7.5 |
| H9–H12 | Topluluk | Üçüncü kişi videoları için: YouTube'da UE/AI eğitim kanallarına **kısa, tek sefer** e-posta (spam değil, kişisel) — 03b §0 #3 "en güçlü sürekli kanal YouTube eğitimleri" | — |
| H12 | Ölçüm | 90 gün raporu; 1.0 duyurusu (Show HN) hazırlığı başlar | 0.8.x |

---

## 8. YAPMA listesi

| # | Yapma | Neden |
|---|---|---|
| Y-1 | Yıldız satın alma, bot, karşılıklı yıldız grubu, sahte hesap | GitHub kabul edilebilir kullanım ihlali; depo kapanabilir (05 §6) |
| Y-2 | Aynı metni birden çok subreddit/Discord'a kopyalama | Spam; ban (05 §6) |
| Y-3 | Arkadaştan oy / yıldız / yorum isteme (HN, Reddit) | HN kuralı (K4); Reddit oy manipülasyonu |
| Y-4 | Hazırlanmış/hileli demo; kasıtlı hatayı gizleme; hızlandırmayı gizleme | Güven ürünün özü ("verified") |
| Y-5 | Henüz çıkmamış ara sürümün yeteneğini iddia etme (ör. 0.6.0'dan önce "ships") | 11 §1.1; ilk kullanıcı denediğinde güven kaybı |
| Y-6 | Büyük demoyu (D1) küçük paylaşımlarda harcama | Patlama ilk 5 haftada olur, bir kez (03b §0 #1) |
| Y-7 | "Official", "Epic", "Unreal" logosu; "replaces Epic's MCP" | Marka hakkı; konumlandırma "next to" (11 §5) |
| Y-8 | Rakibi kötüleme; rakip adıyla karşılaştırma gönderisi | Topluluk tepkisi; 11 §2 tablosu yalnız iç belge |
| Y-9 | **Eski depo adıyla (`shanyup/enginewright`) yeni depo açma**; depo adını ikinci kez değiştirme | Yönlendirme kırılır (K1) |
| Y-10 | Telemetri / izinsiz veri toplama | blender-mcp itirazı (05 §6; S3-11) |
| Y-11 | Sürüm adına, CHANGELOG'a, araç adına oyun adı yazma | 09 S10 genel MCP kuralı |
| Y-12 | Tamamen LLM yazımı duyuru metni (özellikle r/LocalLLaMA, r/ClaudeAI) | Subreddit kuralları (03b §5); metin Talha sesiyle son hâline gelir |
| Y-13 | Duyuruyu 0.6.6 güvenilirlik turu geçmeden yapma | §1 kapı |
| Y-14 | Bekleme listesi / kapalı beta ile hype | r/mcp kuralı (03b §5); şeffaflık |

---

## 9. Gerçekçi yıldız hedefleri (TAHMİN)

Bugün: **10 ★** (17.11, 03b §1). Emsallerin ilk 30 gün / 365 gün eğrisi (03b §1, star-history, ±%3):

| Emsal | ~30. gün ★ | ~365. gün ★ | Ana tetikleyici | Bize benzerliği |
|---|---:|---:|---|---|
| runreal/unreal-mcp | ~15 | ~100 | r/mcp gönderisi, 14 oy | Düşük çaba senaryosu |
| flopperam/unreal-engine-mcp | ~40 | ~1.050 | YouTube kanalı + Shorts | YouTube sürekliliği |
| Coding-Solo/godot-mcp | ~140 | ~2.000 | awesome-mcp-servers kaydı | Dizin + niş |
| chongdashu/unreal-mcp | ~460 | ~1.600 | Yazarın YouTube demosu (Nisan 2025, pazar boştu) | Erken pazar; bugün tekrar edilemez |
| CoplayDev/unity-mcp | ~1.450 | ~8.300 | Viral X demo | Tavan örneği |

Düzeltme: 03b §0 #5 — UE 5.8 resmi MCP'si (Haziran 2026) sonrası topluluk UE MCP eğrileri düzleşti. Bu yüzden chongdashu ve unity değerleri bize **üst sınırdır**, hedef değildir.

| Senaryo | G+30 | G+90 | G+365 | Ne olursa |
|---|---:|---:|---:|---|
| Düşük | 30–60 | 80–150 | 200–400 | Video yayılmaz; yalnız dizinler + Reddit |
| **Hedef (orta)** | **100–200** | **250–500** | **800–1.500** | D1 X'te orta yayılım (≥ 20 bin görüntülenme, TAHMİN), awesome listesi birleşti, YouTube düzenli |
| Yüksek | 400–800 | 1.000–2.000 | 3.000+ | D1 viral ya da bilinen bir hesap paylaşır (03b §0 #2) |

Hepsi **TAHMİN**. Dayanak yalnız yukarıdaki emsal eğrileri; nedensellik kanıtlı değil. 05'teki 30 g 80–150 / 90 g 300–600 hedefi orta senaryoyla aynı aralıktadır.

Duyuru öncesi beklenti (0.4.4 + 0.5.6 küçük paylaşımları): **10 → 30–60 ★** (TAHMİN; dizin kaydı + forum + X).

---

## 10. Ölçüm

Kayıt yeri: `docs/plan_2026-11/research/olcum/` (iç; açık dala girmez — S3-01). Ölçüm kartı ayrı açılır (bu kart açmaz).

| Ne | Nasıl | Sıklık | Neden |
|---|---|---|---|
| Yıldız (toplam + günlük) | `gh api repos/shanyup/enginewright-unreal-mcp/stargazers` (zaman damgalı; kendi depomuzda çalışıyor, 03b yöntem notu) | Haftalık; duyuru haftası günlük | §9 hedefi |
| Trafik kaynakları | `gh api repos/…/traffic/popular/referrers` + `traffic/views` | **Haftalık** (GitHub yalnız 14 günü tutar) | Hangi kanal çalıştı |
| Klon / indirme | `traffic/clones` + release asset `download_count` | Haftalık | Gerçek kurulum sinyali |
| Dış issue / discussion | Talha dışı açılan sayı | Haftalık | Kullanım sinyali (yıldızdan değerli) |
| Gönderi metrikleri | X görüntülenme, Reddit oy/yorum, YouTube izlenme + ortalama izlenme süresi | G+1, G+7, G+30 | Video kalitesi |
| Dizin durumu | Glama skoru, awesome PR, Registry, mcp.so | Aylık | 12 §6 |
| GitHub arama sırası | `unreal mcp` ve `unreal engine mcp` aramasında konum | Aylık | Depo adı etkisi (12 §1) |

Karar kuralı (TAHMİN): G+30'da "Düşük" senaryodaysak 90 gün planında YouTube payı artar, X payı azalır (03b §0 #3).
