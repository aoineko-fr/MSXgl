//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "v9990", ...LibModules];

//-- Target program format (string)
// Target = "ROM_ASCII16";
Target = "ROM_NEO16";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096 for legacy mappers; can be up to 65536 for NEO-16 mapper)
ROMSize = 256;

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - NONE       No ISR
//   - ALL        Handle all interruptions
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      V-blank, h-blank and command end handler (V9990)
CustomISR = "V9990";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "V9";

//-- Add V9990 video-chip extension (boolean)
EmulV9990 = true;

//-- List of raw data files to be added to final binary (array). Each entry must be in the following format: { offset=0x0000, file="myfile.bin" }
RawFiles = [
	{ segment:  3, file:"content/v9990/data_bg_bp2.bin"},		// 4105		/ 16384		1
	{ segment:  4, file:"content/v9990/data_bg_bp4.bin"},		// 8237		/ 16384		1
	{ segment:  5, file:"content/v9990/data_bg_bp6.bin"},		// 16573	/ 16384		2
	{ segment:  7, file:"content/v9990/data_bg_bd8.bin"},		// 16384	/ 16384		1
	{ segment:  8, file:"content/v9990/data_bg_bd16.bin"},		// 32768	/ 16384		2
	{ segment: 10, file:"content/v9990/data_bg_byjk.bin"},		// 16384	/ 16384		1
	{ segment: 11, file:"content/v9990/data_bg_byjkp.bin"},		// 16429	/ 16384		2
	{ segment: 13, file:"content/v9990/data_bg_byuv.bin"},		// 16384	/ 16384		1
	{ segment: 14, file:"content/v9990/data_bg_byuvp.bin"},		// 12336	/ 16384		2
];