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
// - Pratique du MSX 2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

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

// Function: Bios_Exit
// Handle clean exit form Basic or MSX-DOS environment
//
// Parameters:
//   ret - Return value (for MSX-DOS environment)
void Bios_Exit(u8 ret) __FASTCALL;

// Function: Bios_SetKeyClick
// Enable or disable key click
//
// Parameters:
//   ret - TRUE to enable and FALSE to disable
inline void Bios_SetKeyClick(bool enable) { g_CLIKSW = enable; }

// Function: Bios_GetMSXVersion
// Get MSX generation version
//
// Return:
// - MSXVER_1 (0) for MSX 1.
// - MSXVER_2 (1) for MSX 2.
// - MSXVER_2P (2) for MSX 2+.
// - MSXVER_TR (3) for MSX turbo R.
inline u8 Bios_GetMSXVersion() { return g_MSXVER; }

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

// Function: Bios_Startup
// Tests RAM and sets RAM slot for the system. Wrapper for CHKRAM routine.
inline void Bios_Startup() { Call(R_CHKRAM); }

// Function: Bios_InterSlotRead
// Reads the value of an address in another slot. Wrapper for RDSLT routine.
u8 Bios_InterSlotRead(u8 slot, u16 addr);

// Function: Bios_InterSlotWrite
// Writes a value to an address in another slot. Wrapper for WRSLT routine.
void Bios_InterSlotWrite(u8 slot, u16 addr, u8 value);

// Function: Bios_InterSlotCall
// Executes inter-slot call. Wrapper for CALSLT routine.
void Bios_InterSlotCall(u8 slot, u16 addr);

// Function: Bios_SwitchSlot
// Switches indicated slot at indicated page on perpetual. Wrapper for ENASLT routine.
void Bios_SwitchSlot(u8 page, u8 slot);

// Macro: BIOS_CALLF
// Executes an interslot call. Wrapper for CALLF routine.
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
#if BIOS_USE_VDP

// Function: Bios_DisableScreen
// Inhibits the screen display. Wrapper for DISSCR routine.
inline void Bios_DisableScreen() { Call(R_DISSCR); }

// Function: Bios_EnableScreen
// Displays the screen. Wrapper for ENASCR routine.
inline void Bios_EnableScreen() { Call(R_ENASCR); }

// Function: Bios_WriteVDP
// Write data in the VDP-register. Wrapper for WRTVDP routine.
void Bios_WriteVDP(u8 reg, u8 value);

// Function: Bios_ReadVRAM
// Reads the content of VRAM. Wrapper for RDVRM routine.
inline u8 Bios_ReadVRAM(u16 addr) { return ((u8(*)(u16))R_RDVRM)(addr); }

// Function: Bios_WriteVRAM
// Writes data in VRAM. Wrapper for WRTVRM routine.
void Bios_WriteVRAM(u16 addr, u8 value);

// Function: Bios_SetAddressForRead
// Enable VDP to read. Wrapper for SETRD routine.
inline void Bios_SetAddressForRead(u16 addr) { CallHL(R_SETRD, addr); }

// Function: Bios_SetAddressForWrite
// Enable VDP to write. Wrapper for SETWRT routine.
inline void Bios_SetAddressForWrite(u16 addr) { CallHL(R_SETWRT, addr); }

// Function: Bios_FillVRAM
// Fill VRAM with value. Wrapper for FILVRM routine.
void Bios_FillVRAM(u16 addr, u16 length, u8 value);

// Function: Bios_TransfertVRAMtoRAM
// Block transfer to memory from VRAM. Wrapper for LDIRMV routine.
void Bios_TransfertVRAMtoRAM(u16 vram, u16 ram, u16 length);

// Function: Bios_TransfertRAMtoVRAM
// Block transfer to VRAM from memory. Wrapper for LDIRVM routine.
void Bios_TransfertRAMtoVRAM(u16 ram, u16 vram, u16 length);

// Function: Bios_ChangeMode
// Switches to given screenmode. Wrapper for CHGMOD routine.
inline void Bios_ChangeMode(u8 screen) { ((void(*)(u8))R_CHGMOD)(screen); }

// Function: Bios_ChangeColor
// Changes the screencolors. Wrapper for CHGCLR routine.
void Bios_ChangeColor(u8 text, u8 back, u8 border);

// Function: Bios_InitScreen0
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
inline void Bios_InitScreen0() { Call(R_INITXT); }

// Function: Bios_InitScreen0Ex
// Switches to SCREEN 0 (text screen with 40*24 characters). Wrapper for INITXT routine.
void Bios_InitScreen0Ex(u16 pnt, u16 pgt, u8 width, u8 text, u8 bg, u8 border);

#define Bios_InitTextMode   Bios_InitScreen0
#define Bios_InitTextModeEx Bios_InitScreen0Ex

// Function: Bios_InitScreen1
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
inline void Bios_InitScreen1() { Call(R_INIT32); }

// Function: Bios_InitScreen1Ex
// Switches to SCREEN 1 (text screen with 32*24 characters). Wrapper for INIT32 routine.
void Bios_InitScreen1Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitText32Mode   Bios_InitScreen1	
#define Bios_InitText32ModeEx Bios_InitScreen1Ex	

// Function: Bios_InitScreen2
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
inline void Bios_InitScreen2() { Call(R_INIGRP); }

// Function: Bios_InitScreen2Ex
// Switches to SCREEN 2 (high resolution screen with 256*192 pixels). Wrapper for INIGRP routine.
void Bios_InitScreen2Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitGraphicMode   Bios_InitScreen2	
#define Bios_InitGraphicModeEx Bios_InitScreen2Ex	

// Function: Bios_InitScreen3
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
inline void Bios_InitScreen3() { Call(R_INIMLT); }

// Function: Bios_InitScreen3Ex
// Switches to SCREEN 3 (multi-color screen 64*48 pixels). Wrapper for INIMLT routine.
void Bios_InitScreen3Ex(u16 pnt, u16 ct, u16 pgt, u16 sat, u16 sgt, u8 text, u8 bg, u8 border);

#define Bios_InitMulticolorMode   Bios_InitScreen3
#define Bios_InitMulticolorModeEx Bios_InitScreen3Ex

// Function: Bios_SetScreen0
// Switches VDP in SCREEN 0 mode. Wrapper for SETTXT routine.
inline void Bios_SetScreen0() { Call(R_SETTXT); }

#define Bios_SetTextMode Bios_SetScreen0

// Function: Bios_SetScreen1
// Switches VDP in SCREEN 1 mode. Wrapper for SETT32 routine.
inline void Bios_SetScreen1() { Call(R_SETT32); }

#define Bios_SetText32Mode Bios_SetScreen1	

// Function: Bios_SetScreen2
// Switches VDP to SCREEN 2 mode. Wrapper for SETGRP routine.
inline void Bios_SetScreen2() { Call(R_SETGRP); }

#define Bios_SetGraphicMode Bios_SetScreen2	

// Function: Bios_SetScreen3
// Switches VDP to SCREEN 3 mode. Wrapper for SETMLT routine.
inline void Bios_SetScreen3() { Call(R_SETMLT); }

#define Bios_SetMulticolorMode Bios_SetScreen3

// Function: Bios_GetPatternTableAddress
// Returns the address of the sprite pattern table. Wrapper for CALPAT routine.
u16 Bios_GetPatternTableAddress(u8 id) __FASTCALL;

// Function: Bios_GetAttributeTableAddress
// Returns the address of the sprite attribute table. Wrapper for CALATR routine.
u16 Bios_GetAttributeTableAddress(u8 id) __FASTCALL;

// Function: Bios_GetSpriteSize
// Returns current sprite size. Wrapper for GSPSIZ routine.
inline u8 Bios_GetSpriteSize() { return ((u8(*)(void))R_GSPSIZ)(); }

// Function: Bios_GraphPrintChar
// Displays a character on the graphic screen. Wrapper for GRPPRT routine.
inline void Bios_GraphPrintChar(u8 chr) { ((void(*)(u8))R_GRPPRT)(chr); }

// Function: Bios_GraphPrintCharEx
// Displays a character on the graphic screen. Wrapper for GRPPRT routine.
void Bios_GraphPrintCharEx(u8 chr, u16 x, u8 y, u8 color, u8 op);

// Function: Bios_IsSpriteCollision
// Returns FALSE if no collision occured during the previous frame, otherwise returns S00_C.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline bool Bios_IsSpriteCollision() { return g_STATFL & S00_C; }

// Function: Bios_IsSpriteOverScan
// Returns FALSE if no over-scan occured during the previous frame (more than 4/8 sprites on the same line), otherwise returns S00_5S.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline bool Bios_IsSpriteOverScan() { return g_STATFL & S00_5S; }

// Function: Bios_GetSpriteOverScanId
// Returns index of the over-scaned sprite (5th/8th sprite on line). Value is in 0-31 range.
// This function use value of VDP status register S#0 that BIOS backup in RAM (STATFL). 
inline u8 Bios_GetSpriteOverScanId() { return S00_GET_SN(g_STATFL); }

#endif // BIOS_USE_VDP

//=============================================================================
// PSG routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - PSG
//-----------------------------------------------------------------------------
#if BIOS_USE_PSG

// Function: Bios_InitPSG
// Initialises PSG and sets initial value for the PLAY statement. Wrapper for GICINI routine.
inline void Bios_InitPSG() { DisableInterrupt(); Call(R_GICINI); EnableInterrupt(); }

// Function: Bios_WritePSG
// Writes data to PSG-register. Wrapper for WRTPSG routine.
void Bios_WritePSG(u8 reg, u8 value);

// Function: Bios_ReadPSG
// Reads value from PSG-register. Wrapper for RDPSG routine.
inline u8 Bios_ReadPSG(u8 reg) { return ((u8(*)(u8))R_RDPSG)(reg); }

// Function: Bios_PlayPSG
// Tests whether the PLAY statement is being executed as a background task. Wrapper for STRTMS routine.
inline void Bios_PlayPSG() { Call(R_STRTMS); }

#endif // BIOS_USE_PSG

//=============================================================================
// Console routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Console
//-----------------------------------------------------------------------------

// Function: Bios_GetCharacter
// One character input (waiting). Wrapper for CHGET routine.
inline u8 Bios_GetCharacter() { return ((u8(*)(void))R_CHGET)(); }

// Function: Bios_TextPrintChar
// Displays one character. Wrapper for CHPUT routine.
inline void Bios_TextPrintChar(u8 chr) { ((void(*)(u8))R_CHPUT)(chr); }

// Function: Bios_Beep
// Generates beep. Wrapper for BEEP routine.
inline void Bios_Beep() { Call(R_BEEP); }

// Function: Bios_ClearScreen
// Clears the screen. Wrapper for CLS routine.
void Bios_ClearScreen();

// Function: Bios_SetCursorPosition
// Moves cursor to the specified position. Wrapper for POSIT routine.
void Bios_SetCursorPosition(u8 X, u8 Y);

//=============================================================================
// Controller routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Controller
//-----------------------------------------------------------------------------

// Function: Bios_GetJoystickDirection
// Returns the joystick status. Wrapper for GTSTCK routine.
inline u8 Bios_GetJoystickDirection(u8 port) { return ((u8(*)(u8))R_GTSTCK)(port); }

// Function: Bios_GetJoystickTrigger
// Returns current trigger status. Wrapper for GTTRIG routine.
inline bool Bios_GetJoystickTrigger(u8 trigger) { return ((u8(*)(u8))R_GTTRIG)(trigger); }

//=============================================================================
// Misc routines
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - Misc
//-----------------------------------------------------------------------------

// Function: Bios_GetKeyboardMatrix
// Returns the value of the specified line from the keyboard matrix. Wrapper for SNSMAT routine.
inline u8 Bios_GetKeyboardMatrix(u8 line) { return ((u8(*)(u8))R_SNSMAT)(line); }

// Function: Bios_IsKeyPressed
// Check if the given key is pressed
inline bool Bios_IsKeyPressed(u8 key) {	return (g_NEWKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0; }

// Check if the given key is just pushed // don't work with standard system BIOS because g_OLDKEY is reset during ISR
/*inline bool Bios_IsKeyPushed(u8 key)
{
	u8 newKey = Bios_IsKeyPressed(key);
	u8 oldKey = (g_OLDKEY[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0;
	return newKey && !oldKey;
}*/

//=============================================================================
// MSX 2
//=============================================================================

//-----------------------------------------------------------------------------
// Group: Main-ROM - MSX2
//-----------------------------------------------------------------------------
#if (MSX_VERSION >= MSX_2)
#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
// MSX 2+
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

// Function: Bios_SetCPUMode
// Changes CPU mode. Wrapper for CHGCPU routine.
inline void Bios_SetCPUMode(u8 mode) { ((void(*)(u8))R_CHGCPU)(mode); }

// Function: Bios_GetCPUMode
// Returns current CPU mode. Wrapper for GETCPU routine.
inline u8 Bios_GetCPUMode() { return ((u8(*)(void))R_GETCPU)(); }

#endif // (MSX_VERSION == MSX_TR)
