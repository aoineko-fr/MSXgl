// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄  ▄▄    ▄▄   ▄▄▄▄           ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██ █ ▄  ██   ▄██    ██  ▄█▀▄ ▄█▀▄ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄█ ██ ▀█▄ ▀▄██    ██  ▀█▄▀ ▀█▄▀ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//-- Node.jd libraries
const fs = require('fs');
const cp = require('child_process');

//=============================================================================
// HELPER FUNCTION
//=============================================================================

//
module.exports.getHex = function (a, s = 4)
{
	if(typeof a === 'undefined')
		return "";
	return (a).toString(16).toUpperCase();
}

// Determine whether the given `input` is a string.
module.exports.isString = function (input)
{
	return typeof input === 'string' && Object.prototype.toString.call(input) === '[object String]'
}

// Wait for key press
module.exports.waitKey = function ()
{
	console.log('Press any key to exit');
	process.stdin.setRawMode(true);
	process.stdin.resume();
	process.stdin.on('data', process.exit.bind(process, 0));
}

// Default print function
module.exports.print = function (string, flag = PrintNote)
{

	if((flag == PrintDetail) && (!Verbose))
		return;

	if(LogFile)
		fs.appendFileSync(`${OutDir}${LogFileName}`, `${string}\n`);

	if(LogStdout)
	{
		switch(flag) {
			case PrintError:		string = ColorRed + "Error: " + string + ColorReset; break;
			case PrintWarning:		string = ColorYellow + "Warning: " + string + ColorReset; break;
			case PrintSucced:		string = ColorGreen + string + ColorReset; break;
			case PrintHighlight:	string = ColorBlue + string + ColorReset; break;
			case PrintNote:			break;
			case PrintDetail:		string = ColorGray + string + ColorReset; break;
			case PrintBG:			string = ColorBG + string + ColorReset; break;
		}
		console.log(string);
	}
}

// Get date as a tag
module.exports.getDateTag = function ()
{
	const date = new Date();
	const year = date.getFullYear();
	const month = ("0" + (date.getMonth() + 1)).slice(-2);
	const day = ("0" + date.getDate()).slice(-2);
	const hour = ("0" + date.getHours()).slice(-2);
	const minute = ("0" + date.getMinutes()).slice(-2);
	const second = ("0" + date.getSeconds()).slice(-2);
	return `${year}${month}${day}_${hour}${minute}${second}`;
}

// Get date as a string
module.exports.getDateString = function ()
{
	const date = new Date();
	const year = date.getFullYear();
	const month = ("0" + (date.getMonth() + 1)).slice(-2);
	const day = ("0" + date.getDate()).slice(-2);
	const hour = ("0" + date.getHours()).slice(-2);
	const minute = ("0" + date.getMinutes()).slice(-2);
	const second = ("0" + date.getSeconds()).slice(-2);
	return `${year}-${month}-${day} ${hour}:${minute}:${second}`;
}

// Execute command
module.exports.exec = function (cmd) {
	module.exports.print(`Start: ${cmd}`, PrintDetail);
	cp.exec(cmd, (error, stdout, stderr) => {
		if (error) {
			console.log(error.message);
			return;
		}
		if (stderr) {
			console.log(stderr);
			return;
		}
		console.log(stdout);
	});
}

// Execute command and wait for completion
module.exports.execSync = function (cmd)
{
	module.exports.print(`Execute: ${cmd}`, PrintDetail);
	let exitCode = 0;
	try {
		cp.execSync(cmd, {stdio: 'inherit'});
	} catch (error) {
		exitCode = error.status;
		console.log(error.message);
		// module.exports.print(error.message, PrintError);
	}
	module.exports.print(`Exit code: ${exitCode}`, PrintDetail);
	return exitCode;
}

// Wait for all asynch task for completion
module.exports.synch = function ()
{
}

// 
module.exports.copyFile = function (src, dst)
{
	module.exports.print(`Copy ${src} to ${dst}`, PrintDetail);
	let exitCode = 0;
	try
	{
		fs.copyFileSync(src, dst);
	}
	catch (error)
	{
		exitCode = error.status;
		console.log(error.message);
		// module.exports.print(error.message, PrintError);
	}
	return exitCode;
}