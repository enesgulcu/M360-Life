# M360 Life — dedicated sunucuyu durdur

$ErrorActionPreference = "Stop"
Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue | ForEach-Object {
  Write-Host ("Durduruluyor PID {0}" -f $_.Id)
  Stop-Process -Id $_.Id -Force
}
if (-not (Get-Process -Name "ArmaReforgerServer" -ErrorAction SilentlyContinue)) {
  Write-Host "ArmaReforgerServer calismiyor (veya durduruldu)."
}
