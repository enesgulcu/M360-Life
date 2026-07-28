//------------------------------------------------------------------------------------------------
//! Life canta = vanilla Inventory action (varsayilan Tab).
//! I baglama (M360_Input) dedicated istemcide InputManager conf yuklenmeyince oluyor;
//! eve Workbench Play ile dogrulanip tekrar baglanacak (docs/15 §C).
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	override void ActionOpenInventory()
	{
		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (canta)
		{
			canta.EnvanterAcKapa();
			return;
		}
		super.ActionOpenInventory();
	}
}
