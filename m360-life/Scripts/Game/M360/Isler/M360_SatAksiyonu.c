//------------------------------------------------------------------------------------------------
//! M360 Life — Sat aksiyonu (islenmis → nakit)
//------------------------------------------------------------------------------------------------
class M360_SatAksiyonu : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_SatisNoktasiBileseni satis = M360_SatisNoktasiBileseni.Cast(
			pOwnerEntity.FindComponent(M360_SatisNoktasiBileseni));
		if (satis)
			satis.HepsiniSat(pUserEntity);
	}

	override bool GetActionNameScript(out string outName)
	{
		outName = "Sat";
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
