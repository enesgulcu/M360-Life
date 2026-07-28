# Döküman 12 — Lisans, Ticari Model & Roadmap

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 12.1 Harici Mod Kullanım İlkesi

| Kategori | Politika |
|---|---|
| Ekonomi, hesap, sahiplik, güvenlik çekirdeği | Mümkün olduğunca stüdyo tarafından geliştirilir; dış moda kritik bağımlılık verilmez |
| Araç, kıyafet, bina, animasyon, ses ve dekor | Lisansı ve performansı uygunsa harici içerik kullanılabilir |
| Mağaza/banka/garaj sistemleri | Kod ve mimari referans olarak incelenir; lisans ve bağımlılık uygunsa adaptasyon değerlendirilir |
| Kapalı veya ND lisanslı mod | Kaynak kopyalanmaz/değiştirilmez; yalnızca izin verilen kullanım çerçevesi |
| Eski veya bakımsız mod | Kritik çekirdeğe bağlanmaz; test ve alternatif plan gerekir |

**Kaynak kullanımı ilkesi:** Workshop örnekleri (BasicBanking, Shop System, Zeliks Persistent Banking vb.) hazır sistemi doğrudan kopyalama kararı değil — fikir/mimari referansı olarak incelenecek. Lisans, kaynak erişimi, güncellik, bağımlılık ve ticari kullanım hakkı kontrol edilmeden hiçbir mod ürün çekirdeğine alınmayacak.

## 12.2 Ticari Lisans Modeli — Güncellendi: Yıllık Tek Paket

**Karar:** Orijinal tasarımdaki 3/6/12 aylık çoklu paket seçeneği yerine **tek, yıllık lisans modeli** benimsendi. Basitlik hem satış/pazarlama hem de teknik doğrulama (lisans kontrol servisi) tarafında avantaj sağlıyor — tek bir "süre" mantığı kodlanacak, çoklu paket karmaşıklığı yok.

- Çekirdek kaynak kodu stüdyo kontrolünde kapalı tutulur.
- Sunucu sahiplerine **yıllık lisans paketi** sunulur.
- Lisans; sunucu kimliği, bitiş tarihi ve izin verilen modüllerle doğrulanır.
- Müşteri kendi fiyat ve oyun kurallarını yönetir; çekirdek lisans/doğrulama ve güncelleme mekanizmasında stüdyoya bağlı kalır.
- Uzun vadede ortak kullanıcı hesabı kullanılabilir; her sunucunun karakteri, ekonomisi ve varlıkları ayrı tenant/server alanında tutulur.

> **Not (kesinleşti):** Ticari yıllık lisans fiyatı bilinçli olarak açık bırakıldı — bu tamamen senin ticari kararın, ürün hazır olmaya yaklaştıkça (Beta öncesi) piyasa koşullarına göre sen belirleyeceksin. Dökümana kilitlenmiş bir sayı konmadı.

## 12.3 Geliştirme Yol Haritası — Güncellenmiş Faz Yapısı

**Önemli:** Bu sürümde üç kapsam genişlemesi oldu (Döküman 5 — 15 iş, Döküman 8 — Polis/Doktor MVP Core'da, Döküman 9 — Klan MVP Core'da). Bu yüzden faz sürelerini **kesin taahhüt olarak değil, gösterge olarak** işaretliyorum — sen de "şimdilik önemli değil, ileride konuşuruz" dedin, bu doğru bir yaklaşım. Faz *sırası* ve *kapsamı* önemli olan; süre tahminleri ilerledikçe gerçekçileşecek.

| Faz | İçerik | Süre göstergesi |
|---|---|---|
| **0 — Yerel Lab** | Addon, Git, junction senkron, Neon+Vercel `api`, ilk RestApi çağrısı, dedicated ilk çalıştırma | Gösterge: 2-4 hafta |
| **1 — Life Core** | Karakter, hesap, rol state, aktif süre/AFK ölçümü, banka/ledger, temel HUD, admin panel ilk config, rate limit/idempotency/audit altyapısı | Gösterge: 4-8 hafta |
| **2 — İlk Dikey Dilim** | Bağlan→doğ→ATM→lisans→iş yap→sat→market→araç al→garaja koy→restart sonrası korunur | Gösterge: 6-10 hafta |
| **3 — MVP Core (genişletilmiş)** | 15 iş, kapasite envanteri, sağlık+araç sigortası, araç+kiralama+garaj, **Polis/Doktor rütbe ve çağrı sistemi**, **Klan (kuruluş, gider, kiralık nokta)**, saldırgan profil sistemi | Gösterge: orijinal 4-6 aydan daha uzun — kapsam büyüdüğü için gerçekçi süre ilerledikçe netleşecek |
| **4 — Kapalı Alfa** | Rebel/kara borsa, online pazar, gelişmiş vergi, 10-30 oyunculuk testler, sigorta anti-fraud testleri, ertelenen sigorta türleri (silah/ev/hırsızlık) | Gösterge: orijinal plana yakın |
| **5 — Beta / Lisanslı Ürün** | 50-100 oyunculuk performans, Türkiye içerik paketi (görseller), klan noktaları tam sürüm, config sürümleme/geri alma, yıllık lisans mekanizması, ikinci haritaya prefab taşıma kanıtı | Gösterge: 12-18+ ay |

## 12.4 MVP Kabul Kriterleri (değişmedi — bunlar teknik doğruluk testleri)

| Test | Başarı ölçütü |
|---|---|
| Çift satın alma | Aynı idempotency key veya çift tıklama yalnız bir kez para düşürür |
| Eş zamanlı araç spawn | Aynı park/slot iki araca verilmez |
| Restart | Profil, para, lisans, iş partisi ve araç sahipliği geri yüklenir |
| Yetkisiz istemci | Sahte para, rol ve item talebi reddedilir |
| Rol durumu | Sivil rol konum/envanteri kaybolmadan saklanır |
| İş akışı | Toplama-işleme-satış kesintisiz ve iptal senaryosuyla çalışır |
| API kesintisi | Para/item kaybı veya çoğalma olmadan kontrollü hata oluşur |
| Log | Her kritik işlem tek olay ID'siyle admin panelinde bulunur |

Bu kriterler, kapsam ne kadar genişlerse genişlesin **değişmiyor** — "kaç iş var" büyür ama "restart sonrası veri kaybolmasın" testi her zaman aynı sıkılıkta kalıyor. Bu, kapsam büyümesinin kalite güvencesini gevşetmediğinin garantisi.

## 12.5 Ana Riskler ve Yönetim Planı

| Risk | Etkisi | Önlem |
|---|---|---|
| Kapsam büyümesi (bu sürümde 3 kez oldu) | MVP'nin bitmemesi | Faz kapısı; MVP dışı backlog; bir dikey dilim kuralı — **Döküman 12'nin kendisi bu takibi yapacak dosya** |
| Multiplayer replication hataları | Client/server tutarsızlığı | İlk günden dedicated test; 1-2-10-30-64-100 oyuncu basamağı |
| Ekonomi exploiti | Para/item çoğalması | Ledger, transaction, idempotency, provenance ve audit (Döküman 11) |
| Harici mod kırılması | Güncellemede sunucu açılmaması | Kritik çekirdekte bağımlılık azaltma; lisans ve sürüm matrisi |
| Aşırı otomasyon | Oyuncu özgürlüğünün azalması | Önce uyarı, sonra caydırma; kesin engeller az |
| Yıllık ekonominin erken doyması | Son aylarda hedef kalmaması | Servet/varlık gideri, klan maliyeti, piyasa ilişkileri, içerik takvimi |
| Log hacmi | DB ve disk maliyeti | Olay bazlı detay, koordinat örnekleme, arşiv/retention |
| Tek geliştirici bağımlılığı | Gelişimin yavaşlaması | Bu döküman seti + modüler kod + Git + AI ortaklığı |

**Sorumluluk netleştirmesi (bu oturumda kesinleşti):**
- **Test/kalite kontrol:** Kullanıcı ve Claude birlikte yapacak — ayrı bir ekip/QA süreci kurulmayacak, Döküman 12.4'teki MVP kabul kriterleri ikisinin birlikte kontrol edeceği liste olarak kullanılacak.
- **Pazarlama/topluluk büyütme:** Tamamen kullanıcının sorumluluğu, en son öncelik — önce ürün çalışır hale gelmeli.

## 12.6 Bir Sonraki Somut Adım

Tasarım dökümanları (0-12) tamamlandı. Sıradaki adım Döküman **11.12**’de belirtilen: **dedicated server’ı ilk kez yerel olarak çalıştırmak** (11.11 log politikasıdır — karıştırma).

---
*Önceki: [11 - Teknik Mimari](./11_teknik_mimari.md) · Sıradaki: Döküman 13 — Enfusion Prefab & Script Yapılandırma Prosedürü*
