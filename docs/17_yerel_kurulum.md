# Döküman 17 — Kurulum notları (Neon + Vercel)

*Çalışma yolları / junction: [18](./18_calisma_duzeni.md). Güvenlik: [19](./19_guvenlik.md).*

## Güncel hedef

| Bileşen | Nerede |
|---|---|
| Oyun addon | `...\addons\M360-Life` (= junction → `m360-life/`) |
| API | `api/` → **Vercel** (`https://m360-life.vercel.app`) |
| Admin web | `web/` → Vercel (sonra) |
| PostgreSQL | **Neon** — yerel PG yok |
| Migration | Neon SQL Editor + `packages/db/migrations/` |
| Metrik | `/istatistik` · `/api/metrik` |

## Vercel

1. Repo `enesgulcu/M360-Life`, Root Directory: **`api`**
2. Env: `DATABASE_URL` (Neon pooled), isteğe bağlı `M360_SERVER_KEY`, `M360_RATE_LIMIT_DAKIKA`
3. Kontrol: `/api/health`, `/api/jobs`, `/istatistik`

## Oyun ↔ GitHub (kopyasız)

```bat
REM Workbench KAPALI:
powershell -File C:\Users\Enes\Documents\GitHub\M360-Life\tools\bagla-oyun-klasoru.ps1
```

## Yerel API (isteğe bağlı)

```bat
cd C:\Users\Enes\Documents\GitHub\M360-Life\api
npm install
npm run dev
```

`api/.env`: `DATABASE_URL` (git’e girmez).

## Dedicated

Taşınabilir paket: [`tools/dedicated/README.md`](../tools/dedicated/README.md)

```powershell
powershell -File tools\dedicated\install-server.ps1   # bir kez (SteamCMD 1874900)
powershell -File tools\dedicated\bagla-addon.ps1
# secrets\server.json + secrets\M360_ApiLabKey.txt
powershell -File tools\dedicated\start.ps1
```

İstemci: Direct Connect `127.0.0.1:2001`. Senaryo: LabDuzZemin. HTTP → Vercel API (`M360_ApiIstemci`).
