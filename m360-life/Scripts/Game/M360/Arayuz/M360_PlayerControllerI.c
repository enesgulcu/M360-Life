//------------------------------------------------------------------------------------------------
//! Yerel controller → M360 tuş omurgası.
//!
//! KURALLAR:
//!   - Yalnız m_bIsLocalPlayerController (dedicated headless / uzak PC dinlemez)
//!   - ActionOpenInventory override YASAK (Tab = vanilla)
//!   - Ekonomi / item yazma YASAK (docs/19, docs/20)
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	//------------------------------------------------------------------------------------------------
	override void UpdateLocalPlayerController()
	{
		super.UpdateLocalPlayerController();

		if (!m_bIsLocalPlayerController)
			return;

		M360_TusYoneticisi.Kur();
	}
}
