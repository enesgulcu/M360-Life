//------------------------------------------------------------------------------------------------
//! M360 Life — Isle (parti) aksiyonu
//------------------------------------------------------------------------------------------------
class M360_IsleAksiyonu : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_IslemeMakinesiBileseni makine = M360_IslemeMakinesiBileseni.Cast(
			pOwnerEntity.FindComponent(M360_IslemeMakinesiBileseni));
		if (makine)
			makine.IslemeBaslat(pUserEntity);
	}

	override bool GetActionNameScript(out string outName)
	{
		outName = "Isle";
		return true;
	}

	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
}
