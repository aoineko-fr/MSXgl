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
global.PrintSucced    = 2;
global.PrintHighlight = 3;
global.PrintNote      = 4;
global.PrintDetail    = 5;
global.PrintBG        = 6;

//*****************************************************************************
// BUILD TOOL OPTION
//*****************************************************************************

// Compile verbose mode (true or false)
global.Verbose = false;

// Compile verbose mode (true or false)
global.LogStdout = true;

// Compile verbose mode (true or false)
global.LogFile = true;
global.LogFileName = "";

//*****************************************************************************
// TARGET CONFIG
//*****************************************************************************

// Default values
global.Target = "";
global.TargetDesc = "";
global.Ext = "";
global.Crt0 = "";
global.StartAddr = 0;
global.CodeAddr = 0;
global.RamAddr = 0;

// ROM values
// global.ROMSize;
global.FillSize;
// Mapped ROM
global.Mapper = "";
global.MapperSize = 0;
global.SegSize = 0;
global.Bank0Addr = 0;
global.Bank1Addr = 0;
global.Bank2Addr = 0;
global.Bank3Addr = 0;

// DOS values
global.USRAddr = 0;

// Basic values
global.DOS = 0;

// Build files list
global.SrcList = [];
global.RelList = [];
global.LibList = [];

global.MapperBanks = [];

//─────────────────────────────────────────────────────────────────────────────
// Build Tool default configuration
//─────────────────────────────────────────────────────────────────────────────

//*****************************************************************************
// DIRECTORIES SETTINGS
//*****************************************************************************

// Build files list
global.ProjDir = process.cwd() + "/";
global.OutDir = `${ProjDir}out/`;

// Library directory
global.RootDir = path.normalize(`${__dirname}/../../../`);
global.LibDir = `${RootDir}engine/`;
global.ToolsDir = `${RootDir}tools/`;


//*****************************************************************************
// TOOLS SETTINGS
//*****************************************************************************

global.Compiler = `${ToolsDir}sdcc\bin\sdcc.exe`;
global.Assembler = `${ToolsDir}sdcc\bin\sdasz80.exe`;
global.Linker = `${ToolsDir}sdcc\bin\sdcc.exe`;
global.MakeLib = `${ToolsDir}sdcc\bin\sdar.exe`;
global.Hex2Bin = `${ToolsDir}MSXtk\bin\MSXhex.exe`;
global.MSXDOS = `${ToolsDir}build\MSXDOS`;
global.DskTool = `${ToolsDir}build\DskTool\dsktool.exe`;
global.Emulator;
global.Debugger;


//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

// Project name (will be use for output filename)
global.ProjName = "";

// Project modules to build (use ProjName if not defined)
global.ProjModules = [];

// Project segments base name (use ProjName if not defined)
global.ProjSegments = "";

// List of library modules to build
global.LibModules= [ "system", "bios", "vdp", "print", "input", "memory" ];

// Additional sources
global.AddSources = [];

// MSX machine version:
// - 1		MSX 1
// - 2		MSX 2
// - 12		MSX 1 or 2 (dual support)
// - 2K		Korean MSX 2 (SC9 support)
// - 2P		MSX 2+
// - TR		MSX Turbo-R
// - 3		MSX 3 (reserved)
global.Machine = "1";

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
global.Target = "ROM_32K";

// ROM mapper total size in KB (from 64 to 4096). Must be a multiple of 8 or 16 depending on the mapper type
global.ROMSize = 0;

// Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (true or false)
global.ROMDelayBoot = false;

// global.RAM in slot 0 and install ISR there (true or false)
global.InstallRAMISR = false;

// Type of custom ISR (for RAM or ROM)
// - VBLANK		V-blank handler
// - VHBLANK	V-blank and h-blank handler (V9938 or V9958)
// - V9990		v-blank, h-blank and command end handler (V9990)
global.CustomISR = "VBLANK";

// Use banked call and trampoline functions (true or false)
global.BankedCall = false;

// Overwrite RAM starting address (e.g. 0xE0000 for 8K RAM machine)
global.ForceRamAddr = false;

// Data to copy to disk (comma separated list)
global.DiskFiles = [];

// Add application signature to binary data (true or false)
global.AppSignature = false;

// Application company
global.AppCompany = "GL";

// Application ID (0~65535)
global.AppID = 0;

// Application extra data (comma-separated bytes starting with data size)
global.AppExtra = "";


//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

// Generate MSXgl static library (true or false)
global.BuildLibrary = true;

// Generate debug data (true or false)
global.Debug = false;

// Move debug symbols do binary folder (true or false)
global.DebugSymbols = false;

// Assembler code optimizer
// - None
// - PeepHole	SDCC otpimizer
// - MDL		MDL z80 otpimizer
global.AsmOptim = "None";

// Optim:
// - Default
// - Speed
// - Size
global.Optim = "Speed";

// Additionnal compilation flag
global.CompileOpt = "";

// Skip file if compile data is newer than the (true or false)
global.CompileSkipOld = false;

// Additionnal link flag
global.LinkOpt = "";

// Update build version header file
global.BuildVersion = false;


//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

// Emulator options (true or false):
global.EmulMachine = true;
global.Emul60Hz = false;
global.EmulFullScreen = false;
global.EmulMute = false;
global.EmulDebug = false;

// Emulator extensions (true or false):
global.EmulSCC = false;
global.EmulMSXMusic = false;
global.EmulMSXAudio = false;
global.EmulPSG2 = false;
global.EmulV9990 = false;

// Emulator port: joystick, mouse, keyboard (fake joystick)
global.EmulPortA = "";
global.EmulPortB = "";

// Emulator extra parameters to be add to command-line (emulator sotfware specific)
global.EmulExtraParam = "";
global.EmulatorArgs = "";

//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

// Build steps (true or false):
global.DoClean   = false;
global.DoCompile = true;
global.DoMake    = true;
global.DoPackage = true;
global.DoDeploy  = true;
global.DoRun     = true;
