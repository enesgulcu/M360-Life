# M360 Life (GitHub monorepo)

## Iki yol

| Yol | Rol |
|---|---|
| `Documents\GitHub\M360-Life` | Repo — api / web / docs / git / Vercel |
| `...\addons\M360-Life` | Workbench (junction → `m360-life/`) |

Detay: **[docs/18](docs/18_calisma_duzeni.md)** · Guvenlik: **[docs/19](docs/19_guvenlik.md)**

## Gelistirme sirasi

1. LabDuzZemin (tum sistem burada)
2. Dedicated (yerel + tasinabilir paket)
3. Everon / harita yerlestirme (en sonda)

## Monorepo

| Bolum | Yol | Aciklama |
|---|---|---|
| Oyun | `m360-life/` | Junction hedefi |
| API | `api/` | Vercel Root: `api` |
| Web | `web/` | Admin (sonra) |
| DB | `packages/db` | Neon migration |
| Sync | `tools/bagla-oyun-klasoru.ps1` | Tek fiziksel oyun klasoru |

**Bulut:** Neon + Vercel. Yerel PG yok.
