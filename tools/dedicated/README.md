# M360 Life — taşınabilir dedicated (yerel MP)

Lab’da doğrulanan sistemleri **gerçek multiplayer** ortamında çalıştırmak için.
Everon yerleştirme hâlâ en sonda; senaryo şimdilik `LabDuzZemin`.

## Ne kurulu?

| Parça | Yol |
|---|---|
| Paket | `tools/dedicated/` |
| Senaryo | `m360-life/Missions/M360_LabDuzZemin.conf` |
| scenarioId | `{7C2E9A41B8D05F36}Missions/M360_LabDuzZemin.conf` |
| Mod GUID | `69F4E91377BCC9A5` (M360 Life) |
| Addon junction | `tools/dedicated/addons/M360-Life` → `m360-life/` |
| Sırlar | `tools/dedicated/secrets/` (**git’e girmez**) |

## Kurulum (bir kez, bu PC)

```powershell
# Hepsi bir arada:
powershell -File tools\dedicated\setup.ps1

# veya adim adim:
powershell -File tools\dedicated\install-server.ps1
powershell -File tools\dedicated\bagla-addon.ps1
```

SteamCMD ag hatasi verirse (offline): Steam acikken tekrar `install-server.ps1` calistir.

Sifreler: `tools\dedicated\secrets\PASSWORDS.txt` (git'e girmez).
API key: `secrets\M360_ApiLabKey.txt` (lab profilinden kopyalandiysa hazir).

## Çalıştır

```powershell
powershell -File tools\dedicated\start.ps1
powershell -File tools\dedicated\status.ps1   # durum
powershell -File tools\dedicated\stop.ps1     # durdur
```

Lokal mod: `-server` (Lab dünyası) + `-addons` — BI kuralı: `-config` ile `-addons` birlikte olmaz.

- Port: **2001** (oyun) · **17777** (A2S) · **19999** (RCON, localhost)
- `visible: false` — sunucu listesinde görünmez (local lab)
- Profil: `My Games\ArmaReforger\profile\M360Dedicated\` (API key buraya kopyalanır)

Durdur: sunucu penceresinde **Ctrl+C**.

## İstemci bağlan

1. Arma Reforger açık; **M360 Life** modu yüklü (Workbench junction / aynı addon)
2. Multiplayer → **Direct connect** → `127.0.0.1:2001`
3. Admin: `server.json` içindeki `passwordAdmin`

## Başka PC’ye taşıma

1. Bu repo’yu clone / sync et  
2. O PC’de `install-server.ps1` + `bagla-addon.ps1`  
3. `secrets/` dosyalarını güvenli kopyala (git’e koyma)  
4. `start.ps1`

## Geliştirme düzeni

```
Workbench / Cursor  →  m360-life değişir
        ↓ (junction)
dedicated -addonsDir  →  aynı dosyalar
        ↓
istemci Direct Connect
```

Yeni sistem = önce Lab haritada yerleştir → dedicated’ta 2+ oyuncu ile doğrula → sonra Everon.

## Sorun giderme

| Belirti | Kontrol |
|---|---|
| Binary yok | `install-server.ps1` |
| Senaryo bulunamadı | Workbench’te resource rebuild; `M360_LabDuzZemin.conf` + `.meta` |
| Mod yüklenmedi | `bagla-addon.ps1`; `addons\M360-Life\addon.gproj` var mı |
| API 401 | `secrets\M360_ApiLabKey.txt` = Vercel `M360_SERVER_KEY` |
| Port meşgul | `server.json` `bindPort` / `publicPort` değiştir |

Resmi BI: [Server Hosting](https://community.bistudio.com/wiki/Arma_Reforger:Server_Hosting) · [Server Config](https://community.bistudio.com/wiki/Arma_Reforger:Server_Config)
