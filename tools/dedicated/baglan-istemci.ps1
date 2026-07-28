# Istemciyi lokal dedicated'a bagla (Steam'i dogru parametrelerle acar).
# resourceDatabase.rdb ASLA silme.

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$addonsDir = Join-Path $here "addons"
$modGuid = "69F4E91377BCC9A5"

$proc = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
if (-not $proc) {
  Write-Host "Dedicated CALISMIYOR. Once: powershell -File tools\dedicated\start.ps1"
  throw "Sunucu yok"
}

Get-Process -Name "ArmaReforger","ArmaReforgerSteam","ArmaReforger_BE" -ErrorAction SilentlyContinue | ForEach-Object {
  Write-Host ("Istemci kapatiliyor PID {0}" -f $_.Id)
  Stop-Process -Id $_.Id -Force
}
Start-Sleep -Seconds 2

$steam = Find-M360SteamExe
if (-not $steam) { throw "steam.exe yok" }

Write-Host ("Dedicated OK PID {0}" -f ($proc.Id -join ","))
Write-Host "Steam aciliyor..."

Start-Process -FilePath $steam -ArgumentList @(
  "-applaunch", "1874880",
  "-client", "127.0.0.1",
  "-addonsDir", $addonsDir,
  "-addons", $modGuid
)

Write-Host "Test: I + Tab = canta | ESC mouse"
Write-Host "Log: [M360] Inventory <- I"
