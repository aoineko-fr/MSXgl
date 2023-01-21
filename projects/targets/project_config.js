// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

//-- Project name (will be use for output filename)
ProjName = "s_target";

//-- Project modules to build (use ProjName if not defined)
ProjModules = [ ProjName ];

//-- Project segments base name
ProjSegments = ProjName;

//-- List of library modules to build
LibModules = [ "system", "bios", "vdp", "print", "input", "memory" ];

//-- MSX version:
//   - 1		MSX 1
//   - 2		MSX 2
//   - 2P		MSX 2+
//   - TR		MSX TurboR
//   - 12		MSX 1 or 2
Machine = "1";

//-- Target:
//   - BIN				.bin	BASIC binary program (8000h~)
//   - ROM_8K			.rom	8KB ROM in page 1 (4000h ~ 5FFFh)
//   - ROM_8K_P2		.rom	8KB ROM in page 2 (8000h ~ 9FFFh)
//   - ROM_16K			.rom	16KB ROM in page 1 (4000h ~ 7FFFh)
//   - ROM_16K_P2		.rom	16KB ROM in page 2 (8000h ~ BFFFh)
//   - ROM_32K			.rom	32KB ROM in page 1-2 (4000h ~ BFFFh)
//   - ROM_48K			.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 1-2 visible at start
//   - ROM_48K_ISR		.rom	48KB ROM in page 0-2 (0000h ~ BFFFh). Pages 0-2 visible at start
//   - ROM_64K			.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 1-2 visible at start
//   - ROM_64K_ISR		.rom	64KB ROM in page 0-3 (0000h ~ FFFFh). Pages 0-2 visible at start
//   - ROM_ASCII8		.rom	128KB ROM using ASCII-8 mapper
//   - ROM_ASCII16		.rom	128KB ROM using ASCII-16 mapper
//   - ROM_KONAMI		.rom	128KB ROM using Konami mapper (8KB segments)
//   - ROM_KONAMI_SCC	.rom	128KB ROM using Konami SCC mapper (8KB segments)
//   - DOS1				.com	MSX-DOS 1 program (0100h~) No direct acces to Main-ROM
//   - DOS2				.com	MSX-DOS 2 program (0100h~) No direct acces to Main-ROM
Target = "ROM_32K";

//-- Mapper size
// ROMSize = 128;

//-- Install BDOS driver for ROM program? (0 = false, 1 = true)
// ROMDelayBoot = false;

//-- RAM in slot 0 and install ISR there (0 = false, 1 = true)
// InstallRAMISR = false;

//-- Type of custom ISR (for RAM or ROM)
//   - VBLANK		V-blank handler
//   - VHBLANK		V-blank and h-blank handler (V9938 or V9958)
//   - V9990		v-blank, h-blank and command end handler (V9990)
CustomISR = "VBLANK";

//-- Use banked call and add trampoline functions (0 = false, 1 = true)
BankedCall = true;

//-- Overwrite RAM starting address
// ForceRamAddr = 0;

//-- Debug flag (0 = false, 1 = true)
// Debug = false;

//-- Optim:
//   - Default
//   - Speed
//   - Size
Optim = "Size";

//-- Additionnal compilation flag
// CompileOpt = "";

//-- Skip file if compile data is newer than the source (0 = false, 1 = true)
// CompileSkipOld = false;

//-- Verbose mode (0 = false, 1 = true)
Verbose = true;

//*****************************************************************************
// EMULATOR SETING
//*****************************************************************************

//-- Emulator options: 0 or 1
// EmulMachine    = false;
// Emul60Hz       = false;
// EmulFullScreen = false;
// EmulMute       = false;
// EmulDebug      = false;
//-- Emulator extra parameters to be add to command-line
// EmulExtraParam = "";

//*****************************************************************************
// BUILD STEPS
//*****************************************************************************

// DoClean   = false;
// DoCompile = true;
// DoMake    = true;
// DoPackage = true;
// DoDeploy  = true;
DoRun     = false;
