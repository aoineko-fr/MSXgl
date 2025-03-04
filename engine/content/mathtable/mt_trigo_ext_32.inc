//  ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█            ▄▄   ▄▄
//  ██  ▀  █▄  ▀██▄ ▀ ▄█  ▄█▄█ ▄▀██ ██▀  ██▄
//  █  █ █  ▀▀  ▄█  █  █  ██ █ ▀▄██ ▀█▄▄ ██ █
//  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
// ─────────────────────────────────────────────────────────────────────────────
//  MSXmath 1.7.0 by Guillaume "Aoineko" Blanchard
//  under CC-BY-SA free license
// 
//  Generated: Sun Mar 17 16:23:50 2024
//  Parameters: Entries=32, Bytes=2 (16-bits), Shift=8 (Q8.8)
// ─────────────────────────────────────────────────────────────────────────────

//  Tangent table. Range [-Pi/2:Pi/2[
const unsigned short g_Tangent32[] = {
	0x7FFF, 0xF5D9, 0xFAF9, 0xFCB4, 0xFD96, 0xFE21, 0xFE81, 0xFEC8, 
	0xFF00, 0xFF2E, 0xFF55, 0xFF77, 0xFF96, 0xFFB2, 0xFFCD, 0xFFE7, 
	0x0000, 0x0019, 0x0033, 0x004E, 0x006A, 0x0089, 0x00AB, 0x00D2, 
	0x0100, 0x0138, 0x017F, 0x01DF, 0x026A, 0x034C, 0x0507, 0x0A27, 
};

//  ArcSinus table. Range [-1:1]
const unsigned short g_ArcSinus32[] = {
	0xFE6E, 0xFEC7, 0xFEED, 0xFF0B, 0xFF24, 0xFF3B, 0xFF4F, 0xFF63, 
	0xFF75, 0xFF87, 0xFF98, 0xFFA9, 0xFFB9, 0xFFC9, 0xFFD9, 0xFFE9, 
	0xFFF8, 0x0008, 0x0017, 0x0027, 0x0037, 0x0047, 0x0057, 0x0068, 
	0x0079, 0x008B, 0x009D, 0x00B1, 0x00C5, 0x00DC, 0x00F5, 0x0113, 
};

//  ArcCosinus table. Range [-1:1]
const unsigned short g_ArcCosinus32[] = {
	0x0324, 0x02CB, 0x02A5, 0x0287, 0x026E, 0x0258, 0x0243, 0x022F, 
	0x021D, 0x020B, 0x01FA, 0x01E9, 0x01D9, 0x01C9, 0x01B9, 0x01A9, 
	0x019A, 0x018A, 0x017B, 0x016B, 0x015B, 0x014B, 0x013B, 0x012A, 
	0x0119, 0x0108, 0x00F5, 0x00E2, 0x00CD, 0x00B6, 0x009D, 0x007F, 
};

//  ArcTangent table. Range [0:N]
const unsigned short g_ArcTangent32[] = {
	0x0000, 0x00C5, 0x0118, 0x013D, 0x0152, 0x015E, 0x0167, 0x016D, 
	0x0171, 0x0175, 0x0178, 0x017A, 0x017C, 0x017E, 0x017F, 0x0181, 
	0x0182, 0x0183, 0x0183, 0x0184, 0x0185, 0x0186, 0x0186, 0x0187, 
	0x0187, 0x0188, 0x0188, 0x0188, 0x0189, 0x0189, 0x0189, 0x018A, 
};
