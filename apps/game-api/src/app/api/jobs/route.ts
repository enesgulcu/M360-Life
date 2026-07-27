import { NextResponse } from "next/server";
import { Pool } from "pg";

function poolOlustur() {
  const url = process.env.DATABASE_URL;
  if (!url) return null;
  return new Pool({ connectionString: url });
}

export async function GET() {
  const pool = poolOlustur();
  if (!pool) {
    return NextResponse.json(
      {
        ok: false,
        kaynak: "env-yok",
        mesaj: "DATABASE_URL yok — packages/db/.env.example bak",
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
    await pool.end();
    return NextResponse.json({ ok: true, kaynak: "postgresql", isler: sonuc.rows });
  } catch (err) {
    await pool.end().catch(() => undefined);
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
