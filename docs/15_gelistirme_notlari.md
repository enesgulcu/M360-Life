# M360 Life — Geliştirme Notları (AI / Oturum Belleği)

> **Amaç:** Oturumlar arası kaybolmaması gereken teknik bulgular + kullanıcının tercihleri. Konuşma geçmişi değil; **kanıtlanmış gerçekler** buraya yazılır.
>
> **Yeni sohbette zorunlu okuma sırası:** `docs/00`–`14` → **bu dosya (15)** → sonra işe başla.
>
> **Son güncelleme:** 2026-07-29 ~01:55 (TR) — **DEVİR:** Dedicated I≠Life / Tab=Life (yanlış). WB Play I=OK. Dual-PC push. **SON DURUM → §C.**

---

## C. SON DURUM / el değiştirme (yeni sohbet buradan)

> Yeni pencere: AI **§C** okur (kural otomatik). Kullanıcıya “oku” deme.
> Kullanıcı tek tık: **`M360.bat`** — bkz. **C.0** / `TEK_ADIM.txt`.

### C.0 TEK ISLEM (kullanici)

| Ne zaman | Ne yapar |
|---|---|
| PC değişince / güne başlarken | Repo kökü **`M360.bat`** çift tık |
| Oynamak | **`M360-Oyna.bat`** (sunucu yeniler — script checksum Init Error önler) |
| Gün sonu / diğer PC öncesi | Cursor’a: **“commit ve push”** |
| Yeni Cursor sohbet | **Hiçbir şey deme** — kural §C okur |

### C.1 İki PC

| Ne | PC A (sabah / Enes) | PC B (akşam / enesg) |
|---|---|---|
| Windows | `Enes` | `enesg` |
| Repo | `C:\Users\Enes\Documents\GitHub\M360-Life` | `C:\Users\enesg\Documents\GitHub\M360-Life` |
| Addon | junction → repo `m360-life` | aynı |
| Script yolları | `$env:USERPROFILE` + `tools\Resolve-M360Paths.ps1` | sabit `C:\Users\Enes\...` **yazma** |

Senkron = **yalnızca git**. Secrets / Server exe git’e girmez.

### C.3 Tablo — 2026-07-29 ~01:55 (DEVİR / KIRIK)

| | |
|---|---|
| **PC** | B (`enesg`) — kullanıcı yattı; **sabah PC A** devam |
| **Hedef (ürün)** | **I** = Life HUD · **Tab** = **vanilla** envanter (HUD yok) · istemciye ekonomi güvenme (docs/19) |
| **WB Play** | **I → Life HUD KANITLI** (`Debug.KeyState` / `#ifdef WORKBENCH`) |
| **Dedicated + Steam (son test)** | **BAŞARISIZ — kullanıcı:** Tab → Life HUD · **I tepki yok** |
| **Log (01:55)** | `Tus (Save=YOK) Inventory=[]` · `Inventory → Life HUD` (Tab) · `I → Life` **yok** · HUD boot OK |
| **API** | health OK; jobs 401 = `$profile:M360_ApiLabKey.txt` eksik (lab; çanta ile ilgisiz) |
| **rdb** | **ASLA silme** |
| **Sıradaki** | Dedicated’da I=Life + Tab=vanilla **kalıcı**. §C.5 yasakları tekrarlama. |

**Steam kalıcı (`-client` YOK):**  
enesg: `-addonsDir "C:\Users\enesg\Documents\GitHub\M360-Life\tools\dedicated\addons" -addons 69F4E91377BCC9A5`  
Enes: `-addonsDir "C:\Users\Enes\Documents\GitHub\M360-Life\tools\dedicated\addons" -addons 69F4E91377BCC9A5`

### C.5 I + Tab — kök neden / yasaklar (TEKRARLAMA)

| Kök | Açıklama |
|---|---|
| Hijack | `ActionOpenInventory` → Life → Inventory’deki **her** tuş Life. Tab hâlâ Inventory’deyse Tab=HUD. |
| Save (enesg) | `InputBinding.Save()` → `InputUserSettings` **boşalabiliyor** → Tab default Inventory → Life. **YASAK bu PC.** |
| Save yok | `RemoveBinding` + `AddBinding` Save’siz → log `Inventory=[]` → **I bağlanmıyor**. |
| Custom action | `M360_LifeCanta` + ActivateAction/listener dedicated’da **tetiklenmedi**. |
| Text Context | `Contexts { Context … }` → `Unknown class` / ESC. Overlay → ESC mouse. |

| Yasak | Neden |
|---|---|
| `InputBinding.Save` (enesg) | Profil boşalması |
| Text `Context` / Overlay spam | ESC / input ölüm |
| `ActionInput` | Tüm input kırılır |
| rdb silmek | Wrong GUID |
| Hijack + Tab hâlâ Inventory | Tab=Life |
| ActivateAction “köprü” sonsuz deneme | Dedicated’da kanıtsız |
| Steam Oyna / kalıcı `-client` | Eski oturum / Init Error |

**Eski kanıt (2026-07-28 sabah PC — hedef DEĞİŞTİ):** Inventory hijack + I bind + **Save** → Tab+I ikisi Life. O gün hedef buydu; **şimdi Tab vanilla olmalı** — o yol tek başına yetmez.

**Kod (şu an — dedicated kırık):**  
`M360_PlayerControllerI.c` · `M360_TusYoneticisi.c` · `M360_CantaHudBileseni.c` (Play=Debug I) · `addon.gproj` Input=vanilla · `M360_Input.conf` yedek `M360_LifeCanta`

**Sabah AI önerilen yol:** Workbench Config Editor ile action’ı `CharacterMovementContext`’e ekle + `AddActionListener` bir kez; **hijack kaldır** veya Inventory’de yalnız I kalıcı (Save’siz tutan yol). Tab Inventory’den ayrı. Log kanıtı olmadan “çalıştı” deme.

### C.6 Bu oturum (enesg gece)

Dual-PC (`M360.bat` / `M360-Oyna.bat` / `pc-hazirla`) · junction · yapı TR iskelet · I/Tab lab (yukarı) · kullanıcı: gece bitti, sabah işte bakar.

---

## A. Kullanıcı tercihleri (UNUTMA — kritik)

Bu bölüm, kullanıcının açıkça söylediği ve davranışından çıkan kurallardır. İhlal etme.

| Tercih | Ne yap / yapma |
|---|---|
| **Acemi ama yönetici** | Kod yazmayı öğrenmeyecek. AI yazar; kullanıcıya **hangi tuş / hangi menü / neye tıkla** diye Türkçe rehber verilir. Sonuç (ekran, hata) kullanıcıdan gelir. |
| **Tam rehberlik** | “Sen halledersin” deyip bırakma. Omuz omuza yürüt; gözden kaçanı söyle; riskli adımda durup sor. |
| **Karar kullanıcıda** | “Emanet” ≠ nihai sorumluluğu alma. Seçenek sun, onay al, sonra uygula. |
| **Mevcut Workbench** | Açık Workbench varken **yeni açma** (`wb_launch` yasak). Sadece `wb_connect`. Çift açıldıysa fazlalığı kapat, eskisiyle devam. |
| **Test vs ürün ayrımı** | Test kodunu temizle ama **uğraşılan gerçek işi silme** (Pirinç JobStation vb.). Testleri `Test/` altında tut; silinebilir olsun. |
| **Resmi / default çözüm** | Uydurma hack yerine BI’nin default’unu kullan (örn. zemin = `GenericTerrain_Default`, prop scale değil). Kullanıcı “keşke default kullansaydın” diyorsa haklıdır — not al, düzelt. |
| **Resmi döküman hakimiyeti** | Enfusion/Reforger’da varsayım yasak. `api_search` / `wiki_search` / BI wiki. EveronLife yalnızca fikir; bağımlılık değil. |
| **Basit Türkçe** | Teknik terim varsa parantez içinde; açıklama anlaşılır olsun. |
| **Ekran görüntüsü / hata** | Kullanıcı Play sonucu ve VM exception gönderir → önce kök neden, sonra net adım. |
| **Öğrenme notları** | Her önemli keşif **docs/15**’e yazılır. |
| **Dosya envanteri** | Her yeni/değişen dosya **docs/16**’ya işlenir (amaç, ilişki, istemci/sunucu/API). İşlem bitince kontrol et. |
| **Test konumu** | Spawn/spawner/istasyon **0,0’da olmasın** — X/Z ~20 civarı (kenar/dip hissi olmasın). |
| **Play’e odak** | Mümkünse AI script reload + dünya hazırlığını yapsın; kullanıcı sadece Play ile test etsin. NET API yoksa AI uzaktan reload yapamaz — kullanıcıya tek adım bırak. |
| **Önceki süreç** | Tasarım + ilk kod **Claude Agent / Claude Code** ile yapıldı. Şimdi **Cursor + enfusion-mcp**. Klasör: `...\addons\M360-Life`. |
| **Docker istemiyor** | Neon hosted PG; yerel PG yok. Docker yok. |
| **Kapsam büyüdü ama acele yok** | MVP geniş (15 iş, polis/doktor, klan); süre esnek. Faz sırası önemli. |
| **Life envanter tuşu** | **Hedef:** **I** = Life HUD · **Tab** = **vanilla** (HUD değil). WB Play’de I kanıtlı (`Debug.KeyState`). Dedicated çözüm **açık** (§C.5). Custom text Context / Overlay / enesg’de `Save()` **yasak**. |
| **Kod dili / isim (ANA DÜSTUR)** | Değiştirilebilir her şey **Türkçe ASCII**: class, metod, üye, Attribute, dosya adı, oyuncu metni. **Yorumlar Türkçe**. Motor override/API İngilizce dokunulmaz. Prefab + `.et` birlikte. `EnfusionMCP` araç kodu hariç. Bkz. **11.2.1 / 13.2**. |
| **Oyun içi HUD** | Hedef: Life tarzı kenar HUD. **Lab yolu (kanıtlı):** `CreateWidget` + `FrameSlot` + alpha texture. Elle `.layout` + `CreateWidgets` = **Play/WB donması** — yasak. Ürün hedefi ileride Layout Editor. HTML/NUI yok. Detay: **10.8** + **7c**. |
| **Özen / tekrar etme** | Aynı UI ayarını 5 kez deneme. Önce doğru teknik (9-slice, soft AA, CreateWidgets yasak), sonra bir net sonuç. Kullanıcı güven + yol arkadaşı ister. |
| **Lab 3. şahıs kamera** | Lab düz zeminde Enter sonrası zoom/kayma → **M360 kamera kodu yok**; vanilla 3P collision + lab. Lab testte **1. şahıs** kullan. |

### Kullanıcının dikkat ettiği şeyler (gözlem)
- Titreyen / bozuk hissettiren sahne → kabul etmez; kök nedeni sorar.
- “Default vardı, onu kullansaydın” → motorun hazır çözümünü tercih eder.
- Test sonrası silinebilir düzen ister (`Test/` klasörü, README).
- Araç spawn’da: menüden 3 seçenek, doluysa başka yer, **arazi dışına çıkmasın**.
- Workbench süreç disiplini (tek instance) önemli; AI’nin tekrar tekrar yeni pencere açması sinir bozar.

---

## 0. Çalışma protokolü (sabit)

| Kural | Uygulama |
|---|---|
| Kullanıcı yönetici | AI teknik ortak/rehber |
| Workbench | Açıkken `wb_launch` YOK → `wb_connect` |
| NET API | `File → Options → General → Net API` |
| Kaynak sırası | Docs + bu not → enfusion-mcp → BI wiki → EveronLife (sadece referans) |
| Ürün koru / test ayır | Jobs = ürün; `Scripts\|Prefabs/.../Test` = geçici |
| Döküman | Büyük karar → ilgili docs + **bu dosya** |

---

## 1. Ortam (bu makine)

| Bileşen | Değer |
|---|---|
| Addon | `C:\Users\Enes\Documents\My Games\ArmaReforgerWorkbench\addons\M360-Life` |
| Cursor MCP | `C:\Users\Enes\.cursor\mcp.json` → sunucu `enfusion-mcp` (oturumda `user-enfusion-mcp`) |
| Workbench | `D:\SteamLibrary\steamapps\common\Arma Reforger Tools\Workbench\ArmaReforgerWorkbenchSteamDiag.exe` |
| Oyun | `...\Steam\steamapps\common\Arma Reforger` (veri `.pak`) |
| Terrain data | `Worlds/LabDuzZemin/` (+ `M360_LabDuzZemin_Layers/`); eski TestWorld kaldırıldı |
| Lab world | `Worlds/LabDuzZemin/M360_LabDuzZemin.ent` — **boş .ent = boş harita** (bkz. §8e) |
| Eski Claude MCP | `~/.claude.json` (artık birincil değil) |

---

## 2. enfusion-mcp — öğrenilenler

**İşe yarar:** API/wiki arama, GUID’siz araştırma, script/prefab üretimi, Workbench `wb_*` (NET API + handler şart).

**Tuzaklar:**
- `wb_launch` → ikinci Workbench açabilir (**kullanıcıya aykırı**).
- NET API kapalıysa connect fail; process açık olsa bile.
- `Failed to call not existing Net API function 'EMCP_WB_Ping'`: NET API açık ama **WorkbenchGame handler derlenmemiş**. Sık neden: `EMCP_WB_*.c` içinde syntax hatası → tüm WorkbenchGame modülü düşer. Çözüm: `enfusion-mcp` paketinden handler’ları force kopyala + Workbench’i M360 `addon.gproj` ile yeniden aç.
- `asset_search` / `game_browse("Prefabs")` bazen boş (pak index); GUID için `resourceDatabase.rdb` parse.
- Handler’lar: `Scripts/WorkbenchGame/EnfusionMCP/` — yayın öncesi `wb_cleanup`.

---

## 3. Play / minimal dünya — KANITLANDI

**Hatalar (FactionManager yokken):**
- `m_FactionManager` NULL → `SCR_EntityCatalogManagerComponent`
- `IsValidFaction` NULL → identity bio

**Kök neden:** `GameMode_Plain` yetmez. BI **General Game Mode Setup** zorunlu.

| Entity | Prefab | GUID |
|---|---|---|
| GameMode | `.../GameMode_Plain.et` | `1B76F75A3175E85C` |
| FactionManager | `.../FactionManager_FFA.et` | `A1C54BBE72C5C696` |
| LoadoutManager | `.../LoadoutManager_FFA.et` | `B7E5D7F4864D81C0` |
| Spawn | `.../SpawnPoint_FFA.et` | `5392C4D80E4B18F8` |

Layer diskten değişince Workbench’te dünyayı **yeniden yükle**.

Wiki: `Arma_Reforger:General_Game_Mode_Setup`

---

## 4. GUID bulma (`resourceDatabase.rdb`)

```
<path> + 00 06 00 00 00 00 00 + <8 byte LE GUID>
```

8 byte **ters çevir** → `{GUID}`.

Kanıtlı örnekler: GameMode_Plain, PlayerSpawnPoint, ConcretePanel, M151A2, FFA manager’lar, GenericTerrain_Default, Lighting_Default, S1203_transport_beige, UAZ469.

| Prefab | GUID |
|---|---|
| `GenericTerrain_Default.et` | `221ABC927C672E4E` |
| `Lighting_Default.et` | `5B2B348D9520F7C7` |
| `M151A2.et` | `F649585ABB3706C4` |
| `UAZ469.et` | `259EE7B78C51B624` |
| `S1203_transport_beige.et` | `543799AC5C52989C` |
| `M360_Topla_Pirinc.et` | `ADF987310AA53059` |
| `M360_JobProcess_Pirinc.et` | `ADF987310AA5305A` |
| `M360_JobSell_Pirinc.et` | `ADF987310AA5305B` |
| `SackUniversal_01.et` | `F4CA533A99B20DAE` |
| `Workbench_01.et` | `95D947E744336F78` |
| `CashierShop_01.et` | `51D31AB07AE3C1C7` |

---

## 5. Zemin — KANITLANDI (asla prop-scale zemin)

| YANLIŞ | DOĞRU |
|---|---|
| `ConcretePanel` + `scale` (titreme/jitter) | `GenericTerrain_Default` @ 0,0,0 |
| | Sağ tık → **Create new terrain…** |
| | Işık: `Lighting_Default` |

Wiki: `Arma_Reforger:New_Terrain_Setup`, Terrain Preparation Tutorial.

**Kullanıcı tepkisi:** Titreyen zemin kabul edilmedi; default terrain istendi. Bu tercih kalıcı.

---

## 6. Job sistemi (ÜRÜN — Pirinç dikey dilim)

**Akış:** Topla (çuval) → İşle (tezgâh) → Sat (kasiyer) → nakit.

| Parça | Dosya / prefab |
|---|---|
| Config | `M360_IsAyar` (`m_iMaxTasima`, `m_iAdimSuresi`…) |
| Oturum sayac | `M360_IsOturumlari` (oyuncuId → ham/islenmis/nakit) |
| Siteler | `M360_ToplamaAlaniBileseni` / `IslemeMakinesiBileseni` / `SatisNoktasiBileseni` |
| Aksiyonlar | `M360_ToplaAksiyonu`, `IsleAksiyonu`, `SatAksiyonu`, `DurumAksiyonu` |
| Prefab dosya | `M360_Topla/Isle/Sat_Pirinc.et` |
| Lab dunya | `M360_PirincTopla` · `PirincIsle` · `PirincSat` · `M360_CantaHud` |

**Lab config (Play):** adim=4, verim=2, parti=20, isleme=**10sn**, fiyat=600, maxTasima=40.  
Uretim Pirinc (docs 5.7): isleme~180, parti~100 — Attribute ile yukseltilir.

**UserAction:** her prefab’da `additionalActions` + `ParentContextList` (Bolum 7).

**HUD:** `M360_CantaHudBileseni` — Life listesi ipucu; yuzde sayaci; **I** = `Debug.KeyState(KC_I)`.

**Henuz yok:** gercek envanter, banka ledger, replication, DB. Oturum sayac bunlarin yerine gecer.

**Silinen eski:** tek JobStation; Ingilizce `M360_Job*` class adlari (2026-07-27 Turkce ASCII).

---

## 7b. HUD / Life envanter — KANITLANDI (deneme → başarı)

Kullanıcı Play: **yüzde sayacı** + **I ile M360 çanta listesi** çalıştı (önce Tab ile kanıtlandı, sonra I’ye taşındı).

### Başarısız denemeler (ne işe yaramadı)

| # | Denenen | Neden yetmedi |
|---|---|---|
| 1 | `OnPostInit` içinde `world.IsEditMode()` → early return + `CallLater(TickHud)` | World Editor **Play**’de `IsEditMode` true kalabiliyor → HUD hiç boot olmuyor. Ne I/Tab, ne progress. |
| 2 | Sadece `InputManager.AddActionListener("Inventory" / "InventoryOpen")` veya `GetActionTriggered` | Action adı kısmen doğru olsa da **I tuşu bu action değil**. Ayrıca BI zaten Inventory’yi `SCR_PlayerController`’a bağlamış; dışarıdan listener güvenilmez / yetersiz kaldı. |
| 3 | Programatik `CreateWidgetInWorkspace` (Frame, flags=0) “Life popup” | Kullanıcı panel görmedi. Progress’te de görünen şey **widget bar değil**, `SCR_HintManager` yüzde sayacıydı. Widget panel = henüz kanıtlanmadı; hint = kanıtlandı. |
| 4 | `InputBinding` runtime remap: Inventory klavye → `KC_I` + `Save()` | Workbench **Play’de tutmadı** — Tab kaldı, I yok. |
| 5 | `M360_Input.conf` içinde **`ActionInput Inventory`** (yanlış sınıf) + Inventory→I | Dedicated: `Unknown class` → **tüm input kırılır** (hareket/kamera). Vanilla `chimeraInputCommon`’a geri dönüldü. |
| 6 | Sadece `Debug.KeyState(KC_I)` | Workbench Play’de OK; **dedicated/canlı istemcide I ölür** (Debug API kapalı). |

### Son başarılı denemede farklı olan (KRİTİK)

1. **Boot kapısı:** `IsEditMode` yasak. `SetEventMask(FRAME)` + `GetGame().InPlayMode()` olunca lazy `Baslat()`. Progress hint ancak bundan sonra aktı.
2. **Görünürlük garantisi:** Açılışta `SCR_HintManagerComponent.ShowCustomHint` ile Life listesi. Widget denemesi yan ürün; kullanıcıya kanıtlanan UI = **hint**.
3. **I tusu — Workbench (2026-07-27):** FRAME’de `Debug.KeyState(KC_I)` + `ClearKey` → Play lab kanıtı. `"Inventory"` ≠ I (çoğu kurulumda Tab).
4. **I tusu — Dedicated (2026-07-28):** `Debug.KeyState` **yetersiz**. Doğru yol:
   - `Configs/System/M360_Input.conf` → parent `chimeraInputCommon`
   - `Action M360_EnvanterAc` + `InputSourceSum` / `InputSourceValue` + `keyboard:KC_I` (**`ActionInput` sınıfı YASAK**)
   - `Context M360LifeContext` (Priority 0, Flags Overlay) + Actions listesinde action
   - `addon.gproj` → `InputManagerSettings.Default` = bu conf
   - HUD: her frame `ActivateContext("M360LifeContext")` + `AddActionListener(..., EActionTrigger.DOWN, ...)`
   - Log kanıtı: `InputManager config load ... M360_Input.conf` · `CantaHud input: M360_EnvanterAc (I) dinleniyor`
5. **Tab ayrımı:** Tab = vanilla Inventory; M360 sadece **I** (`M360_EnvanterAc`).

### Tuş gerçeği (UNUTMA)

| Tuş | Sonuç |
|---|---|
| **Tab** | BI `"Inventory"` → native envanter (M360 **değil**). |
| **I (WB Play)** | `Debug.KeyState(KC_I)` yedek / eski lab yolu. |
| **I (dedicated / ürün)** | `M360_EnvanterAc` + context + listener — **kanıtlı 2026-07-28**. |
| **Hedef** | InputManager action kalıcı; Debug yalnızca conf yüklenmezse yedek. |

| Parça | Dosya |
|---|---|
| HUD + I | `M360_CantaHudBileseni` — `InputDinleyiciKur` + `ITusunuKontrolEt` (ActivateContext) |
| Input conf | `Configs/System/M360_Input.conf` + `addon.gproj` Default |
| Action hook | ~~`M360_PlayerController`~~ yok — Tab M360 açmaz |
| F yedek | `M360_DurumAksiyonu` → `Envanter` |

### İlke (sonraki UI / input işleri) — KRİTİK ÖZET

- Progress / envanter feedback önce **hint ile kanıtla**, sonra layout/widget güzelleştir.
- **Action adı ≠ klavye harfi.** `"Inventory"` çoğu zaman Tab’dır; I için **ayrı action** (`M360_EnvanterAc`).
- Conf’de **`ActionInput` yazma** — sınıf yok; `Action` + `InputSourceSum` / `InputSourceValue` kullan. Bozuk conf = dedicated’da WASD/kamera ölümü.
- `Debug.KeyState` = Workbench/diag; **canlı MP istemcide güvenme.**
- Yeni action context’te olmalı + her frame `ActivateContext`; listener tek başına yetmez.
- Tab’ı M360’ye bağlama — kullanıcı istemiyor.

---

## 7c. Oyun içi HUD omurgası — LAYOUT DİLİMİ (2026-07-27)

| Madde | Not |
|---|---|
| Hedef | Köşe/kenar Life HUD — aynı teknik, birden fazla nokta |
| Yol | Lab: `CreateWidget` + `FrameSlot`. Ürün hedefi: Workbench `.layout` + `CreateWidgets` (elle layout yasak — donuyor) |
| Dosyalar | Script: `M360_CantaHudBileseni` + widget helper’lar. Diskteki `.layout` runtime’da **yüklenmez**. |
| Script | `M360_CantaHudBileseni` · `M360_CekirdekHudWidgetlari` · `M360_YuvarlakBar` · `M360_CantaPanelWidgetlari` |
| Tuş | I = canta toggle; nakit/saat/vitals her zaman; iş barı sadece topla/isle |
| İkon | `UI/Textures/M360/*` — gerçek alpha; MDI/Iconify → `tools/gen_circle_hud_textures.py` |
| Play kontrol | spawn → nakit pill + vitals rings → F topla → alt bar → I panel |
| Layout sözdizimi | **`Slot FrameWidgetSlot "{GUID}"`** (vanilla). MCP `layout_create` Children/tırnaklı Slot **YANLIŞ** |
| Play kilidi (2026-07-27) | `CreateWidgets` **donuyor**. Elle `.layout` + Layout Editor açmak WB’yi dondürebilir. |
| Lab HUD (şimdi) | **Circle HUD v9** + **Canta v3 soft9** (9-slice panel, L/mid/R satır, soft AA, Kapat yok — I). `CreateWidgets` yasak. |

### Circle HUD dilimi — çıkarımlar (2026-07-27, dilim kapatıldı)

| # | Ders |
|---|---|
| 1 | Opak/alpha’sız ikon DDS → **beyaz kare**; ayrı ikon ImageWidget kırılgan → **tek badge texture** + maskeli ring |
| 2 | Stretch anchor’lı TextWidget → para ikonuyla **üst üste binme**; mutlak `(0,0)` pos/size kullan |
| 3 | Sayı uzayınca kutu: `NakitSeritYerles` — metin genişliği tahmini → şerit `SetSize` → pill **mid** uzar |
| 4 | Dikdörtgen `SetColor` arka plan çirkin; **L+mid+R pill** texture ile oval |
| 5 | FiveM Circle HUD = HTML/NUI; Reforger’da yok — rings+pill yaklaşımı |
| 6 | Kalınlık/ikon scale aşırı kaçmasın; ring içinde padding şart |
| 7 | Lab’da mouse cursor yok → tıklanır Kapat/buton koyma; **I** toggle |
| 8 | Canta: **köşeyi stretch etme** (9-slice / L+mid+R) yoksa piksel merdiven |
| 9 | Soft AA (supersample); `·` glifleri kutu basabilir → ASCII `-` |
| 10 | İkon: Iconify/MDI indir → raster; elle çizme |
| 11 | Agent notu: `.cursor/rules/m360-reforger-hud.mdc` |

*UI lab omurgası kanıtlandı. Sonraki HUD: Layout Editor / daha iyi asset pipeline.*

---

## 7d. Lab kamera (3. şahıs) — NOT (2026-07-27)

| Madde | Not |
|---|---|
| Belirti | Enter ile dış görünüş → yürüyünce kayma / aşırı zoom |
| 1. şahıs | Sorun yok |
| M360 kod | **Kamera/Enter/zoom kodu yok** — HUD sebep değil |
| Muhtemel | Vanilla 3P camera collision + `LabDuzZemin` düz grid / az geometri; Workbench Play |
| Lab pratik | Testlerde **1. şahıs** kullan |
| Doğrulama | İleride Everon/FFA’da M360 kapalı vs açık karşılaştır |

---

## 7e. Dedicated lokal MP — KANITLANDI (2026-07-28)

Lab → portable dedicated → istemci bağlandı; HUD/API/I tuşu/ışık düzeltmeleri bu oturumda.

### Binary / yollar

| Madde | Gerçek |
|---|---|
| Exe | `D:\SteamLibrary\steamapps\common\Arma Reforger Server\ArmaReforgerServer.exe` (Steam Araçlar). Boş “Dedicated Server” klasörü ≠ exe. |
| Addon junction | `tools/dedicated/addons/M360-Life` → repo `m360-life/` |
| Profil | `-profile M360Dedicated` → `Documents\My Games\M360Dedicated\profile\` (**ArmaReforger\profile altında değil**) |
| API key | `secrets/M360_ApiLabKey.txt` → profile’a kopyalanır (`start.ps1`) |
| Scriptler | `tools/dedicated/start.ps1` · `stop.ps1` · `baglan-istemci.ps1` · `status.ps1` |

### BI kısıtlar (UNUTMA)

1. **`-config` ile `-addons` aynı anda kullanılamaz** (lokal yayınlanmamış mod). Lokal: `-server "{GUID}Worlds/...ent"` + `-addonsDir` + `-addons 69F4E91377BCC9A5`.
2. `server.json` `mods: []` — lokal GUID workshop’ta “Addon not found” verir. Workshop publish sonrası `-config` + `mods[]` yolu.
3. UI **Direct Join** → backend lookup → “No server found” (UDP 2001 dinlese bile). Lokal bağlan: istemci CLI `-client 127.0.0.1` + aynı `-addonsDir` / `-addons`.

### Başlatma / bağlanma disiplini

| Kural | Neden |
|---|---|
| Önce `start.ps1`, sonra istemci | `-client` handshake timeout → **Game Initialization Error** (oyun hiç açılmaz) |
| Steam kalıcı launch: **sadece** `-addonsDir ... -addons 69F4E91377BCC9A5` | `-client` kalıcı yazılırsa sunucu kapalıyken Init Error |
| Bağlanmak | `baglan-istemci.ps1` (sunucu PID/2001 kontrol eder, sonra `-client` ile açar) |
| Log | İstemci: `My Games\ArmaReforger\logs\...\console.log` — `handshake timeout` / `Unable to connect as client to '127.0.0.1'` = sunucu yok |

### Siyah ekran / oynanamaz (dedicated lab)

| Belirti | Kök / çözüm |
|---|---|
| Siyah / kamera ayakkabıda / Tab envanter var hareket yok | Kırık input conf (`ActionInput`) + lab’da eksik managers |
| Fix | Vanilla input (sonra düzgün `M360_Input.conf`); layer’a `SCR_CameraManager`, `EnvProbe`, `FogHaze`, `GenericWorldPP`, `PerceptionManager`, `SCR_AIWorld`, `RadioManager`; spawn Y≈3 |
| Lab 3P | Hâlâ kötü → **1. şahıs (V)** (§7d) |

### Lab ışık / hava — kritik kırılma

| | |
|---|---|
| **Belirti** | Dik bakınca OK; bakışı yana/yataya eğince beyaz grid **parlama / yanma**; ufuk üstü siyah |
| **Neden** | (1) Güneş açılı → beyaz zeminde grazing specular + HDR. (2) Saat 10:00 hâlâ yan güneş. (3) Hava state yoksa gökyüzü void. |
| **Yanlış yol** | Sadece soft açı (`-78 35`) — gölge hâlâ uzun, eğince parlıyor |
| **Kırılma** | `Lighting_Default` **silindi** (Direct Light yok). Lab: EnvProbe + Fog + PP + `Overcast`. Parlama kaynağı sert Direct’ti. |
| **Silince ne olur** | Sert gölge/specular yok; düz ambient. Çok karanlıksa sonra **düşük LV** soft ışık eklenir (Lighting_Default değil). |
| **Sıfırdan** | İleride kendi soft prefab; default Lighting alma. |

### I tuşu dedicated — kritik kırılma (SON: 2026-07-28 17:10 KANITLI)

| | |
|---|---|
| **Belirti (eski)** | I ölü; ESC’de mouse yok; bazen sadece Tab |
| **Çalışan çözüm** | (1) `ActionOpenInventory` → Life çanta. (2) `InputBinding.AddBinding("Inventory","keyboard:KC_I")+Save()`. Vanilla InputManager. |
| **Neden Tab yetti** | Inventory action = Tab; hijack Life’a gider. |
| **Neden I de oldu** | Aynı Inventory action’a I eklendi; Save profilde kaldı (dedicated). WB Play’de Save tutmuyordu — yanıltıcı not eski. |
| **Yanlış yollar** | M360_EnvanterAc + Context Overlay; ActivateAction her frame; ActionInput; rdb silmek; gproj’u M360_Input’a zorlamak (client conf yüklemeyebiliyor). |
| **Doğrula** | `baglan-istemci` → I ve Tab çanta; ESC mouse; log `[M360] Inventory <- I` |

### Input conf — doğru / yanlış

```
# CANLI dedicated DOĞRU (kod; conf şart değil)
# M360_PlayerControllerI.c:
#   ActionOpenInventory → EnvanterAcKapa
#   InputBinding AddBinding Inventory <- keyboard:KC_I + Save
# addon.gproj Default = chimeraInputCommon (vanilla)

# YANLIŞ — ESC olumu
Contexts { Context ... Flags Overlay } + her frame ActivateContext

# YANLIŞ — tum input kirilir
Actions { ActionInput Inventory { ... } }
```
### Bilinen uyarılar (şimdilik OK)

- Pirinç prefab `Wrong GUID/name` / `inherited-name` — resource rebuild / Workbench teyidi sonra.
- Direct Join / workshop listing — publish sonrası.

---

## B. Kritik özet kartı (hızlı bakış — 2026-07-28)

| Konu | Kural |
|---|---|
| `CreateWidgets` + elle `.layout` | **YASAK** (Play/WB donar) |
| Lab UI | `CreateWidget` + `FrameSlot` |
| Texture | Gerçek alpha; köşe **sabit** (9-slice / pill L-mid-R); soft AA |
| I tuşu (WB) | `Debug.KeyState` yedek |
| I tuşu (dedicated) | **AÇIK BUG (01:55):** Tab=Life, I ölü. Hedef I=Life / Tab=vanilla. §C.5. Overlay/ActionInput/enesg Save yasak |
| Dedicated bağlan | Önce sunucu; Steam’de `-client` kalıcı **yazma**; `baglan-istemci.ps1` |
| Dedicated lokal start | `-server world` + `-addonsDir` + `-addons` (`-config` ile birlikte değil) |
| Lab ışık | **Tepeden:** `angles -90 0 0` + saat 12 + Cloudy; eğince parlama = yan güneş (§7e) |
| HUD stil | Circle rings + nakit pill + canta panel; HTML yok |
| Lab 3P kamera | M360 değil; labda 1. şahıs |
| Git | junction / `sync-game-to-github.ps1` → push (`/MIR` yasak) |
| Kod isim | Türkçe ASCII (11.2.1) |

---

## 7. UserAction dersleri (araç spawn testi TEMİZLENDİ)

Test script/prefab/entity **silindi** (2026-07-27). Zemin + Play lab kaldı. Pirinç siteleri aynı UserAction kalıbıyla bağlandı (Bölüm 6).

| # | Ders | Sonuç |
|---|---|---|
| 1 | `.et`: `AdditionalActions` **YANLIŞ** | `Unknown keyword` → menü yok |
| 2 | Doğru: **`additionalActions`** | küçük a |
| 3 | Genel action → `additionalActions` | `ParentContextList { "name" }` |
| 4 | Context | `ContextName` + `Position PointInfo` + `UIInfo` |
| 5 | Action | ParentContext + UIInfo.Name + VisibilityRange |
| 6 | Scroll/F | aynı context’te çoklu action = BI default |
| 7 | `ContainerIdPathEntry(name, index)` | `"0"` ayrı path değil |
| 8 | Boş object | `CreateObjectVariableMember` |
| 9 | Property adları | `listProperties` ile doğrula |
| 10 | Kalıcılık | API doldur + Save |
| 11 | WorkbenchGame patch | çoğu zaman WB restart |
| 12 | İyi parent | CashierShop (Mesh/RB/Rpl/ActionsManager) |

Wiki: `Action_Context_Setup`.

---

## 8. Resmi BI referansları (sık)

| Konu | Sayfa |
|---|---|
| Game mode / manager | `General_Game_Mode_Setup` |
| Terrain | `New_Terrain_Setup` / Terrain Preparation |
| User actions | `Action_Context_Setup` |
| World Editor / Play | `World_Editor` |
| Persistence 1.6+ | `Persistence_System` |
| API | enfusion-mcp `api_search` |

**İlke:** API ezbere uydurma.

**Yararlı API notları:**
- Yükseklik: `SCR_TerrainHelper.GetTerrainY`
- Terrain TC: `GenericTerrainEntity.TryGetHeightTC`
- Occupancy: `BaseWorld.QueryEntitiesBySphere` + `Vehicle.Cast`
- Play local action: `HasLocalEffectOnlyScript` → true (editor testi)

---

## 9. Proje durumu (kısa — docs 11/12 ile)

- Tasarım 0–14: tamam.
- Faz 0 lab: Workbench Play + test dünya ilerliyor.
- Neon + Vercel api canlı (health/jobs). **Dedicated lokal lab MP kanıtlandı** (§7e). Güvenlik: docs/19.
- EveronLife: referans only, dependency asla.

---

## 10. Açık maddeler

- [x] GenericTerrain + Create new terrain (kullanıcı yaptı; `M360_Terrain/` var)
- [ ] NET API + `wb_connect` kalıcı yeşil
- [x] TEST spawner: F + scroll UserAction — Play teyidi + **temizlik yapıldı**
- [x] JobStation Topla/İşle/Sat F menüsü — lab’a yerleştirildi
- [x] HUD: yüzde sayacı + M360 envanter listesi (Play teyidi)
- [x] Inventory tuşu **I** — WB: Debug; dedicated: `M360_EnvanterAc` (§7b/7e); Tab M360 açmaz
- [x] Life tarzı `.layout` HUD çerçevesi lab v1 (CekirdekHud + CantaPanel — docs 10.8 / 7c)
- [x] Dedicated lokal ilk çalıştırma + istemci bağlanma (2026-07-28 §7e)
- [ ] Workshop publish → `-config` + Direct Join / dış oyuncu
- [ ] Pirinç prefab GUID / resource rebuild uyarıları

---

## 11. Oturum günlüğü

### 2026-07-27
- Cursor + enfusion-mcp bağlandı; süreç Claude Code’dan Cursor’a taşındı.
- Test oto-kod temizlendi; Pirinç korundu.
- Workbench çift instance → launch yasağı kuralı (kullanıcı tercihi).
- Play NULL FactionManager → FFA manager seti.
- Prop zemin titredi → GenericTerrain_Default (+ kullanıcı uyarısı: default kullan).
- TEST 3’lü araç spawner (bounds, occupancy, terrain Y); silinebilir düzen.
- UserAction: `additionalActions` (camelCase) + `ParentContextList` + PointInfo; R/SPACE workaround kaldırıldı.
- Kullanıcı Play teyidi: F+scroll çalıştı → dersler Bölüm 7’de.
- Test araç spawn tamamen silindi; terrain + FFA lab kaldı.
- Pirinç dikey dilim: Collect/Process/Sell + UserAction + lab yerleşimi (çuval/tezgâh/kasiyer).
- Kullanıcı: tercihler + öğrenmeler bu dosyaya → Bölüm A.
- HUD deneme 1–2 başarısız (IsEditMode / kör Inventory listener / görünmez widget) → deneme 3: FRAME+InPlayMode boot + **modded ActionOpenInventory** + hint listesi → **çalıştı** (Tab; I değil). Ayrıntı **Bölüm 7b**.
- Kullanıcı: Tab iyi ama istemiyor (native envanter); hedef I.
- Runtime InputBinding remap + M360_Input.conf/gproj → Play’de **tutmadı**.
- **Kırılma:** `Debug.KeyState(KC_I)` + `ClearKey` → I çalıştı; Tab hijack kaldırıldı. Kritik özet §7b.
- Kod stili: Türkçe ASCII isim + metod + yorum → docs 00 / 11.2.1 / 13.2 / 15-A.
- HUD ürün yolu net: **layout+ikon+script** (HTML yok) → **docs 10.8** + Bölüm **7c**; Narcos tarzı kenar HUD hedef.
- Repo: Workbench addon → `Documents\GitHub\M360-Life` → GitHub `enesgulcu/M360-Life` ilk push.
- Kök `README.md` eklendi; docs denetimi: lab 12k vs hedef 60k, Doc 13 eski tek-istasyon, kapasite 100 vs 300 sefer — README “Bilinen tutarsızlıklar” + 10.8/00/12.6 düzeltmeleri.
- **Gün sonu final:** Doc 13 üç-site rewrite; 5.7a/b sefer+lab|üretim tablosu; 14 mcp=Cursor; 04/07 zaman hizası; 08/09 stale checkbox; README güncellendi; GitHub push.
- **Türkçe ASCII class rename:** `Job*` → `IsAyar` / `ToplamaAlaniBileseni` / `CantaHudBileseni`…; Attribute `m_iAdim*`; docs **11.2.1 ANA DÜSTUR**; son push.
- **Docs 16:** Dosya/içerik envanteri açıldı (script+prefab+dünya+ileride API). Her işlemde güncelleme kuralı; sunucu-yetkili hatırlatma §0.
- **Yerel lab planı uygulandı:** klasör `Isler/` + `LabDuzZemin`; mesafe iptali (`m_fIptalMesafesi`); docs 17; GitHub `api/` + `packages/db`; sync MIR yasak.
- **Play lab onay (2026-07-27):** Pirinç topla/işle/sat + mesafe iptali OK. Boş harita kök nedeni §8e.
- **Layout HUD lab v1:** `UI/layouts/M360/` + CreateWidgets; hint panel yolu kaldırıldı (§7c). MCP `layout_create` Children/tırnaklı Slot **yanlış** — vanilla `Slot FrameWidgetSlot "{GUID}"`. Play teyidi: kullanıcı (WB Play).

---

## 8e. Dünya `.ent` kabuğu (KRİTİK — 2026-07-27)

**Belirti:** Layer dosyasında entity’ler var ama World Editor / Play’de harita boş (entity count 0).

**Kök neden:** `Something.ent` **0 byte / boş**. Entity’ler `Something_Layers/default.layer` içindedir; Workbench `.ent` olmadan katmanı bağlayamaz.

**Minimum geçerli `.ent` içeriği** (BI `SCR_WorldFilesHelper.CreateWorld`):

```
Layer default {
 Index 0
}
```

**Kurallar:**
- Rename / git move sırasında `.ent` kaybolursa layer dolu kalsa bile dünya boş açılır.
- Düzeltince: resource rebuild + `M360_LabDuzZemin.ent`’i yeniden aç; `wb_entity_list` ile `M360_Pirinc*` doğrula.
- Sync/MIR: world dosyalarını ezme; `.ent` boyutu 0 olmamalı.

**Kanıt:** Boş `.ent` → entity 0; kabuk yazıldı → 11 entity (terrain, spawn, 3 Pirinç, HUD, FFA).

---

*İlgili: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [14](./14_mevcut_modlar_araclar.md) · [16](./16_dosya_envanteri.md) · [17](./17_yerel_kurulum.md) · [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md)*

---

<!-- oturum notlari asagida devam — 8e ustte kalir -->

### 2026-07-28 — Vercel + Neon + güvenlik + lab UI

**Kurulan iskelet**
- Neon PG + migration `job_definitions` (Pirinc)
- Vercel `api/` Root Directory; URL: `https://m360-life.vercel.app`
- `M360_SERVER_KEY` → jobs/metrik kilitli; health açık
- Junction: Workbench `addons\M360-Life` → GitHub `m360-life` (`bagla-oyun-klasoru.ps1`)
- Lab UI: `/` katalog + tip görünüm; `/istatistik` süreler
- Oyun istemci: `Scripts/Game/M360/Ag/M360_ApiIstemci.c` — Play health/jobs **kanitli**
- Docs: 11.1b canlı mimari, 18 iletişim, 19 güvenlik

**Çıkarımlar (önemli)**
1. **Env ≠ kod:** Vercel’e sadece `M360_SERVER_KEY` eklemek yetmez; koruma kodu push edilmeden eski deploy açık kalır.
2. **Tarayıcı ≠ API istemcisi:** Adres çubuğundan custom header yok → lab UI şart; ham `/api/jobs` bilerek 401.
3. **Inline `<script>` + Next hydration:** click listener kaybolur → `"use client"` React handler kullan.
4. **Junction:** kopya senkronu öldürür; Launcher projeyi unutursa `+ Add Project` → `addon.gproj`.
5. **DB sızmaz, API sızabilir:** URL bilmek Neon’a girmez; açık GET endpoint veri sızdırır → key + rate limit minimum.
6. **Ölçek:** Yeni endpoint = `endpoint-katalog.ts` satırı + (gerekirse) tip görünüm; ham JSON her zaman yedek.

**Hâlâ eksik (bilinçli)**
- ~~Play’de RestApi kanıtı~~ → **KANITLANDI 2026-07-28**
- ~~Dedicated paket iskeleti~~ → `tools/dedicated/` + **lokal MP kanıtlandı** (§7e)
- Yazma endpoint’leri + idempotency + audit
- `web/` admin paneli
- Anahtarın yalnız dedicated’ta kalması (oyuncu build’ine gömme)
- Workshop publish (Direct Join / `-config` + mods)

**Play API kanıtı (2026-07-28 ~11:53)**
- Entity: `M360_ApiTest` + `M360_ApiBaglantiTestiBileseni` (LabDuzZemin)
- Anahtar: `$profile:M360_ApiLabKey.txt` (git’e girmez; Attribute boş)
- Log:
  - `OK health http=200 sure~1093ms` · `db: bagli`
  - `OK jobs http=200 sure~1612ms` · Pirinc satırı Neon’dan
- Sonuç: Workbench Play → Vercel → Neon zinciri çalışıyor

**Lab mahalle nizamı (2026-07-28)**
- `LabDuzZemin`: 5 geniş alan (40 m) — 1 Hub · 2 İşler · 3 Ekonomi · 4 Araç · 5 Rol
- Ayırıcı: crash barrier; önünde `M360_LabTabela_1`…`_5`; ışık yok
- Spawn Hub (20,25); Pirinç alan 2 (50/60/70, Z25)
- Yeniden üret: `tools/gen_lab_mahalle.py` · detay: `m360-life/Worlds/LabDuzZemin/README.md`

**Dedicated lokal MP (2026-07-28 öğleden sonra) — özet; ayrıntı §7e**
- Paket: `tools/dedicated/` · exe Steam `Arma Reforger Server` · profil `M360Dedicated`
- Start: `-server` lab world + `-addonsDir` + `-addons` (GUID); **`-config`+`-addons` yasak**
- Bağlan: önce sunucu → `baglan-istemci.ps1`; Steam’de `-client` **kalıcı değil** (Init Error)
- ~15:55 siyah/oynanamaz: kırık `ActionInput` conf + eksik CameraManager/EnvProbe/PP → düzeltildi
- ~16:01 aşırı parlak: `M360_LabOrtam` Cloudy + güneş açısı
- ~16:10 I tuşu dedicated: `M360_EnvanterAc` + context + listener (Debug yetmez)

---

*İlgili: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [14](./14_mevcut_modlar_araclar.md) · [16](./16_dosya_envanteri.md) · [17](./17_yerel_kurulum.md) · [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md)*
