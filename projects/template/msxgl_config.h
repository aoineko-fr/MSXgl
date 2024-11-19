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
// - TARGET_ROM_8K					8 KB ROM in page 1 (4000h ~ 5FFFh)
// - TARGET_ROM_8K_P2				8 KB ROM in page 2 (8000h ~ 9FFFh)
// - TARGET_ROM_16K					16 KB ROM in page 1 (4000h ~ 7FFFh)
// - TARGET_ROM_16K_P2				16 KB ROM in page 2 (8000h ~ BFFFh)
// - TARGET_ROM_32K					32 KB ROM in page 1&2 (4000h ~ BFFFh)
// - TARGET_ROM_48K					48 KB ROM in page 0-2 (0000h ~ BFFFh)
// - TARGET_ROM_48K_ISR				48 KB ROM in page 0-2 (0000h ~ BFFFh) with ISR replacement
// - TARGET_ROM_64K					64 KB ROM in page 0-3 (0000h ~ FFFFh)
// - TARGET_ROM_64K_ISR				64 KB ROM in page 0-3 (0000h ~ FFFFh) with ISR replacement
// - TARGET_ROM_ASCII8				ASCII-8: 8KB segments for a total of 64 KB to 2 MB
// - TARGET_ROM_ASCII16				ASCII-16: 16KB segments for a total of 64 KB to 4 MB
// - TARGET_ROM_KONAMI				Konami MegaROM (aka Konami4): 8 KB segments for a total of 64 KB to 2 MB
// - TARGET_ROM_KONAMI_SCC			Konami MegaROM SCC (aka Konami5): 8 KB segments for a total of 64 KB to 2 MB
// - TARGET_DOS1					MSX-DOS 1 program (starting at 0100h)
// - TARGET_DOS2					MSX-DOS 2 program (starting at 0100h)
// - TARGET_DOS0					Direct program boot from disk (starting at 0100h)
// - TARGET_BIN						BASIC binary program (starting at 8000h)
// - TARGET_BIN_USR					BASIC USR binary driver (starting at C000h)
// TARGET is defined by the build tool

// MSX version
// - MSX_1 ........................ MSX1
// - MSX_2 ........................ MSX2
// - MSX_12 ....................... MSX1 and 2 (support each)
// - MSX_2K ....................... Korean MSX2 (SC9 support)
// - MSX_2P ....................... MSX2+
// - MSX_22P ...................... MSX2 and 2+ (support each)
// - MSX_122P ..................... MSX1, 2 and 2+ (support each)
// - MSX_0 ........................ MSX0 (MSX2+)
// - MSX_TR ....................... MSX turbo R
// - MSX_3 ........................ MSX3
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
#define BIOS_USE_SUBROM				TRUE	// Allow use of Sub-ROM routines (MSX2/2+/turbo R)
#define BIOS_USE_DISKROM			TRUE	// Allow use of Disk-ROM routines

//-----------------------------------------------------------------------------
// VDP MODULE
//-----------------------------------------------------------------------------

// VRAM addressing unit
// - VDP_VRAM_ADDR_14 ............. Use 14-bits 16K VRAM addressing for MSX1 (u16)
// - VDP_VRAM_ADDR_17 ............. Use 17-bits 128K VRAM addressing for MSX2/2+/turbo R (u32)
#define VDP_VRAM_ADDR				VDP_VRAM_ADDR_14

// VDP X/Y units
// - VDP_UNIT_U8 .................. X and Y use 8-bits values
// - VDP_UNIT_X16 ................. X use 16-bits and Y use 8-bits values
// - VDP_UNIT_Y16 ................. X use 8-bits and Y use 16-bits values
// - VDP_UNIT_U16 ................. X and Y use 16-bits values
#define VDP_UNIT					VDP_UNIT_U8

// VDP screen modes (additionnal limitations come from the selected MSX_VERSION)
#define VDP_USE_MODE_T1				TRUE	// MSX1		Screen 0 Width 40
#define VDP_USE_MODE_G1				TRUE	// MSX1		Screen 1
#define VDP_USE_MODE_G2				TRUE	// MSX1		Screen 2
#define VDP_USE_MODE_MC				TRUE	// MSX1		Screen 3
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
#define VDP_USE_VALIDATOR			TRUE	// Handle some option specific for each VDP mode (highly recommended)
#define VDP_USE_DEFAULT_PALETTE		FALSE	// Add data for default MSX2 palette
#define VDP_USE_MSX1_PALETTE		FALSE	// Add data for default MSX1 palette
#define VDP_USE_DEFAULT_SETTINGS	TRUE	// Auto-initialization of common VDP feature
#define VDP_USE_16X16_SPRITE		TRUE	// Use 16x16 sprites mode
#define VDP_USE_RESTORE_S0			TRUE	// Do restore of status register pointer to S#0 (needed onlt for default BIOS ISR)
#define VDP_USE_PALETTE16			FALSE	// Use 16 entries palette (use only 15 entries otherwise)

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
#define VDP_INIT_50HZ				VDP_INIT_DEFAULT

//-----------------------------------------------------------------------------
// V9990  MODULE
//-----------------------------------------------------------------------------

// V9990 screen modes support
#define V9_USE_MODE_P1				TRUE	// Tile mode 0 256x212
#define V9_USE_MODE_P2				TRUE	// Tile mode 1 512x212
#define V9_USE_MODE_B0				TRUE	// Bitmap mode 1 192x240 (Undocumented v9990 mode)
#define V9_USE_MODE_B1				TRUE	// Bitmap mode 1 256x212
#define V9_USE_MODE_B2				TRUE	// Bitmap mode 2 384x240
#define V9_USE_MODE_B3				TRUE	// Bitmap mode 3 512x212
#define V9_USE_MODE_B4				TRUE	// Bitmap mode 4 768x240
#define V9_USE_MODE_B5				TRUE	// Bitmap mode 5 640x400 (VGA)
#define V9_USE_MODE_B6				TRUE	// Bitmap mode 6 640x480 (VGA)
#define V9_USE_MODE_B7				TRUE	// Bitmap mode 7 1024x212 (Undocumented v9990 mode)

#define V9_INT_PROTECT				TRUE	// VRAM access protection mode against interruption
// Palette input data format
// - V9_PALETTE_YSGBR_16 .......... 16 bits RGB + Ys [Ys|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
// - V9_PALETTE_GBR_16 ............ 16 bits RGB      [0|G|G|G|G|G|R|R] [R|R|R|B|B|B|B|B]
// - V9_PALETTE_RGB_24 ............ 24 bits RGB      [0|0|0|R|R|R|R|R] [0|0|0|G|G|G|G|G] [0|0|0|B|B|B|B|B]
#define V9_PALETTE_MODE				V9_PALETTE_RGB_24

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
// PADDLE MODULE
//-----------------------------------------------------------------------------

// Paddle module setting
#define PADDLE_USE_CALIB			TRUE	// Add functions paddle calibration feature

//-----------------------------------------------------------------------------
// MEMORY MODULE
//-----------------------------------------------------------------------------

#define MEM_USE_VALIDATOR			FALSE	// Activate validator to handle invalide input value
#define MEM_USE_FASTCOPY			FALSE	// Add support for fast-copy function (using unrolled-LDI loop)
#define MEM_USE_FASTSET				FALSE	// Add support for fast-set function (using unrolled-LDI loop)
#define MEM_USE_DYNAMIC				FALSE	// Add support for malloc style dynamic allocator
#define MEM_USE_BUILTIN				TRUE	// Use SDCC built-in memcpy and memset function instead of MSXgl ones

//-----------------------------------------------------------------------------
// MSX-DOS MODULE
//-----------------------------------------------------------------------------

#define DOS_USE_FCB					TRUE	// Add support for file managment features through FCB structure
#define DOS_USE_HANDLE				TRUE	// Add support for file managment features through file handle
#define DOS_USE_UTILITIES			TRUE	// Add support for file managment features through filename
#define DOS_USE_VALIDATOR			TRUE	// Add support for last error backup and return value validation

//-----------------------------------------------------------------------------
// CLOCK MODULE
//-----------------------------------------------------------------------------

#define RTC_USE_CLOCK				TRUE	// Add functions to handle date and time
#define RTC_USE_CLOCK_EXTRA			TRUE	// Add extra date and time functions that require additional data
#define RTC_USE_SAVEDATA			TRUE	// Add functions to read/write into the CMOS 
#define RTC_USE_SAVESIGNED			TRUE	// Add signature handling to validate CMOS I/O

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
// SPRITE FX MODULE
//-----------------------------------------------------------------------------

// Supported sprite size
#define SPRITEFX_USE_8x8			TRUE	// Use 8x8 pixels effects
#define SPRITEFX_USE_16x16			TRUE	// Use 16x16 pixels effects

// Supported effect
#define SPRITEFX_USE_CROP			TRUE	// Use cropping effect
#define SPRITEFX_USE_FLIP			TRUE	// Use flipping effect
#define SPRITEFX_USE_MASK			TRUE	// Use masking effect
#define SPRITEFX_USE_ROTATE			TRUE	// Use rotating effect

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
#define GAMEPAWN_BORDER_MIN_Y		0		// High border Y coordinade
#define GAMEPAWN_BORDER_MAX_Y		191		// Low border Y coordinate
#define GAMEPAWN_TILEMAP_WIDTH		32		// Width of the tiles map
#define GAMEPAWN_TILEMAP_HEIGHT		24		// Height of the tiles map
// Collision tilemap source
// - GAMEPAWN_TILEMAP_SRC_AUTO .... Backward compatibility option
// - GAMEPAWN_TILEMAP_SRC_RAM ..... Tilemap located in a buffer in RAM (best for performance)
// - GAMEPAWN_TILEMAP_SRC_VRAM .... Tilemap located in VRAM (slow but don't need additionnal data)
// - GAMEPAWN_TILEMAP_SRC_V9 ...... Tilemap located in V9990's VRAM
#define GAMEPAWN_TILEMAP_SRC		GAMEPAWN_TILEMAP_SRC_VRAM
// Pawn's sprite mode
// - GAMEPAWN_SPT_MODE_AUTO ....... Backward compatibility option
// - GAMEPAWN_SPT_MODE_MSX1 ....... Sprite Mode 1 (MSX1 screens)
// - GAMEPAWN_SPT_MODE_MSX2 ....... Sprite Mode 2 (MSX2 screens)
// - GAMEPAWN_SPT_MODE_V9_P1 ...... V9990 sprite in P1 mode
// - GAMEPAWN_SPT_MODE_V9_P2 ...... V9990 sprite in P2 mode
#define GAMEPAWN_SPT_MODE			GAMEPAWN_SPT_MODE_MSX1

//-----------------------------------------------------------------------------
// GAME MENU MODULE
//-----------------------------------------------------------------------------

#define MENU_USE_DEFAULT_CALLBACK	TRUE	// Use default input/print callback
#define MENU_SCREEN_WIDTH			MENU_VARIABLE		// Screen width
#define MENU_FRAME_X				0		// Frame position X
#define MENU_FRAME_Y				6		// Frame position Y
#define MENU_FRAME_WIDTH			32		// Frame width
#define MENU_FRAME_HEIGHT			8		// Frame height
#define MENU_CHAR_CLEAR				'\0'	// Clear character
#define MENU_CHAR_CURSOR			'@'		// Cursor character
#define MENU_CHAR_TRUE				'O'		// True character
#define MENU_CHAR_FALSE				'X'		// False character
#define MENU_CHAR_LEFT				'<'		// Left edit character
#define MENU_CHAR_RIGHT				'>'		// Right edit character
#define MENU_TITLE_X				4		// Title position X
#define MENU_TITLE_Y				6		// Title position Y
#define MENU_ITEM_X					6		// Item label X position
#define MENU_ITEM_Y					8		// Item label X position
#define MENU_ITEM_X_GOTO			6		// Goto type item label X position
#define MENU_ITEM_ALIGN				MENU_ITEM_ALIGN_LEFT // Item label alignment
#define MENU_ITEM_ALIGN_GOTO		MENU_ITEM_ALIGN_LEFT // Goto type item label alignment
#define MENU_VALUE_X				14		// Item value X position
// Type of cursor
// - MENU_CURSOR_MODE_NONE ........ No cursor
// - MENU_CURSOR_MODE_CHAR ........ Character cursor
// - MENU_CURSOR_MODE_SPRT ........ Sprite cursor
#define MENU_CURSOR_MODE			MENU_CURSOR_MODE_CHAR
#define MENU_CURSOR_OFFSET			(-2)	// Cursor X position offset

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
// TILE
//-----------------------------------------------------------------------------

#define TILE_WIDTH					8		// Tile width
#define TILE_HEIGHT					8		// Tile height
#define TILE_BPP					4		// Screen bits-per-pixel
#define TILE_SCREEN_WIDTH			256		// Screen width
#define TILE_SCREEN_HEIGHT			212		// Screen height
#define TILE_USE_SKIP				TRUE	// Skip drawing of a given index
#define TILE_SKIP_INDEX				0		// The index tile to skip

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

// PCMPlay
// - PCMPLAY_8K or PCMPLAY_11K
#define PCMPLAY_FREQ				PCMPLAY_8K
#define PCMPLAY_USE_RESTORE			FALSE

// PT3 options
#define PT3_SKIP_HEADER				TRUE	// Don't use PT3 data header (first 100 bytes must be truncated)
#define PT3_AUTOPLAY				TRUE	// Play music automatically
#define PT3_EXTRA					TRUE	// Add helper functions

// TriloTracker options
#define TRILO_USE_SFXPLAY			TRUE	// Add SFX playback through Trilo SCC player (ayFX + SCC format)
#define TRILO_USE_TREMOLO			TRUE	// Add support for tremolo effect (little bit expensive)
#define TRILO_USE_TAIL				FALSE	// Add tail to prevent volume to fall to zero

// LVGM replayer options
#define LVGM_USE_PSG				TRUE	// Add parser for PSG data
#define LVGM_USE_MSXMUSIC			TRUE	// Add parser for MSX-Music data
#define LVGM_USE_MSXAUDIO			TRUE	// Add parser for MSX-Audio data
#define LVGM_USE_SCC				TRUE	// Add parser for Konami SCC data
#define LVGM_USE_SCCI				FALSE	// Add parser for Konami SCC+ data
#define LVGM_USE_PSG2				FALSE	// Add parser for secondary PSG data
#define LVGM_USE_OPL4				FALSE	// Add parser for OPL4 data
#define LVGM_USE_NOTIFY				TRUE	// Add parser for PSG data

// WYZ Tracker replayer options
// Channels number
// - WYZ_3CH
// - WYZ_6CH
#define WYZ_CHANNELS				WYZ_3CH	// Number of supported channels (can be 3 for 1 PSG or 6 for 2 PSG)
#define WYZ_USE_DIRECT_ACCESS		FALSE	// Send data directly to PSG registers (otherwise, write in a RAM buffer)
#define WYZ_CHAN_BUFFER_SIZE		0x20	// Size of the channel buffer

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

// Pletter compression
#define PLETTER_LENGTHINDATA		FALSE	// Is length included in data (first 2 bytes)
// Disable interruption mode
// - PLETTER_DI_NONE .............. Don't disable interruption
// - PLETTER_DI_FULL .............. Disable interruption during the whole function
// - PLETTER_DI_LOOP .............. Disable interruption during VRAM write loop
#define PLETTER_DI_MODE				PLETTER_DI_LOOP
// VRAM write timing mode
// - PLETTER_WRITE_SAFE ........... Safe VRAM write speed (30 t-states)
// - PLETTER_WRITE_NODISPLAY ...... Safe VRAM write speed when screen display disable (22 t-states)
// - PLETTER_WRITE_MINIMAL ........ Minimal wait beetween write (17 t-states)
// - PLETTER_WRITE_QUICK .......... No wait beetween write (12 t-states)
// - PLETTER_WRITE_AUTO ........... Determine the worst case according to selected screen mode (12~30 t-states)
#define PLETTER_WRITE_MODE			PLETTER_WRITE_SAFE

// BitBuster compression
// VRAM write timing mode
// - BITBUSTER_WRITE_SAFE ......... Safe VRAM write speed (include nop between write)
// - BITBUSTER_WRITE_QUICK ........ No wait beetween write
#define BITBUSTER_WRITE_MODE		BITBUSTER_WRITE_SAFE

// ZX0 compression
// Unpack mode
// - ZX0_MODE_STANDARD ............ Standard routine: 68 bytes only
// - ZX0_MODE_TURBO ............... Turbo routine: 126 bytes, about 21% faster
// - ZX0_MODE_FAST ................ Fast routine: 187 bytes, about 25% faster
// - ZX0_MODE_MEGA ................ Mega routine: 673 bytes, about 28% faster
#define ZX0_MODE					ZX0_MODE_STANDARD

// LZ48 compression
// - LZ48_MODE_STANDARD ........... Standard routine
// - LZ48_MODE_SPEED .............. Version optimized for speed
// - LZ48_MODE_SIZE ............... Version optimized for size
#define LZ48_MODE					LZ48_MODE_STANDARD

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
// PAC MODULE
//-----------------------------------------------------------------------------

#define PAC_USE_SIGNATURE			TRUE	// Handle application signature to validate saved data
#define PAC_USE_VALIDATOR			TRUE	// Add code to validate input parameters 
#define PAC_DEVICE_MAX				4		// Maximum number of supported PAC devices
// SRAM access method
// - PAC_ACCESS_DIRECT ............ Direct access to SRAM (must be selected in page 1)
// - PAC_ACCESS_BIOS .............. Access through BIOS routines
// - PAC_ACCESS_SWITCH_BIOS ....... Access through BIOS routines with BIOS switched in
// - PAC_ACCESS_SYSTEM ............ Access through MSXgl routine (no need BIOS)
#define PAC_ACCESS					PAC_ACCESS_BIOS

//-----------------------------------------------------------------------------
// QR CODE MODULE
//-----------------------------------------------------------------------------

#define QRCODE_VERSION_MIN			1		// The minimum version number supported in the QR Code Model 2 standard
#define QRCODE_VERSION_MAX			20		// The maximum version number supported in the QR Code Model 2 standard
#define QRCODE_VERSION_CUSTOM		FALSE	// TRUE: Allow to define version using <QRCode_SetVersion>. FALSE: Use hardcoded min/max version
#define QRCODE_USE_BYTE_ONLY		TRUE	// TRUE: Allow only BYTE mode. FALSE allow all mode including NUMERIC, ALPHANUMERIC, KANJI and ECI.
#define QRCODE_USE_EXTRA			FALSE	// TRUE: Add extra function to generate custom data segments.
#define QRCODE_BOOST_ECL			FALSE	// If boostEcl is TRUE, then the ECC level of the result may be higher than the ecl argument if it can be done without increasing the version. QRCODE_PARAM_CUSTOM: Allow to use <QRCode_SetBoostECL> function.

#define QRCODE_TINY_VERSION			10		// The version number supported in the QR Code Model 2 standard
// Error correction level
// - QRCODE_ECC_LOW ............... The QR Code can tolerate about  7% erroneous codewords
// - QRCODE_ECC_MEDIUM ............ The QR Code can tolerate about 15% erroneous codewords
// - QRCODE_ECC_QUARTILE .......... The QR Code can tolerate about 25% erroneous codewords
// - QRCODE_ECC_HIGH .............. The QR Code can tolerate about 30% erroneous codewords
#define QRCODE_TINY_ECC				QRCODE_ECC_LOW
// Mask pattern
// - QRCODE_MASK_0 ................ (i + j) % 2 = 0
// - QRCODE_MASK_1 ................ i % 2 = 0
// - QRCODE_MASK_2 ................ j % 3 = 0
// - QRCODE_MASK_3 ................ (i + j) % 3 = 0
// - QRCODE_MASK_4 ................ (i / 2 + j / 3) % 2 = 0
// - QRCODE_MASK_5 ................ (i * j) % 2 + (i * j) % 3 = 0
// - QRCODE_MASK_6 ................ ((i * j) % 3 + i * j) % 2 = 0
// - QRCODE_MASK_7 ................ ((i * j) % 3 + i + j) % 2 = 0
#define QRCODE_TINY_MASK 			QRCODE_MASK_0

//-----------------------------------------------------------------------------
// DEBUG & PROFILE
//-----------------------------------------------------------------------------

// Debugger options
// - DEBUG_DISABLE ................ No debug tool
// - DEBUG_EMULICIOUS ............. Debug features for Emulicious
// - DEBUG_OPENMSX ................ Debug features for openMSX using 'debugdevice' extension
// - DEBUG_OPENMSX_P .............. Debug features for openMSX using PVM script (tools/script/openMSX/debugger_pvm.tcl)
#define DEBUG_TOOL					DEBUG_DISABLE
// Profiler options
// - PROFILE_DISABLE .............. No profile tool
// - PROFILE_OPENMSX_G ............ Profiler features for openMSX using Grauw script (tools/script/openMSX/profiler_grauw.tcl)
// - PROFILE_OPENMSX_S ............ Profiler features for openMSX using Salutte script (tools/script/openMSX/profiler_salutte.tcl)
#define PROFILE_TOOL				PROFILE_DISABLE
#define PROFILE_LEVEL				10