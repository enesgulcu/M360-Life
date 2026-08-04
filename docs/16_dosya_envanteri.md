# Döküman 16 — Dosya / İçerik Envanteri (ilişki kataloğu)

*Bağlı: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [15](./15_gelistirme_notlari.md) · [17](./17_yerel_kurulum.md) · [20](./20_kritik_kararlar.md)*

> Her yeni/değişen dosyada güncelle. Okuma: `00` → **20 (kilit)** → tasarım → **16** → **15 §C**.

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
| `Arayuz/M360_TusYoneticisi.c` | Tuş omurgası (I canta, F2 ses; yalnız UI) | Input conf + PlayerController | istemci | kanitli |
| `Arayuz/M360_SesModuYoneticisi.c` | F2: AudioSettings → mevcut ayarın %15'i + SetMasterVolume(x/100); VoiceChat dokunulmaz | TusYoneticisi + CantaHud | istemci | ürün |
| `Arayuz/M360_PlayerControllerI.c` | Yerel `Kur()` — hijack yok | TusYoneticisi | istemci | kanitli |
| `Arayuz/M360_CantaHudBileseni.c` | Life HUD boot + panel + SesModu.Tick | TusYoneticisi | istemci | lab |
| `Configs/System/M360_Input.conf` | `M360_LifeCanta`+`M360_SesModu` + CharacterMovementContext ActionRefs | gproj Default | istemci | lab |
| `Arayuz/M360_CekirdekHudWidgetlari.c` | Circle rings + nakit pill + saat + iş barı | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudIkonlari.c` | Texture ResourceName + Yukle/MaskYukle | YuvarlakBar / Canta / Cekirdek | lab UI | lab |
| `Arayuz/M360_YuvarlakBar.c` | Badge + maskeli progress ring | CekirdekHud | lab UI | lab |
| `Arayuz/M360_CantaPanelWidgetlari.c` | I canta v3 soft9 (9-slice + satır havuzu) | CantaHudBileseni | lab UI | lab |
| `Arayuz/M360_HudYazi.c` | NakitFormat (binlik nokta) | Cekirdek + Canta | lab UI | lab |
| `Magaza/M360_ARGH_NakitKoprusu.c` | Hazır ARGH galerisi ödeme/refund ve bakiye görünümü → M360 HUD nakit | ARGH dealer servisi/UI | dedicated + istemci | kanıtlı |
| `Magaza/M360_KiyafetKatalogu.c` + `M360_KiyafetMagazaUI.c` + aksiyon/RPC | Vergys 97 wrapper + path-only katalog yükleme + prova yığını + vanilla-style preview çocuk temizliği + sınırsız FrameSlot kadraj kontrolü + anında toplu alım | MenuManager layout | lab | güncel |
| `Lab/M360_LabOrtamBileseni.c` | Lab saat/hava: Clear öncelikli parlak gündüz + düşük LV diffuse-only soft güneş | TimeAndWeatherManager + LightEntity | lab | güncel |
| `Prefabs/M360/Magaza/M360_KiyafetMagaza.et` | Kıyafet mağaza terminali | lab layer | lab | yeni |
| `UI/layouts/M360/M360_KiyafetMagaza.layout` + `M360_KiyafetSatir.layout` + `M360_KiyafetKategori.layout` | Tam ekran kıyafet menüsü, ürün/kategori satırları, preview kadraj kontrol butonları | Overlay UI | lab | güncel |

---

## 2b. UI texture / tools

| Yol | Amaç | Birlikte | Durum |
|---|---|---|---|
| `UI/Textures/M360/m360_*_UI.png` / `.edds` | Badge, radial, pill, panel 9-slice, item ikon | HudIkonlari | lab |
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
| `tools/dedicated/kur-argh.ps1` | ARGH + DynamicEconomy + DynamicLoot kaynaklarını her PC’de hazırlar | canlı |
| `tools/dedicated/kur-vergys.ps1` + `vergys-download.json` | Vergys Custom Clothing Workshop paketini her PC’de indirir | canlı |
| `tools/dedicated/bagla-addon.ps1` | M360 + ARGH + Vergys bağımlılık junction’ları | canlı |

Harita değişince: aynı `Isler/*` prefab’ları yerleştir.

---

## 4. Dünya

| Yol | Amaç | Durum |
|---|---|---|
| `Worlds/LabDuzZemin/M360_LabDuzZemin.ent` | Düz zemin lab kök | lab |
| `.../M360_LabDuzZemin_Layers/default.layer` | Pirinç + spawn + HUD + arsenal + ARGH araç galerisi/spawn noktası + parlak gündüz LabOrtam | lab |
| `.../M360_Terrain/` | GenericTerrain data | lab |

Entity: `M360_PirincTopla` · `PirincIsle` · `PirincSat` · `M360_CantaHud` · `M360_ApiTest` · `M360_VanilArsenal` · `M360_ARGHAracGalerisi` · FFA managers.

---

## 5. Platform (GitHub — Workbench dışı)

| Yol | Amaç | Durum |
|---|---|---|
| `api/` | Next.js → Vercel (`/api/health`, `/api/jobs`, `/api/metrik`, lab UI `/`) | canlı |
| `web/` | Admin panel | yer tutucu |
| `packages/db/migrations/001_job_definitions.sql` | Neon iş tanım tablosu | canlı |
| `tools/bagla-oyun-klasoru.ps1` (+ sync) | Junction / senkron | canlı |
| `tools/dedicated/kur-argh.ps1` | Üç açık kaynak bağımlılığı `tools/vendor/` altına indirir ve 1.7 uyumluluğunu uygular | canlı |
| `tools/dedicated/argh/M360_ARGH_VehicleDealer_Vehicles.conf` | 4 araç ve M360 fiyat yapılandırması | canlı |
| `tools/vendor/DynamicLoot/` / `DynamicEconomy/` / `ARGH-AmbientVehiclePlugin/` | Yerel/ignore vendor kaynakları; `M360.bat` yeniden kurar | yerel |
| `tools/vendor-workshop/.../VergysCustomClothing_59B70A5A19E9B51E/` | İndirilen paketlenmiş sivil kıyafet içeriği; git-ignore, `M360.bat` yeniden indirir | yerel |

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
