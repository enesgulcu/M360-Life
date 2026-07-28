# M360 Life — taşınabilir dedicated (yerel MP)

Lab’da doğrulanan sistemleri **gerçek multiplayer** ortamında çalıştırmak için.
Everon yerleştirme hâlâ en sonda; senaryo şimdilik `LabDuzZemin`.

**İki PC:** önce `git pull`, sonra `tools\pc-hazirla.ps1` — dedicated addon + server yolu otomatik.

## Ne kurulu?

| Parça | Yol |
|---|---|
| Paket | `tools/dedicated/` |
| Senaryo | `m360-life/Missions/M360_LabDuzZemin.conf` |
| scenarioId | `{7C2E9A41B8D05F36}Missions/M360_LabDuzZemin.conf` |
| Mod GUID | `69F4E91377BCC9A5` (M360 Life) |
| Addon junction | `tools/dedicated/addons/M360-Life` → `m360-life/` |
| Sırlar | `tools/dedicated/secrets/` (**git’e girmez**) — [SECRETS.md](./SECRETS.md) |

## Kurulum (her PC’de bir kez)

```powershell
# Onerilen: tek komut (Workbench kapali)
powershell -File tools\pc-hazirla.ps1

# Dedicated binary yoksa:
# Steam > Kutiphane > Araclar > Arma Reforger Server
# veya: powershell -File tools\dedicated\install-server.ps1
```

SteamCMD “Missing configuration” verirse Steam UI ile kur.

## Çalıştır

**Tek tık (önerilen):** repo kökünde `M360-Oyna.bat` çift tık  
→ junction kontrol → sunucu yoksa aç → Steam istemciyi `127.0.0.1`’e bağla.

```powershell
powershell -File tools\dedicated\oyna.ps1
powershell -File tools\dedicated\start.ps1
powershell -File tools\dedicated\status.ps1
powershell -File tools\dedicated\stop.ps1
```

Lokal: `-server` + `-addons` (`-config` ile birlikte değil). Port **2001**.

## İstemci

`M360-Oyna.bat` yeter. Elle:

```powershell
powershell -File tools\dedicated\baglan-istemci.ps1
```

Steam kalıcı launch: **sadece** `-addonsDir ...\tools\dedicated\addons -addons 69F4E91377BCC9A5` (`-client` kalıcı yazma).

## Senkron (Workbench = GitHub)

`addons\M360-Life` (Workbench) **junction** → repo `m360-life`. Aynı fiziksel dosyalar; kopyala yok.  
Eksikse Workbench kapat → `pc-hazirla.ps1`.

## Başka PC

1. `git pull`
2. `tools\pc-hazirla.ps1`
3. Secrets bir kez ([SECRETS.md](./SECRETS.md))
4. `start.ps1` → `baglan-istemci.ps1`

## Sorun giderme

| Belirti | Kontrol |
|---|---|
| Binary yok | Steam Araclar / `install-server.ps1` / `pc-hazirla` |
| Mod yok | `bagla-addon` / `pc-hazirla` |
| API 401 | `secrets\M360_ApiLabKey.txt` |
| Workbench junction yok | Workbench kapat → `pc-hazirla` |
