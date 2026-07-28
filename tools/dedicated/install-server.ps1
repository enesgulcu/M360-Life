# M360 Life - Arma Reforger Dedicated Server kurulumu (SteamCMD, app 1874900)
#
# Tercih: Steam kutuphane > Araclar > Arma Reforger Server (daha guvenilir).
# Bu script: SteamCMD anonymous. "Missing configuration" olursa Steam UI kullan.
#
#   powershell -File tools\dedicated\install-server.ps1
#   powershell -File tools\dedicated\install-server.ps1 -InstallDir "D:\Games\ArmaReforgerDedicated"

param(
  [string]$InstallDir = "",
  [string]$SteamCmdDir = ""
)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

if (-not $InstallDir) {
  $existing = Find-M360ReforgerServerRoot
  if ($existing) {
    $InstallDir = $existing
    Write-Host ("Mevcut kurulum guncellenecek: {0}" -f $InstallDir)
  } else {
    $InstallDir = Get-M360PreferredServerInstallDir
  }
}
if (-not $SteamCmdDir) {
  $parent = Split-Path (Split-Path $InstallDir -Parent) -Parent
  # ...\steamapps\common\X -> kutuphane kokunde steamcmd
  if ($parent -and (Test-Path -LiteralPath $parent)) {
    $SteamCmdDir = Join-Path $parent "steamcmd"
  } else {
    $SteamCmdDir = Join-Path $env:USERPROFILE "steamcmd"
  }
}

$secrets = Join-Path $here "secrets"
New-Item -ItemType Directory -Force -Path $secrets | Out-Null
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
New-Item -ItemType Directory -Force -Path $SteamCmdDir | Out-Null

$steamcmd = Join-Path $SteamCmdDir "steamcmd.exe"
if (-not (Test-Path -LiteralPath $steamcmd)) {
  Write-Host "SteamCMD indiriliyor..."
  $zip = Join-Path $SteamCmdDir "steamcmd.zip"
  Invoke-WebRequest -Uri "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip" -OutFile $zip
  Expand-Archive -Path $zip -DestinationPath $SteamCmdDir -Force
  Remove-Item $zip -Force
}

Write-Host ("Dedicated kuruluyor: {0} (app 1874900)" -f $InstallDir)
& $steamcmd +force_install_dir $InstallDir +login anonymous +app_update 1874900 validate +quit
Write-Host ("SteamCMD exit: {0}" -f $LASTEXITCODE)

$exe = Join-Path $InstallDir "ArmaReforgerServer.exe"
if (-not (Test-Path -LiteralPath $exe)) {
  Write-Host ""
  Write-Host "SteamCMD binary indiremedi."
  Write-Host "Alternatif: Steam > Kutiphane > Araclar > Arma Reforger Server kur."
  Write-Host "Sonra: powershell -File tools\pc-hazirla.ps1"
  throw ("Kurulum basarisiz - bulunamadi: {0}" -f $exe)
}

Set-Content -Path (Join-Path $secrets "server-root.txt") -Value $InstallDir -Encoding ascii -NoNewline
Write-Host ""
Write-Host ("OK: {0}" -f $exe)
Write-Host "Yol kaydedildi: secrets\server-root.txt"
Write-Host ""
Write-Host "Sonraki: powershell -File tools\pc-hazirla.ps1"
Write-Host "         powershell -File tools\dedicated\start.ps1"
