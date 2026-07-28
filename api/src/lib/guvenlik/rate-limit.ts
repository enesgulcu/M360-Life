/** In-memory rate limit (tek Vercel instance / yerel). Redis sonrasi merkezi olur. */

type Kova = { sayac: number; sifirlaAt: number };

const kovalar = new Map<string, Kova>();

export type RateLimitSonuc = {
  izin: boolean;
  kalan: number;
  sifirlaSn: number;
};

export function rateLimitKontrol(
  anahtar: string,
  limit: number,
  pencereMs: number
): RateLimitSonuc {
  const simdi = Date.now();
  let kova = kovalar.get(anahtar);
  if (!kova || simdi >= kova.sifirlaAt) {
    kova = { sayac: 0, sifirlaAt: simdi + pencereMs };
    kovalar.set(anahtar, kova);
  }
  kova.sayac += 1;
  const kalan = Math.max(0, limit - kova.sayac);
  const sifirlaSn = Math.max(0, Math.ceil((kova.sifirlaAt - simdi) / 1000));
  return { izin: kova.sayac <= limit, kalan, sifirlaSn };
}

/** Periyodik temizlik — bellek sizintisini sinirla */
export function rateLimitTemizle(): void {
  const simdi = Date.now();
  for (const [k, v] of kovalar) {
    if (simdi >= v.sifirlaAt) kovalar.delete(k);
  }
}
