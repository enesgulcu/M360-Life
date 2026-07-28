//------------------------------------------------------------------------------------------------
//! M360 Life — nakit + vitals + saat + is bari + I canta (CreateWidget)
//! CreateWidgets(.layout) Play'de donuyor — kullanilmiyor (docs/15 §7c).
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Arayuz", description: "M360 Life HUD + envanter (I)")]
class M360_CantaHudBileseniClass : ScriptComponentClass
{
}

class M360_CantaHudBileseni : ScriptComponent
{
	protected static M360_CantaHudBileseni s_Ornek;

	protected bool m_bBaslatildi;
	protected bool m_bAcik;
	protected bool m_bIpucuGosterildi;
	protected bool m_bWidgetHazir;
	protected int m_iSonNakit = -1;
	protected int m_iSonIpucuYuzde = -1;
	protected int m_iSonSaatDakika = -1;

	protected ref M360_CekirdekHudWidgetlari m_Cekirdek;
	protected ref M360_CantaPanelWidgetlari m_Canta;

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
		Print("[M360] CantaHud OnPostInit — Life HUD v2", LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		if (s_Ornek == this)
			s_Ornek = null;

		GetGame().GetCallqueue().Remove(GirisIpucuGoster);
		HudYokEt();
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
		M360_IsOturumlari.VitalTick(veri, timeSlice);
		HudGuncelle(veri, oyuncu);
	}

	//------------------------------------------------------------------------------------------------
	protected void Baslat()
	{
		m_bBaslatildi = true;
		m_bAcik = false;
		m_bIpucuGosterildi = false;
		m_iSonNakit = -1;
		m_iSonIpucuYuzde = -1;
		m_iSonSaatDakika = -1;
		m_bWidgetHazir = HudOlustur();
		GetGame().GetCallqueue().CallLater(GirisIpucuGoster, 2000, false);

		if (m_bWidgetHazir)
			Print("[M360] CantaHud baslat — Life HUD OK (I=PlayerController)", LogLevel.NORMAL);
		else
			Print("[M360] CantaHud baslat — widget FAIL, hint yedek", LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	//! I/Tab: M360_PlayerControllerI (ActionOpenInventory). F>Envanter dogrudan EnvanterAcKapa.
	//! Burada Debug.KeyState sadece Workbench Play yedegi.
	protected void ITusunuKontrolEt()
	{
		if (Debug.KeyState(KeyCode.KC_I))
		{
			Debug.ClearKey(KeyCode.KC_I);
			EnvanterAcKapa();
		}
	}

	//------------------------------------------------------------------------------------------------
	protected bool HudOlustur()
	{
		m_Cekirdek = new M360_CekirdekHudWidgetlari();
		m_Canta = new M360_CantaPanelWidgetlari();

		bool cekirdekOk = m_Cekirdek.Olustur();
		bool cantaOk = m_Canta.Olustur();
		if (!cekirdekOk || !cantaOk)
		{
			HudYokEt();
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void HudYokEt()
	{
		if (m_Cekirdek)
			m_Cekirdek.YokEt();
		if (m_Canta)
			m_Canta.YokEt();
		m_Cekirdek = null;
		m_Canta = null;
		m_bWidgetHazir = false;
	}

	//------------------------------------------------------------------------------------------------
	protected void GirisIpucuGoster()
	{
		if (m_bIpucuGosterildi)
			return;
		m_bIpucuGosterildi = true;

		if (m_bWidgetHazir)
			SCR_HintManagerComponent.ShowCustomHint(
				"Nakit | Saat | Yemek/Su/Can | I veya Tab = canta",
				"M360",
				8);
		else
			SCR_HintManagerComponent.ShowCustomHint(
				"I/Tab = M360 Envanter (hint)  |  Widget olusmadi",
				"M360",
				8);
	}

	//------------------------------------------------------------------------------------------------
	void EnvanterAcKapa()
	{
		if (m_bAcik)
		{
			m_bAcik = false;
			if (m_bWidgetHazir && m_Canta)
				m_Canta.GorunurYap(false);
			return;
		}

		m_bAcik = true;
		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(YerelOyuncuyuAl());

		if (m_bWidgetHazir && m_Canta)
		{
			m_Canta.VeriYaz(veri);
			m_Canta.GorunurYap(true);
			return;
		}

		EnvanterIpucuGoster(veri);
	}

	//------------------------------------------------------------------------------------------------
	protected void HudGuncelle(M360_IsOturumVerisi veri, IEntity oyuncu)
	{
		if (!veri)
			return;

		if (m_bWidgetHazir && m_Cekirdek)
		{
			if (veri.m_iNakit != m_iSonNakit)
			{
				m_iSonNakit = veri.m_iNakit;
				m_Cekirdek.NakitYaz(veri.m_iNakit);
			}

			SaatGuncelle();
			VitalGuncelle(veri, oyuncu);

			bool goster = veri.m_bTopluyor || veri.m_bIsliyor;
			if (goster)
			{
				float yuzde = veri.m_fToplamaIlerleme;
				string etiket = "Toplama";
				if (veri.m_bIsliyor)
				{
					yuzde = veri.m_fIslemeIlerleme;
					etiket = "Isleme";
				}
				m_Cekirdek.IlerlemeYaz(true, etiket, yuzde);
			}
			else
			{
				m_Cekirdek.IlerlemeYaz(false, "", 0);
			}

			if (m_bAcik && m_Canta)
				m_Canta.VeriYaz(veri);

			return;
		}

		IlerlemeIpucuGuncelle(veri);
	}

	//------------------------------------------------------------------------------------------------
	protected void SaatGuncelle()
	{
		int year, month, day, hour, minute, second;
		System.GetYearMonthDay(year, month, day);
		System.GetHourMinuteSecond(hour, minute, second);

		int anahtar = hour * 60 + minute;
		if (anahtar == m_iSonSaatDakika)
			return;
		m_iSonSaatDakika = anahtar;

		m_Cekirdek.SaatYaz(
			string.Format("%1:%2", Pad2(hour), Pad2(minute)),
			string.Format("%1.%2.%3", Pad2(day), Pad2(month), year));
	}

	//------------------------------------------------------------------------------------------------
	protected void VitalGuncelle(M360_IsOturumVerisi veri, IEntity oyuncu)
	{
		float yemek = veri.m_fAclik / 100.0;
		float su = veri.m_fSusuzluk / 100.0;
		float can = CanAl(oyuncu);
		m_Cekirdek.VitalYaz(yemek, su, can);
	}

	//------------------------------------------------------------------------------------------------
	protected float CanAl(IEntity oyuncu)
	{
		if (!oyuncu)
			return 1.0;

		DamageManagerComponent dmg = DamageManagerComponent.Cast(oyuncu.FindComponent(DamageManagerComponent));
		if (!dmg)
			return 1.0;

		float scaled = dmg.GetHealthScaled();
		if (scaled < 0)
			scaled = 0;
		if (scaled > 1)
			scaled = 1;
		return scaled;
	}

	//------------------------------------------------------------------------------------------------
	protected string Pad2(int deger)
	{
		if (deger < 10)
			return string.Format("0%1", deger);
		return string.Format("%1", deger);
	}

	//------------------------------------------------------------------------------------------------
	protected void EnvanterIpucuGoster(M360_IsOturumVerisi veri)
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
	protected void IlerlemeIpucuGuncelle(M360_IsOturumVerisi veri)
	{
		bool goster = veri && (veri.m_bTopluyor || veri.m_bIsliyor);
		if (!goster)
		{
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
