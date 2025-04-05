// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  Pixel Phenix Logo
//─────────────────────────────────────────────────────────────────────────────

// Dependencies
#include "msxgl.h"
#include "vdp.h"
#include "compress/pletter.h"

// Function: Logo_Initialize
// Set up the logo and load necessary data
// Usage: Call once
void Logo_Initialize();

// Function: Logo_Update
// Update the logo animation
// Usage: Call every frame until it returns TRUE.
//
// Return:
//  TRUE when animation is done
bool Logo_Update();
