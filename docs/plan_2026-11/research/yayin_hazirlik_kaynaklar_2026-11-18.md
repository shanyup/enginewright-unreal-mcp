# Yayın hazırlığı — web kaynak notları (18.11.2026)

Kart `t_c6e43c15`. 11, 12, 13 ve SORULAR_3 bu notlara dayanır. Scratch silinir; özetler burada kalır.
Etiket: **[okundu 18.11]** = sayfa bu kartta açıldı ve okundu. **[önceki belge]** = 01 / 03b / `docs/arastirma/derin/` içinde, o belgenin kaynağıyla.

## K1 — GitHub: depo yeniden adlandırma

URL: https://docs.github.com/en/repositories/creating-and-managing-repositories/renaming-a-repository **[okundu 18.11]**

- Yeniden adlandırınca issue, wiki, **yıldız**, takipçi yeni ada yönlenir.
- Eski adres üstünden `git clone` / `fetch` / `push` çalışmaya devam eder. Yerel kopyada `git remote set-url origin NEW_URL` önerilir.
- **İstisna 1:** GitHub Pages proje sitesi URL'si yönlenmez. Özel alan adı önerilir.
- **İstisna 2:** Yeniden adlandırılan depodaki GitHub **Action** çağrıları yönlenmez (`repository not found`).
- **Uyarı:** Eski adla yeni depo açılırsa yönlendirme kırılır.

## K2 — Resmi MCP Registry: paket türleri

URL: https://github.com/modelcontextprotocol/registry/blob/main/docs/modelcontextprotocol-io/package-types.mdx **[okundu 18.11]**

- Registry "preview" durumunda; veri sıfırlama olabilir.
- Desteklenen türler: npm, PyPI, NuGet, Docker/OCI, **MCPB** (GitHub/GitLab release dosyası). Her tür kendi sahiplik doğrulamasıyla.
- npm: `package.json` içinde `mcpName` = `server.json` adı. PyPI/NuGet: README'de `mcp-name: <ad>`.
- **MCPB:** `identifier` = release indirme URL'si; **URL "mcp" dizgesini içermek ZORUNDA** (dosya uzantısı ya da depo adı). `fileSha256` zorunlu.
- Sunucu adı biçimi: `io.github.<kullanıcı>/<ad>`.
- Çıkarım (bizim): Enginewright editör içinde HTTP sunucusu (`http://127.0.0.1:3100/mcp`, README). Registry paket türlerinin hepsi istemcinin başlattığı bir süreç ya da uzak URL ister. Editör eklentisi bu türlerden birine **doğrudan uymaz**. Bir köprü paketi (stdio → yerel HTTP) ya da MCPB gerekir → SORULAR_3 S3-02.
- Çıkarım (bizim): `enginewright-unreal-mcp` depo adı "mcp" içerdiği için MCPB URL kuralını kendiliğinden karşılar.

## K3 — punkpeye/awesome-mcp-servers README (Gaming bölümü)

URL: https://raw.githubusercontent.com/punkpeye/awesome-mcp-servers/main/README.md **[okundu 18.11]**

- Bölüm başlığı: `### 🎮 Gaming`. Kayıt biçimi: `- [sahip/depo](url) [![Glama skor rozeti](…/badges/score.svg)](glama url) <dil> <kapsam> <OS> - açıklama`.
- Gösterge: 🌊 = C/C++, 🏠 = yerel servis, 🪟 = Windows. Enginewright için doğru takı: `🌊 🏠 🪟`.
- Listede Glama bağlantısı ~3.355 kez geçiyor → her kayıtta Glama rozeti fiilen standart.
- Gaming bölümündeki UE örnekleri (18.11): `CarlxsMG/UEAgent` ("MCP server built into the Unreal Engine 5 editor … every change checked and read back"), `miracle1814/ue5-automation`, `NAJEMWEHBE/unreal-ai-connection`. Not: UEAgent'ın açıklaması bizim "strict, verified" dilimize çok yakın → açıklamada "verified" tek başına ayırt edici değil; ayırt edici olan **oyunu test etme + paketleme**.
- README başında: "web-based directory (glama.ai/mcp/servers) is synced with the repository".

## K4 — Hacker News Show HN kuralları

URL: https://news.ycombinator.com/showhn.html **[okundu 18.11, 03b §5'te de özet var]**

- "Show HN" kullanıcının deneyebileceği bir şey için. Kayıt/e-posta engeli olmamalı.
- Sürüm notu / küçük güncelleme Show HN değildir.
- Arkadaşlardan oy/yorum istemek yasak. Yazar yorumlarda olmalı.

## K5 — Epic Unreal MCP (UE 5.8 / 5.9 Preview)

Kaynak: `docs/arastirma/derin/01_epic.md` (10.11.2026; Epic dokümanı + motor kaynağı) **[önceki belge]**

- 5.8: 58 toolset sınıfı / 878 araç. Durum `ue_experimental`. https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8
- 5.8'de **yok**: packaging/cook, multiplayer PIE, PIE oyun girdisi, lokalizasyon gather, AnimBP/state machine, Montage/BlendSpace, IK Retarget, MRQ, Enhanced Input. [doğrulandı-kaynak]
- 5.9 Preview (09.11.2026): sunucu varsayılan açık, Read/Write/Execute erişim modu, `execute_python`, cooked/Shipping build'de MCP (`AIRuntimeCallable`; Perf/Console/Input/NavMesh/SceneCapture runtime toolset'leri). Packaging/cook ve multiplayer PIE **bulunamadı**. [doğrulandı-doküman]
- Çıkarım (bizim): 5.9 ile "çalışan oyuna bağlanma" farkı daralır. Kalıcı fark: **paketleme + çok oyunculu PIE + senaryo/bot koşusu + görsel/dil QA zinciri + atomik batch**, ve bugün 5.8'de (5.7 hedefli) çalışması.

## K6 — Rakip ve emsal sayıları

Kaynak: `01_Rakip_ve_Emsal_Karsilastirma.md` §1 (17.11, GitHub API) ve `03b_Buyume_Hikayeleri.md` §1, §3 (17.11, GitHub API + star-history) **[önceki belge]**. Bu kartta yeniden çekilmedi.

| Proje | ★ (17.11) | Mimari | Kaynak |
|---|---:|---|---|
| chongdashu/unreal-mcp | 2.118 | Python sunucu + UE eklentisi; son push 22.04.2025 | 01 §1, 03b §3.5 |
| ChiR24/Unreal_mcp | 1.043 | Python sunucu + editör eklentisi; 5.0–5.8; ~487 araç | 01 §1 |
| kevinpbuckley/VibeUE | 838 | C++ editör içi, Epic 5.8 MCP genişletmesi | 01 §1 |
| db-lyon/ue-mcp | 505 | C++ editör içi | 01 §1 |
| runreal/unreal-mcp | 121 | Python Remote Exec; npx tek komut; durgun | 01 §1, 03b §3.7 |
