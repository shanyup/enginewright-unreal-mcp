# 05 — Duyuru ve Yıldız Büyüme Planı (90 Gün)

> **18.11.2026: Yerini [13_Duyuru_v2.md](13_Duyuru_v2.md) aldı.** Bu belge tarihli kayıt olarak kalır; güncel plan 13'tedir (büyük duyuru 0.6.6, demo senaryoları).

Tarih: 17.11.2026. Hedef: Enginewright v1.0 lansmanı + 90 günlük duyuru/yıldız büyümesi. Rakip verisine dayalı gerçekçi hedefler. **Yasak taktikler sonda.**

---

## 1. Yıldız Hedefleri (Rakip Eğrisi Temelli)

| Proje | Oluşturma | 1. ay | 3. ay | 6. ay | 12. ay | Not |
|---|---|---:|---:|---:|---:|---|
| chongdashu/unreal-mcp | 2025-03-28 | ~200 | ~600 | ~1,200 | 2,118 | Push 04/2025 sonrası durdu |
| ChiR24/Unreal_mcp | 2025-09-07 | ~150 | ~400 | ~700 | 1,043 | Aktif güncelleniyor |
| db-lyon/ue-mcp | 2026-02-24 | ~100 | ~250 | ~400 | 505 | Hızlı başlangıç |
| VibeUE | 2025-09-12 | ~200 | ~400 | ~600 | 838 | Epic MCP genişletmesi |
| **ahujasid/mcp-for-blender** | **2025-03-07** | **~3,000** | **~10,000** | **~18,000** | **34,600** | Viral: demo video + HN + World's Fair |
| **microsoft/playwright-mcp** | **2025-03-21** | **~5,000** | **~15,000** | **~25,000** | **39,700** | Resmi + VS Code entegrasyonu |

**Enginewright pozisyonu:** UE MCP nişinde (blender/playwright gibi cross-platform değil). En yakın rakip **ChiR24 (EngineWire) 1.043★ / 14 ay** ve **db-lyon 505★ / 9 ay**. Bizim avantajımız: aktif geliştirme (haftalık release), RagnaCook/ARPG/GP gerçek oyunlarda kullanım, demo videoları, tek komut kurulum.

| Hedef | Gün | Yıldız | Gerekçe |
|---|---:|---:|---|
| **30 gün** | 30 | **80–150** | db-lyon 9 ay 505 → ay 56; ChiR24 14 ay 1.043 → ay 74; lansman patlaması + demo videoları + registry girişi ile 80–150 gerçekçi |
| **90 gün** | 90 | **300–600** | db-lyon 9 ay 505; ChiR24 14 ay 1.043; 3 ayda 300–600 organik + duyuru dalgaları |
| **180 gün** | 180 | **800–1.500** | 6 ayda ChiR24 seviyesine yakınlaşma; sürekli release + oyun showcases (RagnaCook demo, ARPG prototip) |

> **Ölçüm:** star-history.com (repo eklendikten sonra), GitHub Insights → Traffic → Referrers, `gh api repos/shanyup/enginewright/stargazers` haftalık çekim.

---

## 2. 90 Günlük Takvim (Kanal + Kural/Zamanlama + Başlık Taslağı EN)

| Gün | Kanal | Kural / Zamanlama | Başlık / İçerik Özeti (EN) | Sorumlu |
|---:|---|---|---|---|
| **Lansman Günü (v1.0 tag)** | | | | |
| 0 | GitHub Release | Tag v1.0.0 + zip Win64 UE 5.8 + CHANGELOG | **Release v1.0.0: Enginewright — In-editor MCP for Unreal Engine with PIE, game_run, Visual QA, Batch Rollback** | Talha |
| 0 | README / Repo | Açıklama + 20 topic + social preview + demo GIF | (04 §3–5 uygulanır) | Talha |
| 0 | MCP Registry | `mcp-publisher publish` | Registry listelenir | Talha |
| 0 | awesome-mcp-servers | PR + Glama badge | PR: "Add Enginewright — strict, verified MCP for Unreal Engine" | Talha |
| **+1** | Hacker News (Show HN) | Salı 09:00 PT, "Show HN:" prefix, 1. yorumda demo linki | **Show HN: Enginewright — Playwright for Unreal Engine (strict, verified, undoable MCP tools, no Python/Node)** | Talha |
| **+1** | r/unrealengine | Cuma 11:00 ET, self-promotion kuralı (9:1), demo GIF + link | **Enginewright v1.0: In-editor MCP server for UE 5.8 — PIE control, packaged game runs, visual QA with SSIM, one-command client config** | Talha |
| **+1** | r/gamedev | Aynı gün, farklı başlık, "I built this" flair | **I built an MCP server that lets AI agents run packaged UE games, do multiplayer tests, and visual regression — all from the editor** | Talha |
| **+1** | X/Twitter (Thread) | 6 tweet thread, demo GIF/video, #UnrealEngine #MCP #AIagents | 1/6 Enginewright v1.0 is live. An in-editor MCP server for Unreal Engine that gives AI agents strict, verified tool calls. No Python/Node. 2/6 PIE control with synthetic input, multiplayer net emulation, and packaged game runs (`game_run`). 3/6 Visual QA: screenshot + SSIM diff + side-by-side compose. 4/6 Localization: PO/locres compare, font coverage, glyph misses. 5/6 Atomic batch with rollback, one-command client config for Claude/Codex/Cursor/VS Code. 6/6 MIT, free. Try it: github.com/shanyup/enginewright | Talha |
| **+2** | Unreal Engine Forums (Work in Progress) | Forum kuralları, "Plugins" alt forumu | **Enginewright v1.0 — In-editor MCP server for UE 5.8: PIE, game_run, visual QA, batch rollback** | Talha |
| **+3** | Discord (Unreal Engine, UE5, MCP, AI Gamedev) | Her sunucuda 1 mesaj, #showcase veya #releases kanalı, demo GIF | "Enginewright v1.0 released — in-editor MCP for UE 5.8 with PIE control, game_run, visual QA. MIT, no Python/Node. github.com/shanyup/enginewright" | Talha |
| **+3** | YouTube Shorts / Reels | 3 video (30–60 sn), Shorts formatı, başlık + açıklamada link | Video 1: "Enginewright demo: AI agent builds a room in UE 5.8" — Video 2: "Visual QA with SSIM in Unreal Engine" — Video 3: "Multiplayer test automation with Enginewright" | Talha |
| **+7** | dev.to / Medium | Makale: "Building an MCP Server for Unreal Engine" | **How I built Enginewright: an in-editor MCP server that lets AI agents run packaged UE games and do visual regression testing** | Talha |
| **+14** | Product Hunt | Lansman haftası, "Developer Tools" kategorisi | **Enginewright — Playwright for Unreal Engine** | Talha |
| **+21** | r/IndieDev | Hafta sonu, "Tool" flair | **Free MCP server for Unreal Engine: PIE automation, visual QA, packaged game runs** | Talha |
| **+30** | **2. Dalga: RagnaCook Demo Showcase** | RagnaCook demo build paylaşımı + Enginewright kullanım videosu | **RagnaCook demo built with Enginewright: from art pipeline to packaged build in one MCP session** | Talha |
| **+45** | **3. Dalga: ARPG Prototip Showcase** | ARPG grind prototipi (F1) + Enginewright hızlanma verisi | **ARPG prototype: 357 affixes, 90 uniques, built 10× faster with Enginewright MCP** | Talha |
| **+60** | **4. Dalga: v1.1 Release + Yeni Araçlar** | v1.1 patch + yeni araç duyurusu | **Enginewright v1.1: datatable_csv, world_inventory, headless import — content pipeline tools** | Talha |
| **+90** | **5. Dalga: 90 Gün Raporu + Topluluk** | Star-history grafiği + trafik raporu + katkıcı call | **90 days of Enginewright: 300+ stars, 3 game projects, 6 demo videos. Join us!** | Talha |

---

## 3. Demo Videoları (6 adet, 30–60 sn, saniye saniye senaryo)

| # | Araç / Özellik | Senaryo (saniye) | Çıktı |
|---|---|---|---|
| **V1** | `pie_start` + `pie_input` + `pie_actors` | 0–5: Editor açık, plugin enable. 5–15: `pie_start {players:2, netMode:"listen"}` → 2 pencere. 15–30: `pie_input {action:"Jump", player:1}` → karakter zıplıyor. 30–45: `pie_actors` → replikasyon doğrulama JSON. 45–60: `pie_stop` → kapat. | **MP test otomasyonu** |
| **V2** | `game_run` + `screenshot` + `image_stats` | 0–5: `game_run {map:"/Game/Maps/Demo", culture:"tr", cvars:["r.Lumen.DiffuseIndirect.Allow 0"]}`. 5–20: Oyun başlıyor, log takibi. 20–35: `screenshot` → PNG. 35–50: `image_stats {reference:"ref.png"}` → SSIM/rms/luma histogram. 50–60: Verdict PASSED. | **Paketli oyun çalıştırma + görsel QA** |
| **V3** | `screenshot_ab` + `image_compose` | 0–10: Ayar değiştir (ışık rengi). 10–25: `screenshot` (önce). 25–40: Ayar geri al + `screenshot` (sonra). 40–55: `screenshot_ab {a:"before.png", b:"after.png"}` → diff görseli + değişen piksel %. 55–60: `image_compose` yan yana. | **A/B görsel regresyon** |
| **V4** | `loc_gather` + `stringtable_compare` + `font_coverage` | 0–10: `loc_gather {target:"Game", export_po:true}`. 10–25: PO dosyası düzenle (eksik çeviri). 25–40: `stringtable_compare {source:"loc_tr.csv", target:"Game.locres"}` → eksik/fuzzy/format farkları. 40–55: `font_coverage {font:"Roboto", culture:"tr"}` → çizilemeyen karakterler. 55–60: `game_run {culture:"tr", glyphMisses:true}` → oyun içi glyph miss sayımı. | **Lokalizasyon QA zinciri** |
| **V5** | `asset_pipeline` + `skeletal_import` | 0–10: Klasör seç (Tripo FBX'ler). 10–30: `asset_pipeline {source:"../TripoOut", target:"/Game/RagnaCook/Props", skeleton:"Rig_Medium", physics:true, sockets:["Muzzle"]}` → import raporu. 30–50: `skeletal_import` hedef skeleton bind + IK retarget + Physics Asset + soket. 50–60: `actor_preview` → oyunda karakter. | **Sanat hattı tek komut** |
| **V6** | `batch` + `validate` + `doctor` | 0–10: Yanlış anahtarlı `object_set` çağrısı → hata + öneri. 10–25: `batch {steps:[{tool:"asset_rename",...}, {tool:"object_set",...}], rollback:true}` → atomik uygulama. 25–40: `validate {blueprint:true, data:true, map:true, references:true}` → rapor. 40–55: `doctor` → sağlık raporu (Zen oturum, kirli config, derleme). 55–60: `client_config --all` → 5 istemci config dosyası. | **Güvenilirlik + kurulum** |

**Üretim:** Her video OBS/UE MRQ ile 1080p/60fps, sesli anlatım (EN), altyazı (EN/TR). YouTube + X/Twitter + Reels/Shorts.

---

## 4. Topluluk ve Katkıcı Kazanımı

| Eylem | Detay | Zamanlama |
|---|---|---|
| **Issue şablonları** | Bug report, Feature request, Tool request (zaten `docs/good-first-issues.md` + `.github/ISSUE_TEMPLATE/`) | Lansman öncesi |
| **Good First Issue** | 5 etiketli issue: `docs: add tool example`, `test: add pie_scenario fixture`, `ci: add windows runner`, `tool: add datatable_csv export`, `docs: translate guide to TR` | Lansman günü |
| **GitHub Discussions** | Q&A, Showcase, Ideas kategorileri (Discord **yok** — `SORULAR.md` S-6 öneri: hayır) | Lansman günü |
| **Katkıcı rehberi** | `CONTRIBUTING.md` zaten var; `REQUEST_WITH_AI.md` AI ile katkı kılavuzu | Güncelle |
| **Release notları** | Her v0.x.y için CHANGELOG + GitHub Release notları (breaking/fix/new) | Her release |

---

## 5. Ölçüm ve Analitik

| Metrik | Araç | Sıklık |
|---|---|---|
| Yıldız sayısı / büyüme | star-history.com + `gh api repos/shanyup/enginewright/stargazers` | Haftalık |
| Trafik / Referrer | GitHub Insights → Traffic (Views, Clones, Referrers) | Haftalık |
| Clone sayısı | GitHub Insights | Haftalık |
| Demo video izlenme | YouTube Analytics + X Analytics | Her video sonrası |
| Issue/PR sayısı | GitHub Insights → Issues/PRs | Haftalık |
| Registry/liste görünürlüğü | Glama score, awesome-mcp-servers PR durumu | Olay bazlı |

---

## 6. YAPMA — Yasak / Riskli Taktikler (Açıkça)

1. ❌ Yıldız satın alma / bot / click farm (GitHub ToS ihlali, repo kilitlenir).
2. ❌ Spam: aynı mesajı birden fazla subreddit/Discord'a kopyalama (ban riski).
3. ❌ Astroturf: sahte hesaplarla yorum/upvote (Reddit/GitHub algılar).
4. ❌ Yanıltıcı başlık: "Official Unreal MCP" (Epic markası), "Microsoft/Google backed" (yok).
5. ❌ Telemetri / kullanıcı verisi toplama izinsiz (blender-mcp dersi: telemetri itirazı aldı, kaldırdı).
6. ❌ Repo adını sürekli değiştirme (yönlendirme 1 yıl, sonra kırılır; SEO zarar verir).
7. ❌ Private/kapalı beta ile hype yapıp public açmama (şeffaflık ilkesi).
8. ❌ Lisans değiştirme (MIT → proprietary) topluluk onayı olmadan.

---

## 7. Bütçe ve Kaynaklar

- **Para:** $0 (organik, ücretsiz kanallar).
- **Zaman:** Talha ~2 saat/hafta (duyuru yazımı, video çekimi, topluluk cevapları).
- **Araçlar:** OBS (kayıt), MRQ (UE render), ffmpeg (mp4), GitHub CLI (`gh`), `gh api` (star çekimi).
- **Risk azaltma:** Her duyuru öncesi "dry run" (özel repo/klasörde test), yanlışlıkla secret push etmemek için `git-secrets` / pre-commit hook.