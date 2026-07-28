/** Lab endpoint kayit defteri — yeni API eklerken buraya satir ekle. */

export type LabKategori = "durum" | "veri" | "telemetri";

export type LabGorunum = "health" | "jobs" | "metrik" | "json";

export type LabEndpoint = {
  id: string;
  etiket: string;
  yol: string;
  kategori: LabKategori;
  aciklama: string;
  anahtarGerekli: boolean;
  gorunum: LabGorunum;
};

export const LAB_KATEGORI_ETIKET: Record<LabKategori, string> = {
  durum: "Durum",
  veri: "Veri",
  telemetri: "Telemetri",
};

export const LAB_ENDPOINTS: LabEndpoint[] = [
  {
    id: "health",
    etiket: "Health",
    yol: "/api/health",
    kategori: "durum",
    aciklama: "Servis ayakta mi, Neon baglisi mi",
    anahtarGerekli: false,
    gorunum: "health",
  },
  {
    id: "jobs",
    etiket: "Isler",
    yol: "/api/jobs",
    kategori: "veri",
    aciklama: "job_definitions — fiyat, sure, mesafe",
    anahtarGerekli: true,
    gorunum: "jobs",
  },
  {
    id: "metrik",
    etiket: "Metrik",
    yol: "/api/metrik?limit=40",
    kategori: "telemetri",
    aciklama: "Son istek sureleri (instance bellegi)",
    anahtarGerekli: true,
    gorunum: "metrik",
  },
];

export function labKategoriyeGore(): { kategori: LabKategori; maddeler: LabEndpoint[] }[] {
  const sira: LabKategori[] = ["durum", "veri", "telemetri"];
  return sira.map((kategori) => ({
    kategori,
    maddeler: LAB_ENDPOINTS.filter((e) => e.kategori === kategori),
  }));
}
