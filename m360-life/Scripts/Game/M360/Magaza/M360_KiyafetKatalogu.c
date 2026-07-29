//------------------------------------------------------------------------------------------------
//! Vergys sivil kiyafet katalogu. Fiyat + kategori sunucu dogrulamasi icin.
//------------------------------------------------------------------------------------------------
class M360_KiyafetUrun
{
	ResourceName m_sPrefab;
	string m_sAd;
	string m_sKategori;
	int m_iFiyat;

	void M360_KiyafetUrun(ResourceName prefab, string ad, string kategori, int fiyat)
	{
		m_sPrefab = prefab;
		m_sAd = ad;
		m_sKategori = kategori;
		m_iFiyat = fiyat;
	}
}

//------------------------------------------------------------------------------------------------
class M360_KiyafetKatalogu
{
	protected static ref array<ref M360_KiyafetUrun> s_aUrunler;

	//------------------------------------------------------------------------------------------------
	protected static void Kur()
	{
		if (s_aUrunler)
			return;

		s_aUrunler = {};

		// Giyilebilir wrapper prefablar. *_item.et yollari yalniz PreviewModel'dir.
		Ekle("{B19593D71C07E74E}Prefabs/Characters/Uniforms/Tops/tank_top_white.et", "Atlet - Beyaz", "Ust", 250);
		Ekle("{79759125D97EA784}Prefabs/Characters/Uniforms/Tops/tank_top_black.et", "Atlet - Siyah", "Ust", 250);
		Ekle("{F84D2BD833170642}Prefabs/Characters/Uniforms/Tops/Jacket_grey_white.et", "Ceket - Gri Beyaz", "Ust", 750);
		Ekle("{30AD292AF66E4688}Prefabs/Characters/Uniforms/Tops/Jacket_grey_black.et", "Ceket - Gri Siyah", "Ust", 750);
		Ekle("{540A88DB01A45F27}Prefabs/Characters/Uniforms/Tops/Jacket_blue_white.et", "Ceket - Mavi Beyaz", "Ust", 750);
		Ekle("{E27EEA1200CC8FBF}Prefabs/Characters/Uniforms/Tops/Jacket_black_grey.et", "Ceket - Siyah Gri", "Ust", 750);
		Ekle("{C575E1839794C881}Prefabs/Characters/Uniforms/Tops/Jacket_black_red.et", "Ceket - Siyah Kirmizi", "Ust", 750);
		Ekle("{38B5999E61BEFEC0}Prefabs/Characters/Uniforms/Tops/Jacket_black_blue.et", "Ceket - Siyah Mavi", "Ust", 750);
		Ekle("{9CA812203C532DD4}Prefabs/Characters/Uniforms/Tops/Jacket_green_black.et", "Ceket - Yesil Siyah", "Ust", 750);
		Ekle("{494706283AF39DFA}Prefabs/Characters/Uniforms/Tops/flannel_brown.et", "Flanel - Kahve", "Ust", 550);
		Ekle("{BF2B7ED6C6BF0422}Prefabs/Characters/Uniforms/Tops/flannel_dark_red.et", "Flanel - Koyu Kirmizi", "Ust", 550);
		Ekle("{5F310D795D4ED30C}Prefabs/Characters/Uniforms/Tops/flannel_blue.et", "Flanel - Mavi", "Ust", 550);
		Ekle("{48DE2B7B0C2F73FA}Prefabs/Characters/Uniforms/Tops/flannel_green.et", "Flanel - Yesil", "Ust", 550);
		Ekle("{CD698AE87A600148}Prefabs/Characters/Uniforms/Tops/OTW_top_CADPAT_arid.et", "OTW Ust - CADPAT Arid", "Ust", 900);
		Ekle("{DEA7F15207EC05AC}Prefabs/Characters/Uniforms/Tops/OTW_top_CADPAT_MT.et", "OTW Ust - CADPAT MT", "Ust", 900);
		Ekle("{443F07B7AEF105AD}Prefabs/Characters/Uniforms/Tops/OTW_TOP_CADPAT_TW.et", "OTW Ust - CADPAT TW", "Ust", 900);
		Ekle("{1029F3041529B6AF}Prefabs/Characters/Uniforms/Tops/OTW_top_MARPAT_arid.et", "OTW Ust - MARPAT Arid", "Ust", 900);
		Ekle("{6ADBA8FF1EFABC6F}Prefabs/Characters/Uniforms/Tops/OTW_top_MARPAT_TW.et", "OTW Ust - MARPAT TW", "Ust", 900);
		Ekle("{6CE193F2080A9D5D}Prefabs/Characters/Uniforms/Tops/OTW_top_Multicam.et", "OTW Ust - Multicam", "Ust", 900);
		Ekle("{76576663BBF3052D}Prefabs/Characters/Uniforms/Tops/OTW_top_Multicam_black.et", "OTW Ust - Multicam Siyah", "Ust", 900);
		Ekle("{5FCBAE20A9F9BFC7}Prefabs/Characters/Uniforms/Tops/OTW_top_Multicam_tropic.et", "OTW Ust - Multicam Tropik", "Ust", 900);
		Ekle("{EECF959C23E124BF}Prefabs/Characters/Uniforms/Tops/OTW_top_OCP.et", "OTW Ust - OCP", "Ust", 900);
		Ekle("{7DC290D7C063D773}Prefabs/Characters/Uniforms/Police/Police_shirt.et", "Polis Gomlek", "Ust", 600);
		Ekle("{1E5570C022A17245}Prefabs/Characters/Uniforms/Police/RCMP_shirt.et", "RCMP Gomlek", "Ust", 600);
		Ekle("{7AE54849E5C06E4F}Prefabs/Characters/Uniforms/Police/Sheriff_shirt.et", "Serif Gomlek", "Ust", 600);
		Ekle("{BFE29E7AC2E942F1}Prefabs/Characters/Uniforms/Tops/short_sleeve.et", "Tisort", "Ust", 350);
		Ekle("{AC1AED3B4C3BC7F0}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_babyblue.et", "Tisort - Acik Mavi", "Ust", 350);
		Ekle("{9079C72201226C0D}Prefabs/Characters/Uniforms/Tops/short_sleeve_white.et", "Tisort - Beyaz", "Ust", 350);
		Ekle("{E4BABD401DC80E02}Prefabs/Characters/Uniforms/Tops/short_sleeve_cg.et", "Tisort - CG", "Ust", 350);
		Ekle("{710019785C1676B3}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_IMEF.et", "Tisort - IMEF", "Ust", 350);
		Ekle("{EFCBD7C61269BB9D}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_blue.et", "Tisort - Mavi", "Ust", 350);
		Ekle("{945553110A0B7AE7}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_meme.et", "Tisort - Meme", "Ust", 350);
		Ekle("{4A82EF076DD8246D}Prefabs/Characters/Uniforms/Tops/short_sleeve_pink.et", "Tisort - Pembe", "Ust", 350);
		Ekle("{E8510F70E8DF3FAF}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_black.et", "Tisort - Siyah", "Ust", 350);
		Ekle("{1696E895046EF1A5}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_orange.et", "Tisort - Turuncu", "Ust", 350);
		Ekle("{5BA66581CD91B40E}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_FU_red.et", "Tisort FU - Kirmizi", "Ust", 350);
		Ekle("{5A91C2225BB3E6D5}Prefabs/Characters/Uniforms/Tops/short_sleeve_v2_FU_black.et", "Tisort FU - Siyah", "Ust", 350);
		Ekle("{E73631D27F62EA2C}Prefabs/Characters/Uniforms/Tops/Long_sleeve_white.et", "Uzun Kollu - Beyaz", "Ust", 450);
		Ekle("{AF103B1A0ACF85B0}Prefabs/Characters/Uniforms/Tops/Long_sleeve_red.et", "Uzun Kollu - Kirmizi", "Ust", 450);
		Ekle("{2FD63320BA1BAAE6}Prefabs/Characters/Uniforms/Tops/Long_sleeve_black.et", "Uzun Kollu - Siyah", "Ust", 450);
		Ekle("{44873D0DD7D40A3E}Prefabs/Characters/Uniforms/Pants/joggers_pants_grey.et", "Esofman - Gri", "Alt", 400);
		Ekle("{9E4C4E81B6A67B41}Prefabs/Characters/Uniforms/Pants/joggers_pants_blue.et", "Esofman - Mavi", "Alt", 400);
		Ekle("{5D7A222AD5BB7A22}Prefabs/Characters/Uniforms/Pants/joggers_pants_black.et", "Esofman - Siyah", "Alt", 400);
		Ekle("{5B92D5AEDA335D8B}Prefabs/Characters/Uniforms/Pants/joggers_pants_green.et", "Esofman - Yesil", "Alt", 400);
		Ekle("{E38958AE4AE55A02}Prefabs/Characters/Uniforms/Pants/jeans_light_blue_washed.et", "Jean - Acik Mavi", "Alt", 600);
		Ekle("{5BC452D2DB3E2573}Prefabs/Characters/Uniforms/Pants/jeans_lightblue_washed_ripped.et", "Jean - Acik Mavi Yirtik", "Alt", 600);
		Ekle("{D92493BFD86B23C3}Prefabs/Characters/Uniforms/Pants/jeans_brown_dirty.et", "Jean - Kirli Kahve", "Alt", 600);
		Ekle("{5FADEE769AFC55E7}Prefabs/Characters/Uniforms/Pants/jeans_blue.et", "Jean - Mavi", "Alt", 600);
		Ekle("{4CE818FF4A86E19D}Prefabs/Characters/Uniforms/Pants/jeans_blue_washed.et", "Jean - Mavi Yikanmis", "Alt", 600);
		Ekle("{D2D5D338B121BF53}Prefabs/Characters/Uniforms/Pants/jeans_black.et", "Jean - Siyah", "Alt", 600);
		Ekle("{785F4A26DB826D34}Prefabs/Characters/Uniforms/Pants/jeans_black_washed.et", "Jean - Yikanmis Siyah", "Alt", 600);
		Ekle("{D5E273BB88B47524}Prefabs/Characters/Uniforms/Pants/jeans_black_washed_ripped.et", "Jean - Yirtik Siyah", "Alt", 600);
		Ekle("{80F19C72CFA4CED4}Prefabs/Characters/Uniforms/Pants/cargo_pants_grey.et", "Kargo - Gri", "Alt", 550);
		Ekle("{432A36533B42C11D}Prefabs/Characters/Uniforms/Pants/cargo_pants_grey_black.et", "Kargo - Gri Siyah", "Alt", 550);
		Ekle("{BB8B4C27985C8797}Prefabs/Characters/Uniforms/Pants/cargo_pants_khaki.et", "Kargo - Haki", "Alt", 550);
		Ekle("{0C78AD2BF86D089B}Prefabs/Characters/Uniforms/Pants/cargo_pants_khaki_black.et", "Kargo - Haki Siyah", "Alt", 550);
		Ekle("{924CAA6171029E00}Prefabs/Characters/Uniforms/Pants/cargo_pants_Olive_brown.et", "Kargo - Olive Kahve", "Alt", 550);
		Ekle("{3DD0EE50596260FD}Prefabs/Characters/Uniforms/Pants/OTW_pants_CADPAT_arid.et", "OTW Alt - CADPAT Arid", "Alt", 900);
		Ekle("{E55BF69DF73F459C}Prefabs/Characters/Uniforms/Pants/OTW_pants_CADPAT_MT.et", "OTW Alt - CADPAT MT", "Alt", 900);
		Ekle("{7FC300785E22459D}Prefabs/Characters/Uniforms/Pants/OTW_pants_CADPAT_TW.et", "OTW Alt - CADPAT TW", "Alt", 900);
		Ekle("{E09097BC362BD71A}Prefabs/Characters/Uniforms/Pants/OTW_pants_MARPAT_arid.et", "OTW Alt - MARPAT Arid", "Alt", 900);
		Ekle("{5127AF30EE29FC5F}Prefabs/Characters/Uniforms/Pants/OTW_pants_MARPAT_TW.et", "OTW Alt - MARPAT TW", "Alt", 900);
		Ekle("{78F561EB7D64ED74}Prefabs/Characters/Uniforms/Pants/OTW_pants_Multicam.et", "OTW Alt - Multicam", "Alt", 900);
		Ekle("{A2584D908A3E7E44}Prefabs/Characters/Uniforms/Pants/OTW_pants_Multicam_black.et", "OTW Alt - Multicam Siyah", "Alt", 900);
		Ekle("{5311A47E6D7FEA15}Prefabs/Characters/Uniforms/Pants/OTW_pants_Multicam_tropic.et", "OTW Alt - Multicam Tropik", "Alt", 900);
		Ekle("{2BA5AAEB00E98AD4}Prefabs/Characters/Uniforms/Pants/OTW_pants_OCP.et", "OTW Alt - OCP", "Alt", 900);
		Ekle("{FE4EC122083D9690}Prefabs/Characters/Uniforms/Police/police_pants.et", "Polis Pantolon", "Alt", 650);
		Ekle("{9DD92135EAFF33A6}Prefabs/Characters/Uniforms/Police/RCMP_pants.et", "RCMP Pantolon", "Alt", 650);
		Ekle("{F96919BC2D9E2FAC}Prefabs/Characters/Uniforms/Police/sheriff_pants.et", "Serif Pantolon", "Alt", 650);
		Ekle("{09C3F4DEB275EDDF}Prefabs/Characters/Uniforms/Pants/shorts_floral_v2.et", "Sort - Cicekli", "Alt", 300);
		Ekle("{621052188FC33532}Prefabs/Characters/Uniforms/Pants/shorts_red.et", "Sort - Kirmizi", "Alt", 300);
		Ekle("{E9A452C09ECE85B9}Prefabs/Characters/Uniforms/Pants/shorts_blue.et", "Sort - Mavi", "Alt", 300);
		Ekle("{4B06AC6E12779A56}Prefabs/Characters/Uniforms/Pants/shorts_black.et", "Sort - Siyah", "Alt", 300);
		Ekle("{2BF8E4D8AE840BA4}Prefabs/Characters/Uniforms/Pants/shorts_black_blue_v2.et", "Sort - Siyah Mavi", "Alt", 300);
		Ekle("{12D833F5944A7473}Prefabs/Characters/Uniforms/Pants/shorts_watercolor_v2.et", "Sort - Suluboya", "Alt", 300);
		Ekle("{DCED0313794BD331}Prefabs/Characters/Uniforms/Pants/shorts_green_v2.et", "Sort - Yesil", "Alt", 300);
		Ekle("{4DEE5BEA1DFFBDFF}Prefabs/Characters/Uniforms/Pants/shorts_green.et", "Sort - Yesil Klasik", "Alt", 300);
		Ekle("{A9842546F979348A}Prefabs/Characters/Footwear/Air_one.et", "Air One", "Ayakkabi", 500);
		Ekle("{87D1C743A71EF457}Prefabs/Characters/Footwear/US_Boot_test.et", "Bot - Coyote", "Ayakkabi", 750);
		Ekle("{9AC4B50676BCAFA4}Prefabs/Characters/Footwear/Sandles.et", "Sandalet", "Ayakkabi", 300);
		Ekle("{127D5272DC75C3EA}Prefabs/Characters/Footwear/verg_ones_white&black.et", "Spor - Beyaz Siyah", "Ayakkabi", 500);
		Ekle("{96F0E784D2480AC1}Prefabs/Characters/Footwear/verg_ones_red.et", "Spor - Kirmizi", "Ayakkabi", 500);
		Ekle("{A5AC18C1DA89F35C}Prefabs/Characters/Footwear/verg_ones_black.et", "Spor - Siyah", "Ayakkabi", 500);
		Ekle("{3BAAA3D28DECC868}Prefabs/Characters/Footwear/verg_ones_black&white.et", "Spor - Siyah Beyaz", "Ayakkabi", 500);
		Ekle("{58CDE15D74295DA2}Prefabs/Characters/Footwear/hiking_boots.et", "Yuruyus Botu", "Ayakkabi", 750);
		Ekle("{BD81749D5977942E}Prefabs/Characters/HeadGear/TruckerCap/gas_mask.et", "Gaz Maskesi", "Sapka", 900);
		Ekle("{2760B5074A83E5AC}Prefabs/Characters/HeadGear/Police/police_cap.et", "Polis Sapka", "Sapka", 400);
		Ekle("{6FEE3B821FE70B2C}Prefabs/Characters/HeadGear/Police/RCMP_cap.et", "RCMP Sapka", "Sapka", 400);
		Ekle("{CBBD26B48680B8CA}Prefabs/Characters/HeadGear/Ballcap/ballcap_white.et", "Sapka - Beyaz", "Sapka", 350);
		Ekle("{035D244643F9F800}Prefabs/Characters/HeadGear/Ballcap/ballcap_black.et", "Sapka - Siyah", "Sapka", 350);
		Ekle("{EBF7A239DB7DFABD}Prefabs/Characters/HeadGear/Ballcap/ballcap_black_white.et", "Sapka - Siyah Beyaz", "Sapka", 350);
		Ekle("{FD6BA8325F991D7D}Prefabs/Characters/HeadGear/TruckerCap/trucker_cap.et", "Tirci Sapka", "Sapka", 350);
		Ekle("{5B6998DF1207FB5A}Prefabs/Characters/HeadGear/TruckerCap/trucker_cap_backwards.et", "Tirci Sapka (Ters)", "Sapka", 350);
		Ekle("{53B8CDC2614392CA}Prefabs/Characters/HeadGear/TruckerCap/trucker_cap_EMA.et", "Tirci Sapka EMA", "Sapka", 350);
		Ekle("{A7999292353F300B}Prefabs/Characters/HeadGear/TruckerCap/trucker_cap_backwards_EMA.et", "Tirci Sapka EMA (Ters)", "Sapka", 350);
		Ekle("{56A18B6556D09BCC}Prefabs/Characters/Uniforms/Police/police_vest.et", "Polis Yelek", "Yelek", 900);
		Ekle("{99F2A7EA674DFE52}Prefabs/Characters/Uniforms/Police/RCMP_vest.et", "RCMP Yelek", "Yelek", 900);
	}

	//------------------------------------------------------------------------------------------------
	protected static void Ekle(ResourceName prefab, string ad, string kategori, int fiyat)
	{
		s_aUrunler.Insert(new M360_KiyafetUrun(prefab, ad, kategori, fiyat));
	}

	//------------------------------------------------------------------------------------------------
	static array<ref M360_KiyafetUrun> Hepsi()
	{
		Kur();
		return s_aUrunler;
	}

	//------------------------------------------------------------------------------------------------
	static void Kategoriler(out array<string> outKat)
	{
		Kur();
		outKat = {};
		outKat.Insert("Hepsi");
		foreach (M360_KiyafetUrun u : s_aUrunler)
		{
			if (outKat.Find(u.m_sKategori) < 0)
				outKat.Insert(u.m_sKategori);
		}
	}

	//------------------------------------------------------------------------------------------------
	static M360_KiyafetUrun Bul(ResourceName prefab)
	{
		Kur();
		foreach (M360_KiyafetUrun u : s_aUrunler)
		{
			if (M360_MagazaYardim.PrefabEslesir(u.m_sPrefab, prefab))
				return u;
		}
		return null;
	}

	//------------------------------------------------------------------------------------------------
	static int Fiyat(ResourceName prefab)
	{
		M360_KiyafetUrun u = Bul(prefab);
		if (!u)
			return -1;
		return u.m_iFiyat;
	}
}
