//-- Node.js libraries
const fs = require('fs');
const path = require('path');

//-- MSXgl JS libraries
const util = require("./util.js"); 


if (RunDevice === "EASY-USB")
{
	util.print("Start Easy-USB device");
	if (Ext === "rom")
	{
		// Delete all .ROM files
		fs.readdirSync(RunDeviceOpt).forEach(file => {
			if (path.parse(file).ext.toUpperCase() === ".ROM")
			{
				util.delFile(RunDeviceOpt + file);
			}
		});

		util.print(`Copy ${ProjName}.rom to ${RunDeviceOpt}`, PrintDetail);
		fs.copyFileSync(`${ProjDir}/emul/rom/${ProjName}.rom`, `${RunDeviceOpt}${ProjName}.rom`);
	}
	else
		util.print(`EASY-USB only support execution of ROM files`, PrintWarning);
}
else if (RunDevice === "RISKY MSX")
{
	util.print("Start RISKY MSX device");
	if ((Ext === "rom") && (ROMSize <= 256))
	{
		let destFileName = false;
		switch (Target)
		{
		case "ROM_8K":
		case "ROM_16K":			destFileName = "cart.r16"; break;

		case "ROM_32K":			destFileName = "cart.r32"; break;

		case "ROM_48K":
		case "ROM_48K_ISR":
		case "ROM_64K":
		case "ROM_64K_ISR":		destFileName = "cart.r48"; break;

		case "ROM_ASCII8":		destFileName = "cart.a8k"; break;
		case "ROM_ASCII16":		destFileName = "cart.a16"; break;
		case "ROM_KONAMI":		destFileName = "cart.ko4"; break;
		case "ROM_KONAMI_SCC":	destFileName = "cart.ko5"; break;
		case "ROM_NEO8":		destFileName = "cart.n8k"; break;
		case "ROM_NEO16":		destFileName = "cart.n16"; break;
		}
		if (destFileName)
		{
			util.print(`Generate ${destFileName} file to ${RunDeviceOpt}`, PrintDetail);
			fs.copyFileSync(`${ProjDir}/emul/rom/${ProjName}.rom`, `${RunDeviceOpt}${destFileName}`);
		}
		else
			util.print(`RISKY MSX don't support the ${Target} format`, PrintWarning);
	}
	else
		util.print(`RISKY MSX only support execution of ROM files up to 256 KB`, PrintWarning);
}
else if (RunDevice === "PICOVERSE 2040")
{
	util.print("Start PicoVerse 2040 device");
	let validTarget = false;
	if (Ext === "rom")
	{
		let destFileName = `${ProjName}.uf2`;
		let tag = false;
		switch (Target)
		{
		case "ROM_8K":
		case "ROM_16K":			tag = "PLA-16"; break;
		case "ROM_32K":			tag = "PLA-32"; break;
		case "ROM_48K":
		case "ROM_48K_ISR":		tag = "PLN-48"; break;
		case "ROM_64K":
		case "ROM_64K_ISR":		tag = "PLN-64"; break;
		case "ROM_ASCII8":		tag = "ASC-08"; break;
		case "ROM_ASCII16":		tag = "ASC-16"; break;
		case "ROM_KONAMI":		tag = "Konami"; break;
		case "ROM_KONAMI_SCC":	tag = "KonSCC"; break;
		case "ROM_NEO8":		tag = "NEO-8"; break;
		case "ROM_NEO16":		tag = "NEO-16"; break;
		case "ROM_ASCII16X":	tag = "ASC-16X"; break;
		}
	
		if (tag)
		{
			validTarget = true;
			util.print(`Generate UF2 file (${destFileName})`, PrintDetail);
			fs.copyFileSync(`${OutDir}${ProjName}.rom`, `${OutDir}${ProjName}.${tag}.rom`);
			util.execSync(`${ToolsDir}/build/PicoVerse/2040/loadrom.exe -o ${OutDir}${destFileName} ${OutDir}${ProjName}.${tag}.rom`);
			if (RunDeviceOpt && fs.existsSync(RunDeviceOpt))
			{
				util.print(`Install ${destFileName} on PicoVerse 2040 cartridge (${RunDeviceOpt})`, PrintDetail);
				fs.copyFileSync(`${OutDir}${destFileName}`, `${RunDeviceOpt}/${destFileName}`);
			}
		}
	}
	if (!validTarget)
		util.print(`PICOVERSE 2040 don't support the ${Target} format`, PrintWarning);
}
else
	util.print(`Unknown run device '${RunDevice}'`, PrintWarning);