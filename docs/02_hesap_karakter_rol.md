# Döküman 2 — Hesap, Karakter & Rol Durumu

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 2.1 Karakter Oluşturma (güncellendi)

Orijinal tasarımda "doğum yeri" ayrı bir alan olarak seçiliyordu; **bu karar sadeleştirildi:**

1. İlk girişte oyuncu doğrudan sivil karakter oluşturma ekranına gelir.
2. **Yalnızca ad ve soyad girilir** — ad-soyad birleşimi sunucuda benzersiz olmak zorundadır.
3. Karakter görünümü (ve cinsiyeti) oluşturulur; oluşturulduktan sonra oyuncu tarafından değiştirilemez.
4. Oyuncu, harita üzerindeki **uygun spawn noktalarından birini seçerek** doğrudan oyuna başlar. (Önceki "doğum yeri" metin alanı yerine, doğrudan işlevsel bir spawn seçim ekranı — hem daha basit hem de Döküman 1'deki bölge sınıflarıyla doğal olarak örtüşüyor: yeni oyuncu güvenli bölgedeki spawn noktalarından birini görür.)
5. Karakter silme ve kimlik değiştirme **yalnızca yetkili admin işlemiyle** yapılabilir — oyuncu kendi karakterini silemez. (Bu, ekonomi ve audit bütünlüğü için bilinçli bir kısıtlama: karakterle ilişkili tüm finansal/hukuki geçmişin sürekliliğini korur.)
6. Başlangıçta polis veya doktor rolü seçilemez; bu roller kariyer koşulları tamamlandığında oyuncuya açılır (bkz. Döküman 3, Döküman 8).

## 2.2 Tek Hesap, Ayrı Rol Anlık Görüntüleri

Oyuncu tek bir kimliğe ve **ortak ekonomik hesaba** sahiptir; ancak her aktif rolün dünyadaki anlık durumu ayrı saklanır. Oyuncu polisten sivile geçtiğinde sivil rolünü en son bıraktığı konum, kıyafet ve envanterle sürdürür. Polise döndüğünde polis rolünü bıraktığı noktadan devam eder.

| Ortak hesap verisi | Role özel durum |
|---|---|
| Benzersiz karakter kimliği (ad-soyad) | Son konum ve yönelim |
| Toplam aktif oyun süresi | Taşınan envanter |
| Banka hesabı ve para defteri | Kıyafet / loadout |
| Ev ve araç sahipliği | Sağlık / baygınlık durumu |
| Genel lisanslar ve geçmiş | Aktif görev, çağrı veya operasyon |
| Genel istatistik ve audit log | Role özel araç/ekipman erişimi |

**Sahiplik ≠ kullanım yetkisi.** Bir araç veya item oyuncunun hesabına ait olabilir; aktif rol itemin kullanımına izin vermiyorsa eşya kullanılamaz. Polis kıyafeti sivil rolde, rebel ekipmanı polis rolünde açılamaz. (Bu kural, veri modelinde her item için "kullanım hakkı" alanının rol bazlı kontrol edilmesi gerektiği anlamına geliyor — bkz. Döküman 5, item ana veri kaydı.)

## 2.3 Doğma ve Ölüm Sonrası Giriş

- İlk doğuşta seçilen spawn noktası karakter geçmişine kaydedilir (ilk "nereden başladı" bilgisi olarak — istatistik amaçlı, oynanışı etkilemez).
- Ölüm sonrasında oyuncu aktif role uygun doğma noktalarını gösteren ekrana gelir.
- Sivil, polis ve doktor için ayrı spawn havuzları tanımlanabilir.
- NLR (No Life Rule) veya olay kısıtı bulunan spawn noktaları geçici olarak listeden kaldırılabilir.
- Rol değişiminde spawn yapılmaz; oyuncu ilgili rolün son kaydedilmiş durumuna yüklenir (bkz. 2.2).

## 2.4 Onay Gerektiren Bildirim Sistemi

Admin veya sistem tarafından gönderilen kritik bildirimler normal toast mesajı gibi kaybolmaz:

- Ekranda başlık, açıklama, gönderici, tarih ve gerekirse bağlantılı kural gösterilir.
- Oyuncu **"Okudum ve onaylıyorum"** kutusunu işaretlemeden ekranı kapatamaz.
- Onay tarihi audit loguna yazılır (hukuki/idari referans olarak — örn. kural değişikliği bildirimi sonrası ceza uygulanırsa "bildirildi ve onaylandı" kanıtı olur).
- Düşük önemdeki bildirimler normal HUD bildirimleri olarak gösterilir (onay gerektirmez).

## 2.5 Teknik Not (ileride Döküman 11'de detaylanacak)

Bu dökümandaki veri modeli kabaca şu tabloları işaret ediyor: `accounts`, `characters`, `role_states`, `play_sessions`, `notifications`. Şimdilik bunu bir referans olarak not düşüyorum; şema detayları Teknik Mimari dökümanında netleşecek.

---
*Önceki: [01 - Dünya, Harita & İçerik](./01_dunya_harita_icerik.md) · Sıradaki: Döküman 3 — Yeni Oyuncu Deneyimi & İlerleme Motoru*
