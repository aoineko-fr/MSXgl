// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS routines handler 
//
// References:
// - MSX2 Technical Handbook
// - http://map.grauw.nl/resources/msxbios.php
// - https://www.msx.org/wiki/Main-ROM_BIOS
// - Pratique du MSX2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "system.h"
#include "vdp_reg.h"

// R_xxxx	Data/routines in Main-ROM
// S_xxxx	Data/routines in Sub-ROM
// M_xxxx	Data/routines in Memory
// H_xxxx	Hooks
// P_xxxx	Ports
// I_xxxx	Interrupts
#include "bios_var.h"
#include "bios_hook.h"
#include "bios_mainrom.h"
#include "bios_subrom.h"
#include "bios_diskrom.h"

//=============================================================================
// DEFINES
//=============================================================================

// Input Macros
#define KEYBOARD_HOLD(key)    ((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0)
#define KEYBOARD_PRESS(key)   (((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0) && ((g_OLDKEY[KEY_ROW(key)] & KEY_FLAG(key)) != 0))
#define KEYBOARD_RELEASE(key) (((g_NEWKEY[KEY_ROW(key)] & KEY_FLAG(key)) != 0) && ((g_OLDKEY[KEY_ROW(key)] & KEY_FLAG(key)) == 0))

//#############################################################################
//  █ █ █▀▀ █   █▀█ █▀▀ █▀█
//  █▀█ ██▄ █▄▄ █▀▀ ██▄ █▀▄
//#############################################################################

//-----------------------------------------------------------------------------
// Group: Helper
//-----------------------------------------------------------------------------

// Function: BIOS_Exit
// Handle clean exit form Basic or MSX-DOS environment
//
// Parameters:
//   ret - Return value (for MSX-DOS environment)
void BIOS_Exit(u8 ret);

// Function: BIOS_SetKeyClick
// Enable or disable key click
//
// Parameters:
//   enable - TRUE to enable and FALSE to disable
inline void BIOS_SetKeyClick(bool enable) { g_CLIKSW = enable; }

// Function: BIOS_GetMSXVersion
// Get MSX generation version
//
// Return:
// - MSXVER_1 (0) for MSX1.
// - MSXVER_2 (1) for MSX2.
// - MSXVER_2P (2) for MSX2+.
// - MSXVER_TR (3) for MSX turbo R.
inline u8 BIOS_GetMSXVersion() { return g_MSXVER; }

//#############################################################################
//  █▀▄▀█ ▄▀█ █ █▄ █ ▄▄ █▀█ █▀█ █▀▄▀█
//  █ ▀ █ █▀█ █ █ ▀█    █▀▄ █▄█ █ ▀ █
//#############################################################################

//=============================================================================
// RST-and other routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Core
//-----------------------------------------------------------------------------

// Function: BIOS_Reboot
// Tests RAM and sets RAM slot for the system.
// Wrapper for CHKRAM routine.
inline void BIOS_Reboot() { Call(R_CHKRAM); }

// Function: BIOS_InterSlotRead
// Reads the value of an address in another slot.
// Wrapper for RDSLT routine.
u8 BIOS_InterSlotRead(u8 slot, u16 addr);

// Function: BIOS_InterSlotWrite
// Writes a value to an address in another slot.
// Wrapper for WRSLT routine.
void BIOS_InterSlotWrite(u8 slot, u16 addr, u8 value);

// Function: BIOS_InterSlotCall
// Executes inter-slot call.
// Wrapper for CALSLT routine.
void BIOS_InterSlotCall(u8 slot, u16 addr);

// Function: BIOS_SwitchSlot
// Switches to specified slot and page definitively.
// Wrapper for ENASLT routine.
void BIOS_SwitchSlot(u8 page, u8 slot);

// Macro: BIOS_CALLF
// Executes an interslot call.
// Wrapper for CALLF routine.
#define BIOS_CALLF(slot, addr) \
	__asm                      \
		rst		R_CALLF        \
		db		slot           \
		dw		addr           \
	__endasm

//=============================================================================
// Initialization-routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Init
//-----------------------------------------------------------------------------

//=============================================================================
// VDP routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - VDP
//-----------------------------------------------------------------------------
#if (BIOS_USE_VDP)

// Function: BIOS_DisableScreen
// Inhibits the screen display. Wrapper for DISSCR routine.
inline void BIOS_DisableScreen() { Call(R_DISSCR); }

// Function: BIOS_EnableScreen
// Displays the screen. Wrapper for ENASCR routine.
inline void BIOS_EnableScreen() { Call(R_ENASCR); }

// Function: BIOS_EnableScreen
// Display or disable the screen.
inline void BIOS_DisplayScreen(bool enable) { if (enable) BIOS_EnableScreen(); else BIOS_DisableScreen(); }

// Function: BIOS_WriteVDP
// Writes data in the VDP-register. Wrapper for WRTVDP routine.
void BIOS_WriteVDP(u8 reg, u8 value);

// Function: BIOS_ReadVDP
// Reads data from the VDP-register.
inline u8 BIOS_ReadVDP(u8 reg)
{
	switch (reg)
	{
	case 0: return g_RG0SAV;
	case 1: return g_RG1SAV;
	case 2: return g_RG2SAV;
	case 3: return g_RG3SAV;
	case 4: return g_RG4SAV;
	case 5: return g_RG5SAV;
	case 6: return g_RG6SAV;
	case 7: return g_RG7SAV;
	}
}

// Function: BIOS_ReadVRAM
// Reads the content of VRAM. Wrapper for RDVRM routine.
inline u8 BIOS_ReadVRAM(u16 addr) { return CallHL(R_RDVRM, addr); }

// Function: BIOS_WriteVRAM
// Writes data in VRAM. Wrapper for WRTVRM routine.
void BIOS_WriteVRAM(u8 value, u16 addr);

// Function: BIOS_SetAddressForRead
// Enables VDP to read. Wrapper for SETRD routine.
inline void BIOS_SetAddressForRead(u16 addr) { CallHL(R_SETRD, addr); }

// Function: BIOS_SetAddressForWrite
// Enables VDP to write. Wrapper for SETWRT routine.
inline void BIOS_SetAddressForWrite(u16 addr) { CallHL(R_SETWRT, addr); }

// Function: BIOS_FillVRAM
// Fills VRAM with value. Wrapper for FILVRM routine.
void BIOS_FillVRAM(u16 addr, u16 length, u8 value);

// Function: BIOS_CopyVRAMtoRAM
// Block transfer to memory from VRAM. Wrapper for LDIRMV routine.
void BIOS_CopyVRAMtoRAM(u16 vram, void* ram, u16 length);

// Function: BIOS_CopyRAMtoVRAM
// Block transfer to VRAM from memory. Wrapper for LDIRVM routine.
void BIOS_CopyRAMtoVRAM(const void* ram, u16 vram, u16 length);

// Function: BIOS_SetScreenMode
// Switches to given screen mode. Wrapper for CHGMOD routine.
inline void BIOS_SetScreenMode(u8 screen) { CallA(R_CHGMOD, screen); }

// Function: BIOS_SetColor
// Set screen colors.
inline void BIOS_SetColor(u8 text, u8 back, u8 border) { g_FORCLR = text; g_BAKCLR = back; g_BDRCLR = border; }

// Function: BIOS_ApplyColor
// Changes the screen colors. Wrapper for CHGCLR routine.
inline void BIOS_ApplyColor(u8 text, u8 back, u8 border) { BIOS_SetColor(text, back, border); Call(R_CHGCLR); }

// Function: BIOS_ApplyBorder
// Changes the screen colors. Wrapper for CHGCLR routine.
inline void BIOS_ApplyBorder(u8 border) { g_BDRCLR = border; Call(R_CHGCLR); }

//.............................................................................
// Screen 0

// Function: BIOS_SetWidth40
// 
inline void BIOS_SetWidth40(u8 width) { g_LINL40 = width;}

// Function: BIOS_InitScreen0
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0() { Call(R_INITXT); }

// Function: BIOS_InitScreen0Color
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0Color(u8 width, u8 text, u8 back, u8 border) { g_LINL40 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen0(); }

// Function: BIOS_InitScreen0Ex
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void BIOS_InitScreen0Ex(u16 pnt, u16 pgt, u8 width, u8 text, u8 back, u8 border) { g_TXTNAM = pnt; g_TXTCGP = pgt; g_LINL40 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen0(); }

// Function: BIOS_SetScreen0
// Switches VDP in SCREEN 0 mode. Wrapper for SETTXT routine.
inline void BIOS_SetScreen0() { Call(R_SETTXT); }

#define BIOS_InitTextMode			BIOS_InitScreen0
#define BIOS_InitTextModeColor		BIOS_InitScreen0Color
#define BIOS_InitTextModeEx			BIOS_InitScreen0Ex
#define BIOS_SetTextMode			BIOS_SetScreen0

//.............................................................................
// Screen 1

// Function: BIOS_SetWidth32
// 
inline void BIOS_SetWidth32(u8 width) { g_LINL32 = width;}

// Function: BIOS_InitScreen1
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1() { Call(R_INIT32); }

// Function: BIOS_InitScreen1Color
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1Color(u8 width, u8 text, u8 back, u8 border) { g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen1(); }

// Function: BIOS_InitScreen1Ex
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void BIOS_InitScreen1Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 width, u8 text, u8 back, u8 border) { g_T32NAM = pnt; g_T32COL = ct; g_T32CGP = pgt; g_T32ATR = sat; g_T32PAT = sgt; g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen1(); }

// Function: BIOS_SetScreen1
// Switches VDP in SCREEN 1 mode. Wrapper for SETT32 routine.
inline void BIOS_SetScreen1() { Call(R_SETT32); }

#define BIOS_InitText32Mode			BIOS_InitScreen1
#define BIOS_InitText32ModeColor	BIOS_InitScreen1Color
#define BIOS_InitText32ModeEx		BIOS_InitScreen1Ex
#define BIOS_SetText32Mode			BIOS_SetScreen1

//.............................................................................
// Screen 2

// Function: BIOS_InitScreen2
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2() { Call(R_INIGRP); }

// Function: BIOS_InitScreen2Color
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2Color(u8 width, u8 text, u8 back, u8 border) { g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen2(); }

// Function: BIOS_InitScreen2Ex
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void BIOS_InitScreen2Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 width, u8 text, u8 back, u8 border) { g_GRPNAM = pnt; g_GRPCOL = ct; g_GRPCGP = pgt; g_GRPATR = sat; g_GRPPAT = sgt; g_LINL32 = width; BIOS_SetColor(text, back, border); BIOS_InitScreen2(); }

// Function: BIOS_SetScreen2
// Switches VDP to SCREEN 2 mode. Wrapper for SETGRP routine.
inline void BIOS_SetScreen2() { Call(R_SETGRP); }

#define BIOS_InitGraphicMode		BIOS_InitScreen2	
#define BIOS_InitGraphicModeColor	BIOS_InitScreen2Color	
#define BIOS_InitGraphicModeEx		BIOS_InitScreen2Ex	
#define BIOS_SetGraphicMode			BIOS_SetScreen2	

//.............................................................................
// Screen 3

// Function: BIOS_InitScreen3
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
inline void BIOS_InitScreen3() { Call(R_INIMLT); }

// Function: BIOS_InitScreen3Ex
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
void BIOS_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

// Function: BIOS_SetScreen3
// Switches VDP to SCREEN 3 mode. Wrapper for SETMLT routine.
inline void BIOS_SetScreen3() { Call(R_SETMLT); }

#define BIOS_InitMulticolorMode  	BIOS_InitScreen3
#define BIOS_InitMulticolorModeEx	BIOS_InitScreen3Ex
#define BIOS_SetMulticolorMode		BIOS_SetScreen3

//.............................................................................
// Sprites routines

// Function: BIOS_ClearSprites
// Initialises all sprites.
inline void BIOS_ClearSprites() { Call(R_CLRSPR); }

#define BIOS_SPRITE_MAG_1			(0)
#define BIOS_SPRITE_MAG_2			(0b00000001)
#define BIOS_SPRITE_SIZE_8			(0)
#define BIOS_SPRITE_SIZE_16			(0b00000010)

// Function: BIOS_SetSpriteMode
// Set sprite size (8x8 or 16x16) and magnification mode (x1 or x2).
//
// Parameters:
//   size - Sprite size (BIOS_SPRITE_SIZE_8 or BIOS_SPRITE_SIZE_16)
//   mag - Sprite magnification (BIOS_SPRITE_MAG_1 or BIOS_SPRITE_MAG_2)
inline void BIOS_SetSpriteMode(u8 size, u8 mag) { BIOS_WriteVDP(1, (BIOS_ReadVDP(1) & 0xFC) | size | mag); }

// Function: BIOS_GetSpritePatternAddress
// Returns the address of the sprite pattern table. Wrapper for CALPAT routine.
//
// Parameters:
//   id - Sprite pattern number [0:255] of 8x8 size and [0:63] of 16x16 size)
//
// Return:
//   VRAM address of the sprite pattern table for the given sprite number
u16 BIOS_GetSpritePatternAddress(u8 id);

// Function: BIOS_GetSpriteAttributeAddress
// Returns the address of the sprite attribute table. Wrapper for CALATR routine.
//
// Parameters:
//   id - Sprite attribute number [0:31]
//
// Return:
//   VRAM address of the sprite attribute table for the given sprite number
u16 BIOS_GetSpriteAttributeAddress(u8 id);

// Function: BIOS_SetSpritePosition
// Set the position of a sprite on the screen.
//
// Parameters:
//   id - Sprite attribute number [0:31]
//   x  - Horizontal position of the sprite [0:255]
//   y  - Vertical position of the sprite [0:255]
inline void BIOS_SetSpritePosition(u8 id, u8 x, u8 y) { u16 vram = BIOS_GetSpriteAttributeAddress(id); BIOS_WriteVRAM(x, vram + 1); BIOS_WriteVRAM(y, vram); }

// Function: BIOS_SetSpritePattern
// Set the pattern of a sprite.
//
// Parameters:
//   id  - Sprite attribute number [0:31]
//   pat - Sprite pattern number [0:255]
inline void BIOS_SetSpritePattern(u8 id, u8 pat) { BIOS_WriteVRAM(pat, BIOS_GetSpriteAttributeAddress(id) + 2); }

// Function: BIOS_SetSpriteColor
// Set the color of a sprite.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   color - Sprite color [0:15]
inline void BIOS_SetSpriteColor(u8 id, u8 color) { BIOS_WriteVRAM(color, BIOS_GetSpriteAttributeAddress(id) + 3); }

// Function: BIOS_SetSprite
// Set sprite attributes.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   x     - Horizontal position of the sprite [0:255]
//   y     - Vertical position of the sprite [0:255]
//   pat   - Sprite pattern number [0:255]
//   color - Sprite color [0:15]
inline void BIOS_SetSprite(u8 id, u8 x, u8 y, u8 pat, u8 color) { u16 vram = BIOS_GetSpriteAttributeAddress(id); BIOS_WriteVRAM(y, vram); BIOS_WriteVRAM(x, vram + 1); BIOS_WriteVRAM(pat, vram + 2); BIOS_WriteVRAM(color, vram + 3); }

typedef struct BIOS_SpriteAttributes
{
	u8 y;
	u8 x;
	u8 pattern;
	u8 color;
} BIOS_SpriteAttributes;

// Function: BIOS_SetSpriteData
// Set a sprite attributes.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   attrs - Pointer to a 4-bytes array containing the sprite attributes (y, x, pattern and color)
inline void BIOS_SetSpriteData(u8 id, const BIOS_SpriteAttributes* attrs) { BIOS_CopyRAMtoVRAM(attrs, BIOS_GetSpriteAttributeAddress(id), 4); }

// Function: BIOS_GetSpriteSize
// Returns current sprite size in bytes (8 or 32). Wrapper for GSPSIZ routine.
inline u8 BIOS_GetSpriteSize() { return ((u8(*)(void))R_GSPSIZ)(); }

// Function: BIOS_IsSpriteCollision
// Returns FALSE if no collision occured during the previous frame, otherwise returns S00_C.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline bool BIOS_IsSpriteCollision() { return g_STATFL & S00_C; }

// Function: BIOS_IsSpriteOverScan
// Returns FALSE if no over-scan occured during the previous frame (more than 4/8 sprites on the same line), otherwise returns S00_5S.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline bool BIOS_IsSpriteOverScan() { return g_STATFL & S00_5S; }

// Function: BIOS_GetSpriteOverScanId
// Returns index of the over-scaned sprite (5th/8th sprite on line). Value is in 0-31 range.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline u8 BIOS_GetSpriteOverScanId() { return S00_GET_SN(g_STATFL); }

//.............................................................................
// Graph print routines

// Function: BIOS_GraphSetCursor
// Moves graphical cursor to the specified position.
inline void BIOS_GraphSetCursor(u16 x, u16 y) { g_GRPACX = x; g_GRPACY = y; }

// Function: BIOS_GraphPrintChar
// Displays a character on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrintChar(u8 chr) { CallA(R_GRPPRT, chr); }

// Function: BIOS_GraphPrint
// Displays a string on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrint(const c8* str) { while (*str) BIOS_GraphPrintChar(*str++); }

// Function: BIOS_GraphPrintAt
// Displays a string on the graphic screen. Wrapper for GRPPRT routine.
inline void BIOS_GraphPrintAt(u16 x, u16 y, const c8* str) { BIOS_GraphSetCursor(x, y); BIOS_GraphPrint(str); }

#if (MSX_VERSION >= MSX_2)

// Function: BIOS_GraphSetOperator
// Changes the operator used for graphical print.
inline void BIOS_GraphSetOperator(u8 op) { g_LOGOPR = op; }

#endif // (MSX_VERSION >= MSX_2)


#endif // BIOS_USE_VDP

//=============================================================================
// PSG routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - PSG
//-----------------------------------------------------------------------------
#if (BIOS_USE_PSG)

// Function: BIOS_InitPSG
// Initialises PSG and sets initial value for the PLAY statement. Wrapper for GICINI routine.
//> GICINI
//> Address  : #0090
//> Function : Initialises PSG and sets initial value for the PLAY statement
//> Registers: All
//> Remarks  : Interrupts must be disabled to call this routine
inline void BIOS_InitPSG() { DisableInterrupt(); Call(R_GICINI); EnableInterrupt(); }

// Function: BIOS_WritePSG
// Writes data to PSG-register. Wrapper for WRTPSG routine.
//> WRTPSG
//> Address  : #0093
//> Function : Writes data to PSG register
//> Input    : A  - PSG register number
//>            E  - Data write
//> Remarks  : See https://www.msx.org/wiki/SOUND#Registers_Description
//
// Parameters:
//   reg   - PSG register number (0-15)
//   value - Value to write in the PSG register
void BIOS_WritePSG(u8 reg, u8 value);

// Function: BIOS_ReadPSG
// Reads value from PSG-register. Wrapper for RDPSG routine.
//> RDPSG
//> Address  : #0096
//> Function : Reads value from PSG register
//> Input    : A  - PSG register read
//> Output   : A  - Value read
//
// Parameters:
//   reg - PSG register number (0-15)
//
// Return:
//   Value read from the PSG register
inline u8 BIOS_ReadPSG(u8 reg) { return ((u8(*)(u8))R_RDPSG)(reg); }

// Function: BIOS_PlayPSG
// Tests whether the PLAY statement is being executed as a background task. Wrapper for STRTMS routine.
//> STRTMS
//> Address  : #0099
//> Function : Tests whether the PLAY statement is being executed as a background
//>            task. If not, begins to execute the PLAY statement
//> Registers: All
inline void BIOS_PlayPSG() { Call(R_STRTMS); }

#endif // BIOS_USE_PSG

//=============================================================================
// Console routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Console
//-----------------------------------------------------------------------------

// Function: BIOS_GetCharacter
// One character input (waiting). Wrapper for CHGET routine.
inline c8 BIOS_GetCharacter() { return CallToA(R_CHGET); }

// Function: BIOS_HasCharacter
// Get a character input (if any) or return 0.
u8 BIOS_HasCharacter();

// Function: BIOS_TextSetCursor
// Moves cursor to the specified position. Wrapper for POSIT routine.
void BIOS_TextSetCursor(u8 x, u8 y);

// Function: BIOS_TextPrintChar
// Displays one character. Wrapper for CHPUT routine.
inline void BIOS_TextPrintChar(c8 chr) { CallA(R_CHPUT, chr); }

// Function: BIOS_TextPrint
// Displays a null-terminated string.
inline void BIOS_TextPrint(const c8* str) { while (*str) BIOS_TextPrintChar(*str++); }

// Function: BIOS_TextPrintAt
// Print a text at the specified position
inline void BIOS_TextPrintAt(u8 x, u8 y, const c8* str) { BIOS_TextSetCursor(x, y); BIOS_TextPrint(str); }

// Function: BIOS_Beep
// Generates beep. Wrapper for BEEP routine.
inline void BIOS_Beep() { Call(R_BEEP); }

// Function: BIOS_ClearScreen
// Clears the screen. Wrapper for CLS routine.
void BIOS_ClearScreen();


//=============================================================================
// Controller routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Controller
//-----------------------------------------------------------------------------

#define BIOS_JOYSTICK_KEYBOARD		(0)
#define BIOS_JOYSTICK_PORT1			(1)
#define BIOS_JOYSTICK_PORT2			(2)

#define BIOS_DIRECTION_NONE			(0)
#define BIOS_DIRECTION_UP			(1)
#define BIOS_DIRECTION_UP_RIGHT		(2)
#define BIOS_DIRECTION_RIGHT		(3)
#define BIOS_DIRECTION_DOWN_RIGHT	(4)
#define BIOS_DIRECTION_DOWN			(5)
#define BIOS_DIRECTION_DOWN_LEFT	(6)
#define BIOS_DIRECTION_LEFT			(7)
#define BIOS_DIRECTION_UP_LEFT		(8)

// Function: BIOS_GetJoystickDirection
// Returns the state of the joystick or the cursor keys.
// Wrapper for GTSTCK routine.
//
// Parameters:
//   port - Joystick number to be tested:
//> 0 = Cursor keys
//> 1 = Joystick on port 1
//> 2 = Joystick on port 2
//
// Return:
//   Direction of the joystick or function keys (0-8):
//>  8  1  2
//>   \ | /
//> 7 - 0 - 3
//>   / |'\'
//>  6  5  4
inline u8 BIOS_GetJoystickDirection(u8 port) { return ((u8(*)(u8))R_GTSTCK)(port); }

#define BIOS_TRIGGER_SPACEBAR		(0)
#define BIOS_TRIGGER_PORT1_TRIG1	(1)
#define BIOS_TRIGGER_PORT2_TRIG1	(2)
#define BIOS_TRIGGER_PORT1_TRIG2	(3)
#define BIOS_TRIGGER_PORT2_TRIG2	(4)

// Function: BIOS_GetJoystickTrigger
// Returns the state of the mouse, joystick or keyboard space bar buttons.
// Wrapper for GTTRIG routine.
//
// Parameters:
//   trigger - Trigger button to test:
//> 0 = space bar
//> 1 = port 1, button A
//> 2 = port 2, button A
//> 3 = port 1, button B
//> 4 = port 2, button B
//
// Return:
//   FALSE if trigger button is not pressed
inline bool BIOS_GetJoystickTrigger(u8 trigger) { return ((u8(*)(u8))R_GTTRIG)(trigger); }

// Function: BIOS_GetTouchPad
// Returns the touch pad status.
// Wrapper for GTPAD routine.
//
// Parameters:
//   entry - Function call number. Fetch device data first, then read:
//> MSX1:
//>   0 = Fetch touch pad data from port 1 (#FF if available)
//>   1 = Read X-position
//>   2 = Read Y-position
//>   3 = Read touchpad status from port 1 (#FF if pressed
//>   4 = Fetch touch pad data from port 2 (#FF if available)
//>   5 = Read X-position
//>   6 = Read Y-position
//>   7 = Read touchpad status from port 2 (#FF if pressed)
//> MSX2/2+/turbo R: 
//>   8 = Fetch light pen data (#FF if available)
//>   9 = Read X-position
//>  10 = Read Y-position
//>  11 = Read light pen status (#FF if pressed)
//>  12 = Fetch mouse data from port 1 (#FF if available)
//>  13 = Read X-position
//>  14 = Read Y-position
//>  15 = Read mouse button status from port 1 (#FF if pressed)
//>  16 = Fetch mouse data from port 2 (#FF if available)
//>  17 = Read X-position
//>  18 = Read Y-position
//>  19 = Read mouse button status from port 2 (#FF if pressed)
//
// Return:
//   Value depending on entry parameter
inline u8 BIOS_GetTouchPad(u8 entry) { return ((u8(*)(u8))R_GTPAD)(entry); }

// Function: BIOS_GetPaddle
// Returns the paddle position.
// Wrapper for GTPDL routine.
//
// Parameters:
//   num - Paddle number (1-12):
//> Paddle | Port1 | Port2
//> -------+-------+-------
//>    A   |   1   |   2
//>    B   |   3   |   4
//>    C   |   5   |   6
//>    D   |   7   |   8
//>    E   |   9   |   10
//>    F   |   11  |   12
//
// Return:
//   Paddle position [0:255]
inline u8 BIOS_GetPaddle(u8 num) { return ((u8(*)(u8))R_GTPDL)(num); }


//=============================================================================
// Misc routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Misc
//-----------------------------------------------------------------------------

// Function: BIOS_GetKeyboardMatrix
// Returns the value of the specified line from the keyboard matrix. Wrapper for SNSMAT routine.
inline u8 BIOS_GetKeyboardMatrix(u8 line) { return ((u8(*)(u8))R_SNSMAT)(line); }

// Function: BIOS_IsKeyPressed
// Check if the given key is pressed
inline bool BIOS_IsKeyPressed(u8 key) {	return (g_NEWKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0; }

//=============================================================================
// MSX2
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - MSX2
//-----------------------------------------------------------------------------
#if (MSX_VERSION >= MSX_2)
#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
// MSX2+
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - MSX2+
//-----------------------------------------------------------------------------
#if (MSX_VERSION >= MSX_2P)
#endif // (MSX_VERSION >= MSX_2P)

//=============================================================================
// MSX turbo R
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - MSX turbo R
//-----------------------------------------------------------------------------
#if (MSX_VERSION == MSX_TR)

#define CPU_MODE_Z80		0x00
#define CPU_MODE_R800_ROM	0x01
#define CPU_MODE_R800_DRAM	0x02
#define CPU_TURBO_LED		0x80

// Function: BIOS_SetCPUMode
// Changes CPU mode. Wrapper for CHGCPU routine.
inline void BIOS_SetCPUMode(u8 mode) { ((void(*)(u8))R_CHGCPU)(mode); }

// Function: BIOS_GetCPUMode
// Returns current CPU mode. Wrapper for GETCPU routine.
inline u8 BIOS_GetCPUMode() { return ((u8(*)(void))R_GETCPU)(); }

#endif // (MSX_VERSION == MSX_TR)
