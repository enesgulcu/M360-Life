//------------------------------------------------------------------------------------------------
//! Lab magaza: isaret bileseni VEYA entity adi (M360_Vanil / M360_Arac).
//! Bilesen layer'da yoksa bile nakit odeme calissin.
//------------------------------------------------------------------------------------------------
class M360_MagazaYardim
{
	static const int VARS_ESYA = 25;
	static const int VARS_ARAC = 500;

	//------------------------------------------------------------------------------------------------
	static M360_MagazaNakitBileseni BilesenBul(IEntity ent)
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
	static bool ArsenalMagazaMi(IEntity ent)
	{
		if (BilesenBul(ent))
			return true;

		return AdEslesir(ent, "M360_Vanil") || AdEslesir(ent, "M360_Arsenal");
	}

	//------------------------------------------------------------------------------------------------
	static bool AracMagazaMi(IEntity ent)
	{
		if (BilesenBul(ent))
			return true;

		return AdEslesir(ent, "M360_Arac");
	}

	//------------------------------------------------------------------------------------------------
	//! Lab: tum arsenal / vehicle service M360 nakit kullanir (isaret olmasa da).
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
	static int EsyaFiyat(IEntity magazaEnt, ResourceName prefab, int katalogSupply)
	{
		M360_MagazaNakitBileseni b = BilesenBul(magazaEnt);
		if (b)
			return b.NakitFiyatCozumle(prefab, katalogSupply);

		if (katalogSupply > 0)
			return katalogSupply;

		return VARS_ESYA;
	}

	//------------------------------------------------------------------------------------------------
	static int AracFiyat(IEntity magazaEnt, ResourceName prefab, int katalogSupply)
	{
		M360_MagazaNakitBileseni b = BilesenBul(magazaEnt);
		if (b)
			return b.NakitFiyatCozumle(prefab, katalogSupply);

		if (katalogSupply > 0)
			return katalogSupply;

		return VARS_ARAC;
	}
}
