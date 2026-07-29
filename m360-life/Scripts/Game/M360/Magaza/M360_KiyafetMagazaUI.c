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
//! Preview alaninda sinirli orbit + zoom; input action kullanmaz.
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
		if (button == 0)
			m_Menu.OnizlemeSurukleBaslat(x, y);
		else if (button == 1)
			m_Menu.OnizlemeKamerayiSifirla();
		return true;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (m_Menu && button == 0)
			m_Menu.OnizlemeSurukleBitir();
		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Menu)
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
	protected static const ResourceName CATEGORY_LAYOUT = "{ADF987310AA53260}UI/layouts/M360/M360_KiyafetKategori.layout";
	protected static const ResourceName ROW_LAYOUT = "{ADF987310AA53261}UI/layouts/M360/M360_KiyafetSatir.layout";
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

	protected ItemPreviewManagerEntity m_PreviewManager;
	protected ref PreviewRenderAttributes m_PreviewAttributes;
	protected ref M360_KiyafetOnizlemeHandler m_PreviewHandler;
	protected IEntity m_PreviewEntity;
	protected ResourceName m_sKarakterPrefab;
	protected bool m_bOnizlemeSurukleniyor;
	protected bool m_bOnizlemeDokunuldu;
	protected int m_iSonMouseX;
	protected float m_fOnizlemeYaw;
	protected float m_fOnizlemeZoom;

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
			previewHint.SetText("SOL TIK SURUKLE: DONDUR  |  TEKERLEK: ZOOM  |  SAG TIK: SIFIRLA");

		// Ilk kadraj motor otomatigi: attributes null. Kullanici dondurunce/zoomlayinca olusur.
		m_PreviewAttributes = null;
		m_bOnizlemeDokunuldu = false;
		m_fOnizlemeYaw = 0;
		m_fOnizlemeZoom = 0;
		m_aProvaAlan.Clear();
		m_aProvaPrefab.Clear();
		m_aSonAlinan.Clear();
		WorkspaceWidget workspace = GetGame().GetWorkspace();

		m_PreviewHandler = new M360_KiyafetOnizlemeHandler();
		m_PreviewHandler.Init(this);
		// Preview container + root: tekerlek/surukleme kaybolmasin.
		if (m_wPreviewInputArea)
			m_wPreviewInputArea.AddHandler(m_PreviewHandler);
		if (m_wRoot)
			m_wRoot.AddHandler(m_PreviewHandler);

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

		KarakterPrefabHazirla();
		KategorileriDoldur();
		ListeyiDoldur();
		ProvaOzetGuncelle();
		OnizlemeYenile();
		BakiyeyiGuncelle();
		DurumYaz("Bastikca uzerine giyer. SATIN AL = provadaki hepsi.");

		if (workspace && m_wCloseButton)
			workspace.SetFocusedWidget(m_wCloseButton, true);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuClose()
	{
		m_bKilitli = false;
		m_fSonucBekleme = 0;
		m_bOnizlemeDokunuldu = false;
		m_fOnizlemeYaw = 0;
		m_fOnizlemeZoom = 0;
		m_aProvaAlan.Clear();
		m_aProvaPrefab.Clear();
		m_aSonAlinan.Clear();
		PreviewTemizle();
		GetGame().GetCallqueue().Remove(OnizlemeYenileGecikmeli);
		if (m_wPreviewInputArea && m_PreviewHandler)
			m_wPreviewInputArea.RemoveHandler(m_PreviewHandler);
		if (m_wRoot && m_PreviewHandler)
			m_wRoot.RemoveHandler(m_PreviewHandler);
		m_PreviewHandler = null;
		if (m_InputManager)
		{
			m_InputManager.RemoveActionListener(UIConstants.MENU_ACTION_BACK, EActionTrigger.DOWN, OnMenuBack);
#ifdef WORKBENCH
			m_InputManager.RemoveActionListener(UIConstants.MENU_ACTION_BACK_WB, EActionTrigger.DOWN, OnMenuBack);
#endif
		}
		m_InputManager = null;
		m_PreviewAttributes = null;
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
	void OnizlemeSurukleBaslat(int x, int y)
	{
		m_bOnizlemeSurukleniyor = true;
		m_iSonMouseX = x;
	}

	//------------------------------------------------------------------------------------------------
	bool OnizlemeNoktasiMi(int x, int y)
	{
		if (!m_wPreviewInputArea)
			return false;

		float posX, posY, sizeX, sizeY;
		m_wPreviewInputArea.GetScreenPos(posX, posY);
		m_wPreviewInputArea.GetScreenSize(sizeX, sizeY);
		return x >= posX && x <= posX + sizeX && y >= posY && y <= posY + sizeY;
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeSurukleBitir()
	{
		m_bOnizlemeSurukleniyor = false;
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeSurukleGuncelle()
	{
		if (!m_bOnizlemeSurukleniyor)
			return;

		int x, y;
		WidgetManager.GetMousePos(x, y);
		int farkX = x - m_iSonMouseX;
		m_iSonMouseX = x;
		if (farkX == 0)
			return;

		m_fOnizlemeYaw = Math.Clamp(m_fOnizlemeYaw + farkX * 0.55, -180, 180);
		OnizlemeAttributesUygula();
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeZoom(int wheel)
	{
		if (wheel == 0)
			return;

		// Tekerlek: + yakinlas, - uzaklas.
		m_fOnizlemeZoom = Math.Clamp(m_fOnizlemeZoom + wheel * 4.0, -30, 30);
		OnizlemeAttributesUygula();
	}

	//------------------------------------------------------------------------------------------------
	void OnizlemeKamerayiSifirla()
	{
		m_bOnizlemeSurukleniyor = false;
		m_bOnizlemeDokunuldu = false;
		m_fOnizlemeYaw = 0;
		m_fOnizlemeZoom = 0;
		m_PreviewAttributes = null;
		OnizlemeRenderYenile();
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeAttributesUygula()
	{
		// Her seferinde sifirdan: ZoomCamera/Rotate delta biriktirir, mutlak state isteriz.
		m_PreviewAttributes = new PreviewRenderAttributes();
		m_bOnizlemeDokunuldu = true;

		if (m_fOnizlemeYaw != 0)
			m_PreviewAttributes.RotateItemCamera(Vector(0, m_fOnizlemeYaw, 0), "0 -180 0", "0 180 0");

		// Pozitif m_fOnizlemeZoom = yakinlas (FOV dusur).
		if (m_fOnizlemeZoom != 0)
			m_PreviewAttributes.ZoomCamera(-m_fOnizlemeZoom, 28.0, 85.0);

		OnizlemeRenderYenile();
	}

	//------------------------------------------------------------------------------------------------
	protected void OnizlemeRenderYenile()
	{
		if (!m_PreviewManager || !m_wPreview || !m_PreviewEntity)
			return;

		if (m_bOnizlemeDokunuldu && m_PreviewAttributes)
			m_PreviewManager.SetPreviewItem(m_wPreview, m_PreviewEntity, m_PreviewAttributes, true);
		else
			m_PreviewManager.SetPreviewItem(m_wPreview, m_PreviewEntity, null, true);
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
		if (!ProvaEkle(u.m_sPrefab))
		{
			DurumYaz("Bu kiyafet prova edilemedi.");
			return;
		}

		ListeyiDoldur();
		ProvaOzetGuncelle();
		OnizlemeYenile();
		DurumYaz(u.m_sAd + " uzerine giyildi. Digerlerini de sec, sonra SATIN AL.");
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

		// Satin alinanlari onizlemede tut (istemci loadout sync gecikebilir).
		m_aSonAlinan.Clear();
		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		foreach (ResourceName rn : prova)
			m_aSonAlinan.Insert(rn);

		OnizlemeYenile();

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
		OnizlemeYenile();
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
	protected void OnizlemeYenile()
	{
		if (!m_wPreview)
			return;

		ChimeraWorld world = GetGame().GetWorld();
		if (!world)
			return;

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
			DurumYaz("3D onizleme yoneticisi yok.");
			return;
		}

		if (!m_sKarakterPrefab)
		{
			if (m_iSecim >= 0 && m_iSecim < m_aGorunen.Count())
				m_PreviewManager.SetPreviewItemFromPrefab(m_wPreview, m_aGorunen[m_iSecim].m_sPrefab);
			return;
		}

		IEntity preview = m_PreviewManager.ResolvePreviewEntityForPrefab(m_sKarakterPrefab);
		if (!preview)
		{
			DurumYaz("Karakter onizlemesi olusturulamadi.");
			return;
		}

		m_PreviewEntity = preview;

		OyuncuKiyafetleriniKopyala(preview);

		// Son satin alinanlar: oyuncu sync gecikse bile onizleme dogru kalsin.
		foreach (ResourceName alinan : m_aSonAlinan)
			KiyafetiDene(preview, alinan);

		array<ResourceName> prova = {};
		ProvaListesiAl(prova);
		foreach (ResourceName rn : prova)
			KiyafetiDene(preview, rn);

		OnizlemeRenderYenile();
	}

	//------------------------------------------------------------------------------------------------
	protected bool ProvaEkle(ResourceName prefab)
	{
		M360_KiyafetUrun urun = M360_KiyafetKatalogu.Bul(prefab);
		if (!urun)
			return false;

		string alan = urun.m_sKategori;
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
			return;

		PreviewLoadoutTemizle(dst);

		int count = src.GetSlotsCount();
		for (int i = 0; i < count; i++)
		{
			IEntity cloth = src.Get(i);
			if (!cloth)
				continue;

			EntityPrefabData pdata = cloth.GetPrefabData();
			if (!pdata)
				continue;

			ResourceName rn = pdata.GetPrefabName();
			Resource res = Resource.Load(rn);
			if (!res.IsValid())
				continue;

			IEntity kopya = GetGame().SpawnEntityPrefabLocal(res, preview.GetWorld());
			if (!kopya)
				continue;

			InventoryStorageSlot slot = dst.GetSlot(i);
			if (slot)
				slot.AttachEntity(kopya);
			else
				delete kopya;
		}
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
	protected void KiyafetiDene(IEntity preview, ResourceName prefab)
	{
		if (!preview || !prefab)
			return;

		EquipedLoadoutStorageComponent dst = EquipedLoadoutStorageComponent.Cast(preview.FindComponent(EquipedLoadoutStorageComponent));
		if (!dst)
			return;

		Resource res = Resource.Load(prefab);
		if (!res.IsValid())
			return;

		IEntity cloth = GetGame().SpawnEntityPrefabLocal(res, preview.GetWorld());
		if (!cloth)
			return;

		BaseLoadoutClothComponent clothComp = BaseLoadoutClothComponent.Cast(cloth.FindComponent(BaseLoadoutClothComponent));
		if (!clothComp)
		{
			delete cloth;
			return;
		}

		LoadoutAreaType area = clothComp.GetAreaType();
		if (!area)
		{
			delete cloth;
			return;
		}

		LoadoutSlotInfo slotInfo = dst.GetSlotFromArea(area.Type());
		if (!slotInfo)
		{
			delete cloth;
			return;
		}

		IEntity eski = slotInfo.GetAttachedEntity();
		if (eski)
		{
			slotInfo.DetachEntity(false);
			if (!eski.IsDeleted())
				delete eski;
		}

		InventoryStorageSlot slot = InventoryStorageSlot.Cast(slotInfo);
		if (slot)
			slot.AttachEntity(cloth);
		else
			delete cloth;
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
