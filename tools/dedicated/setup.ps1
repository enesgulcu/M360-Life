# M360 Life - tek seferlik dedicated hazirlik
# Onerilen: ust seviye tools\pc-hazirla.ps1 (Workbench junction dahil)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot

Write-Host "Not: Tam hazirlik icin repo kokunden: powershell -File tools\pc-hazirla.ps1"
Write-Host "=== 1/3 install-server ==="
& (Join-Path $here "install-server.ps1")

Write-Host "=== 2/3 bagla-addon ==="
& (Join-Path $here "bagla-addon.ps1")

if (-not (Test-Path (Join-Path $here "secrets\server.json"))) {
  Copy-Item (Join-Path $here "server.json.template") (Join-Path $here "secrets\server.json")
  Write-Host "secrets\server.json olusturuldu - sifreleri kontrol et"
}

Write-Host "=== 3/3 status ==="
& (Join-Path $here "status.ps1")

Write-Host ""
Write-Host "Hazirsa: powershell -File tools\dedicated\start.ps1"
