//-- Node.js libraries
const fs = require('fs');
const path = require('path');

//-- MSXgl JS libraries
const util = require("./util.js"); 

if (Ext === "rom")
{
	util.print("Generating standalone package...", PrintHighlight);

	// Clean package folder
	let packageDir = `${ProjDir}package/`;
	if (fs.existsSync(packageDir))
		fs.rmSync(packageDir, { recursive: true });
	fs.mkdirSync(packageDir);

	// Copy base package files
	fs.cpSync(`${ToolsDir}build/standalone/Emulicious`, packageDir, { recursive: true });

	// Include ROM to the JAR
	util.execSync(`${ToolsDir}build/7zip/7za a ${packageDir}Emulicious.jar ${ProjDir}emul/rom/${ProjName}.${Ext}`);

	// Generate start script
	let batTxt = `start Emulicious -set System=MSX -set Update=0 -set MSXSCCCartridgeEnabled=true -set IntegerScaling=true -set MSXRAMBankShift=6 -set MSXPAR=1.1428572`;
	if (Machine === "1")
		batTxt += " -set MSXModel=0"; // MSX1
	else
		batTxt += " -set MSXModel=1"; // MSX2
	if (Emul60Hz)
		batTxt += " -set MSXPAL=false"; // NTSC (60 Hz)
	else
		batTxt += " -set MSXPAL=true"; // PAL (50 Hz)
	batTxt += ` -nomenu -nodragandrop -fullscreen resource:${ProjName}.${Ext}`;
	fs.writeFileSync(`${packageDir}${ProjName}.bat`, batTxt);

	util.print("Success", PrintSuccess);
}
else
	util.print(`Standalone package is only compatible with ROM projects (${Ext})`, PrintWarning);