# Döküman 5 — İşler (Topla-İşle-Sat) & Temel İhtiyaçlar/Envanter

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 5.1 Tek İş Mantığı, Çok Sektör

M360 Life'taki işlerin çoğu aynı teknik çekirdeği kullanır. Oyuncu işin toplama bölgesine gider, ham malzemeyi kapasitesi ölçüsünde toplar, ilgili işleme tesisinde lisans kontrolünden geçerek işler ve işlenmiş ürünü özel satış noktasına götürür. Farklılık yalnızca item, lokasyon, süre, lisans, risk, ödeme tipi ve fiyat parametrelerindedir — **kod değil, config değişir.**

## 5.1b Detaylı İş Akışı Şablonu (derinleştirildi)

Bu bölüm, 5.1'deki "tek çekirdek" fikrini somut bir mekanik şablona döküyor. Her yeni iş bu şablonun bir config kopyasıdır — script değişmez, aşağıdaki parametreler değişir.

### Aşama 1 — Toplama (Collection)

1. Oyuncu, işin toplama bölgesine gider (bölge herkese açık — bkz. 5.1c).
2. Toplama başlatmak için **gerekli alet** üzerinde olmalıdır (örn. kazma, balta, olta). Alet tükenmez, sadece "üzerinde var mı" kontrolü yapılır — dayanıklılık sistemi yok (Döküman 5.7 ile tutarlı).
3. Oyuncu toplamayı başlatır; her **tick süresi** (örn. varsayılan 5 saniye) sonunda **tick başı verim** kadar ham madde üzerine eklenir (örn. 1 birim gümüş cevheri).
4. Toplama, oyuncunun kişisel envanter kapasitesi dolana kadar (bkz. Döküman 5.6) veya oyuncu durdurana kadar devam eder.
5. Envanter dolduğunda: oyuncu yakındaki bir araca (varsa) boşaltıp toplamaya devam edebilir, ya da toplamayı bitirip işleme aşamasına geçebilir.

### Aşama 2 — Taşıma (isteğe bağlı ara adım)

- Araç kullanımı zorunlu değildir ama yüksek kapasiteli işler (örn. petrol, taş) pratikte araç gerektirir.
- Oyuncu envanteri ile araç envanteri arasında serbestçe transfer yapabilir (kapasite kontrolü her ikisinde de ayrı ayrı uygulanır).

### Aşama 3 — İşleme (Processing) — kişiye özel oturum modeli

**Karar (bu oturumda netleşti):** Makine paylaşılan bir "slot" sistemi değil, **kişiye özel işlem oturumu** modeliyle çalışır — bu hem daha basit hem daha güvenli (concurrency/yarış durumu riski taşımaz):

1. Oyuncu işleme tesisine (makineye) gider ve etkileşime girer.
2. Sistem lisans, girdi miktarı ve rolü kontrol eder.
3. Oyuncu "işle" der; makine, oyuncunun üzerindeki/yakınındaki ham maddeyi **tanımlı üst sınıra kadar** (örn. varsayılan max 60 birim) kendi işlem penceresinde işler. Bu üst sınır iş bazında config'ten ayarlanır (**batch boyutu**).
4. 0-100 ilerleme çubuğu gösterilir; **işleme süresi** boyunca (config'ten ayarlanır) oyuncunun ilgili envanteri kilitli kalır — transfer/satış yapılamaz.
5. İşlem tamamlanınca **dönüşüm oranına** göre (işe göre değişir — bazıları 1:1, bazıları kayıplı örn. 3 ham:2 işlenmiş) işlenmiş ürün otomatik olarak oyuncunun envanterine döner.
6. **Aynı anda başka bir oyuncu da aynı makineye gidip kendi işlemini başlatabilir** — işlemler birbirinden bağımsızdır çünkü state makineye değil, oyuncu-oturumuna bağlıdır. Bu, "slot sayısı yönetme" ihtiyacını ortadan kaldırır.
7. Oyuncu işlem alanından uzaklaşır ya da iptal ederse işlem durur; girdiler mevcut haliyle korunur, tamamlanmayan çıktı verilmez.
8. Baskın veya çatışma halinde oyuncu işlemi kesip anında oyuna dönebilir.

### Aşama 4 — Satış (Selling)

- Oyuncu işlenmiş ürünü satış noktasına (NPC) götürür.
- **Satış anlıktır**, ek bir süre/işlem gerekmez.
- Fiyat = temel fiyat × nüfus/risk katsayısı (Döküman 4.5) × varsa diğer piyasa/vergi kuralları.
- Ödeme türü işin yasal/yasa dışı durumuna göre değişir (bkz. Döküman 4.1): yasal işler nakit veya banka, yasa dışı işler yalnızca nakit.
- Üretilen her parti; iş, sahip, K1/K2/K3 değerleri, zaman ve kaynak geçmişiyle audit'e kaydedilir (bkz. Döküman 5.3).

### İş Tanımı — Config Şeması (her iş için tutulacak parametreler)

> **Kritik kural (netleşti):** Aşağıdaki tabloda listelenen **her parametre, admin panelinden kod tarafına dokunmadan değiştirilebilir olacak.** Tick süresi, tick verimi, batch boyutu, işleme süresi, dönüşüm oranı, fiyat, lisans şartı, ihbar olasılığı — hiçbiri script içine sabit (hardcoded) değer olarak yazılmayacak. Script yalnızca "bu parametreyi oku ve uygula" mantığını içerecek; sayının kendisi veritabanında (`job_definitions` tablosu, bkz. Döküman 11.8) tutulacak ve admin panel üzerinden okunup güncellenecek. Bu, Döküman 0'daki "Koda dokunmadan yönetim" ilkesinin iş sistemi için somut karşılığı — dengeyi bulmak için her seferinde kod değişikliği/redeploy gerekmeyecek, panelden anlık ayarlanacak.

| Parametre grubu | Alanlar |
|---|---|
| Kimlik | İş adı, kategori, yasal/yasa dışı durum |
| Toplama | Alet tipi, tick süresi, tick başı verim, toplama bölgesi konumu/konumları |
| Taşıma | Ham madde kapasite maliyeti (birim başına alan) |
| İşleme | Makine konumu, batch boyutu (max birim/oturum), işleme süresi, dönüşüm oranı, işlenmiş ürün kapasite maliyeti |
| Satış | Satış noktası konumu, birim fiyat, ödeme türü (nakit/banka) |
| Güvenlik | Gerekli lisans, minimum aktif süre şartı, ihbar olasılığı (yasa dışıysa) |
| Nadir/süreli iş bayrağı | (opsiyonel) Bu iş zaman sınırlı bir etkinlik işi mi? (bkz. 5.1c) |

**Teknik not (Döküman 11 ile bağlantı):** Bu parametreler `job_definitions` tablosunda tutulacak; her değişiklik Döküman 10.6'daki "kural önceliği" ve sürümleme/geri alma mantığına tabi olacak — yani admin bir değeri değiştirdiğinde eski değer kaybolmaz, sürüm geçmişi audit'e yazılır ve gerekirse geri alınabilir.

### 5.1c Toplama Alanları ve Kaynak Süreklilik Kuralı (netleşti)

- **Genel kural:** Toplama alanları herkese açıktır (tüm oyuncular aynı alanı kullanabilir), kaynak **sınırsızdır** — tükenme/yenilenme mekaniği yok.
- **İstisna:** Bazı yüksek değerli/nadir işler **zaman sınırlı "etkinlik" işi** olarak tasarlanabilir (örn. sınırlı süre aktif olan özel bir kaynak alanı). Bu, genel kuralın dışında, panelden aktif/pasif yapılabilen ayrı bir kategori olacak — MVP'nin temel 15 işi için geçerli değil, ileride "nadir iş" olarak eklenebilir.

## 5.2 Sektör Listesi — Genişletilmiş Kapsam

**Karar:** MVP'nin orijinal önerisi "2 yasal + 1 yasa dışı prototip" idi; sen **10 yasal + 4-5 yasa dışı** iş istedin. Bu, mimari açıdan sorun değil (tek prefab çekirdeği sayesinde her yeni iş bir config kaydı) ama **içerik üretimi** (her işin toplama/işleme/satış lokasyonlarının haritada yerleştirilmesi, denge/fiyat ayarı, test edilmesi) orantılı olarak artar. Bunu Döküman 12'de roadmap'e not düşeceğim — "MVP Core" fazının süresi buna göre biraz uzayabilir, ama sistemin kendisi zaten buna hazır tasarlandı.

### Yasal sektörler (10 hedef)

| Sektör | Durum | Not |
|---|---|---|
| Pirinç | Orijinal dökümanda var | Güvenli başlangıç işi, düşük gelir |
| Bakır | Orijinal dökümanda var | Orta rota ve işleme süresi |
| Gümüş | Orijinal dökümanda var | Değerli yük, daha yüksek soygun riski |
| Petrol | Orijinal dökümanda var | Araç ve yüksek kapasite ihtiyacı |
| Balıkçılık | Yeni — placeholder | Kıyı/göl bölgesine bağlı, düşük-orta risk |
| Tarım (sebze/tahıl) | Yeni — placeholder | Yeni oyuncu dostu, düşük risk |
| Kereste/Ahşap | Yeni — placeholder | Orman bölgesi, orta risk |
| Taş/Mermer | Yeni — placeholder | Ağır yük, araç gerektirir |
| Tekstil | Yeni — placeholder | Şehir içi, düşük risk |
| Elektronik parça | Yeni — placeholder | Bakıra bağlı (Döküman 4.6 piyasa ilişkisi) |

### Yasa dışı sektörler (5 hedef — kesinleşti)

| Sektör | Durum | Not |
|---|---|---|
| Koko | Orijinal dökümanda var | Nakit ödeme, polis ihbar ihtimali |
| Ero | Orijinal dökümanda var | Yüksek risk ve yüksek gelir |
| Silah kaçakçılığı | Yeni — placeholder | Yüksek risk, rebel lisansı gerektirir |
| Çalıntı araç parçalama | Yeni — placeholder | Araç sistemine bağlı (Döküman 7) |
| Sahte belge/kara para | Yeni — placeholder | İleri seviye, düşük hacim yüksek getiri |

> Not: Placeholder işaretli sektörlerin isim/detayı henüz tasarlanmadı — bunlar sırayla, prefab çekirdeği kodlandıktan sonra (Faz 2'den itibaren) birer birer config olarak ekleneceğiz. Şimdilik "kaç tane ve hangi kategoride" kararını netleştirdik, bu yeterli.

## 5.3 İşleme Davranışı

Detaylı akış için bkz. **5.1b Aşama 3**. Özet: kişiye özel işlem oturumu, batch sınırlı, otomatik envanter dönüşü, iptal edilebilir, audit'e kaydedilir.

## 5.4 Yasa Dışı İşlerde İhbar

Yasa dışı toplama veya işleme faaliyeti sırasında panelden belirlenen olasılıkla (örn. varsayılan %30) sistem olayı polise bildirebilir. Oyuncuya "Birisi sizi ihbar etmiş olabilir" uyarısı gösterilebilir. Polis tarafında kesin koordinat yerine bölge, zaman gecikmesi veya yaklaşık alan verilebilir. İhbar oranı, çevrim içi polis sayısı, işin değeri ve tekrar sıklığına göre değiştirilebilir.

## 5.5 Temel İhtiyaçlar (onaylandı — değişiklik yok)

| Değer | MVP davranışı |
|---|---|
| Açlık | 100'den 0'a yaklaşık 5-6 aktif saatte iner; yiyecek barı doldurur |
| Susuzluk | 100'den 0'a yaklaşık 5-6 aktif saatte iner; içecek barı doldurur |
| Sağlık | Arma Reforger hasar ve baygınlık akışıyla bütünleşir |
| Enerji | Koşma ve fiziksel eylemlerle azalır; dinlenme/ürünle geri gelir |

MVP'de yemek kalitesi, hijyen, uyku, vitamin ve karmaşık hastalık sistemleri **eklenmez**. İhtiyaç mekanikleri oyuncuya periyodik harcama ve küçük kararlar üretir; sürekli bar yönetimine dönüşmez.

## 5.6 Kapasite Tabanlı Envanter (onaylandı)

Klasik ağırlık veya sabit kutucuk yerine rakamsal kapasite kullanılır. Oyuncu, çanta, araç, ev ve depo bir "kapasite puanı" taşır; her item belirli puan tüketir.

| Örnek | Kapasite / alan |
|---|---|
| Oyuncu temel kapasite | 100 |
| Büyük çanta | Toplam kapasiteyi 200'e çıkarabilir |
| Ham gümüş | 3 alan |
| İşlenmiş gümüş | 2 alan |
| Küçük araç | Örnek 150 alan |
| Kamyon | Örnek 1.000+ alan |
| Ev deposu | Mülk türüne göre değişken |

Tüm değerler panelden yönetilir. İşleme başladıktan sonra ilgili envanter işlem kilidine girer; ürün ekleme, çıkarma veya transfer yapılamaz. Oyuncu alanı terk eder ya da iptal ederse işlem durur ve girdiler mevcut haliyle korunur.

## 5.7 Item Ana Veri Kaydı

| Alan grubu | Tutulacak bilgiler |
|---|---|
| Kimlik | Item ID, prefab adresi, sürüm, kategori, görünür ad |
| Erişim | Kullanabilen roller, lisans, minimum süre, güvenlik sınıfı |
| Ekonomi | NPC fiyatı, oyuncu pazar alt/üst oranı, vergi grubu, sigorta değeri |
| Envanter | Kapladığı alan, stack/adet, kullanım hakkı, kimliksel/fiziksel davranış |
| Sahiplik | Sahip hesap, ilk üretici, son transfer, çalıntı/claimed durumu |
| Yaşam döngüsü | Düşer mi, satılabilir mi, trade yasağı, süreli mi, wipe davranışı |
| Audit | Kim aldı, attı, sattı, kaybetti; tarih, sunucu ve olay ID |

Dayanıklılık sistemi ilk sürümde yoktur. Eşyalar sınırsız kullanılabilir veya item tanımına göre belirli kullanım hakkına sahip olabilir.

### Kazanç Çapası ve Referans İş Örnekleri (kesinleşti — bu oturumda somutlaştırıldı)

**Genel mikro-parametreler (tüm işler için varsayılan başlangıç):**

| Parametre | Değer |
|---|---|
| Tick süresi | 3-5 saniye (ortalama ~4 sn) |
| Tick başı verim | 2-3 birim (ortalama ~2.5) |
| Varsayılan batch boyutu (işleme) | 100 birim — **iş bazında admin tarafından değiştirilebilir** |

**Referans iş örneği 1 — Pirinç (orta ölçek, hedef: 60.000/sefer)**

| Parametre | Değer |
|---|---|
| Kapasite maliyeti (ham / işlenmiş) | 3 alan / 2 alan |
| Batch boyutu | 100 birim |
| Dönüşüm oranı | 1:1 (kayıpsız) |
| İşleme süresi | ~3 dakika/batch |
| Birim satış fiyatı | 600 |
| **Sefer kazancı** | 100 × 600 = **60.000** ✅ hedefle uyumlu |

**Referans iş örneği 2 — Gümüş (tır ölçek, hedef: 120.000/sefer)**

| Parametre | Değer |
|---|---|
| Kapasite maliyeti (ham / işlenmiş) | 3 alan / 2 alan |
| Batch boyutu | 90 birim *(iş-özel override — genel varsayılandan farklı, admin panelden ayarlanmış örnek)* |
| Dönüşüm oranı | 3:2 (kayıplı — 90 ham → 60 işlenmiş) |
| İşleme süresi | ~5 dakika/batch (daha değerli, daha uzun) |
| Birim satış fiyatı | 2.000 |
| **Sefer kazancı** | 60 × 2.000 = **120.000** ✅ hedefle uyumlu (araba-sınıfının tam 2 katı) |

> Bu iki örnek, Faz 2'de ilk prototip yapılırken **doğrudan kullanılacak başlangıç config değerleridir.** Diğer 13 iş (Döküman 5.2) bu iki örneğin sektöre göre uyarlanmış versiyonları olacak — kazanç hedefi hangi ölçeğe (orta/tır) girdiğine göre belirlenip, tick/batch/fiyat oradan geriye doğru hesaplanacak.

| İş ölçeği | 1 sefer kazancı | Örnek işler |
|---|---|---|
| Orta ölçekli (araba-sınıfı işler) | **60.000** | Pirinç (referans), tarım, balıkçılık, bakır |
| Büyük/tır ölçekli (tır-sınıfı işler) | **120.000** | Gümüş (referans), petrol, taş/mermer |

## 5.8 Açık Maddeler

- [ ] 6 yeni yasal + 3 yeni yasa dışı sektörün isim/lokasyon/denge detayları — Faz 2'den itibaren tek tek tasarlanacak
- [ ] Genişletilmiş iş sayısının roadmap süresine etkisi → Döküman 12'de güncellenecek
- [x] ~~İş akışı mikro-mekaniği (toplama/işleme/satış detayları)~~ — bu oturumda 5.1b'de netleşti
- [x] ~~Her iş için tick süresi, tick verimi, batch boyutu, işleme süresi ve dönüşüm oranının gerçek sayısal değerleri~~ — bu oturumda 2 referans iş örneğiyle (pirinç, gümüş) somutlaştırıldı

---
*Önceki: [04 - Ekonomi & Piyasa](./04_ekonomi_piyasa.md) · Sıradaki: Döküman 6 — Sağlık, Ölüm & Sigorta*
