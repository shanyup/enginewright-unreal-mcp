# Enginewright 1.0.0 roadmap — taslak (adım adım, Talha ile)

> Durum: hazırlanıyor (10.11.2026). Onaylanınca `ROADMAP.md`'nin yerini alır. Blender hattı bu belgede yok (ayrı plan).

## Adım 1 — Hedef ve kapsam (Talha 10.11, KESİN)
| Soru | Karar |
|---|---|
| 1.0.0 hedefi | **Genel amaçlı ürün.** RagnaCook yalnız test vakası: her araç oyuna özgü olmayan, her UE projesinde çalışan biçimde yapılır. |
| Sürüm bölme | **Oyun üretim aşamalarına göre:** lookdev → altın asset → seri üretim → demo → çıkış. Her sürüm o aşamanın bütün UE ihtiyacını kapsar; bir aşama sonraki sürümü beklemez. |
| Ajan eğitimi | **Ayrı "sanat ajanı" çalışanı** (Brain `agents/company.json`) + kendi kartları. |
| Kapsam | **Yalnız UE tarafı.** Blender (retopo, UV, bake, skinleme) ayrı plan. |

Sonuç kuralı: aşama sırası RagnaCook'tan gelir, aracın tasarımı geneldir (proje adı, yol, iskelet adı sabit kodlanmaz; parametre ya da proje ayarı).

## Adım 2 — Rakip analizi (10.11, Talha kararı bekliyor)
Kaynak: `docs/arastirma/rakip_analizi_2026-11.md` (13 rakip, 30 eksik, kaynaklı). Alt ajan raporu; RagnaCook etiketleri Genel Merkez'de düzeltildi (alt ajan landscape'i "evet" demişti, mutfak oyununda yok).

Bizde olup rakiplerin çoğunda olmayan (koru, öne çıkar): çok oyunculu PIE + replikasyon/net emülasyonu, `game_run` (paketli/standalone koşu, kültür, varyant, glyph), görsel QA (`image_stats` SSIM), lokalizasyon hattı, CommonUI prompt denetimi, `actor_preview`, `scene_dress`, `id_audit`/`key_audit`, savegame.

| Grup | Eksik | RagnaCook aşaması |
|---|---|---|
| A Kesin | Niagara yazma | **Lookdev / altın asset** (köz, buhar): 1.0.0'dan öne çekilir |
| A | Import hattı (FBX/LOD/UCX/soket), iskelet bağlama, IK retarget, Montage/BlendSpace, AnimBP state machine | Altın asset + karakter |
| A | Sanat bütçe denetimi, asset metadata (kaynak/lisans/AI bayrağı → Steam AI beyanı) | Seri üretim |
| A | Işık + navmesh build, perf profili (paketli build, Insights) | Demo |
| A | Paketleme/cook + build arşivi, headless mod | Demo / çıkış |
| A | Sequencer + Movie Render Queue | Steam sayfası trailer/devlog (demo öncesi) |
| A | Python kaçış kapısı, proje tarifleri, değişiklik raporu | Hepsi (ajan verimi) |
| B Şüpheli | Water/okyanus (gemi güvertesi), PCG, BT/StateTree yazma (boss/NPC) | ? |
| C Genel ürün | GAS, Control Rig, MetaHuman, Slate inspector, Landscape/Foliage, plugin yönetimi | RagnaCook kullanmaz |
| D Kapsam dışı | Eklenti içinde AI 3D üretimi, otonom playtest ajanı, çoklu ajan, yerel LLM | Ajan/oyun tarafı, eklenti değil |

**Kararlar (Talha 10.11):**
- **1.0.0 = tam bir genel eklenti:** "normal bir plugin'in desteklediği her şey". B ve C gruplarının hepsi 1.0.0'a girer (su materyali + Water plugin, BT/StateTree, PCG, GAS, Control Rig, MetaHuman, Landscape/Foliage, Slate inspector, plugin yönetimi), RagnaCook'un kullandığı aşamalardan sonra.
- D grubu kapsam dışı: AI 3D üretimi ve otonom playtest ajan/oyun tarafında kalır.
- Epic araçlarına tavır: açık (Adım 2b, sürüm desteği ile birlikte).

## Adım 2b — Mimari ve sürüm desteği (Talha 10.11, KESİN)
| Soru | Karar |
|---|---|
| Mimari | **Kendi çekirdek + `EWCompat.h` + özellik alt eklentileri.** Epic MCP sarmalanmaz (yalnız 5.8+, deneysel; 5.4-5.7 desteğini öldürür). Epic envanteri yalnız kapsam kontrol listesi. |
| Sürüme özgü özellik | Yalnız destekleyen sürümde açılır; diğerlerinde `VERSION_UNSUPPORTED` + `doctor`/`tools/list` yetenek raporu. Sürüm farkları yalnız `EWCompat.h`'de. |
| Alt eklenti deseni | `EnginewrightMVVM` gibi: Niagara, Sequencer, Anim, PCG, Water, GAS, MetaHuman, ControlRig… Projede olmayan motor eklentisini zorla açmaz (D0 sorunu: bugün Niagara, GameplayAbilities, SmartObjects zorunlu açılıyor → çözülür). |
| UE aralığı | **5.4 – 5.8**, çıkınca 5.9. |
| Platform | **Win64 + Mac + Linux.** Mac/Linux test yöntemi sonra planlanır. |
| Kurulu motorlar | Yalnız 5.8. 5.4-5.7 kurulumu Talha'da (sürüm matrisi bu kurulumdan sonra). |

## Karakter dilimi (Talha 10.11 21:50, KESİN)
RagnaCook Viking aşçısı uçtan uca kurulur: Tripo 3D → Blender (Rig_Medium skin) → UE import → **Mutable** (parça aç/kapa + bölge maskeleri + oyuncu rengi) → **menüde özelleştirme ekranı** → oyuncu karakteri + 4 oyunculu replikasyon. Animasyon: KayKit Rig_Medium klipleri; eksik pişirme animasyonları listelenip sonra karar.
- Enginewright araçları **adım geldikçe** yazılır (iskelet hedefli FBX import, soket, Physics Asset, anim önizleme, Mutable kurma/derleme/parametre), RagnaCook'ta kabul edilir. Kod baştan `EWCompat.h` + alt eklenti deseniyle.
- Paralel: **derin rakip incelemesi** (4 Opus alt ajanı, `docs/arastirma/derin/01-04`). Roadmap bu incelemeden sonra bir kez yazılıp dondurulur; karakter diliminde yazılan araçlar o roadmap'e yerleştirilir.

## Derin rakip incelemesi — özet (10.11 23:00, 4 Opus raporu, kaynaklı)
Raporlar: `docs/arastirma/derin/01_epic.md`, `02_buyuk_acik_kaynak.md`, `03_kucuk_acik_kaynak.md`, `04_ticari.md`. Eski `rakip_analizi_2026-11.md` hatalı, kaynak değil.
- **Epic 5.8:** 58 toolset / 878 araç. Yok: AnimBP, Montage/BlendSpace, IK retarget, Mutable, paketleme, Landscape/Water, profiling, multiplayer PIE, lokalizasyon. BT/StateTree yalnız okuma. 5.9: execute_python, Perf/Input/NavMesh toolset'leri, readOnlyHint, paketli oyunda MCP.
- **Güçlü rakipler:** UE-MCP (5.4-5.9, Mutable + N-client PIE + checkpoint/flow), EngineWire (5.0-5.8, anim yazma, 407 satır compat header), monolith (1457 action, anim/Niagara/GAS yazma), VibeUE (Mutable), NeoStack/Ultimate CoPilot (ticari, geniş).
- **En büyük açığımız:** animasyon + iskeletli import (karakter dilimiyle örtüşüyor), Niagara, Python exec, profiling, paketleme, çoklu sürüm (kodda hiç sürüm makrosu yok).
- **Güvenlik açığı (1.0 öncesi şart):** HTTP sunucuda Origin kontrolü yok; `readOnlyHint` yayımlanmıyor.
- **Bizde olup onlarda olmayan:** N-client PIE + ağ emülasyonu, `game_run`, görsel A/B (`screenshot_ab`/`image_stats`), lokalizasyon zinciri, oyuncu gözünden UI testi + `ui_prompts`, savegame, id/key audit.
- **Çoklu sürüm tekniği önerisi:** Ivan yöntemi (motorun `UE_VERSION_OLDER_THAN` + `__has_include` + CI matrisi) en ucuzu.
- Açık karar: `01_epic.md` Ek A Epic araç adlarını listeliyor (K2: envanter açık dala girmez) → `docs/arastirma/` commit edilmeden önce Talha kararı.

## Adım 3 — Aşama → sürüm (Talha 10.11)
| # | RagnaCook aşaması | Enginewright sürümü |
|---|---|---|
| 0 | Temel (hemen) | **0.5 Temel:** alt eklentilere bölme (D0), `EWCompat.h`, yetenek raporu |
| 1 | Lookdev + altın asset (Kas–Ara) | **0.6 Sanat 1:** FBX/LOD/UCX/soket import, ana malzeme şablonu, post/kontur, texel+bütçe denetimi, Niagara çekirdeği, ışık/gök ayarı |
| 2 | Seri üretim + karakter + boss (Ara–Mar) | **0.7 Sanat 2:** toplu import, metadata/AI bayrağı, iskelet bağlama, IK retarget, Montage/BlendSpace/notify, AnimBP state machine, Leader Pose **+ Mutable (demo customization ile çıkar, Talha)**, Physics Asset |
| 3 | Steam sayfası / trailer (Mar) | **0.8 Sinematik:** Sequencer, MRQ |
| 4 | Demo (Mar–May) | **0.9 Yayın:** paketleme/cook/arşiv, paketli perf profili, ışık/nav build, headless |
| 5 | 1.0.0 | B+C grupları, sürüm/platform matrisi, API dondurma, FAB, vitrin |
- **1.0.0 tarihi yok: hazır olunca** (Talha).
- **Test önceliği: 5.8 + RagnaCook.** Her sürüm önce 5.8'de RagnaCook üzerinde kabul edilir; 5.4-5.7 ve Mac/Linux testleri yayın aşamasında (1.0.0 öncesi). Talha'ya hangi motorları kuracağı o zaman söylenir. Kod baştan `EWCompat.h` ile yazılır ki sonradan toplu düzeltme az olsun.
- Eski plandan fark: 0.4.1 (sanat import) → 0.6'ya genişledi; 0.5 (BT/AnimBP/Mutable) dağıldı (AnimBP+Mutable 0.7, BT 1.0.0); 0.6 API dondurma → 1.0.0 öncesine; Niagara + Sequencer + headless 1.0.0'dan 0.6/0.8/0.9'a çekildi.
## Adım 4 — Sürüm sırası ve kabul testleri (sonra)
## Adım 5 — Sanat ajanı tanımı (sonra)
