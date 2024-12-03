// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//-- Node.jd libraries
const path = require('path');

//*****************************************************************************
// CONSTANTS
//*****************************************************************************

//-- Setup text color
global.ColorReset   = "[0m";
global.ColorGray    = "[90m";
global.ColorRed     = "[91m";
global.ColorGreen   = "[92m";
global.ColorYellow  = "[93m";
global.ColorBlue    = "[94m";
global.ColorMagenta = "[95m";
global.ColorCyan    = "[96m";
global.ColorBG      = "[44m";

//-- Print flag
global.PrintError     = 0;
global.PrintWarning   = 1;
global.PrintSuccess   = 2;
global.PrintHighlight = 3;
global.PrintNote      = 4;
global.PrintDetail    = 5;
global.PrintBG        = 6;

//*****************************************************************************
// TEMPORARY VARIABLES
//*****************************************************************************

//-----------------------------------------------------------------------------
// Initilized in setup_target.js

// Default values
global.TargetType = "";
global.TargetDesc = "";
global.Ext = "";
global.Crt0 = "";
global.StartAddr = 0;
global.CodeAddr = 0;
global.RamAddr = 0;

// ROM values
global.ROMFirstPage = 0;
global.ROMLastPage = 0;
global.ROMWithISR = false;
global.ROMSignature = "";

// Mapped ROM
global.Mapper = "";
global.MapperSize = 0;
global.SegSize = 0;
global.Bank0Addr = 0xFFFF;
global.Bank1Addr = 0xFFFF;
global.Bank2Addr = 0xFFFF;
global.Bank3Addr = 0xFFFF;
global.Bank4Addr = 0xFFFF;
global.Bank5Addr = 0xFFFF;

// Basic values
global.DOS = 0;

// Build files list
global.SrcList;
global.RelList;
global.LibList;
global.MapList;

global.MapperBanks;

// Emulator parameters
global.EmulatorArgs = "";

//─────────────────────────────────────────────────────────────────────────────
// Build Tool default configuration
//─────────────────────────────────────────────────────────────────────────────

//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

//-- Clear all intermediate files and exit (boolean)
global.DoClean = false;

//-- Compile all the project and engine source code (boolean). Generate all REL files
global.DoCompile = true;

//-- Link all the project and engine source code (boolean). Merge all REL into one IHX file
global.DoMake = true;

//-- Generate final binary file (boolean). Binarize the IHX file
global.DoPackage = true;

//-- Gathering of all files necessary for the program to work (boolean). Depends on the type of target
global.DoDeploy = true;

//-- Start the program automatically at the end of the build (boolean)
global.DoRun = true;

//*****************************************************************************
// DIRECTORIES SETTINGS
//*****************************************************************************

//-- Project directory (string)
global.ProjDir = process.cwd() + "/";

//-- Intermediate files directory (string)
global.OutDir = `${ProjDir}out/`;

//-- MSXgl root directory (string)
global.RootDir = path.normalize(`${__dirname}/../../../`);

//-- Library directory (string)
global.LibDir = `${RootDir}engine/`;

//-- Tools directory (string)
global.ToolsDir = `${RootDir}tools/`;

//*****************************************************************************
// TOOLS SETTINGS
//*****************************************************************************

//-- Path to SDCC tools chain (string)
global.SDCCPath = `${ToolsDir}sdcc/`;

//-- Path to the C compile program (string)
global.Compiler = `${SDCCPath}bin/sdcc`;

//-- Path to the assembler program (string)
global.Assembler = `${SDCCPath}bin/sdasz80`;

//-- Path to the linker program (string)
global.Linker = `${SDCCPath}bin/sdcc`;

//-- Path to the program to generate lib file (string)
global.MakeLib = `${SDCCPath}bin/sdar`;

//-- Path to IHX to binary convertor (string)
global.Hex2Bin = `${ToolsDir}MSXtk/bin/MSXhex`;

//-- Path to the MSX-DOS files (string)
global.MSXDOS = `${ToolsDir}build/DOS`;

//-- Path to the tool to generate DSK file (string)
global.DskTool = `${ToolsDir}build/msxtar/msxtar`;

//-- Path to the emulator to launch the project (string)
global.Emulator;

//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

//-- Project name (string). Will be use for output filename
global.ProjName = "";

//-- List of project modules to build (array). If empty, ProjName will be added
global.ProjModules = [];

//-- Project segments base name (string). ProjName will be used if not defined
global.ProjSegments = "";

//-- List of library modules to build (array)
global.LibModules = [];

//-- Additional sources to be compiled and linked with the project (array)
global.AddSources = [];

//-- Target MSX machine version (string)
//   - 1        MSX1
//   - 2        MSX2
//   - 12       MSX1 and 2 (multi support)
//   - 2K       Korean MSX2 (SC9 support)
//   - 2P       MSX2+
//   - 22P      MSX2 and 2+ (multi support)
//   - 122P     MSX1, 2 and 2+ (multi support)
//   - 0        MSX0
//   - TR       MSX turbo R
//   - 3        MSX3 (reserved)
global.Machine = "1";

//-- Target program format (string)
//   - BIN              .bin    BASIC binary program (starting at 8000h)
//   - BIN_DISK         .bin    BASIC binary program (starting at 8000h) on disk
//   - BIN_TAPE         .bin    BASIC binary program (starting at 8000h) on tape
//   - BIN_USR          .bin    BASIC USR binary driver (starting at C000h)
//   - DOS1             .com    MSX-DOS 1 program (starting at 0100h)
//   - DOS2             .com    MSX-DOS 2 program (starting at 0100h)
//   - DOS0             .com    Direct program boot from disk (starting at 0100h)
//   - ROM_8K           .rom    8 KB ROM in page 1 (4000h ~ 5FFFh)
//   - ROM_8K_P2        .rom    8 KB ROM in page 2 (8000h ~ 9FFFh)
//   - ROM_16K          .rom    16 KB ROM in page 1 (4000h ~ 7FFFh)
//   - ROM_16K_P2       .rom    16 KB ROM in page 2 (8000h ~ BFFFh)
//   - ROM_32K          .rom    32 KB ROM in page 1&2 (4000h ~ BFFFh)
//   - ROM_48K          .rom    48 KB ROM in page 0-2 (0000h ~ BFFFh)
//   - ROM_48K_ISR      .rom    48 KB ROM in page 0-2 (0000h ~ BFFFh) with ISR replacement
//   - ROM_64K          .rom    64 KB ROM in page 0-3 (0000h ~ FFFFh)
//   - ROM_64K_ISR      .rom    64 KB ROM in page 0-3 (0000h ~ FFFFh) with ISR replacement
//   - ROM_ASCII8       .rom    ASCII-8: 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_ASCII16      .rom    ASCII-16: 16 KB segments for a total of 64 KB to 4 MB
//   - ROM_KONAMI       .rom    Konami MegaROM (aka Konami4): 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_KONAMI_SCC   .rom    Konami MegaROM SCC (aka Konami5): 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_NEO8         .rom    NEO-8: 8 KB segments for a total of 1 MB to 32 MB
//   - ROM_NEO16        .rom    NEO-16: 16 KB segments for a total of 1 MB to 64 MB
global.Target = "ROM_32K";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
global.ROMSize = 128;

//-- Number of segments in the main program of a mapped-ROM (number). 0 means 'number of segments to fill 32 KB of ROM'
global.ROMMainSegments = 0;

//-- Postpone the ROM startup to let the other ROMs initialize like Disk controller or Network cartridge (boolean)
global.ROMDelayBoot = false;

//-- Add a ROM signature to help flasher and emulator to detect the ROM type properly (boolean)
global.AddROMSignature = false;

//-- Select RAM in slot 0 and install ISR and optional code there (string). For MSX with at least 64 KB of RAM
//   - RAM0_NONE       Don't install anything in RAM 
//   - RAM0_ISR        Install only ISR
//   - RAM0_SEGMENT    Install ISR and segment data (for mapped-ROM)
global.InstallRAMISR = "RAM0_NONE";

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      V-blank, h-blank and command end handler (V9990)
global.CustomISR = "VBLANK";

//-- Use automatic banked call and trampoline functions (boolean). For mapped ROM
global.BankedCall = false;

//-- Overwrite code starting address (number). For example. 0xE0000 for a driver in RAM
global.ForceCodeAddr = 0;

//-- Overwrite RAM starting address (number). For example. 0xE0000 for 8K RAM machine
global.ForceRamAddr = 0;

//-- List of raw data files to be added to final binary (array). Each entry must be in the following format: { offset=0x0000, file="myfile.bin" }
global.RawFiles = [];

//-- List of data files to copy to disk (array)
global.DiskFiles = [];

//-- Size of the final disk (.DSK file). Can be "360K" or "720K" (string)
global.DiskSize = "720K";

//-- Parse MSX-DOS command-line arguments
global.DOSParseArg = true;

//*******************************************************************************
// SIGNATURE SETTINGS
//*******************************************************************************

//-- Add application signature to binary data (boolean)
global.AppSignature = false;

//-- Application company (*). Can be 2 character string or 16-bits integer (0~65535)
global.AppCompany = "GL";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
global.AppID = 0;

//-- Application extra data (array). Comma-separated bytes starting with data size
global.AppExtra = [];

//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

//-- Force to generate MSXgl static library even if 'msxgl.lib' already exist (boolean)
global.BuildLibrary = true;

//-- Prepare program for debug (boolean)
global.Debug = false;

//-- Move debug symbols to deployement folder (boolean)
global.DebugSymbols = false;

//-- Allow compiler to generate undocumented Z80 instructions (boolean)
global.AllowUndocumented = false;

//-- Assembler code optimizer (string)
//   - None
//   - Peep       SDCC peep hole otpimizer
//   - MDL        MDL z80 otpimizer
global.AsmOptim = "None";

//-- Code optimization priority (string)
//   - Default
//   - Speed
//   - Size
global.Optim = "Speed";

//-- Compile complexity. The higher the most optimized but the slowest to compile. Can be one of the predefine name or a integer value (string/integer)
//   - Fast			    2000
//   - Default		    3000
//   - Optimized	   50000
//   - Ultra		  200000
//   - Insane		10000000
global.CompileComplexity = "Default";

//-- Additionnal compilation options (string)
global.CompileOpt = "";

//-- Skip file if compile data (REL) is newer than the source code (boolean)
global.CompileSkipOld = false;

//-- Additionnal link options (string)
global.LinkOpt = "";

//-- Automatic increment of build version in a header file (boolean)
global.BuildVersion = false;

//-- Package all segments into a lib file to reduce the number of files to link (boolean)
global.PackSegments = false;

//-- Additionnal options of Hex to Binary convertor (string)
global.HexBinOpt = "";

//-- Command lines to be executed before the build process (array)
global.PreBuildScripts = [];

//-- Command lines to be executed after the build process (array)
global.PostBuildScripts = [];

//*******************************************************************************
// LOCALISATION SETTINGS
//*******************************************************************************

//-- List files to be localized (array)
global.LocFiles = [];

//-- Localization output filename (string)
global.LocOutput = "";

//-- Localization structure name (string)
global.LocStruct = "";

//-- Split socalization data and definitions in different files (boolean)
global.LocSplitDef = false;

//*****************************************************************************
// BUILD TOOL OPTION
//*****************************************************************************

//-- Activate verbose mode and get more build information (boolean)
global.Verbose = false;

//-- Output build information to the standard console (boolean)
global.LogStdout = true;

//-- Output build information to a log file (boolean)
global.LogFile = false;

//-- Name of the log file (string)
global.LogFileName = "";

//*******************************************************************************
// ANALYZER SETINGS
//*******************************************************************************

//-- Execute MAP analyzer (boolean)
global.Analyzer = false;

//-- Analyzer output selection (string)
//   - Console    Output to termial console
//   - File       Output to file
//   - Both       Output to file and termial console (default)
global.AnalyzerOutput = "Both";

//-- Analyzer report elements (string)
//   - [A]        Report areas
//   - [S]        Report segments
//   - [M]        Report modules
//   - [C]        Report code symbols
//   - [V]        Report variable symbols
global.AnalyzerReport = "ASMCV";

//-- Analyzer report sorting (string)
//   - None       No sorting (MAP file order)
//   - Alpha      Alphanumeric sorting
//   - Size       Size sorting (default)
global.AnalyzerSort = "Size";

//-- Export analyzer data to CSV file (boolean)
global.AnalyzerCSV = false;

//-- Analyzer CSV file separator (string)
global.AnalyzerSeparator = ",";

//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

//-------------------------------------------------------------------------------
// General options

//-- Force the MSX version of the emulated machine (boolean)
global.EmulMachine = true;

//-- Force the emulated machine to be at 60 Hz (boolean)
global.Emul60Hz = false;

//-- Force the emulator to start in fullscreen mode (boolean)
global.EmulFullScreen = false;

//-- Disable emulator sound (boolean)
global.EmulMute = false;

//-- Start emulator debugger with program launch (boolean)
global.EmulDebug = false;

//-- Start emulator in turbo mode (boolean)
global.EmulTurbo = false;

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
global.EmulExtraParam = "";

//-------------------------------------------------------------------------------
// Extension options

//-- Add SCC extension (boolean)
global.EmulSCC = false;

//-- Add MSX-Music extension (boolean)
global.EmulMSXMusic = false;

//-- Add MSX-Audio extension (boolean)
global.EmulMSXAudio = false;

//-- Add OPL4 extension (boolean)
global.EmulOPL4 = false;

//-- Add second PSG extension (boolean)
global.EmulPSG2 = false;

//-- Add V9990 video-chip extension (boolean)
global.EmulV9990 = false;

//-------------------------------------------------------------------------------
// Input options

//-- Plug a virtual device into the joystick port A (string)
//   - Joystick
//   - Mouse
//   - Paddle
//   - JoyMega
//   - NinjaTap
global.EmulPortA = "";

//-- Plug a virtual device into the joystick port B (string)
global.EmulPortB = "";

//-------------------------------------------------------------------------------
// Run device options

//-- Run device like Easy-USB (string)
global.RunDevice = "";

//-- Run device option (string)
global.RunDeviceOpt = "";
