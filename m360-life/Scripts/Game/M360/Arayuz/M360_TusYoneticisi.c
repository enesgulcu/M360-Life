//------------------------------------------------------------------------------------------------
//! Tuş → işlev omurgası (ürün).
//!
//! Mimari (BI Input Manager):
//!   conf Action + CharacterMovementContext ActionRefs +  →  AddActionListener (bir kez)
//!
//! Güvenlik (docs/19 + docs/20):
//!   - Yalnızca YEREL istemci (UpdateLocalPlayerController kapısı)
//!   - Callback = UI aç/kapa veya sunucuya TALEP — para/item/rol ASLA burada yazılmaz
//!   - Inventory / Tab / vanilla action hijack YASAK
//!
//! Yeni tuş (3 adım):
//!   1) M360_Input.conf → Action + ActionRefs +
//!   2) Asagida DinleyiciEkle + callback
//!   3) Log kanıtı
//------------------------------------------------------------------------------------------------
class M360_TusYoneticisi
{
	static const string TUS_LIFE_CANTA = "M360_LifeCanta";
	static const string CTX_HAREKET = "CharacterMovementContext";

	protected static bool s_bKuruldu;

	//------------------------------------------------------------------------------------------------
	//! Yerel oyuncu controller hazır olunca bir kez. Tekrar çağrı güvenli (no-op).
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
		if (DinleyiciEkle(im, TUS_LIFE_CANTA, OnLifeCanta))
			eklenen++;

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
			"[M360] TusYoneticisi OK | kayit=%1 | %2 aktif=%3 | Tab=vanilla | yetki=yalniz UI",
			eklenen, CTX_HAREKET, ctxStr), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! Yeni tuş kaydı burada toplanır (çift kayıt / eksik action yakalar).
	protected static bool DinleyiciEkle(InputManager im, string aksiyonAdi, ActionListenerCallback callback)
	{
		if (!im || !aksiyonAdi || aksiyonAdi == string.Empty)
			return false;

		if (!AksiyonVarMi(im, aksiyonAdi))
		{
			Print(string.Format(
				"[M360] TusYoneticisi: '%1' action YOK — M360_Input.conf Action + ActionRefs + ekle",
				aksiyonAdi), LogLevel.ERROR);
			return false;
		}

		im.AddActionListener(aksiyonAdi, EActionTrigger.DOWN, callback);
		Print(string.Format("[M360] TusYoneticisi: dinleyici + %1", aksiyonAdi), LogLevel.NORMAL);
		return true;
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
	//! I → Life HUD (yalnız görsel; ekonomi yok)
	static void OnLifeCanta()
	{
		Print("[M360] I → Life HUD", LogLevel.NORMAL);
		LifeCantaAcKapa();
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
