//------------------------------------------------------------------------------------------------
//! Paylasilan HUD yazi yardimcilari
//------------------------------------------------------------------------------------------------
class M360_HudYazi
{
	//------------------------------------------------------------------------------------------------
	static string NakitFormat(int nakit)
	{
		if (nakit < 0)
			nakit = 0;

		if (nakit < 1000)
			return string.Format("%1", nakit);

		int ust = nakit / 1000;
		int kalan = nakit - (ust * 1000);
		string pad;
		if (kalan < 10)
			pad = string.Format("00%1", kalan);
		else if (kalan < 100)
			pad = string.Format("0%1", kalan);
		else
			pad = string.Format("%1", kalan);

		return string.Format("%1.%2", NakitFormat(ust), pad);
	}
}
