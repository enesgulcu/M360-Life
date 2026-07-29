//------------------------------------------------------------------------------------------------
//! M360 Life — Oyuncu is oturumu (lab envanter/para sayac)
//! Nakit TEK KAYNAK: SCR_PlayerController RplProp (istemci OwnerOnly sync).
//! Ileride: gercek envanter + banka ledger (Dokuman 4/5/11) ile degistirilir.
//------------------------------------------------------------------------------------------------
class M360_IsOturumVerisi
{
	string m_sIsAdi;
	int m_iHam;
	int m_iIslenmis;
	int m_iNakit;
	int m_iMaxTasima;
	bool m_bTopluyor;
	bool m_bIsliyor;
	float m_fToplamaIlerleme; // 0..100 — tek adim icinde
	float m_fIslemeIlerleme; // 0..100 — isleme suresi icinde
	float m_fAclik; // 0..100 lab stub (docs 5.5)
	float m_fSusuzluk; // 0..100 lab stub
}

class M360_IsOturumlari
{
	protected static ref map<int, ref M360_IsOturumVerisi> s_mOturumlar;

	//------------------------------------------------------------------------------------------------
	//! Oyuncu kimligini cozumle (yoksa lab icin 1)
	static int OyuncuIdCozumle(IEntity kullanici)
	{
		if (!kullanici)
			return 0;

		PlayerManager oyuncuYoneticisi = GetGame().GetPlayerManager();
		if (oyuncuYoneticisi)
		{
			int id = oyuncuYoneticisi.GetPlayerIdFromControlledEntity(kullanici);
			if (id > 0)
				return id;
		}

		return 1;
	}

	//------------------------------------------------------------------------------------------------
	static M360_IsOturumVerisi AlVeyaOlustur(IEntity kullanici)
	{
		if (!s_mOturumlar)
			s_mOturumlar = new map<int, ref M360_IsOturumVerisi>();

		int oyuncuId = OyuncuIdCozumle(kullanici);
		M360_IsOturumVerisi veri = s_mOturumlar.Get(oyuncuId);
		if (!veri)
		{
			veri = new M360_IsOturumVerisi();
			veri.m_iMaxTasima = 40;
			veri.m_fAclik = 100;
			veri.m_fSusuzluk = 100;
			s_mOturumlar.Insert(oyuncuId, veri);
		}

		// Nakit: PlayerController RplProp aynasi (HUD / magaza ayni rakam)
		SCR_PlayerController pc = SCR_PlayerController.M360_ControllerBul(kullanici);
		if (pc)
			veri.m_iNakit = pc.M360_NakitAl();

		return veri;
	}

	//------------------------------------------------------------------------------------------------
	static bool NakitHarca(IEntity kullanici, int miktar)
	{
		SCR_PlayerController pc = SCR_PlayerController.M360_ControllerBul(kullanici);
		if (!pc)
			return false;

		if (!pc.M360_NakitHarca(miktar))
			return false;

		M360_IsOturumVerisi veri = AlVeyaOlustur(kullanici);
		if (veri)
			veri.m_iNakit = pc.M360_NakitAl();

		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool NakitEkle(IEntity kullanici, int miktar)
	{
		SCR_PlayerController pc = SCR_PlayerController.M360_ControllerBul(kullanici);
		if (!pc)
			return false;

		if (!pc.M360_NakitEkle(miktar))
			return false;

		M360_IsOturumVerisi veri = AlVeyaOlustur(kullanici);
		if (veri)
			veri.m_iNakit = pc.M360_NakitAl();

		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool NakitYeterliMi(IEntity kullanici, int miktar)
	{
		SCR_PlayerController pc = SCR_PlayerController.M360_ControllerBul(kullanici);
		if (pc)
			return pc.M360_NakitYeterliMi(miktar);

		M360_IsOturumVerisi veri = AlVeyaOlustur(kullanici);
		return veri && veri.m_iNakit >= miktar;
	}

	//------------------------------------------------------------------------------------------------
	//! Lab: ~5-6 saatlik tuketim (docs 5.5) — gorunur demo icin hafif decay
	static void VitalTick(M360_IsOturumVerisi veri, float timeSlice)
	{
		if (!veri || timeSlice <= 0)
			return;

		// 100 / (5.5 * 3600) ≈ 0.00505 birim/sn
		float azalma = timeSlice * 0.00505;
		veri.m_fAclik = veri.m_fAclik - azalma;
		veri.m_fSusuzluk = veri.m_fSusuzluk - azalma * 1.05;
		if (veri.m_fAclik < 0)
			veri.m_fAclik = 0;
		if (veri.m_fSusuzluk < 0)
			veri.m_fSusuzluk = 0;
	}

	//------------------------------------------------------------------------------------------------
	static string DurumMetni(M360_IsOturumVerisi veri)
	{
		if (!veri)
			return "Oturum yok";

		string isAdi = veri.m_sIsAdi;
		if (isAdi.IsEmpty())
			isAdi = "-";

		return string.Format("%1 | Ham=%2 | Islenmis=%3 | Nakit=%4", isAdi, veri.m_iHam, veri.m_iIslenmis, veri.m_iNakit);
	}
}
