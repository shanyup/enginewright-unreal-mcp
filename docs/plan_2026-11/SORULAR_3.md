# SORULAR_3 — Yayın hazırlığı kararları

Tarih: 18.11.2026 · Kart `t_c6e43c15`. **15 soru.** Her soruda öneri ilk sıradadır (**A**) ve gerekçesi yazılıdır. Bu kart karar vermez; belgeler (11, 12, 13) cevap gelene kadar A'yı varsayar.

Kaynak: Eski SORULAR'dan açık kalan S-1, S-3, S-5, S-6, S-7, S-8, S-9 (S3-05, S3-06, S3-08 … S3-12) + yayın planından çıkan 8 yeni soru.
Kapananlar (SORULAR'a not düşüldü): S-0 (Talha 18.11: `enginewright-unreal-mcp`), S-2 / S-4 / S-10 (SORULAR_2 ve 10 ile geçersiz).

| # | Konu | Kaynak | Bağlı belge |
|---|---|---|---|
| S3-01 | İç planlar açık depoya girsin mi | yeni | 12 §1.4 |
| S3-02 | Depo adı değişikliği hangi yayında | yeni | 12 §1.2 |
| S3-03 | Resmi MCP Registry için köprü | yeni | 12 §6 |
| S3-04 | Seri sonu küçük paylaşımları | yeni | 13 §2 |
| S3-05 | Fab ücretsiz listesi | S-1 | 12 §6 |
| S3-06 | Doküman sitesi zamanı | S-6 | 12 §7 |
| S3-07 | Enginewright işareti (logo) | yeni | 12 §5 |
| S3-08 | Python çalıştırma aracı | S-3 | 11 §2 |
| S3-09 | Kendi Discord sunucusu | S-5 | 13 §3 |
| S3-10 | Lisans | S-7 | — |
| S3-11 | Telemetri | S-8 | 13 §8 |
| S3-12 | Ücretli katman | S-9 | — |
| S3-13 | Show HN zamanı | yeni | 13 §6 |
| S3-14 | Demo videosunda ses | yeni | 13 §4 |
| S3-15 | Duyuru hangi X hesabından | yeni | 13 §1 H2 |

---

## S3-01. Türkçe iç planlar (`docs/plan_2026-11/`, `docs/arastirma/`) açık depoya girsin mi?

Durum (18.11): yerel `main`, `origin/main`'den **47 commit önde**. Bu commit'lerde 58 iç plan dosyası var. YAYIN_HAZIRLIK K4 kararı: "iç Türkçe belgeler açık dala girmez" (o dönemde dört dosya `.git/info/exclude` ile dışarıda tutuldu). Bu klasörler o karardan sonra eklendi ve git'e girdi.

- **A (öneri):** Açık dala girmez. 0.4.4 push'undan önce bir kart bu iki klasörü ayrı özel depoya (ya da Brain'e) taşır; `main` geçmişinde kalmaması için temiz yayın dalı kullanılır. Gerekçe: K4 ile tutarlı; rakip adları, oyun iç notları ve ★ hedefleri açıkta kalmaz; 13 §8 Y-8 (rakip karşılaştırması iç belge).
- B: Olduğu gibi push edilir. Şeffaflık artar; ama Türkçe iç not İngilizce kullanıcıya gürültüdür ve iç kararlar (yıldız hedefi, rakip tablosu) açığa çıkar.
- C: Yalnız `docs/ROADMAP.md`'ye İngilizce özet çıkar, gerisi A gibi.

## S3-02. Depo adı değişikliği (`enginewright` → `enginewright-unreal-mcp`) hangi yayında yapılsın?

- **A (öneri): 0.4.4 yayını.** Gerekçe: dizin kayıtlarından (0.5.6) ve küçük paylaşımlardan önce; ★ 10'dayken arama kaybı en ucuz; Registry MCPB URL kuralını karşılar (12 §1.2).
- B: 0.6.6 yayını (büyük duyurudan hemen önce). Dezavantaj: 0.5.6'daki Glama ve awesome kayıtları eski adla açılır, sonra güncellenmesi gerekir.
- C: Hemen (0.4.1'den önce, ayrı küçük iş). Dezavantaj: push ve sürüm kararı yayın işine bağlı (S2-15); tek başına ad değişikliği için push gerekir.

## S3-03. Resmi MCP Registry'ye nasıl girilsin?

Durum: Registry yalnız npm / PyPI / NuGet / OCI / MCPB paketlerini ya da uzak URL'yi kabul eder (K2). Enginewright editör içinde yerel HTTP sunucusudur; doğrudan uymaz.

- **A (öneri): Küçük npm köprüsü** (`enginewright-unreal-mcp` adlı npm paketi, stdio ↔ `http://127.0.0.1:3100/mcp`, token'ı `Saved/Enginewright/clients`'tan okur). 0.6.0–0.6.2'de kart; yayın 0.6.6. Gerekçe: `npx` tek komut kurulumu da verir (03b §2: tek komut kurulum emsallerin çoğunda var); Registry + mcp.so + Smithery'nin yerel türü bunu okur.
- B: MCPB paketi (release dosyası). Gerekçe: Node gerekmez. Dezavantaj: MCPB'nin editör içi sunucuyu nasıl başlatacağı belirsiz; ekosistem daha küçük (TAHMİN).
- C: Registry'ye girilmez; yalnız Glama + awesome + mcp.so. Dezavantaj: "Python/Node yok" mesajı korunur ama Registry'den beslenen dizinler kaçar.

Not: A seçilirse README'de "No Python or Node" cümlesi "No Python or Node **required** (optional npx bridge)" olur.

## S3-04. Seri sonu küçük paylaşımları ne kadar olsun? (13 §2)

- **A (öneri):** 0.4.4 ve 0.5.6'da yalnız Unreal forumu WIP thread'i + tek X gönderisi (15–25 sn klip). Reddit, HN, YouTube yok. Gerekçe: büyük patlama bir kez olur (03b §0 #1); hesap geçmişi ve ilk geri bildirim yeter.
- B: A + Discord #showcase kanalları (0.5.6). Biraz daha erken kullanıcı; ama aynı sunucuda 0.6.6'da ikinci gönderi zayıf görünür (TAHMİN).
- C: Küçük paylaşım yok; ilk dış ses 0.6.6.

## S3-05. Fab'da ücretsiz listeleme yapılsın mı? (eski S-1)

- **A (öneri): Evet, 1.0.0'da.** Gerekçe: API 1.0'da donar (10 #55); Fab her sürüm için inceleme ister, 0.x'te sık ad değişikliği listeyi eskitir; 1.0'da UE 5.8 + 5.7 (0.9.13) iki sürümle girilir.
- B: Evet, 0.6.6'da (büyük duyuruyla). Yeni kitle erken; ama 0.7–0.9 boyunca her seri sonunda Fab güncellemesi yükü.
- C: Hayır, yalnız GitHub.

## S3-06. Doküman sitesi ne zaman? (eski S-6)

- **A (öneri): 0.9.15'te hazırlık, 1.0.0'da canlı, GitHub Pages `shanyup.github.io/enginewright-unreal-mcp`.** Gerekçe: 0.9.15 README tablosunu açıklamadan üretir; aynı üretici sayfaları da üretir; 0.9.12 ad dondurmasından önceki sayfalar eskirdi. Pages ad değişikliğinden sonra açıldığı için yönlendirme sorunu yok (K1).
- B: 0.6.6'da tek sayfalık site (README'nin kopyası). Arama sinyali erken; bakım çift.
- C: A ama özel alan adı (ör. alt alan adı). Adres kalıcı olur; alan adı ayarı Talha işi.

## S3-07. Enginewright'ın kendi işareti (logo) var mı, sosyal görselde ne kullanılsın?

- **A (öneri): Sosyal görsel için basit yazı işareti** ("Enginewright" + küçük "Unreal Engine MCP"), Shanyup marka kitinin renkleriyle. Ayrı logo 1.0 öncesi istenirse ayrı marka kartı. Gerekçe: logo tasarımı duyuruyu geciktirmez; GitHub önizlemesinde yazı okunurluğu yeter.
- B: Önce Enginewright logosu (marka kartı, 0.6.5'ten önce).
- C: Shanyup Studios "two-block S" işareti kullanılır. Dezavantaj: ürün ile stüdyo karışır.

## S3-08. Python çalıştırma aracı eklensin mi? (eski S-3)

Yeni bilgi: Epic 5.9 Preview'da `execute_python` var (K5). Enginewright Epic'in yanında çalışır; Python isteyen kullanıcı Epic'inkini kullanabilir.

- **A (öneri): Hayır.** Gerekçe: "No Python" mesajı ve güvenlik sınırı korunur; ihtiyaç Epic MCP'sinden karşılanır; S2 kararlarıyla (genel araçlar C++) tutarlı.
- B: Evet, sandbox ile (zaman aşımı, Read/Write/Execute modu gibi izin). Esneklik artar; güvenlik yüzeyi ve bakım artar.

## S3-09. Kendi Discord sunucusu açılsın mı? (eski S-5)

- **A (öneri): Hayır; GitHub Discussions (0.5.6'da açılır).** Gerekçe: aranabilir, tek kanal, Talha zamanı ~2 sa/hafta (05 §7). Başka sunucularda yalnız #showcase.
- B: Evet, 1.0'da. 03b §5: blender/unity/flopperam kendi Discord'unu README'ye koydu; anlık destek ve topluluk hissi. Yük: moderasyon.
- C: Evet, 0.6.6 duyurusuyla.

## S3-10. Lisans MIT kalsın mı? (eski S-7)

- **A (öneri): Evet, MIT.** Gerekçe: emsallerin çoğu MIT (blender, unity); Fab ücretsiz kod eklentisi kabul eder; değiştirmek güven kaybı (13 §8).
- B: Apache-2.0 (patent maddesi). Ek karmaşıklık; mevcut sürümler zaten MIT.

## S3-11. Telemetri toplansın mı? (eski S-8)

- **A (öneri): Hayır, hiç.** Gerekçe: blender-mcp itirazı (05 §6); yerel, token'lı sunucu güven iddiası; ölçüm GitHub trafiğinden yapılır (13 §10).
- B: Açık izinli (opt-in), anonim sürüm/araç sayacı. Veri artar; güven ve bakım riski.

## S3-12. Ücretli katman planlansın mı? (eski S-9)

- **A (öneri): Hayır, araç tamamen ücretsiz ve açık kaynak.** Gerekçe: emsallerin hepsi ücretsiz; Talha'nın kuralı (ortak yok, gelir paylaşımı yok) ile tek kişilik operasyon; stüdyo geliri oyunlardan. Araç oyunların vitrinidir.
- B: 1.0 sonrası ücretli destek / danışmanlık sayfası (araç yine ücretsiz).
- C: Pro katman (barındırılan sunucu, özel araçlar).

## S3-13. Show HN ne zaman?

- **A (öneri): 1.0.0'da bir kez.** Gerekçe: 03b §0 #2 — HN çoğu projede ana neden değil; Show HN tek atış, en olgun sürümde kullanılmalı; 1.0'da API donmuş, doküman sitesi canlı.
- B: 0.6.6 büyük duyurusunun 5. günü (Pazar 19:00 ET). X dalgasını ikinci kez yükseltebilir; 1.0'da tekrar Show HN yapılamaz (sürüm notu Show HN değildir, K4).
- C: Hiç.

## S3-14. Demo videolarında ses olsun mu?

- **A (öneri): Ses yok / hafif müzik; altyazı EN.** Gerekçe: X ve Reddit'te video çoğunlukla sessiz izlenir (TAHMİN); çekim ve kurgu tek kişiyle hızlı; dil engeli yok.
- B: Talha'nın İngilizce anlatımı (yalnız YouTube uzun sürüm). Güven ve kişilik artar; kayıt süresi artar.
- C: Yapay ses (TTS) anlatım. Hızlı; ama "AI slop" algısı riski (13 §8 Y-12 ruhuna ters).

## S3-15. Duyurular hangi X hesabından yapılsın?

- **A (öneri): Talha'nın kişisel geliştirici hesabı** (Shanyup Studios hesabı paylaşır). Gerekçe: emsallerde patlayan videolar kişi hesabından (blender, unity, chongdashu — 03b §1); "I built" dili kişiyle inandırıcı.
- B: Shanyup Studios hesabı. Kurumsal görünür; takipçi kitlesi oyun odaklı (TAHMİN).
- C: Yeni @enginewright hesabı. Temiz ürün hesabı; sıfır takipçiyle başlar.
