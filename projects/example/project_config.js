// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//*****************************************************************************
// TOOLS SETTINGS
//*****************************************************************************

Emulator = `${ToolsDir}OpenMSX/openmsx.exe`;
// Emulator = `${ToolsDir}Emulicious/Emulicious.exe`;
// Emulator = `${ToolsDir}BlueMSX/blueMSX.exe`;
// Emulator = `${ToolsDir}MEISEI/meisei.exe`;
// Emulator = `${ToolsDir}fMSX/fMSX.exe`;
// Emulator = `${ToolsDir}RuMSX/MSX.exe`;
// Debugger = `${ToolsDir}OpenMSX/Debugger/openmsx-debugger.exe`;

//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

// Project name (will be use for output filename)
ProjName = "example";

// Project modules to build (use ProjName if not defined)
ProjModules = [ ProjName ];

// List of modules to link
LibModules = [ "system", "bios", "vdp", "print" ,"input" ,"memory" ,"game" ,"game_pawn" ,"math" ,"string" ];

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
// ROMSize = 

// Postpone the ROM startup to let the other ROMs initialize (BDOS for example) (0 = false, 1 = true)
// ROMDelayBoot = 0

// RAM in slot 0 and install ISR there (0 = false, 1 = true)
// InstallRAMISR = 0

// Use banked call and trampoline functions (0 = false, 1 = true)
// BankedCall = 0

// Overwrite RAM starting address (e.g. 0xE0000 for 8K RAM machine)
// ForceRamAddr = 

// Data to copy to disk (comma separated list)
// DiskFiles = 

// Add application signature to binary data (0 = false, 1 = true)
AppSignature = true;

// Application company
AppCompany = "PP";

// Application ID (0~65535)
AppID = "EX";

//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

// Use static MSXgl library (0 = false, 1 = true)
// BuildLibrary = false;

// debug flag (0 = false, 1 = true)
// Debug = false;

// Assembler code optimizer
// - None
// - PeepHole	SDCC otpimizer
// - MDL		MDL z80 otpimizer
// AsmOptim = None

// Optim:
// - Default
// - Speed
// - Size
// Optim = Speed

// Additionnal compilation flag
// CompileOpt = 

// Skip file if compile data is newer than the (0 = false, 1 = true)
// CompileSkipOld = false;

// Compile verbose mode (0 = false, 1 = true)
Verbose = 1

// Update build version header file
BuildVersion = true;

//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

// Emulator options: 0 or 1
EmulMachine = false;
// Emul60Hz = 0
// EmulFullScreen = 0
// EmulMute = 0
EmulDebug = true;
// Emulator extensions: 0 or 1
// EmulSCC = 0
// EmulMSXMusic = 0
// EmulMSXAudio = 0
// EmulPSG2 = 0
// EmulV9990 = 0
// Emulator port: joystick, mouse, keyboard (fake joystick)
// EmulPortA = 
// EmulPortB = 
// Emulator extra parameters to be add to command-line (emulator sotfware specific)
// EmulExtraParam = 

//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

DoClean   = false;
DoCompile = true;
DoMake    = true;
DoPackage = true;
DoDeploy  = true;
DoRun     = true;
