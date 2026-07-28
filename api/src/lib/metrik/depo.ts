/** Son N API isteginin sure olcumleri (lab telemetri). Vercel instance belleginde. */

export type MetrikKayit = {
  id: string;
  yol: string;
  yontem: string;
  durum: number;
  /** Sunucu is handler suresi (ms) */
  sunucuMs: number;
  /** Istemci baslangic header varsa toplam RTT (ms) */
  toplamMs?: number;
  /** Istemci -> sunucu tahmini (ms) */
  gelisMs?: number;
  ip: string;
  zaman: string;
};

const MAX = 200;
const kayitlar: MetrikKayit[] = [];

export function metrikEkle(k: Omit<MetrikKayit, "id" | "zaman"> & { zaman?: string }): void {
  kayitlar.unshift({
    ...k,
    id: `${Date.now()}-${Math.random().toString(36).slice(2, 8)}`,
    zaman: k.zaman ?? new Date().toISOString(),
  });
  if (kayitlar.length > MAX) kayitlar.length = MAX;
}

export function metrikListe(limit = 50): MetrikKayit[] {
  return kayitlar.slice(0, Math.min(limit, MAX));
}

export function metrikOzet() {
  const liste = kayitlar;
  if (liste.length === 0) {
    return {
      adet: 0,
      ortSunucuMs: 0,
      ortToplamMs: 0,
      p95SunucuMs: 0,
      son5dk: 0,
    };
  }
  const sunucu = [...liste.map((x) => x.sunucuMs)].sort((a, b) => a - b);
  const toplamVar = liste.filter((x) => typeof x.toplamMs === "number") as Array<
    MetrikKayit & { toplamMs: number }
  >;
  const toplam = [...toplamVar.map((x) => x.toplamMs)].sort((a, b) => a - b);
  const p95 = (arr: number[]) => arr[Math.min(arr.length - 1, Math.floor(arr.length * 0.95))] ?? 0;
  const besDkOnce = Date.now() - 5 * 60_000;
  const son5dk = liste.filter((x) => Date.parse(x.zaman) >= besDkOnce).length;
  const ort = (arr: number[]) =>
    arr.length ? Math.round((arr.reduce((a, b) => a + b, 0) / arr.length) * 10) / 10 : 0;

  return {
    adet: liste.length,
    ortSunucuMs: ort(sunucu),
    ortToplamMs: ort(toplam),
    p95SunucuMs: p95(sunucu),
    son5dk,
  };
}
