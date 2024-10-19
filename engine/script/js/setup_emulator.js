// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//-- Node.js libraries
const fs = require('fs');
const path = require('path');

//-- MSXgl JS libraries
const util = require("./util.js"); 

//-----------------------------------------------------------------------------
// INITIALIZATION
//-----------------------------------------------------------------------------

const EmulatorName = path.parse(Emulator).name.toUpperCase();
const EmulatorPath = path.parse(Emulator).dir;
util.print(`Starting ${EmulatorName} emulator...`, PrintHighlight);

util.print("Emulator configuration:", PrintDetail);
util.print(`- EmulMachine: ${EmulMachine}`, PrintDetail);
util.print(`- Emul60Hz: ${Emul60Hz}`, PrintDetail);
util.print(`- EmulFullScreen: ${EmulFullScreen}`, PrintDetail);
util.print(`- EmulMute: ${EmulMute}`, PrintDetail);
util.print(`- EmulDebug: ${EmulDebug}`, PrintDetail);
util.print(`- EmulExtraParam: ${EmulExtraParam}`, PrintDetail);
util.print("Emulator extensions:", PrintDetail);
util.print(`- EmulSCC: ${EmulSCC}`, PrintDetail);
util.print(`- EmulMSXMusic: ${EmulMSXMusic}`, PrintDetail);
util.print(`- EmulMSXAudio: ${EmulMSXAudio}`, PrintDetail);
util.print(`- EmulOPL4: ${EmulOPL4}`, PrintDetail);
util.print(`- EmulPSG2: ${EmulPSG2}`, PrintDetail);
util.print(`- EmulV9990: ${EmulV9990}`, PrintDetail);
util.print(`- EmulPortA: ${EmulPortA}`, PrintDetail);
util.print(`- EmulPortB: ${EmulPortB}`, PrintDetail);

EmulatorArgs = EmulExtraParam;

if (EmulatorName === "MSXW") { EmulatorName = "MSX"; }

//*****************************************************************************
// openMSX
//*****************************************************************************
// Doc: https://openmsx.org/manual/commands.html
if (EmulatorName === "OPENMSX") {

	util.print("You can only change frequency by setting the machine");

	//---- Add launch options ----
	if (EmulMachine) {
		if ((Machine === "1") || (Machine === "12") || (Machine === "122P")) {
			if (Emul60Hz) {
				EmulatorArgs += " -machine C-BIOS_MSX1_JP";
			} else {
				EmulatorArgs += " -machine C-BIOS_MSX1_EU";
			}
		} else if ((Machine === "2") || (Machine === "2K") || (Machine === "22P")) {
			if (Emul60Hz) {
				EmulatorArgs += " -machine C-BIOS_MSX2_JP";
			} else {
				EmulatorArgs += " -machine C-BIOS_MSX2_EU";
			}
		} else if (Machine === "2P") {
			if (Emul60Hz) {
				EmulatorArgs += " -machine C-BIOS_MSX2+_JP";
			} else {
				EmulatorArgs += " -machine C-BIOS_MSX2+_EU";
			}
		} else if (Machine === "TR") {
			util.print("No MSX turbo R default machine in openMSX!", PrintError);
			process.exit(500);
		}
	}
	if (EmulFullScreen) { EmulatorArgs += ' -command "set fullscreen on"'; }
	if (EmulMute)       { EmulatorArgs += ' -command "set mute on"'; }
	if (EmulTurbo)		{ EmulatorArgs += ' -command "set fastforward true"'; }		

	//---- Handle extension ----
	let EmulExtCount = 0;
	if (Ext === "rom") { EmulExtCount++; } // Application cartridge
	if (Ext === "com") { EmulExtCount++; } // MSX-DOS cartridge
	if (EmulSCC)       { EmulExtCount++; }
	if (EmulMSXMusic)  { EmulExtCount++; }
	if (EmulMSXAudio)  { EmulExtCount++; }
	if (EmulOPL4)      { EmulExtCount++; }
	if (EmulPSG2)      { EmulExtCount++; }
	if (EmulV9990)     { EmulExtCount++; }
	if (EmulExtCount >= 3) { EmulatorArgs += " -exta slotexpander"; }
	if (EmulExtCount >= 6) { EmulatorArgs += " -extb slotexpander"; }
	util.print(`${EmulExtCount} extension found`);

	//---- Add launch program ----
	if (Ext === "bin")
	{
		if (Target === "BIN_TAPE")
			EmulatorArgs += ` -command "cassetteplayer insert ${ProjDir}emul/cas/${ProjName}.cas"`;
		else
			EmulatorArgs += ` -diska ${ProjDir}emul/bin`;
	}
	if (Ext === "rom") { 
		EmulatorArgs += ` -cart ${ProjDir}emul/rom/${ProjName}.rom`;
		if (ROMDelayBoot) { EmulatorArgs += ` -ext msxdos2 -diska ${ProjDir}emul/dsk/tmp`; }
	}
	if (Ext === "com")
	{ 
		if (Target === "DOS0")
			EmulatorArgs += ` -diska ${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`;
		else
			EmulatorArgs += ` -ext msxdos2 -diska ${ProjDir}emul/dos${DOS}`;
	}
	// if (Target === "DOS1" {
		// EmulatorArgs += ` -exta slotexpander -ext Panasonic_FS-FD1A -ext ram64k -diska ${ProjDir}emul/dos${DOS}`;
	// } else {
		// if (Ext === "com") { EmulatorArgs += ` -exta slotexpander -ext Panasonic_FS-FD1A -ext msxdos2 -ext ram512k -diska ${ProjDir}emul/dos${DOS}`; }
	// }
	// if (Ext === "com") { EmulatorArgs += ` -diska ${ProjDir}emul/dos${DOS} -ext msxdos2`; }
	// if (Ext === "bin") { EmulatorArgs += ` -ext ide -hda ${ProjDir}emul/dsk/${ProjName}.dsk`; }
	// if (Ext === "rom") { EmulatorArgs += ` -cart ${ProjDir}emul/rom/${ProjName}.rom`; }
	// if (Ext === "com") { EmulatorArgs += ` -ext ide -hda ${ProjDir}emul/dsk/${ProjName}.dsk -ext msxdos2`; }

	//---- Emulator extensions ----
	if (EmulSCC)      { EmulatorArgs += " -ext scc"; }
	if (EmulMSXMusic) { EmulatorArgs += " -ext fmpac"; }
	if (EmulMSXAudio) { EmulatorArgs += " -ext audio"; }
	if (EmulOPL4)     { EmulatorArgs += " -ext moonsound"; }
	if (EmulPSG2)     { EmulatorArgs += " -ext 2nd_PSG"; }
	if (EmulV9990)    { EmulatorArgs += ' -ext gfx9000 -command "after time 0 {set videosource gfx9000}"'; }

	//---- Emulator conenctors ----
	if (EmulPortA === "JOYSTICK") { EmulatorArgs += ' -command "plug joyporta msxjoystick1"'; }
	// if (EmulPortA === "KEYBOARD") { EmulatorArgs += ' -command "plug joyporta keyjoystick1"'; }
	if (EmulPortA === "MOUSE")    { EmulatorArgs += ' -command "plug joyporta mouse"'; }
	if (EmulPortA === "PADDLE")   { EmulatorArgs += ' -command "plug joyporta arkanoidpad"'; }
	if (EmulPortA === "NINJATAP") { EmulatorArgs += ' -command "plug joyporta ninjatap"'; }
	if (EmulPortB === "JOYSTICK") { EmulatorArgs += ' -command "plug joyportb msxjoystick2"'; }
	// if (EmulPortB === "KEYBOARD") { EmulatorArgs += ' -command "plug joyportb keyjoystick2"'; }
	if (EmulPortB === "MOUSE")    { EmulatorArgs += ' -command "plug joyportb mouse"'; }
	if (EmulPortB === "NINJATAP") { EmulatorArgs += ' -command "plug joyportb ninjatap"'; }
	if (EmulPortB === "PADDLE")   { EmulatorArgs += ' -command "plug joyportb arkanoidpad"'; }

	//---- Start emulator ----
	if (EmulDebug) {
		EmulatorArgs += " -ext debugdevice";
		// EmulatorArgs += ` -script ${ToolsDir}script/openMSX/debugger_pvm.tcl`;
	}
}

//*****************************************************************************
// Emulicious
//*****************************************************************************
// See ReadMe.txt and Emulicious.ini
else if (EmulatorName === "EMULICIOUS") {

	EmulatorArgs += " -set System=MSX";

	//---- Open/close debugger
	if (EmulDebug) {
		EmulatorArgs += " -set WindowDebuggerOpen=true -set DebuggerSuspendOnOpen=false";
	} else {
		EmulatorArgs += " -set WindowDebuggerOpen=false";
	}

	//---- Add launch options ----
	if (EmulMachine) {
		if ((Machine === "1") || (Machine === "12") || (Machine === "122P")) {
			EmulatorArgs += " -set MSXModel=0";
		} else if ((Machine === "2") || (Machine === "2K") || (Machine === "22P")) {
			EmulatorArgs += " -set MSXModel=1";
		} else if (Machine =="2P") {
			util.print("EmulMachine MSX2+ can't be use with Emulicious", PrintWarning);
		} else if (Machine =="TR") {
			util.print("EmulMachine MSX turbo R can't be use with Emulicious", PrintWarning);
		}
	}
	if (Emul60Hz) {
		EmulatorArgs += " -set MSXPAL=false";
	} else {
		EmulatorArgs += " -set MSXPAL=true";
	}
	if (EmulFullScreen) { EmulatorArgs += " -fullscreen"; }
	if (EmulMute)       { EmulatorArgs += " -muted"; }
	if (EmulTurbo)		{ EmulatorArgs += " -turbo"; }

	//---- Emulator extensions ----
	if (EmulSCC)      { EmulatorArgs += " -set MSXSCCCartridgeEnabled=true"; }
	// if (EmulMSXMusic) {}
	// if (EmulMSXAudio) {}
	// if (EmulOPL4)     {}
	// if (EmulPSG2)     {}
	// if (EmulV9990)    {}

	//---- Add launch program ----
	if (Ext === "rom") { EmulatorArgs += ` ${ProjDir}emul/rom/${ProjName}.rom`; }
	else 
	{
		util.print("Emulicious only support ROM format natively");
		if (Target === "BIN_TAPE")
			EmulatorArgs += ` ${ProjDir}emul/cas/${ProjName}.cas"`;
		else
			EmulatorArgs += ` ${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`;
	}
}

//*****************************************************************************
// fMSX
//*****************************************************************************
// Doc: https://fms.komkon.org/fMSX/fMSX.html#LABI
else if (EmulatorName === "FMSX") {
	util.print("Command line parameters are only fonctionnal since fMSX 6.0");

	//---- Add launch options ----
	if (EmulMachine) {
		if ((Machine === "1") || (Machine === "12") || (Machine === "122P")) {
			EmulatorArgs += " -msx1";
		} else if ((Machine === "2") || (Machine === "2K") || (Machine === "22P")) {
			EmulatorArgs += " -msx2";
		} else if (Machine =="2P") {
			EmulatorArgs += " -msx2+";
		} else if (Machine =="TR") {
			util.print("EmulMachine MSX turbo R can't be use with fMSX", PrintError);
			process.exit(510);
		}
	}
	if (Emul60Hz) {
		EmulatorArgs += " -ntsc";
	} else {
		EmulatorArgs += " -pal";
	}
	if (EmulFullScreen) { util.print("EmulFullScreen can't be use with fMSX", PrintWarning); }
	if (EmulMute)       { EmulatorArgs += " -nosound"; }
	if (EmulTurbo)		{ util.print("EmulTurbo can't be use with fMSX", PrintWarning); }

	//---- Emulator conenctors ----
	if      (EmulPortA === "JOYSTICK") { EmulatorArgs += ' -joy 1'; }
	else if (EmulPortA === "MOUSE")    { EmulatorArgs += ' -joy 3'; }
	else                               { EmulatorArgs += ' -joy 0'; }
	if      (EmulPortB === "JOYSTICK") { EmulatorArgs += ' -joy 1'; }
	else if (EmulPortB === "MOUSE")    { EmulatorArgs += ' -joy 3'; }
	else                               { EmulatorArgs += ' -joy 0'; }

	//---- Add launch program ----
	if (Ext === "bin")
	{
		if (Target === "BIN_TAPE")
			EmulatorArgs += ` -tape ${ProjDir}emul/cas/${ProjName}.cas"`;
		else
			EmulatorArgs += ` -diska ${ProjDir}/emul/dsk/${ProjName}.dsk`;
	}
	if (Ext === "rom") { EmulatorArgs += ` ${ProjDir}/emul/rom/${ProjName}.rom` }
	if (Ext === "com") { EmulatorArgs += ` -diska ${ProjDir}/emul/dsk/${ProjName}.dsk` }
}

//*****************************************************************************
// MSXEC
//*****************************************************************************
// Doc: http://cngsoft.no-ip.org/cpcec.htm
else if (EmulatorName === "MSXEC") {

	//---- Add launch options ----
	// 0: Generic 8K
	// 1: Generic 16K
	// 2: Konami SCC
	// 3: Konami 8K (no SRAM at all)
	// 4: ASCII 8K (no SRAM at all)
	// 5: ASCII 16K (no SRAM at all)
	// 6: Konami 8K + SRAM (16K SRAM set by BIT 4 and selected by BIT 5)
	// 7: ASCII 8K + SRAM / KOEI SRAM (8/16/32K SRAM set by BIT 5/6/7)
	// 8: ASCII 16K + SRAM (2/8K SRAM set by BIT 4)
	// 9: Miscellaneous
	switch(TargetType)
	{
	case "ROM_ASCII8":		EmulatorArgs += " -g4"; break;
	case "ROM_ASCII16":		EmulatorArgs += " -g5"; break;
	case "ROM_KONAMI":		EmulatorArgs += " -g3"; break;
	case "ROM_KONAMI_SCC":	EmulatorArgs += " -g2"; break;
	case "ROM_NEO8":
	case "ROM_NEO16":
		util.print("NEO mapper formats not supported by MSXEC", PrintError);
		process.exit(520);
	default:
		// EmulatorArgs += " -g1";
	}

	if (EmulMachine) {
		if ((Machine === "1") || (Machine === "12") || (Machine === "122P")) {
			EmulatorArgs += " -m0";
		} else if ((Machine === "2") || (Machine === "2K") || (Machine === "22P")) {
			EmulatorArgs += " -m1";
		} else if (Machine === "2P") {
			EmulatorArgs += " -m2";
		} else if (Machine === "TR") {
			util.print("EmulMachine MSX turbo R can't be use with MSXEC", PrintError);
			process.exit(530);
		}
	}
	if (Emul60Hz)       { util.print("Emul60Hz can't be use with MSXEC", PrintWarning); }
	if (EmulFullScreen) { EmulatorArgs += " -W"; }
	if (EmulMute)       { EmulatorArgs += " -S"; }
	if (EmulTurbo)		{ util.print("EmulTurbo can't be use with MSXEC", PrintWarning); }

	//---- Emulator conenctors ----
	// if      (EmulPortA === "JOYSTICK") { EmulatorArgs += ' -joy 1'; }
	// else if (EmulPortA === "MOUSE")    { EmulatorArgs += ' -joy 3'; }
	// else                               { EmulatorArgs += ' -joy 0'; }
	// if      (EmulPortB === "JOYSTICK") { EmulatorArgs += ' -joy 1'; }
	// else if (EmulPortB === "MOUSE")    { EmulatorArgs += ' -joy 3'; }
	// else                               { EmulatorArgs += ' -joy 0'; }

	//---- Add launch program ----
	if (Ext === "bin")
	{
		if (Target === "BIN_TAPE")
			EmulatorArgs += ` ${ProjDir}emul/cas/${ProjName}.cas"`;
		else
			EmulatorArgs += ` ${ProjDir}/emul/dsk/${ProjName}.dsk`;
	}
	if (Ext === "rom") { EmulatorArgs += ` ${ProjDir}/emul/rom/${ProjName}.rom` }
	if (Ext === "com") { EmulatorArgs += ` ${ProjDir}/emul/dsk/${ProjName}.dsk` }
}

//*****************************************************************************
// blueMSX
//*****************************************************************************
// Doc: http://www.msxblue.com/manual/commandlineargs_c.htm
else if (EmulatorName === "BLUEMSX") {

	//---- Add launch options ----
	if (EmulMachine) {
		if ((Machine === "1") || (Machine === "12") || (Machine === "122P")) {
			if (Emul60Hz) {
				EmulatorArgs += ' /machine "MSX - Japanese"';
			} else {
				EmulatorArgs += ' /machine "MSX"';
			}
		} else if ((Machine === "2") || (Machine === "2K") || (Machine === "22P")) {
			if (Emul60Hz) {
				EmulatorArgs += ' /machine "MSX2 - Japanese"';
			} else {
				EmulatorArgs += ' /machine "MSX2"';
			}
		} else if (Machine === "2P") {
			if (Emul60Hz) {
				EmulatorArgs += ' /machine "MSX2+"';
			} else {
				EmulatorArgs += ' /machine "MSX2+ - C-BIOS"';
				util.print("50Hz MSX2+ is C-BIOS based with BlueMSX");
			}
		} else if (Machine === "TR") {
			if (Emul60Hz) {
				EmulatorArgs += ' /machine "MSXturboR"';
			} else {
				EmulatorArgs += ' /machine "MSXturboR"';
				util.print("MSX turbo R is only 60Hz with BlueMSX", PrintWarning);
			}
		}
	}
	if (EmulFullScreen) { EmulatorArgs += " /fullscreen"; }
	if (EmulMute)       { util.print("EmulMute can't be use with BlueMSX", PrintWarning); }
	if (EmulTurbo)		{ util.print("EmulTurbo can't be use with BlueMSX", PrintWarning); }

	//---- Add launch program ----
	if (Ext === "bin")
	{
		if (Target === "BIN_TAPE")
			EmulatorArgs += ` /cas ${ProjDir}emul/cas/${ProjName}.cas"`;
		else
			EmulatorArgs += ` /diskA ${ProjDir}emul/dsk/${ProjName}.dsk`;
	}
	if (Ext === "rom") { EmulatorArgs += ` /rom1 ${ProjDir}emul/rom/${ProjName}.rom`; }
	if (Ext === "com") { EmulatorArgs += ` /diskA ${ProjDir}emul/dsk/${ProjName}.dsk`; }
}

//*****************************************************************************
// Mesei
//*****************************************************************************
else if (EmulatorName === "MEISEI") {
	//---- Add launch options ----
	if (EmulMachine)    { util.print("EmulMachine can't be use with Mesei", PrintWarning); }
	if (Emul60Hz)       { util.print("Emul60Hz can't be use with Mesei", PrintWarning); }
	if (EmulFullScreen) { util.print("EmulFullScreen can't be use with Mesei", PrintWarning); }
	if (EmulMute)       { util.print("EmulMute can't be use with Mesei", PrintWarning); }
	if (EmulTurbo)		{ util.print("EmulTurbo can't be use with Mesei", PrintWarning); }

	//---- Add launch program ----
	if (Ext === "rom") { 
		EmulatorArgs += ` ${ProjDir}/emul/rom/${ProjName}.rom`;
	} else {
		util.print("Mesei only support ROM format", PrintError);
		process.exit(540);
	}
}

//*****************************************************************************
// RuMSX
//*****************************************************************************

else if (EmulatorName === "MSX") {
	//---- Add launch options ----
	if (EmulMachine)    { util.print("EmulMachine can't be use with RuMSX", PrintWarning); }
	if (Emul60Hz)       { util.print("Emul60Hz can't be use with RuMSX", PrintWarning); }
	if (EmulFullScreen) { util.print("EmulFullScreen can't be use with RuMSX", PrintWarning); }
	if (EmulMute)       { EmulatorArgs += " -NoSoundOut"; }
	if (EmulTurbo)		{ util.print("EmulTurbo can't be use with RuMSX", PrintWarning); }

	//---- Add launch program ----
	if (Ext === "bin") { EmulatorArgs += ` -dirAsDisk -disk ${ProjDir}/emul/bin`; }
	if (Ext === "rom") { EmulatorArgs += ` -rom "${ProjDir}/emul/rom/${ProjName}.rom"`; }
	if (Ext === "com") { EmulatorArgs += ` -dirAsDisk -disk ${ProjDir}/emul/dos${DOS}`; }
}

//*****************************************************************************
// Unknow
//*****************************************************************************

else {
	util.print("Unknown emulator! Only EmulExtraParam parameters will be added to the command line", PrintWarning);
}
