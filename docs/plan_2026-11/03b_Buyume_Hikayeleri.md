# 03b — Emsal Projelerin Yıldız Büyüme Hikâyeleri ve Duyuru Kanalları

Tarih: 17.11.2026. Kapsam: 8 emsal MCP projesi + kanal envanteri. **Yalnız belge; hiçbir yerde paylaşım yapılmadı.**

## Yöntem ve veri notu

| Konu | Açıklama |
|---|---|
| Güncel ★ / fork / tarih | GitHub REST `/repos/{owner}/{repo}`, 17.11.2026. Ham veri: [`research/raw/emsal_meta_2026-11-17.json`](research/raw/emsal_meta_2026-11-17.json) |
| Büyüme eğrisi | star-history.com SVG çıktısı (`api.star-history.com/svg?repos=…&type=Date`). Eğri düğümleri eksen etiketlerinden sayıya çevrildi. Ham veri: [`research/raw/star_curves_2026-11-17.json`](research/raw/star_curves_2026-11-17.json) |
| Eğri hassasiyeti | Örnekleme aralığı ~4–5 hafta. Okuma hatası **±%3 (tahmin)**. Bu yüzden eğri değerleri `~` ile yazıldı. Ara gün tarihleri (ör. "10k'ya 46. gün") interpolasyondur, kesin değildir. |
| Neden star-history? | GitHub stargazer zaman damgası uç noktası 17.11.2026'da başka repolar için 404 döndü (yalnız kendi repomuzda çalıştı). GraphQL `starredAt` boş döndü. |
| HN verisi | hn.algolia.com API, 17.11.2026. Ham liste: [`research/raw/hn_algolia_2026-11-17.txt`](research/raw/hn_algolia_2026-11-17.txt) |
| Reddit | Reddit API ve kurallar sayfası bu makineden 403 verdi. Reddit kuralları ikincil kaynaklardan alındı; hepsi linkli. Paylaşımdan önce sidebar elle okunmalı. |
| "tahmin" | Kaynakla doğrulanmamış neden-sonuç yorumu ya da sayı. |

Star-history sayfaları: [blender](https://www.star-history.com/#ahujasid/mcp-for-blender&Date) · [playwright](https://www.star-history.com/#microsoft/playwright-mcp&Date) · [unity](https://www.star-history.com/#CoplayDev/unity-mcp&Date) · [godot](https://www.star-history.com/#Coding-Solo/godot-mcp&Date) · [chongdashu](https://www.star-history.com/#chongdashu/unreal-mcp&Date) · [flopperam](https://www.star-history.com/#flopperam/unreal-engine-mcp&Date) · [runreal](https://www.star-history.com/#runreal/unreal-mcp&Date) · [chrome-devtools](https://www.star-history.com/#ChromeDevTools/chrome-devtools-mcp&Date)

---

## 0. Beş cümlelik sonuç

1. Büyük patlamaların hepsi **ilk 5 haftada** oldu. Sonra eğri yavaş ama düzenli büyüdü ([star-history](https://www.star-history.com/#ahujasid/mcp-for-blender&Date)).
2. Patlamanın ateşleyicisi **X'te 30–60 sn'lik demo videosu** ya da **büyük bir kurumun resmi duyurusu**. HN çoğu projede ana neden değil, yan etki.
3. Oyun motoru MCP'lerinde en güçlü sürekli kanal **YouTube eğitimleri** (yazarın kendi kanalı + üçüncü kişilerin kurulum videoları).
4. Tek komut kurulum, GIF ve dizin kaydı **gerekli ama yetmez**: runreal hepsine sahip, 121★'da kaldı ([GitHub](https://github.com/runreal/unreal-mcp)).
5. **UE 5.8 resmi MCP'si (Haziran 2026) sonrası topluluk UE MCP'lerinin eğrisi düzleşti.** Enginewright "Epic'in yapmadığı işi" anlatmazsa aynı tavana çarpar.

---

## 1. Proje tablosu

| Proje | ★ (17.11.2026) | Açılış | ~30. gün ★ | ~365. gün ★ | Ana tetikleyici (link) | Bugünkü durum |
|---|---:|---|---:|---:|---|---|
| **ahujasid/mcp-for-blender** (eski `blender-mcp`) | 34.800 ([API](https://github.com/ahujasid/mcp-for-blender)) | 07.03.2025 | ~8.000 | ~17.400 | Kurucunun X demo videosu, 11.03.2025 ([X alıntısı](https://x.com/sidahuj/status/1899828297346974060), [Cline blog](https://cline.bot/blog/human-intent-in-the-age-of-ai-insights-from-our-chat-with-siddharth-ahuja-creator-of-the-ableton-blender-mcps)) | 2026 sonbaharında 2. dalga: ayda +3.6k / +4.3k |
| **microsoft/playwright-mcp** | 39.799 ([API](https://github.com/microsoft/playwright-mcp)) | 21.03.2025 | ~7.300 | ~28.900 | Microsoft markası + Simon Willison yazısı 25.03.2025 ([blog](https://simonwillison.net/2025/Mar/25/playwright-mcp/)) | Copilot cloud agent'ta varsayılan MCP ([GitHub Docs](https://docs.github.com/en/copilot/concepts/coding-agent/mcp-and-coding-agent)) |
| **CoplayDev/unity-mcp** | 16.037 ([API](https://github.com/CoplayDev/unity-mcp)) | 18.03.2025 | ~1.450 | ~8.300 | Justin Barnett'in "Mario klonu tek prompt" demo tweet'i, 18.03.2025 ([X](https://x.com/JustinPBarnett/status/1901957423851557035)) | Coplay satın aldı (Ağu 2025), Aura satın aldı (Mar 2026) ([BusinessWire/Fidelity](https://www.fidelity.com/news/article/mergers-and-acquisitions/202606261000BIZWIRE_USPR_____20260626_BW815100)) |
| **Coding-Solo/godot-mcp** | 6.688 ([API](https://github.com/Coding-Solo/godot-mcp)) | 26.02.2025 | ~140 | ~2.000 | awesome-mcp-servers kaydı, 20.03.2025 ([commit](https://github.com/punkpeye/awesome-mcp-servers/commit/c96871f32c34289081eda8dd8036e605182ab68c)) | Son push 16.04.2026; yine de ayda ~+550–700 |
| **chongdashu/unreal-mcp** | 2.119 ([API](https://github.com/chongdashu/unreal-mcp)) | 28.03.2025 | ~460 | ~1.600 | Yazarın YouTube kanalı "AI Oriented Dev" demo videosu, 04.04.2025 ([YouTube](https://www.youtube.com/watch?v=ei2gulox9Yo)) | Son push 22.04.2025; Haz 2026'dan beri ayda ~+30 |
| **flopperam/unreal-engine-mcp** | 1.119 ([API](https://github.com/flopperam/unreal-engine-mcp)) | 06.08.2025 | ~40 | ~1.050 | YouTube @flopperam (9k takipçi) + Shorts ([LinkedIn](https://www.linkedin.com/in/ryan-thomas-uri), [kurulum videosu](https://youtu.be/ct5dNJC-Hx4)) | Aura satın aldı (Haz 2026); README reklama döndü; ayda ~+7–34 |
| **runreal/unreal-mcp** | 121 ([API](https://github.com/runreal/unreal-mcp)) | 28.05.2025 | ~15 | ~100 | r/mcp gönderisi, 14 oy ([Reddit](https://www.reddit.com/r/mcp/comments/1l38z35/unreal_engine_mcp_control_your_editor_from_claude/)) | Son push 06.06.2025; durgun |
| **ChromeDevTools/chrome-devtools-mcp** | 58.007 ([API](https://github.com/ChromeDevTools/chrome-devtools-mcp)) | 11.09.2025 | ~9.900 | ~51.000 | Google resmi blog + @ChromiumDev tweet'i, 23.09.2025 ([blog](https://developer.chrome.com/blog/chrome-devtools-mcp/), [X](https://x.com/ChromiumDev/status/1970505063064825994)) | HN 604 puan, 15.03.2026 ([HN](https://news.ycombinator.com/item?id=47390817)) sonrası 2. dalga |
| *shanyup/enginewright* | *10* ([API](https://github.com/shanyup/enginewright)) | *07.11.2026* | — | — | — | Henüz duyuru yok |

30./365. gün değerleri: star-history eğrisinden interpolasyon (bkz. yöntem notu).

---

## 2. Etken matrisi — neden yıldız aldı?

✅ = var ve linkli kanıt var. 🟡 = var ama etkisi küçük/dolaylı. ❌ = yok/bulunamadı. Hücredeki link kanıttır.

| Etken | blender | playwright | unity | godot | chongdashu | flopperam | runreal | chrome-devtools |
|---|---|---|---|---|---|---|---|---|
| Demo videosu/GIF (README veya X) | ✅ 6 YouTube + X videoları ([README](https://github.com/ahujasid/mcp-for-blender)) | ❌ README'de medya yok | ✅ `building_scene.gif` + X videosu ([X](https://x.com/JustinPBarnett/status/1901957423851557035)) | ❌ README'de medya yok | ✅ X videosu 24.03.2025 ([X](https://x.com/chongdashu/status/1903982123402318294)) + YouTube | ✅ YouTube + 3 Shorts ([README](https://github.com/flopperam/unreal-engine-mcp)) | 🟡 README GIF ([mcp.gif](https://github.com/runreal/unreal-mcp)) | ✅ duyuru videosu ([X](https://x.com/ChromiumDev/status/1970505063064825994)) |
| Tek komutla kurulum | ✅ `uvx mcp-for-blender setup` ([varg.ai](https://varg.ai/how-to/skills/blender-mcp)) | ✅ `npx @playwright/mcp@latest` ([Willison](https://simonwillison.net/2025/Mar/25/playwright-mcp/)) | 🟡 Git URL + "Configure All Clients" (bkz. [01 §2](01_Rakip_ve_Emsal_Karsilastirma.md)) | ✅ `claude mcp add godot -- npx @coding-solo/godot-mcp` ([README](https://github.com/Coding-Solo/godot-mcp)) | ❌ VS build + manuel config ([kurulum videosu](https://www.youtube.com/watch?v=pt52lUF-mlQ)) | ❌ clone + Python 3.12 | ✅ `npx -y @runreal/unreal-mcp` ([README](https://github.com/runreal/unreal-mcp)) | ✅ npx |
| Viral tweet | ✅ 11.03.2025 dragon demo ([X](https://x.com/sidahuj/status/1899828297346974060)) | 🟡 (Microsoft hesapları, tahmin) | ✅ 18.03.2025 ([X](https://x.com/JustinPBarnett/status/1901957423851557035)) | ❌ bulunamadı | 🟡 24.03.2025 ([X](https://x.com/chongdashu/status/1903982123402318294)) | ❌ bulunamadı | ❌ | ✅ ([X](https://x.com/ChromiumDev/status/1970505063064825994)) |
| HN gönderisi | 🟡 54 puan, 13.03.2025 ([HN](https://news.ycombinator.com/item?id=43357112)) | 🟡 6 puan, 24.03.2025 ([HN](https://news.ycombinator.com/item?id=43465931)) | 🟡 Coplay satın alma yazısı 64 puan ([HN](https://news.ycombinator.com/item?id=44875440)) | ❌ | ❌ | ❌ | ❌ | ✅ 604 puan, 15.03.2026 ([HN](https://news.ycombinator.com/item?id=47390817)); lansman 19 puan ([HN](https://news.ycombinator.com/item?id=45349829)) |
| Reddit gönderisi | 🟡 r/ClaudeAI kullanıcı paylaşımları ([örnek](https://www.reddit.com/r/ClaudeAI/comments/1xmcwbt/opus_55_medium_with_blender_mcp_is_insane_with/)) | ❌ bulunamadı | 🟡 r/cursor, r/Unity2D'de öneri olarak ([r/cursor](https://www.reddit.com/r/cursor/comments/1jgv4aa/will_you_guys_have_any_unity_support_or_is_there/)) | 🟡 r/godot'da "en popüler" diye anılıyor ([r/godot](https://www.reddit.com/r/godot/comments/1xtoenl/no_official_godot_mcp_server_exists_how_the/)) | 🟡 r/unrealengine fork'u bunu temel aldı ([r/unrealengine](https://www.reddit.com/r/unrealengine/comments/1tijkka/opensourced_unreal_mcp_88_tools_to_control_ue5/)) | ❌ | 🟡 r/mcp 14 oy ([Reddit](https://www.reddit.com/r/mcp/comments/1l38z35/unreal_engine_mcp_control_your_editor_from_claude/)) | ❌ |
| awesome-mcp-servers / liste | ✅ punkpeye listesinde ([commit 20.05.2025](https://github.com/punkpeye/awesome-mcp-servers/commit/a5d172fe139c16b10ad9012b62ead7ed66e80bd6)) | ✅ wong2 listesi ([liste](https://github.com/wong2/awesome-mcp-servers)) | ❌ punkpeye/wong2'de yok (17.11 README taraması) | ✅ punkpeye PR #329 ([commit](https://github.com/punkpeye/awesome-mcp-servers/commit/c96871f32c34289081eda8dd8036e605182ab68c)) + wong2 | ❌ | ❌ | ✅ Glama rozeti ([README](https://github.com/runreal/unreal-mcp)) | ❌ listelerde yok (ihtiyacı yok) |
| Ünlü/kurum paylaşımı | ✅ Cline blog röportajı ([Cline](https://cline.bot/blog/human-intent-in-the-age-of-ai-insights-from-our-chat-with-siddharth-ahuja-creator-of-the-ableton-blender-mcps)), CreativeBloq haberi ([CB](https://www.creativebloq.com/3d/vibe-rendering-has-arrived-this-plugin-allows-ai-to-control-blender)), AI Engineer World's Fair konuşması ([talk](https://ai.engineer/talks/nnktgWtfJHE-blender-mcp-future-creative-tools.md)) | ✅ Simon Willison ([blog](https://simonwillison.net/2025/Mar/25/playwright-mcp/)) | ✅ ACM SIGGRAPH Asia '25 makalesi ([DOI](https://doi.org/10.1145/3757376.3771417)) | ❌ | 🟡 üçüncü kişi YouTube eğitimleri ([George](https://www.youtube.com/watch?v=pt52lUF-mlQ), [Aidan Stanik](https://www.youtube.com/watch?v=1evxQpJkXzM)) | ❌ | ❌ | ✅ Addy Osmani blogu ([blog](https://addyosmani.com/blog/devtools-mcp/)) |
| Resmi kurum/marka | ❌ (topluluk) | ✅ Microsoft | 🟡 Coplay → Aura | ❌ | ❌ | 🟡 Aura | 🟡 runreal | ✅ Google |

---

## 3. Büyüme eğrileri ve kırılma noktaları

Tüm `~` değerler star-history eğrisinden okundu (link her başlıkta). Neden sütunu linkli değilse **tahmin**tir.

### 3.1 ahujasid/mcp-for-blender — detaylı hikâye

Kaynak eğri: [star-history](https://www.star-history.com/#ahujasid/mcp-for-blender&Date)

**Zaman çizelgesi**

| Tarih | Olay | Kaynak |
|---|---|---|
| 07.03.2025 | Repo açıldı; ilk soket bağlantısı. 08.03'te PyPI 0.1.0 | [AI Wiki](https://aiwiki.ai/wiki/blender_mcp) |
| 11.03.2025 | X'te "Claude'u Blender'a bağladım" + low-poly ejderha demo videosu | [Cline blog (gömülü tweet)](https://cline.bot/blog/human-intent-in-the-age-of-ai-insights-from-our-chat-with-siddharth-ahuja-creator-of-the-ableton-blender-mcps) |
| 12.03.2025 | "@Blender bunun üstüne resmi MCP kurabilir" alıntı tweet'i (Blender hesabını etiketledi) | [X](https://x.com/sidahuj/status/1899828297346974060) |
| 12.03.2025 | CreativeBloq: "Vibe rendering has arrived" haberi | [CreativeBloq](https://www.creativebloq.com/3d/vibe-rendering-has-arrived-this-plugin-allows-ai-to-control-blender) |
| 13.03.2025 | HN gönderisi, 54 puan / 1 yorum | [HN](https://news.ycombinator.com/item?id=43357112) |
| 13.03.2025 | Poly Haven entegrasyonu birleşti; 17.03'te plaj sahnesi demo tweet'i | [AI Wiki](https://aiwiki.ai/wiki/blender_mcp), [X](https://x.com/sidahuj/status/1901632110395265452) |
| 16.03.2025 | Hyper3D Rodin (Deemos) text-to-3D birleşti; Deemos kullanıcılara ücretsiz API anahtarı verdi | [AI Wiki](https://aiwiki.ai/wiki/blender_mcp) |
| 03.04.2025 | Cline blog röportajı | [Cline](https://cline.bot/blog/human-intent-in-the-age-of-ai-insights-from-our-chat-with-siddharth-ahuja-creator-of-the-ableton-blender-mcps) |
| 09.04.2025 | "Vahşi doğadaki kullanımlar" derleme thread'i (kullanıcı demolarını retweet) | [X](https://x.com/sidahuj/status/1909986466723168344) |
| Haz 2025 | AI Engineer World's Fair konuşması; konuşmada ~11.500★ ve 160.000+ indirme | [ai.engineer](https://ai.engineer/talks/nnktgWtfJHE-blender-mcp-future-creative-tools.md) |
| 09.08.2026 | Bakımcının GitHub hesabı ele geçirildi (HN 24 puan) | [HN](https://news.ycombinator.com/item?id=49238028) |
| 16.09.2026 | Ad değişti: **MCP for Blender**, `mcp-for-blender` 2.0.0 | [AI Wiki](https://aiwiki.ai/wiki/blender_mcp) |
| 30.09.2026 | Codex eklentisi | [AI Wiki](https://aiwiki.ai/wiki/blender_mcp) |
| 27.10.2026 | Meshy ortaklığı | [mcp-for-blender.com/blog](https://www.mcp-for-blender.com/blog) |

**Eğri**

| Dönem | ★ başı → sonu | Artış | Neden |
|---|---|---:|---|
| 06.03 → 11.04.2025 | ~41 → ~9.600 | **+9.600** | X demo videosu + HN + haber + entegrasyon zinciri (yukarıdaki tablo). 100 fork 6. günde ([01 §6](01_Rakip_ve_Emsal_Karsilastirma.md)) |
| 11.04 → 22.12.2025 | ~9.600 → ~14.400 | ayda +400–1.300 | Konferans, kullanıcı demoları (tahmin: organik arama) |
| 22.12.2025 → 27.01.2026 | ~14.400 → ~16.400 | +2.050 | tahmin: Claude Code/agent dalgası; tüm oyun MCP'lerinde aynı dönem hızlanma var |
| 2026 Mar → Ağu | ~17.300 → ~25.000 | ayda +1.400–2.500 | tahmin: Opus 5.x ile kullanıcı demoları ([Creations](https://www.mcp-for-blender.com/creations)) |
| 02.09 → 14.11.2026 | ~26.800 → ~34.800 | **+8.000** (2 ay) | Ad değişikliği + tek komut `uvx` + Codex eklentisi + Meshy ([AI Wiki](https://aiwiki.ai/wiki/blender_mcp)). Resmi Blender konektörü Claude'da yayına girince "Blender MCP" araması arttı (tahmin; [Claude konektörü](https://claude.com/marketplace/connectors/blender)) |

**Ders:** İlk 72 saat her şeyi belirledi. Kurucu her gün yeni bir demo videosu attı (11, 12, 17 Mart). Her entegrasyon (Poly Haven, Rodin) ayrı bir haber oldu. Ortaklar (Deemos) kendi kitlesine taşıdı.

### 3.2 microsoft/playwright-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#microsoft/playwright-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 21.03 → 26.04.2025 | ~50 → ~8.800 | **+8.700** | Microsoft/Playwright markası; Simon Willison 25.03 yazısı ([blog](https://simonwillison.net/2025/Mar/25/playwright-mcp/)). HN yalnız 6 puan ([HN](https://news.ycombinator.com/item?id=43465931)) → HN neden değil |
| 31.05 → 15.09.2025 | ~10.700 → ~19.200 | ayda +2.500–3.000 | Copilot coding agent'ta varsayılan MCP ([GitHub Docs](https://docs.github.com/en/copilot/concepts/coding-agent/mcp-and-coding-agent)); tarih tahmini: Mayıs 2025 |
| 2025 Eki → 2026 Kas | ~21.700 → ~39.800 | ayda +1.200–2.200 | Düzenli; rakip fikirler bile onu referans aldı (HN 189 puan "Playwright MCP'den az context" [HN](https://news.ycombinator.com/item?id=45642911)) |

### 3.3 CoplayDev/unity-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#CoplayDev/unity-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 18.03 → 23.04.2025 | ~240 → ~1.700 | **+1.450** | "Tek prompt'la Mario klonu" X videosu ([X](https://x.com/JustinPBarnett/status/1901957423851557035)); Cursor/Unity Reddit'lerinde öneri ([r/cursor](https://www.reddit.com/r/cursor/comments/1jgv4aa/will_you_guys_have_any_unity_support_or_is_there/)) |
| Ağu 2025 | ~2.660 → ~3.070 | +400 | Coplay satın aldı. Basın bülteni etkisiz, README güncellemesi Discord katılımını 3×, kurulumu 2× yaptı ([Coplay blog](https://coplay.dev/blog/lessons-learned-from-buying-an-open-source-repo)). Repo taşınınca Google sıralaması geçici kayboldu (aynı kaynak) |
| 23.11.2025 → 26.06.2026 | ~4.000 → ~12.500 | ayda +1.100–2.000 | ACM SIGGRAPH Asia '25 makalesi ([DOI](https://doi.org/10.1145/3757376.3771417)); docs sitesi ([site](https://coplaydev.github.io/unity-mcp/)); 16 GitHub topic ([API](https://github.com/CoplayDev/unity-mcp)); tahmin: ajan dalgası |

### 3.4 Coding-Solo/godot-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#Coding-Solo/godot-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 26.02 → 11.03.2025 | ~8 → ~9 | +1 | Duyuru yok |
| 11.03 → 16.04.2025 | ~9 → ~280 | **+270** | awesome-mcp-servers'a kendi PR'ı, 20.03.2025 ([commit](https://github.com/punkpeye/awesome-mcp-servers/commit/c96871f32c34289081eda8dd8036e605182ab68c)); wong2 listesi ([liste](https://github.com/wong2/awesome-mcp-servers)) |
| 2025 Nis → Ara | ~280 → ~1.200 | ayda +120–150 | Tek resmi yolu olmayan motor; "Godot MCP" aramasında ilk sırada (tahmin) |
| 25.12.2025 → 19.05.2026 | ~1.200 → ~3.650 | ayda +350–800 | tahmin: Claude Code dalgası; tek satır `claude mcp add` ([README](https://github.com/Coding-Solo/godot-mcp)); fork'lar onu kaynak gösterdi ([r/mcp](https://www.reddit.com/r/mcp/comments/1rjlqz0/ive_improved_the_godot_mcp_from_coding_solo_to/)) |
| 2026 Haz → Kas | ~4.300 → ~6.700 | ayda +500–700 | Push yok ama "resmi Godot MCP yok" → varsayılan seçim ([r/godot](https://www.reddit.com/r/godot/comments/1xtoenl/no_official_godot_mcp_server_exists_how_the/)) |

### 3.5 chongdashu/unreal-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#chongdashu/unreal-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 24.03.2025 (repo öncesi) | — | — | "Bu hafta sonu UE için MCP yapmaya başladım" X videosu ([X](https://x.com/chongdashu/status/1903982123402318294)) |
| 29.03 → 03.05.2025 | ~40 → ~550 | **+510** | Kendi YouTube kanalında 2 video: 04.04 ([video](https://www.youtube.com/watch?v=ei2gulox9Yo)), 11.04 ([video](https://www.youtube.com/watch?v=Rcj62DqrWXM)); üçüncü kişi kurulum videosu 14.04 ([video](https://www.youtube.com/watch?v=pt52lUF-mlQ)) |
| 2025 May → Kas | ~550 → ~1.020 | ayda +60–140 | Push 22.04.2025'te durdu; "ilk UE MCP" olarak arama trafiği (tahmin) |
| 2026 Oca → May | ~1.200 → ~1.880 | ayda +140–210 | tahmin: ajan dalgası; 2026 eğitimleri hâlâ bunu kullanıyor ([Aidan Stanik, 22.04.2026](https://www.youtube.com/watch?v=1evxQpJkXzM)) |
| 2026 Haz → Kas | ~1.990 → ~2.120 | ayda **~+30** | UE 5.8 resmi MCP'yi getirdi ([HN, 18.06.2026](https://news.ycombinator.com/item?id=48583236)) → düzleşme (tahmin: neden-sonuç) |

### 3.6 flopperam/unreal-engine-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#flopperam/unreal-engine-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 05.08 → 31.10.2025 | ~1 → ~107 | ayda +30–40 | YouTube kanalı yeni |
| 24.12.2025 → 09.05.2026 | ~215 → ~880 | ayda +90–195 | YouTube @flopperam 9k takipçi + Shorts ([LinkedIn](https://www.linkedin.com/in/ryan-thomas-uri)); FlopAI ürün videoları ([video, 28.01.2026](http://www.youtube.com/watch?v=1s_Mx7n29Fg)) |
| 2026 Haz → Kas | ~980 → ~1.110 | ayda +7–50 | Aura satın aldı ([BusinessWire](https://www.fidelity.com/news/article/mergers-and-acquisitions/202606261000BIZWIRE_USPR_____20260626_BW815100)); README reklama döndü ([hysenlabs](https://hysenlabs.com/projects/flopperam-unreal-engine-mcp)); UE 5.8 resmi MCP |

### 3.7 runreal/unreal-mcp (karşı örnek)

Kaynak eğri: [star-history](https://www.star-history.com/#runreal/unreal-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 28.05 → 02.07.2025 | 0 → ~18 | +18 | r/mcp gönderisi, 14 oy ([Reddit](https://www.reddit.com/r/mcp/comments/1l38z35/unreal_engine_mcp_control_your_editor_from_claude/)) |
| 2025 Tem → 2026 Kas | ~18 → 121 | ayda +2–14 | Push 06.06.2025'te durdu. Elinde npx tek komut, GIF, Glama rozeti, mcp.directory kaydı ([mcp.directory](https://mcp.directory/servers/unreal-engine)) vardı; **video, X demosu ve tekrar eden duyuru yoktu** |

### 3.8 ChromeDevTools/chrome-devtools-mcp

Kaynak eğri: [star-history](https://www.star-history.com/#ChromeDevTools/chrome-devtools-mcp&Date)

| Dönem | ★ | Artış | Neden |
|---|---|---:|---|
| 11.09 → 06.10.2025 | ~70 → ~9.200 | **+9.100** | Google resmi blog + @ChromiumDev videolu tweet, 23.09.2025 ([blog](https://developer.chrome.com/blog/chrome-devtools-mcp/), [X](https://x.com/ChromiumDev/status/1970505063064825994)); Addy Osmani 25.09 ([blog](https://addyosmani.com/blog/devtools-mcp/)); HN lansmanı yalnız 19 puan ([HN](https://news.ycombinator.com/item?id=45349829)) |
| 2025 Eki → 2026 Şub | ~9.200 → ~23.100 | ayda +1.900–3.700 | Chrome 142 "What's new in DevTools" notlarına girdi ([blog](https://developer.chrome.com/blog/new-in-devtools-142)) |
| 07.03 → 27.04.2026 | ~27.400 → ~36.800 | **ayda +4.500–4.900** | HN 604 puan / 234 yorum, 15.03.2026 ([HN](https://news.ycombinator.com/item?id=47390817)) |
| 2026 May → Kas | ~40.800 → ~57.900 | ayda +1.500–4.000 | Resmi "DevTools for agents" ürününe katıldı ([blog notu](https://developer.chrome.com/blog/chrome-devtools-mcp/)) |

---

## 4. Örüntüler

### 4.1 İşe yarayanlar

| # | Örüntü | Kanıt | Enginewright karşılığı |
|---|---|---|---|
| 1 | **Kurucu X'te 30–60 sn'lik "tek prompt → sonuç" videosu attı** | blender +9.600 / 5 hafta ([X](https://x.com/sidahuj/status/1899828297346974060)); unity +1.450 / 5 hafta ([X](https://x.com/JustinPBarnett/status/1901957423851557035)) | "Ajan paketli oyunu çalıştırıp bug'ı ekran görüntüsüyle buldu" videosu |
| 2 | **Her hafta yeni demo, her entegrasyon ayrı duyuru** | blender: 11, 12, 17 Mart ve Rodin ([AI Wiki](https://aiwiki.ai/wiki/blender_mcp)) | Her sürümde 1 video + 1 tweet |
| 3 | **Ortak firma kendi kitlesine taşıdı** | Deemos ücretsiz API anahtarı ([AI Wiki](https://aiwiki.ai/wiki/blender_mcp)); Cline röportajı ([Cline](https://cline.bot/blog/human-intent-in-the-age-of-ai-insights-from-our-chat-with-siddharth-ahuja-creator-of-the-ableton-blender-mcps)) | Claude Code / Codex / Cursor ekosistemine örnek; Fab satıcıları (tahmin) |
| 4 | **Kurucunun kendi YouTube kanalı** | chongdashu +510 ilk ay ([video](https://www.youtube.com/watch?v=ei2gulox9Yo)); flopperam 9k abone ([LinkedIn](https://www.linkedin.com/in/ryan-thomas-uri)) | Kısa video serisi (05'teki V1–V6) |
| 5 | **Üçüncü kişilerin kurulum videoları** | chongdashu'yu 2026'da bile anlatıyorlar ([video](https://www.youtube.com/watch?v=1evxQpJkXzM)) | Kurulumu kolay yap ki başkası anlatabilsin |
| 6 | **Tanınmış bir blogcu yazdı** | Simon Willison → playwright ([blog](https://simonwillison.net/2025/Mar/25/playwright-mcp/)) | Ulaşılabilir hedef değil; tahmin: gamedev YouTuber'ları daha gerçekçi |
| 7 | **awesome listesine erken PR** | godot 9 → 280 ([commit](https://github.com/punkpeye/awesome-mcp-servers/commit/c96871f32c34289081eda8dd8036e605182ab68c)) | Ucuz, 1 saatlik iş; küçük ama kalıcı taban |
| 8 | **Tek satır kurulum + "resmi yok" boşluğu** | godot ayda +500–700, push olmadan ([r/godot](https://www.reddit.com/r/godot/comments/1xtoenl/no_official_godot_mcp_server_exists_how_the/)) | UE'de resmi var → boşluk "PIE/paketli oyun/görsel QA" |
| 9 | **README'yi landing page gibi yönet** | Coplay: README güncellemesi Discord 3×, kurulum 2× ([Coplay](https://coplay.dev/blog/lessons-learned-from-buying-an-open-source-repo)) | README'nin ilk ekranı = GIF + tek komut |
| 10 | **Akademik/konferans içerik** | unity ACM makalesi ([DOI](https://doi.org/10.1145/3757376.3771417)); blender World's Fair ([talk](https://ai.engineer/talks/nnktgWtfJHE-blender-mcp-future-creative-tools.md)) | Uzun vade; şimdilik değil |
| 11 | **HN'de güçlü 2. dalga, 1. dalga değil** | chrome 604 puan ayında +4.9k ([HN](https://news.ycombinator.com/item?id=47390817)) | HN'yi lansman günü değil, somut bir hikâyeyle dene |

### 4.2 İşe yaramayanlar

| # | Örüntü | Kanıt | Ders |
|---|---|---|---|
| 1 | **Basın bülteni / medya** | Coplay: TechCrunch, VentureBeat vb. ilgilenmedi; "Cuma öğleden sonra paylaşma" ([Coplay](https://coplay.dev/blog/lessons-learned-from-buying-an-open-source-repo)) | Basına zaman harcama |
| 2 | **Teknik doğru, ama video yok** | runreal: npx + GIF + Glama, 121★ ([GitHub](https://github.com/runreal/unreal-mcp)) | Kontrol listesi tek başına yıldız getirmez |
| 3 | **Tek seferlik Reddit gönderisi** | runreal r/mcp 14 oy → ayda +2–14 ([Reddit](https://www.reddit.com/r/mcp/comments/1l38z35/unreal_engine_mcp_control_your_editor_from_claude/)) | Reddit tekrar eden içerikle çalışır |
| 4 | **Lansman günü HN'ye güvenmek** | playwright 6 puan ([HN](https://news.ycombinator.com/item?id=43465931)); chrome lansmanı 19 puan ([HN](https://news.ycombinator.com/item?id=45349829)) | Büyükler bile HN'de ilk gün tutmadı |
| 5 | **Repo sahipliği değişikliği** | Google'da geçici kaybolma ([Coplay](https://coplay.dev/blog/lessons-learned-from-buying-an-open-source-repo)) | `shanyup/enginewright` adını sabit tut |
| 6 | **Bakımı bırakmak + README'yi reklama çevirmek** | flopperam satış sonrası ayda +7–50 ([hysenlabs](https://hysenlabs.com/projects/flopperam-unreal-engine-mcp)) | Aktif release notu güven verir |
| 7 | **Resmi ürünle aynı işi yapmak** | UE 5.8 resmi MCP sonrası chongdashu/flopperam düzleşti (§3.5–3.6; neden-sonuç tahmin) | Epic'in yapmadığını öne çıkar ([01 §4](01_Rakip_ve_Emsal_Karsilastirma.md)) |
| 8 | **Güvenlik olayı** | blender: hesap ele geçirme ([HN](https://news.ycombinator.com/item?id=49238028)), 3 CVE ([AI Wiki](https://aiwiki.ai/wiki/blender_mcp)) | SECURITY.md + imzalı release; olay yıldızı düşürmedi ama güven riskidir |

### 4.3 Ortak zaman örüntüsü

| Gözlem | Kanıt |
|---|---|
| Patlama projelerinde ilk ~5 haftalık artış, sonraki 4–6 ayın toplamına yakın ya da daha büyük | blender +9.600 vs sonraki ~6 ay ~+3.900; playwright +8.700 vs sonraki ~4,5 ay ~+10.400; chrome +9.100 vs sonraki ~4 ay ~+14.000 ([blender](https://www.star-history.com/#ahujasid/mcp-for-blender&Date), [playwright](https://www.star-history.com/#microsoft/playwright-mcp&Date), [chrome](https://www.star-history.com/#ChromeDevTools/chrome-devtools-mcp&Date)) |
| Aralık 2025 – Mayıs 2026 arası **bütün** oyun MCP'leri hızlandı | blender, unity, godot, chongdashu, flopperam (§3). Neden **tahmin**: ajanlı kodlama araçlarının yaygınlaşması |
| Haziran 2026 sonrası UE topluluk MCP'leri yavaşladı, Unity/Godot/Blender yavaşlamadı | §3.3–3.6. Neden **tahmin**: UE 5.8 resmi MCP ([HN](https://news.ycombinator.com/item?id=48583236)) |

---

## 5. Kanal envanteri — duyuru kanalları

Zamanlama sütununda kaynak yoksa **tahmin**dir. "Başarılı örnek" sütunundaki her gönderi linklidir.

| Kanal | Kurallar (özet) | En uygun zamanlama | Başarılı örnek | Enginewright notu |
|---|---|---|---|---|
| **HN — Show HN** | Başlık "Show HN:" ile başlar. Kullanıcı denenebilir olmalı, kayıt/email engeli olmamalı. Sürüm notu ("1.3.1 çıktı") yeterli değil. Arkadaşlardan oy isteme yasak. Sahibi yorumlarda olmalı ([kurallar](https://news.ycombinator.com/showhn.html)) | Son 1 yılda 50+ puana en çok 12:00 ET (%4,4 vs ort. %2,9); en iyi gün Pazar ([thegrowthlist, 2026](https://www.thegrowthlist.co/launch/best-time-to-post-on-hacker-news)). 2021+ verisinde Pazar 19:00 ET (Pzt 00:00 UTC) %10,8 ([danfking](https://danfking.github.io/blog/2026/04/23/show-hn-by-the-numbers/)) | chrome-devtools-mcp 604 puan ([HN](https://news.ycombinator.com/item?id=47390817)); başka bir Blender MCP projesinin Show HN'i 151 puan ([HN](https://news.ycombinator.com/item?id=44622374)) | 1.0'da bir kez. Başlık "Playwright for Unreal". 3–4 saat yorumlarda kal |
| **r/unrealengine** | Yazılı self-promo kuralı ikincil kaynakta doğrulanamadı; "Marketplace" ve "Show Off" flair'leri var ([agentagency](https://agentagency.si/subreddits/r/unrealengine)). Reddit geneli 9:1 kılavuzu ([r/modnews](https://www.reddit.com/r/modnews/comments/2oamgp/moderators_clarifications_around_our_101/)). **Paylaşmadan önce sidebar elle okunmalı** | tahmin: Salı–Perşembe, 14:00–16:00 TSİ (ABD sabahı) | "88 araçlı açık kaynak Unreal MCP" gönderisi ([Reddit](https://www.reddit.com/r/unrealengine/comments/1tijkka/opensourced_unreal_mcp_88_tools_to_control_ue5/)) | "I built" + video + teknik detay. Hesap geçmişi önce dolsun |
| **r/gamedev** | Self-post ağırlıklı. Bağlamsız self-promo yasak ("No Self-Promotion Without Context") ([about](https://www.reddit.com/r/gamedev/about/)). Tanıtım haftalık thread'lere yönlendirilir; "kalıcı katılımcı" araç paylaşımına izin var ([mod açıklaması](https://www.reddit.com/r/modnews/comments/2oamgp/moderators_clarifications_around_our_101/)) | Haftalık thread'ler (Feedback Friday vb.) ([voicepo.st](https://voicepo.st/tools/find-my-subreddits)) | — (MCP için güçlü örnek bulunamadı) | Ürün değil **postmortem** yaz: "AI ajanı ile UE'de QA otomasyonu: ne çalıştı" |
| **r/IndieDev** | Haftalık promo rotasyonu: Feedback Friday, Screenshot Saturday, Marketing Monday; ana akışta promo silinir ([voicepo.st](https://voicepo.st/tools/find-my-subreddits)) | İlgili haftalık thread günü | — | Düşük öncelik |
| **r/LocalLLaMA** | Kural 2: LLM konusu. Kural 4: self-promo içeriğin ~%10'u, bağlantı açıklanmalı, "I found this" yasak. Tamamen LLM yazımı metin yasak ([subredditmarketing](https://www.subredditmarketing.com/subreddits/localllama), [rankhog](https://rankhog.com/subreddits/localllama)) | tahmin: hafta içi ABD sabahı | — | Yalnız **yerel model** (Ollama/llama.cpp) ile çalışan demo varsa. Yoksa atla |
| **r/ClaudeAI** | Yazılı kurallar alınamadı (403). Kullanıcı demoları çok tutuyor; mod botu özet yazıyor ([örnek](https://www.reddit.com/r/ClaudeAI/comments/1xmcwbt/opus_55_medium_with_blender_mcp_is_insane_with/)) | tahmin: hafta içi ABD sabahı | "Opus 5.5 + Blender MCP inanılmaz" (50+ yorum) ([Reddit](https://www.reddit.com/r/ClaudeAI/comments/1xmcwbt/opus_55_medium_with_blender_mcp_is_insane_with/)) | "Claude Code UE'de paketli oyunumu test etti" iş akışı gönderisi; ürün değil iş akışı anlat |
| **r/mcp** | Açıklamalı lansman, showcase etiketi. Waitlist, gizli promo, AI-slop yasak ([redditfind](https://redditfind.ai/en/research/best-subreddits-ai-agents-api-integration-2026/)) | tahmin: herhangi hafta içi | runreal 14 oy ([Reddit](https://www.reddit.com/r/mcp/comments/1l38z35/unreal_engine_mcp_control_your_editor_from_claude/)) | Düşük getiri; tek gönderi |
| **X (Twitter)** | Platform kuralı dışında kısıt yok. Etiket: @UnrealEngine, @AnthropicAI, ilgili araç hesapları | tahmin: ABD sabahı 15:00–18:00 TSİ, Salı–Perşembe | blender dragon videosu ([X](https://x.com/sidahuj/status/1899828297346974060)); unity Mario videosu ([X](https://x.com/JustinPBarnett/status/1901957423851557035)); chongdashu WIP videosu ([X](https://x.com/chongdashu/status/1903982123402318294)) | **Ana kanal.** Tek tweet + gömülü video; thread ikinci planda |
| **YouTube** (uzun + Shorts) | Platform kuralı. Açıklamaya GitHub linki | tahmin: yayın günü önemsiz; arama trafiği kalıcı | chongdashu 10 dk demo ([video](https://www.youtube.com/watch?v=ei2gulox9Yo)); flopperam Shorts ([README](https://github.com/flopperam/unreal-engine-mcp)) | Kurulum videosu (5 dk) + 3 Shorts |
| **Unreal forumları (Epic Developer Community)** | Showcase ve WIP kategorileri var. Bitmemiş ürün "Work in Progress"e, Fab ürünü "Product Discussion"a ([eski kural](https://forums.unrealengine.com/t/deprecated-marketplace-faq-and-guidelines/29592)) | tahmin: önemsiz; başlık uzun ömürlü | "Aura: AI Agent for Unreal Editor" 19.752 görüntülenme ([forum top](https://forums.unrealengine.com/top)); "StraySpark Unreal MCP Server" 8.374 görüntülenme (aynı) | Showcase'te tek kalıcı thread; her sürümde aynı thread'e yanıt |
| **Epic AI MegaJam 2026** | 12–15.11.2026, MCP ve ajan kurulumu önceden serbest ([itch.io](https://itch.io/jam/2026-epic-ai-megajam)); kazananlar 29.01.2027'ye kadar | Bitti; 2027 jam'ine hazırlık. Jam dev-log thread'leri hâlâ açık ([forum](https://forums.unrealengine.com/t/wip-atlantic-bay-command-a-city-defence-against-a-gpu-bug-horde-epic-ai-megajam-2026-solo-ai-agents/2855483)) | — | Jam katılımcılarına "jam sonrası paketli build testi" içeriği (tahmin) |
| **Discord'lar** | Çoğu sunucuda promo yalnız #showcase/#self-promo kanalında, 24 saat slowmode yaygın ([discords.ai](https://www.discords.ai/wiki/discord-promo-channel-setup)). Unreal Source (eski Unreal Slackers) en büyük UE Discord'u ([Reddit duyurusu](https://www.reddit.com/r/unrealengine/comments/16s558y/unreal_slackers_is_now_unreal_source/)) | tahmin: önemsiz | blender/unity/flopperam kendi Discord'larını README'ye koydu ([blender](https://github.com/ahujasid/mcp-for-blender), [unity](https://github.com/CoplayDev/unity-mcp)) | Önce kendi Discord'unu aç; başka sunucuda yalnız showcase kanalı |

---

## 6. Kanal envanteri — MCP dizinleri ve listeler

| Dizin | Nasıl girilir | Kural / gereksinim | Etkisi (kanıt) | Enginewright notu |
|---|---|---|---|---|
| **Resmi MCP Registry** | `server.json` + `mcp-publisher publish` ([studiomeyer](https://studiomeyer.academy/en/recipes/9.3-mcp-marketplaces)) | Paket (npm/PyPI/Docker) metadata'sı doğrulanır ([venturelist](https://venturelist.app/guides/mcp-server-and-ai-agent-directories)) | Diğer dizinler buradan beslenir (aynı kaynak) | **İlk adım.** UE eklentisi paket değil; bir npm/uvx köprü paketi gerekebilir (tahmin) |
| **punkpeye/awesome-mcp-servers** | Fork → README'ye tek satır → PR ([CONTRIBUTING](https://github.com/punkpeye/awesome-mcp-servers/blob/main/CONTRIBUTING.md)) | Herkese açık GitHub repo; kategori içinde alfabetik; tek satır açıklama. Glama skor rozeti beklentisi var ([01 §2](01_Rakip_ve_Emsal_Karsilastirma.md)) | godot 9 → 280 ilk ay ([commit](https://github.com/punkpeye/awesome-mcp-servers/commit/c96871f32c34289081eda8dd8036e605182ab68c)) | "Gaming" kategorisi. PR başlığı sade |
| **wong2/awesome-mcp-servers** | PR | — | playwright ve godot listede ([liste](https://github.com/wong2/awesome-mcp-servers)) | İkinci PR |
| **Glama** | "Add MCP Server" → GitHub URL; otomatik lisans/güvenlik/sağlık testi, dakikalar içinde ([Glama FAQ](https://glama.ai/mcp/faq)) | `mcp` topic'i ile otomatik indeks de var ([studiomeyer](https://studiomeyer.academy/en/recipes/9.3-mcp-marketplaces)) | runreal rozet koydu, büyümedi ([README](https://github.com/runreal/unreal-mcp)) → tek başına zayıf | Rozet = awesome listesi için kalite kanıtı |
| **Smithery** | Form ya da URL ile yayın; Streamable HTTP sunucular uyumlu ([venturelist](https://venturelist.app/guides/mcp-server-and-ai-agent-directories)) | Ücretli "featured" katmanı var ([studiomeyer](https://studiomeyer.academy/en/recipes/9.3-mcp-marketplaces)) | Kanıt bulunamadı | Yerel eklenti olduğu için uyum sınırlı (tahmin) |
| **mcp.so** | Ücretsiz form (incelemeli) veya 39$ anında yayın ([venturelist](https://venturelist.app/guides/mcp-server-and-ai-agent-directories)) | README'den kurulum bloğu, `## Tools` tablosu, ilk görseli çeker ([studiomeyer](https://studiomeyer.academy/en/recipes/9.3-mcp-marketplaces)) | Kanıt bulunamadı | Ücretsiz yol yeter; README'de `## Tools` tablosu olsun |
| **MCP Market / LobeHub / mcp.directory** | Ücretsiz kuyruk; MCP Market 4–6 hafta ([venturelist](https://venturelist.app/guides/mcp-server-and-ai-agent-directories)) | — | runreal mcp.directory'de ([sayfa](https://mcp.directory/servers/unreal-engine)) | Arka plan işi |

---

## 7. Enginewright için çıkarım (karar değil, öneri)

| Sıra | Öneri | Dayanak |
|---|---|---|
| 1 | Lansmanı **tek bir 45 sn'lik X videosu** etrafında kur: "Claude Code UE 5.8'de paketli oyunu çalıştırdı, bug'ı ekran görüntüsüyle buldu, geri aldı." | §4.1 #1 |
| 2 | Lansmanı 1 güne değil **3 haftaya yay**: her 3–4 günde yeni bir demo (PIE çok oyunculu, görsel QA, lokalizasyon). | §3.1 zaman çizelgesi |
| 3 | Mesajı **"Epic'in resmi MCP'si + Enginewright"** olarak kur; rakip gibi değil, tamamlayıcı gibi. | §4.2 #7 |
| 4 | Dizinleri lansmandan **önce** bitir: Registry → awesome PR → Glama → mcp.so. | §6 |
| 5 | HN'yi lansman günü değil, somut bir hikâyeyle (ör. RagnaCook paketli build QA'sı) **Pazar 19:00 ET** civarı dene. | §5 HN satırı, §4.2 #4 |
| 6 | Reddit'te ürün değil **iş akışı/postmortem** anlat; hesap 9:1 oranını önce doldursun. | §5 Reddit satırları |
| 7 | Basın bültenine zaman ayırma. | §4.2 #1 |

Takvim ve başlık taslakları: [05_Duyuru_ve_Yildiz_Buyume_Plani.md](05_Duyuru_ve_Yildiz_Buyume_Plani.md). Bu belgenin bulguları 05'teki 30/90/180 gün hedeflerini değiştirmez; yalnız "Show HN +1. gün" maddesinin önceliğini düşürmeyi önerir.

---

## 8. Açık noktalar

| Konu | Durum |
|---|---|
| Reddit yazılı kuralları (r/unrealengine, r/ClaudeAI) | Bu makineden 403. Paylaşmadan önce sidebar elle okunmalı. |
| Playwright'ın Copilot'a varsayılan eklenme tarihi | Docs doğruluyor; tarih "Mayıs 2025" **tahmin**. |
| 2026 Ocak–Mayıs ortak hızlanmanın nedeni | **tahmin** (ajan dalgası); kanıtlı tek neden yok. |
| unity-mcp'nin awesome listelerinde olmaması | 17.11 README taramasında bulunamadı; eski adla (`justinpbarnett`) aranmadı. |
