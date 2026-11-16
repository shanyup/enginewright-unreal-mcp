# 05 — Duyuru ve Yıldız Büyüme Planı (90 gün)

Tarih: 16.11.2026. Başlangıç: **10★**. Hedefler emsal verisine dayalı (tahmin aralıkları). Kanallar kuralıyla birlikte; kural ihlali = ban riski.

## 1. Yıldız hedefleri (emsal: bkz 01 §2)

| Süre | Hedef ★ | Gerekçe |
|---|---|---|
| 30 gün | 80-150 | registry + awesome liste + 2-3 subreddit + 5 video; viral olmadan organik taban (blender-mcp 10k'ya ~2 ayda viral ile vardı; biz viralsiz) |
| 90 gün | 300-600 | + HN Show HN (1.0'da), + X thread, + newsletter; playwright 32k/yıl kurumsal — biz tek geliştirici: %1-2'si makul |
| 180 gün | 800-1.500 | + Fab listing + UE forumları + UE 5.9 desteği; unity-mcp 16k (2025'ten beri) — UE'de henüz kimse 3k'yı geçemedi (ChiR24 2.718) → **ilk 3k UE MCP biz olabiliriz** |

Ölçüm: star-history.com haftalık, GitHub repo Insights → Traffic (referrers, uniques), `repos.txt` API'si ile haftalık snapshot.

## 2. 90 günlük takvim (hafta bazlı)

| Hafta | Kanal | Aksiyon | Başlık örneği (EN) | Kural |
|---|---|---|---|---|
| 1 | GitHub | SEO paketi uygula (04 §1): topics, açıklama, social preview, Pages site iskeleti | — | repo değişikliği Talha onayıyla |
| 1 | Registry | `server.json` + `mcp-publisher publish` (resmi MCP registry) | — | önce paket doğrulaması; GH auth namespace `io.github.shanyup/*` |
| 2 | Glama + awesome-mcp-servers | Glama submit → score badge → awesome PR | — | badge olmadan PR reddedilir (CI check) |
| 2 | mcp.so + cursor.directory | submit form / issue | — | free tier; ücretli tier YOK |
| 2-3 | X/Twitter | 5 demo video (30-60 sn) + thread | "Show HN-style": "I built an MCP server for Unreal Engine — your AI agent now compiles, runs PIE and packages your game" | GIF/video ile; link sonda; hashtag #UE5 #gamedev #MCP #AIagents |
| 3 | Reddit r/unrealengine (292K) | tek post: "Enginewright — MCP server for UE 5.8, 129 tools, MIT" + demo GIF | "Enginewright: an open-source MCP server that lets AI agents drive Unreal Editor (PIE, packaging, screenshots)" | dev-for-dev; consumer marketing YOK; self-promo ~%10 sınırı; yorumlara cevap ver |
| 3 | Reddit r/gamedev (2M) | Marketing Monday thread'e yorum | — | showoff yasak; MM thread-only |
| 4 | Reddit r/ClaudeAI, r/LocalLLaMA | "Claude now runs my Unreal playtests" postu | "I gave Claude Code 129 tools for Unreal Engine 5.8 — it runs PIE and packages builds" | kural: değer + demo; spam yok |
| 4 | YouTube | 3 kısa (30-60 sn) + 1 uzun (3-5 dk) demo | "Playwright for Unreal Engine" | ilk 3 sn hook: ekranda agentın PIE başlattığı an |
| 5 | dev.to / Medium | makale: "How we automated UE packaging and playtests with MCP" | — | teknik, kod örnekli |
| 6 | Hacker News | **Show HN** (1.0 lansmanıyla, tek gönderim) | "Show HN: Enginewright — MCP server for Unreal Engine 5.8" | kural: denenebilir olmalı, signup yok, personally worked on, başlık "Show HN"; upvote/comment solicitation YOK; 2026'da Show HN kısıtlı olabilir → showlim'e dikkat; major release'de |
| 6 | Unreal Engine Forums | "Tools & Plugins" bölümüne post + demo link | — | Epic forum kuralı: self-promo makul, spam yok |
| 7 | Discord: UE Discord, MCP Discord | #showcase kanalına video | — | sunucu kurallarına uy |
| 8 | Newsletter | TLDR / Hacker Newsletter pitch (editöre mail) | — | editöre, spam değil |
| 9 | Product Hunt | launch (1.0 ile) | "Enginewright — Playwright for Unreal Engine" | PH kuralı: launch günü erken saat; maker aktif cevap verir |
| 10-13 | Hepsi | yorumlara cevap, issue'lara ilk yanıt <24 saat, haftalık star/traffic snapshot, 2. video dalgası (yeni araçlar) | — | ölçüm: star-history + Insights traffic |

## 3. İçerik planı — demo videoları (30-60 sn senaryolar)

1. `pie_scenario`: agent PIE'i başlatıyor, aktör spawn edip input veriyor, assert geçiyor (EW-6).
2. `package_build`: tek komut → cook/package, süre logu (EW-10).
3. `world_inventory` + `actor_set_props`: 50 aktörün property'si 2 dk'da değişiyor (EW-1/2).
4. `screenshot_tour` + `visual_review`: sahne turu, grid inceleme (RC-3/4).
5. `datatable_csv`: 357 affix CSV roundtrip (EW-8).
6. (1.0 ile) kurulum: plugin + `client_config` → Claude Code bağlanıyor, ilk komut.

## 4. Topluluk temeli

- Issue şablonları: bug report, feature request, tool request (hazır: `docs/good-first-issues.md`).
- Label'lar: `good first issue`, `tool`, `docs`, `UE 5.9`.
- İlk yanıt < 24 saat; haftalık triage.
- Discord: SORULAR S-6 (öneri: hayır, GitHub Discussions yeterli ilk 6 ay).
- Katkıcı: CONTRIBUTING EN, "add one tool" kılavuzu (araç kayıt deseni: `R.Register(TEXT("..."))`).

## 5. YAPMA — yasak/riskli taktikler

1. Yıldız/star satın alma, star-exchange grupları — kalıcı ban + itibar kaybı.
2. HN/Reddit'te upvote veya yorum solicitation ("please upvote") — HN kuralı açıkça yasak.
3. Astroturf (sahte hesaplarla yorum), spam DM.
4. "Ücretsiz hemen yayın" tier'larına ödeme (mcp.so vb.).
5. Her gün aynı kanala aynı içerik (spam algısı).
6. Rakip projeler hakkında negatif yorum — 01'deki karşılaştırma yeterli, saygılı kal.
