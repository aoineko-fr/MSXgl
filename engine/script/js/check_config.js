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
switch(Machine) {
	case "1":  util.print("» Machine: MSX 1"); break;
	case "12": util.print("» Machine: MSX 1&2"); break;
	case "2":  util.print("» Machine: MSX 2"); break;
	case "2P": util.print("» Machine: MSX 2+"); break;
	case "TR": util.print("» Machine: MSX turbo R"); break;
	default:
		util.print("Unknow MSX Version", PrintError);
		process.exit(10);
}

// Check project name
if (!ProjName) {
	util.print(`Invalid project name ${ProjName}`, PrintError);
	process.exit(20);
}

// Check project modules
if (!ProjModules) {
	util.print(`ProjModules not defined. Adding '${ProjName}' to build list`, PrintDetail);
	ProjModules = ProjName;
}

// Project segments base name
if ((MapperSize != 0) && (!ProjSegments)) {
	util.print(`ProjSegments not defined. Using '${ProjName}'`, PrintDetail);
	ProjSegments = ProjName;
}

//***************************************************************************
// CHECK TOOLS PATH
//***************************************************************************

if(process.platform == "win32")
{
	// Check binary tools
	if(!fs.existsSync(Compiler)) {
		util.print(`Invalid path to C Compiler ${Compiler}`, PrintError);
		process.exit(30);
	}

	if(!fs.existsSync(Assembler)) {
		util.print(`Invalid path to Assembler ${Assembler}`, PrintError);
		process.exit(35);
	}

	if(!fs.existsSync(Linker)) {
		util.print(`Invalid path to Linker ${Linker}`, PrintError);
		process.exit(40);
	}
}

if(!fs.existsSync(Hex2Bin)) {
	util.print(`Invalid path to Packager ${Hex2Bin}`, PrintError);
	process.exit(50);
}

// BASIC/MSX-DOS specific tools
if (Ext != "rom") {
	if (!fs.existsSync(DskTool)) {
		util.print(`Invalid path to DskTool ${DskTool}`, PrintWarning);
		util.print("Only programs in ROM format will be testable with most emulators");
	}
}

// MSX-DOS specific tools
if (Ext == "com") {
	if (!fs.existsSync(MSXDOS)) {
		util.print(`Invalid path to MSX-DOS system files ${MSXDOS}`, PrintWarning);
		util.print("Program will not be testable with emulator");
	}
}

// Emulator specific tools
if (DoRun) {
	if (!fs.existsSync(Emulator)) {
		util.print(`Invalid path to Emulator ${Emulator}`, PrintWarning);
		util.print("Disactivate DoRun option");
		DoRun = false; 
	}
}