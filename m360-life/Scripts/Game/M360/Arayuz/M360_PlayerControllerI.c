//------------------------------------------------------------------------------------------------
//! Life canta = Inventory action.
//! Tab: vanilla Inventory + ActionOpenInventory hijack.
//! I: InputBinding.AddBinding("Inventory", keyboard:KC_I) + Save() — profilde kalir (dedicated kanit 2026-07-28).
//! YASAK: custom Context/Overlay, ActivateAction spam, ActionInput sinifi, rdb silmek.
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	protected bool m_bM360IBaglandi;

	//------------------------------------------------------------------------------------------------
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

	//------------------------------------------------------------------------------------------------
	override void OnUpdate(float timeSlice)
	{
		super.OnUpdate(timeSlice);

		if (m_bM360IBaglandi)
			return;
		if (!GetControlledEntity())
			return;

		m_bM360IBaglandi = true;
		M360_InventoryIEkle();
	}

	//------------------------------------------------------------------------------------------------
	protected void M360_InventoryIEkle()
	{
		InputManager im = GetGame().GetInputManager();
		if (!im)
			return;

		InputBinding baglama = im.CreateUserBinding();
		if (!baglama)
		{
			Print("[M360] InputBinding yok — Tab ile canta", LogLevel.WARNING);
			return;
		}

		if (baglama.IsDefault("Inventory", EInputDeviceType.KEYBOARD, string.Empty))
			baglama.CreateUserBinding("Inventory", EInputDeviceType.KEYBOARD, string.Empty);

		baglama.AddBinding("Inventory", string.Empty, "keyboard:KC_I", string.Empty);
		baglama.Save();
		Print("[M360] Inventory <- I (Tab + I = Life canta)", LogLevel.NORMAL);
	}
}
