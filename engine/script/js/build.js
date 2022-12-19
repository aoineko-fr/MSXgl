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
require(`${RootDir}projects/default_config.js`);

//-- Project specific overwrite
require(`${ProjDir}project_config.js`);

//-- Setup command line parameters
const CommandArgs = process.argv.slice(2);
if (CommandArgs[0])
	Target = CommandArgs[0]; // Target overwrite

//-- Target specific initialization
require("./setup_target.js");

//=============================================================================
// DISPLAY INFO
//=============================================================================

console.clear();

//-- Create out directory
if (!fs.existsSync(OutDir))
	fs.mkdirSync(OutDir);

//-- Setup log file
if (!LogFileName)
	LogFileName = `log_${util.getDateTag()}.txt`;
if (fs.existsSync(`${OutDir}${LogFileName}`))
	fs.unlinkSync(`${OutDir}${LogFileName}`);

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

util.print("MSXgl Build Tool using Node.js " + process.version + " (" + process.platform + ")\n");

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
	conf += "ADDR_BOOT=0x" + util.getHex(StartAddr) + "\n";
	if (Mapper)                 conf += `ROM_MAPPER=${Mapper}\n`;
	if (ROMDelayBoot)           conf += "ROM_DELAY=1\n";
	if (BankedCall)             conf += "ROM_BCALL=1\n";
	if (InstallRAMISR)          conf += "ROM_RAMISR=1\n";
	if (CustomISR == "VHBLANK") conf += "ROM_ISR=ISR_VHBLANK\n";
	if (CustomISR == "V9990")   conf += "ROM_ISR=ISR_V9990\n";
	// if (Machine != "1")         conf += "ISR_SET_S0=1\n";
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
		if (AppExtra) conf += `APP_SIGN_EXTRA=${AppExtra}\n`;
	}

	util.print(conf, PrintDetail);
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
		fs.writeFileSync("./version.h", `#define BUILD_VERSION ${Version}`);

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
	if (AddSources)
	{
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
	MapperBanks = [];
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
							util.print(`Segment found: ${segName}.${segExtList[e]} (addr: ${hex}${bankAddr})`);
							compiler.compile(`${segName}.${segExtList[e]}`, SegSize, s);
							MapperBanks.push(`-Wl-b_SEG${s}=0x${hex}${bankAddr}`);
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

		let libStr = LibList.join(" ");
		let SDARParam = `-rc ${OutDir}msxgl.lib ${libStr}`
		util.execSync(`${MakeLib} ${SDARParam}`);
		util.print(`Succeed`, PrintSucced);
	}

	//=========================================================================
	// Link Program
	//=========================================================================
	util.print(`Making ${ProjName} using SDCC...`, PrintHighlight);
	util.execSync(`${Linker} --version`); // display SDCC version

	if (Optim == "Speed") LinkOpt += ` --opt-code-speed`;
	if (Optim == "Size")  LinkOpt += ` --opt-code-size`;
	if (Debug)            LinkOpt += ` --debug`;

	let SDCCParam = `-mz80 --vc --no-std-crt0 --code-loc 0x${util.getHex(CodeAddr)} --data-loc 0x${util.getHex(RamAddr)} ${LinkOpt} ${MapperBanks} ${OutDir}${Crt0}.rel ${OutDir}msxgl.lib ${RelList} -o ${OutDir}${ProjName}.ihx`;
	util.execSync(`${Linker} ${SDCCParam}`);
	util.print(`Succeed`, PrintSucced);
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
	if (MapperSize)
		H2BParam = `${OutDir}${ProjName}.ihx -e ${Ext} -s 0x${util.getHex(StartAddr)} -l ${FillSize}`;
	else
		H2BParam = `${OutDir}${ProjName}.ihx -e ${Ext} -s 0x${util.getHex(StartAddr)} -l ${MapperSize} -b ${SegSize}`;
	util.execSync(`${Hex2Bin} ${H2BParam}`);
	util.print(`Succeed`, PrintSucced);
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
	util.print("Deploying ${Target}...", PrintHighlight);

	//=========================================================================
	// CREATE OUTPUT DIRECTORY
	//=========================================================================
	if (!fs.existsSync(`${ProjDir}emul`)) fs.mkdirSync(`${ProjDir}emul`);
	if (Ext == "rom")
	{
		if (!fs.existsSync(`${ProjDir}emul/rom`)) fs.mkdirSync(`${ProjDir}emul/rom`);
		if (ROMDelayBoot && !fs.existsSync(`${ProjDir}emul/dsk/tmp`)) fs.mkdirSync(`${ProjDir}emul/dsk/tmp`);
	}
	else if (Ext == "bin")
	{
		if (!fs.existsSync(`${ProjDir}emul/bin`)) fs.mkdirSync(`${ProjDir}emul/bin`);
		if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
	}
	else if (Ext == "com")
	{
		if (!fs.existsSync(`${ProjDir}emul/dos${DOS}`)) fs.mkdirSync(`${ProjDir}emul/dos${DOS}`);
		if (DOS == 1)
		{
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}COMMAND.COM`)) fs.copyFileSync(`${MSXDOS}COMMAND.COM`, `${ProjDir}emul/dos${DOS}`);
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}MSXDOS.SYS`))  fs.copyFileSync(`${MSXDOS}MSXDOS.SYS`, `${ProjDir}emul/dos${DOS}`);
		}
		else if (DOS == 2)
		{
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}COMMAND2.COM`)) fs.copyFileSync(`${MSXDOS}COMMAND2.COM`, `${ProjDir}emul/dos${DOS}`);
			if (!fs.existsSync(`${ProjDir}emul/dos${DOS}MSXDOS2.SYS`))  fs.copyFileSync(`${MSXDOS}MSXDOS2.SYS`, `${ProjDir}emul/dos${DOS}`);
		}
		if (!fs.existsSync(`${ProjDir}emul/dsk`)) fs.mkdirSync(`${ProjDir}emul/dsk`);
	}

	let DskToolPath = path.parse(DskTool).path;
	let DskToolName = path.parse(DskTool).base;

	//-----------------------------------------------------------------------------
	if (Ext == "rom")
	{
		//---- Copy program file ----
		util.print(`Copy ${OutDir}${ProjName}.${Ext} to emul/rom/${ProjName}.${Ext}`);
		fs.copyFileSync(`${OutDir}${ProjName}.${Ext}`, `${ProjDir}emul/rom/${ProjName}.${Ext}`);

		//---- Copy symbols files ----
		if (DebugSymbols)
		{
			util.print("Copy symbols files to destination directory");
			if (fs.existsSync(`${OutDir}${ProjName}.map`)) fs.copyFileSync(`${OutDir}${ProjName}.map`, `${ProjDir}emul/rom/${ProjName}.map`);
			if (fs.existsSync(`${OutDir}${ProjName}.noi`)) fs.copyFileSync(`${OutDir}${ProjName}.noi`, `${ProjDir}emul/rom/${ProjName}.noi`);
			if (Debug)
				if (fs.existsSync(`${OutDir}${ProjName}.cdb`)) fs.copyFileSync(`${OutDir}${ProjName}.cdb`, `${ProjDir}emul/rom/${ProjName}.cdb`);
		}

		//---- Copy data files ----
		if (ROMDelayBoot)
		{
			if (DiskFiles)
			{
				util.print(`-- Copy data files to disk (${DiskFiles})`);
				for (let i = 0; i < DiskFiles.length; i++)
					fs.copyFileSync(`${DiskFiles[i]}`, `${ProjDir}emul/dsk/tmp/` + path.parse(DiskFiles[i]).base);
			}
		}
	}
/*
	//-----------------------------------------------------------------------------
	if /I %Ext%==bin (
		//---- Copy program file ----
		util.print(" -- Copy ${OutDir}%ProjName%.%Ext% to emul/bin/%ProjName%.%Ext%
		copy /Y ${OutDir}%ProjName%.%Ext% %ProjDir%/emul/bin/%ProjName%.%Ext%
		if errorlevel 1 goto :Error

		//---- Copy data files ----
		if defined DiskFiles (
			util.print(" -- Copy data files to disk ^(%DiskFiles%^)
			for %%G in (%DiskFiles%) do (
				copy /Y %%G %ProjDir%/emul/bin/%%~nG%%~xG
			)
		)

		//---- Generate autoexec ----
		util.print(" -- Create emul/bin/autoexec.bas
		util.print(" 10 CLS : KEY OFF > %ProjDir%/emul/bin/autoexec.bas
		if /I %Target%==BIN (
			util.print(" 20 PRINT"Loading..." >> %ProjDir%/emul/bin/autoexec.bas
			util.print(" 30 BLOAD"%ProjName:~0,8%.%Ext%",r >> %ProjDir%/emul/bin/autoexec.bas
		) else (
			util.print(" 20 PRINT"Loading USR..." >> %ProjDir%/emul/bin/autoexec.bas
			util.print(" 30 DEF USR=^&HC007 >> %ProjDir%/emul/bin/autoexec.bas
			util.print(" 40 BLOAD"%ProjName:~0,8%.%Ext%" >> %ProjDir%/emul/bin/autoexec.bas
		)

		//---- Generate DSK file ----
		if (fs.existsSync(`%DskTool% (
		
			util.print(" ${ColorGreen}Succeed${ColorReset}
			util.print(" ${ColorBlue}Generating DSK file...${ColorReset}
		
			util.print(" -- Temporary copy files to DskTool directory
			copy /Y %ProjDir%/emul/bin/autoexec.bas %DskToolPath%
			copy /Y %ProjDir%/emul/bin/%ProjName%.%Ext% %DskToolPath%
			if defined DiskFiles (
				for %%G in (%DiskFiles%) do (
					copy /Y %ProjDir%/emul/bin/%%~nG%%~xG %DskToolPath%
				)
			)

			set FilesList=autoexec.bas %ProjName%.%Ext%
			if defined DiskFiles (
				for %%G in (%DiskFiles%) do (
					set FilesList=!FilesList! %%~nG%%~xG
				)
			)

			util.print(" -- Generate .DSK file
			util.print(" %DskToolName% a temp.dsk !FilesList!
			set PrevCD=%cd%
			cd %DskToolPath%
			%DskToolName% a temp.dsk !FilesList!
			cd !PrevCD!
			
			util.print(" -- Copy DSK file to %ProjDir%/emul/dsk/%Target%_%ProjName%.dsk
			copy /Y %DskToolPath%/temp.dsk %ProjDir%/emul/dsk/%Target%_%ProjName%.dsk

			util.print(" -- Clean temporary files
			del /Q %DskToolPath%/autoexec.bas %DskToolPath%/%ProjName%.%Ext% %DskToolPath%/temp.dsk
		)
	)

	//-----------------------------------------------------------------------------
	if /I %Ext%==com (
		//---- Copy program file ----
		util.print(" Copy ${OutDir}%ProjName%.%Ext% to emul/dos%DOS%/%ProjName%.%Ext%
		copy /Y ${OutDir}%ProjName%.%Ext% %ProjDir%/emul/dos%DOS%/%ProjName%.%Ext%
		if errorlevel 1 goto :Error

		//---- Copy data files ----
		if defined DiskFiles (
			util.print(" -- Copy data files to disk ^(%DiskFiles%^)
			for %%G in (%DiskFiles%) do (
				copy /Y %%G %ProjDir%/emul/dos%DOS%/%%~nG%%~xG
			)
		)

		//---- Generate autoexec ----
		util.print(" Create emul/dos%DOS%/autoexec.bat
		util.print(" REM > %ProjDir%/emul/dos%DOS%/autoexec.bat
		if /I not %Machine%==1 ( util.print(" mode 80 >> %ProjDir%/emul/dos%DOS%/autoexec.bat )
		if /I %DOS%==2 ( 
			util.print(" util.print(" Generated by MSXgl on %DATE% at %TIME% >> %ProjDir%/emul/dos%DOS%/autoexec.bat
			util.print(" util.print(" Loading... >> %ProjDir%/emul/dos%DOS%/autoexec.bat
		) else (
			util.print(" REM Generated by MSXgl on %DATE% at %TIME% >> %ProjDir%/emul/dos%DOS%/autoexec.bat
			util.print(" REM Loading... >> %ProjDir%/emul/dos%DOS%/autoexec.bat
		)
		util.print(" %ProjName% >> %ProjDir%/emul/dos%DOS%/autoexec.bat

		//---- Generate DSK file ----
		if (fs.existsSync(`%DskTool% (

			util.print(" ${ColorGreen}Succeed${ColorReset}
			util.print(" ${ColorBlue}Generating DSK file...${ColorReset}

			if /I %DOS%==1 (
				set FilesList=COMMAND.COM MSXDOS.SYS autoexec.bat %ProjName%.%Ext%
			)
			if /I %DOS%==2 (
				set FilesList=COMMAND2.COM MSXDOS2.SYS autoexec.bat %ProjName%.%Ext%
			)
			if defined DiskFiles (
				for %%G in (%DiskFiles%) do (
					set FilesList=!FilesList! %%~nG%%~xG
				)
			)
			
			util.print(" -- Temporary copy files to DskTool directory
			for %%K in (!FilesList!) do ( copy /Y %ProjDir%/emul/dos%DOS%/%%K %DskToolPath% )

			util.print(" -- Generate .DSK file
			util.print(" %DskToolName% a temp.dsk !FilesList!
			set PrevCD=%cd%
			cd %DskToolPath%
			%DskToolName% a temp.dsk !FilesList!
			cd !PrevCD!

			util.print(" -- Copy DSK file to %ProjDir%/emul/dsk/%Target%_%ProjName%.dsk
			copy /Y %DskToolPath%/temp.dsk %ProjDir%/emul/dsk/%Target%_%ProjName%.dsk

			util.print(" -- Clean temporary files
			del /Q %DskToolPath%/temp.dsk
			for %%K in (!FilesList!) do ( del /Q %DskToolPath%/%%K )
		)
	)
*/
	util.print(`Succeed`, PrintSucced);
}

//_____________________________________________________________________________
//  ▄▄▄
//  ██▄▀ ██ █ ██▀▄
//  ██ █ ▀█▄█ ██ █
//_____________________________________________________________________________
if (DoRun) {

util.print("");
util.print("┌───────────────────────────────────────────────────────────────────────────┐");
util.print("│ RUN                                                                       │");
util.print("└───────────────────────────────────────────────────────────────────────────┘");
util.print("Launching program...", PrintHighlight);

//=============================================================================
// EMULATOR
//=============================================================================
require("./setup_emulator.js");
util.execSync(`start /b ${Emulator} ${EmulatorArgs}`);


// // start /b %Emulator% %EmulatorArgs%
/*
call %LibDir%/script/setup_emulator.cmd

:NoRun

util.print(" ${ColorGreen}Build Succeed${ColorReset}
exit /B 0

:Error

util.print(" ${ColorRed}Error: Build Failed with error number %errorlevel%${ColorReset}
exit /B 666*/

}



/*
//_____________________________________________________________________________
//   ▄▄▄      ▄       ▄▄
//  ▀█▄  ██▄▀ ▄  ██▀▄ ██▀
//  ▄▄█▀ ██   ██ ██▀  ▀█▄
//_______________▀▀____________________________________________________________

:toHex dec hex -- convert a decimal number to hexadecimal, i.e. -20 to FFFFFFEC or 26 to 0000001A
//             -- dec [in]      - decimal number to convert
//             -- hex [out,opt] - variable to store the converted hexadecimal number in
//Thanks to 'dbenham' dostips forum users who inspired to improve this function
:$created 20091203 :$changed 20110330 :$categories Arithmetic,Encoding
:$source https://www.dostips.com
setlocal ENABLEDELAYEDEXPANSION
set /a dec=%~1
set "hex="
set "map=0123456789ABCDEF"
for /L %%N in (1,1,8) do (
    set /a "d=dec&15,dec>>=4"
    for %%D in (!d!) do set "hex=!map:~%%D,1!!hex!"
)
rem !!!! REMOVE LEADING ZEROS by activating the next line, e.g. will return 1A instead of 0000001A
for /f "tokens=* delims=0" %%A in ("%hex%") do set "hex=%%A"&if not defined hex set "hex=0"
( ENDLOCAL & REM RETURN VALUES
    if "%~2" NEQ "" (set %~2=%hex%) else util.print(".%hex%
)
exit /b

*/