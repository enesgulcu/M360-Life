# Döküman 10 — Oyuncu Paneli, HUD & Admin Panel

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 10.1 Ana Oyuncu Paneli

| Modül | İşlev |
|---|---|
| Profil | Kimlik, toplam süre, aktif rol, koruma süresi |
| Finans | Banka, para transferi, işlem geçmişi, vergi/sigorta durumu |
| Lisanslar | Sürücü, işleme, silah, rebel ve rol lisansları |
| Varlıklar | Araç, kiralık araç, ev, garaj ve sigorta |
| İşler / piyasa | İş konumları, satış fiyatları ve piyasa yönü |
| Çağrı | Polis, doktor ve admin destek mesajları |
| Mesajlaşma | Oyuncuya özel mesaj, klan ve takım iletişimi |
| Hukuk | Yakalama emri, ceza, kamu görevi ve hapis bilgisi |
| Ayarlar | Rol profili (bkz. Döküman 8.6), bildirim ve korumadan çıkış |

## 10.2 Polis ve Doktor Operasyon Ekranı

- Gelen çağrılar haritada konumsal işaretlenir.
- Bir doktor veya polis görevi kabul ettiğinde diğer personel kimin gittiğini görür.
- Aynı olaya gereksiz ekip yığılması azaltılır.
- Polis tarafında arama emri, devriye, olay ve operasyon; doktor tarafında hasta, rota ve müdahale durumu gösterilir.
- Rütbeye göre farklı buton ve veri erişimi uygulanır.

## 10.3 Sohbet ve Ses Kanalları (onaylandı — hepsi MVP'de)

| Kanal | Durum |
|---|---|
| Yakınlık sesi | Var |
| Araç içi ses | Var |
| Takım sesi/yazısı | Var |
| Klan sesi/yazısı | Var |
| Polis telsizi | Var |
| Doktor telsizi | Var |
| Global yazı | Var; ayrı filtre ve rate limit |
| Ticaret ilanları | Ayrı kanal |
| Global ses | Yok |
| Sistem duyuruları | Salt okunur |

## 10.4 Killfeed ve Olay Akışı (onaylandı — rol bilgisi açık)

Ekranın bir köşesinde öldüren, ölen, silah/ölüm nedeni, mesafe, tarih ve saat bilgisi gösterilir. Aynı olay ayrıntılı sunucu loguna yazılır. **Karar: rol bilgisi açık gösterilecek** — proje tercihi kanıt ve takip önceliği. Admin panelinde görünür alanlar değiştirilebilir (yani ileride topluluk geri bildirimiyle gizlenebilir, ama MVP varsayılanı açık).

## 10.5 Admin Paneli — Panelden Yönetilecek Başlıca Alanlar

| Grup | Örnek parametreler |
|---|---|
| Market ve ürün | Fiyat, stok, lisans, süre, rol, indirim/zam, trade alt/üst oranı |
| İşler | Toplama/işleme süresi, item, satış yeri, ödeme tipi, ihbar oranı |
| Ekonomi | Vergi, servet kademesi, sigorta, nüfus katsayısı, ilişki matrisi |
| Roller | Kota, ihtiyaç eşiği, haftalık sivil süre, rütbe, maaş ve ekipman |
| Harita | Bölge türü, spawn, RDM/KOS, klan noktası ve kira süresi |
| Güvenlik | Rate limit, cooldown, transfer limiti, claim sınırı |
| İçerik | Prefab/item kayıtları, ülke paketi, araç ve kıyafet katalogları |
| Operasyon | Bildirim, bakım modu, wipe, lisans, sunucu ve sürüm yönetimi |

## 10.6 Kural Önceliği

1. Global varsayılan kural
2. Sunucu/ülke paketi kuralı
3. Market veya bölge kuralı
4. Rol veya item kategorisi kuralı
5. Paket/oyun süresi/etkinlik kuralı
6. Tarih-saat kuralı
7. Yetkili admin geçici override

Birden fazla kural çakıştığında öncelik ve birleştirme biçimi (sabit değer, oran ekleme, oran çarpma, minimum/maksimum) açıkça gösterilir. Panel **"Bu değişiklik 42 itemi, 6 marketi ve 3 sigorta formülünü etkiler"** gibi etki özeti üretmelidir — bu, admin panelinin en kritik UX kararlarından biri: yanlışlıkla ekonomiyi bozan bir değişiklik yapılmasını önler.

## 10.7 Admin Rolleri ve Yaptırımlar

Sistem sahibi admin seviyelerini, her seviyenin görebileceği ekranları ve çalıştırabileceği işlemleri tanımlar. Ban süreleri 3/5/10/20 saat, 2/5/10/30 gün, süresiz veya özel süre olarak uygulanabilir. Uyarı, para cezası, kamu görevi, rol yasağı, silah yasağı, hapis ve hesap dondurma ayrı yaptırım türleridir.

## 10.8 Oyun İçi HUD — Teknik Yol (2026-07-27 netleşti)

> **Hedef:** Narcos / Life tarzı ekran kenarı HUD’ları (para, vitals, tuş ipuçları, mermi, sunucu şeridi vb.). Birden fazla noktada HUD ihtiyacı olacak; hepsi aynı teknik omurgayı kullanır.

### Ne kullanılır / ne kullanılmaz

| Yol | Oyun içi HUD? | Not |
|---|---|---|
| **`.layout` + Layout Editor** | **Evet — resmi yol** | Display (bilgi, tıklanmaz) / Menu / Dialog ayrımı |
| **Texture / ikon (`.edds`)** | Evet | ImageWidget; bar ve daireler çoğunlukla texture + dolgu/mask |
| **Enforce script** | Evet | `CreateWidgets` + `FindAnyWidget`; sayıları günceller |
| **SCR_Hint** | Geçici lab | Hızlı feedback; kalıcı Life HUD değil |
| **HTML / React / WebView** | **Hayır** (oyun HUD) | Admin panel / web = Next.js (Döküman 11) — ayrı katman |

### Ekran yerleşimi (Life HUD kalıbı)

- Root layout tam ekran; paneller **anchor** ile köşe/kenara sabitlenir (sol üst para, sol alt vitals, sağ alt silah, üst şerit…).
- İstersen her panel ayrı `.layout` → root’a gömülür (modüler).
- Widget isimleri script için sabit (`m_wNakit`, `m_wCan`…) — “Generate Class from Layout” kullanılabilir.
- BI referans: `Arma_Reforger:Layout_Creation` (Display = HUD), Layout Editor, StatsPanel örneği.

### Entegrasyon sırası (ürün)

1. AI / tasarım: mockup (köşe yerleşimi + ikon listesi).
2. Workbench: `UI/layouts/M360/...layout` + ikon asset.
3. Script: layout yükle → veriyi bağla (nakit, iş ilerlemesi, rol…).
4. İleri: `SCR_InfoDisplay` / HUD manager kaydı (BI HUD parçaları gibi).

### Lab durumu (şimdi)

- **Lab HUD v9 + Canta v3 soft9:** Status rings + oval nakit pill; I canta = 9-slice oval panel, ağırlık barı, ikonlu dinamik satırlar (Kapat yok). `CreateWidget` only — `CreateWidgets` yasak. Çıkarımlar: `docs/15` §7c–7d + `.cursor/rules/m360-reforger-hud.mdc`.

### Lab notu (kamera)
- Enter 3. şahıs zoom/kayma labda görüldü → M360 kamera kodu yok; lab testte 1. şahıs tercih et (docs/15 §7d).

### İhtiyaç duyulacak HUD noktaları (ürün — örnek)

| Nokta | Örnek içerik |
|---|---|
| Her zaman açık (çekirdek) | Nakit/banka, can/açlık, rol, sunucu/saat |
| İş | Toplama/işleme 0–100 bar, taşıma |
| Envanter / çanta | Life liste paneli (I) |
| Polis / doktor | Çağrı, durum, özel aksiyon ipuçları |
| Araç | Hız, yakıt, kilit/sahiplik özeti |
| Killfeed / olay | Köşe akışı (10.4) |

Hepsi aynı omurga: **layout + ikon + script**; HTML değil.

---
*Önceki: [09 - Klan](./09_klan.md) · Sıradaki: Döküman 11 — Teknik Mimari (Arma Reforger + Web)*
*HUD teknik notları: [15](./15_gelistirme_notlari.md) Bölüm 7c*
