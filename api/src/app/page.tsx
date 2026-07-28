export default function HomePage() {
  return (
    <main style={{ fontFamily: "system-ui", padding: 24, maxWidth: 640 }}>
      <h1>M360 game-api</h1>
      <p>
        Oyun sunucusu ve admin paneli icin HTTP API. Veritabani: hosted PostgreSQL
        (Neon). Deploy: Vercel.
      </p>
      <ul>
        <li>
          <a href="/api/health">/api/health</a> — servis + DB durumu
        </li>
        <li>
          <a href="/api/jobs">/api/jobs</a> — <code>job_definitions</code>
        </li>
      </ul>
    </main>
  );
}
