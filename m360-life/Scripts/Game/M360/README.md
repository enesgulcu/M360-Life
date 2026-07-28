# Scripts/Game/M360 — klasor haritasi (buyumeye acik)

Turkce ASCII klasor adlari. Motor API ingilizce kalir.

| Klasor | Rol | Durum |
|---|---|---|
| `Ag/` | Bulut API istemcisi (Vercel) | canli |
| `Arayuz/` | HUD, canta UI, I/Tab | canli |
| `Isler/` | Is siteleri + aksiyonlar + ayar | canli |
| `Lab/` | Lab hava/saat | canli |
| `Envanter/` | Oyuncu cantasi / oturum (ileride) | iskelet — veri simdi `Isler/M360_IsOturumlari.c` |
| `Ekonomi/` | Para, pazar (ileride) | iskelet |
| `Araclar/` | Arac sistemleri (ileride) | iskelet |
| `Klanlar/` | Klan (ileride) | iskelet |
| `Roller/` | Polis/doktor vb. (ileride) | iskelet |
| `Cekirdek/` | Ortak yardimci (ileride) | iskelet |

**Kural:** Yeni ozellik ilgili klasore. `Isler` icine para/envanter sikistirma — `Envanter` / `Ekonomi` kullan.
**Prefab hizasi:** `Prefabs/M360/{Isler,Araclar,Pazarlar,Arayuz,Dunya}` ayni dil.
**Ag:** tek ag katmani. `Net` klasoru yok.
