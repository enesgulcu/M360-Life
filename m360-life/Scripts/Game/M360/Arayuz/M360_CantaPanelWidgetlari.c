//------------------------------------------------------------------------------------------------
//! Tek envanter satiri — soft pill (L/mid/R, kose esnetilmez) + ikon + ad + miktar
//------------------------------------------------------------------------------------------------
class M360_CantaSatirUi
{
	Widget m_wKok;
	ImageWidget m_wPillL;
	ImageWidget m_wPillMid;
	ImageWidget m_wPillR;
	ImageWidget m_wIkonKutu;
	ImageWidget m_wIkon;
	TextWidget m_wAd;
	TextWidget m_wMiktar;
	float m_fW;
	float m_fH;

	//------------------------------------------------------------------------------------------------
	bool Olustur(notnull WorkspaceWidget ws, notnull Widget ebeveyn, float x, float y, float w, float h)
	{
		m_fW = w;
		m_fH = h;

		m_wKok = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 0, ebeveyn);
		if (!m_wKok)
			return false;
		m_wKok.SetEnabled(false);

		FrameSlot.SetAnchorMin(m_wKok, 0, 0);
		FrameSlot.SetAnchorMax(m_wKok, 0, 0);
		FrameSlot.SetPos(m_wKok, x, y);
		FrameSlot.SetSize(m_wKok, w, h);

		Color pillRenk = new Color(0.11, 0.12, 0.15, 0.94);
		m_wPillL = Img(ws, M360_HudIkonlari.PILL_L, pillRenk);
		m_wPillMid = Img(ws, M360_HudIkonlari.PILL_MID, pillRenk);
		m_wPillR = Img(ws, M360_HudIkonlari.PILL_R, pillRenk);
		PillYerles(w, h);

		float ikonKutu = h - 10;
		m_wIkonKutu = Img(ws, M360_HudIkonlari.WELL, new Color(0.07, 0.08, 0.10, 0.98));
		if (m_wIkonKutu)
		{
			FrameSlot.SetPos(m_wIkonKutu, 8, 5);
			FrameSlot.SetSize(m_wIkonKutu, ikonKutu, ikonKutu);
		}

		float ikon = ikonKutu - 12;
		m_wIkon = Img(ws, M360_HudIkonlari.NAKIT, new Color(1, 1, 1, 1));
		if (m_wIkon)
		{
			FrameSlot.SetPos(m_wIkon, 8 + 6, 5 + 6);
			FrameSlot.SetSize(m_wIkon, ikon, ikon);
		}

		float adX = 8 + ikonKutu + 10;
		float miktarW = 120;
		m_wAd = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.93, 0.95, 0.97, 1), 0, m_wKok));
		if (m_wAd)
		{
			m_wAd.SetExactFontSize(15);
			FrameSlot.SetAnchorMin(m_wAd, 0, 0);
			FrameSlot.SetAnchorMax(m_wAd, 0, 0);
			FrameSlot.SetPos(m_wAd, adX, (h - 18) * 0.5);
			FrameSlot.SetSize(m_wAd, w - adX - miktarW - 14, 20);
		}

		m_wMiktar = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, new Color(0.55, 0.95, 0.70, 1), 0, m_wKok));
		if (m_wMiktar)
		{
			m_wMiktar.SetExactFontSize(15);
			FrameSlot.SetAnchorMin(m_wMiktar, 0, 0);
			FrameSlot.SetAnchorMax(m_wMiktar, 0, 0);
			FrameSlot.SetPos(m_wMiktar, w - miktarW - 12, (h - 18) * 0.5);
			FrameSlot.SetSize(m_wMiktar, miktarW, 20);
		}

		Gorunur(false);
		return m_wAd != null;
	}

	//------------------------------------------------------------------------------------------------
	protected ImageWidget Img(notnull WorkspaceWidget ws, ResourceName tex, notnull Color renk)
	{
		ImageWidget img = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wKok));
		if (!img)
			return null;

		FrameSlot.SetAnchorMin(img, 0, 0);
		FrameSlot.SetAnchorMax(img, 0, 0);
		img.SetIsColorInherited(false);
		if (M360_HudIkonlari.Yukle(img, tex))
			img.SetColor(renk);
		else
			img.SetColor(renk);

		return img;
	}

	//------------------------------------------------------------------------------------------------
	protected void PillYerles(float w, float h)
	{
		float cap = h;
		float mid = w - cap * 2.0;
		if (mid < 2)
			mid = 2;

		if (m_wPillL)
		{
			FrameSlot.SetPos(m_wPillL, 0, 0);
			FrameSlot.SetSize(m_wPillL, cap, h);
		}
		if (m_wPillMid)
		{
			FrameSlot.SetPos(m_wPillMid, cap, 0);
			FrameSlot.SetSize(m_wPillMid, mid, h);
		}
		if (m_wPillR)
		{
			FrameSlot.SetPos(m_wPillR, cap + mid, 0);
			FrameSlot.SetSize(m_wPillR, cap, h);
		}
	}

	//------------------------------------------------------------------------------------------------
	void Yaz(string ad, string miktar, ResourceName ikon, notnull Color ikonRenk)
	{
		if (m_wAd)
			m_wAd.SetText(ad);
		if (m_wMiktar)
			m_wMiktar.SetText(miktar);
		if (m_wIkon)
		{
			if (M360_HudIkonlari.Yukle(m_wIkon, ikon))
				m_wIkon.SetColor(ikonRenk);
			else
				m_wIkon.SetVisible(false);
		}
		Gorunur(true);
	}

	//------------------------------------------------------------------------------------------------
	void Yerles(float x, float y, float w, float h)
	{
		if (!m_wKok)
			return;

		m_fW = w;
		m_fH = h;
		FrameSlot.SetPos(m_wKok, x, y);
		FrameSlot.SetSize(m_wKok, w, h);
		PillYerles(w, h);

		float ikonKutu = h - 10;
		if (m_wIkonKutu)
		{
			FrameSlot.SetPos(m_wIkonKutu, 8, 5);
			FrameSlot.SetSize(m_wIkonKutu, ikonKutu, ikonKutu);
		}
		if (m_wIkon)
		{
			float ikon = ikonKutu - 12;
			FrameSlot.SetPos(m_wIkon, 8 + 6, 5 + 6);
			FrameSlot.SetSize(m_wIkon, ikon, ikon);
		}
		if (m_wAd)
		{
			float adX = 8 + ikonKutu + 10;
			FrameSlot.SetPos(m_wAd, adX, (h - 18) * 0.5);
			FrameSlot.SetSize(m_wAd, w - adX - 120 - 14, 20);
		}
		if (m_wMiktar)
		{
			FrameSlot.SetPos(m_wMiktar, w - 120 - 12, (h - 18) * 0.5);
			FrameSlot.SetSize(m_wMiktar, 120, 20);
		}
	}

	//------------------------------------------------------------------------------------------------
	void Gorunur(bool goster)
	{
		if (m_wKok)
			m_wKok.SetVisible(goster);
	}

	//------------------------------------------------------------------------------------------------
	void YokEt()
	{
		if (m_wKok)
			m_wKok.RemoveFromHierarchy();
		m_wKok = null;
		m_wPillL = null;
		m_wPillMid = null;
		m_wPillR = null;
		m_wIkonKutu = null;
		m_wIkon = null;
		m_wAd = null;
		m_wMiktar = null;
	}
}

//------------------------------------------------------------------------------------------------
//! I canta — 9-slice oval panel (kose sabit, soft AA) + dinamik satir havuzu
//------------------------------------------------------------------------------------------------
class M360_CantaPanelWidgetlari
{
	protected static const int MAX_SATIR = 12;
	protected static const float PANEL_W = 440;
	protected static const float PANEL_H = 420;
	protected static const float CORNER = 32;
	protected static const float PAD = 18;
	protected static const float SATIR_H = 52;
	protected static const float SATIR_GAP = 8;
	protected static const float LIST_Y = 118;
	protected static const float BAR_W = 404;
	protected static const float BAR_H = 8;

	Widget m_wKok;
	ImageWidget m_wTL;
	ImageWidget m_wTR;
	ImageWidget m_wBL;
	ImageWidget m_wBR;
	ImageWidget m_wTop;
	ImageWidget m_wBot;
	ImageWidget m_wLeft;
	ImageWidget m_wRight;
	ImageWidget m_wCenter;
	ImageWidget m_wHeaderL;
	ImageWidget m_wHeaderMid;
	ImageWidget m_wHeaderR;
	TextWidget m_wBaslik;
	TextWidget m_wAltBaslik;
	TextWidget m_wAgirlikYazi;
	ImageWidget m_wAgirlikTrack;
	ImageWidget m_wAgirlikDolgu;
	ref array<ref M360_CantaSatirUi> m_aSatirlar;

	//------------------------------------------------------------------------------------------------
	bool Olustur()
	{
		WorkspaceWidget ws = GetGame().GetWorkspace();
		if (!ws)
			return false;

		m_wKok = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 60);
		if (!m_wKok)
			return false;
		m_wKok.SetEnabled(false);

		FrameSlot.SetAnchorMin(m_wKok, 0.5, 0.5);
		FrameSlot.SetAnchorMax(m_wKok, 0.5, 0.5);
		FrameSlot.SetPos(m_wKok, -PANEL_W * 0.5, -PANEL_H * 0.5);
		FrameSlot.SetSize(m_wKok, PANEL_W, PANEL_H);

		Color panelRenk = new Color(0.07, 0.08, 0.11, 0.95);
		DokuzDilPanel(ws, panelRenk);

		Color headRenk = new Color(0.13, 0.15, 0.19, 0.96);
		float headY = PAD;
		float headH = 42;
		float headW = PANEL_W - PAD * 2;
		m_wHeaderL = PillParca(ws, M360_HudIkonlari.PILL_L, headRenk);
		m_wHeaderMid = PillParca(ws, M360_HudIkonlari.PILL_MID, headRenk);
		m_wHeaderR = PillParca(ws, M360_HudIkonlari.PILL_R, headRenk);
		PillSerit(m_wHeaderL, m_wHeaderMid, m_wHeaderR, PAD, headY, headW, headH);

		m_wBaslik = MetinEkle(ws, "m_wBaslik", "OYUNCU CANTASI", 17, PAD + 16, headY + 11, 260, 22, new Color(0.96, 0.97, 0.99, 1));
		m_wAltBaslik = MetinEkle(ws, "m_wAltBaslik", "M360 Life", 12, PANEL_W - PAD - 96, headY + 13, 80, 18, new Color(0.45, 0.80, 0.95, 1));

		m_wAgirlikYazi = MetinEkle(ws, "m_wAgirlikYazi", "Agirlik  0 / 40", 13, PAD, 72, 220, 18, new Color(0.72, 0.76, 0.80, 1));

		m_wAgirlikTrack = ImgKok(ws, M360_HudIkonlari.BAR, new Color(0.16, 0.18, 0.22, 1));
		if (m_wAgirlikTrack)
		{
			FrameSlot.SetPos(m_wAgirlikTrack, PAD, 94);
			FrameSlot.SetSize(m_wAgirlikTrack, BAR_W, BAR_H);
		}

		m_wAgirlikDolgu = ImgKok(ws, M360_HudIkonlari.BAR, new Color(0.30, 0.82, 0.55, 1));
		if (m_wAgirlikDolgu)
		{
			FrameSlot.SetPos(m_wAgirlikDolgu, PAD, 94);
			FrameSlot.SetSize(m_wAgirlikDolgu, 0, BAR_H);
		}

		m_aSatirlar = new array<ref M360_CantaSatirUi>();
		float satirW = PANEL_W - PAD * 2;
		for (int i = 0; i < MAX_SATIR; i++)
		{
			M360_CantaSatirUi satir = new M360_CantaSatirUi();
			float y = LIST_Y + i * (SATIR_H + SATIR_GAP);
			if (!satir.Olustur(ws, m_wKok, PAD, y, satirW, SATIR_H))
				return false;
			m_aSatirlar.Insert(satir);
		}

		GorunurYap(false);
		Print("[M360] Canta paneli v3 soft9", LogLevel.NORMAL);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! Kose sabit (stretch yok) + soft AA — piksellesmeyi keser
	protected void DokuzDilPanel(notnull WorkspaceWidget ws, notnull Color renk)
	{
		float c = CORNER;
		float midW = PANEL_W - c * 2.0;
		float midH = PANEL_H - c * 2.0;

		m_wTL = ImgKok(ws, M360_HudIkonlari.PANEL_CTL, renk);
		m_wTR = ImgKok(ws, M360_HudIkonlari.PANEL_CTR, renk);
		m_wBL = ImgKok(ws, M360_HudIkonlari.PANEL_CBL, renk);
		m_wBR = ImgKok(ws, M360_HudIkonlari.PANEL_CBR, renk);
		m_wTop = ImgKok(ws, M360_HudIkonlari.PANEL_EDGE, renk);
		m_wBot = ImgKok(ws, M360_HudIkonlari.PANEL_EDGE, renk);
		m_wLeft = ImgKok(ws, M360_HudIkonlari.PANEL_EDGE, renk);
		m_wRight = ImgKok(ws, M360_HudIkonlari.PANEL_EDGE, renk);
		m_wCenter = ImgKok(ws, M360_HudIkonlari.PANEL_EDGE, renk);

		Koy(m_wTL, 0, 0, c, c);
		Koy(m_wTR, PANEL_W - c, 0, c, c);
		Koy(m_wBL, 0, PANEL_H - c, c, c);
		Koy(m_wBR, PANEL_W - c, PANEL_H - c, c, c);
		Koy(m_wTop, c, 0, midW, c);
		Koy(m_wBot, c, PANEL_H - c, midW, c);
		Koy(m_wLeft, 0, c, c, midH);
		Koy(m_wRight, PANEL_W - c, c, c, midH);
		Koy(m_wCenter, c, c, midW, midH);
	}

	//------------------------------------------------------------------------------------------------
	protected void Koy(ImageWidget img, float x, float y, float w, float h)
	{
		if (!img)
			return;
		FrameSlot.SetPos(img, x, y);
		FrameSlot.SetSize(img, w, h);
	}

	//------------------------------------------------------------------------------------------------
	protected ImageWidget ImgKok(notnull WorkspaceWidget ws, ResourceName tex, notnull Color renk)
	{
		ImageWidget img = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wKok));
		if (!img)
			return null;

		FrameSlot.SetAnchorMin(img, 0, 0);
		FrameSlot.SetAnchorMax(img, 0, 0);
		img.SetIsColorInherited(false);
		if (M360_HudIkonlari.Yukle(img, tex))
			img.SetColor(renk);
		else
			img.SetColor(renk);

		return img;
	}

	//------------------------------------------------------------------------------------------------
	protected ImageWidget PillParca(notnull WorkspaceWidget ws, ResourceName tex, notnull Color renk)
	{
		return ImgKok(ws, tex, renk);
	}

	//------------------------------------------------------------------------------------------------
	protected void PillSerit(ImageWidget L, ImageWidget mid, ImageWidget R, float x, float y, float w, float h)
	{
		float cap = h;
		float m = w - cap * 2.0;
		if (m < 2)
			m = 2;
		Koy(L, x, y, cap, h);
		Koy(mid, x + cap, y, m, h);
		Koy(R, x + cap + m, y, cap, h);
	}

	//------------------------------------------------------------------------------------------------
	protected TextWidget MetinEkle(notnull WorkspaceWidget ws, string ad, string metin, int font, float x, float y, float ww, float hh, notnull Color renk)
	{
		TextWidget tw = TextWidget.Cast(ws.CreateWidget(WidgetType.TextWidgetTypeID, WidgetFlags.VISIBLE, renk, 0, m_wKok));
		if (!tw)
			return null;

		tw.SetName(ad);
		tw.SetText(metin);
		tw.SetExactFontSize(font);
		FrameSlot.SetAnchorMin(tw, 0, 0);
		FrameSlot.SetAnchorMax(tw, 0, 0);
		FrameSlot.SetPos(tw, x, y);
		FrameSlot.SetSize(tw, ww, hh);
		return tw;
	}

	//------------------------------------------------------------------------------------------------
	void VeriYaz(M360_IsOturumVerisi veri)
	{
		if (!veri || !m_aSatirlar)
			return;

		string isAdi = veri.m_sIsAdi;
		if (isAdi.IsEmpty())
			isAdi = "Pirinc";

		int maxTasima = veri.m_iMaxTasima;
		if (maxTasima < 1)
			maxTasima = 40;

		int agirlik = veri.m_iHam + veri.m_iIslenmis;
		if (m_wAgirlikYazi)
			m_wAgirlikYazi.SetText(string.Format("Agirlik  %1 / %2", agirlik, maxTasima));

		float oran = 0;
		if (maxTasima > 0)
			oran = agirlik / (float)maxTasima;
		if (oran < 0)
			oran = 0;
		if (oran > 1)
			oran = 1;

		if (m_wAgirlikDolgu)
		{
			FrameSlot.SetPos(m_wAgirlikDolgu, PAD, 94);
			FrameSlot.SetSize(m_wAgirlikDolgu, BAR_W * oran, BAR_H);
			if (oran > 0.85)
				m_wAgirlikDolgu.SetColor(new Color(0.95, 0.35, 0.35, 1));
			else if (oran > 0.6)
				m_wAgirlikDolgu.SetColor(new Color(0.95, 0.75, 0.25, 1));
			else
				m_wAgirlikDolgu.SetColor(new Color(0.30, 0.82, 0.55, 1));
		}

		int kullanilan = 0;
		kullanilan = SatirYaz(kullanilan, string.Format("%1 - ham", isAdi), string.Format("x%1", veri.m_iHam), M360_HudIkonlari.ITEM_HAM, new Color(0.95, 0.82, 0.35, 1));
		kullanilan = SatirYaz(kullanilan, string.Format("%1 - islenmis", isAdi), string.Format("x%1", veri.m_iIslenmis), M360_HudIkonlari.ITEM_ISLENMIS, new Color(0.55, 0.85, 1.0, 1));
		kullanilan = SatirYaz(kullanilan, "Nakit", string.Format("$%1", M360_HudYazi.NakitFormat(veri.m_iNakit)), M360_HudIkonlari.NAKIT, new Color(0.35, 0.92, 0.45, 1));

		for (int i = kullanilan; i < m_aSatirlar.Count(); i++)
		{
			M360_CantaSatirUi s = m_aSatirlar.Get(i);
			if (s)
				s.Gorunur(false);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected int SatirYaz(int index, string ad, string miktar, ResourceName ikon, notnull Color renk)
	{
		if (!m_aSatirlar || index < 0 || index >= m_aSatirlar.Count())
			return index;

		M360_CantaSatirUi satir = m_aSatirlar.Get(index);
		if (!satir)
			return index;

		float y = LIST_Y + index * (SATIR_H + SATIR_GAP);
		satir.Yerles(PAD, y, PANEL_W - PAD * 2, SATIR_H);
		satir.Yaz(ad, miktar, ikon, renk);
		return index + 1;
	}

	//------------------------------------------------------------------------------------------------
	void GorunurYap(bool gorunur)
	{
		if (m_wKok)
			m_wKok.SetVisible(gorunur);
	}

	//------------------------------------------------------------------------------------------------
	void YokEt()
	{
		if (m_aSatirlar)
		{
			for (int i = 0; i < m_aSatirlar.Count(); i++)
			{
				M360_CantaSatirUi s = m_aSatirlar.Get(i);
				if (s)
					s.YokEt();
			}
			m_aSatirlar.Clear();
		}
		m_aSatirlar = null;

		if (m_wKok)
			m_wKok.RemoveFromHierarchy();
		m_wKok = null;
		m_wTL = null;
		m_wTR = null;
		m_wBL = null;
		m_wBR = null;
		m_wTop = null;
		m_wBot = null;
		m_wLeft = null;
		m_wRight = null;
		m_wCenter = null;
		m_wHeaderL = null;
		m_wHeaderMid = null;
		m_wHeaderR = null;
		m_wBaslik = null;
		m_wAltBaslik = null;
		m_wAgirlikYazi = null;
		m_wAgirlikTrack = null;
		m_wAgirlikDolgu = null;
	}
}
