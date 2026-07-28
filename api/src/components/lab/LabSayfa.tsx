"use client";

import { useCallback, useEffect, useMemo, useState } from "react";
import { HealthGorunum, JobsGorunum, MetrikGorunum } from "@/components/lab/Gorunumler";
import {
  LAB_ENDPOINTS,
  LAB_KATEGORI_ETIKET,
  labKategoriyeGore,
  type LabEndpoint,
} from "@/lib/lab/endpoint-katalog";

const KEY_STORAGE = "m360_server_key";

export default function LabSayfa() {
  const [keyInput, setKeyInput] = useState("");
  const [keyVar, setKeyVar] = useState(false);
  const [seciliId, setSeciliId] = useState(LAB_ENDPOINTS[0]?.id ?? "health");
  const [filtre, setFiltre] = useState("");
  const [httpDurum, setHttpDurum] = useState<number | null>(null);
  const [data, setData] = useState<unknown>(null);
  const [ham, setHam] = useState("");
  const [hamAcik, setHamAcik] = useState(false);
  const [yukleniyor, setYukleniyor] = useState(false);
  const [hata, setHata] = useState<string | null>(null);

  const secili: LabEndpoint =
    LAB_ENDPOINTS.find((e) => e.id === seciliId) ?? LAB_ENDPOINTS[0];

  const gruplar = useMemo(() => {
    const q = filtre.trim().toLowerCase();
    return labKategoriyeGore()
      .map((g) => ({
        ...g,
        maddeler: g.maddeler.filter(
          (m) =>
            !q ||
            m.etiket.toLowerCase().includes(q) ||
            m.yol.toLowerCase().includes(q) ||
            m.aciklama.toLowerCase().includes(q)
        ),
      }))
      .filter((g) => g.maddeler.length > 0);
  }, [filtre]);

  useEffect(() => {
    setKeyVar(Boolean(sessionStorage.getItem(KEY_STORAGE)));
  }, []);

  const yukle = useCallback(async (ep: LabEndpoint) => {
    setSeciliId(ep.id);
    setYukleniyor(true);
    setHata(null);
    setHttpDurum(null);

    const headers: Record<string, string> = {
      "X-M360-Istek-Baslangic": String(Date.now()),
    };
    const k = sessionStorage.getItem(KEY_STORAGE);
    if (k) headers["X-M360-Server-Key"] = k;

    try {
      const res = await fetch(ep.yol, { headers });
      const text = await res.text();
      setHttpDurum(res.status);
      setHam(text);
      let parsed: unknown = null;
      try {
        parsed = JSON.parse(text);
        setData(parsed);
      } catch {
        setData(null);
      }
      if (res.status === 401) {
        setHata("401 — key kaydet (api/.env / Vercel M360_SERVER_KEY).");
      }
    } catch (err) {
      setHata(err instanceof Error ? err.message : String(err));
      setData(null);
      setHam("");
    } finally {
      setYukleniyor(false);
    }
  }, []);

  useEffect(() => {
    void yukle(secili);
    // sadece ilk mount
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  function kaydet() {
    const v = keyInput.trim();
    if (!v) {
      alert("Key bos.");
      return;
    }
    sessionStorage.setItem(KEY_STORAGE, v);
    setKeyInput("");
    setKeyVar(true);
    void yukle(secili);
  }

  function temizle() {
    sessionStorage.removeItem(KEY_STORAGE);
    setKeyInput("");
    setKeyVar(false);
  }

  return (
    <div className="lab">
      <aside className="yan">
        <div className="yan-baslik">
          <p className="marka">M360 Life</p>
          <h1>API Lab</h1>
          <p className="alt">Katalogdan sec · tip bazli oku · ham JSON ayri</p>
        </div>

        <div className="anahtar">
          <label htmlFor="key">
            Server key
            <span className="muted">{keyVar ? " · kayitli" : " · yok"}</span>
          </label>
          <input
            id="key"
            type="password"
            autoComplete="off"
            spellCheck={false}
            value={keyInput}
            onChange={(e) => setKeyInput(e.target.value)}
            onKeyDown={(e) => {
              if (e.key === "Enter") kaydet();
            }}
            placeholder={keyVar ? "degistirmek icin yaz" : "M360_SERVER_KEY"}
          />
          <div className="anahtar-aksiyon">
            <button type="button" className="btn" onClick={kaydet}>
              Kaydet
            </button>
            <button type="button" className="btn ghost" onClick={temizle}>
              Unut
            </button>
          </div>
        </div>

        <input
          className="filtre"
          value={filtre}
          onChange={(e) => setFiltre(e.target.value)}
          placeholder="Endpoint ara…"
          aria-label="Endpoint ara"
        />

        <nav className="katalog">
          {gruplar.map((g) => (
            <div key={g.kategori} className="grup">
              <p className="grup-baslik">{LAB_KATEGORI_ETIKET[g.kategori]}</p>
              {g.maddeler.map((m) => (
                <button
                  key={m.id}
                  type="button"
                  className={`madde${seciliId === m.id ? " aktif" : ""}`}
                  onClick={() => void yukle(m)}
                >
                  <span className="madde-ust">
                    <span>{m.etiket}</span>
                    {m.anahtarGerekli ? <span className="kilit">key</span> : null}
                  </span>
                  <span className="madde-yol">{m.yol.split("?")[0]}</span>
                </button>
              ))}
            </div>
          ))}
        </nav>

        <div className="yan-alt">
          <a href="/istatistik">Istatistik paneli →</a>
          <p className="ipucu">Yeni endpoint: <code>endpoint-katalog.ts</code></p>
        </div>
      </aside>

      <section className="icerik">
        <header className="icerik-ust">
          <div>
            <h2>{secili.etiket}</h2>
            <p className="aciklama">{secili.aciklama}</p>
            <p className="yol mono">{secili.yol}</p>
          </div>
          <div className="icerik-aksiyon">
            {httpDurum != null && (
              <span className={`badge ${httpDurum < 300 ? "ok" : "bad"}`}>HTTP {httpDurum}</span>
            )}
            <button
              type="button"
              className="btn"
              disabled={yukleniyor}
              onClick={() => void yukle(secili)}
            >
              {yukleniyor ? "…" : "Yenile"}
            </button>
          </div>
        </header>

        {hata ? <p className="banner-hata">{hata}</p> : null}

        <div className="gorunum">
          {secili.gorunum === "health" && <HealthGorunum data={data} httpDurum={httpDurum} />}
          {secili.gorunum === "jobs" && <JobsGorunum data={data} httpDurum={httpDurum} />}
          {secili.gorunum === "metrik" && <MetrikGorunum data={data} httpDurum={httpDurum} />}
        </div>

        <details className="ham" open={hamAcik} onToggle={(e) => setHamAcik((e.target as HTMLDetailsElement).open)}>
          <summary>Ham JSON</summary>
          <pre>{ham || "—"}</pre>
        </details>
      </section>

      <style>{LAB_CSS}</style>
    </div>
  );
}

const LAB_CSS = `
:root {
  --bg0: #0b100e;
  --bg1: #121a17;
  --bg2: #18241f;
  --line: #2a3d34;
  --text: #d7e6dc;
  --muted: #7f9a8a;
  --accent: #c4a35a;
  --ok: #5ecf8e;
  --bad: #e07a6a;
  --mono: "IBM Plex Mono", "Cascadia Mono", Consolas, monospace;
  --sans: "IBM Plex Sans", "Segoe UI", sans-serif;
}
* { box-sizing: border-box; }
body { margin: 0; background: var(--bg0); color: var(--text); font-family: var(--sans); }
.lab {
  display: grid;
  grid-template-columns: minmax(240px, 300px) 1fr;
  min-height: 100vh;
}
@media (max-width: 860px) {
  .lab { grid-template-columns: 1fr; }
}
.yan {
  border-right: 1px solid var(--line);
  background: linear-gradient(180deg, #101816, #0b100e);
  padding: 1.25rem 1rem 1.5rem;
  display: flex; flex-direction: column; gap: 0.85rem;
}
.yan-baslik .marka {
  margin: 0; font-size: 0.7rem; letter-spacing: 0.16em;
  text-transform: uppercase; color: var(--accent); font-weight: 600;
}
.yan-baslik h1 { margin: 0.25rem 0 0; font-size: 1.35rem; font-weight: 600; }
.yan-baslik .alt { margin: 0.35rem 0 0; color: var(--muted); font-size: 0.82rem; line-height: 1.35; }
.anahtar {
  border: 1px solid var(--line); background: var(--bg1); padding: 0.75rem;
}
.anahtar label { display: block; font-size: 0.78rem; margin-bottom: 0.4rem; }
.anahtar input, .filtre {
  width: 100%; background: var(--bg0); border: 1px solid var(--line);
  color: var(--text); padding: 0.45rem 0.55rem; font-family: var(--mono); font-size: 0.8rem;
}
.anahtar-aksiyon { display: flex; gap: 0.4rem; margin-top: 0.45rem; }
.filtre { margin: 0; }
.katalog { flex: 1; overflow: auto; display: flex; flex-direction: column; gap: 0.85rem; }
.grup-baslik {
  margin: 0 0 0.35rem; font-size: 0.68rem; letter-spacing: 0.1em;
  text-transform: uppercase; color: var(--muted);
}
.madde {
  width: 100%; text-align: left; appearance: none; cursor: pointer;
  border: 1px solid transparent; background: transparent;
  color: var(--text); padding: 0.55rem 0.5rem; margin-bottom: 0.2rem;
  font: inherit;
}
.madde:hover { border-color: var(--line); background: var(--bg1); }
.madde.aktif { border-color: var(--accent); background: #1a261f; }
.madde-ust { display: flex; justify-content: space-between; align-items: center; font-size: 0.92rem; }
.madde-yol { display: block; margin-top: 0.15rem; font-family: var(--mono); font-size: 0.72rem; color: var(--muted); }
.kilit {
  font-size: 0.65rem; letter-spacing: 0.06em; text-transform: uppercase;
  color: var(--accent); border: 1px solid var(--line); padding: 0.1rem 0.35rem;
}
.yan-alt { border-top: 1px solid var(--line); padding-top: 0.75rem; font-size: 0.82rem; }
.yan-alt a { color: var(--accent); }
.ipucu { margin: 0.45rem 0 0; color: var(--muted); font-size: 0.72rem; }
.ipucu code { font-family: var(--mono); }
.icerik {
  padding: 1.5rem clamp(1rem, 3vw, 2rem) 2.5rem;
  background:
    radial-gradient(700px 320px at 90% -10%, #2a2414 0%, transparent 50%),
    var(--bg0);
}
.icerik-ust {
  display: flex; flex-wrap: wrap; gap: 1rem;
  justify-content: space-between; align-items: flex-start;
  margin-bottom: 1rem; padding-bottom: 0.85rem; border-bottom: 1px solid var(--line);
}
.icerik-ust h2 { margin: 0; font-size: 1.45rem; }
.aciklama { margin: 0.35rem 0 0; color: var(--muted); }
.yol { margin: 0.35rem 0 0; font-size: 0.8rem; color: var(--accent); }
.mono { font-family: var(--mono); }
.icerik-aksiyon { display: flex; gap: 0.5rem; align-items: center; }
.btn {
  appearance: none; border: 1px solid var(--line); background: var(--bg2);
  color: var(--text); padding: 0.45rem 0.8rem; cursor: pointer; font: inherit; font-size: 0.85rem;
}
.btn:hover:not(:disabled) { border-color: var(--accent); color: var(--accent); }
.btn:disabled { opacity: 0.5; }
.btn.ghost { background: transparent; }
.muted { color: var(--muted); }
.banner-hata {
  border: 1px solid var(--bad); color: var(--bad); padding: 0.65rem 0.8rem;
  background: #1a1210; margin: 0 0 1rem;
}
.gorunum { margin-bottom: 1rem; }
.panel {
  border: 1px solid var(--line); background: #101816; padding: 1rem 1.1rem;
}
.panel-ust {
  display: flex; flex-wrap: wrap; gap: 0.75rem;
  justify-content: space-between; align-items: center; margin-bottom: 0.9rem;
}
.panel-ust h2 { margin: 0; font-size: 1.05rem; font-weight: 600; }
.rozetler { display: flex; flex-wrap: wrap; gap: 0.35rem; }
.badge {
  font-family: var(--mono); font-size: 0.72rem; padding: 0.2rem 0.45rem;
  border: 1px solid var(--line);
}
.badge.ok { color: var(--ok); border-color: #2f5a40; }
.badge.bad { color: var(--bad); border-color: #5a2f2f; }
.badge.notr { color: var(--muted); }
.grid-bilgi {
  display: grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
  gap: 0.75rem; margin: 0;
}
.grid-bilgi dt { margin: 0; color: var(--muted); font-size: 0.72rem; text-transform: uppercase; letter-spacing: 0.06em; }
.grid-bilgi dd { margin: 0.25rem 0 0; font-family: var(--mono); font-size: 0.95rem; }
.grid-bilgi .genis { grid-column: 1 / -1; }
.ozet-grid {
  display: grid; grid-template-columns: repeat(auto-fit, minmax(130px, 1fr));
  gap: 0.55rem; margin-bottom: 0.85rem;
}
.kart {
  border: 1px solid var(--line); background: linear-gradient(160deg, #18241f, #121a17);
  padding: 0.7rem 0.8rem;
}
.kart span { display: block; color: var(--muted); font-size: 0.68rem; text-transform: uppercase; letter-spacing: 0.06em; }
.kart strong { display: block; margin-top: 0.25rem; font-family: var(--mono); font-size: 1.2rem; font-weight: 500; }
.tablo-wrap { overflow: auto; border: 1px solid var(--line); }
table { width: 100%; border-collapse: collapse; font-size: 0.82rem; }
th, td { padding: 0.5rem 0.65rem; text-align: left; border-bottom: 1px solid var(--line); white-space: nowrap; }
th {
  color: var(--muted); font-size: 0.68rem; letter-spacing: 0.05em; text-transform: uppercase;
  background: #0c1210; position: sticky; top: 0;
}
td { font-family: var(--mono); }
.bos { color: var(--muted); font-family: var(--sans); padding: 1rem; }
.uyari { color: var(--bad); }
.not { color: var(--muted); font-size: 0.8rem; }
.ham {
  border: 1px solid var(--line); background: #0c1210; padding: 0.35rem 0.75rem 0.75rem;
}
.ham summary {
  cursor: pointer; color: var(--muted); font-size: 0.82rem; padding: 0.45rem 0;
}
.ham pre {
  margin: 0; overflow: auto; max-height: 320px;
  font-family: var(--mono); font-size: 0.78rem; line-height: 1.4; white-space: pre-wrap;
}
`;
