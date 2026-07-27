# Döküman 3 — Yeni Oyuncu Deneyimi & İlerleme Motoru

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 3.1 Yeni Oyuncu Koruması

Koruma süresi **20 aktif oyun saati** boyunca devam eder (varsayılan — panelden değiştirilebilir; Döküman 3.3'teki "Yeni Oyuncu" kademe sınırıyla bilinçli olarak eşitlendi, böylece koruma ve içerik kademesi birlikte biter). Süre yalnızca aktif kabul edilen oturum dakikalarında azalır. Günde dört saat oynayan bir oyuncu yaklaşık beş günde korumayı tamamlar. Oyuncu panelden korumayı erken kapatabilir; bu karar geri alınamaz.

| Koruma alanı | Uygulama |
|---|---|
| Ağır kayıp | Ölümde başlangıç dönemi eşyaları ve ana envanter korunur veya yalnızca oyuncunun erişebildiği şekilde bırakılır |
| Soygun | Korunan oyuncu soyulamaz ve klan/ekip için güvenli taşıyıcı olarak kullanılamaz |
| Rebel erişimi | Koruma boyunca rebel lisansı, ağır yasa dışı iş ve ağır silah erişimi kapalıdır |
| Çevresel ölüm | Kaza, düşme vb. olaylarda ölüm yerine baygınlık tercih edilebilir; kurşunla öldürülebilir |
| Kötüye kullanım | Korunan oyuncu saldırı başlatırsa olay süresince korumanın ilgili kısmı devre dışı kalır |

**Koruma amacı:** Oyuncuya üstünlük vermek değil, sunucuyu öğrenirken ağır ve geri döndürülemez kayıplar yaşamasını engellemektir.

## 3.2 Oynayarak Öğrenme Görevleri

Zorunlu değildir; tamamlandıkça küçük, ekonomiyi bozmayacak ödüller sağlar. Amaç oyuncuya *okuyarak değil, sistemleri kullanarak* öğretmektir.

1. Telefon/HUD panelini aç ve profil bilgilerini gör.
2. Belediyeden sürücü lisansının koşullarını incele.
3. ATM kullan ve banka hesabını görüntüle.
4. Marketten su ve yiyecek satın al.
5. Kiralık araç merkezini ziyaret et.
6. İlk güvenli işi başlat, ürün topla, işle ve sat.
7. Polis ve doktor çağrısı ekranlarını deneme modunda gör.
8. Harita üzerindeki güvenli, riskli ve RDM bölge açıklamalarını aç.

## 3.3 İlerleme Sistemi — 4 Kademeli Model (sadeleştirildi)

Genel bir "Level 35" göstergesi **yoktur**. Toplam aktif oyun süresi, para, lisans, rol, konum, zaman, kapasite ve item politikasıyla birlikte erişim kararına girer. Orijinal tasarımdaki 6 kademe, yönetim kolaylığı için **4 kademeye** indirildi:

| Kademe | Aktif süre | İçerik açılımı |
|---|---|---|
| **Yeni Oyuncu** | 0–20 saat | Güvenli işler, kiralık araç, temel market, rehber ve koruma (bkz. 3.1) |
| **Yerleşik** | 20–150 saat | Standart araçlar, çoğu yasal iş, temel silah ruhsatları, temel ev hedefi, polis/doktor kariyerine giriş, riskli işler |
| **Gelişmiş** | 150–500 saat | Nadir lisanslar, güçlü araçlar, klan liderliği, büyük operasyonlar |
| **Kıdemli** | 500+ saat | Yüksek riskli ekonomi, ileri rütbeler, prestij hedefleri, mentorluk, koleksiyon, uzun vadeli topluluk hedefleri |

**70/30 kuralı korunuyor:** İçeriğin yaklaşık %70'i "Gelişmiş" kademeye kadar açılır; son %30 nadir, güçlü ve yönetim etkisi yüksek içeriklerden oluşur (Kıdemli kademe).

> Not: Saat eşikleri (20/150/500) başlangıç varsayımı — sunucu doldukça ve oyuncu davranışı gözlemlendikçe panelden ayarlanacak. Kesin sayı değil, oran mantığı önemli.

## 3.4 Genel Satın Alma / Kullanım Kontrol Matrisi

Her satın alma veya kullanım isteği bu kontrollerden geçer:

| Kontrol | Örnek |
|---|---|
| Para | Banka/nakit bakiyesi yeterli mi? |
| Aktif oyun süresi | Ağır araç için minimum süre doldu mu? |
| Lisans | Sürücü, ağır vasıta, silah veya işleme lisansı var mı? |

### Lisans Sistemi — Kalıcılık Kuralı (bu oturumda netleşti)

**Bir lisans bir kez satın alındığında, bir sonraki global wipe'a kadar (bkz. Döküman 4.7) geçerlidir.** Yenileme, aylık ödeme veya süre dolması yoktur — "aldın mı, bitti" mantığı. Wipe olduğunda tüm lisanslar sıfırlanır (Döküman 4.7'deki "aktif ekonomik güç" sıfırlanan kalemler listesiyle tutarlı).

Her lisansın **kendine özgü, değişken bir fiyatı** vardır — işin/aracın/rolün değerine göre ölçeklenir. İlk örnekler (Döküman 4.9 ekonomi ölçeğiyle uyumlu):

| Lisans | Örnek fiyat | Nereden alınır |
|---|---|---|
| Sürücü lisansı | 250.000 | Belediye |
| Rebel lisansı | 1.000.000 | (bkz. Döküman 8.5 — güven görevi sonrası) |
| İş-özel işleme lisansı (örn. pirinç işleme) | Düşük değerli işler için düşük (örn. 50.000-100.000) | İlgili işleme merkezi |
| İş-özel işleme lisansı (örn. gümüş işleme, yüksek değerli) | Yüksek değerli işler için yüksek (örn. 200.000-300.000) | İlgili işleme merkezi |

*Genel kural: lisans fiyatı, o işin/aracın sağladığı kazanç potansiyeliyle orantılı olmalı — ucuz iş ucuz lisans, değerli iş değerli lisans. Tam sayılar Faz 2'de her iş prototiplenirken netleşecek, ama oran mantığı bu.*
| Rol uygunluğu | Aktif rol bu itemi satın alabilir/kullanabilir mi? |
| Yer ve zaman | Doğru markette mi, market açık mı, olay kilidi var mı? |
| Kapasite | Oyuncu, araç, ev veya garaj alanı yeterli mi? |
| Stok / durum | Ürün aktif mi, sigorta/vergiden dolayı varlık kilitli mi? |
| Kural motoru | Paket, etkinlik, tarih veya admin override kuralı var mı? |

**Oyuncu ticareti istisnası:** Bir item oyuncudan satın alınabiliyorsa, NPC'nin süre/lisans şartı varsayılan olarak aşılabilir; ancak itemin güvenlik açısından zorunlu kullanım politikası (örn. polis rolü şartı) her koşulda devam eder. Her item için "oyuncu ticareti edinim şartlarını aşar/aşmaz" parametresi bulunur — bu, ikinci el ekonomisini teşvik ederken güvenlik açığı oluşturmaz.

---
*Önceki: [02 - Hesap, Karakter & Rol](./02_hesap_karakter_rol.md) · Sıradaki: Döküman 4 — Ekonomi & Piyasa Motoru*
