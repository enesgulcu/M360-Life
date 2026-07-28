//------------------------------------------------------------------------------------------------
//! Lab duz zemin: sert gunes + beyaz grid asiri parlak / siyah gokyuzu.
//! Soft sabah saati + bulutlu hava (loop) ile dengele.
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Lab", description: "Lab isik/hava yumusatma")]
class M360_LabOrtamBileseniClass : ScriptComponentClass
{
}

class M360_LabOrtamBileseni : ScriptComponent
{
	[Attribute("12", UIWidgets.Slider, "Baslangic saati (12 = tepeden gunes)", "0 23 1")]
	protected int m_iSaat;

	[Attribute("0", UIWidgets.Slider, "Baslangic dakika", "0 59 1")]
	protected int m_iDakika;

	[Attribute("Overcast", desc: "Tercih edilen hava (yoksa Cloudy / listeden)")]
	protected string m_sHava;

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;

		// Hava sunucuda set edilir; istemciye replicate olur.
		if (!Replication.IsServer())
			return;

		GetGame().GetCallqueue().CallLater(OrtamUygula, 500, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void OrtamUygula()
	{
		ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (!world)
			return;

		TimeAndWeatherManagerEntity mgr = world.GetTimeAndWeatherManager();
		if (!mgr)
		{
			Print("[M360] LabOrtam: TimeAndWeatherManager yok", LogLevel.WARNING);
			return;
		}

		mgr.SetHoursMinutesSeconds(m_iSaat, m_iDakika, 0, true);

		string hava = HavaSec(mgr);
		if (hava && hava.Length() > 0)
		{
			mgr.ForceWeatherTo(true, hava, 0, 0.001);
			Print("[M360] LabOrtam: saat " + m_iSaat.ToString() + ":" + m_iDakika.ToString() + " hava=" + hava, LogLevel.NORMAL);
		}
		else
		{
			Print("[M360] LabOrtam: hava state bulunamadi; sadece saat ayarlandi", LogLevel.WARNING);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected string HavaSec(TimeAndWeatherManagerEntity mgr)
	{
		if (m_sHava && m_sHava.Length() > 0)
		{
			// Once tercih edilen isim — yanlis olsa bile Force dener; listeden dogrula
			array<ref WeatherState> states = {};
			mgr.GetWeatherStatesList(states);

			foreach (WeatherState st : states)
			{
				if (!st)
					continue;
				string ad = st.GetStateName();
				if (ad == m_sHava)
					return ad;
			}
		}

		array<ref WeatherState> hepsi = {};
		mgr.GetWeatherStatesList(hepsi);

		string overcast;
		string cloudy;
		string ilk;

		foreach (WeatherState st : hepsi)
		{
			if (!st)
				continue;
			string ad = st.GetStateName();
			if (!ad || ad.Length() < 1)
				continue;

			if (!ilk || ilk.Length() < 1)
				ilk = ad;

			string lower = ad;
			lower.ToLower();

			if (lower.Contains("overcast"))
				overcast = ad;
			else if (lower.Contains("cloud"))
				cloudy = ad;
		}

		if (overcast && overcast.Length() > 0)
			return overcast;
		if (cloudy && cloudy.Length() > 0)
			return cloudy;
		return ilk;
	}
}
