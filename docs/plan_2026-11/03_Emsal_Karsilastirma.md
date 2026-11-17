# 03 — Emsal Karşılaştırması (Unreal + diğer motorlar + Playwright konumlandırması)

Kart: `t_bea7137b` · Tarih: **17.11.2026** · Yalnız belge, GitHub'da değişiklik yok.

**Taban:** `docs/arastirma/rakip_analizi_2026-11.md` (10.11) ve `docs/arastirma/derin/01–04` (10.11, kaynak kodu okuması). Bu belge onları **günceller**, baştan yazmaz. Özellik hücreleri derin raporlardan gelir; yıldız, son commit, lisans ve sürüm bugün yeniden çekildi.

**Kaynak kuralları**

| Kısaltma | Anlamı | Erişim |
|---|---|---|
| ★ / son commit / lisans | GitHub REST `api.github.com/repos/<repo>` ve `/commits?per_page=1` (varsayılan dal). Ham çıktı: `research/raw/emsal_gh_2026-11-17.json` | 17.11.2026 |
| Araç sayısı | Kod sayımı ya da README. Kaynağı hücrede. Diğer motorlar: `research/raw/emsal_toolcounts_2026-11-17.json` | 17.11.2026 (aksi yazmıyorsa) |
| D1–D4 | `docs/arastirma/derin/01_epic.md`, `02_buyuk_acik_kaynak.md`, `03_kucuk_acik_kaynak.md`, `04_ticari.md` (kaynak kodu okuması) | 10.11.2026 |
| (d) | Kodda doğrulandı | — |
| (i) | Yalnız README/pazarlama iddiası | — |
| **doğrulanamadı** | Kaynakta yok ya da okunamadı | — |
| *tahmin* | Değerlendirme; ölçüm değil | — |

---

## 0. Özet (karar için)

1. **Unreal'da 26 proje** tarandı (Epic 5.8 + 5.9, 22 açık kaynak, Enginewright, 2 kapsam dışı arama sonucu). Ticari ürünler (Aura, Ludus, NeoStack, UEC) §1.4'te kısa satırla var, ayrıntı D4'te.
2. **Yıldız lideri hâlâ ölü bir repo:** chongdashu/unreal-mcp [2119★](https://api.github.com/repos/chongdashu/unreal-mcp), son commit **2025-04-22**. Aktif liderler: EngineWire [1045★](https://api.github.com/repos/ChiR24/Unreal_mcp), VibeUE [841★](https://api.github.com/repos/kevinpbuckley/VibeUE), UE-MCP [508★](https://api.github.com/repos/db-lyon/ue-mcp), monolith [374★](https://api.github.com/repos/tumourlove/monolith).
3. **Enginewright'ın tek gerçek farkı "oyunu oynatıp doğrulamak":** çok oyunculu PIE + ağ emülasyonu, `game_run` (-game/paketli exe), runtime UI testi (`COVERED`), sayısal görsel karşılaştırma, lokalizasyon hattı. Bu beşinin hiçbiri Epic 5.8'de yok (D1 §5). Açık kaynakta yalnız UE-MCP çok oyunculu PIE'da bizden derin (D2).
4. **Geride kaldığımız yer katalog derinliği:** animasyon (AnimBP SM, Montage, retarget), Niagara, Sequencer, profiling, BT/StateTree yazma. Rakiplerin 4–5'inde var, bizde yok.
5. **Diğer motorlarda iki net trend var.** (a) Resmi sunucular CLI + skill'e kayıyor: Playwright README'si coding agent'lara CLI'yi öneriyor, Unity in-editor MCP'yi deprecated edip `unity mcp` CLI'ye taşıdı. (b) Büyüyenler tek satır kurulum ve görsel doğrulama ile büyüdü (blender `look`, Figma `get_screenshot`).
6. **"Playwright for Unreal" konumu tutarlı ama eksik:** Playwright'ın `verify_*` assert araçları, trace/video kaydı, kararlı ref'li snapshot ve test kodu üretimi bizde yok. Bunlar eklenirse konum iddiası kanıtlanır (§5).

---

## 1. Unreal emsalleri

### 1.1 GitHub "unreal mcp" araması — ilk 15 sonuç (best match)

Kaynak: [`search/repositories?q=unreal+mcp`](https://api.github.com/search/repositories?q=unreal+mcp) — toplam **678** sonuç, 17.11.2026. Enginewright bu sıralamada ilk 30'da **yok** (16.11 ölçümü: 198. sıra, `01_Rakip_ve_Emsal_Karsilastirma.md` §1b).

| Sıra | Repo | Bu belgede |
|---:|---|---|
| 1 | chongdashu/unreal-mcp | §1.2 #4 |
| 2 | ChiR24/Unreal_mcp (EngineWire) | §1.2 #3 |
| 3 | flopperam/unreal-engine-mcp | §1.2 #5 |
| 4 | kvick-games/UnrealMCP | §1.2 #7 |
| 5 | winyunq/UnrealMotionGraphicsMCP | §1.2 #15 |
| 6 | VedantRGosavi/UE5-MCP | §1.2 #19 (kod yok) |
| 7 | Natfii/UnrealClaude | §1.2 #13 |
| 8 | db-lyon/ue-mcp | §1.2 #8 |
| 9 | prajwalshettydev/UnrealGenAISupport | §1.2 #16 |
| 10 | ayeletstudioindia/unreal-analyzer-mcp | §1.2 #20 |
| 11 | runreal/unreal-mcp | §1.2 #6 |
| 12 | appleweed/UnrealMCPBridge | §1.2 #21 |
| 13 | DreamFekk/Andriod_UnrealMemoryTools_Mcp | **Kapsam dışı**: Android oyun belleği aracı, editör MCP'si değil ([43★](https://api.github.com/repos/DreamFekk/Andriod_UnrealMemoryTools_Mcp)) |
| 14 | GenOrca/unreal-mcp | §1.2 #14 |
| 15 | remiphilippe/mcp-unreal | §1.2 #22 |

Yıldız sırasıyla aramada çıkan ama UE MCP olmayanlar: scenario-labs/skills [3864★](https://api.github.com/repos/scenario-labs/skills) (3D üretim skill paketi), genex-games/genex-desktop [877★](https://api.github.com/repos/genex-games/genex-desktop) (çok motorlu masaüstü uygulama), islandmed/SculptForge [725★](https://api.github.com/repos/islandmed/SculptForge) (sculpt uygulaması), HurtzDonutStudios/ai-forge-mcp [102★](https://api.github.com/repos/HurtzDonutStudios/ai-forge-mcp) (asset hattı). Matrise alınmadı.

### 1.2 Künye matrisi (link, ★, son commit, lisans, araç sayısı, UE sürümü)

| # | Proje | ★ | Son commit | Lisans | Araç sayısı (kaynak) | UE sürümü | Durum |
|---:|---|---:|---|---|---|---|---|
| 1 | **[Enginewright](https://github.com/shanyup/enginewright)** | [10](https://api.github.com/repos/shanyup/enginewright) | [2026-11-10](https://api.github.com/repos/shanyup/enginewright/commits?per_page=1) (v0.4.0) | MIT | **124** yayında ([README](../../README.md), [RELIABILITY_0.4](../RELIABILITY_0.4.md)); dev'de 129 tekil ad (grep `Register(Async)?(TEXT(`, 17.11) | 5.8, Win64 (README rozeti) | Aktif, erken önizleme |
| 2a | [Epic Unreal MCP (UE 5.8)](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8) | yok (motor içi) | 5.8.4 (D1 §1.1) | UE EULA | **878** + 3 meta, 58 sınıf (kaynak sayımı, [D1 §1.5](../arastirma/derin/01_epic.md)); envanter JSON'da 870/55 ([epic_ue58_inventory.json](../epic_ue58_inventory.json)) | Yalnız 5.8 | Experimental, varsayılan kapalı |
| 2b | [Epic Unreal MCP (UE 5.9 Preview)](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9) | yok | 5.9.0-preview-1, 09.11.2026 ([forum](https://forums.unrealengine.com/t/unreal-engine-5-9-megajam-preview/2852840)) | UE EULA | **doğrulanamadı** (5.9 kaynağı okunmadı, D1 §2.1) | 5.9 | Preview, varsayılan açık |
| 3 | [EngineWire (ChiR24/Unreal_mcp)](https://github.com/ChiR24/Unreal_mcp) | [1045](https://api.github.com/repos/ChiR24/Unreal_mcp) | [2026-11-10](https://api.github.com/repos/ChiR24/Unreal_mcp/commits?per_page=1) (dev); son stable [v0.6.0](https://github.com/ChiR24/Unreal_mcp/releases) 2026-10-27 | MIT | 1 MCP aracı altında **489 action**, 23 domain ([D2 §1](../arastirma/derin/02_buyuk_acik_kaynak.md), dev `2792ba1`) | 5.0–5.8 (README (i) + koşullu derleme (d)) | Aktif |
| 4 | [chongdashu/unreal-mcp](https://github.com/chongdashu/unreal-mcp) | [2119](https://api.github.com/repos/chongdashu/unreal-mcp) | [2025-04-22](https://api.github.com/repos/chongdashu/unreal-mcp/commits?per_page=1) | LICENSE yok (README "MIT") | **32** MCP (d) ([D3 §2](../arastirma/derin/03_kucuk_acik_kaynak.md)) | 5.5+ (README) | **Ölü** (~19 ay) |
| 5 | [flopperam/unreal-engine-mcp](https://github.com/flopperam/unreal-engine-mcp) → Aura | [1119](https://api.github.com/repos/flopperam/unreal-engine-mcp) | [2026-06-26](https://api.github.com/repos/flopperam/unreal-engine-mcp/commits?per_page=1) | LICENSE yok | Yerel **43** (d); hosted "50+" (i) (D3 §5) | 5.5+ (README) | Aura'ya devredildi |
| 6 | [runreal/unreal-mcp](https://github.com/runreal/unreal-mcp) | [121](https://api.github.com/repos/runreal/unreal-mcp) | [2025-06-06](https://api.github.com/repos/runreal/unreal-mcp/commits?per_page=1) | MIT | **20** (d) (D3 §3) | 5.4+ (README) | Stagnant |
| 7 | [kvick-games/UnrealMCP](https://github.com/kvick-games/UnrealMCP) | [616](https://api.github.com/repos/kvick-games/UnrealMCP) | [2025-06-22](https://api.github.com/repos/kvick-games/UnrealMCP/commits?per_page=1) | LICENSE yok | **8** (d) (D3 §4) | 5.5 (yalnız test edilen) | **Ölü**, "VERY WIP" |
| 8 | [UE-MCP (db-lyon/ue-mcp)](https://github.com/db-lyon/ue-mcp) | [508](https://api.github.com/repos/db-lyon/ue-mcp) | [2026-11-17](https://api.github.com/repos/db-lyon/ue-mcp/commits?per_page=1), [v1.4.14](https://github.com/db-lyon/ue-mcp/releases) | MIT | **1510** kendi + 2185 `epic_*` sarmalayıcı ([D2 §1](../arastirma/derin/02_buyuk_acik_kaynak.md), `docs/tool-reference.md`) | 5.4–5.9 Win, 5.6+ Mac/Linux (README) | Çok aktif |
| 9 | [IvanMurzak/Unreal-MCP](https://github.com/IvanMurzak/Unreal-MCP) | [63](https://api.github.com/repos/IvanMurzak/Unreal-MCP) | [2026-11-17](https://api.github.com/repos/IvanMurzak/Unreal-MCP/commits?per_page=1), [v0.32.0](https://github.com/IvanMurzak/Unreal-MCP/releases) | Apache-2.0 | **27** çekirdek + 4 sistem + 28 eklenti (D2 §1, `extensions-index.json`) | 5.5+; CI 5.5–5.9 (d) | Aktif, bulut odaklı |
| 10 | [monolith (tumourlove)](https://github.com/tumourlove/monolith) | [374](https://api.github.com/repos/tumourlove/monolith) | [2026-10-18](https://api.github.com/repos/tumourlove/monolith/commits?per_page=1), [v0.24.0](https://github.com/tumourlove/monolith/releases) | MIT | **1457** action + ~167 `ui` (d) (D3 §7.1) | 5.7–5.8 | Aylık sürüm |
| 11 | [VibeUE (kevinpbuckley)](https://github.com/kevinpbuckley/VibeUE) | [841](https://api.github.com/repos/kevinpbuckley/VibeUE) | [2026-11-09](https://api.github.com/repos/kevinpbuckley/VibeUE/commits?per_page=1) | MIT | **1048** `UFUNCTION`, ~60 servis (d) (D3 §7.2) | Yalnız 5.8 (Epic MCP eklentisi) | Haftalık commit |
| 12 | [Claireon (believer-oss)](https://github.com/believer-oss/Claireon) | [147](https://api.github.com/repos/believer-oss/Claireon) | [2026-09-15](https://api.github.com/repos/believer-oss/Claireon/commits?per_page=1) | MIT | Yüzey **2** (`tool_search` + `python_execute`), katalog ~776 araç dosyası (d) (D3 §7.3) | 5.5+ (README) | Aktif değil (2 ay) |
| 13 | [Natfii/UnrealClaude](https://github.com/Natfii/UnrealClaude) | [1037](https://api.github.com/repos/Natfii/UnrealClaude) | [2026-06-26](https://api.github.com/repos/Natfii/UnrealClaude/commits?per_page=1), v1.5.1 | API: yok (alt klasörde LICENSE) | "20+" (i) (D3 §7.4) | 5.7 | Yavaşlıyor |
| 14 | [GenOrca/unreal-mcp](https://github.com/GenOrca/unreal-mcp) | [149](https://api.github.com/repos/GenOrca/unreal-mcp) | [2026-07-07](https://api.github.com/repos/GenOrca/unreal-mcp/commits?per_page=1), v2.2.0 | Apache-2.0 | **253** action, 21 domain (README, kısmen (d)) (D3 §7.5) | 5.6+, Win64 | 4 ay sessiz |
| 15 | [winyunq/UnrealMotionGraphicsMCP](https://github.com/winyunq/UnrealMotionGraphicsMCP) | [224](https://api.github.com/repos/winyunq/UnrealMotionGraphicsMCP) | [2026-08-02](https://api.github.com/repos/winyunq/UnrealMotionGraphicsMCP/commits?per_page=1), v1.0.12 | MIT | **doğrulanamadı** (D3 §7.7 araç saymadı) | 5.8 (uplugin) | UMG uzmanı |
| 16 | [prajwalshettydev/UnrealGenAISupport](https://github.com/prajwalshettydev/UnrealGenAISupport) | [686](https://api.github.com/repos/prajwalshettydev/UnrealGenAISupport) | [2026-04-28](https://api.github.com/repos/prajwalshettydev/UnrealGenAISupport/commits?per_page=1) | MIT | **30** (d) (D3 §6) | 5.4–5.7+ (README) | MCP kısmı terk |
| 17 | [Italink/UnrealClientProtocol](https://github.com/Italink/UnrealClientProtocol) | [131](https://api.github.com/repos/Italink/UnrealClientProtocol) | [2026-08-28](https://api.github.com/repos/Italink/UnrealClientProtocol/commits?per_page=1) | MIT | Sabit liste yok (reflection protokolü, tasarım gereği) (D3 §7.6) | 5.x | Niş |
| 18 | [voodoofox/unreal-mcp](https://github.com/voodoofox/unreal-mcp) | [9](https://api.github.com/repos/voodoofox/unreal-mcp) | [2026-06-22](https://api.github.com/repos/voodoofox/unreal-mcp/commits?per_page=1) | MIT | **100** MCP; C++'ta 149 komut, 73'ü Niagara (d) (D3 §1) | 5.5+ (test 5.7) | Tek geliştirici, sessiz |
| 19 | [VedantRGosavi/UE5-MCP](https://github.com/VedantRGosavi/UE5-MCP) | [438](https://api.github.com/repos/VedantRGosavi/UE5-MCP) | [2025-06-02](https://api.github.com/repos/VedantRGosavi/UE5-MCP/commits?per_page=1) | NOASSERTION | **0** (kod yok, 16 markdown) (D3 §7.8) | — | **Ölü** |
| 20 | [ayeletstudioindia/unreal-analyzer-mcp](https://github.com/ayeletstudioindia/unreal-analyzer-mcp) | [161](https://api.github.com/repos/ayeletstudioindia/unreal-analyzer-mcp) | [2025-08-06](https://api.github.com/repos/ayeletstudioindia/unreal-analyzer-mcp/commits?per_page=1) | MIT | doğrulanamadı (editöre bağlanmaz, C++ kaynak analizi) | — | Alan dışı |
| 21 | [appleweed/UnrealMCPBridge](https://github.com/appleweed/UnrealMCPBridge) | [64](https://api.github.com/repos/appleweed/UnrealMCPBridge) | [2026-11-11](https://api.github.com/repos/appleweed/UnrealMCPBridge/commits?per_page=1) | MIT | doğrulanamadı (README'de sayı yok; Editor Python API köprüsü) | doğrulanamadı | Aktif, küçük |
| 22 | [remiphilippe/mcp-unreal](https://github.com/remiphilippe/mcp-unreal) | [82](https://api.github.com/repos/remiphilippe/mcp-unreal) | [2026-02-20](https://api.github.com/repos/remiphilippe/mcp-unreal/commits?per_page=1) | Apache-2.0 | **49** (i) ([README](https://github.com/remiphilippe/mcp-unreal)) | 5.7 (README) | 9 ay sessiz |
| 23 | [softdaddy-o/yes-ue-mcp](https://github.com/softdaddy-o/yes-ue-mcp) | [87](https://api.github.com/repos/softdaddy-o/yes-ue-mcp) | [2026-03-16](https://api.github.com/repos/softdaddy-o/yes-ue-mcp/commits?per_page=1) | MIT | "50+ komut" (i) ([README](https://github.com/softdaddy-o/yes-ue-mcp)) | 5.4+ (açıklama) | 8 ay sessiz |
| 24 | [aadeshrao123/Unreal-MCP](https://github.com/aadeshrao123/Unreal-MCP) | [40](https://api.github.com/repos/aadeshrao123/Unreal-MCP) | [2026-11-03](https://api.github.com/repos/aadeshrao123/Unreal-MCP/commits?per_page=1) | MPL-2.0 | "288 komut" (i) ([README](https://github.com/aadeshrao123/Unreal-MCP)) | README'de 5.3/5.6/5.8 geçiyor, aralık doğrulanamadı | Ticari ürünün (CodeFizz) community sürümü |
| 25 | [Codeturion/unreal-api-mcp](https://github.com/Codeturion/unreal-api-mcp) | [100](https://api.github.com/repos/Codeturion/unreal-api-mcp) | [2026-10-26](https://api.github.com/repos/Codeturion/unreal-api-mcp/commits?per_page=1), v0.3.1 | MIT | **5** (i) ([README](https://github.com/Codeturion/unreal-api-mcp)) | 5.7/5.8 API dokümanı | Editör kontrolü yok, API arama |
| 26 | [per-simmons/unreal-agent-harness](https://github.com/per-simmons/unreal-agent-harness) | [211](https://api.github.com/repos/per-simmons/unreal-agent-harness) | [2026-06-24](https://api.github.com/repos/per-simmons/unreal-agent-harness/commits?per_page=1) | LICENSE yok | 0 kendi aracı; Epic 5.8 MCP üstü harness (D3 §7.8) | 5.8 | Demo |

**Kapsam: 26 satır, 24'ü ayrı Unreal projesi** (2a/2b aynı ürün, #1 biz). Kabul ölçütü "en az 15" karşılandı.

### 1.3 Yetenek matrisi (kurulum, doğrulama/geri alma, görsel, PIE, doküman)

Kurulum ve doküman sütunları *tahmin* (değerlendirme). Diğer sütunlar D1–D3 kod okumasından; kaynak bölümü parantezde.

| Proje | Kurulum zorluğu | Doğrulama / geri alma | Görsel doğrulama | PIE / oyun testi | Doküman |
|---|---|---|---|---|---|
| **Enginewright** | **Kolay**: tek C++ eklenti zip'i, Python/Node yok, `client_config` (README "Why"). Yalnız 5.8/Win64 | **Güçlü**: strict şema (bilinmeyen anahtar = hata), her yazma 1 undo + geri okuma, atomik `batch` + rollback, `validate`/`id_audit`/`key_audit`/`doctor` (D1 §5) | **Güçlü**: `screenshot`, `screenshot_ab`, `image_stats` (histogram/fark), `image_compose`, `widget_render`, `actor_preview` (D1 §4) | **En güçlüler arasında**: `pie_*` (1–4 oyuncu, netMode), `pie_net_emulation`, `pie_walk_to/travel`, `ui_click` (`COVERED`), `game_run` (-game/paketli exe) (D1 §5) | İyi: EN README + demo GIF + RELIABILITY + SUPPORT; site yok |
| Epic 5.8 | Orta: eklenti + `AllToolsets` elle açılır; Python toolset'ler PythonScriptPlugin ister (D1 §1.2) | Zayıf/dağınık: transaction yalnız ~4 toolset + `execute_tool_script`; property yazma "NOT transactional" (D1 §1.6) | Capture var (viewport/editor/asset/Slate); karşılaştırma yok (D1 §1.6) | `StartPIE/StopPIE`; oyun girdisi yok, çok oyunculu yok (D1 §1.6) | İyi: resmi doküman, Experimental |
| Epic 5.9 Preview | Kolay (varsayılan açık) ama toolset'ler kapalı geliyor (`UE-412135`) (D1 §2.2) | Erişim modları Read/Write/Execute + `readOnlyHint` (doküman) | Scene Capture runtime toolset (doküman) | Input/Console/Perf runtime toolset'leri, cooked build'de MCP (doküman); çok oyunculu: doğrulanamadı | İyi, Preview |
| EngineWire | Orta: eklenti + opsiyonel npm stdio proxy; Fab listesi var (D2 §1) | Güçlü: undo/redo + restore point, consent ledger, her yazmada Data Validation uyarısı (D2 §2) | Var: `screenshot`, `capture_passes` (D2 §2) | Var: `play/possess`, `playtest` (NavMesh yürüt), `simulate_input`; N-client PIE **yok** (D2 §2) | İyi: README + wiki |
| chongdashu | Orta: eklenti derle + Python/uv (D3 §2) | Yok (transaction 0) | `take_screenshot` C++'ta var, MCP'ye açık değil | Yok | Orta, eskimiş |
| flopperam (yerel) | Orta: Python + C++ eklenti (D3 §5) | Yok (undo 0) | Yok (yerel); hosted `window_capture` (i) | Yerel yok; hosted `pie_test_*` "30+ assertion" (i) | Yönlendirme sayfası |
| runreal | **Kolay**: eklentisiz, npx + UE Python Remote Execution açılır (D3 §3) | `editor_validate_assets` sahte (ilk 100 asset) (D3 §3) | `editor_take_screenshot` (640×520) | Yok | Zayıf |
| kvick-games | Orta: eklenti + Python (D3 §4) | Yok | Yok | Yok | Zayıf |
| UE-MCP (db-lyon) | Orta/Zor: Node daemon + shim + C++ eklenti (D2 §1) | **En güçlü**: undo/redo/steps, `batch`, shadow-git checkpoint, YAML flow + rollback, guard/lock/journal (D2 §3) | `capture_screenshot(pieInstance)`, sequence, view mode (D2 §3) | **En derin çok oyunculu**: `numClients`, ayrı süreç, windowless client, client başına log, input kayıt/oynatma (D2 §3) | Çok iyi: site + docs klasörü |
| IvanMurzak Unreal-MCP | Zor: eklenti + .NET sidecar indirme + bulut OAuth (varsayılan) (D2 §1) | RQL mutasyonu tek transaction, `dry_run`, 200 hedef limiti (D2 §4) | 4 screenshot modu, izole render (D2 §4) | Yalnız `IsPlaying` yazma; çok oyunculu yok (D2 §4) | Orta, ürün sitesi |
| monolith | Orta: eklenti + Python proxy (D3 §7.1) | `FScopedTransaction` 67 dosyada; `tools/list` readOnly/destructive annotation (D3 §7.1) | Güçlü capture: gif, flipbook, material grid, PIE hareket klibi (D3 §7.1) | `start/stop_pie`, `run_pie_smoke`, input inject; çok oyunculu yok (D3 §7.1) | Zayıf/orta |
| VibeUE | Orta: Epic 5.8 MCP + ToolsetRegistry + Python eklentileri gerekir (D3 §7.2) | `TransactionService` Undo/Redo/History, checkpoint (D3 §7.2) | Epic'e bırakıyor (README "screenshots, logs, PIE" çoğaltmaz) | PIE actor servisi; StartStandalone; çok oyunculu yok (D3 §7.2) | İyi: ~36 skill dosyası |
| Claireon | Orta: eklenti (+proxy) (D3 §7.3) | Transaction grubu + rollback; editör çökme atfı (proxy) (D3 §7.3) | `PIEScreenshot`, `SlateScreenshot` (D3 §7.3) | `PIEStart` net mode, `PIEWaitFor`, test yardımcıları; client sayısı yok (D3 §7.3) | Zayıf |
| UnrealClaude | Orta: editör içi chat + Claude Code CLI (D3 §7.4) | Undo 0 | `CaptureViewport` | Yok | İyi README |
| GenOrca | Orta: Python MCP + editör Python eklentisi (D3 §7.5) | 3 dosyada transaction | `vision` 3 action | `util` PIE kontrolü (kısmi) | Orta; Fab'da da var (i) |
| winyunq UMG | Orta (D3 §7.7) | doğrulanamadı | UMG odaklı; doğrulanamadı | Yok | Orta |
| UnrealGenAISupport | Orta: socket sunucu elle açılır (D3 §6) | Undo 0 | `take_editor_screenshot` | Yok | İyi README, MCP terk |
| Italink UCP | Orta (D3 §7.6) | Undo 8 dosyada | doğrulanamadı | doğrulanamadı | Zayıf |
| voodoofox | Orta (D3 §1) | Undo 0 | Yok (handler açık değil) | Yok | Orta |
| remiphilippe | doğrulanamadı | doğrulanamadı | README "screenshot/capture" geçiyor (i) | README "PIE", "automation test" geçiyor (i) | Orta |
| softdaddy-o, aadeshrao123, appleweed, Codeturion, per-simmons, VedantRGosavi, ayeletstudio | — | doğrulanamadı (kod okunmadı) | doğrulanamadı | doğrulanamadı | — |

### 1.4 Ticari ürünler (kısa; ayrıntı ve kaynak D4)

| Ürün | Model | Bizimle ilişkisi |
|---|---|---|
| Ramen VR **Aura** (flopperam'ı aldı) | Kapalı beta, ticari; "Verification" playtest ajanı (i) | Oyun testi alanında en yakın rakip iddiası; doğrulanamadı ([D4 §2](../arastirma/derin/04_ticari.md)) |
| **Ludus AI** | Abonelik, proje bağlamı + MCP endpoint (D4 §1) | Chat ürünü; MCP sunucusu olarak doğrudan rakip değil |
| Betide **NeoStack** | Fab, abonelik/ömür boyu (D4 §3) | Editör içi chat, çok ajan |
| **Ultimate Engine CoPilot** | Ticari, "1655 araç" (i) (D4 §4a) | Sayı iddiası doğrulanamadı |

### 1.5 Epic resmi araçları ↔ `epic_ue58_inventory.json` ↔ Enginewright

| Ölçü | Envanter JSON | Motor kaynağı (D1) | Enginewright | Not |
|---|---|---|---|---|
| Sınıf / toolset | **55** ([json](../epic_ue58_inventory.json)) | **58** (D1 §0) | 30 kategori civarı (*tahmin*, README) | JSON 3 sınıfı kaçırmış; güncel sayı 58 |
| Araç | **870** | **878 + 3 meta** | 124 (v0.4.0) | Epic ~7 kat büyük |
| Araç keşfi | — | Tool search: `list_toolsets`/`describe_toolset`/`call_tool` | Kategori kapatma (`DisabledCategories`) | Epic'in tekniği büyüdükçe gerekir |
| Undo / transaction | — | Dağınık (D1 §1.6) | Merkezi, her yazma | **Biz öndeyiz** |
| Read-only modu | — | 5.8 yok, 5.9 var | Var (`bReadOnly`) ama MCP annotation yayımlanmıyor | 5.9 istemcileri `readOnlyHint` bekleyecek |
| Origin allow-list | — | Var | **Yok** (D1 §0) | 1.0 öncesi zorunlu |
| Python | — | 30/58 toolset Python; 5.9 `execute_python` | Yok (bilinçli) | Fark ya da açık, karar gerekli |
| MCP client toolset | — | Var: dış MCP sunucularını bağlar | — | Epic asistanı Enginewright'ı tüketebilir → uyumluluk fırsatı |

---

## 2. Diğer motorlar ve araçlar

Hepsi 17.11.2026 erişim. Araç sayıları kod ya da resmi dokümandan sayıldı (ham: `research/raw/emsal_toolcounts_2026-11-17.json`).

| Proje | ★ | Son commit | Lisans | Araç sayısı (kaynak) | Desteklenen sürüm | Kurulum (*tahmin*) | Doğrulama / geri alma | Görsel doğrulama | Oyun/çalıştırma testi | Doküman |
|---|---:|---|---|---|---|---|---|---|---|---|
| [CoplayDev/unity-mcp](https://github.com/CoplayDev/unity-mcp) | [16037](https://api.github.com/repos/CoplayDev/unity-mcp) | [2026-11-17](https://api.github.com/repos/CoplayDev/unity-mcp/commits?per_page=1), [v10.3.0](https://github.com/CoplayDev/unity-mcp/releases) | MIT | **50** `@mcp_for_unity_tool` / 43 dosya (kod sayımı, [`Server/src/services/tools`](https://github.com/CoplayDev/unity-mcp/tree/beta/Server/src/services/tools)) | Unity 2021.3 LTS → 6.x, Python 3.10+ ([README](https://github.com/CoplayDev/unity-mcp)) | Kolay: Git URL ile paket + uv; "Configure All Clients"; MCPB paketi | `manage_editor` undo/redo; C# tarafında `Undo.*` 8 dosyada ([kod arama](https://github.com/search?q=repo%3ACoplayDev%2Funity-mcp+Undo.RegisterCreatedObjectUndo&type=code)); `batch_execute` atomik değil (rollback kelimesi yok) | `manage_camera` screenshot + multiview | `manage_editor` play/pause/stop; `run_tests` EditMode/PlayMode | Çok iyi: site + katalog + skill |
| Unity resmi: [com.unity.ai.assistant MCP](https://docs.unity3d.com/Packages/com.unity.ai.assistant@2.21/manual/integration/unity-mcp-landing.html) → [Unity CLI `unity mcp`](https://docs.unity.com/en-us/unity-cli/replace-mcp-server-unity-cli) | yok (kapalı) | Assistant 2.21.0-pre.2 dokümanı | Unity lisansı; CLI ücretsiz | **doğrulanamadı** (araç listesi okunmadı) | Unity 6.0+ ([doküman](https://docs.unity3d.com/Packages/com.unity.ai.assistant@2.20/manual/integration/unity-mcp-get-started.html)) | Kolay: `unity mcp configure <client>` | doğrulanamadı | doğrulanamadı | CLI: headless build + test suite koşar ([doküman](https://docs.unity.com/en-us/unity-cli/replace-mcp-server-unity-cli)) | İyi. **Not:** in-editor MCP **deprecated**, yerine CLI |
| [IvanMurzak/Unity-MCP](https://github.com/IvanMurzak/Unity-MCP) | [4657](https://api.github.com/repos/IvanMurzak/Unity-MCP) | [2026-11-17](https://api.github.com/repos/IvanMurzak/Unity-MCP/commits?per_page=1), 1.11.3 | Apache-2.0 | doğrulanamadı | README'de Unity 2022 geçiyor | Orta (.NET köprü) | doğrulanamadı | README'de screenshot geçiyor (i) | README'de test geçiyor (i) | Orta |
| [Coding-Solo/godot-mcp](https://github.com/Coding-Solo/godot-mcp) | [6688](https://api.github.com/repos/Coding-Solo/godot-mcp) | [2026-04-16](https://api.github.com/repos/Coding-Solo/godot-mcp/commits?per_page=1) | MIT | **14** (kod: `src/index.ts`) | Godot (sürüm belirtilmemiş; UID araçları 4.4+), Node 18+ | **Kolay**: `npx @coding-solo/godot-mcp` | Yok (dosya tabanlı) | Yok | `run_project`, `get_debug_output`, `stop_project` | Orta, kısa README |
| [hi-godot/godot-ai](https://github.com/hi-godot/godot-ai) | [3585](https://api.github.com/repos/hi-godot/godot-ai) | [2026-11-16](https://api.github.com/repos/hi-godot/godot-ai/commits?per_page=1) | MIT | **46** araç / 120+ operasyon ([README](https://github.com/hi-godot/godot-ai)) | Godot 4.7+, uv | Kolay: AssetLib + uv | doğrulanamadı | doğrulanamadı | "Run the scene test suite" örneği (i) | İyi; README'de "21.000+ aylık aktif kurulum" (i, telemetri) |
| [ee0pdt/Godot-MCP](https://github.com/ee0pdt/Godot-MCP) | [623](https://api.github.com/repos/ee0pdt/Godot-MCP) | [2025-03-19](https://api.github.com/repos/ee0pdt/Godot-MCP/commits?per_page=1) | MIT | doğrulanamadı | doğrulanamadı | Orta (klonla + build) | doğrulanamadı | doğrulanamadı | doğrulanamadı | Ölü |
| [ahujasid/mcp-for-blender](https://github.com/ahujasid/mcp-for-blender) (eski `blender-mcp`) | [34800](https://api.github.com/repos/ahujasid/blender-mcp) | [2026-11-15](https://api.github.com/repos/ahujasid/blender-mcp/commits?per_page=1) | MIT | README: **9** araç grubu (11 ad); kodda 15 `@mcp.tool` (bazıları koşullu) ([server.py](https://github.com/ahujasid/mcp-for-blender/blob/main/src/blender_mcp/server.py)) | Blender 3.0+ (splat 5.3+), Python 3.10+ | **Çok kolay**: tek satır installer, `uvx mcp-for-blender setup`, Claude/Codex plugin | Yok; "safe mode" + onay istemi var (`safe_mode.py`, `consent_prompt.py`) | **Güçlü**: `look` (viewport/kamera/çok açı/kare şeridi, solid/wireframe/xray/render) | Yok (oyun motoru değil) | Çok iyi, uzun README, guide dosyaları |
| [microsoft/playwright-mcp](https://github.com/microsoft/playwright-mcp) | [39799](https://api.github.com/repos/microsoft/playwright-mcp) | [2026-11-17](https://api.github.com/repos/microsoft/playwright-mcp/commits?per_page=1), [v0.0.85](https://github.com/microsoft/playwright-mcp/releases) | Apache-2.0 | **72** `browser_*` (README sayımı) | Node 20+; Chromium/Firefox/WebKit | **Çok kolay**: `npx @playwright/mcp@latest`, istemci başına hazır komut (Claude Code, Codex, Cursor, VS Code, Goose, Kiro …) | `browser_verify_*` (4 assert aracı), deterministik ref'ler | Screenshot + a11y snapshot (görselsiz) + video + trace | Tarayıcı = "oyun"; `browser_wait_for`, tracing, video | Çok iyi; README CLI'yi öneriyor |
| [ChromeDevTools/chrome-devtools-mcp](https://github.com/ChromeDevTools/chrome-devtools-mcp) | [58007](https://api.github.com/repos/ChromeDevTools/chrome-devtools-mcp) | [2026-11-17](https://api.github.com/repos/ChromeDevTools/chrome-devtools-mcp/commits?per_page=1), v1.13.0 | Apache-2.0 | **61** (`docs/tool-reference.md` başlık sayımı) | Chrome stable + Node LTS | Çok kolay: npx; Claude/Cursor/Gemini/Kimi plugin klasörleri | n/a | Screenshot (şeffaf dahil) | **Perf trace + insight**, network, console, logpoint | Çok iyi; CLI + skills |
| [Figma resmi MCP](https://developers.figma.com/docs/figma-mcp-server/tools-and-prompts/) | yok (kapalı); rehber repo [2203](https://api.github.com/repos/figma/mcp-server-guide) | rehber repo [2026-11-16](https://api.github.com/repos/figma/mcp-server-guide/commits?per_page=1) | Ticari (Figma planı) | **34**: 19 okuma + 15 yazma (Weave araçları ayrı) ([doküman](https://developers.figma.com/docs/figma-mcp-server/tools-and-prompts/)) | Remote `mcp.figma.com/mcp`; desktop 127.0.0.1:3845 (ücretli koltuk) | Kolay (remote, OAuth) | n/a | `get_screenshot`; rehber "önce context, sonra screenshot, sonra 1:1 doğrula" diyor ([best practices](https://help.figma.com/hc/en-us/articles/34855090823063-MCP-server-best-practices)) | n/a | Çok iyi; skill + MCP prompt (`create_design_system_rules`) |
| [GLips/Figma-Context-MCP](https://github.com/GLips/Figma-Context-MCP) | [16232](https://api.github.com/repos/GLips/Figma-Context-MCP) | [2026-06-24](https://api.github.com/repos/GLips/Figma-Context-MCP/commits?per_page=1), v0.13.2 | MIT | **2** (`get_figma_data`, `download_figma_images`, kod sayımı) | Figma API anahtarı | Kolay (npx) | n/a | Görsel indirme | n/a | İyi |

**Diğer motorlardan çıkan dersler (kısa)**

| Gözlem | Kanıt | Enginewright için anlamı |
|---|---|---|
| Az araç, çok yıldız mümkün | Blender 9 araç / 34.8k★, Figma-Context 2 araç / 16.2k★ | Katalog sayısı büyümeyi belirlemiyor; kurulum + demo + görsel geri bildirim belirliyor |
| Resmi oyuncu CLI'ye kayıyor | Playwright README "CLI+SKILLS" önerisi ([satır](https://github.com/microsoft/playwright-mcp#playwright-mcp-vs-playwright-cli)); Unity in-editor MCP deprecated ([doküman](https://docs.unity.com/en-us/unity-cli/replace-mcp-server-unity-cli)) | `ewtool.py` + skill dosyaları ikinci giriş kapısı olabilir |
| Görsel doğrulama standart oldu | Blender `look`, Figma `get_screenshot`, unity-mcp multiview | Bizim `image_stats`/`screenshot_ab` farkı korunmalı ve vitrine çıkarılmalı |
| Topluluk ürünü resmi ürünü geçebilir | unity-mcp 16k★, Unity resmi MCP deprecated | Epic 5.9 tehdidi kesin değil; fark ve kalite belirleyici |
| Godot'ta lider değişti | Coding-Solo son commit 2026-04-16; hi-godot/godot-ai 7 ayda 3585★ | Durgun lider (chongdashu) geçilebilir |

---

## 3. Enginewright: farklılaştığı ve geride kaldığı noktalar

### 3.1 Farklılaştığı noktalar

| # | Fark | Kimde yok (kanıt) | Kimde kısmen var |
|---:|---|---|---|
| 1 | **Çok oyunculu PIE + ağ emülasyonu** (`pie_start` 1–4 oyuncu, `pie_actors` replikasyon bekleme, `pie_net_emulation`) | Epic 5.8 (D1 §1.6), EngineWire (D2 §2), monolith, VibeUE, Claireon (D3 §10) | **UE-MCP daha derin** (ayrı süreç, windowless, client logu) (D2 §3) |
| 2 | **`game_run`**: -game / paketli exe, kültür, cvar, varyant, `shotNames`, `glyphMisses`, karar | Hiçbirinde doğrulanmadı (D1 §5, D3 §11) | VibeUE `StartStandalone` (perf amaçlı) |
| 3 | **Runtime UI testi**: `ui_state` (oyuncunun kullanabildikleri), `ui_click` `COVERED` reddi, `ui_key`, `ui_prompts` + `input_map_list` | Epic (Slate Inspector yalnız editör UI) (D1 §4), açık kaynakta yok (D3 §11) | monolith `pie_inject_input_action`; UE-MCP runtime widget inspection (D2 §6) |
| 4 | **Sayısal görsel karşılaştırma**: `screenshot_ab`, `image_stats`, `image_compose`, `widget_render`, `actor_preview` | Epic, EngineWire, UE-MCP yalnız capture (D2 §6) | IvanMurzak izole render; monolith gif/grid |
| 5 | **Lokalizasyon hattı**: `loc_gather`, `stringtable_compare` (.po/.locres), `font_coverage` | Epic (yalnız StringTable), monolith (okuma) | UE-MCP gather + StringTable (D2) |
| 6 | **Merkezi güvenlik modeli**: strict şema, her yazma undo + geri okuma, atomik `batch`, read-only mod | Epic'te dağınık (D1 §1.6); chongdashu/voodoofox/kvick'te undo 0 | UE-MCP flow rollback, Claireon transaction grubu |
| 7 | **Bağımlılıksız kurulum** (Python/Node/proxy yok) | Epic toolset'lerin 30/58'i Python; UE-MCP Node; Ivan .NET | EngineWire native (proxy opsiyonel) |
| 8 | **Savegame okuma/yazma, enum/struct oluşturma, `scene_dress`, `pie_walk_to/travel`** | Epic, D3 grubunda bulunamadı (D1 §5, D3 §11) | EngineWire `playtest` (NavMesh yürütme) |
| 9 | **MVVM yazma** (`viewmodel_*`) | Açık kaynakta doğrulanmış tek (D3 §11) | Epic 5.8 MVVM 9 araç |

### 3.2 Geride kaldığı noktalar

| # | Açık | Kimde var (derinlik, kaynak) | Etki |
|---:|---|---|---|
| 1 | **Animasyon yazma**: AnimBP SM, Montage/BlendSpace/Notify, IK retarget, soket, Physics Asset | EngineWire W, UE-MCP W, monolith W, VibeUE W, Claireon W, GenOrca W(retarget) (D2 §6, D3 §10); Epic: soket + Physics Asset | RagnaCook için kritik (rakip_analizi §3 #3) |
| 2 | **Niagara** | Epic 56, monolith 129, EngineWire, VibeUE, Claireon (D1, D3) | RagnaCook kritik |
| 3 | **Sequencer / MRQ** | Epic ~330 araç; EngineWire MRQ dahil (D1 §1.5, D2 §2) | Devlog/trailer otomasyonu |
| 4 | **Profiling** (Insights trace, frame timing) | VibeUE verdict, Claireon trace sorgu, UE-MCP trace, Epic 5.9 Perf (D1, D2, D3) | Dev'de `perf_*` başladı (01_Rakip §3); yayında yok |
| 5 | **BT / StateTree / EQS yazma** | EngineWire, UE-MCP, monolith, Claireon, VibeUE (D3 §10); Epic yalnız okuma | Düşman AI |
| 6 | **Landscape / Foliage / PCG** | EngineWire, VibeUE, Claireon, Epic PCG 31 (D1, D3) | Seviye üretimi |
| 7 | **Packaging / cook** | EngineWire BuildCookRun (D2) | Steam build hattı |
| 8 | **Çoklu UE sürümü** (yalnız 5.8) | EngineWire 5.0–5.8, UE-MCP 5.4–5.9, Ivan CI 5.5–5.9 (D2 §5) | Kullanıcı tabanını daraltır |
| 9 | **Origin allow-list + MCP annotation (`readOnlyHint`)** | Epic 5.8 Origin, Epic 5.9 annotation, monolith annotation (D1, D3) | Güvenlik + istemci uyumu |
| 10 | **Görünürlük**: 10★, aramada ilk 30'da yok, registry/listelerde yok | Rakipler 100–2119★ (§1.2) | 04/05 planları |
| 11 | **Python kaçış kapısı** | 8+ projede var (D3 §10), Epic 5.9 `execute_python` | Bilinçli tercih; karar `SORULAR.md`'de |

---

## 4. Doğrudan kopyalanamayan ama uyarlanabilecek fikirler

"Kopyalanamaz" sebebi: lisans (Epic EULA, LICENSE'sız repo), mimari farkı (Python/Node/.NET bağımlılığı, bulut), kapalı kaynak ya da bizim ilkelerimizle çakışma (strict, bağımlılıksız).

| # | Fikir (kaynak) | Neden doğrudan alınamaz | Enginewright'a uyarlanmış hali |
|---:|---|---|---|
| 1 | **Tool search meta-araçları** (Epic: `list_toolsets`/`describe_toolset`/`call_tool`, D1 §1.2) | Epic EULA; motor içi registry | Mevcut kategori yapısıyla `ew_categories` + `ew_describe`; istemci isterse lazy şema |
| 2 | **Namespace dispatch** (monolith `{ns}_query(action)`, D3 §7.1) ve **2 araçlı yüzey + FTS arama** (Claireon) | Strict şema ve araç başına doğrulama ilkemizle çakışır | Yalnız keşif için arama aracı; yazma araçları ayrı ve strict kalır |
| 3 | **YAML flow + adım başına rollback, shadow-git checkpoint** (UE-MCP, D2 §3) | Node/isomorphic-git bağımlılığı | `batch`'i "senaryo dosyası"na genişlet: C++ içinde JSON senaryo + mevcut undo; dosya checkpoint'i için yalnız `Saved/` altına kopya |
| 4 | **Epic ToolsetRegistry'ye eklenti olarak kaydolmak** (VibeUE, D3 §7.2) | VibeUE yalnız 5.8'e kilitlendi; Epic deneysel API'sine bağımlılık | Opsiyonel köprü modülü: Enginewright araçlarını Epic registry'ye de yayımla (5.9 Unreal Assistant içinden görünürlük) — çekirdek bağımsız kalır |
| 5 | **Consent ledger** (EngineWire, D2 §2) | Kod MIT ama mimari farklı | Yıkıcı araçlar için proje başına onay listesi (read-only modun ince ayarı) |
| 6 | **Her yazmada Data Validation uyarısı dönmek** (EngineWire, rakip_analizi §3 #29) | — (fikir) | Yazma yanıtına `validate` özetini ekle (opsiyonel bayrak) |
| 7 | **Editör çökme atfı proxy'si** (Claireon, D3 §7.3) | Ayrı süreç = "bağımlılık yok" ilkesine ters | `ewtool.py` içinde: bağlantı koparsa `Saved/Crashes` oku, nedeni raporla |
| 8 | **Ayrı süreç client + windowless PIE + client başına log** (UE-MCP, D2 §3) | Runtime modülünü client'lara yükleme mimarisi | `pie_start` için `runUnderOneProcess=false` + client log toplama; co-op testinde UE-MCP'yi yakalar |
| 9 | **Tek satır installer + Claude/Codex plugin** (blender: `uvx mcp-for-blender setup`, Claude plugin dizini) | Python/uv tabanlı | `install.ps1` ile zip indir → projeye kopyala → `client_config` çalıştır; `.claude-plugin` klasörü |
| 10 | **`look` çok açılı görsel sayfa** (blender, §2) | Blender API'si | `actor_preview` + `image_compose` ile "4 açı + wireframe" tek çağrı (`actor_look`) |
| 11 | **`browser_verify_*` assert araçları** (Playwright, 4 araç) | Tarayıcı DOM'u | `ui_verify_text`, `actor_verify`, `pie_verify_value`: sonuç PASS/FAIL + kanıt screenshot |
| 12 | **Tracing / video kaydı** (Playwright `start_tracing`, `start_video`) | Tarayıcı altyapısı | Test oturumu kaydı: adım listesi + her adımda kare + log kesiti (`session_trace`) |
| 13 | **Perf trace + insight** (Chrome DevTools MCP, 61 araç) | Chrome'a özgü | `perf_capture` çıktısına "en pahalı 5 scope + öneri" özeti |
| 14 | **CLI + skills ikinci kapı** (Playwright CLI, Unity CLI, Chrome DevTools CLI) | Resmi oyuncuların altyapısı | `ewtool.py call <tool> <json>` + repo içinde `skills/` (Claude/Codex) |
| 15 | **MCP prompt: kural dosyası üret** (Figma `create_design_system_rules`) | Figma'ya özgü | `create_project_rules`: proje adlandırma/klasör kurallarını tarayıp `AGENTS.md` taslağı üret |
| 16 | **Test runner entegrasyonu** (unity-mcp `run_tests` EditMode/PlayMode) | — | Var (`automation_run`); eksik: PIE senaryosunu Automation Spec'e dışa aktarma |
| 17 | **CI sürüm matrisi** (IvanMurzak self-hosted 5.5–5.9, D2 §5) | Self-hosted runner maliyeti | Önce derleme matrisi (5.6/5.7/5.8) yerelde `ewtool.py build --engine` |
| 18 | **`tools/list` annotation** (monolith readOnly/destructive, Epic 5.9) | — | `bWrites`/`HasSideEffects` bilgisini MCP `annotations`'a yaz (iş küçük, D1 §6) |
| 19 | **Sosyal kanıt metriği** (godot-ai "21.000+ aylık kurulum", blender telemetri) | Telemetri kullanıcı güveni riski | Telemetri yok; yerine GitHub release indirme sayısı + Fab sayfası |

---

## 5. Playwright MCP konumlandırma analizi ("Playwright for Unreal")

### 5.1 Playwright MCP neyi doğru yaptı

| Özellik | Kanıt | Neden önemli |
|---|---|---|
| Yapısal durum, piksel değil | README: "accessibility tree, not pixel-based input", "No vision models needed" ([README](https://github.com/microsoft/playwright-mcp)) | Ajan tahmin etmez, ref ile tıklar |
| Deterministik araç uygulama | "Avoids ambiguity common with screenshot-based approaches" | Tekrarlanabilir test |
| Assert araçları | `browser_verify_element_visible/list_visible/text_visible/value` | Test = kanıt |
| Kayıt | `start_tracing`, `start_video`, `video_chapter` | Hata raporuna kanıt |
| Kurulum | `npx @playwright/mcp@latest`; README'de istemci başına hazır komut | Hızlı başlar (*tahmin*: birkaç dakika) |
| Kurum güveni | Microsoft; [39799★](https://api.github.com/repos/microsoft/playwright-mcp), 2025-03 açılış | Resmi destek |
| Kendi sınırını söylüyor | README: coding agent için CLI+SKILLS daha az token yer; MCP uzun süreli, durumlu döngüler için | Konumlandırma dürüst |

### 5.2 Eşleme: Playwright kavramı → Enginewright karşılığı

| Playwright | Enginewright bugün | Durum | Yapılacak |
|---|---|---|---|
| a11y snapshot + ref (`e12`) | `ui_state` (oyuncunun kullanabildikleri), widget yolları; sahne için `actor_*` sorguları | 🟡 Ref'ler kararlı ID değil | Tek `snapshot` aracı: widget + aktör, kararlı ref |
| Actionability (görünür, sabit, olay alır) | `ui_click` `COVERED` reddi | ✅ Birebir karşılık | Vitrine koy: "Playwright gibi gerçek tıklama" |
| `click/type/press_key` | `ui_click`, `ui_key`, `pie_input` | ✅ | — |
| `wait_for` | `log_wait`, `pie_actors` replikasyon bekleme | ✅ | Genel `wait_for(koşul)` |
| `verify_*` | Yok (`validate` editör verisi içindir) | ❌ | §4 #11 |
| Screenshot | `screenshot`, `screenshot_ab`, `image_stats`, `image_compose` | ✅+ (Playwright'ta sayısal fark yok) | Ana farkımız olarak anlat |
| Trace / video | Yok | ❌ | §4 #12 |
| Çoklu tarayıcı / bağlam | 1–4 oyuncu PIE, netMode | ✅ | Ayrı süreç client (§4 #8) |
| Network mock / emülasyon | `pie_net_emulation` (gecikme/kayıp profili) | ✅ | — |
| Headless | `game_run` (-game); nullrhi yok | 🟡 | Headless koşu 1.0 hedefi |
| Test runner (`@playwright/test`) | `automation_list/run` | 🟡 Senaryo dışa aktarımı yok | §4 #16 |
| Codegen / `generate_locator` | Yok | ❌ | Oturumdan Automation Spec üretimi (1.0 sonrası) |
| `npx` tek satır | Zip + `client_config` | 🟡 | §4 #9 |
| CLI + SKILLS | `ewtool.py` (yalnız build) | 🟡 | §4 #14 |

**Sonuç:** 14 kavramdan 6'sı ✅, 5'i 🟡, 3'ü ❌ (*sayım bu tablodan*). Eksik üçü (verify, trace, codegen) Playwright'ı "test aracı" yapan kısımdır. Bunlar olmadan "Playwright for Unreal" demek bir benzetme olarak kalır.

### 5.3 Konum: güçlü yanlar ve riskler

| | Madde |
|---|---|
| **Güçlü** | Epic'in resmi MCP'si oyun testi yapmıyor (5.8: girdi yok, çok oyunculu yok, -game yok; D1 §1.6). Playwright'ın "resmi araç ama test odaklı" boşluğu Unreal'da boş. |
| **Güçlü** | `COVERED` reddi ve `image_stats` Playwright'ın actionability + görsel karşılaştırmasına doğrudan denk; anlatması kolay. |
| **Risk** | Epic 5.9 "gameplay testing" toolset'i duyurdu (i) ve Input/Perf runtime toolset'leri ekliyor (D1 §2.6). Doğrulanırsa fark daralır. |
| **Risk** | Epic, Slate Inspector'ı Playwright benzeri ref'lerle editör UI'si için yaptı (D1 §1.7). "Playwright for Unreal" sözü Epic'le karışabilir → "for Unreal **games**" vurgusu gerekli. |
| **Risk** | Playwright'ın kendisi coding agent'lar için CLI'yi öneriyor. Yalnız MCP olarak konumlanmak eksik kalabilir. |
| **Risk** | UE-MCP çok oyunculu PIE'da önde ve 1510 action ile hızlı büyüyor (D2). |

### 5.4 Önerilen konum cümlesi (karar Talha'da)

> **Enginewright: Playwright for Unreal games. Play the game, look at it, assert what the player sees.**

Üç dayanak: (1) oyna = `pie_*`, `game_run`, çok oyunculu; (2) bak = screenshot + `image_stats`; (3) doğrula = `ui_*` + gelecek `verify_*` + trace. Katalog genişliğinde Epic ve UE-MCP ile yarışılmaz (D2 §8 #5).

---

## 6. Önceki belgelerdeki düzeltmeler

| Belge | Eski bilgi | Doğrusu (kaynak) |
|---|---|---|
| `01_Rakip_ve_Emsal_Karsilastirma.md` §1 | chongdashu "487–489 araç" | **32** MCP aracı (D3 §2). 489, EngineWire'ın action sayısı (D2 §1) |
| aynı | db-lyon "C++ editör içi" | Node daemon + shim + C++ eklenti, 3 katman (D2 §1) |
| aynı | VibeUE "~1510 + Epic 2171" | **1048** `UFUNCTION`; 1510+2171 UE-MCP'nin sayısı (D3 §7.2, D2 §1) |
| aynı | Claireon "27 araç" | Yüzey **2** araç; 27 IvanMurzak'ın çekirdek sayısı (D3 §7.3, D2 §1) |
| aynı | Epic "55 toolset / 870 araç" | Kaynakta **58 / 878** (+3 meta); 870/55 envanter JSON'un sayısı (D1 §0) |
| `rakip_analizi_2026-11.md` §1 | flopperam/Aura, voodoofox, UnrealGenAISupport, Epic sayıları | D1 §3, D3 §9, D4 §0 düzeltmeleri geçerli; bu belgeye işlendi |
| `rakip_analizi_2026-11.md` §2 | "Multiplayer PIE: UE-MCP compare: sadece Enginewright/UE-MCP'de var" | Doğru; EngineWire'da yok (D2 §2) |

---

## 7. Doğrulanamayanlar ve sonraki adımlar

| Konu | Neden doğrulanamadı | Nasıl doğrulanır |
|---|---|---|
| Epic 5.9 araç sayısı ve "gameplay testing" kapsamı | 5.9 kaynağı yerelde yok | 5.9 kur → `docs/arastirma/derin/araclar/extract_epic_toolsets.py` |
| Unity resmi CLI MCP araç listesi | Doküman araç listesi vermiyor | `unity mcp` kurup `tools/list` |
| winyunq, appleweed, softdaddy-o, aadeshrao123, remiphilippe, Codeturion yetenekleri | Kod okunmadı (yalnız README) | `git clone --depth 1` + D3 yöntemi |
| hi-godot/godot-ai, IvanMurzak/Unity-MCP doğrulama/geri alma | README'de yok | Kod okuması |
| Aura, Ludus, NeoStack, UEC araç sayıları | Kapalı kaynak | Deneme hesabı (ücretli olabilir) |
| Kurulum ve doküman puanları | Değerlendirme | Temiz makinede kurulum süresi ölçümü |

**Not:** `docs/plan_2026-11/03_Yol_Haritasi.md` aynı numarayı taşıyor. Bu dosyanın adı kart gereği `03_Emsal_Karsilastirma.md`. Yeniden numaralandırma kararı orkestratörde (`t_1e95fc46`).
