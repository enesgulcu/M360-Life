import type { Metadata } from "next";

export const metadata: Metadata = {
  title: "M360 — Istatistik",
  description: "API istek sureleri ve lab telemetri",
};

export default function IstatistikPage() {
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
          <button type="button" id="yenile" className="btn">
            Yenile
          </button>
          <button type="button" id="ornek" className="btn ikincil">
            Ornek istek
          </button>
          <label className="otomatik">
            <input type="checkbox" id="otomatik" defaultChecked />
            Canli
          </label>
        </div>
      </header>

      <section className="ozet" id="ozet" aria-live="polite" />

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
          <tbody id="satirlar" />
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

      <script
        dangerouslySetInnerHTML={{
          __html: `
(function () {
  const ozetEl = document.getElementById("ozet");
  const satirlar = document.getElementById("satirlar");
  const otomatik = document.getElementById("otomatik");
  let timer;

  function ms(v) {
    if (v == null || v === undefined) return "—";
    return Math.round(v) + " ms";
  }

  function durumSinif(d) {
    if (d >= 200 && d < 300) return "durum-ok";
    if (d === 429 || d === 401) return "durum-warn";
    return "durum-bad";
  }

  function kart(etiket, deger) {
    return '<div class="kart"><span>' + etiket + '</span><strong>' + deger + '</strong></div>';
  }

  async function anahtarAl() {
    var k = sessionStorage.getItem("m360_server_key") || "";
    if (!k) {
      k = prompt("X-M360-Server-Key (Vercel / api/.env ile ayni)") || "";
      if (k) sessionStorage.setItem("m360_server_key", k);
    }
    return k;
  }

  function basliklar(ekstra) {
    var h = Object.assign({ "X-M360-Istek-Baslangic": String(Date.now()) }, ekstra || {});
    return anahtarAl().then(function (k) {
      if (k) h["X-M360-Server-Key"] = k;
      return h;
    });
  }

  async function yukle() {
    const headers = await basliklar();
    const res = await fetch("/api/metrik?limit=60", { headers });
    const data = await res.json();
    if (res.status === 401) {
      sessionStorage.removeItem("m360_server_key");
      ozetEl.innerHTML = kart("Hata", "401 - anahtar gerekli");
      satirlar.innerHTML = '<tr><td colspan="7" class="bos">Anahtar yanlis veya yok. Yenile ile tekrar dene.</td></tr>';
      return;
    }
    const o = data.ozet || {};
    ozetEl.innerHTML =
      kart("Kayit", o.adet ?? 0) +
      kart("Ort. sunucu", ms(o.ortSunucuMs)) +
      kart("Ort. toplam", ms(o.ortToplamMs)) +
      kart("p95 sunucu", ms(o.p95SunucuMs)) +
      kart("Son 5 dk", o.son5dk ?? 0);

    const list = data.kayitlar || [];
    if (!list.length) {
      satirlar.innerHTML = '<tr><td colspan="7" class="bos">Henuz kayit yok. Ornek istek gonder.</td></tr>';
      return;
    }
    satirlar.innerHTML = list.map(function (k) {
      return '<tr>' +
        '<td>' + (k.zaman || "").replace("T", " ").replace("Z", "") + '</td>' +
        '<td>' + k.yontem + " " + k.yol + '</td>' +
        '<td class="' + durumSinif(k.durum) + '">' + k.durum + '</td>' +
        '<td>' + ms(k.gelisMs) + '</td>' +
        '<td>' + ms(k.sunucuMs) + '</td>' +
        '<td>' + ms(k.toplamMs) + '</td>' +
        '<td>' + (k.ip || "") + '</td>' +
        '</tr>';
    }).join("");
  }

  async function ornek() {
    const headers = await basliklar();
    await fetch("/api/health", { headers });
    await fetch("/api/jobs", { headers });
    await yukle();
  }

  document.getElementById("yenile").addEventListener("click", yukle);
  document.getElementById("ornek").addEventListener("click", ornek);
  otomatik.addEventListener("change", function () {
    clearInterval(timer);
    if (otomatik.checked) timer = setInterval(yukle, 4000);
  });
  yukle();
  timer = setInterval(yukle, 4000);
})();
          `,
        }}
      />
    </main>
  );
}
