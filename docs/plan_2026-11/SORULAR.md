# SORULAR — Talha'ya (en fazla 10, ilki öneri)

Tarih: 16.11.2026. Her soruda öneri ilk sırada.

## S-1 — Türkçe iç belgeler public repoda nerede dursun?
- Öneri: `docs/plan_2026-11/` ve Türkçe iç notlar push edilmeden ayrı private repo (veya private dal); public repo yalnızca EN (README + docs-site).
- Alternatif: public repoda `docs/internal/` altında kalsın (şeffaflık).
- Alternatif: olduğu gibi kalsın, kimse bakmaz.
- Etki: SEO ve ilk izlenim (04 §5); bu kartın çıktıları şu an yerelde, push yok.

## S-2 — Fab'a free code plugin listing? (v1.0 ile)
- Öneri: evet, tek UE sürümü (5.8), free (kural: free sadece code plugin).
- Alternatif: hayır, sadece GitHub.
- Etki: Fab araması yeni kullanıcı kanalı (05 §2'ye eklenir); review süresi günler; her sürüm için ayrı zip yükü.

## S-3 — Resmi MCP Registry'ye publish (server.json repo'ya girer)
- Öneri: evet, v1.0'da; `io.github.shanyup/enginewright`; PulseMCP buradan otomatik çeker.
- Alternatif: sadece awesome-mcp-servers + Glama.
- Etki: registry = en büyük liste trafiği (04 §2); repo'ya `server.json` ekleme = public değişiklik → onay gerekli.

## S-4 — Epic resmi UE 5.8 MCP karşısında konumlandırma
- Öneri: "oyun iş akışları" (cook/package, PIE senaryoları, ağ/replikasyon testi, görsel QA) — Epic oralarda yok (01 §5).
- Alternatif: "daha kolay kurulum + tek komut client config" (Playwright modeli).
- Alternatif: her ikisi (önerilen kombinasyon aslında bu).
- Etki: README ilk cümlesi + duyuru başlıkları.

## S-5 — Lisans ve katkı: MIT + CLA?
- Öneri: MIT kalsın, CLA yok (tek geliştirici, sürtünme istemiyoruz).
- Alternatif: CLA ekle (ileride şirketleşirse).
- Etki: katkıcı havuzu; awesome listeler lisans filtresi uygulamıyor ama MIT en yaygın.

## S-6 — Discord sunucusu açalım mı?
- Öneri: hayır, ilk 6 ay GitHub Discussions yeterli.
- Alternatif: evet, lansmanda aç.
- Etki: moderasyon yükü (tek kişi); topluluk 05 §4'te Discussions + issue ile başlar.

## S-7 — Demo videoları kim üretir?
- Öneri: agent kaydı (ekran görüntüsü + senaryo), Talha onayı + ses/müzik ekler.
- Alternatif: Talha tek başına çeker.
- Etki: 05 §3'teki 6 videonun üretim hızı.

## S-8 — v0.5 kapsamı: önce RagnaCook demo mu, GP ağ testleri mi?
- Öneri: RagnaCook demo (çekim + paketleme) önce — yakın tarih, somut tanık; GP ağ testleri v0.6.
- Alternatif: GP önce (kart t_e6c46ed7 aktif).
- Etki: 03'teki sürüm sırası.

## S-9 — Ücretli katman / premium (blender-mcp'in premium generation modeli gibi)?
- Öneri: hayır, saf OSS + MIT; monetizasyon kendi oyunlarımız üzerinden.
- Alternatif: "Sponsor" GitHub Sponsors butonu (ücretsiz, pasif).
- Etki: algı (community trust); blender-mcp telemetri/premium tepkisi dersi.

## S-10 — Hangi haftada HN Show HN? (tek hak)
- Öneri: v1.0 lansman haftası (05 §2 hafta 6), signup-free, denenebilir build ile.
- Alternatif: v0.5 demo ile erken deneme (risk: "early one-off" algısı).
- Etki: HN tek gönderim; 2026'da Show HN kısıtlı (showlim) — mod kommentine dikkat.
