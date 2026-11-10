# 04 — Ticari / ajan tabanlı Unreal AI ürünleri: derin, kaynaklı envanter

Okuma tarihi: 2026-11-10. Tüm URL'ler bu tarihte okundu (curl/HTML→metin, web_extract, GitHub API).
Etiketler: **doğrulandı** = resmî doküman/changelog/fiyat sayfasında somut detayla yazıyor; **iddia** = pazarlama sayfası, ürünün kendi benchmark'ı veya kanıtsız genel cümle; **3. taraf** = başka bir firma/kullanıcı; **bulunamadı** = okunan resmî kaynaklarda geçmiyor (ürünün sahip olmadığı anlamına gelmez).
Kapsam: Ludus AI, Ramen VR Aura, Betide NeoStack (eski adı Agent Integration Kit), Ultimate Engine CoPilot (UEC, eski adı Ultimate Blueprint Generator), GitHubCopilotUE (+ atgoldberg/UnrealCopilot), Tripo UE, Meshy UE, Muddy Terrain GenAI Model Generator.
Enginewright temel çizgisi: `README.md` (v0.4.0, commit 192322b, 2026-11-10) ve `Plugins/**/Tools/*.cpp` içinde `Register(TEXT("..."))` ile kayıtlı **107 tool** (+3 meta tool `search/describe/call`, + opsiyonel MVVM eklentisi).

---

## 0. Önceki taslakta (`rakip_analizi_2026-11.md`) düzeltilmesi gerekenler

| Taslaktaki iddia | Doğrusu (kaynak) |
|---|---|
| Aura: "fiyat açıklanmadı", "Beta: davetli" | Fiyat açık: Indie $20/ay (yıllık $10/ay), Pro $40/ay, Ultimate $200/ay, Fab'da "Source Access + 1 yıl Indie" seçeneği, Enterprise. 2 hafta deneme + $10 premium kredi. https://www.tryaura.dev/about — doğrulandı |
| Aura: "5.8+ (Mac/Win)", "v15.0 (Haz 2026)" | Windows UE 5.4–5.8, macOS (Apple Silicon) yalnız 5.7–5.8 (https://www.tryaura.dev/documentation). Sürüm numaralandırması 15.x → 1.0'a sıfırlandı; güncel **v1.2.5, 6 Kas 2026** (https://www.tryaura.dev/documentation/release-notes) |
| Aura: "Telos, Dragon" ajanları | Okunan dokümanda **bulunamadı**. Dokümanda 8 uzman ajan: Blueprint, VFX, Level Design, Material, Behavior Tree, Audio, Python, Verification (https://www.tryaura.dev/documentation/custom-agents) |
| NeoStack: "5.8+", "fiyat açıklanmamış", "v1.0.66" | UE 5.5–5.8, plugin **v4.0.17 (21 Eyl 2026)**, $20/koltuk/ay (+ Fab'da tek seferlik plugin lisansı) — https://neostack.dev/for-ai, https://neostack.dev/changelog |
| Ludus: "v1.0 Eyl 2026" | Ludus için herkese açık sürüm numarası/changelog **bulunamadı** (`/changelog` 404). |
| Tripo/Meshy: "5.1–5.7" | Tripo API plugin v1.0 → UE 5.6.0; Tripo DCC Bridge → UE 5.4+; Meshy → UE 5.4–5.8; 5.1–5.7 yalnız **Muddy Terrain** için doğru (kaynaklar §5) |
| flopperam "Aura'ya devredildi" | Aura dokümanlarında/hakkında sayfasında **bulunamadı**; doğrulanmadan kullanılmamalı. |

---

## 1. Ludus AI (Ludus AI P.S.A.)

**Kimlik/sürüm:** Sürüm/changelog bulunamadı. Plugin açılışta güncelleme bildirir ve mümkünse kendini günceller (https://docs.ludusengine.com/help/technical-faq — doğrulandı).
**Mimari:** UE editör plugin'i + bulut backend (Ludus API, konuşma durumu sunucuda) + web app (app.ludusengine.com). Proje "sentezi"/indeksi ve uzun dönem hafıza. Harici ajanlar için **uzak (hosted) MCP**: `https://mcp.ludusengine.com/mcp`, HTTP, tarayıcı OAuth; varsayılan kapalı ("Allow external agents (MCP)"). MCP **6 tool** açar: `list_editors, send_message, get_thread, list_threads, cancel, get_account` — yani dış ajan granüler editör tool'u değil, **Ludus ajanına görev devreder**; Ludus görevi kendi tool'larıyla (Python dahil) çalıştırır, onaylar yine Unreal içinde beklenir, MCP görevleri aynı kredi ile faturalanır (https://docs.ludusengine.com/mcp — doğrulandı).
**Fiyat:** Pro $25/ay (yıllık $240) 70k kredi; Studio $80 (yıllık $768) 250k; Ultimate $180 (yıllık $1.728) 600k; kredi paketleri 35k/$17 … 600k/$190; 14 gün deneme 50k kredi; tüm planlarda tüm özellikler; Enterprise "Private Cloud"; iade genelde yok (https://docs.ludusengine.com/account/plans-and-billing — doğrulandı). Kredi maliyeti dinamik: mod (Agent Lite < Agent < Agent Max), bağlam boyutu, sohbet uzunluğu (https://docs.ludusengine.com/account/credits-and-costs).
**UE/platform:** UE 5.4–5.8 (her editing sayfasında). Windows (imzalı MSI), macOS yalnız Apple Silicon, Linux AppImage (technical-faq — doğrulandı).
**Güvenlik/uyum:** ISO/IEC 27001:2022 ve SOC 2 sertifikası beyanı (https://docs.ludusengine.com/data-security — iddia, sertifika belgesi görülmedi).

**Alanlar (hepsi https://docs.ludusengine.com/<sayfa>, doğrulandı, derinlik notuyla):**
- Blueprint (`blueprints/overview`): değişken/fonksiyon/default, Timeline, custom event, delegate, UMG Widget graph mantığı, comment box gruplama, çoklu-BP sistemleri. Widget *tree/layout* yazımı açıkça geçmiyor → bulunamadı.
- AnimBP + State Machine, transition + koşul yazımı; **Control Rig**: bone/control/null ekleme, hiyerarşi parent/transform, control ayarları (`generation/character-animation`).
- Animasyon üretimi (text-to-motion), loop, iki klip "stitch" blend, **retarget** (Mixamo→Mannequin gibi farklı iskelet), import öncesi önizleme kartı.
- **Auto-rigging**: static mesh → skeletal mesh + skeleton; "yalnız standart humanoid güvenilir" (`generation/rigging`).
- 3D üretim: text/görsel→mesh, retexture, retopology, parça segmentasyonu; kalite low–ultra; level'e placeholder koyup bitince değiştirme; önce 2D referans görsel iterasyonu (`generation/3d-models-and-transforms`). Sağlayıcı adı **bulunamadı**.
- Material graph + Material Instance toplu parametre (`editing/materials`); Niagara: modül ekle/sil/sırala, emitter/renderer/data interface/user param, tarif üzerinden tam sistem kurma (`editing/niagara`); PCG graph pin/param ve nokta yerleşimi (`editing/pcg`); Behavior Tree + EQS + Blackboard key (`editing/behavior-trees-and-eqs`); Enum/Struct/DataTable (`editing/data-structures`); Audio (`editing/audio`); Level: actor taşı/ekle/sil, property, component ekle/reparent, folder (`editing/levels-and-scenes`).
- **MetaHuman**: yalnız face solver / contour tracker *leaf* ayarları; semantic root salt-okunur; prompt'tan yeni kimlik yok (`editing/metahuman`) — sığ.
- C++: .h/.cpp yaz/düzenle, sonrasında otomatik Live Coding (`editing/cpp-live-coding`).
- **Bulunamadı:** Sequencer/MRQ, Landscape/Foliage, Water, StateTree, GAS, Montage/BlendSpace yazımı, Physics Asset/socket, Mutable, PIE/playtest/doğrulama ajanı, multiplayer, packaging, profiling, localization, headless.

**UX/güvenlik (doğrulandı, `agent/approvals-safety`, `agent/planning-memory-subagents`, `getting-started/plugin-interface`):** 4 onay modu — Default (mutasyon öncesi sor), Ask every time (okumalar dahil), **Yolo**, Custom (tool başına); joker karakterli "always-approved" asset yolları; kategori bazlı tool izinleri; değişiklikler **stage edilip tek "commit" adımında**, düz İngilizce özetle onaylanır; Git/Perforce yoksa girişte uyarı (geri alma = kaynak kontrol); plan paneli (kabul/ret/revize) + canlı to-do listesi; paralel sub-agent "carousel"; uzun dönem proje hafızası; sohbetten bug-report ekran görüntüsü; pause/resume.
**Kendi iddiası:** "Standart MCP kurulumundan 10x hızlı, 6x ucuz" — mekanizma "proje önceden sentezlenmiş", sayısal benchmark yok (https://ludusengine.com/blog/ludus-vs-mcp-speed-cost — iddia, 13 Eyl 2026).
**Şikâyetler (3. taraf):** "$5 plan 15.000 kredi ~2 saatte bitti, level/World Partition denemesi" (r/unrealengine "DO NOT GET LUDUS AI", https://www.reddit.com/r/unrealengine/comments/1i6t0t4/ — eski fiyat modeli dönemi); "projeyi değerlendirip öneri veriyor ama Blueprint üretmede yetersiz" (https://www.reddit.com/r/unrealengine/comments/1w6y6tn/); "geçen yıl çok kredi harcadım, sonuç alamadım" (https://www.reddit.com/r/unrealengine/comments/1vpn6wl/). Olumlu: forum showcase "denediğim tek tutarlı araç" (https://forums.unrealengine.com/t/ludus-ai-fab-plugin-recreation/2695009). Reddit gövdeleri arama özetinden okundu (doğrudan erişim 403).

---

## 2. Ramen VR — Aura

**Sürüm/bakım:** v1.2.5 (6 Kas 2026); sık sürüm: 30 Eki, 13 Eki, 6 Eki, 22/18/11/9/7/3/1 Eyl … (https://www.tryaura.dev/documentation/release-notes — doğrulandı). 15.x serisinden 1.0'a geçiş; 1.0 ile Mac + Unity desteği.
**Mimari:** Engine seviyesinde kurulan UE plugin'i + opsiyonel **standalone Electron app** (headless modda editörü kendisi başlatabilir) + bulut modeller ("Auto Mode" + premium frontier modeller). Unity'ye de uzanır (Coplay teknolojisi). Proje indeksleme (≤30.000 dosyada otomatik, `INDEX_IGNORE.txt`) (https://www.tryaura.dev/documentation/project-understanding). IDE/MCP entegrasyonu **Alpha**: Claude Code, Codex, VS, Cursor, Rider için tek tık; ücretli abonelik/deneme şart; MCP üzerinden çağrılan alt-ajanlar (Python Agent, Blueprint Agent) Aura kredisinden düşer; tek seferde tek proje (https://www.tryaura.dev/documentation/aura-ide-mcp — doğrulandı). Claude Code kurulum videosu 1:11 (aynı sayfa referans veriyor; video URL'si sayfadan çıkarılamadı).
**Fiyat:** yukarıdaki §0; kullanım token bazlı, Auto Mode haftalık/günlük limitli; **ücretsiz Auto Mode yalnız "Training Data" açıkken** (https://www.tryaura.dev/documentation/pricing-explained — doğrulandı; veri politikası açısından kritik). Overage yalnız Pro/Ultimate.
**UE/platform:** Win 5.4–5.8, macOS 5.7–5.8; özel engine sürümü için Fab "Source Access".

**Alanlar (https://www.tryaura.dev/documentation/<sayfa>):**
- Blueprint (`blueprints`): event/interface/timer/Timeline/trace/struct/enum/DataTable/**UMG/replication**; "yeni/niş sistemler tam desteklenmeyebilir". Kaynak kontrol + yedek önerisi.
- C++ coding agent (`coding-agent-cpp`); Python "Editor Agent" (`editor-agent`): toplu actor düzenleme.
- **Verification Agent** (`verification-agent`, doğrulandı): PIE başlatır, özelliği oynar, canlı durumu okur — Actor (spawn/konum/varlık), **GAS** (ability, attribute değerleri, aktif effect), **UMG** (widget görünürlük/değer), **video kaydı kanıt olarak**, **multiplayer** (çok client); hata görürse düzeltip yeniden doğrular. Plan modundaki planlar sıkça doğrulama adımıyla biter. "Material/mesh doğrulamada değeri az" diye kendisi yazıyor; normal sohbetten pahalı.
- **Autopilot** (açık kaynak, Apache-2.0, https://github.com/ramenvr/autopilot — 835★, son push 2026-11-10): oyun-genel gerçek zamanlı playtest ajanı; LLM + karar modeli, oyuna özel "harness"ı dinamik yazar, oyun başına neurosymbolic sürekli öğrenme; UE5/Unity 6/Godot 4, Win/macOS; RTS, FPS, Soulslike vb. "başarıyla oynadı" (https://www.tryaura.dev/updates/autopilot-continually-learning-real-time-general-playtesting-system, 31 Eki 2026 — genellik iddiası; white paper var). Plugin ile WebSocket köprüsü; coding agent olarak Claude Code/Codex SDK.
- Level Design Agent (`level-design-agent`): GeometryScript blockout → Blueprint'e çevirme, **Landscape kurulum + texture painting** (heightmap), **PCG ile scatter**; sınırlama: GeometryScript mesh'lerinde undo/redo tutarsız.
- Material Agent (`material-agent`): unlit/emissive/panning/su dalgası/decal, Substrate dahil (5.3–5.8).
- VFX agent (Niagara), Behavior Tree agent, Audio agent, Performance Profiling (`performance-profiling`: PIE'de capture, CPU/GPU/render-thread darboğazı analizi).
- Art (`art-tooling`): image gen, Art Direction kataloğu (`ArtDirection/` source control'e girer), model gen (yüksek detay ~50k tri, "Tripo tavanı ~1.5M" — **Tripo tabanlı** olduğu metinden çıkıyor), AutoUV/UV check (salt-okunur)/texture bake, 8K retexture, **rigged karakter üretimi**, **Animation Retargeting** (IK Rig + retargeter oluşturma, forward hizalama, toplu animasyon dönüştürme; karakteri değiştirip animasyonları korur), **Character Motion** text-to-motion ≤10 sn, loop.
- Skills (`aura-skills`): `Saved/.Aura/Skills/<ad>/SKILL.md`, **Claude Code SKILL.md ile birebir uyumlu**, slash komut + otomatik çağırma + "bu iş akışını skill yap".
- Custom sub-agent + workflow grafiği (fan-out, bağımlılık) (`custom-agents`).
- **Filesystem Sandbox** (`filesystem-sandbox`): UE 5.8'in deneysel sanal FS'i; tüm kayıtlar `Intermediate` altındaki sandbox ağacına, Accept ile asıl dosyalara yazılır, Reject geri almaya çalışır; P4/SVN checkout yalnız Accept anında. Deneysel.
- **Crash recovery** (`crash-recovery`): yalnız Windows + standalone app; crash raporunu okur, gerekirse incremental build / küçük C++ düzeltme, editörü önceki modda (tam/headless) açar, sandbox'ı geri açar, göreve devam eder.
- Slices (1.2.3–1.2.5): küçük oynanabilir prototip, C++ + BP subclass ya da BP olarak export; müzik üretimi 3 sn–5 dk (1.2.5).
- **Bulunamadı:** Sequencer/MRQ, Foliage, Water (sadece su *materyali*), StateTree, Control Rig, MetaHuman, CommonUI, Mutable, Physics Asset/socket, packaging, localization, Montage/BlendSpace.
**Şikâyetler (3. taraf, https://www.reddit.com/r/UnrealEngine5/comments/1ry2iq4/):** "~%30 vakada 'property'yi ayarladım' deyip hiçbir şey yapmadı"; "MCP yalnız sorgu açıyor, düzenleme tool'ları Aura kredisine kilitli"; "kredi = taban maliyet, token başına her zaman doğrudan modelden pahalı"; "deneme sürümü karakter BP'mi dağıttı"; "sağlık bileşeni bağlanmamış tellerle geldi"; olumlu: "proje bağlamını çok iyi anlıyor". Aura yanıtı: "Python execution gibi sorgu olmayan tool'lar var, güvenilirliğe odaklandık". Fiyat şikâyeti: "$160 token harcadım, temel BP'de bile dağınık" (https://www.reddit.com/r/UnrealEngine5/comments/1rr8be2/). Rakip benchmark'ında Aura 1/5 (NeoStack'in kendi testi, §3).

---

## 3. Betide Studio — NeoStack (eski: Agent Integration Kit)

**Sürüm/bakım:** Plugin **v4.0.17 (21 Eyl 2026)**, Desktop v3.1.73; changelog'da 76 sürüm girdisi (Plugin/Desktop/Mobile) (https://neostack.dev/changelog — doğrulandı). Eski 2.x buildleri Fab'da/ betide.studio'da "legacy" (https://betide.studio/plugins/agent-integration-kit).
**Mimari:** Geliştirici makinesinde editör plugin'i ("~60 entegrasyon modülü", 26 extension plugin, Lua binding'leri — docs llms-full v2.x notları); kendi **token-auth'lu in-editor MCP sunucusu**; aynı canlı oturuma **desktop/web/mobil** erişim, izin istemleri telefondan yanıtlanabilir; NeoStack Cloud (hosted model) veya BYO ajan: Claude Code, Codex, Copilot CLI, Gemini CLI, Cursor CLI, Grok, Antigravity, ACP uyumlu her ajan; OpenRouter/DeepSeek/Qwen/Groq/xAI/Vercel; yerel Ollama/LM Studio/vLLM/llama.cpp (https://neostack.dev/for-ai — doğrulandı). Epic'in 5.8 MCP'si ve **ToolsetRegistry köprüsü** (4.0.5).
**Fiyat:** $20/koltuk/ay (11–30 koltuk $19, 31+ $17), 3 gün deneme (kart gerekli), ücretsiz plan; Fab'da tek seferlik plugin lisansı (desktop/web/mobil/cloud hariç); eski lifetime lisanslar korunuyor (for-ai — doğrulandı).
**UE/platform:** UE 5.5–5.8; plugin Win+macOS (for-ai); docs changelog'unda "Linux plugin binaries UE 5.5–5.8" notu var → çelişkili, Linux **belirsiz**. UE6/Verse'e lansmanda destek sözü (iddia).
**Alanlar (doğrulandı = changelog/for-ai):** Blueprint, material, level, Niagara, UMG, **AnimBP** (Sequence/Blend Space Player düğümleri — 3.1.6), **Control Rig, IK Rig retargeting, Sequencer**, MetaHuman, PCG, Chaos, MetaSounds, **GAS**, Mass AI, LiveLink, DMX, nDisplay, C++ (for-ai listesi — liste düzeyi, derinlik görülmedi); **StateTree** şema değişiklikleri, **skeleton socket yazımı**, UE 5.8 HLOD (3.1.67, 1 Eyl); **katmanlı Landscape** (height/weightmap kontrolüyle), spline yol (3.1.67); **Mutable**, Nanite Assemblies, Dynamic Wind, Mover 2.0, UAF, Gameplay Cameras, **Movie Render Graph**, Interchange (4.0.5, 16 Eyl); Water ancak derleme düzeltmesi olarak geçiyor.
**Playtest/doğrulama (4.0.8, 16 Eyl, doğrulandı):** PIE'yi zaman damgalı still track + GPU encoder varsa H.264/265 MP4 olarak filme alma, isimli mark, input etrafında burst capture, PIE öncesi kayıt; "tek çağrıda playtest survey" (census, oyuncu/kamera, tehlike, yaklaşma noktası, **navmesh erişilebilirliği**, hedef ekranda mı); tick bazlı property/transform/velocity izleri; **UMG ve native Slate widget'larını seçiciyle sürme (click/type/drag/wait/assert, OS imleci yok)**; çalışan PIE'ye **geç katılan multiplayer client** ekleme, RPC callspace (3.1.67).
**Güvenlik/UX:** her düzenleme journal'lı, incelenebilir, **geri alınabilir**; geri döndürülemez işlemden önce izin; ajan asset düzenlemesinde source control checkout; editör kapanınca/crash'te **arka plan runtime turu sürdürür**, tarayıcıda yerel chat açar, editör geri gelince yeniden bağlanır (4.0.8); güncellemeler kirli paketleri bekler; ürün UI'ı 7 dilde (4.0.5 — oyun lokalizasyonu değil).
**Benchmark (kendi, 31 Tem 2026, UE 5.8, 5 görev, tek koşu):** NeoStack 3/5, Epic MCP 1/5, Aura 1/5, UEC 0/5 (448 istek), NWIRO 0/5; geçme koşulu "kaydedilmiş durum + validation + runtime kanıtı + görsel inceleme" (https://neostack.dev/benchmark — **iddia**: satıcının kendi testi, n=1).
**3. taraf:** Fab puanı 4.4 (113), listing "beta" diyor (https://cosindra.ai/compare/fab-ai-plugins-compared — rakip firma yazısı, 2 Kas 2026). İnceleme videosu: https://www.youtube.com/watch?v=04GuUbWUgZs (9 Şub 2026, 16:25; arena shooter şablonunda respawn/medkit/AI değişikliği). Kamuya açık ciddi şikâyet thread'i **bulunamadı**; r/unrealengine "UE-MCP 1.3.9" yorumunda "Aura ve NeoStack kutudan çıktığı haliyle üstün" (https://www.reddit.com/r/unrealengine/comments/1womszc/).

---

## 4. "CoPilot" tarzı asistanlar

### 4a. Ultimate Engine CoPilot (UEC) — GameDev Core / BlueprintsLab
**Sürüm/bakım:** V2.4.0 (3 Kas 2026); V2.0.0 (30 Eyl 2026, "en büyük güncelleme", 1.655 tool, arayüz baştan); 1–2 haftada bir sürüm (https://www.gamedevcore.com/docs/updates — doğrulandı). **Sürüm bölünmesi:** V2.4.0 yalnız Win UE 5.7/5.8; Win 5.6 → V2.3.5'te; Mac, Linux ve Win 5.5 → V2.3.0'da kaldı.
**Mimari:** Yerel editör plugin'i; arayüz rayı: Architect (sohbet), **Crew** (çoklu worker + reviewer, Full Auto'da kendiliğinden yeniden dener), Tools, Dashboard, Local Studio, Cloud Studio, Source Control, MCP, C++ Classes, GDD penceresi; BYO anahtar veya yerel LLM, ya da token'lı abonelik. Tam dokümantasyon **lisans arkasında** (https://www.gamedevcore.com/docs/play-testing).
**Fiyat:** $284.99/koltuk tek seferlik (BYO AI); Indie $25/ay (50M token/ay); Studio $80/ay (150M); Teams özel; sahiplere token eklentisi $20/ay (https://www.gamedevcore.com/pricing — doğrulandı).
**Alanlar (V2.0.0 changelog, doğrulandı, liste düzeyi):** Landscape sculpt/paint (katmanlar restart sonrası kalıcı), **Localization** (target, dil, gather/compile/export, ilerleme), **MetaHuman** (5.6+: cilt/çil, build, önizleme spawn), **Mutable** (parametre/instance/graph/compile), **Packaging** (arka planda cook/package/archive, ilerleme+iptal, light build, cihazda başlatma), Physics Asset (bone body, primitive, constraint limit), Physics Control, socket'leri farklı kemik adlı iskeletler arası taşıma, Retarget Studio paneli, Skeletal Mesh Merger, Play Montage düğümü, Blend Space Evaluator vb. AnimGraph düğümleri, Control Rig (bake/collapse/tween), StateTree/EQS, GAS (Gameplay Effect bileşenleri), Water (Gerstner, spline), Foliage, CommonUI, Groom, Mass, Vehicles, Chooser, Geometry Script, Level Snapshots, Interchange (USD/glTF/MaterialX/FBX), Blueprint debugging (breakpoint, pin watch), unused asset/dependency cycle tespiti.
**Playtest/QA:** AI PIE başlatır, oyuncuyu hareket ettirir, tuşa basar, **kareleri görerek** PASS/FAIL verir (https://www.gamedevcore.com/docs/play-testing); **listen/dedicated server + 8 client'a kadar** multiplayer test, replication denetimi, client'lar arası değer karşılaştırma, ağ istatistikleri, replay; HUD dahil video kaydı (**ffmpeg gerekli**, ayrı pencere); **Unreal Insights trace** başlat/durdur + en yavaş kare ve sorumlu BP; CPU/GPU bound anlık görüntü; kamera çekimi analizleri (V2.0.0).
**Güvenlik/UX:** V2.0.0'dan itibaren AI değişiklikleri **Ctrl+Z ile geri alınabilir** (level, Sequencer, Niagara, anim, audio, material, widget, GAS) — "1.7.3'te çoğunda undo yoktu"; neyin sorulacağı için ayrı anahtarlar, screenshot öncesi izin (V2.3.0); `delete_asset` açık level'i reddeder ve yedekler (V2.4.0); bağlam uzunluğu uyarısı; Git commit/branch/shelve sorarak; Python AI scriptleri.
**Kalite sinyali (kendi changelog'u, doğrulandı):** V2.0.0: "~90 tool, kodun hiç okumadığı parametre adlarını ilan ediyordu", "düzinelerce tool hiçbir şey yapmadan başarı döndü ya da typo'da sessizce default kullandı (BT wait = 0, ışık Static)". Bu, Enginewright'ın "bilinmeyen anahtarı reddet" ilkesinin pazardaki karşılığını doğrudan kanıtlıyor.
**Şikâyetler (3. taraf):** "pahalı ve token verimli değil" (https://www.reddit.com/r/unrealengine/comments/1womszc/ — UE-MCP thread'i, taraflı bağlam); NeoStack benchmark'ında 0/5 + 448 istek (iddia). Olumlu: "kaynak kodu var, yerel LLM ile çalışıyor, kendim değiştirebiliyorum" (https://www.reddit.com/r/unrealengine/comments/1r96tsx/).

### 4b. GitHubCopilotUE (ULT7RA/GitHubCoPilotUNREALEngine)
Native C++ plugin, dockable Slate paneli + konsol REPL; GitHub Copilot aboneliğiyle cihaz-akışı OAuth, ardından `copilot_internal/v2/token` takası (resmî olmayan uç nokta — ToS/kırılganlık riski, README'den gözlem); **22 tool** (README; Mart 2026 Reddit postunda 19): dosya view/glob/rg/read/write/edit/move/delete, `get_project_structure`, `create_cpp_class`, `create_blueprint_asset`, `compile`, `live_coding_patch`, `run_automation_tests`; patch önizleme/uygula/**rollback**, izinli yazma kökleri, yedekli düzenleme. UE 5.3+ (5.6/5.7 test), Windows; MCP **bulunamadı**. 22★, son push 2026-05-29, GitHub API lisans "None" ama README "open source" diyor → lisans belirsiz (https://github.com/ULT7RA/GitHubCoPilotUNREALEngine, https://www.reddit.com/r/unrealengine/comments/1rwcst6/). Reddit'te "VS/Rider'daki resmî Copilot'tan farkı ne?" sorusu (aynı thread). Derinlik: dosya/C++ odaklı; asset/graph düzeyi sığ.
**atgoldberg/UnrealCopilot:** UE 5.6+ "sanal teknik artist", 10★, son push 2025-09-29 → bakımsız (GitHub API).

### 4c. Diğer ticari girişler (yalnız 3. taraf kaynak; doğrudan doğrulanmadı)
Cosindra karşılaştırması (rakip yazısı, 2 Kas 2026, https://cosindra.ai/compare/fab-ai-plugins-compared): **NWIRO AI Integration Kit** (Leartes, tek seferlik, UE 5.5+, Fab 4.7/161) ve NWIRO Pro (kredili); **Hyper AI Integration Studio** (ücretsiz, UE 5.8, Epic'in Unreal MCP'sini kurar, üstüne "40 toolset'te 109 tool"); Cosindra (desktop + tarayıcı workspace). Bunlar 1.0 kararında izlenmeli; özellikle Hyper AI = Epic MCP üstüne katman, Enginewright ile aynı kategori.

---

## 5. Unreal AI 3D üretim eklentileri

| Ürün | UE / platform | Mimari | Özellik | Fiyat | Kaynak |
|---|---|---|---|---|---|
| **Tripo API plugin** | v1.0, UE 5.6.0 | Tripo API'ye bağlanan editör + **Blueprint** iş akışı | text/image→3D (API) | API kredisi; plugin ücretsiz indirme | https://developers.tripo3d.ai/en/docs/plugins (doğrulandı) |
| **Tripo DCC Bridge** | Windows, UE 5.4+ (5.7+ önerilir), Chrome/Edge 116+ | Tarayıcıdaki Tripo Studio'dan canlı gönderim, auto asset placement | Studio'da üretilen animasyonları da gönderir | Tripo aboneliği (plugin ücretsiz) | https://www.tripo3d.ai/blog/tripo-dcc-bridge-for-ue (27 Eki 2026) |
| **Meshy for UE** | UE 5.4–5.8 (integrasyon sayfası); docs: Windows için 5.4/5.5/5.6/5.7 ayrı build, macOS için kaynak yolu | Ücretsiz plugin, web app'ten **DCC Bridge** ile tek tık; engine ya da proje seviyesinde | Animasyonlu model (tam anim verisi) | Meshy kredisi | https://www.meshy.ai/integrations/unreal-engine, https://docs.meshy.ai/en/webapp/plugins/unreal/introduction |
| **Muddy Terrain GenAI Model Generator** | UE 5.1–5.7, Win/macOS/Linux | Fab plugin; Slate widget (kodsuz) + **runtime** async Blueprint düğümleri/C++ | 7 sağlayıcı: Meshy-6, Tripo v3.1, fal.ai (Hunyuan3D 3.1/2.1, TripoSR, Rodin Gen-2, Trellis 2), Google; text/image→3D, auto-rig (deneysel, Meshy), remesh, retexture, Smart LowPoly, PBR texture | Plugin fiyatı **bulunamadı**; BYO API anahtarı (~$0.07–0.60/üretim, Mart 2026) | https://muddyterrain.com/genai-model-generator, https://muddyterrain.com/docs/genai-modelgenerator/ |

Ortak gözlem: hiçbiri MCP/ajan yüzeyi sunmuyor; hepsi "import köprüsü". Import sonrası doğrulama (ölçek, pivot, LOD, collision, materyal slotu, tri bütçesi) yok → Enginewright 0.4.1 art pipeline'ı (`asset_import` + `mesh_info` + `class_measure` + bütçe kontrolü) bu köprülerin **arkasına** oturabilir.

---

## 6. MCP plugin'imizin chat ürününe dönüşmeden öğrenebilecekleri

1. **"Kanıtlı bitti" sözleşmesi** (Aura Verification, NeoStack benchmark kriteri, UEC PASS/FAIL): tool sonuçlarında `evidence` alanı standardı (read-back değer, screenshot yolu, log satırı). Biz zaten read-back yapıyoruz; bunu her yazma tool'unda tek şemaya bağlamak 1.0 API freeze'ine girmeli.
2. **Playtest kaydı**: NeoStack 4.0.8'in still-track + mark + tick bazlı property izi, UEC'nin video + Insights trace'i. Bizde `screenshot`, `screenshot_ab`, `pie_walk_to`, `pie_actors` var; eksik: zaman serisi izi (`pie_trace` gibi) ve PIE kaydı. Video yerine PNG dizisi + `image_compose` bizim felsefeye uygun (ffmpeg bağımlılığı yok).
3. **Onay modları ama sunucu tarafında**: Ludus Default/Ask/Yolo/Custom + joker yollar; UEC kategori anahtarları. Bizde read-only mode + kategori kapatma var; eksik: **yol bazlı yazma izin listesi** (`/Game/Sandbox/**`) — chat UI gerektirmez, panel + settings yeterli.
4. **Sandbox/geri alma**: Aura UE 5.8 Filesystem Sandbox'ı kullanıyor; NeoStack journal; UEC Ctrl+Z. Bizde `batch` atomik rollback + tek undo adımı var; 1.0'da her mutasyonun transaction'a girdiğini `doctor`/test ile garanti etmek ve 5.8'de sandbox'ı opsiyonel olarak açan bir tool düşünülebilir.
5. **Editör çökmesine dayanıklılık**: Aura crash recovery, NeoStack arka plan runtime. MCP sunucusu editör içinde yaşadığı için biz ölürüz; `ewtool.py` zaten editör dışı → 1.0 headless hedefi buna "editor_launch/crash_report_read" eklemeli.
6. **Skill dosyası**: Aura'nın SKILL.md'si Claude Code uyumlu. 1.0 roadmap'indeki "agent skill" doğru; Aura ile aynı formatta yayınlamak ajanların her iki tarafta da kullanmasını sağlar.
7. **Sürüm kapsamı dürüstlüğü**: UEC'nin platform başına farklı sürümde kalması kullanıcıyı bölüyor; NeoStack "version-gated per engine". 1.0'da 5.4–5.8 hedefi için tool başına `minEngine` meta verisi + `describe` çıktısında göstermek.
8. **Görev devri vs granüler tool**: Ludus MCP'si yalnız `send_message/get_thread` — dış ajan kör kalıyor ve Ludus kredisi yakıyor; Aura MCP'si de alt-ajanları kendi kredisinden düşüyor. Granüler, ücretsiz, yerel tool yüzeyi bizim ana farkımız; pazarlamada açıkça söylenmeli.

---

## 7. Özet tablo

Derinlik: **D** = derin (çok alt özellik/doğrulama), **O** = orta, **S** = sığ/liste düzeyi, **?** = yalnız liste/iddia. Kısaltma: Lu=Ludus, Au=Aura, Neo=NeoStack, UEC=Ultimate Engine CoPilot, GHC=GitHubCopilotUE, 3D=Tripo/Meshy/Muddy.

| Özellik | Kimde var (derinlik) | Enginewright? | Not |
|---|---|---|---|
| Skeletal import / skeleton | Lu auto-rig (O, humanoid), Au rigged gen (O), UEC skel merger (?), Muddy auto-rig (S) | Kısmi (`asset_import` mesh, `meshType`) | 0.4.1 skeleton bind planlı |
| Socket | Neo (O, 3.1.67), UEC socket taşıma (?) | Yok | 0.4.1 |
| Physics Asset | UEC (O) | Yok | 0.4.1 |
| IK retarget | Au (D: IK Rig+retargeter+batch), Lu (O, Mixamo→Manny), Neo (?), UEC Retarget Studio (?) | Yok | 0.4.1 |
| AnimBP authoring | Lu (O: state machine+transition), Neo (O), UEC (O: çok AnimGraph düğümü) | Yok | 0.5 |
| Montage / BlendSpace | UEC (S: Play Montage düğümü, BS evaluator), Neo (S: BS Player fix) | Yok | 0.4.1 import ile |
| Mutable | UEC (O), Neo (S, 4.0.5) | Yok | 0.5 |
| Niagara | Lu (D: sistem kurma), Au VFX agent (O), Neo (?), UEC (O) | Yok | 1.0.0 |
| Materials | Lu (D), Au (D, Substrate), Neo, UEC | Var (O: create/expr/connect/MI) | |
| Sequencer / MRQ | Neo (? MRG), UEC (S: undo + sequence frame) | Yok | 1.0.0 |
| PCG | Lu (O), Au (O: scatter), Neo (?) | Yok (`scene_dress` alternatif) | |
| Landscape / Foliage | Au (O: setup+paint), UEC (O: sculpt/paint; foliage ?), Neo (O: layered) | Yok | |
| Water | UEC (S), Au (su materyali) | Yok | |
| BT / StateTree | Lu BT+EQS (O), Au BT agent (O), Neo StateTree (O), UEC (O) | Yok | 0.5 |
| GAS | Au doğrulamada okuma (O), UEC GE authoring (O), Neo (?) | Yok (gameplay tags var) | |
| Control Rig | Lu (O: hiyerarşi), UEC (O), Neo (?) | Yok | |
| MetaHuman | Lu (S: leaf ayar), UEC (S), Neo (?) | Yok | |
| UMG / CommonUI | Lu (S: widget graph), Au (S), Neo (O: Slate/UMG sürme), UEC (O: widget anim) | **Var (D)**: widget_*, ui_state/click/prompts/input_audit, MVVM | |
| PIE + input | Au (D), Neo (D), UEC (D) | **Var (D)**: pie_*, pie_walk_to, ui_click | |
| Multiplayer test | UEC (D: 8 client, ded./listen), Au (O), Neo (O: late join) | **Var (O)**: pie_start 1–4, pie_actors, pie_net_emulation | Ded. server yok |
| Packaging | UEC (O: cook/package/archive/launch) | Kısmi (`game_run` paketli exe çalıştırır, paketlemez) | |
| Profiling | UEC (O: Insights), Au (O: PIE capture) | Yok | |
| Headless | Au (standalone headless + crash recovery), Neo (arka plan runtime) | Kısmi (`ewtool.py build`) | 1.0.0 |
| Python / C++ exec | Lu (Python+C++/Live Coding), Au (Python agent), UEC (Python scripts), GHC (C++/compile) | C++: `live_coding_compile`, `editor_restart build`; Python **yok** (`console_command` var) | Bilinçli tercih mi, 1.0'da karar |
| Visual QA | Au (video), Neo (still track+MP4), UEC (frame okuma, video) | **Var (D)**: screenshot_ab, image_stats, widget_render, actor_preview, screen_coverage | |
| Otonom playtest ajanı | Au Verification + Autopilot (D, açık kaynak), UEC Crew+PASS/FAIL (O), Neo survey (O) | Tool seviyesinde (ajan kullanıcıda) | Ürün kararı: ajan değil tool |
| Localization (oyun) | UEC (O: pipeline) | **Var (D)**: loc_gather, stringtable_compare po/locres, font_coverage, glyphMisses, culture | |
| Source control | UEC (O: commit/branch/shelve), Neo (checkout), Au sandbox+P4, Lu (uyarı) | Yok | |
| Metadata / tags | — (bulunamadı) | Var: gameplay_tag_*, asset_references | |
| Validation | UEC (unused/dep cycle), Neo (benchmark validation) | **Var (D)**: validate, id_audit, key_audit | |
| Sandbox / undo | Au FS Sandbox (deneysel), Neo journal+undo, UEC Ctrl+Z, Lu staged commit, GHC rollback | Var: batch atomik rollback, tek undo, read-only mode | Yol bazlı izin yok |
| Çoklu UE sürümü | Lu 5.4–5.8, Au 5.4–5.8, Neo 5.5–5.8, UEC 5.5–5.8 (bölünmüş) | Yalnız 5.8 Win64 | 1.0.0: 5.4–5.8 |
| UX: chat paneli/onay | Hepsinde chat; Lu 4 onay modu, Neo mobil onay | Panel (durum, token, son çağrılar), chat yok | Bilinçli |
| Ajan-agnostik MCP | Neo (D: BYO + kendi MCP), UEC (MCP sekmesi), Au (Alpha, ücretli), Lu (görev devri, 6 tool) | **Var (D)**: yerel, token, 5 istemci config | |
| 3D asset üretimi | Lu, Au (Tripo tabanlı), Muddy/Tripo/Meshy | Yok | Kapsam dışı; import doğrulama ile tamamlayıcı |
| Fiyat | Lu $25–180/ay, Au $10–200/ay, Neo $20/ay veya Fab tek sefer, UEC $284.99 veya $25–80/ay | MIT, ücretsiz | |

---

## 8. Bizde olup onlarda olmayan (okunan resmî kaynaklarda **bulunamadı**)

- **Hata-öncelikli tool sözleşmesi**: bilinmeyen anahtar/typo reddi, `dryRun`, per-asset ret nedeni (`asset_delete`). UEC'nin kendi changelog'u (~90 tool parametre okumuyordu, sessiz başarı) ve Aura kullanıcı şikâyeti (%30 "yaptım" deyip yapmama) bu açığı kanıtlıyor.
- **`ui_click` COVERED tespiti**, `ui_prompts` (ekrandaki ipucu ↔ gerçek input binding, input tipine göre), `ui_input_audit` (CommonUI input wiring denetimi), `ui_input_type` (gamepad/touch glyph kontrolü).
- **Lokalizasyon QA derinliği**: `stringtable_compare` (.po/.locres/CSV, format argümanı/rich text farkı, eski kaynak metne çeviri, fuzzy), `font_coverage` (çizilemeyen karakter), `game_run` `glyphMisses` sayımı, `-culture` ile koşu. UEC yalnız pipeline'ı çalıştırıyor.
- **`savegame_get/set`**: reflection ile slot okuma/yazma, oyun checksum/footer'ını bozmayı reddetme.
- **`game_run`**: `-game` veya **paketlenmiş exe**'yi map URL'si, çoklu çözünürlük, `cvars`/`variants`, log-pattern verdict ile koşturma; Live Coding sonrası eski DLL ile başlatmayı reddetme.
- **Piksel düzeyi görsel ölçüm**: `screenshot_ab` (değişen piksel oranı), `image_stats` (luma histogramı, clipping, referans farkı), `screen_coverage` (aktörün ekran payı), `widget_render`/`actor_preview` (PIE'siz PNG).
- **`pie_net_emulation`** (lag/loss/dup/reorder profilleri) — rakiplerde ağ *istatistiği* var, emülasyon bulunamadı.
- **`id_audit` / `key_audit`** (tanımsız ID'ler, hard-coded input tuşları), `asset_rename` sonrası metinde kalan eski ad raporu.
- **`doctor`** (sağlık + her sorunun düzeltmesi), `client_config` (5 istemci için hazır config), editör içi panelde son çağrılar.
- **Kredi/bulut yok, MIT, yerel token**: proje verisi makineden çıkmaz; Aura'nın ücretsiz katmanı "Training Data" açık olmasını şart koşuyor, Ludus/Aura MCP'leri kendi kredisini harcıyor.
- **Granüler tool'ları her ajana açma**: Ludus MCP 6 görev-devri tool'u, Aura IDE MCP Alpha + ücretli; bizde 107 tool `search/describe/call` ile doğrudan.

**Bizde eksik olup 1.0 için rakiplerin standart saydığı:** Python exec (Lu/Au/UEC), Niagara, AnimBP/retarget/socket/Physics Asset, BT/StateTree, Landscape/PCG, profiling (Insights), packaging tool'u, source control, çoklu UE sürümü, editör çökmesine dayanıklılık/headless.

## Kaynak erişim notları
- Reddit doğrudan 403 verdi; gövdeler web_search özetleri ve web_extract (yalnız 1ry2iq4, 1rr8be2, 1rwcst6 tam okundu) ile alındı.
- UEC tam dokümanı lisans arkasında; UEC iddiaları changelog'a dayanıyor (liste düzeyi, derinlik doğrulanmadı).
- Ludus changelog/sürüm bulunamadı; Muddy Fab fiyatı bulunamadı; Aura kurulum videosu URL'si çıkarılamadı.
- Ham sayfa metinleri: `C:\Users\shanyup\AppData\Local\hermes\cache\scratch\ticari\` (24 saat sonra silinir).
