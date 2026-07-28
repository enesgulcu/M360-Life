export default function HomePage() {
  return (
    <main className="shell">
      <header className="ust">
        <div>
          <p className="marka">M360 Life</p>
          <h1>API laboratuvar</h1>
          <p className="alt">
            Tarayici ham <code>/api/jobs</code> adresine anahtar gonderemez. Burada key
            girersin; health / jobs / metrik ayni oturumda acilir.
          </p>
        </div>
        <a className="btn ikincil" href="/istatistik">
          Istatistik paneli
        </a>
      </header>

      <section className="anahtar-kutu">
        <label htmlFor="key">
          M360_SERVER_KEY
          <span id="key-durum" className="muted"> — kayitli degil</span>
        </label>
        <div className="satir">
          <input
            id="key"
            type="password"
            autoComplete="off"
            spellCheck={false}
            placeholder="api/.env veya Vercel env ile ayni"
          />
          <button type="button" id="kaydet" className="btn">
            Kaydet
          </button>
          <button type="button" id="temizle" className="btn ikincil">
            Unut
          </button>
        </div>
      </section>

      <nav className="sekmeler" id="sekmeler">
        <button type="button" className="sekme aktif" data-yol="/api/health">
          health
        </button>
        <button type="button" className="sekme" data-yol="/api/jobs">
          jobs
        </button>
        <button type="button" className="sekme" data-yol="/api/metrik?limit=20">
          metrik
        </button>
        <button type="button" id="yenile" className="btn ikincil">
          Yenile
        </button>
      </nav>

      <p className="yol-etiket">
        <span id="yol-goster">/api/health</span>
        <span id="http-durum" className="muted" />
      </p>
      <pre id="cikti" className="cikti">Key kaydet, sonra bir sekme sec…</pre>

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
            radial-gradient(700px 360px at 100% 0%, #2a2414 0%, transparent 45%),
            var(--bg0);
        }
        .ust {
          display: flex; flex-wrap: wrap; gap: 1rem;
          justify-content: space-between; align-items: end;
          margin-bottom: 1.5rem; padding-bottom: 1.1rem;
          border-bottom: 1px solid var(--line);
        }
        .marka {
          margin: 0 0 0.35rem; font-size: 0.75rem; letter-spacing: 0.18em;
          text-transform: uppercase; color: var(--accent); font-weight: 600;
        }
        h1 { margin: 0; font-size: clamp(1.55rem, 3vw, 2.1rem); font-weight: 600; }
        .alt { margin: 0.45rem 0 0; max-width: 38rem; color: var(--muted); line-height: 1.45; }
        .alt code { font-family: var(--mono); font-size: 0.85em; color: var(--text); }
        .btn {
          appearance: none; border: 1px solid var(--line); background: var(--bg1);
          color: var(--text); padding: 0.55rem 0.95rem; border-radius: 2px;
          cursor: pointer; font: inherit; font-size: 0.9rem; text-decoration: none;
          display: inline-block;
        }
        .btn:hover { border-color: var(--accent); color: var(--accent); }
        .btn.ikincil { background: transparent; }
        .anahtar-kutu {
          margin-bottom: 1.25rem; padding: 1rem;
          border: 1px solid var(--line); background: #121a17;
        }
        .anahtar-kutu label { display: block; margin-bottom: 0.5rem; font-size: 0.85rem; }
        .muted { color: var(--muted); }
        .satir { display: flex; gap: 0.5rem; flex-wrap: wrap; }
        #key {
          flex: 1 1 220px; min-width: 180px;
          background: var(--bg0); border: 1px solid var(--line); color: var(--text);
          padding: 0.55rem 0.7rem; font-family: var(--mono); font-size: 0.85rem;
        }
        #key:focus { outline: 1px solid var(--accent); }
        .sekmeler { display: flex; flex-wrap: wrap; gap: 0.45rem; margin-bottom: 0.85rem; }
        .sekme {
          appearance: none; border: 1px solid var(--line); background: transparent;
          color: var(--muted); padding: 0.45rem 0.8rem; cursor: pointer; font: inherit;
        }
        .sekme.aktif, .sekme:hover { color: var(--accent); border-color: var(--accent); }
        .yol-etiket {
          font-family: var(--mono); font-size: 0.8rem; color: var(--muted);
          margin: 0 0 0.5rem;
        }
        .cikti {
          margin: 0; padding: 1rem 1.1rem; overflow: auto;
          background: #0c1210; border: 1px solid var(--line);
          font-family: var(--mono); font-size: 0.82rem; line-height: 1.45;
          white-space: pre-wrap; word-break: break-word; min-height: 12rem;
        }
        .http-ok { color: var(--ok); }
        .http-bad { color: var(--bad); }
      `}</style>

      <script
        dangerouslySetInnerHTML={{
          __html: `
(function () {
  var KEY = "m360_server_key";
  var aktifYol = "/api/health";
  var input = document.getElementById("key");
  var durum = document.getElementById("key-durum");
  var cikti = document.getElementById("cikti");
  var yolGoster = document.getElementById("yol-goster");
  var httpDurum = document.getElementById("http-durum");

  function keyOku() { return sessionStorage.getItem(KEY) || ""; }

  function keyDurumGuncelle() {
    var k = keyOku();
    durum.textContent = k ? " — kayitli (bu sekme)" : " — kayitli degil";
    if (k && !input.value) input.placeholder = "(kayitli — degistirmek icin yaz)";
  }

  function basliklar() {
    var h = { "X-M360-Istek-Baslangic": String(Date.now()) };
    var k = keyOku();
    if (k) h["X-M360-Server-Key"] = k;
    return h;
  }

  async function yukle(yol) {
    aktifYol = yol || aktifYol;
    yolGoster.textContent = aktifYol;
    document.querySelectorAll(".sekme").forEach(function (b) {
      b.classList.toggle("aktif", b.getAttribute("data-yol") === aktifYol);
    });
    cikti.textContent = "Yukleniyor…";
    httpDurum.textContent = "";
    try {
      var res = await fetch(aktifYol, { headers: basliklar() });
      var text = await res.text();
      var goster = text;
      try { goster = JSON.stringify(JSON.parse(text), null, 2); } catch (e) {}
      httpDurum.textContent = " · HTTP " + res.status;
      httpDurum.className = res.ok ? "http-ok" : "http-bad";
      if (res.status === 401) {
        cikti.textContent = goster + "\\n\\n---\\n401: Key kaydet (api/.env / Vercel M360_SERVER_KEY ile ayni).";
        return;
      }
      cikti.textContent = goster;
    } catch (err) {
      httpDurum.textContent = " · hata";
      httpDurum.className = "http-bad";
      cikti.textContent = String(err);
    }
  }

  document.getElementById("kaydet").addEventListener("click", function () {
    var v = (input.value || "").trim();
    if (!v) { alert("Key bos."); return; }
    sessionStorage.setItem(KEY, v);
    input.value = "";
    keyDurumGuncelle();
    yukle(aktifYol);
  });

  document.getElementById("temizle").addEventListener("click", function () {
    sessionStorage.removeItem(KEY);
    input.value = "";
    keyDurumGuncelle();
    cikti.textContent = "Key silindi. Kaydetmeden kilitli endpoint'ler 401 verir.";
  });

  document.getElementById("sekmeler").addEventListener("click", function (e) {
    var t = e.target;
    if (t && t.getAttribute && t.getAttribute("data-yol")) yukle(t.getAttribute("data-yol"));
  });

  document.getElementById("yenile").addEventListener("click", function () { yukle(aktifYol); });

  keyDurumGuncelle();
  yukle("/api/health");
})();
          `,
        }}
      />
    </main>
  );
}
