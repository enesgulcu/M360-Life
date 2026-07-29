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

//------------------------------------------------------------------------------------------------
//! Runtime'da .conf dosyasindan yuklenen tek yetkili magaza listesi.
//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true)]
class M360_MagazaFiyatTablosu
{
	[Attribute("1", desc: "Yalniz aktif kayitlar satilabilir")]
	bool m_bSadeceListedekiler;

	[Attribute("0", desc: "Liste disi varsayilan fiyat")]
	int m_iVarsayilanFiyat;

	[Attribute("", UIWidgets.Object)]
	ref array<ref M360_MagazaFiyatKaydi> m_aKayitlar;
}
