// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "msxgl.h"
#include "ascii.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: Printer_CheckReady
// Check of the printer is connected and ready
//
// Return:
//   FALSE if printer is not ready
bool Printer_CheckReady();

// Function: Printer_SendChar
// Send a character to printer using direct IO port access
//
// Parameters:
//   chr - ASCII code of character to send
void Printer_SendChar(u8 chr) __FASTCALL;

// Function: Printer_SendString
// Send a null-terminated string to printer using direct IO port access
//
// Parameters:
//   str - Pointer to the string to send
inline void Printer_SendString(const char* str) { while (*str) Printer_SendChar(*str++); }

// Function: Printer_LineReturn
// Send a carriage return to the printer
inline void Printer_LineReturn() { Printer_SendString("\r\n"); }

// Function: Printer_ChangePage
// Send a change page code to the printer
inline void Printer_ChangePage() { Printer_SendChar(ASCII_FORM_FEED); }