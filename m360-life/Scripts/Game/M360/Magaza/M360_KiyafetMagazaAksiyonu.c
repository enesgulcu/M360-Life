//------------------------------------------------------------------------------------------------
//! Kiyafet magazasini Overlay UI ile acar (istemci).
//------------------------------------------------------------------------------------------------
class M360_KiyafetMagazaAksiyonu : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_KiyafetMagazaUI.Ac();
	}

	override bool GetActionNameScript(out string outName)
	{
		if (M360_KiyafetMagazaUI.AcikMi())
			outName = "Kapat";
		else
			outName = "Kiyafet Magazasi";
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
