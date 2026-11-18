# -*- coding: utf-8 -*-
"""10_Ara_Surumler.md üretici + denetim.

Yalnız okur: ../08_Yol_Haritasi_v2.md (§10 tablosu). Yazar: ../10_Ara_Surumler.md.
Denetimler: (1) her ara sürüm <= 12 sa, (2) 08'de Kısmen/Yok olup sürüme bağlı her
EW-xxx en az bir ara sürümde, (3) T-01..T-16 yeri, (4) Y1..Y10 yeri, (5) seri toplamları.
Çalıştır: python research/ara_surum.py
"""
import re, pathlib, sys

KOK = pathlib.Path(__file__).resolve().parent.parent
KG = 6  # 1 kart-gün ≈ 6 sa (TAHMİN; bu belgenin dönüşüm varsayımı)


# (no, seri, hedef, araç/düzeltme, EW listesi, T/Y listesi, ilk kullanan oyun / açtığı oyun işi (iç),
#  kabul (genel test projesi), bağımlılık, sa, soru, oyun doğrulaması (iç))
# Genel MCP kuralı (Talha 18.11, kart t_5f61e99c): hedef, araç, kabul ve şema etiketlerinde oyun adı yok;
# oyun bağlantısı yalnız `oyun` ve `ic` alanlarında (belgede "(iç)" etiketli).
A = []
def s(no, seri, hedef, arac, ew, ty, oyun, kabul, bag, sa, soru="", ic=""):
    A.append(dict(no=no, seri=seri, hedef=hedef, arac=arac, ew=ew, ty=ty, oyun=oyun,
                  kabul=kabul, bag=bag, sa=sa, soru=soru, ic=ic))

# ---------------- 0.4.x  Kararlılık + veri aktarımı ve UI metin denetimi (08 §2, ≈ 35 sa) ----------------
S = "0.4"
s("0.4.1", S, "UI metin taşma denetimini ekle (TextBlock istenen genişlik > ayrılan genişlik).",
  "`text_overflow_audit` (yeni); `asset_find` olmayan klasör uyarısı",
  ["EW-055"], ["T-03", "Y1"],
  "RC LOC-4 → LOC-5 (taşma), IS-34 kapısının LOC kısmı (RC'nin tek \"Yok\" P0'ı)",
  "`tests/`: taşan + taşmayan widget (2 durum) geçer · `asset_find` olmayan klasörde `warning` alanı · test projesinde 1 ekran × 2 dil raporu ekran görüntüsüyle aynı",
  "— (ilk ara sürüm)", 9, "S2-11 (öneri A: 09 kuralları bu sürümden zorunlu)",
  "RC 1 ekran × 2 dil (EN + DE ya da RU) raporu ekran görüntüsüyle aynı")
s("0.4.2", S, "CSV/JSON'dan DataTable aktarımını (upsert + fark + `dryRun`) ekle; \"Var\" araçları gerçek oyun verisiyle dene.",
  "`datatable_import` (yeni, `dryRun`); gerçek proje deneme koşusu (iş)",
  ["EW-002", "EW-001", "EW-005", "EW-006", "EW-050", "EW-051", "EW-052", "EW-053", "EW-060", "EW-061", "EW-111"], ["Y1"],
  "RC Demo D1 IS-10 (48 kart + 4 lanet + 21 sunak), D1 güveni",
  "`tests/`: upsert + fark + bilinmeyen alan reddi · test tablosunda `dryRun` → uygula → `datatable_get` aynı · 10 \"Var\" ihtiyaç için 1'er gerçek proje kaydı",
  "0.4.1", 10, "S2-14 (öneri A: kayıt Enginewright `docs/oyun_kosulari/`)",
  "RC `DT_Cards` dryRun → uygula → `datatable_get` aynı · 10 kayıt `docs/oyun_kosulari/RC_0.4.md`")
s("0.4.3", S, "Import ve çok oyunculu PIE açılışını kıran küçük kararlılık açıklarını kapat.",
  "ASCII dışı ad uyarısı (`asset_import`, `blueprint_create`); `pie_start` bekleme parametresi + belge; `game_run` koşu saklama (son N); `actor_preview` kamera kollu Character çerçevesi; `input_map_list` yavaşlık belge notu",
  [], ["T-04", "T-09", "T-05", "T-06", "T-10", "Y1"],
  "RC Türkçe adlı asset importu, RC IS-33 (2 oyunculu listen açılışı); ARPG varyant panosu zemini",
  "`BP_Çaydanlık` → uyarı · yavaş açılan haritada 2 oyunculu listen açılışı `TIMEOUT` vermeden ayarlı süreyle açılır · N+1. koşuda en eski klasör silinir (test) · Character + SpringArm testinde çerçeve gövdeye oturur",
  "0.4.2", 10, "",
  "RC ana menü 2 oyunculu açılış · RC Türkçe adlı asset importu uyarı verir")
s("0.4.4", S, "Testsiz 7 aracı teste bağla ve 0.4 serisini güvenilirlik turuyla kapat.",
  "7 testsiz araca test (`perf_capture`, `perf_scene`, `perf_gpu_profile`, `mrq_render`, `mrq_status`, `asset_references`, `asset_save`); 5 RELIABILITY satırı; RELIABILITY_0.4.x turu",
  [], ["T-01", "T-02", "Y1"],
  "GP G1 perf ölçümüne güven (0.5.1, 0.5.3 bu araçları kullanır)",
  "`arac_test_kapsami.py`: testsiz 0 (129/129) · RELIABILITY'de 5 yeni satır OK/NOTE · tur dosyası \"Kept for 0.5\" bölümüyle kapanır",
  "0.4.3", 9)

# ---------------- 0.5.x  YZ test ve ölçüm paketi (08 §3, ≈ 55 sa) ----------------
S = "0.5"
s("0.5.0", S, "Başsız automation koşusunu ve ölçüm dosyası okuyucuyu (eşik + kapı özeti) ekle; test standardını başlat.",
  "`automation_run` `headless` + rapor ayrıştırma; `metrics_read` (eşik + kapı özeti; `format` parametresi = oyunun ölçüm dosyası biçimi); test standardı T1–T5 \"bitti\" tanımı, RELIABILITY_0.5.md açılır",
  ["EW-011", "EW-010", "EW-014"], ["Y2"],
  "GP YZ-0 kapı G0; yan: RC IS-32 bot ölçümü, ARPG IC-12",
  "Test projesinde 1 geçen + 1 kalan automation testi ayrışır · örnek `metrics.json` + eşik dosyasıyla geçti/kaldı/eksik · özet < 2 KB",
  "0.4.4", 10, "S2-06 = A (kapandı: genel ad `metrics_read`, dosya biçimi parametre) · S2-02 = A",
  "GP kanıtı 0.5.7'de (S2-02 A); oyun belgesindeki ad `ai_metrics_read`")
s("0.5.1", S, "NavMesh kopuk ada raporu ve stat grubu filtreli ölçümü ekle.",
  "`nav_build_check`; `perf_capture` + `scope` (stat grubu filtresi)",
  ["EW-090", "EW-041"], ["Y2"],
  "GP G1 (M1 takılma, M9 host maliyeti); ARPG IC-10 zemini",
  "Test haritasında 1 bilinçli kopuk ada bulunur · `stat group` filtresiyle yalnız istenen grup döner",
  "0.5.0", 9)
s("0.5.2", S, "Visual Logger kaydını aktör + zaman aralığına göre metne çevir.",
  "`vislog_extract`",
  ["EW-015"], ["Y2"],
  "GP G1 (kırmızı olayın nedeni)",
  "Elle kaydedilmiş küçük `.bvlog` testinden olay satırları çıkar · okuma olmazsa canlı Visual Logger dışa aktarma yolu belgelenir (08 §3 risk 2)",
  "0.5.0", 8)
s("0.5.3", S, "Gameplay Debugger anlık görüntüsü ve paketli oyunda FPS örneklemeyi ekle.",
  "`gdt_snapshot`; `game_perf_sample`",
  ["EW-016", "EW-042"], ["Y2"],
  "GP G1 (utility puanı, M9); RC IS-31 (1080p / 800p pencere ölçümü)",
  "Test projesinde varsayılan kategori metin + PNG · paketli test oyununda ortalama / p95 / takılma sayısı",
  "0.5.1", 11)
s("0.5.4", S, "StateTree hata ayıklayıcı izini metne çevir.",
  "`statetree_debug_dump` (yalnız PIE)",
  ["EW-017"], ["Y2"],
  "GP G2 (M12 titreme nedeni)",
  "Test StateTree'de 3 durum geçişi sırasıyla metinde · paket koşusunda `EDITOR_ONLY` benzeri açık hata (08 §3 risk 3)",
  "0.5.3", 8)
s("0.5.5", S, "Uzun soak koşusunu ve kalıcı çağrı kaydını ekle.",
  "`soak_run` (async + `_status`); çağrı kaydı diske (son 100 sınırı kalkar)",
  ["EW-018"], ["T-16", "Y2"],
  "GP G3 (30 dk soak); RC IS-31 paket kararlılığı",
  "5 dk test koşusu: zaman serisi (bellek, ms, takılma) + çökme özeti · uzun oturumda 100+ çağrı kaydı dosyada",
  "0.5.4", 8)
s("0.5.6", S, "Senaryo yerleştiriciyi ekle ve 0.5 serisini güvenilirlik turuyla kapat.",
  "`scenario_place` (`dryRun`); RELIABILITY_0.5 turu",
  ["EW-019"], ["Y2"],
  "GP S1/S2/S3 test haritaları; **GP YZ-0 başlayabilir** (araçlar test projesinde kabul, S2-02 A)",
  "10 aktörlük liste tek çağrıda yerleşir + `dryRun` listesi · RELIABILITY_0.5 bütün 0.5 araçları OK/NOTE",
  "0.5.5", 8, "S2-13 (öneri A: `scenario_place` burada; B seçilirse 0.6.6'ya kayar)")
s("0.5.7", S, "0.5 araçlarını ilk gerçek oyun projesinde doğrula (kanıt koşusu, araç yok).",
  "Oyun doğrulama koşusu (iş, araç yok)",
  ["EW-011", "EW-014", "EW-015", "EW-016", "EW-017", "EW-018", "EW-041", "EW-042", "EW-090"], ["Y2"],
  "GP YZ-0 G0…G3 kanıtı (YZ-0 ilk haftası)",
  "9 araç için 1'er kayıt (çağrı, süre, sonuç) `docs/oyun_kosulari/` · bulunan açık → 0.5.8 düzeltme ara sürümü (kapsam büyütme yok)",
  "0.5.6 + oyun tarafında test modülü iskeleti (dış). 0.6.x ile paralel", 4, "S2-02 = A · S2-14",
  "GP YZ-0 ilk haftası; kayıt `docs/oyun_kosulari/GP_0.5.md`")

# ---------------- 0.6.x  Paketleme, senaryo ve bot testi (08 §4, ≈ 52 sa) ----------------
S = "0.6"
s("0.6.0", S, "Paketleme ve durum takibini async araç olarak ekle; envanter betiklerini `tools/`'a taşı.",
  "`package_build` + `package_status`; `arac_envanteri.py` / `arac_test_kapsami.py` → `tools/`",
  ["EW-043"], ["Y3"],
  "RC IS-31 (paket + perf), IS-35 (Next Fest build) adım 0; GP/ARPG Demo paketi",
  "Test projesi Win64 Development paketi async biter, editör kilitlenmez · `tools/` betiği: kayıtlı araç = README = test kapsamı",
  "0.5.6 (`game_perf_sample` paket ölçümü için)", 10, "",
  "RC demo paketi elle adım 0")
s("0.6.1", S, "PIE senaryo oynatıcıyı ve bot koşusu kalıbını ekle.",
  "`pie_scenario`; `bot_run` (`game_run` + `metrics_read`)",
  ["EW-012", "EW-013"], ["Y2"],
  "RC IS-21 (kart kuralları), IS-32 (playtest/denge), IS-62; ARPG IC-43; GP bot profilleri",
  "5 adımlı senaryoda 1 bilinçli hata yakalanır · test projesindeki örnek bot komutuyla 1 koşu özeti",
  "0.6.0", 9, "",
  "RC `RC.BotRun` 1 koşu özeti · RC EK-03 bir kural kartı senaryosu")
s("0.6.2", S, "Asset beyanı ve yer tutucu raporunu ekle; yeni araçlara belge şablonunu uygula.",
  "`asset_metadata_set` / `asset_declaration_report`; `placeholder_report`; EWTOOL açıklama şablonu (yeni araçlar)",
  ["EW-075", "EW-074"], ["Y5"],
  "RC IS-35 (beyan), IS-30 ★ (`_Kullanilmayan`); GP K-70. **RC Demo build araçları tamam** (0.6a sınırı)",
  "10 asset metadata yaz-oku · beyan CSV elle tutulan örnek kayıtla aynı · yer tutucu klasöründe oyuncu yolu referansı 0 raporlanır · 0.4–0.6 yeni araç açıklamaları şablona uyar",
  "0.6.1", 9, "S2-07 (Talha notu: ara sürümlere bölündü; 0.6.0–0.6.2 önce = öneri A sırası)",
  "RC `Asset_Kaydi.csv` ile beyan CSV aynı · RC `_Kullanilmayan` oyuncu yolu referans 0")
s("0.6.3", S, "Replikasyon iddiası ve dil × ekran döngüsünü ekle.",
  "`replication_check`; `loc_screen_sweep` (`text_overflow_audit` ile)",
  ["EW-031", "EW-054", "EW-032"], ["Y2"],
  "RC IS-33 (co-op), LOC-4 tam döngü; GP GI-07",
  "2 istemcide bilinçli replike edilmeyen değer bulunur · test projesinde 2 dil × 3 ekran tek çağrı · 2 makine/Steam testi elle kalır (EW-032 notu)",
  "0.6.2 (0.4.1 `text_overflow_audit`)", 9, "",
  "RC 2 dil × 3 ekran tek çağrı · GP GI-07 replikasyon")
s("0.6.4", S, "Veri dışa aktarma ve toplu DataAsset oluşturmayı ekle.",
  "`data_export`; `dataasset_create` (`dryRun`)",
  ["EW-004", "EW-003"], ["Y2"],
  "GP GI-29 (sim köprüsü), GI-02…27 içerik; ARPG IC-12/IC-43, F1; RC IS-32",
  "`datatable_import` ↔ `data_export` gidiş-dönüş 0 kayıp · 20 DataAsset tek çağrı, `validate` temiz",
  "0.6.3 (0.4.2 `datatable_import`)", 8)
s("0.6.5", S, "Kamera turu ve bölge maskeli stil ölçümünü ekle.",
  "`screenshot_tour`; `image_stats` bölge maskesi + palet",
  ["EW-064", "EW-063"], ["Y2"],
  "RC IS-35 mağaza 8 kare, IS-53; RC IS-03 sonrası stil denetimi",
  "Test haritası 5 nokta PNG + manifest · değeri bilinen test görselinde bölge ölçümü beklenenle aynı (±%2)",
  "0.6.4", 9, "",
  "RC `stil_olcu.py` ile aynı sonuç (±%2) · RC mağaza 8 kare")
s("0.6.6", S, "Yerleşim seti modunu ekle, statik CI'ı kur, 0.6 serisini turla kapat.",
  "`scenario_place` yerleşim seti modu (grup + tohum); statik CI (envanter + kapsam + test sözdizimi); RELIABILITY_0.6 turu",
  ["EW-092"], ["T-12", "Y4"],
  "GP GI-03, GI-26 (B1 mekânları)",
  "Test haritasında 1 grup aynı tohumla aynı yerleşimi verir · CI iş akışı yerelde koşar (push ayrı karar) · RELIABILITY_0.6 OK/NOTE",
  "0.6.5 (0.5.6 `scenario_place`)", 8, "S2-08 (öneri A: statik CI şimdi, 1.0 öncesi elle tur) · S2-13",
  "GP GI-03 1 mekân tohumla yerleşir")

# ---------------- 0.7.x  İskelet, animasyon ve malzeme hattı (08 §5, 8–10 kart-gün + 7 sa) ----------------
S = "0.7"
s("0.7.0", S, "FBX'ten iskeletli mesh, Skeleton ve Physics Asset içe aktar.",
  "`skeletal_import` temel (SkeletalMesh + Skeleton + PhysicsAsset)",
  ["EW-071"], ["Y2"],
  "ARPG F0 stil testi (AT EW-1 başı); RC Garmr zemini",
  "1 aile FBX → SkeletalMesh + PhysicsAsset; `validate` temiz; ASCII uyarısı (0.4.3) çalışır",
  "0.6.6 · UE 5.8 hedefi (S2-12 A)", 10)
s("0.7.1", S, "Animasyon dizileri ve soketleri iskelete bağla.",
  "`skeletal_import` anim + soket (AnimSequence, soket ekleme)",
  ["EW-071"], ["Y2"],
  "ARPG F0 (22 aile + 8 boss hattı)",
  "Aynı aile: 5 AnimSequence + 3 soket; `actor_preview` ile kare",
  "0.7.0", 10)
s("0.7.2", S, "IK Retargeter ile animasyonu başka iskelete aktar.",
  "`anim_retarget` (IKRig + Retargeter)",
  ["EW-071"], ["Y2"],
  "ARPG F0; GP YZ-2 retarget + Physical Animation",
  "Kaynak → hedef iskelet 5 animasyon (test projesi)",
  "0.7.1", 12, "",
  "GP YZ-2 bir karakter (GP kanıtı GP YZ-2'de)")
s("0.7.3", S, "Montage, BlendSpace ve anim notify yazarlığını ekle.",
  "`montage_create`, `blendspace_create`, anim notify",
  ["EW-071"], ["Y2"],
  "ARPG F0 (AT EW-1 sonu)",
  "1 montage + 1 blendspace + 2 notify PIE'de oynar",
  "0.7.2", 10)
s("0.7.4", S, "Mesh slotlarına göre malzeme örneği ve doku bağlamayı otomatikleştir.",
  "`material_instances_for_mesh`",
  ["EW-072"], ["Y2"],
  "ARPG F0 (66 düşman varyantı, AT EW-2); RC IS-13 `M_RC_Boya` MI",
  "Mesh slot başına MI + doku; 3 varyant; geri okuma temiz",
  "0.7.3", 12)
s("0.7.5", S, "Varyant panosunu ve sanat bütçesi denetimini ekle; 0.7 serisini kapat.",
  "`actor_preview` `material_overrides` + `anim`/`time`; `art_budget_audit`; RELIABILITY_0.7 turu",
  ["EW-065", "EW-073"], ["Y2"],
  "ARPG F0 varyant panosu; RC 155 asset bütçe denetimi",
  "8×8 varyant panosu tek çağrı · test klasöründe eşik (üçgen, doku) raporu · RELIABILITY_0.7 OK/NOTE",
  "0.7.4 (0.4.3 `actor_preview` çerçevesi)", 10, "",
  "ARPG 66 varyant panosu · RC 155 asset eşik raporu")

# ---------------- 0.8.x  Oynanış yazarlığı (GAS, StateTree, BT, AnimBP) (08 §6, 12–15 kart-gün) ----------------
S = "0.8"
s("0.8.0", S, "GAS attribute set okumayı ve GameplayEffect oluşturmayı ekle.",
  "`attribute_set_info`, `effect_create`",
  ["EW-082"], ["Y2"],
  "ARPG F0 (AT EW-5 başı); IC-02 attribute set zemini",
  "Test projesinde 1 attribute set okunur · 2 Effect oluşur, geri okuma temiz",
  "0.7.5", 10)
s("0.8.1", S, "GameplayAbility oluşturmayı ekle.",
  "`ability_create`",
  ["EW-082"], ["Y2"],
  "ARPG F0 (36 skill hattı)",
  "1 Ability oluşur, Effect bağlanır, PIE'de verilir",
  "0.8.0", 10)
s("0.8.2", S, "GameplayCue ve GAS iddia testini ekle.",
  "`cue_create`, `gas_test`",
  ["EW-082"], ["Y2"],
  "ARPG IC-02 attribute set; AT EW-5 bitti",
  "1 Ability + 2 Effect + 1 Cue PIE'de uygulanır; `gas_test` attribute değişimini iddia eder (1 bilinçli kalan)",
  "0.8.1", 10)
s("0.8.3", S, "Şablondan StateTree oluştur.",
  "`statetree_create` (şablon kopyala + parametre; serbest grafik yok)",
  ["EW-080"], ["Y2"],
  "ARPG F0 (22 düşman ailesi; AT EW-3 başı); GP StateTree kullanıyor (YZ_v2/02 §3), yeni aileler için kullanabilir",
  "1 şablon JSON → StateTree; `validate` temiz",
  "0.8.2 (0.5.4 `statetree_debug_dump` ortak altyapı)", 12, "S2-05 kapandı (genel araç, Talha 18.11)",
  "ARPG 1 düşman ailesi şablonu")
s("0.8.4", S, "StateTree'ye düğüm ekle ve PIE'de durum geçişini kanıtla.",
  "`statetree_node_add`",
  ["EW-080"], ["Y2"],
  "**ARPG F1 grind prototipi başlayabilir** (AT: ilk dört EW prototipten önce)",
  "PIE'de 3 durum; `statetree_debug_dump` geçişleri sırasıyla gösterir",
  "0.8.3", 10, "S2-05 kapandı")
s("0.8.5", S, "Behavior Tree oluşturmayı ekle.",
  "`behaviortree_create`",
  ["EW-080"], ["Y2"],
  "ARPG boss fazları; RC Garmr (P2)",
  "1 BT + Blackboard; `validate` temiz; PIE'de görev çalışır",
  "0.8.4", 8, "S2-05 kapandı")
s("0.8.6", S, "İskelet ailesi için AnimBP state machine şablonunu kur.",
  "`animbp_statemachine_template` (şablon kopyala)",
  ["EW-081"], ["Y2"],
  "ARPG F1 (5 iskelet ailesi; AT EW-4)",
  "1 iskelet ailesi: Idle/Move/Attack/Death durumları derlenir",
  "0.8.5 (0.7.3 montage/blendspace)", 12)
s("0.8.7", S, "AnimBP varyant parametrelerini ekle; 0.8 serisini kapat.",
  "`animbp_statemachine_template` varyant parametresi; RELIABILITY_0.8 turu",
  ["EW-081"], ["Y2"],
  "ARPG F1 (ikinci aile tekrar işi yok)",
  "2. ailede yalnız parametreyle aynı şablon çalışır · RELIABILITY_0.8 OK/NOTE",
  "0.8.6", 10)

# ---------------- 0.9.x  Seviye, VFX, Sequencer + headless (08 §7; dondurma adayı dahil) ----------------
S = "0.9"
s("0.9.0", S, "Oda parçası bağlantı noktası ve ızgara doğrulayıcıyı ekle.",
  "`room_validate` (port + ızgara)",
  ["EW-091"], ["Y2"],
  "ARPG F1 IC-10 (tek kit gri kutu; AT EW-7 başı)",
  "1 kit × 5 oda; bilinçli bozuk port bulunur",
  "0.8.7", 12)
s("0.9.1", S, "Kat doğrulamasını NavMesh denetimine bağla.",
  "`room_validate` kat modu + `nav_build_check` zinciri",
  ["EW-091", "EW-090"], ["Y2"],
  "ARPG IC-10 kat erişilebilirliği",
  "5 odalık katta ulaşılamaz nokta raporu; kopuk ada 1 bilinçli örnekte bulunur",
  "0.9.0 (0.5.1 `nav_build_check`)", 10)
s("0.9.2", S, "İçerik duman testi ve fizik/ragdoll doğrulamasını ekle.",
  "`content_smoke`; `physics_test`",
  ["EW-007", "EW-084"], ["Y2"],
  "ARPG IC-68 (İçerik Testi haritası); GP YZ-1 ragdoll, K-106 sekme",
  "50 satırlık tabloda 1 bilinçli bozuk satır bulunur · ragdoll + sekme 3 tekrarda sapma raporu",
  "0.9.1 (0.6.1 `pie_scenario`)", 10)
s("0.9.3", S, "Niagara sistem bilgisini ve parametre yazmayı ekle.",
  "`niagara_info` + parametre okuma/yazma",
  ["EW-083"], ["Y2"],
  "ARPG Dikey dilim IC-16 (AT EW-6 başı)",
  "1 sistemin kullanıcı parametreleri okunur, 1 renk yazılır, geri okuma temiz",
  "0.9.2", 12)
s("0.9.4", S, "Niagara element varyantı ve önizleme panosunu ekle.",
  "`niagara_variant` + önizleme panosu",
  ["EW-083"], ["Y2"],
  "ARPG IC-16 (skill + element + aura VFX)",
  "1 sistem → 4 element varyantı + pano",
  "0.9.3 (0.7.5 pano)", 12)
s("0.9.5", S, "Bütün araçlara \"editor only / headless ok\" bayrağını ekle.",
  "S9 bayrağı (açıklama + envanter betiği sütunu)",
  ["EW-110"], ["Y8"],
  "ARPG F3 headless hazırlığı (AT EW-9 başı)",
  "`tools/` envanteri her araçta bayrak gösterir; bayraksız 0",
  "0.9.4", 8, "S2-11")
s("0.9.6", S, "Commandlet + MCP iskeletiyle editör kapalıyken araç çağır.",
  "Headless iskelet (commandlet, kayıt defteri, MCP taşıma)",
  ["EW-110"], ["Y8"],
  "ARPG F3",
  "Editör kapalı: `ping` + 1 salt okunur araç (ör. `asset_find`) commandlet'ten yanıt verir",
  "0.9.5", 12)
s("0.9.7", S, "Headless toplu içe aktarmayı çalıştır.",
  "Headless `asset_import` (+ `batch`)",
  ["EW-110", "EW-070"], ["Y8"],
  "ARPG F3 prodüksiyon importu",
  "Editör kapalı 100 asset import; rapor editör içiyle aynı",
  "0.9.6", 12)
s("0.9.8", S, "Headless veri araçlarını çalıştır ve eşitlik raporu ver.",
  "Headless `datatable_import`, `data_export`, `validate`",
  ["EW-110", "EW-002", "EW-004"], ["Y8"],
  "ARPG F3; RC/GP gece veri koşuları",
  "Editör kapalı `datatable_import` + `data_export` gidiş-dönüş; editör içi sonuçla fark 0",
  "0.9.7", 10)
s("0.9.9", S, "Level Sequence ve kamera kurmayı ekle.",
  "`sequence_create` + kamera",
  ["EW-100"], ["Y2"],
  "ARPG ara sahne (AT EW-11 başı); RC EA IS-53",
  "1 sekans + 2 kamera; geri okuma temiz",
  "0.9.8", 10)
s("0.9.10", S, "Sekansa kesme ekle ve MRQ ile çıktı al.",
  "`sequence_create` kesme + `mrq_render` zinciri",
  ["EW-100", "EW-101"], ["Y2"],
  "ARPG ara sahne; RC IS-53 (trailer/sekans)",
  "1 ara sahne: 2 kamera, 1 kesme; `mrq_render` video çıktısı",
  "0.9.9", 8)
s("0.9.11", S, "Hata kodlarını düzenle ve hata kodu sözlüğünü yaz.",
  "Düz `NOT_FOUND` → `<NESNE>_NOT_FOUND`; hata kodu sözlüğü (H6)",
  [], ["T-07", "T-08", "Y9"],
  "Bütün oyun ajanları (hata kodu tahmin edilebilir)",
  "Kaynak grep: düz `NOT_FOUND` 0 · sözlükte kaynaktaki bütün kodlar (betikle sayım eşit)",
  "0.9.10", 10)
s("0.9.12", S, "API dondurma adayını çıkar: ad ve şema listesi, uymayanlara takma ad.",
  "Ad/şema listesi; 09 §1 S1/S10'a uymayanlar yeniden ad + eski ad takma ad",
  [], ["Y9"],
  "Bütün oyunlar (1.x içinde ad kırılmaz)",
  "Liste betikle üretilir · eski adla çağrı uyarıyla çalışır (test)",
  "0.9.11", 10, "S2-11 · S2-06 = A (eski ad takma ad)")
s("0.9.13", S, "Motor sürümü korumasını ekle ve ikinci UE sürümünde derle.",
  "`ENGINE_MAJOR/MINOR_VERSION` korumaları; UE 5.7 derlemesi",
  [], ["T-11", "Y7"],
  "Oyun dışı (eklenti kullanıcıları); 0.7 retarget riskini erken görür",
  "5.8 + 5.7 temiz derleme · sürüme bağlı kırılma listesi",
  "0.9.12", 12, "S2-09 (öneri A: 5.8 + 5.7)")
s("0.9.14", S, "Eski araçların ilk yarısını belge şablonuna geçir.",
  "EWTOOL açıklama şablonu: araçlar 1–65 (alfabetik)",
  [], ["T-13", "Y5"],
  "Bütün oyun ajanları (`describe` çıktısı tutarlı)",
  "65 aracın açıklaması şablon denetiminden geçer (betik)",
  "0.9.13", 10)
s("0.9.15", S, "Eski araçların ikinci yarısını belge şablonuna geçir; README tablosunu açıklamadan üret.",
  "EWTOOL şablonu: araçlar 66–129 + 0.4–0.9 yeni araçlar; README \"Tools\" tablosu",
  [], ["T-13", "Y5"],
  "Bütün oyun ajanları",
  "Bütün araçlar şablonda (betik: uymayan 0) · README satır sayısı = kayıtlı araç sayısı",
  "0.9.14", 10)
s("0.9.16", S, "Dondurma adayında tam güvenilirlik turu yap.",
  "RELIABILITY_0.9 (bütün araçlar)",
  [], ["Y1", "Y2"],
  "Bütün oyunlar (1.0 öncesi temel)",
  "Bütün araçlar OK/FIXED/NOTE + süre · \"Kept for 1.0\" listesi",
  "0.9.15", 12)

# ---------------- 1.0.x  Dondurma + cila (08 §8, ≈ 40 sa) ----------------
S = "1.0"
s("1.0.0-rc.1", S, "Örnek projeyi kur.",
  "Örnek proje (test projesi temelli; veri, test, sanat iş akışlarına uygun içerik)",
  [], ["Y6"],
  "Oyun dışı (yeni kullanıcı); oyun ajanları için ortak örnek",
  "Temiz klonda örnek proje açılır, `smoke.py` geçer",
  "0.9.16", 10)
s("1.0.0-rc.2", S, "Üç iş akışı tarifini yaz: veri, test, sanat.",
  "3 tarif (veri: import → validate; test: automation → metrics; sanat: import → MI → pano)",
  [], ["Y6"],
  "RC / GP / ARPG iş akışı belgeleri",
  "Her tarif örnek projede baştan sona koşar (kayıt)",
  "1.0.0-rc.1", 8)
s("1.0.0-rc.3", S, "Çoklu UE testini ve sürüm öncesi denetim listesini bitir.",
  "`run_all.py` 5.7 + 5.8; sürüm kontrol listesi (elle tur)",
  [], ["Y7", "Y4"],
  "Oyun dışı",
  "İki sürümde `run_all.py` sonucu · kontrol listesi CHANGELOG öncesi doldurulur",
  "1.0.0-rc.2", 10, "S2-09 · S2-08 (öneri A: 1.0 öncesi C = elle tur listesi)")
s("1.0.0-rc.4", S, "Gerçek oyun projelerinde son kanıt koşusunu yap, 07 durumlarını güncelle.",
  "Kanıt koşusu (iş): bütün \"Var\" ihtiyaçlar + 0.4–0.9 araçları",
  ["EW-005", "EW-006", "EW-020", "EW-030", "EW-040", "EW-050", "EW-051", "EW-052", "EW-053", "EW-060", "EW-061", "EW-062", "EW-070", "EW-101", "EW-111"], ["Y2"],
  "RC / GP / ARPG: aracın oyunda çalıştığı kanıtı",
  "Her \"Var\" ihtiyaç için 1 kayıt `docs/oyun_kosulari/` · 07 tablosu güncel",
  "1.0.0-rc.3", 10, "S2-14",
  "RC, GP, ARPG projelerinde birer kayıt dosyası")
s("1.0.0", S, "API'yi dondur ve 1.0'ı kapat.",
  "RELIABILITY_1.0; Y9 dondurma onayı (takma adlar kalır); CHANGELOG 1.0",
  [], ["Y1", "Y9"],
  "Bütün oyunlar (1.x içinde ad/şema kırılmaz)",
  "RELIABILITY_1.0: bütün araçlar OK/FIXED, NOTE yalnız belgelenmiş motor sınırı · ad/şema listesi 0.9.12 ile fark 0",
  "1.0.0-rc.4", 8)

# (nerede, kilometre taşı = genel yetenek, ilk kullanan oyun / açtığı oyun işi (iç))
KILOMETRE = [
    ("0.4.4 sonrası", "Veri aktarımı + UI metin denetimi + kararlılık turu tamam", "RC Demo D1 araç güveni (IS-10, LOC-4/5)"),
    ("0.5.6 sonrası", "YZ test ve ölçüm paketi test projesinde kabul (başsız test, ölçüm, nav, vislog, GDT, StateTree izi, soak, senaryo)", "GP YZ-0 başlayabilir (G0…G3)"),
    ("0.6.2 sonrası", "Demo build hattı araçları tamam (paket, senaryo/bot, asset beyanı, yer tutucu)", "RC Demo / Next Fest build araçları tamam (eski \"0.6a\")"),
    ("0.8.4 sonrası", "Oynanış prototipi araçları tamam (iskelet/anim/malzeme + GAS + StateTree)", "ARPG F1 grind prototipi başlayabilir (AT: EW-1, 2, 5, 3 bitti)"),
    ("0.9.16 sonrası", "API dondurma adayı + headless hazır", "—"),
    ("1.0.0 sonrası", "**YAYIN İŞİ (Y10)** — kapsam dışı (04, 05, SORULAR S-0…S-10)", "—"),
]

# seri: (ad = Talha onaylı genel ad, 08 bölümü, 08 TAHMİN)
SERI = {
    "0.4": ("Kararlılık + veri aktarımı ve UI metin denetimi", "08 §2", "≈ 35 sa"),
    "0.5": ("YZ test ve ölçüm paketi", "08 §3", "≈ 55 sa"),
    "0.6": ("Paketleme, senaryo ve bot testi", "08 §4", "≈ 52 sa"),
    "0.7": ("İskelet, animasyon ve malzeme hattı", "08 §5", "8–10 kart-gün + 7 sa ≈ 55–67 sa"),
    "0.8": ("Oynanış yazarlığı (GAS, StateTree, BT, AnimBP)", "08 §6", "12–15 kart-gün ≈ 72–90 sa"),
    "0.9": ("Seviye, VFX, Sequencer + headless", "08 §7", "18–21 kart-gün + 10 sa ≈ 118–136 sa"),
    "1.0": ("Dondurma + cila", "08 §8", "≈ 40 sa"),
}

# (soru, konu, etkilediği ara sürüm, yazılan seçenek / durum)
SORU = [
    ("S2-03", "Oyun belgelerindeki iki eski numara listesi", "Ara sürüm etkisi yok (oyun reposu belgesi; ayrı oyun kartı)", "A (öneri)"),
    ("S2-04", "Oyun belgelerinde eski durum notları (T-14)", "Ara sürüm etkisi yok; 0.4.2 ve 0.5.7 kanıt kayıtları düzeltme kaynağı olur", "A (öneri)"),
    ("S2-05", "StateTree yazma kimin", "0.8.3, 0.8.4, 0.8.5: genel araç, her oyun kullanabilir", "**Kapandı** (Talha 18.11, genel MCP kuralı)"),
    ("S2-06", "Genel araç adları", "0.5.0 (`metrics_read` + biçim parametresi), 0.9.12 (takma adlar)", "**Kapandı = A** (Talha 18.11, genel MCP kuralı)"),
    ("S2-07", "0.6 bölme", "Talha notuyla karşılandı: 0.6.0–0.6.6; demo build hattı önce (0.6.0–0.6.2 = eski 0.6a)", "A sırası"),
    ("S2-08", "CI", "0.6.6 (statik CI), 1.0.0-rc.3 (elle tur listesi)", "A (öneri)"),
    ("S2-09", "Çoklu UE sürümleri", "0.9.13, 1.0.0-rc.3 (B seçilirse 5.6 için +1 ara sürüm)", "A (öneri)"),
    ("S2-10", "03'ten kalan araçlar", "Ara sürüm yok; Sonrası listesi (B seçilirse 1.0.0-rc öncesine 1 ara sürüm)", "A (öneri)"),
    ("S2-11", "09 kuralları zorunlu mu", "0.4.1'den itibaren her yeni araç; 0.9.5, 0.9.12", "A (öneri)"),
    ("S2-13", "`scenario_place` 0.5'te mi", "0.5.6 (B seçilirse 0.6.6'ya taşınır: 0.5.6 3 sa, 0.6.6 13 sa → 0.6.6 ikiye bölünür)", "A (öneri)"),
    ("S2-14", "Kanıt kayıtları yeri", "0.4.2, 0.5.7, 1.0.0-rc.4", "A (öneri)"),
]


def oku_08():
    t = (KOK / "08_Yol_Haritasi_v2.md").read_text(encoding="utf-8")
    sat = re.findall(r"^\| (EW-\d{3}) \| .*? \| (Var|Kısmen|Yok) \| (.*?) \|$", t, re.M)
    return {i: (d, v) for i, d, v in sat}


def denetle():
    hata = []
    for a in A:
        if a["sa"] > 12:
            hata.append(f"{a['no']} {a['sa']} sa > 12")
    ew08 = oku_08()
    kapsam = {e for a in A for e in a["ew"]}
    eksik = [i for i, (d, v) in ew08.items() if d != "Var" and "Sonrası" != v.strip() and i not in kapsam]
    # Sonrası-only (EW-102) ve elle (EW-032 parça) hariç
    eksik = [i for i in eksik if ew08[i][1].strip() != "Sonrası"]
    tset = {t for a in A for t in a["ty"]}
    t_eksik = [f"T-{n:02d}" for n in range(1, 17) if f"T-{n:02d}" not in tset and n not in (14, 15)]
    y_eksik = [f"Y{n}" for n in range(1, 10) if f"Y{n}" not in tset]
    nolar = [a["no"] for a in A]
    if len(nolar) != len(set(nolar)):
        hata.append("çift numara")
    return hata, eksik, t_eksik, y_eksik, ew08, kapsam


def md():
    hata, eksik, t_eksik, y_eksik, ew08, kapsam = denetle()
    toplam = sum(a["sa"] for a in A)
    seri_sa = {}
    for a in A:
        seri_sa.setdefault(a["seri"], []).append(a)
    L = []
    w = L.append
    w("# 10 — Ara sürümler (08 yol haritası → küçük, sıralı adımlar)")
    w("")
    w("Kart: `t_035891ab` · 18.11.2026 · Yalnız belge. Üretici + denetim: `research/ara_surum.py` (tablolar betikten). Genel ad düzeltmesi: kart `t_5f61e99c` (18.11).")
    w("Talha (18.11): *\"İlerlemeyi tamamen ara versiyonlar hâlinde yaz; iş yaparken ona göre yapacağız.\"*")
    w("Girdi: 08 (büyük sürümler), 06/07 (EW-xxx), 09 (T-xx, kurallar), SORULAR_2 (S2-01/02/12 = A). **Tarih yok; sıra var.**")
    w("")
    w("Okuma kuralları:")
    w("- Bir ara sürüm = 1–2 iş günü, **≤ 12 sa TAHMİN** (ajan + Talha onayı). Betik 12'yi aşanı reddeder.")
    w(f"- Kart-gün dönüşümü (08 §5–§7 eforları): **1 kart-gün ≈ {KG} sa (TAHMİN, bu belgenin varsayımı)**; AT kart-gün tanımı vermiyor.")
    w("- Cila şeridi (Y1–Y9) ayrı şerit değil: işleri ara sürümlerin içinde (§5 eşleme). Y10 YAYIN İŞİ yalnız kilometre taşı satırı.")
    w("- Her ara sürüm 09 §3.2 \"bitti\" tanımına uyar (T1 test, T3 oyun doğrulaması, T4 RELIABILITY satırı); kabul ölçütü genel test projesi/örnek üzerinden yazılır, oyun kanıtı ayrı satırdır: **\"Oyun doğrulaması (iç)\"**.")
    w("- **Genel MCP kuralı (Talha 18.11):** sürüm/ara sürüm başlığı, hedef, araç adı, kabul ölçütü ve şema etiketlerinde oyun adı geçmez. Oyun bağlantısı yalnız iç planlama alanlarında: **\"İlk kullanan oyun / açtığı oyun işi (iç)\"** ve **\"Oyun doğrulaması (iç)\"**. Denetim: `research/oyun_adi_denetim.py`.")
    w("- Açık soru (S2-03, 04, 07…11, 13, 14; S2-05 ve S2-06 kapandı) etkilediği satırda **\"S2-xx cevabına bağlı\"**; satır önerilen seçenekle yazıldı. Karar verilmedi.")
    w("- Seri sonu ara sürümü RELIABILITY turuyla kapanır; tur \"Kept for\" listesi bir sonraki serinin ilk ara sürümüne girer (09 §6).")
    w("- Bir ara sürümde açık çıkarsa düzeltme yeni numara alır (ör. 0.4.4 sonrası bulunan açık → 0.4.5); kapsam büyütülmez.")
    w("")
    w("## 1. Sayılar")
    w("")
    w("| | |")
    w("|---|---|")
    w(f"| Ara sürüm | **{len(A)}** (0.4.x {len(seri_sa['0.4'])} · 0.5.x {len(seri_sa['0.5'])} · 0.6.x {len(seri_sa['0.6'])} · 0.7.x {len(seri_sa['0.7'])} · 0.8.x {len(seri_sa['0.8'])} · 0.9.x {len(seri_sa['0.9'])} · 1.0.x {len(seri_sa['1.0'])}) |")
    w(f"| Toplam efor TAHMİN | **≈ {toplam} sa** (en büyük ara sürüm {max(a['sa'] for a in A)} sa, en küçük {min(a['sa'] for a in A)} sa) |")
    w(f"| Kapsam denetimi | 08'de Kısmen/Yok olup sürüme bağlı EW: ara sürümsüz **{len(eksik)}**{(' ' + ', '.join(eksik)) if eksik else ''} · T-01…T-16 yersiz **{len(t_eksik)}** (T-14 oyun kartı, T-15 not) · Y1–Y9 yersiz **{len(y_eksik)}** |")
    w(f"| Betik hatası | {len(hata)} {'; '.join(hata)} |")
    w("")
    w("### 1.1 Seri toplamı ↔ 08 tahmini")
    w("")
    w("| Seri | Ad | 08 karşılığı | 08 TAHMİN | Ara sürüm | Bu belge TAHMİN |")
    w("|---|---|---|---|---|---|")
    for k, (ad, bol, t) in SERI.items():
        ar = seri_sa[k]
        w(f"| {k}.x | {ad} | {bol} | {t} | {ar[0]['no']} … {ar[-1]['no']} ({len(ar)}) | {sum(a['sa'] for a in ar)} sa |")
    w("")
    w("Fark notu: 0.4.x ve 0.5.x 08'den biraz büyük (RELIABILITY turu, T-16, 0.5.7 oyun doğrulama koşusu ayrı satır). 0.9.x 08'den büyük: Y5 (129 araç belge), Y7, T-07/T-08 ve tam tur 0.9'a yerleşti (08'de 1.0 cila şeridindeydi). 1.0.x bu yüzden küçük.")
    w("")
    w("## 2. Kritik yol şeması")
    w("")
    w("```mermaid")
    w("flowchart LR")
    w("  subgraph S04[\"0.4.x Kararlılık + veri + UI metin\"]")
    w("    a041[0.4.1 taşma] --> a042[0.4.2 datatable_import] --> a043[0.4.3 kararlılık] --> a044[0.4.4 test borcu]")
    w("  end")
    w("  subgraph S05[\"0.5.x YZ test ve ölçüm\"]")
    w("    a050[0.5.0 automation+metrics] --> a051[0.5.1 nav+scope] --> a053[0.5.3 gdt+FPS] --> a054[0.5.4 StateTree izi] --> a055[0.5.5 soak] --> a056[0.5.6 scenario]")
    w("    a050 --> a052[0.5.2 vislog] --> a053")
    w("    a056 -.->|paralel| a057[0.5.7 oyun doğrulaması]")
    w("  end")
    w("  subgraph S06[\"0.6.x Paketleme, senaryo, bot\"]")
    w("    a060[0.6.0 paket] --> a061[0.6.1 senaryo+bot] --> a062[0.6.2 beyan] --> a063[0.6.3 ağ+loc] --> a064[0.6.4 veri] --> a065[0.6.5 tur+stil] --> a066[0.6.6 yerleşim+CI]")
    w("  end")
    w("  subgraph S07[\"0.7.x İskelet, anim, malzeme\"]")
    w("    a070[0.7.0 iskelet] --> a071[0.7.1 anim] --> a072[0.7.2 retarget] --> a073[0.7.3 montage] --> a074[0.7.4 MI] --> a075[0.7.5 pano]")
    w("  end")
    w("  subgraph S08[\"0.8.x Oynanış yazarlığı\"]")
    w("    a080[0.8.0-0.8.2 GAS] --> a083[0.8.3-0.8.4 StateTree] --> a085[0.8.5 BT] --> a086[0.8.6-0.8.7 AnimBP]")
    w("  end")
    w("  subgraph S09[\"0.9.x Seviye, VFX, Sequencer + headless\"]")
    w("    a090[0.9.0-0.9.1 oda] --> a092[0.9.2 smoke+fizik] --> a093[0.9.3-0.9.4 Niagara] --> a095[0.9.5-0.9.8 headless] --> a099[0.9.9-0.9.10 Sequencer] --> a0911[0.9.11-0.9.16 cila]")
    w("  end")
    w("  subgraph S10[\"1.0.x Dondurma + cila\"]")
    w("    r1[rc.1-rc.2 örnek] --> r3[rc.3 çoklu UE] --> r4[rc.4 kanıt] --> v100[1.0.0] -.-> YAYIN[YAYIN İŞİ]")
    w("  end")
    w("  a044 --> a050")
    w("  a056 --> a060")
    w("  a066 --> a070")
    w("  a075 --> a080")
    w("  a086 --> a090")
    w("  a0911 --> r1")
    w("  a062 ==>|demo build hattı| M1((Demo build araçları))")
    w("  a056 ==>|test paketi kabul| M2((YZ test paketi))")
    w("  a083 ==>|prototip araçları| M3((Oynanış prototipi))")
    w("```")
    w("")
    w("ASCII (`─►` kritik yol, `═►` kilometre taşı, `└►` yan dal):")
    w("")
    w("```")
    w("0.4.1 ─► 0.4.2 ─► 0.4.3 ─► 0.4.4 ─► 0.5.0 ─► 0.5.1 ─► 0.5.3 ─► 0.5.4 ─► 0.5.5 ─► 0.5.6 ─┬► 0.6.0 ─► 0.6.1 ─► 0.6.2 ═► demo build hattı")
    w("UI metin    veri                      auto+    nav+     gdt+     ST izi   soak    │")
    w("                                      └► 0.5.2 vislog (0.5.3'ten önce)              ├═► YZ test paketi kabul")
    w("                                                                                    └► 0.5.7 oyun doğrulaması (paralel)")
    w("0.6.2 ─► 0.6.3 ─► 0.6.4 ─► 0.6.5 ─► 0.6.6 ─► 0.7.0 … 0.7.5 ─► 0.8.0 … 0.8.4 ═► oynanış prototipi araçları")
    w("                                                                         └► 0.8.5 ─► 0.8.6 ─► 0.8.7")
    w("0.8.7 ─► 0.9.0 … 0.9.10 (oda, smoke, Niagara, headless, Sequencer) ─► 0.9.11 … 0.9.16 (cila) ─► 1.0.0-rc.1 … rc.4 ─► 1.0.0 ═► YAYIN İŞİ")
    w("```")
    w("")
    w("Paralel yapılabilenler (kritik yolda değil): 0.5.2 (0.5.0'dan sonra, 0.5.3'ten önce), 0.5.7 (oyun doğrulaması, dışa bağlı), 0.6.3–0.6.5 (demo build hattını beklemez), 0.9.13 (UE 5.7 derleme; 0.9.12'den bağımsız başlatılabilir).")
    w("")
    w("### 2.1 Kilometre taşları")
    w("")
    w("| Nerede | Kilometre taşı (yetenek) | İlk kullanan oyun / açtığı oyun işi (iç) |")
    w("|---|---|---|")
    for n, k, o in KILOMETRE:
        w(f"| {n} | {k} | {o} |")
    w("")
    w("## 3. Sıralı tablo")
    w("")
    w("| # | Ara sürüm | Hedef | EW / T / Y | İlk kullanan oyun / açtığı oyun işi (iç) | Bağımlılık | TAHMİN | Açık soru |")
    w("|---|---|---|---|---|---|---|---|")
    for i, a in enumerate(A, 1):
        ids = ", ".join(a["ew"][:4] + (["…"] if len(a["ew"]) > 4 else []) + a["ty"])
        w(f"| {i} | **{a['no']}** | {a['hedef']} | {ids} | {a['oyun']} | {a['bag']} | {a['sa']} sa | {a['soru'] or '—'} |")
    w("")
    w("## 4. Tam liste")
    w("")
    son = None
    for a in A:
        if a["seri"] != son:
            son = a["seri"]
            ad, bol, t = SERI[son]
            w(f"### {son}.x — {ad} ({bol}; 08 TAHMİN {t})")
            w("")
        w(f"#### {a['no']}")
        w("")
        w(f"- **Hedef:** {a['hedef']}")
        w(f"- **Araç / düzeltme:** {a['arac']}")
        w(f"- **İhtiyaç / borç / cila:** {', '.join(a['ew'] + a['ty']) or '—'}")
        w(f"- **İlk kullanan oyun / açtığı oyun işi (iç):** {a['oyun']}")
        w(f"- **Kabul ölçütü (genel test projesi):** {a['kabul']}")
        if a["ic"]:
            w(f"- **Oyun doğrulaması (iç):** {a['ic']}")
        w(f"- **Bağımlılık:** {a['bag']}")
        w(f"- **Efor TAHMİN:** {a['sa']} sa")
        if a["soru"]:
            w(f"- **İlgili S2 sorusu:** {a['soru']}")
        w("")
    w("## 5. Cila şeridi → ara sürüm (ayrı şerit kalmadı)")
    w("")
    w("| Cila | Ara sürümler |")
    w("|---|---|")
    ad = {"Y1": "Kararlılık", "Y2": "Test standardı (her yeni araç T1–T5)", "Y3": "Envanter + kapsam betiği `tools/`",
          "Y4": "CI", "Y5": "Belge şablonu", "Y6": "Örnek proje + tarifler", "Y7": "Çoklu UE", "Y8": "Headless",
          "Y9": "API dondurma"}
    for y in [f"Y{n}" for n in range(1, 10)]:
        nl = [a["no"] for a in A if y in a["ty"]]
        if y == "Y2":
            nl = [f"{nl[0]} … {nl[-1]}: yeni araç ekleyen her ara sürüm ({len(nl)} ara sürüm)"]
        w(f"| {y} {ad[y]} | {', '.join(nl)} |")
    w("| Y10 YAYIN İŞİ | Kilometre taşı (1.0.0 sonrası); bu belge ayrıntı vermez |")
    w("")
    w("### 5.1 Teknik borç (09 §5) → ara sürüm")
    w("")
    w("| Borç | Ara sürüm |")
    w("|---|---|")
    for n in range(1, 17):
        t = f"T-{n:02d}"
        nl = [a["no"] for a in A if t in a["ty"]]
        if n == 14:
            nl = ["oyun reposu kartı (S2-04)"]
        if n == 15:
            nl = ["08/09 kartında not edildi"]
        w(f"| {t} | {', '.join(nl)} |")
    w("")
    w("## 6. İhtiyaç (EW-xxx) → ara sürüm (08 §10 denetimi)")
    w("")
    w("| ID | 08 durumu | 08 sürümü | Ara sürüm(ler) |")
    w("|---|---|---|---|")
    for i, (d, v) in sorted(ew08.items()):
        nl = [a["no"] for a in A if i in a["ew"]]
        ek = ""
        if i == "EW-102":
            ek = "Sonrası (08 §9)"
        if i in ("EW-032",):
            ek = " + 2 makine elle (Sonrası)"
        if i == "EW-042":
            ek = " + Steam Deck elle (Sonrası)"
        w(f"| {i} | {d} | {v.strip()} | {', '.join(nl) if nl else ''}{ek} |")
    w("")
    w("## 7. Açık sorular → etkilediği ara sürüm")
    w("")
    w("| Soru | Konu | Etkilediği ara sürüm | Yazılan seçenek / durum |")
    w("|---|---|---|---|")
    for q in SORU:
        w(f"| {q[0]} | {q[1]} | {q[2]} | {q[3]} |")
    w("")
    w("Yeni karar sorusu: **S2-15** (SORULAR_2 sonunda) — ara sürüm numaralarının CHANGELOG/etiket karşılığı.")
    w("")
    w("## 8. İş yaparken kullanım")
    w("")
    w("1. Sıradaki ara sürümü §3'ten al. Bir kart = bir ara sürüm.")
    w("2. Kart gövdesine §4'teki hedef, kabul ölçütü, oyun doğrulaması (iç) ve TAHMİN'i kopyala.")
    w("3. Kabul ölçütü + 09 §3.2 \"bitti\" tanımı karşılanınca kartı kapat.")
    w("4. Açık çıkarsa düzeltmeyi yeni ara sürüm numarasıyla sıraya ekle; mevcut ara sürümü büyütme.")
    w("5. Seri sonu (0.4.4, 0.5.6, 0.6.6, 0.7.5, 0.8.7, 0.9.16, 1.0.0) RELIABILITY turunu içerir.")
    w("")
    return "\n".join(L), hata, eksik, t_eksik, y_eksik, toplam


if __name__ == "__main__":
    metin, hata, eksik, t_eksik, y_eksik, toplam = md()
    (KOK / "10_Ara_Surumler.md").write_text(metin, encoding="utf-8")
    print("ara surum:", len(A), "toplam sa:", toplam)
    print("hata:", hata, "eksik EW:", eksik, "eksik T:", t_eksik, "eksik Y:", y_eksik)
    sys.exit(1 if (hata or eksik or t_eksik or y_eksik) else 0)
