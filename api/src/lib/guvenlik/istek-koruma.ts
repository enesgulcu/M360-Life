import { timingSafeEqual } from "crypto";
import { rateLimitKontrol, rateLimitTemizle } from "./rate-limit";

/** Dakikada IP basina istek (lab). Uretimde env ile artirilir. */
const LIMIT_DAKIKA = Number(process.env.M360_RATE_LIMIT_DAKIKA ?? "60");
const PENCERE_MS = 60_000;

export function istemciIp(req: Request): string {
  const xf = req.headers.get("x-forwarded-for");
  if (xf) return xf.split(",")[0]?.trim() || "bilinmiyor";
  const real = req.headers.get("x-real-ip");
  if (real) return real.trim();
  return "bilinmiyor";
}

function guvenliEsit(a: string, b: string): boolean {
  const ba = Buffer.from(a);
  const bb = Buffer.from(b);
  if (ba.length !== bb.length) return false;
  return timingSafeEqual(ba, bb);
}

/**
 * Sunucu anahtari: M360_SERVER_KEY set ise X-M360-Server-Key zorunlu.
 * Lab'da bos birakilirsa sadece rate limit uygulanir (acik endpoint uyarisi).
 */
export function sunucuAnahtariKontrol(req: Request): { ok: true } | { ok: false; mesaj: string } {
  const beklenen = process.env.M360_SERVER_KEY?.trim();
  if (!beklenen) return { ok: true };
  const gelen = req.headers.get("x-m360-server-key")?.trim() ?? "";
  if (!gelen || !guvenliEsit(gelen, beklenen)) {
    return { ok: false, mesaj: "Gecersiz veya eksik X-M360-Server-Key" };
  }
  return { ok: true };
}

export function istekRateLimit(req: Request, yol: string): {
  ok: true;
  kalan: number;
  sifirlaSn: number;
} | {
  ok: false;
  kalan: number;
  sifirlaSn: number;
  mesaj: string;
} {
  rateLimitTemizle();
  const ip = istemciIp(req);
  const sonuc = rateLimitKontrol(`${ip}:${yol}`, LIMIT_DAKIKA, PENCERE_MS);
  if (!sonuc.izin) {
    return {
      ok: false,
      kalan: sonuc.kalan,
      sifirlaSn: sonuc.sifirlaSn,
      mesaj: "Rate limit — cok fazla istek",
    };
  }
  return { ok: true, kalan: sonuc.kalan, sifirlaSn: sonuc.sifirlaSn };
}

/** Istemci gonderirse yuvarlak seyahat olculur (unix ms). */
export function istemciBaslangicMs(req: Request): number | undefined {
  const ham = req.headers.get("x-m360-istek-baslangic");
  if (!ham) return undefined;
  const n = Number(ham);
  if (!Number.isFinite(n) || n <= 0) return undefined;
  return n;
}
