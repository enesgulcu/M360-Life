# M360 Life - dedicated baslat (LOKAL mod)
#
# Not: -config ile -addons AYNI ANDA kullanilamaz (BI).
# Lokal/yayinlanmamis M360 icin: -server (dunya) + -addonsDir + -addons
# Workshop'a cikinca: server.json mods[] + -config yolu kullanilabilir.
#
#   powershell -File tools\dedicated\start.ps1
#   powershell -File tools\dedicated\stop.ps1

param(
  [string]$ServerRoot = "",
  [switch]$SkipKeyCopy,
  [switch]$Background
)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$secrets = Join-Path $here "secrets"
$addonsDir = Join-Path $here "addons"
$keySrc = Join-Path $secrets "M360_ApiLabKey.txt"
$modGuids = "59B70A5A19E9B51E,66B2F0B008DC590F,66F0560F1BDE732A,686A96CE03A95D30,69F4E91377BCC9A5"
$world = "{60855889A2B4FE4E}Worlds/LabDuzZemin/M360_LabDuzZemin.ent"
$bindPort = 2001

if (-not $ServerRoot) {
  $ServerRoot = Find-M360ReforgerServerRoot
}
if (-not $ServerRoot) {
  throw "Dedicated binary bulunamadi. Steam > Araclar > Arma Reforger Server kur, sonra: tools\pc-hazirla.ps1"
}

# Bu PC icin yolu kalici kaydet (git'e girmez)
New-Item -ItemType Directory -Force -Path $secrets | Out-Null
Set-Content -Path (Join-Path $secrets "server-root.txt") -Value $ServerRoot -Encoding ascii -NoNewline

$exe = Join-Path $ServerRoot "ArmaReforgerServer.exe"
if (-not (Test-Path -LiteralPath $exe)) {
  throw ("Dedicated binary yok: {0}" -f $exe)
}

if (
  -not (Test-Path -LiteralPath (Join-Path $addonsDir "M360-Life\addon.gproj")) -or
  -not (Test-Path -LiteralPath (Join-Path $addonsDir "DynamicLoot\addon.gproj")) -or
  -not (Test-Path -LiteralPath (Join-Path $addonsDir "DynamicEconomy\addon.gproj")) -or
  -not (Test-Path -LiteralPath (Join-Path $addonsDir "ARGH-AmbientVehiclePlugin\addon.gproj")) -or
  -not (Test-Path -LiteralPath (Join-Path $addonsDir "VergysCustomClothing\addon.gproj"))
) {
  Write-Host "Addon junction yok - baglaniyor..."
  & (Join-Path $here "bagla-addon.ps1")
}

$profileName = "M360Dedicated"
$profileRoot = Join-Path $env:USERPROFILE "Documents\My Games\$profileName\profile"
New-Item -ItemType Directory -Force -Path $profileRoot | Out-Null

if (-not $SkipKeyCopy) {
  [void](Sync-M360ApiKeyFromEnv)
  if (Test-Path -LiteralPath $keySrc) {
    Copy-Item -LiteralPath $keySrc -Destination (Join-Path $profileRoot "M360_ApiLabKey.txt") -Force
    Write-Host ("API key -> profile\{0}\M360_ApiLabKey.txt (kaynak: api/.env)" -f $profileName)
  } else {
    Write-Host "UYARI: API key yok. api/.env icine M360_SERVER_KEY yaz."
  }
}

Write-Host ("Server : {0}" -f $exe)
Write-Host ("Addons : {0}" -f $addonsDir)
Write-Host ("Profile: {0}" -f $profileName)
Write-Host ("World  : {0}" -f $world)
Write-Host ("Port   : {0}" -f $bindPort)
Write-Host ""
Write-Host "Istemci Steam baslatma (KALICI, -client YOK):"
Write-Host ('  -addonsDir "{0}" -addons {1}' -f $addonsDir, $modGuids)
Write-Host "Baglanmak icin (sunucu ayaktayken):"
Write-Host "  powershell -File tools\dedicated\baglan-istemci.ps1"
Write-Host ""

$launchArgs = @(
  "-server", $world,
  "-addonsDir", $addonsDir,
  "-addons", $modGuids,
  "-profile", $profileName,
  "-bindPort", "$bindPort",
  "-maxFPS", "60",
  "-logStats", "30"
)

if ($Background) {
  $existing = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
  if ($existing) {
    Write-Host ("Zaten calisiyor PID {0}" -f ($existing.Id -join ","))
    return
  }
  Start-Process -FilePath $exe -ArgumentList $launchArgs -WorkingDirectory $ServerRoot
  Write-Host "Sunucu arka planda baslatildi."
  return
}

Set-Location -LiteralPath $ServerRoot
& $exe @launchArgs
