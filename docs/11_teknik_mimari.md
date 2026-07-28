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

## 11.1b Canlı mimari (2026-07-28 — ne kurduk, nasıl konuşuyor)

```
┌─────────────────────┐     junction      ┌──────────────────────────┐
│ Workbench / Play    │ ←───────────────→ │ GitHub monorepo          │
│ addons\M360-Life    │   aynı dosyalar   │ m360-life/  api/  docs/  │
└─────────┬───────────┘                   └────────────┬─────────────┘
          │ RestApi (HTTP)                             │ git push
          ▼                                            ▼
┌─────────────────────┐                   ┌──────────────────────────┐
│ Vercel  api/        │ ←──── SQL ──────→ │ Neon PostgreSQL          │
│ m360-life.vercel.app│                   │ job_definitions …        │
│ /api/health|jobs|…  │                   └──────────────────────────┘
│ Lab UI /istatistik  │
└─────────────────────┘
```

| Katman | Ne yapar | Ne yapmaz |
|---|---|---|
| **Oyun (`m360-life`)** | HUD, aksiyon, lab iş akışı, ileride RestApi ile API çağrısı | Para/ledger’a doğrudan yazmaz; istemci kaynak değil |
| **API (`api/` → Vercel)** | HTTP JSON; Neon okur/yazar; rate limit + `M360_SERVER_KEY` | Oyuncu tarayıcısından açık veri sızdırmamalı |
| **DB (Neon)** | Gerçeklik kaynağı (`job_definitions` …) | İnternete doğrudan açık değil |
| **Lab UI (`/`)** | Key ile health/jobs/metrik inceler | Üretim admin paneli değil (`web/` sonra) |
| **Dedicated** | (henüz) Sunucu-yetkili kurallar + API’ye imzalı istek | — |

**İletişim kuralı (hedef):**  
Oyuncu → (görsel) istemci → **dedicated doğrular** → HTTP (`X-M360-Server-Key`) → Vercel API → Neon.  
Lab’da şimdilik: Play / script `M360_ApiIstemci` doğrudan API’ye test çağrısı atabilir; üretimde anahtar yalnız dedicated’ta kalır.

**Web tarafı ayrımı:**
- `api/` = oyun + lab HTTP (canlı)
- `web/` = ileride admin panel (ayrı Vercel proje veya monorepo ikinci root)
- Tarayıcıda ham `/api/jobs` anahtarsız **401**; lab UI key’i session’da tutar

Detay: [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md) · lab katalog: `api/src/lib/lab/endpoint-katalog.ts`


## 11.2 Arma Reforger İlkeleri

- Paylaşılan entity ve etkiler Enfusion replication sistemiyle istemcilere aktarılır; ağda görünmesi gereken entity'ler uygun RplComponent ve replication düzenine sahip olmalıdır.
- RPC, makineler arasında işlem çağrısı için kullanılır; **kritik karar her zaman sunucu tarafında verilir.**
- Dedicated server ve JSON sunucu yapılandırması başlangıçtan itibaren kullanılacak.
- **Reforger'ın 1.6+ sürümünden itibaren resmi/built-in bir persistence sistemi var** (doğrulandı — bkz. Döküman 14.1). Bu sistem dünya/entity durumunu (konum, envanter, araç) sunucu restart'ında geri yükler. **Ama M360'ın ekonomi verisi (para ledger, transaction, audit, admin panel) için hâlâ kendi PostgreSQL katmanımız ana gerçeklik kaynağı olacak** — resmi sistem entity durumu içindir, bizim ihtiyacımız olan yapısal/ilişkisel ekonomi verisi için tasarlanmamıştır. İkisinin nasıl bir arada çalışacağı Faz 0'da test edilecek (Döküman 14.1, açık madde).
- **Doğrulama notu (Döküman 14.2):** Enforce Script'in native bir PostgreSQL sürücüsü yok; topluluğun kendi açık kaynak framework'ünde bile (Arkensor'ın EnfusionDatabaseFramework) PostgreSQL desteği henüz olgunlaşmamış (🚧 durumda). Bu, bizim "dedicated server → HTTP → Next.js → PostgreSQL" mimari kararımızın **doğru ve gerekli** olduğunu doğruluyor — zaten motor seviyesinde başka bir seçenek yok.
- Sistemler tek sorumluluklu component/service biçiminde ayrılır; büyük tek script dosyası oluşturulmaz (bu, ileride benim seninle kod üzerinde çalışmamı da kolaylaştıracak — küçük, isimli, anlaşılır parçalar).

### 11.2.1 Script isimlendirme & yorum dili (ANA DÜSTUR — 2026-07-27)

| Kural | Uygulama |
|---|---|
| Anlam | Class, değişken, üye, **metod**, yerel, Attribute, oyuncu metni → **Türkçe ASCII** |
| Alfabe | İsimlerde **Türkçe karakter yok** (`bicak`, `IslemeBaslat`, `m_iNakit`, `M360_ToplamaAlaniBileseni`) |
| Yorum / Print / Hint | **Türkçe** (yorumda ş/ğ/ü serbest; kod isminde değil) |
| Motor API / override | İngilizce **zorunlu** — dokunulmaz (`OnPostInit`, `PerformAction`, `EOnFrame`, `GetGame()`, `pOwnerEntity` imza parametreleri, `ScriptedUserAction`…) |
| Önek | Public: `M360_` |
| Prefab | Class/Attribute rename = `.et` + dünya layer **aynı anda** |
| Dosya adı | Class ile uyumlu `.c` (`M360_IsAyar.c`) |
| Yasak kaçış | “Lab için İngilizce class kalsın” **yok** — değiştirilebilir her şey Türkçe |

Örnek: `JobConfig` → `IsAyar`; `TickDuration` → `AdimSuresi`; `CollectAction` → `ToplaAksiyonu`; `StartProcessing` → `IslemeBaslat`.

**Dokunulmayanlar:** BI/Enfusion API, `Scripts/WorkbenchGame/EnfusionMCP/*` (araç handler’ları), resource GUID path’lerindeki dosya adları (örn. eski `M360_JobCollect_Pirinc.et` GUID’si; disk adı artık `M360_Topla_Pirinc.et`; içindeki class Türkçe).

## 11.3 Next.js Fullstack + PostgreSQL Kararı

Next.js Route Handlers oyun sunucusu ve admin paneli için HTTP API sunar. **Karar (2026-07):** API → **Vercel** (pi/), PostgreSQL → **Neon** (hosted). Yerel PG yok. Yerel 
pm run dev isteğe bağlı.

**Mimari öneri:** MVP'de pi/ tek Next.js uygulaması; iş mantığı route dosyalarına gömülmez. Domain / DB / sözleşmeler packages/ altında büyür.

## 11.4 PostgreSQL — Karar: Neon (Docker'sız, yerel PG yok)

PostgreSQL **Neon** üzerinde. Migration: packages/db/migrations/ + Neon SQL Editor. Docker yok.

## 11.5 Önerilen Monorepo Yapısı

| Dizin | Sorumluluk |
|---|---|
| web/ | Next.js admin paneli (sonraki faz) |
| pi/ | Next.js Route Handlers — oyun HTTP API → Vercel |
| packages/db | Neon şema / migration |
| packages/domain | (plan) Para, item, iş, kural motoru |
| packages/contracts | (plan) Sürümlü DTO |
| m360-life/ | Enfusion addon (Workbench junction: ddons\M360-Life) |
| 	ools/ | agla-oyun-klasoru.ps1, sync, texture |

Detay: [18](./18_calisma_duzeni.md).

## 11.6 PostgreSQL ve Redis

PostgreSQL ana DB. **Redis başlangıçta yok** — bellek rate limit (pi/src/lib/guvenlik). Çok instance'ta Redis.

## 11.7 Geliştirme Ortamı — Güncel Durum

| Bileşen | Durum | Not |
|---|---|---|
| Arma Reforger Client | ✅ | Workbench lab |
| Dedicated Server | ⏳ | Faz 0 |
| Next.js API | ✅ | Vercel m360-life |
| PostgreSQL | ✅ | Neon + job_definitions |
| Rate limit / metrik | ✅ | [19](./19_guvenlik.md) |
| Redis | İhtiyaçta | |
| Git | ✅ | |

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
| API | Kimlik: `M360_SERVER_KEY`; rate limit; schema validation (sonra); bkz. [19](./19_guvenlik.md) |
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
| Aynı makine/ağda barındırma (ideal) | Dedicated ile API aynı DC’de | Vercel+Neon uzak; lab için kabul. Üretimde düşük latency bölgesi seç |
| Asenkron çağrılar | RestApi callback; oyun donmaz | `M360_ApiIstemci` |
| Önbellekleme | `job_definitions` vb. aralıklı tazele | Prefab Attribute lab; sonra API cache |

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

**Tamamlanan (2026-07-28):** Neon + Vercel pi, güvenlik anahtarı, lab UI, junction, Play RestApi health/jobs.

**Geliştirme sırası:** LabDuzZemin → dedicated (taşınabilir) → Everon yerleştirme (en sonda).

**Sırada:** Dedicated lab paketi (yerel ayağa kaldırma + başka cihaza taşınabilir yapı).

---
*Önceki: [10](./10_panel_hud_admin.md) · Sonraki: [12](./12_lisans_roadmap.md) · Güvenlik: [19](./19_guvenlik.md)*

