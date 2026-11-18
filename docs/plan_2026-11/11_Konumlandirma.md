# 11 — Konumlandırma (Enginewright, genel Unreal MCP)

Tarih: 18.11.2026 · Kart `t_c6e43c15` · YALNIZ BELGE.
Girdiler: 01 §1/§4/§5, 03b §0/§4, 10 §3, `docs/arastirma/derin/01_epic.md`, README (yalnız okundu).
Web notları: [`research/yayin_hazirlik_kaynaklar_2026-11-18.md`](research/yayin_hazirlik_kaynaklar_2026-11-18.md) (K1–K6).

Kurallar: Marka adı **Enginewright** (değişmez). Depo adı planı 12 §1'de. Oyun adları yalnız vitrin örneğidir (§6); sürüm/araç adlarında yoktur (09 S10).

---

## 1. Tek cümle

Cümle **0.6.6 sonrası** (büyük duyuru) için yazıldı. Paketleme 0.6.0, bot koşusu 0.6.1'de gelir. O zamana kadar §1.1'deki ara cümle kullanılır.

**EN (duyuru, README, açıklama kaynağı):**

> **Enginewright lets AI agents test and ship your Unreal game, not just edit it: multiplayer Play In Editor, packaged builds, scripted bot runs and visual QA, with every change verified and undoable. It runs inside the editor, next to Epic's built-in MCP.**

**TR:**

> **Enginewright, yapay zekâ ajanının Unreal oyununu yalnız düzenlemesini değil, test edip paketlemesini sağlar: çok oyunculu Play In Editor, paketli build, senaryolu bot koşusu ve görsel QA; her değişiklik doğrulanır ve geri alınabilir. Editörün içinde, Epic'in yerleşik MCP'sinin yanında çalışır.**

Kısa etiket (başlık, sosyal görsel, X biyografisi): **"Playwright for Unreal"** (README'de zaten var; 01 §5 madde 5).

### 1.1 Ara cümle (0.4.x – 0.6.5, küçük paylaşımlar)

> EN: Enginewright lets AI agents play and check your Unreal game, not just edit it: multiplayer Play In Editor, packaged-game runs and visual QA, with every change verified and undoable.
>
> TR: Enginewright, yapay zekâ ajanının Unreal oyununu yalnız düzenlemesini değil, oynayıp denetlemesini sağlar: çok oyunculu PIE, paketli oyun koşusu ve görsel QA; her değişiklik doğrulanır ve geri alınabilir.

Neden ayrı: "ships / paketler" iddiası `package_build` (0.6.0) çıkmadan yazılmaz. Yanlış iddia güveni bozar (13 §8 YAPMA).

---

## 2. Neden bu cümle (fark tablosu)

Epic'in MCP'si **editörü** geniş biçimde düzenler (878 araç). Topluluk MCP'lerinin çoğu da editör düzenler. Enginewright'ın boş alanı **oyunu çalıştırma, test etme ve paketleme** zinciridir (01 §4 "Kimsede yok": cook/package, 2 instance co-op testi).

| Oyuncu | ★ (17.11) | Ne yapar | Oyun testi / paket | Enginewright farkı | Kaynak |
|---|---:|---|---|---|---|
| **Epic Unreal MCP, UE 5.8** | — (motor içi) | Editör içi HTTP sunucu, 58 toolset / 878 araç; Sequencer, Niagara, PCG yazma güçlü. `ue_experimental` | `StartPIE`/`StopPIE` var. **Yok:** packaging/cook, multiplayer PIE, PIE oyun girdisi, lokalizasyon gather | Rakip değil, **yanında** çalışır. Bizde çok oyunculu PIE + girdi, `game_run`, paketleme (0.6.0), dil zinciri, atomik `batch` | [Epic doküman 5.8](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8); `derin/01_epic.md` §0, §1 |
| **Epic Unreal MCP, UE 5.9 Preview** | — | Sunucu varsayılan açık; Read/Write/Execute modları; `execute_python`; cooked build'de runtime araçlar (Perf/Console/Input) | Çalışan oyuna bağlanma **gelir** (fark daralır). Packaging/cook ve multiplayer PIE **bulunamadı** | 5.9 çıkınca "paketli oyunu çalıştırma" tek başına fark değildir. Kalıcı fark: **paketleme + çok oyunculu test + senaryo/bot + görsel QA zinciri + bugün 5.8/5.7'de çalışma** | [Epic doküman 5.9](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9); `derin/01_epic.md` §2.2 |
| **chongdashu/unreal-mcp** | 2.118 | Python sunucu + UE eklentisi; aktör/Blueprint düzenleme | ❌ PIE/oyun testi yok; son push 22.04.2025 | Bakımlı; Python/Node yok; test ve paket | 01 §1 #2; 03b §3.5 |
| **ChiR24/Unreal_mcp** | 1.043 | Python sunucu + editör eklentisi; ~487 araç; UE 5.0–5.8; aktif | 🟡 girdi var, N istemcili PIE yok; paket yok | Çok oyunculu PIE + net emulation; paketleme; tek süreç (Python yok) | 01 §1 #4, §3b |
| **kevinpbuckley/VibeUE** | 838 | C++ editör içi; Epic 5.8 MCP'sini genişletir | 🟡 | Epic'e eklenti değil, bağımsız; oyun testi ve paket hattı | 01 §1 #7 |
| **db-lyon/ue-mcp** | 505 | C++ editör içi | ❌ PIE testi yok | Oyun testi + görsel QA + paket | 01 §1 #11 |

Not (K3): awesome-mcp-servers Gaming listesinde başka bir editör içi UE MCP de "every change checked and read back" diyor. **"Verified" tek başına ayırt edici değildir.** Ayırt edici olan "test and ship" kısmıdır; "verified, undoable" destek kanıtıdır.

---

## 3. Üç kanıt maddesi (hangi araç, hangi ara sürüm)

Her madde duyuru metninde ve demo videosunda (13 §4) kanıt olarak gösterilir. "Var" = v0.4.0'da kayıtlı araç (`research/arac_envanteri.json`, 129 araç).

| # | İddia (EN) | Kanıt aracı | Ne zaman hazır | Demo |
|---|---|---|---|---|
| K-1 | **"It tests multiplayer."** İki oyunculu listen-server PIE açılır, oyun girdisi gönderilir, replike olmayan değer bulunur. | Var: `pie_start` (players, netMode), `pie_net_emulation`, `pie_input`, `pie_actors`. Gelecek: `replication_check` | **0.6.3** (`replication_check`); taban 0.4.3 (çok oyunculu PIE açılış kararlılığı) | 13 §4 D2 |
| K-2 | **"It ships the build."** Ajan Win64 paketini async alır, paketli oyunu açar, senaryo koşturur, kare hızını ölçer. | Var: `game_run`. Gelecek: `game_perf_sample` (0.5.3), `package_build` + `package_status` (**0.6.0**), `pie_scenario` + `bot_run` (**0.6.1**) | **0.6.1** sonrası tam zincir | 13 §4 D1 (ana video) |
| K-3 | **"Every change is verified and undoable, and it checks what the player sees."** Toplu değişiklik tek adımda geri alınır; ekran görüntüsü referansla karşılaştırılır; dil × ekran taşması bulunur. | Var: `batch` (rollback), `validate`, `doctor`, `widget_render` (COVERED), `screenshot_ab`, `image_stats`, `loc_gather`, `font_coverage`. Gelecek: `text_overflow_audit` (0.4.1), `loc_screen_sweep` (0.6.3), `screenshot_tour` (0.6.5) | Bugün kısmen; **0.6.5** sonrası tam | 13 §4 D3 |

Kural: Bir madde, ilgili ara sürümün kabul ölçütü (10 §4) geçmeden duyuruda kullanılmaz.

---

## 4. Kim için

**Birincil kullanıcı:**
- Unreal Engine **5.8** (0.9.13 sonrası **5.7** de; S2-09 A) kullanan, **Win64** geliştiren,
- Claude Code, Codex, Cursor, VS Code ya da başka bir MCP istemcisiyle çalışan,
- **ayrı QA ekibi olmayan** bağımsız geliştirici ve küçük ekip (1–10 kişi).
- İhtiyacı: "Ajan değişikliği yaptı; oyun hâlâ açılıyor mu, çok oyunculu bozuldu mu, ekranda yazı taşıyor mu, paket alınıyor mu?" sorusunu elle değil, ajana sordurmak.

**İkincil kullanıcı:**
- Epic MCP'sini zaten kullanan ve **test + paket** ayağını arayan ekip (yanında kurar).
- CI/otomasyon kuran teknik sanatçı / araç geliştirici (0.9.x headless sonrası daha güçlü).

**Kim için değil (dürüst sınır, README'de de yazılır):**
- Mac / Linux (1.0 sonrası; S2-09 A).
- UE 5.6 ve öncesi.
- Editör içi sohbet paneli isteyen kullanıcı (Enginewright sunucudur, sohbet arayüzü değildir).
- Metinden 3D model üretimi (kapsam dışı).

---

## 5. Mesaj sırası (her metinde aynı)

1. **Ne yapar:** test and ship, not just edit (§1).
2. **Kanıt:** K-1, K-2, K-3 içinden en fazla ikisi + video.
3. **Güven:** strict, verified, undoable; Python/Node yok; MIT; açık yol haritası.
4. **Epic ile ilişki:** "Works next to Epic's built-in MCP." Asla "replaces" ya da "official" denmez.
5. **Vitrin:** "Built while making real games" + oyun klibi (§6).

---

## 6. Oyun vitrini kuralı (konumlandırma tarafı)

- Oyunlar **kanıt vitrinidir**, ürünün parçası değildir. Cümle kalıbı: *"Used daily while building three real Unreal games."* Oyun adı yalnız vitrin satırında ve klip başlığında geçer.
- Sürüm adı, CHANGELOG, araç adı, araç açıklaması, `server.json`, topic: **oyun adı yok** (09 S10).
- Ayrıntılı klip kuralı: 13 §5.

---

## 7. Kaynaklar

- Epic 5.8 / 5.9 MCP: `docs/arastirma/derin/01_epic.md` (10.11.2026; Epic dokümanı + motor kaynağı).
- Rakip sayıları: 01 §1 (GitHub API, 17.11.2026); büyüme: 03b §1, §3.
- awesome-mcp-servers Gaming örnekleri, Registry paket kuralı: `research/yayin_hazirlik_kaynaklar_2026-11-18.md` K2, K3.
- Ara sürüm ve araç adları: 10 §3, §4.
