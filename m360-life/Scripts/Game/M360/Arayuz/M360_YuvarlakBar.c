//------------------------------------------------------------------------------------------------
//! Yuvarlak durum halkasi — tek badge texture (disk+ikon+halka) + ustunde maskeli progress ring
//------------------------------------------------------------------------------------------------
class M360_YuvarlakBar
{
	Widget m_wKok;
	ImageWidget m_wBadge;
	ImageWidget m_wRing;
	bool m_bMaskVar;

	//------------------------------------------------------------------------------------------------
	bool Olustur(notnull WorkspaceWidget ws, notnull Widget ebeveyn, string ad, float x, float y, float boyut, notnull Color halkaRenk, ResourceName badge)
	{
		m_bMaskVar = false;

		m_wKok = ws.CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 0, ebeveyn);
		if (!m_wKok)
			return false;

		m_wKok.SetName(ad);
		FrameSlot.SetAnchorMin(m_wKok, 0, 1);
		FrameSlot.SetAnchorMax(m_wKok, 0, 1);
		FrameSlot.SetPos(m_wKok, x, y);
		FrameSlot.SetSize(m_wKok, boyut, boyut);

		// Tek parca rozet (kare widget ama texture alpha ile yuvarlak)
		m_wBadge = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, new Color(1, 1, 1, 1), 0, m_wKok));
		if (!m_wBadge)
			return false;

		FrameSlot.SetAnchorMin(m_wBadge, 0, 0);
		FrameSlot.SetAnchorMax(m_wBadge, 1, 1);
		FrameSlot.SetOffsets(m_wBadge, 0, 0, 0, 0);
		m_wBadge.SetIsColorInherited(false);
		if (!M360_HudIkonlari.Yukle(m_wBadge, badge))
		{
			m_wBadge.SetVisible(false);
			Print(string.Format("[M360] Badge yuklenemedi: %1", ad), LogLevel.ERROR);
		}
		else
		{
			m_wBadge.SetColor(new Color(1, 1, 1, 1));
		}

		// Progress halkasi (renkli) — badge uzerinde
		m_wRing = ImageWidget.Cast(ws.CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.BLEND, halkaRenk, 0, m_wKok));
		if (m_wRing)
		{
			FrameSlot.SetAnchorMin(m_wRing, 0, 0);
			FrameSlot.SetAnchorMax(m_wRing, 1, 1);
			FrameSlot.SetOffsets(m_wRing, 0, 0, 0, 0);
			m_wRing.SetIsColorInherited(false);
			if (M360_HudIkonlari.Yukle(m_wRing, M360_HudIkonlari.RADIAL_RING))
			{
				m_wRing.SetColor(halkaRenk);
				m_bMaskVar = M360_HudIkonlari.MaskYukle(m_wRing, M360_HudIkonlari.RADIAL_MASK);
				if (m_bMaskVar)
				{
					m_wRing.SetMaskMode(ImageMaskMode.REGULAR);
					m_wRing.SetMaskProgress(1.0);
				}
			}
			else
			{
				m_wRing.SetVisible(false);
			}
		}

		Set(1.0);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	void Set(float deger)
	{
		if (deger < 0)
			deger = 0;
		if (deger > 1)
			deger = 1;

		if (m_wRing && m_bMaskVar)
			m_wRing.SetMaskProgress(deger);
		else if (m_wRing)
			m_wRing.SetOpacity(0.15 + 0.85 * deger);
	}

	//------------------------------------------------------------------------------------------------
	void YokEt()
	{
		if (m_wKok)
			m_wKok.RemoveFromHierarchy();
		m_wKok = null;
		m_wBadge = null;
		m_wRing = null;
	}
}
