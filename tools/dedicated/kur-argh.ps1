# ARGH Vehicle Dealer + DynamicEconomy kaynaklarini yerel test icin kurar.
# Vendor klasoru git'e girmez; her PC'de bu script ayni kurulumu tekrarlar.

$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
$repo = (Resolve-Path (Join-Path $here "..\..")).Path
$vendor = Join-Path $repo "tools\vendor"

New-Item -ItemType Directory -Force -Path $vendor | Out-Null

$projects = @(
  @{ Name = "DynamicLoot"; Url = "https://github.com/wyqydsyq/DynamicLoot.git" },
  @{ Name = "DynamicEconomy"; Url = "https://github.com/wyqydsyq/DynamicEconomy.git" },
  @{ Name = "ARGH-AmbientVehiclePlugin"; Url = "https://github.com/josharghhh/ARGH_VEHICLE_TRADER_DE.git" }
)

foreach ($project in $projects) {
  $target = Join-Path $vendor $project.Name
  if (-not (Test-Path -LiteralPath (Join-Path $target "addon.gproj"))) {
    Write-Host ("Indiriliyor: {0}" -f $project.Name)
    & git clone --depth 1 $project.Url $target
    if ($LASTEXITCODE -ne 0) {
      throw ("Indirme basarisiz: {0}" -f $project.Name)
    }
  }
}

# Dealer icin gerekmeyen 331 MB ARGH-Racers bagimliligini yerel test
# kopyasindan cikar. Resmi ARGH kaynaklari yeniden dagitilmaz.
$arghRoot = Join-Path $vendor "ARGH-AmbientVehiclePlugin"
$gproj = Join-Path $arghRoot "addon.gproj"
$gprojText = Get-Content -LiteralPath $gproj -Raw
$gprojText = $gprojText.Replace(
  '"58D0FB3206B6F859" "66F0560F1BDE732A" "66B2F0B008DC590F" "66A8C33421A72540"',
  '"58D0FB3206B6F859" "66F0560F1BDE732A" "66B2F0B008DC590F"')
Set-Content -LiteralPath $gproj -Value $gprojText -Encoding ascii -NoNewline

$configSrc = Join-Path $here "argh\M360_ARGH_VehicleDealer_Vehicles.conf"
$configDst = Join-Path $arghRoot "Configs\Systems\ARGH_VehicleDealer_Vehicles.conf"
Copy-Item -LiteralPath $configSrc -Destination $configDst -Force

# ARGH GitHub kaynagini Reforger 1.7 NotifyPlayerDataChange imzasina uyarla.
$dealerScripts = @(
  (Join-Path $arghRoot "Scripts\Game\ARGH\Dealer\ARGH_VehicleDealerServiceComponent.c"),
  (Join-Path $arghRoot "Scripts\Game\ARGH\Dealer\ARGH_VehicleDealerComponent.c")
)
foreach ($script in $dealerScripts) {
  $text = Get-Content -LiteralPath $script -Raw
  $text = $text.Replace("NotifyPlayerDataChange(-amount)", "NotifyPlayerDataChange()")
  $text = $text.Replace("NotifyPlayerDataChange(amount)", "NotifyPlayerDataChange()")
  Set-Content -LiteralPath $script -Value $text -Encoding ascii -NoNewline
}

# Orijinal tabela prefabinda iki RplComponent var; script devre disi olani
# buldugu icin katalog/satin alma RPC'leri sunucuya ulasmiyor.
$traderPrefab = Join-Path $arghRoot "Prefabs\trader.et"
$traderText = Get-Content -LiteralPath $traderPrefab -Raw
$traderText = $traderText.Replace(
  "  RplComponent `"{566CB016AAC374C6}`" {`r`n   Enabled 0`r`n  }`r`n  RplComponent `"{75678DE0CC4E4E0C}`" {`r`n  }",
  "  RplComponent `"{566CB016AAC374C6}`" {`r`n   Enabled 1`r`n  }")
$traderText = $traderText.Replace(
  "  RplComponent `"{566CB016AAC374C6}`" {`n   Enabled 0`n  }`n  RplComponent `"{75678DE0CC4E4E0C}`" {`n  }",
  "  RplComponent `"{566CB016AAC374C6}`" {`n   Enabled 1`n  }")
Set-Content -LiteralPath $traderPrefab -Value $traderText -Encoding ascii -NoNewline

# DynamicEconomy, katalogda bulunmayan sivil araclarda null entry okuyordu.
$deSellAction = Join-Path $vendor "DynamicEconomy\scripts\Game\UserActions\DE_TraderSellAction.c"
$deText = Get-Content -LiteralPath $deSellAction -Raw
$oldEntry = "SCR_EntityCatalogEntry entry = lootSystem.vehicleCatalog.GetEntryWithPrefab(prefabName);"
$newEntry = "$oldEntry`r`n`t`tif (!entry)`r`n`t`t`treturn;"
if (-not $deText.Contains("if (!entry)")) {
  $deText = $deText.Replace($oldEntry, $newEntry)
  Set-Content -LiteralPath $deSellAction -Value $deText -Encoding ascii -NoNewline
}

Write-Host "ARGH Vehicle Dealer yerel kaynaklari hazir."
