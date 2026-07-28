# M360 Life — Oyun senkron (Workbench <-> GitHub)
# MIR YASAK. Sadece oyun klasoru; api/ web/ packages/ docs/ dokunulmaz.
#
# ESKIMIS / YEDEK: Tercih etme.
# Canon: M360.bat → tools\pc-hazirla.ps1 (junction).
# Bu script: junction YOKSA kopyalar; VARSA dogrulama (kopya yok).
#
#   powershell -File tools\sync-game-to-github.ps1
#   powershell -File tools\sync-game-to-github.ps1 -Yon github-to-workbench

param(
  [ValidateSet("workbench-to-github", "github-to-workbench")]
  [string]$Yon = "workbench-to-github"
)

$ErrorActionPreference = "Stop"
. "$PSScriptRoot\Resolve-M360Paths.ps1"

$workbench = Get-M360WorkbenchAddon
$github = Get-M360OyunKaynak

if ((Test-M360Junction $workbench)) {
  Write-Host "Junction aktif - kopya gerekmez."
  Write-Host "Workbench: $workbench"
  Write-Host "Hedef:     $((Get-Item -LiteralPath $workbench).Target -join ', ')"
  Write-Host "GitHub:    $github"
  Write-Host "Dosyalar zaten ayni fiziksel konumda."
  exit 0
}

if ($Yon -eq "workbench-to-github") {
  $src = $workbench
  $dst = $github
  $etiket = "Workbench -> GitHub m360-life"
} else {
  $src = $github
  $dst = $workbench
  $etiket = "GitHub m360-life -> Workbench"
}

if (-not (Test-Path -LiteralPath $src)) { throw "Kaynak yok: $src" }
if (-not (Test-Path -LiteralPath (Split-Path $dst -Parent))) {
  throw "Hedef ust klasor yok: $(Split-Path $dst -Parent)"
}
if (-not (Test-Path -LiteralPath $dst)) {
  New-Item -ItemType Directory -Force -Path $dst | Out-Null
}

$includeDirs = @("Configs", "Prefabs", "Scripts", "Worlds", "UI", "Missions")
$includeFiles = @("addon.gproj")

Write-Host "Yon: $etiket (kopya modu - junction icin: tools\pc-hazirla.ps1)"
Write-Host "Kaynak: $src"
Write-Host "Hedef:  $dst"
Write-Host ""

foreach ($d in $includeDirs) {
  $from = Join-Path $src $d
  $to = Join-Path $dst $d
  if (Test-Path -LiteralPath $from) {
    robocopy $from $to /E /XD .git node_modules /XF *.rdb /NFL /NDL /NJH /NJS /nc /ns /np | Out-Null
    $code = $LASTEXITCODE
    if ($code -ge 8) { throw "robocopy hata ($code): $d" }
    Write-Host "OK $d"
  } else {
    Write-Host "ATLANDI (yok) $d"
  }
}

foreach ($f in $includeFiles) {
  $from = Join-Path $src $f
  if (Test-Path -LiteralPath $from) {
    Copy-Item -LiteralPath $from -Destination (Join-Path $dst $f) -Force
    Write-Host "OK $f"
  }
}

Write-Host ""
Write-Host "Senkron bitti: $etiket"
Write-Host "Kalici cozum: powershell -File tools\pc-hazirla.ps1"
