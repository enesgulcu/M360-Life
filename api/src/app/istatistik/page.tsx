"use client";

import { useCallback, useEffect, useState } from "react";

const KEY_STORAGE = "m360_server_key";

type MetrikKayit = {
  zaman?: string;
  zamanTr?: string;
  yontem?: string;
  yol?: string;
  durum?: number;
  gelisMs?: number;
  sunucuMs?: number;
  toplamMs?: number;
  ip?: string;
};

type Ozet = {
  adet?: number;
  ortSunucuMs?: number;
  ortToplamMs?: number;
  p95SunucuMs?: number;
  son5dk?: number;
};

function ms(v?: number) {
  if (v == null) return "—";
  return `${Math.round(v)} ms`;
}

function durumSinif(d?: number) {
  if (d != null && d >= 200 && d < 300) return "durum-ok";
  if (d === 429 || d === 401) return "durum-warn";
  return "durum-bad";
}

export default function IstatistikPage() {
  const [ozet, setOzet] = useState<Ozet>({});
  const [kayitlar, setKayitlar] = useState<MetrikKayit[]>([]);
  const [hata, setHata] = useState<string | null>(null);
  const [otomatik, setOtomatik] = useState(true);
  const [bosMesaj, setBosMesaj] = useState("Yukleniyor…");

  const anahtarAl = useCallback((zorla: boolean) => {
    let k = sessionStorage.getItem(KEY_STORAGE) || "";
    if (!k || zorla) {
      k = window.prompt("M360_SERVER_KEY (api/.env / Vercel ile ayni)", k || "") || "";
      if (k) sessionStorage.setItem(KEY_STORAGE, k);
      else if (zorla) sessionStorage.removeItem(KEY_STORAGE);
    }
    return k;
  }, []);

  const basliklar = useCallback(
    (zorla = false) => {
      const h: Record<string, string> = {
        "X-M360-Istek-Baslangic": String(Date.now()),
      };
      const k = anahtarAl(zorla);
      if (k) h["X-M360-Server-Key"] = k;
      return h;
    },
    [anahtarAl]
  );

  const yukle = useCallback(async () => {
    try {
      const res = await fetch("/api/metrik?limit=60", { headers: basliklar() });
      const data = await res.json();
      if (res.status === 401) {
        sessionStorage.removeItem(KEY_STORAGE);
        setHata("401 - anahtar gerekli. Key butonuna bas.");
        setOzet({});
        setKayitlar([]);
        setBosMesaj("Anahtar yanlis veya yok.");
        return;
      }
      setHata(null);
      setOzet(data.ozet || {});
      const list: MetrikKayit[] = data.kayitlar || [];
      setKayitlar(list);
      setBosMesaj(list.length ? "" : "Henuz kayit yok. Ornek istek gonder.");
    } catch (err) {
      setHata(err instanceof Error ? err.message : String(err));
    }
  }, [basliklar]);

  const ornek = useCallback(async () => {
    const headers = basliklar();
    await fetch("/api/health", { headers });
    await fetch("/api/jobs", { headers });
    await yukle();
  }, [basliklar, yukle]);

  useEffect(() => {
    void yukle();
  }, [yukle]);

  useEffect(() => {
    if (!otomatik) return;
    const t = setInterval(() => void yukle(), 4000);
    return () => clearInterval(t);
  }, [otomatik, yukle]);

  return (
    <main className="shell">
      <header className="ust">
        <div>
          <p className="marka">M360 Life</p>
          <h1>API istatistik</h1>
          <p className="alt">
            Istek gelis / sunucu islem / toplam tur suresi. Lab telemetrisi — cold start
            sonrasi sifirlanabilir.
          </p>
        </div>
        <div className="aksiyonlar">
          <a className="btn ikincil" href="/">
            API lab
          </a>
          <button type="button" className="btn ikincil" onClick={() => { anahtarAl(true); void yukle(); }}>
            Key
          </button>
          <button type="button" className="btn" onClick={() => void yukle()}>
            Yenile
          </button>
          <button type="button" className="btn ikincil" onClick={() => void ornek()}>
            Ornek istek
          </button>
          <label className="otomatik">
            <input
              type="checkbox"
              checked={otomatik}
              onChange={(e) => setOtomatik(e.target.checked)}
            />
            Canli
          </label>
        </div>
      </header>

      <section className="ozet" aria-live="polite">
        {hata ? (
          <div className="kart">
            <span>Hata</span>
            <strong>{hata}</strong>
          </div>
        ) : (
          <>
            <div className="kart">
              <span>Kayit</span>
              <strong>{ozet.adet ?? 0}</strong>
            </div>
            <div className="kart">
              <span>Ort. sunucu</span>
              <strong>{ms(ozet.ortSunucuMs)}</strong>
            </div>
            <div className="kart">
              <span>Ort. toplam</span>
              <strong>{ms(ozet.ortToplamMs)}</strong>
            </div>
            <div className="kart">
              <span>p95 sunucu</span>
              <strong>{ms(ozet.p95SunucuMs)}</strong>
            </div>
            <div className="kart">
              <span>Son 5 dk</span>
              <strong>{ozet.son5dk ?? 0}</strong>
            </div>
          </>
        )}
      </section>

      <section className="tablo-wrap">
        <table>
          <thead>
            <tr>
              <th>Zaman</th>
              <th>Yol</th>
              <th>Durum</th>
              <th>Gelis</th>
              <th>Sunucu</th>
              <th>Toplam</th>
              <th>IP</th>
            </tr>
          </thead>
          <tbody>
            {kayitlar.length === 0 ? (
              <tr>
                <td colSpan={7} className="bos">
                  {bosMesaj}
                </td>
              </tr>
            ) : (
              kayitlar.map((k, i) => (
                <tr key={`${k.zaman}-${i}`}>
                  <td>{k.zamanTr || (k.zaman || "").replace("T", " ").replace("Z", "")}</td>
                  <td>
                    {k.yontem} {k.yol}
                  </td>
                  <td className={durumSinif(k.durum)}>{k.durum}</td>
                  <td>{ms(k.gelisMs)}</td>
                  <td>{ms(k.sunucuMs)}</td>
                  <td>{ms(k.toplamMs)}</td>
                  <td>{k.ip || ""}</td>
                </tr>
              ))
            )}
          </tbody>
        </table>
      </section>

      <style>{`
        :root {
          --bg0: #0e1412;
          --bg1: #15201c;
          --line: #2a3d34;
          --text: #d7e6dc;
          --muted: #7f9a8a;
          --accent: #c4a35a;
          --ok: #5ecf8e;
          --bad: #e07a6a;
          --warn: #d4a017;
          --mono: "IBM Plex Mono", "Cascadia Mono", "Consolas", monospace;
          --sans: "IBM Plex Sans", "Segoe UI", sans-serif;
        }
        * { box-sizing: border-box; }
        body { margin: 0; background: var(--bg0); color: var(--text); font-family: var(--sans); }
        .shell {
          min-height: 100vh;
          padding: 2rem clamp(1rem, 4vw, 3rem) 3rem;
          background:
            radial-gradient(900px 400px at 10% -10%, #1c3328 0%, transparent 55%),
            radial-gradient(700px 360px at 90% 0%, #2a2414 0%, transparent 50%),
            var(--bg0);
        }
        .ust {
          display: flex; flex-wrap: wrap; gap: 1.5rem;
          justify-content: space-between; align-items: end;
          margin-bottom: 1.75rem;
          border-bottom: 1px solid var(--line);
          padding-bottom: 1.25rem;
        }
        .marka {
          margin: 0 0 0.35rem;
          font-size: 0.75rem; letter-spacing: 0.18em; text-transform: uppercase;
          color: var(--accent); font-weight: 600;
        }
        h1 { margin: 0; font-size: clamp(1.6rem, 3vw, 2.2rem); font-weight: 600; }
        .alt { margin: 0.5rem 0 0; max-width: 36rem; color: var(--muted); line-height: 1.45; }
        .aksiyonlar { display: flex; gap: 0.6rem; align-items: center; flex-wrap: wrap; }
        .btn {
          appearance: none; border: 1px solid var(--line); background: var(--bg1);
          color: var(--text); padding: 0.55rem 0.95rem; border-radius: 2px;
          cursor: pointer; font: inherit; font-size: 0.9rem;
        }
        .btn:hover { border-color: var(--accent); color: var(--accent); }
        .btn.ikincil { background: transparent; }
        a.btn { text-decoration: none; display: inline-block; }
        .otomatik { display: flex; gap: 0.4rem; align-items: center; color: var(--muted); font-size: 0.85rem; }
        .ozet {
          display: grid;
          grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
          gap: 0.75rem;
          margin-bottom: 1.5rem;
        }
        .kart {
          background: linear-gradient(160deg, #18241f, #121a17);
          border: 1px solid var(--line);
          padding: 0.9rem 1rem;
        }
        .kart span { display: block; color: var(--muted); font-size: 0.72rem; letter-spacing: 0.06em; text-transform: uppercase; }
        .kart strong { display: block; margin-top: 0.35rem; font-family: var(--mono); font-size: 1.45rem; font-weight: 500; }
        .tablo-wrap {
          overflow: auto; border: 1px solid var(--line); background: #101816;
        }
        table { width: 100%; border-collapse: collapse; font-size: 0.88rem; }
        th, td { padding: 0.65rem 0.8rem; text-align: left; border-bottom: 1px solid var(--line); white-space: nowrap; }
        th { color: var(--muted); font-weight: 500; font-size: 0.72rem; letter-spacing: 0.06em; text-transform: uppercase; background: #0c1210; position: sticky; top: 0; }
        td { font-family: var(--mono); }
        .durum-ok { color: var(--ok); }
        .durum-bad { color: var(--bad); }
        .durum-warn { color: var(--warn); }
        .bos { padding: 2rem; color: var(--muted); font-family: var(--sans); }
      `}</style>
    </main>
  );
}
