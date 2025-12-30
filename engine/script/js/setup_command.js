// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//-- MSXgl JS libraries
const util = require("./util.js"); 

//-----------------------------------------------------------------------------
// PARSE COMMAND LINE ARGUMENTS
//-----------------------------------------------------------------------------

//-- Setup command line overwrite parameters
CommandArgs = process.argv.slice(2);
for (let i = 0; i < CommandArgs.length; i++)
{
	const arg = CommandArgs[i].toLowerCase();
	if (arg.startsWith("projname="))
	{
		// Handled before loading sub-project file
	}
	else if (arg.startsWith("target="))
	{
		let val = CommandArgs[i].substring(7);
		if (val)
		{
			Target = val;
			util.print(`Command line overwrite => Target=${Target}`, PrintDetail);
		}
	}
	else if (arg.startsWith("machine="))
	{
		let val = CommandArgs[i].substring(8);
		if (val)
		{
			Machine = val;
			util.print(`Command line overwrite => Machine=${Machine}`, PrintDetail);
		}
	}
	else if (arg.startsWith("romsize="))
	{
		let val = CommandArgs[i].substring(8);
		if (val)
		{
			ROMSize = parseInt(val);
			util.print(`Command line overwrite => ROMSize=${ROMSize}`, PrintDetail);
		}
	}
	else if (arg === "delay")
	{
		ROMDelayBoot = true;
		util.print(`Command line overwrite => ROMDelayBoot=${ROMDelayBoot}`, PrintDetail);
	}
	else if (arg === "ramisr")
	{
		InstallRAMISR = "RAM0_ISR";
		util.print(`Command line overwrite => InstallRAMISR=${InstallRAMISR}`, PrintDetail);
	}
	else if (arg === "ramseg")
	{
		InstallRAMISR = "RAM0_SEGMENT";
		util.print(`Command line overwrite => InstallRAMISR=${InstallRAMISR}`, PrintDetail);
	}
	else if (arg === "clean")
	{
		DoClean = true;
	}
	else if (arg === "compile")
	{
		DoCompile = true;
	}
	else if (arg === "make")
	{
		DoMake = true;
	}
	else if (arg === "package")
	{
		DoPackage = true;
	}
	else if (arg === "deploy")
	{
		DoDeploy = true;
	}
	else if (arg === "run")
	{
		DoRun = true;
	}
	else if (arg === "all")
	{
		DoCompile = DoMake = DoPackage = DoDeploy = true;
	}
	else if (arg === "rebuild")
	{
		DoClean = DoCompile = DoMake = DoPackage = DoDeploy = true;
	}
	else if (arg.startsWith("define="))
	{
		let params = CommandArgs[i].substring(7).split(":");
		let val = params[0];
		if (params[1] !== undefined)
			val += `=${params[1]}`;
		CompileOpt += ` -D${val}`;
		util.print(`Command line overwrite => CompileOpt+=-D${val}`, PrintDetail);
	}
	else if (arg === "help")
	{
		util.print("Command line arguments:");
		util.print(" projname=<name>        Set project name (without extension)");
		util.print(" target=<name>          Set target name");
		util.print(" machine=<name>         Set machine name");
		util.print(" romsize=<size>         Set ROM size (in KB)");
		util.print(" delay                  Enable ROM delay boot");
		util.print(" ramisr                 Install the RAM0 interrupt service routine");
		util.print(" ramseg                 Install the RAM0 segment (with no ISR)");
		util.print(" clean                  Clean all generated files");
		util.print(" compile                Compile all source files");
		util.print(" make                   Link all files together");
		util.print(" package                Convert binary into target format");
		util.print(" deploy                 Deploy final files into separate directory");
		util.print(" run                    Run the built program with the selected emulator");
		util.print(" all                    Do all steps from compile to deploy");
		util.print(" rebuild                Clean all generated files, then do all steps");
		util.print(" define=<name>[:value]  Define a compilation symbol (can be used multiple times)");
		util.print(" help                   Display this command-line arguments list");
	}
	else
	{
		util.print(`Unknown command line overwrite '${arg}'`, PrintWarning);
	}
}