//------------------------------------------------------------------------------------------------
//! M360 Life — Oyuncu is oturumu (lab envanter/para stub)
//! Ileride: gercek envanter + banka ledger (Dokuman 4/5/11) ile degistirilir.
//------------------------------------------------------------------------------------------------
class M360_JobSessionData
{
	string m_sIsAdi;
	int m_iHam;
	int m_iIslenmis;
	int m_iNakit;
	int m_iMaxTasima;
	bool m_bTopluyor;
	bool m_bIsliyor;
	float m_fToplamaIlerleme; // 0..100 — tek tick icinde
	float m_fIslemeIlerleme; // 0..100 — isleme suresi icinde
}

class M360_JobSessions
{
	protected static ref map<int, ref M360_JobSessionData> s_mOturumlar;

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
	static M360_JobSessionData AlVeyaOlustur(IEntity kullanici)
	{
		if (!s_mOturumlar)
			s_mOturumlar = new map<int, ref M360_JobSessionData>();

		int oyuncuId = OyuncuIdCozumle(kullanici);
		M360_JobSessionData veri = s_mOturumlar.Get(oyuncuId);
		if (!veri)
		{
			veri = new M360_JobSessionData();
			veri.m_iMaxTasima = 40;
			s_mOturumlar.Insert(oyuncuId, veri);
		}
		return veri;
	}

	//------------------------------------------------------------------------------------------------
	static string DurumMetni(M360_JobSessionData veri)
	{
		if (!veri)
			return "Oturum yok";

		string isAdi = veri.m_sIsAdi;
		if (isAdi.IsEmpty())
			isAdi = "-";

		return string.Format("%1 | Ham=%2 | Islenmis=%3 | Nakit=%4", isAdi, veri.m_iHam, veri.m_iIslenmis, veri.m_iNakit);
	}
}
