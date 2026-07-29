# Workbench addons klasorune ARGH ve ekonomi kaynaklarini junction olarak baglar.

$ErrorActionPreference = "Stop"
. "$PSScriptRoot\Resolve-M360Paths.ps1"

$wbM360 = Get-M360WorkbenchAddon
$wbAddons = Split-Path $wbM360 -Parent
$repo = Get-M360RepoRoot
$vendor = Join-Path $repo "tools\vendor"
$workshopVendor = Join-Path $repo "tools\vendor-workshop\addons"

$wbProc = Get-Process -Name "ArmaReforgerWorkbenchSteamDiag" -ErrorAction SilentlyContinue
if ($wbProc) {
  throw "Workbench acik. ARGH bagimliliklarini baglamak icin once kapat."
}

$links = @(
  @{ Name = "DynamicLoot"; Target = (Join-Path $vendor "DynamicLoot") },
  @{ Name = "DynamicEconomy"; Target = (Join-Path $vendor "DynamicEconomy") },
  @{ Name = "ARGH-AmbientVehiclePlugin"; Target = (Join-Path $vendor "ARGH-AmbientVehiclePlugin") },
  @{ Name = "VergysCustomClothing"; Target = (Join-Path $workshopVendor "VergysCustomClothing_59B70A5A19E9B51E") }
)

foreach ($entry in $links) {
  $name = $entry.Name
  $target = $entry.Target
  $link = Join-Path $wbAddons $name
  if (-not (Test-Path -LiteralPath (Join-Path $target "addon.gproj"))) {
    throw ("ARGH kaynagi eksik: {0}" -f $target)
  }

  if (Test-M360Junction $link) {
    continue
  }
  if (Test-Path -LiteralPath $link) {
    throw ("Workbench addon klasoru var ama junction degil: {0}" -f $link)
  }

  cmd /c "mklink /J `"$link`" `"$target`""
  if ($LASTEXITCODE -ne 0) {
    throw ("Workbench junction basarisiz: {0}" -f $name)
  }
}

Write-Host "Workbench ARGH bagimliliklari hazir."
