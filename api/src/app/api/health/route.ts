import { NextResponse } from "next/server";
import { getPool } from "@/lib/db";

export const runtime = "nodejs";

export async function GET() {
  const pool = getPool();
  let db: "bagli" | "env-yok" | "hata" = "env-yok";
  let dbMesaj: string | undefined;

  if (pool) {
    try {
      await pool.query("SELECT 1");
      db = "bagli";
    } catch (err) {
      db = "hata";
      dbMesaj = err instanceof Error ? err.message : String(err);
    }
  }

  return NextResponse.json({
    ok: db === "bagli" || db === "env-yok",
    servis: "m360-game-api",
    zaman: new Date().toISOString(),
    db,
    ...(dbMesaj ? { dbMesaj } : {}),
  });
}
