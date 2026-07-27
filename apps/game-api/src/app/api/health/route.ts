import { NextResponse } from "next/server";

export async function GET() {
  return NextResponse.json({
    ok: true,
    servis: "m360-game-api",
    zaman: new Date().toISOString(),
  });
}
