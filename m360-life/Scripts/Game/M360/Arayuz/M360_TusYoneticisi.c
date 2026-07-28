//------------------------------------------------------------------------------------------------
//! Panel omurgasi: Life canta + vanilla envanter.
//------------------------------------------------------------------------------------------------
class M360_TusYoneticisi
{
	//------------------------------------------------------------------------------------------------
	static void LifeCantaAcKapa()
	{
		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (!canta)
		{
			Print("[M360] TusYoneticisi: CantaHud yok", LogLevel.WARNING);
			return;
		}
		canta.EnvanterAcKapa();
	}

	//------------------------------------------------------------------------------------------------
	//! Tab = vanilla (ActionOpenInventory CAGIRMA — Life hijack'e duser)
	static void VanillaEnvanterAc(IEntity karakter)
	{
		if (!karakter)
			return;

		SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(
			karakter.FindComponent(SCR_InventoryStorageManagerComponent));
		if (inv)
			inv.OpenInventory();
	}
}
