import { NextResponse } from "next/server";
import { getPool } from "@/lib/db";
import { withApiKoruma } from "@/lib/guvenlik/with-api-koruma";

export const runtime = "nodejs";

async function jobsHandler() {
  const pool = getPool();
  if (!pool) {
    return NextResponse.json(
      {
        ok: false,
        kaynak: "env-yok",
        mesaj: "DATABASE_URL yok — Vercel env veya .env",
        isler: [],
      },
      { status: 200 }
    );
  }

  try {
    const sonuc = await pool.query(
      `SELECT id, is_adi, yasal_mi, adim_suresi, adim_verim, parti_boyutu,
              isleme_suresi, donusum_orani, satis_fiyati, max_tasima, iptal_mesafesi
       FROM job_definitions
       ORDER BY is_adi`
    );
    return NextResponse.json({ ok: true, kaynak: "postgresql", isler: sonuc.rows });
  } catch (err) {
    const mesaj = err instanceof Error ? err.message : String(err);
    return NextResponse.json(
      {
        ok: false,
        kaynak: "postgresql-hata",
        mesaj,
        isler: [],
        not: "Migration calistirildi mi? packages/db/migrations/001_job_definitions.sql",
      },
      { status: 200 }
    );
  }
}

export const GET = withApiKoruma("/api/jobs", jobsHandler);
