# tools/dedicated/addons/M360-Life  ->  monorepo m360-life  (junction)
# Dedicated -addonsDir bu klasoru kullanir.

param([switch]$Coz)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
$link = Join-Path $here "addons\M360-Life"
$target = (Resolve-Path (Join-Path $here "..\..\m360-life")).Path

function Test-IsJunction([string]$Path) {
  if (-not (Test-Path $Path)) { return $false }
  $item = Get-Item $Path -Force
  return [bool]($item.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

if ($Coz) {
  if (Test-IsJunction $link) {
    cmd /c "rmdir `"$link`""
    Write-Host "Junction kaldirildi: $link"
  } else {
    Write-Host "Junction yok: $link"
  }
  exit 0
}

New-Item -ItemType Directory -Force -Path (Join-Path $here "addons") | Out-Null

if (Test-IsJunction $link) {
  $cur = (Get-Item $link).Target
  Write-Host "Zaten junction: $link -> $cur"
  exit 0
}

if (Test-Path $link) {
  throw "Klasor var ama junction degil: $link - elle kontrol et."
}

cmd /c "mklink /J `"$link`" `"$target`""
if ($LASTEXITCODE -ne 0) { throw "mklink basarisiz" }

Write-Host ('BAGLANDI: {0} <-> {1}' -f $link, $target)
