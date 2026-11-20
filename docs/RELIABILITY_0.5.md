# Reliability round for 0.5.2

Before 0.5.2 every one of the 132 tools was called on a real game project in development (UE 5.8, C++ and Blueprints,
CommonUI, Enhanced Input, localization, co-op), with the editor open. This round was larger than the
[0.4.0 sweep](RELIABILITY_0.4.md): 996 checked calls instead of 330, and every tool got at least one real call and, where
wrong input exists, at least one wrong call.

| Kind of call | Count | What it checks |
|---|---|---|
| Real | 352 | The answer is right: checked against the asset, the file, the level or the screen, not only the reply |
| Wrong input | 473 | Missing assets, maps, folders, keys, rows, out-of-range numbers, bad types: a clear error code, a message that says how to fix it, and nothing changed |
| During Play In Editor | 115 | Reads work; writes are refused with `PIE_RUNNING` and leave nothing behind |
| Turkish text | 46 | Paths, asset names and values with `ç ğ ı ö ş ü` read back unchanged |
| Long jobs | 10 | A short `wait` answers `STILL_RUNNING` with an id, the `*_status` call gives the result |

Writes went only into a temporary folder and a test level, which were removed afterwards; project config, localization
and save game files were backed up and compared after each run. Nothing was left in the project.

Status: **OK** works as described · **FIXED** a problem found here, fixed in 0.5.2 with a test that reproduces it.

## Result

| Run | Passed | Failed | Editor crashes |
|---|---|---|---|
| First run (0.5.1) | 935 | 51 | 2 |
| After the fixes (0.5.2) | 996 | 0 | 0 |

Of the 51 failures in the first run, about 14 were real problems in the plugin (below). The rest were mistakes in the
round's own scripts: a wrong node title, a measurement taken before Play In Editor had started, the wrong expected error
code, a tool called without a required option, a button covered by the game's own tutorial window. In each of those the
tool answered correctly.

The plugin's own suites pass on the test project (24 suites, every new fix with a test).

## Problems found and fixed

| # | Tool | What was wrong | Fix |
|---|---|---|---|
| 1 | `widget_get` | Without `properties` on a panel widget the editor crashed (stack overflow): reading every property followed the widget's slot back to the widget forever. | Each subobject is written once; a repeat, or the widget itself, is written as its path. The same reader serves `object_get`, `settings_get` and `savegame_get`. |
| 2 | `widget_set` | An enum number outside the enum (`Visibility: 12345`) was stored and answered ok; the next render of the widget hit an engine assert and closed the editor. | Every property write checks enum numbers: out of range is refused with `INVALID_VALUE` and the valid names. Nothing is written. |
| 3 | `log_read` | `minVerbosity` also returned empty `Log` lines (console colour changes carry no level). | These lines are not stored. |
| 4 | `editor_capture`, `perf_capture`, `perf_gpu_profile`, `ui_key`, `pie_input` | Numbers outside the schema range were accepted: a negative width or duration, `top: 0`, a key hold longer than the limit. | The server checks the schema `minimum` / `maximum` of every tool before the call (`INVALID_ARGUMENT` with the range); the limits are in the schemas. |
| 5 | `asset_search_text`, `id_audit`, `ui_input_audit` | A content folder that does not exist gave an empty, successful result. | `FOLDER_NOT_FOUND`. |
| 6 | `id_audit` | A broken `fieldPattern` regex matched nothing, so the audit looked clean. | `INVALID_ARGUMENT` with what is wrong in the pattern. |
| 7 | `mrq_render` | A map that does not exist started a render job that could not load it. | `MAP_NOT_FOUND` before anything starts. |
| 8 | `blueprint_info` | Variable defaults read back empty after the Blueprint was compiled (the engine keeps them on the class defaults). | Read from the compiled class defaults. |

## Kept for 0.6

Nothing open from this round. `pie_state` and `pie_stop` take no input that can be wrong, so they only had real calls.

## Every tool

| Tool | Status | Real calls | Wrong-input calls | Slowest | Note |
|---|---|---|---|---|---|
| `actor_bounds` | OK | 4 | 2 | 0.03 s |  |
| `actor_delete` | OK | 3 | 4 | 0.58 s |  |
| `actor_find` | OK | 8 | 3 | 0.06 s |  |
| `actor_preview` | OK | 2 | 3 | 0.75 s |  |
| `actor_set_transform` | OK | 4 | 3 | 0.28 s |  |
| `actor_spawn` | OK | 5 | 3 | 0.55 s |  |
| `asset_delete` | OK | 3 | 4 | 1.98 s |  |
| `asset_duplicate` | OK | 1 | 3 | 0.26 s |  |
| `asset_exists` | OK | 16 | 5 | 0.27 s |  |
| `asset_find` | OK | 5 | 3 | 0.08 s |  |
| `asset_import` | OK | 4 | 4 | 1.12 s |  |
| `asset_references` | OK | 5 | 3 | 0.03 s |  |
| `asset_rename` | OK | 3 | 3 | 3.51 s |  |
| `asset_save` | OK | 3 | 2 | 0.28 s |  |
| `asset_search_text` | FIXED | 6 | 3 | 0.04 s | A missing folder gave an empty result (fix 5). |
| `automation_list` | OK | 3 | 2 | 13.29 s |  |
| `automation_run` | OK | 1 | 3 | 10.84 s |  |
| `batch` | OK | 4 | 6 | 0.03 s |  |
| `blueprint_add_component` | OK | 3 | 5 | 0.36 s |  |
| `blueprint_add_node` | OK | 7 | 4 | 0.24 s |  |
| `blueprint_add_variables` | OK | 2 | 4 | 0.34 s |  |
| `blueprint_compile` | OK | 4 | 3 | 0.72 s |  |
| `blueprint_connect` | OK | 2 | 5 | 0.26 s |  |
| `blueprint_create` | OK | 3 | 4 | 0.20 s |  |
| `blueprint_graph_read` | OK | 3 | 2 | 0.03 s |  |
| `blueprint_info` | FIXED | 4 | 2 | 0.24 s | Variable defaults read back empty after a compile (fix 8). |
| `blueprint_remove_component` | OK | 1 | 3 | 0.30 s |  |
| `blueprint_remove_node` | OK | 2 | 3 | 0.26 s |  |
| `blueprint_remove_variables` | OK | 1 | 3 | 0.36 s |  |
| `blueprint_set_component_defaults` | OK | 2 | 4 | 0.34 s |  |
| `blueprint_set_pin` | OK | 3 | 5 | 0.23 s |  |
| `build_status` | OK | 1 | 1 | 0.04 s |  |
| `class_measure` | OK | 5 | 2 | 0.60 s |  |
| `client_config` | OK | 2 | 2 | 0.02 s |  |
| `console_command` | OK | 6 | 3 | 0.04 s |  |
| `datatable_create` | OK | 3 | 3 | 0.69 s |  |
| `datatable_get` | OK | 20 | 6 | 0.34 s |  |
| `datatable_import` | OK | 4 | 9 | 0.33 s |  |
| `datatable_remove_rows` | OK | 1 | 3 | 0.40 s |  |
| `datatable_rename_row` | OK | 2 | 4 | 0.33 s |  |
| `datatable_set_rows` | OK | 6 | 10 | 0.34 s |  |
| `doctor` | OK | 3 | 1 | 0.27 s |  |
| `editor_capture` | FIXED | 6 | 4 | 0.24 s | A negative `maxWidth` was accepted (fix 4). |
| `editor_restart` | OK | 0 | 2 | 0.02 s |  |
| `editor_state` | OK | 8 | 1 | 0.12 s |  |
| `enum_create` | OK | 2 | 4 | 0.64 s |  |
| `font_coverage` | OK | 4 | 7 | 0.07 s |  |
| `game_run` | OK | 3 | 5 | 0.18 s |  |
| `game_run_status` | OK | 3 | 3 | 25.58 s |  |
| `gameplay_tag_add` | OK | 4 | 4 | 0.06 s |  |
| `gameplay_tag_remove` | OK | 3 | 3 | 0.21 s |  |
| `gameplay_tags_list` | OK | 7 | 3 | 0.18 s |  |
| `id_audit` | FIXED | 4 | 3 | 0.05 s | A broken regex matched nothing and the audit looked clean (fix 6). |
| `image_compose` | OK | 4 | 3 | 0.26 s |  |
| `image_stats` | OK | 6 | 3 | 0.04 s |  |
| `input_action_create` | OK | 3 | 2 | 0.27 s |  |
| `input_context_create` | OK | 1 | 2 | 0.25 s |  |
| `input_context_info` | OK | 5 | 2 | 0.19 s |  |
| `input_map` | OK | 4 | 4 | 0.29 s |  |
| `input_map_list` | OK | 6 | 4 | 0.88 s |  |
| `input_unmap` | OK | 2 | 2 | 0.17 s |  |
| `key_audit` | OK | 3 | 2 | 2.36 s |  |
| `level_audit` | OK | 6 | 3 | 0.25 s |  |
| `level_open` | OK | 4 | 3 | 1.13 s |  |
| `level_save` | OK | 3 | 2 | 0.66 s |  |
| `live_coding_compile` | OK | 0 | 1 | 0.02 s |  |
| `loc_culture` | OK | 5 | 3 | 3.33 s |  |
| `loc_gather` | OK | 2 | 6 | 0.08 s |  |
| `loc_gather_status` | OK | 2 | 4 | 0.03 s |  |
| `log_read` | FIXED | 7 | 3 | 0.03 s | `minVerbosity` also let empty colour-change lines through (fix 3). |
| `log_wait` | OK | 2 | 3 | 1.03 s |  |
| `material_add_expression` | OK | 2 | 4 | 0.81 s |  |
| `material_connect` | OK | 1 | 3 | 0.62 s |  |
| `material_create` | OK | 4 | 3 | 0.93 s |  |
| `material_info` | OK | 9 | 4 | 0.16 s |  |
| `material_instance_create` | OK | 2 | 3 | 0.22 s |  |
| `material_instance_set` | OK | 1 | 4 | 0.24 s |  |
| `mesh_info` | OK | 7 | 3 | 0.20 s |  |
| `mesh_set_material` | OK | 2 | 4 | 0.22 s |  |
| `mrq_render` | FIXED | 1 | 5 | 0.40 s | A missing map started a render that could not load it (fix 7). |
| `mrq_status` | OK | 4 | 1 | 0.02 s |  |
| `object_get` | OK | 7 | 4 | 0.29 s |  |
| `object_set` | OK | 3 | 6 | 0.29 s |  |
| `perf_capture` | FIXED | 3 | 4 | 6.03 s | Negative `seconds` was accepted (fix 4). |
| `perf_gpu_profile` | FIXED | 3 | 2 | 0.61 s | `top: 0` was accepted (fix 4). |
| `perf_scene` | OK | 3 | 2 | 0.02 s |  |
| `pie_actors` | OK | 4 | 4 | 2.06 s |  |
| `pie_input` | FIXED | 4 | 6 | 0.88 s | A hold longer than the limit was accepted (fix 4). |
| `pie_net_emulation` | OK | 4 | 5 | 0.58 s |  |
| `pie_start` | OK | 2 | 3 | 10.30 s |  |
| `pie_state` | OK | 6 | 0 | 0.19 s |  |
| `pie_stop` | OK | 5 | 0 | 24.17 s |  |
| `pie_travel` | OK | 1 | 4 | 1.22 s |  |
| `pie_walk_to` | OK | 1 | 4 | 2.04 s |  |
| `savegame_get` | OK | 6 | 3 | 0.03 s |  |
| `savegame_set` | OK | 1 | 4 | 0.05 s |  |
| `scene_dress` | OK | 8 | 6 | 0.51 s |  |
| `screen_coverage` | OK | 2 | 6 | 0.78 s |  |
| `screenshot` | OK | 5 | 3 | 0.43 s |  |
| `screenshot_ab` | OK | 3 | 2 | 1.09 s |  |
| `settings_get` | OK | 8 | 3 | 0.04 s |  |
| `settings_set` | OK | 4 | 4 | 0.14 s |  |
| `stringtable_compare` | OK | 8 | 8 | 0.06 s |  |
| `stringtable_get` | OK | 9 | 4 | 0.35 s |  |
| `stringtable_list` | OK | 4 | 2 | 0.03 s |  |
| `stringtable_set` | OK | 6 | 6 | 0.54 s |  |
| `struct_create` | OK | 3 | 3 | 2.44 s |  |
| `text_overflow_audit` | OK | 8 | 6 | 5.27 s |  |
| `texture_info` | OK | 3 | 3 | 0.03 s |  |
| `type_info` | OK | 6 | 2 | 0.33 s |  |
| `ui_click` | OK | 2 | 6 | 0.69 s |  |
| `ui_close` | OK | 2 | 4 | 0.05 s |  |
| `ui_input_audit` | FIXED | 5 | 3 | 0.06 s | A missing folder gave an empty result (fix 5). |
| `ui_input_type` | OK | 4 | 3 | 0.04 s |  |
| `ui_key` | FIXED | 4 | 6 | 0.48 s | A hold longer than the limit was accepted (fix 4). |
| `ui_open` | OK | 3 | 6 | 0.17 s |  |
| `ui_prompts` | OK | 4 | 2 | 0.06 s |  |
| `ui_state` | OK | 4 | 5 | 0.07 s |  |
| `validate` | OK | 5 | 2 | 0.22 s |  |
| `viewmodel_add` | OK | 2 | 3 | 0.04 s |  |
| `viewmodel_bind` | OK | 1 | 3 | 0.04 s |  |
| `viewmodel_list` | OK | 1 | 2 | 0.03 s |  |
| `viewmodel_remove` | OK | 1 | 2 | 0.02 s |  |
| `viewmodel_unbind` | OK | 1 | 2 | 0.02 s |  |
| `widget_add` | OK | 9 | 9 | 0.44 s |  |
| `widget_create` | OK | 5 | 4 | 0.35 s |  |
| `widget_get` | FIXED | 5 | 5 | 0.15 s | Without `properties` on a panel widget the editor crashed (fix 1). |
| `widget_remove` | OK | 2 | 3 | 0.23 s |  |
| `widget_render` | OK | 6 | 7 | 0.59 s |  |
| `widget_set` | FIXED | 5 | 6 | 0.37 s | An enum number outside the enum was stored and crashed the next render (fix 2). |
| `widget_tree` | OK | 5 | 4 | 0.13 s |  |
| `world_trace` | OK | 5 | 2 | 0.67 s |  |
