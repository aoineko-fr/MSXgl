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
#define VERSION_CURRENT				MSXGL_VERSION
#define SDCC_VERSION_CURRENT		SDCC_VERSION

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
#define Menu_DrawPage				Menu_Display

// Game pawn module
#define GAMEPAWN_BORDER_NONE		PAWN_BORDER_NONE
#define GAMEPAWN_BORDER_DOWN		PAWN_BORDER_DOWN
#define GAMEPAWN_BORDER_UP			PAWN_BORDER_UP
#define GAMEPAWN_BORDER_RIGHT		PAWN_BORDER_RIGHT
#define GAMEPAWN_BORDER_LEFT		PAWN_BORDER_LEFT
#define GAMEPAWN_COL_NONE			PAWN_COL_NONE
#define GAMEPAWN_COL_0				PAWN_COL_0
#define GAMEPAWN_COL_25				PAWN_COL_25
#define GAMEPAWN_COL_50				PAWN_COL_50
#define GAMEPAWN_COL_75				PAWN_COL_75
#define GAMEPAWN_COL_100			PAWN_COL_100
#define GAMEPAWN_COL_1P_MIDDLE		PAWN_COL_1P_MIDDLE
#define GAMEPAWN_COL_2P_MIDDLE		PAWN_COL_2P_MIDDLE
#define GAMEPAWN_COL_2P_CORNER		PAWN_COL_2P_CORNER
#define GAMEPAWN_BOUND_CUSTOM		PAWN_BOUND_CUSTOM
#define GAMEPAWN_TILEMAP_SRC_AUTO	PAWN_TILEMAP_SRC_AUTO
#define GAMEPAWN_TILEMAP_SRC_RAM	PAWN_TILEMAP_SRC_RAM
#define GAMEPAWN_TILEMAP_SRC_VRAM	PAWN_TILEMAP_SRC_VRAM
#define GAMEPAWN_TILEMAP_SRC_V9		PAWN_TILEMAP_SRC_V9
#define GAMEPAWN_SPT_MODE_AUTO		PAWN_SPT_MODE_AUTO
#define GAMEPAWN_SPT_MODE_MSX1		PAWN_SPT_MODE_MSX1
#define GAMEPAWN_SPT_MODE_MSX2		PAWN_SPT_MODE_MSX2
#define GAMEPAWN_SPT_MODE_V9_P1		PAWN_SPT_MODE_V9_P1
#define GAMEPAWN_SPT_MODE_V9_P2		PAWN_SPT_MODE_V9_P2
#define GAMEPAWN_SPT_MODE_BITMAP	PAWN_SPT_MODE_BITMAP
// #define GAMEPAWN_ID_PER_LAYER		PAWN_ID_PER_LAYER
// #define GAMEPAWN_USE_PHYSICS		PAWN_USE_PHYSICS
// #define GAMEPAWN_BOUND_X			PAWN_BOUND_X
// #define GAMEPAWN_BOUND_Y			PAWN_BOUND_Y
// #define GAMEPAWN_COL_DOWN			PAWN_COL_DOWN
// #define GAMEPAWN_COL_UP				PAWN_COL_UP
// #define GAMEPAWN_COL_RIGHT			PAWN_COL_RIGHT
// #define GAMEPAWN_COL_LEFT			PAWN_COL_LEFT
// #define GAMEPAWN_BORDER_EVENT		PAWN_BORDER_EVENT
// #define GAMEPAWN_BORDER_BLOCK		PAWN_BORDER_BLOCK
// #define GAMEPAWN_BORDER_MIN_Y		PAWN_BORDER_MIN_Y
// #define GAMEPAWN_BORDER_MAX_Y		PAWN_BORDER_MAX_Y
// #define GAMEPAWN_TILEMAP_WIDTH		PAWN_TILEMAP_WIDTH
// #define GAMEPAWN_TILEMAP_HEIGHT		PAWN_TILEMAP_HEIGHT
// #define GAMEPAWN_TILEMAP_SRC		PAWN_TILEMAP_SRC
// #define GAMEPAWN_SPT_MODE			PAWN_SPT_MODE
#define g_GamePawn_TileMap			g_Pawn_TileMap
#define g_Game_CellX				g_Pawn_CellX
#define g_Game_CellY				g_Pawn_CellY
#define GamePawn_Initialize			Pawn_Initialize
#define GamePawn_SetPosition		Pawn_SetPosition
#define GamePawn_SetAction			Pawn_SetAction
#define GamePawn_SetTileMap			Pawn_SetTileMap
#define GamePawn_SetEnable			Pawn_SetEnable
#define GamePawn_Disable			Pawn_Disable
#define GamePawn_Enable				Pawn_Enable
#define GamePawn_Update				Pawn_Update
#define GamePawn_Draw				Pawn_Draw
#define GamePawn_SetMovement		Pawn_SetMovement
#define GamePawn_SetTargetPosition	Pawn_SetTargetPosition
#define GamePawn_InitializePhysics	Pawn_InitializePhysics
#define GamePawn_GetPhysicsState	Pawn_GetPhysicsState
#define GamePawn_GetCallbackCellX	Pawn_GetCallbackCellX
#define GamePawn_GetCallbackCellY	Pawn_GetCallbackCellY
#define Game_Pawn					Pawn
#define Game_Frame					Pawn_Frame
#define Game_Action					Pawn_Action
#define Game_Sprite					Pawn_Sprite
#define Game_PhysicsCB				Pawn_PhysicsCB
#define Game_CollisionCB			Pawn_CollisionCB