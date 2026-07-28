//------------------------------------------------------------------------------------------------
//! M360 Life — Bulut API istemcisi (Vercel + Neon)
//! Ilk baglanti testi: /api/health ve /api/jobs
//! Sure olcumu: X-M360-Istek-Baslangic header (API metrik paneli)
//------------------------------------------------------------------------------------------------

class M360_ApiIstekCallback : RestCallback
{
	protected string m_sEtiket;
	protected int m_iBaslangicMs;

	void M360_ApiIstekCallback(string etiket, int baslangicMs)
	{
		m_sEtiket = etiket;
		m_iBaslangicMs = baslangicMs;
		SetOnSuccess(OnBasarili);
		SetOnError(OnHata);
	}

	protected void OnBasarili(RestCallback cb)
	{
		int gecen = System.GetTickCount() - m_iBaslangicMs;
		string veri = cb.GetData();
		int kod = cb.GetHttpCode();
		Print(string.Format("[M360 API] OK %1 http=%2 sure~%3ms", m_sEtiket, kod, gecen), LogLevel.NORMAL);
		if (veri && veri.Length() > 0)
		{
			int maxLen = 400;
			if (veri.Length() > maxLen)
				Print(string.Format("[M360 API] govde: %1...", veri.Substring(0, maxLen)), LogLevel.NORMAL);
			else
				Print(string.Format("[M360 API] govde: %1", veri), LogLevel.NORMAL);
		}
	}

	protected void OnHata(RestCallback cb)
	{
		int gecen = System.GetTickCount() - m_iBaslangicMs;
		ERestResult sonuc = cb.GetRestResult();
		Print(string.Format("[M360 API] HATA %1 rest=%2 sure~%3ms", m_sEtiket, sonuc, gecen), LogLevel.WARNING);
		string veri = cb.GetData();
		if (veri && veri.Length() > 0)
			Print(string.Format("[M360 API] hata govde: %1", veri), LogLevel.WARNING);
	}
}

//------------------------------------------------------------------------------------------------
class M360_ApiIstemci
{
	protected static string s_sApiKok = "https://m360-life.vercel.app";
	protected static string s_sSunucuAnahtari = ""; // Vercel M360_SERVER_KEY ile ayni (bos = lab acik)
	protected static ref RestCallback s_HealthCb;
	protected static ref RestCallback s_JobsCb;

	static void ApiKokAyarla(string kok)
	{
		s_sApiKok = kok;
	}

	static void SunucuAnahtariAyarla(string anahtar)
	{
		s_sSunucuAnahtari = anahtar;
	}

	static string ApiKok()
	{
		return s_sApiKok;
	}

	//! Play / dedicated: health + jobs cagir (log'a yazar)
	static void BaglantiTesti()
	{
		RestApi api = GetGame().GetRestApi();
		if (!api)
		{
			Print("[M360 API] RestApi yok — oyun baglaminda calistir.", LogLevel.ERROR);
			return;
		}

		string kok = s_sApiKok;
		if (!kok || kok.Length() < 8)
		{
			Print("[M360 API] Api kok bos.", LogLevel.ERROR);
			return;
		}

		RestContext ctx = api.GetContext(kok);
		if (!ctx)
		{
			Print("[M360 API] RestContext olusturulamadi.", LogLevel.ERROR);
			return;
		}

		// Not: X-M360-Istek-Baslangic unix ms ister (tarayici istatistik).
		// Oyunda GetTickCount farkli saat — RTT burada Print ile olculur.
		if (s_sSunucuAnahtari && s_sSunucuAnahtari.Length() > 0)
			ctx.SetHeaders("Content-Type,application/json,X-M360-Server-Key," + s_sSunucuAnahtari);
		else
			ctx.SetHeaders("Content-Type,application/json");

		int t0 = System.GetTickCount();
		s_HealthCb = new M360_ApiIstekCallback("health", t0);
		s_JobsCb = new M360_ApiIstekCallback("jobs", t0);

		Print(string.Format("[M360 API] Test basliyor: %1", kok), LogLevel.NORMAL);
		ctx.GET(s_HealthCb, "/api/health");
		ctx.GET(s_JobsCb, "/api/jobs");
	}
}

//------------------------------------------------------------------------------------------------
//! Lab: Play basinda otomatik API testi (prefab'a ekle)
class M360_ApiBaglantiTestiBileseniClass : ScriptComponentClass
{
}

class M360_ApiBaglantiTestiBileseni : ScriptComponent
{
	[Attribute("https://m360-life.vercel.app", desc: "Vercel API kok URL (sonda / yok)")]
	string m_sApiKok;

	[Attribute("", desc: "Opsiyonel X-M360-Server-Key (Vercel env ile ayni)")]
	string m_sSunucuAnahtari;

	[Attribute("1", desc: "Play basinda otomatik health+jobs testi")]
	bool m_bPlayBasindaTest;

	//------------------------------------------------------------------------------------------------
	//! Lab: Attribute bos ise $profile:M360_ApiLabKey.txt (git'e girmez)
	protected string AnahtarCoz()
	{
		if (m_sSunucuAnahtari && m_sSunucuAnahtari.Length() > 0)
			return m_sSunucuAnahtari;

		string yol = "$profile:M360_ApiLabKey.txt";
		if (!FileIO.FileExists(yol))
			return "";

		FileHandle fh = FileIO.OpenFile(yol, FileMode.READ);
		if (!fh)
			return "";

		string satir;
		fh.ReadLine(satir);
		fh.Close();
		if (!satir)
			return "";
		satir.Replace("\r", "");
		satir.Replace("\n", "");
		return satir;
	}

	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}

	override void EOnInit(IEntity owner)
	{
		if (!m_bPlayBasindaTest)
			return;

		if (m_sApiKok && m_sApiKok.Length() > 0)
			M360_ApiIstemci.ApiKokAyarla(m_sApiKok);

		string anahtar = AnahtarCoz();
		if (anahtar && anahtar.Length() > 0)
			M360_ApiIstemci.SunucuAnahtariAyarla(anahtar);
		else
			Print("[M360 API] Uyari: sunucu anahtari yok — jobs 401 olabilir. $profile:M360_ApiLabKey.txt yaz.", LogLevel.WARNING);

		GetGame().GetCallqueue().CallLater(GecikmeliTest, 1500, false);
	}

	protected void GecikmeliTest()
	{
		M360_ApiIstemci.BaglantiTesti();
	}
}
