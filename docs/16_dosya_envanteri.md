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

- **Canon:** `M360.bat` → `tools/pc-hazirla.ps1` (junction — kopya yok)
- İç: `tools/bagla-oyun-klasoru.ps1` (hazirla çağırır)
- **Eskimiş yedek:** `tools/sync-game-to-github.ps1` (tercih etme)
- **`robocopy /MIR` yasak** — `api/` / `web/` / `packages/` / `docs/` silinir.
- Platform: `api/` `web/` `packages/`; oyun: `m360-life/`
- Klasör haritası: `m360-life/YAPI.md` · `Scripts/Game/M360/README.md`

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
| `Arayuz/M360_TusYoneticisi.c` | Tuş→panel omurgası (I Life, Tab vanilla; ileride genişler) | PlayerControllerI | lab UI | lab |
| `Arayuz/M360_PlayerControllerI.c` | InputBinding oturum (Save yok) + ActionOpenInventory | TusYoneticisi | lab UI | lab |
| `Arayuz/M360_CantaHudBileseni.c` | Life HUD boot + panel | TusYoneticisi | lab UI | lab |
| `Arayuz/M360_CekirdekHudWidgetlari.c` | Circle rings + nakit pill + saat + iş barı | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudIkonlari.c` | Texture ResourceName + Yukle/MaskYukle | YuvarlakBar / Canta / Cekirdek | lab UI | lab |
| `Arayuz/M360_YuvarlakBar.c` | Badge + maskeli progress ring | CekirdekHud | lab UI | lab |
| `Arayuz/M360_CantaPanelWidgetlari.c` | I canta v3 soft9 (9-slice + satır havuzu) | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudYazi.c` | NakitFormat (binlik nokta) | Cekirdek + Canta | lab UI | lab |
| `Ag/M360_ApiIstemci.c` | Vercel RestApi health/jobs | BaglantiTesti + lab entity | lab Ag | kanitli |
| `WorkbenchGame/EnfusionMCP/*` | MCP köprü | Cursor | WB only | arac |

---

## 2b. UI texture / tools

| Yol | Amaç | Birlikte | Durum |
|---|---|---|---|
| `UI/Textures/M360/m360_*_UI.png` / `.edds` | Badge, radial, pill, panel 9-slice, item ikon | HudIkonlari | lab |
| `UI/layouts/M360/*.layout` | Diskte; **runtime yüklenmez** (CreateWidgets yasak) | — | bekleyen |
| `tools/gen_circle_hud_textures.py` | Badge/radial DDS üretici | Textures | lab |
| `tools/icon_src/` | MDI/Iconify SVG+PNG kaynak (`node_modules` ignore) | gen script | lab |
| `tools/bagla-oyun-klasoru.ps1 (+ sync-game-to-github.ps1)` | Workbench → GitHub (MIR yasak) | GitHub repo | canlı |
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

Entity: `M360_PirincTopla` (~20,20) · `PirincIsle` (26,20) · `PirincSat` (~31,20) · `M360_CantaHud` · `M360_ApiTest` · FFA managers.

---

## 5. Platform (GitHub — Workbench dışı)

| Yol | Amaç | Durum |
|---|---|---|
| `api/` | Next.js → Vercel (`/api/health`, `/api/jobs`, `/api/metrik`, lab UI `/`) | canlı |
| `web/` | Admin panel | yer tutucu |
| `packages/db/migrations/001_job_definitions.sql` | Neon iş tanım tablosu | canlı |
| `tools/bagla-oyun-klasoru.ps1` (+ sync) | Junction / senkron | canlı |

---

## 6. Doküman

| Dosya | Amaç |
|---|---|
| `00`–`14` | Tasarım |
| `15` | Kanıt / tuzak |
| `16` | Bu envanter |
| `17` | Neon + Vercel kurulum |
| `18` | Junction + iletişim |
| `19` | Güvenlik / rate limit |
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

Ürün: bilesen talep → dedicated → `api/` (Vercel) → Neon PostgreSQL.
