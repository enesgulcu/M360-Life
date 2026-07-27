//------------------------------------------------------------------------------------------------
//! M360 Life — Envanter / durum aksiyonu (F menü yedek)
//------------------------------------------------------------------------------------------------
class M360_JobStatusAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_JobHudComponent hud = M360_JobHudComponent.Al();
		if (hud)
		{
			hud.EnvanterAcKapa();
			return;
		}

		M360_JobSessionData veri = M360_JobSessions.AlVeyaOlustur(pUserEntity);
		string satir = M360_JobSessions.DurumMetni(veri);
		SCR_HintManagerComponent.ShowCustomHint(satir, "M360 Durum", 5);
	}

	override bool GetActionNameScript(out string outName)
	{
		outName = "Envanter";
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
