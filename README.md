# M360 Life

Arma Reforger için **modüler Life ekosistemi** — ekonomi, işler, roller, araçlar, klan ve yönetim aynı veri/yetki omurgasında.

Bu repo yalnızca “topla-sat eklentisi” değil; sunucu yetkili, panelden ayarlanabilir, harita bağımsız bir **Life platformu** hedefidir. İlk kanıt: yerel dedicated + PostgreSQL ile iş yap → sat → araç al → restart sonrası korunsun.

---

## Şu an nerede?

| Katman | Durum |
|---|---|
| Tasarım (`docs/00`–`14`) | Tamam — canlı referans |
| Lab bulguları (`docs/15`) | Canlı — kanıtlanmış tuzaklar buraya yazılır |
| Pirinç dikey dilim (topla → işle → sat) | Workbench Play’de çalışıyor (oturum stub) |
| HUD lab | **I** = M360 çanta hint; yüzde sayacı; gerçek `.layout` HUD henüz yok |
| API + PostgreSQL | Neon (hosted PG) + `api/` → Vercel (kurulum aşaması) |

**Lab ≠ ürün.** Lab’da para/envanter stub (`M360_IsOturumlari`), yerel action, hint UI vardır. Ürün hedefi: sunucu yetkili ledger, gerçek envanter, replication, admin panel.

---

## Repo düzeni (aynı hizada)

| Yol | Rol |
|---|---|
| `m360-life/` | Enfusion addon — Workbench bunu açar |
| `api/` | Next.js game-api → Vercel Root Directory: `api` |
| `web/` | Admin panel (sonraki faz) |
| `packages/db/` | Neon migration SQL |
| `docs/` | Tasarım + lab |

**Yerelde sadece oyun** (`m360-life/`). API ve DB bulutta (Vercel + Neon).

Remote: https://github.com/enesgulcu/M360-Life

---

## Doküman haritası

Yeni sohbette okuma sırası: **`docs/00` → ilgili konu → `docs/15` + `docs/16`**.

| # | Dosya | Konu |
|---|---|---|
| 0 | [00_ana_dokuman.md](docs/00_ana_dokuman.md) | Vizyon, ilkeler, indeks |
| 1 | [01_dunya_harita_icerik.md](docs/01_dunya_harita_icerik.md) | Harita / içerik |
| 2 | [02_hesap_karakter_rol.md](docs/02_hesap_karakter_rol.md) | Hesap / karakter / rol |
| 3 | [03_yeni_oyuncu_ilerleme.md](docs/03_yeni_oyuncu_ilerleme.md) | Yeni oyuncu / ilerleme |
| 4 | [04_ekonomi_piyasa.md](docs/04_ekonomi_piyasa.md) | Ekonomi / piyasa |
| 5 | [05_isler_envanter.md](docs/05_isler_envanter.md) | İşler / envanter |
| 6–9 | `06`…`09` | Sağlık, araç, roller, klan |
| 10 | [10_panel_hud_admin.md](docs/10_panel_hud_admin.md) | Panel / HUD / admin |
| 11 | [11_teknik_mimari.md](docs/11_teknik_mimari.md) | Teknik mimari |
| 12 | [12_lisans_roadmap.md](docs/12_lisans_roadmap.md) | Lisans / roadmap |
| 13 | [13_enfusion_prefab_prosedur.md](docs/13_enfusion_prefab_prosedur.md) | Prefab / script — **üç site** (Collect/Process/Sell) |
| 14 | [14_mevcut_modlar_araclar.md](docs/14_mevcut_modlar_araclar.md) | Araçlar / referans; enfusion-mcp = Cursor |
| **15** | [15_gelistirme_notlari.md](docs/15_gelistirme_notlari.md) | Kanıt / tuzak |
| **16** | [16_dosya_envanteri.md](docs/16_dosya_envanteri.md) | Dosya ilişki kataloğu |
| **17** | [17_yerel_kurulum.md](docs/17_yerel_kurulum.md) | Yerel PG + Next + Dedicated rehber |

Konuşma geçmişi kaynak değildir. Güncel gerçek: bu dökümanlar + kod.

---

## Çalışma protokolü

- Kullanıcı **yönetici**; AI teknik ortak/rehber.
- Kod yazmayı kullanıcı öğrenmez; AI yazar, Türkçe adım adım rehber verir.
- Açık Workbench varken **yeni Workbench açılmaz** (`wb_launch` yasak) → `wb_connect`.
- Enfusion/Reforger’da varsayım yasak → resmi BI wiki / API.
- **EveronLife** = fikir kaynağı, **asla dependency değil**.
- Docker yok → PostgreSQL **Neon** (hosted); yerel PG yok.
- Kod isimleri: **Türkçe anlam, ASCII** (`m_iNakit`, `ToplamaBaslat`); yorumlar Türkçe. Detay: docs 11.2.1 / 15-A.

---

## Pirinç lab (dikey dilim)

```
Topla (çuval) → İşle (tezgâh) → Sat (kasiyer)
```

| Parça | Konum |
|---|---|
| Script | `m360-life/Scripts/Game/M360/Isler/` + `Arayuz/` |
| Prefab | `m360-life/Prefabs/M360/Isler/Pirinc/M360_{Topla,Isle,Sat}_Pirinc.et` |
| HUD | `m360-life/Scripts/Game/M360/Arayuz/M360_CantaHudBileseni.c` |
| Lab dünya | `m360-life/Worlds/LabDuzZemin/M360_LabDuzZemin.ent` |

### Lab sayıları (prefab Attribute — Play’de bunlar geçerli)

| Parametre | Lab | Üretim hedefi (docs 5.7) |
|---|---|---|
| Tick süresi | 4 sn | ~4 sn |
| Tick verim | 2 | ~2–3 |
| Batch | **20** | **100** |
| İşleme süresi | **10 sn** | **~180 sn** |
| Fiyat / birim | 600 | 600 |
| Max ham taşıma | 40 | kapasite puanı (ürün) |
| ~Sefer kazancı | **12.000** (20×600) | **60.000** (100×600) |

Lab bilerek hızlı; denge sayıları docs’taki üretim hedefidir. Karıştırma.

### Scriptsiz iş ayarı

Yeni iş (ör. Gümüş) için script gerekmez:

1. Üç prefabı kopyala (Collect / Process / Sell)
2. `m_Ayar` (`M360_IsAyar`) alanlarını doldur: isim, adim, verim, parti, süre, oran, fiyat, max taşıma
3. F menü `UIInfo.Name` + isteğe bağlı model (parent prefab)
4. Dünyaya yerleştir

Aynı işin dengesi: World Editor’da entity seç → Object Properties → `m_Ayar` → kaydet.

İleride: aynı parametreler admin panel + `job_definitions` (PostgreSQL) — docs 5.1b / 13.4.

---

## HUD & tuşlar (lab)

| Tuş | Sonuç |
|---|---|
| **I** | M360 çanta (hint) — `Debug.KeyState(KC_I)` |
| **Tab** | BI native Inventory — M360’ye bağlı değil |
| F + scroll | Site UserAction (Topla / İşle / Sat / Durum) |

Ürün HUD yolu: **`.layout` + ikon (.edds) + script** — HTML değil. Detay: [docs/10.8](docs/10_panel_hud_admin.md) + [docs/15 §7b–7c](docs/15_gelistirme_notlari.md).

---

## Bilinen ayrımlar (lab vs ürün — bilinçli)

Bunlar hata değil; karıştırılmaması gereken katmanlar:

1. **Lab sefer ~12k** (batch 20) vs **üretim 60k** (batch 100) — tek tablo: [docs/05 §5.7b](docs/05_isler_envanter.md).
2. **Sefer** = ekonomik hedef batch; kapasite 100 alan ile 300 alanlık ham yük **araç/çok tur** ister (5.7a).
3. **Alet şartı** (docs 5) lab’da henüz yok.
4. **HUD:** lab = hint + I; ürün = `.layout` (docs 10.8).
5. Doc 13 artık üç site; eski tek-istasyon = tarihçe.

Detay: [docs/15](docs/15_gelistirme_notlari.md).

---

## Klasör özeti

```
M360-Life/
├── m360-life/          ← oyun (Scripts, Prefabs, Worlds, …)
├── api/                ← Next.js → Vercel
├── web/                ← admin panel (sonra)
├── packages/db/        ← Neon migration
├── docs/               ← 00–17
└── tools/
```

---

## Sonraki kapılar (sıra)

1. Neon + Vercel (`api`) deploy  
2. Dedicated server ilk çalıştırma  
3. Gerçek envanter / banka ledger (stub’ı değiştir)  
4. `.layout` Life HUD  
5. Admin panel (`web/`) ile iş parametresi override  

Roadmap: [docs/12](docs/12_lisans_roadmap.md).

---

## Kod stili (ANA DÜSTUR)

Değiştirilebilir her şey **Türkçe ASCII** (class, metod, üye, Attribute, oyuncu metni). Motor API/override İngilizce dokunulmaz. Detay: [docs/11.2.1](docs/11_teknik_mimari.md).

---

## Lisans / dağıtım niyeti

Kapalı kaynak; ileride **yıllık sunucu lisansı**. Ticari fiyat bilinçli açık (docs 12.2). Bu repo şu an stüdyo geliştirme deposudur — public fork/dağıtım varsayma.

---

*Marka adı geçici: Medyanes 360 / M360 Life. İlk harita: Everon (kurgusal Türkiye teması).*
