# 04 — Keşfedilebilirlik / SEO (GitHub + liste + site)

Tarih: 16.11.2026. **Kart kuralı: GitHub'da hiçbir değişiklik YOK** — bu dosya öneridir; uygulama Talha onayı + Opus push sonrası. Limitler kaynak: GitHub Docs + aramalar 16.11.

## 1. GitHub repo ayarları (onay sonrası uygulanacak)

| Alan | Limit (kaynak) | Öneri |
|---|---|---|
| Açıklama (description) | ≤350 karakter | Öneri metin: "Unreal Engine 5.8 editor MCP server — AI agents (Claude, Codex, Cursor) drive the editor: compile, PIE, packaging, screenshots, level inspection. The 'Playwright for Unreal'." (mevcut açıklama zaten bu; doğrula ≤350) |
| Topics | max 20 | Mevcut 20 topic var. Arama hacmi gerekçesiyle seçim (tahmin hacimler): `unreal-engine`, `unreal-engine-5`, `ue5`, `mcp`, `mcp-server`, `model-context-protocol`, `ai-agents`, `llm`, `claude`, `game-development`, `gamedev`, `unreal`, `editor-plugin`, `codegen`, `agents`, `claude-desktop`, `game-dev`, `unreal-engine-58`, `ai`. Not: `unreal-mcp` ve `ue5-mcp` gibi spesifik terimler ek arama hacmi taşır — yer açmak için düşük hacimli 2-3 topic çıkarılabilir (karar SORULAR) |
| Sosyal önizleme görseli | 1280x640 en iyi, <1 MB, PNG/JPG/GIF | 1 görsel: logo + "Playwright for Unreal" + tek komut kurulum satırı; `docs/media/` içine konur, Settings'ten yüklenir |
| Release notları + tag | — | Her sürümde tag (v0.4.1...) + CHANGELOG özeti; GitHub Releases "latest" rozeti çeker |
| README ilk ekran | — | 1 cümle değer + GIF (30-60 sn demo) + tek komut kurulum + mini rakip tablosu (01'den) + yıldız rozeti |
| About / website | — | Website: GitHub Pages sitesi (aşağıda) |

## 2. Liste kayıtları (her birinin kabul kuralı + link) — hepsi onay sonrası

| Liste | Kabul kuralı (16.11, kaynak) | Aksiyon | Link |
|---|---|---|---|
| Resmi MCP Registry | `server.json` (name `io.github.shanyup/enginewright`, ≤100 krk açıklama), `mcp-publisher login github` + `publish`; paket doğrulaması; GitHub Actions ile otomatik publish opsiyonel (dikkat: `pull_request_target` tuzağı) | v1.0'da publish; PulseMCP ve diğerleri buradan auto-ingest eder | modelcontextprotocol.io/registry/quickstart |
| awesome-mcp-servers | Sadece GitHub-hosted, installable server; **Glama listing + Glama score badge zorunlu** (CI check); alfabetik, tek satır | Glama'ya önce kaydol, badge ekle, PR aç | github.com/punkpeye/awesome-mcp-servers (CONTRIBUTING) |
| Glama | Sunucu başlar + introspection (tools/list) geçmeli; Dockerfile direkt Glama'ya | submit + score al | glama.ai/mcp/servers |
| PulseMCP | **Kapalı** (2026-09-04: "not accepting submissions"); önerileri: resmi registry'ye publish et, otomatik alınır | resmi registry után re-check (1 ay sonra) | pulsemcp.com/submit |
| mcp.so | Submit formu veya GitHub issue (chatmcp/mcp-directory); free tier review kuyruğu; **ücretli "hemen yayın" tier'ına para verilmez** | form/issue | mcp.so/submit |
| Smithery | Local/stdio sunucular için **MCPB bundle** (manifest.json + kod, `npx @anthropic-ai/mcpb`); URL yöntemi sadece hosted | Enginewright editör içi plugin → MCPB uygun değil; **değerlendirme: atla veya remote mod sonrası** | smithery.ai/new |
| cursor.directory | `.mcp.json` ile client config | `client_config` aracı zaten üretiyor → ekle | cursor.directory |
| mcp.tools / Cline marketplace | GitHub issue ile submit | issue aç | — |
| awesome-unreal / awesome-ue5 listeleri | Coop56/awesome-unreal (AI bölümü var), insthync/awesome-unreal (230 kaynak), tomByrer/awesome-unreal-engine | PR: "AI" kategorisine ekle | github.com/Coop56/awesome-unreal, insthync/awesome-unreal |

## 3. GitHub Pages dokümantasyon sitesi

- `docs-site/` (veya Pages branch): EN, anahtar kelimeler sayfa başlıklarında.
- Anahtar kelime listesi (Google, EN): `unreal engine mcp`, `unreal engine mcp server`, `ue5 ai agent`, `claude unreal engine`, `unreal engine 5.8 mcp`, `ai agent unreal editor`, `mcp unreal plugin`, `model context protocol unreal`, `automate unreal engine`, `ue5 automation testing`.
- Sayfalar: Install (tek komut) / Tools (129 araç tablo) / Demos (videolar) / Roadmap / FAQ / vs Epic MCP (01 §5 özeti).

## 4. Fab / Marketplace uygunluğu (bilgi + karar SORULAR S-2)

- Marketplace kuralı 1.2.4.b: **free fiyat sadece Code Plugin** için → Enginewright free code plugin olarak listelenebilir (uygun).
- Teknik: `.uplugin` + `Source` + zip; **her UE sürümü ayrı zip**; Fab otomatik build eder; ≤15 GB; indirme linki permission'sız olmalı.
- Akış: Fab account → listing → upload → review → publish. Süre: günler (tahmin).
- Karar: v1.0 ile birlikte, tek sürüm (5.8).

## 5. Dokümantasyon dili ve Türkçe iç belgeler (SORULAR S-1)

- Public repo: README + docs-site **İngilizce** (global kitle, SEO EN).
- Türkçe iç belgeler (`docs/plan_2026-11/`, roadmap taslakları, EKSIKLER...) public repoda durursa: karışık görünüm + SEO gürültüsü. Öneri: iç notlar ya private repoya ya da `docs/internal/` + robots/noindex; ya da ayrı private repo. Bu kart bunları yerelde tutuyor — push edilirse public olur. **Karar Talha'da.**
