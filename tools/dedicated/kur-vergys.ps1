# Vergys Custom Clothing Workshop paketini bu PC'ye indirir.

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$repo = Get-M360RepoRoot
$downloadRoot = Join-Path $repo "tools\vendor-workshop"
$addonName = "VergysCustomClothing_59B70A5A19E9B51E"
$addonRoot = Join-Path $downloadRoot "addons\$addonName"
$gproj = Join-Path $addonRoot "addon.gproj"

if (Test-Path -LiteralPath $gproj) {
  Write-Host "Vergys Custom Clothing hazir."
  return
}

$runningServer = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
if ($runningServer) {
  throw "Vergys paketi eksik ve dedicated acik. Oyunu kapatip M360.bat calistir."
}

$serverRoot = Find-M360ReforgerServerRoot
if (-not $serverRoot) {
  throw "Vergys indirmek icin Arma Reforger Server kurulu olmali."
}

New-Item -ItemType Directory -Force -Path $downloadRoot | Out-Null
$exe = Join-Path $serverRoot "ArmaReforgerServer.exe"
$config = Join-Path $here "vergys-download.json"
$args = @(
  "-config", $config,
  "-profile", "M360VergysDownload",
  "-addonDownloadDir", $downloadRoot
)

Write-Host "Vergys Custom Clothing indiriliyor (~460 MB)..."
$proc = Start-Process -FilePath $exe -ArgumentList $args -WorkingDirectory $serverRoot -PassThru
try {
  $deadline = (Get-Date).AddMinutes(4)
  while ((Get-Date) -lt $deadline) {
    if (Test-Path -LiteralPath $gproj) {
      Write-Host "Vergys Custom Clothing indirildi."
      return
    }
    if ($proc.HasExited) {
      throw "Vergys indirme sunucusu erken kapandi (kod: $($proc.ExitCode))."
    }
    Start-Sleep -Seconds 2
  }
  throw "Vergys indirme zaman asimina ugradi."
}
finally {
  if (-not $proc.HasExited) {
    Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
  }
}
