# Istemciyi lokal dedicated'a bagla (Steam'i dogru parametrelerle acar).
# resourceDatabase.rdb ASLA silme.

$ErrorActionPreference = "Stop"
$addonsDir = Join-Path $PSScriptRoot "addons"
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

$steam = "C:\Program Files (x86)\Steam\steam.exe"
if (-not (Test-Path -LiteralPath $steam)) { throw "steam.exe yok" }

Write-Host ("Dedicated OK PID {0}" -f ($proc.Id -join ","))
Write-Host "Steam aciliyor..."

Start-Process -FilePath $steam -ArgumentList @(
  "-applaunch", "1874880",
  "-client", "127.0.0.1",
  "-addonsDir", $addonsDir,
  "-addons", $modGuid
)

Write-Host "Test: I=canta | Tab=vanilla | ESC=mouse"
Write-Host "Logda olmali: InputManager config load ... M360_Input.conf"
Write-Host "Logda OLMAMALI: Unknown class Context"
