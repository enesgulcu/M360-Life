//------------------------------------------------------------------------------------------------
//! Kiyafet satin alma: istemci talep → sunucu nakit + envanter/giydirme → sonuc RPC.
//! Toplu prova alimi: prefablar '|' ile ayrilmis tek string.
//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController
{
	protected float m_fM360KiyafetSonIslem;

	//------------------------------------------------------------------------------------------------
	void M360_KiyafetSatinAlIste(ResourceName prefab)
	{
		array<ResourceName> tek = {};
		tek.Insert(prefab);
		M360_KiyafetSatinAlIsteToplu(tek);
	}

	//------------------------------------------------------------------------------------------------
	void M360_KiyafetSatinAlIsteToplu(array<ResourceName> prefabs)
	{
		if (!prefabs || prefabs.Count() < 1)
			return;

		string payload;
		for (int i = 0; i < prefabs.Count(); i++)
		{
			if (i > 0)
				payload += "|";
			payload += prefabs[i];
		}

		Rpc(M360_RpcAskKiyafetSatinAl, payload);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void M360_RpcAskKiyafetSatinAl(string prefabStr)
	{
		float now = GetGame().GetWorld().GetWorldTime();
		if (now - m_fM360KiyafetSonIslem < 1500)
		{
			Rpc(M360_RpcDoKiyafetSonucu, false, "rate_limited");
			return;
		}
		m_fM360KiyafetSonIslem = now;

		array<ResourceName> prefabs = {};
		M360_KiyafetPrefabListesiniAyikla(prefabStr, prefabs);
		if (prefabs.Count() < 1)
		{
			Rpc(M360_RpcDoKiyafetSonucu, false, "not_for_sale");
			return;
		}

		array<ref M360_KiyafetUrun> urunler = {};
		int toplam = 0;
		foreach (ResourceName prefab : prefabs)
		{
			M360_KiyafetUrun urun = M360_KiyafetKatalogu.Bul(prefab);
			if (!urun || urun.m_iFiyat < 0)
			{
				Rpc(M360_RpcDoKiyafetSonucu, false, "not_for_sale");
				return;
			}
			urunler.Insert(urun);
			toplam += urun.m_iFiyat;
		}

		IEntity karakter = GetControlledEntity();
		if (!karakter)
		{
			Rpc(M360_RpcDoKiyafetSonucu, false, "invalid_player");
			return;
		}

		if (!M360_NakitHarca(toplam))
		{
			Rpc(M360_RpcDoKiyafetSonucu, false, "insufficient_funds");
			return;
		}

		int basarili = 0;
		int iade = 0;
		foreach (M360_KiyafetUrun urun : urunler)
		{
			if (M360_KiyafetTeslimEt(karakter, urun.m_sPrefab))
			{
				basarili++;
				Print(string.Format("[M360] Kiyafet OK %1 fiyat=%2", FilePath.StripPath(urun.m_sPrefab), urun.m_iFiyat), LogLevel.NORMAL);
			}
			else
			{
				iade += urun.m_iFiyat;
			}
		}

		if (iade > 0)
			M360_NakitEkle(iade);

		if (basarili < 1)
		{
			Rpc(M360_RpcDoKiyafetSonucu, false, "spawn_failed");
			return;
		}

		Print(string.Format("[M360] Kiyafet paket OK adet=%1 toplam=%2 iade=%3 kalan=%4", basarili, toplam, iade, M360_NakitAl()), LogLevel.NORMAL);
		if (iade > 0)
			Rpc(M360_RpcDoKiyafetSonucu, true, "partial");
		else
			Rpc(M360_RpcDoKiyafetSonucu, true, "ok");
	}

	//------------------------------------------------------------------------------------------------
	protected void M360_KiyafetPrefabListesiniAyikla(string payload, out array<ResourceName> outList)
	{
		outList = {};
		if (!payload || payload.Length() < 1)
			return;

		string rest = payload;
		while (rest.Length() > 0)
		{
			int idx = rest.IndexOf("|");
			string part;
			if (idx < 0)
			{
				part = rest;
				rest = string.Empty;
			}
			else
			{
				part = rest.Substring(0, idx);
				rest = rest.Substring(idx + 1, rest.Length() - idx - 1);
			}

			if (part.Length() < 1)
				continue;

			ResourceName prefab = part;
			bool var = false;
			foreach (ResourceName mevcut : outList)
			{
				if (M360_MagazaYardim.PrefabEslesir(mevcut, prefab))
				{
					var = true;
					break;
				}
			}
			if (!var)
				outList.Insert(prefab);
		}
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void M360_RpcDoKiyafetSonucu(bool basarili, string kod)
	{
		string mesaj;
		if (basarili && kod == "partial")
			mesaj = "Bazi kiyafetler alindi. Alinamayanlarin ucreti iade edildi.";
		else if (basarili)
			mesaj = "Satin alma basarili. Secilen kiyafetler uzerinde.";
		else if (kod == "insufficient_funds")
			mesaj = "Yetersiz nakit. Ucret kesilmedi.";
		else if (kod == "spawn_failed")
			mesaj = "Kiyafet verilemedi. Ucret iade edildi.";
		else if (kod == "rate_limited")
			mesaj = "Cok hizli islem. Biraz bekle.";
		else if (kod == "not_for_sale")
			mesaj = "Bu urun satista degil.";
		else
			mesaj = "Satin alma basarisiz: " + kod;

		M360_KiyafetMagazaUI ui = M360_KiyafetMagazaUI.Aktif();
		if (ui)
			ui.SatinAlmaSonucu(basarili, mesaj);
	}

	//------------------------------------------------------------------------------------------------
	protected bool M360_KiyafetTeslimEt(IEntity karakter, ResourceName prefab)
	{
		SCR_InventoryStorageManagerComponent inv = null;
		ChimeraCharacter chim = ChimeraCharacter.Cast(karakter);
		if (chim && chim.GetCharacterController())
			inv = SCR_InventoryStorageManagerComponent.Cast(chim.GetCharacterController().GetInventoryStorageManager());
		if (!inv)
			inv = SCR_InventoryStorageManagerComponent.Cast(karakter.FindComponent(SCR_InventoryStorageManagerComponent));
		if (!inv)
		{
			Print("[M360] Kiyafet teslim hata: inventory manager yok", LogLevel.ERROR);
			return false;
		}

		// Kiyafet magazasinda once giydir; canta yalniz giydirilemeyen esya icin yedek.
		if (M360_KiyafetGiydir(karakter, prefab, inv))
			return true;

		BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(karakter.FindComponent(SCR_CharacterInventoryStorageComponent));
		if (!storage)
			storage = BaseInventoryStorageComponent.Cast(karakter.FindComponent(EquipedLoadoutStorageComponent));

		if (storage && inv.TrySpawnPrefabToStorage(prefab, storage))
			return true;

		Resource res = Resource.Load(prefab);
		if (!res.IsValid())
		{
			Print("[M360] Kiyafet teslim hata: resource gecersiz " + prefab, LogLevel.ERROR);
			return false;
		}

		IEntity item = GetGame().SpawnEntityPrefab(res, karakter.GetWorld());
		if (!item)
		{
			Print("[M360] Kiyafet teslim hata: prefab spawn olmadi " + prefab, LogLevel.ERROR);
			return false;
		}

		if (inv.TryInsertItem(item))
			return true;

		delete item;
		Print("[M360] Kiyafet teslim hata: giydirme ve envanter reddetti " + prefab, LogLevel.ERROR);
		return false;
	}

	//------------------------------------------------------------------------------------------------
	protected bool M360_KiyafetGiydir(IEntity karakter, ResourceName prefab, SCR_InventoryStorageManagerComponent inv)
	{
		EquipedLoadoutStorageComponent loadout = EquipedLoadoutStorageComponent.Cast(karakter.FindComponent(EquipedLoadoutStorageComponent));
		if (!loadout)
		{
			Print("[M360] Kiyafet giydir hata: loadout storage yok", LogLevel.ERROR);
			return false;
		}

		Resource res = Resource.Load(prefab);
		if (!res.IsValid())
		{
			Print("[M360] Kiyafet giydir hata: resource gecersiz " + prefab, LogLevel.ERROR);
			return false;
		}

		IEntity cloth = GetGame().SpawnEntityPrefab(res, karakter.GetWorld());
		if (!cloth)
		{
			Print("[M360] Kiyafet giydir hata: spawn olmadi " + prefab, LogLevel.ERROR);
			return false;
		}

		BaseLoadoutClothComponent clothComp = BaseLoadoutClothComponent.Cast(cloth.FindComponent(BaseLoadoutClothComponent));
		if (!clothComp)
		{
			Print("[M360] Kiyafet giydir: cloth component yok, cantaya deneniyor", LogLevel.WARNING);
			if (!inv.TryInsertItem(cloth))
			{
				delete cloth;
				return false;
			}
			return true;
		}

		LoadoutAreaType area = clothComp.GetAreaType();
		if (!area)
		{
			Print("[M360] Kiyafet giydir: area yok, cantaya deneniyor", LogLevel.WARNING);
			if (!inv.TryInsertItem(cloth))
			{
				delete cloth;
				return false;
			}
			return true;
		}

		LoadoutSlotInfo slotInfo = loadout.GetSlotFromArea(area.Type());
		if (!slotInfo)
		{
			Print(string.Format("[M360] Kiyafet giydir: uygun slot yok area=%1", area.Type()), LogLevel.WARNING);
			if (!inv.TryInsertItem(cloth))
			{
				delete cloth;
				return false;
			}
			return true;
		}

		IEntity eski = slotInfo.GetAttachedEntity();
		if (eski)
		{
			// Takili urun once slottan ayrilmadan canta islemi daima reddedilir.
			slotInfo.DetachEntity(false);
			if (!inv.TryInsertItem(eski))
			{
				// Canta doluysa eski parcayi oyuncunun ayagina birak; satin alma yine tamamlanir.
				eski.SetOrigin(karakter.GetOrigin());
			}
		}

		InventoryStorageSlot slot = InventoryStorageSlot.Cast(slotInfo);
		slotInfo.AttachEntity(cloth);
		if (slotInfo.GetAttachedEntity() != cloth && slot)
			slot.AttachEntity(cloth);

		if (slotInfo.GetAttachedEntity() != cloth)
		{
			Print("[M360] Kiyafet giydir hata: slot yeni parcayi kabul etmedi", LogLevel.ERROR);
			delete cloth;
			return false;
		}

		Print(string.Format("[M360] Kiyafet giydir OK area=%1 prefab=%2", area.Type(), FilePath.StripPath(prefab)), LogLevel.NORMAL);
		return true;
	}
}
