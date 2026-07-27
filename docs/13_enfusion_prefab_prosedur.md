# Döküman 13 — Enfusion Prefab & Script Yapılandırma Prosedürü

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md) · [11 - Teknik Mimari](./11_teknik_mimari.md)*

> **Durum notu:** Bu döküman, resmi Bohemia Interactive modding dökümantasyonundan (community.bistudio.com/wiki) doğrulanan güncel kurallara dayanıyor. Ancak bazı detaylar (tam API metodları, component davranışları) gerçekten Workbench'te denenmeden %100 kesinleşmez — bu yüzden **"ilk prosedür standardı"** olarak işaretleniyor, Faz 0'da senin Workbench'teki denemelerinle birlikte doğrulanıp güncellenecek.

## 13.1 Neden Bu Döküman Var

Döküman 11 backend mimarisini (Next.js, PostgreSQL, güvenlik) kapsıyor. Döküman 5 iş sisteminin oyun tasarımı mantığını kapsıyor. Ama **"Enfusion içinde bunu gerçekten nasıl kodlarız"** sorusu ayrı bir katman — bu döküman o katmanı standardize ediyor. Amaç: her yeni sistem (iş, market, araç, rol) yazılırken aynı kalıba uyması, böylece "bir kere yaz, çok yerde kullan" ilkesinin (Ana Döküman) gerçekten teknik karşılığı olması.

## 13.2 İsimlendirme Standardı

Bohemia Interactive'in resmi kuralı: her mod geliştiricisi çakışmayı önlemek için **kendine özgü bir Tag (önek)** seçmeli ve tüm class/global fonksiyon/global değişkenlerde bunu kullanmalı.

| Kural | Uygulama |
|---|---|
| Proje etiketi (Tag) | **M360_** — tüm public class, global fonksiyon ve global değişkenlerde |
| Class isimlendirme | PascalCase, çoğul kullanılmaz; **anlam Türkçe (ASCII)** tercih (örn. `M360_IsAyar`, ~~Notifications~~ değil). Lab’daki eski İngilizce class adları (`M360_JobConfig`) kırılmamak için durabilir; yeni yazımlarda Türkçe. |
| Üye değişkenler | `m_` + tip harfi + **Türkçe ASCII PascalCase** (örn. `m_iSatisFiyati`, `m_sIsAdi`, `m_bYasalMi`) |
| Fonksiyon / metod | **Türkçe ASCII PascalCase** (örn. `ToplamaBaslat`, `IslemeBitir`, `EnvanterAcKapa`, `Al`). BI override’lar İngilizce kalır (`OnPostInit`, `PerformAction`). |
| Sabitler | `UPPER_CASE` ASCII Türkçe anlam (örn. `ILERLEME_ADIM_MS`) |
| Yerel değişkenler | camelCase, Türkçe ASCII (`hamMiktar`, `nakit`) |
| Yorumlar | Türkçe (karakter serbest) |
| Türkçe karakter | **İsimlerde yasak** (ş→s, ı→i, ğ→g, ü→u, ö→o, ç→c) — değişken, üye, **metod**, class |
| Dosya/script konumu | Yalnızca `.gproj` içinde tanımlı **Modules** klasörlerinde olabilir — dışarıdaki scriptler motor tarafından yok sayılır |

> Detay ve örnekler: Ana Döküman çalışma protokolü + Döküman 11.2.1 + Döküman 15 Bölüm A.

## 13.3 Config-Driven Prefab Mimarisi — "Bir Kere Yaz, Çok Yerde Kullan"nın Teknik Karşılığı

Enfusion'un resmi **Config Class** mekanizması tam olarak Döküman 5'teki "iş şablonu" fikrinin motor karşılığı. Mantık:

1. **Bir Config Class tanımlanır** (örn. `M360_JobConfig`) — içinde Döküman 5.1b'deki tüm parametreler (`m_iTickDuration`, `m_iTickYield`, `m_iBatchSize`, `m_fConversionRatio`, `m_iSellPrice` vb.) `[Attribute()]` dekoratörüyle işaretlenir. Bu, Resource Manager'da düzenlenebilir bir `.conf` dosyası üretir.
2. **Tek bir generic Component yazılır** (örn. `M360_JobStationComponent`) — bu component, kendi mantığını (toplama/işleme/satış akışı, Döküman 5.1b) **hardcode sayılar yerine bağlı olduğu Config'ten okuyarak** çalıştırır.
3. **Her iş için ayrı bir prefab varyantı** oluşturulur (örn. `M360_JobStation_Pirinc.et`, `M360_JobStation_Gumus.et`) — bunlar aynı base prefab'ın (`M360_JobStation_Base.et`) çocukları, sadece bağlı Config dosyaları farklı.
4. Harita üzerine yerleştirme, bu prefab varyantlarının World Editor'da sürükle-bırak edilmesiyle olur (Döküman 1.2'deki "harita bağımsız prefab kuralı" ile birebir örtüşüyor).

**Sonuç:** Yeni bir iş eklemek = yeni script yazmak değil, yeni bir `.conf` dosyası + prefab varyantı oluşturmak. Script bir kez yazılır.

## 13.4 Config → Admin Panel Köprüsü (Döküman 11 ile bağlantı)

Enfusion'un kendi `.conf` sistemi **Workbench içinde statik** olarak düzenlenir — yani admin panelden canlı değiştirilemez. Bu, Döküman 10.6 ve Ana Döküman'daki "koda dokunmadan yönetim" ilkesiyle bir gerilim yaratıyor. Çözüm mimarisi (Döküman 11 ile tutarlı):

- Prefab'daki Config Class, **başlangıç/varsayılan değerleri** tutar (Workbench'te düzenlenir, build'e gömülür).
- Oyun sunucusu açılışta veya periyodik olarak **PostgreSQL'deki güncel değerleri Next.js API üzerinden çeker** (Döküman 11.1'deki RPC/HTTP akışı) ve component'in çalışma zamanı (runtime) değerlerini bu veriyle **override eder**.
- Yani Config Class = "ilk kurulum/fallback değeri", PostgreSQL = "gerçek, admin panelden değişen değer". Component her zaman önce DB'den gelen değeri kullanır, DB'ye erişilemezse Config'teki varsayılana döner (güvenlik/dayanıklılık için).

> Bu, Döküman 11.9'daki güvenlik ilkesiyle de uyumlu: "istemci sahte para/rol gönderemez" — burada da "prefab'taki gömülü değer tek gerçek kaynak değildir", DB her zaman üstün.

## 13.5 Script Klasör Yapısı (arma-addon içinde, Döküman 11.5 ile genişletildi)

Resmi kural: scriptler `.gproj`'da tanımlı Modules altında olmalı; ilişkili class'lar alt klasörlerde gruplanmalı.

```
arma-addon/
├── M360Life.gproj
├── Scripts/
│   └── Game/
│       └── M360/
│           ├── Core/              # Hesap, karakter, rol state (Döküman 2)
│           ├── Economy/           # Para, vergi, market, K-katsayısı (Döküman 4)
│           ├── Jobs/              # M360_JobStationComponent + config class'lar (Döküman 5)
│           ├── Vehicles/          # Araç sahiplik, garaj, kiralama (Döküman 7)
│           ├── Roles/             # Polis/doktor/rebel, saldırgan profil (Döküman 8)
│           ├── Clans/             # Klan sistemi (Döküman 9)
│           ├── UI/                # HUD, panel, bildirim (Döküman 10)
│           └── Net/               # API/RPC köprü katmanı (Döküman 11)
├── Prefabs/
│   └── M360/
│       ├── Jobs/                  # M360_JobStation_Base.et + varyantlar
│       ├── Markets/
│       ├── Vehicles/
│       └── World/                 # Türkiye teması / World Pack (Döküman 1)
└── Configs/
    └── M360/                      # .conf dosyaları (Config Class instance'ları)
```

## 13.6 Faz 0 İçin Somut İlk Adım

Faz 0'da (Döküman 12.3) yapılacak ilk teknik iş: yukarıdaki klasör iskeletini oluşturmak, `M360_` tag'iyle boş bir test class'ı yazıp derlemek, ve **bir `M360_JobConfig` + tek bir `M360_JobStationComponent` + tek bir test prefab'ı** uçtan uca çalıştırmak (Döküman 5.1b'deki pirinç referans örneğiyle). Bu başarılı olursa, kalan 14 iş sadece config+prefab çoğaltmasıdır — yeni script gerekmez.

## 13.7 Açık Maddeler (Faz 0'da doğrulanacak)

- [ ] Config Class'ın runtime'da PostgreSQL verisiyle override edilme mekanizması — gerçek RPC/API çağrı detayları Workbench'te test edilerek netleşecek
- [ ] `M360_` tag'inin proje genelinde çakışmasız olduğunun teyidi (Bohemia veya başka bir mod tarafından kullanılmadığından emin olunmalı)
- [ ] Tam component API'leri (hangi built-in Enfusion sınıflarından inherit edileceği) — Faz 0'da Workbench'te "Find Symbol" ile araştırılacak

---
*Önceki: [12 - Lisans & Roadmap](./12_lisans_roadmap.md) · Sıradaki: Döküman 14 — Mevcut Modlar, Framework'ler & Geliştirme Araçları*
