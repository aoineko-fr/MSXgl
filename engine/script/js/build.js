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
const localizer = require("./localizer.js"); 

//_____________________________________________________________________________
//  ▄▄▄▄      ▄  ▄▄ 
//   ██  ██▀▄ ▄  ██▀
//  ▄██▄ ██ █ ██ ▀█▄
//_____________________________________________________________________________

//=============================================================================
// BUILD INITIALIZATION
//=============================================================================

//-- Start timer
const buildStartTime = Date.now();

//-- Setup global variables
require("./setup_global.js"); 

//-- Default configuration
if (!fs.existsSync(`${RootDir}projects/default_config.js`))
	util.copyFile(`${RootDir}engine/script/js/default_config.js`, `${RootDir}projects/default_config.js`);
require(`${RootDir}projects/default_config.js`);

//-- Project specific overwrite
require(`${ProjDir}project_config.js`);

//-- Setup command line overwrite parameters
let CommandArgs = process.argv.slice(2);
for (let i = 0; i < CommandArgs.length; i++)
{
	const arg = CommandArgs[i].toLowerCase();
	if(arg.startsWith("projname="))
	{
		let val = CommandArgs[i].substring(9);
		if (val)
		{
			ProjName = val;
			util.print(`Command line overwrite => ProjName=${ProjName}`, PrintDetail);
		}
	}
}

//-- Sub-project specific overwrite
if (fs.existsSync(`${ProjDir}${ProjName}.js`))
{
	util.print(`Sub-project configuration found '${ProjDir}${ProjName}.js'`, PrintDetail);
	require(`${ProjDir}${ProjName}.js`);
}

//-- Setup command line overwrite parameters
CommandArgs = process.argv.slice(2);
for (let i = 0; i < CommandArgs.length; i++)
{
	const arg = CommandArgs[i].toLowerCase();
	if(arg.startsWith("projname="))
	{
	}
	else if(arg.startsWith("target="))
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
		util.print(`Unknown command line overwrite '${arg}'`, PrintWarning);
	}
}

//-- Validate enum
Machine   = Machine.toUpperCase();
Target    = Target.toUpperCase();
CustomISR = CustomISR.toUpperCase();
AsmOptim  = AsmOptim.toUpperCase();
Optim     = Optim.toUpperCase();
EmulPortA = EmulPortA.toUpperCase();
EmulPortB = EmulPortB.toUpperCase();
RunDevice = RunDevice.toUpperCase();
if(util.isString(CompileComplexity))
	CompileComplexity = CompileComplexity.toUpperCase();

//-- Target specific initialization
global.TargetType = Target;
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
process.title = `MSXgl Build Tool – ${ProjName} – ${Target} – ${util.getMachineName(Machine)}`;

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

process.env.path += `;${SDCCPath}`; // Hotfix for SDCC 4.3.0 path error for CC1

//-- Command lines to be executed before the build process (array)
if(PreBuildScripts.length)
{
	util.print("Executing pre-build scripts...", PrintHighlight);
	for (let i = 0; i < PreBuildScripts.length; i++)
	{
		let err = util.execSync(PreBuildScripts[i]);
		if(err)
		{
			util.print(`Pre-build scripts error! Code: ${err}`, PrintError);
			process.exit(1);
		}
	}
	util.print("Success", PrintSuccess);
}

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
	//-- Start timer
	const compileStartTime = Date.now();

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
	// Defines
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
	// Macros
	conf += ".macro ROM_SIGN_MACRO\n";
	if (AddROMSignature) conf += `\t.ascii "${ROMSignature}"\n`;
	conf += ".endm\n";		

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
	// GENERATE LOCALIZATION FILE
	//=========================================================================
	if (LocFiles.length)
	{
		util.print(`Generating '${LocOutput}' localization file...`, PrintHighlight);
		localizer.generate(LocFiles, LocOutput, LocStruct, Verbose);
		util.print("Success", PrintSuccess);
	}

	//=========================================================================
	// GENERATE MODULES LIST
	//=========================================================================

	// Add crt0 source to build list (it must be the first in the list)
	SrcList = [ `${LibDir}src/crt0/${Crt0}.asm` ];
	RelList = [];
	LibList = [];
	MapList = [];
	const codeExtList = [ "c", "s", "asm" ];

	// Add project sources to build list
	for (let i = 0; i < ProjModules.length; i++)
	{
		let bFound = false;

		for (let e = 0; e < codeExtList.length; e++)
		{
			if (fs.existsSync(`./${ProjModules[i]}.${codeExtList[e]}`))
			{
				SrcList.push(`./${ProjModules[i]}.${codeExtList[e]}`);
				let fileName = path.parse(ProjModules[i]).name;
				RelList.push(`${OutDir}${fileName}.rel`);
				bFound = true;
				break;
			}
		}

		if(!bFound)
		{
			util.print(`Source file ${ProjModules[i]}.c not found!`, PrintError);
			process.exit(100);
		}
	}

	// Add modules sources to build list
	if (BuildLibrary)
	{
		util.print(`» MSXgl Modules: ${LibModules}`);
		for (let i = 0; i < LibModules.length; i++)
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
	for (let i = 0; i < AddSources.length; i++)
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
	if ((Ext === "rom") && (MapperSize))
	{
		let FirstSeg = 1;
		let LastSeg = (MapperSize / SegSize) - 1;
		const segExtList = [ "c", "s", "asm" ];
		const bankAddrList = [ Bank0Addr, Bank1Addr, Bank2Addr, Bank3Addr, Bank4Addr, Bank5Addr ];

		util.print(`Search for extra mapper segments to compile [${FirstSeg}-${LastSeg}]...`, PrintHighlight);
		for (let e = 0; e < segExtList.length; e++)
		{
			for (let s = FirstSeg; s <= LastSeg; s++)
			{
				let hex = util.getHex(s);
				for (let b = 0; b < bankAddrList.length; b++)
				{
					let bankAddr = bankAddrList[b];
					if (bankAddr != 0xFFFF)
					{
						let segPath = `${ProjSegments}_s${s}_b${b}`;
						let segName = path.parse(segPath).name;
						if (fs.existsSync(`${segPath}.${segExtList[e]}`))
						{
							let bankHex = util.getHex(bankAddr);
							util.print(`Segment found: ${segPath}.${segExtList[e]} (addr: ${hex}${bankHex}h)`);
							compiler.compile(`${ProjDir}${segPath}.${segExtList[e]}`, SegSize, `SEG${s}`);
							MapperBanks += `-Wl-b_SEG${s}=0x${hex}${bankHex} `;
							if (PackSegments)
								MapList.push(`${OutDir}${segName}.rel`);
							else
								RelList.push(`${OutDir}${segName}.rel`);
						}
					}
				}
			}
		}
	}

	//=========================================================================
	// COMPILE PLAIN ROM PAGES CODE
	//=========================================================================
	else if (Ext === "rom")
	{
		const segExtList = [ "c", "s", "asm" ];
		let pageFound = 0;

		util.print(`Search for ROM's pages specific code (from ${ROMFirstPage} to ${ROMLastPage})`, PrintHighlight);
		for (let p = ROMFirstPage; p < ROMLastPage; p++) // Parse all ROM's pages
		{
			let pageName = `${ProjName}_p${p}`;
			let pageStartAddr;
			switch(p)
			{
			case 0: pageStartAddr = (ROMWithISR) ? "0x0100" : "0x0000"; break;
			case 1: pageStartAddr = "0x4000"; break;
			case 2: pageStartAddr = "0x8000"; break;
			case 3: pageStartAddr = "0xC000"; break;
			}

			for (let e = 0; e < segExtList.length; e++) // Parse all supported extension
			{
				if (fs.existsSync(`${pageName}.${segExtList[e]}`))
				{
					util.print(`Page ${p} found: ${pageName}.${segExtList[e]} (address:${pageStartAddr})`);
					compiler.compile(`${ProjDir}${pageName}.${segExtList[e]}`, 16 * 1024, `PAGE${p}`);
					MapperBanks += `-Wl-b_PAGE${p}=${pageStartAddr} `;
					RelList.push(`${OutDir}${pageName}.rel`);
					pageFound++;
				}
			}
		}
		if(!pageFound)
			util.print("No pages code found", PrintDetail);
	}

	//-- Display step duration
	const compileElapsTime = Date.now() - compileStartTime;
	util.print('Compile duration: ' + util.getTimeString(compileElapsTime), PrintDetail);
}

//_____________________________________________________________________________
//  ▄▄   ▄       ▄▄
//  ██   ▄  ██▀▄ ██▄▀
//  ██▄▄ ██ ██ █ ██ █
//_____________________________________________________________________________
if (DoMake)
{
	//-- Start timer
	const linkStartTime = Date.now();

	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ LINK                                                                      │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");

	//-- Overwrite code start address
	if (ForceCodeAddr)
	{
		util.print(`» Force code address to ${util.getHex(ForceCodeAddr)}h`);
		CodeAddr = ForceCodeAddr;
	}

	//-- Overwrite RAM start address
	if (ForceRamAddr)
	{
		util.print(`» Force RAM address to ${util.getHex(ForceRamAddr)}h`);
		RamAddr = ForceRamAddr;
	}

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
		let err = util.execSync(`"${MakeLib}" ${SDARParam}`);
		if(err)
		{
			util.print(`Lib generation error! Code: ${err}`, PrintError);
			process.exit(1);
		}
		util.print("Success", PrintSuccess);
	}

	//=========================================================================
	// Generate Mapper Segment Package
	//=========================================================================
	if (PackSegments && MapList.length)
	{
		util.print("Generate mapper.lib...", PrintHighlight);

		// if (fs.existsSync(`${OutDir}mapper.lib`))
			// util.delFile(`${OutDir}mapper.lib`);

		let mapStr = MapList.join(" ");
		let SDARParam = `-rc ${OutDir}mapper.lib ${mapStr}`
		let err = util.execSync(`"${MakeLib}" ${SDARParam}`);
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
	util.execSync(`"${Linker}" --version`); // display SDCC version

	if (Optim === "SPEED") LinkOpt += " --opt-code-speed";
	if (Optim === "SIZE")  LinkOpt += " --opt-code-size";
	if (Debug)             LinkOpt += " --debug";
	let mapLibStr = "";
	if (PackSegments && MapList.length)
		mapLibStr = `${OutDir}mapper.lib`;

	let SDCCParam = `-mz80 --vc --no-std-crt0 -L${ToolsDir}sdcc/lib/z80 --code-loc 0x${util.getHex(CodeAddr)} --data-loc 0x${util.getHex(RamAddr)} ${LinkOpt} ${MapperBanks} ${OutDir}${Crt0}.rel ${OutDir}msxgl.lib ${mapLibStr} ${RelList.join(" ")} -o ${OutDir}${ProjName}.ihx`;
	let err = util.execSync(`"${Linker}" ${SDCCParam}`);
	if(err)
	{
		util.print(`Link error! Code: ${err}`, PrintError);
		process.exit(1);
	}
	util.print("Success", PrintSuccess);

	//-- Display step duration
	const linkElapsTime = Date.now() - linkStartTime;
	util.print('Link duration: ' + util.getTimeString(linkElapsTime), PrintDetail);
}

//_____________________________________________________________________________
//  ▄▄▄            ▄▄
//  ██▄▀ ▄▀██ ▄█▀▀ ██▄▀ ▄▀██ ▄▀██ ▄███
//  ██   ▀▄██ ▀█▄▄ ██ █ ▀▄██  ▀██ ▀█▄▄
//____________________________▀▀_______________________________________________
if (DoPackage)
{
	//-- Start timer
	const packStartTime = Date.now();

	util.print("");
	util.print("┌───────────────────────────────────────────────────────────────────────────┐");
	util.print("│ PACKAGE                                                                   │");
	util.print("└───────────────────────────────────────────────────────────────────────────┘");
	util.print("Packaging binary...", PrintHighlight);

	//=========================================================================
	// MSXhex
	//=========================================================================
	H2BParam = `${OutDir}${ProjName}.ihx -e ${Ext} -s 0x${util.getHex(StartAddr)}`;
	if ((Ext === "rom") && (MapperSize))
		H2BParam += ` -l ${MapperSize} -b ${SegSize}`;
	else
		H2BParam += ` -l ${FillSize}`;
	// if (Target === "DOS2_MAPPER")
	// 	H2BParam += " -split";
	for (let i = 0; i < RawFiles.length; i++)
	{
		let raw = RawFiles[i];
		if(raw.offset !== undefined)
			H2BParam += ` -r ${raw.offset} ${raw.file}`;
		else if(raw.page !== undefined)
			H2BParam += ` -r ${raw.page * 16 * 1024} ${raw.file}`;
		else if(raw.segment !== undefined)
			H2BParam += ` -r ${raw.segment * SegSize} ${raw.file}`;
	}
	H2BParam += ` ${HexBinOpt}`;

	let err = util.execSync(`"${Hex2Bin}" ${H2BParam}`);
	if(err)
	{
		util.print(`Package error! Code: ${err}`, PrintError);
		process.exit(1);
	}
	
	util.print("Success", PrintSuccess);

	//-- Display step duration
	const packElapsTime = Date.now() - packStartTime;
	util.print('\nPackage duration: ' + util.getTimeString(packElapsTime), PrintDetail);
}

//_____________________________________________________________________________
//  ▄▄▄            ▄▄
//  ██ █ ▄███ ██▀▄ ██  ▄█▀▄ █ ██
//  ██▄▀ ▀█▄▄ ██▀  ▀█▄ ▀█▄▀  ▀██
//____________▀▀_____________▀▀________________________________________________
if (DoDeploy)
{
	//-- Start timer
	const deployStartTime = Date.now();

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
		if (Target === "BIN_TAPE")
			if (!fs.existsSync(`${ProjDir}emul/cas`)) fs.mkdirSync(`${ProjDir}emul/cas`);
		else if (Target !== "RAW")
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
		if (Target === "RAW")
		{
			//---- Copy program file ----
			util.print(`Copy driver binary to emul/bin/`);
			util.copyFile(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/bin/${ProjName}.${Ext}`);
		}
		else
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
			if (Target === "BIN_USR")
			{
				basTxt += '20 PRINT"Loading USR..."\r\n';
				basTxt += '30 DEF USR=&HC007\r\n';
				basTxt += `40 BLOAD"${projNameShort}.${Ext}"\r\n`;
				basTxt += '50 PRINT"USR(0) routine insalled"\r\n';
			}
			else
			{
				basTxt += '20 PRINT"Loading BIN..."\r\n';
				basTxt += `30 BLOAD"${projNameShort}.${Ext}",r\r\n`;
			}
			util.print("----------------------------------------", PrintDetail);
			util.print(basTxt, PrintDetail);
			util.print("----------------------------------------", PrintDetail);
			fs.writeFileSync(`${ProjDir}emul/bin/autoexec.bas`, basTxt);
			util.print("Success", PrintSuccess);

			//---- Generate CAS file ----
			if (Target === "BIN_TAPE")
			{
				util.print("Generating CAS file...", PrintHighlight);
				util.createCAS(`${ProjDir}emul/bin/${ProjName}.${Ext}`, `${ProjDir}emul/cas/${ProjName}.cas`);
			}
			//---- Generate DSK file ----
			else if (fs.existsSync(DskTool))
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
				let err = util.execSync(`"${DskToolName}" -cf temp.dsk --dos1 --verbose --size=${DiskSize} ` + filesList.join(" "));
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
				let err = util.execSync(`"${DskToolName}" -cf temp.dsk --dos${DOS} --verbose --size=${DiskSize} ` + filesList.join(" "));
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
				let err = util.execSync(`"${DskToolName}" -cf temp.dsk --dos0 --verbose --size=${DiskSize} ` + filesList.join(" "));
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

	//-- Display step duration
	const deployElapsTime = Date.now() - deployStartTime;
	util.print('Deploy duration: ' + util.getTimeString(deployElapsTime), PrintDetail);
}

//_____________________________________________________________________________
//
//
//
//_____________________________________________________________________________

util.print("");
util.print("┌───────────────────────────────────────────────────────────────────────────┐");
util.print("│ POST BUILD                                                                │");
util.print("└───────────────────────────────────────────────────────────────────────────┘");

//-- Build duration
const buildElapsTime = Date.now() - buildStartTime;
util.print('\nTotal build time: ' + util.getTimeString(buildElapsTime), PrintDetail);

//-- Command lines to be executed after the build process (array)
if(PostBuildScripts.length)
{
	util.print("Executing post-build scripts...", PrintHighlight);
	for (let i = 0; i < PostBuildScripts.length; i++)
	{
		let err = util.execSync(PostBuildScripts[i]);
		if(err)
		{
			util.print(`Post-build scripts error! Code: ${err}`, PrintError);
			process.exit(1);
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
	// RUN DEVICE
	//=============================================================================
	if(RunDevice)
	{
		if(RunDevice === "EASY-USB")
		{
			if (Ext === "rom")
			{
				// Delete all .ROM files
				fs.readdirSync(RunDeviceOpt).forEach(file => {
					if(path.parse(file).ext.toUpperCase() === ".ROM")
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
		else
			util.print(`Unknown run device '${RunDevice}'`, PrintWarning);
	}
	//=============================================================================
	// EMULATOR
	//=============================================================================
	else
	{
		require("./setup_emulator.js");

		util.exec(`"${Emulator}" ${EmulatorArgs}`);
	}
}