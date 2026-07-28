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
  [switch]$SkipKeyCopy
)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
$secrets = Join-Path $here "secrets"
$addonsDir = Join-Path $here "addons"
$keySrc = Join-Path $secrets "M360_ApiLabKey.txt"
$modGuid = "69F4E91377BCC9A5"
$world = "{60855889A2B4FE4E}Worlds/LabDuzZemin/M360_LabDuzZemin.ent"
$bindPort = 2001

if (-not $ServerRoot) {
  $cfgPath = Join-Path $secrets "server-root.txt"
  if (Test-Path $cfgPath) {
    $ServerRoot = (Get-Content $cfgPath -Raw).Trim()
  } else {
    $ServerRoot = "D:\SteamLibrary\steamapps\common\Arma Reforger Server"
  }
}

$exe = Join-Path $ServerRoot "ArmaReforgerServer.exe"
if (-not (Test-Path $exe)) {
  throw ("Dedicated binary yok: {0}" -f $exe)
}

if (-not (Test-Path (Join-Path $addonsDir "M360-Life\addon.gproj"))) {
  Write-Host "Addon junction yok - baglaniyor..."
  & (Join-Path $here "bagla-addon.ps1")
}

$profileName = "M360Dedicated"
# -profile X -> Documents\My Games\X\profile  (ArmaReforger\profile\X DEGIL)
$profileRoot = Join-Path $env:USERPROFILE "Documents\My Games\$profileName\profile"
New-Item -ItemType Directory -Force -Path $profileRoot | Out-Null

if (-not $SkipKeyCopy) {
  if (Test-Path $keySrc) {
    Copy-Item $keySrc (Join-Path $profileRoot "M360_ApiLabKey.txt") -Force
    Write-Host ("API key -> profile\{0}\M360_ApiLabKey.txt" -f $profileName)
  } else {
    Write-Host "UYARI: secrets\M360_ApiLabKey.txt yok"
  }
}

Write-Host ("Server : {0}" -f $exe)
Write-Host ("Addons : {0}" -f $addonsDir)
Write-Host ("Profile: {0}" -f $profileName)
Write-Host ("World  : {0}" -f $world)
Write-Host ("Port   : {0}" -f $bindPort)
Write-Host ""
Write-Host "Istemci Steam baslatma (KALICI, -client YOK):"
Write-Host ('  -addonsDir "{0}" -addons {1}' -f $addonsDir, $modGuid)
Write-Host "Baglanmak icin (sunucu ayaktayken):"
Write-Host "  powershell -File tools\dedicated\baglan-istemci.ps1"
Write-Host ""

$launchArgs = @(
  "-server", $world,
  "-addonsDir", $addonsDir,
  "-addons", $modGuid,
  "-profile", $profileName,
  "-bindPort", "$bindPort",
  "-maxFPS", "60",
  "-logStats", "30"
)

Set-Location $ServerRoot
& $exe @launchArgs
