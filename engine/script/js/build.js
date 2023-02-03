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
const compiler = require("./compiler.js"); 

//_____________________________________________________________________________
//  ▄▄▄▄      ▄  ▄▄ 
//   ██  ██▀▄ ▄  ██▀
//  ▄██▄ ██ █ ██ ▀█▄
//_____________________________________________________________________________

//=============================================================================
// BUILD INITIALIZATION
//=============================================================================

//-- Setup global variables
require("./setup_global.js"); 

//-- Default configuration
if (!fs.existsSync(`${RootDir}projects/default_config.js`))
	util.copyFile(`${RootDir}engine/script/js/default_config.js`, `${RootDir}projects/default_config.js`);
require(`${RootDir}projects/default_config.js`);

//-- Project specific overwrite
require(`${ProjDir}project_config.js`);

//-- Setup command line overwrite parameters
const CommandArgs = process.argv.slice(2);
for (let i=0; i < CommandArgs.length; i++)
{
	const arg = CommandArgs[i].toLowerCase();
	if(arg.startsWith("target="))
	{
		let val = CommandArgs[i].substring(7);
		if (val)
		{
			Target = val;
			util.print(`Command line overwrite => Target=${Target}`, PrintDetail);
		}
	}
	else if(arg.startsWith("machine="))
	{
		let val = CommandArgs[i].substring(7);
		if (val)
		{
			Machine = val;
			util.print(`Command line overwrite => Machine=${Machine}`, PrintDetail);
		}
	}
	else if(arg.startsWith("projname="))
	{
		let val = CommandArgs[i].substring(9);
		if (val)
		{
			ProjName = val;
			// ProjModules = [ ProjName ];
			// ProjSegments = ProjName;
			util.print(`Command line overwrite => ProjName=${ProjName}`, PrintDetail);
		}
	}
	else if(arg.startsWith("romsize="))
	{
		let val = CommandArgs[i].substring(8);
		if (val)
		{
			ROMSize = parseInt(val);
			util.print(`Command line overwrite => ROMSize=${ROMSize}`, PrintDetail);
		}
	}
	else if(arg === "delay")
	{
		ROMDelayBoot = true;
		util.print(`Command line overwrite => ROMDelayBoot=${ROMDelayBoot}`, PrintDetail);
	}
	else if(arg === "ramisr")
	{
		InstallRAMISR = true;
		util.print(`Command line overwrite => InstallRAMISR=${InstallRAMISR}`, PrintDetail);
	}
	else
	{
		util.print(`Unknow command line overwrite '${arg}'`, PrintWarning);
	}
}

//-- Sub-project specific overwrite
if (fs.existsSync(`${ProjDir}${ProjName}.js`))
{
	util.print(`Sub-project configuration found '${ProjDir}${ProjName}.js'`, PrintDetail);
	require(`${ProjDir}${ProjName}.js`);
}

//-- Validate enum
Machine   = Machine.toUpperCase();
Target    = Target.toUpperCase();
CustomISR = CustomISR.toUpperCase();
AsmOptim  = AsmOptim.toUpperCase();
Optim     = Optim.toUpperCase();
EmulPortA = EmulPortA.toUpperCase();
EmulPortB = EmulPortB.toUpperCase();

//-- Target specific initialization
require("./setup_target.js");

//=============================================================================
// DISPLAY INFO
//=============================================================================

// console.clear();

//-- Create out directory
if (!fs.existsSync(OutDir))
	fs.mkdirSync(OutDir);

//-- Setup log file
if (!LogFileName)
	LogFileName = `log_${util.getDateTag()}.txt`;
if (fs.existsSync(`${OutDir}${LogFileName}`))
	util.delFile(`${OutDir}${LogFileName}`);

//-- Display information
process.title = `MSXgl Build Tool – ${ProjName} – ${Target} – MSX ${Machine}`;

//-- Project specific overwrite
util.print("╔═══════════════════════════════════════════════════════════════════════════╗", PrintBG);
util.print("║                                                                           ║", PrintBG);
util.print("║  ██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█                                         ║", PrintBG);
util.print("║  ██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █                                         ║", PrintBG);
util.print("║  █  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █                                         ║", PrintBG);
util.print("║  █▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█                                         ║", PrintBG);
util.print("║                                                                           ║", PrintBG);
util.print("║   ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄                               ║", PrintBG);
util.print("║   ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██                               ║", PrintBG);
util.print("║   ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄                              ║", PrintBG);
util.print("║                                                                           ║", PrintBG);
util.print("╚═══════════════════════════════════════════════════════════════════════════╝", PrintBG);

util.print(`MSXgl Build Tool using Node.js ${process.version} (${process.platform})\n`);

if (LogFile)
	util.print(`Log to file: ${LogFileName}`, PrintDetail);

//-- Parmaters validation
require("./check_config.js");

util.print(`» Target: ${TargetDesc}`);

util.print("Project paths:", PrintDetail);
util.print(`- ProjDir: ${ProjDir}`, PrintDetail);
util.print(`- OutDir: ${OutDir}`, PrintDetail);
util.print(`- RootDir: ${RootDir}`, PrintDetail);
util.print(`- LibDir: ${LibDir}`, PrintDetail);
util.print(`- ToolsDir: ${ToolsDir}`, PrintDetail);

//_____________________________________________________________________________
//   ▄▄  ▄▄
//  ██ ▀ ██  ▄███ ▄▀██ ██▀▄
//  ▀█▄▀ ▀█▄ ▀█▄▄ ▀▄██ ██ █
//_____________________________________________________________________________
if (DoClean)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ CLEAN                                                                     │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print(" Cleaning...", PrintHighlight);

	//-- Remove out directory
	if (fs.existsSync(OutDir))
	{
		util.print(`Removing ${OutDir} directory...`);
		fs.rmSync(OutDir, { recursive: true });
	}

	//-- Remove emul directory
	if (fs.existsSync(`${ProjDir}emul`))
	{
		util.print(`Removing ${ProjDir}emul directory...`);
		fs.rmSync(`${ProjDir}emul`, { recursive: true });
	}

	return;
}

//_____________________________________________________________________________
//   ▄▄                 ▄  ▄▄
//  ██ ▀ ▄█▀▄ ▄█▄█ ██▀▄ ▄  ██  ▄███
//  ▀█▄▀ ▀█▄▀ ██ █ ██▀  ██ ▀█▄ ▀█▄▄
//_________________▀▀__________________________________________________________
if (DoCompile)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ COMPILE                                                                   │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print(`Compiling ${ProjName} using SDCC...`, PrintHighlight);

	//=========================================================================
	// CREATE CTR0 CONFIG FILE
	//=========================================================================
	util.print("Creating crt0 config file...");

	let conf = "; MSXgl | Crt0 config file generated by the Build tool\n";
	conf += `ADDR_BOOT=0x${util.getHex(StartAddr)}\n`;
	if (Mapper)                  conf += `ROM_MAPPER=${Mapper}\n`;
	if (ROMDelayBoot)            conf += "ROM_DELAY=1\n";
	if (BankedCall)              conf += "ROM_BCALL=1\n";
	if (InstallRAMISR)           conf += "ROM_RAMISR=1\n";
	if (CustomISR === "VHBLANK") conf += "ROM_ISR=ISR_VHBLANK\n";
	if (CustomISR === "V9990")   conf += "ROM_ISR=ISR_V9990\n";
	if (DOSParseArg)             conf += "DOS_PARSEARG=1\n";
	if (Target === "DOS0")       conf += "DOS_ISR=1\n";
	// if (Machine != "1")          conf += "ISR_SET_S0=1\n";
	if (AppSignature)
	{
		conf += "APP_SIGN=1\n";
		let appComp = AppCompany;
		if (util.isString(appComp))
			appComp = `"${AppCompany}"`;
		conf += `APP_SIGN_NAME=${appComp}\n`;
		let appId = AppID;
		if (util.isString(appId))
			appId = `"${AppID}"`;
		conf += `APP_SIGN_ID=${appId}\n`;
		if (AppExtra.length) conf += `APP_SIGN_EXTRA=${AppExtra.join(", ")}\n`;
	}

	util.print("----------------------------------------", PrintDetail);
	util.print(conf, PrintDetail);
	util.print("----------------------------------------", PrintDetail);
	fs.writeFileSync(`${OutDir}crt0_config.asm`, conf);

	//=========================================================================
	// UPDATE BUILD VERSION HEADER FILE
	//=========================================================================
	if (BuildVersion)
	{
		util.print("Updating build version...");

		let Version = 0;
		if (fs.existsSync("./version.h"))
		{
			const data = fs.readFileSync("./version.h");
			Version = data.slice(22).toString();
		}
		Version++;
		fs.writeFileSync("./version.h", `#define BUILD_VERSION ${Version}\r\n`);

		util.print(`New build version: ${Version}`);
	}

	//=========================================================================
	// GENERATE MODULES LIST
	//=========================================================================

	// Add crt0 source to build list (it must be the first in the list)
	SrcList = [ `${LibDir}src/crt0/${Crt0}.asm` ];
	RelList = [];
	LibList = [];

	// Add project sources to build list
	for (let i=0; i < ProjModules.length; i++)
	{
		if (!fs.existsSync(`./${ProjModules[i]}.c`))
		{
			util.print(`Source file ${ProjModules[i]}.c not found!`, PrintError);
			process.exit(100);
		}

		SrcList.push(`./${ProjModules[i]}.c`);

		let fileName = path.parse(ProjModules[i]).name;
		RelList.push(`${OutDir}${fileName}.rel`);
	}

	// Add modules sources to build list
	if (BuildLibrary)
	{
		util.print(`» MSXgl Modules: ${LibModules}`);
		for (let i=0; i < LibModules.length; i++)
		{
			if (!fs.existsSync(`${LibDir}src/${LibModules[i]}.c`)) {
				util.print(`Module ${LibModules[i]}.c not found!`, PrintError);
				process.exit(110);
			}

			SrcList.push(`${LibDir}src/${LibModules[i]}.c`);

			let fileName = path.parse(LibModules[i]).name;
			LibList.push(`${OutDir}${fileName}.rel`);
		}
	}

	// Additional source files to build
	for (let i=0; i < AddSources.length; i++)
	{
		if (!fs.existsSync(`./${AddSources[i]}`))
		{
			util.print(`Additionnal source ${AddSources[i]} not found!`, PrintError);
			process.exit(120);
		}

		SrcList.push(`./${AddSources[i]}`);

		let fileName = path.parse(AddSources[i]).name;
		RelList.push(`${OutDir}${fileName}.rel`);
	}

	//-- Overwrite RAM start address
	if (ForceRamAddr)
	{
		util.print(`» Force RAM address to ${ForceRamAddr}`);
		RamAddr = ForceRamAddr;
	}

	//=========================================================================
	// COMPILE ALL MODULES
	//=========================================================================
	for (let i = 0; i < SrcList.length; i++)
	{
		compiler.compile(SrcList[i]);
	}

	//=========================================================================
	// COMPILE MAPPER SEGMENT
	//=========================================================================
	MapperBanks = "";
	if (MapperSize)
	{
		let FirstSeg = FillSize / SegSize;
		let LastSeg = (MapperSize / SegSize) - 1;
		const segExtList = [ "c", "s", "asm" ];
		const bankAddrList = [ Bank0Addr, Bank1Addr, Bank2Addr, Bank3Addr ];

		util.print(`Search for extra mapper segments to compile [${FirstSeg}-${LastSeg}]...`, PrintHighlight);
		for (let e = 0; e < segExtList.length; e++)
		{
			for (let s = FirstSeg; s <= LastSeg; s++)
			{
				let hex = util.getHex(s);
				for (let b = 0; b < 4; b++)
				{
					let bankAddr = bankAddrList[b];
					if (bankAddr)
					{
						let segName = `${ProjSegments}_s${s}_b${b}`;
						if (fs.existsSync(`${segName}.${segExtList[e]}`))
						{
							let bankHex = util.getHex(bankAddr);
							util.print(`Segment found: ${segName}.${segExtList[e]} (addr: ${hex}${bankHex})`);
							compiler.compile(`${ProjDir}${segName}.${segExtList[e]}`, SegSize, s);
							MapperBanks += `-Wl-b_SEG${s}=0x${hex}${bankHex} `;
							RelList.push(`${OutDir}${segName}.rel`);
						}
					}
				}
			}
		}
	}
//-- end if (DoCompile)
}

//_____________________________________________________________________________
//  ▄▄   ▄       ▄▄
//  ██   ▄  ██▀▄ ██▄▀
//  ██▄▄ ██ ██ █ ██ █
//_____________________________________________________________________________
if (DoMake)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ LINK                                                                      │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");

	//=========================================================================
	// Generate Library
	//=========================================================================
	if (BuildLibrary)
	{
		util.print("Generate msxgl.lib...", PrintHighlight);

		// if (fs.existsSync(`${OutDir}msxgl.lib`))
			// util.delFile(`${OutDir}msxgl.lib`);

		let libStr = LibList.join(" ");
		let SDARParam = `-rc ${OutDir}msxgl.lib ${libStr}`
		let err = util.execSync(`${MakeLib} ${SDARParam}`);
		if(err)
		{
			util.print(`Lib generation error! Code: ${err}`, PrintError);
			process.exit(1);
		}
		util.print("Success", PrintSuccess);
	}

	//=========================================================================
	// Link Program
	//=========================================================================
	util.print(`Link ${ProjName} project using SDCC...`, PrintHighlight);
	util.execSync(`${Linker} --version`); // display SDCC version

	if (Optim === "SPEED") LinkOpt += " --opt-code-speed";
	if (Optim === "SIZE")  LinkOpt += " --opt-code-size";
	if (Debug)             LinkOpt += " --debug";

	let SDCCParam = `-mz80 --vc --no-std-crt0 -L${ToolsDir}sdcc/lib/z80 --code-loc 0x${util.getHex(CodeAddr)} --data-loc 0x${util.getHex(RamAddr)} ${LinkOpt} ${MapperBanks} ${OutDir}${Crt0}.rel ${OutDir}msxgl.lib ${RelList.join(" ")} -o ${OutDir}${ProjName}.ihx`;
	let err = util.execSync(`${Linker} ${SDCCParam}`);
	if(err)
	{
		util.print(`Link error! Code: ${err}`, PrintError);
		process.exit(1);
	}
	util.print("Success", PrintSuccess);
}

//_____________________________________________________________________________
//  ▄▄▄            ▄▄
//  ██▄▀ ▄▀██ ▄█▀▀ ██▄▀ ▄▀██ ▄▀██ ▄███
//  ██   ▀▄██ ▀█▄▄ ██ █ ▀▄██  ▀██ ▀█▄▄
//____________________________▀▀_______________________________________________
if (DoPackage)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ PACKAGE                                                                   │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print("Packaging binary...", PrintHighlight);

	//=========================================================================
	// MSXhex
	//=========================================================================
	if ((Ext === "rom") && (MapperSize))
		H2BParam = `${OutDir}${ProjName}.ihx -e ${Ext} -s 0x${util.getHex(StartAddr)} -l ${MapperSize} -b ${SegSize}`;
	else
		H2BParam = `${OutDir}${ProjName}.ihx -e ${Ext} -s 0x${util.getHex(StartAddr)} -l ${FillSize}`;
	let err = util.execSync(`${Hex2Bin} ${H2BParam}`);
	if(err)
	{
		util.print(`Package error! Code: ${err}`, PrintError);
		process.exit(1);
	}
	util.print("Success", PrintSuccess);
}

//_____________________________________________________________________________
//  ▄▄▄            ▄▄
//  ██ █ ▄███ ██▀▄ ██  ▄█▀▄ █ ██
//  ██▄▀ ▀█▄▄ ██▀  ▀█▄ ▀█▄▀  ▀██
//____________▀▀_____________▀▀________________________________________________
if (DoDeploy)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ DEPLOY                                                                    │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print(`Deploying ${Target}...`, PrintHighlight);

	//=========================================================================
	// CREATE OUTPUT DIRECTORY
	//=========================================================================
	if (!fs.existsSync(`${ProjDir}emul`)) fs.mkdirSync(`${ProjDir}emul`);
	if (Ext === "rom")
	{
		if (!fs.existsSync(`${ProjDir}emul/rom`)) fs.mkdirSync(`${ProjDir}emul/rom`);
		if (ROMDelayBoot && !fs.existsSync(`${ProjDir}emul/dsk/tmp`)) fs.mkdirSync(`${ProjDir}emul/dsk/tmp`);
	}
	else if (Ext === "bin")
	{
		if (!fs.existsSync(`${ProjDir}emul/bin`)) fs.mkdirSync(`${ProjDir}emul/bin`);
		if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
	}
	else if (Ext === "com")
	{
		if (!fs.existsSync(`${ProjDir}emul/dos${DOS}`)) fs.mkdirSync(`${ProjDir}emul/dos${DOS}`);
		if (DOS === 1)
		{
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}/COMMAND.COM`)) util.copyFile(`${MSXDOS}COMMAND.COM`, `${ProjDir}emul/dos${DOS}/COMMAND.COM`);
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}/MSXDOS.SYS`))  util.copyFile(`${MSXDOS}MSXDOS.SYS`, `${ProjDir}emul/dos${DOS}/MSXDOS.SYS`);
		}
		else if (DOS === 2)
		{
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}/COMMAND2.COM`)) util.copyFile(`${MSXDOS}COMMAND2.COM`, `${ProjDir}emul/dos${DOS}/COMMAND2.COM`);
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}/MSXDOS2.SYS`))  util.copyFile(`${MSXDOS}MSXDOS2.SYS`, `${ProjDir}emul/dos${DOS}/MSXDOS2.SYS`);
		}
		if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
	}

	let DskToolPath = path.parse(DskTool).dir + '/';
	let DskToolName = path.parse(DskTool).base;
	if (process.platform === "linux")
		DskToolName = `./${DskToolName}`;
	let projNameShort = ProjName.substring(0, 8);

	//-------------------------------------------------------------------------
	// ROM TARGET
	//-------------------------------------------------------------------------
	if (Ext === "rom")
	{
		//---- Copy program file ----
		util.print("Copy ROM file to emul/rom/ folder");
		util.copyFile(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/rom/${ProjName}.${Ext}`);

		//---- Copy symbols files ----
		if (DebugSymbols)
		{
			util.print("Copy symbols files to destination directory");
			if (fs.existsSync(`${OutDir}${ProjName}.map`)) util.copyFile(`${OutDir}${ProjName}.map`, `${ProjDir}emul/rom/${ProjName}.map`);
			if (fs.existsSync(`${OutDir}${ProjName}.noi`)) util.copyFile(`${OutDir}${ProjName}.noi`, `${ProjDir}emul/rom/${ProjName}.noi`);
			if (Debug)
				if (fs.existsSync(`${OutDir}${ProjName}.cdb`)) util.copyFile(`${OutDir}${ProjName}.cdb`, `${ProjDir}emul/rom/${ProjName}.cdb`);
		}

		//---- Copy data files ----
		if (ROMDelayBoot)
		{
			if (DiskFiles.length)
			{
				util.print(`-- Copy data files to disk (${DiskFiles})`);
				for (let i = 0; i < DiskFiles.length; i++)
					util.copyFile(`${DiskFiles[i]}`, `${ProjDir}emul/dsk/tmp/` + path.parse(DiskFiles[i]).base);
			}
		}
	}

	//-------------------------------------------------------------------------
	// BASIC BINARY TARGET
	//-------------------------------------------------------------------------
	else if (Ext === "bin")
	{
		//---- Copy program file ----
		util.print(`Copy BASIC binary to emul/bin/`);
		util.copyFile(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/bin/${ProjName}.${Ext}`);

		//---- Copy data files ----
		if (DiskFiles.length)
		{
			util.print(`-- Copy data files to disk (${DiskFiles})`);
			for (let i = 0; i < DiskFiles.length; i++)
				util.copyFile(`${DiskFiles[i]}`, `${ProjDir}emul/bin/` + path.parse(DiskFiles[i]).base);
		}

		//---- Generate autoexec ----
		util.print("-- Create emul/bin/autoexec.bas");
		let basTxt = "10 CLS : KEY OFF\r\n";
		if (Target === "BIN")
		{
			basTxt += '20 PRINT"Loading BIN..."\r\n';
			basTxt += `30 BLOAD"${projNameShort}.${Ext}",r\r\n`;
		}
		else
		{
			basTxt += '20 PRINT"Loading USR..."\r\n';
			basTxt += '30 DEF USR=&HC007\r\n';
			basTxt += `40 BLOAD"${projNameShort}.${Ext}"\r\n`;
			basTxt += '50 PRINT"USR(0) routine insalled"\r\n';
		}
		util.print("----------------------------------------", PrintDetail);
		util.print(basTxt, PrintDetail);
		util.print("----------------------------------------", PrintDetail);
		fs.writeFileSync(`${ProjDir}emul/bin/autoexec.bas`, basTxt);
		util.print("Success", PrintSuccess);

		//---- Generate DSK file ----
		if (fs.existsSync(DskTool))
		{
			util.print("Generating DSK file...", PrintHighlight);

			let filesList = [ "autoexec.bas", `${ProjName}.${Ext}` ];
			for (let i = 0; i < DiskFiles.length; i++)
				filesList.push(path.parse(DiskFiles[i]).base);
			
			util.print("-- Temporary copy files to DskTool directory");
			for (let i = 0; i < filesList.length; i++)
				util.copyFile(`${ProjDir}emul/bin/${filesList[i]}`, `${DskToolPath}${filesList[i]}`);

			util.print("-- Generate .DSK file");
			let curDir = process.cwd();
			process.chdir(DskToolPath);
			let err = util.execSync(`${DskToolName} -cf temp.dsk --dos1 --verbose ` + filesList.join(" "));
			if(err)
			{
				util.print(`DSK generation error! Code: ${err}`, PrintError);
				process.exit(1);
			}
			process.chdir(curDir);

			util.print(`-- Copy DSK file to ${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);
			util.copyFile(`${DskToolPath}temp.dsk`, `${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);

			util.print("-- Clean temporary files");
			util.delFile(`${DskToolPath}temp.dsk`);
			for (let i = 0; i < filesList.length; i++)
				util.delFile(`${DskToolPath}${filesList[i]}`);
		}
	}

	//-------------------------------------------------------------------------
	// MSX-DOS TARGET
	//-------------------------------------------------------------------------
	else if (Ext === "com")
	{
		if (DOS !== 0)
		{
			//---- Copy program file ----
			util.print(`Copy DOS binary to emul/dos${DOS}/`);
			util.copyFile(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/dos${DOS}/${ProjName}.${Ext}`);

			//---- Copy data files ----
			if (DiskFiles.length)
			{
				util.print(`-- Copy data files to disk (${DiskFiles})`);
				for (let i = 0; i < DiskFiles.length; i++)
					util.copyFile(`${DiskFiles[i]}`, `${ProjDir}emul/dos${DOS}/` + path.parse(DiskFiles[i]).base);
			}

			//---- Generate autoexec ----
			util.print(`Create emul/dos${DOS}/autoexec.bat`);
			
			let dosTxt = "";
			if (Machine !== "1") dosTxt += "MODE 80\r\n";
			if (DOS === 1)
			{
				dosTxt += "REM Generated by MSXgl on " + util.getDateString() + "\r\n";
				dosTxt += "REM Loading...\r\n";
			}
			else if (DOS === 2)
			{
				dosTxt += "COLOR 15,0,0\r\n";
				dosTxt += "ECHO Generated by MSXgl on " + util.getDateString() + "\r\n";
				dosTxt += "ECHO Loading...\r\n";
			}
			dosTxt += `${ProjName}\r\n`;

			util.print("----------------------------------------", PrintDetail);
			util.print(dosTxt, PrintDetail);
			util.print("----------------------------------------", PrintDetail);
			fs.writeFileSync(`${ProjDir}emul/dos${DOS}/autoexec.bat`, dosTxt);
			util.print("Success", PrintSuccess);

			//---- Generate DSK file ----
			if (fs.existsSync(DskTool))
			{
				util.print("Generating DSK file...", PrintHighlight);

				let filesList = [ "autoexec.bat", `${ProjName}.${Ext}` ];
				if (DOS === 1)
				{
					filesList.push("COMMAND.COM");
					filesList.push("MSXDOS.SYS");
				}
				else if (DOS === 2)
				{
					filesList.push("COMMAND2.COM");
					filesList.push("MSXDOS2.SYS");
				}
				for (let i = 0; i < DiskFiles.length; i++)
					filesList.push(path.parse(DiskFiles[i]).base);

				util.print("-- Temporary copy files to DskTool directory");
				for (let i = 0; i < filesList.length; i++)
					util.copyFile(`${ProjDir}emul/dos${DOS}/${filesList[i]}`, `${DskToolPath}${filesList[i]}`);

				util.print("-- Generate .DSK file");
				let curDir = process.cwd();
				process.chdir(DskToolPath);
				let err = util.execSync(`${DskToolName} -cf temp.dsk --dos${DOS} --verbose ` + filesList.join(" "));
				if(err)
				{
					util.print(`DSK generation error! Code: ${err}`, PrintError);
					process.exit(1);
				}
				process.chdir(curDir);

				util.print(`-- Copy DSK file to ${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);
				util.copyFile(`${DskToolPath}temp.dsk`, `${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);

				util.print("-- Clean temporary files");
				util.delFile(`${DskToolPath}temp.dsk`);
				for (let i = 0; i < filesList.length; i++)
					util.delFile(`${DskToolPath}${filesList[i]}`);
			}
		}
		else // if (DOS === 0)
		{
			//---- Copy program file ----
			util.print(`Copy DOS binary to emul/dos${DOS}/`);
			util.copyFile(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/dos${DOS}/BOOTDISK.COM`);

			//---- Copy data files ----
			if (DiskFiles.length)
			{
				util.print(`-- Copy data files to disk (${DiskFiles})`);
				for (let i = 0; i < DiskFiles.length; i++)
					util.copyFile(`${DiskFiles[i]}`, `${ProjDir}emul/dos${DOS}/` + path.parse(DiskFiles[i]).base);
			}

			//---- Generate DSK file ----
			if (fs.existsSync(DskTool))
			{
				util.print("Generating DSK file...", PrintHighlight);

				let filesList = [ "BOOTDISK.COM" ];
				for (let i = 0; i < DiskFiles.length; i++)
					filesList.push(path.parse(DiskFiles[i]).base);

				util.print("-- Temporary copy files to DskTool directory");
				for (let i = 0; i < filesList.length; i++)
					util.copyFile(`${ProjDir}emul/dos${DOS}/${filesList[i]}`, `${DskToolPath}${filesList[i]}`);

				util.print("-- Generate .DSK file");
				let curDir = process.cwd();
				process.chdir(DskToolPath);
				let err = util.execSync(`${DskToolName} -cf temp.dsk --dos0 --verbose ` + filesList.join(" "));
				if(err)
				{
					util.print(`DSK generation error! Code: ${err}`, PrintError);
					process.exit(1);
				}
				process.chdir(curDir);

				util.print(`-- Copy DSK file to ${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);
				util.copyFile(`${DskToolPath}temp.dsk`, `${ProjDir}emul/dsk/${Target}_${ProjName}.dsk`);

				util.print("-- Clean temporary files");
				util.delFile(`${DskToolPath}temp.dsk`);
				for (let i = 0; i < filesList.length; i++)
					util.delFile(`${DskToolPath}${filesList[i]}`);
			}
		}
	}

	util.print("Success", PrintSuccess);
}

//_____________________________________________________________________________
//  ▄▄▄
//  ██▄▀ ██ █ ██▀▄
//  ██ █ ▀█▄█ ██ █
//_____________________________________________________________________________
if (DoRun)
{
	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ RUN                                                                       │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print("Launching program...", PrintHighlight);

	//=============================================================================
	// EMULATOR
	//=============================================================================
	require("./setup_emulator.js");

	util.exec(`${Emulator} ${EmulatorArgs}`);
}