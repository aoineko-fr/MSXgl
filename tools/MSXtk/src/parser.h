//_____________________________________________________________________________
//   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//__________________________▀▀_________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)
#pragma once

// std
#include <string>
#include <vector>

// FreeImage
#include "FreeImage.h"
// MSXi
#include "color.h"
#include "exporter.h"
#include "image.h"

//-----------------------------------------------------------------------------
// Public interface
//-----------------------------------------------------------------------------

// Invoke the selected mode's exporter
bool ParseImage(ExportParameters* param, ExporterInterface* exp);

// Build 256 colors palette
void Create256ColorsPalette(const char* filename);

// Build 16 colors palette
void Create16ColorsPalette(const char* filename);

//-----------------------------------------------------------------------------
// Shared helpers (parser.cpp)
//-----------------------------------------------------------------------------

extern u32 CustomPalette[16];

// Get palette
u32* GetPalette(ExportParameters* param);

// Get nearest color index
u8 GetNearestColorIndex(u32 color, u32* pal, i32 count, i32 offset);

// Get 8-bit GRB color
u8 GetGBR8(u32 color, bool bUseTrans, u32 transRGB);

// Get 16-bit GBR color
u16 GetGBR16(u32 color, bool bUseTrans, u32 transRGB);

// Export a table
void ExportTable(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data, u32 modX);

// Export Pletter data
void ExportPletter(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data);

// Export RLEp data
void ExportRLEp(ExportParameters* param, ExporterInterface* exp, const std::vector<u8>& data);

// Prepare image for export
FIBITMAP* GetPreparedImage(std::string filename, ExportParameters* param, bool bCrop = false);

//-----------------------------------------------------------------------------
// Mode exporters
//-----------------------------------------------------------------------------

// Export bitmap image (parser_bitmap.cpp)
bool ExportBitmap(ExportParameters* param, ExporterInterface* exp);

// Export GM1 image (parser_tilemap.cpp)
bool ExportGM1(ExportParameters* param, ExporterInterface* exp);

// Export GM2 image (parser_tilemap.cpp)
bool ExportGM2(ExportParameters* param, ExporterInterface* exp);

// Export GM2i image (parser_tilemap.cpp)
bool ExportGM2i(ExportParameters* param, ExporterInterface* exp);

// Export sprite sheet (parser_sprite.cpp)
bool ExportSprite(ExportParameters* param, ExporterInterface* exp);

// Export TEXT image (parser_text.cpp)
bool ExportText(ExportParameters* param, ExporterInterface* exp);

// Convert multiple images into MGLV video data (parser_mglv.cpp)
bool ExportMGLV(ExportParameters* param, ExporterInterface* exp);