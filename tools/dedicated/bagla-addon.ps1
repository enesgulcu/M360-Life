# tools/dedicated/addons/M360-Life -> repo m360-life (junction).

param([switch]$Coz)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
$addonsDir = Join-Path $here "addons"
$repoRoot = (Resolve-Path (Join-Path $here "..\..")).Path
$vendor = Join-Path $repoRoot "tools\vendor"
$workshopVendor = Join-Path $repoRoot "tools\vendor-workshop\addons"

$links = @(
  @{ Name = "DynamicLoot"; Target = (Join-Path $vendor "DynamicLoot"); Required = $true },
  @{ Name = "DynamicEconomy"; Target = (Join-Path $vendor "DynamicEconomy"); Required = $true },
  @{ Name = "ARGH-AmbientVehiclePlugin"; Target = (Join-Path $vendor "ARGH-AmbientVehiclePlugin"); Required = $true },
  @{ Name = "VergysCustomClothing"; Target = (Join-Path $workshopVendor "VergysCustomClothing_59B70A5A19E9B51E"); Required = $true },
  @{ Name = "M360-Life"; Target = (Join-Path $repoRoot "m360-life"); Required = $true }
)

function Test-IsJunction([string]$Path) {
  if (-not (Test-Path $Path)) { return $false }
  $item = Get-Item $Path -Force
  return [bool]($item.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

New-Item -ItemType Directory -Force -Path $addonsDir | Out-Null

# Eski Shop/Bacon junction temizligi
foreach ($eski in @("ShopSystem", "BaconLoadoutEditor")) {
  $eskiLink = Join-Path $addonsDir $eski
  if (Test-IsJunction $eskiLink) {
    cmd /c "rmdir `"$eskiLink`""
    Write-Host ("Eski junction kaldirildi: {0}" -f $eskiLink)
  }
}

foreach ($entry in $links) {
  $link = Join-Path $addonsDir $entry.Name
  $target = $entry.Target

  if (-not (Test-Path -LiteralPath $target)) {
    if ($entry.Required) { throw ("Kaynak addon yok: {0}" -f $target) }
    Write-Host ("ATLANDI: {0}" -f $target)
    continue
  }

  if ($Coz) {
    if (Test-IsJunction $link) {
      cmd /c "rmdir `"$link`""
      Write-Host ("Junction kaldirildi: {0}" -f $link)
    }
    continue
  }

  if (Test-IsJunction $link) {
    $cur = (Get-Item $link).Target
    $t0 = if ($cur -is [array]) { $cur[0] } else { "$cur" }
    $resolvedCur = (Resolve-Path -LiteralPath $t0 -ErrorAction SilentlyContinue).Path
    $resolvedExp = (Resolve-Path -LiteralPath $target -ErrorAction SilentlyContinue).Path
    if ($resolvedCur -and $resolvedExp -and ($resolvedCur -eq $resolvedExp)) {
      Write-Host ("Zaten junction: {0}" -f $link)
      continue
    }
    cmd /c "rmdir `"$link`""
  }

  if (Test-Path -LiteralPath $link) {
    throw ("Klasor var ama junction degil: {0}" -f $link)
  }

  cmd /c "mklink /J `"$link`" `"$target`""
  if ($LASTEXITCODE -ne 0) { throw ("mklink basarisiz: {0}" -f $link) }
  Write-Host ('BAGLANDI: {0} <-> {1}' -f $link, $target)
}
