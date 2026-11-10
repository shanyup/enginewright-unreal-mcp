# 03 — Küçük / Uzmanlaşmış Açık Kaynak Unreal MCP Projeleri (derin, kaynak doğrulamalı)

**Kontrol tarihi:** 10.11.2026 · **Yöntem:** her repo `git clone --depth 1` ile indirildi (scratch: `%LOCALAPPDATA%/hermes/cache/scratch/ue03/`), tool kayıtları kodda sayıldı (`@mcp.tool`, `CommandType == TEXT(...)`, `RegisterAction(...)`, `UFUNCTION`), alan kapsamı UE API sınıf adlarıyla grep edildi, istatistikler GitHub REST API'den (`api.github.com/repos/...`) alındı.
**Etiketler:** **doğrulandı** = koddan/manifestten okundu · **iddia** = sadece README/pazarlama metni, kodda teyit edilmedi/edilemedi · **bulunamadı** = kodda iz yok.
**Önemli düzeltme:** `docs/arastirma/rakip_analizi_2026-11.md` içindeki bu projelere dair birçok bilgi yanlış (aşağıda "Eski analizdeki hatalar").

Enginewright referans durumu (yerel, doğrulandı): `Plugins/Enginewright/Source/Enginewright/Private/Tools/*.cpp` içinde **124 `Register/RegisterAsync` tool**, v0.4.0 (`Enginewright.uplugin`), commit `192322b` (2026-11-10). `FScopedTransaction` 9 dosyada; `ENGINE_MINOR_VERSION`/`UE_VERSION_*` kullanımı **yok**.

---

## 0. Kapsam ve keşif

GitHub arama (`unreal mcp`, `unreal-mcp`, `ue5 mcp`, `unreal engine mcp`, `UnrealMCP`, sort=stars) ile ≥100 yıldızlı ve listede olmayan UE-MCP repoları bulundu. Büyük/ticari rakipler (Epic built-in, ChiR24/Unreal_mcp, db-lyon/ue-mcp, IvanMurzak) bu dosyanın kapsamı dışında (diğer derin raporlar).

| Repo | ★ | Fork | Son push | Açık issue | Lisans (API) | Son commit (clone) |
|---|---|---|---|---|---|---|
| chongdashu/unreal-mcp | 2113 | 349 | 2025-04-22 | 41 | None (README "MIT", LICENSE dosyası yok) | 4e5f00d 2025-04-22 |
| flopperam/unreal-engine-mcp | 1117 | 208 | 2026-06-26 | 6 | None (LICENSE dosyası yok) | b1b495b 2026-06-26 |
| Natfii/UnrealClaude ➕ | 1021 | 148 | 2026-06-26 | 11 | API None; `UnrealClaude/LICENSE` var, README MIT | 1f97837 2026-06-26 |
| kevinpbuckley/VibeUE ➕ | 825 | 179 | 2026-11-10 | 2 | MIT | 1fa6700 2026-11-08 |
| prajwalshettydev/UnrealGenAISupport | 685 | 111 | 2026-04-28 | 11 | MIT | 6514568 2026-04-28 |
| kvick-games/UnrealMCP | 615 | 81 | 2025-06-22 | 15 | None (MIT metni README'de) | f989d0e 2025-06-21 |
| VedantRGosavi/UE5-MCP ➕ | 436 | 59 | 2025-06-02 | 8 | NOASSERTION | c284cec 2025-06-01 |
| tumourlove/monolith ➕ | 370 | 95 | 2026-10-18 | 14 | MIT | 4548266 2026-10-18 |
| winyunq/UnrealMotionGraphicsMCP ➕ | 223 | 42 | 2026-08-02 | 5 | MIT | a0494ef 2026-08-02 |
| per-simmons/unreal-agent-harness ➕ | 210 | 30 | 2026-06-24 | 0 | None | bf8dc7a 2026-06-23 |
| ayeletstudioindia/unreal-analyzer-mcp ➕ | 161 | 33 | 2025-08-06 | 9 | MIT | 985fc06 2025-08-06 |
| GenOrca/unreal-mcp ➕ | 147 | 18 | 2026-07-07 | 10 | Apache-2.0 | f7986db 2026-07-07 |
| believer-oss/Claireon ➕ | 146 | 18 | 2026-09-15 | 0 | MIT | 89bee2f 2026-09-14 |
| Italink/UnrealClientProtocol ➕ | 130 | 15 | 2026-08-28 | 2 | MIT | 7f0a56e 2026-08-29 |
| runreal/unreal-mcp | 121 | 28 | 2025-06-06 | 5 | MIT | 90fb017 2025-06-06 |
| voodoofox/unreal-mcp | **9** | 3 | 2026-06-22 | 0 | MIT | e6f2ffa 2026-06-22 |

➕ = brief'teki listede yoktu, keşfedildi. Kapsam dışı bırakılan ≥100★ sonuçlar: `scenario-labs/skills` (genel skill paketi), `genex-games/genex-desktop` (çok motorlu masaüstü app, UE-MCP değil), `islandmed/SculptForge` (sculpt app), `HurtzDonutStudios/ai-forge-mcp` (101★, LICENSE yok, Blender/Substance pipeline; incelenmedi).
Kaynak: https://api.github.com/search/repositories?q=unreal+mcp&sort=stars

---

## 1. voodoofox/unreal-mcp

- **Kaynak:** https://github.com/voodoofox/unreal-mcp (commit e6f2ffa). Release/tag yok. 9★ — **topluluk benimsemesi neredeyse yok** (eski analiz bunu "aktif, en derin" diye sundu).
- **Mimari (doğrulandı):** chongdashu fork'u. Python FastMCP (stdio) → TCP 55557 newline-JSON → C++ `UnrealMCPBridge` → `AsyncTask(GameThread)` (`UnrealMCPBridge.cpp:213`). README: "Based on chongdashu/unreal-mcp".
- **Lisans:** MIT (LICENSE var). **UE:** README "5.5+ (tested on 5.7)"; `.uplugin` Win64/Mac/Linux; dependency `Niagara`, `NiagaraEditor`, `MetasoundEngine`, `AIModule`.
- **Boyut (doğrulandı):** C++ ~14.6K satır; `UnrealMCPNiagaraCommands.cpp` 5695 satır + `NiagaraViewModelCommands.cpp` 854 satır.
- **Tool sayısı:** README rozeti "150+ tools", repo açıklaması "88 tools". **Gerçek (doğrulandı):** Python'da **100 MCP tool** (`@async_tool`), C++ bridge'de **149 komut** (`CommandType == TEXT`), bunların **73'ü Niagara**.

### "73 Niagara komutu" — kodda doğrulama
- 73 C++ komutu **gerçek ve yazma yapıyor** (doğrulandı): `FNiagaraStackGraphUtilities::AddScriptModuleToStack` (`UnrealMCPNiagaraCommands.cpp:1486`), `GetOrCreateStackFunctionInputOverridePin` (:2561), `SetModuleIsEnabled` (:1657), `FGraphNodeCreator<UNiagaraNodeCustomHlsl>` ile Custom HLSL modül oluşturma (:4126), ViewModel tabanlı `niagara_vm_set_input / set_dynamic_input / set_module_enabled / delete_module`, simulation stage, event handler, curve keypoint, DI property, scalability override, stateless emitter, parameter collection, scratch pad, `export/import_niagara_system_spec`, `batch_niagara_commands`.
- **AMA MCP yüzeyine sadece 2 tool çıkıyor** (doğrulandı): `get_niagara_deep_inspect` ve `import_niagara_system_spec` (`Python/tools/inspection_tools.py:835,863`). `Python/tools/` altında `niagara_tools.py` **yok**. Kendi dokümanı: "these are individual C++ command handlers, not 1:1 MCP tools … send any command name + params as JSON over TCP to 127.0.0.1:55557" (`Docs/Tools/niagara_tools.md`). Yani bir MCP client 71 komuta ancak spec-batch üzerinden veya ham TCP ile ulaşır.
- `create_niagara_system`, `add_niagara_emitter` (39 engine template — iddia, doc'ta), `add_niagara_module`, `add_niagara_renderer` var → **Niagara authoring derinliği gerçek: yazma + HLSL + ViewModel**; MCP ergonomisi zayıf.

### Alanlar (tool listesi gruplu, doğrulandı)
- **Actor/Editor (17):** get_actors_in_level, find_actors_by_name, spawn_actor, delete_actor, set_actor_transform, get/set_actor_property, focus_viewport, spawn_blueprint_actor, run_console_command, save/duplicate/rename/delete_asset, **run_python_in_unreal**, get_plugin_info.
- **Blueprint (8) + Graph (18):** create_blueprint, add_component, set_*_properties, compile; event/function/input-action/variable get-set/branch/cast/select node ekleme, connect/disconnect, delete node, pin default.
- **Material/Inspection (~24):** get_material_graph, add/delete/connect expression, connect/disconnect material property, set expression property, add_custom_node_input, compile_material, set_material_instance_parent, list_nanite_status, set_static_mesh_nanite_enabled, list_assets, get_asset_details …
- **Animation (7):** list_animation_assets, get_animation_info, get_skeleton_info, get_blend_space_info, get_animation_notifies, get_actor_animation_state, set_actor_animation → **okuma ağırlıklı**; Montage/BlendSpace/notify **oluşturma yok**, AnimBP/state machine **yok**.
- **Audio (6):** inceleme + play_sound_at_location. **DataTable (4):** okuma/JSON export. **Level (6):** okuma + open_level. **AI (6):** list/get BT, blackboard, MetaSound graph reader, Niagara data channels → **sadece okuma**.
- **UMG (7):** create widget BP, text/button ekle, bind event, add to viewport, set root.
- **Project (1):** create_input_mapping (legacy input).
- **Bulunamadı:** skeletal import, socket, Physics Asset, IK retarget, Mutable, Sequencer/MRQ, PCG, Landscape/Foliage/Water, StateTree, GAS, Control Rig, MetaHuman, CommonUI, PIE, packaging, profiling, headless, screenshot (C++'ta chongdashu'dan kalma `take_screenshot` handler'ı var — `UnrealMCPEditorCommands.cpp:76` — ama MCP tool olarak expose edilmemiş), localization, source control, validation.
- **Safety/undo:** `FScopedTransaction` **0** (doğrulandı). **Multi-version:** `ENGINE_*_VERSION` kullanımı **0**.
- **Benzersiz:** Niagara system spec export/import + Custom HLSL modül + ViewModel input; paralel stdio/TCP testleri (`Tests/test_mcp_parallel_stdio.py`, `test_parallel_tcp.py`).

---

## 2. chongdashu/unreal-mcp

- **Kaynak:** https://github.com/chongdashu/unreal-mcp (4e5f00d, 2025-04-22). En yüksek yıldızlı (2113) ama **~19 aydır commit yok**, 41 açık issue. README "Status: Experimental". Bu ekosistemin "atası": voodoofox, flopperam-local doğrudan fork/türev.
- **Mimari:** Python FastMCP → TCP 55557 → C++ bridge (doğrulandı). UE 5.5+ (README), `.uplugin` Win64/Mac/Linux. Lisans: README MIT rozeti, **LICENSE dosyası yok** (API: None).
- **Tool (doğrulandı):** **32 MCP tool**, C++'ta 36 komut (fazladan: `ping`, `create_actor`, `add_blueprint_get_component_node`, `take_screenshot` — **MCP'ye expose edilmemiş**).
  - Actor (8): get_actors_in_level, find_actors_by_name, spawn_actor, delete_actor, set_actor_transform, get/set_actor_property, focus_viewport, spawn_blueprint_actor
  - Blueprint (8): create_blueprint, add_component_to_blueprint, set_static_mesh/component/physics/blueprint/pawn_properties, compile_blueprint
  - Graph (7): event/input action/function node, connect, variable, self component ref, self ref, find nodes
  - Input (1): create_input_mapping · UMG (6): widget BP, text block, button, bind event, add to viewport, text binding
- **Alan derinliği:** Blueprint yazma temel; geri kalan tüm zorunlu alanlar **bulunamadı** (Python exec bile yok). Undo 0, multi-version 0.
- **Karar:** Referans/mimari tarihi; **ölü**, takip etmeye değmez.

## 3. runreal/unreal-mcp

- **Kaynak:** https://github.com/runreal/unreal-mcp (90fb017, 2025-06-06); npm `@runreal/unreal-mcp` latest **0.1.4** (npm modified 2025-06-04). Son ~17 ay sessiz.
- **Mimari (doğrulandı):** **Eklentisiz**. Node.js MCP server + `unreal-remote-execution` paketi; UE'nin yerleşik Python Remote Execution protokolü (multicast `239.0.0.1:6766`, `server/index.ts:16`). Her tool = `server/editor/scripts/*.py` dosyası editöre gönderilir. UE 5.4+ (README). MIT.
- **Tool (doğrulandı, 20):** set/get_unreal_engine_path, set/get_unreal_project_path, **editor_run_python**, editor_list_assets, editor_export_asset (text), editor_get_asset_info (LOD dahil), editor_get_asset_references, editor_console_command, editor_project_info, editor_get_map_info, editor_search_assets, editor_get_world_outliner, editor_validate_assets, editor_create/update/delete_object, editor_take_screenshot (`AutomationLibrary.take_high_res_screenshot(640,520)`), editor_move_camera.
- **Derinlik:** `editor_validate_assets` gerçek DataValidation değil — ilk 100 asset'i yükleyip kontrol eden basit script (`ue_validate_assets.py`). Diğer zorunlu alanlar **bulunamadı** (Python exec ile teorik olarak her şey).
- **Benzersiz:** plugin kurulumu gerektirmeyen tek yaklaşım. **Karar:** stagnant.

## 4. kvick-games/UnrealMCP

- **Kaynak:** https://github.com/kvick-games/UnrealMCP (f989d0e, 2025-06-21). README: "# VERY WIP REPO", "Unreal Engine 5.5 (I have only tested on this version)". `.uplugin` VersionName 0.1.0. LICENSE dosyası yok (MIT metni README'de). Repoda `__pycache__/*.pyc` commit'lenmiş (hijyen sinyali).
- **Mimari:** C++ TCP server (`MCPTCPServer.cpp`) + handler sınıfları (`MakeShared<FMCP*Handler>`) + Python MCP bridge; editör toolbar'da start/stop UI (iddia, README).
- **Tool (doğrulandı):** MCP'de **8**: get_scene_info, create_object, modify_object, delete_object, create_material, modify_material, get_material_info, execute_python. C++'ta ayrıca CreateBlueprint/ModifyBlueprint/GetBlueprintInfo/CreateBlueprintEvent handler'ları var ama MCP'ye bağlı değil; README roadmap'te "[ ] Blueprints" işaretsiz.
- **Karar:** **ölü**. Görmezden gelinmeli.

## 5. flopperam/unreal-engine-mcp (local) + Hosted Flop MCP

- **Kaynak:** https://github.com/flopperam/unreal-engine-mcp (b1b495b, 2026-06-26). Repo açıklaması: "This MCP is now owned by Aura!". README başlığı: "Flopperam has been acquired by Aura". LICENSE dosyası yok.
- **Local (doğrulandı):** chongdashu türevi Python FastMCP + C++ `UnrealMCP` plugin (iki kopya: `UnrealMCP/` ve `FlopperamUnrealMCP/Plugins/UnrealMCP/`), Win64/Mac/Linux, UE 5.5+. **43 MCP tool** (`Python/unreal_mcp_server_advanced.py`):
  - Actor (4), Blueprint (5: create, add_component, set_static_mesh/physics, compile), BP analiz (4: read_blueprint_content, analyze_blueprint_graph, get_*_details), BP graph (11: add_node, connect_nodes, create_variable, set_blueprint_variable_properties, add_event_node, delete_node, set_node_property, create/delete/rename_function, add_function_input/output),
  - **Prosedürel "world building" (13):** create_pyramid/wall/tower/staircase/arch/maze/town/castle_fortress/suspension_bridge/aqueduct, construct_house/mansion, spawn_physics_blueprint_actor — primitive actor yığınları; gerçek PCG/Landscape değil.
  - Material (4): get_available_materials, apply_material_to_actor/blueprint, set_mesh_material_color, get_actor_material_info.
  - Python exec **yok**; undo 0; multi-version 0.
- **Hosted Flop MCP (iddia — kapalı kaynak, kod yok):** `agent.flopperam.com/mcp` (Streamable HTTP + API key) → WebSocket → "FlopAI" plugin. README'ye göre 50+ tool: `bp_create/bp_class/bp_variable/bp_component/bp_graph/bp_nodes/bp_wire/bp_input/bp_commit/bp_author/bp_dry_run/bp_skills`, `bp_brief/bp_inspect/bp_export`, `scene_query/scene_brief/scene_compose/actor_inspect/level_inspect/search_assets/asset_references/project_context`, `material_inspect/edit`, `niagara_inspect/edit`, `niagara_script_edit`, `chaos_edit`, `animation_inspect/edit`, `animation_graph_edit`, `ik_rig_edit`, `ik_retarget`, `widget_inspect/edit` (MVVM dahil), `behavior_tree`, `gas_edit`, `tag_registry_edit`, `landscape_inspect/edit`, `foliage_inspect/edit`, `sequencer_edit`, `metasound_edit`, `sound_asset_edit`, `pcg_graph_edit`, `asset_factory`, `editor_actions` (undo/redo), `editor_log`, `performance_audit`, `window_capture`, `cpp_source` (+Live Coding), `pie_test_bp/pie_test_scene` ("30+ assertion types"), `python_execution`, `unreal_api` ("15,000+ API lookups"), `skills`. UE 5.5/5.6/5.7 (README). Hiçbiri doğrulanamaz.
- flopperam.com/docs (10.11.2026 erişildi) hâlâ "FlopAI" dokümanı sunuyor (Level Design, Blueprint, Materials, Animation "Montages, state machines, IK", Widget UI, BT, 3D Generation, C++ Source, "full undo support") — iddia. https://flopperam.com/docs
- tryaura.dev: "Unreal 5.4+ · Unity 5/6", "build a feature and then playtest itself", "recover from editor crashes" — iddia. https://www.tryaura.dev/
- **Karar:** local repo **stagnant/demo** (Haziran 2026'dan beri commit yok, Aura'ya devredildi). Hosted taraf = Aura ürünü (ticari raporda ele alınmalı).

## 6. prajwalshettydev/UnrealGenAISupport

- **Kaynak:** https://github.com/prajwalshettydev/UnrealGenAISupport (6514568, 2026-04-28). Son release `v0.2-alpha` (2025-02-15). MIT. README rozeti "UE 5.4 to 5.7+".
- **Mimari (doğrulandı):** İki kısım: (a) **GenerativeAISupport** C++ runtime modülü — OpenAI/Anthropic/DeepSeek/XAI API wrapper'ları (`Source/GenerativeAISupport/Public/Models/{OpenAI,Anthropic,DeepSeek,XAI}`); (b) MCP: `Content/Python/mcp_server.py` (FastMCP) → socket → editör içi `unreal_socket_server.py` (Python handler'lar: actor/basic/blueprint/python/ui). `socket_server_config.json`: `auto_start_socket_server: false`.
- **Tool (doğrulandı, 30):** how_to_use, handshake_test, **execute_python_script**, execute_unreal_command, spawn_object, edit_component_property, create_material, create_blueprint, **take_editor_screenshot**, add_component/variable/function_to_blueprint, add_node_to_blueprint, get_node_suggestions, delete_node, get_all_nodes_in_graph, connect_blueprint_nodes(+bulk), compile_blueprint, spawn_blueprint_actor, add_nodes_to_blueprint_bulk, add_component_with_events, get_blueprint_node_guid, get_all_scene_objects, create_project_folder, get_files_in_folder, create_game_mode, add_widget_to_user_widget, edit_widget_property, add_input_binding.
- **Eski analizdeki hata:** "3D gen (Meshy, Tripo, Hunyuan3D, Rodin)" bu repoda **yok** — kodda `meshy|tripo|hunyuan|rodin` geçen tek dosya README; bunlar **ücretli Fab "Pro Plugins"** (MuddyTerrain) reklamı. README: "MCP in this free plugin is not being actively developed"; "Adding nodes and connections 🛠️ (buggy, issues open)".
- Undo 0, multi-version 0. Zorunlu alanların neredeyse tamamı **bulunamadı**.
- **Karar:** MCP kısmı **stagnant** (yazar kendisi söylüyor). Değeri: runtime LLM API wrapper'ı (oyun içi NPC sohbet vb.) — Enginewright'ın alanı değil.

---

## 7. Keşfedilen ≥100★ ek projeler

### 7.1 tumourlove/monolith ➕ (en derin açık kaynak rakip bu grupta)
- **Kaynak:** https://github.com/tumourlove/monolith (4548266; release **v0.24.0**, 2026-10-18; CHANGELOG 0.22→0.24 Ağu–Eki 2026 → aylık sürüm). MIT. README: UE 5.7 & 5.8.
- **Mimari (doğrulandı):** Tek C++ plugin, 20 modül (`MonolithAI/Animation/Audio/Blueprint/ComboGraph/Config/Core/Editor/GAS/Index/LevelSequence/LogicDriver/Material/Mesh/Niagara/ReflectionIntel/Source/UI/...`), MCP HTTP port 9316 (README), **namespace-dispatch**: her domain tek `{ns}_query(action, params)` tool + `monolith_discover()`. Offline `monolith_query.exe` (engine source index ~967K sembol, SQLite FTS5 asset index — iddia/README). Python proxy (`MCP/pyproject.toml`).
- **Action sayısı (doğrulandı):** `RegisterAction(TEXT(ns),TEXT(action))` ile **1457 benzersiz action** + `MonolithUI`'de ayrı kayıtlı ~167 `ui` action. Dağılım: mesh 241, ai 223, animation 201, blueprint 139, gas 135, niagara 129, audio 98, logicdriver 66, material 64, editor 49, source 20, chooser 16, input 15, combograph 13, level_sequence 8, config 7, risk/cppreflect 6, decision 5, network/localization 4 …
- **Zorunlu alan derinliği (doğrulandı, action adlarından):**
  - Skeletal import: `mesh.import_mesh` (`import_as_skeletal`, `import_animations`, `UFbxImportUI`) — **hedef skeleton parametresi yok** (opsiyonlar: replace_existing, combine_meshes, generate_lightmap_uvs, auto_generate_collision, normal_import_method, material_import).
  - Socket: `animation.add_socket/remove_socket/set_socket_transform/get_skeleton_sockets` ✔ yazma.
  - Physics Asset: `animation.get_physics_asset_info`, `mesh.configure_physics_props` → **okuma ağırlıklı**.
  - IK retarget: `create_ik_rig`, `create_ik_retargeter`, `add/remove_retarget_chain`, `set_retarget_chain_mapping`, `align_retarget_pose`, `batch_retarget_animations` ✔ tam yazma.
  - AnimBP SM: `create_state_machine`, `build_state_machine`, `add_state_to_machine`, `add_transition`, `set_transition_rule`, `set_state_animation`, `set_anim_entry_state` ✔.
  - Montage/BlendSpace/Notify: `create_montage(_from_sections)`, `add_montage_section/slot/anim_segment`, `create_blend_space(_1d)`, `add_blendspace_sample`, `add_notify(_state)`, `bulk_add_notify` ✔.
  - Control Rig: `add_control_rig_node/element`, `connect_control_rig_pins` ✔ (5.8 guard: `MonolithControlRigWriteActions.cpp:15`).
  - Niagara: 129 action (`create_system`, `add_emitter/module/renderer`, `add_dynamic_input`, `create_module_from_hlsl`, `create_stateless_emitter`, `batch_execute` …) ✔.
  - Material: 64 (graph build/export/import, custom HLSL, `begin/end_transaction`, `render_preview`) ✔.
  - BT/StateTree/EQS/SmartObject/Perception/Nav: `build_behavior_tree_from_spec`, `build_state_tree_from_spec`, `add_st_state/task/transition` ✔.
  - GAS 135 ✔. UMG+**CommonUI** ✔ (`create_activatable_widget/stack`, `create_common_button_style`, `bind_common_action_widget`, `audit_commonui_widget`).
  - Sequencer: level_sequence 8 action — **sadece okuma** (list_bindings, director info). MRQ **bulunamadı**.
  - PIE: `editor.start_pie/stop_pie`, `run_pie_smoke`, `pie_call_function`, `pie_inject_input_action`, `pie_get_object_properties` ✔. **Multiplayer PIE bulunamadı** (`SetPlayNumberOfClients` yok).
  - Profiling: `editor.get_stat_group_values` (iddia: time-series PIE sampling). Screenshot: `capture_scene_preview/anim_frames/material_grid/pie_movement_clip/sequence_frames/system_gif`, `stitch_flipbook` ✔ (görsel QA güçlü).
  - Localization: 4 action, **okuma** (get/list string tables, list_cultures, validate_string_table). GatherText yok.
  - Build: `trigger_build`, `get_build_errors`, `live_compile`, `run_automation_tests` ✔. Packaging/headless/Mutable/MetaHuman/Landscape/Foliage/Water/PCG **bulunamadı**.
  - Undo: `FScopedTransaction` 67 dosyada. Multi-version: `#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 8` (16 dosya, ör. `MonolithMirrorTableActions.cpp:153`).
- **Benzersiz:** namespace-dispatch (context tasarrufu), engine-source offline index, Reflection Intelligence (`cppreflect`, `network` replication audit, `risk`, `decision`), MCP `tools/list` annotations (readOnly/destructive), `did_you_mean`, Logic Driver & ComboGraph (3. parti) desteği.

### 7.2 kevinpbuckley/VibeUE ➕
- **Kaynak:** https://github.com/kevinpbuckley/VibeUE (1fa6700, 2026-11-08; en aktif). MIT. `.uplugin` VersionName 5.0, **EngineVersion 5.8.0**, Win64/Mac/Linux. Bağımlılık: `ModelContextProtocol`, `ToolsetRegistry`, `PythonScriptPlugin`, `Fab`, `StateTree`, `MetaSound`, `MeshModelingToolset`.
- **Mimari (doğrulandı):** Epic 5.8 built-in MCP'nin **eklentisi** ("MCP Expansion"): servisler `UToolsetDefinition` olarak `ToolsetRegistry`'ye kaydolur, metotlar AICallable + BlueprintCallable; ~36 markdown skill `UAgentSkill` olarak (`Content/Skills/*`). README: "deliberately does not duplicate the engine's general tools … screenshots, logs, PIE".
- **Yüzey (doğrulandı):** `Public/PythonAPI` başlıklarında **1048 `UFUNCTION`**, ~60 servis sınıfı.
- **Alan derinliği (doğrulandı, UFUNCTION adları):**
  - **Mutable ✔ yazma** — `UCustomizableObjectService`: CreateCustomizableObject, GetGraph, DiscoverNodeTypes, AddNode, RemoveNode, ConnectPins, SetNodeProperty, SetParentObject, **CompileObject**. *Bu gruptaki tek Mutable authoring.*
  - AnimBP SM ✔ — `UAnimGraphService`: AddStateMachine, AddState, AddConduit, AddTransition, SetTransitionRuleFromBool/Comparison/Automatic, BuildStateMachine, ValidateStateMachine, AddBlendSpacePlayer, AddLayeredBlend, AddSlotNode, AddTwoBoneIKNode, AddModifyBoneNode.
  - Montage ✔ — `UAnimMontageService` (~60 fonk.: CreateMontageFromAnimation, AddSection, SetNextSection, slot/segment/notify/branching point/root motion).
  - Skeleton ✔ — `USkeletonService`: AddBone/RemoveBone/ReparentBone, compatible skeleton, retargeting mode, curve metadata, blend profile. Socket: kod izi zayıf (1 dosya); IK Retargeter **bulunamadı**.
  - Landscape (22 dosya, edit layer + spline testleri), Foliage, Landscape auto-material, RVT, Map blockout ✔; Water: 1 iz (bulunamadı sayılır).
  - Niagara (System/Emitter/ScratchPad servisleri) ✔; Material node ✔; BT/Blackboard/EQS/StateTree ✔ (StateTree binding testleri); GAS 2 dosya (sınırlı); MetaHuman 3 iz (derinlik bulunamadı).
  - **Profiling ✔ (flagship)** — `UPerformanceService`: FrameTiming, ForceHitch, StartTrace/StopTrace, Bookmark, RegionStart/End, Analyse, **StartStandalone**/StopStandalone, StartPIE/StopPIE, SetBackgroundThrottling; `PerformanceVerdict.h`.
  - PIE actor ✔ (`UPIEActorService`: SpawnActor, CallFunction, ConsoleCommand in PIE). Multiplayer PIE: sadece net-mode okuma (`UEngineSettingsService.cpp:70`) → bulunamadı.
  - **Undo ✔** — `UTransactionService`: Undo/Redo/CancelTransaction/GetHistory; README "checkpoints … roll back its own edits".
  - Fab servisi (asset arama/kurulum — iddia derinlik). Multi-version: `ENGINE_*_VERSION` 0 → sadece 5.8.
- **Benzersiz:** Mutable authoring, Epic ToolsetRegistry'ye entegre olma tekniği, performans triage verdict'i, otomasyon test kapsamı (`*Tests.cpp` çok sayıda).

### 7.3 believer-oss/Claireon ➕
- **Kaynak:** https://github.com/believer-oss/Claireon (89bee2f; CHANGELOG 2.2.0 = 2026-09-14). MIT. README "Unreal Engine 5.x (tested with 5.5+)". C++ ~251K satır, `Private/Tools` 776 dosya, 160 test dosyası.
- **Mimari (iddia+doğrulandı):** editör içi MCP server, yüzey bilerek **2 tool**: `tool_search` (SQLite FTS5 + gömülü embedding ile katalog arama) + `python_execute` (her katalog tool'u `claireon.<tool>(...)` olarak çağrılır). Opsiyonel proxy: editör ölümünü sınıflandırır (`editor_crashed` / `editor_closed` …, `Saved/Crashes` okuyarak), editörsüz `launch_editor`.
- **Kapsam (doğrulandı, dosya adlarından):** BlueprintGraph 59, WidgetBP 44, StateTree 40, CameraAsset 30, Niagara 28, PCGGraph 24, LevelSequence 23, Input 22, Material 21, MI 19, BT 19, EQS 15, MetaSound/SoundCue 14+14, Landscape 14 (+LandscapeSpline 12, `LandscapeImport`), Foliage 11, Chooser 14, AnimGraph (StateMachine dahil), Anim (Montage/BlendSpace/Notify/Curve/Create).
  - PIE: PIEStart (net mode Standalone/ListenServer/Client, `ClaireonTool_PIEStart.cpp:304-312`; **client sayısı ayarı yok**), PIEScreenshot, PIEWaitFor, PIESpawnEnemy, PIETestAbility, PIEGetDamageEvents, PIETraceStart/Stop.
  - **Profiling ✔:** TraceOpen/Close, TraceGetFrameStats/TopScopes/ScopeDetails/Threads (Insights trace analizi).
  - **Undo ✔:** TransactionBeginGroup/EndGroup/RollbackGroup/Undo/Redo/History.
  - GAS runtime: GasGrantAbility/ApplyEffect/SetAttribute; World Partition: WPGenerate; AssetCook, CommandletRun (headless kısmi), AssetValidate, Lint, BlueprintDiff, StateTreeDiff, SlateScreenshot, FlythroughStart.
  - Bulunamadı: Mutable, MetaHuman, Water, IK retarget, MRQ, packaging, localization, source control.
  - Multi-version: `UE_VERSION_OLDER_THAN(5,7,0)` (`ClaireonBlueprintNodeFactory.cpp:872`), 26 dosya.
- **Benzersiz:** 2-tool yüzey + embedding'li tool arama; editör çökme atfı (proxy); Insights trace sorgulama; transaction grupları + rollback.

### 7.4 Natfii/UnrealClaude ➕
- **Kaynak:** https://github.com/Natfii/UnrealClaude (1f97837; release v1.5.1, 2026-06-26). Lisans dosyası `UnrealClaude/LICENSE` (README MIT). `.uplugin` EngineVersion **5.7.0**, Win64/Linux/Mac.
- **Mimari:** Editörde Claude Code CLI chat paneli + C++ MCP server (README "20+ tools"). `Private/MCP/Tools`: AnimBlueprintModify, Asset, AssetDependencies/Referencers/Search, BlueprintModify/Query, CaptureViewport, Character, CharacterData, CleanupScripts, DeleteActors, EnhancedInput, ExecuteScript, GetLevelActors, GetOutputLog, GetScriptHistory, Material, MoveActor, OpenLevel, RunConsoleCommand, SetProperty, SpawnActor, Task* (async görev kuyruğu).
- **Derinlik:** AnimBP state machine yazma (12 dosya iz) — README: "state machines (Few bugs still, don't rely on fully)" (iddia). Montage/notify izleri var. Diğer zorunlu alanlar bulunamadı. Undo 0, multi-version 0 (sadece 5.7).
- **Karar:** 6 aydır commit yok (Haziran 2026) → yavaşlıyor.

### 7.5 GenOrca/unreal-mcp ➕
- **Kaynak:** https://github.com/GenOrca/unreal-mcp (f7986db; release v2.2.0, 2026-06-17). Apache-2.0. README UE 5.6+, `.uplugin` sadece Win64. Fab'da da var (iddia).
- **Mimari (doğrulandı):** Python MCP server (`mcp-server/src/unreal_mcp`) → editör içi Python (`UnrealMCPython` plugin, `PythonScriptPlugin`) + opsiyonel C++ helper (`MCPythonHelper_AnimGraph.cpp`). Namespace tool + `list_actions`.
- **Kapsam (README, kısmen doğrulandı):** "253 actions across 21 domains": actor 36, asset 21 (FBX/glTF import/export), material 20, blueprint 19, util 19 (PIE control, LiveCoding), animation 17 (**socket** C++), umg 15, **level_sequence 13 (yazma: camera cut, keyframe)**, behavior_tree 12, editor 12, static_mesh 12, **gas 11**, data_table 8, level 7, **control_rig 6**, layer 6, **retarget 6** (`ue_create_ik_rig`, `ue_create_retargeter`, `ue_auto_map_chains`, `ue_batch_retarget` — `retarget_actions.py:37-170`, doğrulandı), **anim_blueprint 4** (skeleton-bound AnimBP + locomotion state machine), game 3, texture 3, vision 3.
- Skeletal import hedef skeleton: `asset_actions.py`'da skeleton parametresi **bulunamadı**. Multi-version: `ENGINE_MINOR_VERSION >= 7` (`MCPythonHelper_AnimGraph.cpp:267`). Undo: 3 dosyada transaction.
- **Karar:** 4 aydır sessiz; orta.

### 7.6 Italink/UnrealClientProtocol ➕
- https://github.com/Italink/UnrealClientProtocol (7f0a56e, 2026-08-29). MIT. "UE 5.x". **Felsefe:** atomik reflection protokolü (TCP+JSON) — herhangi bir UFunction çağır, UPROPERTY oku/yaz, obje bul; domain bilgisi "Skills" ile. Editor modülünde Niagara (yazma: `FNiagaraStackGraphUtilities` izi 3 dosya), Material graph differ, Blueprint "NodeCode", PneumaView. Undo 8 dosya; multi-version `UE_VERSION_OLDER_THAN(5,5,0)` (`MaterialPropertyHandlers.cpp:20`). Spesifik tool envanteri yok (tasarım gereği).

### 7.7 winyunq/UnrealMotionGraphicsMCP ➕
- https://github.com/winyunq/UnrealMotionGraphicsMCP (a0494ef; release v1.0.12, 2026-08-02). MIT. `.uplugin` EngineVersion 5.8.0, Win64. **UMG odaklı**: widget tree JSON export/patch/apply, "BlueCode" (kod benzeri BP), UI material/HLSL, widget animation (Sequencer izleri `MovieScene` 3 dosya). Fab sürümü ayrı (ticari). Multi-version `UmgMcpJsonCompat.h:6` (5.8 JSON API). Uzman, dar.

### 7.8 Diğerleri (zayıf / ilgisiz)
- **VedantRGosavi/UE5-MCP** — **kod yok**, sadece 16 markdown; README: "unfortunately we didnt move forward with the project". **Ölü.** https://github.com/VedantRGosavi/UE5-MCP
- **ayeletstudioindia/unreal-analyzer-mcp** — editöre bağlanmaz; UE **C++ kaynak kodu analizi** MCP'si (TS, ~2.2K satır). Son commit 2025-08. Alan dışı.
- **per-simmons/unreal-agent-harness** — Epic 5.8 resmi MCP üzerine Claude harness'ı + Python remote exec scriptleri (`ue_remote/`, `ue_qa.py`), Cesium/PCG şehir demoları. Ürün değil, demo; 2026-06'dan beri commit yok.

---

## 8. Ölü / stagnant — görmezden gelinmeli

| Proje | Neden | Kanıt |
|---|---|---|
| chongdashu/unreal-mcp | son commit 2025-04-22, 41 açık issue, "Experimental" | API pushed_at |
| kvick-games/UnrealMCP | 2025-06, "VERY WIP", 8 tool | README:10 |
| runreal/unreal-mcp | 2025-06, npm 0.1.4 | npm registry |
| VedantRGosavi/UE5-MCP | kod yok, terk edildi | README |
| prajwalshettydev (MCP kısmı) | "MCP … not being actively developed" | README:122 |
| flopperam local | Aura'ya devredildi, 2026-06'dan beri yok | README başlık |
| ayeletstudioindia analyzer | 2025-08, editör entegrasyonu yok | — |
| voodoofox | teknik olarak derin Niagara ama 9★, 2026-06'dan beri commit yok, tek geliştirici | API |

**İzlenmesi gerekenler:** monolith (aylık release), VibeUE (haftalık commit, 5.8 Epic MCP üstü), Claireon (2.2.0, test yoğun), GenOrca (orta).

---

## 9. Eski analizdeki (rakip_analizi_2026-11.md) hatalar

1. voodoofox "Aktif, 150+ araç, Niagara 73 komut" → 9★; MCP'de 100 tool; 73 Niagara komutunun **yalnızca 2'si** MCP tool (doğrulandı).
2. UnrealGenAISupport "3D gen (Meshy, Tripo, Hunyuan3D, Rodin) entegre" → **ücretsiz repoda yok**, ücretli Fab eklentileri (doğrulandı: kodda hiç geçmiyor). "Özel (ticari modüller)" lisansı → repo MIT.
3. runreal "asset import" → import tool'u **yok**; sadece export (text) var.
4. kvick-games "Actor spawn/transform, Python bridge" doğru; ama Blueprint handler'ları C++'ta mevcut, MCP'ye bağlanmamış.
5. flopperam hosted tool listesi "doğrulanmış" gibi sunulmuş → kapalı kaynak, **iddia**.
6. chongdashu "LICENSE dosyası yok" doğru; "~14 ay stagnant" → 10.11.2026 itibarıyla ~19 ay.

---

## 10. Karşılaştırma tablosu

Derinlik: **W** = yazma/authoring, **R** = sadece okuma, **K** = kısmi, — = yok. (d)=doğrulandı (kod), (i)=iddia.

| Feature | Kimde (derinlik) | Enginewright? | Not |
|---|---|---|---|
| Skeletal FBX import + **target skeleton** | monolith K(d: skeletal import, skeleton param yok), GenOrca K(d: FBX import, skeleton param yok), flop-hosted (i) | K (`asset_import` meshType) | **Hiç kimsede hedef-skeleton parametresi doğrulanmadı** → fırsat |
| Sockets | monolith W(d), GenOrca W(d, C++), Claireon K(d) | — | |
| Physics Asset | monolith R(d), VibeUE K(d iz) | — | yazma kimsede doğrulanmadı |
| IK Rig / Retarget | monolith W(d), GenOrca W(d), flop-hosted (i) | — | batch retarget monolith+GenOrca |
| AnimBP state machine | VibeUE W(d), monolith W(d), Claireon W(d), GenOrca K(d locomotion), UnrealClaude K(d, "buggy") | — | |
| Montage/BlendSpace/Notify | monolith W(d), VibeUE W(d montage), Claireon W(d), voodoofox R(d), GenOrca R/K | — | |
| Mutable (Customizable Object) | **VibeUE W(d)** tek | — | |
| Niagara authoring | monolith W(d,129), voodoofox W(d,73 C++/2 MCP), VibeUE W(d), Claireon W(d), Italink K(d), flop-hosted (i) | — | |
| Materials | monolith W, voodoofox W, Claireon W, GenOrca W, VibeUE W, kvick K, chongdashu — | W (6 tool) | EW'de custom HLSL/graph export yok |
| Sequencer | GenOrca W(d,13), Claireon W(d), monolith R(d), winyunq K(widget anim), flop-hosted (i) | — | |
| MRQ | bulunamadı (hepsi) | — | |
| PCG | Claireon W(d,24), flop-hosted (i), per-simmons (Epic MCP üstü) | K (`scene_dress`, PCG değil) | |
| Landscape / Foliage | VibeUE W(d), Claireon W(d import/spline), flop-hosted (i) | — | |
| Water | bulunamadı | — | |
| BT / StateTree / EQS | monolith W, Claireon W, VibeUE W, GenOrca W(BT), voodoofox R | — | |
| GAS | monolith W(d,135), GenOrca W(d,11), Claireon K(runtime), VibeUE K | K (gameplay_tag_*) | |
| Control Rig | monolith W(d), GenOrca W(d,6) | — | |
| MetaHuman | bulunamadı (VibeUE'de 3 iz, derinlik yok) | — | |
| UMG | hepsinde W (temel→derin: monolith, Claireon, winyunq) | W (widget_* + render) | |
| CommonUI | monolith W(d) | K (ui_* runtime, input_audit) | |
| MVVM | flop-hosted (i), Claireon dep. | W (viewmodel_*) | |
| PIE | monolith W(smoke, input inject), Claireon W, VibeUE W, GenOrca K | W (pie_* + walk_to/travel) | |
| **Multiplayer PIE (N client)** | **bulunamadı** (Claireon sadece net mode) | **W (SetPlayNumberOfClients, pie_actors, pie_net_emulation)** | EW farkı |
| Packaging | bulunamadı (Claireon AssetCook, VibeUE StartStandalone kısmi) | K (game_run packaged exe çalıştırır) | |
| Profiling | VibeUE W(d, trace+verdict), Claireon W(d, Insights sorgu), monolith K(stat group), flop-hosted (i) | — | **EW açığı** |
| Headless | Claireon K(CommandletRun), monolith offline exe (sadece index) | K (game_run -game) | |
| Python exec | voodoofox, kvick, runreal, prajwal, monolith, Claireon, VibeUE, GenOrca | — | bilinçli tercih mi karar ver |
| Screenshot / visual QA | monolith W(gif/flipbook/grid), Claireon (PIE/Slate), GenOrca vision, runreal, prajwal, VibeUE | **W (screenshot_ab, image_stats, image_compose)** | A/B + SSIM sadece EW |
| Localization | monolith R(d, 4) | **W (loc_gather, stringtable_*, font_coverage)** | EW farkı |
| Source control | bulunamadı (prajwal 1 iz) | — | |
| Metadata | GenOrca K(asset metadata tags), Claireon (SetBlueprintMetadata) | — | |
| Validation | Claireon (AssetValidate, Lint), monolith (validate_material/string_table), runreal (sahte) | W (`validate`, id/key_audit) | |
| Safety / undo | Claireon W(transaction group+rollback), VibeUE W(TransactionService), monolith (67 dosya FScopedTransaction), diğerleri 0 | K (9 dosyada FScopedTransaction, undo tool yok) | |
| Multi-version | monolith/GenOrca/winyunq `ENGINE_MINOR_VERSION`, Claireon/Italink `UE_VERSION_OLDER_THAN` | — (makro yok) | |
| Context-ergonomi (az tool) | monolith namespace-dispatch, Claireon 2-tool+FTS/embedding, GenOrca namespace | `batch` | |
| Build / Live Coding / automation | monolith, Claireon, GenOrca(LiveCoding) | W (live_coding_compile, automation_run, build_status) | |

## 11. Bizde olup onlarda olmayan (bu gruptaki 16 repoya göre)

- **Multiplayer PIE N-client** (`PieTools.cpp:255` `SetPlayNumberOfClients`) + `pie_actors` replication kontrolü + `pie_net_emulation` profilleri — hiçbirinde doğrulanmadı (Claireon sadece net mode seçer).
- **`game_run`**: `-game`/packaged exe başlatıp verdict, culture, cvar, variant, `glyphMisses` toplama — eşdeğeri yok.
- **Lokalizasyon yazma zinciri**: `loc_gather` (GatherText), `stringtable_compare` (po/csv), `font_coverage`, `loc_culture` — monolith'te sadece okuma.
- **Görsel regresyon**: `screenshot_ab`, `image_stats`, `image_compose` — diğerleri sadece capture yapıyor.
- **UI runtime sürüş**: `ui_state/ui_click/ui_key/ui_open/ui_close/ui_input_type/ui_prompts/ui_input_audit` — PIE içi widget tıklama/klavye denetimi başkasında yok (monolith `pie_inject_input_action` en yakını).
- **`pie_walk_to` / `pie_travel`**, `world_trace`, `class_measure`/`screen_coverage`, `savegame_get/set`, `id_audit`/`key_audit`, `doctor`, `client_config` — benzeri bulunamadı.
- **MVVM yazma** (`viewmodel_*`) — açık kaynakta doğrulanmış tek (flop-hosted iddia).

**Not (1.0.0 için en kritik açıklar, bu gruba göre):** Animation authoring (SM/Montage/BlendSpace/notify/socket/retarget) — monolith, VibeUE, Claireon, GenOrca'da hepsi var; Niagara; profiling (VibeUE/Claireon); undo/transaction tool'u (Claireon/VibeUE); multi-version makro disiplini. Hedef-skeleton'lı skeletal import ve MRQ/Water/MetaHuman/source control **hiç kimsede doğrulanmadı** → farklılaşma fırsatı.
