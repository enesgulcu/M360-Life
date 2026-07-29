//------------------------------------------------------------------------------------------------
//! Vehicle Service → M360 nakit. Supply menusu / 0 supply engelini kaldir.
//------------------------------------------------------------------------------------------------
modded class SCR_CatalogEntitySpawnerComponent
{
	//------------------------------------------------------------------------------------------------
	override bool IsSuppliesConsumptionEnabled()
	{
		if (M360_MagazaYardim.LabNakitAktif())
			return false;

		return m_bSuppliesConsumptionEnabled;
	}

	//------------------------------------------------------------------------------------------------
	override void InitiateSpawn(notnull SCR_EntityCatalogEntry entityEntry, int userId, SCR_EntitySpawnerSlotComponent slot)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
		{
			super.InitiateSpawn(entityEntry, userId, slot);
			return;
		}

		IEntity user = GetGame().GetPlayerManager().GetPlayerControlledEntity(userId);
		if (!user)
			return;

		SCR_EntityCatalogSpawnerData spawnerData = SCR_EntityCatalogSpawnerData.Cast(entityEntry.GetEntityDataOfType(SCR_EntityCatalogSpawnerData));
		if (!spawnerData)
			return;

		ResourceName prefab = entityEntry.GetPrefab();
		int nakitFiyat = M360_MagazaYardim.AracFiyat(GetOwner(), prefab, spawnerData.GetSupplyCost());
		if (nakitFiyat < 0)
		{
			Print(string.Format("[M360 Magaza] Arac satisa kapali: %1", prefab), LogLevel.WARNING);
			return;
		}

		if (GetRequestState(entityEntry, user) != SCR_EEntityRequestStatus.CAN_SPAWN)
		{
			Print(string.Format("[M360 Magaza] Arac spawn red"), LogLevel.WARNING);
			return;
		}

		if (!M360_IsOturumlari.NakitHarca(user, nakitFiyat))
		{
			Print(string.Format("[M360 Magaza] Arac yetersiz nakit (gerekli %1)", nakitFiyat), LogLevel.WARNING);
			return;
		}

		bool eskiSupply = m_bSuppliesConsumptionEnabled;
		m_bSuppliesConsumptionEnabled = false;
		PerformSpawn(entityEntry, user, slot);
		m_bSuppliesConsumptionEnabled = eskiSupply;
		Print(string.Format("[M360 Magaza] Arac OK %1 fiyat=%2", FilePath.StripPath(prefab), nakitFiyat), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	override SCR_EEntityRequestStatus GetRequestState(notnull SCR_EntityCatalogEntry entityEntry, IEntity user = null)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
			return super.GetRequestState(entityEntry, user);

		bool eski = m_bSuppliesConsumptionEnabled;
		m_bSuppliesConsumptionEnabled = false;
		SCR_EEntityRequestStatus st = super.GetRequestState(entityEntry, user);
		m_bSuppliesConsumptionEnabled = eski;

		if (st == SCR_EEntityRequestStatus.NOT_ENOUGH_SUPPLIES)
			st = SCR_EEntityRequestStatus.CAN_SPAWN;

		if (st != SCR_EEntityRequestStatus.CAN_SPAWN)
			return st;

		SCR_EntityCatalogSpawnerData data = SCR_EntityCatalogSpawnerData.Cast(entityEntry.GetEntityDataOfType(SCR_EntityCatalogSpawnerData));
		if (!data)
			return SCR_EEntityRequestStatus.NOT_AVAILABLE;

		int fiyat = M360_MagazaYardim.AracFiyat(GetOwner(), entityEntry.GetPrefab(), data.GetSupplyCost());
		if (fiyat < 0)
			return SCR_EEntityRequestStatus.NOT_AVAILABLE;

		if (user && !M360_IsOturumlari.NakitYeterliMi(user, fiyat))
			return SCR_EEntityRequestStatus.NOT_ENOUGH_SUPPLIES;

		return SCR_EEntityRequestStatus.CAN_SPAWN;
	}
}
