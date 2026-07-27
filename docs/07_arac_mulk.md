# Döküman 7 — Araç, Garaj & Mülkiyet

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 7.1 Sahiplik ve Sınırsız Koleksiyon (onaylandı)

Oyuncunun sahip olabileceği araç sayısına sabit üst sınır konmaz — para ve oyun saati kadar araç alınabilir. Bunlar dünyada sürekli entity olarak tutulmaz, oyuncu panelindeki garaj kayıtları olarak saklanır. **Performansı belirleyen sahip olunan araç sayısı değil, aynı anda dünyada spawn edilen araç sayısıdır** — bu ayrım teknik açıdan kritik (bkz. Döküman 11, performans ilkesi).

### Fiyat Kademeleri (son karar — bu oturumda kesinleşti)

| Araç sınıfı | Fiyat | Ulaşım hedefi (Döküman 4.9) |
|---|---|---|
| Motor | 100.000 | Gün 1-2 |
| Eski/püskü araba | 250.000 | Gün 1 (başlangıç parası + lisans ile, iş yapmadan bile alınabilir) |
| Sıradan araba | 500.000 | Gün 2-5 (birkaç iş seferi) |
| Bagajlı araba | 750.000 | Hafta 1 |
| Minibüs | 1.250.000 | Hafta 1-2 |
| Tır (5 alt sınıf) | 1.500.000 — 10.000.000 arası kademeli | **Hafta 1 sonu** küçük tır (1.5M) mümkün; orta/üst tırlar Hafta 2+ / Gelişmiş–Kıdemli |

Araç ve tır satış noktaları **ayrı lokasyonlarda** olacak. Bagaj kapasitesi ve hız gibi özellikler, sınıf/fiyata göre kademeli olarak artacak — tam değerler Faz 2'de içerik üretilirken netleşecek.

## 7.2 Garaj ve Otomatik Temizleme

| Durum | Varsayılan davranış |
|---|---|
| Sahip araçtan 1 km'den fazla uzak | 15 dakika sonra garaja gönderilebilir |
| Sahip 1 km içinde fakat araçtan 5 m'den uzak | 30 dakika sonra garaja gönderilebilir |
| Aktif çatışma / görev / hırsızlık | Otomatik garaj geçici kapatılır |
| Araçta başka yetkili oyuncu | Aktif kullanım sayılır; süre başlamaz veya farklı kural uygulanır |
| Sunucu reseti | Kalıcı sahiplik korunur; araç güvenli biçimde garaja döner |

## 7.3 Anahtar ve Paylaşım

- Fiziksel anahtar itemi yoktur; araç sahibi otomatik kilitleme, açma, sürme, envanter ve garaj yetkisine sahiptir.
- Sahip başka oyuncuya geçici anahtar erişimi verebilir; bu sahiplik devri değildir.
- Paylaşılan oyuncu sürme, kilitleme ve araç envanterine erişim gibi ayrı ayrı seçilebilir yetkiler alır.
- Hırsız, maymuncuk işlemini başarıyla tamamlarsa süreli hırsız anahtarı elde eder; araç sahibi olmaz.
- Çalınan araç kalıcı garaja konulamaz; gelecekte parçalama, kaçak satış veya kurtarma akışına bağlanabilir (bkz. Döküman 5, "çalıntı araç parçalama" — placeholder sektör).

## 7.4 Araç Kaybı ve Sigorta

Araç patladığında sahiplik kaydı silinir. Sigorta yoksa araç ve değer kaybolur. Sigorta varsa olayın niteliği ve poliçe oranına göre para iadesi yapılır (bkz. Döküman 6.3 — Araç sigortası MVP kapsamında). Araç hasarlı fakat patlamamışsa geçici reset veya garaj çağrısıyla geri gelebilir; bu davranış panelden belirlenir.

## 7.5 24 Saatlik Araç Kiralama (onaylandı — MVP kapsamında)

- Kira süresi kiralama anından itibaren 24 saattir; varsayılan fiyat satın alma bedelinin üçte biridir.
- Kiralık araç garaja konabilir ve panelden çağrılabilir; **satılamaz veya kalıcı devredilemez.**
- Son beş dakikada HUD geri sayımı gösterilir.
- Süre bittiğinde kara aracı kontrollü olarak durur, oyuncuyu güvenli biçimde indirir ve silinir.
- Hava aracında havada oyuncu atılmaz; güvenli iniş/grace period veya en yakın uygun noktaya geri çağırma uygulanır.

> Not: Kiralama sisteminin MVP'ye alınması, satın alma/sahiplik akışıyla **aynı prefab ve backend mantığını** paylaştığı için ek karmaşıklık yaratmıyor — sadece "süreli mülkiyet" flag'i ekleniyor. Bu yüzden ilk dikey dilimde (Döküman 12, Faz 2) birlikte ele alınabilir.

## 7.6 Ev ve Garaj — Başlangıç Seviyesi (onaylandı — MVP kapsamında)

Oyuncunun satın alabileceği ev veya garaj sayısına sabit üst sınır konmaz. Mülkler pahalı ve uzun vadeli hedeflerdir; türüne göre depolama kapasitesi, garaj erişimi, vergi ve güvenlik özelliği değişir.

- Polis veya hırsız evin konumunu bulmak zorundadır; listeden doğrudan ışınlanma veya otomatik erişim yoktur.
- Kapı maymuncuk, arama emri veya özel yetkiyle açılır; düz biçimde içeri girilemez.
- Ev içi envanter ve işlem audit logları tutulur.
- Ev sigortası gelecekte kaybın bir bölümünü azaltabilir (Döküman 6.3'te MVP dışı olarak işaretlendi).

**Kesinleşmiş sınır:** Dükkan/işletme yönetimi MVP ve ilk alfa kapsamı dışındadır; ayrı bir sonraki ürün fazı olarak ele alınacaktır — bu, ev/garajın basit depolama seviyesinde MVP'de kalmasıyla çelişmiyor, sadece "ticari işletme" (çalışan, satış noktası işletme vb.) ertelendi.

---
*Önceki: [06 - Sağlık, Ölüm & Sigorta](./06_saglik_olum_sigorta.md) · Sıradaki: Döküman 8 — Roller (Sivil/Polis/Doktor/Rebel) & Çatışma Kuralları*
