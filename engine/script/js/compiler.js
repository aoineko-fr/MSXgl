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

// %~1 - expands %1 //oving any surrounding quotes (")
// %~f1 - expands %1 to a fully qualified path name
// %~d1 - expands %1 to a drive letter only
// %~p1 - expands %1 to a path only
// %~n1 - expands %1 to a file name only
// %~x1 - expands %1 to a file extension only
// %~s1 - expanded path contains short names only
// %~a1 - expands %1 to file attributes
// %~t1 - expands %1 to date/time of file
// %~z1 - expands %1 to size of file

// Wait for all asynch task for completion
module.exports.compile = function (file, size, seg)
{
	let filePath = path.parse(file).dir + "/"; // %~d1%~p1
	let fileName = path.parse(file).name; // %~n1
	let fileExt = path.parse(file).ext; // %~x1

	//-------------------------------------------------------------------------
	// Skip file if compiled data is newer than the source
	if (CompileSkipOld)
	{
		let dstFile = `${OutDir}${fileName}.rel`;
		if (fs.existsSync(dstFile))
		{
			let dateSrc = fs.statSync(file);
			let dateDst = fs.statSync(dstFile);
			if (dateDst.mtime >= dateSrc.mtime)
			{
				util.print(`Skip compiling ${fileName}${fileExt}`, PrintWarning);
				return;
			}
		}
	}

	//*************************************************************************
	//* COMPILE C SOURCE
	//*************************************************************************
	if (fileExt == ".c")
	{
		let AddOpt = CompileOpt;
		// if (Verbose)			AddOpt += " --verbose";
		if (Optim === "SPEED")	AddOpt += " --opt-code-speed";
		if (Optim === "SIZE")	AddOpt += " --opt-code-size";
		if (size !== undefined)	AddOpt += ` --code-size ${size}`;
		if (seg !== undefined)	AddOpt += ` --codeseg ${seg}`;
		if (Debug)				AddOpt += " --debug";
		if (AllowUndocumented)	AddOpt += " --allow-undocumented-instructions";
		if (AppSignature)		AddOpt += " -DAPPSIGN";
		let MaxAllocs = 3000;
		switch (CompileComplexity)
		{
			case "FAST":		MaxAllocs = 2000; break;
			case "DEFAULT":		MaxAllocs = 3000; break;
			case "OPTIMIZED":	MaxAllocs = 50000; break;
			case "ULTRA":		MaxAllocs = 200000; break;
			case "INSANE":		MaxAllocs = 10000000; break;
			default:			MaxAllocs = CompileComplexity; break;
		}
		if(MaxAllocs != 3000) // skip default value 
			AddOpt += ` --max-allocs-per-node ${MaxAllocs}`;
		// set AddOpt=!AddOpt! --constseg RODATA

		let SDCCParam = `-c -mz80 -DTARGET=TARGET_${Target} -DMSX_VERSION=MSX_${Machine} -I${ProjDir} -I${LibDir}src -I${LibDir}content -I${ToolsDir} ${AddOpt} ${file} -o ${OutDir}`;

		util.print(`Compiling ${file} using SDCC C compiler...`, PrintHighlight);
		let err = util.execSync(`"${Compiler}" ${SDCCParam}`);
		if(err)
		{
			util.print(`Compile error! Code: ${err}`, PrintError);
			process.exit(310);
		}

		// Add file to Compile database
		if (GenCompileDB)
			CompileDB.push({directory: ProjDir, command: `"${Compiler}" ${SDCCParam}`, file: `${file}`, output: `${OutDir}${fileName}.rel` });

		// Generate dependencies list
		// err = util.execSync(`${Compiler} -M ${SDCCParam} > ${OutDir}${path.parse(file).name}.dep`);

		util.print(`Success`, PrintSuccess);
	}
	//*************************************************************************
	//* COMPILE ASSEMBLER SOURCE
	//*************************************************************************
	else if ((fileExt == ".s") || (fileExt == ".asm"))
	{
		let ASMParam = `-o -l -s -I${ProjDir} -I${OutDir} -I${LibDir}src ${file}`;

		util.print(`Compiling ${file} using SDASZ80 ASM compiler...`, PrintHighlight);

		let err = util.execSync(`"${Assembler}" ${ASMParam}`);
		if(err)
		{
			util.print(`Compile error! Code: ${err}`, PrintError);
			process.exit(320);
		}

		// Add file to Compile database
		if (GenCompileDB)
			CompileDB.push({directory: ProjDir, command: `"${Assembler}" ${ASMParam}`, file: `${file}`, output: `${OutDir}${fileName}.rel` });

		// Move symbols files to output directory
		fs.renameSync(`${filePath}${fileName}.rel`, `${OutDir}${fileName}.rel`)
		fs.renameSync(`${filePath}${fileName}.lst`, `${OutDir}${fileName}.lst`)
		fs.renameSync(`${filePath}${fileName}.sym`, `${OutDir}${fileName}.sym`)

		util.print(`Success`, PrintSuccess);
	}
	else
	{
		util.print(`Invalid file format '${fileExt}'!`, PrintError);
		process.exit(300);
	}
}