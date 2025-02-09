//-- Node.js libraries
const fs = require('fs');
const path = require('path');

//-- MSXgl JS libraries
const util = require("./util.js"); 


if (RunDevice === "EASY-USB")
{
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
	if ((Ext === "rom") && (ROMSize <= 256))
	{
		let destFileName = false;
		switch (Target)
		{
		case "ROM_8K":
		case "ROM_16K":
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
else
	util.print(`Unknown run device '${RunDevice}'`, PrintWarning);