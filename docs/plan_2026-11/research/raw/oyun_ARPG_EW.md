# ARPG Tasarim §12.3 + T1/T3/T4
- [ ] **T1 Motor** [AÇIK] — Öneri: **UE 5.8**. Neden: RagnaCook'tan bilgi + Enginewright MCP + GAS (yetenek/stat sistemi hazır) + Niagara. Alternatif Unity 6 (idle-factory deneyimi), Godot 4 (Halls of Torment kullandı).
- [ ] **T2 Asset yolu** [AÇIK] — Öneri: **Hibrit.** Ortam kitleri Fab/Synty'den satın alınır (tek stil ailesi). Yaratıklar AI hattı (Gemini → Tripo → Blender) + gerekirse paket. Kahraman ve bosslar en çok emek verilen kısım.
- [ ] **T3 Kapsam** [AÇIK] — Öneri: **4 akt (aşağıdaki tablo).** Dar alternatif: 3 akt, ~%25 kısa süre.
- [ ] **T4 Başlama zamanı** [AÇIK] — Öneri: Şimdi sadece **ön-prodüksiyon** (hikaye iskeleti + bu doküman + 2 haftalık his prototipi). Asıl prodüksiyon RagnaCook demosu çıktıktan sonra. Üç oyun paralel = üçü de gecikir.
### 12.3 Enginewright'ta ARPG için kapanması gereken eksikler
| # | Eksik | Neden ARPG'de kritik | Mevcut plan | Tahmin (kart-gün) |
|---|---|---|---|---|
| EW-1 | **Sanat hattı 0.4.1** (iskeletli import + anim, IK retarget, soket, Physics Asset, montage/blendspace/notify, bütçe) | 22 aile + 8 boss + oyuncu; ragdoll ölüm | v0.4.1 | 6–8 |
| EW-2 | **Malzeme varyant aracı** (master → MI, slot eşleme, çoklu önizleme panosu) | 66 düşman varyantı, elit görünümü | E24/E34 açık | 2 |
| EW-3 | **StateTree / BT yazma** | 22 aile davranışı + boss fazları | v0.5 | 4–5 |
| EW-4 | **AnimBP state machine** (iskelet ailesi başına 1 şablon) | 5 iskelet ailesi | v0.5 | 3–4 |
| EW-5 | **GAS araçları** (Ability/Effect/Attribute/Cue oluştur, oku, test) | 36 skill, 108 mutasyon, affix'ler = GameplayEffect | Yok (Epic'te 14 okuma aracı) | 5–6 |
| EW-6 | **Niagara parametre/varyant** (kopyala, parametre, renk/element varyantı, önizleme) | Skill + element + aura VFX | 1.0 | 4 |
| EW-7 | **Oda parçası + kat doğrulayıcı** (bağlantı noktaları, ızgara, navmesh build, erişilebilirlik) | 7 kit × ~30 oda, prosedürel kat | B15 açık | 4–5 |
| EW-8 | **Bot playtest + denge koşusu** (bot zindan temizler: TTK, ölüm, drop, "ilk unique'e süre") | Grind dengesi | E1/E2/E18 açık | 5 |
| EW-9 | **Headless mod** (editör kapalı import/veri işi) | Tek editör kuyruğunu açar, paralel üretim | 1.0 | 6–8 |
| EW-10 | **Asset metadata / AI beyanı** | Steam AI içerik beyanı | E14 açık | 1 |
| EW-11 | Sequencer temel (akt ara sahnesi ~8) | P2 | 1.0 | 3 |
| | **Toplam** | | | **≈ 43–51 kart-gün** |

