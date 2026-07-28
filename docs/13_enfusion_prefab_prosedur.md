# Döküman 13 — Enfusion Prefab & Script Yapılandırma Prosedürü

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md) · [11 - Teknik Mimari](./11_teknik_mimari.md) · [15 - Geliştirme Notları](./15_gelistirme_notlari.md)*

> **Durum (2026-07-27):** Lab’da kanıtlandı. Model = **üç site** + `M360_IsAyar`. İsimler Türkçe ASCII (11.2.1).

## 13.1 Neden Bu Döküman Var

Döküman 11 backend’i, Döküman 5 oyun tasarımını kapsar. Bu dosya **Enfusion içinde nasıl kodlanır** kalıbını standardize eder: her yeni sistem aynı isimlendirme + config + prefab yolunu izler.

## 13.2 İsimlendirme Standardı

| Kural | Uygulama |
|---|---|
| Proje etiketi (Tag) | **M360_** |
| Class | PascalCase **Türkçe ASCII** (`M360_IsAyar`, `M360_ToplamaAlaniBileseni`) — İngilizce class adı yok |
| Üye | `m_` + tip + Türkçe ASCII (`m_iSatisFiyati`, `m_iAdimSuresi`) |
| Metod | Türkçe ASCII (`ToplamaBaslat`). BI override İngilizce (`OnPostInit`, `PerformAction`) |
| Sabit | `UPPER_CASE` ASCII (`ILERLEME_ADIM_MS`) |
| Yerel | camelCase Türkçe ASCII (`hamMiktar`) |
| Yorum | Türkçe (karakter serbest) |
| Türkçe karakter isimde | Yasak (ş→s, ı→i, …) |
| Script konumu | Yalnızca `.gproj` Modules altı |

Detay: Ana Döküman + 11.2.1 + 15-A.

## 13.3 Config-Driven Mimari — Üç Site (güncel, lab kanıtlı)

Döküman 5’teki “tek çekirdek, çok iş” fikrinin motor karşılığı:

1. **Config class:** `M360_IsAyar` — parametreler `[Attribute()]` ile prefab’da düzenlenir:

| Attribute | Anlam |
|---|---|
| `m_sIsAdi` | İş adı |
| `m_bYasalMi` | Yasal mı |
| `m_iAdimSuresi` | Toplama adım süresi (sn) |
| `m_iAdimVerim` | Adım başı ham |
| `m_iPartiBoyutu` | İşleme parti üst sınırı |
| `m_iIslemeSuresi` | İşleme süresi (sn) |
| `m_fDonusumOrani` | Ham → işlenmiş oranı |
| `m_iSatisFiyati` | İşlenmiş birim fiyat |
| `m_iMaxTasima` | Lab max ham taşıma |

2. **Üç bilesen:**

| Component | Aşama | Prefab dosyası (GUID path) |
|---|---|---|
| `M360_ToplamaAlaniBileseni` | Toplama | `M360_Topla_Pirinc.et` |
| `M360_IslemeMakinesiBileseni` | İşleme | `M360_JobProcess_Pirinc.et` |
| `M360_SatisNoktasiBileseni` | Satış | `M360_JobSell_Pirinc.et` |

Her birinde `m_Ayar` → `M360_IsAyar`. Aksiyonlar: `M360_ToplaAksiyonu` / `IsleAksiyonu` / `SatAksiyonu` / `DurumAksiyonu`. HUD: `M360_CantaHudBileseni`.

3. **UserAction:** `additionalActions` + `ParentContextList` + `PointInfo` — Döküman 15 §7.

4. **Yeni iş:** üç prefab kopyala → `m_Ayar` + F menü isimleri → yerleştir.

> **Tarihçe:** Tek `JobStation` silindi; İngilizce `M360_Job*` class adları → Türkçe ASCII (2026-07-27).

## 13.4 Config → Admin Panel Köprüsü

- Prefab `m_Ayar` = **varsayılan / lab / fallback**
- Ürün: PostgreSQL `job_definitions` + Next.js API → runtime override (Döküman 11)
- DB yoksa prefab değeri; DB varsa DB üstün

## 13.5 Script / Prefab Klasör Yapısı (gerçek)

```
m360-life/   (= Workbench addons\M360-Life via junction)
├── addon.gproj
├── Scripts/Game/M360/
│   ├── Isler/
│   ├── Arayuz/
│   ├── Ag/                 # RestApi istemci (lab)
│   └── Core/ Economy/ …    # iskelet (bos stub)
├── Prefabs/M360/
│   ├── Isler/Pirinc/       # Topla / Isle / Sat .et
│   ├── Arayuz/ Dunya/ Pazarlar/ Araclar/
├── Worlds/LabDuzZemin/     # lab harita (asıl gelistirme zemini)
├── UI/ Configs/
└── Scripts/WorkbenchGame/EnfusionMCP/   # arac, urun degil
```

Repo kokunde ayrica: `api/` `web/` `packages/db/` `docs/` `tools/bagla-oyun-klasoru.ps1`

## 13.6 Lab’da Kanıtlanan İlk Adım (tamam)

- [x] Klasör iskeleti + `M360_` tag
- [x] `M360_IsAyar` + üç site bilesen + Pirinç prefab’ları
- [x] UserAction F menü
- [x] Play: ilerleme ipucu + I ile çanta
- [x] Class/Attribute Türkçe ASCII geçişi (11.2.1)
- [ ] Dedicated (yerel, tasinabilir paket) + Neon/API (13.4)

Sayılar: lab vs üretim tablosu → **Döküman 5.7b** + kök README.

## 13.7 Açık Maddeler

- [x] ~~Runtime PostgreSQL override (RPC/HTTP)~~ — lab RestApi health/jobs kanitli; yazma sonra
- [ ] Alet şartı (Döküman 5.1b) — lab’da henüz yok
- [ ] Gerçek kapasite envanteri (lab = `m_iMaxTasima` sayaç)
- [x] ~~Tek vs üç site~~ — üç site kesin
- [ ] Everon yerlestirme — lab + dedicated bitince (harita secimi en sonda)- [x] ~~UserAction `additionalActions`~~ — 15 §7

---
*Önceki: [12 - Lisans & Roadmap](./12_lisans_roadmap.md) · Sıradaki: [14](./14_mevcut_modlar_araclar.md)*
