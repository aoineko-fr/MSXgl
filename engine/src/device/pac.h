// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄█ ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██ █ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Panasoft PAC and FM-PAC's SRAM handler
//
// Need BIOS module
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// PAC_USE_SIGNATURE
#if (defined(APPSIGN))
	#ifndef PAC_USE_SIGNATURE
		#warning PAC_USE_SIGNATURE is not defined in "msxgl_config.h"! Default value will be used: TRUE
		#define PAC_USE_SIGNATURE		TRUE
	#endif
#else
	#if (PAC_USE_SIGNATURE)
		#warning PAC_USE_SIGNATURE can't be use if AppSignature option is not set in "project_config.js"! PAC_USE_SIGNATURE will be disable //'
		#define PAC_USE_SIGNATURE		FALSE
	#endif
#endif

// PAC_USE_VALIDATOR
#ifndef PAC_USE_VALIDATOR
	#warning PAC_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PAC_USE_VALIDATOR			TRUE
#endif

// PAC_DEVICE_MAX
#ifndef PAC_DEVICE_MAX
	#warning PAC_DEVICE_MAX is not defined in "msxgl_config.h"! Default value will be used: 4
	#define PAC_DEVICE_MAX				4
#endif

//=============================================================================
// DEFINES
//=============================================================================

#define PAC_PAGE_MAX				8
#define PAC_EMPTY_CHAR				0xFF

//
enum PAC_CHECK
{
	PAC_CHECK_EMPTY = 0,
	PAC_CHECK_UNDEF,
	PAC_CHECK_ERROR,
	#if (PAC_USE_SIGNATURE)
	PAC_CHECK_APP,
	#endif
};

// Number of detected PAC cartridge
extern u8 g_PAC_Num;

// Detected PACs' slot ID
extern u8 g_PAC_Slot[PAC_DEVICE_MAX];

// Selected PAC's slot ID
extern u8 g_PAC_Current;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: PAC_Initialize
// Initialize PAC module
bool PAC_Initialize();

// Function: PAC_GetSlot
// Get a given PAC device's slot ID
inline u8 PAC_GetSlot(u8 dev) { return g_PAC_Slot[dev]; }

// Function: PAC_GetDefaultSlot
// Get the default (first) PAC device's slot ID
inline u8 PAC_GetDefaultSlot() { return PAC_GetSlot(0); }

// Function: PAC_GetNumber
// Get number of detected PAC devices
inline u8 PAC_GetNumber() { return g_PAC_Num; }

// Function: PAC_Select
// Select a given PAC device
inline void PAC_Select(u8 dev) { g_PAC_Current = g_PAC_Slot[dev]; }

// Function: PAC_Activate
// Activate or disactive the current PAC device
void PAC_Activate(bool bEnable);

// Function: PAC_Write
// Write data to the current PAC device in the given page
void PAC_Write(u8 page, const u8* data, u16 size);

// Function: PAC_Read
// Read data from the current PAC device in the given page
void PAC_Read(u8 page, u8* data, u16 size);

// Function: PAC_Format
// Format the given page in the current PAC device
void PAC_Format(u8 page);

// Function: PAC_FormatAll
// Format all pages in the current PAC device
inline void PAC_FormatAll() { for(u8 i = 0; i < PAC_PAGE_MAX; ++i) PAC_Format(i); }

// Function: PAC_Check
// Check the stat of the given page in the current PAC device
u8 PAC_Check(u8 page);