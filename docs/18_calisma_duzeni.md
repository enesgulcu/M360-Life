# M360 Life — Çalışma düzeni (iki yol, tek fiziksel oyun)

## Klasörler

| Yol | Rol |
|---|---|
| `C:\Users\Enes\Documents\GitHub\M360-Life` | Monorepo — api, web, docs, packages, **git** |
| `...\GitHub\M360-Life\m360-life` | Oyun kaynakları (git’te) |
| `...\My Games\ArmaReforgerWorkbench\addons\M360-Life` | Workbench’in açtığı yol |

## Senkron = junction (önerilen)

Kopyala-yapıştır yok. Workbench yolu **junction** ile GitHub `m360-life`’a bağlanır; birinde değişen dosya diğerinde de aynıdır.

```bat
REM Workbench KAPALI iken:
powershell -File C:\Users\Enes\Documents\GitHub\M360-Life\tools\bagla-oyun-klasoru.ps1
```

- Geri al: `...\bagla-oyun-klasoru.ps1 -Coz`
- Junction yoksa geçici kopya: `tools\sync-game-to-github.ps1`
- Launcher’da proje yoksa: **+ Add Project** → `addons\M360-Life\addon.gproj`

`resourceDatabase.rdb` gitignore’da (yerel cache).

## Monorepo

```
M360-Life/
├── m360-life/     ← oyun (junction hedefi)
├── api/           ← Vercel Root Directory: api
├── web/           ← admin (sonra)
├── packages/db/   ← Neon SQL
├── docs/
└── tools/
```

## Sistem nasıl konuşur (özet)

1. **Geliştirme:** Cursor monorepo’da `api/` + `docs/`; Workbench junction’daki oyunu düzenler.
2. **Kalıcılık:** Oyun parametreleri / ekonomi → Neon. API Vercel’de Neon’a bağlanır (`DATABASE_URL`).
3. **Oyun → API:** Enforce `RestApi` / `RestContext` → `https://m360-life.vercel.app` + header `X-M360-Server-Key`.
4. **İnceleme:** Tarayıcı lab UI (`/`) key ile jobs/health/metrik; istatistik `/istatistik`.
5. **Güvenlik:** Anahtarsız jobs/metrik = 401. Health açık (uptime). Detay [19](./19_guvenlik.md).

Üretim hedefi: HTTP’yi **dedicated** atar; oyuncu istemcisi sahte fiyat gönderemez — sunucu DB’den okur.

## Geliştirme sırası (kesin — 2026-07-28)

1. **LabDuzZemin** — sistem/prefab burada  
2. **Dedicated** — `tools/dedicated/` taşınabilir paket; MP doğrulama burada  
3. **Harita / Everon** — lab + dedicated sonrası

Prefab’lar haritadan bağımsızdır; lab’da kanıt → dedicated MP → Everon.

## Bulut

| Bileşen | Nerede |
|---|---|
| PostgreSQL | Neon (yerel PG yok) |
| API | Vercel → `https://m360-life.vercel.app` |
| Lab UI | `/` (katalog + tablolar) |
| Metrik | `/istatistik` |
| Güvenlik | [docs/19](./19_guvenlik.md) |
| Mimari diyagram | [docs/11 §11.1b](./11_teknik_mimari.md) |

## Günlük akış

1. Workbench → `addons\M360-Life` (junction = git klasörü)
2. Cursor → `Documents\GitHub\M360-Life` (`api/`, `docs/`)
3. Oyun değişince `git add m360-life` (junction ile kopya yok)
4. API değişince push → Vercel otomatik deploy
