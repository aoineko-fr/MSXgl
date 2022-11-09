//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)

#include "color.h"

u32 PaletteMSX[16] = { 0x000000, 0x000000, 0x3EB849, 0x74D07D, 0x5955E0, 0x8076F1, 0xB95E51, 0x65DBEF, 0xDB6559, 0xFF897D, 0xCCC35E, 0xDED087, 0x3AA241, 0xB766B5, 0xCCCCCC, 0xFFFFFF };

const bool bNativeBlueScale = false;
const bool bNativeGammeCorrection = false;
// MSX monitor generally has a gamma of 2.5, while on a PC the usual colourspace is sRGB nowadays, with a gamma of 2.2.
// So, in order to get the correct colors displayed on your PC, you’ll have to apply a gamma correction of 2.2 / 2.5 = 0.88 to the palette.
// http://map.grauw.nl/articles/vdp_guide.php

// Convert RGB24 to GRB8
GRB8::GRB8(RGB24 color)
{
	i32 r, g, b;

	r = ((18 + color.R) * 7 / 255) & 0x07;
	g = ((18 + color.G) * 7 / 255) & 0x07;
	if (bNativeBlueScale)
	{
		switch (((18 + color.B) * 7 / 255) & 0x07)
		{
		case 0:
			b = 0;
		case 1:
		case 2:
			b = 1;
		case 3:
		case 4:
			b = 2;
		case 5:
		case 6:
		case 7:
			b = 3;
		}
	}
	else // use linear scale
	{
		b = ((42 + color.B) * 3 / 255) & 0x03;
	}

	RGB = u8((g << 5) + (r << 2) + b);
}

// Convert GRB8 to RGB24
RGB24::RGB24(GRB8 color)
{
	i32 r, g, b;

	r = color / 32;
	g = (color & 0x1C) / 4;
	b = color & 0x03;

	R = u8(r * 255 / 7);
	G = u8(g * 255 / 7);
	B = u8(b * 255 / 3);
}