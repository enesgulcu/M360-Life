//------------------------------------------------------------------------------------------------
//! M360 Life — Toplama alani (Dokuman 5.1b Asama 1)
//! Her tick 0→100 ilerleme; dolunca ham eklenir.
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Jobs", description: "Toplama alani")]
class M360_JobCollectSiteComponentClass : ScriptComponentClass
{
}

class M360_JobCollectSiteComponent : ScriptComponent
{
	[Attribute(desc: "Is ayarlari")]
	ref M360_JobConfig m_Ayar;

	protected IEntity m_ToplayanKullanici;
	protected int m_iTamamlananTick;
	protected int m_iTickMs;
	protected int m_iIlerlemeGecenMs;
	protected const int ILERLEME_ADIM_MS = 50;

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		if (!m_Ayar)
		{
			Print("[M360] ToplamaAlani: m_Ayar eksik", LogLevel.ERROR);
			return;
		}

		BaseWorld dunya = owner.GetWorld();
		if (dunya && dunya.IsEditMode())
			return;

		Print(string.Format("[M360] ToplamaAlani hazir: %1", m_Ayar.m_sIsAdi), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		GetGame().GetCallqueue().Remove(ToplamaIlerlemeAdimi);
		super.OnDelete(owner);
	}

	//------------------------------------------------------------------------------------------------
	void ToplamaDegistir(IEntity kullanici)
	{
		M360_JobSessionData oturum = M360_JobSessions.AlVeyaOlustur(kullanici);
		if (oturum.m_bTopluyor && m_ToplayanKullanici == kullanici)
			ToplamaDurdur(kullanici);
		else
			ToplamaBaslat(kullanici);
	}

	//------------------------------------------------------------------------------------------------
	void ToplamaBaslat(IEntity kullanici)
	{
		if (!m_Ayar || !kullanici)
			return;

		M360_JobSessionData oturum = M360_JobSessions.AlVeyaOlustur(kullanici);
		if (oturum.m_bIsliyor)
		{
			MesajGoster("Once isleme bitsin");
			return;
		}

		if (oturum.m_bTopluyor)
		{
			MesajGoster("Zaten topluyorsun");
			return;
		}

		int maxTasima = m_Ayar.m_iMaxTasima;
		if (maxTasima < 1)
			maxTasima = 40;

		if (oturum.m_iHam >= maxTasima)
		{
			MesajGoster("Tasima dolu (ham=" + oturum.m_iHam.ToString() + ")");
			return;
		}

		oturum.m_sIsAdi = m_Ayar.m_sIsAdi;
		oturum.m_iMaxTasima = maxTasima;
		oturum.m_bTopluyor = true;
		oturum.m_fToplamaIlerleme = 0;
		m_ToplayanKullanici = kullanici;
		m_iTamamlananTick = 0;
		m_iIlerlemeGecenMs = 0;

		m_iTickMs = m_Ayar.m_iTickSuresi * 1000;
		if (m_iTickMs < 100)
			m_iTickMs = 100;

		GetGame().GetCallqueue().CallLater(ToplamaIlerlemeAdimi, ILERLEME_ADIM_MS, true);
		MesajGoster("Toplama basladi: " + m_Ayar.m_sIsAdi);
	}

	//------------------------------------------------------------------------------------------------
	protected void ToplamaIlerlemeAdimi()
	{
		if (!m_Ayar || !m_ToplayanKullanici)
		{
			GetGame().GetCallqueue().Remove(ToplamaIlerlemeAdimi);
			return;
		}

		M360_JobSessionData oturum = M360_JobSessions.AlVeyaOlustur(m_ToplayanKullanici);
		if (!oturum.m_bTopluyor)
		{
			GetGame().GetCallqueue().Remove(ToplamaIlerlemeAdimi);
			return;
		}

		m_iIlerlemeGecenMs += ILERLEME_ADIM_MS;
		float yuzde = (m_iIlerlemeGecenMs * 100.0) / m_iTickMs;
		if (yuzde > 100)
			yuzde = 100;
		oturum.m_fToplamaIlerleme = yuzde;

		if (m_iIlerlemeGecenMs < m_iTickMs)
			return;

		// Tick tamam — ham ekle, ilerlemeyi sifirla
		m_iIlerlemeGecenMs = 0;
		oturum.m_fToplamaIlerleme = 0;

		int maxTasima = m_Ayar.m_iMaxTasima;
		if (maxTasima < 1)
			maxTasima = 40;

		int bosYer = maxTasima - oturum.m_iHam;
		if (bosYer <= 0)
		{
			ToplamaDurdur(m_ToplayanKullanici);
			MesajGoster("Tasima doldu");
			return;
		}

		int kazanc = m_Ayar.m_iTickVerim;
		if (kazanc > bosYer)
			kazanc = bosYer;

		oturum.m_iHam += kazanc;
		m_iTamamlananTick++;
		MesajGoster(string.Format("Toplandi +%1  |  ham=%2/%3", kazanc, oturum.m_iHam, maxTasima));
	}

	//------------------------------------------------------------------------------------------------
	void ToplamaDurdur(IEntity kullanici)
	{
		M360_JobSessionData oturum = M360_JobSessions.AlVeyaOlustur(kullanici);
		if (!oturum.m_bTopluyor)
			return;

		oturum.m_bTopluyor = false;
		oturum.m_fToplamaIlerleme = 0;
		GetGame().GetCallqueue().Remove(ToplamaIlerlemeAdimi);
		m_ToplayanKullanici = null;
		m_iIlerlemeGecenMs = 0;
		MesajGoster("Toplama durdu. Ham=" + oturum.m_iHam.ToString());
	}

	//------------------------------------------------------------------------------------------------
	bool BuKullaniciTopluyorMu(IEntity kullanici)
	{
		M360_JobSessionData oturum = M360_JobSessions.AlVeyaOlustur(kullanici);
		return oturum.m_bTopluyor && m_ToplayanKullanici == kullanici;
	}

	//------------------------------------------------------------------------------------------------
	protected void MesajGoster(string mesaj)
	{
		SCR_HintManagerComponent.ShowCustomHint(mesaj, "M360 Toplama", 2.5);
	}
}
