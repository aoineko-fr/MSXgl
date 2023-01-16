// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS 2 Memory Mapper extension BIOS routines
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

// Memory Mapper routines
#define DOS_FUNC_SET				0x00 // Get the information table on the main Memory Mapper
#define DOS_FUNC_GET				0x01 // Get the variable table from all Memory Mappers
#define DOS_FUNC_CALL				0x02 // Get the jump table to the routines of the Memory Mapper

// Address entry name function
#define DOS_ALL_SEG					0x00 // Allocate a 16k segment.
#define DOS_FRE_SEG					0x03 // Free a 16k segment.
#define DOS_RD_SEG					0x06 // Read byte from address A:HL to A.
#define DOS_WR_SEG					0x09 // Write byte from E to address A:HL.
#define DOS_CAL_SEG					0x0C // Inter-segment call.  Address in IYh:IX
#define DOS_CALLS					0x0F // Inter-segment call.  Address in line after the call instruction.
#define DOS_PUT_PH					0x12 // Put segment into page (HL).
#define DOS_GET_PH					0x15 // Get current segment for page (HL)
#define DOS_PUT_P0					0x18 // Put segment into page 0.
#define DOS_GET_P0					0x1B // Get current segment for page 0.
#define DOS_PUT_P1					0x1E // Put segment into page 1.
#define DOS_GET_P1					0x21 // Get current segment for page 1.
#define DOS_PUT_P2					0x24 // Put segment into page 2.
#define DOS_GET_P2					0x27 // Get current segment for page 2.
#define DOS_PUT_P3					0x2A // Not supported since page-3 must never be changed.  Acts like a "NOP" if called.
#define DOS_GET_P3					0x2D // Get current segment for page 3.

// Segment allocation type
#define DOS_ALLOC_USER				0
#define DOS_ALLOC_SYS				1

// Slot of the primary Memory Mapper
#define DOS_SEGSLOT_PRIM			0

// Slot flag
#define DOS_SEGSLOT_THIS			0x00 // Allocate a segment to the specified slot
#define DOS_SEGSLOT_OTHER			0x10 // allocate a segment in another slot than the one specified
#define DOS_SEGSLOT_THISFIRST		0x20 // allocate a segment to the specified slot if free, otherwise in another slot
#define DOS_SEGSLOT_OTHERFIRST		0x30 // allocate a segment in another slot than the one specified if free otherwise try in specified slot

// Memory Mappers variable table
typedef struct
{
	u8  Slot;						// Main Memory Mapper slot Number (Address in HL as output)
	u8  NumSeg;						// Number of segments
	u8  FreeSeg;					// Number of free segments
	u8  SysSeg;						// Number of segments reserved by the system
	u8  UserSeg;					// Number of segments reserved by the user
	u8  Reserved[3];				// Reserved
} DOS_VarTable;

// Memory Mappers segment info
typedef struct
{
	u8  Number;						// Segment number
	u8  Slot;						// Segment's slot
} DOS_Segment;

// Memory Mappers variable table
extern DOS_VarTable* g_DOS_VarTable;

// Start address of jump table
extern u16 g_DOS_JumpTable;

//=============================================================================
// FUNCTIONS
//=============================================================================

//.............................................................................
// Group: Core

// Function: DOSMapper_Init
// Initialize DOS extended BIOS
bool DOSMapper_Init();

// Function: DOSMapper_GetVarTable
// Get Memory Mappers variable table
inline DOS_VarTable* DOSMapper_GetVarTable() { return g_DOS_VarTable; }

//.............................................................................
// Group: Allocation

// Function: DOSMapper_Alloc
// Allocate a segment
bool DOSMapper_Alloc(u8 type, u8 slot, DOS_Segment* seg);

// Function: DOSMapper_Free
// Free a segment
bool DOSMapper_Free(u8 seg, u8 slot);

// Function: DOSMapper_FreeStruct
// Free a segment
inline bool DOSMapper_FreeStruct(DOS_Segment* seg) { return DOSMapper_Free(seg->Number, seg->Slot); }

//.............................................................................
// Group: Page handling

// Function: DOSMapper_SetPage
// Select a segment on the corresponding memory page at the specified address
void DOSMapper_SetPage(u8 page, u8 seg);

// Function: DOSMapper_SetPage0
// Select a segment on page 0 (0000h~3FFFh)
void DOSMapper_SetPage0(u8 seg);

// Function: DOSMapper_SetPage1
// Select a segment on page 1 (4000h~7FFFh)
void DOSMapper_SetPage1(u8 seg);

// Function: DOSMapper_SetPage2
// Select a segment on page 2 (8000h~BFFFh)
void DOSMapper_SetPage2(u8 seg);

// Function: DOSMapper_GetPage
// Get the selected segment number on the corresponding memory page at the specified address
u8 DOSMapper_GetPage(u8 page);

// Function: DOSMapper_GetPage0
// Get the segment number on page 0 (0000h~3FFFh)
u8 DOSMapper_GetPage0();

// Function: DOSMapper_GetPage1
// Get the segment number on page 1 (4000h~7FFFh)
u8 DOSMapper_GetPage1();

// Function: DOSMapper_GetPage2
// Get the segment number on page 2 (8000h~BFFFh)
u8 DOSMapper_GetPage2();

// Function: DOSMapper_GetPage3
// Get the segment number on page 3 (C000h~FFFFh)
u8 DOSMapper_GetPage3();

