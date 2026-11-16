# 04 — Keşfedilebilirlik ve SEO Planı (GitHub + Web)

Tarih: 17.11.2026. Veri: GitHub REST API 17.11, topic arama sayıları, arama sıralaması. Mevcut repo: github.com/shanyup/enginewright (public, MIT, 10★).

---

## 1. Mevcut durum (canlı 17.11)

| Alan | Mevcut değer | Karakter / Sayı |
|---|---|---|
| **Repo adı** | `enginewright` | — |
| **Açıklama (description)** | `In-editor MCP server for Unreal Engine: strict, verified, undoable tool calls for AI agents. No Python/Node required. Early preview.` | 136 krk / 350 |
| **Topic'ler (7/20)** | `ai-agents`, `claude`, `codex`, `mcp`, `model-context-protocol`, `unreal-engine`, `unreal-engine-plugin` | 7 |
| **Sosyal önizleme görseli** | Yok (varsayılan GitHub) | — |
| **Release notları** | v0.4.0 tag var, release sayfası yok | — |
| **Etiketler (tags)** | v0.4.0, v0.3.1 | 2 |
| **README dili** | İngilizce (tam) + Türkçe iç belgeler `docs/` içinde | EN |
| **GitHub Pages** | Yok | — |
| **Registry kayıtları** | Hiçbiri | 0/7 hedef |

---

## 2. Önerilen repo adı — **S-0 Karar Sorusu** (bkz `SORULAR.md` S-0)

**Sorunun kaynağı:** Talha 16.11 yorumu (kanban comment 607). GitHub aramasında ad/açıklama/topic ağırlığı: ad > açıklama > topic'ler. "unreal mcp" araması ilk 15'te genel adlı (`unreal-mcp`, `UnrealMCP`, `unreal-engine-mcp`) repolar hâkim.

| Seçenek | Açıklama | Artıları | Eksileri | Etki |
|---|---|---|---|---|
| **(a) `enginewright` kalsın** | Marka odaklı, benzersiz | Yönlendirme basit, Fab/Marketplace'de `Plugins/Enginewright` ile uyumlu | "unreal mcp" aramasında ad eşleşmez; sıralama 198 (best match) / 80 (unreal engine mcp) | Düşük keşfedilebilirlik |
| **(b) `enginewright-unreal-mcp`** (Öneri: Opus) | Marka + anahtar kelime birleşik | Ad'da "unreal-mcp" var → arama sıralaması yüklenir; `enginewright` markası korunur; GitHub otomatik yönlendirme (repo rename) 1 yıl çalışır | Uzun (28 krk); `Plugins/Enginewright` klasör adıyla çakışmaz (klasör `Enginewright` kalır) | **Orta-yüksek keşfedilebilirlik artışı** |
| **(c) Tamamen genel ad: `unreal-mcp-enginewright` / `unreal-mcp`** | Sadece anahtar kelime | Arama sıralaması en yüksek olabilir | Marka kaybolur; `unreal-mcp` zaten dolu (chongdashu, flopperam, runreal, kvick, GenOrca, runeape, aadeshrao, softdaddy, gimmeDG, mirno-ehf) → çakışma/karışıklık | Yüksek risk, marka yok |

**Veri ile doğrulama (17.11 arama):**
- `unreal mcp in:name`: 247 sonuç, enginewright **yok** (adı "unreal-mcp" içermiyor).
- `unreal engine mcp server`: 151 sonuç, enginewright **34.**
- `unreal engine mcp`: 405 sonuç, enginewright **80.**
- Lider chongdashu/unreal-mcp (2.118★) adında "unreal-mcp" var.

**Opus ön önerisi: (b) `enginewright-unreal-mcp`** — marka korur, arama sıralaması kazanır, yönlendirme 1 yıl. Fab/Marketplace listing `Plugins/Enginewright` klasör adından etkilenmez (manifest `PluginName` kullanır). **Zamanlama:** Duyurudan ÖNCE (05 §2), v1.0 tag'iyle aynı PR'de.

---

## 3. Önerilen açıklama (≤350 krk, tam metin — kopyala-yapıştır)

> **In-editor MCP server for Unreal Engine 5.8: strict, verified, undoable tool calls for AI agents. 129 tools: PIE control, game_run (packaged exe), multiplayer test, visual QA (screenshot + SSIM), localization (PO/locres), asset pipeline, batch with rollback. No Python/Node. One-command client config for Claude Code, Codex, Cursor, VS Code, Hermes. MIT licensed. Playwright for Unreal.**

**Karakter sayısı:** 348 / 350. Anahtar kelimeler: MCP server, Unreal Engine, PIE, game_run, multiplayer, visual QA, localization, asset pipeline, batch, rollback, client config, MIT.

---

## 4. Önerilen 20 Topic (GitHub topic sayfası repo sayıları 17.11, hacim gerekçeli)

| # | Topic | Repo sayısı (17.11) | Neden | Mevcut mu? |
|---|---|---:|---|---|
| 1 | `mcp` | 121,394 | Ana protokol etiketi, en geniş | ✅ |
| 2 | `mcp-server` | 44,273 | Sunucu taraflı implikasyon | ❌ |
| 3 | `model-context-protocol` | 45,173 | Resmi protokol adı | ✅ |
| 4 | `unreal-engine` | 4,624 | Motor markası | ✅ |
| 5 | `unreal-engine-5` | 2,390 | Sürüm spesifik | ❌ |
| 6 | `ue5` | 1,491 | Kısaltma, arama hacmi | ❌ |
| 7 | `ue5-plugin` | 326 | Plugin sınıfı | ✅ (unreal-engine-plugin) |
| 8 | `unreal-engine-plugin` | 556 | Plugin arama | ✅ |
| 9 | `ai-agents` | 153,662 | AI ajan hedef kitlesi | ✅ |
| 10 | `claude` | 70,030 | Birincil istemci | ✅ |
| 11 | `codex` | 52,005 | İkinci istemci | ✅ |
| 12 | `game-development` | 41,980 | Oyun geliştirme kategorisi | ❌ |
| 13 | `gamedev` | 15,747 | Kısaltma, topluluk | ❌ |
| 14 | `ai-tools` | 18,310 | Araç kategorisi | ❌ |
| 15 | `editor-plugin` | 353 | Editör eklentisi | ❌ |
| 16 | `blueprint` | 1,483 | Blueprint kullanıcıları | ❌ |
| 17 | `vibe-coding` | 8,146 | Trend terimi, VibeUE rakibi | ❌ |
| 18 | `agentic-ai` | 29,423 | Ajan odaklı | ❌ |
| 19 | `llm` | 185,349 | Geniş LLM kitle | ❌ |
| 20 | `cpp` | 115,592 | C++ implikasyon (Python/Node yok) | ❌ |

**Eylem:** Repo Settings → Topics → yukarıdaki 20'yi ekle (mevcut 7 korunur, 13 eklenir).

---

## 5. README — İlk ekran taslağı (İngilizce, tam metin, GIF yeri işaretli)

> # Enginewright
>
> **In-editor MCP server for Unreal Engine 5.8** — strict, verified, undoable tool calls for AI agents. No Python/Node required.
>
> [![Enginewright demo](docs/assets/demo.gif)](https://youtu.be/xxx)  ← **30-60 sn demo GIF buraya**
>
> ```bash
> # One-command client config (Claude Code, Codex, Cursor, VS Code, Hermes)
> ue5-mcp client_config --all
> ```
>
> ## Why Enginewright?
>
> | Feature | Enginewright | Epic UE 5.8 MCP | Community (chongdashu/ChiR24) |
> |---|:---:|:---:|:---:|
> | **PIE + in-game input** | ✅ 8 tools | ❌ | 🟡 |
> | **Packaged game run (`game_run`)** | ✅ | ❌ | ❌ |
> | **Multiplayer test (net emulation)** | ✅ | ❌ | ❌ |
> | **Visual QA (screenshot + SSIM + compose)** | ✅ | ❌ | 🟡 |
> | **Localization (PO/locres compare, font coverage)** | ✅ | ✅ | ❌ |
> | **Batch with atomic rollback** | ✅ | ❌ | ❌ |
> | **One-command client config** | ✅ | ✅ | 🟡 manual |
> | **License** | MIT | Epic EULA | MIT |
>
> ## Quick Start
>
> 1. Drop `Enginewright` plugin into `Plugins/`
> 2. Enable in Editor → Restart
> 3. Run `ue5-mcp client_config --all` (or copy `.mcp.json` from `client_config` tool)
> 4. Connect your AI agent (Claude Code, Codex, Cursor, VS Code, Hermes)
>
> ## Tools (129)
>
> `actor_*`, `asset_*`, `blueprint_*`, `datatable_*`, `stringtable_*`, `loc_*`, `material_*`, `pie_*`, `game_run*`, `screenshot*`, `image_stats`, `image_compose`, `screen_coverage`, `perf_*`, `mrq_*`, `validate`, `batch`, `doctor`, `client_config`, `ui_*`, `widget_*`, `viewmodel_*`, `input_*`, `log_*`, `automation_*`, `build_status`, `editor_restart`, `live_coding_compile`, `class_measure`, `id_audit`, `key_audit`, `asset_rename`, `asset_references`, `scene_dress`, `actor_preview`, `font_coverage`, `savegame_*`, `pie_net_emulation`, `pie_walk_to`, `pie_travel`, `ui_click`, `ui_key`, `ui_input_audit`, `ui_prompts`, `ui_input_type`, `ui_state`, `ui_open`, `ui_close`, `widget_render`, `widget_tree`, `widget_add`, `widget_create`, `widget_get`, `widget_remove`, `widget_set`, `world_trace`, `settings_*`, `type_info`, `enum_create`, `struct_create`, `font_coverage`, `gameplay_tag_*`, `stringtable_compare`, `stringtable_list`, `stringtable_get`, `stringtable_set`, `loc_culture`, `loc_gather`, `loc_gather_status`, `editor_capture`, `editor_state`, `console_command`, `level_open`, `level_save`, `mesh_info`, `mesh_set_material`, `texture_info`, `viewmodel_bind`, `viewmodel_list`, `viewmodel_add`, `viewmodel_remove`, `viewmodel_unbind`, `widget_render`, `widget_tree`, `screen_coverage`, `class_measure`, `image_stats`, `image_compose`, `screenshot`, `screenshot_ab`, `game_run`, `game_run_status`, `mrq_render`, `mrq_status`, `perf_capture`, `perf_scene`, `perf_gpu_profile`, `batch`, `validate`, `doctor`, `client_config`, `id_audit`, `key_audit`, `asset_rename`, `asset_references`, `asset_import`, `asset_find`, `asset_exists`, `asset_duplicate`, `asset_delete`, `asset_save`, `asset_search_text`, `automation_list`, `automation_run`, `build_status`, `editor_restart`, `editor_capture`, `editor_state`, `live_coding_compile`, `console_command`, `log_read`, `log_wait`, `gameplay_tag_add`, `gameplay_tag_remove`, `gameplay_tags_list`, `datatable_create`, `datatable_get`, `datatable_set_rows`, `datatable_remove_rows`, `datatable_rename_row`, `stringtable_compare`, `stringtable_get`, `stringtable_list`, `stringtable_set`, `loc_culture`, `loc_gather`, `loc_gather_status`, `font_coverage`, `mesh_info`, `mesh_set_material`, `texture_info`, `type_info`, `enum_create`, `struct_create`, `blueprint_create`, `blueprint_info`, `blueprint_add_node`, `blueprint_connect`, `blueprint_remove_node`, `blueprint_add_variables`, `blueprint_remove_variables`, `blueprint_set_component_defaults`, `blueprint_set_pin`, `blueprint_compile`, `blueprint_add_component`, `blueprint_remove_component`, `blueprint_graph_read`, `material_create`, `material_add_expression`, `material_connect`, `material_info`, `material_instance_create`, `material_instance_set`, `actor_spawn`, `actor_find`, `actor_delete`, `actor_preview`, `actor_set_transform`, `actor_bounds`, `pie_start`, `pie_stop`, `pie_state`, `pie_actors`, `pie_input`, `pie_net_emulation`, `pie_walk_to`, `pie_travel`, `ui_click`, `ui_key`, `ui_open`, `ui_close`, `ui_state`, `ui_prompts`, `ui_input_audit`, `ui_input_type`, `widget_add`, `widget_create`, `widget_get`, `widget_remove`, `widget_set`, `widget_render`, `widget_tree`, `viewmodel_add`, `viewmodel_bind`, `viewmodel_list`, `viewmodel_remove`, `viewmodel_unbind`, `savegame_get`, `savegame_set`, `screen_coverage`, `class_measure`, `world_trace`, `image_stats`, `image_compose`, `screenshot`, `screenshot_ab`, `perf_capture`, `perf_scene`, `perf_gpu_profile`, `mrq_render`, `mrq_status`, `game_run`, `game_run_status`, `batch`, `validate`, `doctor`, `client_config`, `id_audit`, `key_audit`, `asset_rename`, `asset_references`, `asset_import`, `asset_find`, `asset_exists`, `asset_duplicate`, `asset_delete`, `asset_save`, `asset_search_text`, `automation_list`, `automation_run`, `build_status`, `editor_restart`, `editor_capture`, `editor_state`, `live_coding_compile`, `console_command`, `log_read`, `log_wait`, `gameplay_tag_add`, `gameplay_tag_remove`, `gameplay_tags_list`.
>
> ## License
> MIT — free for commercial use. See [LICENSE](../LICENSE).

---

## 6. Sosyal önizleme görseli (1280×640, <1 MB)

- **İçerik:** Enginewright logosu (two-block S) + "In-editor MCP for Unreal Engine" + 3 ikon: 🎮 PIE, 📸 Visual QA, 🔧 129 Tools
- **Format:** PNG, 1280×640, <1 MB
- **Yer:** Repo Settings → Social preview → Upload
- **Araç:** Figma/Canva (şablon: GitHub social preview)

---

## 7. Registry / Liste kayıtları (kabul kuralı + link)

| Platform | Kabul kuralı | Link | Durum |
|---|---|---|---|
| **MCP Resmi Registry** (`modelcontextprotocol/registry`) | `server.json` (name, version, repository, homepage, license, tools[]) + `mcp-publisher publish` | https://github.com/modelcontextprotocol/registry | ❌ |
| **awesome-mcp-servers** (punkpeye) | Glama score badge zorunlu (CI check); sadece GitHub-hosted, installable server | https://github.com/punkpeye/awesome-mcp-servers | ❌ |
| **Glama** (glama.ai) | Otomatik tarama; `server.json` + GitHub repo | https://glama.ai/mcp/servers | ❌ |
| **Smithery** (smithery.ai) | `smithery.yaml` + Dockerfile; ücretsiz plan | https://smithery.ai | ❌ |
| **mcp.so** | GitHub repo + `server.json` | https://mcp.so | ❌ |
| **PulseMCP** | Otomatik; GitHub topics `mcp` + `mcp-server` | https://pulsemcp.com | ❌ |
| **MCP.so / MCPServer.io** | Benzer | — | ❌ |

**Eylem sırası (1.0 öncesi):** MCP Registry → awesome-mcp-servers (Glama badge) → Glama/Smithery → mcp.so. Her biri PR/issue açılır, badge README'ya eklenir.

---

## 8. GitHub Pages Dokümantasyon Sitesi + Anahtar Kelimeler

- **Statik site:** `docs/` klasöründen (EN) → GitHub Pages (custom domain yok, `shanyup.github.io/enginewright`)
- **Anahtar kelimeler (10, sayfa başına 1–2):** `unreal engine mcp`, `ue5 ai agent`, `claude unreal`, `mcp server unreal`, `unreal engine plugin mcp`, `game development ai tools`, `unreal editor automation`, `multiplayer test unreal`, `visual qa unreal`, `localization unreal engine`
- **Yapı:** `index.html` (README hero), `tools/` (her araç için sayfa, `search/describe/call` örnekleri), `guides/` (quickstart, client config, PIE testing, visual QA), `changelog/`, `roadmap/`

---

## 9. Fab / Unreal Marketplace Listesi Uygunluğu

- **Kural 1.2.4.b:** Sadece code plugin ücretsiz listelenebilir; content plugin ücretli.
- **Enginewright:** Code-only plugin (`.uplugin` + C++ kaynak), `NoRedist: false` (MIT), her UE sürümü ayrı zip (Fab otomatik build eder, ≤15 GB).
- **Durum:** UE 5.8 sürümüyle başla (`SORULAR.md` S-2). Fab listing PR'si 1.0 sonrası.

---

## 10. Google / Web Anahtar Kelime Listesi

| Anahtar kelime | Aylık hacim (tahmin, Semrush/Ahrefs benzeri) | Hedef sayfa |
|---|---|---|
| unreal engine mcp | 500–1k | README / GitHub Pages |
| ue5 ai agent | 300–800 | GitHub Pages / guides |
| claude unreal | 400–1k | README / guides/client-config |
| mcp server unreal | 200–600 | GitHub Pages |
| unreal engine plugin mcp | 100–400 | GitHub Pages / tools |
| game development ai tools | 1k–3k | Blog / guides |
| unreal editor automation | 300–800 | GitHub Pages / guides/pie-testing |
| multiplayer test unreal | 200–600 | GitHub Pages / guides/net-test |
| visual qa unreal | 100–300 | GitHub Pages / guides/visual-qa |
| localization unreal engine | 300–800 | GitHub Pages / guides/localization |

---

## 11. Dokümantasyon dili ve Türkçe iç belgeler

- **Public repo dili:** İngilizce (README, CHANGELOG, docs/sites, issue templates, PR template).
- **Türkçe iç belgeler (`docs/plan_2026-11/`, `docs/arastirma/`, `docs/*.md` hariç README/CHANGELOG):** **Public repoda kalabilir** — GitHub dil algılayıcısı README baz alır; `docs/` altındaki TR belgeler dil istatistiklerini bozmaz. Alternatif: `docs/tr/` altına taşı + `.gitattributes` ile `linguist-vendored` işaretle. **Öneri:** Şu anki yerde kalsın, linguist sorunu çıkarsa taşı.