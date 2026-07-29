# Istemciyi lokal dedicated'a bagla (dogrudan exe = Steam LaunchOptions karismaz).
# resourceDatabase.rdb ASLA silme.

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$addonsDir = Join-Path $here "addons"
$modGuids = "59B70A5A19E9B51E,66B2F0B008DC590F,66F0560F1BDE732A,686A96CE03A95D30,69F4E91377BCC9A5"

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

Write-Host ("Dedicated OK PID {0}" -f ($proc.Id -join ","))
[void](Start-M360Istemci -ServerHost "127.0.0.1" -AddonsDir $addonsDir -ModGuids $modGuids)
Write-Host "Test: I = Life canta | Tab = vanilla | Arsenal/Arac = M360 nakit"
