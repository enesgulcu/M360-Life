//------------------------------------------------------------------------------------------------
//! F menusu: supply / 0 supply engelini atla, M360 nakit goster.
//------------------------------------------------------------------------------------------------
modded class SCR_CatalogSpawnerUserAction
{
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
			return super.GetActionNameScript(outName);

		if (!m_EntitySpawner || !m_EntitySpawnerData || !m_EntityData)
			return false;

		ActionNameParams[0] = m_EntitySpawnerData.GetOverwriteName();
		if (ActionNameParams[0] == string.Empty)
			ActionNameParams[0] = m_EntityData.GetEntityName();

		int fiyat = M360_MagazaYardim.AracFiyat(m_EntitySpawner.GetOwner(), m_EntityData.GetPrefab(), m_EntitySpawnerData.GetSupplyCost());
		IEntity localChar = SCR_PlayerController.GetLocalControlledEntity();
		int bakiye = 0;
		SCR_PlayerController pc = SCR_PlayerController.M360_ControllerBul(localChar);
		if (pc)
			bakiye = pc.M360_NakitAl();

		ActionNameParams[1] = fiyat.ToString();
		ActionNameParams[2] = bakiye.ToString();
		outName = "#AR-EntitySpawner_Request";
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
			return super.CanBePerformedScript(user);

		if (!m_EntitySpawner || !m_EntityData)
			return false;

		m_iRequestStatus = m_EntitySpawner.GetRequestState(m_EntityData, user);

		if (m_iRequestStatus == SCR_EEntityRequestStatus.CAN_SPAWN)
		{
			if (IsAnySlotAvailable())
				return true;

			SetCannotPerformReason("#AR-EntitySpawner_NoSlot");
			return false;
		}

		if (m_iRequestStatus == SCR_EEntityRequestStatus.NOT_ENOUGH_SUPPLIES)
		{
			SetCannotPerformReason("Yetersiz nakit");
			return false;
		}

		if (m_iRequestStatus == SCR_EEntityRequestStatus.NOT_AVAILABLE)
		{
			SetCannotPerformReason("#AR-EntitySpawner_NotAvailable");
			return false;
		}

		if (m_iRequestStatus == SCR_EEntityRequestStatus.COOLDOWN)
		{
			SetCannotPerformReason("#AR-EntitySpawner_Cooldown");
			return false;
		}

		SetCannotPerformReason("#AR-EntitySpawner_NotAvailable");
		return false;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!M360_MagazaYardim.LabNakitAktif())
			return super.CanBeShownScript(user);

		if (!m_EntitySpawner || !m_EntityData || !m_EntitySpawnerData)
			return false;

		SCR_ChimeraCharacter chimeraCharacter = SCR_ChimeraCharacter.Cast(user);
		if (!chimeraCharacter)
			return false;

		Faction spawnerFaction = m_EntitySpawner.GetFaction();
		Faction playerFaction = chimeraCharacter.GetFaction();
		// NoAffiliation: spawnerFaction null olabilir — engelleme
		if (spawnerFaction && playerFaction && spawnerFaction != playerFaction)
			return false;

		m_iRequestStatus = m_EntitySpawner.GetRequestState(m_EntityData, user);
		return m_iRequestStatus != SCR_EEntityRequestStatus.NOT_AVAILABLE;
	}
}
