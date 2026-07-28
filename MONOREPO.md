# M360 Life (GitHub monorepo)

Ayni hizada uc ana bolum:

| Bolum | Yol | Aciklama |
|---|---|---|
| Oyun (Enfusion addon) | `m360-life/` | Scripts, Prefabs, Worlds, UI, Configs, addon.gproj |
| API | `api/` | Next.js game-api → Vercel (Root Directory: `api`) |
| Web | `web/` | Admin panel (sonraki faz) → Vercel |
| DB | `packages/db` | PostgreSQL migration (Neon) |
| Docs | `docs/` | Tasarim + lab notlari |
| Sync | `tools/` | Workbench senkron scriptleri |

**Yerel PC:** Workbench addon koku = `m360-life/` (veya senkron ile duz addon klasoru).  
**Bulut:** Neon (PG) + Vercel (`api`, sonra `web`). Yerel PG/Next zorunlu degil.
