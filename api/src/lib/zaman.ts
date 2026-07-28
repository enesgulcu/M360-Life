/** Sunucu ISO (UTC) saklar; arayuz Turkiye saati gosterir. */

const TZ = "Europe/Istanbul";

export function zamanIsoUtc(d = new Date()): string {
  return d.toISOString();
}

/** Ornek: 2026-07-28 11:56:28 (TR) */
export function zamanTrMetin(isoVeyaMs?: string | number | Date | null): string {
  if (isoVeyaMs == null || isoVeyaMs === "") return "—";
  const d =
    typeof isoVeyaMs === "number"
      ? new Date(isoVeyaMs)
      : isoVeyaMs instanceof Date
        ? isoVeyaMs
        : new Date(isoVeyaMs);
  if (Number.isNaN(d.getTime())) return "—";

  const parcalar = new Intl.DateTimeFormat("tr-TR", {
    timeZone: TZ,
    year: "numeric",
    month: "2-digit",
    day: "2-digit",
    hour: "2-digit",
    minute: "2-digit",
    second: "2-digit",
    hour12: false,
  }).formatToParts(d);

  const al = (tip: Intl.DateTimeFormatPartTypes) =>
    parcalar.find((p) => p.type === tip)?.value ?? "";

  return `${al("year")}-${al("month")}-${al("day")} ${al("hour")}:${al("minute")}:${al("second")}`;
}

export function healthZamanAlanlari(d = new Date()) {
  return {
    zaman: zamanIsoUtc(d),
    zamanTr: zamanTrMetin(d),
    saatDilimi: TZ,
  };
}
