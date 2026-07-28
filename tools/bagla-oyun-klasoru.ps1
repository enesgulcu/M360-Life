# M360 Life — Workbench addon klasorunu GitHub m360-life ile BIRLESTIR (junction)
# Sonuc: iki yol ayni fiziksel dosyalar. Kopyala-yapistir gerekmez.
#
# Workbench yolu = junction
# Gercek dosyalar = GitHub\M360-Life\m360-life
#
# ONEMLI: Workbench KAPALI olmali.
#
# Kullanim:
#   powershell -File tools\bagla-oyun-klasoru.ps1
#   powershell -File tools\bagla-oyun-klasoru.ps1 -Coz

param(
  [switch]$Coz
)

$ErrorActionPreference = "Stop"

$wb = "C:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life"
$gh = "C:\Users\Enes\Documents\GitHub\M360-Life\m360-life"
$bak = "C:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life.bak"

$wbProc = Get-Process -Name "ArmaReforgerWorkbenchSteamDiag" -ErrorAction SilentlyContinue
if ($wbProc) {
  throw "Workbench acik (PID $($wbProc.Id)). Once kapat, sonra bu scripti tekrar calistir."
}

function Test-IsJunction([string]$Path) {
  if (-not (Test-Path $Path)) { return $false }
  $item = Get-Item $Path -Force
  return [bool]($item.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

if ($Coz) {
  if (-not (Test-IsJunction $wb)) { throw "Junction yok: $wb" }
  cmd /c "rmdir `"$wb`""
  if (Test-Path $bak) {
    Rename-Item $bak $wb
    Write-Host "Junction kaldirildi; .bak geri yuklendi: $wb"
  } else {
    Write-Host "Junction kaldirildi. GitHub kopyasi duruyor: $gh"
    Write-Host "Istersen: powershell -File tools\sync-game-to-github.ps1 -Yon github-to-workbench"
  }
  exit 0
}

if (-not (Test-Path $gh)) { throw "GitHub oyun klasoru yok: $gh" }

if (Test-IsJunction $wb) {
  $target = (Get-Item $wb).Target
  Write-Host "Zaten junction: $wb -> $target"
  exit 0
}

if (Test-Path $wb) {
  Write-Host "Son kopya: Workbench -> GitHub (baglamadan once)"
  & "$PSScriptRoot\sync-game-to-github.ps1" -Yon workbench-to-github
}

if (Test-Path $bak) { throw "Yedek zaten var: $bak - elle kontrol et." }

if (Test-Path $wb) {
  Rename-Item $wb $bak
  Write-Host "Yedek: $bak"
}

cmd /c "mklink /J `"$wb`" `"$gh`""
if ($LASTEXITCODE -ne 0) {
  if (Test-Path $bak) { Rename-Item $bak $wb }
  throw "mklink /J basarisiz"
}

Write-Host ""
Write-Host "BAGLANDI: $wb  <=>  $gh"
Write-Host "Artik birinde degisen dosya digerinde de ayni."
Write-Host "Workbench'i ac: addons\M360-Life (junction)."
Write-Host "Istersen eski yedegi sil: $bak (emin olduktan sonra)."
