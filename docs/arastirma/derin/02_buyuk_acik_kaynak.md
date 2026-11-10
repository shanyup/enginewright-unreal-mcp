# Derin rakip analizi 02: büyük açık kaynak Unreal MCP sunucuları

Kontrol tarihi: **10 Kasım 2026**. Kapsam: EngineWire (ChiR24/Unreal_mcp), UE-MCP (db-lyon/ue-mcp, ue-mcp.com), Unreal-MCP (IvanMurzak). Karşılaştırılan: Enginewright 0.4.0 (`192322b`, 2026-11-10).

**Yöntem.** Üç repo `git clone --depth 1` ile indirildi ve kaynak kodları okundu. Sayılar GitHub REST API'den alındı (`/repos/{r}`, `/releases`).
- EngineWire: `dev` dalı, HEAD `2792ba1` (2026-11-10). Aynı ürünün eski kopyası `C:/Users/shanyup/Desktop/UE/_kaldirilan/EngineWire` yolunda. Brief'te RagnaCook/Plugins geçiyordu ama eklenti oradan kaldırılmış, bu klasöre taşınmış.
- UE-MCP: `main`, HEAD `68a1602`.
- Ivan: `main`, HEAD `7094df5`.

**Etiketler:**
- **doğrulandı**: kaynak kodu veya repodaki dokümanı okudum.
- **iddia**: yalnızca README veya pazarlama metninde geçiyor.
- **bulunamadı**: kaynakta arandı, karşılığı yok.

EngineWire'ın action envanteri `plugins/EngineWire/Source/EngineWire/Private/MCP/Generated/McpCapShard*.cpp` dosyalarından otomatik çıkarıldı (23 domain, 489 action). UE-MCP envanteri repodaki `docs/tool-reference.md` dosyasından sayıldı.

Önceki `rakip_analizi_2026-11.md` raporu bu çalışmada referans alınmadı. Aşağıdaki her bulgu doğrudan kaynaktan geliyor.

URL kısaltmaları:
- `EW:` = `https://github.com/ChiR24/Unreal_mcp/blob/dev/`
- `UM:` = `https://github.com/db-lyon/ue-mcp/blob/main/`
- `IV:` = `https://github.com/IvanMurzak/Unreal-MCP/blob/main/`

---

## 1. Künye karşılaştırması

| | EngineWire | UE-MCP | Unreal-MCP (Ivan) | Enginewright |
|---|---|---|---|---|
| Sürüm (kontrol günü) | Stable 0.6.0 (README). Son release `v0.6.1-beta-b`, 2026-11-08. uplugin `0.6.1-beta-b` (`EW:plugins/EngineWire/EngineWire.uplugin`). **doğrulandı** | `v1.4.13`, 2026-11-10. package.json ve uplugin ikisi de 1.4.13. **doğrulandı** | `v0.30.0`, 2026-11-09. Durum: "beta" (`IV:README.md` satır 43). **doğrulandı** | 0.4.0 (`Plugins/Enginewright/Enginewright.uplugin`) |
| Lisans / fiyat | MIT (`EW:LICENSE`). Fab listesi var (README:10). Fab fiyatı **bulunamadı** | MIT. README:60'a göre "UE-MCP ve Studio ücretsiz". Studio ayrı repo: `db-lyon/ue-mcp-studio-plugin` | Apache-2.0 (`IV:LICENSE`). Varsayılan bağlantı **bulut** `ai-game.dev`, OAuth device flow (README:56, 168). Plan: Free $0 (BYO agent), Indie $24–25/ay, Studio $94–99/ay, Studio Max $189–199/ay (https://ai-game.dev/pricing). Self-host da mümkün: GameDev-MCP-Server | MIT |
| Yıldız / fork / açık issue | 1028 / 187 / 8 | 490 / 76 / 42 | 58 / 9 / 1 | — |
| Son push | 2026-11-10 | 2026-11-10 | 2026-11-10 | — |
| Desteklenen UE | 5.0–5.8 (README:12, 115, 370). **iddia** + kodda koşullu derleme var (bkz. §5) | 5.4–5.9 Windows, 5.6+ macOS/Linux (README:64). Studio yalnız Windows, 5.7/5.8 | "5.5+, CI 5.7 ve 5.8'de" (README rozet). PR CI matrisi 5.5–5.9 (`IV:.github/workflows/test_pull_request.yml`). **doğrulandı** | Yalnız 5.8, Win64 (README rozetleri) |
| Platform | Win64/Mac/Linux (uplugin `SupportedTargetPlatforms`). **doğrulandı** (bildirim düzeyinde) | Win, macOS, Linux (README:64). **iddia** | CI "Any OS" runner'larında koşuyor (workflow yorumları). **doğrulandı** (CI düzeyinde) | Win64 |
| Mimari | Native C++ eklenti, editör içinde MCP sunucusu. Erişim HTTP (`127.0.0.1:3000/mcp`) veya stdio proxy (npm `unreal-engine-mcp-server`, `EW:package.json`). Dışarıya tek MCP tool gösteriliyor ("One tool, over 400 capabilities", README:25); altında 23 domain ve 489 action var | 3 katman (`UM:docs/architecture.md` satır 5–7): (1) proje başına Node.js daemon; (2) stdio relay "shim"; (3) C++ `UE_MCP_Bridge` eklentisi, editörde WebSocket sunucusu çalıştırıp işleri game thread'de yürütüyor. Eklentide 396 `.cpp`, 0 `.py`. Ayrıca `UE_MCP_BridgeRuntime` DeveloperTool modülü PIE client'larına yükleniyor (`UM:docs/pie.md`:44) | C++ editör eklentisi + .NET sidecar (`bridge/src`, C#) + `unreal-mcp-cli` (npm) + bulut/self-host MCP sunucusu. Sidecar Fab paketine gömülmüyor, ilk açılışta indiriliyor (README:110) | Native C++ eklenti, editör içinde MCP sunucusu, 124 ayrı tool |
| Yüzey büyüklüğü | 489 action (dev). Envanter ham çıktısı: `C:/Users/shanyup/AppData/Local/hermes/cache/scratch/ew2/caps_dev.json` | 1510 kendi action'ı + 2185 `epic_*` action'ı. `epic_*`, Epic'in UE 5.8+ AI Toolset Registry'sini sarıyor (`UM:docs/native-tools.md`: "2171 official tools across 137 toolsets") | 27 çekirdek tool + 4 sistem tool + 6 eklenti paketinde 28 tool (`IV:extensions-index.json`) | 124 tool |

---

## 2. EngineWire (ChiR24/Unreal_mcp): alan bazında

Kaynak: `EW:plugins/EngineWire/Source/EngineWire/Private/MCP/Generated/McpCapShard<Domain>.cpp`. Action adlarındaki örnekler action id'leridir. Hepsi **doğrulandı** (şema okundu).

| Alan | Var mı / derinlik | Kanıt |
|---|---|---|
| Skeletal FBX import + hedef skeleton | **Var, write.** `asset.import`: `skeletonPath`, `importMesh`, `importAnimations`, `createPhysicsAsset` (varsayılan true), `physicsAssetPath`. Proje dışı yol `SECURITY_VIOLATION` | McpCapShardManageAsset.cpp |
| Socket | Var, write: `animation_physics.configure_socket` | McpCapShardAnimationPhysics.cpp |
| Physics Asset | Var, write: `edit_physics_asset`, `setup_ragdoll`, `setup_physics_simulation` (skeletal için PhysicsAsset üretip mesh'e atıyor) | aynı |
| IK / retarget | Var, write: `setup_ik`, `setup_retargeting`. IK Retargeter yoksa iki skeleton'ı karakterize edip zincirleri otomatik eşliyor ve AnimSequence bake ediyor | aynı |
| AnimBP / state machine | Var, write: `create_animation_blueprint`, `edit_anim_graph` (create_state_machine, add_state, add_transition, set_transition_rules, blend tree, cached pose, slot), `configure_anim_graph_node` | aynı |
| Montage / BlendSpace / notify | Var, write: `edit_montage` (section, slot, notify, blend in/out), `edit_blend_space`, `auto_blend_space`, `edit_animation` (bone/curve key, notify, sync marker, root motion) | aynı |
| Mutable | **bulunamadı**: kaynakta `CustomizableObject` geçmiyor (0 dosya) | grep |
| Niagara | Var, write: `manage_effect.create_effect`, `edit_niagara_system`, `add_niagara_module`, `add_niagara_data_interface`, `remove_niagara_node`, `spawn_niagara`, `advance_simulation` | McpCapShardManageEffect.cpp |
| Material | Var, write: `material.create_material`, `add_material_node`, `connect_nodes`, `compile_material`, `create_material_instance`, `set_material_parameter`, `configure_layer_blend`, `add_function_io`. Ayrıca `inspect.audit_materials` | McpCapShardMaterial* |
| Sequencer / MRQ | Var, write: `sequence.create`, `edit_sequence_tracks`, `edit_sequence_bindings`, `sequence.cinematic.*`, `sequence.mrq.create_render_job`, `configure_render_job`, `sequence.planner.fly_through`/`auto_edit`, replay/demo | McpCapShardManageSequence.cpp |
| PCG | Sığ, write: `add_pcg_node`, `edit_pcg_graph`, `execute_pcg_graph`. `MCP_HAS_PCG` ile opsiyonel derleniyor | Build.cs:59–60 |
| Landscape / Foliage | Var, write: `create_landscape`, `edit_landscape`, `erode_landscape`, `add_foliage`, `paint_foliage_instances`, `create_foliage_type` | McpCapShardBuildEnvironment.cpp |
| Water | Var, write: `create_water_body`, `configure_water` | aynı |
| BT / StateTree | Var, write: `create_behavior_tree`, `edit_behavior_tree`, `edit_blackboard`, `edit_eqs_query`, `edit_state_tree`, `run_behavior_tree` | ManageAI |
| GAS | Var, write: `create_gas_asset`, `configure_ability`, `configure_attribute_set`, `configure_gameplay_effect`, `configure_asc`, `get_gas_info` | ManageGAS |
| Control Rig | Var, write: `create_control_rig`, `edit_control_rig`, `get_control_rig`, `list_rig_units` | AnimationPhysics |
| MetaHuman | Var: `manage_character.create_metahuman`, `build_metahuman`, `rig_metahuman`, `export_metahuman`, `metahuman_status` | McpCapShardManageCharacter.cpp |
| UMG / CommonUI | UMG güçlü (~70 widget action): `blueprint.add_*_widget`, `bind_widget`, `edit_widget_animation`, `add_widget_viewmodel`/`bind_widget_viewmodel` (MVVM), `audit_widget`, `preview_widget`. CommonUI **bulunamadı** (0 dosya) | ManageBlueprint |
| PIE | `control_editor.play` (play/pause/resume/stop/eject/possess), `playtest` (pawn'ı NavMesh üzerinden hedefe yürütüp sonucu değerlendiriyor), `simulate_input` | McpCapShardControlEditor.cpp |
| Multiplayer PIE | **Çoklu client PIE bulunamadı**: `PlayNumberOfClients` / `EPlayNetMode` kaynakta yok. Var olanlar: `manage_networking.add_local_player` (split-screen), `configure_replication`/`configure_rpc`/`configure_prediction`, `check_authority`, `host_lan_server` | grep + ManageNetworking |
| Packaging / cook | Var, write: `system_control` package, RunUAT `BuildCookRun -cook -stage -archive -package [-pak]` (`McpSystemPackage.cpp`:57–62), `package_status`, `run_build` | doğrulandı |
| Perf profiling | `profile_performance`: benchmark ile avgFps, game/render/GPU thread süreleri; `stat startfile` ile `.uestats` kaydı ("no MCP action reads it", yani dosyayı okuyan action yok). Ayrıca `fit_frame_budget`, `get_trace_status`, `inspect.get_stats` | SystemControl şeması |
| Headless / commandlet | Ayrı bir headless çalıştırma **bulunamadı**. `IsRunningCommandlet` referansları yalnız lifecycle/korumalarda | EngineWireSubsystemLifecycle.cpp |
| Python | `system_control.execute_python`: inline en fazla 1 MB veya proje içi `.py`. Symlink ya da proje dışı yol `SECURITY_VIOLATION`. `run_script` da var | şema |
| Screenshot / görsel QA | `control_editor.screenshot`, `system_control.screenshot`, `inspect.capture_passes` (render pass'leri), scene capture actor'leri | şema |
| Lokalizasyon | Sığ: yalnız widget `set_localization_key` (`McpWidgetLocalization.cpp`) ve metin arama (`McpAssetQueryFindText.cpp`). Gather/compile, culture, StringTable **bulunamadı** | grep `StringTable`/`GatherText`: 0 |
| Source control | `asset.source_control` (tek action, alt modlu) | ManageAsset |
| Asset metadata | `asset.set_metadata`, `manage_level.set_metadata`, `sequence.get/set_metadata`, actor tag | şema |
| Validation | `system_control.validate_assets`, `manage_level.validate_level`, `blueprint.lint_blueprint`, `audit_widget`, `control_actor.audit_placement` | şema |
| Güvenlik | Undo/redo + restore point: `control_editor.undo` "roll back to a restore point, undoing only what this server changed since". UE 5.7+'da enum-editor adımını `UNDO_WOULD_CRASH` ile reddediyor (`McpEditorTransactions.cpp`:22–25). **Consent ledger** (`Core/Security/McpConsentLedger.cpp`). Varsayılan loopback + token (README:319) | doğrulandı |

**Benzersiz:**
- Tek tool + capability router mimarisi; shard'lar kod üretimiyle oluşturuluyor (`MCP/Generated`).
- `playtest`: NavMesh üzerinde yürüyüp başarıyı değerlendiriyor.
- `sequence.planner.fly_through`.
- Vehicle (`configure_vehicle`), cloth bind, morph target ve skin weight düzenleme.
- Inventory/combat/interaction gibi gameplay şablon domain'leri (ManageInventory, ManageCombat).
- Consent ledger.

---

## 3. UE-MCP (db-lyon/ue-mcp): alan bazında

Kaynak: `UM:docs/tool-reference.md` (repo içi, otomatik üretilmiş). Satırlar handler imzalarını içeriyor. Kategori başına kendi/`epic_*` action sayısı:

project 72/31 · asset 168/165 · blueprint 99/92 · level 146/99 · material 62/39 · animation 131/1004 · landscape 45/0 · pcg 38/75 · foliage 15/0 · niagara 45/57 · audio 51/44 · widget 60/139 · editor 164/101 · pie 67/0 · reflection 14/7 · gameplay 106/99 · gas 38/15 · networking 12/0 · statetree 56/49 · chooser 35/0 · metahuman 8/0 · source_control 13/38 · canvas 28/0 · dataflow 0/70 · fab 9 · datasmith 5 · demo 4 · feedback 2 · plugins 2/40 · epic 15/14 · conversation 0/7

Aşağıdakilerin hepsi **doğrulandı** (doküman imzası; bir kısmı handler kaynağında da teyit edildi).

| Alan | Derinlik | Kanıt (tool-reference imzası) |
|---|---|---|
| Skeletal FBX import + hedef skeleton | write: `import_skeletal_mesh(filePath, …, skeletonPath?, …, createPhysicsAsset?:b)`, `import_animation(…, skeletonPath)`, `import_static_mesh`, `reimport` | asset kategorisi |
| Socket | write: `add_socket(assetPath, socketName, boneName?, …)`, `set_socket_transform` | |
| Physics Asset | read + kısmi write: `get_physics_asset`, `get_physics_asset_constraints`, `set_physics_asset_constraint_limits`, `set_physics_asset_body_material`. Oluşturma import sırasında `createPhysicsAsset` ile; bağımsız `create_physics_asset` **bulunamadı** | |
| IK / retarget | write, en derin: `create_ik_rig`, `configure_ik_rig` (UE 5.8+), `create_ik_retargeter(autoMapChains)`, `configure_ik_retargeter`, `set_ik_retargeter_rig`, `auto_align_retarget_pose`, `batch_retarget_animations` | |
| AnimBP / state machine | write: `create_state_machine`, `add_state`, `add_transition(blendDuration, blendLogic)`, `set_transition_condition(variableName, negate)`, `read_graph` | |
| Montage / BlendSpace / notify | write: `read_montage`, `add_montage_segment`, `add_notify`/`remove_notify`, `create_blendspace`, `add_blend_sample`/`set_blend_sample`, `create_sequence` | |
| **Mutable** | **write + compile:** `create_customizable_object` (Mutable factory), `compile_customizable_object(optimizationLevel, textureCompression)`. Graph düzenleme genel EdGraph araçlarıyla (`add_graph_node`, `connect_graph_pins`, `read_graph`). Handler: `UM:plugin/ue_mcp_bridge/Source/UE_MCP_Bridge/Private/Handlers/Asset/AssetHandlers_Graph.cpp` | üç rakip arasında Mutable'ı bulunan **tek** proje |
| Niagara | write: `create_emitter`, `add_emitter`, `create_system_from_spec`, `spawn_actor` + 57 epic Niagara action'ı | |
| Material | write: 62 action. Graph surgery dokümanı var (`UM:docs/material-graph-surgery.md`) | |
| Sequencer / MRQ | write: `create_sequence`, `add_sequence_track`, `add_sequence_section`, `render_sequence_frames`, `mrq_create_job`, `mrq_list_jobs`, `mrq_delete_job`, `mrq_render(wait)` | |
| PCG | 38 kendi + 75 epic action | |
| Landscape / Foliage | 45 + 15 kendi action, `apply_erosion` | |
| Water | read/write: `get_water_state`, `set_water_body_property`, `rebuild_water_zone`. Water body spawn genel actor spawn ile | |
| BT / StateTree | BT: `list_behavior_trees`, `get_behavior_tree_info`, `read_behavior_tree_graph`, `add_bt_node`, `set_bt_node_property`. StateTree: 56 kendi action. Chooser: 35 | |
| GAS | 38 kendi action, canlı `grant_ability` | |
| Control Rig | `create_control_rig`, `read_control_rig_graph`, `read_control_rig_hierarchy`, `set_control_rig_preview_mesh` (`UM:docs/control-rig-animation.md`) | |
| MetaHuman | `status`, `create_character`, `read_character`, `request_rig` (MetaHuman bulutuna auto-rig isteği) | |
| UMG / CommonUI | 60 kendi + 139 epic action. **`audit_commonui`** (read-only CommonUI bağlantı denetimi). Runtime widget inspection, interaction ve layout tanılama dokümanları var | |
| PIE + multiplayer | **En derin:** `play_in_editor`, `configure_pie(numClients, netMode, runUnderOneProcess, launchSeparateServer, …)`, `get_pie_pawn(pieInstance)`. Ayrı süreçteki client'lara runtime modül yükleniyor. `clients` (oyuncu listesi, net mode, client başına log). `pie.windowless` ile `-RenderOffscreen` client'lar. Input kayıt/oynatma (`UM:docs/pie.md`:44, 298, 333, 366; `docs/pie-scripts.md`) | |
| Packaging / cook | `cook_content(platform?)`. Tam BuildCookRun/stage/archive action'ı **bulunamadı** | tool-reference |
| Perf | `start_trace` (Unreal Insights `.utrace`), `start_csv_profile`, trace regions (`UE_MCP_HAS_TRACE_REGIONS`) | |
| Headless | `start_editor(dialogPolicy, extraArgs, …)` editörü başlatıp hazır olana kadar bekliyor. Headless editör yalnız test altyapısında (`UM:docs/development.md`:150). Ürün olarak commandlet modu **bulunamadı** | |
| Python | `execute_python(code, taskSummary, …)`: "GATED LAST RESORT". Her çağrı bir `taskSummary` istiyor ve `execute_python_report` ile ölçülüyor | |
| Screenshot / görsel QA | `capture_screenshot(target, pieInstance)`, `capture_screenshot_sequence`, `capture_scene_png`, `set_view_mode`, `open_tab` (UI kanıt görüntüsü) | |
| Lokalizasyon | write: `list_localization_cultures`, `list/set/remove_localization_target`, `gather_localization_target` (GatherText commandlet'i ayrı süreçte çalıştırıyor), `compile_localization_target` (LocRes üretiyor), `read_stringtable`, `get_stringtable_entry`, `set_stringtable_entry_metadata` | |
| Source control | 13 kendi + 38 epic action: `get_provider`, `get_status`, … | |
| Asset metadata | StringTable metadata var. Genel UPackage metadata setter **bulunamadı** | |
| Validation | `validate_assets`, `run_map_check`, `log_map_check_message` | |
| Güvenlik | `undo`, `redo`, `undo_redo_steps`. `batch(ops, onError, transactionDescription)` tek game-thread işi olarak çalışıyor. **Checkpoints:** `<project>/Saved/UE_MCP_Bridge/shadow.git` içinde isomorphic-git ile dosya bazında keep/revert (`UM:docs/checkpoints.md`). **Flow engine:** YAML flow'lar, her mutating handler `rollback:{method,payload}` döndürüyor, `rollback_on_failure` ile ters işlemler ters sırada uygulanıyor, best effort (`UM:docs/flows.md`:253). Guards, locks, journal (`docs/plugins-guards.md`, `docs/journal.md`) | |

**Benzersiz:**
- Epic AI Toolset Registry sarmalayıcısı (`epic` kategorisi; 2171 resmi tool, UE 5.8/5.9).
- Flow engine + rollback, shadow-git checkpoint.
- Çok editör yönetimi (`list_editors`, `drop_editor`).
- `search_tools` (intent/synonym araması).
- Feedback gönderimi (`feedback.submit`).
- Fab, Datasmith, Dataflow, Chooser, PoseSearch (`UE_MCP_HAS_POSESEARCH_DATABASE_ASSET_API`).
- Studio adlı masaüstü chat istemcisi.

---

## 4. Unreal-MCP (IvanMurzak): alan bazında

Kaynak: `IV:README.md` satır 185–320 (README notuna göre tablo `UnrealMcp*Tools.cpp` kayıtlarından üretiliyor), `IV:docs/rql.md`, `IV:extensions-index.json`.

**Çekirdek (27 tool):**
- RQL: `rql-query`, `rql-mutate`, `rql-schema`.
- Blueprint (11 tool): create, get, add/remove component, add/modify variable, set default, add function, add event, compile (yapılandırılmış hata listesi döner), spawn.
- `console-run-command`.
- `python-execute`.
- Source/C++ (6 tool): read, create-class, update, delete, list, compile (Live Coding ya da UBT).
- Screenshot (4 tool): viewport, game-view, camera, isolated.
- `unreal-tool-list`.

**Sistem tool'ları (AI'a açık değil):** `ping`, `unreal-skill-create` (yeni C++ tool üretir), `unreal-skill-generate`, `editor-quit`.

**Eklentiler (her biri 0.1–0.2 sürüm, hepsi read-ağırlıklı):**
- Niagara: list, get, spawn-component.
- EnhancedInput: list, get, action-create, context-create, add-mapping.
- PCG: list, get, add-component, get-component, generate.
- GAS: list abilities/effects/attribute sets, inspect, grant.
- Landscape: actor ve water body/zone listeleme, get.
- ControlRig: list, get, controls, bones, component.

**RQL:** Tek sorgu dili. `rql-mutate` "all-or-nothing, one undo step, `dry_run`" çalışıyor ve varsayılan en fazla 200 hedefe dokunuyor (`IV:docs/rql.md`:106, 121). Reflection üzerinden genel property set ve `call` mümkün; bu yüzden pek çok alanda "genel yazma" var ama alana özgü (domain) araç yok.

| Alan | Durum |
|---|---|
| Skeletal import / skeleton / socket / Physics Asset / retarget / AnimBP / Montage / BlendSpace / Mutable / Sequencer / MRQ / BT / StateTree / MetaHuman / CommonUI / packaging / lokalizasyon / source control / validation | Alana özgü tool **bulunamadı**. Yalnız `python-execute` ve RQL genel reflection ile dolaylı yapılabilir. |
| Niagara / PCG / GAS / Landscape / Water / Control Rig | Eklenti, sığ (çoğu read). Water yalnız listeleme. |
| UMG | Alana özgü tool **bulunamadı** |
| PIE | RQL `editor` kaynağında `IsPlaying`/`IsPaused` yazılabilir, `PlayState` (rql.md:160). Multiplayer PIE **bulunamadı** |
| Perf | **bulunamadı** |
| Headless | `unreal-mcp-cli` editörü başlatıp kapatıyor (`editor-quit` nonce/lease korumalı). Commandlet modu **bulunamadı** |
| Python | `python-execute` (tam `unreal` API) |
| Screenshot | 4 tool, `screenshot-isolated` actor'ü izole render ediyor |
| Güvenlik | RQL mutasyonu tek transaction, `dry_run`, hedef limiti var. Undo geçmişi RQL ile okunabiliyor (`NextUndo`, `UndoHistory`) |

**Benzersiz:**
- RQL (genel sorgu ve mutasyon dili).
- Unity/Godot kardeş projeleriyle ortak kebab-case yüzey.
- Sidecar mimarisi + bulut yönlendirme (proje başına pinned URL).
- `unreal-skill-create` (AI'nın kendi tool'unu C++ olarak yazdırması).
- SKILL.md üretimi.

---

## 5. Çoklu UE sürüm tekniği

**EngineWire.** Yöntem: tek dal + compat header + koşullu derleme + opsiyonel modül tespiti.
- `EW:plugins/EngineWire/Source/EngineWire/Private/Core/Compatibility/McpVersionCompatibility.h` (407 satır) API farklarını makrolara sarıyor. Örnekler: `MCP_DISALLOW_SHRINKING` (`EAllowShrinking::No` / `false`), `MCP_USER_DEFINED_STRUCT_HEADER` (`StructUtils/…` / `Engine/…`), `MCP_INSTANCED_STRUCT_HEADER`, `MCP_MOVIE_PIPELINE_CONFIG_CLASS` (`UMoviePipelinePrimaryConfig` / `…MasterConfig`), MRQ queue dirty makroları.
- Kodda 208 dosyada `ENGINE_MINOR_VERSION` kontrolü var. En sık geçenler: `>= 1` (146 kez), `>= 5` (49), `>= 3` (48).
- `EngineWire.Build.cs` içindeki `AddOptionalModules` / `AddOptionalModuleGroup` (satır 41–76), engine dizininde modül var mı diye bakıp `MCP_HAS_PCG`, `MCP_HAS_SUBDIVIDE_POLY` gibi define'lar üretiyor. GAS, StateTree, ControlRig, Mass, MRQ ve TakeRecorder opsiyonel.
- Canlı CI ancak self-hosted engine runner varsa çalışıyor (`EW:.github/workflows/ci.yml`:14–17).

**UE-MCP.** Yöntem: tek dal + feature makroları + CI.
- `UM:plugin/ue_mcp_bridge/Source/UE_MCP_Bridge/Public/HandlerEngineVersion.h` (95 satır) şu makroları tanımlıyor: `UE_MCP_HAS_5_4_API` … `UE_MCP_HAS_5_9_API`. Kullanım sayıları: 5_5 → 145, 5_8 → 77, 5_9 → 48, 5_7 → 42, 5_4 → 13.
- Özellik makroları da var: `UE_MCP_HAS_STATETREE_GENERAL_PROPERTY_BINDING`, `UE_MCP_HAS_POSESEARCH_DATABASE_ASSET_API`, `UE_MCP_HAS_TRACE_REGIONS`.
- Release, her motor için ayrı prebuilt bridge binary taşıyor (`UM:.github/workflows/ci.yml`:448).
- Engine'e bağlı testler sürüm öncesinde lokal çalıştırılıyor (ci.yml:78–89).

**Ivan.** Yöntem: tek dal + motorun kendi makroları + çoklu motor CI.
- Motorun kendi `UE_VERSION_OLDER_THAN(5,6,0)` / `(5,7,0)` / `(5,8,0)` makroları kullanılıyor; toplam 12 kullanım. Örnekler: `UnrealMcpPythonTools.cpp`:284, `UnrealMcpScreenshotTools.cpp`:204, `UnrealMcpRqlGameplayTags.cpp`:490.
- Header farkları `__has_include("Misc/StringOutputDevice.h")` ile çözülüyor.
- PR CI'ı 5.5–5.9 matrisinde self-hosted `unreal-5-X` etiketli runner'larda koşuyor, `fail-fast:false` (`IV:.github/workflows/test_pull_request.yml`:255–275).

**Enginewright.** Kaynakta `ENGINE_MINOR_VERSION` veya `UE_VERSION_*` kullanımı **yok** (grep 0). Yalnız 5.8 hedefleniyor. 1.0'da çoklu sürüm isteniyorsa Ivan modeli (motor makroları + `__has_include` + engine-etiketli CI matrisi) en düşük maliyetli yol. EngineWire'ın Build.cs opsiyonel-modül tespiti de eklenmeli.

---

## 6. Özet tablo

Derinlik: W = write/authoring, R = read-only, S = sığ, — = bulunamadı. "Bizde" sütunu Enginewright kaynağına bakılarak dolduruldu (`Plugins/Enginewright/Source/**`, `Register(TEXT("…"))` grep'i, 124 tool).

| Özellik | EngineWire | UE-MCP | Ivan | Enginewright | Not |
|---|---|---|---|---|---|
| FBX skeletal import + hedef skeleton | W (`skeletonPath`, `importMesh`) | W (`import_skeletal_mesh` skeletonPath, `import_animation`) | — | **Kısmi**: `asset_import` fbx/gltf alıyor, skin'i algılıyor (`ImportValidationTools.cpp`:37–54) ama skeleton/physics parametresi yok | 1.0 için kritik açık |
| Socket | W | W | — | — | |
| Physics Asset | W (oluştur, düzenle, ragdoll) | R + constraint/material W | — | — | |
| IK Rig / retarget | W (otomatik retargeter + bake) | W (en derin, batch) | — | — | |
| AnimBP state machine | W | W | — | — | |
| Montage / BlendSpace / notify | W | W | — | — | |
| Mutable | — | **W + compile** | — | — | Yalnız UE-MCP'de var |
| Niagara | W | W | S (eklenti) | — | |
| Material | W | W | — | W (`material_create`, `material_add_expression`, `material_connect`, `material_instance_*`, `material_info`) | Bizde var, daha dar |
| Sequencer / MRQ | W | W | — | — | |
| PCG | S/W | W | S | — | |
| Landscape / Foliage | W | W | S (R) | Kısmi: `scene_dress` (toplu mesh yerleştirme, tek undo) | |
| Water | W | R/W | S (R) | — | |
| BT / StateTree | W | W (+Chooser) | — | — | |
| GAS | W | W | S | — | Bizde yalnız gameplay tag (`gameplay_tag_*`) |
| Control Rig | W | W/R | S (R) | — | |
| MetaHuman | W | W (S) | — | — | |
| UMG | W (~70) | W (60+139) | — | W (`widget_*` 7, `widget_render`, `ui_*` 9) | Bizde UI test tarafı güçlü |
| MVVM | W | bulunamadı (kendi action'ı yok) | — | W (`viewmodel_*` 5, opsiyonel eklenti) | |
| CommonUI | — | R (`audit_commonui`) | — | — | |
| PIE otomasyonu | W (play, possess, playtest, simulate_input) | W (+input kayıt/replay, pie-scripts) | S (RQL) | W (`pie_start`/`stop`/`state`/`actors`/`input`/`walk_to`/`travel`, `ui_click`/`ui_key`) | |
| Multiplayer PIE | — (yalnız split-screen local player) | **W** (numClients, netMode, ayrı süreç, windowless) | — | W: `pie_start` players 1–4, netMode standalone/listen/client (`PieTools.cpp`:234–248), `pie_net_emulation` | UE-MCP'nin ayrı süreç ve client log desteği bizde yok |
| Standalone oyun çalıştırma | bulunamadı | bulunamadı | — | W (`game_run`: culture, cvars, variants, shotNames) | Bizde var |
| Packaging / cook | W (BuildCookRun tam) | S (`cook_content`) | — | — | |
| Perf profiling | S/W (benchmark, .uestats) | W (Insights trace, CSV profile) | — | — | |
| Headless / commandlet | — | test altyapısında | — | — | Hiçbirinde ürün özelliği olarak yok |
| Python | W (1 MB, sandbox yolu) | W (gated) | W | — | Bilinçli tercih olabilir |
| C++ kaynak + derleme | bulunamadı | W (`build` UBT, `live_coding_compile`, `live_coding_status`, `build_engine_index`) | W (`source-*`, compile) | W (`live_coding_compile`, `build_status`) | |
| Screenshot / görsel QA | W | W (+sequence, view mode) | W (4 mod) | **W+** (`screenshot`, `screenshot_ab`, `editor_capture`, `widget_render`, `image_stats`, `image_compose`, `screen_coverage`, `actor_preview`) | Görsel karşılaştırma ve istatistikte biz öndeyiz |
| Lokalizasyon | S (widget key) | W (target, gather, StringTable) | — | **W** (`loc_gather` gather+compile+check, `loc_culture`, `loc_gather_status`, `stringtable_get`/`set`/`list`/`compare` .po/.locres, `font_coverage`, `key_audit`, `id_audit`) | Biz ve UE-MCP eşit güçte. `.po`/`.locres` karşılaştırma ve font coverage yalnız bizde |
| Source control | S (1 action) | W (51) | — | — | |
| Asset metadata | W | S | RQL | — | |
| Validation | W | W (data validation, map check) | — | W (`validate`: blueprint, data, references, map) | |
| Undo / transaction | undo/redo + restore point | undo/redo/steps + batch | tek transaction RQL | `batch` atomic + rollback (`BatchTool.cpp`:30), 20 dosyada `FScopedTransaction` | |
| Checkpoint / flow rollback | restore point | **shadow-git checkpoint + flow rollback** | dry_run | — | |
| Güvenlik / izin | consent ledger, token, loopback | guards, locks | OAuth (bulut), nonce lease | `EWRegistry` riskli tool listesi (`console_command`, `game_run`…, `EWRegistry.cpp`:108) | |
| Save game | bulunamadı | bulunamadı | — | W (`savegame_get`/`savegame_set`, checksum koruması) | Yalnız bizde |
| Input (Enhanced Input) | W (`manage_networking.configure_input`) | W | S (eklenti W) | W (`input_*` 7, `ui_input_audit`, `ui_prompts`) | |
| Çoklu UE sürümü | 5.0–5.8 compat header | 5.4–5.9 makro + per-engine binary | 5.5–5.9 CI matrisi | yalnız 5.8 | 1.0 kararı gerekli |
| Epic AI Toolset Registry köprüsü | — | **W (2171 tool)** | — | — | UE 5.8+ |

---

## 7. Bizde olup onlarda olmayan

Kanıt olarak Enginewright tool adları ve kaynak yolları veriliyor. "Onlarda yok" iddiası, rakip kaynaklarında yapılan grep ve tool listesi taramasına dayanıyor.

1. **Standalone oyun çalıştırma ve varyant matrisi.** `game_run` (`Tools/GameRunTools.cpp`:991) culture, cvars, variants ve shotNames parametreleri alıyor; `game_run_status` ile izleniyor. Üç rakipte de paketlenmemiş standalone oyunu parametreli koşturan tool bulunamadı.
2. **Görsel QA analitiği.** `screenshot_ab`, `image_stats` (parlaklık, histogram, fark), `image_compose`, `screen_coverage`, `class_measure`, `actor_preview` (PIE olmadan sınıf fotoğrafı). Rakiplerde ekran görüntüsü var ama sayısal karşılaştırma ya da analiz yok.
3. **Lokalizasyon kalite döngüsü.** `stringtable_compare` (.po/.locres okuma), `font_coverage` (glif kapsamı), `key_audit`, `id_audit`. UE-MCP'de gather ve StringTable var, bu denetimler yok.
4. **Oyuncu gözünden UI testi.** `ui_state` (yalnız oyuncunun kullanabildiği öğeler), `ui_prompts` + `input_map_list` (ekrandaki ipucu ile gerçek binding karşılaştırması), `ui_input_audit`, `ui_input_type`, `ui_click`/`ui_key`/`ui_open`/`ui_close`. UE-MCP'nin runtime widget inspection'ı en yakın rakip; prompt/binding tutarlılık denetimi orada yok.
5. **Save game okuma/yazma.** `savegame_get`/`savegame_set`, reflection tabanlı, checksum korumalı.
6. **PIE seviye geçişi ve yol yürüme.** `pie_travel`, `pie_walk_to`. EngineWire'daki `playtest` benzer ama `pie_travel` karşılığı bulunamadı.
7. **Log bekleme.** `log_wait` ile ajan belirli bir log satırını bekleyebiliyor. Rakiplerde log okuma var, bekleme semantiği bulunamadı.
8. **Ayrı tool modeli.** 124 düz tool, tek tool + router yok (EngineWire) ve bulut bağımlılığı yok (Ivan). Bu bir avantaj mı tasarım tercihi mi, ayrıca tartışılmalı.

---

## 8. 1.0 roadmap için çıkarımlar

1. **Animasyon hattı en büyük açık.** EngineWire ve UE-MCP'nin ikisinde de write düzeyinde olup bizde olmayanlar:
   - skeleton hedefli FBX import,
   - socket,
   - Physics Asset,
   - IK retarget,
   - state machine,
   - montage/notify.
   
   Mutable yalnız UE-MCP'de var. Mutable'a girersek ikinci oyuncu oluruz.
2. **Multiplayer PIE'da** temel netMode ve oyuncu sayısı bizde var. Ayrı süreç client'lar, client başına log ve windowless client'larda UE-MCP önde.
3. **Güvenlik modelinde** UE-MCP'nin dosya bazlı shadow-git checkpoint'i ve flow rollback'i, bizim atomic `batch`'imizden kapsamlı. Restore point bir 1.0 adayı.
4. **Çoklu sürüm** için §5'teki Ivan modeli (motor makroları + `__has_include` + engine-etiketli CI) en düşük maliyetli yol.
5. **Katalog sayısıyla yarışmak anlamsız.** UE-MCP 1510 kendi action'ı + Epic registry'yi sarıyor. Bizim farkımız test ve QA döngüsü (§7). 1.0 bu farkı derinleştirip animasyon açığını kapatmalı.

## Kaynak dosya yolları (lokal klonlar)

- `C:/Users/shanyup/AppData/Local/hermes/cache/scratch/ew2/chir` (EngineWire dev `2792ba1`)
- `…/ew2/uemcp` (UE-MCP `68a1602`)
- `…/ew2/ivan` (Unreal-MCP `7094df5`)
- `…/ew2/caps_dev.json` (EngineWire'ın 489 action'lık ham envanteri)
- `C:/Users/shanyup/Desktop/UE/_kaldirilan/EngineWire` (eski lokal EngineWire kopyası)
