//------------------------------------------------------------------------------------------------
//! F2 ses modu — gürültüyü yumuşak kıs, oyuncu konuşmasını (VoiceChat) koru.
//!
//! GERCEK KAYNAK: Engine UserSettings → AudioSettings (Options ile ayni).
//! Olcek: Volume* alanlari 0..100 (int). AudioSystem.Get/SetMasterVolume 0..1.
//!
//! BUG (2026-07-29): SetMasterVolume'a 0..100 deger vermek (ornegin 75) API'de
//! 1.0'a clamp olup ayar dusumunu iptal ediyordu — ikon calisir, ses hic dusmezdi.
//!
//! VoiceChat / VolumeVoiceChat'e DOKUNULMAZ. SaveUserSettings YOK.
//------------------------------------------------------------------------------------------------
class M360_SesModuYoneticisi
{
	static const float HEDEF_CARPAN = 0.15;
	static const float GECIS_SN = 0.45;
	static const float IKON_GECIS_SN = 0.12;

	protected static bool s_bSessizMod;
	protected static bool s_bGecis;
	protected static bool s_bKayitVar;
	protected static bool s_bDogrulamaBekliyor;
	protected static bool s_bLabYapildi;

	// Kayit = oyuncunun gercek ayar degeri 0..100 (F2 oncesi)
	protected static float s_fMasterKayit;
	protected static float s_fSfxKayit;
	protected static float s_fMusicKayit;
	protected static float s_fDialogKayit;

	protected static float s_fMasterSimdi;
	protected static float s_fSfxSimdi;
	protected static float s_fMusicSimdi;
	protected static float s_fDialogSimdi;

	protected static float s_fMasterHedef;
	protected static float s_fSfxHedef;
	protected static float s_fMusicHedef;
	protected static float s_fDialogHedef;

	protected static float s_fRozetAlpha;

	//------------------------------------------------------------------------------------------------
	static void HudHazirla()
	{
		// Lab tetikleyici CantaHud.Baslat → CallLater(SesLabOlcum)
	}

	//------------------------------------------------------------------------------------------------
	//! Ayni uygula yolu ile SFX'i gecici %50 yapip GetMaster kontrolu; sonra geri al.
	static void LaboratuvarOlcum()
	{
		if (s_bLabYapildi)
			return;
		s_bLabYapildi = true;

		if (s_bSessizMod || s_bGecis)
		{
			Print("[M360] F2 SES LAB SKIP | sessiz mod acik", LogLevel.NORMAL);
			return;
		}

		BaseContainer audio = AudioAyarAl();
		if (!audio)
		{
			Print("[M360] F2 SES LAB FAIL | AudioSettings yok", LogLevel.WARNING);
			return;
		}

		float m0 = FloatOku(audio, "Volume", 100);
		float s0 = FloatOku(audio, "VolumeSfx", 100);
		float u0 = FloatOku(audio, "VolumeMusic", 100);
		float d0 = FloatOku(audio, "VolumeDialog", 100);
		float getOnce = AudioSystem.GetMasterVolume(AudioSystem.SFX);

		s_fMasterSimdi = m0 * 0.5;
		s_fSfxSimdi = s0 * 0.5;
		s_fMusicSimdi = u0 * 0.5;
		s_fDialogSimdi = d0 * 0.5;
		UygulaKanallar();

		float getYari = AudioSystem.GetMasterVolume(AudioSystem.SFX);
		float ayarYari = FloatOku(audio, "VolumeSfx", -1);
		float beklenen = (s0 * 0.5) / 100.0;

		// Geri al (oyuncu ayarina dokunma kalici)
		s_fMasterSimdi = m0;
		s_fSfxSimdi = s0;
		s_fMusicSimdi = u0;
		s_fDialogSimdi = d0;
		UygulaKanallar();

		float getSon = AudioSystem.GetMasterVolume(AudioSystem.SFX);
		bool okDusme = getYari <= beklenen + 0.08;
		bool okAyar = ayarYari <= (s0 * 0.5) + 1.0;
		bool okGeri = getSon >= getOnce - 0.08;

		if (okDusme && okAyar && okGeri)
		{
			Print(string.Format(
				"[M360] F2 SES LAB OK | once=%1 yari=%2 geri=%3 ayarYari=%4 beklenen~%5",
				getOnce, getYari, getSon, ayarYari, beklenen), LogLevel.NORMAL);
		}
		else
		{
			Print(string.Format(
				"[M360] F2 SES LAB FAIL | once=%1 yari=%2 geri=%3 ayarYari=%4 beklenen~%5 (dusme=%6 ayar=%7 geri=%8)",
				getOnce, getYari, getSon, ayarYari, beklenen, okDusme, okAyar, okGeri), LogLevel.WARNING);
		}
	}

	//------------------------------------------------------------------------------------------------
	static void AcKapa()
	{
		if (!s_bSessizMod)
			SessizModAc();
		else
			SessizModKapat();
	}

	//------------------------------------------------------------------------------------------------
	protected static void SessizModAc()
	{
		BaseContainer audio = AudioAyarAl();
		if (!audio)
		{
			Print("[M360] F2 SES | AudioSettings yok — iptal", LogLevel.WARNING);
			SCR_HintManagerComponent.ShowCustomHint(
				"Ses ayari okunamadi",
				"M360 Ses",
				3);
			return;
		}

		s_fMasterKayit = FloatOku(audio, "Volume", 100);
		s_fSfxKayit = FloatOku(audio, "VolumeSfx", 100);
		s_fMusicKayit = FloatOku(audio, "VolumeMusic", 100);
		s_fDialogKayit = FloatOku(audio, "VolumeDialog", 100);
		s_bKayitVar = true;

		s_fMasterSimdi = s_fMasterKayit;
		s_fSfxSimdi = s_fSfxKayit;
		s_fMusicSimdi = s_fMusicKayit;
		s_fDialogSimdi = s_fDialogKayit;

		s_fMasterHedef = s_fMasterKayit * HEDEF_CARPAN;
		s_fSfxHedef = s_fSfxKayit * HEDEF_CARPAN;
		s_fMusicHedef = s_fMusicKayit * HEDEF_CARPAN;
		s_fDialogHedef = s_fDialogKayit * HEDEF_CARPAN;

		s_bSessizMod = true;
		s_bGecis = true;
		s_bDogrulamaBekliyor = true;

		UygulaKanallar();

		float voip = FloatOku(audio, "VolumeVoiceChat", -1);
		Print(string.Format(
			"[M360] F2 SES KIS | ayarSFX %1->%2 | VoiceChat=%3 (dokunulmaz) | olcek=0..100→Master0..1",
			s_fSfxKayit, s_fSfxHedef, voip), LogLevel.NORMAL);

		SCR_HintManagerComponent.ShowCustomHint(
			"Oyun sesi %15 (konusma ayni) | F2 ile ac",
			"M360 Ses",
			4);
	}

	//------------------------------------------------------------------------------------------------
	protected static void SessizModKapat()
	{
		if (!s_bKayitVar)
		{
			s_bSessizMod = false;
			s_bGecis = false;
			return;
		}

		s_fMasterHedef = s_fMasterKayit;
		s_fSfxHedef = s_fSfxKayit;
		s_fMusicHedef = s_fMusicKayit;
		s_fDialogHedef = s_fDialogKayit;

		s_bSessizMod = false;
		s_bGecis = true;
		s_bDogrulamaBekliyor = false;
		UygulaKanallar();

		Print("[M360] F2 SES AC | kayitli ayarlara donus", LogLevel.NORMAL);

		SCR_HintManagerComponent.ShowCustomHint(
			"Oyun sesi normale donuyor",
			"M360 Ses",
			3);
	}

	//------------------------------------------------------------------------------------------------
	static void Tick(float timeSlice)
	{
		if (s_bGecis)
			GecisAdim(timeSlice);

		RozetGuncelle(timeSlice);
	}

	//------------------------------------------------------------------------------------------------
	protected static void GecisAdim(float timeSlice)
	{
		if (timeSlice <= 0)
			timeSlice = 0.016;

		float hiz = timeSlice / GECIS_SN;
		if (hiz > 1)
			hiz = 1;

		s_fMasterSimdi = Lerp(s_fMasterSimdi, s_fMasterHedef, hiz);
		s_fSfxSimdi = Lerp(s_fSfxSimdi, s_fSfxHedef, hiz);
		s_fMusicSimdi = Lerp(s_fMusicSimdi, s_fMusicHedef, hiz);
		s_fDialogSimdi = Lerp(s_fDialogSimdi, s_fDialogHedef, hiz);

		UygulaKanallar();

		if (Yakin(s_fMasterSimdi, s_fMasterHedef)
			&& Yakin(s_fSfxSimdi, s_fSfxHedef)
			&& Yakin(s_fMusicSimdi, s_fMusicHedef)
			&& Yakin(s_fDialogSimdi, s_fDialogHedef))
		{
			s_fMasterSimdi = s_fMasterHedef;
			s_fSfxSimdi = s_fSfxHedef;
			s_fMusicSimdi = s_fMusicHedef;
			s_fDialogSimdi = s_fDialogHedef;
			UygulaKanallar();
			s_bGecis = false;

			if (s_bDogrulamaBekliyor && s_bSessizMod)
			{
				s_bDogrulamaBekliyor = false;
				DogrulaDusme();
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static BaseContainer AudioAyarAl()
	{
		ArmaReforgerScripted game = GetGame();
		if (!game)
			return null;

		UserSettings us = game.GetEngineUserSettings();
		if (!us)
			return null;

		return us.GetModule("AudioSettings");
	}

	//------------------------------------------------------------------------------------------------
	protected static float FloatOku(notnull BaseContainer c, string ad, float varsayilan)
	{
		float v = varsayilan;
		c.Get(ad, v);
		return v;
	}

	//------------------------------------------------------------------------------------------------
	//! 0..100 ayari yaz + motora bildir. MasterVolume'a SADECE 0..1 ver.
	//! VolumeVoiceChat'e dokunma. SaveUserSettings YOK.
	protected static void UygulaKanallar()
	{
		BaseContainer audio = AudioAyarAl();
		if (!audio)
			return;

		// Alanlar int 0..100 — yuvarla ki restore drift olmasin (100→99→98)
		int master = Math.Round(s_fMasterSimdi);
		int sfx = Math.Round(s_fSfxSimdi);
		int music = Math.Round(s_fMusicSimdi);
		int dialog = Math.Round(s_fDialogSimdi);

		audio.Set("Volume", master);
		audio.Set("VolumeSfx", sfx);
		audio.Set("VolumeMusic", music);
		audio.Set("VolumeDialog", dialog);

		ArmaReforgerScripted game = GetGame();
		if (game)
			game.UserSettingsChanged();

		// 0..1 — asla 0..100 verme (clamp=1.0 olup kismi iptal eder)
		float sfx01 = sfx / 100.0;
		float music01 = music / 100.0;
		float dialog01 = dialog / 100.0;
		AudioSystem.SetMasterVolume(AudioSystem.SFX, sfx01);
		AudioSystem.SetMasterVolume(AudioSystem.Music, music01);
		AudioSystem.SetMasterVolume(AudioSystem.Dialog, dialog01);
	}

	//------------------------------------------------------------------------------------------------
	//! Gecis bitince: GetMaster ~ ayar/100 olmali. Logdan AI dogrular.
	protected static void DogrulaDusme()
	{
		BaseContainer audio = AudioAyarAl();
		float ayarSfx = -1;
		if (audio)
			ayarSfx = FloatOku(audio, "VolumeSfx", -1);

		float get01 = AudioSystem.GetMasterVolume(AudioSystem.SFX);
		float beklenen = s_fSfxHedef / 100.0;
		bool ok = (get01 <= beklenen + 0.08) && (ayarSfx <= s_fSfxHedef + 1.0);

		if (ok)
		{
			Print(string.Format(
				"[M360] F2 SES DOGRULA OK | ayarSFX=%1 GetMaster=%2 beklenen~%3",
				ayarSfx, get01, beklenen), LogLevel.NORMAL);
		}
		else
		{
			Print(string.Format(
				"[M360] F2 SES DOGRULA FAIL | ayarSFX=%1 GetMaster=%2 beklenen~%3",
				ayarSfx, get01, beklenen), LogLevel.WARNING);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static void RozetGuncelle(float timeSlice)
	{
		float hedefAlpha = 0;
		if (s_bSessizMod)
			hedefAlpha = 1;

		float hiz = timeSlice / IKON_GECIS_SN;
		if (hiz > 1)
			hiz = 1;
		if (timeSlice <= 0)
			hiz = 1;
		s_fRozetAlpha = Lerp(s_fRozetAlpha, hedefAlpha, hiz);

		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (!canta)
			return;

		M360_CekirdekHudWidgetlari cekirdek = canta.CekirdekAl();
		if (!cekirdek)
			return;

		cekirdek.SesIkonGoster(s_fRozetAlpha > 0.02, s_fRozetAlpha);
	}

	//------------------------------------------------------------------------------------------------
	protected static float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	//------------------------------------------------------------------------------------------------
	protected static bool Yakin(float a, float b)
	{
		float d = a - b;
		if (d < 0)
			d = -d;
		return d < 0.5; // int alanlar icin ~0.5 birim yeter
	}

	//------------------------------------------------------------------------------------------------
	static bool SessizMi()
	{
		return s_bSessizMod;
	}

	//------------------------------------------------------------------------------------------------
	static void YokEt()
	{
		if (s_bKayitVar && (s_bSessizMod || s_bGecis))
		{
			s_fMasterSimdi = s_fMasterKayit;
			s_fSfxSimdi = s_fSfxKayit;
			s_fMusicSimdi = s_fMusicKayit;
			s_fDialogSimdi = s_fDialogKayit;
			UygulaKanallar();
		}

		s_bSessizMod = false;
		s_bGecis = false;
		s_bDogrulamaBekliyor = false;
		s_fRozetAlpha = 0;

		M360_CantaHudBileseni canta = M360_CantaHudBileseni.Al();
		if (canta)
		{
			M360_CekirdekHudWidgetlari cekirdek = canta.CekirdekAl();
			if (cekirdek)
				cekirdek.SesIkonGoster(false, 0);
		}
	}
}
