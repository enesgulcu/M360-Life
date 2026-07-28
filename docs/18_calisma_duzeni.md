# M360 Life — Çalışma düzeni (iki PC, tek repo)

## İki PC — somut yollar

| Ne | PC A (sabah) — kullanıcı `Enes` | PC B (akşam) — kullanıcı `enesg` |
|---|---|---|
| Git repo | `C:\Users\Enes\Documents\GitHub\M360-Life` | `C:\Users\enesg\Documents\GitHub\M360-Life` |
| Oyun (git) | `...\m360-life` | `...\m360-life` |
| Workbench | `...\My Games\ArmaReforgerWorkbench\addons\M360-Life` → junction → `m360-life` | aynı mantık |
| Dedicated addons | `...\tools\dedicated\addons` | aynı göreli |
| Sırlar | `api\.env` (git yok) | her PC’de bir kez kopya |
| Steam kalıcı launch | `-addonsDir "C:\Users\Enes\Documents\GitHub\M360-Life\tools\dedicated\addons" -addons 69F4E91377BCC9A5` | `-addonsDir "C:\Users\enesg\Documents\GitHub\M360-Life\tools\dedicated\addons" -addons 69F4E91377BCC9A5` |

Scriptler sabit kullanıcı adı yazmaz: `tools\Resolve-M360Paths.ps1` + `$env:USERPROFILE`.

**Cursor:** sohbet PC’ler arası senkron değil. Ortak bellek = [docs/15 §C](./15_gelistirme_notlari.md). Yeni sohbet: “docs/15 §C oku”.

## Klasörler (mantık)

| Yol | Rol |
|---|---|
| `...\Documents\GitHub\M360-Life` | Monorepo — git |
| `...\m360-life` | Oyun kaynakları (git) — **tek gerçek kopya** |
| `...\addons\M360-Life` | Workbench = **junction** |
| `tools\dedicated\addons\M360-Life` | Dedicated = **junction** |
| `tools\dedicated\secrets\` | Yerel (git’e girmez) |
| `api\.env` | `DATABASE_URL` + `M360_SERVER_KEY` (git’e girmez) |

## İki PC günlük akış

**Kullanıcı:** sadece `M360.bat` (veya oynamak için `M360-Oyna.bat`). Bkz. [TEK_ADIM.txt](../TEK_ADIM.txt).

```text
M360.bat → git pull + pc-hazirla
M360-Oyna.bat → ayni + sunucu yenile + baglan
```

## Senkron = junction (önerilen)

Kopyala-yapıştır yok. Workbench ve dedicated aynı `m360-life` dosyalarına bakar.

```powershell
# Workbench KAPALI:
powershell -File tools\pc-hazirla.ps1
```

- Sadece Workbench: `tools\bagla-oyun-klasoru.ps1`
- Geri al: `...\bagla-oyun-klasoru.ps1 -Coz`
- Junction yoksa geçici kopya: `tools\sync-game-to-github.ps1`
- Launcher’da proje yoksa: **+ Add Project** → `addons\M360-Life\addon.gproj`

`resourceDatabase.rdb` gitignore’da (yerel cache; silme).

## Monorepo

```
M360-Life/
├── m360-life/     ← oyun (junction hedefi)
├── api/           ← Vercel
├── web/
├── packages/db/
├── docs/
└── tools/
    ├── pc-hazirla.ps1          ← PC değişince TEK komut
    ├── Resolve-M360Paths.ps1
    ├── bagla-oyun-klasoru.ps1
    └── dedicated/
```

## Sistem nasıl konuşur (özet)

1. **Geliştirme:** Cursor monorepo; Workbench junction’daki oyun.
2. **Kalıcılık:** Neon + Vercel API (`DATABASE_URL`).
3. **Oyun → API:** dedicated / lab → `https://m360-life.vercel.app` + `X-M360-Server-Key`.
4. **Güvenlik:** [19](./19_guvenlik.md).

## Geliştirme sırası (kesin — 2026-07-28)

1. **LabDuzZemin** — sistem/prefab burada  
2. **Dedicated** — `tools/dedicated/` taşınabilir; MP burada  
3. **Harita / Everon** — lab + dedicated sonrası

## Bulut

| Bileşen | Nerede |
|---|---|
| PostgreSQL | Neon |
| API | Vercel → `https://m360-life.vercel.app` |
| Lab UI | `/` |
| Metrik | `/istatistik` |

## Yeni PC / ilk kurulum checklist

1. Steam: Reforger + Reforger Tools + **Arma Reforger Server**
2. Repo: `git clone` / pull → `Documents\GitHub\M360-Life`
3. Workbench **kapalı** → `powershell -File tools\pc-hazirla.ps1`
4. Secrets: diğer PC’den `M360_ApiLabKey.txt` (bir kez)
5. Dedicated: `tools\dedicated\start.ps1` → `baglan-istemci.ps1`
