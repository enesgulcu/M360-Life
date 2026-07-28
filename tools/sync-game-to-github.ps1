# M360 Life — Oyun senkron (Workbench <-> GitHub)
# MIR YASAK. Sadece oyun klasoru; api/ web/ packages/ docs/ dokunulmaz.
#
# Tercih: tools\bagla-oyun-klasoru.ps1 (junction = tek fiziksel klasor).
# Bu script: junction YOKSA kopyalar; VARSA dogrulama (kopya yok).
#
#   powershell -File tools\sync-game-to-github.ps1
#   powershell -File tools\sync-game-to-github.ps1 -Yon github-to-workbench

param(
  [ValidateSet("workbench-to-github", "github-to-workbench")]
  [string]$Yon = "workbench-to-github"
)

$ErrorActionPreference = "Stop"

$workbench = "C:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life"
$github = "C:\Users\Enes\Documents\GitHub\M360-Life\m360-life"

function Test-IsJunction([string]$Path) {
  if (-not (Test-Path $Path)) { return $false }
  $item = Get-Item $Path -Force
  return [bool]($item.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

if ((Test-IsJunction $workbench)) {
  Write-Host "Junction aktif - kopya gerekmez."
  Write-Host "Workbench: $workbench"
  Write-Host "Hedef:     $((Get-Item $workbench).Target -join ', ')"
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

if (-not (Test-Path $src)) { throw "Kaynak yok: $src" }
if (-not (Test-Path (Split-Path $dst -Parent))) { throw "Hedef ust klasor yok: $(Split-Path $dst -Parent)" }
if (-not (Test-Path $dst)) { New-Item -ItemType Directory -Force -Path $dst | Out-Null }

$includeDirs = @("Configs", "Prefabs", "Scripts", "Worlds", "UI")
$includeFiles = @("addon.gproj")

Write-Host "Yon: $etiket (kopya modu - junction icin: tools\bagla-oyun-klasoru.ps1)"
Write-Host "Kaynak: $src"
Write-Host "Hedef:  $dst"
Write-Host ""

foreach ($d in $includeDirs) {
  $from = Join-Path $src $d
  $to = Join-Path $dst $d
  if (Test-Path $from) {
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
  if (Test-Path $from) {
    Copy-Item $from (Join-Path $dst $f) -Force
    Write-Host "OK $f"
  }
}

Write-Host ""
Write-Host "Senkron bitti: $etiket"
Write-Host "Kalici cozum: powershell -File tools\bagla-oyun-klasoru.ps1"
