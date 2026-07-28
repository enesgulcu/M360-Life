# Döküman 19 — Güvenlik, rate limit ve manipülasyon önleme

*Bağlı: [11.9](./11_teknik_mimari.md) · [12](./12_lisans_roadmap.md) · canlı kod: `api/src/lib/guvenlik/`*

## İlke

İstemci **kaynak değildir**. Para, envanter, rol, iş sonucu yalnızca sunucu + DB doğrular. UI kilidi deneyimdir; gerçek koruma API + transaction + audit hattındadır.

Kritik hat (11.9):

**UI kilidi → Rate limit → Idempotency → Sunucu doğrulama → Transaction/Lock → Audit**

## Canlı durum (2026-07-28)

| Katman | Durum | Not |
|---|---|---|
| Rate limit (IP + yol) | ✅ `withApiKoruma` | Varsayılan 60/dk; `M360_RATE_LIMIT_DAKIKA` |
| Sunucu anahtarı | ✅ | `M360_SERVER_KEY` set → `/api/jobs` + `/api/metrik` zorunlu header; yalnız `/api/health` açık |
| Schema validation | ⏳ | Yazma endpoint’lerinde eklenecek |
| Idempotency key | ⏳ | Satın alma / transfer Faz 1 |
| Audit log tablosu | ⏳ | `packages/db` migration |
| Redis dağıtık limit | ⏳ | Çok instance gerektiğinde |
| Oyun RestApi imza | ⏳ | Dedicated → API HMAC |

Lab’da anahtar boş bırakılabilir; **üretimde `M360_SERVER_KEY` zorunlu**.

## Header sözleşmesi

| Header | Kim | Amaç |
|---|---|---|
| `X-M360-Server-Key` | Dedicated / güvenilir istemci | Paylaşımlı sır |
| `X-M360-Istek-Baslangic` | Tarayıcı / istemci (unix ms) | RTT / geliş süresi ölçümü |
| `X-M360-Sunucu-Ms` | API yanıt | Handler süresi |
| `X-M360-Toplam-Ms` | API yanıt | İstemci başlangıcından toplam |
| `X-M360-Gelis-Ms` | API yanıt | İstemci → sunucu tahmini |

## Metrik / istatistik

- JSON: `/api/metrik`
- Panel: `/istatistik`
- Bellek içi (instance); cold start sıfırlar — kalıcı metrik sonra (Neon veya Analytics)

## Oyun tarafı

`Scripts/Game/M360/Ag/M360_ApiIstemci.c` — `BaglantiTesti()` → `/api/health` + `/api/jobs`.  
Bileşen: `M360_ApiBaglantiTestiBileseni` (Play’de otomatik test).

Manipülasyon: istemciden gelen fiyat/miktar **asla** yazma kaynağı olmaz; DB’deki `job_definitions` / ledger okunur.

## Test kabul (özet)

- 50 hızlı istek → rate limit 429
- Yanlış anahtar → 401 (anahtar set iken)
- Sahte `satis_fiyati` istemci gövdesi → sunucu DB fiyatını kullanır (yazma endpoint’lerinde)
- API down → oyun işlemi başarısız, istemci bakiyesi düşmez
