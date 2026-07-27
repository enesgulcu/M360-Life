# Döküman 1 — Dünya, Harita & İçerik Paket Stratejisi

*Bağlı olduğu: [00 - Ana Döküman](./00_ana_dokuman.md)*

## 1.1 İlk Dünya: Everon

İlk sunucu **Everon** üzerinde kurulur. Everon'un mevcut yerleşimleri, yolları ve doğal bölgeleri; Türkiye temalı kurgusal kurumlar, tabela, güvenlik bölgeleri, iş merkezleri, hastaneler, polis noktaları ve ticari alanlarla yeniden yorumlanır.

"Türkiye temalı" yaklaşımı şu anlama gelir: gerçek kurumları/markaları bire bir kopyalamak değil, tanıdık görsel dili ve çalışma mantığını kurgusal bir ülke içinde kullanmak — hem özgünlük hem hukuki güvenlik için.

## 1.2 İsimlendirme (Yer Tutucu — henüz kesinleşmedi)

| Öğe | Değer | Not |
|---|---|---|
| Ülke/bölge adı | *(yer tutucu — bilinçli olarak ertelendi)* | Kritik değil, ileride admin panelden de değiştirilebilir bir alan; isim netleşince tüm dosyalarda güncellenecek |
| Para birimi | **"M360 Lirası" (kısaltma: ML veya ₺)** — onaylandı | Gerçekçi ama nötr; Türk Lirası'na görsel/işlevsel olarak yakın, isim olarak kurgusal. Panelden değiştirilebilir alan olduğu için kritik bir karar değil. |

**Kurum isimlendirme kararı (netleşti):** Özel kurgusal isimler uydurulmayacak. Kurumlar doğrudan işlevsel adla anılacak: *Polis Karakolu, Hastane, Belediye, ATM, Market* vb. İleride bölgesel/lokasyon prefixi eklenebilir (örn. "Altis Karakolu", "Merkez Hastanesi") — bu, harita üzerindeki her prefab'ın `region_name` config alanına bağlı olacağı için tasarımsal bir sorun çıkarmaz, istenildiğinde admin panelden eklenir.

## 1.3 Harita Bağımsız Prefab Kuralı

Bu kural, projenin en önemli mimari kararlarından biri — hem oyun içi hem web tarafını etkiliyor:

- Her market, ATM, galeri, garaj, iş toplama alanı, işleme tesisi, satış noktası, spawn noktası ve kamu terminali **bağımsız bir prefab** olarak tamamlanır.
- Prefab'ın iş mantığı (script) sabittir; **konum, ürün listesi, rol şartı ve fiyatlar config üzerinden** değişir.
- Harita değiştirilirse (örn. ileride başka bir haritaya geçilirse) çekirdek script yeniden yazılmaz — prefablar yeni dünyaya yerleştirilip config güncellenir.
- Haritaya özel değişiklikler (Everon'a özgü konumlandırma, Türkiye paketi görselleri) ayrı bir **"World Pack"** içinde tutulur; **Life Core** (oynanış mantığı) ile karıştırılmaz.

Bu ayrım şunu sağlıyor: Life Core = "nasıl çalışır" (kod), World Pack = "nerede ve nasıl görünür" (config + asset). İleride başka bir harita veya başka bir ülke temasıyla lisans satarken bu ayrım kritik olacak.

**Türkiye paketi (World Pack içeriği):** polis aracı kaplamaları, ambulans tasarımı, üniforma, tabela, para birimi görseli, kurum adları ve çevre dekorasyonu.

## 1.4 Bölge Sınıfları

| Bölge | Kurallar | Hedef oyuncu |
|---|---|---|
| Güvenli bölge | Ateş/hasar kapalı; soygun ve zorla rol yok; yeni oyuncu eğitimi ve temel ticaret | Yeni ve düşük risk isteyen oyuncu |
| Kontrollü şehir / ticari bölge | RP başlatma gerekir; polis ve kamera riski yüksek | Tüm seviyeler |
| Riskli bölge | Daha yüksek gelir; polis bildirimi, soygun ve pusu ihtimali | Orta ve gelişmiş oyuncu |
| Sürekli RDM/KOS bölgesi | Rol başlatmadan çatışma serbest | Çatışma isteyen oyuncu |
| Geçici etkinlik / büyük soygun | Olay süresince dinamik kurallar ve HUD işareti | Ekip ve klanlar |
| Klan savaş alanı | İlan edilen taraflar için sistemsel çatışma bağlamı | Klan üyeleri |

Bölge tipi her lokasyona config ile atanır; aynı prefab türü (örn. market) farklı bölge tiplerinde farklı davranabilir (örn. güvenli bölgedeki market vs riskli bölgedeki market — risk/getiri farkı).

## 1.5 Görsel / Asset Üretimi — Karar (netleşti)

**MVP için Arma Reforger'ın orijinal/vanilla asset'leri (araç, kıyafet, bina) kullanılacak.** Özgün Türkiye temalı görsel kimlik (kaplama, tabela, üniforma) baştan üretilmeyecek — bunun yerine **mevcut asset'lere dışarıdan küçük müdahaleler** (retexture/reskin gibi) yapılacak. Bu, hem hızlı başlangıç sağlıyor hem de "Tasarım sınırı" ilkesiyle (Döküman 0) uyumlu: görsel kimlik MVP'de oynanışı zenginleştiren bir öncelik değil.

> Bu yaklaşım World Pack / Life Core ayrımıyla (bkz. 1.3) tam uyumlu — orijinal asset'ler World Pack'in ilk, en minimal versiyonu olarak düşünülebilir. İleride gerçek Türkiye teması (özgün tabela, üniforma, kaplama) eklenmek istendiğinde, bu sadece World Pack'i değiştirmek anlamına gelecek, Life Core'a dokunulmayacak.

## 1.6 Bu Dökümandan Çıkan Açık Maddeler

- [ ] Ülke/bölge ismi — bilinçli olarak yer tutucu bırakıldı, kritik değil, panelden de değiştirilebilir
- [x] ~~"M360 Lirası" para birimi ismi~~ — onaylandı
- [x] ~~Görsel/asset üretim yöntemi~~ — netleşti: MVP'de orijinal Arma asset'leri + küçük dış müdahaleler

---
*Önceki: [00 - Ana Döküman](./00_ana_dokuman.md) · Sıradaki: Döküman 2 — Hesap, Karakter & Rol Durumu*
