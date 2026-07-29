//------------------------------------------------------------------------------------------------
//! Lab: Arsenal kutusu supply kullanmasin (UI gri/0 supply engeli).
//! Satin alma odemesi: M360_SCR_ResourcePlayerControllerInventoryComponent (nakit).
//------------------------------------------------------------------------------------------------
modded class SCR_ArsenalComponent
{
	//------------------------------------------------------------------------------------------------
	override bool IsArsenalUsingSupplies()
	{
		if (M360_MagazaYardim.LabNakitAktif())
			return false;

		return super.IsArsenalUsingSupplies();
	}
}
