# Döküman 6 — Sağlık, Ölüm, Kayıp & Sigorta

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 6.1 Baygınlık ve Ölüm Akışı

1. Oyuncu ağır yaralandığında baygın duruma geçer; bu sırada eşyaları alınamaz.
2. İlk **2 dakika** "öl" seçeneği kapalıdır; doktor müdahalesi için minimum fırsat tanınır.
3. İki dakika sonra oyuncu isterse doktoru sınırsız süre bekleyebilir veya ölerek respawn ekranına geçebilir.
4. Oyuncu bağlantıyı baygın/çatışma durumunda keserse combat-log ve ölüm kuralları uygulanır.
5. Ölüm gerçekleştiğinde fiziksel envanter yere düşer; kimliksel veriler ve banka hesabı korunur.
6. Oyuncu aktif role uygun doğma noktasını seçerek devam eder (bkz. Döküman 2.3).

## 6.2 Ceset ve Eşya Erişimi

| Konum | Süre | Erişim |
|---|---|---|
| Güvenli bölge | 30 dakika | Yalnızca eşya sahibi; süre sonunda silinir |
| Güvenli bölge dışı | 15 dakika | Tüm oyuncular; süre sonunda silinir |
| Baygın oyuncu | Sınırsız bekleme boyunca | Kimse envanteri alamaz |
| Kimliksel itemler | Düşmez | Profil verisi olarak kalır |

## 6.3 Sigorta — MVP Kapsamı (daraltıldı)

**Karar:** MVP'de yalnızca **Sağlık** ve **Araç** sigortası uygulanacak. Silah, Ev ve Hırsızlık/Yük sigortası tasarımsal olarak bu dökümanda tutulur ama **geliştirme sırası Kapalı Alfa/Beta'ya** ertelenir (bkz. Döküman 12).

| Sigorta | Durum | Temel mantık |
|---|---|---|
| **Sağlık** | ✅ MVP | Günlük sabit prim **1.000**; hastane/tedavi maliyetinin **%50'sini** karşılar (ilk varsayım) |
| **Araç** | ✅ MVP | Prim: aracın değerinin **günlük %1'i**; patladığında sabit **%60 oranında** değer iadesi yapılır (ilk varsayım) |
| Silah | ⏳ Sonraya ertelendi | Aylık prim; kayıp başına azalan para iadesi (%50, %48, %46...; 24 saatte yenilenir) |
| Ev | ⏳ Sonraya ertelendi | Baskın/hasar gibi kayıpların bir bölümünü azaltır |
| Hırsızlık / yük | ⏳ Sonraya ertelendi | Kayıp eşyayı çoğaltmadan, soygunda alınabilir miktarı veya net zararı sınırlar |

*(Prim kesintisi, Döküman 4.2'deki günlük vergi döngüsüyle aynı anda işler — tek bir günlük "kesinti turu" hem vergi hem sigorta primini kapsar, teknik olarak tek bir zamanlanmış görev.)*

## 6.4 Sigorta Suistimalini Önleyen Model

Sigorta, oyuncunun arkadaşıyla anlaşarak malı "çaldırıp" hem ürünü hem tazminatı kazanmasına izin vermemelidir. Bu nedenle farklı sigorta türleri aynı biçimde para basmaz. Bu ilkeler **hem MVP'deki sağlık/araç sigortası hem de ileride eklenecek diğer türler için geçerli** olacak referans kurallardır:

| Risk | Koruma |
|---|---|
| Hırsızlıkta ürün + tazminat çoğalması | Sigorta para basmak yerine sigortalı oranı soygunda korunan miktar olarak uygular; yalnızca sigortasız kısım transfer edilir |
| Arkadaş/klan ile organize kayıp | Aynı klan, takım, araç anahtarı, yakın transfer geçmişi, aynı cihaz/ağ örüntüsü ve tekrarlı karşılaşmalar claim risk puanı üretir |
| Araç patlatma | Son sürücü, hasar kaynağı, taraf ilişkisi ve tekrar oranı kaydedilir; şüpheli claim beklemeye alınır |
| Çifte ödeme | Her kayıp olay ID'sine tek claim; idempotency ve transaction ile ikinci ödeme reddedilir |

**Sigorta felsefesi:** Sigorta kaybı yok etmez; ağır darbenin bir kısmını yumuşatır. Uzun vadede düzenli oynayan riskli oyuncu için anlamlı, sistem için sürdürülebilir olmalıdır.

## 6.5 Araç Kaybı Detayı (Döküman 7 ile bağlantılı)

Araç patladığında sahiplik kaydı silinir. Sigorta yoksa araç ve değer kaybolur. Sigorta varsa olayın niteliği ve poliçe oranına göre para iadesi yapılır. Araç hasarlı fakat patlamamışsa geçici reset veya garaj çağrısıyla geri gelebilir; bu davranış panelden belirlenir. (Detaylı araç mekanikleri Döküman 7'de.)

---
*Önceki: [05 - İşler & Envanter](./05_isler_envanter.md) · Sıradaki: Döküman 7 — Araç & Mülk Sistemi*
