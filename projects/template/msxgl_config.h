// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Library configuration
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// BUILD
//-----------------------------------------------------------------------------

// Target
// - TARGET_BIN ...................	BASIC binary program
// - TARGET_ROM16 ................. 16KB ROM in page 1
// - TARGET_ROM16P2 ............... 16KB ROM in page 2
// - TARGET_ROM32 ................. 32KB ROM in page 1-2
// - TARGET_ROM32P0 ............... 32KB ROM in page 0-1
// - TARGET_ROM48 ................. 48KB ROM in page 0-2
// - TARGET_DOS ................... MSX-DOS program
// - TARGET_DOSARG ................ MSX-DOS program (using command line arguments)
// TARGET is defined by the build tool

// Target type
// - TYPE_BIN ..................... BASIC binary program
// - TYPE_ROM ..................... ROM
// - TYPE_DOS ..................... MSX-DOS program
// TARGET_TYPE is defined by the build tool

// MSX version
// - MSX_1 ........................ MSX
// - MSX_2 ........................ MSX 2
// - MSX_2P ....................... MSX 2+
// - MSX_TR ....................... MSX turbo R
// MSX_VERSION is defined by the build tool

//-----------------------------------------------------------------------------
// BIOS MODULE
//-----------------------------------------------------------------------------

// Default slot BIOS access
// - BIOS_CALL_DIRECT ............. Use direct access to Bios routines (ROM slot must be selected in corresponding page)
// - BIOS_CALL_INTERSLOT .......... Use inter-slot access to Bios routines (through CALSLT routine)
#define BIOS_CALL_MAINROM			BIOS_CALL_DIRECT
#define BIOS_CALL_SUBROM			BIOS_CALL_INTERSLOT
#define BIOS_CALL_DISKROM			BIOS_CALL_INTERSLOT

// MAIN-Bios module setting
#define BIOS_USE_MAINROM			TRUE	// Allow use of Main-ROM routines
#define BIOS_USE_VDP				TRUE	// Give access to Main-ROM routines related to VDP
#define BIOS_USE_PSG				TRUE	// Give access to Main-ROM routines related to PSG
#define BIOS_USE_SUBROM				TRUE	// Allow use of Sub-ROM routines (MSX 2/2+/turbo R)
#define BIOS_USE_DISKROM			TRUE	// Allow use of Disk-ROM routines

//-----------------------------------------------------------------------------
// VDP MODULE
//-----------------------------------------------------------------------------

// VRAM addressing unit
// - VDP_VRAM_ADDR_14 ............. Use 14-bits 16K VRAM addressing for MSX 1 (u16)
// - VDP_VRAM_ADDR_17 ............. Use 17-bits 128K VRAM addressing for MSX 2/2+/turbo R (u32)
#define VDP_VRAM_ADDR				VDP_VRAM_ADDR_14

// VDP X/Y units
// - VDP_UNIT_U8 .................. X and Y use 8-bits values
// - VDP_UNIT_X16 ................. X use 16-bits and Y use 8-bits values
// - VDP_UNIT_Y16 ................. X use 8-bits and Y use 16-bits values
// - VDP_UNIT_U16 ................. X and Y use 16-bits values
#define VDP_UNIT					VDP_UNIT_U8

// VDP screen modes (additionnal limitations come from the selected MSX_VERSION)
#define VDP_USE_MODE_T1				TRUE	// MSX1		Screen 0 Width 40
#define VDP_USE_MODE_MC				TRUE	// MSX1		Screen 3
#define VDP_USE_MODE_G1				TRUE	// MSX1		Screen 1
#define VDP_USE_MODE_G2				TRUE	// MSX1		Screen 2
#define VDP_USE_MODE_T2				FALSE	// MSX2		Screen 0 Width 80
#define VDP_USE_MODE_G3				FALSE	// MSX2		Screen 4
#define VDP_USE_MODE_G4				FALSE	// MSX2		Screen 5
#define VDP_USE_MODE_G5				FALSE	// MSX2		Screen 6
#define VDP_USE_MODE_G6				FALSE	// MSX2		Screen 7
#define VDP_USE_MODE_G7				FALSE	// MSX2/2+	Screen 8, 10, 11 & 12

#define VDP_USE_VRAM16K				TRUE	// Use 16K VRAM access functions on MSX2
#define VDP_USE_SPRITE				TRUE	// Use sprite handling functions
#define VDP_USE_COMMAND				TRUE	// Use VDP commands wrapper functions
#define VDP_USE_CUSTOM_CMD			FALSE	// Use custom VDP commands through data buffer
#define VDP_AUTO_INIT				TRUE	// Call VDP_Initialize() at the first call to VDP_SetMode()
#define VDP_USE_UNDOCUMENTED		TRUE	// Allow the use of undocumented screen mode (WIP)
#define VDP_USE_VALIDATOR			TRUE	// Let the engine fix some invalid input (highly recommended)
#define VDP_USE_DEFAULT_PALETTE		FALSE	// Add data for default MSX 2 palette
#define VDP_USE_MSX1_PALETTE		FALSE	// Add data for default MSX 1 palette
#define VDP_USE_DEFAULT_SETTINGS	TRUE	// Auto-initialization of common VDP feature
#define VDP_USE_16X16_SPRITE		TRUE	// Use 16x16 sprites mode
#define VDP_USE_RESTORE_S0			TRUE	// Do restore of status register pointer to S#0 (needed onlt for default BIOS ISR)

// ISR protection while modifiying VDP registers
// - VDP_ISR_SAFE_NONE ............ No ISR protection (for program not using VDP interruption)
// - VDP_ISR_SAFE_DEFAULT ......... Protect only VDP register pair writing (default behavior; ISR can read/write registers but VRAM ones)
// - VDP_ISR_SAFE_ALL ............. Protect all VDP writing process
#define VDP_ISR_SAFE_MODE			VDP_ISR_SAFE_DEFAULT

// Initial screen mode setting
// - VDP_INIT_OFF ................. Force option to be disable
// - VDP_INIT_ON .................. Force option to be enable
// - VDP_INIT_AUTO ................ Determining the best value for the context
// - VDP_INIT_DEFAULT ............. Keep default value
#define VDP_INIT_50HZ				VDP_INIT_ON

//-----------------------------------------------------------------------------
// INPUT MODULE
//-----------------------------------------------------------------------------

// Input module setting
#define INPUT_USE_JOYSTICK			TRUE	// Add functions to handle joystick using I/O port
#define INPUT_USE_KEYBOARD			TRUE	// Add functions to handle keyboard using I/O port
#define INPUT_USE_MOUSE				FALSE	// Add support for Mouse handling functions
#define INPUT_USE_DETECT			FALSE	// Add feature to detect device plugged in General purpose ports
#define INPUT_USE_ISR_PROTECTION	TRUE	// Disable interruptions while access PSG registers (needed if you use BIOS or access PSG in your own ISR)
#define INPUT_JOY_UPDATE			FALSE	// Add function to update all joystick states at once
// Key update handler
#define INPUT_KB_UPDATE				FALSE	// Add function to update all keyboard rows at once
#define INPUT_KB_UPDATE_MIN			0		// First row to update
#define INPUT_KB_UPDATE_MAX			8		// Last row to update (10 for numerical-pad, 8 otherwise)

//-----------------------------------------------------------------------------
// MEMORY MODULE
//-----------------------------------------------------------------------------

#define MEM_USE_VALIDATOR			FALSE	// Activate validator to handle invalide input value
#define MEM_USE_FASTCOPY			FALSE	// Add support for fast-copy function (using unrolled-LDI loop)
#define MEM_USE_FASTSET				FALSE	// Add support for fast-set function (using unrolled-LDI loop)
#define MEM_USE_DYNAMIC				FALSE	// Add support for malloc style dynamic allocator

//-----------------------------------------------------------------------------
// MSX-DOS MODULE
//-----------------------------------------------------------------------------

#define DOS_USE_FCB					TRUE	// Add support for file managment features through FCB structure
#define DOS_USE_HANDLE				TRUE	// Add support for file managment features through file handle
#define DOS_USE_UTILITIES			TRUE	// Add support for file managment features through filename
#define DOS_USE_VALIDATOR			TRUE	// Add support for last error backup and return value validation

//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// Print module setting
#define PRINT_USE_TEXT				TRUE	// Allow use of Text font (T1-T2, G1-G3)
#define PRINT_USE_BITMAP			FALSE	// Allow use of Bitmap font (G4-G7)
#define PRINT_USE_VRAM				FALSE	// Allow use of VRAM stored font (G4-G7)
#define PRINT_USE_SPRITE			FALSE	// Allow use of Sprite font (G3-G7)
#define PRINT_USE_FX_SHADOW			FALSE	// [Bitmap] Allow use of text shadow
#define PRINT_USE_FX_OUTLINE		FALSE	// [Bitmap] Allow use of text outline
#define PRINT_USE_2_PASS_FX			FALSE	// [Bitmap] Allow use 2-pass FX render to prevent character overlap
#define PRINT_USE_GRAPH				TRUE	// Allow use of character lines and boxes
#define PRINT_USE_VALIDATOR			TRUE	// Add validator character code
#define PRINT_USE_UNIT				FALSE	// Display integer type (h: hexadecimal, b: binary)
#define PRINT_USE_FORMAT			TRUE	// Add printf type function
#define PRINT_USE_32B				TRUE	// Allow to print 32-bits integers
#define PRINT_SKIP_SPACE			FALSE	// Skill space character
#define PRINT_COLOR_NUM				12		// 1 color per line
// Character width
// - PRINT_WIDTH_1 (text mode)
// - PRINT_WIDTH_6
// - PRINT_WIDTH_8
// - PRINT_WIDTH_X (variable)
#define PRINT_WIDTH					PRINT_WIDTH_1
// Character height
// - PRINT_HEIGHT_1 (text mode)
// - PRINT_HEIGHT_8
// - PRINT_HEIGHT_X (variable)
#define PRINT_HEIGHT				PRINT_HEIGHT_1

//-----------------------------------------------------------------------------
// GAME MODULE
//-----------------------------------------------------------------------------

// Game state setting
#define GAME_USE_STATE				TRUE	// Add state machine features
#define GAME_USE_VSYNC				TRUE	// Add vertical synchronization features
#define GAME_USE_LOOP				TRUE	// Add game main loop with call to v-synch and state

//-----------------------------------------------------------------------------
// GAME PAWN MODULE
//-----------------------------------------------------------------------------

// GamePawn setting
#define GAMEPAWN_ID_PER_LAYER		FALSE	// Set sprite ID for each layer (otherwise set per pawn)
#define GAMEPAWN_USE_PHYSICS		TRUE	// Add physics and collision features
// Pawn's bound (can be fixed for all pawn, or setable for each one)
#define GAMEPAWN_BOUND_X			GAMEPAWN_BOUND_CUSTOM
#define GAMEPAWN_BOUND_Y			GAMEPAWN_BOUND_CUSTOM
// Collision position options for each pawn's side
// - GAMEPAWN_COL_0
// - GAMEPAWN_COL_25
// - GAMEPAWN_COL_50
// - GAMEPAWN_COL_75
// - GAMEPAWN_COL_100
#define GAMEPAWN_COL_DOWN			(GAMEPAWN_COL_25|GAMEPAWN_COL_75)
#define GAMEPAWN_COL_UP				GAMEPAWN_COL_50
#define GAMEPAWN_COL_RIGHT			GAMEPAWN_COL_50
#define GAMEPAWN_COL_LEFT			GAMEPAWN_COL_50
// Options to determine which border collide or trigger events
// - GAMEPAWN_BORDER_NONE
// - GAMEPAWN_BORDER_DOWN
// - GAMEPAWN_BORDER_UP
// - GAMEPAWN_BORDER_RIGHT
// - GAMEPAWN_BORDER_LEFT
#define GAMEPAWN_BORDER_EVENT		(GAMEPAWN_BORDER_DOWN|GAMEPAWN_BORDER_RIGHT)
#define GAMEPAWN_BORDER_BLOCK		(GAMEPAWN_BORDER_UP|GAMEPAWN_BORDER_LEFT)
// Top/bottom border position (in pixel)
#define GAMEPAWN_BORDER_MIN_Y		0
#define GAMEPAWN_BORDER_MAX_Y		191

//-----------------------------------------------------------------------------
// GAME MENU MODULE
//-----------------------------------------------------------------------------

#define MENU_USE_DEFAULT_CALLBACK	TRUE	// Use default input/print callback
#define MENU_SCREEN_WIDTH			32		// Screen width
#define MENU_CLEAR					0		// Clear character
#define MENU_POS_X					4		// Frame position X
#define MENU_POS_Y					11		// Frame position Y
#define MENU_WIDTH					24		// Frame width
#define MENU_HEIGHT					8		// Frame height
#define MENU_CHAR_CURSOR			'@'		// Cursor charactter
#define MENU_CHAR_TRUE				'O'		// True charactter
#define MENU_CHAR_FALSE				'X'		// False charactter
#define MENU_CHAR_LEFT				'<'		// Left edit charactter
#define MENU_CHAR_RIGHT				'>'		// Right edit charactter
// Type of cursor
// - MENU_CURSOR_MODE_NONE			No cursor
// - MENU_CURSOR_MODE_CHAR			Character cursor
// - MENU_CURSOR_MODE_SPRT			Sprite cursor
#define MENU_CURSOR_MODE			MENU_CURSOR_MODE_CHAR
#define MENU_CURSOR_OFFSET			(-1)	// Cursor X position offset
#define MENU_ITEM_X					6		// Item label X position
#define MENU_ITEM_X_GOTO			6		// Goto type item label X position
#define MENU_ITEM_ALIGN				MENU_ITEM_ALIGN_LEFT // Item label alignment
#define MENU_ITEM_ALIGN_GOTO		MENU_ITEM_ALIGN_LEFT // Goto type item label alignment
#define MENU_VALUE_X				13		// Item value X position

//-----------------------------------------------------------------------------
// STRING MODULE
//-----------------------------------------------------------------------------

// Support for integer to ASCII string converter functions
#define STRING_USE_FROM_INT8		TRUE	// Signed 8-bits integer to string
#define STRING_USE_FROM_UINT8		TRUE	// Unsigned 8-bits integer to string
#define STRING_USE_FROM_INT16		TRUE	// Signed 16-bits integer to string
#define STRING_USE_FROM_UINT16		TRUE	// Unsigned 16-bits integer to string

// Support for sprintf style formating function
#define STRING_USE_FORMAT			TRUE
#define STRING_USE_INT32			TRUE	// Add support for 32-bits integer

//-----------------------------------------------------------------------------
// SCROLL MODULE 
//-----------------------------------------------------------------------------

// Allow horizontal and/or vertical scrolling
#define SCROLL_HORIZONTAL			TRUE	// Activate horizontal scrolling
#define SCROLL_VERTICAL				TRUE	// Activate vertical scrolling
// Source data info
#define SCROLL_SRC_X				64		// Start X coordinate of the source data
#define SCROLL_SRC_Y				0		// Start Y coordinate of the source data
#define SCROLL_SRC_W				128		// Width of the source data
#define SCROLL_SRC_H				24		// Height of the source data
// Destination data info
#define SCROLL_DST_X				0		// Destination x coordinate (in layout table)
#define SCROLL_DST_Y				2		// Destination y coordinate (in layout table)
#define SCROLL_DST_W				32		// Destination width
#define SCROLL_DST_H				20		// Destination height
#define SCROLL_SCREEN_W				32		// Screen width in tile number
// Allow scroll data looping (only for horizontal scrolling)
#define SCROLL_WRAP					TRUE
// Use screen position adjust register (allow per-pixel scrolling) [MSX2]
#define SCROLL_ADJUST				FALSE	// Global ajustement
#define SCROLL_ADJUST_SPLIT			FALSE	// Destination windows ajustement using screen split
// Use sprite mask (allow smooth per-pixel scrolling) [MSX2]
#define SCROLL_MASK					FALSE	// Use sprite to mask
#define SCROLL_MASK_ID				0		// First sprite ID to use
#define SCROLL_MASK_COLOR			COLOR_BLACK // Must be the same than border color
#define SCROLL_MASK_PATTERN			0		// Sprite pattern to use

//-----------------------------------------------------------------------------
// AUDIO 
//-----------------------------------------------------------------------------

// PSG options
// - PSG_INTERNAL ................. Use internal PSG chip (port A0-A2)
// - PSG_EXTERNAL ................. Use external PSG chip (port 10-12)
// - PSG_BOTH ..................... Use both internal and external PSG chips
#define PSG_CHIP					PSG_INTERNAL
// - PSG_DIRECT ................... Function set directly the PSG registers
// - PSG_INDIRECT ................. Function set a buffer (Apply() function must be call once a frame)
#define PSG_ACCESS					PSG_INDIRECT
#define PSG_USE_NOTES				FALSE	// Add notes table to convert note to tone
#define PSG_USE_EXTRA				TRUE	// Add helper functions to handle PSG settings
#define PSG_USE_RESUME				TRUE	// Add function to allow playback pause and resume

// MSX-Music options
#define MSXAUDIO_USE_RESUME			TRUE	// Add function to allow playback pause and resume

// MSX-Audio options
#define MSXMUSIC_USE_RESUME			TRUE	// Add function to allow playback pause and resume

// SCC options
#define SCC_USE_EXTA				TRUE	// Add helper functions to handle PSG settings
#define SCC_USE_RESUME				TRUE	// Add function to allow playback pause and resume
// - SCC_SLOT_DIRECT .............. Program on a SCC cartridge
// - SCC_SLOT_FIXED	............... Fixed slot-id (non-expanded second cartridge slot)
// - SCC_SLOT_USER ................ Defined by the user
// - SCC_SLOT_AUTO ................ First auto-detected cartridge
#define SCC_SLOT_MODE				SCC_SLOT_AUTO

// VGM supported chip
#define VGM_USE_PSG					TRUE	// Allow PSG data parsing and audio output
#define VGM_USE_MSXMUSIC			TRUE	// Allow MSX-Music data parsing and audio output
#define VGM_USE_MSXAUDIO			TRUE	// Allow MSX-Audio data parsing and audio output
#define VGM_USE_SCC					TRUE	// Allow SCC data parsing and audio output

// PCM-Encoder supported frequency (more than 1 value allowed)
// - PCMENC_NONE, PCMENC_8K, PCMENC_11K, PCMENC_22K and PCMENC_44K
#define PCMENC_FREQ					PCMENC_8K | PCMENC_11K | PCMENC_22K | PCMENC_44K

// PT3 options
#define PT3_SKIP_HEADER				TRUE	// Don't use PT3 data header (first 100 bytes must be truncated)
#define PT3_AUTOPLAY				TRUE	// Play music automatically
#define PT3_EXTRA					TRUE	// Add helper functions

// TriloTracker options
#define TRILO_USE_SFXPLAY			TRUE	// Add SFX playback through Trilo SCC player (ayFX + SCC format)
#define TRILO_USE_TREMOLO			TRUE	// Add support for tremolo effect (little bit expensive)
#define TRILO_USE_TAIL				FALSE	// Add tail to prevent volume to fall to zero

//-----------------------------------------------------------------------------
// MATH MODULE
//-----------------------------------------------------------------------------

// Random methods
// - RANDOM_8_NONE ................ No 8-bits random
// - RANDOM_8_REGISTER ............ R Register value (7-bits)
// - RANDOM_8_RACC ................ R Register accumulation (7-bits)
// - RANDOM_8_ION ................. Ion Random
// - RANDOM_8_MEMORY .............. Memory Peek from R
#define RANDOM_8_METHOD				RANDOM_8_ION
// - RANDOM_16_NONE ............... No 16-bits random
// - RANDOM_16_LINEAR ............. Linear congruential
// - RANDOM_16_XORSHIFT ........... XOR Shift
// - RANDOM_16_LFSR_LCG ........... Combined LFSR/LCG
#define RANDOM_16_METHOD			RANDOM_16_XORSHIFT

//-----------------------------------------------------------------------------
// COMPRESS 
//-----------------------------------------------------------------------------

// RLEp compression
#define COMPRESS_USE_RLEP			TRUE	// Use RLEp unpacker
#define COMPRESS_USE_RLEP_DEFAULT	TRUE	// Data include the default value (0 otherwise)
#define COMPRESS_USE_RLEP_FIXSIZE	FALSE	// Gize the data size as function input (loop up to terminator otherwise)

//-----------------------------------------------------------------------------
// MSXi MODULE
//-----------------------------------------------------------------------------

// MSXi compressor support
#define MSXi_USE_COMP_NONE			TRUE
#define MSXi_USE_COMP_CROP16		TRUE
#define MSXi_USE_COMP_CROP32		TRUE
#define MSXi_USE_COMP_CROP256		TRUE
#define MSXi_USE_COMP_CROPLINE16	TRUE
#define MSXi_USE_COMP_CROPLINE32	TRUE
#define MSXi_USE_COMP_CROPLINE256	TRUE
#define MSXi_USE_COMP_RLE0			TRUE
#define MSXi_USE_COMP_RLE4			TRUE
#define MSXi_USE_COMP_RLE8			TRUE

//-----------------------------------------------------------------------------
// NINJATAP MODULE
//-----------------------------------------------------------------------------

// Supported driver(s)
// - NTAP_DRIVER_MSXGL ............ MSXgl custom driver (based on DM-System2 one)
// - NTAP_DRIVER_GIGAMIX .......... Original Gigamix's DM-System2 driver
// - NTAP_DRIVER_SHINOBI .......... Shinobi Tap driver by Danjovic
#define NTAP_DRIVER					NTAP_DRIVER_MSXGL | NTAP_DRIVER_GIGAMIX | NTAP_DRIVER_SHINOBI
#define NTAP_USE_PREVIOUS			TRUE	// Backup previous data to allow push/release detection

//-----------------------------------------------------------------------------
// DEBUG
//-----------------------------------------------------------------------------

// Profiler method
// - DEBUG_DISABLE ................ No profiler
// - DEBUG_OPENMSX_G .............. Grauw profile script for OpenMSX
// - DEBUG_OPENMSX_S .............. Salutte profile script for OpenMSX
// - DEBUG_EMULICIOUS ............. Profile script for Emulicious
#define DEBUG_TOOL					DEBUG_DISABLE
#define PROFILE_LEVEL				10 
