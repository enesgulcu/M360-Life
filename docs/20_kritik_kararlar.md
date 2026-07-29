# Döküman 20 — Kritik kararlar & kırılma noktaları

*Bağlı: [00](./00_ana_dokuman.md) · [15](./15_gelistirme_notlari.md) · [11](./11_teknik_mimari.md) · [16](./16_dosya_envanteri.md) · [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md)*

> **Demir kararlar.** Lab’ta kanıtlandı; tekrar “keşfetme”. Günlük oturum / el değiştirme → **[15 §C](./15_gelistirme_notlari.md)**.
>
> **Çelişki:** **20 + 19 > 15** günlük not. Eski 15 bölümleri (7b Overlay, 7e hijack+Save) **geçersiz** — bu dosya geçerli.
>
> **Son güncelleme:** 2026-07-29 ~10:15 (TR) — 15’ten taşındı + I/Tab ürün kilidi.

---

## 0. Dosya rolleri (iki PC / iki Cursor)

| Dosya | Rol |
|---|---|
| **20 (bu)** | Nasıl yapılır / asla yapma — ürün kilidi |
| **15 §C** | Şu an neredeyiz, sıradaki, hangi PC |
| **15 §A** | Kullanıcı tercihleri (davranış) |
| **16** | Dosya envanteri |
| **18** | Dual-PC / junction / `M360.bat` |
| **19** | API güvenlik |

**Akşam PC / yeni Cursor:** `git pull` → **20** → **15 §C** → işe başla. Sohbet senkron değil; bellek = git’teki docs.

---

## 1. Tuş → işlev (Input omurgası) — KİLİT 2026-07-29

### Karar

| Madde | Değer |
|---|---|
| Hedef | **I** = Life HUD · **Tab** = **vanilla** envanter |
| Resmi kaynak | BI [Input Manager](https://community.bistudio.com/wiki/Arma_Reforger:Input_Manager) |
| Yol | `Action` + aktif **`CharacterMovementContext`** (`ActionRefs +`) + bir kez `AddActionListener` |
| Conf | `Configs/System/M360_Input.conf` parent=`chimeraInputCommon` |
| gproj | `InputManagerSettings.Default` = `M360_Input.conf` |
| Script | `M360_TusYoneticisi` (`DinleyiciEkle`) · `M360_PlayerControllerI` yalnız `Kur()` |
| Kapı | `UpdateLocalPlayerController` + `m_bIsLocalPlayerController` |
| Yetki | Callback = **UI** veya sunucuya talep. Para/item/rol **yazılmaz** (19). |
| Performans | İstemci yerel; frame poll yok; dedicated klavye işlemez; tuş basımı ağ’a gitmez |
| Kanıt | Kullanıcı 2026-07-29 — I=HUD, Tab=vanilla |

### Yeni tuş (omurga)

1. `M360_Input.conf` → `Action` + `CharacterMovementContext` `ActionRefs +`  
2. `M360_TusYoneticisi` → `DinleyiciEkle` + callback  
3. Log kanıtı  

Inventory / Tab’a **dokunma**.

### Neden battı (tekrar deneme)

| Deneme | Neden |
|---|---|
| `ActionOpenInventory` hijack | Inventory’deki her tuş (Tab) Life |
| Inventory + `Save()` | Eski “ikisi Life” hedefi; enesg’de profil boşalabiliyor |
| Save’siz remap | `Inventory=[]` → I ölü |
| Action conf’te, Context’te yok | Listener hiç tetiklenmez |
| `ActionInput` / text `Context` | Unknown class / tüm input ölür |
| Overlay özel context + ActivateContext | ESC / cursor |
| `Debug.KeyState` | Yalnız Workbench; canlıda ölü |
| `ActivateAction` her frame | Yanlış araç |

### YASAK

Inventory hijack · `InputBinding.Save` · Save’siz remap · `ActionInput` · text `Context` · Overlay özel context · `Debug.KeyState` ürün · `ActivateAction` köprü · Steam kalıcı `-client`

---

## 2. HUD / widget — KİLİT

| Madde | Karar |
|---|---|
| Lab UI | `CreateWidget` + `FrameSlot` + gerçek alpha texture |
| Yasak | Elle `.layout` + `CreateWidgets` → Play/WB **donar** |
| Layout sözdizimi (ileride) | `Slot FrameWidgetSlot "{GUID}"` — MCP Children/tırnaklı Slot yanlış |
| Stil | Circle rings + nakit pill (L-mid-R) + canta 9-slice; HTML/NUI yok |
| Texture | Opak DDS = beyaz kare; köşe stretch etme; soft AA; lab’da tıklanır Kapat yok → **I** toggle |
| Boot | `IsEditMode` ile HUD kapatma **yasak** (Play’de true kalabilir). `InPlayMode` + FRAME lazy `Baslat` |
| Ürün hedefi | Layout Editor (sonra); şimdilik CreateWidget yolu |

---

## 3. Dedicated / MP lab — KİLİT

| Madde | Karar |
|---|---|
| Exe | Steam `Arma Reforger Server` (`ArmaReforgerServer.exe`) — boş “Dedicated Server” klasörü ≠ exe |
| Addon | `tools/dedicated/addons/M360-Life` → junction → repo `m360-life` |
| Lokal start | `-server "{GUID}Worlds/..." ` + `-addonsDir` + `-addons 69F4E91377BCC9A5` |
| Yasak kombo | **`-config` + `-addons` aynı anda** (lokal yayınlanmamış mod) |
| Bağlan | Önce sunucu → `baglan-istemci.ps1` / `M360-Oyna.bat` |
| Steam kalıcı | Yalnız `-addonsDir` + `-addons` — **`-client` yazma** (Init Error) |
| Direct Join | Lokal’da “No server found” normal; CLI `-client 127.0.0.1` |
| Profil | `-profile M360Dedicated` → `Documents\My Games\M360Dedicated\` |
| Script değişince | Sunucuyu yenile (checksum Init Error) |

---

## 4. Lab dünya / Play — KİLİT

### Game mode

`GameMode_Plain` yetmez. BI General Game Mode: FactionManager_FFA + LoadoutManager_FFA + SpawnPoint_FFA.

### Zemin

| Yanlış | Doğru |
|---|---|
| `ConcretePanel` + scale (titreme) | `GenericTerrain_Default` + Create new terrain |

### Dünya `.ent` kabuğu

Boş / 0-byte `.ent` → harita boş (layer dolu olsa bile). Minimum:

```
Layer default {
 Index 0
}
```

### Lab ışık

`Lighting_Default` (Direct) beyaz grid’de yanmaya yol açtı → lab’da kaldırıldı; EnvProbe + Fog + PP + Overcast. Sert Direct geri alma.

### Lab kamera 3P

M360 kodu yok; düz lab + vanilla 3P. Lab testte **1. şahıs**.

### Spawn / test konum

0,0 kenar hissi — X/Z ~20+; lab mahalle alanları docs/15 lab notu / `LabDuzZemin/README`.

---

## 5. UserAction — KİLİT

| Doğru | Yanlış |
|---|---|
| `additionalActions` (küçük a) | `AdditionalActions` |
| `ParentContextList` + `PointInfo` + `UIInfo` | Context’siz action |
| Wiki: `Action_Context_Setup` | Ezbere property |

---

## 6. İş sistemi (Pirinç dilim) — KİLİT

| Madde | Karar |
|---|---|
| Model | Üç site: Topla → İşle → Sat (`M360_IsAyar` config) |
| Class | Türkçe ASCII (`ToplamaAlaniBileseni` …) — İngilizce `Job*` yok |
| Lab veri | `M360_IsOturumlari` stub (henüz ledger/DB değil) |
| Mesafe iptal | `m_fIptalMesafesi` |
| Yeni iş | Üç prefab kopyala + Attribute — çekirdek script değişmez |

---

## 7. API / Neon / güvenlik çıkarımları — KİLİT

| # | Ders |
|---|---|
| 1 | Env ≠ kod: Vercel’e key yazmak yetmez; koruma kodu deploy şart |
| 2 | Tarayıcı adres çubuğu ≠ API istemcisi (custom header yok) |
| 3 | DB URL bilmek Neon’u açmaz; açık GET endpoint veri sızdırır → key + rate limit |
| 4 | Oyun → API: üretimde anahtar yalnız dedicated; istemci ekonomi yazmaz |
| 5 | Junction = tek kopya; `robocopy /MIR` yasak (api/docs silinir) |
| 6 | Input tuş omurgası güven sınırı değil (yalnız girdi yüzeyi) |

Canlı: `https://m360-life.vercel.app` · detay [19](./19_guvenlik.md).

---

## 8. Kod / araç / repo — KİLİT

| Madde | Karar |
|---|---|
| İsim | Değiştirilebilir her şey **Türkçe ASCII** (11.2.1); motor API İngilizce |
| EveronLife | Referans only, dependency **asla** |
| Workbench | Açıkken `wb_launch` yasak → `wb_connect` |
| rdb | `resourceDatabase.rdb` **silme** |
| Senkron | git + junction (`M360.bat` / `pc-hazirla`) |
| MCP | Cursor `enfusion-mcp`; handler syntax hatası tüm WB Game modülünü düşürür |
| Varsayım | Enfusion’da API ezbere uydurma — `api_search` / wiki |

### Sık GUID (lab)

| Prefab | GUID |
|---|---|
| GenericTerrain_Default | `221ABC927C672E4E` |
| GameMode_Plain | `1B76F75A3175E85C` |
| FactionManager_FFA | `A1C54BBE72C5C696` |
| M360 mod | `69F4E91377BCC9A5` |
| M360_Input.conf | `ADF987310AA5305C` |

---

## 9. Yasaklar özet kartı

| Yasak | Neden |
|---|---|
| `CreateWidgets` / elle layout | Donma |
| Prop-scale zemin | Titreme |
| Inventory hijack / Save remap | Tab/Life / profil |
| `ActionInput` / text `Context` | Input ölümü |
| Overlay özel + ActivateContext spam | ESC |
| Debug.KeyState ürün | Dedicated ölü |
| rdb silme | Wrong GUID |
| `wb_launch` (WB açıkken) | Çift instance |
| Steam kalıcı `-client` | Init Error |
| `/MIR` sync | Monorepo silinir |
| Boş `.ent` | Boş harita |
| Tuş callback’te para/item | Exploit yüzeyi |

---

## 10. Şablon (yeni kilit)

```
## N. Başlık — KİLİT YYYY-MM-DD
| Madde | Karar |
YASAK: ...
Kanıt: kullanıcı / log
```

Günlük deneme → **15**, buraya değil.
