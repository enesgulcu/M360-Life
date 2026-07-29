//------------------------------------------------------------------------------------------------
//! Lab arsenal magazasi: isaret bileseni VEYA entity adi.
//! Prefab eslesme: GUID once (ResourceName string formati farkli olabiliyor).
//------------------------------------------------------------------------------------------------
class M360_MagazaYardim
{
	static const int VARS_ESYA = 25;

	//------------------------------------------------------------------------------------------------
	static M360_MagazaNakitBileseni BilesenBul(IEntity ent)
	{
		M360_MagazaNakitBileseni b = M360_MagazaNakitBileseni.Bul(ent);
		if (b)
			return b;

		if (ent)
		{
			SCR_ArsenalComponent ars = SCR_ArsenalComponent.FindArsenalComponent(ent);
			if (ars)
			{
				b = M360_MagazaNakitBileseni.Bul(ars.GetOwner());
				if (b)
					return b;
			}
		}

		return null;
	}

	//------------------------------------------------------------------------------------------------
	//! ResourceName "{GUID}path" — yalniz 16 hex GUID. Path-only / bozuk IndexOf YASAK (yanlis eslesme).
	static string PrefabGuidAl(ResourceName rn)
	{
		string s = rn;
		if (!s || s.Length() < 18)
			return string.Empty;

		int ac = s.IndexOf("{");
		if (ac < 0)
			return string.Empty;

		int kapa = s.IndexOf("}");
		if (kapa != ac + 17)
			return string.Empty;

		string g = s.Substring(ac + 1, 16);
		if (!GuidHexMi(g))
			return string.Empty;

		return g;
	}

	//------------------------------------------------------------------------------------------------
	static bool GuidHexMi(string g)
	{
		if (!g || g.Length() != 16)
			return false;

		// Path/sapan GUID: PrefabGuidAl eskiden IndexOf hatasiyla "refabs/Vehicles/" uretiyordu
		if (g.Contains("/") || g.Contains(".") || g.Contains("\\") || g.Contains(":"))
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool PrefabEslesir(ResourceName a, ResourceName b)
	{
		string sa = a;
		string sb = b;
		if (!sa || !sb || sa.Length() < 1 || sb.Length() < 1)
			return false;

		if (a == b)
			return true;

		if (sa == sb)
			return true;

		string ga = PrefabGuidAl(a);
		string gb = PrefabGuidAl(b);
		if (ga.Length() == 16 && gb.Length() == 16 && ga == gb)
			return true;

		string pa = FilePath.StripPath(a);
		string pb = FilePath.StripPath(b);
		if (pa.Length() > 0 && pa == pb)
			return true;

		return false;
	}

	//------------------------------------------------------------------------------------------------
	//! Layer Attribute bos kalsa bile lab liste fiyatlari (GUID).
	//! -1 = bu tabloda yok.
	static int LabListeFiyat(ResourceName prefab)
	{
		string g = PrefabGuidAl(prefab);
		if (g.Length() < 1)
		{
			string ad = FilePath.StripPath(prefab);
			if (ad == "Handgun_M9.et")
				return 50;
			if (ad == "Rifle_AK74.et")
				return 250;
			if (ad == "Rifle_M16A2.et")
				return 300;
			return -1;
		}

		if (g == "1353C6EAD1DCFE43")
			return 50;
		if (g == "FA5C25BF66A53DCF")
			return 250;
		if (g == "3E413771E1834D2F")
			return 300;
		return -1;
	}

	//------------------------------------------------------------------------------------------------
	static bool ArsenalMagazaMi(IEntity ent)
	{
		if (BilesenBul(ent))
			return true;

		return AdEslesir(ent, "M360_Vanil") || AdEslesir(ent, "M360_Arsenal");
	}

	//------------------------------------------------------------------------------------------------
	static bool LabNakitAktif()
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool AdEslesir(IEntity ent, string prefix)
	{
		while (ent)
		{
			string ad = ent.GetName();
			if (ad && ad.Contains(prefix))
				return true;

			ent = ent.GetParent();
		}

		return false;
	}

	//------------------------------------------------------------------------------------------------
	//! Oncelik: bilesen listesi → lab GUID listesi → katalog supply → varsayilan.
	static int EsyaFiyat(IEntity magazaEnt, ResourceName prefab, int katalogSupply)
	{
		M360_MagazaNakitBileseni b = BilesenBul(magazaEnt);
		if (b)
			return b.NakitFiyatCozumle(prefab, katalogSupply);

		int lab = LabListeFiyat(prefab);
		if (lab >= 0)
		{
			Print(string.Format("[M360 Magaza] Fiyat LAB-LISTE (bilesen yok) prefab=%1 fiyat=%2", FilePath.StripPath(prefab), lab), LogLevel.NORMAL);
			return lab;
		}

		if (katalogSupply > 0)
		{
			Print(string.Format("[M360 Magaza] Fiyat KATALOG (bilesen yok) prefab=%1 fiyat=%2", FilePath.StripPath(prefab), katalogSupply), LogLevel.NORMAL);
			return katalogSupply;
		}

		return VARS_ESYA;
	}

}
