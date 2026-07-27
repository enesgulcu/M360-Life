# Döküman 8 — Roller (Sivil/Polis/Doktor/Rebel) & Çatışma Kuralları

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

> **Kapsam değişikliği notu:** Orijinal dökümanda Polis/Doktor "Core MVP sonrası" (öncelik 8/10) olarak planlanmıştı. Sen ikisinin de **MVP Core'da** olmasını istedin. Bu, Döküman 12'deki roadmap'e işlenecek bir kapsam genişlemesi — iş sayısı artışıyla (Döküman 5) birlikte MVP süresini etkileyen ikinci büyük madde. Sistemin kendisi buna teknik olarak hazır (rol mimarisi zaten Faz 1'de kuruluyor), ama içerik/dengeleme işi (rütbe sistemi, çağrı paneli, operasyon ekranı) daha erken başlamış olacak.

## 8.1 Rol Kotası ve Sunucu İhtiyacı (onaylandı)

| Rol | Maksimum | İhtiyaç eşiği | Giriş davranışı |
|---|---|---|---|
| Polis | Aktif nüfusun %20'si | %10 | %10 altındaysa yetkili oyuncu haftalık sivil süre şartı olmadan girebilir; üzerinde ise örnek 8 saat sivil şartı aranır |
| Doktor | Aktif nüfusun %15'i | %5 | İhtiyaç altında esnetilir; üstünde rol koşulları ve aktif sivil katkısı aranır |
| Sivil | Ana nüfus | - | Varsayılan ve her zaman erişilebilir rol |
| Rebel | Sivil içinde lisans/kariyer | - | Ayrı görünür faction değil; sivil rol içindeki yasa dışı erişim |

Kota doluysa sıra sistemi yoktur; oyuncuya "Rol şu anda dolu, daha sonra tekrar deneyin" mesajı gösterilir. Tüm yüzdeler ve haftalık sivil süreleri panelden ayarlanır.

## 8.2 Rol Değiştirme Güvenliği

- Son 10 dakika içinde ateş alma, hasar verme/alma veya düşmanca olay varsa rol değiştirilemez.
- Aktif görev, kabul edilmiş polis/doktor çağrısı, tutuklama, işleme veya olay kilidi varsa değiştirilemez.
- Güvenli bölge dışında 20 metre çevrede aktif ateş varsa çatışma alanı kabul edilir ve değişim reddedilir.
- Şartlar uygunsa menüden rol değiştirme isteği verilir; 10 saniyelik geri sayım tamamlanınca ilgili rol durumu yüklenir.
- Geri sayım sırasında hasar, hareket veya yeni olay oluşursa işlem iptal edilir.

## 8.3 Polis Rütbeleri (10 kademe)

| Kademe | Rütbe | Varsayılan maaş (15 dk) |
|---|---|---|
| 1 | Stajyer Polis | 7.500 |
| 2 | Polis Memuru | 9.000 |
| 3 | Kıdemli Polis Memuru | 10.500 |
| 4 | Başpolis Memuru | 12.000 |
| 5 | Komiser Yardımcısı | 14.000 |
| 6 | Komiser | 16.500 |
| 7 | Başkomiser | 19.000 |
| 8 | Emniyet Amiri | 21.500 |
| 9 | Emniyet Müdür Yardımcısı | 24.000 |
| 10 | Emniyet Müdürü | 26.000 |

*(Sivil vatandaşlık maaşı 2.500/15dk — bkz. Döküman 4.9. Bu tablo o referansa göre kademeli olarak 3-10 kat arasında. İlk varsayım, panelden ayarlanabilir.)*

Polis bireysel olarak rebelden bir miktar güçlü olabilir; bu avantaj yalnızca zırh yüzdesinden değil, haberleşme, olay paneli, takviye, bayıltıcı ekipman, düzenli araç, bilgi erişimi ve rütbe koordinasyonundan doğmalıdır. Polis kendi araç ve ekipmanını satın alır; yüksek maaş ve role özel uygun fiyatlar bu gideri karşılar.

- Polis arama emri, başına ödül, ceza, hapis ve kamu görevi uygulayabilir.
- Her işlemin kanun/kural karşılığı, yetkili rütbesi, maksimum süre ve delil gereksinimi config olarak tanımlanır.
- Düşük rütbeler yüksek riskli operasyonlara liderlik edemez veya ileri ekipmanı kullanamaz.
- Devriye ve operasyon paneli görev çakışmasını azaltır; acil durum override sistemi katı bölge kurallarını geçici esnetir.

## 8.4 Doktor Rütbeleri (10 kademe)

| Kademe | Rütbe | Varsayılan maaş (15 dk) |
|---|---|---|
| 1 | Stajyer Sağlık Görevlisi | 7.500 |
| 2 | Acil Tıp Teknisyeni | 9.000 |
| 3 | Paramedik | 10.500 |
| 4 | Kıdemli Paramedik | 12.000 |
| 5 | Ambulans Sorumlusu | 14.000 |
| 6 | Doktor | 16.500 |
| 7 | Uzman Doktor | 19.000 |
| 8 | Kıdemli Uzman | 21.500 |
| 9 | Başhekim Yardımcısı | 24.000 |
| 10 | Başhekim | 26.000 |

MVP'de doktorun temel görevi baygın oyuncuyu kaldırmak ve iyileştirmektir. Çağrıyı panelden kabul eder; hasta ve diğer doktorlar hangi personelin müdahale ettiğini görür. İlaç, rapor ve gelişmiş tıp süreçleri sonraki sürümlere bırakılır.

## 8.5 Rebel Erişimi

- Minimum örnek 10 aktif oyun saati.
- Oyun parasıyla alınan rebel lisansı.
- Basit güven görevi: belirli bir eşyayı bir noktadan diğerine ulaştırma.
- Lisans sonrası yasa dışı işler, kara borsa ve rebel silah marketleri açılır.
- Tek başına rebel olunabilir; klan üyeliği zorunlu değildir.
- Rebel ayrı aktif rol değildir; sivil rol içinde yasa dışı kariyer ve lisans setidir.

## 8.6 Saldırgan Profil Sistemi (onaylandı — MVP kapsamında)

Oyuncu bir tuşa saldırgan rol profili atar. Profil, sunucunun izin verdiği baremler içinde X alanı, Y bekleme penceresi ve Z mesajı içerir.

> Örnek: "Tuşa bastıktan sonraki 30 saniye içinde 20 metre alana giren uygun hedefe 'Teslim ol, ellerini kaldır!' bildirimi gönder."

| Parametre | Kural |
|---|---|
| X - alan | Oyuncu kendisi ayarlar; sunucu sınırı **5-30 metre**, **varsayılan/başlangıç değeri 22 metre** (kesinleşti — geniş taraf tercih edildi). Sınır dışı değerler (örn. 5 km) sunucu tarafında reddedilir. |
| Y - aktivasyon süresi | **Tamamen oyuncu tarafından belirlenir** (kesinleşti) — sunucu sınırı 5-30 saniye içinde kalmak kaydıyla, her oyuncu kendi profilinde kendi tercih ettiği süreyi ayarlar; sabit bir sunucu varsayılanı dayatılmaz. |
| Z - rol mesajı | Önceden tanımlı ve moderasyona uygun mesaj havuzu veya sınırlı özel metin |
| Güvenli bölge | Sistem tamamen pasif |
| Tepki süresi | Normal durumda 3-5 saniye; silaha davranma veya kaçışta erken müdahale mümkün |

> **Geliştirme notu:** Bu sistem, MVP'de olsun dedik ama teknik olarak dikkat gerektiren bir parça — parametre doğrulama (X/Y/Z sınırları) sunucu tarafında yapılmalı, istemci güvenilir kaynak değil (bkz. Döküman 11, güvenlik ilkeleri). Faz sıralamasında bunu "İlk dikey dilim" değil, MVP Core'un ortasına konumlandıracağız — önce temel rol geçişleri ve çatışma logu (8.8) sağlam oturmalı.

## 8.7 Grup Yayılımı

- Rol veren oyuncunun 1 km içindeki aynı klan üyeleri çatışmaya dahil sayılabilir.
- Rol alan hedefin 1 km içindeki aynı klan üyeleri de rol almış sayılabilir.
- Aynı araçtaki oyuncular tarafın parçası kabul edilir.
- Yakından geçen ilişkisiz oyuncu otomatik taraf olmaz; sistem olay bağlamını gösterir ve tarafın saldırgan eylemiyle dahil eder.
- Bildirimde rolü alan oyuncu/klan ve olay ID görünür; sesli iletişimle birlikte kanıt oluşturur.

## 8.8 Sistemsel Kurallar

| Kural | Sistem karşılığı |
|---|---|
| Güvenli bölge | Mermi çıkmaz, hasar uygulanmaz, saldırgan profil çalışmaz |
| NLR / ölüm bölgesi | Ölüm merkezi ve süre kaydedilir; uyarı, rota engeli veya kontrollü geri yönlendirme |
| Combat log | Hasar/rol olayından sonra çıkış ve rol değişimi kilitlenir; bağlantı kesilmesi loglanır |
| RDM bölgeleri | Rol bildirimi gerektirmeyen açık çatışma kuralı HUD'da gösterilir |
| Büyük soygun | Minimum çevrim içi polis ve olay alanı kuralı |
| Tekrarlı hedefleme | Yeni veya aynı oyuncuya karşı tekrar saldırı bekleme süresi |

## 8.9 Açık Maddeler

- [ ] Polis/Doktor'un MVP Core'a alınması → Döküman 12 roadmap süresine yansıtılacak
- [ ] Saldırgan profil sisteminin faz içi konumu (MVP Core ortası) → Döküman 12'de detaylandırılacak

---
*Önceki: [07 - Araç & Mülk](./07_arac_mulk.md) · Sıradaki: Döküman 9 — Klan Sistemi*
