// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

// DoClean   = false;	//-- Clear all intermediate files and exit (boolean)
// DoCompile = true;	//-- Compile all the project and engine source code (boolean). Generate all REL files
// DoMake    = true;	//-- Link all the project and engine source code (boolean). Merge all REL into one IHX file
// DoPackage = true;	//-- Generate final binary file (boolean). Binarize the IHX file
// DoDeploy  = true;	//-- Gathering of all files necessary for the program to work (boolean). Depends on the type of target
// DoRun     = true;	//-- Start the program automatically at the end of the build (boolean)

//*****************************************************************************
// TOOLS SETTINGS
//*****************************************************************************

//-- Path to the emulator to launch the project (string)
// Emulator = `${ToolsDir}openMSX/openmsx`
// Emulator = `${ToolsDir}Emulicious/Emulicious`
// Emulator = `${ToolsDir}BlueMSX/blueMSX`
// Emulator = `${ToolsDir}MEISEI/meisei`
// Emulator = `${ToolsDir}fMSX/fMSX`
// Emulator = `${ToolsDir}RuMSX/MSX`

//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

//-- Project name (string). Will be use for output filename
// ProjName = %Input%

//-- List of project modules to build (array). If empty, ProjName will be added
// ProjModules = %ProjName%

//-- Project segments base name (string). ProjName will be used if not defined
// ProjSegments = %ProjName%

//-- List of library modules to build (array)
LibModules = [ "system", "bios", "vdp", "print", "input", "memory", "math", "draw" ];

//-- Additional sources to be compiled and linked with the project (array)
// AddSources = [];

//-- Target MSX machine version (string)
//   - 1        MSX 1
//   - 2        MSX 2
//   - 12       MSX 1 or 2 (dual support)
//   - 2K       Korean MSX 2 (SC9 support)
//   - 2P       MSX 2+
//   - TR       MSX turbo R
//   - 3        MSX 3 (reserved)
Machine = "2";

//-- Target program format (string)
//   - BIN              .bin    BASIC binary program (8000h~)
//   - BIN_USR          .bin    BASIC USR binary driver (C000h~)
//   - ROM_8K           .rom    8KB ROM in page 1 (4000h ~ 5FFFh)
//   - ROM_8K_P2        .rom    8KB ROM in page 2 (8000h ~ 9FFFh)
//   - ROM_16K          .rom    16KB ROM in page 1 (4000h ~ 7FFFh)
//   - ROM_16K_P2       .rom    16KB ROM in page 2 (8000h ~ BFFFh)
//   - ROM_32K          .rom    32KB ROM in page 1-2 (4000h ~ BFFFh)
//   - ROM_48K          .rom    48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 1-2 visible at start
//   - ROM_48K_ISR      .rom    48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 0-2 visible at start
//   - ROM_64K          .rom    64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 1-2 visible at start
//   - ROM_64K_ISR      .rom    64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 0-2 visible at start
//   - ROM_ASCII8       .rom    128KB ROM using ASCII-8 mapper
//   - ROM_ASCII16      .rom    128KB ROM using ASCII-16 mapper
//   - ROM_KONAMI       .rom    128KB ROM using Konami mapper (8KB segments)
//   - ROM_KONAMI_SCC   .rom    128KB ROM using Konami SCC mapper (8KB segments)
//   - DOS1             .com    MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
//   - DOS2             .com    MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
//   - DOS2_ARG         .com    [WIP] MSX-DOS 2 program (using command line arguments ; 0100h~) No direct acces to Main-ROM. 
Target = "ROM_32K";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
// ROMSize = 0;

//-- Postpone the ROM startup to let the other ROMs initialize like Disk controller or Network cartridge (boolean)
// ROMDelayBoot = false;

//-- Select RAM in slot 0 and install ISR there (boolean). For MSX with at least 64 KB of RAM
// InstallRAMISR = false;

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      v-blank, h-blank and command end handler (V9990)
CustomISR = "VBLANK";

//-- Use automatic banked call and trampoline functions (boolean). For mapped ROM
// BankedCall = false;

//-- Overwrite RAM starting address (number). For example. 0xE0000 for 8K RAM machine
// ForceRamAddr = 0;

//-- List of data files to copy to disk (array)
// DiskFiles = "";

//-- BASIC USR driver default address (number)
// USRAddr = 0xC000;

//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

//-- Force to generate MSXgl static library even if 'msxgl.lib' already exist (boolean)
// BuildLibrary = true;

//-- Prepare program for debug (boolean)
// Debug = false;

//-- Move debug symbols to deployement folder (boolean)
DebugSymbols = true;

//-- Assembler code optimizer (string)
//   - None
//   - Peep       SDCC peep hole otpimizer
//   - MDL        MDL z80 otpimizer
// AsmOptim = "None";

//-- Code optimization priority (string)
//   - Default
//   - Speed
//   - Size
// Optim = Speed;

//-- Additionnal compilation options (string)
// CompileOpt = "";

//-- Skip file if compile data (REL) is newer than the source code (boolean)
// CompileSkipOld = false;

//-- Additionnal link options (string)
// LinkOpt = "";

//-- Automatic increment of build version in a header file (boolean)
// BuildVersion = false;

//*****************************************************************************
// BUILD TOOL OPTION
//*****************************************************************************

//-- Activate verbose mode and get more build information (boolean)
Verbose = true;

//-- Output build information to the standard console (boolean)
// LogStdout = true;

//-- Output build information to a log file (boolean)
// LogFile = false;

//-- Name of the log file (string)
// LogFileName = "";

//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

//-------------------------------------------------------------------------------
// General options

// EmulMachine    = true;				//-- Force the MSX version of the emulated machine (boolean)
// Emul60Hz       = false;				//-- Force the emulated machine to be at 60 Hz (boolean)
// EmulFullScreen = false;				//-- Force the emulator to start in fullscreen mode (boolean)
// EmulMute       = false;				//-- Disable emulator sound (boolean)
// EmulDebug      = false;				//-- Start emulator debugger with program launch (boolean)

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
// EmulExtraParam = "";

//-------------------------------------------------------------------------------
// Extension options

// EmulSCC      = false;				//-- Add SCC extension (boolean)
// EmulMSXMusic = false;				//-- Add MSX-Music extension (boolean)
// EmulMSXAudio = false;				//-- Add MSX-Audio extension (boolean)
// EmulPSG2     = false;				//-- Add second PSG extension (boolean)
// EmulV9990    = false;				//-- Add V9990 video-chip extension (boolean)

//-------------------------------------------------------------------------------
// Input options

//-- Plug a virtual device into the joystick port A (string)
//   - Joystick
//   - Keyboard         Fake joystick
//   - Mouse
//   - NinjaTap
// EmulPortA = "";

//-- Plug a virtual device into the joystick port B (string)
// EmulPortB = "";
