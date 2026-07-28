import { NextResponse } from "next/server";
import {
  istemciBaslangicMs,
  istemciIp,
  istekRateLimit,
  sunucuAnahtariKontrol,
} from "@/lib/guvenlik/istek-koruma";
import { metrikEkle } from "@/lib/metrik/depo";

type Handler = (req: Request) => Promise<Response> | Response;

/**
 * Tum API route'lari icin ortak koruma + sure olcumu.
 * Rate limit -> (opsiyonel) sunucu anahtari -> handler -> metrik.
 */
export function withApiKoruma(yol: string, handler: Handler, opts?: { anahtarZorunlu?: boolean }) {
  return async (req: Request): Promise<Response> => {
    const t0 = Date.now();
    const istemciT0 = istemciBaslangicMs(req);

    const rl = istekRateLimit(req, yol);
    if (!rl.ok) {
      const res = NextResponse.json(
        { ok: false, hata: "rate-limit", mesaj: rl.mesaj },
        { status: 429 }
      );
      res.headers.set("Retry-After", String(rl.sifirlaSn));
      res.headers.set("X-RateLimit-Remaining", "0");
      kaydet(req, yol, 429, t0, istemciT0);
      return res;
    }

    // Sadece /api/health anahtarsiz (uptime). jobs/metrik/yazma: anahtar zorunlu (set ise).
    const acikYollar = new Set(["/api/health"]);
    const anahtarGerekli =
      opts?.anahtarZorunlu === true || Boolean(process.env.M360_SERVER_KEY?.trim());
    if (anahtarGerekli && !acikYollar.has(yol)) {
      const auth = sunucuAnahtariKontrol(req);
      if (!auth.ok) {
        const res = NextResponse.json(
          { ok: false, hata: "yetkisiz", mesaj: auth.mesaj },
          { status: 401 }
        );
        kaydet(req, yol, 401, t0, istemciT0);
        return res;
      }
    }

    try {
      const ham = await handler(req);
      const sunucuMs = Date.now() - t0;
      const headers = new Headers(ham.headers);
      headers.set("X-M360-Sunucu-Ms", String(sunucuMs));
      headers.set("X-RateLimit-Remaining", String(rl.kalan));
      if (istemciT0) {
        const toplamMs = Date.now() - istemciT0;
        headers.set("X-M360-Toplam-Ms", String(toplamMs));
        headers.set("X-M360-Gelis-Ms", String(Math.max(0, t0 - istemciT0)));
      }
      const res = new Response(ham.body, {
        status: ham.status,
        statusText: ham.statusText,
        headers,
      });
      kaydet(req, yol, ham.status, t0, istemciT0, sunucuMs);
      return res;
    } catch (err) {
      const mesaj = err instanceof Error ? err.message : String(err);
      kaydet(req, yol, 500, t0, istemciT0);
      return NextResponse.json({ ok: false, hata: "sunucu", mesaj }, { status: 500 });
    }
  };
}

function kaydet(
  req: Request,
  yol: string,
  durum: number,
  t0: number,
  istemciT0?: number,
  sunucuMs = Date.now() - t0
) {
  const toplamMs = istemciT0 ? Date.now() - istemciT0 : undefined;
  const gelisMs = istemciT0 ? Math.max(0, t0 - istemciT0) : undefined;
  metrikEkle({
    yol,
    yontem: req.method,
    durum,
    sunucuMs,
    toplamMs,
    gelisMs,
    ip: istemciIp(req),
  });
}
