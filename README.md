# M360 Life

Arma Reforger için **modüler Life ekosistemi** — ekonomi, işler, roller, araçlar, klan ve yönetim aynı veri/yetki omurgasında.

Sunucu yetkili, panelden ayarlanabilir, harita bağımsız **Life platformu**. İlk kanıt yolu: lab harita → dedicated → Neon/API → sonra Everon yerleştirme.

---

## Şu an nerede?

| Katman | Durum |
|---|---|
| Tasarım (`docs/00`–`14`) | Tamam — canlı referans |
| Lab bulguları (`docs/15`) | Canlı — kanıtlar buraya |
| Pirinç dilim (topla → işle → sat) | Play OK (oturum stub) |
| HUD lab | CreateWidget circle HUD + I çanta; `.layout` diskte (runtime CreateWidgets yasak) |
| API + DB | **Canlı:** Neon + Vercel `https://m360-life.vercel.app` |
| Oyun → API | Play RestApi health/jobs **kanıtlı** |
| Dedicated | Henüz — sıradaki kapı |

**Lab ≠ ürün.** Lab’da para/envanter stub vardır. Ürün: dedicated + ledger + replication + admin.

---

## Geliştirme sırası (kesin)

1. **LabDuzZemin** — tüm prefab/sistem burada bitirilir ve doğrulanır  
2. **Dedicated (yerel, taşınabilir paket)** — cihaz değişince taşınır; modüler  
3. **Harita seçimi / Everon yerleştirme** — lab + dedicated bittikten **sonra**

Detay: [docs/18](docs/18_calisma_duzeni.md) · mimari [docs/11.1b](docs/11_teknik_mimari.md)

---

## İki disk yolu + monorepo

| Disk yolu | Rol |
|---|---|
| `Documents\GitHub\M360-Life` | **Git + api + web + docs** — Cursor |
| `...\addons\M360-Life` | Workbench (**junction** → `m360-life/`) |
| `...\GitHub\M360-Life\m360-life` | Oyun kaynakları (git) |

Repo: `m360-life/` · `api/` · `web/` · `packages/db/` · `docs/`

İki PC / kopyasız senkron: `tools/pc-hazirla.ps1` ([docs/18](docs/18_calisma_duzeni.md)).  
API: Vercel + Neon. Güvenlik: [docs/19](docs/19_guvenlik.md). Metrik: `/istatistik`.

Remote: https://github.com/enesgulcu/M360-Life

---

## Yeni sohbette okuma sırası

`docs/00` → konu dokümanı → **`docs/15`** + **`docs/16`** → `docs/18`/`19`. Konuşma geçmişi kaynak değildir.

| # | Dosya | Not |
|---|---|---|
| **00** | [00_ana_dokuman.md](docs/00_ana_dokuman.md) | Kararlar |
| 01–14 | `docs/` | Tasarım |
| **15** | [15_gelistirme_notlari.md](docs/15_gelistirme_notlari.md) | Kanıt / tuzak |
| **16** | [16_dosya_envanteri.md](docs/16_dosya_envanteri.md) | Dosya kataloğu |
| **17** | [17_yerel_kurulum.md](docs/17_yerel_kurulum.md) | Neon + Vercel |
| **18** | [18_calisma_duzeni.md](docs/18_calisma_duzeni.md) | Junction + iletişim |
| **19** | [19_guvenlik.md](docs/19_guvenlik.md) | Rate limit / anti-manipülasyon |

---

## Bilinen lab sınırları

1. Para/envanter = stub (`M360_IsOturumlari`), sunucu ledger değil.  
2. `.layout` dosyaları diskte; runtime CreateWidgets yasak (docs 15).  
3. Alet şartı (docs 5) lab’da henüz yok.  
4. Dedicated henüz çalıştırılmadı.

---

## Repo iskeleti

```
M360-Life/
├── m360-life/          ← oyun
├── api/                ← Vercel
├── web/                ← admin (sonra)
├── packages/db/        ← Neon
├── docs/               ← 00–19
└── tools/
```

---

## Sonraki kapılar (sıra)

1. Dedicated lab (yerel + taşınabilir paket)  
2. Gerçek envanter / banka ledger  
3. Life HUD ürün yolu (layout editor)  
4. Admin panel (`web/`)  
5. Everon / harita yerleştirme (en sonda)

Roadmap: [docs/12](docs/12_lisans_roadmap.md).

---

## Kod stili (ANA DÜSTUR)

Değiştirilebilir her şey **Türkçe ASCII**. Motor API İngilizce dokunulmaz. [docs/11.2.1](docs/11_teknik_mimari.md).

---

## Lisans / dağıtım niyeti

Kapalı kaynak; ileride **yıllık sunucu lisansı**. Fiyat bilinçli açık (docs 12.2).

---

*Marka geçici: Medyanes 360 / M360 Life. Ürün haritası Everon — yerleştirme lab+dedicated sonrası.*
