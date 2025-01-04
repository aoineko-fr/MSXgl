// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//-- Node.js libraries
const fs = require('fs');

//-- MSXgl JS libraries
const util = require("./util.js"); 

//***************************************************************************
// CHECK BASE PARAMETERS
//***************************************************************************

// Check MSX version
util.print(`» Machine: ${util.getMachineName(Machine)}`);

// Check project name
if (!ProjName)
{
	util.print(`Invalid project name ${ProjName}`, PrintError);
	process.exit(20);
}

// Check project modules
if (!ProjModules.length)
{
	util.print(`ProjModules not defined. Adding '${ProjName}' to build list`, PrintDetail);
	ProjModules.push(ProjName);
}

// Project segments base name
if (!ProjSegments)
{
	util.print(`ProjSegments not defined. Using '${ProjName}'`, PrintDetail);
	ProjSegments = ProjName;
}
	
// Build library
if (!BuildLibrary && !fs.existsSync(`${OutDir}msxgl.lib`))
{
	util.print(`BuildLibrary is false but ${OutDir}msxgl.lib not found!`, PrintWarning);
	util.print("BuildLibrary will be activated");
	BuildLibrary = true;
}

//***************************************************************************
// CHECK TOOLS PATH
//***************************************************************************

// Check binary tools
if (!fs.existsSync(Compiler) && ( (process.platform === "win32") && !fs.existsSync(`${Compiler}.exe`)))
{
	util.print(`Invalid path to C Compiler ${Compiler}`, PrintError);
	process.exit(30);
}

if (!fs.existsSync(Assembler) && ( (process.platform === "win32") && !fs.existsSync(`${Assembler}.exe`)))
{
	util.print(`Invalid path to Assembler ${Assembler}`, PrintError);
	process.exit(35);
}

if (!fs.existsSync(Linker) && ( (process.platform === "win32") && !fs.existsSync(`${Linker}.exe`)))
{
	util.print(`Invalid path to Linker ${Linker}`, PrintError);
	process.exit(40);
}

if (!fs.existsSync(Hex2Bin) && ( (process.platform === "win32") && !fs.existsSync(`${Hex2Bin}.exe`)))
{
	util.print(`Invalid path to Packager ${Hex2Bin}`, PrintError);
	process.exit(50);
}

// BASIC/MSX-DOS specific tools
if (Ext !== "rom")
{
	if (!fs.existsSync(DskTool))
	{
		util.print(`Invalid path to DskTool ${DskTool}!`, PrintWarning);
		util.print("Only programs in ROM format will be testable with most emulators");
	}
}

// MSX-DOS specific tools
if (Ext === "com")
{
	if (!fs.existsSync(MSXDOS))
	{
		util.print(`Invalid path to MSX-DOS system files ${MSXDOS}!`, PrintWarning);
		util.print("Program will not be testable with emulator");
	}
}

// Emulator specific tools
if (DoRun)
{
	if ((!RunDevice) && !fs.existsSync(Emulator) && ( (process.platform === "win32") && !fs.existsSync(`${Emulator}.exe`)))
	{
		util.print(`Invalid path to Emulator ${Emulator}!`, PrintWarning);
		util.print("Disactivate DoRun option");
		DoRun = false; 
	}
}