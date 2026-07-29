# M360 Life — ortak yollar (iki PC / farkli kullanici adi)
# Dot-source: . "$PSScriptRoot\Resolve-M360Paths.ps1"
# NOT: fonksiyon icinde $PSScriptRoot guvenilmez — yukleme aninda sabitle.

$script:M360ToolsDir = $PSScriptRoot

function Get-M360RepoRoot {
  # tools\ -> monorepo kok
  return (Resolve-Path (Join-Path $script:M360ToolsDir "..")).Path
}

function Get-M360OyunKaynak {
  return (Join-Path (Get-M360RepoRoot) "m360-life")
}

function Get-M360WorkbenchAddon {
  return (Join-Path $env:USERPROFILE "Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life")
}

function Get-M360WorkbenchAddonBak {
  return (Join-Path $env:USERPROFILE "Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life.bak")
}

function Test-M360Junction([string]$Path) {
  if (-not (Test-Path -LiteralPath $Path)) { return $false }
  $item = Get-Item -LiteralPath $Path -Force
  return [bool]($item.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

function Get-M360SteamLibraries {
  $libs = New-Object System.Collections.Generic.List[string]
  $steamPath = $null
  try {
    $steamPath = (Get-ItemProperty -Path "HKCU:\Software\Valve\Steam" -ErrorAction Stop).SteamPath
  } catch {}
  if (-not $steamPath) {
    $steamPath = "C:\Program Files (x86)\Steam"
  }
  $steamPath = $steamPath -replace "/", "\"
  if (Test-Path -LiteralPath $steamPath) {
    $libs.Add($steamPath) | Out-Null
  }
  $vdf = Join-Path $steamPath "steamapps\libraryfolders.vdf"
  if (Test-Path -LiteralPath $vdf) {
    Get-Content -LiteralPath $vdf -ErrorAction SilentlyContinue | ForEach-Object {
      if ($_ -match '"path"\s+"([^"]+)"') {
        $p = $Matches[1] -replace "\\\\", "\"
        if ($p -and (Test-Path -LiteralPath $p) -and -not $libs.Contains($p)) {
          $libs.Add($p) | Out-Null
        }
      }
    }
  }
  foreach ($extra in @(
      "D:\SteamLibrary",
      "E:\SteamLibrary",
      "C:\SteamLibrary"
    )) {
    if ((Test-Path -LiteralPath $extra) -and -not $libs.Contains($extra)) {
      $libs.Add($extra) | Out-Null
    }
  }
  return $libs
}

function Find-M360SteamAppDir([string]$InstallDirName) {
  foreach ($lib in (Get-M360SteamLibraries)) {
    $dir = Join-Path $lib "steamapps\common\$InstallDirName"
    if (Test-Path -LiteralPath $dir) { return $dir }
  }
  return $null
}

function Find-M360ReforgerServerRoot {
  $secretsRoot = Join-Path (Get-M360RepoRoot) "tools\dedicated\secrets\server-root.txt"
  if (Test-Path -LiteralPath $secretsRoot) {
    $saved = (Get-Content -LiteralPath $secretsRoot -Raw).Trim()
    if ($saved -and (Test-Path -LiteralPath (Join-Path $saved "ArmaReforgerServer.exe"))) {
      return $saved
    }
  }
  foreach ($name in @("Arma Reforger Server", "Arma Reforger Dedicated Server")) {
    $found = Find-M360SteamAppDir $name
    if ($found -and (Test-Path -LiteralPath (Join-Path $found "ArmaReforgerServer.exe"))) {
      return $found
    }
  }
  foreach ($candidate in @(
      "C:\Program Files (x86)\Steam\steamapps\common\Arma Reforger Server",
      "D:\SteamLibrary\steamapps\common\Arma Reforger Server",
      "E:\SteamLibrary\steamapps\common\Arma Reforger Server"
    )) {
    if (Test-Path -LiteralPath (Join-Path $candidate "ArmaReforgerServer.exe")) {
      return $candidate
    }
  }
  return $null
}

function Find-M360SteamExe {
  $fromReg = $null
  try {
    $sp = (Get-ItemProperty -Path "HKCU:\Software\Valve\Steam" -ErrorAction Stop).SteamPath
    $fromReg = Join-Path ($sp -replace "/", "\") "steam.exe"
  } catch {}
  foreach ($c in @(
      $fromReg,
      "C:\Program Files (x86)\Steam\steam.exe",
      "C:\Program Files\Steam\steam.exe"
    )) {
    if ($c -and (Test-Path -LiteralPath $c)) { return $c }
  }
  return $null
}

# Arma Reforger istemci exe (Steam LaunchOptions birlestirmesini atlamak icin).
function Find-M360ReforgerClientExe {
  $root = Find-M360SteamAppDir "Arma Reforger"
  if (-not $root) { return $null }
  foreach ($name in @("ArmaReforgerSteam.exe", "ArmaReforger.exe")) {
    $p = Join-Path $root $name
    if (Test-Path -LiteralPath $p) { return $p }
  }
  return $null
}

# Istemciyi yalniz M360 ile ac (Steam kalici LaunchOptions Shop/Bacon eklemesin).
function Start-M360Istemci([string]$ServerHost = "127.0.0.1", [string]$AddonsDir = "", [string]$ModGuids = "69F4E91377BCC9A5") {
  if (-not $AddonsDir) {
    $AddonsDir = Join-Path (Get-M360RepoRoot) "tools\dedicated\addons"
  }
  [void](Set-M360SteamLaunchOptions)

  $exe = Find-M360ReforgerClientExe
  if ($exe) {
    Write-Host ("Istemci (dogrudan exe, Steam LaunchOptions atlandi): {0}" -f $exe)
    Write-Host ("  -client {0} -addons {1}" -f $ServerHost, $ModGuids)
    Start-Process -FilePath $exe -ArgumentList @(
      "-client", $ServerHost,
      "-addonsDir", $AddonsDir,
      "-addons", $ModGuids
    ) -WorkingDirectory (Split-Path $exe -Parent)
    return $true
  }

  $steam = Find-M360SteamExe
  if (-not $steam) { throw "steam.exe / ArmaReforger istemci yok" }
  Write-Host "UYARI: istemci exe bulunamadi; steam -applaunch kullaniliyor (LaunchOptions karisabilir)"
  Start-Process -FilePath $steam -ArgumentList @(
    "-applaunch", "1874880",
    "-client", $ServerHost,
    "-addonsDir", $AddonsDir,
    "-addons", $ModGuids
  )
  return $true
}

function Get-M360PreferredServerInstallDir {
  # Oyunun oldugu Steam kutuphanesine kur (aynı disk)
  $game = Find-M360SteamAppDir "Arma Reforger"
  if ($game) {
    $common = Split-Path $game -Parent
    return (Join-Path $common "Arma Reforger Server")
  }
  $libs = @(Get-M360SteamLibraries)
  if ($libs.Count -gt 0) {
    return (Join-Path $libs[0] "steamapps\common\Arma Reforger Server")
  }
  return "C:\Program Files (x86)\Steam\steamapps\common\Arma Reforger Server"
}

function Get-M360ApiEnvPath {
  return (Join-Path (Get-M360RepoRoot) "api\.env")
}

function Read-M360EnvValue([string]$EnvPath, [string]$KeyName) {
  if (-not (Test-Path -LiteralPath $EnvPath)) { return $null }
  foreach ($line in (Get-Content -LiteralPath $EnvPath -ErrorAction SilentlyContinue)) {
    if ($line -match '^\s*#' -or $line -match '^\s*$') { continue }
    if ($line -match ("^\s*{0}\s*=\s*(.*)$" -f [regex]::Escape($KeyName))) {
      $v = $Matches[1].Trim().Trim('"').Trim("'")
      if ($v.Length -gt 0) { return $v }
    }
  }
  return $null
}

# api/.env (DATABASE_URL + M360_SERVER_KEY) = tek kaynak.
# Oyuna: secrets\M360_ApiLabKey.txt (+ start.ps1 profile'a kopyalar)
function Sync-M360ApiKeyFromEnv {
  $envPath = Get-M360ApiEnvPath
  $secretsDir = Join-Path (Get-M360RepoRoot) "tools\dedicated\secrets"
  $keyOut = Join-Path $secretsDir "M360_ApiLabKey.txt"
  New-Item -ItemType Directory -Force -Path $secretsDir | Out-Null

  $key = Read-M360EnvValue $envPath "M360_SERVER_KEY"
  if (-not $key) {
    if (Test-Path -LiteralPath $keyOut) { return $true }
    return $false
  }
  Set-Content -LiteralPath $keyOut -Value $key -Encoding ascii -NoNewline
  return $true
}

function Test-M360DatabaseUrlInEnv {
  $v = Read-M360EnvValue (Get-M360ApiEnvPath) "DATABASE_URL"
  return [bool]$v
}

# Steam kalici LaunchOptions: yalniz M360 GUID (Shop/Bacon/ -client YASAK).
function Set-M360SteamLaunchOptions {
  $modGuid = "69F4E91377BCC9A5"
  $addonsDir = Join-Path (Get-M360RepoRoot) "tools\dedicated\addons"
  $wanted = ('-addonsDir "{0}" -addons {1}' -f $addonsDir, $modGuid)
  $escapedWanted = $wanted.Replace('\', '\\').Replace('"', '\"')

  $steamPath = $null
  try {
    $steamPath = ((Get-ItemProperty -Path "HKCU:\Software\Valve\Steam" -ErrorAction Stop).SteamPath) -replace "/", "\"
  } catch {}
  if (-not $steamPath) { return $false }

  $userdata = Join-Path $steamPath "userdata"
  $fixed = $false
  Get-ChildItem -LiteralPath $userdata -Directory -ErrorAction SilentlyContinue | ForEach-Object {
    $cfg = Join-Path $_.FullName "config\localconfig.vdf"
    if (-not (Test-Path -LiteralPath $cfg)) { return }
    $text = [System.IO.File]::ReadAllText($cfg)
    if ($text -notmatch '69F4E91377BCC9A5|5D2D1436D1FA5A13|606B100247F5C709') { return }

    $newText = [regex]::Replace(
      $text,
      '"LaunchOptions"\s+"([^"]*)"',
      {
        param($m)
        $val = $m.Groups[1].Value
        if ($val -match '69F4E91377BCC9A5|5D2D1436D1FA5A13|606B100247F5C709|M360-Life\\tools\\dedicated\\addons') {
          return ('"LaunchOptions"' + "`t`t" + '"' + $escapedWanted + '"')
        }
        return $m.Value
      }
    )

    if ($newText -eq $text) {
      if ($text -match '69F4E91377BCC9A5' -and $text -notmatch '5D2D1436D1FA5A13|606B100247F5C709|-client\s') {
        Write-Host "Steam LaunchOptions zaten temiz (yalniz M360)"
        $fixed = $true
      }
      return
    }

    $bak = "$cfg.bak-m360-launch"
    if (-not (Test-Path -LiteralPath $bak)) {
      Copy-Item -LiteralPath $cfg -Destination $bak -Force
    }
    [System.IO.File]::WriteAllText($cfg, $newText)
    $fixed = $true
    Write-Host ("Steam LaunchOptions temizlendi (yalniz M360): {0}" -f $wanted)
  }
  return $fixed
}
