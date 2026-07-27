//------------------------------------------------------------------------------------------------
//! M360 Life — Satis noktasi (Dokuman 5.1b Asama 4)
//! Islenmis urunu aninda nakde cevirir (lab cuzdan sayac).
//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "M360/Isler", description: "Satis noktasi")]
class M360_SatisNoktasiBileseniClass : ScriptComponentClass
{
}

class M360_SatisNoktasiBileseni : ScriptComponent
{
	[Attribute(desc: "Is ayarlari")]
	ref M360_IsAyar m_Ayar;

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		if (!m_Ayar)
		{
			Print("[M360] SatisNoktasi: m_Ayar eksik", LogLevel.ERROR);
			return;
		}

		BaseWorld dunya = owner.GetWorld();
		if (dunya && dunya.IsEditMode())
			return;

		Print(string.Format("[M360] SatisNoktasi hazir: %1 @ %2/birim", m_Ayar.m_sIsAdi, m_Ayar.m_iSatisFiyati), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	void HepsiniSat(IEntity kullanici)
	{
		if (!m_Ayar || !kullanici)
			return;

		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(kullanici);
		if (oturum.m_bIsliyor)
		{
			MesajGoster("Isleme bitmeden satilamaz");
			return;
		}

		if (oturum.m_iIslenmis <= 0)
		{
			MesajGoster("Satilacak islenmis urun yok");
			return;
		}

		int miktar = oturum.m_iIslenmis;
		int birimFiyat = m_Ayar.m_iSatisFiyati;
		if (birimFiyat < 0)
			birimFiyat = 0;

		int gelir = miktar * birimFiyat;
		oturum.m_iIslenmis = 0;
		oturum.m_iNakit += gelir;
		oturum.m_sIsAdi = m_Ayar.m_sIsAdi;

		MesajGoster(string.Format("Satildi: %1 x %2 = +%3  |  Nakit=%4", miktar, birimFiyat, gelir, oturum.m_iNakit));
		Print(string.Format("[M360] Satis: miktar=%1 fiyat=%2 gelir=%3 nakit=%4", miktar, birimFiyat, gelir, oturum.m_iNakit), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	bool SatilabilirMi(IEntity kullanici)
	{
		M360_IsOturumVerisi oturum = M360_IsOturumlari.AlVeyaOlustur(kullanici);
		return oturum.m_iIslenmis > 0 && !oturum.m_bIsliyor;
	}

	//------------------------------------------------------------------------------------------------
	protected void MesajGoster(string mesaj)
	{
		SCR_HintManagerComponent.ShowCustomHint(mesaj, "M360 Satis", 5);
	}
}
