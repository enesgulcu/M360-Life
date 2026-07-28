# M360 Life - Arma Reforger Dedicated Server kurulumu (SteamCMD, app 1874900)
#
# Kullanim:
#   powershell -File tools\dedicated\install-server.ps1
#   powershell -File tools\dedicated\install-server.ps1 -InstallDir "D:\Games\ArmaReforgerDedicated"

param(
  [string]$InstallDir = "D:\SteamLibrary\steamapps\common\Arma Reforger Server",
  [string]$SteamCmdDir = "D:\SteamLibrary\steamcmd"
)

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
$secrets = Join-Path $here "secrets"
New-Item -ItemType Directory -Force -Path $secrets | Out-Null
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
New-Item -ItemType Directory -Force -Path $SteamCmdDir | Out-Null

$steamcmd = Join-Path $SteamCmdDir "steamcmd.exe"
if (-not (Test-Path $steamcmd)) {
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
if (-not (Test-Path $exe)) {
  Write-Host ""
  Write-Host "SteamCMD binary indiremedi (sik gorulen: 'Steam needs to be online')."
  Write-Host "Alternatif: Steam kutuphane > Araclar > Arma Reforger Dedicated Server kur."
  Write-Host "Sonra: Set-Content tools\dedicated\secrets\server-root.txt '<kurulum_yolu>'"
  throw ("Kurulum basarisiz - bulunamadi: {0}" -f $exe)
}

Set-Content -Path (Join-Path $secrets "server-root.txt") -Value $InstallDir -Encoding ascii -NoNewline
Write-Host ""
Write-Host ("OK: {0}" -f $exe)
Write-Host "Yol kaydedildi: secrets\server-root.txt"
Write-Host ""
Write-Host "Sonraki adimlar:"
Write-Host "  - powershell -File tools\dedicated\bagla-addon.ps1"
Write-Host "  - secrets\M360_ApiLabKey.txt  (Vercel key, tek satir)"
Write-Host "  - secrets\server.json sifreleri (veya PASSWORDS.txt)"
Write-Host "  - powershell -File tools\dedicated\start.ps1"
