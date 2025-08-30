// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  ROM mapper sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

const c8 const Seg4Data1[] = "-> Segment #4 Data 1";
const c8 const Seg4Data2[] = "-> Segment #4 Data 2";

// Exten function
void DrawSegments(u8 y, const c8* txt);

//-----------------------------------------------------------------------------
//
void Seg4Func2(u8 x, u8 y, const c8* txt)
{
	x;
	DrawSegments(y, txt);
}

//-----------------------------------------------------------------------------
//
void Seg4Func1(u8 id) __banked
{
	if (id & 1)
		Seg4Func2(0, id, Seg4Data1);
	else
		Seg4Func2(0, id, Seg4Data2);
}
