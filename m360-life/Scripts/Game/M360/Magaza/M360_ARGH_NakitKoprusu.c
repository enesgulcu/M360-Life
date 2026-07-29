// ARGH hazir arac galerisi UI/spawn akisini M360 HUD nakdine baglar.
// Satin alma yine ARGH sunucu servisi ve rate-limit uzerinden dogrulanir.
class M360_ARGH_GaleriKaydi
{
	protected static ref array<ARGH_VehicleDealerComponent> s_aGaleriler = {};

	static void Ekle(ARGH_VehicleDealerComponent galeri)
	{
		if (galeri && s_aGaleriler.Find(galeri) < 0)
			s_aGaleriler.Insert(galeri);
	}

	static ARGH_VehicleDealerComponent Bul(string galeriId)
	{
		foreach (ARGH_VehicleDealerComponent galeri : s_aGaleriler)
		{
			if (galeri && galeri.GetDealerId() == galeriId)
				return galeri;
		}
		return null;
	}
}

modded class ARGH_VehicleDealerComponent
{
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		M360_ARGH_GaleriKaydi.Ekle(this);
	}

	override void ClientRequestCatalog()
	{
		array<ref ARGH_VehicleForSaleDto> katalog = BuildCatalogFromConfig();
		if (katalog && m_OnCatalogReceived)
			m_OnCatalogReceived.Invoke(katalog);
	}

	override void ClientPurchaseVehicle(ResourceName vehiclePrefab)
	{
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!pc)
		{
			M360_ARGH_SatinAlmaSonucu(false, "invalid_player");
			return;
		}

		pc.M360_ARGH_SatinAlIste(GetDealerId(), vehiclePrefab);
	}

	void M360_ARGH_SatinAlmaSonucu(bool basarili, string hata)
	{
		string mesaj;
		if (basarili)
		{
			mesaj = "Satin alma basarili. Aracin teslim alaninda hazir.";
		}
		else if (hata == "insufficient_funds")
		{
			mesaj = "Yetersiz nakit. Ucret kesilmedi.";
		}
		else if (hata == "spawn_failed")
		{
			mesaj = "Arac olusturulamadi. Ucret iade edildi.";
		}
		else if (hata == "rate_limited")
		{
			mesaj = "Cok hizli islem yaptin. Biraz bekleyip tekrar dene.";
		}
		else if (hata == "vehicle_not_available")
		{
			mesaj = "Bu arac artik satista degil.";
		}
		else if (hata == "invalid_price")
		{
			mesaj = "Arac fiyati gecersiz.";
		}
		else
		{
			mesaj = "Satin alma tamamlanamadi: " + hata;
		}

		if (m_OnPurchaseResult)
			m_OnPurchaseResult.Invoke(basarili, mesaj);
	}
}

modded class SCR_PlayerController
{
	void M360_ARGH_SatinAlIste(string galeriId, ResourceName aracPrefab)
	{
		string prefab = aracPrefab;
		Rpc(M360_RpcAskARGHAracSatinAl, galeriId, prefab);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void M360_RpcAskARGHAracSatinAl(string galeriId, string aracPrefab)
	{
		int playerId = GetPlayerId();
		ARGH_VehicleDealerServiceComponent servis = ARGH_VehicleDealerServiceComponent.GetInstance();
		string hata;
		bool basarili = false;

		if (!servis)
			hata = "economy_unavailable";
		else
			basarili = servis.ProcessVehiclePurchase(playerId, galeriId, aracPrefab, hata);

		Rpc(M360_RpcDoARGHAracSonucu, galeriId, basarili, hata);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void M360_RpcDoARGHAracSonucu(string galeriId, bool basarili, string hata)
	{
		ARGH_VehicleDealerComponent galeri = M360_ARGH_GaleriKaydi.Bul(galeriId);
		if (galeri)
			galeri.M360_ARGH_SatinAlmaSonucu(basarili, hata);
	}
}

modded class ARGH_VehicleDealerServiceComponent
{
	override protected ARGH_VehicleDealerComponent FindDealerById(string dealerId)
	{
		return M360_ARGH_GaleriKaydi.Bul(dealerId);
	}

	override protected bool TryTakePayment(SCR_PlayerController pc, int price, out string errorReason)
	{
		errorReason = string.Empty;
		if (!pc)
		{
			errorReason = "invalid_player";
			return false;
		}

		if (!pc.M360_NakitHarca(price))
		{
			errorReason = "insufficient_funds";
			return false;
		}

		return true;
	}

	override protected void RefundPayment(SCR_PlayerController pc, int amount)
	{
		if (pc && amount > 0)
			pc.M360_NakitEkle(amount);
	}
}

modded class ARGH_VehicleDealerMenuUI
{
	override protected void UpdateBalances()
	{
		if (!m_WalletValue || !m_BankValue)
			return;

		SCR_PlayerController pc = SCR_PlayerController.Cast(
			GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
		if (!pc)
			return;

		string nakit = "$" + M360_HudYazi.NakitFormat(pc.M360_NakitAl());
		m_WalletValue.SetText(nakit);
		m_BankValue.SetText("$0");
		if (m_HeaderWalletValue)
			m_HeaderWalletValue.SetText(nakit);
		if (m_HeaderBankValue)
			m_HeaderBankValue.SetText("$0");
	}
}

class M360_ARGH_TamSatinAlButonu : ScriptedWidgetEventHandler
{
	protected ARGH_VehicleDealerOverlayUI m_pGaleriUI;

	void Kur(ARGH_VehicleDealerOverlayUI galeriUI)
	{
		m_pGaleriUI = galeriUI;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (!m_pGaleriUI || button != 0)
			return false;

		m_pGaleriUI.HandleButtonAction("buy");
		return true;
	}
}

modded class ARGH_VehicleDealerOverlayUI
{
	protected bool m_bM360SatinAlmaBekliyor;
	protected bool m_bM360ButonNabziAktif;
	protected int m_iM360ButonNabzi;
	protected int m_iM360GeriSayim;
	protected ResourceName m_rM360BekleyenArac;

	override bool Open(ARGH_VehicleDealerComponent dealer)
	{
		if (!super.Open(dealer))
			return false;

		M360_ModernGorunum();
		Widget dekor = m_Root.FindAnyWidget("BuyOverlay");
		if (dekor)
			dekor.SetFlags(WidgetFlags.IGNORE_CURSOR);
		M360_TamButonYuzeyiBagla("BuyButton");
		return true;
	}

	override void Close()
	{
		m_bM360SatinAlmaBekliyor = false;
		m_bM360ButonNabziAktif = false;
		m_rM360BekleyenArac = string.Empty;
		super.Close();
	}

	protected void M360_TamButonYuzeyiBagla(string widgetAdi)
	{
		if (!m_Root)
			return;

		Widget yuzey = m_Root.FindAnyWidget(widgetAdi);
		if (!yuzey)
			return;

		M360_ARGH_TamSatinAlButonu handler = new M360_ARGH_TamSatinAlButonu();
		handler.Kur(this);
		yuzey.AddHandler(handler);
		m_Handlers.Insert(handler);
	}

	protected void M360_ModernGorunum()
	{
		M360_Renk("DialogBackground", new Color(0.025, 0.04, 0.065, 0.98));
		M360_Renk("HeaderBg", new Color(0.035, 0.075, 0.12, 0.99));
		M360_Renk("CategoriesBg", new Color(0.025, 0.055, 0.09, 0.98));
		M360_Renk("VehiclePreviewBackground", new Color(0.015, 0.03, 0.05, 1));
		M360_Renk("GridBg", new Color(0.025, 0.05, 0.075, 0.98));
		M360_Renk("DetailsBg", new Color(0.025, 0.055, 0.09, 0.98));
		M360_Renk("BuyBg", new Color(0.08, 0.82, 0.66, 1));
		M360_Renk("BuyText", new Color(0.015, 0.08, 0.075, 1));

		M360_Metin("TitleText", "M360 ARAC GALERISI", 20);
		M360_Metin("CategoryText", "TUM ARACLAR", 14);
		M360_Metin("BuyText", "SATIN AL", 16);
		M360_Metin("WalletLabel", "M360 NAKIT BAKIYESI", 12);
		M360_Metin("ControlsHint", "Araci sec  >  fiyati kontrol et  >  SATIN AL", 12);
		M360_Metin("ControlsHint0", "Teslimat: galerinin sagindaki isaretli alan", 11);

		Widget bankRow = m_Root.FindAnyWidget("BankRow");
		if (bankRow)
			bankRow.SetVisible(false);
		Widget bankValue = m_Root.FindAnyWidget("BankValue");
		if (bankValue)
			bankValue.SetVisible(false);

		SetStatus("Aracini sec ve satin alma bilgilerini kontrol et.", false, false);
	}

	protected void M360_Renk(string ad, Color renk)
	{
		if (!m_Root)
			return;

		Widget widget = m_Root.FindAnyWidget(ad);
		if (widget)
			widget.SetColor(renk);
	}

	protected void M360_Metin(string ad, string metin, int boyut)
	{
		if (!m_Root)
			return;

		TextWidget yazi = TextWidget.Cast(m_Root.FindAnyWidget(ad));
		if (!yazi)
			return;

		yazi.SetText(metin);
		yazi.SetExactFontSize(boyut);
	}

	override void HandleButtonAction(string action)
	{
		if (action != "buy")
		{
			super.HandleButtonAction(action);
			return;
		}

		if (m_bM360SatinAlmaBekliyor)
		{
			SetStatus("Satin alma islemi devam ediyor...", false, false);
			return;
		}

		if (!m_Dealer)
		{
			SetStatus("Galeri sunucusuna ulasilamiyor.", true, false);
			return;
		}

		if (m_SelectedIndex < 0 || m_SelectedIndex >= m_Filtered.Count())
		{
			SetStatus("Once bir arac sec.", true, false);
			return;
		}

		m_bM360SatinAlmaBekliyor = true;
		m_bM360ButonNabziAktif = true;
		m_iM360ButonNabzi = 0;
		m_iM360GeriSayim = 3;
		m_rM360BekleyenArac = m_SelectedPrefab;
		M360_SatinAlmaNabzi();
		M360_SatinAlmaGeriSayim();
	}

	protected void M360_SatinAlmaGeriSayim()
	{
		if (!m_bM360SatinAlmaBekliyor || m_rM360BekleyenArac.IsEmpty())
			return;

		if (m_iM360GeriSayim > 0)
		{
			if (m_BuyText)
				m_BuyText.SetText(string.Format("SATIN ALINIYOR  %1", m_iM360GeriSayim));
			SetStatus(string.Format("Odeme ve teslimat hazirlaniyor... %1 saniye", m_iM360GeriSayim), false, false);
			m_iM360GeriSayim--;
			GetGame().GetCallqueue().CallLater(M360_SatinAlmaGeriSayim, 1000);
			return;
		}

		if (m_BuyText)
			m_BuyText.SetText("SUNUCUDA ONAYLANIYOR");
		SetStatus("Bakiye sunucuda kontrol ediliyor...", false, false);
		m_Dealer.ClientPurchaseVehicle(m_rM360BekleyenArac);
	}

	override protected void OnPurchaseResult(bool success, string message)
	{
		m_bM360ButonNabziAktif = false;
		m_rM360BekleyenArac = string.Empty;
		super.OnPurchaseResult(success, message);
		if (m_BuyText)
		{
			if (success)
				m_BuyText.SetText("TESLIM EDILDI");
			else
				m_BuyText.SetText("TEKRAR DENE");
		}
		GetGame().GetCallqueue().CallLater(UpdateBalances, 300);
		GetGame().GetCallqueue().CallLater(M360_ButonuSifirla, 1500);
	}

	protected void M360_SatinAlmaNabzi()
	{
		if (!m_bM360ButonNabziAktif || !m_BuyBg)
			return;

		if ((m_iM360ButonNabzi % 2) == 0)
			m_BuyBg.SetColor(new Color(0.2, 1, 0.82, 1));
		else
			m_BuyBg.SetColor(new Color(0.06, 0.55, 0.46, 1));

		m_iM360ButonNabzi++;
		GetGame().GetCallqueue().CallLater(M360_SatinAlmaNabzi, 160);
	}

	protected void M360_ButonuSifirla()
	{
		m_bM360SatinAlmaBekliyor = false;
		m_bM360ButonNabziAktif = false;
		m_rM360BekleyenArac = string.Empty;
		if (!m_BuyText || !m_BuyBg)
			return;

		m_BuyText.SetText("SATIN AL");
		m_BuyBg.SetColor(new Color(0.08, 0.82, 0.66, 1));
	}

	override protected void UpdateBalances()
	{
		if (!m_WalletValue || !m_BankValue)
			return;

		SCR_PlayerController pc = SCR_PlayerController.Cast(
			GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
		if (!pc)
			return;

		string nakit = "$" + M360_HudYazi.NakitFormat(pc.M360_NakitAl());
		m_WalletValue.SetText(nakit);
		m_BankValue.SetText("$0");
		if (m_HeaderWalletValue)
			m_HeaderWalletValue.SetText(nakit);
		if (m_HeaderBankValue)
			m_HeaderBankValue.SetText("$0");
	}
}
