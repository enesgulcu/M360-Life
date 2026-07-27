# Döküman 16 — Dosya / İçerik Envanteri (ilişki kataloğu)

*Bağlı: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [15](./15_gelistirme_notlari.md) · [17](./17_yerel_kurulum.md)*

> Her yeni/değişen dosyada güncelle. Okuma: `00` → tasarım → **16** → **15**.

---

## 0. Yetki

| Katman | Rol |
|---|---|
| İstemci mod | Görüntü / girdi / talep |
| Dedicated | Doğrulama / replication |
| Next.js + PostgreSQL | Kalıcı gerçeklik |
| Workbench MCP | Sadece geliştirme |

---

## 1. Senkron kuralı (kritik)

- Workbench → GitHub: `tools/sync-addon-to-github.ps1`
- **`robocopy /MIR` yasak** — `apps/` ve `packages/` silinir.
- Platform kodu yalnız `Documents\GitHub\M360-Life\apps|packages`.

---

## 2. Enfusion script (`Scripts/Game/M360/`)

| Yol | Amaç | Birlikte | Yer | Durum |
|---|---|---|---|---|
| `Isler/M360_IsAyar.c` | İş parametreleri (+ `m_fIptalMesafesi`) | Üç site prefab | Attribute | lab |
| `Isler/M360_IsOturumlari.c` | Lab ham/işlenmiş/nakit + aclik/susuzluk stub | Siteler + HUD | yerel stub | lab |
| `Isler/M360_ToplamaAlaniBileseni.c` | Toplama + **mesafe iptal** | Topla prefab | lab | lab |
| `Isler/M360_IslemeMakinesiBileseni.c` | İşleme + **mesafe iptal** | Isle prefab | lab | lab |
| `Isler/M360_SatisNoktasiBileseni.c` | Satış anlık | Sat prefab | lab | lab |
| `Isler/M360_ToplaAksiyonu.c` | F Topla/Durdur | Collect | UserAction | lab |
| `Isler/M360_IsleAksiyonu.c` | F Isle | Process | UserAction | lab |
| `Isler/M360_SatAksiyonu.c` | F Sat | Sell | UserAction | lab |
| `Isler/M360_DurumAksiyonu.c` | F Envanter | HUD | UserAction | lab |
| `Arayuz/M360_CantaHudBileseni.c` | Life HUD boot + I canta | CreateWidget + oturum | lab UI | lab |
| `Arayuz/M360_CekirdekHudWidgetlari.c` | Circle rings + nakit pill + saat + iş barı | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudIkonlari.c` | Texture ResourceName + Yukle/MaskYukle | YuvarlakBar / Canta / Cekirdek | lab UI | lab |
| `Arayuz/M360_YuvarlakBar.c` | Badge + maskeli progress ring | CekirdekHud | lab UI | lab |
| `Arayuz/M360_CantaPanelWidgetlari.c` | I canta v3 soft9 (9-slice + satır havuzu) | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudYazi.c` | NakitFormat (binlik nokta) | Cekirdek + Canta | lab UI | lab |
| `WorkbenchGame/EnfusionMCP/*` | MCP köprü | Cursor | WB only | arac |

---

## 2b. UI texture / tools

| Yol | Amaç | Birlikte | Durum |
|---|---|---|---|
| `UI/Textures/M360/m360_*_UI.png` / `.edds` | Badge, radial, pill, panel 9-slice, item ikon | HudIkonlari | lab |
| `UI/layouts/M360/*.layout` | Diskte; **runtime yüklenmez** (CreateWidgets yasak) | — | bekleyen |
| `tools/gen_circle_hud_textures.py` | Badge/radial DDS üretici | Textures | lab |
| `tools/icon_src/` | MDI/Iconify SVG+PNG kaynak (`node_modules` ignore) | gen script | lab |
| `tools/sync-addon-to-github.ps1` | Workbench → GitHub (MIR yasak) | GitHub repo | canlı |
| `.cursor/rules/m360-reforger-hud.mdc` | Agent HUD kuralları | Cursor | canlı |

## 3. Prefab

| Yol | Amaç | Birlikte | Durum |
|---|---|---|---|
| `Prefabs/M360/Isler/Pirinc/M360_Topla_Pirinc.et` | Pirinç çuval | ToplamaAlaniBileseni | lab |
| `.../M360_Isle_Pirinc.et` | Tezgâh | IslemeMakinesiBileseni | lab |
| `.../M360_Sat_Pirinc.et` | Kasiyer | SatisNoktasiBileseni | lab |
| `Prefabs/M360/Isler/README.md` | Klasör notu | — | lab |
| `Prefabs/M360/Arayuz/` | İleride prefab HUD | layout’lar `UI/layouts/M360/` | taslak |
| `Prefabs/M360/Dunya/` | İleride spawn prefab | — | taslak |
| `Prefabs/M360/Pazarlar/` `Araclar/` | İleride | — | taslak |

Harita değişince: aynı `Isler/*` prefab’ları yerleştir.

---

## 4. Dünya

| Yol | Amaç | Durum |
|---|---|---|
| `Worlds/LabDuzZemin/M360_LabDuzZemin.ent` | Düz zemin lab kök | lab |
| `.../M360_LabDuzZemin_Layers/default.layer` | Pirinç + spawn + HUD | lab |
| `.../M360_Terrain/` | GenericTerrain data | lab |

Entity: `M360_PirincTopla` (~20,20) · `PirincIsle` (26,20) · `PirincSat` (~31,20) · `M360_CantaHud` · FFA managers.

---

## 5. Platform (GitHub — Workbench dışı)

| Yol | Amaç | Durum |
|---|---|---|
| `apps/game-api` | Next.js API :3100 (`/api/health`, `/api/jobs`) | iskelet |
| `apps/admin-web` | Admin panel | yer tutucu |
| `packages/db/migrations/001_job_definitions.sql` | İş tanım tablosu | iskelet |
| `tools/sync-addon-to-github.ps1` | Güvenli senkron | canlı |

---

## 6. Doküman

| Dosya | Amaç |
|---|---|
| `00`–`14` | Tasarım |
| `15` | Kanıt / tuzak |
| `16` | Bu envanter |
| `17` | Yerel kurulum rehberi |
| `docs/arsiv/` | Eski konuşma notları |

---

## 7. Pirinç ilişki özeti

```
Oyuncu F/I → Prefab Isler/Pirinc → Aksiyon → Bilesen (+ mesafe iptal)
                         ↓
                   M360_IsAyar (dinamik)
                         ↓
                   IsOturumlari (lab) → CantaHud
```

Ürün: bilesen talep → dedicated → game-api → PostgreSQL.
