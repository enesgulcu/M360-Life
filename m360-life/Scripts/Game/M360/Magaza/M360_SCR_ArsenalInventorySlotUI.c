//------------------------------------------------------------------------------------------------
//! Arsenal slot UI: her zaman M360 nakit (supply atla).
//------------------------------------------------------------------------------------------------
modded class SCR_ArsenalInventorySlotUI : SCR_InventorySlotUI
{
	//------------------------------------------------------------------------------------------------
	override float GetTotalResources()
	{
		if (!M360_MagazaYardim.LabNakitAktif())
			return super.GetTotalResources();

		m_fSupplyCost = 0;
		if (!m_pItem || !m_pItem.GetOwner())
			return 0;

		IEntity storageEnt = GetStorageUI().GetCurrentNavigationStorage().GetOwner();
		SCR_ArsenalComponent arsenalComponent;
		if (storageEnt)
			arsenalComponent = SCR_ArsenalComponent.Cast(storageEnt.FindComponent(SCR_ArsenalComponent));

		SCR_EntityCatalogManagerComponent entityCatalogManager = SCR_EntityCatalogManagerComponent.GetInstance();
		if (!entityCatalogManager)
			return 0;

		ResourceName prefab = m_pItem.GetOwner().GetPrefabData().GetPrefabName();
		SCR_Faction faction;
		if (arsenalComponent)
			faction = arsenalComponent.GetAssignedFaction();

		SCR_EntityCatalogEntry entry;
		if (faction)
			entry = entityCatalogManager.GetEntryWithPrefabFromFactionCatalog(EEntityCatalogType.ITEM, prefab, faction);
		else
			entry = entityCatalogManager.GetEntryWithPrefabFromCatalog(EEntityCatalogType.ITEM, prefab);

		int katalogSupply = 0;
		if (entry)
		{
			SCR_ArsenalItem data = SCR_ArsenalItem.Cast(entry.GetEntityDataOfType(SCR_ArsenalItem));
			if (data && arsenalComponent)
				katalogSupply = data.GetSupplyCost(arsenalComponent.GetSupplyCostType());
			else if (data)
				katalogSupply = data.GetSupplyCost(SCR_EArsenalSupplyCostType.DEFAULT);
		}

		int fiyat = M360_MagazaYardim.EsyaFiyat(storageEnt, prefab, katalogSupply);
		if (fiyat < 0)
		{
			m_fSupplyCost = -1;
			return m_fSupplyCost;
		}

		m_fSupplyCost = fiyat;
		return m_fSupplyCost;
	}

	//------------------------------------------------------------------------------------------------
	override void UpdateTotalResources(float totalResources)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
		{
			super.UpdateTotalResources(totalResources);
			return;
		}

		if (totalResources < 0)
		{
			// -1 = satisa kapali (liste disi)
			SetItemAvailability(false);
			if (m_CostResourceHolder)
				m_CostResourceHolder.SetVisible(false);
			return;
		}

		if (m_CostResourceHolder)
			m_CostResourceHolder.SetVisible(true);

		if (m_CostResourceHolderText)
			m_CostResourceHolderText.SetText("$" + totalResources.ToString(0));

		IEntity localChar = SCR_PlayerController.GetLocalControlledEntity();
		bool yeterli = M360_IsOturumlari.NakitYeterliMi(localChar, totalResources);
		SetItemAvailability(yeterli);
	}

	//------------------------------------------------------------------------------------------------
	override protected void CheckPersonalResources(int cost)
	{
		if (M360_MagazaYardim.LabNakitAktif())
			return;

		super.CheckPersonalResources(cost);
	}
}
