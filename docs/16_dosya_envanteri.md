# Döküman 16 — Dosya / İçerik Envanteri (ilişki kataloğu)

*Bağlı olduğu: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [15](./15_gelistirme_notlari.md)*

> **Amaç:** Projede **ne varsa** buraya yazılır: script, prefab, dünya, config, ileride API/sunucu/web dosyaları.  
> Her satır: **ne işe yarar**, **ne ile birlikte kullanılır**, **nerede çalışır** (istemci / dedicated / API / sadece Workbench).  
> **Kural:** Yeni dosya eklenince veya amacı değişince **bu döküman aynı oturumda güncellenir.** AI yeni sohbette `00` → ilgili tasarım → **16** → **15** okur.

---

## 0. Yetki hatırlatması (okumadan geçme)

| Katman | Rol |
|---|---|
| **İstemci (oyuncunun indirdiği mod)** | Görüntü, girdi, talep (F menü, HUD, animasyon). **Kritik karar vermez.** |
| **Dedicated oyun sunucusu** | Talebi doğrular, replication, anti-exploit kapısı |
| **Next.js API + PostgreSQL** | Para, sahiplik, iş tanımı, audit — **kalıcı gerçeklik** |
| **Workbench / enfusion-mcp** | Sadece geliştirme aracı; oyuncuya gitmez |

Lab’daki `M360_IsOturumlari` (ham/nakit sayaç) **geçici stub**tur. Üründe aynı mantık sunucu + DB’ye taşınır; istemci “ben kazandım” diyemez.

Detay: Döküman **11.1**.

---

## 1. Bakım protokolü

Her işlemde kontrol listesi:

1. Yeni / değişen dosya var mı?
2. Bu tabloda satırı var mı? Yoksa ekle.
3. İlişki (prefab ↔ script ↔ dünya ↔ API) doğru mu?
4. `Durum` kolonu: `lab` | `urun-hedef` | `arac` | `taslak` | `silindi`

Şablon satır:

```
| Yol | Amaç (1 cümle) | Birlikte | Çalıştığı yer | Durum |
```

---

## 2. Enfusion — ürün scriptleri (`Scripts/Game/M360/`)

| Yol | Amaç | Birlikte | Çalıştığı yer | Durum |
|---|---|---|---|---|
| `Jobs/M360_IsAyar.c` | İş parametreleri (adım, parti, fiyat…) | Üç site prefab `m_Ayar` | Prefab Attribute (istemci+sunucu aynı asset) | lab |
| `Jobs/M360_IsOturumlari.c` | Oyuncu ham/işlenmiş/nakit **lab sayacı** | Tüm iş bilesenleri + HUD | Şimdilik yerel; **ürün → sunucu/DB** | lab stub |
| `Jobs/M360_ToplamaAlaniBileseni.c` | Toplama ilerleme + ham ekleme | Collect prefab, `ToplaAksiyonu` | Lab: yerel; ürün: sunucu onaylı | lab |
| `Jobs/M360_IslemeMakinesiBileseni.c` | Parti işleme | Process prefab, `IsleAksiyonu` | aynı | lab |
| `Jobs/M360_SatisNoktasiBileseni.c` | İşlenmiş → nakit | Sell prefab, `SatAksiyonu` | aynı | lab |
| `Jobs/M360_ToplaAksiyonu.c` | F menü Topla/Durdur | Collect prefab `additionalActions` | UserAction | lab |
| `Jobs/M360_IsleAksiyonu.c` | F menü İşle | Process prefab | UserAction | lab |
| `Jobs/M360_SatAksiyonu.c` | F menü Sat | Sell prefab | UserAction | lab |
| `Jobs/M360_DurumAksiyonu.c` | F menü Envanter/durum | Üç site + `CantaHudBileseni` | UserAction | lab |
| `UI/M360_CantaHudBileseni.c` | I tuşu çanta ipucu + ilerleme | `IsOturumlari`, dünya `M360_CantaHud` | Lab yerel UI | lab |

Boş klasörler (`Core/`, `Economy/`, `Net/`, …): iskelet — satır eklenince doldurulur.

---

## 3. Prefab’lar (`Prefabs/M360/`)

| Yol | Amaç | Birlikte | Çalıştığı yer | Durum |
|---|---|---|---|---|
| `Jobs/M360_JobCollect_Pirinc.et` | Pirinç toplama çuvalı | `ToplamaAlaniBileseni` + aksiyonlar | Haritada yerleştirilir | lab |
| `Jobs/M360_JobProcess_Pirinc.et` | Pirinç işleme tezgâhı | `IslemeMakinesiBileseni` | Harita | lab |
| `Jobs/M360_JobSell_Pirinc.et` | Pirinç satış kasiyeri | `SatisNoktasiBileseni` | Harita | lab |
| `Jobs/README.md` | Prefab klasör notu | — | geliştirici | lab |
| `Markets/ Vehicles/ World/` | İleride | — | — | taslak |

> Dosya adında `JobCollect` GUID path için kalabilir; **iç class Türkçe** (`M360_ToplamaAlaniBileseni`).

---

## 4. Dünya / harita (`Worlds/`)

| Yol | Amaç | Birlikte | Çalıştığı yer | Durum |
|---|---|---|---|---|
| `TestWorld/M360_TestWorld.ent` | Lab dünya kökü | Layer + terrain + FFA | Workbench Play | lab |
| `TestWorld/M360_TestWorld_Layers/default.layer` | Entity yerleşimi | Pirinç siteleri, spawn, HUD entity | Play | lab |
| `TestWorld/M360_Terrain/` | Lab arazi verisi | GenericTerrain | Play | lab |

**Layer’daki önemli entity’ler:** `M360_PirincTopla` / `PirincIsle` / `PirincSat` / `M360_CantaHud` / `M360_PlayerSpawn` + FFA managers.

---

## 5. Config / proje

| Yol | Amaç | Birlikte | Çalıştığı yer | Durum |
|---|---|---|---|---|
| `addon.gproj` | Addon kimliği, bağımlılık, input Default | Tüm kaynaklar | Workbench/oyun | lab |
| `Configs/System/M360_Input.conf` | Input denemeleri (Inventory→I tutmadı) | gproj InputManagerSettings | lab deneme | lab (I artık `Debug.KeyState`) |

---

## 6. Geliştirme araçları (oyuncuya gitmez)

| Yol | Amaç | Birlikte | Çalıştığı yer | Durum |
|---|---|---|---|---|
| `Scripts/WorkbenchGame/EnfusionMCP/*.c` | Cursor MCP ↔ Workbench köprüsü | enfusion-mcp sunucusu | Sadece Workbench | arac |

Ürün mantığı buraya yazılmaz.

---

## 7. Dokümanlar (`docs/`)

| Yol | Amaç | Durum |
|---|---|---|
| `00` … `14` | Tasarım + prosedür | hazır |
| `15_gelistirme_notlari.md` | Kanıt / tuzak / tercih belleği | canlı |
| `16_dosya_envanteri.md` (bu dosya) | Dosya ilişki kataloğu | canlı |

Kök `README.md`: GitHub giriş + lab özeti.

---

## 8. Henüz yok — yer tutucu (eklenince satır aç)

| Beklenen | Amaç | Çalışacağı yer |
|---|---|---|
| `apps/game-api` (Next.js) | Oyun sunucusunun HTTP API’si | sunucu makinesi |
| `apps/admin-web` | Admin panel | tarayıcı / sunucu |
| `packages/db` | PostgreSQL şema / migration | sunucu |
| Dedicated server config / mission | Canlı oyun host | dedicated |
| `.layout` HUD dosyaları | Ürün Life HUD | istemci asset (veri sunucudan) |

---

## 9. Hızlı ilişki özeti (Pirinç lab)

```
Oyuncu (F / I)
    ↓
Prefab (çuval / tezgâh / kasiyer)  ←→  Aksiyon .c  ←→  Site bileseni .c
                                              ↓
                                    M360_IsAyar (sayılar)
                                              ↓
                                    M360_IsOturumlari (lab sayaç)  ←→  CantaHud
```

Ürün hedefi: site bileseni **talep üretir** → dedicated doğrular → API/DB yazar → istemci sonucu görür.

---
*Önceki: [15](./15_gelistirme_notlari.md) · Bu döküman canlıdır — her dosya değişiminde güncelle.*
