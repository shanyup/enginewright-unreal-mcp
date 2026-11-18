# SORULAR — Karar Bekleyen 10 Soru

> **Durum 18.11.2026 (kart t_c6e43c15):** S-0 Talha kararıyla kapandı (`enginewright-unreal-mcp`). S-2, S-4, S-10 **SORULAR_2 / 10 ile geçersiz**. Açık kalan S-1, S-3, S-5, S-6, S-7, S-8, S-9 → [SORULAR_3.md](SORULAR_3.md) (S3-05, S3-08, S3-09, S3-06, S3-10, S3-11, S3-12). Aşağıdaki metin tarihli kayıt olarak değişmeden kalır.

Tarih: 17.11.2026. Her soru: 2–3 seçenek, **ilk öneri**, gerekçe + etki. Talha cevaplarsa `OZET.md` ve ilgili dosyalar güncellenir.

---

## S-0: Repo adı ne olmalı? (bkz 04 §2)

> ✅ **Kapandı (Talha 18.11):** depo adı `enginewright-unreal-mcp`, marka adı Enginewright değişmez. Geçiş planı: 12 §1. Zaman sorusu: SORULAR_3 S3-02.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) `enginewright-unreal-mcp`** ← **ÖNERİ** | Marka + anahtar kelime; arama sıralaması kazanır, yönlendirme 1 yıl, Fab/Marketplace `Plugins/Enginewright` ile uyumlu | Keşfedilebilirlik ↑, marka korunur |
| (b) `enginewright` kalsın | Marka saf; arama sıralaması düşük (198/80/34) | Keşfedilebilirlik ↓ |
| (c) `unreal-mcp-enginewright` | Sadece anahtar kelime; marka kaybolur, çakışma riski (chongdashu, flopperam, runreal, kvick, GenOrca) | Marka ↓, risk ↑ |

**Gerekçe:** GitHub arama ad > açıklama > topic. "unreal mcp in:name" 247 sonuç, enginewright yok. Lider chongdashu "unreal-mcp" adıyla 2.118★. v1.0 lansmanında rename + yönlendirme en güvenli.

---

## S-1: Fab / Unreal Marketplace ücretsiz listing yapılır mı?

> ↪ SORULAR_3 **S3-05**'e taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Evet, UE 5.8 sürümüyle başla** ← **ÖNERİ** | Code-only plugin, MIT, her sürüm ayrı zip; Fab otomatik build, ≤15 GB. Yeni kitle (Marketplace arama) + güven damgası | Keşfedilebilirlik ↑, kurulum kolaylığı ↑ |
| (b) Hayır, sadece GitHub | Marketplace süreci (review, versioning) yükü; GitHub yeterli | Basitlik ↑ |

**Gerekçe:** Kural 1.2.4.b: free code plugin listelenebilir. Enginewright tam uyar. UE 5.8 zip'i v1.0 Release assets'ına ekle → Fab'a gönder.

---

## S-2: Headless / commandlet MCP (editör kapalı import/veri) v0.6 mı 1.0 mı?

> ⛔ **SORULAR_2 / 10 ile geçersiz:** headless 0.9.5 – 0.9.8 ara sürümlerinde (10 §3 #39–#42).

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) v0.6 başlangıcı, 1.0 tam** ← **ÖNERİ** | v0.6 `headless_import` + `headless_batch` (commandlet MCP sunucusu) başlar; 1.0 `headless_datatable` + stabil CLI | ARPG EW-9, RC EW-RC-01 erken kazanım |
| (b) Sadece 1.0 | Tek seferde tam; v0.6'ya ek yük yok | v0.6 hafif, 1.0 riskli |

**Gerekçe:** ARPG §12.3 EW-9 "headless mod" 6–8 kart-gün; editör kuyruğunu açar. v0.6'da commandlet MCP başlatma (mevcut `editor_restart` altyapısı) yapılır, 1.0'da tam CLI paketlenir.

---

## S-3: Python exec aracı eklenir mi? (Bilinçli tercih: YOK)

> ↪ SORULAR_3 **S3-08**'e taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Hayır, C++ yolu devam** ← **ÖNERİ** | `console_command` + `editor_restart` + `live_coding_compile` + `headless_import` (v0.6) Python'ı kapsar; güvenlik yüklenmez | Güvenlik ↑, bakım ↓ |
| (b) Evet, sandbox ile (1 MB, timeout) | EngineWire / UE-MCP / IvanMurzak / VibeUE hepsi var; Python betikleri için istek gelebilir | Esneklik ↑, risk ↑ |

**Gerekçe:** Mevcut 129 araç + v0.6 `headless_import` + `console_command` Python kullanım alanlarını (asset işleme, veri dönüşümü) kapatır. Python exec supply-chain riski (pip, venv, sürüm çakışması) getirir.

---

## S-4: Çoklu UE sürümü destek (5.4–5.8) 1.0'da mı Sonra mı?

> ⛔ **SORULAR_2 / 10 ile geçersiz:** S2-09 kararı; ikinci UE sürümü 0.9.13 ve 1.0.0-rc.3 (10 §3 #47, #53).

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) 1.0: 5.8 tek sürüm; 5.4–5.7 Sonra** ← **ÖNERİ** | Tek binary, compat header bakımı yok; Fab listing 5.8 ile başlar; Epic 5.9/6.0 MCP büyürken takip | Odak ↑, bakım ↓ |
| (b) 1.0: 5.4–5.8 hepsi | CI matrisi 5 sürüm × 2 config = 10 build; her araç testi 5× | Kapsam ↑, yük ↑↑ |

**Gerekçe:** Epic UE 5.9/6.0'da resmi MCP genişleyecek (01 §5). Tek sürümle hızlı iterasyon, sonra resmi MCP'ye uyum.

---

## S-5: Discord sunucusu açılır mı?

> ↪ SORULAR_3 **S3-09**'a taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Hayır, GitHub Discussions** ← **ÖNERİ** | Discussions: Q&A + Showcase + Ideas; SEO dostu, GitHub entegre, moderasyon az; Discord: anlık ama arama motoru indekslemez, moderasyon yükü | SEO ↑, yük ↓ |
| (b) Evet, küçük topluluk | Anlık yardım, sesli demo; ama 2. kanal = 2. yük | Etkileşim ↑ |

**Gerekçe:** MCP topluluğu GitHub/Reddit/HN merkezli. blender-mcp / playwright-mcp / unity-mcp Discord yok (GitHub Discussions + Reddit). Enginewright da aynı.

---

## S-6: Dokümantasyon sitesi (GitHub Pages) v1.0'da mı Sonra mı?

> ↪ SORULAR_3 **S3-06**'ya taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) v1.0'da canlı** ← **ÖNERİ** | `docs/` (EN) → GitHub Pages; 10 anahtar kelime sayfada; arama trafiği çeker | SEO ↑, keşfedilebilirlik ↑ |
| (b) Sonra | v1.0 lansman odaklı; site sonrası | Lansman hızı ↑ |

**Gerekçe:** 04 §8: 10 anahtar kelime, `tools/` her araç sayfası. v1.0 Release PR'sinde `docs/` deploy workflow eklenir.

---

## S-7: Lisans MIT mi kalır? (Öneri: EVET)

> ↪ SORULAR_3 **S3-10**'a taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Evet, MIT sabit** ← **ÖNERİ** | blender-mcp (MIT), playwright-mcp (Apache-2.0), unity-mcp (MIT), awesome-mcp-servers (MIT). MIT en permissive, ticari kullanım serbest, katkıcı çeker. | Topluluk ↑, Fab uygunluğu ↑ |
| (b) Apache-2.0 / dual | Patent koruması; ama karmaşıklık | Koruma ↑ |

**Gerekçe:** MIT zaten var, değiştirme nedeni yok. Fab free code plugin kuralı MIT/BSD/Apache kabul eder.

---

## S-8: Telemetri / anonim kullanım istatistiği toplanır mı?

> ↪ SORULAR_3 **S3-11**'e taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Hayır, asla** ← **ÖNERİ** | blender-mcp telemetri ekleyince itiraz aldı, kaldırdı (GitHub issue #247). Enginewright "strict, verified" markası — telemetri zıtlık. | Güven ↑, marka tutarlı |
| (b) Opt-in, anonim | Yine de risk; bakım yükü | Veri ↑ |

**Gerekçe:** S-8 (YAPMA listesi 05 §6 maddesi 5). Şeffaf, izinli, kapalı kaynak olmaması yeterli.

---

## S-9: Ücretli / Pro katman planlanır mı?

> ↪ SORULAR_3 **S3-12**'ye taşındı.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Hayır, saf OSS** ← **ÖNERİ** | blender-mcp/playwright-mcp/unity-mcp/awesome-mcp-servers hepsi ücretsiz. Gelir modeli: danışmanlık / eğitim / özel geliştirme (şirket bazında), araç ücretsiz. | Topluluk ↑, basitlik ↑ |
| (b) Evet, Pro (hosted, SLA, özel araçlar) | SaaS modeli; ama tek geliştirici için operasyon yükü yüksek | Gelir ↑ |

**Gerekçe:** 05 §6 madde 3. Şirket (Shanyup Studios) danışmanlık/özel geliştirme ile gelir; araç OSS kalır.

---

## S-10: v0.4.1 art pipeline araçları (EW-1, EW-2) RagnaCook demo takvimine yetişir mi?

> ⛔ **SORULAR_2 / 10 ile geçersiz:** sanat hattı 0.7.x serisine taşındı (10 §3 #20–#25); 0.4.1 artık UI metin taşma denetimi.

| Seçenek | Açıklama | Etki |
|---|---|---|
| **(a) Evet, 2 hafta (30 saat) yeterli** ← **ÖNERİ** | `asset_pipeline` + `asset_import_batch` + `skeletal_import` + `image_diff` + `material_variants` = 30 saat; RagnaCook M1-5/M2-5b yakın, demo çekimi bu araçları çeker | Demo kalite ↑, doğrulama gerçek |
| (b) Kısalt, sadece `asset_pipeline` + `skeletal_import` | 18 saat; ama `image_diff` + `material_variants` olmadan görsel QA / varyant üretimi eksik | Hız ↑, kalite ↓ |

**Gerekçe:** RagnaCook `Devir_Notu_16-11.md` M1-5 (sanat hattı) ve M2-5b (demo çekimi) bu araçları doğrudan kullanır. 03 v0.4.1 tablosu bu bağımlılığı yazıyor.