//------------------------------------------------------------------------------------------------
//! Hud texture ResourceName — Circle HUD badge + ring/mask + nakit/plus
//------------------------------------------------------------------------------------------------
class M360_HudIkonlari
{
	static const ResourceName BADGE_CAN = "{C8A10001D0E10001}UI/Textures/M360/m360_badge_can_UI.edds";
	static const ResourceName BADGE_YEMEK = "{C8A10001D0E10002}UI/Textures/M360/m360_badge_yemek_UI.edds";
	static const ResourceName BADGE_SU = "{C8A10001D0E10003}UI/Textures/M360/m360_badge_su_UI.edds";

	static const ResourceName RADIAL_MASK = "{B7E41A92C0D35F01}UI/Textures/M360/m360_radial_mask_UI.edds";
	static const ResourceName RADIAL_RING = "{B7E41A92C0D35F02}UI/Textures/M360/m360_radial_ring_UI.edds";

	static const ResourceName NAKIT = "{6EB67A75E83334F8}UI/Textures/M360/m360_nakit_UI.edds";
	static const ResourceName PLUS = "{C8A10001D0E10004}UI/Textures/M360/m360_plus_UI.edds";
	static const ResourceName SES_MUTE = "{C8A10001D0E10030}UI/Textures/M360/m360_ses_mute_UI.edds";
	static const ResourceName PILL_L = "{C8A10001D0E10011}UI/Textures/M360/m360_pill_cap_L_UI.edds";
	static const ResourceName PILL_R = "{C8A10001D0E10012}UI/Textures/M360/m360_pill_cap_R_UI.edds";
	static const ResourceName PILL_MID = "{C8A10001D0E10013}UI/Textures/M360/m360_pill_mid_UI.edds";
	static const ResourceName PANEL = "{C8A10001D0E10020}UI/Textures/M360/m360_panel_UI.edds";
	static const ResourceName ROW = "{C8A10001D0E10021}UI/Textures/M360/m360_row_UI.edds";
	static const ResourceName ITEM_HAM = "{C8A10001D0E10022}UI/Textures/M360/m360_item_ham_UI.edds";
	static const ResourceName ITEM_ISLENMIS = "{C8A10001D0E10023}UI/Textures/M360/m360_item_islenmis_UI.edds";
	static const ResourceName BAR = "{C8A10001D0E10024}UI/Textures/M360/m360_bar_UI.edds";
	static const ResourceName PANEL_CORNER = "{C8A10001D0E10025}UI/Textures/M360/m360_panel_corner_UI.edds";
	static const ResourceName PANEL_EDGE = "{C8A10001D0E10026}UI/Textures/M360/m360_panel_edge_UI.edds";
	static const ResourceName WELL = "{C8A10001D0E10027}UI/Textures/M360/m360_well_UI.edds";
	static const ResourceName PANEL_CTL = "{C8A10001D0E10028}UI/Textures/M360/m360_panel_cTL_UI.edds";
	static const ResourceName PANEL_CTR = "{C8A10001D0E10029}UI/Textures/M360/m360_panel_cTR_UI.edds";
	static const ResourceName PANEL_CBL = "{C8A10001D0E1002A}UI/Textures/M360/m360_panel_cBL_UI.edds";
	static const ResourceName PANEL_CBR = "{C8A10001D0E1002B}UI/Textures/M360/m360_panel_cBR_UI.edds";

	//------------------------------------------------------------------------------------------------
	static bool Yukle(ImageWidget img, ResourceName yol)
	{
		if (!img || yol.IsEmpty())
			return false;

		img.SetIsColorInherited(false);
		if (!img.LoadImageTexture(0, yol))
		{
			Print(string.Format("[M360] Texture yok: %1", yol), LogLevel.WARNING);
			return false;
		}

		img.SetImage(0);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool MaskYukle(ImageWidget img, ResourceName mask)
	{
		if (!img || mask.IsEmpty())
			return false;

		return img.LoadMaskTexture(mask);
	}
}
