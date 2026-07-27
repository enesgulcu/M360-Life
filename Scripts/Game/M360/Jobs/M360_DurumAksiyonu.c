//------------------------------------------------------------------------------------------------
//! M360 Life — Envanter / durum aksiyonu (F menu yedek)
//------------------------------------------------------------------------------------------------
class M360_DurumAksiyonu : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (canta)
		{
			canta.EnvanterAcKapa();
			return;
		}

		M360_IsOturumVerisi veri = M360_IsOturumlari.AlVeyaOlustur(pUserEntity);
		string satir = M360_IsOturumlari.DurumMetni(veri);
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
