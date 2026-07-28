import { NextResponse } from "next/server";
import { withApiKoruma } from "@/lib/guvenlik/with-api-koruma";
import { metrikListe, metrikOzet } from "@/lib/metrik/depo";
import { zamanTrMetin } from "@/lib/zaman";

export const runtime = "nodejs";

async function metrikHandler(req: Request) {
  const url = new URL(req.url);
  const limit = Math.min(200, Math.max(1, Number(url.searchParams.get("limit") ?? "40") || 40));
  const kayitlar = metrikListe(limit).map((k) => ({
    ...k,
    zamanTr: zamanTrMetin(k.zaman),
  }));
  return NextResponse.json({
    ok: true,
    ozet: metrikOzet(),
    kayitlar,
    saatDilimi: "Europe/Istanbul",
    not: "Metrikler bu Vercel instance belleginde; cold start sonrasi sifirlanabilir. zaman=UTC, zamanTr=Turkiye.",
  });
}

export const GET = withApiKoruma("/api/metrik", metrikHandler);
