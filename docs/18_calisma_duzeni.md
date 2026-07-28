# M360 Life — Çalışma düzeni (iki yol, tek fiziksel oyun)

## Klasörler

| Yol | Rol |
|---|---|
| `C:\Users\Enes\Documents\GitHub\M360-Life` | Monorepo — api, web, docs, packages, **git** |
| `...\GitHub\M360-Life\m360-life` | Oyun kaynakları (git’te) |
| `...\My Games\ArmaReforgerWorkbench\addons\M360-Life` | Workbench’in açtığı yol |

## Senkron = junction (önerilen)

Kopyala-yapıştır yok. Workbench yolu **junction** ile GitHub `m360-life`’a bağlanır; birinde değişen dosya diğerinde de aynıdır.

```bat
REM Workbench KAPALI iken:
powershell -File C:\Users\Enes\Documents\GitHub\M360-Life\tools\bagla-oyun-klasoru.ps1
```

- Geri al: `...\bagla-oyun-klasoru.ps1 -Coz`
- Junction yoksa geçici kopya: `tools\sync-game-to-github.ps1`

`resourceDatabase.rdb` gitignore’da (yerel cache).

## Monorepo

```
M360-Life/
├── m360-life/     ← oyun (junction hedefi)
├── api/           ← Vercel Root Directory: api
├── web/           ← admin (sonra)
├── packages/db/   ← Neon SQL
├── docs/
└── tools/
```

## Bulut

| Bileşen | Nerede |
|---|---|
| PostgreSQL | Neon (yerel PG yok) |
| API | Vercel → `https://m360-life.vercel.app` |
| Metrik paneli | `https://m360-life.vercel.app/istatistik` |
| Güvenlik | [docs/19](./19_guvenlik.md) |

## Günlük akış

1. Workbench → `addons\M360-Life` (junction ise = git klasörü)
2. Cursor → `Documents\GitHub\M360-Life` (`api/`, `docs/`)
3. Oyun değişince doğrudan `git add m360-life` (junction ile)
4. API değişince push → Vercel otomatik deploy
