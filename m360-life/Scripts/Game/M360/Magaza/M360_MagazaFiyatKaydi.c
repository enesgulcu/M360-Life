//------------------------------------------------------------------------------------------------
//! Tek urun fiyati — Configs/Magaza/*.conf icinde liste.
//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class M360_MagazaFiyatKaydi
{
	[Attribute("", UIWidgets.ResourceNamePicker, "Prefab (silah/kiyafet/arac)", "et")]
	ResourceName m_sPrefab;

	[Attribute("100", desc: "M360 nakit fiyati")]
	int m_iFiyat;

	[Attribute("1", desc: "0 = listede gosterilmez / satin alinamaz")]
	bool m_bAktif;
}
