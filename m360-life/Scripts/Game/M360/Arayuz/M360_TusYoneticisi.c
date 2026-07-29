//------------------------------------------------------------------------------------------------
//! Tuş → işlev omurgası (ürün).
//!
//! Mimari (BI Input Manager / docs/20 §1):
//!   conf Action + CharacterMovementContext ActionRefs +  →  AddActionListener (bir kez)
//!
//! NOT (Enforce): metod parametresinde ActionListenerCallback / func YASAK
//!   ("func arguments are not supported in script methods")
//!   → AddActionListener doğrudan Kur() içinde çağrılır.
//!
//! Güvenlik: yalnız yerel istemci; callback'te para/item yok; Inventory hijack yok.
//------------------------------------------------------------------------------------------------
class M360_TusYoneticisi
{
	static const string TUS_LIFE_CANTA = "M360_LifeCanta";
	static const string TUS_SES_MODU = "M360_SesModu";
	static const string CTX_HAREKET = "CharacterMovementContext";

	protected static bool s_bKuruldu;

	//------------------------------------------------------------------------------------------------
	static void Kur()
	{
		if (s_bKuruldu)
			return;

		InputManager im = GetGame().GetInputManager();
		if (!im)
		{
			Print("[M360] TusYoneticisi: InputManager yok", LogLevel.WARNING);
			return;
		}

		int eklenen = 0;

		if (AksiyonVarMi(im, TUS_LIFE_CANTA))
		{
			im.AddActionListener(TUS_LIFE_CANTA, EActionTrigger.DOWN, OnLifeCanta);
			Print(string.Format("[M360] TusYoneticisi: dinleyici + %1", TUS_LIFE_CANTA), LogLevel.NORMAL);
			eklenen++;
		}
		else
		{
			Print(string.Format(
				"[M360] TusYoneticisi: '%1' action YOK — M360_Input.conf Action + ActionRefs + ekle",
				TUS_LIFE_CANTA), LogLevel.ERROR);
		}

		if (AksiyonVarMi(im, TUS_SES_MODU))
		{
			im.AddActionListener(TUS_SES_MODU, EActionTrigger.DOWN, OnSesModu);
			Print(string.Format("[M360] TusYoneticisi: dinleyici + %1", TUS_SES_MODU), LogLevel.NORMAL);
			eklenen++;
		}
		else
		{
			Print(string.Format(
				"[M360] TusYoneticisi: '%1' action YOK — M360_Input.conf Action + ActionRefs + ekle",
				TUS_SES_MODU), LogLevel.ERROR);
		}

		if (eklenen < 1)
		{
			Print("[M360] TusYoneticisi: hicbir M360 action dinlenemedi — conf/gproj kontrol", LogLevel.ERROR);
			return;
		}

		s_bKuruldu = true;

		string ctxStr = "hayir";
		if (im.IsContextActive(CTX_HAREKET))
			ctxStr = "evet";

		Print(string.Format(
			"[M360] TusYoneticisi OK | kayit=%1 | I=canta F2=ses | %2 aktif=%3",
			eklenen, CTX_HAREKET, ctxStr), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected static bool AksiyonVarMi(InputManager im, string aksiyonAdi)
	{
		int n = im.GetActionCount();
		for (int i = 0; i < n; i++)
		{
			if (im.GetActionName(i) == aksiyonAdi)
				return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------------------------
	static void OnLifeCanta()
	{
		Print("[M360] I → Life HUD", LogLevel.NORMAL);
		LifeCantaAcKapa();
	}

	//------------------------------------------------------------------------------------------------
	static void OnSesModu()
	{
		Print("[M360] F2 → Ses modu", LogLevel.NORMAL);
		M360_SesModuYoneticisi.AcKapa();
	}

	//------------------------------------------------------------------------------------------------
	static void LifeCantaAcKapa()
	{
		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (!canta)
		{
			Print("[M360] TusYoneticisi: CantaHud yok", LogLevel.WARNING);
			return;
		}
		canta.EnvanterAcKapa();
	}
}
