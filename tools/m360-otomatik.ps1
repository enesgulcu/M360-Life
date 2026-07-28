# M360 Life - TEK komut (PC degisimi / gunluk baslangic)
#
# Cift tik: M360.bat
# Oyna:     M360.bat oyna   veya  M360-Oyna.bat
#
# Yapar: git pull + junction/secrets/binary kontrol + (istege) sunucu+oyun
# ASCII only (Windows PowerShell encoding).

param(
  [switch]$Oyna,
  [switch]$SkipPull
)

$ErrorActionPreference = "Stop"
$tools = $PSScriptRoot
$repo = (Resolve-Path (Join-Path $tools "..")).Path
Set-Location -LiteralPath $repo

function Write-Adim([string]$Msg) {
  Write-Host ""
  Write-Host ("======== {0} ========" -f $Msg)
}

Write-Host "M360 otomatik - PC adapte"
Write-Host ("Kullanici : {0}" -f $env:USERNAME)
Write-Host ("Repo      : {0}" -f $repo)

# --- 1) git pull (YEREL IS KORUNUR) ---
Write-Adim "1/3 Git guncelle"
if ($SkipPull) {
  Write-Host "Atlandi (-SkipPull)."
} else {
  $git = Get-Command git -ErrorAction SilentlyContinue
  if (-not $git) {
    Write-Host "UYARI: git yok - pull atlandi. GitHub Desktop / git kur."
  } else {
    try {
      git -C $repo rev-parse --is-inside-work-tree 2>$null | Out-Null

      # Kaydedilmemis / commit edilmemis degisiklik varsa pull YAPMA
      $porcelain = @(git -C $repo status --porcelain 2>$null)
      if ($porcelain.Count -gt 0) {
        Write-Host "Yerel degisiklik var - git pull ATLANDI (islerin guvende)."
        Write-Host ("  {0} dosya/degisiklik bekliyor (commit+push sonrasi diger PC alir)." -f $porcelain.Count)
        Write-Host "  pull sadece temizken veya sen 'commit ve push' dedikten sonra."
      } else {
        Write-Host "git pull --ff-only ..."
        git -C $repo pull --ff-only
        if ($LASTEXITCODE -ne 0) {
          Write-Host "UYARI: git pull basarisiz (uzak ile yerel commit ayrildi?). Ilerliyorum."
          Write-Host "  Elle: git status  |  Is silinmez; --ff-only sifirlama yapmaz."
        } else {
          Write-Host "Git OK."
        }
      }
    } catch {
      Write-Host ("UYARI: git pull hata: {0}" -f $_.Exception.Message)
    }
  }
}

# --- 2) pc-hazirla ---
Write-Adim "2/3 Bu PC hazirlik (junction + secrets + server)"
& (Join-Path $tools "pc-hazirla.ps1")

$envPath = Join-Path $repo "api\.env"
if (-not (Test-Path -LiteralPath $envPath)) {
  Write-Host ""
  Write-Host "EKSIK (bir kez): api\.env  (DATABASE_URL + M360_SERVER_KEY)"
  Write-Host "  Diger PC'den kopyala veya Vercel/Neon'dan yaz. Git'e GIRMEZ."
}

# --- 3) Oyna (sadece -Oyna ile; soru yok) ---
if ($Oyna) {
  Write-Adim "3/3 Oyun"
  Write-Host "Oyna basliyor (sunucu yenile + Steam)..."
  & (Join-Path $tools "dedicated\oyna.ps1")
} else {
  Write-Adim "3/3 Hazir"
  Write-Host "Bu PC adapte. Cursor ile calisabilirsin (kural otomatik)."
  Write-Host "Oynamak: M360-Oyna.bat"
}

Write-Host ""
Write-Host "Diger PC oncesi: Cursor'a 'commit ve push' demen yeter."
