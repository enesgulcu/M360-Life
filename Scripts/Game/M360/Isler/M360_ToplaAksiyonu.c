//------------------------------------------------------------------------------------------------
//! M360 Life — Topla / Durdur aksiyonu
//------------------------------------------------------------------------------------------------
class M360_ToplaAksiyonu : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_ToplamaAlaniBileseni alan = M360_ToplamaAlaniBileseni.Cast(
			pOwnerEntity.FindComponent(M360_ToplamaAlaniBileseni));
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
