//------------------------------------------------------------------------------------------------
//! Yerel controller → M360 tuş omurgası + nakit RplProp (istemci↔sunucu tek kaynak).
//!
//! KURALLAR:
//!   - Yalnız m_bIsLocalPlayerController (dedicated headless / uzak PC dinlemez) — tuş
//!   - ActionOpenInventory override YASAK (Tab = vanilla)
//!   - Nakit yazma yalnız sunucu (BumpMe → OwnerOnly istemci)
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	static const int M360_LAB_NAKIT_BASLANGIC = 2750499;

	//! -1 = henuz init yok (istemci sync bekliyor olabilir)
	[RplProp(condition: RplCondition.OwnerOnly, onRplName: "M360_OnNakitReplike")]
	protected int m_iM360Nakit = -1;

	//------------------------------------------------------------------------------------------------
	override void UpdateLocalPlayerController()
	{
		super.UpdateLocalPlayerController();

		if (!m_bIsLocalPlayerController)
			return;

		M360_TusYoneticisi.Kur();
	}

	//------------------------------------------------------------------------------------------------
	void M360_OnNakitReplike()
	{
		// HUD bir sonraki frame AlVeyaOlustur ile ceker
		Print(string.Format("[M360] Nakit sync (istemci) = %1", m_iM360Nakit), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! Sunucuda yoksa lab bakiyesi acar ve istemciye yollar.
	//! Istemci sync oncesi lab varsayilan gosterir (Rpl gelince dogru deger).
	int M360_NakitAl()
	{
		if (m_iM360Nakit < 0)
		{
			if (Replication.IsServer())
			{
				m_iM360Nakit = M360_LAB_NAKIT_BASLANGIC;
				Replication.BumpMe();
				Print(string.Format("[M360] Nakit init (sunucu) = %1", m_iM360Nakit), LogLevel.NORMAL);
				return m_iM360Nakit;
			}

			return M360_LAB_NAKIT_BASLANGIC;
		}

		return m_iM360Nakit;
	}

	//------------------------------------------------------------------------------------------------
	bool M360_NakitYeterliMi(int miktar)
	{
		if (miktar <= 0)
			return true;

		return M360_NakitAl() >= miktar;
	}

	//------------------------------------------------------------------------------------------------
	bool M360_NakitHarca(int miktar)
	{
		if (!Replication.IsServer() || miktar < 0)
			return false;

		if (miktar == 0)
			return true;

		if (!M360_NakitYeterliMi(miktar))
			return false;

		m_iM360Nakit = M360_NakitAl() - miktar;
		Replication.BumpMe();
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool M360_NakitEkle(int miktar)
	{
		if (!Replication.IsServer() || miktar == 0)
			return false;

		m_iM360Nakit = M360_NakitAl() + miktar;
		if (m_iM360Nakit < 0)
			m_iM360Nakit = 0;

		Replication.BumpMe();
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static SCR_PlayerController M360_ControllerBul(IEntity karakterVeyaNull)
	{
		PlayerManager pm = GetGame().GetPlayerManager();
		if (!pm)
			return null;

		int id = 0;
		if (karakterVeyaNull)
			id = pm.GetPlayerIdFromControlledEntity(karakterVeyaNull);

		if (id <= 0)
			id = SCR_PlayerController.GetLocalPlayerId();

		if (id <= 0)
			return null;

		return SCR_PlayerController.Cast(pm.GetPlayerController(id));
	}
}
