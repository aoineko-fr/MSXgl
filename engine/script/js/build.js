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
const analyzer = require("./analyzer.js");

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

//-- Display title
console.log(ColorBG +
	        "╔═══════════════════════════════════════════════════════════════════════════╗");
console.log("║                                                                           ║");
console.log("║  ██▀▀█▀▀███▀▀▀▀▀▀▀███▀▀█▀▀▀▀▀▀▀▀█                                         ║");
console.log("║  ██  ▀  ██   ▄▄▄▄  ▀  ▄█ ▄▀▀ █  █                                         ║");
console.log("║  █  ▄ ▄  ▀▀▀   █▀  ▄  ▀█ ▀▄█ █▄ █                                         ║");
console.log("║  █▄▄█▄█▄▄▄▄▄▄▄██▄▄███▄▄█▄▄▄▄▄▄▄▄█                                         ║");
console.log("║                                                                           ║");
console.log("║   ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄                               ║");
console.log("║   ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██                               ║");
console.log("║   ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄                              ║");
console.log("║                                                                           ║");
console.log("╚═══════════════════════════════════════════════════════════════════════════╝");
console.log(ColorReset+`MSXgl Build Tool using Node.js ${process.version} (${process.platform})\n`);

//-- Default configuration
if (!fs.existsSync(`${RootDir}projects/default_config.js`))
	util.copyFile(`${RootDir}engine/script/js/default_config.js`, `${RootDir}projects/default_config.js`);
require(`${RootDir}projects/default_config.js`);

//-- Project configuration overwrite
require(`${ProjDir}project_config.js`);

//... Log option and directory paths are now defined

//-- Create out directory
if (!fs.existsSync(OutDir))
	fs.mkdirSync(OutDir);	

//-- Setup log file
if (LogFile)
{
	if (!LogFileName)
		LogFileName = `log_${util.getDateTag()}.txt`;

	if (fs.existsSync(`${OutDir}${LogFileName}`))
		util.delFile(`${OutDir}${LogFileName}`);	

	util.print(`Log to file: ${LogFileName}`, PrintDetail);
}

//-- Parse command line for project name overwrite
let CommandArgs = process.argv.slice(2);
for (let i = 0; i < CommandArgs.length; i++)
{
	const arg = CommandArgs[i].toLowerCase();
	if (arg.startsWith("projname="))
	{
		let val = CommandArgs[i].substring(9);
		if (val)
		{
			ProjName = val;
			util.print(`Command line overwrite => ProjName=${ProjName}`, PrintDetail);
		}
	}
}

//-- Sub-project configuration overwrite
if (fs.existsSync(`${ProjDir}${ProjName}.js`))
{
	util.print(`Sub-project configuration found '${ProjDir}${ProjName}.js'`, PrintDetail);
	require(`${ProjDir}${ProjName}.js`);
}

//-- Command line parameters overwrite
require("./setup_command.js");

//-- Target specific initialization
global.TargetType = Target;
require("./setup_target.js");

//-- Parmaters validation
require("./check_config.js");

//=============================================================================
// DISPLAY INFO
//=============================================================================

//-- Update console title
process.title = `MSXgl – ${ProjName} – ${Target} – ${util.getMachineName(Machine)}`;

// console.clear();

// Display configuration summary
util.print(`» Project: ${ProjName}`);
util.print(`» Machine: ${util.getMachineName(Machine)}`);
util.print(`» Target:  ${TargetDesc}`);

util.print("Project paths:", PrintDetail);
util.print(`- ProjDir:  ${ProjDir}`, PrintDetail);
util.print(`- OutDir:   ${OutDir}`, PrintDetail);
util.print(`- RootDir:  ${RootDir}`, PrintDetail);
util.print(`- LibDir:   ${LibDir}`, PrintDetail);
util.print(`- ToolsDir: ${ToolsDir}`, PrintDetail);

process.env.path += `;${SDCCPath}bin`; // Hotfix for SDCC 4.3.0 path error for CC1

//-- Command lines to be executed before the build process (array)
if (PreBuildScripts.length)
{
	util.print("Executing pre-build scripts...", PrintHighlight);
	for (let i = 0; i < PreBuildScripts.length; i++)
	{
		let err = util.execSync(PreBuildScripts[i]);
		if (err)
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

	fs.mkdirSync(OutDir);
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
	if (ROMSkipBoot)
	{
		conf += "ROM_SKIP=1\n";
		conf += `ROM_SKIP_KEY=KEY_${ROMSkipBootKey}\n`;
	}
	if (ROMDelayBoot)            conf += "ROM_DELAY=1\n";
	if (BankedCall)              conf += "ROM_BCALL=1\n";
	if (InstallRAMISR === true)  conf += `ROM_RAMISR=RAM0_ISR\n`;
	else if ((InstallRAMISR === "RAM0_ISR") || (InstallRAMISR === "RAM0_SEGMENT")) conf += `ROM_RAMISR=${InstallRAMISR}\n`;
	if (CustomISR === "VHBLANK") conf += "ROM_ISR=ISR_VHBLANK\n";
	if (CustomISR === "V9990")   conf += "ROM_ISR=ISR_V9990\n";
	if (CustomISR === "ALL")     conf += "ROM_ISR=ISR_ALL\n";
	if (DOSParseArg)             conf += "DOS_PARSEARG=1\n";
	if (Target === "DOS0")       conf += "DOS_ISR=1\n";
	// if (Machine != "1")          conf += "ISR_SET_S0=1\n";
	if (CheckVersion)            conf += `CHECK_MSX=${util.getMachineID(Machine)}\n`;
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
	// GENERATE RAW FILES DEFINITIONS
	//=========================================================================
	if (/*RawFilesDef &&*/ RawFiles.length)
	{
		util.print("Generate raw files definitions...", PrintHighlight);

		let rawPages = [];
		let rawSegments = [];
		let rawDefTxt = "// MSXgl | Raw files definitions generated by the Build tool\n";

		for (let i = 0; i < RawFiles.length; i++)
		{
			let raw = RawFiles[i];
			let stats = fs.statSync(raw.file);
			let name = path.parse(raw.file).base.replace(".", "_").toUpperCase();
			
			rawDefTxt += "\n";
			if (raw.offset !== undefined)
			{
			}
			else if (raw.page !== undefined)
			{
				let baseOffset = raw.page * 16 * 1024;

				if (rawPages[raw.page] === undefined)
					rawPages[raw.page] = ((raw.page == 0) && ROMWithISR) ? 0x0100 : 0x0000;			
				let relOffset = rawPages[raw.page];
				rawPages[raw.page] += stats.size;

				raw.offset = baseOffset + relOffset; // Compute absolute offset

				rawDefTxt += `#define ${name}_REL \t0x${util.getHex(relOffset)}\n`; 
				rawDefTxt += `#define ${name}_SIZE\t0x${util.getHex(stats.size)}\n`;
			}
			else if (raw.segment !== undefined)
			{
				let baseOffset = raw.segment * SegSize;

				if (rawSegments[raw.segment] === undefined)
					rawSegments[raw.segment] = 0;
				let relOffset = rawSegments[raw.segment];
				rawSegments[raw.segment] += stats.size;

				raw.offset = baseOffset + relOffset; // Compute absolute offset

				rawDefTxt += `#define ${name}_SEG \t${raw.segment}\n`; 
				rawDefTxt += `#define ${name}_REL \t0x${util.getHex(relOffset)}\n`; 
			}

			rawDefTxt += `#define ${name}_ABS \t0x${util.getHex(raw.offset)}\n`; 
			rawDefTxt += `#define ${name}_SIZE\t0x${util.getHex(stats.size)}\n`;
		}
		fs.writeFileSync(`${ProjName}_rawdef.h`, rawDefTxt);
		util.print("Success", PrintSuccess);
	}
	
	//=========================================================================
	// GENERATE LOCALIZATION FILE
	//=========================================================================
	if (LocFiles.length)
	{
		util.print(`Generating '${LocOutput}' localization file...`, PrintHighlight);
		localizer.generate(LocFiles, LocOutput, LocStruct, LocSplitDef, Verbose);
		util.print("Success", PrintSuccess);
	}

	//=========================================================================
	// GENERATE MODULES LIST
	//=========================================================================

	// Add crt0 source to build list (it must be the first in the list)
	SrcList = [];
	RelList = [];
	LibList = [];
	MapList = [];
	const codeExtList = [ "c", "s", "asm" ];

	if (Target !== "LIB")
		SrcList.push(`${LibDir}src/crt0/${Crt0}.asm`);

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

		if (!bFound)
		{
			util.print(`Source file ${ProjModules[i]}.c not found!`, PrintError);
			process.exit(100);
		}
	}

	// Add modules sources to build list
	if (BuildLibrary && LibModules.length)
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

		util.print(`Searching for extra mapper segments to compile [${FirstSeg}-${LastSeg}] (${ProjSegments}_s?_b?)...`, PrintHighlight);
		for (let e = 0; e < segExtList.length; e++)
		{
			for (let s = FirstSeg; s <= LastSeg; s++)
			{
				let hex = util.getHex(s);
				for (let b = 0; b < bankAddrList.length; b++)
				{
					let bankAddr = bankAddrList[b];
					switch (Mapper)
					{
					case "ROM_NEO8":  if((b == 0) && (s == 4)) bankAddr = 0x0100; break;
					case "ROM_NEO16": if((b == 0) && (s == 2)) bankAddr = 0x0100; break;
					}
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

		if (InstallRAMISR === "RAM0_SEGMENT")
		{
			let pageName = `${ProjSegments}_p0`;
			util.print(`Searching for page 0 code to be copied to RAM (${pageName})...`, PrintHighlight);
			let pageFound = 0;
			let pageStartAddr;
			switch (Mapper)
			{
			case "ROM_ASCII8":		pageStartAddr = "0x40100"; break;
			case "ROM_ASCII16":		pageStartAddr = "0x20100"; break;
			case "ROM_KONAMI":		pageStartAddr = "0x40100"; break;
			case "ROM_KONAMI_SCC":	pageStartAddr = "0x40100"; break;
			case "ROM_NEO8":		pageStartAddr = "0x60100"; break;
			case "ROM_NEO16":		pageStartAddr = "0x30100"; break;
			case "ROM_YAMANOOTO":	pageStartAddr = "0x40100"; break;
			case "ROM_ASCII16X":	pageStartAddr = "0x20100"; break;
			default: util.print(`Unknow mapper: ${Mapper}`, PrintError);
			}

			for (let e = 0; e < segExtList.length; e++) // Parse all supported extension
			{
				if (fs.existsSync(`${pageName}.${segExtList[e]}`))
				{
					util.print(`Page 0 found: ${pageName}.${segExtList[e]} (address:${pageStartAddr})`);
					compiler.compile(`${ProjDir}${pageName}.${segExtList[e]}`, 16 * 1024, `PAGE0`);
					MapperBanks += `-Wl-b_PAGE0=${pageStartAddr} `;
					RelList.push(`${OutDir}${pageName}.rel`);
					HexBinOpt += ` --segaddr ${pageStartAddr >> 16} 0x0000`;
					pageFound++;
				}
			}
			if (!pageFound)
				util.print("No page 0 code found", PrintDetail);
		}
	}

	//=========================================================================
	// COMPILE PLAIN ROM PAGES CODE
	//=========================================================================
	else if (Ext === "rom")
	{
		const segExtList = [ "c", "s", "asm" ];
		let pageFound = 0;

		util.print(`Searching for ROM's pages specific code [${ROMFirstPage}-${ROMLastPage}] (${ProjSegments}_p?)...`, PrintHighlight);
		for (let p = ROMFirstPage; p <= ROMLastPage; p++) // Parse all ROM's pages
		{
			let pageName = `${ProjSegments}_p${p}`;
			let pageStartAddr;
			switch(p)
			{
			// case 0: pageStartAddr = (ROMWithISR) ? "0x0100" : "0x0000"; break;
			case 0: pageStartAddr = "0x0100"; break; // 0x0000 is not a valid address (it move the segment after the code)
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
		if (!pageFound)
			util.print("No pages code found", PrintDetail);
	}

	if (Target === "LIB")
	{
		util.print(`Generate ${ProjName}.lib...`, PrintHighlight);

		let libStr = RelList.join(" ");
		let SDARParam = `-rc ${OutDir}${ProjName}.lib ${libStr}`
		let err = util.execSync(`"${MakeLib}" ${SDARParam}`);
		if (err)
		{
			util.print(`Lib generation error! Code: ${err}`, PrintError);
			process.exit(1);
		}
		util.print("Success", PrintSuccess);
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
	if (BuildLibrary && LibModules.length)
	{
		util.print("Generate msxgl.lib...", PrintHighlight);

		// if (fs.existsSync(`${OutDir}msxgl.lib`))
			// util.delFile(`${OutDir}msxgl.lib`);

		let libStr = LibList.join(" ");
		let SDARParam = `-rc ${OutDir}msxgl.lib ${libStr}`
		let err = util.execSync(`"${MakeLib}" ${SDARParam}`);
		if (err)
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
		if (err)
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
	
	// Build library list
	AddLibs.push(`${OutDir}msxgl.lib`);
	if (PackSegments && MapList.length)
		AddLibs.push(`${OutDir}mapper.lib`);
	let libList = AddLibs.join(" ");

	// Build command line
	let SDCCParam = `-mz80 --vc --no-std-crt0 -L${SDCCPath}lib/z80 --code-loc 0x${util.getHex(CodeAddr)} --data-loc 0x${util.getHex(RamAddr)} ${LinkOpt} ${MapperBanks} ${OutDir}${Crt0}.rel ${libList} ${RelList.join(" ")} -o ${OutDir}${ProjName}.ihx`;
	let err = util.execSync(`"${Linker}" ${SDCCParam}`);
	if (err)
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
	H2BParam += ` ${HexBinOpt}`;

	// Add raw files definitions
	if (RawFiles.length)
	{
		let hexFile = "";
		for (let raw of RawFiles)
			hexFile += ` -r ${raw.offset} ${raw.file}`;

		fs.writeFileSync(`${OutDir}msxhex.txt`, hexFile);
		H2BParam += `-f ${OutDir}msxhex.txt`;
	}
	
	// Execute MSXhex
	let err = util.execSync(`"${Hex2Bin}" ${H2BParam}`);
	if (err)
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
	if (Ext === "rom")
	{
		if (!fs.existsSync(`${ProjDir}emul`)) fs.mkdirSync(`${ProjDir}emul`);
		if (!fs.existsSync(`${ProjDir}emul/rom`)) fs.mkdirSync(`${ProjDir}emul/rom`);
		if (ROMDelayBoot)
		{
			if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
			if (!fs.existsSync(`${ProjDir}emul/dsk/tmp`)) fs.mkdirSync(`${ProjDir}emul/dsk/tmp`);
		}
	}
	else if (Ext === "bin")
	{
		if (!fs.existsSync(`${ProjDir}emul`)) fs.mkdirSync(`${ProjDir}emul`);
		if (!fs.existsSync(`${ProjDir}emul/bin`)) fs.mkdirSync(`${ProjDir}emul/bin`);
		if (Target === "BIN_TAPE")
			if (!fs.existsSync(`${ProjDir}emul/cas`)) fs.mkdirSync(`${ProjDir}emul/cas`);
		else if (Target !== "RAW")
			if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
	}
	else if (Ext === "com")
	{
		if (!fs.existsSync(`${ProjDir}emul`)) fs.mkdirSync(`${ProjDir}emul`);
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
	else if (Ext === "lib")
	{
		if (!fs.existsSync(`${ProjDir}lib`)) fs.mkdirSync(`${ProjDir}lib`);
	}

	let DskToolPath = path.parse(DskTool).dir + '/';
	let DskToolName = path.parse(DskTool).base;
	if (process.platform !== "win32") // Linux & MacOS
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
				if (err)
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
				if (err)
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
				if (err)
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
	// C LIBRARY TARGET
	//-------------------------------------------------------------------------
	else if (Ext === "lib")
	{
		// Copy library file
		util.copyFile(`${OutDir}${ProjName}.lib`, `${ProjDir}lib/${ProjName}.lib`);
		// Copy all available header files
		for (let i = 0; i < ProjModules.length; i++)
		{
			if (fs.existsSync(`./${ProjModules[i]}.h`))
				util.copyFile(`./${ProjModules[i]}.h`, `${ProjDir}lib/${ProjModules[i]}.h`);
		}	
	}
	util.print("Success", PrintSuccess);

	//-------------------------------------------------------------------------
	// STANDALONE PACKAGE
	//-------------------------------------------------------------------------
	if (Standalone)
	{
		require("./standalone.js");
	}

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

//-- Execute MAP file analyzer
if (Analyzer)
{
	util.print(`Analyzing ${OutDir}${ProjName}.map...`, PrintHighlight);
	analyzer.exec(`${OutDir}${ProjName}.map`, false, AnalyzerOutput, AnalyzerSort, AnalyzerReport, AnalyzerCSV, AnalyzerSeparator);
	util.print("Success", PrintSuccess);
}

//-- Command lines to be executed after the build process
if (PostBuildScripts.length)
{
	util.print("Executing post-build scripts...", PrintHighlight);
	for (let i = 0; i < PostBuildScripts.length; i++)
	{
		let err = util.execSync(PostBuildScripts[i]);
		if (err)
		{
			util.print(`Post-build scripts error! Code: ${err}`, PrintError);
			process.exit(1);
		}
	}
	util.print("Success", PrintSuccess);
}

//-- Generate Clang compatible Compilation database
// See https://clang.llvm.org/docs/JSONCompilationDatabase.html
if (GenCompileDB)
{
	util.print("Generating Compilation database...", PrintHighlight);
	const compDB = JSON.stringify(CompileDB, null, "\t");
	fs.writeFileSync(`${OutDir}compile_commands.json`, compDB);
	util.print(`${OutDir}compile_commands.json generated`, PrintDetail);
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
	if (RunDevice)
	{
		require("./rundevice.js");
	}
	//=============================================================================
	// EMULATOR
	//=============================================================================
	else
	{
		require("./setup_emulator.js");

		util.execSync(`"${Emulator}" ${EmulatorArgs}`);
	}
}