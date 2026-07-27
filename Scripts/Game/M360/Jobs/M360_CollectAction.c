//------------------------------------------------------------------------------------------------
//! M360 Life — Topla / Durdur aksiyonu
//------------------------------------------------------------------------------------------------
class M360_CollectAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_JobCollectSiteComponent alan = M360_JobCollectSiteComponent.Cast(
			pOwnerEntity.FindComponent(M360_JobCollectSiteComponent));
		if (alan)
			alan.ToplamaDegistir(pUserEntity);
	}

	override bool GetActionNameScript(out string outName)
	{
		outName = "Topla / Durdur";
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
