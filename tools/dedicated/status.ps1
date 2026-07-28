# M360 Life — dedicated durum ozeti

$ErrorActionPreference = "Continue"
$here = $PSScriptRoot
$secrets = Join-Path $here "secrets"
$rootFile = Join-Path $secrets "server-root.txt"
$root = if (Test-Path $rootFile) { (Get-Content $rootFile -Raw).Trim() } else { Join-Path $env:USERPROFILE "ArmaReforgerDedicated" }
$exe = Join-Path $root "ArmaReforgerServer.exe"

Write-Host "=== M360 dedicated durum ==="
Write-Host ("ServerRoot : {0}" -f $root)
Write-Host ("Binary     : {0}" -f ($(if (Test-Path $exe) { "OK" } else { "YOK - install-server.ps1" })))
Write-Host ("Addon link : {0}" -f ($(if (Test-Path (Join-Path $here "addons\M360-Life\addon.gproj")) { "OK" } else { "YOK - bagla-addon.ps1" })))
Write-Host ("server.json: {0}" -f ($(if (Test-Path (Join-Path $secrets "server.json")) { "OK" } else { "YOK" })))
Write-Host ("API key    : {0}" -f ($(if (Test-Path (Join-Path $secrets "M360_ApiLabKey.txt")) { "OK" } else { "YOK" })))
Write-Host ("Mission    : {0}" -f ($(if (Test-Path (Join-Path $here "..\..\m360-life\Missions\M360_LabDuzZemin.conf")) { "OK" } else { "YOK" })))

$proc = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
if ($proc) {
  Write-Host ("Calisiyor  : PID {0}" -f ($proc.Id -join ","))
} else {
  Write-Host "Calisiyor  : hayir"
}
