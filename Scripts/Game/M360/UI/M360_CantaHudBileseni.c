//------------------------------------------------------------------------------------------------
//! M360 Life — Envanter popup + toplama/isleme ilerleme cubugu
//! Acilis: Debug.KeyState(KeyCode.KC_I) — BI Envanter/Tab aksiyonuna bagli DEGIL
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Arayuz", description: "M360 envanter popup (I)")]
class M360_CantaHudBileseniClass : ScriptComponentClass
{
}

class M360_CantaHudBileseni : ScriptComponent
{
	protected static M360_CantaHudBileseni s_Ornek;

	protected bool m_bBaslatildi;
	protected bool m_bAcik;
	protected bool m_bIpucuGosterildi;

	protected ref array<Widget> m_aEnvanterWidgetlari;
	protected ref array<Widget> m_aIlerlemeWidgetlari;

	protected TextWidget m_wBaslikSol;
	protected TextWidget m_wBaslikSag;
	protected TextWidget m_wListeBasligi;
	protected TextWidget m_wSatir1;
	protected TextWidget m_wSatir2;
	protected TextWidget m_wSatir3;
	protected TextWidget m_wSatir4;
	protected TextWidget m_wBos;
	protected TextWidget m_wKapat;

	protected Widget m_wIlerlemeArka;
	protected Widget m_wIlerlemeDolu;
	protected TextWidget m_wIlerlemeYazi;
	protected int m_iSonIlerlemeYuzde;
	protected int m_iSonIpucuYuzde;

	//------------------------------------------------------------------------------------------------
	static M360_CantaHudBileseni Al()
	{
		return s_Ornek;
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		s_Ornek = this;
		SetEventMask(owner, EntityEvent.FRAME);
		Print("[M360] CantaHud OnPostInit — kare maske kuruldu", LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		if (s_Ornek == this)
			s_Ornek = null;

		GetGame().GetCallqueue().Remove(GirisIpucuGoster);
		EnvanterKapat();
		IlerlemeyiYokEt();
		super.OnDelete(owner);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (!GetGame().InPlayMode())
			return;

		if (!m_bBaslatildi)
			Baslat();

		ITusunuKontrolEt();

		IEntity oyuncu = YerelOyuncuyuAl();
		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(oyuncu);
		IlerlemeCubugunuGuncelle(veri);
		if (m_bAcik)
			EnvanterYenile(veri);
	}

	//------------------------------------------------------------------------------------------------
	protected void Baslat()
	{
		m_bBaslatildi = true;
		m_bAcik = false;
		m_bIpucuGosterildi = false;
		m_iSonIlerlemeYuzde = -1;
		m_iSonIpucuYuzde = -1;

		GetGame().GetCallqueue().CallLater(GirisIpucuGoster, 2000, false);
		Print("[M360] CantaHud baslat — I = Debug.KeyState(KC_I), Tab = BI envanter", LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! Ham klavye — girdi yoneticisi / conf / remap bagimsiz (BI SCR_DebugEditorComponent deseni)
	protected void ITusunuKontrolEt()
	{
		if (!Debug.KeyState(KeyCode.KC_I))
			return;

		Debug.ClearKey(KeyCode.KC_I);
		Print("[M360] KC_I basildi — EnvanterAcKapa", LogLevel.NORMAL);
		EnvanterAcKapa();
	}

	//------------------------------------------------------------------------------------------------
	protected void GirisIpucuGoster()
	{
		if (m_bIpucuGosterildi)
			return;
		m_bIpucuGosterildi = true;
		SCR_HintManagerComponent.ShowCustomHint(
			"I = M360 Envanter  |  Toplarken yuzde sayaci",
			"M360",
			8);
	}

	//------------------------------------------------------------------------------------------------
	void EnvanterAcKapa()
	{
		Print(string.Format("[M360] Envanter toggle (open=%1)", m_bAcik), LogLevel.NORMAL);
		if (m_bAcik)
			EnvanterKapat();
		else
			EnvanterAc();
	}

	//------------------------------------------------------------------------------------------------
	protected WidgetFlags ArayuzBayraklari()
	{
		return WidgetFlags.VISIBLE;
	}

	//------------------------------------------------------------------------------------------------
	protected Widget DikdortgenOlustur(WorkspaceWidget ws, int x, int y, int w, int h, Color color, int sort)
	{
		Widget rect = ws.CreateWidgetInWorkspace(WidgetType.ImageWidgetTypeID, x, y, w, h, ArayuzBayraklari(), color, sort);
		if (!rect)
			rect = ws.CreateWidgetInWorkspace(WidgetType.FrameWidgetTypeID, x, y, w, h, ArayuzBayraklari(), color, sort);
		if (!rect)
			return null;

		rect.SetColor(color);
		rect.SetOpacity(color.A());
		rect.SetVisible(true);
		rect.SetZOrder(sort);
		if (m_aEnvanterWidgetlari)
			m_aEnvanterWidgetlari.Insert(rect);
		return rect;
	}

	//------------------------------------------------------------------------------------------------
	//! I basinca kesin gorunen Life listesi (ipucu). Widget panel de denenir.
	protected void EnvanterIpucuGoster(M360_IsOturumVerisi veri)
	{
		if (!veri)
			veri = M360_IsOturumlari.AlVeyaOlustur(YerelOyuncuyuAl());
		if (!veri)
			return;

		string isAdi = veri.m_sIsAdi;
		if (isAdi.IsEmpty())
			isAdi = "Pirinc";

		int maxTasima = veri.m_iMaxTasima;
		if (maxTasima < 1)
			maxTasima = 40;

		int agirlik = veri.m_iHam + veri.m_iIslenmis;
		string govde = string.Format(
			"Agirlik: %1/%2\n[%3] - %4 (ham)\n[%5] - %4 (islenmis)\n[%6] - Nakit\n---\nTekrar I = kapat",
			agirlik,
			maxTasima,
			veri.m_iHam,
			isAdi,
			veri.m_iIslenmis,
			veri.m_iNakit);

		SCR_HintManagerComponent.ShowCustomHint(govde, "M360 — Oyuncu Cantasi", 12);
	}

	//------------------------------------------------------------------------------------------------
	protected void EnvanterAc()
	{
		if (m_bAcik)
			return;

		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(YerelOyuncuyuAl());
		EnvanterIpucuGoster(veri);

		WorkspaceWidget ws = GetGame().GetWorkspace();
		if (!ws)
		{
			Print("[M360] Envanter: Workspace yok — ipucu gosterildi", LogLevel.WARNING);
			m_bAcik = true;
			return;
		}

		float sw = ws.GetWidth();
		float sh = ws.GetHeight();
		if (sw < 1) sw = 1920;
		if (sh < 1) sh = 1080;

		int panelW = (int)ws.DPIScale(520);
		int panelH = (int)ws.DPIScale(420);
		int left = (int)((sw - panelW) * 0.5);
		int top = (int)((sh - panelH) * 0.5);

		Color dim = new Color(0, 0, 0, 0.55);
		Color panelBg = new Color(0.08, 0.08, 0.08, 0.95);
		Color headerRed = new Color(0.72, 0.08, 0.08, 0.98);
		Color white = new Color(1, 1, 1, 1);
		Color rowBg = new Color(0.12, 0.12, 0.12, 0.9);

		m_aEnvanterWidgetlari = new array<Widget>();

		DikdortgenOlustur(ws, 0, 0, (int)sw, (int)sh, dim, 40);
		if (!DikdortgenOlustur(ws, left, top, panelW, panelH, panelBg, 45))
		{
			Print("[M360] Envanter: panel widget yok — ipucu yeterli", LogLevel.WARNING);
			m_bAcik = true;
			return;
		}

		DikdortgenOlustur(ws, left, top, panelW, (int)ws.DPIScale(40), headerRed, 46);
		m_wBaslikSol = MetinOlustur(ws, left + (int)ws.DPIScale(14), top + (int)ws.DPIScale(8), panelW - (int)ws.DPIScale(180), (int)ws.DPIScale(26), white, 18, true);
		m_wBaslikSag = MetinOlustur(ws, left + panelW - (int)ws.DPIScale(170), top + (int)ws.DPIScale(10), (int)ws.DPIScale(160), (int)ws.DPIScale(24), white, 15, false);

		DikdortgenOlustur(ws, left + (int)ws.DPIScale(12), top + (int)ws.DPIScale(52), panelW - (int)ws.DPIScale(24), (int)ws.DPIScale(32), headerRed, 46);
		m_wListeBasligi = MetinOlustur(ws, left + (int)ws.DPIScale(24), top + (int)ws.DPIScale(56), (int)ws.DPIScale(280), (int)ws.DPIScale(24), white, 16, true);
		if (m_wListeBasligi)
			m_wListeBasligi.SetText("Oyuncu Envanteri");

		int rowY = top + (int)ws.DPIScale(96);
		int rowH = (int)ws.DPIScale(36);
		int rowGap = (int)ws.DPIScale(4);
		for (int r = 0; r < 5; r++)
			DikdortgenOlustur(ws, left + (int)ws.DPIScale(12), rowY + r * (rowH + rowGap), panelW - (int)ws.DPIScale(24), rowH, rowBg, 46);

		m_wSatir1 = MetinOlustur(ws, left + (int)ws.DPIScale(28), rowY + (int)ws.DPIScale(6), panelW - (int)ws.DPIScale(56), (int)ws.DPIScale(28), white, 17, false);
		m_wSatir2 = MetinOlustur(ws, left + (int)ws.DPIScale(28), rowY + (rowH + rowGap) + (int)ws.DPIScale(6), panelW - (int)ws.DPIScale(56), (int)ws.DPIScale(28), white, 17, false);
		m_wSatir3 = MetinOlustur(ws, left + (int)ws.DPIScale(28), rowY + 2 * (rowH + rowGap) + (int)ws.DPIScale(6), panelW - (int)ws.DPIScale(56), (int)ws.DPIScale(28), white, 17, false);
		m_wSatir4 = MetinOlustur(ws, left + (int)ws.DPIScale(28), rowY + 3 * (rowH + rowGap) + (int)ws.DPIScale(6), panelW - (int)ws.DPIScale(56), (int)ws.DPIScale(28), white, 17, false);
		m_wBos = MetinOlustur(ws, left + (int)ws.DPIScale(28), rowY + 4 * (rowH + rowGap) + (int)ws.DPIScale(6), panelW - (int)ws.DPIScale(56), (int)ws.DPIScale(28), new Color(0.7, 0.7, 0.7, 1), 15, false);

		DikdortgenOlustur(ws, left + (int)ws.DPIScale(12), top + panelH - (int)ws.DPIScale(52), (int)ws.DPIScale(120), (int)ws.DPIScale(36), new Color(0.05, 0.05, 0.05, 0.95), 46);
		m_wKapat = MetinOlustur(ws, left + (int)ws.DPIScale(28), top + panelH - (int)ws.DPIScale(44), (int)ws.DPIScale(100), (int)ws.DPIScale(24), white, 16, true);
		if (m_wKapat)
			m_wKapat.SetText("KAPAT (I)");

		if (m_wBaslikSol)
			m_wBaslikSol.SetText("M360 — Oyuncu Cantasi");

		m_bAcik = true;
		EnvanterYenile(veri);
		Print("[M360] Envanter ACILDI (ipucu + panel denemesi)", LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected TextWidget MetinOlustur(WorkspaceWidget ws, int x, int y, int w, int h, Color color, int fontSize, bool bold)
	{
		Widget wgt = ws.CreateWidgetInWorkspace(WidgetType.TextWidgetTypeID, x, y, w, h, ArayuzBayraklari(), color, 55);
		TextWidget text = TextWidget.Cast(wgt);
		if (!text)
			return null;
		text.SetExactFontSize(fontSize);
		text.SetBold(bold);
		text.SetColor(color);
		text.SetOpacity(1);
		text.SetVisible(true);
		text.SetZOrder(55);
		if (m_aEnvanterWidgetlari)
			m_aEnvanterWidgetlari.Insert(text);
		return text;
	}

	//------------------------------------------------------------------------------------------------
	protected void EnvanterYenile(M360_IsOturumVerisi veri)
	{
		if (!veri)
			return;

		string isAdi = veri.m_sIsAdi;
		if (isAdi.IsEmpty())
			isAdi = "Pirinc";

		int maxTasima = veri.m_iMaxTasima;
		if (maxTasima < 1)
			maxTasima = 40;

		int agirlik = veri.m_iHam + veri.m_iIslenmis;
		if (m_wBaslikSag)
			m_wBaslikSag.SetText(string.Format("Agirlik: %1/%2", agirlik, maxTasima));

		if (m_wSatir1)
			m_wSatir1.SetText(string.Format("[%1] - %2 (ham)", veri.m_iHam, isAdi));
		if (m_wSatir2)
			m_wSatir2.SetText(string.Format("[%1] - %2 (islenmis)", veri.m_iIslenmis, isAdi));
		if (m_wSatir3)
			m_wSatir3.SetText(string.Format("[%1] - Nakit", veri.m_iNakit));

		string durumSatiri = "Hazir";
		if (veri.m_bTopluyor)
			durumSatiri = string.Format("Toplaniyor... %1%%", (int)veri.m_fToplamaIlerleme);
		else if (veri.m_bIsliyor)
			durumSatiri = string.Format("Isleniyor... %1%%", (int)veri.m_fIslemeIlerleme);

		if (m_wSatir4)
			m_wSatir4.SetText("[*] - " + durumSatiri);
		if (m_wBos)
			m_wBos.SetText("(ileride alet / lisans satirlari)");
	}

	//------------------------------------------------------------------------------------------------
	protected void EnvanterKapat()
	{
		m_bAcik = false;
		WidgetListesiniYokEt(m_aEnvanterWidgetlari);
		m_aEnvanterWidgetlari = null;
		m_wBaslikSol = null;
		m_wBaslikSag = null;
		m_wListeBasligi = null;
		m_wSatir1 = null;
		m_wSatir2 = null;
		m_wSatir3 = null;
		m_wSatir4 = null;
		m_wBos = null;
		m_wKapat = null;
	}

	//------------------------------------------------------------------------------------------------
	protected void IlerlemeCubugunuGuncelle(M360_IsOturumVerisi veri)
	{
		bool goster = veri && (veri.m_bTopluyor || veri.m_bIsliyor);
		if (!goster)
		{
			IlerlemeyiYokEt();
			m_iSonIpucuYuzde = -1;
			return;
		}

		float yuzde = veri.m_fToplamaIlerleme;
		string etiket = "Toplama";
		if (veri.m_bIsliyor)
		{
			yuzde = veri.m_fIslemeIlerleme;
			etiket = "Isleme";
		}

		int yuzdeTam = (int)yuzde;

		if (yuzdeTam != m_iSonIpucuYuzde && (yuzdeTam == 0 || yuzdeTam % 25 == 0 || yuzdeTam >= 99))
		{
			m_iSonIpucuYuzde = yuzdeTam;
			SCR_HintManagerComponent.ShowCustomHint(
				string.Format("%1: %2%%", etiket, yuzdeTam),
				"M360",
				1.2);
		}

		WorkspaceWidget ws = GetGame().GetWorkspace();
		if (!ws)
			return;

		float sw = ws.GetWidth();
		float sh = ws.GetHeight();
		if (sw < 1) sw = 1920;
		if (sh < 1) sh = 1080;

		int barW = (int)ws.DPIScale(420);
		int barH = (int)ws.DPIScale(22);
		int left = (int)((sw - barW) * 0.5);
		int top = (int)(sh - ws.DPIScale(110));
		WidgetFlags flags = ArayuzBayraklari();

		if (!m_wIlerlemeArka)
		{
			m_aIlerlemeWidgetlari = new array<Widget>();
			Color bg = new Color(0.05, 0.05, 0.05, 0.92);
			Color fillCol = new Color(0.75, 0.12, 0.12, 0.98);
			Color white = new Color(1, 1, 1, 1);

			m_wIlerlemeArka = ws.CreateWidgetInWorkspace(WidgetType.ImageWidgetTypeID, left, top, barW, barH, flags, bg, 70);
			if (!m_wIlerlemeArka)
				m_wIlerlemeArka = ws.CreateWidgetInWorkspace(WidgetType.FrameWidgetTypeID, left, top, barW, barH, flags, bg, 70);
			if (m_wIlerlemeArka)
			{
				m_wIlerlemeArka.SetColor(bg);
				m_wIlerlemeArka.SetVisible(true);
				m_aIlerlemeWidgetlari.Insert(m_wIlerlemeArka);
			}

			m_wIlerlemeDolu = ws.CreateWidgetInWorkspace(WidgetType.ImageWidgetTypeID, left + 2, top + 2, 1, barH - 4, flags, fillCol, 71);
			if (!m_wIlerlemeDolu)
				m_wIlerlemeDolu = ws.CreateWidgetInWorkspace(WidgetType.FrameWidgetTypeID, left + 2, top + 2, 1, barH - 4, flags, fillCol, 71);
			if (m_wIlerlemeDolu)
			{
				m_wIlerlemeDolu.SetColor(fillCol);
				m_aIlerlemeWidgetlari.Insert(m_wIlerlemeDolu);
			}

			m_wIlerlemeYazi = TextWidget.Cast(ws.CreateWidgetInWorkspace(WidgetType.TextWidgetTypeID, left, top - (int)ws.DPIScale(28), barW, (int)ws.DPIScale(24), flags, white, 72));
			if (m_wIlerlemeYazi)
			{
				m_wIlerlemeYazi.SetExactFontSize(16);
				m_wIlerlemeYazi.SetBold(true);
				m_wIlerlemeYazi.SetColor(white);
				m_aIlerlemeWidgetlari.Insert(m_wIlerlemeYazi);
			}
		}

		int doluGenislik = (int)((barW - 4) * (yuzde / 100.0));
		if (doluGenislik < 1)
			doluGenislik = 1;

		if (m_wIlerlemeDolu && m_iSonIlerlemeYuzde == yuzdeTam)
		{
			if (m_wIlerlemeYazi)
				m_wIlerlemeYazi.SetText(string.Format("%1  %2%%", etiket, yuzdeTam));
			return;
		}
		m_iSonIlerlemeYuzde = yuzdeTam;

		if (m_wIlerlemeDolu)
		{
			m_wIlerlemeDolu.RemoveFromHierarchy();
			if (m_aIlerlemeWidgetlari)
				m_aIlerlemeWidgetlari.RemoveItem(m_wIlerlemeDolu);
			m_wIlerlemeDolu = null;
		}

		Color fillCol2 = new Color(0.75, 0.12, 0.12, 0.98);
		m_wIlerlemeDolu = ws.CreateWidgetInWorkspace(WidgetType.ImageWidgetTypeID, left + 2, top + 2, doluGenislik, barH - 4, flags, fillCol2, 71);
		if (!m_wIlerlemeDolu)
			m_wIlerlemeDolu = ws.CreateWidgetInWorkspace(WidgetType.FrameWidgetTypeID, left + 2, top + 2, doluGenislik, barH - 4, flags, fillCol2, 71);
		if (m_wIlerlemeDolu && m_aIlerlemeWidgetlari)
		{
			m_wIlerlemeDolu.SetColor(fillCol2);
			m_aIlerlemeWidgetlari.Insert(m_wIlerlemeDolu);
		}

		if (m_wIlerlemeYazi)
			m_wIlerlemeYazi.SetText(string.Format("%1  %2%%", etiket, yuzdeTam));
	}

	//------------------------------------------------------------------------------------------------
	protected void IlerlemeyiYokEt()
	{
		WidgetListesiniYokEt(m_aIlerlemeWidgetlari);
		m_aIlerlemeWidgetlari = null;
		m_wIlerlemeArka = null;
		m_wIlerlemeDolu = null;
		m_wIlerlemeYazi = null;
		m_iSonIlerlemeYuzde = -1;
	}

	//------------------------------------------------------------------------------------------------
	protected void WidgetListesiniYokEt(array<Widget> liste)
	{
		if (!liste)
			return;
		for (int i = liste.Count() - 1; i >= 0; i--)
		{
			Widget w = liste[i];
			if (w)
				w.RemoveFromHierarchy();
		}
		liste.Clear();
	}

	//------------------------------------------------------------------------------------------------
	protected IEntity YerelOyuncuyuAl()
	{
		PlayerController pc = GetGame().GetPlayerController();
		if (!pc)
			return null;
		return pc.GetControlledEntity();
	}
}
