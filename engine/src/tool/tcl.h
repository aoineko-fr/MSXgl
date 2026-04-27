// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │   ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// TCL module to communicate with the Programmable Device
//   Based on Pedro de Medeiros "pvmm" works
//   https://github.com/pvmm/openmsx-tcl-bridge/tree/main
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// Data to send to tcl_bridge
typedef struct TCL_Data
{
    c8* Output;        // offset + 0
    u16 OutputSize;    // offset + 2
    c8* Input;         // offset + 4
    u16 InputMax;      // offset + 6
    u16 InputSize;     // offset + 8
    i8  Status;        // offset + 10
} TCL_Data;

// Status codes sent back by Programmable Device
enum TCL_STATUS
{
	TCL_STATUS_COMPLETED    = 0, // TCL command completed successfully
	TCL_STATUS_ERROR        = 1, // Error while executing the command
};

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: TCL_Send
// Send command to TCL engine
//
// Parameters:
//   data - Pointer to the TCL_Data structure
void TCL_Send(const TCL_Data* data);