# M360 Life - Workbench addon klasorunu GitHub m360-life ile BIRLESTIR (junction)
# Sonuc: iki yol ayni fiziksel dosyalar. Kopyala-yapistir gerekmez.
#
# Workbench yolu = junction (kullanici profiline gore)
# Gercek dosyalar = bu repo icindeki m360-life (script'e gore)
#
# ONEMLI: Workbench KAPALI olmali.
#
# Kullanim:
#   powershell -File tools\bagla-oyun-klasoru.ps1
#   powershell -File tools\bagla-oyun-klasoru.ps1 -Coz
# Onerilen: powershell -File tools\pc-hazirla.ps1

param(
  [switch]$Coz
)

$ErrorActionPreference = "Stop"
. "$PSScriptRoot\Resolve-M360Paths.ps1"

$wb = Get-M360WorkbenchAddon
$gh = Get-M360OyunKaynak
$bak = Get-M360WorkbenchAddonBak

$wbProc = Get-Process -Name "ArmaReforgerWorkbenchSteamDiag" -ErrorAction SilentlyContinue
if ($wbProc) {
  throw "Workbench acik (PID $($wbProc.Id)). Once kapat, sonra bu scripti tekrar calistir."
}

if ($Coz) {
  if (-not (Test-M360Junction $wb)) { throw "Junction yok: $wb" }
  cmd /c "rmdir `"$wb`""
  if (Test-Path -LiteralPath $bak) {
    Rename-Item -LiteralPath $bak -NewName (Split-Path $wb -Leaf)
    Write-Host "Junction kaldirildi; .bak geri yuklendi: $wb"
  } else {
    Write-Host "Junction kaldirildi. GitHub kopyasi duruyor: $gh"
    Write-Host "Istersen: powershell -File tools\sync-game-to-github.ps1 -Yon github-to-workbench"
  }
  exit 0
}

if (-not (Test-Path -LiteralPath $gh)) { throw "GitHub oyun klasoru yok: $gh" }
if (-not (Test-Path -LiteralPath (Join-Path $gh "addon.gproj"))) {
  throw ("addon.gproj yok: {0} - once git pull" -f $gh)
}

if (Test-M360Junction $wb) {
  $target = (Get-Item -LiteralPath $wb).Target
  Write-Host ("Zaten junction: {0} -> {1}" -f $wb, ($target -join ", "))
  $resolvedGh = (Resolve-Path -LiteralPath $gh).Path
  $t0 = if ($target -is [array]) { $target[0] } else { "$target" }
  if ($t0 -and ((Resolve-Path -LiteralPath $t0 -ErrorAction SilentlyContinue).Path -ne $resolvedGh)) {
    Write-Host "UYARI: Junction baska yere bakiyor. Cozup yeniden bagla:"
    Write-Host "  powershell -File tools\bagla-oyun-klasoru.ps1 -Coz"
    Write-Host "  powershell -File tools\bagla-oyun-klasoru.ps1"
  }
  exit 0
}

# Cift PC: git pull sonrasi kaynak = repo. Eski Workbench kopyasi git'i EZMESIN.
# Workbench klasoru yedeklenir (bak); icerik git uzerine kopyalanmaz.
if (Test-Path -LiteralPath $bak) { throw "Yedek zaten var: $bak - elle kontrol et." }

if (Test-Path -LiteralPath $wb) {
  Rename-Item -LiteralPath $wb -NewName (Split-Path $bak -Leaf)
  Write-Host "Eski Workbench kopyasi yedeklendi (git ezilmedi): $bak"
  Write-Host "Istersen sonra sil: $bak"
}

cmd /c "mklink /J `"$wb`" `"$gh`""
if ($LASTEXITCODE -ne 0) {
  if (Test-Path -LiteralPath $bak) {
    Rename-Item -LiteralPath $bak -NewName (Split-Path $wb -Leaf)
  }
  throw "mklink /J basarisiz"
}

Write-Host ""
Write-Host ("BAGLANDI: {0}  <==>  {1}" -f $wb, $gh)
Write-Host "Artik birinde degisen dosya digerinde de ayni."
Write-Host "Workbench ac: addons\M360-Life (junction)."
Write-Host ("Istersen eski yedegi sil: {0} (emin olduktan sonra)." -f $bak)
