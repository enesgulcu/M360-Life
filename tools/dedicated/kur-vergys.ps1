# Vergys Custom Clothing Workshop paketini bu PC'ye indirir.
# NOT: yalniz addon.gproj varligi YETERSIZ — data.pak ~460MB olmali.

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
. (Join-Path $here "..\Resolve-M360Paths.ps1")

$repo = Get-M360RepoRoot
$downloadRoot = Join-Path $repo "tools\vendor-workshop"
$addonName = "VergysCustomClothing_59B70A5A19E9B51E"
$addonRoot = Join-Path $downloadRoot "addons\$addonName"
$gproj = Join-Path $addonRoot "addon.gproj"
$dataPak = Join-Path $addonRoot "data.pak"
# header FORM size + 8 => tam paket ~483073873; 409MB yarim indirme motor tarafindan "not valid pak" olur
$minPakBytes = 483000000

function Test-VergysPaketGecerli {
	if (-not (Test-Path -LiteralPath $gproj)) {
		return $false
	}
	if (-not (Test-Path -LiteralPath $dataPak)) {
		return $false
	}
	$len = (Get-Item -LiteralPath $dataPak).Length
	if ($len -lt $minPakBytes) {
		return $false
	}
	# IFF FORM boyut alani (BE) + 8 == dosya boyutu olmali; aksi yarim/bozuk
	try {
		$fs = [IO.File]::OpenRead($dataPak)
		$b = New-Object byte[] 8
		[void]$fs.Read($b, 0, 8)
		$fs.Close()
		if ($b[0] -ne 0x46 -or $b[1] -ne 0x4F -or $b[2] -ne 0x52 -or $b[3] -ne 0x4D) {
			return $false
		}
		$formSize = [int64]$b[4] * 16777216 + [int64]$b[5] * 65536 + [int64]$b[6] * 256 + [int64]$b[7]
		return ($len -eq ($formSize + 8))
	} catch {
		return $false
	}
}

if (Test-VergysPaketGecerli) {
	$lenMb = [math]::Round((Get-Item -LiteralPath $dataPak).Length / 1MB, 1)
	Write-Host ("Vergys Custom Clothing hazir ({0} MB)." -f $lenMb)
	return
}

if (Test-Path -LiteralPath $dataPak) {
	$badLen = (Get-Item -LiteralPath $dataPak).Length
	Write-Host ("UYARI: data.pak bozuk/eksik ({0} byte). Yeniden indirilecek." -f $badLen)
}

$runningServer = Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue
if ($runningServer) {
	throw "Vergys indirme icin dedicated'i kapat. Sonra: M360.bat veya tools\dedicated\kur-vergys.ps1"
}

$serverRoot = Find-M360ReforgerServerRoot
if (-not $serverRoot) {
	throw "Vergys indirmek icin Arma Reforger Server kurulu olmali."
}

# Bozuk iskeleti temizle (yalniz bos/eksik pakette) — yeniden indirme icin.
if ((Test-Path -LiteralPath $addonRoot) -and -not (Test-VergysPaketGecerli)) {
	Write-Host "Bozuk Vergys klasoru kaldiriliyor..."
	Remove-Item -LiteralPath $addonRoot -Recurse -Force -ErrorAction SilentlyContinue
}

New-Item -ItemType Directory -Force -Path $downloadRoot | Out-Null
$exe = Join-Path $serverRoot "ArmaReforgerServer.exe"
$config = Join-Path $here "vergys-download.json"
$args = @(
	"-config", $config,
	"-profile", "M360VergysDownload",
	"-addonDownloadDir", $downloadRoot
)

Write-Host "Vergys Custom Clothing indiriliyor (~460 MB, birkac dakika)..."
$proc = Start-Process -FilePath $exe -ArgumentList $args -WorkingDirectory $serverRoot -PassThru
try {
	$deadline = (Get-Date).AddMinutes(15)
	while ((Get-Date) -lt $deadline) {
		if (Test-VergysPaketGecerli) {
			$lenMb = [math]::Round((Get-Item -LiteralPath $dataPak).Length / 1MB, 1)
			Write-Host ("Vergys Custom Clothing indirildi ({0} MB)." -f $lenMb)
			return
		}
		if ($proc.HasExited) {
			$exitHint = "paket hâlâ eksik"
			if (Test-Path -LiteralPath $dataPak) {
				$exitHint = ("data.pak={0} byte" -f (Get-Item -LiteralPath $dataPak).Length)
			}
			throw ("Vergys indirme sunucusu erken kapandi (kod: {0}, {1})." -f $proc.ExitCode, $exitHint)
		}
		Start-Sleep -Seconds 3
	}
	throw "Vergys indirme zaman asimina ugradi (15 dk)."
}
finally {
	if (-not $proc.HasExited) {
		Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
	}
}
