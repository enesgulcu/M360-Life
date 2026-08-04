# M360 Life - tek tik: sunucu YENIDEN + istemci baglan
#
# Cift tik: repo kokundeki M360-Oyna.bat
# veya: powershell -File tools\dedicated\oyna.ps1
#
# ONEMLI: Dedicated her seferinde yeniden baslar.
# Aksi halde script degisince "checksum does not match" -> Init Error.
# NOT: Bu dosyada sadece ASCII (Windows PowerShell UTF-8 tireleri kirar).

param(
  [int]$HazirBeklemeSn = 120,
  [switch]$SadeceSunucu,
  [switch]$SkipJunctionCheck,
  [switch]$MevcutSunucuyuKullan,
  [switch]$SunucuAcikKalsin
)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$repo = Get-M360RepoRoot
$oyun = Get-M360OyunKaynak
$wb = Get-M360WorkbenchAddon
$addonsDir = Join-Path $here "addons"
$modGuids = "59B70A5A19E9B51E,66B2F0B008DC590F,66F0560F1BDE732A,686A96CE03A95D30,69F4E91377BCC9A5"
$bindPort = 2001

function Write-Adim([string]$Msg) {
  Write-Host ""
  Write-Host ("=== {0} ===" -f $Msg)
}

function Test-JunctionHedef([string]$LinkPath, [string]$ExpectedTarget) {
  if (-not (Test-M360Junction $LinkPath)) { return $false }
  $target = (Get-Item -LiteralPath $LinkPath).Target
  $t0 = if ($target -is [array]) { $target[0] } else { "$target" }
  if (-not $t0) { return $false }
  $resolvedLink = (Resolve-Path -LiteralPath $t0 -ErrorAction SilentlyContinue).Path
  $resolvedExp = (Resolve-Path -LiteralPath $ExpectedTarget -ErrorAction SilentlyContinue).Path
  return ($resolvedLink -and $resolvedExp -and ($resolvedLink -eq $resolvedExp))
}

function Get-M360DedicatedLogDir {
  $logsRoot = Join-Path $env:USERPROFILE "Documents\My Games\M360Dedicated\logs"
  if (-not (Test-Path -LiteralPath $logsRoot)) { return $null }
  $latest = Get-ChildItem -LiteralPath $logsRoot -Directory -ErrorAction SilentlyContinue |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1
  if (-not $latest) { return $null }
  return $latest.FullName
}

function Test-M360ServerRplHazir([datetime]$NotBefore) {
  $dir = Get-M360DedicatedLogDir
  if (-not $dir) { return $false }
  $dirInfo = Get-Item -LiteralPath $dir -ErrorAction SilentlyContinue
  if (-not $dirInfo) { return $false }
  # Eski oturum logu "RPL listening" icerir - sadece bu baslatmadan sonraki klasor
  if ($dirInfo.CreationTime -lt $NotBefore.AddSeconds(-15)) { return $false }
  $log = Join-Path $dir "console.log"
  if (-not (Test-Path -LiteralPath $log)) { return $false }
  $hit = Select-String -LiteralPath $log -Pattern "Starting RPL server, listening" -SimpleMatch -ErrorAction SilentlyContinue |
    Select-Object -First 1
  return [bool]$hit
}

Write-Host "M360 Oyna - hizli baslat"
Write-Host ("PC   : {0}" -f $env:USERNAME)
Write-Host ("Repo : {0}" -f $repo)

if ($repo -match '(?i)[\\/]Downloads[\\/]M360-Life-main') {
  $dogru = Join-Path $env:USERPROFILE "Documents\GitHub\M360-Life"
  Write-Host ""
  Write-Host "HATA: Oyun Downloads ZIP kopyasindan baslatiliyor."
  Write-Host ("  {0}" -f $repo)
  Write-Host ("Dogru bat: {0}\M360-Oyna.bat" -f $dogru)
  throw "Yanlis M360 klasoru (Downloads ZIP). GitHub yolunu kullan."
}

# Vergys data.pak dogrulama (0 byte = 3D kiyafet asla calismaz)
$vergysPak = Join-Path $repo "tools\vendor-workshop\addons\VergysCustomClothing_59B70A5A19E9B51E\data.pak"
if (-not (Test-Path -LiteralPath $vergysPak) -or ((Get-Item -LiteralPath $vergysPak).Length -lt 400MB)) {
  $len = 0
  if (Test-Path -LiteralPath $vergysPak) { $len = (Get-Item -LiteralPath $vergysPak).Length }
  Write-Host ("UYARI: Vergys data.pak eksik/kucuk ({0} byte). kur-vergys calisacak..." -f $len)
}

# --- 1) Kaynak + junction ---
Write-Adim "1/4 Senkron kontrol (junction)"

if (-not (Test-Path -LiteralPath (Join-Path $oyun "addon.gproj"))) {
  throw "m360-life/addon.gproj yok. Once: git pull"
}

if (-not $SkipJunctionCheck) {
  & (Join-Path $here "kur-vergys.ps1")
  & (Join-Path $here "kur-argh.ps1")
  & (Join-Path $here "bagla-addon.ps1")
  $dedLink = Join-Path $addonsDir "M360-Life"
  if (-not (Test-JunctionHedef $dedLink $oyun)) {
    Write-Host "Dedicated addon junction eksik/yanlis - baglaniyor..."
    & (Join-Path $here "bagla-addon.ps1")
  } else {
    Write-Host ("Dedicated OK  : {0} -> m360-life" -f $dedLink)
  }

  if (Test-JunctionHedef $wb $oyun) {
    Write-Host ("Workbench OK  : {0} -> m360-life" -f $wb)
    Write-Host "  (WB veya Cursor dosya degisince digeri ANINDA ayni - kopyala yok)"
  } else {
    $wbProc = Get-Process -Name "ArmaReforgerWorkbenchSteamDiag" -ErrorAction SilentlyContinue
    if ($wbProc) {
      Write-Host "UYARI: Workbench acik ve junction YOK/yanlis. Dosyalar ayri klasorde olabilir."
      Write-Host "  Workbench KAPAT, sonra: powershell -File tools\pc-hazirla.ps1"
    } else {
      Write-Host "Workbench junction yok - baglaniyor (git ezilmez)..."
      & (Join-Path $here "..\bagla-oyun-klasoru.ps1")
    }
  }
}

[void](Sync-M360ApiKeyFromEnv)

# --- 2) Dedicated (varsayilan: YENIDEN baslat - checksum eslesmesi icin) ---
Write-Adim "2/4 Dedicated sunucu"

$proc = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
$baslangic = Get-Date
if ($proc -and $MevcutSunucuyuKullan) {
  Write-Host ("Mevcut sunucu kullaniliyor PID {0}" -f ($proc.Id -join ","))
  Write-Host "  (script degistiyse Init Error alirsin - normalde bat sunucuyu yeniden acar)"
  $baslangic = $proc[0].StartTime
} else {
  if ($proc) {
    Write-Host "Eski sunucu durduruluyor (taze script checksum)..."
    & (Join-Path $here "stop.ps1")
    Start-Sleep -Seconds 2
  }
  Write-Host "Sunucu baslatiliyor..."
  $baslangic = Get-Date
  & (Join-Path $here "start.ps1") -Background
}

$deadline = (Get-Date).AddSeconds([Math]::Max(30, $HazirBeklemeSn))
$hazir = $false
while ((Get-Date) -lt $deadline) {
  $proc = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
  if (-not $proc) {
    Start-Sleep -Seconds 2
    continue
  }
  if (Test-M360ServerRplHazir $baslangic) {
    $hazir = $true
    break
  }
  $uptime = ((Get-Date) - $proc[0].StartTime).TotalSeconds
  Write-Host ("  Yukleniyor... (~{0:N0}s, RPL bekleniyor)" -f $uptime)
  Start-Sleep -Seconds 3
}

if (-not (Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue)) {
  throw "ArmaReforgerServer baslamadi. Steam > Araclar > Arma Reforger Server kurulu mu? status.ps1 bak."
}

if (-not $hazir) {
  Write-Host "UYARI: RPL satiri logda gorulmedi; yine de istemci acilacak."
} else {
  Write-Host ("Sunucu hazir (port {0}, PID {1})" -f $bindPort, ((Get-Process -Name "ArmaReforgerServer").Id -join ","))
}

if ($SadeceSunucu) {
  Write-Host "SadeceSunucu - istemci acilmadi."
  exit 0
}

# --- 3) Istemci ---
Write-Adim "3/4 Istemci (Steam)"

Get-Process -Name "ArmaReforger","ArmaReforgerSteam","ArmaReforger_BE" -ErrorAction SilentlyContinue | ForEach-Object {
  Write-Host ("Eski istemci kapatiliyor PID {0}" -f $_.Id)
  Stop-Process -Id $_.Id -Force
}
Start-Sleep -Seconds 2

[void](Start-M360Istemci -ServerHost "127.0.0.1" -AddonsDir $addonsDir -ModGuids $modGuids)

Write-Adim "4/4 Tamam"
Write-Host "Oyun aciliyor -> 127.0.0.1 (M360 Life)."
Write-Host "Test: I = Life canta | Tab = normal envanter | NPC'de F = magaza"

if ($SunucuAcikKalsin) {
  Write-Host "SunucuAcikKalsin: oyun bitsa de dedicated acik kalir."
  Write-Host "Durdur: powershell -File tools\dedicated\stop.ps1"
  exit 0
}

# ArmaReforger VEYA ArmaReforgerSteam (isim PC'ye gore degisir)
Write-Host ""
Write-Host "Oyun sureci bekleniyor..."
Write-Host "Oyun kapaninca bu pencere + dedicated kapanacak."

function Get-M360IstemciProc {
  $p = Get-Process -Name "ArmaReforger" -ErrorAction SilentlyContinue
  if ($p) { return ($p | Sort-Object StartTime | Select-Object -Last 1) }
  $p = Get-Process -Name "ArmaReforgerSteam" -ErrorAction SilentlyContinue
  if ($p) { return ($p | Sort-Object StartTime | Select-Object -Last 1) }
  return $null
}

$oyunPid = $null
$araDeadline = (Get-Date).AddSeconds(180)
while ((Get-Date) -lt $araDeadline) {
  $aday = Get-M360IstemciProc
  if ($aday) {
    Write-Host ("Aday {0} PID {1} - 15 sn stabilite..." -f $aday.ProcessName, $aday.Id)
    Start-Sleep -Seconds 15
    $hala = Get-Process -Id $aday.Id -ErrorAction SilentlyContinue
    if ($hala) {
      $oyunPid = $aday.Id
      break
    }
    Write-Host "Kisa sureli process - yeniden bekleniyor..."
  }
  Start-Sleep -Seconds 2
}

if (-not $oyunPid) {
  Write-Host "UYARI: Istemci 3 dk stabil acilmadi. Dedicated durduruluyor."
  & (Join-Path $here "stop.ps1")
  Write-Host "Pencere 10 sn sonra kapanir."
  Start-Sleep -Seconds 10
  exit 0
}

Write-Host ("Oyun stabil PID {0} - kapaninca temizlik..." -f $oyunPid)
while ($true) {
  $canli = Get-M360IstemciProc
  if (-not $canli) {
    Start-Sleep -Seconds 5
    if (-not (Get-M360IstemciProc)) { break }
  }
  Start-Sleep -Seconds 3
}

Start-Sleep -Seconds 2
Get-Process -Name "ArmaReforger","ArmaReforgerSteam","ArmaReforger_BE" -ErrorAction SilentlyContinue | ForEach-Object {
  Stop-Process -Id $_.Id -Force -ErrorAction SilentlyContinue
}

Write-Host "Oyun kapandi - dedicated durduruluyor..."
& (Join-Path $here "stop.ps1")
Write-Host "Temiz. Pencere 5 sn sonra kapanir."
Start-Sleep -Seconds 5
