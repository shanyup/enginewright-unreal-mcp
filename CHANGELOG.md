# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.5.2] - 2026-11-21
### Fixed
- `widget_get` without `properties` on a panel widget crashed the editor (stack overflow): reading every property followed the widget's `Slot` back to the widget forever. Each subobject is now exported once; repeats and the owner itself are written as their path. The same reader serves `object_get`, `settings_get` and `savegame_get`.
- `widget_set` (and every property write) accepted an enum number outside the enum, e.g. `Visibility: 12345`, and the next widget render hit an engine assert. Out-of-range numbers are refused with `INVALID_VALUE` and the valid names.
- `log_read` with `minVerbosity` also returned empty `Log` lines (console colour changes); these are no longer stored.
- Number arguments are checked against the schema `minimum` / `maximum` (e.g. negative `maxWidth` in `editor_capture`, negative `seconds` in `perf_capture`, `top: 0` in `perf_gpu_profile`, holds above the limit in `ui_key` and `pie_input`).
- `asset_search_text`, `id_audit` and `ui_input_audit` answer `FOLDER_NOT_FOUND` for a missing content folder instead of an empty result.
- `id_audit` refuses a broken `fieldPattern` regex with `INVALID_ARGUMENT` instead of matching nothing.
- `mrq_render` answers `MAP_NOT_FOUND` for a map that does not exist instead of starting a render that cannot load it.

## [0.5.1] - 2026-11-20
### Security
- The HTTP server refuses requests whose `Origin` header names another web site (403, before the token check), so a web page open in a browser cannot reach the editor. Requests without `Origin` (MCP clients, scripts) and from `localhost`, `127.0.0.1` or `[::1]` pass. Trusted local web tools can be listed in the new `AllowedOrigins` setting.

### Added
- [docs/WITH_EPIC_MCP.md](docs/WITH_EPIC_MCP.md): Enginewright and Epic's Unreal MCP server in the same editor, connected to one agent.

### Changed
- `pie_travel`: the `options` example uses a generic game mode.
- README, SUPPORT, ROADMAP and RELIABILITY_0.4 brought up to date (tool table complete: 132 tools; roadmap follows the release series).

## [0.5.0] - 2026-11-20
### Added
- `text_overflow_audit`: for each visible TextBlock of a Widget Blueprint or UserWidget class (or the open UI during Play In Editor), the width the text wants against the width its slot gives it, per language; lists the overflows. Auto Wrap Text and ScaleBox are not reported as overflow (a word wider than the slot is).
- `datatable_import`: CSV or JSON file into a DataTable (upsert) with a strict field check, a diff of added / changed / removed rows, `dryRun`, and a read-back after the write.
- `level_audit`: hidden actors, visible static-mesh actors with no gameplay component, and meshes that match placeholder / kit name patterns, with counts and actor paths; works on the open level or a saved map.

### Changed
- `asset_find` warns when the folder does not exist.
- `asset_import` and `blueprint_create` warn about non-ASCII asset names and suggest an ASCII name.
- `game_run` `keepRuns` (default 20) deletes the oldest run folders under `Saved/Enginewright/runs` before a run starts.
- `actor_preview` leaves camera proxy, frustum and boom parts out of the framing bounds.
- `pie_start`: the default `timeout` is 180 s for several players (60 s for one), the range is checked (1 to 900), and the TIMEOUT message says the session is still starting.
- Tests: `perf_capture`, `perf_scene`, `perf_gpu_profile`, `mrq_render`, `mrq_status`, `asset_references` and `asset_save` have tests now (132 of 132 tools covered).

### Fixed (found by using the new tools on a real game project)
- `text_overflow_audit` measured game text in the source language in the editor; it now turns on the game localization preview for each culture (and restores it), and warns when a translation was not applied.
- `stringtable_compare`: a format argument used twice (`{0} {0}`) counted as two arguments; a CRLF line ending in the CSV source counted as a changed source.
- `stringtable_compare`, `game_run`: file arguments resolve to full paths (no `../` in results).
- `datatable_import`: no "did not change" warning for every kept field.
- `asset_rename`: `referencersUpdated` was empty; it lists the assets whose references were fixed.

## [0.4.0] - 2026-11-10
Release notes: [docs/release-notes/v0.4.0.md](docs/release-notes/v0.4.0.md)

### Added
- Localization: `stringtable_compare` reads a `.po`, `.locres` or CSV translation, not only two String Tables, with translations of an older source text and fuzzy entries.
- Localization: `loc_gather` / `loc_gather_status` take a target through gather, export, import, compile and check in one call, with the verdict from both exit codes.
- Localization: `game_run` `culture` starts the game process with a language, not only the editor.
- Localization: `font_coverage` and `game_run` `glyphMisses` find characters a font or Slate cannot draw.
- MVVM: `viewmodel_list`, `viewmodel_add`, `viewmodel_bind`, `viewmodel_unbind` and `viewmodel_remove` in the optional `EnginewrightMVVM` plugin.
- `image_stats`: mean luma, luma histogram (16 bins), saturation, clipped black / white and the difference to a reference image.
- `game_run` `shotNames`, `cvars` and `variants`.
- Game testing: `ui_key`, `pie_walk_to`, `pie_travel`, `input_map_list`, `ui_prompts`.
- Save games: `savegame_get` / `savegame_set`.
- Scene tools: `scene_dress`, `actor_preview`.
- Editor panel: the version label and the "What's new / Roadmap" and "Changelog" buttons.
- `docs/SUPPORT.md` (Unreal feature support matrix), `CHANGELOG.md`, the roadmap Requests section and the request flow in CONTRIBUTING.md.
- GitHub Discussions (Announcements, Ideas, Q&A, Show and tell); the issue chooser links questions and rough ideas there.
- Release: the optional MVVM plugin as its own zip, `EnginewrightMVVM-0.4.0-UE5.8-Win64.zip`.

### Changed
- `ui_state` lists only visible widgets on the top page of their stack (visible, page activated, button text, tooltip and label); `includeHidden` adds the rest.
- `ui_click` finds a button by name, label, any text inside it or tooltip, and ignores closed and covered pages.
- `asset_delete` says why an asset cannot be deleted, per asset.
- `mesh_info` returns local bounds, size and the pivot offsets without spawning an actor.
- `blueprint_set_component_defaults` writes defaults of an inherited component.
- `asset_import` takes `scale`, `rotation`, `translation` and `meshType`.
- Every tool argument has a type and a description.

### Fixed
- `game_run` `shotsFound` counted a screenshot copied from another folder twice (ER-09); it now counts each PNG once.
- `datatable_set_rows` checks rows on copies first, so a failed call no longer overwrites every row's `Name` field.
- `level_save` with `path` writes the `.umap` inside `Content/` and registers it with the asset registry.
- `asset_delete` deletes a set of assets that only use each other, and String Tables without `force`.
- `gameplay_tag_add` / `gameplay_tag_remove` remove a tag ini they created when its last tag is removed.
- `game_run` moves screenshots from `Saved/Screenshots` into the run folder instead of copying them.
- `game_run` reports `MAP_NOT_FOUND` for a `/Game` map in `url` that does not exist, also in `dryRun`.
- `game_run` `dryRun` heavy-level warning matches what happens (viewports pause by default).
- `stringtable_compare` uses the CSV file name as namespace when the `.po` has that namespace.
- 80 arguments had no description and 8 had no type; every argument now has a type and a description.
- `key_audit`, `id_audit`, `automation_list`, `automation_run`, a stopped `game_run` and `settings_set` describe their real behaviour.
- `tests/smoke.py` with no editor prints one line instead of a urllib traceback.

## [0.3.1] - 2026-11-09
Release notes: [docs/release-notes/v0.3.1.md](docs/release-notes/v0.3.1.md)

### Added
- Status bar indicator (`Enginewright :3100 (n)`) and the Enginewright panel (server on/off, port, token, client configs, clients, tool categories, read-only mode, log level, recent calls, `doctor`).
- Settings in Project Settings > Plugins > Enginewright, saved per user and project.
- `editor_capture` writes the editor window or the Enginewright panel to a PNG.
- Issue forms, pull request template, [CONTRIBUTING.md](CONTRIBUTING.md), [docs/REQUEST_WITH_AI.md](docs/REQUEST_WITH_AI.md) and labelled good first issues.

### Fixed
- `widget_render` also renders C++ UserWidget classes, not only Widget Blueprints.
- `game_run` refuses to start after a Live Coding patch and warns when C++ source changed after the last build.
- `game_run` pauses the editor's realtime level viewports and reports a heavy editor level.
- `game_run` `shotsExpected` ends a screenshot run when the PNG files are written, with a `PASSED` verdict.

## [0.3.0] - 2026-11-08
Release notes: [docs/release-notes/v0.3.0.md](docs/release-notes/v0.3.0.md)

### Added
- UMG: `widget_create`, `widget_tree`, `widget_get`, `widget_add`, `widget_set`, `widget_remove` and `widget_render`.
- Runtime UI with CommonUI: `ui_state`, `ui_click`, `ui_open`, `ui_close`, `ui_input_audit`, `ui_input_type`.
- Localization: `stringtable_list`, `stringtable_get`, `stringtable_set`, `stringtable_compare`, `loc_culture`.
- `game_run` / `game_run_status`, `image_compose`.
- Gameplay Tags: `gameplay_tags_list`, `gameplay_tag_add`, `gameplay_tag_remove`.
- Multiplayer tests: `pie_actors`, `pie_net_emulation`.
- `class_measure`, `screen_coverage`.
- Optional `Scripts/ewtool.py` build helper for the one job outside the editor.

### Changed
- `asset_rename` reports text and IDs that still name the old asset; `dryRun` previews them.
- `object_set` on a class default also updates placed instances and widget templates that still had the old default.
- `automation_list` / `automation_run` keep the editor at full speed in the background.
- `screenshot` of Play In Editor includes the UI, as the player sees it; `ui: false` captures the scene only.
- Every tool argument has a description in `describe`.

### Fixed
- The plugin builds in projects that use unity builds.
- Adding and removing widgets keeps the Widget Blueprint's variable list consistent.

## [0.2.0] - 2026-11-07
Release notes: [docs/release-notes/v0.2.0.md](docs/release-notes/v0.2.0.md)

### Added
- Prebuilt UE 5.8 Win64 zip and ready-to-paste client configs for Claude Code, Codex, Cursor, VS Code and Hermes (`client_config`).
- `screenshot`, `screenshot_ab`.
- Play In Editor control: `pie_start` (1-4 players, standalone / listen / client), `pie_input`, `log_wait`.
- Build loop: `live_coding_compile`, `editor_restart`, `build_status`, `automation_list`, `automation_run`.
- Blueprint, Material and Material Instance, mesh and texture tools; enum and struct creation.
- Import while the editor is open (texture, sound, font, mesh), Enhanced Input helpers, validation.
- Content checks: `id_audit`, `key_audit`.
- `level_open`, `level_save`.
- Test suites that build their own fixtures (`tests/run_all.py`).

### Fixed
- Deleting an asset and creating it again at the same path in one editor session works.
- Renaming an asset no longer redirects a new asset created later at the old path.
- Blueprint enum variables accept the enumerator's display name as default value; an unknown name is an error with a suggestion.

## [0.1.0] - 2026-11-06

### Added
- In-editor HTTP MCP server with token and the three meta tools `search`, `describe` and `call`.
- Strict validation with read-back.
- Object, asset, DataTable, level, editor, project settings and log tools.
- `doctor`.
- Atomic `batch` with rollback.

[Unreleased]: https://github.com/shanyup/enginewright/compare/v0.5.1...main
[0.5.1]: https://github.com/shanyup/enginewright/compare/v0.5.0...v0.5.1
[0.5.0]: https://github.com/shanyup/enginewright/compare/v0.4.0...v0.5.0
[0.4.0]: https://github.com/shanyup/enginewright/compare/v0.3.1...v0.4.0
[0.3.1]: https://github.com/shanyup/enginewright/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/shanyup/enginewright/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/shanyup/enginewright/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/shanyup/enginewright/releases/tag/v0.1.0
