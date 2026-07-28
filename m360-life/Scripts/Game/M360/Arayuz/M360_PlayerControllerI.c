//------------------------------------------------------------------------------------------------
//! I = Life: ActionOpenInventory hijack + Inventory klavyede sadece I (oturum, Save YOK).
//! Tab = vanilla: Inventory'de Tab yok; QS8<-Tab + OpenInventory API.
//!
//! KRITIK (bu PC): InputBinding.Save() profili BOSALTIYOR → Tab default Inventory'ye
//! donup Life aciyor. Save YASAK. Profil dosyasi elle I-only yazili.
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	protected bool m_bM360Kuruldu;
	protected bool m_bM360TabListener;
	protected int m_iM360Deneme;

	//------------------------------------------------------------------------------------------------
	override void ActionOpenInventory()
	{
		// Inventory action (sadece I bagli olmali) → Life
		Print("[M360] Inventory → Life HUD", LogLevel.NORMAL);
		M360_TusYoneticisi.LifeCantaAcKapa();
	}

	//------------------------------------------------------------------------------------------------
	override void OnUpdate(float timeSlice)
	{
		super.OnUpdate(timeSlice);

		if (!GetControlledEntity())
			return;

		// Spawn sonrasi birkaç kez uygula (Save yok; oturum baglari dusebilir)
		if (m_bM360Kuruldu)
			return;

		m_iM360Deneme++;
		if (m_iM360Deneme == 1 || m_iM360Deneme == 30 || m_iM360Deneme == 60)
			M360_DedicatedTusKur();

		if (m_iM360Deneme >= 60)
			m_bM360Kuruldu = true;
	}

	//------------------------------------------------------------------------------------------------
	protected void M360_DedicatedTusKur()
	{
		InputManager im = GetGame().GetInputManager();
		if (!im)
			return;

		InputBinding baglama = im.CreateUserBinding();
		if (!baglama)
			return;

		// Inventory: user bind → TUM klavye sil → sadece I (Save YOK)
		baglama.CreateUserBinding("Inventory", EInputDeviceType.KEYBOARD, string.Empty);
		M360_KlavyeTemizle(baglama, "Inventory");
		baglama.AddBinding("Inventory", string.Empty, "keyboard:KC_I", string.Empty);

		// Tab vanilla: QS8
		baglama.CreateUserBinding("InventoryQuickSlot8", EInputDeviceType.KEYBOARD, string.Empty);
		M360_KlavyeTemizle(baglama, "InventoryQuickSlot8");
		baglama.AddBinding("InventoryQuickSlot8", string.Empty, "keyboard:KC_TAB", string.Empty);

		// Save() YASAK — bu PC'de bos profil yaziyor

		if (!m_bM360TabListener)
		{
			im.AddActionListener("InventoryQuickSlot8", EActionTrigger.DOWN, M360_OnTab);
			m_bM360TabListener = true;
		}

		array<string> inv = {};
		baglama.GetBindings("Inventory", inv, EInputDeviceType.KEYBOARD, string.Empty, true);
		string s = "";
		foreach (string x : inv)
		{
			if (s != "") s = s + ",";
			if (x) s = s + x;
		}
		Print(string.Format("[M360] Tus (Save=YOK) Inventory=[%1] | Tab=QS8 vanilla", s), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void M360_KlavyeTemizle(InputBinding baglama, string aksiyon)
	{
		int n = baglama.GetBindingsCount(aksiyon, EInputDeviceType.KEYBOARD, string.Empty);
		for (int i = n - 1; i >= 0; i--)
			baglama.RemoveBinding(aksiyon, EInputDeviceType.KEYBOARD, string.Empty, i);
	}

	//------------------------------------------------------------------------------------------------
	protected void M360_OnTab()
	{
		Print("[M360] Tab → vanilla envanter", LogLevel.NORMAL);
		M360_TusYoneticisi.VanillaEnvanterAc(GetControlledEntity());
	}
}
