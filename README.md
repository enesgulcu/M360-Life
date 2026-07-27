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
| Dedicated / Next.js / PostgreSQL | Henüz yok — sıradaki büyük kapı |

**Lab ≠ ürün.** Lab’da para/envanter stub (`M360_IsOturumlari`), yerel action, hint UI vardır. Ürün hedefi: sunucu yetkili ledger, gerçek envanter, replication, admin panel.

---

## İki klasör (önemli)

| Yol | Rol |
|---|---|
| `...\ArmaReforgerWorkbench\addons\M360 Life` | **Günlük çalışma** (Workbench burayı açar) |
| `Documents\GitHub\M360-Life` | **Git / GitHub kopyası** (bu repo) |

Workbench klasörü git repo değildir. Push öncesi Workbench → bu klasöre senkron, sonra `commit` + `push`. Kaybolmaması gereken her şey GitHub’da olmalı.

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
| **15** | [15_gelistirme_notlari.md](docs/15_gelistirme_notlari.md) | **AI oturum belleği — kanıtlar, tuzaklar** |
| **16** | [16_dosya_envanteri.md](docs/16_dosya_envanteri.md) | **Dosya ilişki kataloğu — her eklemede güncelle** |

Konuşma geçmişi kaynak değildir. Güncel gerçek: bu dökümanlar + kod.

---

## Çalışma protokolü

- Kullanıcı **yönetici**; AI teknik ortak/rehber.
- Kod yazmayı kullanıcı öğrenmez; AI yazar, Türkçe adım adım rehber verir.
- Açık Workbench varken **yeni Workbench açılmaz** (`wb_launch` yasak) → `wb_connect`.
- Enfusion/Reforger’da varsayım yasak → resmi BI wiki / API.
- **EveronLife** = fikir kaynağı, **asla dependency değil**.
- Docker yok → PostgreSQL native.
- Kod isimleri: **Türkçe anlam, ASCII** (`m_iNakit`, `ToplamaBaslat`); yorumlar Türkçe. Detay: docs 11.2.1 / 15-A.

---

## Pirinç lab (dikey dilim)

```
Topla (çuval) → İşle (tezgâh) → Sat (kasiyer)
```

| Parça | Konum |
|---|---|
| Script | `Scripts/Game/M360/Jobs/` |
| Prefab | `Prefabs/M360/Jobs/M360_Job{Collect,Process,Sell}_Pirinc.et` |
| HUD | `Scripts/Game/M360/UI/M360_CantaHudBileseni.c` |
| Test dünya | `Worlds/TestWorld/M360_TestWorld.ent` |

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
├── README.md                 ← buradasın
├── addon.gproj
├── docs/                     ← 00–15 tasarım + lab belleği
├── Scripts/Game/M360/        ← ürün scriptleri (Jobs, UI, …)
├── Scripts/WorkbenchGame/    ← enfusion-mcp Workbench handler’ları
├── Prefabs/M360/Jobs/        ← Pirinç site prefab’ları
├── Configs/                  ← input vb.
└── Worlds/TestWorld/         ← lab dünyası + terrain
```

---

## Sonraki kapılar (sıra)

1. Dedicated server ilk çalıştırma  
2. PostgreSQL native + Next.js iskelet  
3. Gerçek envanter / banka ledger (stub’ı değiştir)  
4. `.layout` Life HUD  
5. Admin panel ile iş parametresi override  

Roadmap: [docs/12](docs/12_lisans_roadmap.md).

---

## Kod stili (ANA DÜSTUR)

Değiştirilebilir her şey **Türkçe ASCII** (class, metod, üye, Attribute, oyuncu metni). Motor API/override İngilizce dokunulmaz. Detay: [docs/11.2.1](docs/11_teknik_mimari.md).

---

## Lisans / dağıtım niyeti

Kapalı kaynak; ileride **yıllık sunucu lisansı**. Ticari fiyat bilinçli açık (docs 12.2). Bu repo şu an stüdyo geliştirme deposudur — public fork/dağıtım varsayma.

---

*Marka adı geçici: Medyanes 360 / M360 Life. İlk harita: Everon (kurgusal Türkiye teması).*
