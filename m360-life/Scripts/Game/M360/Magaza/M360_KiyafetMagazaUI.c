//------------------------------------------------------------------------------------------------
//! Sol liste tiklama isleyicisi.
//------------------------------------------------------------------------------------------------
class M360_KiyafetListeHandler : ScriptedWidgetEventHandler
{
	protected M360_KiyafetMagazaUI m_Menu;
	protected int m_iIndex;

	void Init(M360_KiyafetMagazaUI menu, int index)
	{
		m_Menu = menu;
		m_iIndex = index;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_Menu || button != 0)
			return false;
		m_Menu.SecimYap(m_iIndex);
		return true;
	}
}

//------------------------------------------------------------------------------------------------
class M360_KiyafetKategoriHandler : ScriptedWidgetEventHandler
{
	protected M360_KiyafetMagazaUI m_Menu;
	protected string m_sKategori;

	void Init(M360_KiyafetMagazaUI menu, string kategori)
	{
		m_Menu = menu;
		m_sKategori = kategori;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_Menu || button != 0)
			return false;
		m_Menu.KategoriSec(m_sKategori);
		return true;
	}
}

//------------------------------------------------------------------------------------------------
class M360_KiyafetButonHandler : ScriptedWidgetEventHandler
{
	protected M360_KiyafetMagazaUI m_Menu;
	protected string m_sAksiyon;

	void Init(M360_KiyafetMagazaUI menu, string aksiyon)
	{
		m_Menu = menu;
		m_sAksiyon = aksiyon;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_Menu || button != 0)
			return false;

		if (m_sAksiyon == "buy")
			m_Menu.SatinAlBaslat();
		else if (m_sAksiyon == "close")
			m_Menu.Close();
		return true;
	}
}

//------------------------------------------------------------------------------------------------
//! Preview mouse: Workspace handler (vanilla SCR_InventoryCharacterWidgetHelper gibi).
//! Input action / Inventory context GEREKMEZ — ham mouse delta + FrameSlot pan/zoom.
//------------------------------------------------------------------------------------------------
class M360_KiyafetOnizlemeHandler : ScriptedWidgetEventHandler
{
	protected M360_KiyafetMagazaUI m_Menu;

	void Init(M360_KiyafetMagazaUI menu)
	{
		m_Menu = menu;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_Menu || !m_Menu.OnizlemeNoktasiMi(x, y))
			return false;

		// Sol = orbit, orta = pan, sag = sifirla.
		if (button == 0)
			m_Menu.OnizlemeSurukleBaslat(x, y, false);
		else if (button == 2)
			m_Menu.OnizlemeSurukleBaslat(x, y, true);
		else if (button == 1)
			m_Menu.OnizlemeKamerayiSifirla();
		return true;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (m_Menu && (button == 0 || button == 2))
			m_Menu.OnizlemeSurukleBitir();
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (m_Menu && m_Menu.OnizlemeNoktasiMi(x, y))
		{
			m_Menu.OnizlemeZoom(wheel);
			return true;
		}
		return false;
	}
}

//------------------------------------------------------------------------------------------------
//! MenuManager tabanli kiyafet magazasi.
//! Karakter prova: ItemPreviewManager + EquipedLoadoutStorage (SCR_LoadoutPreview mantigi).
//------------------------------------------------------------------------------------------------
class M360_KiyafetMagazaUI : MenuBase
{
	protected static const ResourceName CATEGORY_LAYOUT = "UI/layouts/M360/M360_KiyafetKategori.layout";
	protected static const ResourceName ROW_LAYOUT = "UI/layouts/M360/M360_KiyafetSatir.layout";
	protected static const ResourceName PREVIEW_MANAGER = "{9F18C476AB860F3B}Prefabs/World/Game/ItemPreviewManager.et";
	protected static M360_KiyafetMagazaUI s_Aktif;

	protected Widget m_wRoot;
	protected VerticalLayoutWidget m_wItemList;
	protected VerticalLayoutWidget m_wCategoryList;
	protected ScrollLayoutWidget m_wItemScroll;
	protected ItemPreviewWidget m_wPreview;
	protected Widget m_wPreviewInputArea;
	protected TextWidget m_wCash;
	protected TextWidget m_wPrice;
	protected TextWidget m_wStatus;
	protected TextWidget m_wSelectedName;
	protected Widget m_wCloseButton;
	protected Widget m_wBuyButton;
	protected Widget m_wBuyPanel;
	protected TextWidget m_wBuyText;

	// Envanter (SCR_InventoryMenuUI) yolu: karakterin kendi SCR_CharacterInventoryPreviewAttributes.
	// new PreviewRenderAttributes() / null = kiyafet item kadrajina kilit (bel zoom).
	protected ItemPreviewManagerEntity m_PreviewManager;
	protected PreviewRenderAttributes m_CharacterPreviewAttrs;
	protected ref M360_KiyafetOnizlemeHandler m_PreviewHandler;
	protected WorkspaceWidget m_Workspace;
	protected IEntity m_PreviewEntity;
	protected ResourceName m_sKarakterPrefab;
	protected bool m_bOnizlemeSurukleniyor;
	protected bool m_bOnizlemePan;
	protected bool m_bFrameBazHazir;
	protected bool m_bSurukleLog;
	protected bool m_bFullBodyFitOk;
	protected int m_iFullBodyDeneme;
	//! Canli oyuncu prova: kapanista eski kiyafetlere donmek icin prefab listesi.
	protected ref array<ResourceName> m_aLoadoutSnapshot = {};
	protected bool m_bSnapshotAlindi;
	protected bool m_bSatinAlindiBuOturum;
	protected int m_iSonMouseX;
	protected int m_iSonMouseY;
	protected float m_fFrameBaseX;
	protected float m_fFrameBaseY;
	protected float m_fFrameBaseW;
	protected float m_fFrameBaseH;
	protected float m_fPanX;
	protected float m_fPanY;
	protected float m_fOlcek;

	protected ref array<ref M360_KiyafetUrun> m_aGorunen = {};
	protected ref array<string> m_aProvaAlan = {};
	protected ref array<ResourceName> m_aProvaPrefab = {};
	protected ref array<ResourceName> m_aSonAlinan = {};
	protected string m_sKategori = "Hepsi";
	protected int m_iSecim = -1;
	protected bool m_bKilitli;
	protected float m_fSonucBekleme;

	protected ref array<ref M360_KiyafetListeHandler> m_aListeHandlers = {};
	protected ref array<ref M360_KiyafetKategoriHandler> m_aKatHandlers = {};
	protected ref M360_KiyafetButonHandler m_BuyHandler;
	protected ref M360_KiyafetButonHandler m_CloseHandler;
	protected InputManager m_InputManager;

	//------------------------------------------------------------------------------------------------
	static M360_KiyafetMagazaUI Aktif()
	{
		return s_Aktif;
	}

	//------------------------------------------------------------------------------------------------
	static bool AcikMi()
	{
		return s_Aktif != null;
	}

	//------------------------------------------------------------------------------------------------
	static void Ac()
	{
		if (s_Aktif)
		{
			s_Aktif.Close();
			return;
		}

		MenuBase menu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.M360_KiyafetMagazaMenu);
		if (!menu)
			Print("[M360] Kiyafet magazasi MenuManager ile acilamadi", LogLevel.ERROR);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		s_Aktif = this;
		m_wRoot = GetRootWidget();
		if (!m_wRoot)
		{
			Print("[M360] Kiyafet layout yuklenemedi", LogLevel.ERROR);
			Close();
			return;
		}

		m_wItemList = VerticalLayoutWidget.Cast(m_wRoot.FindAnyWidget("ItemList"));
		m_wCategoryList = VerticalLayoutWidget.Cast(m_wRoot.FindAnyWidget("CategoryList"));
		m_wItemScroll = ScrollLayoutWidget.Cast(m_wRoot.FindAnyWidget("ItemScroll"));
		// Render target karakteri cizer; dis container guvenilir mouse hit alanidir.
		m_wPreview = ItemPreviewWidget.Cast(m_wRoot.FindAnyWidget("playerRender"));
		m_wPreviewInputArea = m_wRoot.FindAnyWidget("PreviewContainer");
		m_wCash = TextWidget.Cast(m_wRoot.FindAnyWidget("CashText"));
		m_wPrice = TextWidget.Cast(m_wRoot.FindAnyWidget("PriceText"));
		m_wStatus = TextWidget.Cast(m_wRoot.FindAnyWidget("StatusText"));
		m_wSelectedName = TextWidget.Cast(m_wRoot.FindAnyWidget("SelectedName"));
		m_wBuyButton = m_wRoot.FindAnyWidget("BuyButton");
		m_wBuyPanel = m_wRoot.FindAnyWidget("BuyBg");
		m_wBuyText = TextWidget.Cast(m_wRoot.FindAnyWidget("BuyText"));

		m_wCloseButton = m_wRoot.FindAnyWidget("CloseButton");
		if (m_wCloseButton)
			m_wCloseButton.SetColor(new Color(0.72, 0.08, 0.08, 1));

		TextWidget previewHint = TextWidget.Cast(m_wRoot.FindAnyWidget("PreviewHint"));
		if (previewHint)
		{
			previewHint.SetText("SOL: DONDUR  |  ORTA: KAYDIR  |  TEKERLEK: YAKIN/UZAK  |  SAG: FULL BODY");
			previewHint.SetFlags(WidgetFlags.IGNORE_CURSOR);
		}

		Widget previewBg = m_wRoot.FindAnyWidget("PreviewBackground");
		if (previewBg)
			previewBg.SetFlags(WidgetFlags.IGNORE_CURSOR);

		// Envanter: karakter attribute koleksiyonundan full-body kadraj.
		m_CharacterPreviewAttrs = null;
		m_bOnizlemeSurukleniyor = false;
		m_bOnizlemePan = false;
		m_bFrameBazHazir = false;
		m_bFullBodyFitOk = false;
		m_iFullBodyDeneme = 0;
		m_fPanX = 0;
		m_fPanY = 0;
		m_fOlcek = 1.0;
		m_aProvaAlan.Clear();
		m_aProvaPrefab.Clear();
		m_aSonAlinan.Clear();
		m_aLoadoutSnapshot.Clear();
		m_bSnapshotAlindi = false;
		m_bSatinAlindiBuOturum = false;
		m_Workspace = GetGame().GetWorkspace();

		m_PreviewHandler = new M360_KiyafetOnizlemeHandler();
		m_PreviewHandler.Init(this);
		// Vanilla envanter gibi: handler Workspace'te — menu leaf event yutsa bile gelisin.
		if (m_Workspace)
			m_Workspace.AddHandler(m_PreviewHandler);
		if (m_wPreview)
			m_wPreview.AddHandler(m_PreviewHandler);

		m_InputManager = GetGame().GetInputManager();
		if (m_InputManager)
		{
			m_InputManager.AddActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnMenuBack);
#ifdef WORKBENCH
			m_InputManager.AddActionListener(UIConstants.MENU_ACTION_BACK_WB, EActionTrigger.DOWN, OnMenuBack);
#endif
		}

		if (m_wBuyButton)
		{
			m_BuyHandler = new M360_KiyafetButonHandler();
			m_BuyHandler.Init(this, "buy");
			m_wBuyButton.AddHandler(m_BuyHandler);
		}
		if (m_wBuyPanel)
		{
			Widget dekor = m_wRoot.FindAnyWidget("BuyText");
			if (dekor)
				dekor.SetFlags(WidgetFlags.IGNORE_CURSOR);
		}
		if (m_wCloseButton)
		{
			m_CloseHandler = new M360_KiyafetButonHandler();
			m_CloseHandler.Init(this, "close");
			m_wCloseButton.AddHandler(m_CloseHandler);
		}

		// SCR_InventoryMenuUI.RefreshPlayerWidget ile ayni: canli player + CharacterInventoryPreviewAttributes
		LoadoutSnapshotKaydet();
		OnizlemeKarakterAttrAl();
		KarakterPrefabHazirla();
		KategorileriDoldur();
		ListeyiDoldur();
		ProvaOzetGuncelle();
		int acOk, acFail;
		string acHata;
		OnizlemeYenileSonuc(acOk, acFail, acHata);
		BakiyeyiGuncelle();
		DurumYaz("Bastikca uzerine giyer. SATIN AL = provadaki hepsi.");

		if (m_Workspace && m_wCloseButton)
			m_Workspace.SetFocusedWidget(m_wCloseButton, true);

		// Layout otursun; envanter InitQueue sonrasi force refresh gibi.
		GetGame().GetCallqueue().CallLater(OnizlemeFullBodyYenile, 80, false);
		GetGame().GetCallqueue().CallLater(OnizlemeFullBodyYenile, 200, false);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuClose()
	{
		m_bKilitli = false;
		m_fSonucBekleme = 0;
		m_bOnizlemeSurukleniyor = false;
		m_bOnizlemePan = false;
		m_bFrameBazHazir = false;
		m_fPanX = 0;
		m_fPanY = 0;
		m_fOlcek = 1.0;
		m_aProvaAlan.Clear();
		m_aProvaPrefab.Clear();
		m_aSonAlinan.Clear();
		PreviewTemizle();
		GetGame().GetCallqueue().Remove(OnizlemeYenileGecikmeli);
		GetGame().GetCallqueue().Remove(OnizlemeFullBodyYenile);
		GetGame().GetCallqueue().Remove(OnizlemeFrameBaziHazirla);
		if (m_Workspace && m_PreviewHandler)
			m_Workspace.RemoveHandler(m_PreviewHandler);
		if (m_wPreview && m_PreviewHandler)
			m_wPreview.RemoveHandler(m_PreviewHandler);
		m_PreviewHandler = null;
		m_Workspace = null;

		// Satin almadan kapattiysa canli oyuncuyu acilis kiyafetine dondur.
		if (!m_bSatinAlindiBuOturum && m_bSnapshotAlindi)
			LoadoutSnapshotGeriYukle();

		// Envanter attribute delta birikimini sifirla (canli player entity uzerinde).
		if (m_CharacterPreviewAttrs)
			m_CharacterPreviewAttrs.ResetDeltaRotation();
		m_CharacterPreviewAttrs = null;

		if (m_InputManager)
		{
			m_InputManager.RemoveActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnMenuBack);
#ifdef WORKBENCH
			m_InputManager.RemoveActionListener(UIConstants.MENU_ACTION_BACK_WB, EActionTrigger.DOWN, OnMenuBack);
#endif
		}
		m_InputManager = null;
		m_wRoot = null;

		if (s_Aktif == this)
			s_Aktif = null;
		super.OnMenuClose();
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuUpdate(float tDelta)
	{
		super.OnMenuUpdate(tDelta);

		if (m_fSonucBekleme > 0)
		{
			m_fSonucBekleme -= tDelta;
			if (m_fSonucBekleme <= 0)
				ButonSifirla();
		}

		OnizlemeSurukleGuncelle();
		BakiyeyiGuncelle();
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeSurukleBaslat(int x, int y, bool pan)
	{
		m_bOnizlemeSurukleniyor = true;
		m_bOnizlemePan = pan;
		m_bSurukleLog = false;
		m_iSonMouseX = x;
		m_iSonMouseY = y;
		if (!m_bFrameBazHazir)
			OnizlemeFrameBaziHazirla();
	}

	//------------------------------------------------------------------------------------------------
	bool OnizlemeNoktasiMi(int x, int y)
	{
		// Tum PreviewContainer (orta panel) — dar 400x500 kutuya sikisma yok.
		Widget hit = m_wPreviewInputArea;
		if (!hit)
			hit = m_wPreview;
		if (!hit)
			return false;

		float posX, posY, sizeX, sizeY;
		hit.GetScreenPos(posX, posY);
		hit.GetScreenSize(sizeX, sizeY);
		return sizeX > 1 && sizeY > 1
			&& x >= posX && x <= posX + sizeX
			&& y >= posY && y <= posY + sizeY;
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeSurukleBitir()
	{
		m_bOnizlemeSurukleniyor = false;
		m_bOnizlemePan = false;
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeSurukleGuncelle()
	{
		if (!m_bOnizlemeSurukleniyor)
			return;

		int x, y;
		WidgetManager.GetMousePos(x, y);
		int farkX = x - m_iSonMouseX;
		int farkY = y - m_iSonMouseY;
		m_iSonMouseX = x;
		m_iSonMouseY = y;
		if (farkX == 0 && farkY == 0)
			return;

		if (m_bOnizlemePan)
		{
			float px = farkX;
			float py = farkY;
			WorkspaceWidget ww = GetGame().GetWorkspace();
			if (ww)
			{
				px = ww.DPIUnscale(px);
				py = ww.DPIUnscale(py);
			}
			m_fPanX += px;
			m_fPanY += py;
			float maxPanX = Math.Max(20, m_fFrameBaseW * 0.25);
			float maxPanY = Math.Max(20, m_fFrameBaseH * 0.25);
			m_fPanX = Math.Clamp(m_fPanX, -maxPanX, maxPanX);
			m_fPanY = Math.Clamp(m_fPanY, -maxPanY, maxPanY);
			OnizlemeFrameUygula();
			return;
		}

		// Envanter: ayni attribute nesnesine DELTA RotateItemCamera (yeniden new YASAK).
		if (!m_CharacterPreviewAttrs)
			OnizlemeKarakterAttrAl();
		if (!m_CharacterPreviewAttrs)
			return;

		// inventory limits: "-30 -180 0" .. "0 180 0"
		vector rot = Vector(farkY * 0.35, farkX * 0.55, 0);
		vector limMin = Vector(-30, -180, 0);
		vector limMax = Vector(0, 180, 0);
		m_CharacterPreviewAttrs.RotateItemCamera(rot, limMin, limMax);
		OnizlemeRenderGuncelle(false);
		if (!m_bSurukleLog)
		{
			m_bSurukleLog = true;
			Print("[M360] PROVA SURUKLE character-attrs delta", LogLevel.NORMAL);
		}
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeZoom(int wheel)
	{
		if (wheel == 0)
			return;

		if (!m_CharacterPreviewAttrs)
			OnizlemeKarakterAttrAl();
		if (!m_CharacterPreviewAttrs)
			return;

		// Envanter ZoomCamera(delta). Tekerlek asagi = uzaklas (FOV+).
		m_CharacterPreviewAttrs.ZoomCamera(-wheel * 4.0, 25.0, 120.0);
		OnizlemeFrameUygula();
		OnizlemeRenderGuncelle(false);
		Print(string.Format("[M360] PROVA ZOOM character-attrs wheel=%1", wheel), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeKamerayiSifirla()
	{
		m_bOnizlemeSurukleniyor = false;
		m_bOnizlemePan = false;
		m_fPanX = 0;
		m_fPanY = 0;
		m_fOlcek = 1.0;
		if (m_CharacterPreviewAttrs)
			m_CharacterPreviewAttrs.ResetDeltaRotation();
		OnizlemeFullBodyYenile();
		Print("[M360] PROVA kamera sifir (character-attrs + force)", LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! SCR_InventoryMenuUI OnMenuOpen ile birebir: storage attribute = full-body kadraj.
	protected void OnizlemeKarakterAttrAl()
	{
		m_CharacterPreviewAttrs = null;
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		SCR_CharacterInventoryStorageComponent storage =
			SCR_CharacterInventoryStorageComponent.Cast(player.FindComponent(SCR_CharacterInventoryStorageComponent));
		if (!storage)
		{
			Print("[M360] PROVA attr: CharacterInventoryStorage yok", LogLevel.WARNING);
			return;
		}

		ItemAttributeCollection collection = storage.GetAttributes();
		if (!collection)
		{
			Print("[M360] PROVA attr: GetAttributes null", LogLevel.WARNING);
			return;
		}

		m_CharacterPreviewAttrs = PreviewRenderAttributes.Cast(
			collection.FindAttribute(SCR_CharacterInventoryPreviewAttributes));
		if (m_CharacterPreviewAttrs)
		{
			m_CharacterPreviewAttrs.ResetDeltaRotation();
			Print("[M360] PROVA attr: SCR_CharacterInventoryPreviewAttributes OK", LogLevel.NORMAL);
		}
		else
			Print("[M360] PROVA attr: CharacterInventoryPreviewAttributes YOK", LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	//! force true = envanter RefreshPlayerWidget; false = UpdateCharacterPreview.
	protected void OnizlemeRenderGuncelle(bool forceRefresh)
	{
		if (!m_PreviewManager || !m_wPreview || !m_PreviewEntity)
			return;

		OnizlemeFrameUygula();
		if (m_CharacterPreviewAttrs)
			m_PreviewManager.SetPreviewItem(m_wPreview, m_PreviewEntity, m_CharacterPreviewAttrs, forceRefresh);
		else
			m_PreviewManager.SetPreviewItem(m_wPreview, m_PreviewEntity, null, forceRefresh);
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeFrameBaziHazirla()
	{
		if (!m_wPreview)
			return;

		FrameSlot.SetAnchorMin(m_wPreview, 0, 0);
		FrameSlot.SetAnchorMax(m_wPreview, 1, 1);
		FrameSlot.SetOffsets(m_wPreview, 8, 8, 8, 36);
		m_bFrameBazHazir = true;

		float sw, sh;
		m_wPreview.GetScreenSize(sw, sh);
		m_fFrameBaseW = sw;
		m_fFrameBaseH = sh;
		Print(string.Format("[M360] PROVA FRAME fill px=%1x%2 aspect=%3", sw, sh, sw / Math.Max(1, sh)), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeFrameUygula()
	{
		if (!m_wPreview)
			return;

		if (!m_bFrameBazHazir)
			OnizlemeFrameBaziHazirla();

		float l = 8 + m_fPanX;
		float t = 8 + m_fPanY;
		float r = 8 - m_fPanX;
		float b = 36 - m_fPanY;
		FrameSlot.SetAnchorMin(m_wPreview, 0, 0);
		FrameSlot.SetAnchorMax(m_wPreview, 1, 1);
		FrameSlot.SetOffsets(m_wPreview, l, t, r, b);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeRenderYenile()
	{
		OnizlemeRenderGuncelle(false);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnMenuBack()
	{
		Close();
	}

	//------------------------------------------------------------------------------------------------
	void KategoriSec(string kategori)
	{
		if (m_bKilitli)
			return;
		m_sKategori = kategori;
		m_iSecim = -1;
		KategorileriDoldur();
		ListeyiDoldur();
		ListeScrollSifirla();
		GetGame().GetCallqueue().CallLater(ListeScrollSifirla, 50, false);
	}

	//------------------------------------------------------------------------------------------------
	void SecimYap(int index)
	{
		if (m_bKilitli)
			return;
		if (index < 0 || index >= m_aGorunen.Count())
			return;

		m_iSecim = index;
		M360_KiyafetUrun u = m_aGorunen[index];

		string ekHata;
		if (!ProvaEkle(u.m_sPrefab, ekHata))
		{
			Print(string.Format("[M360] PROVA EKLE FAIL %1 | %2", FilePath.StripPath(u.m_sPrefab), ekHata), LogLevel.WARNING);
			DurumYaz("Prova eklenemedi: " + ekHata);
			return;
		}

		// Canli oyuncu (envanter full-body). Attach + refresh.
		if (!m_PreviewEntity || !m_PreviewManager)
		{
			int okAdet;
			int failAdet;
			string lastFail;
			OnizlemeYenileSonuc(okAdet, failAdet, lastFail);
		}

		IEntity hedef = m_PreviewEntity;
		if (!hedef)
			hedef = SCR_PlayerController.GetLocalControlledEntity();

		string hGiy;
		if (!KiyafetiDene(hedef, u.m_sPrefab, hGiy))
		{
			ProvaKaldir(u.m_sPrefab);
			ListeyiDoldur();
			ProvaOzetGuncelle();
			DurumYaz("3D'ye giydirilemedi: " + hGiy);
			Print(string.Format("[M360] PROVA TIK FAIL %1 | %2", FilePath.StripPath(u.m_sPrefab), hGiy), LogLevel.WARNING);
			return;
		}

		m_PreviewEntity = hedef;
		// Envanter RefreshPlayerWidget: force true + character attrs.
		OnizlemeRenderGuncelle(true);

		ListeyiDoldur();
		ProvaOzetGuncelle();
		DurumYaz(u.m_sAd + " uzerine giyildi. Digerlerini de sec, sonra SATIN AL.");
		Print(string.Format("[M360] PROVA TIK OK %1 character-attrs", FilePath.StripPath(u.m_sPrefab)), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	void SatinAlBaslat()
	{
		if (m_bKilitli)
			return;

		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		if (prova.Count() < 1)
		{
			DurumYaz("Once listeden kiyafet sec (uzerine giyer).");
			return;
		}

		int toplam = ProvaToplamFiyat();
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!pc || !pc.M360_NakitYeterliMi(toplam))
		{
			DurumYaz("Yetersiz nakit.");
			return;
		}

		m_bKilitli = true;
		m_fSonucBekleme = 4.0;
		if (m_wBuyText)
			m_wBuyText.SetText("...");
		DurumYaz("Sunucuya gonderiliyor...");
		pc.M360_KiyafetSatinAlIsteToplu(prova);
	}

	//------------------------------------------------------------------------------------------------
	void SatinAlmaSonucu(bool basarili, string mesaj)
	{
		DurumYaz(mesaj);
		BakiyeyiGuncelle();
		m_fSonucBekleme = 1.5;
		if (!basarili)
			return;

		// Satin alinanlar artik gercek loadout; snapshot guncelle, kapanista bozma.
		m_bSatinAlindiBuOturum = true;
		LoadoutSnapshotKaydet();

		// Satin alinanlari onizlemede tut (istemci loadout sync gecikebilir).
		m_aSonAlinan.Clear();
		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		foreach (ResourceName rn : prova)
			m_aSonAlinan.Insert(rn);

		int ok, fail;
		string hata;
		OnizlemeYenileSonuc(ok, fail, hata);

		m_aProvaAlan.Clear();
		m_aProvaPrefab.Clear();
		m_iSecim = -1;
		ListeyiDoldur();
		ProvaOzetGuncelle();

		GetGame().GetCallqueue().Remove(OnizlemeYenileGecikmeli);
		GetGame().GetCallqueue().CallLater(OnizlemeYenileGecikmeli, 450, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeYenileGecikmeli()
	{
		if (!s_Aktif || s_Aktif != this)
			return;
		int ok, fail;
		string hata;
		OnizlemeYenileSonuc(ok, fail, hata);
	}

	//------------------------------------------------------------------------------------------------
	protected void ButonSifirla()
	{
		m_bKilitli = false;
		m_fSonucBekleme = 0;
		if (m_wBuyText)
			m_wBuyText.SetText("SATIN AL");
		if (m_wBuyPanel)
			m_wBuyPanel.SetColor(new Color(0.15, 0.55, 0.45, 0.95));
	}

	//------------------------------------------------------------------------------------------------
	protected void KategorileriDoldur()
	{
		if (!m_wCategoryList)
			return;

		while (m_wCategoryList.GetChildren())
			m_wCategoryList.RemoveChild(m_wCategoryList.GetChildren());

		m_aKatHandlers.Clear();
		array<string> katlar = {};
		M360_KiyafetKatalogu.Kategoriler(katlar);

		foreach (string kat : katlar)
		{
			Widget root = GetGame().GetWorkspace().CreateWidgets(CATEGORY_LAYOUT, m_wCategoryList);
			if (!root)
				continue;

			ButtonWidget btn = ButtonWidget.Cast(root.FindAnyWidget("CategoryButton"));
			TextWidget txt = TextWidget.Cast(root.FindAnyWidget("CategoryText"));
			ImageWidget bg = ImageWidget.Cast(root.FindAnyWidget("CategoryBg"));
			if (!btn)
				continue;

			if (txt)
			{
				txt.SetText(kat);
				if (kat == m_sKategori)
					txt.SetColor(new Color(0.47, 0.86, 0.71, 1));
				else
					txt.SetColor(new Color(0.82, 0.82, 0.82, 1));
			}
			if (bg)
			{
				if (kat == m_sKategori)
					bg.SetColor(new Color(0.12, 0.42, 0.37, 1));
				else
					bg.SetColor(new Color(0.105, 0.12, 0.15, 1));
			}

			M360_KiyafetKategoriHandler h = new M360_KiyafetKategoriHandler();
			h.Init(this, kat);
			btn.AddHandler(h);
			m_aKatHandlers.Insert(h);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void ListeyiDoldur()
	{
		if (!m_wItemList)
			return;

		while (m_wItemList.GetChildren())
			m_wItemList.RemoveChild(m_wItemList.GetChildren());

		m_aListeHandlers.Clear();
		m_aGorunen.Clear();

		array<ref M360_KiyafetUrun> hepsi = M360_KiyafetKatalogu.Hepsi();
		foreach (M360_KiyafetUrun u : hepsi)
		{
			if (m_sKategori != "Hepsi" && u.m_sKategori != m_sKategori)
				continue;
			m_aGorunen.Insert(u);
		}

		for (int i = 0; i < m_aGorunen.Count(); i++)
		{
			M360_KiyafetUrun u = m_aGorunen[i];
			Widget root = GetGame().GetWorkspace().CreateWidgets(ROW_LAYOUT, m_wItemList);
			if (!root)
				continue;

			ButtonWidget btn = ButtonWidget.Cast(root.FindAnyWidget("ClothingRowButton"));
			TextWidget name = TextWidget.Cast(root.FindAnyWidget("RowName"));
			TextWidget price = TextWidget.Cast(root.FindAnyWidget("RowPrice"));
			ImageWidget bg = ImageWidget.Cast(root.FindAnyWidget("RowBg"));
			if (!btn)
				continue;

			if (name)
			{
				bool prova = ProvaIcindeMi(u.m_sPrefab);
				name.SetText(u.m_sAd);
				if (prova || i == m_iSecim)
					name.SetColor(new Color(1, 0.8, 0.38, 1));
				else
					name.SetColor(new Color(0.92, 0.94, 0.97, 1));
			}
			if (price)
				price.SetText("$" + M360_HudYazi.NakitFormat(u.m_iFiyat));
			if (bg && (ProvaIcindeMi(u.m_sPrefab) || i == m_iSecim))
				bg.SetColor(new Color(0.2, 0.3, 0.3, 1));

			M360_KiyafetListeHandler h = new M360_KiyafetListeHandler();
			h.Init(this, i);
			btn.AddHandler(h);
			m_aListeHandlers.Insert(h);
		}

		if (m_iSecim >= m_aGorunen.Count())
			m_iSecim = -1;
	}

	//------------------------------------------------------------------------------------------------
	protected void ListeScrollSifirla()
	{
		if (m_wItemScroll)
			m_wItemScroll.SetSliderPos(0, 0);
	}

	//------------------------------------------------------------------------------------------------
	protected void BakiyeyiGuncelle()
	{
		if (!m_wCash)
			return;
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		int nakit = 0;
		if (pc)
			nakit = pc.M360_NakitAl();
		m_wCash.SetText("$" + M360_HudYazi.NakitFormat(nakit));
	}

	//------------------------------------------------------------------------------------------------
	protected void DurumYaz(string mesaj)
	{
		if (m_wStatus)
			m_wStatus.SetText(mesaj);
	}

	//------------------------------------------------------------------------------------------------
	protected void KarakterPrefabHazirla()
	{
		m_sKarakterPrefab = ResourceName.Empty;
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		EntityPrefabData data = player.GetPrefabData();
		if (data)
			m_sKarakterPrefab = data.GetPrefabName();
	}

	//------------------------------------------------------------------------------------------------
	//! Prefab'in gercek loadout alan anahtari (kask/sapka carpismaz).
	protected string PrefabLoadoutAlan(ResourceName prefab, out string hata)
	{
		hata = string.Empty;
		if (!prefab)
		{
			hata = "prefab bos";
			return string.Empty;
		}

		Resource res = Resource.Load(prefab);
		if (!res.IsValid())
		{
			hata = "resource yuklenemedi (Wrong GUID/name?)";
			Print(string.Format("[M360] PROVA RESOURCE FAIL %1", prefab), LogLevel.ERROR);
			return string.Empty;
		}

		ChimeraWorld world = GetGame().GetWorld();
		if (!world)
		{
			hata = "world yok";
			return string.Empty;
		}

		IEntity tmp = GetGame().SpawnEntityPrefabLocal(res, world);
		if (!tmp)
		{
			hata = "spawn olmadi";
			return string.Empty;
		}

		BaseLoadoutClothComponent clothComp = BaseLoadoutClothComponent.Cast(tmp.FindComponent(BaseLoadoutClothComponent));
		if (!clothComp)
		{
			delete tmp;
			hata = "BaseLoadoutClothComponent yok (*_item.et / yanlis prefab)";
			return string.Empty;
		}

		LoadoutAreaType area = clothComp.GetAreaType();
		if (!area)
		{
			delete tmp;
			hata = "LoadoutAreaType yok";
			return string.Empty;
		}

		string key = string.Format("%1", area.Type());
		delete tmp;
		return key;
	}

	//------------------------------------------------------------------------------------------------
	//! Canli oyuncu + SCR_CharacterInventoryPreviewAttributes (vanilla envanter).
	protected void OnizlemeYenileSonuc(out int okAdet, out int failAdet, out string lastFail)
	{
		okAdet = 0;
		failAdet = 0;
		lastFail = string.Empty;

		if (!m_wPreview)
		{
			lastFail = "playerRender widget yok";
			return;
		}

		ChimeraWorld world = GetGame().GetWorld();
		if (!world)
		{
			lastFail = "world yok";
			return;
		}

		m_PreviewManager = world.GetItemPreviewManager();
		if (!m_PreviewManager)
		{
			Resource res = Resource.Load(PREVIEW_MANAGER);
			if (res.IsValid())
				GetGame().SpawnEntityPrefabLocal(res, world);
			m_PreviewManager = world.GetItemPreviewManager();
		}
		if (!m_PreviewManager)
		{
			lastFail = "ItemPreviewManager yok";
			DurumYaz("3D onizleme yoneticisi yok.");
			return;
		}

		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
		{
			lastFail = "oyuncu yok";
			return;
		}

		m_PreviewEntity = player;
		if (!m_CharacterPreviewAttrs)
			OnizlemeKarakterAttrAl();

		foreach (ResourceName alinan : m_aSonAlinan)
		{
			string h;
			if (KiyafetiDene(player, alinan, h))
				okAdet++;
			else
			{
				failAdet++;
				lastFail = h;
			}
		}

		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		foreach (ResourceName rn : prova)
		{
			string h;
			if (KiyafetiDene(player, rn, h))
				okAdet++;
			else
			{
				failAdet++;
				lastFail = h;
				Print(string.Format("[M360] PROVA GIY FAIL %1 | %2", FilePath.StripPath(rn), h), LogLevel.WARNING);
			}
		}

		OnizlemeFrameBaziHazirla();
		// RefreshPlayerWidget: force true + character attrs
		OnizlemeRenderGuncelle(true);
		m_bFullBodyFitOk = true;

		string attrDurum = "YOK";
		if (m_CharacterPreviewAttrs)
			attrDurum = "SCR_CharacterInventoryPreviewAttributes";
		Print(string.Format("[M360] PROVA RENDER envanter-yolu ok=%1 fail=%2 attr=%3", okAdet, failAdet, attrDurum), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeFullBodyYenile()
	{
		if (!m_wPreview)
			return;

		if (!m_PreviewManager)
		{
			ChimeraWorld world = GetGame().GetWorld();
			if (world)
				m_PreviewManager = world.GetItemPreviewManager();
		}
		if (!m_PreviewManager)
			return;

		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		m_PreviewEntity = player;
		if (!m_CharacterPreviewAttrs)
			OnizlemeKarakterAttrAl();
		if (m_CharacterPreviewAttrs)
			m_CharacterPreviewAttrs.ResetDeltaRotation();

		OnizlemeFrameBaziHazirla();
		OnizlemeRenderGuncelle(true);
		m_bFullBodyFitOk = true;

		float sw, sh;
		m_wPreview.GetScreenSize(sw, sh);
		Print(string.Format("[M360] PROVA FULLBODY envanter-yolu px=%1x%2 attr=%3", sw, sh, m_CharacterPreviewAttrs != null), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void LoadoutSnapshotKaydet()
	{
		m_aLoadoutSnapshot.Clear();
		m_bSnapshotAlindi = false;
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		EquipedLoadoutStorageComponent lo = EquipedLoadoutStorageComponent.Cast(player.FindComponent(EquipedLoadoutStorageComponent));
		if (!lo)
			return;

		int count = lo.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			IEntity cloth = lo.Get(i);
			if (!cloth)
				continue;
			if (!BaseLoadoutClothComponent.Cast(cloth.FindComponent(BaseLoadoutClothComponent)))
				continue;
			EntityPrefabData pd = cloth.GetPrefabData();
			if (!pd)
				continue;
			ResourceName rn = pd.GetPrefabName();
			if (rn)
				m_aLoadoutSnapshot.Insert(rn);
		}
		m_bSnapshotAlindi = true;
		Print(string.Format("[M360] PROVA snapshot n=%1", m_aLoadoutSnapshot.Count()), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void LoadoutSnapshotGeriYukle()
	{
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return;

		EquipedLoadoutStorageComponent lo = EquipedLoadoutStorageComponent.Cast(player.FindComponent(EquipedLoadoutStorageComponent));
		if (!lo)
			return;

		// Prova spawn'larini temizle (yalniz kiyafet slotlari).
		int count = lo.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			InventoryStorageSlot slot = lo.GetSlot(i);
			if (!slot)
				continue;
			IEntity mevcut = slot.GetAttachedEntity();
			if (!mevcut)
				continue;
			if (!BaseLoadoutClothComponent.Cast(mevcut.FindComponent(BaseLoadoutClothComponent)))
				continue;
			slot.DetachEntity(false);
			if (!mevcut.IsDeleted())
				delete mevcut;
		}

		foreach (ResourceName rn : m_aLoadoutSnapshot)
		{
			string h;
			KiyafetiDene(player, rn, h);
		}

		Print(string.Format("[M360] PROVA snapshot restore n=%1", m_aLoadoutSnapshot.Count()), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! Preview entity slotlarinda bu prefab takili mi?
	protected bool PreviewdePrefabVarMi(ResourceName prefab)
	{
		if (!m_PreviewEntity || !prefab)
			return false;

		EquipedLoadoutStorageComponent dst = EquipedLoadoutStorageComponent.Cast(m_PreviewEntity.FindComponent(EquipedLoadoutStorageComponent));
		if (!dst)
			return false;

		int count = dst.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			IEntity cloth = dst.Get(i);
			if (!cloth)
				continue;

			EntityPrefabData pdata = cloth.GetPrefabData();
			if (!pdata)
				continue;

			if (M360_MagazaYardim.PrefabEslesir(pdata.GetPrefabName(), prefab))
				return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------------------------
	protected bool ProvaEkle(ResourceName prefab, out string hata)
	{
		hata = string.Empty;
		M360_KiyafetUrun urun = M360_KiyafetKatalogu.Bul(prefab);
		if (!urun)
		{
			hata = "katalogda yok";
			return false;
		}

		// Ayni fiziksel slot: gercek LoadoutAreaType (kategori degil).
		string alan = PrefabLoadoutAlan(prefab, hata);
		if (alan.Length() < 1)
		{
			// Fall-back: en azinda yigin kaydi kategoriden tutulsun ama basarisizlik loglu.
			alan = "kat:" + urun.m_sKategori;
			Print(string.Format("[M360] PROVA ALAN FALLBACK %1 -> %2 | %3", FilePath.StripPath(prefab), alan, hata), LogLevel.WARNING);
			if (hata.Length() > 0 && hata.Contains("resource"))
				return false;
			hata = string.Empty;
		}

		int idx = m_aProvaAlan.Find(alan);
		if (idx >= 0)
		{
			m_aProvaPrefab[idx] = prefab;
			return true;
		}

		m_aProvaAlan.Insert(alan);
		m_aProvaPrefab.Insert(prefab);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void ProvaKaldir(ResourceName prefab)
	{
		array<string> kalanAlan = {};
		array<ResourceName> kalanPrefab = {};

		for (int i = 0; i < m_aProvaPrefab.Count(); i++)
		{
			if (M360_MagazaYardim.PrefabEslesir(m_aProvaPrefab[i], prefab))
				continue;

			kalanPrefab.Insert(m_aProvaPrefab[i]);
			if (i < m_aProvaAlan.Count())
				kalanAlan.Insert(m_aProvaAlan[i]);
		}

		m_aProvaPrefab.Clear();
		m_aProvaAlan.Clear();
		foreach (ResourceName rn : kalanPrefab)
			m_aProvaPrefab.Insert(rn);
		foreach (string al : kalanAlan)
			m_aProvaAlan.Insert(al);
	}

	//------------------------------------------------------------------------------------------------
	protected void ProvaListesiAl(out array<ResourceName> outList)
	{
		outList = {};
		foreach (ResourceName rn : m_aProvaPrefab)
			outList.Insert(rn);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ProvaIcindeMi(ResourceName prefab)
	{
		foreach (ResourceName rn : m_aProvaPrefab)
		{
			if (M360_MagazaYardim.PrefabEslesir(rn, prefab))
				return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------------------------
	protected int ProvaToplamFiyat()
	{
		int toplam = 0;
		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		foreach (ResourceName rn : prova)
		{
			int f = M360_KiyafetKatalogu.Fiyat(rn);
			if (f > 0)
				toplam += f;
		}
		return toplam;
	}

	//------------------------------------------------------------------------------------------------
	protected void ProvaOzetGuncelle()
	{
		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		int adet = prova.Count();
		int toplam = ProvaToplamFiyat();

		if (m_wSelectedName)
		{
			if (adet < 1)
				m_wSelectedName.SetText("Prova bos");
			else if (adet == 1)
			{
				M360_KiyafetUrun u = M360_KiyafetKatalogu.Bul(prova[0]);
				if (u)
					m_wSelectedName.SetText(u.m_sAd);
				else
					m_wSelectedName.SetText("1 parca");
			}
			else
				m_wSelectedName.SetText(string.Format("%1 parca prova", adet));
		}

		if (m_wPrice)
		{
			if (adet < 1)
				m_wPrice.SetText("Fiyat: $0");
			else
				m_wPrice.SetText(string.Format("Toplam: $%1", M360_HudYazi.NakitFormat(toplam)));
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void OyuncuKiyafetleriniKopyala(IEntity preview)
	{
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player || !preview)
			return;

		EquipedLoadoutStorageComponent src = EquipedLoadoutStorageComponent.Cast(player.FindComponent(EquipedLoadoutStorageComponent));
		EquipedLoadoutStorageComponent dst = EquipedLoadoutStorageComponent.Cast(preview.FindComponent(EquipedLoadoutStorageComponent));
		if (!src || !dst)
		{
			Print("[M360] PROVA kopya: loadout storage yok", LogLevel.WARNING);
			return;
		}

		int count = src.GetSlotsCount();
		int kopyaOk = 0;
		for (int i = 0; i < count; i++)
		{
			IEntity cloth = src.Get(i);
			if (!cloth)
				continue;

			EntityPrefabData pdata = cloth.GetPrefabData();
			if (!pdata)
				continue;

			ResourceName rn = pdata.GetPrefabName();
			// Silah / el esyasi loadout kiyafet degilse yoksay: sivil prova gorunsun.
			if (!BaseLoadoutClothComponent.Cast(cloth.FindComponent(BaseLoadoutClothComponent)))
				continue;

			string h;
			if (KiyafetiDene(preview, rn, h))
				kopyaOk++;
		}

		Print(string.Format("[M360] PROVA oyuncu kopya ok=%1 / slot=%2", kopyaOk, count), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void PreviewLoadoutTemizle(EquipedLoadoutStorageComponent loadout)
	{
		if (!loadout)
			return;

		int count = loadout.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			InventoryStorageSlot slot = loadout.GetSlot(i);
			if (!slot)
				continue;

			IEntity mevcut = slot.GetAttachedEntity();
			if (!mevcut)
				continue;

			slot.DetachEntity(false);
			if (!mevcut.IsDeleted())
				delete mevcut;
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Alan uzerindeki takimlari bosalt (kask / sapka ayni aile).
	protected void AlanSlotlariniTemizle(EquipedLoadoutStorageComponent loadout, typename areaType)
	{
		if (!loadout)
			return;

		// 1) Alan API — birincil slot
		LoadoutSlotInfo birincil = loadout.GetSlotFromArea(areaType);
		if (birincil)
		{
			IEntity eskiB = birincil.GetAttachedEntity();
			if (eskiB)
			{
				birincil.DetachEntity(false);
				if (!eskiB.IsDeleted())
					delete eskiB;
			}
		}

		// 2) Tum slot taramasi (ayni tip adi)
		string sWant = string.Format("%1", areaType);
		int count = loadout.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			InventoryStorageSlot invSlot = loadout.GetSlot(i);
			if (!invSlot)
				continue;

			LoadoutSlotInfo si = LoadoutSlotInfo.Cast(invSlot);
			if (!si)
				continue;

			LoadoutAreaType slotArea = si.GetAreaType();
			if (!slotArea)
				continue;

			string sSlot = string.Format("%1", slotArea.Type());
			if (sSlot != sWant)
				continue;

			IEntity eski = si.GetAttachedEntity();
			if (!eski)
				continue;

			si.DetachEntity(false);
			if (!eski.IsDeleted())
				delete eski;
		}
	}

	//------------------------------------------------------------------------------------------------
	protected bool KiyafetiDene(IEntity preview, ResourceName prefab, out string hata)
	{
		hata = string.Empty;
		if (!preview || !prefab)
		{
			hata = "preview/prefab null";
			return false;
		}

		EquipedLoadoutStorageComponent dst = EquipedLoadoutStorageComponent.Cast(preview.FindComponent(EquipedLoadoutStorageComponent));
		if (!dst)
		{
			hata = "preview loadout yok";
			return false;
		}

		Resource res = Resource.Load(prefab);
		if (!res.IsValid())
		{
			hata = "resource gecersiz " + FilePath.StripPath(prefab);
			return false;
		}

		IEntity cloth = GetGame().SpawnEntityPrefabLocal(res, preview.GetWorld());
		if (!cloth)
		{
			hata = "spawn null " + FilePath.StripPath(prefab);
			return false;
		}

		BaseLoadoutClothComponent clothComp = BaseLoadoutClothComponent.Cast(cloth.FindComponent(BaseLoadoutClothComponent));
		if (!clothComp)
		{
			delete cloth;
			hata = "cloth component yok";
			return false;
		}

		LoadoutAreaType area = clothComp.GetAreaType();
		if (!area)
		{
			delete cloth;
			hata = "area yok";
			return false;
		}

		typename areaType = area.Type();

		// Ayni alan ailesindeki tum takimlari kaldir (asker kask vs sivil sapka).
		AlanSlotlariniTemizle(dst, areaType);

		LoadoutSlotInfo slotInfo = dst.GetSlotFromArea(areaType);
		if (!slotInfo)
		{
			delete cloth;
			hata = string.Format("slot yok area=%1", areaType);
			return false;
		}

		// LoadoutSlotInfo dogrudan AttachEntity destekler.
		slotInfo.AttachEntity(cloth);

		IEntity takili = slotInfo.GetAttachedEntity();
		if (takili != cloth)
		{
			// Cast yolu yedek (eski kodda yalniz cast vardi)
			InventoryStorageSlot invSlot = InventoryStorageSlot.Cast(slotInfo);
			if (invSlot)
			{
				invSlot.AttachEntity(cloth);
				takili = invSlot.GetAttachedEntity();
			}
		}

		if (takili != cloth)
		{
			if (!cloth.IsDeleted())
				delete cloth;
			hata = string.Format("AttachEntity red area=%1", areaType);
			return false;
		}

		Print(string.Format("[M360] PROVA GIY OK area=%1 prefab=%2", areaType, FilePath.StripPath(prefab)), LogLevel.NORMAL);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void PreviewCocuklariSil(IEntity entity)
	{
		if (!entity)
			return;

		IEntity child = entity.GetChildren();
		while (child)
		{
			IEntity sibling = child.GetSibling();
			if (child.FindComponent(InventoryItemComponent))
			{
				PreviewCocuklariSil(child);
				if (!child.IsDeleted())
					delete child;
			}
			child = sibling;
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void PreviewTemizle()
	{
		m_PreviewEntity = null;
	}
}
