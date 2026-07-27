//------------------------------------------------------------------------------------------------
//! M360 Life — Isle (parti) aksiyonu
//------------------------------------------------------------------------------------------------
class M360_ProcessAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_JobProcessSiteComponent makine = M360_JobProcessSiteComponent.Cast(
			pOwnerEntity.FindComponent(M360_JobProcessSiteComponent));
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
