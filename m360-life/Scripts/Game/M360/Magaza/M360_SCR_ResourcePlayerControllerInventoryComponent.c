//------------------------------------------------------------------------------------------------
//! Arsenal satin alma → HER ZAMAN M360 nakit (lab). Supply yok.
//------------------------------------------------------------------------------------------------
modded class SCR_ResourcePlayerControllerInventoryComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	override protected void RpcAsk_ArsenalRequestItem_(RplId resourceComponentRplId, RplId storageComponentRplId, ResourceName resourceNameItem, EResourceType resourceType)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
		{
			super.RpcAsk_ArsenalRequestItem_(resourceComponentRplId, storageComponentRplId, resourceNameItem, resourceType);
			return;
		}

		if (!resourceComponentRplId.IsValid())
			return;

		SCR_ResourceComponent resourceComponent = SCR_ResourceComponent.Cast(Replication.FindItem(resourceComponentRplId));
		if (!resourceComponent)
			return;

		IEntity resourcesOwner = resourceComponent.GetOwner();
		if (!resourcesOwner)
			return;

		SCR_PlayerController controller = SCR_PlayerController.Cast(GetOwner());
		if (!controller)
			return;

		SCR_ChimeraCharacter buyer = SCR_ChimeraCharacter.Cast(controller.GetControlledEntity());
		if (!buyer)
			return;

		SCR_InventoryStorageManagerComponent inventoryManagerComponent = SCR_InventoryStorageManagerComponent.Cast(buyer.GetCharacterController().GetInventoryStorageManager());
		if (!inventoryManagerComponent)
			return;

		if (!storageComponentRplId.IsValid())
			return;

		BaseInventoryStorageComponent storageComponent = BaseInventoryStorageComponent.Cast(Replication.FindItem(storageComponentRplId));
		if (!storageComponent)
			return;

		if (!inventoryManagerComponent.ValidateStorageRequest(resourcesOwner))
			return;

		SCR_ArsenalComponent arsenalComponent = SCR_ArsenalComponent.FindArsenalComponent(resourcesOwner);
		if (!arsenalComponent)
			return;

		IEntity magazaEnt = arsenalComponent.GetOwner();
		if (!magazaEnt)
			magazaEnt = resourcesOwner;

		SCR_EntityCatalogManagerComponent entityCatalogManager = SCR_EntityCatalogManagerComponent.GetInstance();
		if (!entityCatalogManager)
			return;

		SCR_Faction faction = arsenalComponent.GetAssignedFaction();
		SCR_EntityCatalogEntry entry;
		if (faction)
			entry = entityCatalogManager.GetEntryWithPrefabFromFactionCatalog(EEntityCatalogType.ITEM, resourceNameItem, faction);
		else
			entry = entityCatalogManager.GetEntryWithPrefabFromCatalog(EEntityCatalogType.ITEM, resourceNameItem);

		int katalogSupply = 0;
		if (entry)
		{
			SCR_ArsenalItem data = SCR_ArsenalItem.Cast(entry.GetEntityDataOfType(SCR_ArsenalItem));
			if (data)
				katalogSupply = data.GetSupplyCost(arsenalComponent.GetSupplyCostType());
		}

		int nakitFiyat = M360_MagazaYardim.EsyaFiyat(magazaEnt, resourceNameItem, katalogSupply);
		if (nakitFiyat < 0)
		{
			Print(string.Format("[M360 Magaza] Esya satisa kapali: %1", resourceNameItem), LogLevel.WARNING);
			return;
		}

		if (!M360_IsOturumlari.NakitHarca(buyer, nakitFiyat))
		{
			Print(string.Format("[M360 Magaza] Yetersiz nakit (gerekli %1, bakiye=%2)", nakitFiyat, controller.M360_NakitAl()), LogLevel.WARNING);
			return;
		}

		if (!inventoryManagerComponent.TrySpawnPrefabToStorage(resourceNameItem, storageComponent, cb: new SCR_PrefabSpawnCallback(storageComponent)))
		{
			M360_IsOturumlari.NakitEkle(buyer, nakitFiyat);
			Print("[M360 Magaza] Spawn basarisiz — nakit iade", LogLevel.WARNING);
			return;
		}

		Print(string.Format("[M360 Magaza] Arsenal OK %1 fiyat=%2 kalan=%3", FilePath.StripPath(resourceNameItem), nakitFiyat, controller.M360_NakitAl()), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	override protected void RpcAsk_ArsenalRefundItem_(RplId resourceComponentRplId, RplId inventoryItemRplId, EResourceType resourceType)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
		{
			super.RpcAsk_ArsenalRefundItem_(resourceComponentRplId, inventoryItemRplId, resourceType);
			return;
		}

		if (!inventoryItemRplId.IsValid() || !resourceComponentRplId.IsValid())
			return;

		InventoryItemComponent inventoryItemComponent = InventoryItemComponent.Cast(Replication.FindItem(inventoryItemRplId));
		SCR_ResourceComponent resourceComponent = SCR_ResourceComponent.Cast(Replication.FindItem(resourceComponentRplId));
		if (!inventoryItemComponent || !resourceComponent)
			return;

		IEntity itemEnt = inventoryItemComponent.GetOwner();
		IEntity resourcesOwner = resourceComponent.GetOwner();
		if (!itemEnt || !resourcesOwner)
			return;

		EntityPrefabData prefabData = itemEnt.GetPrefabData();
		if (!prefabData)
			return;

		ResourceName prefab = prefabData.GetPrefabName();
		SCR_ArsenalComponent arsenalComponent = SCR_ArsenalComponent.FindArsenalComponent(resourcesOwner);
		IEntity magazaEnt = resourcesOwner;
		int katalogSupply = 0;
		if (arsenalComponent)
		{
			magazaEnt = arsenalComponent.GetOwner();
			if (!magazaEnt)
				magazaEnt = resourcesOwner;
			katalogSupply = SCR_ArsenalManagerComponent.GetItemRefundAmount(itemEnt, arsenalComponent, false);
		}

		int nakit = M360_MagazaYardim.EsyaFiyat(magazaEnt, prefab, katalogSupply);
		super.RpcAsk_ArsenalRefundItem_(resourceComponentRplId, inventoryItemRplId, resourceType);

		SCR_PlayerController controller = SCR_PlayerController.Cast(GetOwner());
		if (controller && nakit > 0)
		{
			IEntity buyer = controller.GetControlledEntity();
			if (buyer)
				M360_IsOturumlari.NakitEkle(buyer, nakit);
		}
	}
}
