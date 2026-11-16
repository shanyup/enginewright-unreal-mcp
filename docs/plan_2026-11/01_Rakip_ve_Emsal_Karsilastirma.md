# 01 — Rakip ve Emsal Karşılaştırması

Tarih: 16.11.2026. Yıldız/push/lisans: GitHub REST API, 16.11.2026 (ham veri `$TMPDIR/ewplan/repos.txt`). Özellik hücreleri: `docs/arastirma/derin/01–04` (10.11, kaynak kodu okuması) + web. "Tahmin" = doğrulanmamış. "—" = veri yok.

## 1. UE ekosistemi — rakip tablosu

| Proje | ★ (16.11) | Son push | Lisans | Mimari / kurulum | Araç sayısı | UE sürüm | Doğrulama/geri alma | Görsel doğrulama | PIE / oyun testi | Doküman |
|---|---|---|---|---|---|---|---|---|---|---|
| **Epic resmi Unreal MCP** (UE 5.8, Experimental, `dev.epicgames.com` 5.8 dokümanı) | motor içi | 5.8 yayında | Epic lisansı | Editör içi plugin; AllToolsets; `http://127.0.0.1:8000/mcp`; localhost, auth yok; tek tık enable + restart | ~870 araç / ~50 toolset (`epic_ue58_inventory.json` sayımı) | 5.8+ | Logs toolset var; geri alma — | — | AutomationTest toolset (7 araç); PIE yok | resmi, iyi |
| **Enginewright (biz)** github.com/shanyup/enginewright | **10** | 16.11 | MIT | Editör içi C++ plugin, MCP over HTTP; plugin klasörü + enable, `client_config` ile tek komut | **129** (kod sayımı 16.11, `tools.txt`) | 5.8 | `validate`, `batch`, `doctor`, `id_audit`, `key_audit`, `ui_input_audit` | `screenshot`, `editor_capture`, `screen_coverage`, `image_stats`, `image_compose` | `pie_*` (8 araç), `game_run`, `game_run_status`, `pie_net_emulation` | README EN; iç docs TR |
| ChiR24/Unreal_mcp | 2.718 | 15.11 | MIT | Python sunucu + editör plugin köprüsü (topic'lardan, tahmin); pip + plugin + client config | derin raporda | 5.x (tahmin) | — | — | — | iyi README + demo |
| chongdashu/unreal-mcp | 2.508 | 07.11 | MIT | Python MCP sunucu + UE plugin (tahmin) | derin raporda | 5.x (tahmin) | — | — | — | iyi |
| flopperam/unreal-engine-mcp | 1.316 | 12.11 | MIT | — | — | — | — | — | — | — |
| runreal/unreal-mcp | 1.291 | 16.11 | MIT | — (aktif) | — | — | — | — | — | — |
| kvick-games/UnrealMCP | 536 | 30.10 | MIT | — | — | — | — | — | — | — |
| db-lyon/ue-mcp | 340 | 14.11 | MIT | — | — | — | — | — | — | — |
| IvanMurzak/Unreal-MCP | 287 | 15.11 | MIT | — | — | — | — | — | — | — |
| tumourlove/monolith | 151 | 16.11 | MIT | — | — | — | — | — | — | — |
| kevinpbuckley/VibeUE | 146 | 15.11 | MIT | — | — | — | — | — | — | — |
| Natfii/UnrealClaude | 114 | 16.11 | — | — | — | — | — | — | — | — |
| GenOrca/unreal-mcp | 93 | 16.11 | Apache-2.0 | — | — | — | — | — | — | — |
| prajwalshettydev/UnrealGenAISupport | 69 | 16.11 | MIT | — | — | — | — | — | — | — |
| believer-oss/Claireon | 16 | 16.11 | MIT | — | — | — | — | — | — | — |
| EngineWire (3rd-party, RagnaCook'ta kurulu, port 3000) | alınmadı | — | — | harici sunucu | — | — | — | — | — | — |

Not: GitHub "unreal mcp" araması (sort=stars, 16.11) ilk 15'i yukarıdaki tabloya denk; sonrası kırpma (bkz API log).

## 2. Diğer motorlarınatis — neden yıldız aldılar (emsal)

| Proje | ★ (16.11) | Neden büyüdü (kaynak: web, 16.11) | Büyüme notu | Dersimiz |
|---|---|---|---|---|
| ahujasid/mcp-for-blender (eski adıyla blender-mcp) | 32.730 (API) / 34.3k (star-history 14.11) | 7 Mar 2025'te açıldı; demo videolar; Rodin/Hyper3D entegrasyonu (13-16 Mar 2025); AI Engineer World's Fair 2025'te sunum (11k★ o an); HN + GitHub Trending (Eki 2026'te #6) | 10k★'a May 2025'te ulaştı (~2 ay) | tek lider + video + entegrasyonlar + trending dalgası |
| microsoft/playwright-mcp | 39.142 | 22 Mar 2025 lansman; accessibility tree (piksel yerine structured snapshot); Simon Willison yazısı; VS Code Copilot agent modunda default (VS Code 1.99); Claude Code'a resmi entegrasyon | 32k+★ ilk yıl içinde | kurumsal destek + hazır client entegrasyonu = katmanlı büyüme |
| upstash/context7 | 42.928 | dokümantasyon MCP; tek satır kurulum | — | docs =_growth kanalı |
| modelcontextprotocol/servers | 114.064 | resmi referans liste | — | liste itself = trafik |
| punkpeye/awesome-mcp-servers | 56.441 | Glama score badge zorunlu (CI check); sadece GitHub-hosted, installable server | — | listeye girmek = kalite damgası |
| CoplayDev/unity-mcp | 16.285 | git URL ile tek komut kurulum (Package Manager); ACM SA'25 makalesi; resmi MCP registry'de (`mcp/coplaydev/unity-mcp`); Autopilot duyurusu; Discord topluluğu | 14.8k (OSS Insight) ile 16k arası kaynak farkı | akademik makale + registry + tek komut kurulum |
| IvanMurzak/Unity-MCP | 11.231 | C# köprü, çok motor | — | — |
| Coding-Solo/godot-mcp | 6.445 | Godot topluluğu, tek komut | — | nişe motor = az rekabet |
| ChromeDevTools/chrome-devtools-mcp | 17.087 | Google resmi, 2025 lansman | — | resmi destek = güven |
| GLips/Figma-Context-MCP | 13.868 | tasarımcı kitlesi, viral tweetler | — | dikey (vertical) kitle büyütür |
| github/github-mcp-server | 21.086 | GitHub resmi | — | platform entegrasyonu |

## 3. Özellik matrisi (satır: kategori, sütun: oyuncu)

Değerler: ✅ var, 🟡 kısmi, ❌ yok. Enginewright sütunu 16.11 kod sayımından (129 araç); Epic sütunu `epic_ue58_inventory.json`; community sütunu derin raporlardan (10.11); emsal sütunları kendi README/dokümanlarından.

| Kategori | Enginewright 0.4.x | Epic UE 5.8 MCP | Community UE MCP (ChiR24/chongdashu) | unity-mcp | mcp-for-blender | playwright-mcp |
|---|---|---|---|---|---|---|
| Aktör spawn/taşı/sil/bul | ✅ (actor_*) | ✅ ActorTools 17 | ✅ | ✅ | ✅ | n/a |
| Asset import/rename/references | ✅ (asset_*) | ✅ AssetTools 21 | 🟡 | ✅ | 🟡 (import yok, create var) | n/a |
| Blueprint düzenleme (node ekle/sil/bağla) | ✅ (blueprint_*, 10+ araç) | ✅ BlueprintTools 53 | 🟡 | ✅ | ❌ (kod çalıştırır) | n/a |
| DataTable / CSV | 🟡 datatable_* var, CSV yok | ✅ DataTableTools 10 | 🟡 | ✅ | ❌ | n/a |
| StringTable / loc | ✅ (stringtable_*, loc_*) | ✅ StringTableTools 8 | ❌ | 🟡 | ❌ | n/a |
| Malzeme/doku | ✅ (material_*, texture_info) | ✅ MaterialTools 22, TextureTools 2 | 🟡 | ✅ | 🟡 | n/a |
| PIE başlat/durdur/oyun içi input | ✅ pie_* 8 araç | ❌ (sadece automation test) | 🟡 | ✅ (playmode) | n/a | n/a |
| Paketli oyun çalıştırma (game_run) | ✅ game_run/status | ❌ | ❌ | 🟡 | n/a | n/a |
| Ağ / replikasyon test | 🟡 pie_net_emulation tek | ❌ | ❌ | 🟡 | n/a | n/a |
| Cook / package (build) | ❌ | ❌ | ❌ | ❌ | n/a | n/a |
| Screenshot / görsel doğrulama | ✅ screenshot, editor_capture, image_stats | ❌ | 🟡 | ✅ | ✅ viewport screenshot | ✅ screenshot + a11y snapshot |
| Perf ölçüm | ✅ perf_*, mrq_* | 🟡 | ❌ | 🟡 | ❌ | ✅ (traces) |
| Log / crash analizi | 🟡 log_read/log_wait, analiz yok | ✅ LogsToolset 4 | 🟡 | 🟡 | ❌ | ✅ console/network log |
| UI/Slate otomasyonu | ✅ ui_*, widget_* (~20 araç) | ✅ SlateInspector 14 | 🟡 | 🟡 | ❌ | ✅ a11y tree + click |
| Doğrulama / geri alma / audit | ✅ validate, batch, doctor, 3 audit | 🟡 logs only | ❌ | ❌ | ❌ | ✅ deterministic refs |
| Toplu (batch) işlem | ✅ batch | ❌ | ❌ | ❌ | ❌ | n/a |
| Client config üretimi (tek komut kurulum) | ✅ client_config | ✅ GenerateClientConfig komutu | 🟡 manuel | ✅ "Configure All Clients" | ✅ tek JSON | ✅ npx |
| Doküman dili | EN README + TR iç docs | EN resmi | EN | EN + site | EN | EN |
| Lisans | MIT | Epic | MIT çoğunluk | MIT | MIT | Apache-2.0 |

## 4. Nerede öndeyiz / gerideyiz / kimsede yok

| Durum | Özellik | Not |
|---|---|---|
| **Öndeyiz** | PIE + oyun içi test (`pie_*`) | Epic'te yok; community'de zayıf |
| **Öndeyiz** | Paketli oyun çalıştırma (`game_run`) | kimsede yok |
| **Öndeyiz** | Görsel doğrulama zinciri (screenshot → image_stats → compose) | Epic'te yok |
| **Öndeyiz** | Doğrulama/geri alma/audit (`validate`, `batch`, `doctor`, audit'ler) | kimse bu kadar yok |
| **Öndeyiz** | Loc/StringTable | sadece Epic'te var |
| **Öndeyiz** | Tek komut client config (`client_config`) | playwright ve unity-mcp kadar otomatik |
| **Gerideyiz** | Toplam araç sayısı (129 vs Epic ~870) |beklenen; her sürüm kapanır |
| **Gerideyiz** | Yıldız (10 vs 2.718 en yakın rakip) | 05'te plan |
| **Gerideyiz** | Registry/liste görünürlüğü | henüz hiçbir listede yok |
| **Gerideyiz** | Demo video / viral içerik | yok |
| **Kimsede yok** | Cook/package tek komut | kategori boş (Epic dahil) |
| **Kimsede yok** | Sahne snapshot + diff | kategori boş |
| **Kimsede yok** | Görsel inceleme grid'i (N screenshot toplu karşılaştırma) | kategori boş |
| **Kimsede yok** | 2-instance co-op test otomasyonu | kategori boş |

## 5. Epic resmi MCP ile çakışma riski ve farklılaşma

Risk: UE 5.9/6.0'da Experimental → stabil olunca "resmi" araç olur; Epic'in toolset'leri genişler, genel "editör otomasyonu" ayağı daralır.

Farklılaşma stratejisi (karar verilebilir):

1. **Oyun iş akışlarına odaklan, editör CRUD'ına değil**: cook/package, PIE senaryoları, replikasyon/ağ testi, görsel QA, crash analizi. Epic oralarda zayıf (bkz matris satırları ❌).
2. **"Tek geliştirici = haftalık release"** — Epic release trenine bağlı değil; şeffaf roadmap + CHANGELOG.
3. **Keşif kanalları**: resmi MCP registry, awesome-mcp-servers, Glama — Epic listelerde yok, biz gireceğiz (05/04).
4. **Kurulum kolaylığı + demo video**: tek komut `client_config`, 30-60 sn videolar.
5. **Konumlandırma cümlesi**: "Playwright for Unreal" (zaten README'de) — Playwright MCP'nin 39k★'lık modeli: structured state (bizde: JSON envanter + screenshot zinciri), determinism (bizde: validate/batch), hazır client entegrasyonu.
