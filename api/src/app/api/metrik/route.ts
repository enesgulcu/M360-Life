import { NextResponse } from "next/server";
import { withApiKoruma } from "@/lib/guvenlik/with-api-koruma";
import { metrikListe, metrikOzet } from "@/lib/metrik/depo";

export const runtime = "nodejs";

async function metrikHandler(req: Request) {
  const url = new URL(req.url);
  const limit = Math.min(200, Math.max(1, Number(url.searchParams.get("limit") ?? "40") || 40));
  return NextResponse.json({
    ok: true,
    ozet: metrikOzet(),
    kayitlar: metrikListe(limit),
    not: "Metrikler bu Vercel instance belleginde; cold start sonrasi sifirlanabilir.",
  });
}

export const GET = withApiKoruma("/api/metrik", metrikHandler);
