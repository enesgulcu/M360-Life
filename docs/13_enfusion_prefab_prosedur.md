# Döküman 13 — Enfusion Prefab & Script Yapılandırma Prosedürü

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md) · [11 - Teknik Mimari](./11_teknik_mimari.md) · [15 - Geliştirme Notları](./15_gelistirme_notlari.md)*

> **Durum (2026-07-27):** Lab’da kanıtlandı. Eski “tek JobStation” iskeleti **tarihçe**; güncel model = **üç site** (Collect / Process / Sell) + paylaşılan `M360_JobConfig`. Detaylı tuzaklar: Döküman 15.

## 13.1 Neden Bu Döküman Var

Döküman 11 backend’i, Döküman 5 oyun tasarımını kapsar. Bu dosya **Enfusion içinde nasıl kodlanır** kalıbını standardize eder: her yeni sistem aynı isimlendirme + config + prefab yolunu izler.

## 13.2 İsimlendirme Standardı

| Kural | Uygulama |
|---|---|
| Proje etiketi (Tag) | **M360_** |
| Class | PascalCase; anlam Türkçe ASCII tercih. Lab class adları (`M360_JobConfig`, `M360_JobCollectSiteComponent`) kırılmamak için durabilir |
| Üye | `m_` + tip + Türkçe ASCII (`m_iSatisFiyati`, `m_sIsAdi`, `m_bYasalMi`) |
| Metod | Türkçe ASCII (`ToplamaBaslat`, `IslemeBitir`, `EnvanterAcKapa`). BI override İngilizce (`OnPostInit`, `PerformAction`) |
| Sabit | `UPPER_CASE` ASCII (`ILERLEME_ADIM_MS`) |
| Yerel | camelCase Türkçe ASCII (`hamMiktar`) |
| Yorum | Türkçe (karakter serbest) |
| Türkçe karakter isimde | Yasak (ş→s, ı→i, …) |
| Script konumu | Yalnızca `.gproj` Modules altı |

Detay: Ana Döküman + 11.2.1 + 15-A.

## 13.3 Config-Driven Mimari — Üç Site (güncel, lab kanıtlı)

Döküman 5’teki “tek çekirdek, çok iş” fikrinin motor karşılığı:

1. **Config class:** `M360_JobConfig` — parametreler `[Attribute()]` ile prefab’da düzenlenir:

| Attribute | Anlam |
|---|---|
| `m_sIsAdi` | İş adı |
| `m_bYasalMi` | Yasal mı |
| `m_iTickSuresi` | Toplama tick (sn) |
| `m_iTickVerim` | Tick başı ham |
| `m_iPartiBoyutu` | İşleme batch üst sınırı |
| `m_iIslemeSuresi` | İşleme süresi (sn) |
| `m_fDonusumOrani` | Ham → işlenmiş oranı |
| `m_iSatisFiyati` | İşlenmiş birim fiyat |
| `m_iMaxTasima` | Lab max ham taşıma (ürün = kapasite puanı) |

2. **Üç generic component** (tek monolit istasyon değil):

| Component | Aşama | Prefab örneği |
|---|---|---|
| `M360_JobCollectSiteComponent` | Toplama | `M360_JobCollect_Pirinc.et` |
| `M360_JobProcessSiteComponent` | İşleme | `M360_JobProcess_Pirinc.et` |
| `M360_JobSellSiteComponent` | Satış | `M360_JobSell_Pirinc.et` |

Her birinde `m_Ayar` → aynı `M360_JobConfig` tipi. Sayılar **scriptte hardcoded değil**.

3. **UserAction:** `ActionsManagerComponent` → `additionalActions` (camelCase) + `ParentContextList` + `PointInfo`. Dersler: Döküman 15 §7.

4. **Yeni iş:** üç prefab kopyala → `m_Ayar` + F menü isimleri (+ isteğe bağlı model) → dünyaya koy. Script yok.

> **Tarihçe:** İlk taslak tek `M360_JobStationComponent` / `M360_JobStation_Base.et` idi. Silindi; üç site daha net (lokasyon ayrımı, UserAction, denge).

## 13.4 Config → Admin Panel Köprüsü

- Prefab `m_Ayar` = **varsayılan / lab / fallback**
- Ürün: PostgreSQL `job_definitions` + Next.js API → runtime override (Döküman 11)
- DB yoksa prefab değeri; DB varsa DB üstün

## 13.5 Script / Prefab Klasör Yapısı (gerçek addon)

```
addons/M360 Life/          (Workbench)  ≈  Documents/GitHub/M360-Life (Git)
├── addon.gproj
├── Scripts/Game/M360/
│   ├── Jobs/              # Config, Sessions, Collect/Process/Sell + Action’lar
│   ├── UI/                # JobHud (lab)
│   ├── Core/ Economy/ …   # iskelet
│   └── Net/
├── Prefabs/M360/Jobs/     # Collect/Process/Sell_Pirinc.et (+ ileride Gumus…)
├── Configs/
├── Worlds/TestWorld/      # lab dünya + terrain
└── docs/
```

`Scripts/WorkbenchGame/EnfusionMCP/` — MCP Workbench handler’ları (ürün değil, araç).

## 13.6 Lab’da Kanıtlanan İlk Adım (tamam)

- [x] Klasör iskeleti + `M360_` tag
- [x] `M360_JobConfig` + üç site component + Pirinç prefab’ları
- [x] UserAction F menü (topla / işle / sat / durum)
- [x] Play: ilerleme hint + I ile çanta stub
- [ ] Dedicated + PG override (13.4) — sonraki kapı

Sayılar: lab vs üretim tablosu → **Döküman 5.7b** + kök README.

## 13.7 Açık Maddeler

- [ ] Runtime PostgreSQL override (RPC/HTTP)
- [ ] Alet şartı (Döküman 5.1b) — lab’da henüz yok
- [ ] Gerçek kapasite envanteri (lab = `m_iMaxTasima` sayaç)
- [x] ~~Tek vs üç site~~ — üç site kesin
- [x] ~~UserAction `additionalActions`~~ — 15 §7

---
*Önceki: [12 - Lisans & Roadmap](./12_lisans_roadmap.md) · Sıradaki: [14](./14_mevcut_modlar_araclar.md)*
