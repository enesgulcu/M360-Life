-- M360 Life — job_definitions (Dokuman 5.1b / 11.8)
-- Uygula: psql -U m360 -d m360_life -f 001_job_definitions.sql

CREATE TABLE IF NOT EXISTS job_definitions (
  id              SERIAL PRIMARY KEY,
  is_adi          TEXT NOT NULL UNIQUE,
  yasal_mi        BOOLEAN NOT NULL DEFAULT TRUE,
  adim_suresi     INT NOT NULL DEFAULT 4,
  adim_verim      INT NOT NULL DEFAULT 2,
  parti_boyutu    INT NOT NULL DEFAULT 100,
  isleme_suresi   INT NOT NULL DEFAULT 180,
  donusum_orani   REAL NOT NULL DEFAULT 1.0,
  satis_fiyati    INT NOT NULL DEFAULT 600,
  max_tasima      INT NOT NULL DEFAULT 40,
  iptal_mesafesi  REAL NOT NULL DEFAULT 4.0,
  guncelleme_at   TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

INSERT INTO job_definitions (
  is_adi, yasal_mi, adim_suresi, adim_verim, parti_boyutu,
  isleme_suresi, donusum_orani, satis_fiyati, max_tasima, iptal_mesafesi
) VALUES (
  'Pirinc', TRUE, 4, 2, 100,
  180, 1.0, 600, 40, 4.0
)
ON CONFLICT (is_adi) DO NOTHING;
