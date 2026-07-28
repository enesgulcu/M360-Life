# M360 Life — Workbench addon ↔ GitHub senkron (MIR YASAK)
# Repo duzeni: m360-life/ (oyun) | api/ | web/ | packages/
#
# Kullanim:
#   powershell -File tools/sync-addon-to-github.ps1

$ErrorActionPreference = "Stop"

# Kaynak: Workbench'te acik addon koku (Scripts/Prefabs burada)
$src = "c:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life\m360-life"
# Hedef: ayni monorepo icindeki m360-life (bu makinede kaynak=hedef olabilir)
$dst = "c:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life\m360-life"

if (-not (Test-Path $src)) { throw "Kaynak yok: $src" }
if (-not (Test-Path $dst)) { New-Item -ItemType Directory -Force -Path $dst | Out-Null }

# Sadece Enfusion addon — api/ web/ packages/ dokunulmaz
$includeDirs = @("Configs", "Prefabs", "Scripts", "Worlds", "UI")
$includeFiles = @("addon.gproj")

foreach ($d in $includeDirs) {
  $from = Join-Path $src $d
  $to = Join-Path $dst $d
  if (Test-Path $from) {
    robocopy $from $to /E /XD .git node_modules /NFL /NDL /NJH /NJS /nc /ns /np | Out-Null
    Write-Host "OK $d"
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
Write-Host "Senkron bitti. api/ web/ packages/ docs/ korunur."
Write-Host "Sonraki: git add -A; git status; git commit; git push"
