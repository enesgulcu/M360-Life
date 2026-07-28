//------------------------------------------------------------------------------------------------
//! Cekirdek HUD — sol alt 3 badge + nakit pill (status halkalari)
//------------------------------------------------------------------------------------------------
class M360_CekirdekHudWidgetlari
{
	protected static const float CIRCLE = 64;
	protected static const float CIRCLE_GAP = 8;
	protected static const float STRIP_H = 42;
	protected static const float STRIP_PAD = 12;
	protected static const float STRIP_IKON = 22;
	protected static const float STRIP_GAP = 8;
	protected static const float STRIP_PLUS = 22;
	protected static const float STRIP_MIN_W = 128;
	protected static const float IS_BAR_GENISLIK = 400;
	protected static const float IS_BAR_YUKSEKLIK = 12;

	Widget m_wKok;
	ref M360_YuvarlakBar m_Can;
	ref M360_YuvarlakBar m_Yemek;
	ref M360_YuvarlakBar m_Su;
	Widget m_wSerit;
	ImageWidget m_wPillL;
	ImageWidget m_wPillMid;
	ImageWidget m_wPillR;
	ImageWidget m_wNakitIkon;
	TextWidget m_wNakit;
	ImageWidget m_wNakitPlus;

	Widget m_wSaatKart;
	TextWidget m_wSaat;
	TextWidget m_wTarih;

	Widget m_wIsPanel;
	ImageWidget m_wIsDolgu;
	TextWidget m_wIsYazi;

	//------------------------------------------------------------------------------------------------
	bool Olustur()
	{
		WorkspaceWidget ws = GetGame().GetWorkspace();
		if (!ws)
			return false;

		m_wKok = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 50);
		if (!m_wKok)
			return false;

		FrameSlot.SetAnchorMin(m_wKok, 0, 0);
		FrameSlot.SetAnchorMax(m_wKok, 1, 1);
		FrameSlot.SetOffsets(m_wKok, 0, 0, 0, 0);

		if (!VitalGrupOlustur(ws))
			return false;
		if (!SaatKartOlustur(ws))
			return false;
		if (!IsBarOlustur(ws))
			return false;

		Print("[M360] Circle HUD v9 pill", LogLevel.NORMAL);
		return m_wNakit != null;
	}

	//------------------------------------------------------------------------------------------------
	protected bool VitalGrupOlustur(notnull WorkspaceWidget ws)
	{
		float sol = 16;
		float alt = -18 - CIRCLE;

		m_Can = new M360_YuvarlakBar();
		m_Yemek = new M360_YuvarlakBar();
		m_Su = new M360_YuvarlakBar();

		bool ok1 = m_Can.Olustur(ws, m_wKok, "m_wVitalCan", sol, alt, CIRCLE,
			new Color(1.0, 0.22, 0.55, 1), M360_HudIkonlari.BADGE_CAN);
		bool ok2 = m_Yemek.Olustur(ws, m_wKok, "m_wVitalYemek", sol + CIRCLE + CIRCLE_GAP, alt, CIRCLE,
			new Color(1.0, 0.66, 0.22, 1), M360_HudIkonlari.BADGE_YEMEK);
		bool ok3 = m_Su.Olustur(ws, m_wKok, "m_wVitalSu", sol + (CIRCLE + CIRCLE_GAP) * 2, alt, CIRCLE,
			new Color(0.2, 0.92, 0.95, 1), M360_HudIkonlari.BADGE_SU);

		float seritX = sol + (CIRCLE + CIRCLE_GAP) * 3 + 10;
		float seritY = alt + (CIRCLE - STRIP_H) * 0.5;

		m_wSerit = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 0, m_wKok);
		if (!m_wSerit)
			return false;

		FrameSlot.SetAnchorMin(m_wSerit, 0, 1);
		FrameSlot.SetAnchorMax(m_wSerit, 0, 1);
		FrameSlot.SetPos(m_wSerit, seritX, seritY);
		FrameSlot.SetSize(m_wSerit, STRIP_MIN_W, STRIP_H);

		Color pillRenk = new Color(0.05, 0.06, 0.08, 0.92);
		m_wPillL = PillParca(ws, "m_wPillL", M360_HudIkonlari.PILL_L, pillRenk);
		m_wPillMid = PillParca(ws, "m_wPillMid", M360_HudIkonlari.PILL_MID, pillRenk);
		m_wPillR = PillParca(ws, "m_wPillR", M360_HudIkonlari.PILL_R, pillRenk);

		float ikonY = (STRIP_H - STRIP_IKON) * 0.5;
		m_wNakitIkon = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wSerit));
		if (m_wNakitIkon)
		{
			m_wNakitIkon.SetName("m_wNakitIkon");
			FrameSlot.SetAnchorMin(m_wNakitIkon, 0, 0);
			FrameSlot.SetAnchorMax(m_wNakitIkon, 0, 0);
			FrameSlot.SetPos(m_wNakitIkon, STRIP_PAD, ikonY);
			FrameSlot.SetSize(m_wNakitIkon, STRIP_IKON, STRIP_IKON);
			m_wNakitIkon.SetIsColorInherited(false);
			if (M360_HudIkonlari.Yukle(m_wNakitIkon, M360_HudIkonlari.NAKIT))
				m_wNakitIkon.SetColor(new Color(0.35, 0.92, 0.45, 1));
			else
				m_wNakitIkon.SetVisible(false);
		}

		m_wNakit = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(1, 1, 1, 1), 0, m_wSerit));
		if (m_wNakit)
		{
			m_wNakit.SetName("m_wNakit");
			m_wNakit.SetExactFontSize(17);
			FrameSlot.SetAnchorMin(m_wNakit, 0, 0);
			FrameSlot.SetAnchorMax(m_wNakit, 0, 0);
		}

		m_wNakitPlus = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wSerit));
		if (m_wNakitPlus)
		{
			m_wNakitPlus.SetName("m_wNakitPlus");
			FrameSlot.SetAnchorMin(m_wNakitPlus, 0, 0);
			FrameSlot.SetAnchorMax(m_wNakitPlus, 0, 0);
			m_wNakitPlus.SetIsColorInherited(false);
			if (M360_HudIkonlari.Yukle(m_wNakitPlus, M360_HudIkonlari.PLUS))
				m_wNakitPlus.SetColor(new Color(0.85, 0.88, 0.9, 1));
			else
				m_wNakitPlus.SetVisible(false);
		}

		NakitYaz(2750499);
		return ok1 && ok2 && ok3 && m_wNakit != null;
	}

	//------------------------------------------------------------------------------------------------
	protected ImageWidget PillParca(notnull WorkspaceWidget ws, string ad, ResourceName tex, notnull Color renk)
	{
		ImageWidget img = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wSerit));
		if (!img)
			return null;

		img.SetName(ad);
		FrameSlot.SetAnchorMin(img, 0, 0);
		FrameSlot.SetAnchorMax(img, 0, 0);
		img.SetIsColorInherited(false);
		if (M360_HudIkonlari.Yukle(img, tex))
			img.SetColor(renk);
		else
			img.SetVisible(false);

		return img;
	}

	//------------------------------------------------------------------------------------------------
	//! [pad][wallet][gap][$text][gap][plus][pad] — sayi uzayinca serit genisler
	protected void NakitSeritYerles(string yazi)
	{
		if (!m_wSerit || !m_wNakit)
			return;

		float textW = NakitMetinGenislik(yazi);
		float textH = 20;
		float textX = STRIP_PAD + STRIP_IKON + STRIP_GAP;
		float textY = (STRIP_H - textH) * 0.5;
		float plusX = textX + textW + STRIP_GAP;
		float plusY = (STRIP_H - STRIP_PLUS) * 0.5;
		float stripW = plusX + STRIP_PLUS + STRIP_PAD;
		if (stripW < STRIP_MIN_W)
			stripW = STRIP_MIN_W;

		FrameSlot.SetSize(m_wSerit, stripW, STRIP_H);

		// Oval pill: L + mid + R (cap kare = yukseklik)
		float cap = STRIP_H;
		float midW = stripW - (cap * 2.0);
		if (midW < 4)
			midW = 4;

		if (m_wPillL)
		{
			FrameSlot.SetPos(m_wPillL, 0, 0);
			FrameSlot.SetSize(m_wPillL, cap, STRIP_H);
		}
		if (m_wPillMid)
		{
			FrameSlot.SetPos(m_wPillMid, cap, 0);
			FrameSlot.SetSize(m_wPillMid, midW, STRIP_H);
		}
		if (m_wPillR)
		{
			FrameSlot.SetPos(m_wPillR, cap + midW, 0);
			FrameSlot.SetSize(m_wPillR, cap, STRIP_H);
		}

		if (m_wNakitIkon)
		{
			FrameSlot.SetPos(m_wNakitIkon, STRIP_PAD, (STRIP_H - STRIP_IKON) * 0.5);
			FrameSlot.SetSize(m_wNakitIkon, STRIP_IKON, STRIP_IKON);
		}

		FrameSlot.SetPos(m_wNakit, textX, textY);
		FrameSlot.SetSize(m_wNakit, textW, textH);

		if (m_wNakitPlus)
		{
			FrameSlot.SetPos(m_wNakitPlus, plusX, plusY);
			FrameSlot.SetSize(m_wNakitPlus, STRIP_PLUS, STRIP_PLUS);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected float NakitMetinGenislik(string yazi)
	{
		int n = yazi.Length();
		if (n < 1)
			n = 1;
		float w = 8.0 + (n * 10.0);
		if (w < 28)
			w = 28;
		return w;
	}

	//------------------------------------------------------------------------------------------------
	protected string NakitFormat(int nakit)
	{
		return M360_HudYazi.NakitFormat(nakit);
	}

	//------------------------------------------------------------------------------------------------
	protected bool SaatKartOlustur(notnull WorkspaceWidget ws)
	{
		m_wSaatKart = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 0, m_wKok);
		if (!m_wSaatKart)
			return false;

		FrameSlot.SetAnchorMin(m_wSaatKart, 1, 0);
		FrameSlot.SetAnchorMax(m_wSaatKart, 1, 0);
		FrameSlot.SetPos(m_wSaatKart, -140, 14);
		FrameSlot.SetSize(m_wSaatKart, 124, 44);

		ImageWidget arka = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.04, 0.05, 0.07, 0.82), 0, m_wSaatKart));
		if (arka)
		{
			FrameSlot.SetAnchorMin(arka, 0, 0);
			FrameSlot.SetAnchorMax(arka, 1, 1);
			FrameSlot.SetOffsets(arka, 0, 0, 0, 0);
		}

		m_wSaat = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(1, 1, 1, 1), 0, m_wSaatKart));
		if (m_wSaat)
		{
			m_wSaat.SetName("m_wSaat");
			m_wSaat.SetText("00:00");
			m_wSaat.SetExactFontSize(18);
			FrameSlot.SetAnchorMin(m_wSaat, 0, 0);
			FrameSlot.SetAnchorMax(m_wSaat, 1, 0);
			FrameSlot.SetPos(m_wSaat, 10, 4);
			FrameSlot.SetSize(m_wSaat, 104, 22);
		}

		m_wTarih = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.65, 0.7, 0.74, 1), 0, m_wSaatKart));
		if (m_wTarih)
		{
			m_wTarih.SetName("m_wTarih");
			m_wTarih.SetText("01.01.2026");
			m_wTarih.SetExactFontSize(11);
			FrameSlot.SetAnchorMin(m_wTarih, 0, 0);
			FrameSlot.SetAnchorMax(m_wTarih, 1, 1);
			FrameSlot.SetPos(m_wTarih, 10, 26);
			FrameSlot.SetSize(m_wTarih, 104, 14);
		}

		return m_wSaat != null;
	}

	//------------------------------------------------------------------------------------------------
	protected bool IsBarOlustur(notnull WorkspaceWidget ws)
	{
		m_wIsPanel = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 0, m_wKok);
		if (!m_wIsPanel)
			return false;

		FrameSlot.SetAnchorMin(m_wIsPanel, 0.5, 1);
		FrameSlot.SetAnchorMax(m_wIsPanel, 0.5, 1);
		FrameSlot.SetPos(m_wIsPanel, -IS_BAR_GENISLIK * 0.5, -118);
		FrameSlot.SetSize(m_wIsPanel, IS_BAR_GENISLIK, 44);
		m_wIsPanel.SetVisible(false);

		ImageWidget arka = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.04, 0.05, 0.07, 0.88), 0, m_wIsPanel));
		if (arka)
		{
			FrameSlot.SetAnchorMin(arka, 0, 0);
			FrameSlot.SetAnchorMax(arka, 1, 1);
			FrameSlot.SetOffsets(arka, 0, 0, 0, 0);
		}

		ImageWidget track = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.14, 0.15, 0.18, 1), 0, m_wIsPanel));
		if (track)
		{
			FrameSlot.SetAnchorMin(track, 0, 0);
			FrameSlot.SetAnchorMax(track, 0, 0);
			FrameSlot.SetPos(track, 14, 24);
			FrameSlot.SetSize(track, IS_BAR_GENISLIK - 28, IS_BAR_YUKSEKLIK);
		}

		m_wIsDolgu = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.28, 0.84, 0.48, 1), 0, m_wIsPanel));
		if (m_wIsDolgu)
		{
			FrameSlot.SetAnchorMin(m_wIsDolgu, 0, 0);
			FrameSlot.SetAnchorMax(m_wIsDolgu, 0, 0);
			FrameSlot.SetPos(m_wIsDolgu, 14, 24);
			FrameSlot.SetSize(m_wIsDolgu, 0, IS_BAR_YUKSEKLIK);
		}

		m_wIsYazi = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.92, 0.94, 0.96, 1), 0, m_wIsPanel));
		if (m_wIsYazi)
		{
			m_wIsYazi.SetName("m_wIlerlemeYazi");
			m_wIsYazi.SetText("");
			m_wIsYazi.SetExactFontSize(12);
			FrameSlot.SetAnchorMin(m_wIsYazi, 0, 0);
			FrameSlot.SetAnchorMax(m_wIsYazi, 1, 0);
			FrameSlot.SetPos(m_wIsYazi, 14, 5);
			FrameSlot.SetSize(m_wIsYazi, IS_BAR_GENISLIK - 28, 16);
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	void NakitYaz(int nakit)
	{
		if (!m_wNakit)
			return;

		string yazi = string.Format("$%1", NakitFormat(nakit));
		m_wNakit.SetText(yazi);
		NakitSeritYerles(yazi);
	}

	//------------------------------------------------------------------------------------------------
	void SaatYaz(string saat, string tarih)
	{
		if (m_wSaat)
			m_wSaat.SetText(saat);
		if (m_wTarih)
			m_wTarih.SetText(tarih);
	}

	//------------------------------------------------------------------------------------------------
	void VitalYaz(float yemek, float su, float can)
	{
		if (m_Can)
			m_Can.Set(can);
		if (m_Yemek)
			m_Yemek.Set(yemek);
		if (m_Su)
			m_Su.Set(su);
	}

	//------------------------------------------------------------------------------------------------
	void IlerlemeYaz(bool goster, string etiket, float yuzde)
	{
		if (!m_wIsPanel)
			return;

		m_wIsPanel.SetVisible(goster);
		if (!goster)
			return;

		if (yuzde < 0)
			yuzde = 0;
		if (yuzde > 100)
			yuzde = 100;

		if (m_wIsYazi)
			m_wIsYazi.SetText(string.Format("%1   %2%%", etiket, (int)yuzde));

		if (m_wIsDolgu)
		{
			float maxW = IS_BAR_GENISLIK - 28;
			float w = maxW * (yuzde / 100.0);
			FrameSlot.SetPos(m_wIsDolgu, 14, 24);
			FrameSlot.SetSize(m_wIsDolgu, w, IS_BAR_YUKSEKLIK);
		}
	}

	//------------------------------------------------------------------------------------------------
	void YokEt()
	{
		if (m_Can)
			m_Can.YokEt();
		if (m_Yemek)
			m_Yemek.YokEt();
		if (m_Su)
			m_Su.YokEt();
		m_Can = null;
		m_Yemek = null;
		m_Su = null;

		if (m_wKok)
			m_wKok.RemoveFromHierarchy();
		m_wKok = null;
		m_wSerit = null;
		m_wPillL = null;
		m_wPillMid = null;
		m_wPillR = null;
		m_wNakitIkon = null;
		m_wNakit = null;
		m_wNakitPlus = null;
		m_wSaatKart = null;
		m_wSaat = null;
		m_wTarih = null;
		m_wIsPanel = null;
		m_wIsDolgu = null;
		m_wIsYazi = null;
	}
}
