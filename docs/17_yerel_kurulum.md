# Döküman 17 — Yerel tek cihaz kurulum (PostgreSQL + Next.js + Dedicated)

*Bağlı: [11](./11_teknik_mimari.md) · [12](./12_lisans_roadmap.md) · [16](./16_dosya_envanteri.md)*

> Bu rehber **senin PC’nde** adım adım. Docker yok. AI komutları yazar; sen kurulum sihirbazını tıklarsın.

---

## 0. Hedef düzen

```
Workbench:  ...\addons\M360 Life          ← Enfusion mod (Play / edit)
GitHub:     Documents\GitHub\M360-Life
              Scripts/ Prefabs/ Worlds/ … ← addon (sync script ile)
              apps/game-api               ← Next.js API :3100
              apps/admin-web              ← sonra
              packages/db                 ← SQL migration
```

Senkron: `tools/sync-addon-to-github.ps1` — **MIR kullanma** (apps silinir).

---

## 1. PostgreSQL native (Faz D)

### 1.1 İndir / kur
1. https://www.postgresql.org/download/windows/ → installer (önerilen güncel 16/17).
2. Kurulumda:
   - Port **5432**
   - Superuser `postgres` + **güçlü şifre** (kaydet)
   - “Stack Builder” atlanabilir
3. Kurulum bitince Windows servisi `postgresql-x64-…` Running olmalı.

### 1.2 DB ve kullanıcı
**SQL Shell (psql)** veya pgAdmin → postgres ile bağlan:

```sql
CREATE USER m360 WITH PASSWORD 'm360_lab';
CREATE DATABASE m360_life OWNER m360;
GRANT ALL PRIVILEGES ON DATABASE m360_life TO m360;
```

> Lab şifresi `m360_lab` — sadece yerel. Üretimde değiştir.

### 1.3 Migration
PowerShell:

```bat
cd C:\Users\enesg\Documents\GitHub\M360-Life\packages\db
psql -U m360 -d m360_life -f migrations\001_job_definitions.sql
```

Başarı: `CREATE TABLE` / `INSERT` görürsün. Kontrol:

```sql
SELECT is_adi, satis_fiyati, iptal_mesafesi FROM job_definitions;
```

### 1.4 Doğrulama
- [ ] Servis çalışıyor
- [ ] `m360_life` var
- [ ] `job_definitions` satırında Pirinc var

---

## 2. Next.js game-api (Faz E)

```bat
cd C:\Users\enesg\Documents\GitHub\M360-Life\apps\game-api
copy .env.example .env
npm install
npm run dev
```

Tarayıcı:
- http://localhost:3100/api/health → `{ "ok": true, ... }`
- http://localhost:3100/api/jobs → PG bağlıysa Pirinc satırı; değilse `env-yok` / hata mesajı (çökmez)

---

## 3. Dedicated sunucu — ilk ayağa kaldırma (Faz F)

Resmi yol (özet; BI sürümüne göre menü adı değişebilir):

1. Steam → Arma Reforger → **Dedicated Server** aracı / ayrı kurulum (varsa).
2. Alternatif: Reforger Workbench / oyun klasöründe `ArmaReforgerServer.exe` (kurulum yoluna göre).
3. Minimal `server.json` / profile:
   - scenario / world: lab için önce BI FFA veya senin `M360_LabDuzZemin` (addon yüklü olmalı)
   - mods: **M360 Life** addon GUID/path
   - maxPlayers düşük (lab 4–8)
4. Sunucuyu başlat → log’da “listening” / port (varsayılan genelde 2001 UDP/TCP — sürüme bak).
5. İstemci: aynı PC’den LAN / localhost ile bağlan.

**İlk milestone (bu faz):** istemci dedicated’a girer, dünya görünür. Ekonomi API köprüsü **sonra**.

Detaylı JSON alanları BI wiki: `Arma_Reforger:Server_Hosting` / Server Config. Belirsizlikte AI’ye log yapıştır.

### Güvenlik lab notu
- Firewall’da sadece lab portunu aç.
- İnternete port açma (şimdilik).

---

## 4. Play test — mesafe iptali (Faz B kontrol)

1. Workbench → `Worlds/LabDuzZemin/M360_LabDuzZemin.ent` aç.
2. Play → çuvala F → Topla.
3. **Uzaklaş** (~5 m+; `m_fIptalMesafesi` = 4) → ipucu: toplama iptal.
4. Tezgâhta İşle → uzaklaş → isleme iptal, **ham korunur**.

---

## 5. Sıra özeti

| Sıra | Sen | AI |
|---|---|---|
| 1 | PG kur + migration | Rehber (bu dosya) |
| 2 | `npm install` + `npm run dev` | Kod iskeleti hazır |
| 3 | Dedicated ilk deneme | Log okuma / config düzeltme |
| 4 | Play mesafe testi | Script düzeltme |

---
*Önceki: [16](./16_dosya_envanteri.md)*
