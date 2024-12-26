// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█		   │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//	by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Deprecated symbols for backward compatibility
//─────────────────────────────────────────────────────────────────────────────
#pragma once

// Core
#define true						TRUE
#define false						FALSE
#define null						NULL

// VDP module
#define VDP_HideSpriteFrom			VDP_DisableSpritesFrom
#define VADDR_Lo					VADDR_LO
#define VADDR_Hi					VADDR_HI
#define g_SpriteAtributeLow			g_SpriteAttributeLow
#define g_SpriteAtributeHigh		g_SpriteAttributeHigh

// Compress module
#define UnpackRLEpToRAM				RLEp_UnpackToRAM

// Input module
#define JOY_INPUT_DIR_UP_RIGTH		JOY_INPUT_DIR_UP_RIGHT
#define JOY_INPUT_DIR_DOWN_RIGTH	JOY_INPUT_DIR_DOWN_RIGHT
#define Joystick_GetTrigger			Joystick_IsButtonPressed

// Compress module
#define ZX0_Unpack					ZX0_UnpackToRAM
#define Pletter_Unpack				Pletter_UnpackToRAM
#define Bitbuster_Unpack			Bitbuster_UnpackToRAM

// BIOS module
#define Bios_InterSlotCallF			BIOS_CALLF
#define Bios_Reboot					Bios_Startup
#define Bios_MainCall				Call

// Math module
#define Abs8						ABS8
#define Abs16						ABS16
#define Abs32						ABS32
#define Invert						INVERT
#define Merge44						MERGE44
#define Merge88						MERGE88
#define ModuloPow2					MOD_POW2
#define Clamp8						CLAMP8
#define Clamp16						CLAMP16
#define Max							MAX
#define Min							MIN

// Memory module
#define Mem_ChunkInitialize			Mem_DynamicInitialize
#define Mem_ChunkAlloc				Mem_DynamicAlloc
#define Mem_ChunkFree				Mem_DynamicFree
#define Mem_GetChunkSize			Mem_GetDynamicSize

// MSX-DOS module
#define DOS_NO_ERROR				DOS_ERR_NONE
#define FCB							DOS_FCB
#define DOS2_Open					DOS_FOpen
#define DOS2_Create					DOS_FCreate
#define DOS2_Close					DOS_FClose
#define DOS2_Duplicate				DOS_FDuplicate
#define DOS2_Read					DOS_FRead
#define DOS2_Write					DOS_FWrite
#define DOS2_LSeek					DOS_FSeek
#define DOS2_Exit					DOS_Exit
#define DOS_Open					DOS_OpenFCB
#define DOS_GetSize					DOS_GetSizeFCB
#define DOS_Create					DOS_CreateFCB
#define DOS_Close					DOS_CloseFCB
#define DOS_SequentialRead			DOS_SequentialReadFCB
#define DOS_SequentialWrite			DOS_SequentialWriteFCB
#define DOS_RandomBlockRead			DOS_RandomBlockReadFCB
#define DOS_RandomBlockWrite		DOS_RandomBlockWriteFCB
#define DOS_FindFirstFile			DOS_FindFirstFileFCB
#define DOS_FindNextFile			DOS_FindNextFileFCB

// Clock module
#define Clock_Initialize			RTC_Initialize
#define Clock_SetMode				RTC_SetMode
#define Clock_GetSecond				RTC_GetSecond
#define Clock_GetMinute				RTC_GetMinute
#define Clock_GetHour				RTC_GetHour
#define Clock_GetDayOfWeek			RTC_GetDayOfWeek
#define Clock_GetDay				RTC_GetDay
#define Clock_GetMonth				RTC_GetMonth
#define Clock_GetYear				RTC_GetYear
#define Clock_Read					RTC_Read
#define Clock_Write					RTC_Write
#define Clock_GetDayOfWeekString	RTC_GetDayOfWeekString
#define Clock_GetMonthString		RTC_GetMonthString
#define Clock_GetYear4				RTC_GetYear4

// System module
#define Sys_GetHeardAddr            Sys_GetHeaderAddr
#define POKE						Poke
#define POKEW						Poke16
#define PEEK						Peek
#define PEEKW						Poke16

// V9990 module
#define V9_SetLayerPalette          V9_SelectPaletteP1
#define V9_SetMode					V9_SetScreenMode
#define V9_SetSpritePalette			V9_SetSpritePaletteOffset

// Menu module
#define MENU_CLEAR					MENU_CHAR_CLEAR
#define MENU_POS_X					MENU_FRAME_X
#define MENU_POS_Y					MENU_FRAME_Y
#define MENU_WIDTH					MENU_FRAME_WIDTH
#define MENU_HEIGHT					MENU_FRAME_HEIGHT
