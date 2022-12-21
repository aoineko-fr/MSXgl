// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//─────────────────────────────────────────────────────────────────────────────
// Build Tool default configuration
//─────────────────────────────────────────────────────────────────────────────

//*****************************************************************************
// DIRECTORIES SETTINGS
//*****************************************************************************

ProjDir = process.cwd() + "/";
OutDir = `${ProjDir}out/`;


//*****************************************************************************
// TOOLS SETTINGS
//*****************************************************************************

Compiler = `${ToolsDir}sdcc/bin/sdcc.exe`;
Assembler = `${ToolsDir}sdcc/bin/sdasz80.exe`;
Linker = `${ToolsDir}sdcc/bin/sdcc.exe`;
MakeLib = `${ToolsDir}sdcc/bin/sdar.exe`;
Hex2Bin = `${ToolsDir}MSXtk/bin/MSXhex.exe`;
MSXDOS = `${ToolsDir}build/MSXDOS/`;
DskTool = `${ToolsDir}build/DskTool/dsktool.exe`;
Emulator =
// Emulator = `${ToolsDir}OpenMSX/openmsx.exe`;
// Emulator = `${ToolsDir}Emulicious/Emulicious.exe`;
// Emulator = `${ToolsDir}BlueMSX/blueMSX.exe`;
// Emulator = `${ToolsDir}MEISEI/meisei.exe`;
// Emulator = `${ToolsDir}fMSX/fMSX.exe`;
// Emulator = `${ToolsDir}RuMSX/MSX.exe`;
Debugger =
// Debugger = `${ToolsDir}OpenMSX/Debugger/openmsx-debugger.exe`;


//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

// Project name (will be use for output filename)
ProjName = "";

// Project modules to build (use ProjName if not defined)
ProjModules = [];

// Project segments base name (use ProjName if not defined)
ProjSegments = "";

// List of library modules to build
LibModules= [ "system", "bios", "vdp", "print", "input", "memory" ];

// Additional sources
AddSources = [];

// MSX machine version:
// - 1		MSX 1
// - 2		MSX 2
// - 12		MSX 1 or 2 (dual support)
// - 2K		Korean MSX 2 (SC9 support)
// - 2P		MSX 2+
// - TR		MSX Turbo-R
// - 3		MSX 3 (reserved)
Machine = "1";

// Program media target:
// - BIN			.bin	BASIC binary program (8000h~)
// - BIN_USR		.bin	BASIC USR binary driver (C000h~)
// - ROM_8K			.rom	8KB ROM in page 1 (4000h ~ 5FFFh)
// - ROM_8K_P2		.rom	8KB ROM in page 2 (8000h ~ 9FFFh)
// - ROM_16K		.rom	16KB ROM in page 1 (4000h ~ 7FFFh)
// - ROM_16K_P2		.rom	16KB ROM in page 2 (8000h ~ BFFFh)
// - ROM_32K		.rom	32KB ROM in page 1-2 (4000h ~ BFFFh)
// - ROM_48K		.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 1-2 visible at start
// - ROM_48K_ISR	.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 0-2 visible at start
// - ROM_64K		.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 1-2 visible at start
// - ROM_64K_ISR	.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 0-2 visible at start
// - ROM_ASCII8		.rom	128KB ROM using ASCII-8 mapper
// - ROM_ASCII16	.rom	128KB ROM using ASCII-16 mapper
// - ROM_KONAMI		.rom	128KB ROM using Konami mapper (8KB segments)
// - ROM_KONAMI_SCC	.rom	128KB ROM using Konami SCC mapper (8KB segments)
// - DOS1			.com	MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
// - DOS2			.com	MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
// - DOS2_ARG		.com	[WIP] MSX-DOS 2 program (using command line arguments ; 0100h~) No direct acces to Main-ROM. 
Target = "ROM_32K";

// ROM mapper size (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
ROMSize = 0;

// Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (true or false)
ROMDelayBoot = false;

// RAM in slot 0 and install ISR there (true or false)
InstallRAMISR = false;

// Type of custom ISR (for RAM or ROM)
// - VBLANK		V-blank handler
// - VHBLANK	V-blank and h-blank handler (V9938 or V9958)
// - V9990		v-blank, h-blank and command end handler (V9990)
CustomISR = "VBLANK";

// Use banked call and trampoline functions (true or false)
BankedCall = 0;

// Overwrite RAM starting address (e.g. 0xE0000 for 8K RAM machine)
ForceRamAddr = 0;

// Data to copy to disk (comma separated list)
DiskFiles = [];

// Add application signature to binary data (true or false)
AppSignature = false;

// Application company
AppCompany = "GL";

// Application ID (0~65535)
AppID = 0;

// Application extra data (comma-separated bytes starting with data size)
AppExtra = "";


//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

// Generate MSXgl static library (true or false)
BuildLibrary = true;

// Generate debug data (true or false)
Debug = false;

// Move debug symbols do binary folder (true or false)
DebugSymbols = false;

// Assembler code optimizer
// - None
// - PeepHole	SDCC otpimizer
// - MDL		MDL z80 otpimizer
AsmOptim = "None";

// Optim:
// - Default
// - Speed
// - Size
Optim = "Speed";

// Additionnal compilation flag
CompileOpt = "";

// Skip file if compile data is newer than the (true or false)
CompileSkipOld = false;

// Additionnal link flag
LinkOpt = "";

// Update build version header file
BuildVersion = false;


//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

// Emulator options (true or false):
EmulMachine = true;
Emul60Hz = false;
EmulFullScreen = false;
EmulMute = false;
EmulDebug = false;

// Emulator extensions (true or false):
EmulSCC = false;
EmulMSXMusic = false;
EmulMSXAudio = false;
EmulPSG2 = false;
EmulV9990 = false;

// Emulator port: joystick, mouse, keyboard (fake joystick)
EmulPortA = "";
EmulPortB = "";

// Emulator extra parameters to be add to command-line (emulator sotfware specific)
EmulExtraParam = "";


//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

// Build steps (true or false):
DoClean   = false;
DoCompile = true;
DoMake    = true;
DoPackage = true;
DoDeploy  = true;
DoRun     = true;


//*****************************************************************************
// BUILD TOOL OPTION
//*****************************************************************************

// Compile verbose mode (true or false)
Verbose = false;

// Compile verbose mode (true or false)
LogStdout = true;

// Compile verbose mode (true or false)
LogFile = false;
LogFileName = "";
