# M360 Life — Workbench addon → GitHub senkron (MIR YASAK)
# apps/ ve packages/ GitHub tarafinda kalir; silinmez.
#
# Kullanim:
#   powershell -File tools/sync-addon-to-github.ps1

$ErrorActionPreference = "Stop"
$src = "c:\Users\enesg\Documents\My Games\ArmaReforgerWorkbench\addons\M360 Life"
$dst = "C:\Users\enesg\Documents\GitHub\M360-Life"

if (-not (Test-Path $src)) { throw "Kaynak yok: $src" }
if (-not (Test-Path $dst)) { New-Item -ItemType Directory -Force -Path $dst | Out-Null }

# Sadece Enfusion addon klasor/dosyalari (platform apps dokunulmaz)
$includeDirs = @("Configs", "docs", "Prefabs", "Scripts", "Worlds", "UI", "tools", ".cursor")
$includeFiles = @("addon.gproj", "README.md", ".gitignore")

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

# Kokteki gereksiz konusma notlarini GitHub'dan temizleme (arsiv docs'ta)
@(
  "cursor agent konusmalar.md",
  "cursor code konusmalar.md"
) | ForEach-Object {
  $p = Join-Path $dst $_
  if (Test-Path $p) { Remove-Item $p -Force; Write-Host "silindi $_" }
}

Write-Host ""
Write-Host "Senkron bitti. apps/ ve packages/ korunur."
Write-Host "Sonraki: cd `"$dst`"; git add -A; git status; git commit; git push"
