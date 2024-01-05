// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

// Configure alias
if (Target === "DOS")     Target = "DOS1";
if (Target === "ROM")     Target = "ROM_32K";
if (Target === "BAS")     Target = "BIN";
if (Target === "USR")     Target = "BIN_USR";
if (Target === "BOOT")    Target = "DOS0";
if (Target === "ROM_K4")  Target = "ROM_KONAMI";
if (Target === "ROM_K5")  Target = "ROM_KONAMI_SCC";

//*****************************************************************************
// BASIC
//*****************************************************************************

//-----------------------------------------------------------------------------
if (Target === "BIN")
{
	Ext = "bin";
	Crt0 = "crt0_basic";
	StartAddr = 0x8000;
	CodeAddr = 0x8000;
	RamAddr = 0; // Right after the code
	FillSize = 0;

	TargetDesc = "BASIC binary program (8000h~)";
}
//-----------------------------------------------------------------------------
else if (Target === "BIN_USR")
{
	Ext = "bin";
	Crt0 = "crt0_basic_usr";
	StartAddr = USRAddr;
	CodeAddr = USRAddr;
	RamAddr = 0; // Right after the code
	FillSize = 0;

	TargetDesc = "BASIC USR binary driver (C000h~)";
}

//*****************************************************************************
// PLAIN ROM
//*****************************************************************************

//-----------------------------------------------------------------------------
else if (Target === "ROM_8K")
{
	Ext = "rom";
	Crt0 = "crt0_rom16";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0x8000;
	FillSize = 8*1024;
	ROMFirstPage = 1;
	ROMLastPage = 1;
	
	TargetDesc = "8KB ROM in page 1 (4000h ~ 5FFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_8K_P2")
{
	Ext = "rom";
	Crt0 = "crt0_rom16";
	StartAddr = 0x8000;
	CodeAddr = 0x8000;
	RamAddr = 0xC000;
	FillSize = 8*1024;
	ROMFirstPage = 2;
	ROMLastPage = 2;

	TargetDesc = "8KB ROM in page 2 (8000h ~ 9FFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_16K")
{
	Ext = "rom";
	Crt0 = "crt0_rom16";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0x8000;
	FillSize = 16*1024;
	ROMFirstPage = 1;
	ROMLastPage = 1;

	TargetDesc = "16KB ROM in page 1 (4000h ~ 7FFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_16K_P2")
{
	Ext = "rom";
	Crt0 = "crt0_rom16";
	StartAddr = 0x8000;
	CodeAddr = 0x8000;
	RamAddr = 0xC000;
	FillSize = 16*1024;
	ROMFirstPage = 2;
	ROMLastPage = 2;

	TargetDesc = "16KB ROM in page 2 (8000h ~ BFFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_32K")
{
	Ext = "rom";
	Crt0 = "crt0_rom32";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	FillSize = 32*1024;
	ROMFirstPage = 1;
	ROMLastPage = 2;

	TargetDesc = "32KB ROM in page 1&2 (4000h ~ BFFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_48K")
{
	Ext = "rom";
	Crt0 = "crt0_rom48";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	FillSize = 48*1024;
	ROMFirstPage = 0;
	ROMLastPage = 2;

	TargetDesc = "48KB ROM in page 0-2 (0000h ~ BFFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_48K_ISR")
{
	Ext = "rom"
	Crt0 = "crt0_rom48_isr";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	FillSize = 48*1024;
	ROMFirstPage = 0;
	ROMLastPage = 2;
	ROMWithISR = true;

	TargetDesc = "48KB ROM in page 0-2 (0000h ~ BFFFh) with ISR replacement";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_64K")
{
	Ext = "rom";
	Crt0 = "crt0_rom48";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	FillSize = 64*1024;
	ROMFirstPage = 0;
	ROMLastPage = 3;

	TargetDesc = "64KB ROM in page 0-3 (0000h ~ FFFFh)";
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_64K_ISR")
{
	Ext = "rom";
	Crt0 = "crt0_rom48_isr";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	FillSize = 64*1024;
	ROMFirstPage = 0;
	ROMLastPage = 3;
	ROMWithISR = true;

	TargetDesc = "64KB ROM in page 0-3 (0000h ~ FFFFh) with ISR replacement";
}

//*****************************************************************************
// MAPPED ROM
//*****************************************************************************

//-----------------------------------------------------------------------------
else if (Target === "ROM_ASCII8")
{
	if(!ROMMainSegments)
		ROMMainSegments = 4;
	Mapper = "ROM_ASCII8";
	Target = `ROM_ASCII8_${ROMSize}K`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 8*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x4000;
	Bank1Addr = 0x6000;
	Bank2Addr = 0x8000;
	Bank3Addr = 0xA000;

	TargetDesc = `${ROMSize}KB ROM using ASCII-8 mapper (starting at 4000h)`;
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_ASCII16")
{
	if(!ROMMainSegments)
		ROMMainSegments = 2;
	Mapper = "ROM_ASCII16";
	Target = `ROM_ASCII16_${ROMSize}K`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 16*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x4000;
	Bank1Addr = 0x8000;

	TargetDesc = `${ROMSize}KB ROM using ASCII-16 mapper (starting at 4000h)`;
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_KONAMI")
{
	if(!ROMMainSegments)
		ROMMainSegments = 4;
	Mapper = "ROM_KONAMI";
	Target = `ROM_KONAMI_${ROMSize}K`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 8*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank1Addr = 0x6000;
	Bank2Addr = 0x8000;
	Bank3Addr = 0xA000;

	TargetDesc = `${ROMSize}KB ROM using KONAMI mapper (starting at 4000h)`;
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_KONAMI_SCC")
{
	if(!ROMMainSegments)
		ROMMainSegments = 4;
	Mapper = "ROM_KONAMI_SCC";
	Target = `ROM_KONAMI_SCC_${ROMSize}K`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 8*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x4000;
	Bank1Addr = 0x6000;
	Bank2Addr = 0x8000;
	Bank3Addr = 0xA000;

	TargetDesc = `${ROMSize}KB ROM using KONAMI SCC mapper (starting at 4000h)`;
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_NEO8")
{
	if(!ROMMainSegments)
		ROMMainSegments = 4;
	Mapper = "ROM_NEO8";
	Target = `ROM_NEO8_${ROMSize/1024}M`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 8*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x0000;
	Bank1Addr = 0x2000;
	Bank2Addr = 0x4000;
	Bank3Addr = 0x6800;
	Bank4Addr = 0x8000;
	Bank5Addr = 0xA000;

	TargetDesc = `${ROMSize/1024}MB ROM using NEO-8 mapper (starting at 4000h)`;
}
//-----------------------------------------------------------------------------
else if (Target === "ROM_NEO16")
{
	if(!ROMMainSegments)
		ROMMainSegments = 2;
	Mapper = "ROM_NEO16";
	Target = `ROM_NEO16_${ROMSize/1024}M`;
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x0000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 16*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x0000;
	Bank1Addr = 0x4000;
	Bank2Addr = 0x8000;

	TargetDesc = `${ROMSize/1024}MB ROM using NEO-16 mapper (starting at 4000h)`;
}

//*****************************************************************************
// MSX-DOS
//*****************************************************************************

//-----------------------------------------------------------------------------
else if (Target === "DOS0")
{
	DOS = 0;
	Ext = "com";
	Crt0 = "crt0_dos";
	StartAddr = 0x0100;
	CodeAddr = 0x0100;
	RamAddr = 0; // Right after the code
	FillSize = 0;

	TargetDesc = "Direct disk program using custom boot sector (starting at 0100h)";
}
//-----------------------------------------------------------------------------
else if (Target === "DOS1")
{
	DOS = 1;
	Ext = "com";
	Crt0 = "crt0_dos";
	StartAddr = 0x0100;
	CodeAddr = 0x0100;
	RamAddr = 0; // Right after the code
	FillSize = 0;

	TargetDesc = "MSX-DOS 1 program (starting at 0100h)";
}
//-----------------------------------------------------------------------------
else if (Target === "DOS2")
{
	DOS = 2;
	Ext = "com";
	Crt0 = "crt0_dos";
	StartAddr = 0x0100;
	CodeAddr = 0x0100;
	RamAddr = 0; // Right after the code
	FillSize = 0;

	TargetDesc = "MSX-DOS 2 program (starting at 0100h)";
}
//-----------------------------------------------------------------------------
else if (Target === "DOS2_MAPPER")
{
	if(!ROMMainSegments)
		ROMMainSegments = 2;
	Mapper = "ROM_ASCII16";
	Ext = "rom";
	Crt0 = "crt0_rom_mapper";
	StartAddr = 0x4000;
	CodeAddr = 0x4000;
	RamAddr = 0xC000;
	MapperSize = ROMSize*1024;
	SegSize = 16*1024;
	FillSize = ROMMainSegments * SegSize;
	Bank0Addr = 0x4000;
	Bank1Addr = 0x8000;

	TargetDesc = "MSX-DOS 2 launcher to RAM mapper (program starting at 4000h)";
}

//-----------------------------------------------------------------------------
else
{
	console.log(`${ColorRed} Error: Unknow target ${Target}!${ColorReset}`);
	process.exit(1);
}