# Döküman 11 — Teknik Mimari (Arma Reforger + Web)

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

> Bu döküman, kod yazarken en çok referans alacağımız dosya olacak. Diğer dökümanlardaki her kural burada teknik karşılığını buluyor.

## 11.1 Genel Mimari

```
Arma Reforger İstemcisi          Dedicated Oyun Sunucusu          M360 Web/API Katmanı
  HUD ve kullanıcı arayüzü   →     Server-authoritative kurallar →   Next.js Admin Paneli
  Girdi/etkileşim talepleri        Replication ve RPC                Route Handlers / API
  Görsel senkronizasyon            Aktif oturum belleği               Kural ve parametre servisi
  Kritik kararlarda yetkisiz       İşlem doğrulama ve olay üretimi   Lisans ve sunucu doğrulama
                                          ↓                    ↓
                              PostgreSQL - Gerçeklik Kaynağı    Redis - Geçici Hız Katmanı
                              Hesaplar ve rol durumları          Rate limit sayaçları
                              Para hareketleri ve sahiplik        Kısa süreli kilitler
                              İş partileri ve sigorta              Oturum/cache
                              Audit ve yapılandırma sürümleri     Kuyruk ve tekrar istek önleme
```

**Temel ilke:** İstemci talep eder; oyun sunucusu doğrular; PostgreSQL kalıcı kaydı tutar; Redis yalnızca geçici koruma sağlar.

## 11.2 Arma Reforger İlkeleri

- Paylaşılan entity ve etkiler Enfusion replication sistemiyle istemcilere aktarılır; ağda görünmesi gereken entity'ler uygun RplComponent ve replication düzenine sahip olmalıdır.
- RPC, makineler arasında işlem çağrısı için kullanılır; **kritik karar her zaman sunucu tarafında verilir.**
- Dedicated server ve JSON sunucu yapılandırması başlangıçtan itibaren kullanılacak.
- **Reforger'ın 1.6+ sürümünden itibaren resmi/built-in bir persistence sistemi var** (doğrulandı — bkz. Döküman 14.1). Bu sistem dünya/entity durumunu (konum, envanter, araç) sunucu restart'ında geri yükler. **Ama M360'ın ekonomi verisi (para ledger, transaction, audit, admin panel) için hâlâ kendi PostgreSQL katmanımız ana gerçeklik kaynağı olacak** — resmi sistem entity durumu içindir, bizim ihtiyacımız olan yapısal/ilişkisel ekonomi verisi için tasarlanmamıştır. İkisinin nasıl bir arada çalışacağı Faz 0'da test edilecek (Döküman 14.1, açık madde).
- **Doğrulama notu (Döküman 14.2):** Enforce Script'in native bir PostgreSQL sürücüsü yok; topluluğun kendi açık kaynak framework'ünde bile (Arkensor'ın EnfusionDatabaseFramework) PostgreSQL desteği henüz olgunlaşmamış (🚧 durumda). Bu, bizim "dedicated server → HTTP → Next.js → PostgreSQL" mimari kararımızın **doğru ve gerekli** olduğunu doğruluyor — zaten motor seviyesinde başka bir seçenek yok.
- Sistemler tek sorumluluklu component/service biçiminde ayrılır; büyük tek script dosyası oluşturulmaz (bu, ileride benim seninle kod üzerinde çalışmamı da kolaylaştıracak — küçük, isimli, anlaşılır parçalar).

### 11.2.1 Script isimlendirme & yorum dili (kullanıcı kararı — 2026-07-27)

| Kural | Uygulama |
|---|---|
| Anlam | Değişken / üye / **fonksiyon-metod** / yerel isimler **Türkçe** (`araba`, `bicak`, `nakit`, `ToplamaBaslat`) |
| Alfabe | İsimlerde **Türkçe karakter yok** — ASCII: s, i, g, u, o, c (`bicak` ← bıçak, `IslemeBaslat` ← İşlemeBaşlat) |
| Yorum | **Türkçe**; yorum metninde ş/ğ/ü/ö/ç/ı kullanılabilir |
| Motor API / override | İngilizce **zorunlu** kalır (`OnPostInit`, `PerformAction`, `EOnFrame`, `ActionOpenInventory`, `GetGame()`) — motor bunları arar |
| Önek | Public class/global: `M360_` (Döküman 13.2) |
| Prefab Attribute | Alan adı değişince ilgili `.et` / `.conf` **aynı commit’te** güncellenir |
| Mevcut sınıf adları | Lab’daki `M360_Job*Component` gibi class adları anlık kırılmayı önlemek için korunabilir; **yeni kod** ve üyeler/metodlar bu tabloya uyar |

Örnekler: `car` → `araba`; `knife` → `bicak`; `m_iCash` → `m_iNakit`; `StartProcessing` → `IslemeBaslat`; `Boot` → `Baslat`; `Get` → `Al`.

## 11.3 Next.js Fullstack + PostgreSQL Kararı

Next.js fullstack ve PostgreSQL yerel bilgisayarda birlikte çalıştırılabilir. Next.js Route Handlers oyun sunucusu ve admin paneli için HTTP API sunar.

**Mimari öneri:** MVP'de tek Next.js uygulaması kullanılacak; fakat iş mantığı route dosyalarına yazılmayacak. Domain servisleri, veritabanı ve kural motoru ayrı paketlerde tutulacak. Yük arttığında game API aynı kodu kullanarak ayrı Node servisine çıkarılabilir. Böylece ilk gün gereksiz mikroservis karmaşası olmaz, gelecekte yeniden yazım gerekmez.

## 11.4 PostgreSQL Kurulumu — Karar: Docker'sız, Native Kurulum

**Sen Docker istemediğini belirttin — bu tercih desteklendi.** PostgreSQL, Windows üzerine doğrudan (native installer ile) kurulacak. Bu, Docker'a göre biraz daha "elle" bir kurulum ama:
- Tek seferlik kurulum, sonra arka planda Windows servisi olarak çalışır.
- Docker Desktop'ın ek kaynak tüketimi ve öğrenme yükü olmaz.
- İleride hosting'e geçerken (gerçek sunucu ortamı) genelde yönetilen PostgreSQL servisleri kullanılacağı için bu native kurulum sadece **geliştirme ortamına özgü** bir tercih — üretim ortamı kararını etkilemez.

> Bu adımın somut kurulum talimatlarını (indirme linki, kurulum adımları, ilk veritabanı/kullanıcı oluşturma) Faz 0'a geçtiğimizde adım adım vereceğim.

## 11.5 Önerilen Monorepo Yapısı

| Dizin | Sorumluluk |
|---|---|
| `apps/admin-web` | Next.js admin paneli ve yetkili yönetim ekranları |
| `apps/game-api` | İlk aşamada Next.js Route Handlers veya ayrı Node uygulaması; oyun sunucusu API'leri |
| `packages/domain` | Para, item, araç, iş, rol, sigorta ve kural motoru |
| `packages/db` | PostgreSQL şema, migration ve transaction katmanı |
| `packages/contracts` | Oyun sunucusu ile API arasında sürümlü DTO/mesaj sözleşmeleri |
| `packages/config` | Ortak doğrulama, environment ve feature flag |
| `infra` | Reverse proxy, PostgreSQL/Redis notları, gözlemleme (Docker'sız native kurulum notlarıyla güncellenecek) |
| `arma-addon` | Enfusion addon, prefab, UI, script ve config kaynakları |

Git/GitHub kullanılacak (onaylandı) — her çalışan adım commit/tag olarak işaretlenecek; deneme kodundan ürün koduna kontrollü taşıma yapılacak.

## 11.6 PostgreSQL ve Redis

PostgreSQL, para ve sahiplik işlemlerinde transaction ve kilit mekanizmaları sağlayan ana veritabanıdır. **Redis başlangıçta zorunlu değildir** — sistem tek API instance'ı ve sınırlı oyuncuyla çalışırken bellek tabanlı sayaçlar kullanılabilir. Birden fazla instance veya yoğun eş zamanlılık başladığında Redis merkezi rate limit, kısa süreli lock, cache ve kuyruk için eklenir.

## 11.7 Yerel Geliştirme Ortamı — Güncel Durum

| Bileşen | Durum | Not |
|---|---|---|
| Arma Reforger Client | ✅ Denenmiş | Workbench'te küçük scriptler yazıldı (AI yönlendirmesiyle) |
| Arma Reforger Dedicated Server | ⏳ **Henüz denenmedi — bir sonraki somut adım** | Faz 0'ın ilk pratik görevi bu olacak |
| Next.js / Node | ⏳ Kurulacak | Cursor ile büyük ölçüde bağımsız ilerleyebilirsin |
| PostgreSQL | ⏳ Kurulacak | Native (Docker'sız) kurulum — bkz. 11.4 |
| Redis | İhtiyaç oluştuğunda | MVP başlangıcında gerekli değil |
| Git | ✅ Kullanılacak | Onaylandı |

İkinci bilgisayar zorunlu değildir; ilk server/client ve API testleri tek cihazda yapılabilir. Gerçek replication ve oyuncu davranış testleri için zamanla ikinci istemci veya test oyuncuları gerekir.

## 11.8 İlk Veri Tabanı Çekirdeği

| Tablo grubu | Örnek tablolar |
|---|---|
| Hesap ve rol | `accounts`, `characters`, `role_states`, `play_sessions`, `licenses` |
| Finans | `wallets`, `money_ledger`, `transfers`, `taxes`, `insurance_policies`, `claims` |
| Item ve envanter | `item_definitions`, `item_instances`, `inventories`, `inventory_movements` |
| Araç ve mülk | `vehicles`, `vehicle_access`, `rentals`, `properties`, `garages` |
| İşler | `job_definitions`, `job_batches`, `processing_sessions`, `market_prices` |
| Klan ve roller | `clans`, `clan_members`, `clan_assets`, `role_ranks`, `role_eligibility` |
| Yönetim | `config_rules`, `config_versions`, `notifications`, `punishments`, `licenses_servers` |
| Kanıt ve log | `game_events`, `combat_events`, `position_samples`, `audit_log` |

## 11.9 Güvenlik, Rate Limit ve Manipülasyon Önleme

Kritik işlem güvenlik hattı (satın alma, garaj, para transferi, rol değiştirme, envanter transferi için geçerli):

**1. UI Kilidi → 2. Rate Limit → 3. Idempotency → 4. Sunucu Doğrulama → 5. Transaction/Lock → 6. Audit Log**

> İstemci bekleme göstergesi deneyimdir; gerçek koruma sunucu, işlem kimliği ve veritabanı seviyesindedir.

| Katman | Örnek önlem |
|---|---|
| İstemci UI | Buton yükleniyor durumu, debounce, çift tıklama önleme |
| Oyun sunucusu | Kullanıcı/işlem cooldown, rol ve dünya durumu doğrulama |
| API | Kimlik doğrulama, imzalı sunucu isteği, schema validation, rate limit |
| İşlem kimliği | Her satın alma/garaj/transfer için idempotency key |
| PostgreSQL | Transaction, unique constraint, sıra/versiyon ve atomic ledger |
| Redis | Dağıtık rate limit ve kısa süreli resource lock (ihtiyaç oluşunca) |
| Audit | Önce/sonra değerleri, olay ID, kullanıcı, server, zaman ve hata |

**Kritik senaryolar (test kabul kriteri olarak da kullanılacak):**
- Oyuncu satın alma butonuna 50 kez basar → tek işlem kabul edilir.
- İki oyuncu aynı park noktasına araç çağırır → resource lock yalnız birine verir.
- İşlem sırasında bağlantı kesilir → transaction tamamlandıysa sonuç yüklenir, değilse rollback.
- API cevap vermiyor → oyun sunucusu işlemi başarısız gösterir, para istemcide düşmez.
- Aynı item iki envantere taşınmaya çalışılır → item instance version/ownership koşulu tek transferi kabul eder.
- İstemci sahte para/rol gönderir → sunucu/DB mevcut değerleri kendisi okur, istemci değeri kaynak kabul edilmez.

## 11.10 Performans İlkeleri — Gecikme (Latency) Yönetimi (bu oturumda eklendi)

**Temel ayrım (kritik):** Hiçbir "her karede" (frame) olan şey (hareket, ateş etme, çarpışma) bu API üzerinden geçmez — bunlar tamamen Enfusion'un kendi ağ sistemi (**replication**, yani "çoğaltma/senkronizasyon") üzerinden yürür. API yalnızca **olay bazlı** ekonomik işlemler içindir (satın alma, iş bitirme, para transferi) — bunlar saniyede binlerce kez değil, tüm sunucu genelinde saniyede birkaç kez olur.

Gecikmeyi minimumda tutmak için üç ilke:

| İlke | Basit Türkçesi | Neden |
|---|---|---|
| Aynı makine/ağda barındırma | Next.js API'yi dedicated server ile aynı sunucuda çalıştır | Farklı sunucularda olursa gerçek internet gecikmesi eklenir; aynı makinede gecikme birkaç milisaniyeye iner |
| Asenkron (eş zamanlı olmayan) çağrılar | Oyuncu "satın al" dediğinde oyun donmaz; istek arka planda gönderilir, sonuç gelince ekran güncellenir | Enfusion'un bunun için hazır bir yöntemi var (Async API) — kullanıcı beklerken oyun kilitlenmez |
| Önbellekleme (cache) | Sık okunan ama az değişen veri (market fiyatları, iş tanımları) her seferinde API'ye sorulmaz; belirli aralıkla (örn. 5 dakika) tazelenir | Gereksiz ağ trafiğini azaltır, API'ye yük bindirmez |

**Özet karar:** Mimari doğru — sadece "hangi işlemin bu yoldan geçtiği" net tutulmalı. Oyun akışı (hareket/çatışma) hiç etkilenmez; sadece ekonomik olaylar (saniyede az sayıda) bu köprüden geçer, o da asenkron ve önbellekli şekilde.

**Açık madde:** Async API'nin tam kullanım şekli (hangi built-in Enfusion metodları) — Faz 0'da Workbench'te doğrulanacak (bkz. Döküman 13).

## 11.11 Log ve Kanıt Politikası

| Kayıt | Önerilen saklama |
|---|---|
| Para, sahiplik, admin ve ceza audit | En az 1-2 yıl |
| Hasar, ölüm, rol başlatma, kelepçe, araç olayları | 90 gün |
| Yoğun koordinat örnekleri | 7-14 gün; olay çevresinde daha uzun arşiv |
| Chat ve bildirim onayı | 30-90 gün |
| Teknik performans logları | 15-30 gün, özet metrik daha uzun |

Koordinatlar her karede kaydedilmez; belirli aralıkla örneklenir, önemli olaylarda kayıt sıklığı artırılır.

## 11.12 Sıradaki Somut Adım

Workbench'te client tarafını denedin ama **dedicated server'ı hiç çalıştırmadın** — bu, Faz 0'ın ilk ve en öncelikli pratik görevi olacak. Tasarım dökümanları bittiğinde, sana adım adım (hangi ayarlar, hangi dosya, nasıl başlatılır) dedicated server'ı yerel olarak ayağa kaldırmanı sağlayacağım.

---
*Önceki: [10 - Panel, HUD & Admin](./10_panel_hud_admin.md) · Sıradaki: Döküman 12 — Lisans, Ticari Model & Roadmap*
