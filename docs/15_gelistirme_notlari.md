# M360 Life — Geliştirme Notları (AI / Oturum Belleği)

> **Amaç:** Oturumlar arası kaybolmaması gereken teknik bulgular + kullanıcının tercihleri. Konuşma geçmişi değil; **kanıtlanmış gerçekler** buraya yazılır.
>
> **Yeni sohbette zorunlu okuma sırası:** `docs/00`–`14` → **bu dosya (15)** → sonra işe başla.
>
> **Son güncelleme:** 2026-07-27 — HUD/Canta lab dilimi kapatıldı (§7c v3 soft9); kamera notu §7d; kritik özet §B güncellendi.

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
| **Life envanter tuşu** | Arma 3 Life = **I**. Reforger `"Inventory"` ≈ **Tab**. **Kanıtlı lab yolu (2026-07-27):** `Debug.KeyState(KeyCode.KC_I)` + `ClearKey` → M360 aç/kapa. Tab’ı `ActionOpenInventory` ile M360’ye **bağlama**. Input.conf / runtime remap Play’de **tutmadı** — ayrıntı **7b**. |
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
| 5 | `M360_Input.conf` (parent=chimeraInputCommon) + `addon.gproj` `InputManagerSettings.Default` + ayrı `M360_EnvanterAc` action | Conf/gproj Play’e yansımadı veya override uygulanmadı; kullanıcı: **hâlâ sadece Tab**. |

### Son başarılı denemede farklı olan (KRİTİK)

1. **Boot kapısı:** `IsEditMode` yasak. `SetEventMask(FRAME)` + `GetGame().InPlayMode()` olunca lazy `Baslat()`. Progress hint ancak bundan sonra aktı.
2. **Görünürlük garantisi:** Açılışta `SCR_HintManagerComponent.ShowCustomHint` ile Life listesi. Widget denemesi yan ürün; kullanıcıya kanıtlanan UI = **hint**.
3. **I tusu (asıl kırılma — 2026-07-27):** ActionManager / Inventory action / conf remap **atlandı**. FRAME’de ham klavye:
   - `if (Debug.KeyState(KeyCode.KC_I)) { Debug.ClearKey(KeyCode.KC_I); EnvanterAcKapa(); }`
   - Desen kaynağı: BI `SCR_DebugEditorComponent` (aynı `KeyState` + `ClearKey`).
   - **Neden kritik:** `"Inventory"` action’ın fiziksel tuşu preset’e bağlı (çoğu kurulumda **Tab**). Action adını dinlemek ≠ **I** dinlemek. Conf/remap Play lab’da güvenilir olmadı; ham `KeyCode` güvenilir oldu.
4. **Tab ayrımı:** `modded ActionOpenInventory` → M360 **kaldırıldı**. Tab artık M360 açmaz (vanilla Inventory yolu). M360 sadece **I**.

### Tuş gerçeği (UNUTMA)

| Tuş | Sonuç |
|---|---|
| **Tab** | BI `"Inventory"` → native envanter (M360 **değil**). |
| **I** | `Debug.KeyState(KC_I)` → M360 çanta (hint). **Kanıtlandı.** |
| **Hedef (ürün)** | İleride gerçek input action / kullanıcı ayarı; lab’da Debug.KeyState yeterli ve doğru. |

| Parça | Dosya |
|---|---|
| HUD + I tusu | `M360_CantaHudBileseni.ITusunuKontrolEt` (`Debug.KeyState(KC_I)`) |
| Action hook | ~~`M360_PlayerController`~~ kaldırıldı — Tab M360 açmaz |
| F yedek | `M360_DurumAksiyonu` → `Envanter` |

### İlke (sonraki UI / input işleri) — KRİTİK ÖZET

- Progress / envanter feedback önce **hint ile kanıtla**, sonra layout/widget güzelleştir.
- **Action adı ≠ klavye harfi.** `"Inventory"` çoğu zaman Tab’dır; I için ayrı çözüm gerekir.
- Workbench Play’de **Input.conf Default / runtime InputBinding remap’e güvenme** (bu projede tutmadı). Lab’da ham tuş: `Debug.KeyState` + `ClearKey`.
- Oyuncu tuşu için BI’nın hangi `Action*` sahibini bul; kör `GetActionTriggered("I")` uydurma.
- Tab’ı M360’ye bağlamak kolay ama kullanıcı istemiyor → M360’yi Inventory action’dan **ayır**.

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

## B. Kritik özet kartı (hızlı bakış — 2026-07-27)

| Konu | Kural |
|---|---|
| `CreateWidgets` + elle `.layout` | **YASAK** (Play/WB donar) |
| Lab UI | `CreateWidget` + `FrameSlot` |
| Texture | Gerçek alpha; köşe **sabit** (9-slice / pill L-mid-R); soft AA |
| I tuşu | `Debug.KeyState(KC_I)` canta; Tab’a bağlama |
| HUD stil | Circle rings + nakit pill + canta panel; HTML yok |
| Lab 3P kamera | M360 değil; labda 1. şahıs |
| Git | Workbench addon → `bagla-oyun-klasoru.ps1 / sync-game-to-github.ps1` → `Documents\GitHub\M360-Life` push (`/MIR` yasak) |
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
- Neon + Vercel api canlı (health/jobs). Dedicated server hâlâ sırada. Güvenlik iskeleti: docs/19.
- EveronLife: referans only, dependency asla.

---

## 10. Açık maddeler

- [x] GenericTerrain + Create new terrain (kullanıcı yaptı; `M360_Terrain/` var)
- [ ] NET API + `wb_connect` kalıcı yeşil
- [x] TEST spawner: F + scroll UserAction — Play teyidi + **temizlik yapıldı**
- [x] JobStation Topla/İşle/Sat F menüsü — lab’a yerleştirildi
- [x] HUD: yüzde sayacı + M360 envanter listesi (Play teyidi)
- [x] Inventory tuşu **I** — `Debug.KeyState(KC_I)` kanıtlandı; Tab M360 açmaz
- [x] Life tarzı `.layout` HUD çerçevesi lab v1 (CekirdekHud + CantaPanel — docs 10.8 / 7c)
- [ ] Dedicated server ilk çalıştırma

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
- ~~Play’de RestApi kanıtı~~ → **KANITLANDI 2026-07-28** (aşağıda)
- Dedicated server
- Yazma endpoint’leri + idempotency + audit
- `web/` admin paneli
- Anahtarın yalnız dedicated’ta kalması (oyuncu build’ine gömme)

**Play API kanıtı (2026-07-28 ~11:53)**
- Entity: `M360_ApiTest` + `M360_ApiBaglantiTestiBileseni` (LabDuzZemin)
- Anahtar: `$profile:M360_ApiLabKey.txt` (git’e girmez; Attribute boş)
- Log:
  - `OK health http=200 sure~1093ms` · `db: bagli`
  - `OK jobs http=200 sure~1612ms` · Pirinc satırı Neon’dan
- Sonuç: Workbench Play → Vercel → Neon zinciri çalışıyor

---

*İlgili: [00](./00_ana_dokuman.md) · [11](./11_teknik_mimari.md) · [13](./13_enfusion_prefab_prosedur.md) · [14](./14_mevcut_modlar_araclar.md) · [16](./16_dosya_envanteri.md) · [17](./17_yerel_kurulum.md) · [18](./18_calisma_duzeni.md) · [19](./19_guvenlik.md)*
