# packages/db — PostgreSQL sema

**Hedef:** Hosted PostgreSQL ([Neon](https://neon.tech)) — yerel PG kurulmaz.

## Ilk kurulum (Neon)

1. https://console.neon.tech → proje `m360-life`.
2. Connection string — **Pooled connection** (Vercel icin).
3. Neon SQL Editor'de `migrations/001_job_definitions.sql` calistir.
4. Ayni URL:
   - Yerel test: `api/.env` → `DATABASE_URL=...` (git'e girmez)
   - Vercel: Project Settings → Environment Variables → `DATABASE_URL`

## Kontrol

```sql
SELECT is_adi, satis_fiyati, iptal_mesafesi FROM job_definitions;
```

Beklenen: `Pirinc` satiri.
