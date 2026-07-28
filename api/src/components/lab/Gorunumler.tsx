"use client";

type Props = {
  data: unknown;
  httpDurum: number | null;
};

function Badge({ ok, children }: { ok: boolean; children: React.ReactNode }) {
  return <span className={`badge ${ok ? "ok" : "bad"}`}>{children}</span>;
}

export function HealthGorunum({ data, httpDurum }: Props) {
  const d = (data && typeof data === "object" ? data : {}) as Record<string, unknown>;
  const db = String(d.db ?? "—");
  const ok = Boolean(d.ok);
  const dbOk = db === "bagli";

  return (
    <div className="panel">
      <div className="panel-ust">
        <h2>Servis durumu</h2>
        <div className="rozetler">
          <Badge ok={httpDurum === 200}>HTTP {httpDurum ?? "—"}</Badge>
          <Badge ok={ok}>API {ok ? "ok" : "degil"}</Badge>
          <Badge ok={dbOk}>DB {db}</Badge>
        </div>
      </div>
      <dl className="grid-bilgi">
        <div>
          <dt>Servis</dt>
          <dd>{String(d.servis ?? "—")}</dd>
        </div>
        <div>
          <dt>Zaman (TR)</dt>
          <dd className="mono">{String(d.zamanTr ?? "—")}</dd>
        </div>
        <div>
          <dt>Zaman (UTC)</dt>
          <dd className="mono">{String(d.zaman ?? "—")}</dd>
        </div>
        <div>
          <dt>Veritabani</dt>
          <dd>{db}</dd>
        </div>
        {d.dbMesaj ? (
          <div className="genis">
            <dt>DB mesaj</dt>
            <dd className="uyari">{String(d.dbMesaj)}</dd>
          </div>
        ) : null}
      </dl>
    </div>
  );
}

type IsSatir = Record<string, unknown>;

export function JobsGorunum({ data, httpDurum }: Props) {
  const d = (data && typeof data === "object" ? data : {}) as Record<string, unknown>;
  const isler = Array.isArray(d.isler) ? (d.isler as IsSatir[]) : [];
  const kolonlar = [
    "is_adi",
    "yasal_mi",
    "adim_suresi",
    "adim_verim",
    "parti_boyutu",
    "isleme_suresi",
    "donusum_orani",
    "satis_fiyati",
    "max_tasima",
    "iptal_mesafesi",
  ];

  return (
    <div className="panel">
      <div className="panel-ust">
        <h2>Is tanimlari</h2>
        <div className="rozetler">
          <Badge ok={httpDurum === 200 && Boolean(d.ok)}>HTTP {httpDurum ?? "—"}</Badge>
          <Badge ok={String(d.kaynak) === "postgresql"}>{String(d.kaynak ?? "—")}</Badge>
          <span className="badge notr">{isler.length} satir</span>
        </div>
      </div>
      {d.mesaj ? <p className="uyari">{String(d.mesaj)}</p> : null}
      {isler.length === 0 ? (
        <p className="bos">Satir yok.</p>
      ) : (
        <div className="tablo-wrap">
          <table>
            <thead>
              <tr>
                {kolonlar.map((k) => (
                  <th key={k}>{k}</th>
                ))}
              </tr>
            </thead>
            <tbody>
              {isler.map((satir, i) => (
                <tr key={String(satir.id ?? i)}>
                  {kolonlar.map((k) => (
                    <td key={k}>{formatHucre(satir[k])}</td>
                  ))}
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      )}
    </div>
  );
}

export function MetrikGorunum({ data, httpDurum }: Props) {
  const d = (data && typeof data === "object" ? data : {}) as Record<string, unknown>;
  const ozet = (d.ozet && typeof d.ozet === "object" ? d.ozet : {}) as Record<string, unknown>;
  const kayitlar = Array.isArray(d.kayitlar) ? (d.kayitlar as Record<string, unknown>[]) : [];

  return (
    <div className="panel">
      <div className="panel-ust">
        <h2>Istek metrikleri</h2>
        <div className="rozetler">
          <Badge ok={httpDurum === 200}>HTTP {httpDurum ?? "—"}</Badge>
          <span className="badge notr">{String(ozet.adet ?? 0)} kayit</span>
        </div>
      </div>
      <div className="ozet-grid">
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
          <strong>{String(ozet.son5dk ?? 0)}</strong>
        </div>
      </div>
      {d.not ? <p className="not">{String(d.not)}</p> : null}
      <div className="tablo-wrap">
        <table>
          <thead>
            <tr>
              <th>Zaman</th>
              <th>Yol</th>
              <th>Durum</th>
              <th>Gelis</th>
              <th>Sunucu</th>
              <th>Toplam</th>
            </tr>
          </thead>
          <tbody>
            {kayitlar.length === 0 ? (
              <tr>
                <td colSpan={6} className="bos">
                  Kayit yok — bir endpoint cagir.
                </td>
              </tr>
            ) : (
              kayitlar.slice(0, 25).map((k, i) => (
                <tr key={`${k.zaman}-${i}`}>
                  <td>{String(k.zamanTr ?? k.zaman ?? "").replace("T", " ").replace("Z", "")}</td>
                  <td>
                    {String(k.yontem ?? "")} {String(k.yol ?? "")}
                  </td>
                  <td>{String(k.durum ?? "")}</td>
                  <td>{ms(k.gelisMs)}</td>
                  <td>{ms(k.sunucuMs)}</td>
                  <td>{ms(k.toplamMs)}</td>
                </tr>
              ))
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
}

function formatHucre(v: unknown): string {
  if (typeof v === "boolean") return v ? "evet" : "hayir";
  if (v == null) return "—";
  return String(v);
}

function ms(v: unknown) {
  if (v == null || v === "") return "—";
  const n = Number(v);
  if (!Number.isFinite(n)) return "—";
  return `${Math.round(n)} ms`;
}
