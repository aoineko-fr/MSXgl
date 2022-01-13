//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄           ▄▄   ▄▄   
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄█▄█ ▄▀██ ██▀  ██▄  
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ █ ▀▄██ ▀█▄▄ ██ █ 
//_____________________________________________________________________________
// CMSXmath 1.5.1 by Guillaume "Aoineko" Blanchard                              
// under CC-BY-AS free license                                                 
//                                                                             
// Generated: Sat Feb 27 18:45:38 2021
// Parameters: Entries=32, Bytes=2 (16-bits), Shift=12 (Q4.12)               
//_____________________________________________________________________________

// Sinus table. Range [0:Pi*2[
static const unsigned short g_Sinus32[32] =
{
	0x0000, 0x031F, 0x061F, 0x08E3, 0x0B50, 0x0D4D, 0x0EC8, 0x0FB1, 
	0x0FFF, 0x0FB1, 0x0EC8, 0x0D4D, 0x0B50, 0x08E3, 0x061F, 0x031F, 
	0x0000, 0xFCE1, 0xF9E1, 0xF71D, 0xF4B0, 0xF2B3, 0xF138, 0xF04F, 
	0xF001, 0xF04F, 0xF138, 0xF2B3, 0xF4B0, 0xF71D, 0xF9E1, 0xFCE1, 
};

// Cosinus table. Range [0:Pi*2[
static const unsigned short g_Cosinus32[32] =
{
	0x1000, 0x0FB1, 0x0EC8, 0x0D4D, 0x0B50, 0x08E3, 0x061F, 0x031F, 
	0x0000, 0xFCE1, 0xF9E1, 0xF71D, 0xF4B0, 0xF2B3, 0xF138, 0xF04F, 
	0xF001, 0xF04F, 0xF138, 0xF2B3, 0xF4B0, 0xF71D, 0xF9E1, 0xFCE1, 
	0x0000, 0x031F, 0x061F, 0x08E3, 0x0B50, 0x0D4D, 0x0EC8, 0x0FB1, 
};
