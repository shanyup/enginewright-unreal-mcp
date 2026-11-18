# 12 — Keşfedilebilirlik ve SEO v2

Tarih: 18.11.2026 · Kart `t_c6e43c15` · YALNIZ BELGE. **04_Kesfedilebilirlik_SEO.md'nin yerini alır** (04 silinmedi; üstüne not düşüldü).
Girdiler: 04, 01, 03b §5–§7, 10 §3–§4, 11, SORULAR (S-0 … S-10), SORULAR_2, README / CHANGELOG / `.uplugin` (yalnız okundu).
Web notları: [`research/yayin_hazirlik_kaynaklar_2026-11-18.md`](research/yayin_hazirlik_kaynaklar_2026-11-18.md) (K1–K6). Topic sayıları: [`research/raw/topic_sayilari_2026-11-18.json`](research/raw/topic_sayilari_2026-11-18.json).

**Bu kart GitHub'da hiçbir şey değiştirmez.** Aşağıdaki her iş, yazılı ara sürümde ayrı bir kartla yapılır. "S3-xx" = [SORULAR_3](SORULAR_3.md) sorusu; cevap gelene kadar öneri (ilk seçenek) varsayılır.

---

## 0. Takvim özeti (iş → ara sürüm)

S2-15 A: GitHub sürümü (zip) yalnız seri sonlarında çıkar: **0.4.4, 0.5.6, 0.6.6**, 0.7.5, 0.8.7, 0.9.16, 1.0.0. SEO işleri bu üç yayına bağlanır.

| Ne zaman | SEO / keşif işi | Bölüm |
|---|---|---|
| **0.4.4 yayını** (ilk seri sonu) | Push öncesi iç belge ayrımı (S3-01) · **depo adı → `enginewright-unreal-mcp`** (S3-02) · ara açıklama · 20 topic · sürüm sayfası · CHANGELOG/README bağlantıları yeni adla | §1, §2, §3 |
| **0.5.6 yayını** | Glama kaydı + skor rozeti · punkpeye/awesome-mcp-servers PR (birleşme haftalar sürebilir, duyurudan önce açılmalı) · README "Tools" tablosu dizinlerin okuyacağı biçimde | §6 |
| **0.6.0 – 0.6.2** | Registry köprüsü kararı uygulanır (S3-03) · `server.json` taslağı | §6 |
| **0.6.5** | Sosyal görsel kareleri (`screenshot_tour` ile) · ana demo GIF'i (13 D1) | §5 |
| **0.6.6 yayını = büyük duyuru − 1…3 gün** | Son açıklama · README ilk ekran v2 · sosyal görsel yüklenir · Resmi MCP Registry yayını · mcp.so / PulseMCP / mcp.directory formları | §2, §4, §5, §6 |
| **0.9.15** | README araç tablosu açıklamadan üretilir (10 §3 #49) → aynı üretici doküman sitesi sayfalarını da üretir | §7 |
| **1.0.0** | Doküman sitesi canlı (S-6 → S3-06) · Fab ücretsiz listesi (S-1 → S3-05) · Show HN (13 §6) | §7, §6 |

---

## 1. Depo adı geçiş planı: `shanyup/enginewright` → `shanyup/enginewright-unreal-mcp`

Karar (Talha 18.11): marka **Enginewright** kalır; depo adı `enginewright-unreal-mcp`. Bu bölüm yalnız plandır.

### 1.1 GitHub ne yapar (K1, GitHub Docs, 18.11 okundu)

- Issue, wiki, **yıldız**, takipçi yeni ada **yönlenir**. Web adresleri ve `git clone/fetch/push` eski adla çalışmaya devam eder.
- **Yönlenmeyenler:** GitHub Pages proje sitesi URL'si; yeniden adlandırılan depoda barınan GitHub Action çağrıları.
- **Kırılma riski:** `shanyup` hesabında ileride `enginewright` adlı yeni depo açılırsa bütün yönlendirmeler kırılır. → **Kalıcı kural: eski ad bir daha kullanılmaz** (13 §8 YAPMA).
- Bizim için: Pages yok, yayımlanmış Action yok → iki istisna bugün bizi etkilemez. **Pages adı değişiklikten sonra açılır** (§7).

### 1.2 Neden 0.4.4'te (duyurudan önce, en erken seri sonunda)

- Dizinler (Glama, awesome listesi, Registry) depo URL'sini kaydeder. **Ad, ilk dizin kaydından önce** değişirse hiçbir listede eski ad kalmaz.
- Küçük paylaşımlar (13 §2) 0.4.4'ten sonra başlar. Paylaşılan bütün linkler baştan yeni adı taşır.
- 03b §4.2 #5: sahiplik/ad değişikliği aramada geçici kayba yol açabilir. Yıldız 10'dayken bu kayıp en ucuz anındadır.
- Resmi MCP Registry MCPB yolu, indirme URL'sinde "mcp" ister (K2). Yeni ad bunu kendiliğinden karşılar.
- Seçenek ve gerekçe: **S3-02** (öneri: 0.4.4).

### 1.3 Etki envanteri (18.11 grep, `shanyup/enginewright` geçen dosyalar)

| Dosya | Geçiş | Ne olur | Kim / ne zaman |
|---|---:|---|---|
| `Plugins/Enginewright/Enginewright.uplugin` | 2 | `CreatedByURL`, `DocsURL` yeni ada. **Fab/eklenti manifesti budur**; `MarketplaceURL` ve `SupportURL` alanları bugün yok, Fab listesinde (S3-05) eklenir | Kod kartı, 0.4.4 |
| `Plugins/EnginewrightMVVM/EnginewrightMVVM.uplugin` | 2 | Aynı | Kod kartı, 0.4.4 |
| `Plugins/Enginewright/Source/Enginewright/Private/EWEditorUI.cpp` | 2 | Paneldeki "Roadmap" ve "Changelog" düğmeleri `LaunchURL` ile sabit adres açar. Yönlendirme sayesinde çalışmaya devam eder; yine de yeni ada çevrilir. **Eski sürümü kurmuş kullanıcıda yönlendirme kalıcı olarak gerekir** (§1.1 kuralı bunun için) | Kod kartı, 0.4.4 |
| `README.md` | 6 | Releases linki, rozetler, klon komutu | Belge kartı, 0.4.4 |
| `CHANGELOG.md` | 6 | **Eski girdiler değişmez** (geçmiş kaydı; yönlendirme çalışır). Yeni `[0.4.4]` girdisine "Repository renamed" satırı + alttaki karşılaştırma linkleri yeni ad | Belge kartı, 0.4.4 |
| `CONTRIBUTING.md` | 4 | Yeni ad | Belge kartı, 0.4.4 |
| `docs/good-first-issues.md` | 9 | Yeni ad | Belge kartı, 0.4.4 |
| `docs/REQUEST_WITH_AI.md`, `docs/SUPPORT.md` | 2 + 1 | Yeni ad | Belge kartı, 0.4.4 |
| `.github/ISSUE_TEMPLATE/config.yml`, `feature_request.yml` | 2 + 1 | Yeni ad | Belge kartı, 0.4.4 |
| `docs/release-notes/v0.3.1.md` | 1 | **Değişmez** (yayımlanmış sürüm notu) | — |
| `docs/plan_2026-11/*`, `docs/arastirma/*` | çok | **Değişmez** (iç plan; tarihli kayıt). Ayrıca bu klasörler açık dala girmeyebilir (S3-01) | — |
| Yayımlanmış zip'ler (v0.3.x, v0.4.0) | — | İçlerindeki `.uplugin` eski adı taşır; yönlendirme çalışır. Yeniden paketlenmez | — |
| Yerel kopyalar | — | Talha PC: `git remote set-url origin https://github.com/shanyup/enginewright-unreal-mcp.git`. Oyun projelerindeki `Plugins/Enginewright` junction'ı dosya yoludur, git remote değildir → etkilenmez | Talha, ad değişikliği günü |

Toplam: kod kartında 3 dosya (6 satır), belge kartında 8 dosya (~25 satır). **Marka adı, eklenti klasörü (`Plugins/Enginewright`), modül adı, araç adları, `ENGINEWRIGHT_*` ortam değişkenleri, port ve token yolu değişmez.**

### 1.4 Sıra (0.4.4 yayın günü, adım adım)

Uyarı: 1. adım bitmeden 2. adıma geçme. Push kararı yayın işindedir (S2-15).

1. **İç belge ayrımı (S3-01):** `main` yerelde `origin/main`'den 47 commit önde; bu commit'lerde `docs/plan_2026-11/` ve `docs/arastirma/` var (58 dosya, Türkçe iç plan). YAYIN_HAZIRLIK K4 kararı "iç Türkçe belgeler açık dala girmez" der. Push'tan önce bu klasörlerin yeri kararlaştırılır.
2. Kod + belge kartları (§1.3) yeni adı yerelde yazar. 0.4.4 RELIABILITY turu geçer.
3. GitHub → Settings → Repository name → `enginewright-unreal-mcp` → Rename (Talha).
4. Talha PC: `git remote set-url origin …/enginewright-unreal-mcp.git`.
5. Push + `v0.4.4` etiketi + GitHub sürümü (zip). Sürüm notunun ilk satırı: *"The repository moved to shanyup/enginewright-unreal-mcp. Old links redirect."*
6. Açıklama (§2 ara metin) + 20 topic (§3) GitHub'a girilir.
7. Doğrulama: eski URL tarayıcıda yeni adrese gider; eski URL ile `git ls-remote` çalışır; paneldeki iki düğme açılır; yıldız sayısı aynı.
8. 03b ve 01'deki "★ ölçümü" betiklerinin depo adı güncellenir (iç belge kartı; bu karta dahil değil).

---

## 2. Açıklama (≤ 350 karakter, tam metin)

Karakter sayıları Python `len()` ile ölçüldü.

**Son metin — 0.6.6 yayını ve sonrası (342 karakter):**

> Unreal Engine MCP server that lets AI agents test and ship your game, not just edit it: multiplayer Play In Editor, packaged builds, bot runs, visual QA and localization checks. Runs inside the UE 5.8 editor; every change is verified and undoable. No Python or Node. Works with Claude Code, Codex, Cursor and next to Epic's built-in MCP. MIT.

**Ara metin — 0.4.4 … 0.6.5 (322 karakter):**

> Unreal Engine MCP server that lets AI agents play and check your game, not just edit it: multiplayer Play In Editor, packaged-game runs, visual QA and localization checks. Runs inside the UE 5.8 editor; every change is verified and undoable. No Python or Node. Works with Claude Code, Codex and Cursor. MIT. Early preview.

**Kısa metin — dizin formları, awesome listesi satırı (151 karakter):**

> Unreal Engine MCP: AI agents test and ship your UE game - multiplayer PIE, packaged builds, bot runs, visual QA. Verified, undoable, inside the editor.

04 §3'ten farkı: araç sayısı (129) açıklamadan çıktı (her sürümde eskir); "test and ship" öne geldi (11 §1); "Epic'in yanında" eklendi. 0.9.13 sonrası "UE 5.8" → "UE 5.8 and 5.7".

---

## 3. 20 topic (sayılar 18.11.2026, GitHub REST `search/repositories?q=topic:<t>`)

Bugünkü 7 topic: `ai-agents`, `claude`, `codex`, `mcp`, `model-context-protocol`, `unreal-engine`, `unreal-engine-plugin`.

| # | Topic | Depo sayısı | Neden | Bugün var mı |
|---:|---|---:|---|---|
| 1 | `mcp` | 122.506 | Ana protokol etiketi; PulseMCP/Glama taraması bunu okur (04 §7) | ✅ |
| 2 | `mcp-server` | 44.678 | Sunucu türü | ➕ |
| 3 | `model-context-protocol` | 45.534 | Resmi ad | ✅ |
| 4 | `unreal-engine` | 4.651 | Motor; ana nişin topic sayfası | ✅ |
| 5 | `unreal-engine-5` | 2.402 | Sürüm | ➕ |
| 6 | `ue5` | 1.498 | Kısaltma | ➕ |
| 7 | `unreal` | 1.158 | Kısa ad | ➕ |
| 8 | `unreal-engine-plugin` | 559 | Eklenti türü | ✅ |
| 9 | `ue5-plugin` | 326 | Eklenti türü, kısa | ➕ |
| 10 | `ai-agents` | 160.021 | Hedef kitle | ✅ |
| 11 | `claude` | 70.521 | Birincil istemci ailesi | ✅ |
| 12 | `claude-code` | 122.250 | Birincil istemci; hızlı büyüyen etiket | ➕ |
| 13 | `codex` | 52.443 | İstemci | ✅ |
| 14 | `cursor` | 21.653 | İstemci | ➕ |
| 15 | `game-development` | 42.093 | Kategori | ➕ |
| 16 | `gamedev` | 15.802 | Kategori, kısa | ➕ |
| 17 | `test-automation` | 10.785 | **Konumlandırma: "test"** (11 §1) | ➕ |
| 18 | `game-testing` | 171 | Tam niş; küçük sayfada üst sırada görünür | ➕ |
| 19 | `visual-testing` | 450 | Görsel QA (K-3); küçük sayfa | ➕ |
| 20 | `multiplayer` | 9.328 | Çok oyunculu PIE (K-1) | ➕ |

7 korunur, 13 eklenir = 20 (GitHub sınırı). 04'ten çıkanlar ve neden: `llm`, `cpp`, `agentic-ai`, `ai-tools` (çok geniş, niş sayfada görünmeyiz), `vibe-coding` (konumlandırmaya ters: biz doğrulamayı satıyoruz), `blueprint`, `editor-plugin` (düzenleme vurgusu; yerine test/paket etiketleri). Ölçüldü ama alınmadı: `automation` 120.175, `qa-automation` 3.591, `localization` 8.566, `packaging` 1.844 (anlamı farklı: yazılım paketleme). Yer açılırsa ilk aday `qa-automation`.

Uygulama: 0.4.4 yayın günü (§1.4 adım 6). Oyun adı topic'e girmez (09 S10).

---

## 4. README ilk ekran v2 (EN, tam metin)

Bu metin **0.6.6 yayınında** README'nin başına girer (README bu kartta değişmez). 0.4.4 … 0.6.5 arasında aynı iskelet kullanılır; yalnız alt başlık 11 §1.1 ara cümlesi olur ve "Ships the build" maddesi "Runs the packaged game (`game_run`)" olur.

Kural: ilk ekran = rozetler + bir cümle + GIF + üç madde + kurulum linki (03b §4.1 #9). Oyun adı ilk ekranda yok; vitrin satırı aşağıda (§4.1).

```markdown
# Enginewright — Unreal Engine MCP for AI agents

[![Version](https://img.shields.io/badge/version-0.6.6-blue)](docs/release-notes/v0.6.6.md)
[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.8-black)](#supported-versions)
[![Platform](https://img.shields.io/badge/platform-Win64-lightgrey)](#supported-versions)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![Glama score](https://glama.ai/mcp/servers/shanyup/enginewright-unreal-mcp/badges/score.svg)](https://glama.ai/mcp/servers/shanyup/enginewright-unreal-mcp)

**Let your AI agent test and ship your Unreal game, not just edit it.**
Enginewright runs a Model Context Protocol server inside the Unreal Editor. Claude Code, Codex, Cursor, VS Code or any MCP client can play the game in multiplayer, build and run the package, replay scripted bot runs and check what the player sees. Every change is verified and can be undone. It works next to Epic's built-in Unreal MCP.

![An agent packages a test project, starts two players, finds a value that does not replicate and shows the screenshot diff](docs/media/ship-demo.gif)

- **Tests multiplayer.** Two-player listen-server Play In Editor, network emulation, game input, and a replication check that names the value that did not sync.
- **Ships the build.** Async Win64 packaging, then runs the packaged game with a scripted scenario or a bot run and samples the frame rate.
- **Checks what the player sees, safely.** Screenshot A/B and region stats, language × screen overflow sweeps, and one-step rollback for batched edits.

**Install in about 5 minutes:** download the zip from [Releases](https://github.com/shanyup/enginewright-unreal-mcp/releases), unzip it into `<YourProject>/Plugins/`, open the editor and paste the ready-made client config. [Step by step ↓](#install-about-5-minutes)

> No Python, no Node: one C++ plugin. UE 5.8, Win64. MIT. Roadmap: [docs/ROADMAP.md](docs/ROADMAP.md).
```

### 4.1 İlk ekranın altındaki vitrin satırı (EN)

```markdown
## Built while making real games

Enginewright is used every day to build three Unreal games: a co-op cooking roguelite, a tactical shooter and an action RPG. Short clips: [showcase](docs/showcase.md).
```

Oyun adları yalnız `docs/showcase.md` içinde ve kliplerin başlığında geçer (13 §5). README ilk ekranı oyun adı taşımaz.

---

## 5. Sosyal önizleme görseli (brif)

| Alan | Değer |
|---|---|
| Boyut | 1280 × 640 PNG, < 1 MB (GitHub önerisi; 04 §6) |
| Güvenli alan | Kenarlardan 64 px boşluk (X / Discord / Slack kırpar — TAHMİN payı) |
| Sol üçte bir | Enginewright işareti + **"Enginewright"** yazısı. Altında küçük: "Unreal Engine MCP" |
| Orta | Başlık (2 satır, büyük): **"Test and ship your Unreal game with AI agents"** |
| Alt şerit | Üç etiket: **Multiplayer PIE** · **Packaged builds** · **Visual QA** |
| Sağ | Gerçek kare: iki PIE penceresi yan yana + küçük `image_stats` fark karesi (genel test projesinden, `screenshot_tour` 0.6.5 ile) |
| Köşe | "MCP · UE 5.8 · MIT" |
| Yasak | Epic / Unreal logosu (marka izni yok), "Official" sözcüğü, oyun adı/logosu, araç sayısı |
| Kaynak dosya | Marka kitinde (ShanyupBrand / Enginewright işareti) katmanlı kaynak + `docs/media/social-preview.png` |
| Ne zaman | Kareler 0.6.5'te, yükleme 0.6.6 yayınında (Settings → Social preview). Aynı görsel X kartı ve YouTube kapak taslağı olur |

İşaret: 04 "two-block S" diyordu; o Shanyup Studios işaretidir. Enginewright'ın kendi işareti var mı belirsiz → **S3-07**.

---

## 6. Dizin ve registry kayıtları

Sıra mantığı (03b §7 #4): dizinler **duyurudan önce** bitsin; duyuru günü link verdiğimiz her yerde ad aynı olsun. Ad değişikliği (0.4.4) her kayıttan önce.

| # | Yer | Kabul kuralı (kaynak) | Bizde ne gerekir | Ne zaman |
|---:|---|---|---|---|
| 1 | **Glama** (glama.ai/mcp/servers) | "Add MCP Server" + GitHub URL; otomatik lisans/güvenlik/sağlık taraması; `mcp` topic ile kendiliğinden de indeksler (03b §6; Glama FAQ). awesome listesiyle eşlenir (K3) | Açık depo, LICENSE, README'de kurulum. Not: sunucu editör ister; Glama'nın otomatik "başlat ve araçları listele" testi geçmeyebilir → skor düşük çıkabilir (TAHMİN) | **0.5.6 yayını** |
| 2 | **punkpeye/awesome-mcp-servers** | Fork → README'de doğru kategoriye **tek satır** → PR. Satır biçimi: `[sahip/depo](url) [Glama rozeti] <dil> <kapsam> <OS> - açıklama` (K3) | Glama sayfası + rozet (1. satır önce). Kategori `🎮 Gaming`. Takılar `🌊 🏠 🪟`. Açıklama = §2 kısa metin | **0.5.6 yayını** (+ PR birleşmesi beklenir) |
| 3 | **Resmi MCP Registry** | `server.json` + `mcp-publisher publish`; paket türü npm/PyPI/NuGet/OCI/MCPB, her biri sahiplik doğrulamalı; ad `io.github.shanyup/<ad>`; preview, veri sıfırlanabilir (K2) | Editör içi HTTP sunucu bu türlere **doğrudan uymaz**. Köprü gerekir: MCPB ya da küçük npm stdio köprüsü (S3-03) | Karar 0.6.0, yayın **0.6.6** |
| 4 | **mcp.so** | Ücretsiz form (incelemeli); README'den kurulum bloğu, `## Tools` tablosu ve ilk görseli çeker (03b §6) | README `## Tools` tablosu (var) + ilk görsel (§4 GIF) | **0.6.6 yayını** |
| 5 | **PulseMCP** | Otomatik tarama; `mcp` + `mcp-server` topic (04 §7). Kural bu kartta yeniden doğrulanmadı | §3 topic'leri | 0.4.4'ten sonra kendiliğinden; 0.6.6'da kontrol |
| 6 | **mcp.directory / MCP Market / LobeHub** | Ücretsiz kuyruk; MCP Market 4–6 hafta (03b §6) | Form | **0.6.6 yayını** (kuyruk uzun, erken gir) |
| 7 | **wong2/awesome-mcp-servers** | PR (03b §6). Güncel CONTRIBUTING bu kartta okunmadı → PR öncesi okunur | Tek satır | 0.6.6 sonrası, 90 gün planında (13 §7) |
| — | **Smithery** | Uzak (Streamable HTTP) ya da paketlenmiş sunucu ister (03b §6) | Yerel editör sunucusu uymaz | **Yapılmaz** (köprü çıkarsa yeniden bakılır) |
| — | **Fab (ücretsiz kod eklentisi)** | Kod eklentisi ücretsiz listelenebilir (04 §9, SORULAR S-1) | `.uplugin` `MarketplaceURL` + `SupportURL`; sürüm başına zip | S-1 hâlâ açık → **S3-05** (öneri: 1.0.0) |

---

## 7. Doküman sitesi zamanı

- **Öneri (S3-06): 1.0.0'da canlı; hazırlığı 0.9.15'te.** 0.9.15 "README tablosunu açıklamadan üret" işidir (10 §3 #49). Aynı üretici araç başına sayfa da üretir. Daha önce açılırsa araç adları 0.9.12 API dondurmasından önce değişir ve sayfalar eskir.
- 1.0'a kadar "site" = README + `docs/` (GitHub'ın kendi gösterimi).
- Adres: depo adı değiştikten sonra açıldığı için `shanyup.github.io/enginewright-unreal-mcp`. İstenirse özel alan adı (ör. alt alan adı) — S3-06 seçenek C.
- Sayfa yapısı (04 §8'den, aynen geçerli): `index` (README ilk ekranı), `tools/` (araç başına), `guides/` (quickstart, client config, multiplayer testing, packaging, visual QA), `changelog/`.

---

## 8. Anahtar kelimeler

Hacim sayısı **yok**: 04 §10'daki aylık hacimler kaynaksız tahmindi ve bu kartta ölçülemedi (araç erişimi yok). Sıra, konumlandırmaya (11) göre.

| Öncelik | İfade (EN) | Nerede kullanılır |
|---|---|---|
| 1 | unreal engine mcp | Depo adı, H1, açıklama başı, `server.json` title |
| 1 | unreal mcp server | Açıklama, README ilk paragraf |
| 2 | ai agent unreal engine testing | README madde 1, docs `guides/` |
| 2 | unreal engine packaging automation | README madde 2, D1 video başlığı |
| 2 | unreal multiplayer test automation / PIE automation | README madde 1, D2 video başlığı |
| 2 | claude code unreal engine | Kurulum bölümü, r/ClaudeAI gönderisi |
| 3 | unreal visual regression testing / screenshot diff | README madde 3, D3 |
| 3 | unreal localization testing | Araç sayfası `loc_*` |
| 3 | epic unreal mcp alternative / works with epic mcp | "Epic ile ilişki" bölümü (rakip dili değil, "next to") |
| 3 | codex / cursor unreal engine | Kurulum tablosu |

Kural: Oyun adları anahtar kelime değildir; yalnız vitrin sayfasında geçer.

---

## 9. 04'ten değişenler (kısa)

| Konu | 04 | 12 |
|---|---|---|
| Depo adı | Soru (S-0), "v1.0 lansmanında rename" | **Karar** `enginewright-unreal-mcp`; **0.4.4**'te, dizinlerden önce |
| Açıklama | 129 araç sayısı içinde | Sayı yok; "test and ship"; ara + son + kısa metin |
| Topic | `llm`, `cpp`, `vibe-coding`… | Test/QA/multiplayer etiketleri; 18.11 ölçümü |
| README ilk ekran | Araç listesi ağırlıklı | 11 konumlandırması, üç kanıt maddesi |
| Registry | "İlk adım, `server.json`" | Paket türü uyumsuzluğu tespit edildi → köprü sorusu (S3-03) |
| Doküman sitesi | v1.0 | 0.9.15 hazırlık, 1.0 canlı (S3-06) |
| Zaman bağı | "1.0 öncesi" | Her iş bir ara sürüme bağlı (§0) |
