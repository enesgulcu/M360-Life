export default function HomePage() {
  return (
    <main style={{ fontFamily: "Georgia, serif", padding: 24, maxWidth: 640, background: "#0e1412", color: "#d7e6dc", minHeight: "100vh" }}>
      <p style={{ letterSpacing: "0.14em", textTransform: "uppercase", color: "#c4a35a", fontSize: 12 }}>M360 Life</p>
      <h1 style={{ marginTop: 8 }}>game-api</h1>
      <p style={{ color: "#7f9a8a", lineHeight: 1.5 }}>
        Oyun sunucusu HTTP API. Veritabani: Neon. Deploy: Vercel (Root Directory: api).
      </p>
      <ul style={{ lineHeight: 1.8 }}>
        <li>
          <a href="/api/health" style={{ color: "#c4a35a" }}>/api/health</a> — servis + DB
        </li>
        <li>
          <a href="/api/jobs" style={{ color: "#c4a35a" }}>/api/jobs</a> — job_definitions
        </li>
        <li>
          <a href="/api/metrik" style={{ color: "#c4a35a" }}>/api/metrik</a> — istek sureleri (JSON)
        </li>
        <li>
          <a href="/istatistik" style={{ color: "#c4a35a" }}>/istatistik</a> — canli panel
        </li>
      </ul>
    </main>
  );
}
