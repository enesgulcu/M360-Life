# Döküman 14 — Mevcut Modlar, Framework'ler ve Geliştirme Araçları

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md) · [11 - Teknik Mimari](./11_teknik_mimari.md) · [12 - Lisans & Roadmap](./12_lisans_roadmap.md)*

> **Kaynak ilkesi (kesinleşti):** Buradaki her bilgi resmi Bohemia Interactive dökümantasyonundan (community.bistudio.com/wiki) veya doğrudan mod sayfalarından/GitHub depolarından doğrulanmıştır. Varsayımla yazılmamıştır. Bu döküman zamanla güncellenecek — Arma Reforger sık güncellenen bir motor, buradaki bilgiler periyodik olarak tekrar kontrol edilmeli.

## 14.0 Sürekli Referans İlkesi (kesinleşti — bu oturumda karar verildi)

**EveronLife** ve **Narcos Life Roleplay**, proje boyunca **sürekli gözetilecek iki referans noktası** olarak belirlendi. Bu, tek seferlik bir araştırma değil — her yeni sistem tasarlarken ("bu işi nasıl çözelim") bu ikisine bakma alışkanlığı olacak:

| Referans | Nasıl kullanılacak | Kullanılmayacak şekilde |
|---|---|---|
| **EveronLife** | Kod/mimari fikir kaynağı — GitHub'ı açık, "bunu nasıl çözmüşler" diye bakılabilir | Workbench'e bağımlılık olarak asla eklenmeyecek (bkz. 14.3b) |
| **Narcos Life Roleplay** | Pazar/özellik/denge referansı — canlı bir sunucu olarak "bu özellik oyuncularda nasıl karşılık buluyor" gözlemi | Koduna erişimimiz yok (kapalı kaynak), doğrudan inceleme yapılamaz — sadece dışarıdan gözlem (video, wiki, oyuncu yorumları) |

Bundan sonra her yeni döküman/sistem tasarlarken, ilgiliyse bu iki kaynağa hızlı bir bakış atılacak.

## 14.1 Kritik Mimari Bulgu — Resmi Persistence Sistemi (Döküman 11'i Etkiliyor)

**Arma Reforger 1.6 sürümünden itibaren motora resmi/built-in bir persistence (kalıcılık) sistemi eklendi** (bkz. `community.bistudio.com/wiki/Arma_Reforger:Persistence_System`, `Server_Config#persistence`, `Startup_Parameters#worldSystemsConfig`). Bunun etkisi:

- Topluluk tarafından yapılmış eski **"Enfusion Persistence Framework"** (Arkensor) bu yüzden **deprecated (kullanımdan kaldırıldı)** — motorun kendi resmi sistemi onun yerini aldı.
- Bu resmi sistem, **entity/dünya durumunu** (konum, envanter, araç durumu vb.) sunucu yeniden başladığında geri yüklemek için var — yani "restart sonrası oyuncu kaybolmasın" ihtiyacının bir kısmını motor kendisi zaten çözüyor.
- **Ama bu, Döküman 11'deki PostgreSQL kararımızı geçersiz kılmıyor.** Resmi sistem dünya/entity durumu içindir; bizim ihtiyacımız olan şey (para ledger'ı, transaction, audit, admin panel, çoklu tenant/sunucu desteği, karmaşık ekonomi kuralları) hâlâ kendi PostgreSQL + Next.js katmanımızı gerektiriyor. **Faz 0'da bu ikisinin nasıl bir arada çalışacağı (resmi sistem dünya durumu için, bizim sistemimiz ekonomi/hesap verisi için) net şekilde test edilmeli** — bu, açık madde olarak işaretlendi.

## 14.2 Enfusion Database Framework (Arkensor) — Mimari Doğrulama

`github.com/Arkensor/EnfusionDatabaseFramework` (MIT lisanslı, açık kaynak) incelendiğinde şu netleşti:

| Depolama tipi | Durum |
|---|---|
| Local JSON/Binary dosya | ✅ Tam destekli |
| MongoDB (document DB) | ✅ Tam destekli (HTTP proxy üzerinden) |
| SQLite, MySQL, **PostgreSQL** | 🚧 **Henüz tam desteklenmiyor / geliştirme aşamasında** |

**Bunun bizim için anlamı:** Enforce Script'in doğrudan bir PostgreSQL sürücüsü yok ve topluluk framework'ünde bile bu henüz olgunlaşmamış. Bu aslında **Döküman 11'deki mimari kararımızı doğruluyor** — dedicated server'ın PostgreSQL'e doğrudan bağlanması yerine, **HTTP üzerinden Next.js API'sine bağlanıp, Next.js'in PostgreSQL'i yönetmesi** doğru ve zaten yaygın kullanılan bir desen (Arkensor'ın kendi MongoDB sürücüsü de aynı "motor → HTTP proxy → gerçek veritabanı" desenini kullanıyor).

**Karar:** EnfusionDatabaseFramework'ü bağımlılık olarak eklemeyeceğiz (Döküman 12.1 ilkesiyle uyumlu — kritik çekirdek dışa bağımlı olmasın). Onun yerine kendi basit HTTP client script'imizi yazacağız (Döküman 11.1'deki mimariye zaten uygun). Ama bu framework'ün dokümantasyonu, HTTP proxy deseninin nasıl kurulacağına dair iyi bir referans olarak kullanılabilir.

## 14.3 Rakip/Referans Analizi — Narcos Life Roleplay

Araştırdığım en yakın örnek: **Narcos Life Roleplay (NLRP)** — `narcosliferp.com`, aktif ve büyük bir Arma Reforger Life sunucusu.

| Özellik | Bulgular |
|---|---|
| Harita | Kendi "Lakeside Valley" haritası (Everon değil, özel/farklı bir World Pack) |
| Oyuncu kapasitesi | 128 (bizim MVP hedefimiz 100-150 ile aynı ölçekte) |
| Sistemler | Law enforcement (LVSO), mafya/suç ekonomisi, EMS (sağlık), sivil roller — **bizim Sivil/Polis/Doktor/Rebel yapımızla kavramsal olarak birebir örtüşüyor** |
| Oyun modu tipi | Resmi "Roleplay" gamemode kategorisinde, custom sistemlerle inşa edilmiş |
| Durum | Canlı, aktif geliştirilen, topluluğu olan (Discord, portal, wiki, monetization sayfası var) |

**Çıkarımlar:**
- Bu, "Life modu Arma Reforger'da işler mi" sorusuna **evet, işliyor** cevabını veriyor — pazar/konsept doğrulaması olarak değerli.
- Kodlarına doğrudan erişimimiz yok (kapalı kaynak, kendi custom sistemleri) — bu yüzden **kopyalanacak bir şey değil, rakip/pazar analizi** olarak değerli.
- İleride kendi sunucumuz büyüdüğünde, feature karşılaştırması yapmak için referans noktası olarak kullanılabilir.

> Not: Bu konuyu istersen ayrıca **derinlemesine bir rakip analizi** olarak genişletebiliriz (fiyatlandırma, monetization modeli, topluluk büyüklüğü) — bu, Araştırma (Research) özelliğiyle daha kapsamlı yapılabilir.

## 14.3b EveronLife — Tam Netleştirme: Ne Katar, Ne Kısıtlar? (derinleştirildi)

### Tam olarak ne içeriyor?

EveronLife bir **Workshop öğesi** (yani Arma Reforger'ın kendi mod mağazasından indirilen bir paket). Sunucu sahipleri bunu kendi Workbench projelerine **"bağımlılık" (dependency)** olarak ekliyor — yani "bu paketi kullanıyorum" diyorlar, kendi üstüne inşa ediyorlar. Kendi resmi uyarıları: **"hâlâ yoğun geliştirme aşamasında, oynanmaya hazır değil"** ve Workshop sayfasında **"özellikler alfa prototip seviyesinde"** yazıyor.

**Kendi yol haritalarını da buldum — bizimkiyle neredeyse birebir aynı mantık:**

| EveronLife'ın 4 fazı | Bizim planımızdaki karşılığı |
|---|---|
| Faz 1: İlk doğma, başkentte toplanma, ilk ekipman/küçük araç alımı | Döküman 12 Faz 2 — "İlk Dikey Dilim" |
| Faz 2: Toplanan kaynağı taşıma, küçük kamyon alımı | Aynı — Döküman 4.9'daki "Gün 1-5" ekonomi çizelgesi |
| Faz 3: Farklı oyuncu işleri (toplama, madencilik, tarım, teslimat) + yasa dışı faaliyetler | Döküman 5 — tam olarak bizim "topla-işle-sat" sistemimiz |
| Faz 4: Oyuncu mülkiyeti, gelişmiş işler (dükkan sahipliği), çete/bölge savaşları, büyük soygunlar | Döküman 7 (mülk), Döküman 9 (klan), Döküman 8 (çatışma) |

**Bu çok değerli bir doğrulama:** Onlar da bağımsız olarak aynı sıralamaya ulaşmış (önce temel ekonomi, sonra roller, en son mülk/çete). Bu, bizim faz sıralamamızın (Döküman 12) doğru bir mantık izlediğinin bağımsız bir kanıtı.

### Net cevap: Bizi kısıtlar mı?

**Sadece referans/inceleme kaynağı olarak kullanırsak: Hayır, hiçbir kısıtlama yok.** Genel fikirler (yaklaşım, sıralama, mimari düşünce biçimi) telif hakkına tabi değildir — sadece onların yazdığı **spesifik kod** korunur, biz onu kopyalamayacağız zaten.

**Eğer Workbench'te gerçek bir bağımlılık olarak eklersek (ÖNERMİYORUM): Evet, kısıtlar.**
- Onlar kodlarını değiştirdikçe bizim projemiz de otomatik etkilenir — kontrolümüz dışında bir değişiklik bizim sunucumuzu bozabilir (Döküman 12.5'teki "harici mod kırılması" riski tam burada gerçekleşir).
- MIT lisans atıf gerektiriyor — Workshop açıklamamızda "EveronLife framework'üne dayanmaktadır" yazmamız gerekir. Bu, "tamamen kendi kapalı kaynak ürünümüz" hikayemizi biraz zedeler (yasal olarak sorun değil ama marka/hikaye açısından).
- En önemlisi: **kendileri bile "henüz oynanabilir değil" diyor.** Üzerine bağımlı olmak, olgunlaşmamış bir temel üzerine inşa etmek demek — hızlandırmak yerine yavaşlatabilir (onların hatalarını da miras alırız).

### Kesin Karar

**EveronLife'ı yalnızca "nasıl yapmışlar, bakalım" amacıyla okuyacağız — Workbench projemize bağımlılık olarak asla eklemeyeceğiz.** Bu, sıfır risk + maksimum öğrenme demek. Faz 0-2'de kendi script'lerimizi yazarken (Döküman 13'teki `M360_` yapımızla) ara sıra onların GitHub'ındaki ilgili dosyaya bakıp "bu sorunu nasıl çözmüşler" diye kontrol edebiliriz, o kadar.

## 14.4 Kullanılabilir/İncelenebilir Diğer Workshop Mod ve Framework'ler

| Mod/Framework | Lisans | Neden ilgili | Kullanım kararı |
|---|---|---|---|
| **BasicBanking** | Workshop (lisans kontrol edilecek) | ATM + banka entegrasyonu, orijinal dökümanda zaten referans alınmıştı | Demo/referans olarak incelenecek, Faz 1'de |
| **Zeliks Persistent Banking** | Workshop (lisans kontrol edilecek) | Nakit, ATM, persistent banking | Veri modeli referansı |
| **P27 Economy & Spawn Points System** | Workshop (yeni keşfedildi) | Server-authoritative güvenlik deseni bizimkiyle birebir aynı prensip: *"oyuncu karşılayamadığı bir aracı spawn etmeye çalışırsa, araç anında silinir"* — Döküman 11.9'daki güvenlik senaryolarımızla doğrudan örtüşüyor | Güvenlik deseni referansı — kod kopyalanmayacak ama yaklaşım doğrulaması olarak değerli |
| **Enfusion Database Framework** (Arkensor) | MIT (açık kaynak) | Bkz. 14.2 | Bağımlılık olarak kullanılmayacak, mimari referans olarak kullanıldı |

## 14.5 Geliştirme Araçları — enfusion-mcp (güncel: Cursor)

**enfusion-mcp** = Model Context Protocol sunucusu; AI’nin Enfusion/Reforger API’sini tahmin etmek yerine aramasını ve Workbench’i uzaktan kullanmasını sağlar.

| Özellik | Açıklama |
|---|---|
| `api_search` | Enfusion/Arma Reforger API sınıf/metod araması |
| `wiki_search` / BI wiki | Resmi tutorial/rehber |
| Workbench | `wb_connect`, reload, entity, resource, script… (açık WB varken `wb_launch` yasak — docs 15) |
| **Durum (2026-07-27)** | **Cursor IDE** içinde bağlı ve kullanılıyor. Önceki “Claude Desktop connector dizini / henüz bağlı değil” notu **eski**. Kurulum: Cursor MCP ayarları + Workbench NET API. |

Yeni sohbetlerde birincil ortam: **Cursor + enfusion-mcp**; kanıtlar Döküman 15’e yazılır.

## 14.6 Açık Maddeler

- [x] ~~EveronLife bağımlılık mı referans mı~~ — yalnızca referans (14.3b)
- [ ] Resmi persistence + PostgreSQL birlikte test (Faz 0 dedicated sonrası)
- [ ] BasicBanking / Zeliks lisans kontrolü (12.1)
- [x] ~~enfusion-mcp bağlantısı~~ — Cursor’da bağlı (14.5)
- [ ] Narcos derin rakip analizi — isteğe bağlı
- [ ] **Sürekli radar:** Faz 0–2 boyunca periyodik güncelle

---
*Önceki: [13 - Enfusion Prefab Prosedürü](./13_enfusion_prefab_prosedur.md) · Bu, döküman setinin son parçasıdır (15 = canlı lab belleği).*
