# ÖZET — Enginewright yol haritası + emsal + SEO + duyuru (t_1e95fc46, 16.11.2026)

## Konumlandırma

**"Playwright for Unreal"**: UE 5.8 editör içi MCP sunucusu; AI agent (Claude/Codex/Cursor) derleme, PIE, paketleme, screenshot, seviye incelemesi yapar. 129 araç (kod sayımı 16.11), MIT, public, **10★**. En yakın rakip ChiR24/Unreal_mcp 2.718★; Epic'in resmi UE 5.8 MCP'si (~870 araç) var ama oyun iş akışlarında (cook/package, PIE senaryosu, ağ testi, görsel QA) boş — biz oraya odaklanıyoruz.

## Çıktı listesi (docs/plan_2026-11/)

- `00_Plan.md` — plan + okuma listesi
- `01_Rakip_ve_Emsal_Karsilastirma.md` — rakip matrisi (16.11 verisi), özellik matrisi, önde/geride/kimsede-yok, Epic çakışma stratejisi
- `02_Oyun_Ihtiyaclari.md` — ARPG (bitmiş kart t_1ac37fc7 çıktısı: §12 EW-1..11 + 08/09) + GP (kart çalışıyor → eski belgeler: GDD_v2 + ParoleLoop_TaskList) + RagnaCook (kart çalışıyor → eski belgeler: Demo_Plan/03, TaskList, tools/art/README, Gorsel_Inceleme, Devir_Notu) → 16 yeni araç listesi, saat kazançlarıyla
- `03_Yol_Haritasi.md` — v0.4.1 sanat hattı → v0.5 demo/çekim → v0.6 içerik+ağ (API freeze) → 1.0 launch → sonrası; eski roadmaplerle eşleme; yapmayacaklarımız
- `04_Kesfedilebilirlik_SEO.md` — GitHub ayarları (350 krk açıklama, 20 topic, 1280x640 önizleme), liste kayıtları (resmi registry, awesome-mcp-servers+Glama badge, mcp.so, cursor.directory, awesome-unreal; PulseMCP kapalı→registry'den; Smithery MCPB değil), Pages docs sitesi + EN anahtar kelimeler, Fab free code plugin uygunluğu
- `05_Duyuru_ve_Yildiz_Buyume_Plani.md` — 90 günlük takvim (HN Show HN 1.0'da tek hak, r/unrealengine dev-for-dev, r/gamedev MM, X, YouTube, dev.to, PH, UE forum, Discord), 6 demo video senaryosu, yıldız hedefleri 30g 80-150 / 90g 300-600 / 180g 800-1.500 (tahmin, emsal: blender-mcp 34k, playwright 39k, unity-mcp 16k), YAPMA listesi
- `SORULAR.md` — 10 soru (Türkçe iç belgeler, Fab, registry, konumlandırma, lisans, Discord, video, v0.5 kapsamı, ücretli katman, HN zamanlaması)

## İlk 10 iş (sıralı)

1. S-1 kararı: Türkçe iç belgeler private'a (push yok — onay sonrası).
2. v0.4.1: `asset_pipeline` + `asset_import_batch` + `image_diff` (RagnaCook art hattı).
3. SEO paketi (04 §1): topics, social preview, Pages iskeleti — Talha onayıyla.
4. Resmi MCP registry: `server.json` + `mcp-publisher publish` (S-3).
5. Glama submit → score badge → awesome-mcp-servers PR.
6. v0.5: `package_build`, `screenshot_tour`, `visual_review`, `coop_test`, `pie_scenario`.
7. 5 demo video (05 §3) — X + YouTube + r/unrealengine.
8. v0.6: `datatable_csv`, `world_inventory`, `actor_set_props`, `scene_snapshot/diff`, `net_test_2pc`, `replication_check`, `log_analyze`, `blueprint_compile_all`, `level_flow_test`, `physics_test` → API freeze.
9. v1.0 launch: Show HN (tek), Product Hunt, release notes, tag, Fab listing (S-2).
10. Haftalık ölçüm: star-history + GitHub traffic; hedef 90 gün 300-600★.

## Sürüm takvimi (tahmin)

| Sürüm | Süre | Tema |
|---|---|---|
| v0.4.1 | ~2 hf | sanat hattı (RagnaCook) |
| v0.5 | ~3-4 hf | demo + çekim otomasyonu |
| v0.6 | ~4-5 hf | içerik üretimi + ağ, API freeze |
| v1.0 | ~2-3 hf | launch + SEO/registry |

## Yıldız hedefleri

10★ (bugün) → 30 gün: 80-150 → 90 gün: 300-600 → 180 gün: 800-1.500 (tahmin; emsal büyüme 01 §2'de).

## Notlar

- Yalnız belge üretildi; kod/plugin/derleme yok. GitHub'da hiçbir değişiklik yok — tüm dış aksiyonlar Talha onayı sonrası.
- Her çıktı tek başına yerel commit; **push yok**.
- GP/RagnaCook ihtiyaçları eski belgelerden alındı (kartlar hâlâ çalışıyor) — kartlar bitince 02 yeniden gözden geçirilir.
- Rakip sayıları 16.11.2026 GitHub API; özellik hücreleri 10.11 derin raporları + 16.11 web.
