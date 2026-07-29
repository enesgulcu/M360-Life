# M360 Life — bu PC'yi hazirla (sabah/aksam PC degisimi)
#
# Amac: git pull sonrasi TEK komut. Junction + dedicated addon + server yolu.
# Sirlar (API key) git'te YOK — her PC'de bir kez elle / USB.
#
#   git pull
#   powershell -File tools\pc-hazirla.ps1
#
# Workbench ACIKSA oyun junction atlanir (uyari); once kapatip tekrar calistir.

param(
  [switch]$SkipWorkbench,
  [switch]$SkipServerDetect
)

$ErrorActionPreference = "Stop"
$tools = $PSScriptRoot
. (Join-Path $tools "Resolve-M360Paths.ps1")

$repo = Get-M360RepoRoot
$oyun = Get-M360OyunKaynak
$wb = Get-M360WorkbenchAddon
$dedicated = Join-Path $tools "dedicated"
$secrets = Join-Path $dedicated "secrets"

Write-Host "=== M360 pc-hazirla ==="
Write-Host ("PC kullanici : {0}" -f $env:USERNAME)
Write-Host ("Repo         : {0}" -f $repo)
Write-Host ("Oyun kaynak  : {0}" -f $oyun)
Write-Host ""

if (-not (Test-Path -LiteralPath (Join-Path $oyun "addon.gproj"))) {
  throw "m360-life/addon.gproj yok. Once: git pull (repo eksik veya yanlis klasor)."
}

New-Item -ItemType Directory -Force -Path $secrets | Out-Null
$keep = Join-Path $secrets ".gitkeep"
if (-not (Test-Path -LiteralPath $keep)) {
  Set-Content -Path $keep -Value "" -Encoding ascii
}

# --- 1) Kiyafet paketi
Write-Host "--- 1/6 Vergys kiyafet paketi ---"
& (Join-Path $dedicated "kur-vergys.ps1")

# --- 2) Hazir arac galerisi bagimliliklari
Write-Host "--- 2/6 ARGH arac galerisi ---"
& (Join-Path $dedicated "kur-argh.ps1")

# --- 3) Dedicated addon junction (Workbench gerekmez)
Write-Host "--- 3/6 dedicated addon junction ---"
& (Join-Path $dedicated "bagla-addon.ps1")

# --- 4) Workbench <-> git junction
Write-Host ""
Write-Host "--- 4/6 Workbench junction ---"
$wbProc = Get-Process -Name "ArmaReforgerWorkbenchSteamDiag" -ErrorAction SilentlyContinue
if ($SkipWorkbench) {
  Write-Host "Atlandi (-SkipWorkbench)."
} elseif ($wbProc) {
  Write-Host ("UYARI: Workbench acik (PID {0}). Junction icin KAPAT, sonra tekrar:" -f ($wbProc.Id -join ","))
  Write-Host "  powershell -File tools\pc-hazirla.ps1"
} else {
  & (Join-Path $tools "bagla-oyun-klasoru.ps1")
  & (Join-Path $tools "bagla-argh-workbench.ps1")
}

# --- 5) server.json sablon
Write-Host ""
Write-Host "--- 5/6 secrets ---"
$sj = Join-Path $secrets "server.json"
$tmpl = Join-Path $dedicated "server.json.template"
if (-not (Test-Path -LiteralPath $sj)) {
  if (Test-Path -LiteralPath $tmpl) {
    Copy-Item $tmpl $sj
    Write-Host "secrets\server.json olusturuldu (sablon)."
  }
} else {
  Write-Host "secrets\server.json OK"
}

$key = Join-Path $secrets "M360_ApiLabKey.txt"
$dbOk = Test-M360DatabaseUrlInEnv
$keyOk = Sync-M360ApiKeyFromEnv
if ($dbOk) { Write-Host "DATABASE_URL (api/.env) OK" } else { Write-Host "UYARI: api/.env icinde DATABASE_URL yok" }
if ($keyOk -and (Test-Path -LiteralPath $key)) {
  Write-Host "API key OK (api/.env -> secrets\M360_ApiLabKey.txt)"
} else {
  Write-Host "UYARI: M360_SERVER_KEY yok. api/.env icine yaz (git'e girmez)."
  Write-Host "  Ornek: api/.env.example"
}

# --- 6) Dedicated binary yolunu bul / kaydet
Write-Host ""
Write-Host "--- 6/6 dedicated binary ---"
if (-not $SkipServerDetect) {
  $root = Find-M360ReforgerServerRoot
  if ($root) {
    Set-Content -Path (Join-Path $secrets "server-root.txt") -Value $root -Encoding ascii -NoNewline
    Write-Host ("Binary OK: {0}" -f $root)
  } else {
    $prefer = Get-M360PreferredServerInstallDir
    Write-Host "Binary YOK. Steam > Araclar > Arma Reforger Server kur (~8 GB)."
    Write-Host ("Beklenen yol ornegi: {0}" -f $prefer)
    Write-Host "Kurulum sonrasi tekrar: powershell -File tools\pc-hazirla.ps1"
    Write-Host "veya SteamCMD: powershell -File tools\dedicated\install-server.ps1"
  }
}

# --- Ozet
Write-Host ""
Write-Host "=== Durum ozeti ==="
& (Join-Path $dedicated "status.ps1")

$wbOk = Test-M360Junction $wb
Write-Host ("Workbench J : {0}" -f ($(if ($wbOk) { "OK junction" } else { "YOK / kopya - Workbench kapatip pc-hazirla" })))

Write-Host ""
Write-Host "Kullanici tek tik: M360.bat  |  oyna: M360-Oyna.bat"
Write-Host "(Detay: TEK_ADIM.txt)"
