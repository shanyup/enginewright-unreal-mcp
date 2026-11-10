# 01 — Epic Built-in Unreal MCP (UE 5.8) ve UE 5.9 MegaJam Preview: derin envanter

**Kontrol tarihi:** 10.11.2026 · **Hazırlayan:** alt-ajan (Epic grubu) · **Enginewright karşılaştırma tabanı:** v0.4.0 (`192322b`, `Plugins/Enginewright/Enginewright.uplugin` VersionName 0.4.0)

> Etiketler: **[doğrulandı-kaynak]** = motor kaynağı / bizim kaynak okundu · **[doğrulandı-doküman]** = resmi Epic dokümanı okundu (kaynak yok) · **[iddia]** = yalnız duyuru/pazarlama metni · **bulunamadı** = hiçbir kaynakta yok.
>
> Lisans notu: Ek A, Epic araç adlarını listeler. `docs/YAYIN_HAZIRLIK.md` K2 kararı (`epic_ue58_inventory.json` açık dala girmez) bu dosya için de geçerli olabilir; açık repoya koymadan önce karar verin.

## 0. Özet (karar için)

- UE 5.8'de Epic'in MCP'si **gerçek ve geniş**: motor kaynağından sayıldı, **58 toolset sınıfı / 878 araç** (+3 meta-araç). Eski envanter (870/55) doğru ama eksik; eski analizdeki "59 toolset" yanlış. [doğrulandı-kaynak]
- Mimari: C++ HTTP sunucu (`ModelContextProtocol`) + **Toolset Registry** (C++ ve **Python** toolset'ler). 30 toolset sınıfı Python, 28'i C++. Python toolset'ler için PythonScriptPlugin şart. [doğrulandı-kaynak]
- **5.8'de olmayanlar** (eski analizde bazıları "var" denmişti): Landscape/Foliage/Water, profiling, packaging/cook, multiplayer PIE, PIE oyun girdisi (input injection), AnimBP/state machine, Montage/BlendSpace/Notify, IK Retarget, Mutable, MRQ, lokalizasyon toplama (gather), genel Python çalıştırma, Enhanced Input. [doğrulandı-kaynak]
- **5.9 Preview (09.11.2026)**: sunucu varsayılan açık, araç erişim modları (Read/Write/Execute → MCP `readOnlyHint`), `execute_python`, cooked/Shipping build'de MCP (`AIRuntimeCallable`), Perf/Console/Input/NavMesh/SceneCapture runtime toolset'leri, Unreal Assistant paneline harici ajan (ACP), Unreal Canvas, Fab toolset, Lore entegrasyonu. 5.9 kaynağını yerelde okuyamadık (yalnız 5.8 kurulu); 5.9 maddeleri doküman/duyuru seviyesinde. [doğrulandı-doküman]/[iddia]
- Enginewright'ın Epic'e karşı net farkları: **multiplayer PIE + net emulation, PIE içi oyun girdisi/yürütme, `game_run` (-game/paketli exe), UI runtime testleri (COVERED), lokalizasyon gather + font coverage, audit araçları, savegame, widget_render, read-only mod, atomik batch**. Epic'in net farkları: **Sequencer/Control Rig (~330 araç), Niagara yazma, PCG yazma, Physics Asset, soket, Skeletal mesh import + hedef iskelet, MetaHuman, Slate Inspector, Plugin yönetimi, Semantic Search, Dataflow/Cloth, agent skill sistemi, Origin allow-list güvenliği**.
- **Güvenlik bulgusu (bizde):** Enginewright HTTP sunucusu `Origin` başlığını kontrol etmiyor (`EnginewrightModule.cpp`'de origin kontrolü yok; grep boş). Epic 5.8 DNS-rebinding'e karşı Origin allow-list uyguluyor. 1.0 öncesi kapatılmalı.

---

## 1. Epic Built-in Unreal MCP — UE 5.8

### 1.1 Sürüm, tarih, durum
| Alan | Değer | Kaynak |
|---|---|---|
| Motor sürümü (incelenen) | UE 5.8.4, CL 58902052, `++UE5+Release-5.8` | `C:/Program Files/Epic Games/UE_5.8/Engine/Build/Build.version` [doğrulandı-kaynak] |
| Plugin | "Unreal MCP" (`ModelContextProtocol`), Version 1 / "1.0", `IsExperimentalVersion: true`, `EnabledByDefault: false`, `NoRedist: true` | `Engine/Plugins/Experimental/ModelContextProtocol/ModelContextProtocol.uplugin` [doğrulandı-kaynak] |
| Toolset'ler | `AllToolsets` toplayıcı plugin (varsayılan kapalı) + 27 toolset plugin `Engine/Plugins/Experimental/Toolsets/` | `AllToolsets.uplugin` [doğrulandı-kaynak] |
| Doküman | "Unreal MCP in Unreal Editor" 5.8 sürümü, readiness `ue_experimental`, güncelleme 2026-11-07 | https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8 [doğrulandı-doküman] |
| MCPClientToolset | API dokümanında "Beta" | https://dev.epicgames.com/documentation/unreal-engine/API/PluginIndex/MCPClientToolset [doğrulandı-doküman] |

### 1.2 Mimari [doğrulandı-kaynak]
- **Taşıma:** UE `HTTPServer` üzerinde Streamable HTTP (POST) + SSE (GET) + DELETE oturum kapama; `Mcp-Session-Id` ve `Mcp-Protocol-Version` başlıkları zorunlu/denetlenir. `ModelContextProtocolServer.cpp` satır 23-36, 435-447, 579-601.
- **Protokol sürümleri:** `2025-11-25` (varsayılan), `2025-06-18`, `2024-11-05` müzakere edilir. `ModelContextProtocol/Public/ModelContextProtocol.h:19-27`.
- **Varsayılanlar:** port 8000, yol `/mcp`, `bAutoStartServer = false` (5.8'de kapalı), `bEnableToolSearch = true`. `ModelContextProtocolEngine/Public/ModelContextProtocolSettings.h`.
- **Tool search:** açıkken `tools/list` yalnız `list_toolsets`, `describe_toolset`, `call_tool` döndürür; araçlar talep üzerine keşfedilir (`ModelContextProtocolToolSearch.cpp`). Büyük katalog için bağlam tasarrufu tekniği.
- **Registry:** `ToolsetRegistry` plugin (bağımlılıklar: PythonScriptPlugin, EditorScriptingUtilities, **FileSandbox**). C++ toolset = `UToolsetDefinition` + `UFUNCTION(meta=(AICallable))`; Python toolset = `unreal.ToolsetDefinition` + `@toolset_registry.tool_call`. Doc yorumları şemaya yansır. `ToolsetRegistry/Public/ToolsetRegistry/ToolsetDefinition.h`.
- **Eski yol:** `UModelContextProtocolToolLibrary` (Blueprint ile araç yazma, "MCP Tool Library" asset) — **deprecated**, her çağrıyı `FScopedTransaction` ile sarar. `ModelContextProtocolToolLibrary.h/.cpp:251`.
- **Modüller:** `ModelContextProtocol` ve `...Engine` **Runtime** tipte; `...Editor` editor. Commandlet'te sunucu otomatik başlamaz; `-ModelContextProtocolStartServer` ile zorlanır, `-ModelContextProtocolPort=` ile port. `ModelContextProtocolSettings.cpp:20-58`.
- **Client config üretici:** Claude Code (`.mcp.json`), Cursor, VS Code, Gemini (`httpUrl`), Codex (TOML, yalnız ilk yazım). `ModelContextProtocolClientConfig.cpp:26-41`.
- **MCPClientToolset:** dış MCP sunucularını (SSE / Streamable HTTP; auth None/Bearer/OAuth2 PKCE) registry'ye toolset olarak bağlar. Yani Epic asistanı **Enginewright'ı da tüketebilir**. `MCPClientToolset/Public/MCPClientToolset/MCPClientToolset.h`, `MCPToolsetSettings.h`.
- **Analytics:** oturum başlangıç/bitiş ve her araç çağrısı (ad, süre, başarı) kaydı; `ModelContextProtocol.EnableAnalytics` varsayılan true. `ModelContextProtocolAnalytics.h:39-45`, `.cpp:15-17`.
- **AIAssistant (5.8):** editörde gömülü web paneli, `https://dev.epicgames.com/community/assistant/embedded` yükler; toolset'i `GetProjectContext`, `GetDockedContext`. `AIAssistant/Source/AIAssistant/Private/AIAssistantToolset.h`, `AIAssistantWebBrowser.cpp`.
- **Agent skill:** `UAgentSkillToolset` (ListSkills/GetSkills/CreateSkill/UpdateSkill); 5.8'de Python skill'ler: `editor_toolset/skills/{blueprint_basics, default_outdoor_lighting, material_basics, unreal_skill_best_practices}.py`; PCG için `PCGToolset/Content/Skills`.

### 1.3 Lisans / fiyat / platform
- Motorla birlikte gelir, ayrıca ücret yok; UE EULA'sına tabi. `NoRedist: true` (oyunla dağıtılamaz). [doğrulandı-kaynak]
- Platform: ToolsetRegistry/AIAssistant `TargetAllowList: Editor`. ModelContextProtocol modüllerinde platform kısıtı yok; doküman Windows/Mac/Linux ayrımı yapmıyor → editörün desteklediği platformlar. Platform test matrisi **bulunamadı**.
- Desteklenen UE sürümü: yalnız 5.8 (motor içi; geri port yok). 5.7 ve öncesi için **bulunamadı**.

### 1.4 Bakım sinyalleri
- GitHub yıldız/issue kavramı yok (motor içi). Epic issue tracker'da 5.9 için `UE-412135` (MCP açık ama toolset'ler kapalı). https://forums.unrealengine.com/t/unreal-engine-5-9-megajam-preview/2852840 (post #3) [doğrulandı-doküman]
- Son sürüm: 5.8.4 (yerel kurulum). 5.9 Preview 1: 09.11.2026 (forum post #1 tarih damgası). Doküman güncellemeleri 07-10.11.2026.

### 1.5 Toolset envanteri ve derinlik (UE 5.8, kaynak sayımı)
Sınıflama araç adına göre sezgiseldir: **R** okuma (get/list/find/is/…), **W** yazma (create/add/set/remove/…), **X** çalıştırma/editör eylemi (run/play/compile/select/import/…). Tam ad listesi Ek A.

| Toolset (plugin/sınıf) | Dil | Araç | R/W/X | Derinlik notu |
|---|---|---|---|---|
| AnimationAssistant/SequencerTools | Py | 140 | 55/69/16 | Level Sequence oluşturma, binding/track/section, kamera, spawnable, oynatma kontrolü — tam yazma |
| AnimationAssistant/SequencerControlRigTools | Py | 72 | 26/29/17 | CR anim katmanları, bake, space, key, FBX import/export rig'e |
| AnimationAssistant/ControlRigTools | Py | 44 | 21/22/1 | Control Rig asset oluşturma, hiyerarşi (bone/control/null), graph node/pin — yazma |
| EditorToolset/BlueprintTools | Py | 53 | 22/30/1 | Graph node/pin/variable/function/dispatcher, replication, reparent, **Graph DSL** (`write_graph_dsl`/`read_graph_dsl`) |
| NiagaraToolsets (5 sınıf) | C++ | 56 | 33/21/2 | System oluşturma (template'ten), emitter ekle/sil (template'ten), module ekle/sil/enable, renderer ekle/sil/set, user var, stack input set, dynamic input okuma, compile state, stack issue + otomatik fix, BP wrapper |
| PCGToolset (2 sınıf) | C++ | 31 | 10/18/3 | Graph oluşturma, native/subgraph node, pin bağlama, graph param, instance spawn/execute, **node data view**, spline çizdirme (kullanıcıya) |
| UMGToolSet/UUMGToolSet | C++ | 23 | 7/15/1 | WBP oluşturma, ekle/taşı/sil/sar/değiştir, named slot, UI component, event bind, compile |
| EditorToolset/MaterialTools | Py | 22 | 8/13/1 | Material/MaterialFunction/MPC oluşturma, expression ekle/bağla/çıkışa bağla, layout, param grupları, recompile |
| EditorToolset/MaterialInstanceTools | Py | 13 | 5/8/0 | MIC oluşturma, scalar/vector/texture/static switch get/set, parent değiştir, override |
| EditorToolset/SkeletalMeshTools | Py | 22 | 15/6/1 | `import_file(skeleton=…, import_animations, create_physics_asset)`, soket ekle/sil/yeniden adlandır/transform, physics asset ata, morph target adları |
| EditorToolset/StaticMeshTools | Py | 16 | 8/7/1 | Import, LOD üret/sil/eşik, convex collision üret/sil, Nanite aç/kapa |
| PhysicsToolsets/PhysicsAsset | C++ | 17 | 5/12/0 | Mesh'ten PA oluştur, body/shape (sphere/capsule/box), mod, kütle, constraint ekle/limit/sil |
| EditorToolset/SceneTools | Py | 20 | 7/12/1 | Level yükle, actor ekle/sil, outliner klasörleri, trace, merge actors, Level Instance |
| EditorToolset/ActorTools | Py | 17 | 9/8/0 | Component ekle/sil, tag, label, transform, look_at |
| EditorToolset/AssetTools | Py | 21 | 13/8/0 | Klasör, dup/move/delete, find, **metadata tag get/update**, referencer/dependency, `read_file`/`write_file` (yalnız /Game, plugin Content, Saved; uzantı beyaz listesi) |
| EditorToolset/EditorAppToolset | C++ | 21 | 12/3/6 | CaptureViewport/CaptureEditorImage/CaptureAssetImage, kamera, seçim, Content Browser, `StartPIE`/`StopPIE`/`IsPIERunning`, `SearchCVars`, ekran↔dünya koordinatı |
| EditorToolset/Logs | C++ | 4 | 3/1/0 | Log girdileri (kategori filtresi), verbosity |
| EditorToolset/ProgrammaticToolset | Py | 2 | — | `execute_tool_script`: sandbox'lı Python ile **yalnız toolset çağrılarını** toplu yürütme; editör transaction'ı içinde, başarısızsa **undo** (`programmatic.py:764-880`) |
| DataTable/CurveTable/StringTable/DataAsset/Texture/Object/Primitive | Py | 10/9/8/1/2/6/4 | — | Oluşturma, satır/anahtar CRUD, CSV/JSON import, reflection property get/set |
| SequencerAnimMixer | Py | 21 | 13/8/0 | Anim Mixer katman/geçiş/dekorasyon |
| Sequencer Outliner/Keyframing/CustomBinding/Condition/ImportExport | Py | 18/22/8/9/6 | — | Key ekleme, curve editor, spawnable/possessable dönüşümü, FBX/AnimSequence export-import, `link_anim_sequence` |
| DataflowAgent | C++ | 22 | 7/15/0 | Dataflow graph oluşturma/node/var, Dataflow uyumlu asset (Cloth, GeometryCollection, Flesh, Groom) template'ten |
| ChaosClothAsset | C++ | 6 | 2/4/0 | Clothing asset oluştur/section'a ata/dönüştür |
| MetaHumanGenerator | Py | 9 | 3/6/0 | MetaHuman oluştur, body shape, skin tone, göz rengi (begin/end edit) |
| GASToolsets (3 sınıf) | C++ | 14 | 10/3/1 | ASC inspector (attribute, aktif efekt, ability, tag — PIE/runtime okuma), AttributeSet keşfi, GameplayCue tag/notify oluşturma, cue'yu seçili aktörde çalıştır |
| GameplayTags | C++ | 6 | 3/3/0 | Add/Remove/**Rename (referans güncelleyerek)**, referencer |
| AIModule/BehaviorTree | Py | 7 | 7/0/0 | **Yalnız okuma** |
| StateTree | Py | 9 | 9/0/0 | **Yalnız okuma** (5.8) |
| Conversation | Py | 7 | 7/0/0 | Yalnız okuma |
| WorldConditions | C++ | 2 | 2/0/0 | Açıklama okuma |
| MVVM | C++ | 9 | 4/5/0 | ViewModel oluştur/property, widget'a ekle, binding oluştur/sil |
| SlateInspector | C++ | 14 | 7/0/7 | Editör UI erişilebilirlik snapshot, ref ile Click/Hover/Type/PressKey/Drag/FillForm/SelectOption, Windows, Screenshot, Observe |
| Plugin | C++ | 17 | 11/6/0 | Plugin oluştur (template), enable/disable, descriptor/dependency düzenle |
| ConfigSettings | C++ | 8 | 5/3/0 | Settings container/section şema + set + save + reset |
| GameFeatures | C++ | 7 | 5/0/2 | GFP listele, aktive/deaktive |
| DataRegistry | C++ | 7 | 7/0/0 | Yalnız okuma |
| AutomationTest | C++ | 7 | 4/0/3 | Discover/List/Run/RunByFilter/Results/Status/Stop |
| LiveCoding | C++ | 1 | 0/0/1 | `CompileLiveCoding` (sonucu bekler) |
| SemanticSearch | C++ | 2 | 2/0/0 | İçerik embedding araması (SemanticSearch plugin indeksi gerekir) |
| AgentSkill / AIAssistant | C++ | 4 / 2 | — | Skill CRUD; proje/dock bağlamı |
| **Toplam** | | **878** | **415/382/81** | + 3 meta-araç |

### 1.6 Zorunlu alanlar — UE 5.8 (kaynak taraması)
Yöntem: `Engine/Plugins/Experimental/Toolsets/` altında anahtar kelime grep'i (`MoviePipeline`, `Customizable`, `WaterBody`, `IKRetarget`, `AnimMontage`, `BlendSpace`, `AnimNotify`, `AnimBlueprint`, `Localization`, `Cook`, `Insights` → **0 dosya**; `Landscape`/`Foliage` → yalnız `PCGToolsetLibraryCore.h`).

| Alan | Epic 5.8 | Kanıt |
|---|---|---|
| Skeletal mesh FBX import + hedef iskelet | **Var (yazma)**: `SkeletalMeshTools.import_file(folder_path, asset_name, source_file, skeleton, import_materials, import_textures, import_animations, create_physics_asset)`; `FbxImportUI` kullanır | `EditorToolset/Content/Python/editor_toolset/toolsets/skeletal_mesh.py:16-60` [doğrulandı-kaynak] |
| Sockets | **Var (CRUD)**: add/remove/rename/get/set_socket_transform | aynı dosya :314+ |
| Physics Asset | **Var (yazma, derin)**: CreateFromMesh, body/shape/constraint | `PhysicsToolsets/.../PhysicsAssetToolset.h` |
| IK Retarget | **Yok** | grep 0 |
| AnimBP / state machine | **Yok** (eski analizdeki "AnimBP okuma" yanlış) | grep 0 |
| Montage / BlendSpace / Notify | **Yok** (yalnız Sequencer'da `link_anim_sequence`, `export_anim_sequence`) | grep 0 |
| Mutable (Customizable Object) | **Yok** | grep 0 |
| Niagara authoring | **Var (yazma)**: system/emitter (template'ten), module (script asset'ten) ekle/sil/enable, SetParameters modülü, renderer, stack input, user var, issue-fix. **Yeni emitter/module script'i sıfırdan yazma, Custom HLSL: bulunamadı** | `NiagaraToolsets/Source/NiagaraToolsets/Private/NiagaraToolset.h` |
| Materials / MI | **Var (yazma)**: graph düzenleme + MIC + MaterialFunction + MPC | `material.py`, `material_instance.py` |
| Sequencer / MRQ | Sequencer **çok derin (yazma)**; MRQ/Movie Render Graph **yok** (5.9'da Canvas Bridge üzerinden) | grep `MoviePipeline` 0 |
| PCG | **Var (yazma)**: graph authoring + instance execute + data view | `PCGToolset.h` |
| Landscape / Foliage | **Yok** (yalnız PCG içi landscape örnekleme) | grep |
| Water | **Yok** | grep 0 |
| BT / StateTree | **Yalnız okuma** | `behavior_tree.py`, `state_tree.py` |
| GAS | Okuma + GameplayCue yazma; Ability/Effect/AttributeSet oluşturma **yok** | `GASToolsets/Private/*.h` |
| Control Rig | **Var (yazma, derin)**: asset + hiyerarşi + graph | `controlrig.py` |
| MetaHuman | **Var (yazma, dar)**: create + body/skin/eye | `metahuman.py` |
| UMG / CommonUI | UMG **yazma**; CommonUI'ye özel araç **yok** (UI component var) | `UMGToolSet.h` |
| PIE otomasyonu | `StartPIE` (Simulate, PlayMode, StartTransform, warmup) / `StopPIE`; out-of-process modlar in-viewport'a düşürülür. **Oyun girdisi (input injection) yok**; Slate Inspector editör widget'larına tıklar/tuşlar | `EditorAppToolset.h:21-60, 359-365` |
| Multiplayer PIE | **Yok** (NumPlayers/NetMode alanı yok) | grep `NetMode` 0 |
| Packaging / cook | **Yok** | grep 0 |
| Perf profiling | **Yok** (yalnız `SearchCVars`) | grep `Insights` 0 |
| Headless / commandlet | Sunucu commandlet'te `-ModelContextProtocolStartServer` ile açılabilir; `ModelContextProtocolToolHashMapping` commandlet'i yalnız araç adı hash haritası üretir | `ModelContextProtocolSettings.cpp:50-58`, `...ToolHashMappingCommandlet.h` |
| Python çalıştırma | **Genel Python yok**. `execute_tool_script` = kısıtlı import'lu sandbox, amacı "tool orchestration, not general Python execution" | `programmatic.py:880-884` |
| Screenshot / görsel QA | Viewport (gizmo gizleme), tüm editör penceresi, asset thumbnail, Slate widget screenshot. Görüntü karşılaştırma/istatistik **yok** | `EditorAppToolset.h:210-246`, `SlateInspectorToolset.h` |
| Lokalizasyon | StringTable CRUD + import. Gather/compile, kültür değiştirme, font coverage **yok** | `string_table.py` |
| Source control | Okuma seviyesi: `is_checked_out`, `can_edit_asset`, `can_edit`; `write_file` ve `UpdatePluginDescriptor` dosyayı check-out eder. Commit/submit/diff **yok** | `asset.py:526-545`, `PluginToolset.cpp` |
| Asset metadata | **Var**: `get_metadata_tags`, `update_metadata_tags`, `get_asset_tags` (registry) | `asset.py` |
| Validation | Genel Data Validation / Map Check aracı **yok**; parça parça: BP/WBP compile, Niagara `GetStackIssues`/`ApplyStackIssueFix`, PCG execute issue listesi, `bSave` save validator'ları (5.9 dokümanı) | ilgili header'lar |
| Güvenlik: undo/transaction | Merkezi değil. Transaction kullanan: ChaosCloth, Dataflow, Niagara_Component, PCG (C++) + `execute_tool_script` (rollback'li). Diğer ~50 toolset'te transaction yok. `ToolsetLibrary.h:64-68`: property yazma "NOT transactional", partial write olabilir | grep `FScopedTransaction` |
| Güvenlik: sandbox / ağ | Origin allow-list (localhost/127.0.0.1/[::1]) DNS-rebinding'e karşı; **kimlik doğrulama yok**; dosya erişimi kök beyaz listesi; FileSandbox altyapısı (`FGlobalSandbox` Enter/Persist/Discard) registry'de var ama MCP araçlarına bağlı kullanım **bulunamadı**; araç açıklamalarında "ONLY after explicit permission" metinleri (GameplayTags, AgentSkill, Cue) | `ModelContextProtocolServer.cpp:70-112`, `ToolsetRegistry/.../SandboxLibrary.h` |
| Read-only modu / erişim modu | 5.8'de **yok** (`AIAccessMode`/`readOnlyHint` grep 0) | grep |
| Eşzamanlılık | Araçlar game thread'de sırayla; SSE ile progress | doküman 5.8 |
| Çoklu sürüm tekniği | Uygulanmaz: her motor sürümü kendi toolset'lerini taşır. API farkına karşı: Python toolset'ler `unreal` reflection modülü üzerinden (derleme gerektirmez); MCP protokolünde 3 sürüm müzakeresi | `ModelContextProtocol.h:19-27` |

### 1.7 Benzersiz yönler (5.8)
- Tool search (3 meta-araç) ile 878 aracı bağlam patlamadan sunma.
- Blueprint **Graph DSL** (metinden graph yazma/okuma) — `blueprint_dsl.py` (≈96 fonksiyon).
- `execute_tool_script`: bir script'te çok araç + tek transaction + hata halinde undo.
- MCP **client** toolset: dış MCP sunucularını aynı registry'ye bağlama (OAuth2 PKCE dahil).
- Agent Skill sistemi (Python/C++ sınıf; 5.9'da SKILL.md).
- Slate Inspector: editör UI'sini Playwright benzeri ref'lerle sürme.
- Semantic Search (embedding) toolset'i.

---

## 2. UE 5.9 MegaJam Preview (Unreal Assistant, Canvas, Fab, Lore, yeni toolset'ler)

### 2.1 Sürüm, tarih, durum
- **5.9.0-preview-1**, 09.11.2026; "MegaJam Preview", "first Unreal Engine release built primarily with agentic workflows", ~1 haftalık geliştirme, kısa QA. https://forums.unrealengine.com/t/unreal-engine-5-9-megajam-preview/2852840 (post #1, raw: https://forums.unrealengine.com/raw/2852840/1) [doğrulandı-doküman]
- Release notes (son güncelleme 11/9/26): https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-9-release-notes [doğrulandı-doküman]
- 5.9 motor kaynağı **yerelde yok** (kurulu: 5.3, 5.4, 5.6, 5.8). GitHub tag `5.9.0-preview-1` Epic hesabı bağlantısı ister; okunmadı. Aşağıdakiler resmi doküman seviyesinde.

### 2.2 Unreal MCP değişiklikleri (5.8 → 5.9) [doğrulandı-doküman: https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9]
- **Auto Start varsayılan true**; plugin varsayılan açık. Ama toolset plugin'leri kapalı → `UE-412135` (ajan hiç araç görmüyor). Ayar yeri "Agentic Workflows > Model Context Protocol".
- Meta-araçlar 4'e çıktı: `list_toolsets`, `describe_toolsets` (opsiyonel `include_schemas`), `describe_tools`, `call_tool`.
- **Erişim modları:** `AIAccessMode = Read/Write/Execute` (Python: `mode=unreal.ToolAccessMode.*`), MCP `readOnlyHint`'e eşlenir; varsayılan Execute.
- **`AIRuntimeCallable`**: cooked/Shipping build'de MCP sunucusu; yalnız bu etiketli araçlar build-time manifest'e girer. Runtime araçlı engine toolset'leri: **NavMesh, Scene Capture, Console, Widget Finder, Perf (CSV + GPU profiler, Stats), Input**. Doküman uyarısı: "Cooked games aren't read-only".
- **PythonScriptingTools** (EditorToolset içinde): `execute_python` (Execute modu, async, script içinden `await call_tool(...)`), `search_symbols`, `get_symbol`, `grep_stubs`, `read_me`. https://dev.epicgames.com/documentation/unreal-engine/using-python-with-ai-agents-in-unreal-engine?application_version=5.9
- Yeni ayarlar: Additional Allowed Origins, Dump Media To Disk, kullanıcı ve proje düzeyi **Instructions** (DefaultGame.ini), en yüksek protokol sürümü; CVar'lar: `WrapPODToolResultsInObject`, `AudioResultOggFormat`, `ProgressIntervalSeconds`, `PaginationPageSize`, `EnableAnalytics`.
- `initialize` sonucu `_meta["com.epicgames.unrealengine/instance"]`: instance ID, PID, engine version, branch, CL, proje adı/klasörleri (çok-editör ayrımı).
- `ExitEditor` (EditorAppToolset) ile ajan editörü yeniden başlatabilir; ImportFile araçlarında `bReplaceExisting`, `bSave` (save validator'ları çalışır); Sound Wave importu.
- SKILL.md tabanlı skill'ler (`<Plugin>/Skills/`, `<Project>/Skills/`, `Engine/Extras/AgentTools/Skills/`).
- Yeni toolset adları (dokümanda geçen): BlueprintToolsets, ConsoleToolset, GeometryToolset (`Engine/Plugins/Experimental/Toolsets/GeometryToolset/...`), PythonScriptingTools; release notes: "Blueprints, editor actions, MVVM, Slate inspection, performance, files, scene capture, console, Texture Graph, widget animation and more". Araç bazında liste **bulunamadı** (kaynak gerekli).
- Script API genişlemesi: Blueprint, Chooser, **StateTree yazma** (task/consideration/evaluator/global task), Dataflow graph düzenleme Python/BP'den; PCG custom HLSL kaynağı okuma/yazma; GraphDsl framework. (Release notes) — MCP aracı olarak sunulup sunulmadığı **bulunamadı**.
- Sınırlar (5.9 dokümanı): yalnız Streamable HTTP + SSE (stdio/WebSocket yok), auth yok, Prompts yok, engine'de Resource provider yok.

### 2.3 Unreal Assistant (5.9) [doğrulandı-doküman: https://dev.epicgames.com/documentation/unreal-engine/run-external-ai-agents-in-the-unreal-assistant-panel-in-unreal-engine]
- Panel, Epic'in "Unreal Assistant service" yerine yerel ajanı **Agent Client Protocol (ACP)** üzerinden çalıştırır: Claude Code (`@agentclientprotocol/claude-agent-acp`), Codex (`codex-acp`), Cursor (`agent acp`), OpenCode (`opencode acp`), Custom komut. Editör ajanı kendi MCP sunucusuna otomatik bağlar.
- Komut onayı proje/kullanıcı/komut satırı başına (Windows registry `HKCU\Software\Epic Games\Unreal Engine\AIAssistant\ApprovedAgentCommands`).
- Release notes: tool-access ayarı **Ask / Read / Edit / Auto / Full**, bildirim tercihleri, custom command ile harici ajana devretme. [doğrulandı-doküman, ayrıntı bulunamadı]

### 2.4 Unreal Canvas (5.9) [doğrulandı-doküman: https://dev.epicgames.com/documentation/unreal-engine/unreal-canvas-overview-in-unreal-engine , https://dev.epicgames.com/documentation/unreal-engine/unreal-canvas-in-unreal-engine]
- Yerel sunucu + web istemci (editör sekmesi veya bağımsız); görsel/video/mesh üretimi (BYO API key), graph tabanlı tekrarlanabilir süreçler, ajan paneli.
- Plugin'ler: **Unreal Canvas Bridge** (Movie Render Graph render'ını MCP aracı olarak sunar; MCP plugin'ini gerektirir), Spatial Annotations, Canvas Graph.
- Editörden: viewport render + pass'ler (beauty/depth/normal), Level Sequence render, canlı viewport stream (renk/derinlik/normal/obje maskesi), seçili asset, 3D anotasyonlar. Canvas'tan: cutout actor, mesh/görsel/**Gaussian splat** import, level'a yerleştirme, rigged dosyayı skeletal mesh olarak import.
- Gereksinim: NVIDIA GPU ≥8 GB VRAM. Import kökü `/Game/Merlin` (iç kod adı). Quick Render PIE kullanır.

### 2.5 Fab toolset ve Lore [iddia]
- Fab toolset: "lets agents search the catalog, browse your library, and install free or purchased assets directly into your projects" — yalnız forum post #1. Araç listesi/doküman **bulunamadı**.
- Lore: "New integrations with Lore enable AI agents to manage project changes and automate source control workflows" — forum post #1. Lore doküman sayfası yalnız lore.org'a yönlendiriyor (https://dev.epicgames.com/documentation/unreal-engine/lore-version-control-in-unreal-engine , 2026-06-17). Lore MCP araçları **bulunamadı**.
- Diğer duyurular [iddia/doküman-özet]: Fab görsel/mesh üretimi, chat completion, ElevenLabs ses, Gemini, Claude Code entegrasyonları; "live AI-processed viewport preview"; Knowledge Base (canlı motor durumu veya disk markdown'ından); UAF AnimGen; IK Retargeter yeni op'lar (Proximity IK, Pelvis Volume, Limb Twist) — bunların MCP aracı olup olmadığı **bulunamadı**.
- Forum duyurusu (post #1): toolset'ler "rendering, source control, profiling, debugging, gameplay testing, geometry, audio, UI, and level creation" alanlarına genişledi; Python API: "audio, MetaSounds, modeling, physics, Niagara, materials, PCG, networking, and UI". MCP dokümanı MetaSounds araçlarından söz ediyor. Araç düzeyinde doğrulama **bulunamadı**.

### 2.6 5.9 için zorunlu alanlar (bildiğimiz kadarıyla)
| Alan | 5.9 durumu | Etiket |
|---|---|---|
| Perf profiling | Perf toolset (CSV, GPU profiler, Stats), runtime-callable | doğrulandı-doküman |
| Packaging/cook | bulunamadı | — |
| Multiplayer PIE | bulunamadı ("gameplay testing" iddiası var) | iddia |
| Python | `execute_python` | doğrulandı-doküman |
| Headless | Cooked/Shipping build'de MCP + commandlet bayrağı | doğrulandı-doküman |
| Source control | Lore entegrasyonu | iddia |
| StateTree yazma | Script API'de var; MCP aracı bulunamadı | doğrulandı-doküman (API) |
| MRQ | Canvas Bridge → Movie Render Graph MCP araçları | doğrulandı-doküman |
| Input | Input runtime toolset | doğrulandı-doküman (içerik bulunamadı) |
| Erişim modu / onay | AIAccessMode + Assistant Ask/Read/Edit/Auto/Full | doğrulandı-doküman |
| Diğer zorunlu alanlar (IK retarget, AnimBP, Montage, Mutable, Landscape, Water, lokalizasyon gather) | bulunamadı | — |

---

## 3. Eski analizdeki (`rakip_analizi_2026-11.md`) Epic hataları
| Eski iddia | Doğrusu | Kanıt |
|---|---|---|
| "870 araç, 59 toolset" | 55 sınıf/870 araç envanterde; kaynakta 58 sınıf/878 araç (+3 meta) | §1.5, Ek A |
| Epic'te Landscape/Water toolset var | 5.8'de yok | §1.6 grep |
| Epic profiling toolset (5.8) | 5.8'de yok; 5.9'da Perf | §1.6, §2.2 |
| Animation: "AnimBP okuma" | 5.8'de AnimBP aracı yok | §1.6 |
| Epic'te Python yok | 5.8: sandbox `execute_tool_script` (genel değil); 5.9: `execute_python` | §1.5, §2.2 |
| Epic'te batch/transaction yok | `execute_tool_script` transaction+undo yapar; 4 C++ toolset transaction kullanır | §1.6 |
| Epic'te validation yok | Genel validate yok; BP/WBP compile, Niagara issue+fix, PCG issue var | §1.6 |
| Epic 5.9 "Canvas: concept/3D/video gen", "Fab toolset" | Canvas doğrulandı-doküman; Fab toolset yalnız iddia | §2.4-2.5 |

---

## 4. Karşılaştırma tablosu: Epic (5.8 kaynak / 5.9 doküman) ↔ Enginewright v0.4.0
Enginewright sütunu `Plugins/Enginewright/Source/Enginewright/Private/Tools/*.cpp` içindeki `Register/RegisterAsync` çağrılarından (124 araç) doğrulandı.

| Özellik | Epic (derinlik) | Enginewright? | Not |
|---|---|---|---|
| Sunucu taşıma | HTTP POST + SSE + DELETE, 3 protokol sürümü, session | HTTP POST, `Mcp-Session-Id`; GET/DELETE 405 (`EnginewrightModule.cpp:238-244`) | SSE/progress yok; uzun çağrılar için `RegisterAsync` + `*_status` araçları |
| Origin / DNS-rebinding koruması | Var (allow-list) | **Yok** | 1.0 öncesi eklenmeli |
| Auth | Yok | Yok | eşit |
| Tool search / lazy şema | Var (3→4 meta-araç) | Kategori kapatma (`DisabledCategories`) | 124 araçla şimdilik gerek yok |
| Read-only / erişim modu | 5.8 yok; 5.9 AIAccessMode + readOnlyHint | **Var**: `bReadOnly` + `bWrites` + `HasSideEffects` (`EWRegistry.cpp:254`) | MCP `readOnlyHint`/`annotations` yayımlamıyoruz (Source'ta grep boş) → 1.0 işi |
| Undo/transaction | Dağınık (4 C++ toolset + script) | Her yazma transaction (`bTransactional`), `batch` atomik + rollback, undo edilemez adım reddi (`BatchTool.cpp:41-60`) | Bizde merkezi ve tutarlı |
| Toplu çağrı | `execute_tool_script` (Python sandbox) | `batch` (JSON adım listesi) | Epic'te mantık/döngü var, bizde yok |
| Blueprint graph | 53 araç + Graph DSL | 13 araç (create/info/compile/var/component/node/connect/pin) | Epic daha geniş; DSL yok |
| Material / MI | Graph + MF + MPC + MI tam | 6 araç (create/info/add_expression/connect/MI create/set) | MF/MPC/layout/disconnect yok |
| UMG | 23 araç (yazma) | 7 widget + `widget_render` (PNG) | Render bizde var, Epic'te yok (CaptureAssetImage thumbnail) |
| CommonUI | Özel araç yok | `ui_input_type`, `ui_prompts`, `ui_input_audit` | |
| MVVM | 9 | 5 (`viewmodel_*`) | |
| UI runtime test | Yok (Slate Inspector editör UI içindir) | `ui_state/click/key/open/close`, COVERED tespiti | Bizde benzersiz |
| Slate (editör UI) inspector | 14 araç | Yok | |
| Skeletal mesh import + hedef iskelet | Var (`skeleton`, anim, physics asset) | `asset_import` var, **hedef iskelet parametresi yok** | |
| Sockets | CRUD | Yok | |
| Physics Asset | 17 araç | Yok | |
| Static mesh LOD/collision/Nanite | Var | `mesh_info`, `mesh_set_material` | |
| IK Retarget | Yok (5.8) | Yok | ikimizde boş |
| AnimBP / state machine | Yok | Yok | ikimizde boş — fırsat |
| Montage / BlendSpace / Notify | Yok | Yok | ikimizde boş — fırsat |
| Mutable | Yok | Yok | ikimizde boş |
| Niagara | 56 araç, yazma (template tabanlı) | Yok | 1.0 hedefi |
| Sequencer | ~330 araç (Seq+CR+Mixer) | Yok | 1.0 hedefi; derinlikte yetişmek zor |
| MRQ | 5.8 yok; 5.9 Canvas Bridge | Yok | |
| PCG | 31 araç, yazma | `scene_dress` (prosedürel yerleştirme) | farklı yaklaşım |
| Landscape / Foliage / Water | Yok | Yok | ikimizde boş |
| BT / StateTree | Yalnız okuma | Yok | yazma = fark yaratır |
| GAS | Okuma + Cue | Yok | |
| Control Rig | 44 + 72 | Yok | |
| MetaHuman | 9 (dar) | Yok | |
| Dataflow / Chaos Cloth | 28 | Yok | |
| Gameplay Tags | 6 (rename+referans) | 3 (list/add/remove, ref kontrolü) | rename yok |
| DataTable / CurveTable / StringTable | 10 / 9 / 8 | DataTable 5, StringTable 4 | CurveTable yok |
| Enhanced Input | Yok (5.9: Input runtime) | 6 araç | |
| PIE başlat/durdur | `StartPIE` (Simulate, spawn transform, warmup) | `pie_start/stop/state` | |
| PIE oyun girdisi / yürütme | Yok | `pie_input`, `pie_walk_to`, `pie_travel` | bizde benzersiz |
| Multiplayer PIE / net emulation | Yok | `pie_actors`, `pie_net_emulation` | bizde benzersiz |
| -game / paketli build çalıştırma | Yok | `game_run`, `game_run_status` | bizde benzersiz |
| Packaging / cook | Yok | Yok (`game_run` yalnız çalıştırır) | ikimizde boş |
| Perf profiling | 5.8 yok; 5.9 Perf toolset | `class_measure`, `screen_coverage` (perf değil) | 5.9 bizi geçiyor |
| Headless / commandlet / cooked | Commandlet bayrağı; 5.9 cooked build MCP | Yok (`ewtool.py build` dışında) | 1.0 hedefi |
| Python | 5.8 sandbox script; 5.9 `execute_python` | Yok (bilinçli) | |
| Console komutu | 5.8 yok (yalnız SearchCVars); 5.9 Console toolset | `console_command`, `log_wait` | |
| Log | 4 araç | `log_read`, `log_wait` | |
| Screenshot / görsel QA | Viewport/editor/asset/Slate capture | `screenshot`, `screenshot_ab`, `editor_capture`, `image_compose`, `image_stats`, `widget_render`, `actor_preview` | karşılaştırma/istatistik bizde |
| Lokalizasyon | StringTable | `loc_gather(+status)`, `loc_culture`, `stringtable_*`, `stringtable_compare`, `font_coverage` | bizde çok daha derin |
| Source control | Durum okuma + check-out; 5.9 Lore [iddia] | Yok | |
| Asset metadata | get/update metadata tag | Yok | |
| Asset CRUD / referans | 21 araç + read/write_file | 7 araç + `asset_search_text` | |
| Validation | Parçalı | `validate` (BP compile, Data Validation, broken refs, Map Check), `id_audit`, `key_audit`, `doctor` | bizde merkezi |
| Automation test | 7 | `automation_list`, `automation_run` | |
| Live Coding / restart | `CompileLiveCoding`; 5.9 `ExitEditor` | `live_coding_compile`, `editor_restart` (build+yeniden aç), `build_status` | bizde derin |
| Plugin yönetimi | 17 | Yok | |
| Config/Settings | 8 (tüm settings container) | `settings_get/set` | |
| Game Features | 7 | Yok | |
| Semantic search / Knowledge Base | 2 araç; 5.9 KB | Yok | |
| Agent skills | Var (5.9 SKILL.md) | Yok (repo dışı skill) | |
| MCP client (dış sunucu proxy) | Var | Yok | Epic asistanı bizi tüketebilir → uyumluluk fırsatı |
| Client config üretimi | 5 istemci | `client_config` | |
| Savegame | Yok | `savegame_get/set` | |
| Type authoring (enum/struct) | Yok | `enum_create`, `struct_create`, `type_info` | |
| Object reflection get/set | 6 | `object_get/set` | Epic: partial write uyarısı; biz: strict |
| Çoklu sürüm | Motor başına tek sürüm | Yalnız 5.8 (`ENGINE_MINOR_VERSION` grep 0); 1.0 hedefi 5.4–5.8 (README:196) | |
| Analytics/telemetri | Varsayılan açık | Yok | |

## 5. Bizde olup onlarda olmayan (Epic 5.8 kaynağına göre; 5.9 dokümanında da karşılığı bulunamadı)
1. **Multiplayer PIE + ağ emülasyonu** (`pie_actors` replication bekleme, `pie_net_emulation` profilleri).
2. **PIE içinde oyun girdisi ve yürütme** (`pie_input`, `pie_walk_to`, `pie_travel`).
3. **Oyunu -game / paketli exe olarak çalıştırıp ekran görüntüsü + karar toplama** (`game_run`, kültür/cvar/varyant, glyphMisses).
4. **Runtime UI test katmanı** (`ui_state/click/key/open/close`, örtülü buton = `COVERED`, `ui_input_audit`, `ui_prompts`, `ui_input_type`).
5. **Görsel QA hesapları** (`screenshot_ab`, `image_stats`, `image_compose`) ve **PIE gerektirmeyen `widget_render`**, `actor_preview` (izole önizleme dünyası).
6. **Lokalizasyon hattı** (`loc_gather`, `loc_culture`, `stringtable_compare` po/csv, `font_coverage`).
7. **Merkezi doğrulama/denetim** (`validate`: BP + Data Validation + kırık referans + Map Check; `id_audit`, `key_audit`, `asset_search_text`, `doctor`).
8. **Tutarlı güvenlik modeli**: her yazma transaction, atomik `batch` + rollback, global read-only mod, kategori kapatma, strict şema (bilinmeyen anahtar = hata). Epic 5.8'de read-only yok, transaction dağınık, property yazma "partial".
9. **Enhanced Input authoring** (`input_action_create`, `input_context_create`, `input_map/unmap`, canlı `input_map_list`) — Epic 5.8'de yok.
10. **Savegame okuma/yazma**, **enum/struct oluşturma**, `scene_dress` (halka/grid/scatter yerleştirme), `class_measure`/`screen_coverage`.
11. **Editör build/restart döngüsü** (`editor_restart` build + yeniden açma, `build_status`).
12. **Python bağımlılığı yok** (Epic toolset'lerinin 30/58'i Python; PythonScriptPlugin şart).

## 6. 1.0 yol haritası için çıkarımlar (bu rapora dayalı)
- Kapatılması şart: **Origin allow-list** (Epic'in `ModelContextProtocolServer.cpp:70-112` mantığına eşdeğer); MCP `readOnlyHint`/`destructiveHint` annotation'ları (5.9 istemcileri bunu bekleyecek); uzun çağrılar için SSE progress (Epic Claude Code ~120 sn zaman aşımı notu).
- Epic'in **hiç yapmadığı** ve oyun geliştirici için kritik alanlar: AnimBP/state machine, Montage/BlendSpace/Notify, IK Retarget, BT/StateTree **yazma**, packaging/cook, Landscape/Foliage. Niagara/Sequencer'da Epic'le derinlik yarışı yerine bu boşluklar daha yüksek getirili.
- Uyumluluk fırsatı: Epic'in `MCPClientToolset`'i dış MCP sunucularını kendi asistanına bağlıyor (Streamable HTTP) → Enginewright'ın bu istemciyle test edilmesi, 5.9'da Unreal Assistant içinden kullanılabilirlik sağlar.
- 5.9 doğrulaması eksik: 5.9 Preview kurulup `Engine/Plugins/Experimental/Toolsets` için aynı çıkarım betiği çalıştırılmalı (betik: `docs/arastirma/derin/araclar/extract_epic_toolsets.py <Engine/Plugins/Experimental> <out.json>`).

## 7. Kaynaklar
- Motor kaynağı (UE 5.8.4): `C:/Program Files/Epic Games/UE_5.8/Engine/Plugins/Experimental/{ModelContextProtocol, ToolsetRegistry, AIAssistant, Toolsets/*}`
- https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8 (güncelleme 2026-11-07)
- https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9 (güncelleme 2026-11-10)
- https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-9-release-notes
- https://dev.epicgames.com/documentation/unreal-engine/using-python-with-ai-agents-in-unreal-engine?application_version=5.9
- https://dev.epicgames.com/documentation/unreal-engine/run-external-ai-agents-in-the-unreal-assistant-panel-in-unreal-engine
- https://dev.epicgames.com/documentation/unreal-engine/unreal-canvas-overview-in-unreal-engine , https://dev.epicgames.com/documentation/unreal-engine/unreal-canvas-in-unreal-engine
- https://dev.epicgames.com/documentation/unreal-engine/working-with-pcg-and-llms-using-unreal-mcp-in-unreal-engine
- https://dev.epicgames.com/documentation/unreal-engine/lore-version-control-in-unreal-engine
- https://dev.epicgames.com/documentation/unreal-engine/API/PluginIndex/MCPClientToolset
- https://forums.unrealengine.com/t/unreal-engine-5-9-megajam-preview/2852840 (post #1 duyuru, post #3 bilinen sorunlar)
- Not: dev.epicgames.com sayfaları tarayıcıda 403 / Cloudflare verdi; içerik `https://dev.epicgames.com/community/api/documentation/document.json?path=en-us/unreal-engine/<slug>&application_version=<v>` JSON uç noktasından okundu.
- Enginewright: `Plugins/Enginewright/Source/Enginewright/Private/{EWRegistry.cpp, EnginewrightModule.cpp, Tools/*.cpp}`, `Public/EnginewrightSettings.h`, `README.md`

## Ek A — UE 5.8 tam araç listesi
Yerel dosyada: `01_epic_EkA_araclar.local.md` (git dışı, K2 kararı). Yeniden üretmek için `araclar/extract_epic_toolsets.py`.
