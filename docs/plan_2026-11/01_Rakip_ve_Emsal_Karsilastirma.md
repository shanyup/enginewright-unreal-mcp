# 01 — Rakip ve Emsal Karşılaştırması

> **Güncelleme (17.11.2026, `t_bea7137b`):** §1 tablosunda 4 araç sayısı hücresi yanlış (chongdashu, db-lyon mimarisi, VibeUE, Claireon) ve Epic sayısı 55/870 yerine kaynakta 58/878. Düzeltilmiş tablo: `03_Emsal_Karsilastirma.md` §1.2 ve §6.

Tarih: 17.11.2026. Yıldız/push/lisans: GitHub REST API 17.11.2026 (ham veri `$TMPDIR/ewplan/repos.txt` + canlı arama). Özellik hücreleri: `docs/arastirma/derin/01–04` (10.11 kaynak kodu okuması) + web. "Tahmin" = doğrulanmamış. "—" = veri yok.

## 1. UE ekosistemi — rakip tablo (GitHub "unreal mcp" arama ilk 25, yıldız sırası)

| Sıra | Proje | ★ | Fork | Son push | Oluşturma | Lisans | Mimari / Kurulum | Araç sayısı | UE sürüm | Doğrulama / Geri alma | Görsel doğrulama | PIE / Oyun testi | Doküman |
|---|---|---:|---:|---|---|---|---|---|---|---|---|---|---|
| 1 | scenario-labs/skills | 3837 | 335 | 2026-11-16 | 2026-08-12 | MIT | Python MCP + 3D üretim (UE/Unity/Blender) | ~565 (16 MCP) | 5.x (tahmin) | — | — | — | İyi |
| 2 | **chongdashu/unreal-mcp** | **2118** | 349 | **2025-04-22** | **2025-03-28** | **Yok** | Python MCP sunucu + UE plugin (pip + plugin) | **487–489** (EngineWire raporu) | 5.x (tahmin) | ❌ | 🟡 | ❌ | İyi README |
| 3 | flopperam/unreal-engine-mcp | 1119 | 207 | 2026-06-26 | 2025-08-06 | Yok | Aura ticari ürününe geçti | — | — | — | — | — | Yönlendirme |
| 4 | **ChiR24/Unreal_mcp (EngineWire)** | **1043** | 188 | **2026-11-16** | **2025-09-07** | **MIT** | Python sunucu + editör plugin köprüsü (pip + plugin) | **~487** (dev `2792ba1`) | 5.0–5.8 | ✅ | 🟡 | 🟡 (girdi var, N-istemci yok) | İyi demo |
| 5 | Natfii/UnrealClaude | 1036 | 153 | 2026-06-26 | 2026-01-03 | Yok | C++ editör içi sohbet + MCP | 5.x (tahmin) | 5.7 | — | — | — | İyi |
| 6 | genex-games/genex-desktop | 875 | 107 | 2026-11-16 | 2026-10-02 | MIT | Masaüstü uygulama (UE/Unity/Godot) | — | 5.x | — | — | — | İyi |
| 7 | **kevinpbuckley/VibeUE** | **838** | 181 | **2026-11-15** | **2025-09-12** | **MIT** | C++ editör içi, Epic 5.8 MCP genişletmesi | **~1510 + Epic 2171** (derin rapor) | 5.x | ✅ (TransactionService) | 🟡 | 🟡 | İyi |
| 8 | islandmed/SculptForge | 718 | 89 | 2026-11-16 | 2026-10-21 | GPL-3.0 | Blender sculpt eklentisi (UE aktarım) | — | — | — | — | — | — |
| 9 | prajwalshettydev/UnrealGenAISupport | 686 | 111 | 2026-04-28 | 2024-11-17 | MIT | Çoklu LLM + MCP sunucusu | — | 5.x | — | — | — | İyi |
| 10 | **kvick-games/UnrealMCP** | **616** | 81 | **2025-06-22** | **2025-03-12** | **Yok** | C++ editör içi | — | 5.x | — | — | — | Zayıf |
| 11 | **db-lyon/ue-mcp** | **505** | 77 | **2026-11-16** | **2026-02-24** | **MIT** | C++ editör içi | **1510 + 2171** (derin rapor) | 5.x | 🟡 | 🟡 | ❌ | İyi |
| 12 | VedantRGosavi/UE5-MCP | 437 | 59 | 2025-06-02 | 2025-03-15 | NOASSERTION | C++ editör içi | — | 5.x | — | — | — | Zayıf |
| 13 | tumourlove/monolith | 372 | 95 | 2026-10-18 | 2026-03-06 | MIT | C++ editör içi (GAS/Niagara/AnimBP derin) | **~135 GAS** (derin rapor) | 5.7–5.8 | 🟡 | 🟡 | ❌ | Zayıf |
| 14 | winyunq/UnrealMotionGraphicsMCP | 223 | 42 | 2026-08-02 | 2025-09-04 | MIT | UMG odaklı MCP | — | 5.x | — | — | — | Zayıf |
| 15 | per-simmons/unreal-agent-harness | 211 | 30 | 2026-06-24 | 2026-06-22 | Yok | Epic MCP üstü ajan kuşatması | — | 5.8 | — | — | — | — |
| 16 | ayeletstudioindia/unreal-analyzer-mcp | 161 | 33 | 2025-08-06 | 2025-01-03 | MIT | Analiz odaklı | — | 5.x | — | — | — | — |
| 17 | GenOrca/unreal-mcp | 149 | 18 | 2026-07-07 | 2025-05-17 | Apache-2.0 | Python MCP | — | 5.x | — | — | — | Zayıf |
| 18 | believer-oss/Claireon | 147 | 18 | 2026-09-15 | 2026-06-10 | MIT | C++ + .NET köprü | **27** (derin rapor) | 5.x | ✅ (Transaction) | ❌ | ❌ | Zayıf |
| 19 | IvanMurzak/Unreal-MCP | 60 | 9 | 2026-11-16 | 2026-06-10 | Apache-2.0 | C++ + .NET + npm CLI | **27** (derin rapor) | 5.x | ✅ | ❌ | ❌ | Zayıf |
| 20 | runreal/unreal-mcp | 121 | 28 | 2025-06-06 | 2025-05-28 | MIT | Python Remote Exec | — | 5.x | — | — | — | — |
| 21 | HurtzDonutStudios/ai-forge-mcp | 102 | 8 | 2026-04-09 | 2026-03-25 | Yok | 565 araç / 16 MCP (Tripo/Muddy/Substance/Maya/Houdini/Blender) | — | — | — | — | — | — |
| 22 | Codeturion/unreal-api-mcp | 100 | 12 | 2026-10-26 | 2026-02-26 | MIT | UE API arama MCP | — | 5.x | — | — | — | — |
| 23 | appleweed/UnrealMCPBridge | 64 | 16 | 2026-11-11 | 2025-04-07 | MIT | Editor Python API köprüsü | — | 5.x | — | — | — | — |
| 24 | softdaddy-o/yes-ue-mcp | 87 | 22 | 2026-03-16 | 2025-12-16 | MIT | Native C++ plugin 5.4+ | — | 5.4+ | — | — | — | — |
| 25 | remiphilippe/mcp-unreal | 82 | 16 | 2026-02-20 | 2026-02-20 | Apache-2.0 | Headless build/test MCP | — | 5.7 | — | — | — | — |

**Not:** scenario-labs/skills (3.8k★), genex-games/genex-desktop (875★), islandmed/SculptForge (718★), VedantRGosavi/UE5-MCP (437★) "unreal mcp" aramasında çıkıyor ama **UE MCP değil** (3D üretim / masaüstü uygulama / sculpt / genel MCP). Tabloya "filtre sebebi" notuyla eklenmiştir.

---

## 1b. Bizim repo: github.com/shanyup/enginewright (canlı 17.11)

| ★ | Fork | Son push | Oluşturma | Lisans | Topic'ler (7) | Açıklama |
|---|---|---|---|---|---|---|
| **10** | **0** | **2026-11-10** | **2026-11-07** | **MIT** | ai-agents,claude,codex,mcp,model-context-protocol,unreal-engine,unreal-engine-plugin | In-editor MCP server for Unreal Engine: strict, verified, undoable tool calls for AI agents. No Python/Node required. Early preview. |

**Arama sıralaması (GitHub REST, 17.11):**
- `unreal mcp` (best match): 673 sonuç, enginewright **198.**
- `unreal engine mcp` (best match): 405 sonuç, enginewright **80.**
- `unreal engine mcp server`: 151 sonuç, enginewright **34.**
- `unreal mcp in:name`: 247 sonuç, enginewright **yok** (adı "unreal-mcp" içermez).
- `ue5 mcp`: 177 sonuç, enginewright **yok**.

---

## 2. Diğer motorların emsalleri — neden büyüdüler (kaynaklı)

| Proje | ★ (17.11) | Neden büyüdü (kaynak: web, star-history.com, Simon Willison blog, VS Code blog) | Büyüme milestones (star-history) | Dersimiz |
|---|---:|---|---|---|
| **ahujasid/mcp-for-blender** (eski `blender-mcp`) | **34.6k** | 07.03.2025 açıldı; demo videolar; Rodin/Hyper3D entegrasyonu (13–16.03.2025); AI Engineer World's Fair 2025 sunumu (11k★ o an); HN + GitHub Trending | 10k★ **May 2025** (≈2 ay); 27k★ Ağu 2026 | Tek komut kurulum + 3D üretim demo videoları + HN |
| **microsoft/playwright-mcp** | **39.7k** | 22.03.2025 lansman; accessibility tree (piksel yerine structured snapshot); Simon Willison yazısı (25.03.2025); VS Code 1.99 Copilot agent mode default (Mar 2025); Anthropic Claude Code entegrasyonu | 25k+★ **ilk yıl içinde**; HN front page launch thread | Resmi destek + accessibility snapshot diferenciation + VS Code entegrasyonu |
| **upstash/context7** | 42.9k | Dokümantasyon MCP; tek satır kurulum | — | Docs = growth kanalı |
| **modelcontextprotocol/servers** (resmi referans) | 114k | Resmi MCP referans sunucular listesi | — | Liste itself = trafik |
| **punkpeye/awesome-mcp-servers** | 97.8k | Glama score badge zorunlu (CI check); sadece GitHub-hosted, installable server | — | Listeye girmek = kalite damgası |
| **CoplayDev/unity-mcp** | 16.0k | Git URL ile tek komut kurulum (Package Manager); ACM SA'25 makalesi; Resmi MCP registry (`mcp/coplaydev/unity-mcp`); Autopilot duyurusu; Discord topluluğu | 14.8k (OSS Insight) ↔ 16k | Akademik makale + resmi registry + tek komut |
| **IvanMurzak/Unity-MCP** | 4.6k | C# köprü, çok motor | — | — |
| **Coding-Solo/godot-mcp** | 6.7k | Godot topluluğu, tek komut | — | Niş motor = az rekabet |
| **ChromeDevTools/chrome-devtools-mcp** | 57.6k | Google resmi, 2025 lansman | — | Resmi destek = güven |
| **GLips/Figma-Context-MCP** | 16.2k | Tasarımcı kitlesi, viral tweetler | — | Dikey kitle büyütür |
| **github/github-mcp-server** | 34.5k | GitHub resmi | — | Platform entegrasyonu |

---

## 3. Özellik matrisi (satır: kategori, sütun: oyuncu)

Derinlik: ✅ var, 🟡 kısmi, ❌ yok. Enginewright sütunu 17.11 kod sayımından (**129 araç**, 129 tekil ad `Register(TEXT("..."))` grep); Epic sütunu `epic_ue58_inventory.json` (**55 toolset / 870 araç**); community sütunu derin raporlardan (10.11, kaynak kod okuması); emsal sütunları kendi README/dokümanlarından.

| Kategori | Enginewright 0.4.x | Epic UE 5.8 MCP | Community UE MCP (ChiR24/chongdashu) | unity-mcp | mcp-for-blender | playwright-mcp |
|---|---|---|---|---|---|---|
| Aktör spawn/taşı/sil/bul | ✅ (`actor_*`) | ✅ ActorTools 17 | ✅ | ✅ | ✅ | n/a |
| Asset import/rename/references | ✅ (`asset_*`) | ✅ AssetTools 21 | 🟡 | ✅ | 🟡 (import yok, create var) | n/a |
| Blueprint düzenleme (node ekle/sil/bağla) | ✅ (`blueprint_*` 10+) | ✅ BlueprintTools 53 | 🟡 | ✅ | ❌ (kod çalıştırır) | n/a |
| DataTable / CSV | 🟡 `datatable_*` var, CSV yok | ✅ DataTableTools 10 | 🟡 | ✅ | ❌ | n/a |
| StringTable / loc | ✅ (`stringtable_*`, `loc_*`) | ✅ StringTableTools 8 | ❌ | 🟡 | ❌ | n/a |
| Malzeme/doku | ✅ (`material_*`, `texture_info`) | ✅ MaterialTools 22, TextureTools 2 | 🟡 | ✅ | 🟡 | n/a |
| PIE başlat/durdur/oyun içi input | ✅ `pie_*` 8 araç | ❌ (sadece automation test) | 🟡 | ✅ (playmode) | n/a | n/a |
| Paketli oyun çalıştırma (`game_run`) | ✅ `game_run`/`status` | ❌ | ❌ | 🟡 | n/a | n/a |
| Ağ / replikasyon test | 🟡 `pie_net_emulation` tek | ❌ | ❌ | 🟡 | n/a | n/a |
| Cook / package (build) | ❌ | ❌ | ❌ | ❌ | n/a | n/a |
| Screenshot / görsel doğrulama | ✅ `screenshot`, `editor_capture`, `image_stats`, `screenshot_ab`, `image_compose`, `screen_coverage`, `actor_preview` | ❌ | 🟡 | ✅ | ✅ viewport screenshot | ✅ screenshot + a11y snapshot |
| Perf ölçüm | ✅ `perf_*`, `mrq_*` | 🟡 | ❌ | 🟡 | ❌ | ✅ (traces) |
| Log / crash analizi | 🟡 `log_read`/`log_wait`, analiz yok | ✅ LogsToolset 4 | 🟡 | 🟡 | ❌ | ✅ console/network log |
| UI/Slate otomasyonu | ✅ `ui_*`, `widget_*` (~20 araç) | ✅ SlateInspector 14 | 🟡 | 🟡 | ❌ | ✅ a11y tree + click |
| Doğrulama / geri alma / audit | ✅ `validate`, `batch`, `doctor`, 3 audit | 🟡 logs only | ❌ | ❌ | ❌ | ✅ deterministic refs |
| Toplu (batch) işlem | ✅ `batch` | ❌ | ❌ | ❌ | ❌ | n/a |
| Client config üretimi (tek komut kurulum) | ✅ `client_config` | ✅ GenerateClientConfig komutu | 🟡 manuel | ✅ "Configure All Clients" | ✅ tek JSON | ✅ npx |
| Doküman dili | EN README + TR iç docs | EN resmi | EN | EN + site | EN | EN |
| Lisans | MIT | Epic EULA | MIT çoğunluk | MIT | MIT | Apache-2.0 |

---

## 3b. Derin raporlardan dolu alanlar (EngineWire / UE-MCP / Ivan / küçük açık kaynak)

| Özellik | EngineWire | UE-MCP (db-lyon) | IvanMurzak | Monolith | VibeUE | UnrealClaude | Not |
|---|---|---|---|---|---|---|---|
| FBX skeletal import + hedef skeleton | W (`skeletonPath`) | W (`import_skeletal_mesh`) | — | K | K | — | 1.0 için kritik açık |
| Socket | W | W | — | W | — | — | 0.4.1 |
| Physics Asset | W | R+W constraint | — | R | K | — | 0.4.1 |
| IK Rig / Retarget | W (otomatik retargeter) | W (en derin, batch) | — | W | — | — | 0.4.1 |
| AnimBP state machine | W | W | — | W | W | K | 0.5 |
| Montage / BlendSpace / Notify | W | W | — | W | W (montage) | K | 0.4.1 |
| Mutable | — | **W + compile** | — | — | W | — | Yalnız UE-MCP |
| Niagara | W | W | S | W | W | — | 1.0 |
| Sequencer / MRQ | W | W | — | R | — | — | 1.0 |
| BT / StateTree / EQS | W | W (+Chooser) | — | W | W | K | 0.5 |
| GAS | W | W | S | W (135) | K | — | 1.0 |
| Control Rig | W | W/R | S | W | — | — | 1.0 |
| Multiplayer PIE (N client) | 🟡 (girdi var, N-istemci yok) | **W** (numClients, netMode, ayrı süreç, windowless) | — | — | — | — | UE-MCP önde |
| Standalone oyun çalıştırma | — | — | — | — | — | — | **Bizde var** (`game_run`) |
| Packaging / Cook | W (BuildCookRun tam) | S (`cook_content`) | — | — | — | — | — |
| Profiling (Insights/trace) | S/W | W (Insights trace, CSV) | — | — | W (trace+verdict) | — | **EW açığı** |
| Headless / commandlet | — | test altyapısında | — | — | — | K (`CommandletRun`) | Hiçbirinde ürün özelliği yok |
| Python exec | W (1 MB sandbox) | W (gated) | W | W | W | — | Bilinçli tercih mi? |
| Visual QA (A/B + SSIM) | — | — | W (4 mod) | — | — | — | **Bizde öndeyiz** |
| Lokalizasyon (gather + font) | S (widget key) | W (target, gather, StringTable) | — | — | — | — | **Biz ve UE-MCP eşit**; `.po`/`.locres` karşılaştırma ve font coverage **yalnız bizde** |
| Source control | S (1 action) | W (51) | — | — | — | — | — |
| Validation (data, map, BP) | W | W | — | — | W | — | — |
| Undo / Transaction | undo/redo + restore point | undo/redo/steps + batch | tek transaction | 67 dosya `FScopedTransaction` | TransactionService | — | **Bizde `batch` atomik + rollback**; tek `undo` aracı yok |
| Checkpoint / flow rollback | restore point | **shadow-git checkpoint + flow rollback** | dry_run | — | — | — | 1.0 adayı |
| Güvenlik / izin | consent ledger, token, loopback | guards, locks | OAuth (bulut), nonce lease | — | — | — | Origin kontrolü **eksik** (1.0 öncesi) |
| Çoklu UE sürümü | 5.0–5.8 compat header | 5.4–5.9 makro + per-engine binary | 5.5–5.9 CI matrisi | — | — | — | 1.0 kararı gerekli |

---

## 4. Nerede öndeyiz / gerideyiz / kimsede yok

| Durum | Özellik | Not |
|---|---|---|
| **Öndeyiz** | PIE + oyun içi test (`pie_*`) | Epic'te yok; community'de zayıf |
| **Öndeyiz** | Paketli oyun çalıştırma (`game_run`) | Kimsede yok |
| **Öndeyiz** | Görsel doğrulama zinciri (screenshot → image_stats → compose + SSIM) | Epic'te yok |
| **Öndeyiz** | Doğrulama/geri alma/audit (`validate`, `batch`, `doctor`, 3 audit) | Kimse bu kadar yok |
| **Öndeyiz** | Loc/StringTable (`.po`/`.locres` karşılaştırma, font coverage) | Sadece Epic'te var (bizde fazlası) |
| **Öndeyiz** | Tek komut client config (`client_config`) | playwright/unity-mcp kadar otomatik |
| **Gerideyiz** | Toplam araç sayısı (129 vs Epic ~870) | Beklenen; her sürüm kapanır |
| **Gerideyiz** | Yıldız (10 vs 2.118 lider chongdashu) | 05'te plan |
| **Gerideyiz** | Registry/liste görünürlüğü | Henüz hiçbir listede yok |
| **Gerideyiz** | Demo video / viral içerik | Yok |
| **Kimsede yok** | Cook/package tek komut | Kategori boş (Epic dahil) |
| **Kimsede yok** | Sahne snapshot + diff | Kategori boş |
| **Kimsede yok** | Görsel inceleme grid'i (N screenshot toplu karşılaştırma) | Kategori boş |
| **Kimsede yok** | 2-instance co-op test otomasyonu | Kategori boş |

---

## 5. Epic resmi MCP ile çakışma riski ve farklılaşma

**Risk:** UE 5.9/6.0'da Experimental → stabil olunca "resmi" araç olur; Epic'in toolset'leri genişler, genel "editör otomasyonu" ayağı daralır.

**Farklılaşma stratejisi (karar verilebilir):**

1. **Oyun iş akışlarına odaklan, editör CRUD'ına değil:** cook/package, PIE senaryoları, replikasyon/ağ testi, görsel QA, crash analizi. Epic oralarda zayıf (bkz matris satırları ❌).
2. **"Tek geliştirici = haftalık release"** — Epic release trenine bağlı değil; şeffaf roadmap + CHANGELOG.
3. **Keşif kanalları:** resmi MCP registry, awesome-mcp-servers, Glama — Epic listelerde yok, biz gireceğiz (04/05).
4. **Kurulum kolaylığı + demo video:** tek komut `client_config`, 30–60 sn videolar.
5. **Konumlandırma cümlesi:** "Playwright for Unreal" (zaten README'de) — Playwright MCP'nin 39.7k★ modeli: structured state (bizde: JSON envanter + screenshot zinciri), determinism (bizde: validate/batch), hazır client entegrasyonu.

---

## 6. Yıldız büyüme proxy'si (fork tarihi dağılımı → star-history yerine)

| Proje | Oluşturma | Fork 100. Tarih | Fork 500. Tarih | Yorum |
|---|---|---|---|---|
| chongdashu/unreal-mcp | 2025-03-28 | 2025-08-12 | — | 4.5 ay 100 fork; son 1.5 yıldır push yok |
| ChiR24/Unreal_mcp | 2025-09-07 | 2026-05-11 | — | 8 ay 100 fork; aktif güncelleniyor |
| kevinpbuckley/VibeUE | 2025-09-12 | 2026-07-13 | — | 10 ay 100 fork; aktif |
| db-lyon/ue-mcp | 2026-02-24 | — | — | 77 fork, 100'e ulaşmamış |
| Natfii/UnrealClaude | 2026-01-03 | 2026-06-11 | — | 5 ay 100 fork |
| flopperam/unreal-engine-mcp | 2025-08-06 | 2026-02-25 | — | 6.5 ay 100 fork; Aura'ya geçti |
| **ahujasid/mcp-for-blender** | **2025-03-07** | **2025-03-13** | **2025-03-18** | **6 gün 100 fork; 11 gün 500 fork → 10k★ May 2025** |
| **microsoft/playwright-mcp** | **2025-03-21** | **2025-03-26** | **2025-05-08** | **5 gün 100 fork; 48 gün 500 fork → 25k+★ ilk yıl** |
| **CoplayDev/unity-mcp** | **2025-03-18** | **2025-03-25** | **2025-12-04** | **7 gün 100 fork; 8.5 ay 500 fork → 16k★** |
| **Coding-Solo/godot-mcp** | **2025-02-26** | **2025-10-17** | **2026-10-25** | **7.5 ay 100 fork; 1.5 yıl 500 fork → 6.7k★** |
| shanyup/enginewright | 2026-11-07 | 0 | — | Yeni |

**Kaynak:** GitHub REST `/repos/{owner}/{repo}/forks?sort=oldest` (17.11.2026). Fork milestone'ları star-history.com ve GitStarClub ile tutarlı. Star-history linkleri: [blender-mcp](https://www.star-history.com/ahujasid/mcp-for-blender/), [playwright-mcp](https://www.star-history.com/microsoft/playwright-mcp/).