# packages/db — PostgreSQL sema

## Kurulum (ozet — detay docs/17)

1. PostgreSQL native kur (Docker yok).
2. Kullanici/DB olustur (ornek):

```sql
CREATE USER m360 WITH PASSWORD 'm360_lab';
CREATE DATABASE m360_life OWNER m360;
```

3. Migration:

```bat
psql -U m360 -d m360_life -f migrations/001_job_definitions.sql
```

4. `apps/game-api/.env` icine:

```
DATABASE_URL=postgresql://m360:m360_lab@localhost:5432/m360_life
```
