# M360 Life — dedicated durum ozeti

$ErrorActionPreference = "Continue"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$secrets = Join-Path $here "secrets"
$root = Find-M360ReforgerServerRoot
if (-not $root) { $root = "(bulunamadi)" }
$exeOk = ($root -ne "(bulunamadi)") -and (Test-Path -LiteralPath (Join-Path $root "ArmaReforgerServer.exe"))

Write-Host "=== M360 dedicated durum ==="
Write-Host ("PC         : {0}" -f $env:USERNAME)
Write-Host ("ServerRoot : {0}" -f $root)
Write-Host ("Binary     : {0}" -f ($(if ($exeOk) { "OK" } else { "YOK - Steam Araclar veya install-server.ps1" })))
Write-Host ("Addon link : {0}" -f ($(if (Test-Path -LiteralPath (Join-Path $here "addons\M360-Life\addon.gproj")) { "OK" } else { "YOK - bagla-addon / pc-hazirla" })))
Write-Host ("server.json: {0}" -f ($(if (Test-Path -LiteralPath (Join-Path $secrets "server.json")) { "OK" } else { "YOK" })))
Write-Host ("API key    : {0}" -f ($(if (Test-Path -LiteralPath (Join-Path $secrets "M360_ApiLabKey.txt")) { "OK" } else { "YOK - SECRETS.md" })))
Write-Host ("Mission    : {0}" -f ($(if (Test-Path -LiteralPath (Join-Path $here "..\..\m360-life\Missions\M360_LabDuzZemin.conf")) { "OK" } else { "YOK" })))

$wb = Get-M360WorkbenchAddon
Write-Host ("Workbench J: {0}" -f ($(if (Test-M360Junction $wb) { "OK" } else { "YOK - Workbench kapat + pc-hazirla" })))

$proc = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
if ($proc) {
  Write-Host ("Calisiyor  : PID {0}" -f ($proc.Id -join ","))
} else {
  Write-Host "Calisiyor  : hayir"
}
