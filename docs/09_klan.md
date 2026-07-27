# Döküman 9 — Klan Sistemi

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

> **Kapsam değişikliği notu:** Orijinal dökümanda Klan "Kapalı alfa aşaması" (öncelik 6/10, MVP Core'dan sonra) idi. Sen bunu da **MVP Core'a** almak istedin — "en temel özelliklerden" olduğunu belirttin. Bu, Döküman 8'deki Polis/Doktor kararıyla birlikte üçüncü kapsam genişletmesi oldu (Döküman 5 - iş sayısı, Döküman 8 - Polis/Doktor, şimdi Döküman 9 - Klan). Bunların hepsini Döküman 12'de roadmap'e topluca yansıtacağım; MVP Core artık orijinal plandan daha geniş bir kapsam taşıyor — bu senin bilinçli tercihin, sorun değil, ama süre tahminini gerçekçi tutmak için not düşüyorum.

## 9.1 Kuruluş ve Üye Yapısı

- Klan kurmak için minimum oyun süresi, kuruluş bedeli ve **en az 5 kurucu ekip üyesi** gerekir.
- Klan kapasitesi sunucu ayarına göre **10, 15 veya 20 üyelik** paketlerden biri olabilir (onaylandı). **Kuruluş bedeli kapasite paketine göre kademeli (kesinleşti):**

| Kapasite paketi | Kuruluş bedeli |
|---|---|
| 10 üye | 750.000 |
| 15 üye | 1.250.000 |
| 20 üye | 2.000.000 |

- Kurucu ve yardımcı yönetici sayıları, rütbe ve kasa yetkileri panelden tanımlanır.
- Klanın ortak bankası, araçları, deposu, amblemi, rütbeleri ve yönetim kayıtları bulunur.

## 9.2 Günlük Klan Maliyeti (somutlaştırıldı — Döküman 4.9 ölçeğiyle uyumlu)

| Gider | Hesap temeli | İlk varsayım |
|---|---|---|
| Üye gideri | Aktif/toplam üye başına sabit bedel, günlük | 5.000/üye |
| Kasa/servet gideri | Klan kasası için kademeli oran (Döküman 4.2'deki servet vergisiyle aynı mantık, eşikler klan ölçeğine göre büyütüldü) | 0-5M muaf, 5M-20M %1, 20M+ %2 |
| Araç gideri | Klan araçlarının referans değerine göre, günlük | %1-2 (Döküman 4.2'deki araç vergisiyle aynı oran) |
| Bölge/kiralama | Klan noktasının 3/5/7 günlük kira bedeli | 500.000 / 800.000 / 1.000.000 |
| Depo/avantaj | Kapasite ve açılan hizmetlere bağlı ek gider | Panelden ayarlanır, ilk sürümde sabit küçük bir ek (örn. 20.000/gün) |

*Tüm bu kesintiler, Döküman 4.2'deki günlük vergi döngüsüyle aynı zamanlanmış görevde işlenir — ayrı bir "klan kesinti turu" yok.*

Ödeme yapılamazsa klan anında silinmez. Varsayılan 2-3 günlük süreçte: uyarı → avantajların kapanması → varlık kilidi → fesih aşamaları çalışır.

## 9.3 Kiralanabilir Klan Noktaları

Haritada sınırlı klan noktaları bulunur. Uygun klan 3, 5 veya 7 günlük süre için noktayı kiralayabilir. Varsayılan dağıtım ilk uygun kiralayan esasına göre yapılır; admin açık artırma veya etkinlik kuralı tanımlayabilir. Bir klanın aynı anda kiralayabileceği nokta sayısı sınırlandırılır.

- Klan garajı ve depo erişimi
- Klan üyelerine özel silah/araç satış noktası
- Sınırlı lojistik veya üretim kolaylığı
- Klan HUD/harita buluşma noktası
- Kirayı ödemeyen veya süresi biten klan için otomatik kapanma

## 9.4 Açık Maddeler

- [ ] Klan'ın MVP Core'a alınması → Döküman 12 roadmap süresine yansıtılacak (3. kapsam genişletmesi)
- [ ] Klan noktalarının haritadaki sayısı/lokasyonu → Döküman 1 (Dünya) ile birlikte netleşecek

---
*Önceki: [08 - Roller & Çatışma](./08_roller_catisma.md) · Sıradaki: Döküman 10 — Oyuncu Paneli, HUD & Admin Panel*
