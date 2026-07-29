//------------------------------------------------------------------------------------------------
//! Bu entity uzerinde vanilla arsenal / vehicle service → M360 HUD nakit odemesi.
//! Fiyatlar: bilesen uzerindeki m_aKayitlar (layer/prefab) — harici conf GUID sart degil.
//------------------------------------------------------------------------------------------------
[EntityEditorProps(category: "M360/Magaza", description: "Supply yerine M360 nakit")]
class M360_MagazaNakitBileseniClass : ScriptComponentClass
{
}

class M360_MagazaNakitBileseni : ScriptComponent
{
	[Attribute("0", desc: "1 = sadece listedeki aktif urunler")]
	bool m_bSadeceListedekiler;

	[Attribute("0", desc: "Listede yoksa ve katalog 0 ise son care nakit (0 = yok)")]
	int m_iVarsayilanFiyat;

	[Attribute(desc: "Prefab → nakit satirlari")]
	ref array<ref M360_MagazaFiyatKaydi> m_aKayitlar;

	[Attribute("1", desc: "Iade (refund) da nakit kredi etsin")]
	bool m_bIadeAcik;

	//------------------------------------------------------------------------------------------------
	static M360_MagazaNakitBileseni Bul(IEntity ent)
	{
		while (ent)
		{
			M360_MagazaNakitBileseni b = M360_MagazaNakitBileseni.Cast(ent.FindComponent(M360_MagazaNakitBileseni));
			if (b)
				return b;

			ent = ent.GetParent();
		}

		return null;
	}

	//------------------------------------------------------------------------------------------------
	//! -1 = satilamaz
	//! Oncelik: m_aKayitlar → lab GUID listesi → katalogSupply (urun basina) → varsayilan
	int NakitFiyatCozumle(ResourceName prefab, int katalogSupply)
	{
		if (m_aKayitlar)
		{
			foreach (M360_MagazaFiyatKaydi kayit : m_aKayitlar)
			{
				if (!kayit)
					continue;

				string kayitPrefab = kayit.m_sPrefab;
				if (!kayitPrefab || kayitPrefab.Length() < 1)
					continue;

				if (!M360_MagazaYardim.PrefabEslesir(kayit.m_sPrefab, prefab))
					continue;

				if (!kayit.m_bAktif)
					return -1;

				return kayit.m_iFiyat;
			}
		}

		if (m_bSadeceListedekiler)
		{
			Print(string.Format("[M360 Magaza] Fiyat LISTE-DISI kapali prefab=%1", FilePath.StripPath(prefab)), LogLevel.WARNING);
			return -1;
		}

		// Layer Attribute eslesmese bile lab GUID tablosu
		int lab = M360_MagazaYardim.LabListeFiyat(prefab);
		if (lab >= 0)
		{
			Print(string.Format("[M360 Magaza] Fiyat LAB-LISTE prefab=%1 fiyat=%2", FilePath.StripPath(prefab), lab), LogLevel.NORMAL);
			return lab;
		}

		// KRITIK: once katalog (her urun farkli) — varsayilan sabit degil
		if (katalogSupply > 0)
		{
			Print(string.Format("[M360 Magaza] Fiyat KATALOG prefab=%1 fiyat=%2", FilePath.StripPath(prefab), katalogSupply), LogLevel.NORMAL);
			return katalogSupply;
		}

		if (m_iVarsayilanFiyat > 0)
		{
			Print(string.Format("[M360 Magaza] Fiyat VARSAYILAN prefab=%1 fiyat=%2", FilePath.StripPath(prefab), m_iVarsayilanFiyat), LogLevel.NORMAL);
			return m_iVarsayilanFiyat;
		}

		return 0;
	}

	//------------------------------------------------------------------------------------------------
	bool NakitOde(IEntity oyuncu, int miktar)
	{
		if (!Replication.IsServer() || !oyuncu || miktar < 0)
			return false;

		if (miktar == 0)
			return true;

		if (!M360_IsOturumlari.NakitHarca(oyuncu, miktar))
		{
			Print(string.Format("[M360 Magaza] Yetersiz nakit (gerekli %1)", miktar), LogLevel.WARNING);
			return false;
		}

		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(oyuncu);
		int kalan = 0;
		if (veri)
			kalan = veri.m_iNakit;

		Print(string.Format("[M360 Magaza] Nakit -%1 → kalan %2", miktar, kalan), LogLevel.NORMAL);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool NakitIade(IEntity oyuncu, int miktar)
	{
		if (!Replication.IsServer() || !oyuncu || miktar <= 0 || !m_bIadeAcik)
			return false;

		if (!M360_IsOturumlari.NakitEkle(oyuncu, miktar))
			return false;

		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(oyuncu);
		int kalan = 0;
		if (veri)
			kalan = veri.m_iNakit;

		Print(string.Format("[M360 Magaza] Iade +%1 → kalan %2", miktar, kalan), LogLevel.NORMAL);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool NakitYeterliMi(IEntity oyuncu, int miktar)
	{
		if (miktar <= 0)
			return true;

		// Karakter null olsa bile yerel controller'dan bak (arsenal UI)
		return M360_IsOturumlari.NakitYeterliMi(oyuncu, miktar);
	}
}
