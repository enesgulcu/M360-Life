import { Pool } from "pg";

declare global {
  // eslint-disable-next-line no-var
  var __m360PgPool: Pool | undefined;
}

/** Vercel serverless: tek Pool ornegi (hot reload / cold start). */
export function getPool(): Pool | null {
  const url = process.env.DATABASE_URL;
  if (!url) return null;

  if (!global.__m360PgPool) {
    global.__m360PgPool = new Pool({
      connectionString: url,
      ssl: url.includes("sslmode=require") || url.includes("neon.tech")
        ? { rejectUnauthorized: false }
        : undefined,
      max: 3,
    });
  }
  return global.__m360PgPool;
}
