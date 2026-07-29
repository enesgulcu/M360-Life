# Döküman 20 — Kritik kararlar & kırılma noktaları

*Bağlı: [00](./00_ana_dokuman.md) · [15](./15_gelistirme_notlari.md) · [11](./11_teknik_mimari.md) · [16](./16_dosya_envanteri.md) · [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md)*

> **Demir kararlar.** Lab’ta kanıtlandı; tekrar “keşfetme”. Günlük oturum / el değiştirme → **[15 §C](./15_gelistirme_notlari.md)**.
>
> **Çelişki:** **20 + 19 > 15** günlük not. Eski 15 bölümleri (7b Overlay, 7e hijack+Save) **geçersiz** — bu dosya geçerli.
>
> **Son güncelleme:** 2026-07-30 ~00:40 (TR) — Kıyafet sol dikey kategori + zoom/scroll/satın-alma önizleme.

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

---

## 1b. F2 ses modu (AudioSettings) — KİLİT 2026-07-29

### Karar

| Madde | Değer |
|---|---|
| Tuş | F2 → `M360_SesModu` (§1 omurga ile aynı) |
| Hedef | Oyun gürültüsü **mevcut ayarın %15’i** (`HEDEF_CARPAN = 0.15`) |
| Korunan | **VolumeVoiceChat** — dokunulmaz |
| Kaynak | `GetEngineUserSettings().GetModule("AudioSettings")` |
| Alanlar | `Volume`, `VolumeSfx`, `VolumeMusic`, `VolumeDialog` (**0..100**) |
| Uygulama | `audio.Set(...)` → `UserSettingsChanged()` → `SetMasterVolume(id, x/100)` |
| Kalıcı yazma | **SaveUserSettings YOK** (oturum içi; çıkışta oyuncu ayarı kalır) |
| UI | Mute ikonu nakit yanı (~0.12 sn fade); ses geçişi ~0.45 sn |
| Kanıt | Kullanıcı 2026-07-29 — F2 çalışıyor · log `F2 SES LAB OK` |

### İki ölçek (tekrar karıştırma)

| API / alan | Ölçek | Örnek |
|---|---|---|
| `AudioSettings.VolumeSfx` vb. | **0..100** (menü kaydırıcı) | 100 → F2 → 15 |
| `AudioSystem.Get/SetMasterVolume` | **0..1** | 1.0 → F2 → 0.15 |

**YASAK:** `SetMasterVolume(AudioSystem.SFX, 75)` — 75 > 1 → **clamp 1.0** → ses hiç düşmez; ikon yine görünür.

### Kök neden araştırma sırası (benzer sorun rehberi)

1. **Log oku** — `Documents\My Games\ArmaReforger\logs\...\script.log`  
   - `[M360] F2 SES KIS` satırında `SFX 100->15` gibi **0..100** değerler mi?  
   - Eski hatalı log: `SFX 1->0.75 ok=1` (SetMasterVolume yolu) + kullanıcı “ses yok” → ölçek şüphesi.
2. **Ayar dosyası** — `profile\.save\...\settings\ReforgerEngineSettings.conf` → `VolumeSfx 100` (0..100 doğrula).
3. **BI menü yolu** — `SCR_AudioSettingsSubMenu`: `SCR_SettingBindingEngine("AudioSettings", "VolumeSfx", ...)` — script ile aynı modül.
4. **Uygulama sırası** — Önce `AudioSettings.Set`, sonra `UserSettingsChanged()`, sonra `SetMasterVolume(x/100)`.
5. **Otomatik lab** — `M360_SesModuYoneticisi.LaboratuvarOlcum()` (geçici %50, geri al): log `F2 SES LAB OK | once=1 yari=0.5 geri=1` = yol sağlam.
6. **Dedicated** — Ses **istemci yerel**; sunucu script’i işe yaramaz; test = `M360-Oyna.bat` veya `baglan-istemci.ps1`.

### Neden battı (tekrar deneme)

| Deneme | Neden |
|---|---|
| Yalnız `SetMasterVolume` (0..1 doğru olsa bile) | Ayar motoru her frame geri yazabilir; tek başına yetmeyebilir |
| `SetMasterVolume(75)` — ayar 0..100 sanıp | Clamp → 1.0; **ikon var, ses yok** |
| `GetGameUserSettings` / `SCR_AudioSettings` | Oyun modülü; **ses kaydırıcıları Engine `AudioSettings`** |
| `SaveUserSettings` her F2 | Oyuncu profilini kalıcı bozar |
| `VolumeVoiceChat` düşürme | Ürün: konuşma aynı kalmalı |

### YASAK (ses)

`SetMasterVolume`’a 0..100 vermek · VoiceChat düşürmek · F2’de `SaveUserSettings` · sunucuda ses duck · tuş callback’te ekonomi yazma (§1)

### Neden battı — input (tekrar deneme)

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

Inventory hijack · `InputBinding.Save` · Save’siz remap · `ActionInput` · text `Context` · Overlay özel context · `Debug.KeyState` ürün · `ActivateAction` köprü · Steam kalıcı `-client` · metod parametresinde `ActionListenerCallback`/func (Enforce derleme hatası)

---

## 2. HUD / widget — KİLİT

| Madde | Karar |
|---|---|
| Lab UI | `CreateWidget` + `FrameSlot` + gerçek alpha texture |
| Yasak | Elle `.layout` + `CreateWidgets` → Play/WB **donar**; istisna yok |
| Araç mağazası | **Custom/sıfırdan UI yapılmayacak.** Hazır ARGH dealer overlay + M360 nakit köprüsü kullanılır |
| Layout sözdizimi (ileride) | `Slot FrameWidgetSlot "{GUID}"` — MCP Children/tırnaklı Slot yanlış |
| Stil | Circle rings + nakit pill (L-mid-R) + canta 9-slice; HTML/NUI yok |
| Texture | Opak DDS = beyaz kare; köşe stretch etme; soft AA; lab’da tıklanır Kapat yok → **I** toggle |
| Boot | `IsEditMode` ile HUD kapatma **yasak** (Play’de true kalabilir). `InPlayMode` + FRAME lazy `Baslat` |
| Ürün hedefi | Layout Editor (sonra); şimdilik CreateWidget yolu |
| Enforce scope | **Aynı fonksiyonda** `for (int i)` / `ImageWidget bg` / `cat` gibi isimler **tekrar tanımlanamaz** (blok `{}` ayırmaz). Her döngüde benzersiz isim (`katIdx`, `bgUrun`) veya helper metod kullan. |
| Enforce method arg | `notnull ref array<...>` parametre **YASAK** → `array<...>` veya helper metod. |
| ItemPreview CreateWidget | **`GameWidgetType.ItemPreviewWidgetTypeID`** — `WidgetType.ItemPreview...` **YOK** (compile: Can't find variable). |
| ChimeraWorld | `GetGame().GetWorld()` zaten ChimeraWorld döner — **`ChimeraWorld.Cast(...)` YASAK** (Cast not supported). |
| Prefab GUID | Yanlış GUID = çamur/boş RT. Lab’da yalnız **kanıtlı GUID** (docs/15). |

> **Not (2026-07-29):** Eski CreateWidget Magaza HUD / Preview World denemesi **tamamen kaldirildi**. O HUD yoluna donulmez.
>
> **Araç mağazası (2026-07-29 ~21:45):** Kullanıcı sıfırdan/custom UI istemiyor. Hazır `ARGH Ambient Vehicle Plugin` overlay’i kullanılır. Yalnız sunucu tarafı M360 HUD nakit köprüsü ve araç/fiyat yapılandırması bize aittir; ARGH UI’sini yeniden yazma.

> **ARGH UI tıklama (2026-07-29 ~22:30):** İç içe `ButtonWidget` / dekor katmanına ayrı click handler güvenilmez. Tek ana `ButtonWidget`; içindeki renk-yazı hiyerarşisi `WidgetFlags.IGNORE_CURSOR`; işlem `OnMouseButtonDown` ile başlar. Async satın almada istemci kilidi + geri sayım, sunucuda bakiye/doğrulama/spawn, sonuç RPC’si ve kesin tutarlı bakiye gösterimi birlikte olmalı.

> **Kıyafet mağazası (B, 2026-07-29 / güncel 2026-07-30):** Gerçek `MenuBase` + `MenuManager`. Sol panel **dikey**: kategori sütunu (`CategoryList`) + yan yana ürün listesi (`ItemScroll`/`ItemList`). Üst yatay kategori şeridi kullanılmaz (taşma). Liste tıklaması **prova**; **SATIN AL** provadakilerin hepsini tek RPC ile giydirir. Geri sayım yok. Satın alma sonrası önizleme `m_aSonAlinan` ile yeni kıyafetleri tutar (istemci loadout sync gecikmesine karşı) + 450 ms `CallLater` yenileme. Kategori değişince `ItemScroll.SetSliderPos(0,0)`. Karakter prova: oyuncu loadout kopyası + son alınan + prova. Vergys yalnız içerik.

> **Kıyafet mouse/kamera kanıtı:** Ürün yolu MenuManager. İlk kadraj `SetPreviewItem(..., null, true)`. Kullanıcı döndür/zoom: yaw/zoom state tutulur, her seferinde **yeni** `PreviewRenderAttributes` + `forceRefresh`. Handler hem `PreviewContainer` hem root’ta. Sağ tık null’a sıfırlar. Kümülatif ZoomCamera biriktirme **yasak** (FOV kaçar).

> **Vergys prefab kanıtı:** `*_item.et` yasak. Katalog ~97 wrapper (`Uniform_Base` / `Footwear_Base` / `Headgear_Base` / `Vest_Base`).
>
> ~~Magaza denemesi (~15:35): Shop/DE/Reloadz…~~ · ~~Shop System (~14:10)~~ (gecersiz)

---

## 3. Dedicated / MP lab — KİLİT

| Madde | Karar |
|---|---|
| Exe | Steam `Arma Reforger Server` (`ArmaReforgerServer.exe`) — boş “Dedicated Server” klasörü ≠ exe |
| Addon | `tools/dedicated/addons/M360-Life` → `m360-life` |
| Lokal start | `-server "{GUID}Worlds/..." ` + `-addonsDir` + `-addons 69F4E91377BCC9A5` |
| Yasak kombo | **`-config` + `-addons` aynı anda** (lokal yayınlanmamış mod) |
| Bağlan | Önce sunucu → `baglan-istemci.ps1` / `M360-Oyna.bat` |
| Steam kalıcı | Yalnız `-addonsDir` + `-addons 69F4E91377BCC9A5` — **`-client` / Shop / Bacon YAZMA** |
| Istemci acilis | `oyna`/`baglan` → **dogrudan `ArmaReforgerSteam.exe`** (Steam LaunchOptions birlestirmesini atlar) |
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
| `SetMasterVolume`’a 0..100 | Clamp 1.0; F2 ikon var ses yok (20 §1b) |

---

## 10. Şablon (yeni kilit)

```
## N. Başlık — KİLİT YYYY-MM-DD
| Madde | Karar |
YASAK: ...
Kanıt: kullanıcı / log
```

Günlük deneme → **15**, buraya değil.
