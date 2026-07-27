//------------------------------------------------------------------------------------------------
//! M360 Life — Is ayarlari (Dokuman 5.1b / 13.3)
//! Prefab varsayilan degerleri; ileride PostgreSQL override (13.4).
//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class M360_JobConfig
{
	[Attribute("Pirinc", desc: "Is adi (Dokuman 5.2)")]
	string m_sIsAdi;

	[Attribute("1", desc: "Yasal is mi?")]
	bool m_bYasalMi;

	[Attribute("4", desc: "Tick suresi (sn)")]
	int m_iTickSuresi;

	[Attribute("2", desc: "Tick basi ham verim")]
	int m_iTickVerim;

	[Attribute("100", desc: "Isleme parti ust siniri")]
	int m_iPartiBoyutu;

	[Attribute("180", desc: "Isleme suresi (sn) — uretim Pirinc ~180")]
	int m_iIslemeSuresi;

	[Attribute("1.0", desc: "Donusum orani islenmis/ham")]
	float m_fDonusumOrani;

	[Attribute("600", desc: "Islenmis birim satis fiyati")]
	int m_iSatisFiyati;

	[Attribute("40", desc: "Oyuncu uzeri max ham tasima (lab envanter stub)")]
	int m_iMaxTasima;
}
