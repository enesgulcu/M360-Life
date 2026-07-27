# M360 Life — Ana Döküman (Master / İndeks)

> Bu dosya canlıdır — proje ilerledikçe güncellenir. Diğer 14 döküman burada listelenir; detaylar kendi dosyalarında tutulur, burada tekrarlanmaz.

## Döküman Kontrolü

| Alan | Değer |
|---|---|
| Proje | M360 Life — Arma Reforger için modüler, sistem tarafından yönetilen Life ekosistemi |
| Marka/Stüdyo adı | **Geçici**: "Medyanes 360 / M360 Life" — kesinleşmedi, ileride değişebilir |
| Kaynak döküman | M360 Life Ana Proje Tasarım ve Uygulama Dokümanı v2.0 (23 Temmuz 2026) |
| Bu sürüm | Parçalanmış döküman seti — v1 |
| Ekip | Tek kişi (sen) + AI teknik ortak (şimdi **Cursor + enfusion-mcp**; önceki Claude Code oturumları) |
| İlk hedef | Kendi/topluluk sunucusu; lisans satışı ileride ikinci aşama |
| Geliştirme ortamı | Başlangıçta local cihaz (test/deneme); büyüdükçe uygun hosting'e taşınacak |

---

## 1. Ürün Vizyonu

### Çalışma Protokolü (yeni sohbette — Cursor + enfusion-mcp)

Bu proje, kullanıcı ile AI arasında şu şekilde yürüyor:

- **Kullanıcı yönetici**, kararları o verir; AI teknik ortak/rehber — sorar, seçenek sunar, onay ister.
- Kullanıcı scripting öğrenmez — AI kod yazar, Türkçe adım adım rehber verir; kullanıcı Play sonucu / hata aktarır.
- Belirsizlikte AI varsaymaz, sorar.
- Enfusion/Reforger’da **resmi BI dökümantasyonu** birincil kaynak.
- **EveronLife** / **Narcos Life** sürekli referans (asla dependency değil) — Döküman 14.
- Kod isimlendirme **ANA DÜSTUR:** değiştirilebilir her şey Türkçe ASCII; motor API dokunulmaz — **11.2.1**.
- Kanıtlanmış bulgular **Döküman 15**’e yazılır.
- Ortam: **Cursor + enfusion-mcp** (önceki Claude Code oturumları arşiv). GitHub: `enesgulcu/M360-Life`.

**Hızlı devam:** `docs/00`–`14` + **`15`** + kök `README.md` okunur; konuşma geçmişi gerekmez.

M360 Life; para kazanma, araç kullanma ve çatışma eklentilerinin yan yana getirildiği bir mod değildir. Oyuncu ekonomisini, rol geçişlerini, kayıpları, kamu hizmetlerini, klanları ve yönetim kurallarını **aynı veri ve yetki omurgasında** çalıştıran bir Life platformudur.

Temel fark: yazılı kurallara mümkün olduğunca oyun içi karşılık üretmek. Sistem oyuncuyu sürekli cezalandırmak yerine önce bilgilendirir, sonra caydırır; yalnızca kesin ölçülebilen kötüye kullanımlarda sert engel uygular. İnsan yöneticiler rutin düzenin taşıyıcısı değil, istisnai olayların hakemi olur.

**Başarı tanımı (ilk milestone):** Oyuncu yerel dedicated sunucuya bağlanır, profili PostgreSQL'den yüklenir, bir iş yapar, ürün satar, araç satın alır; çıkıp yeniden girdiğinde para ve aracı korunur — tüm bunlar iki oyuncu aynı anda işlem yaptığında çoğaltma (dupe) açığı üretmeden çalışır.

---

## 2. Değişmez İlkeler

| İlke | Karşılığı |
|---|---|
| Emek en değerli ilerleme unsurudur | Para tek başına en üst erişimleri açmaz; süre, lisans, rol ve koşullar birlikte değerlendirilir |
| Sunucu yetkilidir | İstemci hiçbir kritik para/eşya/araç/rol sonucunu kendisi belirleyemez |
| Özgürlük korunur | Sistem her davranışı duvarla engellemez; mümkünse uyarı ve sonuç üretir |
| Modülerlik zorunludur | İşler, marketler, garaj, rol noktaları prefab + config olarak tekrar kullanılabilir |
| Koda dokunmadan yönetim | Fiyat, süre, kota, lisans, vergi, sigorta kuralları admin panelinden değişir |
| Performans baştan düşünülür | Yalnızca aktif entity'ler dünyada tutulur |
| Manipülasyon varsayılır | Her ekonomik/yetkisel özellik kötüye kullanılmaya çalışılacakmış gibi tasarlanır |
| Harita bağımsız çekirdek | Everon ilk dünya olsa da sistemler yeni haritaya taşınabilir |
| Kapalı kaynak ve lisanslı dağıtım | Kritik çekirdek stüdyo kontrolünde kalır; süreli lisans + sunucu doğrulaması |

**Tasarım sınırı:** Bir mekanik oyun kararını zenginleştirmiyor, eğlenceyi artırmıyor veya ekonomiyi korumuyorsa MVP'ye alınmaz.

---

## 3. Öncelik Sırası

| Alan | Öncelik | Not |
|---|---|---|
| Temel oynanabilirlik | 10/10 | Doğma, HUD, lisans, iş, market, banka, araç, envanter, temel kurallar |
| Ekonomi | 10/10 | Para defteri, fiyat kural motoru, vergi, nüfus katsayısı, anti-dupe çekirdeği |
| Araçlar | 10/10 | Satın alma, sahiplik, garaj, paylaşım, kiralama, kayıp |
| Oyuncu paneli / HUD | 10/10 | Tüm sistemlerin görünen yüzü |
| Polis / Doktor / Rebel | **9/10** *(güncellendi)* | ~~Core MVP sonrası~~ → **MVP Core'a alındı** (Döküman 8 kararı) |
| Klan | **9/10** *(güncellendi)* | ~~Kapalı alfa aşaması~~ → **MVP Core'a alındı** (Döküman 9 kararı) |
| Admin paneli / Güvenlik / Global altyapı | 8/10 | Mimaride baştan var; işlevsel kapsam aşamalı |
| Mülk | 4/10 | Ev/garaj başlangıç; işletme sistemi ertelendi |

> **Düzeltme notu:** Bu tablo, orijinal dökümandan kalan eski önceliklendirmeyi yansıtıyordu. Sonradan Polis/Doktor ve Klan'ı MVP Core'a almıştık (bkz. Döküman 8, 9) ama bu tablo hiç güncellenmemişti — çelişki fark edilip düzeltildi.

---

## 4. Kesinleşen Kararlar (özet)

- Türkiye temalı **kurgusal** ülke; ilk harita **Everon**.
- Tek karakter kimliği, benzersiz ad; görünüm/cinsiyet sonradan değiştirilemez.
- Sivil varsayılan rol; polis/doktor koşulla açılır; rebel, sivil içindeki bir lisans/kariyer dalı.
- Rol bazlı ayrı konum/envanter/kıyafet durumu (tek hesap, ayrı rol anlık görüntüsü).
- 20 aktif saat yeni oyuncu koruması (kademe sınırıyla eşitlendi); tek yönlü erken çıkış mümkün.
- Genel level sistemi yok; aktif süre + para + lisans + rol + bağlam birlikte erişimi belirler.
- Kapasite tabanlı envanter (ağırlık/slot değil, puan sistemi).
- NPC sabit referans fiyat; oyuncu ticareti varsayılan %50–%400 aralığında.
- İşler **üç site** + `M360_IsAyar` (Türkçe ASCII class’lar) — kod değil config.
- Yılda bir tam global wipe.
- Sınırsız araç/ev sahipliği; yalnızca dünyadaki aktif entity sayısı yönetilir.
- Polis kotası aktif nüfusun max %20'si, doktor %15'i (ihtiyaç eşikleri %10/%5 başlangıç).
- Next.js fullstack + PostgreSQL yerelde geliştirilecek; Redis ölçek/dağıtık koruma gerektiğinde eklenecek.
- Kapalı kaynak, **yıllık lisans modeli** (3/6/12 aylık seçenekten yıllık tek pakete sadeleştirildi) — ama önce kendi sunucumuzda kanıtlanacak.
- Karakter oluşturma sadeleştirildi: yalnızca ad-soyad + spawn noktası seçimi; doğum yeri alanı kaldırıldı.
- İlerleme kademeleri 6'dan 4'e indirildi: Yeni Oyuncu (0-20s) → Yerleşik (20-150s) → Gelişmiş (150-500s) → Kıdemli (500+s).
- Nüfus/risk katsayısı basit ortalamaya çevrildi: Kfinal = (K1+K2+K3)/3.
- **MVP Core kapsamı 3 kez genişledi:** iş sayısı 3→15 (10 yasal+5 yasa dışı), Polis/Doktor MVP Core'a alındı, Klan MVP Core'a alındı. Bu, roadmap süresini esnek/belirsiz bırakıyor (bkz. Döküman 12).
- Sigorta MVP'de sadece Sağlık + Araç; Silah/Ev/Hırsızlık sigortası Kapalı Alfa/Beta'ya ertelendi.
- PostgreSQL geliştirme ortamında Docker'sız, native kurulum ile çalışacak.
- Git/GitHub kullanılacak.
- **İş sistemi mikro-mekaniği netleşti:** toplama (tick-tabanlı, alet gerektirir, alan sınırsız/herkese açık) → işleme (kişiye özel oturum, batch sınırlı, makine paylaşılan state tutmaz) → satış (anlık). Detay Döküman 5.
- **Denge felsefesi netleşti:** yapay tavan/kısıtlama yok; denge nüfus/risk katsayısı, klan gideri ve doğal oyun mekaniğiyle kendiliğinden oluşur. Detay Döküman 4.8.
- **Somut ekonomi ölçeği kesinleşti:** başlangıç parası 500.000, pasif vatandaşlık maaşı 2.500/15dk (güvenlik ağı rolü), iş geliri 60.000-120.000/sefer (aktif çalışmanın pasif maaştan bariz üstün olması için), araç fiyatları 100.000'den (motor) 10.000.000'a (üst tır) kademeli. Detay Döküman 4.9, 5, 7, 8.
- **Oyun içi HUD teknik yolu kesinleşti:** Life tarzı kenar HUD’lar `.layout` + ikon + script ile; HTML değil. Admin web paneli ayrı (Next.js). Detay Döküman 10.8.

---

## 5. Henüz Netleşmemiş / Karar Bekleyen Konular

Bunlar ilgili döküman yazılırken tek tek karara bağlanacak — şimdiden not düşülüyor ki unutulmasın:

- [ ] Stüdyo/marka ismi kesinleşmedi ("Medyanes 360 / M360 Life" geçici) — kritik değil, ileride değişebilir
- [ ] Kurgusal ülke ismi — bilinçli olarak yer tutucu bırakıldı, panelden de değiştirilebilir (para birimi ismi onaylandı: "M360 Lirası")
- [ ] Hosting/sunucu sağlayıcısı — local'den sonra karar verilecek (bilinçli erteleme)
- [x] ~~Görsel/asset üretim planı~~ — netleşti: MVP'de orijinal Arma asset'leri + küçük dış müdahaleler (Döküman 1.5)
- [x] ~~Oyun içi lisans fiyatlandırma modeli~~ — netleşti: bir kez alınır, wipe'a kadar geçerli, iş/role göre değişken fiyat (Döküman 3.4)
- [ ] Ticari (sunucu satış) yıllık lisans fiyatı — bilinçli olarak açık bırakıldı, tamamen kullanıcının ticari kararı (Döküman 12.2)
- [x] ~~Enfusion prefab/component teknik prosedürü~~ — Döküman 13 oluşturuldu

---

## 6. Döküman Haritası

| # | Döküman | Durum |
|---|---|---|
| 0 | Ana Döküman (bu dosya) | ✅ Hazır |
| 1 | [Dünya, Harita & İçerik Paket Stratejisi](./01_dunya_harita_icerik.md) | ✅ Hazır |
| 2 | [Hesap, Karakter & Rol Durumu](./02_hesap_karakter_rol.md) | ✅ Hazır |
| 3 | [Yeni Oyuncu Deneyimi & İlerleme Motoru](./03_yeni_oyuncu_ilerleme.md) | ✅ Hazır |
| 4 | [Ekonomi & Piyasa Motoru](./04_ekonomi_piyasa.md) | ✅ Hazır |
| 5 | [İşler & Temel İhtiyaçlar / Envanter](./05_isler_envanter.md) | ✅ Hazır |
| 6 | [Sağlık, Ölüm & Sigorta](./06_saglik_olum_sigorta.md) | ✅ Hazır |
| 7 | [Araç & Mülk Sistemi](./07_arac_mulk.md) | ✅ Hazır |
| 8 | [Roller (Sivil/Polis/Doktor/Rebel) & Çatışma Kuralları](./08_roller_catisma.md) | ✅ Hazır |
| 9 | [Klan Sistemi](./09_klan.md) | ✅ Hazır |
| 10 | [Oyuncu Paneli, HUD & Admin Panel](./10_panel_hud_admin.md) | ✅ Hazır |
| 11 | [Teknik Mimari (Arma Reforger + Web)](./11_teknik_mimari.md) | ✅ Hazır |
| 12 | [Lisans, Ticari Model & Roadmap](./12_lisans_roadmap.md) | ✅ Hazır |
| 13 | [Enfusion Prefab & Script Yapılandırma Prosedürü](./13_enfusion_prefab_prosedur.md) | ✅ Güncel — **üç site** model (2026-07-27) |
| 14 | [Mevcut Modlar, Framework'ler & Geliştirme Araçları](./14_mevcut_modlar_araclar.md) | ✅ Güncel — enfusion-mcp = Cursor |
| 15 | [Geliştirme Notları (AI oturum belleği / kanıtlanmış bulgular)](./15_gelistirme_notlari.md) | ✅ Canlı — her oturumda güncellenir |

---

## 7. Riskler (üst seviye — detay Döküman 12'de)

| Risk | Önlem |
|---|---|
| Kapsam büyümesi | Faz kapısı; MVP dışı backlog; "bir dikey dilim" kuralı |
| Tek geliştirici bağımlılığı | Bu döküman seti + modüler kod + Git ile sürdürülebilirlik |
| Ekonomi exploiti | Ledger, transaction, idempotency, provenance, audit (Döküman 11) |
| Aşırı otomasyon / oyuncu özgürlüğünün azalması | Önce uyarı, sonra caydırma; kesin engeller az |

---

*Sıradaki adım: tasarım seti tamam — lab/ürün için [15](./15_gelistirme_notlari.md) + kök [README](../README.md).*
