//------------------------------------------------------------------------------------------------
//! M360 Life — Isleme makinesi (Dokuman 5.1b Asama 3)
//! Parti isler; 0→100 isleme ilerlemesi. Alandan uzaklasinca iptal (ham korunur).
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Isler", description: "Isleme makinesi")]
class M360_IslemeMakinesiBileseniClass : ScriptComponentClass
{
}

class M360_IslemeMakinesiBileseni : ScriptComponent
{
	[Attribute(desc: "Is ayarlari")]
	ref M360_IsAyar m_Ayar;

	protected IEntity m_IsleyenKullanici;
	protected int m_iBekleyenParti;
	protected int m_iIslemMs;
	protected int m_iIslemGecenMs;
	protected const int ILERLEME_ADIM_MS = 50;

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		if (!m_Ayar)
		{
			Print("[M360] IslemeMakinesi: m_Ayar eksik", LogLevel.ERROR);
			return;
		}

		BaseWorld dunya = owner.GetWorld();
		if (dunya && dunya.IsEditMode())
			return;

		Print(string.Format("[M360] IslemeMakinesi hazir: %1", m_Ayar.m_sIsAdi), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		GetGame().GetCallqueue().Remove(IslemeIlerlemeAdimi);
		super.OnDelete(owner);
	}

	//------------------------------------------------------------------------------------------------
	void IslemeBaslat(IEntity kullanici)
	{
		if (!m_Ayar || !kullanici)
			return;

		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(kullanici);
		if (oturum.m_bIsliyor)
		{
			MesajGoster("Zaten isleniyor...");
			return;
		}

		if (oturum.m_bTopluyor)
		{
			MesajGoster("Once toplamayi durdur");
			return;
		}

		if (oturum.m_iHam <= 0)
		{
			MesajGoster("Ham madde yok — once topla");
			return;
		}

		m_iBekleyenParti = Math.Min(oturum.m_iHam, m_Ayar.m_iPartiBoyutu);
		oturum.m_bIsliyor = true;
		oturum.m_sIsAdi = m_Ayar.m_sIsAdi;
		oturum.m_fIslemeIlerleme = 0;
		m_IsleyenKullanici = kullanici;
		m_iIslemGecenMs = 0;

		m_iIslemMs = m_Ayar.m_iIslemeSuresi * 1000;
		if (m_iIslemMs < 100)
			m_iIslemMs = 100;

		MesajGoster(string.Format("Isleniyor: %1 birim (%2 sn)", m_iBekleyenParti, m_Ayar.m_iIslemeSuresi));
		GetGame().GetCallqueue().CallLater(IslemeIlerlemeAdimi, ILERLEME_ADIM_MS, true);
	}

	//------------------------------------------------------------------------------------------------
	protected void IslemeIlerlemeAdimi()
	{
		if (!m_Ayar || !m_IsleyenKullanici)
		{
			GetGame().GetCallqueue().Remove(IslemeIlerlemeAdimi);
			return;
		}

		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(m_IsleyenKullanici);
		if (!oturum.m_bIsliyor)
		{
			GetGame().GetCallqueue().Remove(IslemeIlerlemeAdimi);
			return;
		}

		if (MesafeAsildiMi(m_IsleyenKullanici))
		{
			IslemeIptal();
			return;
		}

		m_iIslemGecenMs += ILERLEME_ADIM_MS;
		float yuzde = (m_iIslemGecenMs * 100.0) / m_iIslemMs;
		if (yuzde > 100)
			yuzde = 100;
		oturum.m_fIslemeIlerleme = yuzde;

		if (m_iIslemGecenMs < m_iIslemMs)
			return;

		GetGame().GetCallqueue().Remove(IslemeIlerlemeAdimi);
		IslemeBitir();
	}

	//------------------------------------------------------------------------------------------------
	protected void IslemeBitir()
	{
		if (!m_Ayar || !m_IsleyenKullanici)
			return;

		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(m_IsleyenKullanici);
		oturum.m_bIsliyor = false;
		oturum.m_fIslemeIlerleme = 0;

		int uretilen = Math.Round(m_iBekleyenParti * m_Ayar.m_fDonusumOrani);
		oturum.m_iHam -= m_iBekleyenParti;
		if (oturum.m_iHam < 0)
			oturum.m_iHam = 0;
		oturum.m_iIslenmis += uretilen;

		MesajGoster(string.Format("Islem bitti: +%1 islenmis (ham=%2, islenmis=%3)", uretilen, oturum.m_iHam, oturum.m_iIslenmis));
		m_iBekleyenParti = 0;
		m_IsleyenKullanici = null;
		m_iIslemGecenMs = 0;
	}

	//------------------------------------------------------------------------------------------------
	//! Alan terk — cikti yok, ham aynen kalir (Dokuman 5.1b)
	protected void IslemeIptal()
	{
		GetGame().GetCallqueue().Remove(IslemeIlerlemeAdimi);
		if (!m_IsleyenKullanici)
			return;

		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(m_IsleyenKullanici);
		oturum.m_bIsliyor = false;
		oturum.m_fIslemeIlerleme = 0;
		MesajGoster("Tezgahdan uzaklastim — isleme iptal (ham korundu)");
		m_iBekleyenParti = 0;
		m_IsleyenKullanici = null;
		m_iIslemGecenMs = 0;
	}

	//------------------------------------------------------------------------------------------------
	protected bool MesafeAsildiMi(IEntity kullanici)
	{
		if (!kullanici || !m_Ayar)
			return true;

		IEntity site = GetOwner();
		if (!site)
			return true;

		float limit = m_Ayar.m_fIptalMesafesi;
		if (limit < 0.5)
			limit = 4.0;

		float mesafe = vector.Distance(site.GetOrigin(), kullanici.GetOrigin());
		return mesafe > limit;
	}

	//------------------------------------------------------------------------------------------------
	bool IslemeYapilabilirMi(IEntity kullanici)
	{
		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(kullanici);
		return oturum.m_iHam > 0 && !oturum.m_bIsliyor && !oturum.m_bTopluyor;
	}

	//------------------------------------------------------------------------------------------------
	protected void MesajGoster(string mesaj)
	{
		SCR_HintManagerComponent.ShowCustomHint(mesaj, "M360 Isleme", 4);
	}
}
