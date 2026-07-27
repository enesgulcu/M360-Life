# Döküman 4 — Ekonomi & Piyasa Motoru

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

> Bu döküman projenin en kritik parçası — "Ekonomi" ve "Temel oynanabilirlik" öncelik listesinde 10/10. Teknik Mimari (Döküman 11) buradaki her kuralı transaction/ledger seviyesinde uygulayacak.

## 4.1 Para Türleri ve Ödeme Kuralları

| İşlem | Varsayılan ödeme |
|---|---|
| Polis/doktor maaşı | Banka |
| Yasal iş satışı | Nakit veya kesintili doğrudan banka seçeneği |
| Yasa dışı iş satışı | Nakit |
| Oyuncular arası pazar ve transfer | Banka |
| Normal NPC marketleri | Banka; market bazlı nakit istisnası olabilir |
| Kara borsa | Nakit |
| Ölüm | Üzerindeki nakit sistemden silinir; hayattayken soygunla alınabilir |

**Kural:** Banka bakiyesi hiçbir zaman eksiye düşmez. Yetersiz bakiye işlemi reddeder veya hizmeti/varlığı pasif hale getirir.

## 4.2 Vergi, Pasif Hesap ve Varlık Dondurma

**Kesinti sıklığı (netleşti): Tüm vergiler günde 1 kez kesilir** — sunucu saatine bağlı sabit bir zamanda (örn. her gün 00:00 sunucu saati), otomatik olarak banka bakiyesinden düşülür. Bu, pasif maaşın 15 dakikalık sıklığından kasıtlı olarak farklı — vergi sık sık rahatsız edici olmasın, ama düzenli ve öngörülebilir olsun.

- Araç, ev, garaj ve klan varlıklarından **günde 1 kez** sabit hizmet bedeli + değerle orantılı **%1-2** kesinti alınabilir (ilk varsayım — panelden ayarlanır).
- **Banka serveti vergisi — 3 kademeli (kesinleşti):**

| Kademe | Bakiye aralığı | Oran |
|---|---|---|
| Düşük | 0 — 2.000.000 | Muaf (%0) |
| Orta | 2.000.000 — 10.000.000 | %1 |
| Yüksek | 10.000.000+ | %3 |

  *(Bu eşikler Döküman 4.9'daki araç ölçeğiyle uyumlu: 2M eşiği "birkaç araç/küçük tır" seviyesini, 10M eşiği "üst segment tır" seviyesini işaretliyor — yani vergi gerçekten "zengin" olanı hedefliyor, sıradan oyuncuyu değil.)*

- Oyuncu yedi günden uzun süre giriş yapmazsa hesap pasif duruma alınır ve vergi/sigorta kesintileri (günlük döngü) donar.
- Oyuncu geri geldiğinde kesintiler kaldığı yerden yeniden başlar; geçmiş pasif dönem topluca tahsil edilmez.
- Vergisi ödenemeyen araç/mülk silinmez; kullanım ve garajdan çıkarma kilitlenir.
- Oyuncu varlığını devlete referans değerinin belirlenen oranıyla (örn. yarı fiyat) satıp borcunu karşılayabilir.
- Sigorta primi ödenmezse sigorta iptal olur; varlık sigortasız kalır.

## 4.3 Sistem Marketleri ve Fiyat Kural Motoru

Tüm temel ürünler NPC marketlerinde sürekli bulunur ve standart bir referans fiyata sahiptir. Market, ürün, rol, paket, oyun süresi, tarih aralığı veya etkinlik bazında fiyat kuralı uygulanabilir.

| Kural örneği | Sonuç |
|---|---|
| MarketGroup = Temel Market; Role = Sivil | Referans fiyat |
| Package = Pro; Category = Tüketilebilir | -%5 |
| Role = Polis; Category = Polis Ekipmanı | -%20 |
| Event = Bayram; Category = Kıyafet | -%30, 72 saat |
| Region = Özel Bölge; Item = X | +%10 |
| DateRange + TimeRange | Süreli kampanya veya zam |

**Denge güvenliği:** Panel değişikliği yayımlanmadan önce etkilenen işler, sigortalar, vergi, ikinci el alt sınırı ve ilişkili fiyatlar gösterilmelidir. Her değişiklik sürümlenir ve geri alınabilir.

## 4.4 Oyuncular Arası Ticaret

| Kanal | Kurallar |
|---|---|
| Yüz yüze ticaret | İki taraflı pencere; iki taraf son durumu onaylamadan işlem tamamlanmaz |
| Online pazar | Eşya, silah, araç, ev, kaynak, işlenmiş ürün ve izin verilen nadir itemler listelenebilir |
| Fiyat alt sınırı | NPC referans fiyatının varsayılan en az **%50'si** |
| Fiyat üst sınırı | NPC referans fiyatının varsayılan en fazla **%400'ü** |
| Vergi | İlan fiyatı, referans fiyat, item grubu ve hacme göre sistem kesintisi |
| Transfer limiti | Tek işlem varsayılan **2.000.000**, günlük **10.000.000**; panelden değişir (Döküman 4.9'daki son ekonomi ölçeğine göre — tırlar 10M'a kadar çıktığı için limit buna göre büyütüldü) |
| Kullanım | Oyuncudan edinim bazı NPC erişim şartlarını aşabilir; rol/item güvenlik şartı devam eder (bkz. Döküman 3.4) |

*(Fiyat aralığı ve transfer limitleri şimdilik varsayılan olarak bırakıldı — canlıya geçmeden önce test verisiyle gözden geçirilecek.)*

## 4.5 Nüfus/Risk Katsayısı — Basitleştirilmiş Model

**Karar:** Orijinal tasarımdaki ağırlıklı ortalama (K1'e %25, K2'ye %25, K3'e %50 ağırlık) yerine **basit aritmetik ortalama** kullanılacak:

```
Kfinal = (K1 + K2 + K3) / 3
Nihai gelir = Temel fiyat × Kfinal × diğer piyasa/vergiler
```

| Aşama | Kayıt |
|---|---|
| Toplama | K1 = toplama anındaki 30 dakikalık aktif nüfus/risk katsayısı |
| İşleme | K2 = işleme anındaki 30 dakikalık aktif nüfus/risk katsayısı |
| Satış | K3 = satış anındaki 30 dakikalık aktif nüfus/risk katsayısı |

**Neden basitleştirdik:** Ağırlıklı model manipülasyona karşı teorik olarak biraz daha güçlü olsa da, MVP aşamasında anlaşılması ve panelden takip edilmesi daha kolay bir sistem tercih edildi. İleride, canlı veriyle manipülasyon testi yapıldığında gerekirse ağırlıklı modele geçiş **panelden bir seçenek olarak** eklenebilir — kod değişikliği gerektirmeyecek şekilde tasarlanacak.

## 4.6 Birbirine Bağlı İş Piyasaları

Her iş yalnız kendi fiyatını etkilemez. Admin panelinde "Piyasa İlişki Matrisi" tanımlanır.

| Olay | Örnek etki |
|---|---|
| Gümüş satışı çok arttı | Gümüş satış fiyatı düşer; yakıt ve lojistik talebi artabilir |
| Petrol üretimi düştü | Yakıt maliyeti ve taşıma işleri yükselir |
| Yasa dışı ürün arzı arttı | Kara borsa fiyatı düşer; polis ihbar olasılığı veya operasyon talebi artabilir |
| Bakır arzı azaldı | Elektronik/araç parçası gruplarının maliyeti artabilir |

*(Bu matris MVP'nin ilk dikey diliminde gerekli değil — Döküman 12'deki faz planında "MVP Core" sonrasına konumlanıyor. Şimdilik tasarım olarak not ediliyor.)*

## 4.7 Yıllık Global Wipe

**Karar: yılda bir tam sıfırlama, olduğu gibi kabul edildi.**

| Sıfırlanan | Kalıcı tarihsel kayıt |
|---|---|
| Banka/nakit, araç, ev, eşya, silah, klan varlıkları | Toplam oynama süresi ve geçmiş sezon istatistikleri |
| Aktif ekonomik piyasalar ve stoklar | Başarımlar, kozmetik rozetler ve kıdem tarihi |
| Aktif klan/alan kiraları | Audit ve yönetim geçmişi |
| Aktif ekonomik güç | Karakter kimliği; lisans/kariyer kapsamı ayrıca belirlenir |

**Wipe ilkesi:** Wipe bozuk ekonomiyi onarmak için kullanılmaz. Vergi, sigorta ve doğal giderler yıl boyunca çalışmalı; wipe yeni yarış ve içerik dönemi başlatmalıdır. (Yani wipe bir "kolaylık" değil — ekonominin yıl içinde kendi kendini dengelemesi gerekiyor, wipe sadece taze başlangıç için.)

## 4.8 Denge Felsefesi — Yapay Tavan Yok, Doğal Denge Var (derinleştirildi)

Bu oturumda üç önemli denge kararı netleşti — hepsi aynı felsefeden geliyor:

| Soru | Karar | Gerekçe |
|---|---|---|
| Aynı işi saatlerce tekrarlayan oyuncu için yapay "azalan verim" tavanı olsun mu? | **Hayır** | Zaten Kfinal = (K1+K2+K3)/3 (bkz. 4.5) çok kişi aynı işi yaparsa fiyatı doğal olarak düşürüyor. Tek oyuncu saatlerce çalışırsa zaten zaman/fırsat maliyeti öder (o süre başka iş/risk alamaz). Yapay tavan, "emek en değerli ilerleme unsurudur" ilkesiyle çelişir — emek harcayan gerçekten kazanmalı. |
| Büyük klanların (örn. 20 kişi) sayısal üstünlükle sunucuyu domine etmesine karşı sistemsel kısıtlama olsun mu? | **Hayır, doğal dengeye bırakıldı** | İki doğal fren zaten var: (1) Arma Reforger'ın gerçekçi silah/hasar sistemi — sayı avantajı taktik/beceriyi geçersiz kılmaz; (2) Klanın günlük gideri (Döküman 9.2) üye sayısıyla orantılı büyür — büyük klan büyük masraf demektir, bu da doğal bir "büyüklük vergisi" işlevi görür. |
| 3+ ay oynayan oyuncu için uzun vadeli hedef nasıl korunur? | Mevcut **Kıdemli kademe** (Döküman 3.3) yeterli kabul edildi | Prestij, koleksiyon, mentorluk, tarihsel unvan hedefleri + yıllık wipe (4.7) döngüsel bir "yeniden yarış" motivasyonu sağlıyor. |

**Genel ilke (özetle):** Sistem oyuncuyu yapay duvarlarla sınırlamaz (bu, Ana Döküman'daki "Özgürlük korunur" ilkesiyle birebir örtüşüyor). Denge; nüfus/risk katsayısı, vergi/gider yapısı ve doğal oyun mekaniği (silah sistemi, zaman/fırsat maliyeti) üzerinden **kendiliğinden** oluşur. Bu bilinçli bir tercih — eğer canlı sunucu verisiyle bu yeterli görülmezse (örn. gerçekten bir klan sunucuyu domine ediyorsa), müdahale **kural motoru üzerinden panelden** yapılır, koda yapay bir tavan gömülmez.

## 4.9 Ekonomi Ölçek Referansı (son karar — bu oturumda derinleştirildi)

Buraya kadarki tüm kurallar (K-katsayısı, vergi, ticaret aralığı) **oransaldı** — hiçbiri "1 birim para gerçekte ne kadar değerli" sorusuna cevap vermiyordu. Bu bölüm, o boşluğu somut sayılarla dolduruyor.

### Temel Felsefe: Pasif Maaş = Güvenlik Ağı, İş = Gerçek İlerleme

> Pasif maaşın amacı, oyuncu her şeyini kaybetse bile yeniden başlayabileceği bir "dip nokta" garantilemektir — birincil ilerleme yolu değildir. Aktif iş yapmak her zaman çok daha hızlı ilerletmelidir. Bu ayrım, Ana Döküman'daki "Emek en değerli ilerleme unsurudur" ilkesinin doğrudan sayısal karşılığıdır.

### Temel Sayılar (kesinleşti — panelden ayarlanabilir varsayılan)

| Kalem | Değer |
|---|---|
| Başlangıç bakiyesi (yeni karakter) | **500.000** |
| Araç lisansı fiyatı (sürücü lisansı) | **250.000** *(bkz. Döküman 3.4 — lisans sistemi kalıcılık kuralı ve diğer lisans fiyatları)* |
| Pasif vatandaşlık maaşı (sivil, 15 dk başı) | **2.500** (saatte 10.000) |

### Araç Fiyat Kademeleri (kesinleşti)

| Sınıf | Fiyat |
|---|---|
| Motor | 100.000 |
| Eski/püskü araba | 250.000 |
| Sıradan araba | 500.000 |
| Bagajlı araba | 750.000 |
| Minibüs | 1.250.000 |
| Tır (5 alt sınıf) | 1.500.000 — 10.000.000 arası kademeli |

*Araç ve tır satış noktaları ayrı lokasyonlarda olacak (bkz. Döküman 7). Bagaj kapasitesi ve hız gibi özellikler fiyat/sınıfa göre değişecek.*

**Gün 1 deneyimi (kasıtlı tasarım):** Yeni oyuncu başlangıç parasıyla (500.000) sürücü lisansını (250.000) alıp kalan tam 250.000 ile eski/püskü arabayı (250.000) karşılayabilir — yani **araçsız kalmaz**, ilk günden itibaren hareket edebilir, hiç iş yapmadan. Sonraki araç kademeleri gerçek bir ilerleme hedefi olur.

### İş Geliri (kesinleşti — pasif maaştan belirgin şekilde yüksek)

| İş ölçeği | 1 sefer kazancı | Örnek işler |
|---|---|---|
| Orta ölçekli (araba-sınıfı işler) | **60.000** | Pirinç, tarım, balıkçılık, bakır |
| Büyük/tır ölçekli (tır-sınıfı işler) | **120.000** (aracaya göre tam 2 kat — kesin karar) | Petrol, taş/mermer, gümüş |

**Karşılaştırma (neden iş kazanır):** Pasif maaşla sıradan arabaya (500.000) ulaşmak ≈ **2 hafta** sürer (haftada ~250.000 pasif ≈ 10.000/saat × ~25 saat). Aktif çalışan biri aynı hedefe birkaç saatlik yoğun oyunla (yaklaşık 8–9 orta ölçek sefer) ulaşır. İş her zaman çok daha hızlı.

### Standart Oyuncu Personası ve Zaman Çizelgesi

Günde ~4-5 saat, haftada 5 gün → haftalık ~20-25 saat aktif süre (Döküman 3'teki kademe sınırlarıyla uyumlu).

| Zaman | Kademe | Beklenen kazanım |
|---|---|---|
| Gün 1 | Yeni Oyuncu başlangıcı (0 saat) | Lisans + eski/püskü araba (başlangıç parasıyla, iş yapmadan) |
| Gün 2-5 | Yeni Oyuncu (~4-16 saat) | Birkaç iş seferiyle sıradan/bagajlı araba |
| **Hafta 1 sonu (~20-25 saat)** | Yeni Oyuncu biter → Yerleşik | Sıradan/bagajlı araba oturmuş; **ilk küçük tır (1.5M) mümkün** (yoğun iş + birikim) |
| Ay 1-2 (~80-150 saat) | Yerleşik → Gelişmiş | Üst segment araç/tır, ev/garaj, polis/doktor girişi |
| Ay 5-6 (~500 saat) | Gelişmiş sonu → Kıdemli eşiği | İçeriğin %70'i açık, üst segment tırlara (10M'a yaklaşan) yaklaşıyor, klan liderliği |
| Ay 10-12+ (~1000+ saat) | Kıdemli | Prestij hedefleri, mentorluk, wipe ile yeniden yarış |

> **Düzeltme notu:** Önceki taslakta "Hafta 2-3'te Yeni Oyuncu biter" yazıyordu — bu, Döküman 3'teki 0-20 saat kademe sınırıyla çelişiyordu (senin personana göre haftada 20-25 saat oynanıyor, yani 20 saat sınırı 2-3 hafta değil, **1 hafta içinde** doluyor). Düzeltildi.
>
> ~~**Ayrıca fark edilen bir tutarsızlık:** Döküman 3.1'deki yeni oyuncu koruma süresi 24 aktif saat, ama ilerleme kademesi sınırı 20 aktif saatti.~~ **Çözüldü:** Koruma süresi 20 aktif saate düşürülerek kademe sınırıyla eşitlendi (bkz. Döküman 3.1) — artık koruma ve "Yeni Oyuncu" içerik kademesi birlikte, aynı anda bitiyor.

### Vatandaşlık Maaşı — Uygulama Detayları

- VIP/donate paketi alan oyuncularda bu maaşa bir çarpan uygulanabilir (örn. x1.25 / x1.5, pakete göre panelden ayarlanır).
- **Botlama/istismar koruması:** Maaş yalnızca oyuncu gerçekten çevrimiçi ve **AFK-koruma mekanizması tarafından aktif kabul edildiği** sürece işler (bkz. Döküman 3.1, "aktif kabul edilen oturum dakikaları" — yeni oyuncu koruma süresi hesaplamasında kullanılan mantığın aynısı). AFK kalan oyuncu zaten oyundan atılıyor, dolayısıyla maaş almaya devam edemez.
- Polis/Doktor maaşı, sivil pasif maaşın üzerine rütbeye göre kademeli olarak artan ayrı bir tablo izler — bkz. Döküman 8.

## 4.10 Açık Maddeler (birleştirildi)

- [x] ~~Transfer limitleri~~ — son haliyle **2.000.000 / günlük 10.000.000** (araç/tır ölçeği 10M'a çıktığı için son kez büyütüldü — bkz. 4.4)
- [ ] Piyasa İlişki Matrisi (4.6) — MVP kapsamı dışında, ileride detaylandırılacak

---
*Önceki: [03 - Yeni Oyuncu & İlerleme](./03_yeni_oyuncu_ilerleme.md) · Sıradaki: Döküman 5 — İşler & Temel İhtiyaçlar/Envanter*
