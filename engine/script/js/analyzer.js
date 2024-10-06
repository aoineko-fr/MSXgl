//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)

//=============================================================================
// DEFINES
//=============================================================================

const fs = require('fs');
const path = require('path');

//-----------------------------------------------------------------------------
// Contants

const commandLine = false;
const version = "1.0.0";
const hexaChar = [ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' ];

const symbType_Abs  = 0;
const symbType_Code = 1;
const symbType_Var  = 2;

//-----------------------------------------------------------------------------
// Variables

// Configurations
var bVerbose = false;
var output = "both"; // "both", "file", "console"
var sortMethod = "size"; // "none", "alpha", "size"
var report = "ASMCV"; // [A]rea, [S]egment; [M]odule, [C]ode symbol, [V]ariable symbol
var bCSV = false;
var csvSeparator = ",";

// Global variables
var inputFiles = [];
var symbols = [];
var areas = [];

var bReportArea = true;
var bReportSegment = true;
var bReportModule = true;
var bReportCode = true;
var bReportVariable = true;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
function log(str)
{
	if (bVerbose)
		console.log(str);
}

//-----------------------------------------------------------------------------
// 
function isHexa(str)
{
	if (!str)
		return false;

	const res = str.match(/[^0-9A-F]/gi);
	return res === null;
}

//-----------------------------------------------------------------------------
// 
function extractWords(str)
{
	var tab = [];
	var cur = "";
	for (var i = 0; i < str.length; i++)
	{
		if ((str[i] == ' ') && (cur.length))
		{
			tab.push(cur);
			cur = "";
		}
		else if ((str[i] != ' ') && (str[i] != '\r'))
		{
			cur += str[i];
		}
	}

	if (cur.length)
		tab.push(cur);

	return tab;
}

//-----------------------------------------------------------------------------
// 
function parseFile(filename)
{
	log(`Analizing ${filename} ...`);

	symbols = [];
	areas = [];

	var curArea = {};
	var prevSymb = {};

	const fileData = fs.readFileSync(filename, 'utf8');
	const lines = fileData.split('\n');
	for (var i = 0; i < lines.length; i++)
	{
		var words = extractWords(lines[i]);
		
		// Found an area section 
		if (words[0] == "Area")
		{
			i += 2;
			words = extractWords(lines[i]);
			const areaName = words[0];
			if (!curArea || (areaName != curArea.name))
			{
				log(`Found area: ${curArea.name}`);

				// Set size of the last symbol of the previous area
				if (curArea && prevSymb)
					prevSymb.size = curArea.value + curArea.size - prevSymb.value;

				// Build area object
				var a = {};
				a.name = areaName;
				if (areaName == ".")
				{
					a.addr = 0;
					a.value = 0;
					a.segment = 0;
					a.size = 0;
				}
				else
				{
					a.addr = words[1];
					a.value = parseInt(a.addr.substring(4), 16);
					a.segment = parseInt(a.addr.substring(0, 4), 16);
					a.size = parseInt(words[2], 16);
				}
				areas.push(a);

				if (areaName == "_INITIALIZER")
				{
					var symb = {};
					symb.name = "INITIALIZER";
					symb.area = areaName;
					symb.addr = a.addr;
					symb.value = a.value;
					symb.segment = a.segment;
					symb.size = a.size;
					symb.module = "INITIALIZER";
					symbols.push(symb);
				}
	
				curArea = a;
				prevSymb = null;
			}
		}
		// Found a valid symbol
		else if (isHexa(words[0]) && (words[1].indexOf("$") == -1) && (curArea.name != "."))
		{
			// Build symbol object
			const label = words[1];
			var symb = {};
			symb.name = label;
			symb.area = curArea.name;
			symb.addr = words[0];
			symb.value = parseInt(symb.addr.substring(4), 16);
			symb.segment = parseInt(symb.addr.substring(0, 4), 16);
			symb.size = 0;
			symb.module = (words[2] && label.substring(0, 2) != "__") ? words[2] : "sdcc";
			symbols.push(symb);
			// log(symb);

			if (prevSymb)
				prevSymb.size = symb.value - prevSymb.value;

			prevSymb = symb;
		}
	}

	// Set size of the last symbol of the previous area
	if (curArea && prevSymb)
		prevSymb.size = curArea.value + curArea.size - prevSymb.value;
}

//-----------------------------------------------------------------------------
//
function getSorted(obj) 
{
	var keys = [];
	for (var key in obj)
		keys.push(key);

	if (sortMethod == "size")
		return keys.sort(function(a, b) { return obj[b].size - obj[a].size } );
	else if (sortMethod == "alpha")
		return keys.sort(function(a, b) { return obj[a].name.localeCompare(obj[b].name) } );

	return keys;
}

//-----------------------------------------------------------------------------
//
function exportFile(filename)
{
	var areaData = [];
	var segData = [];
	var modData = [];
	var codeSize = 0;
	var varSize = 0;

	// Analyze
	for (var i = 0; i < symbols.length; i++)
	{
		const label = symbols[i];
		// log(label);

		var type = symbType_Abs;
		if ((label.area == "_DATA") || (label.area == "_INITIALIZED"))
			type = symbType_Var;
		else
			type = symbType_Code;

		if (!areaData[label.area])
			areaData[label.area] = { name: label.area, size: 0 };
		areaData[label.area].size += label.size;

		if (type == symbType_Code)
		{
			codeSize += label.size;

			if (!segData[label.segment])
				segData[label.segment] = { name: `Segment ${label.segment}`, size: 0 };
			segData[label.segment].size += label.size;
	
			if (!modData[label.module])
				modData[label.module] = { name: label.module, size: 0 };
			modData[label.module].size += label.size;
		}
		else
			varSize += label.size;
	}

	// Report
	var outLog = "";
	var outCsv = "";

	outLog += "-------------------------------------------------------------------------------\n";
	outLog += " Total size (bytes)\n";
	outLog += "-------------------------------------------------------------------------------\n";
	outLog += `Code\t\t${codeSize}\n`;
	outLog += `Vars\t\t${varSize}\n`;

	if (bReportArea)
	{
		outLog += "-------------------------------------------------------------------------------\n";
		outLog += " Areas size (bytes)\n";
		outLog += "-------------------------------------------------------------------------------\n";
		var sortedArea = getSorted(areaData);
		for (var i = 0; i < sortedArea.length; i++)
		{
			const a = areaData[sortedArea[i]];
			outLog += `${a.name}\t\t${a.size}\n`;
		}
	}

	if (bReportSegment)
	{
		outLog += "-------------------------------------------------------------------------------\n";
		outLog += " Segments size (bytes)\n";
		outLog += "-------------------------------------------------------------------------------\n";
		var sortedSeg = getSorted(segData);
		for (var i = 0; i < sortedSeg.length; i++)
		{
			const a = segData[sortedSeg[i]];
			outLog += `${a.name}\t\t${a.size}\n`;
		}
	}

	if (bReportModule)
	{
		outLog += "-------------------------------------------------------------------------------\n";
		outLog += " Modules size (bytes)\n";
		outLog += "-------------------------------------------------------------------------------\n";
		var sortedMod = getSorted(modData);
		for (var i = 0; i < sortedMod.length; i++)
		{
			const a = modData[sortedMod[i]];
			outLog += `${a.name}\t\t${a.size}\n`;
		}
	}

	if (bReportCode || bReportVariable)
	{
		outCsv = `SYMBOL${csvSeparator}SIZE${csvSeparator}MODULE${csvSeparator}AREA${csvSeparator}SEGMENT${csvSeparator}TYPE\n`;

		var sortedSymb = getSorted(symbols);

		if(bReportCode)
		{
			outLog += "-------------------------------------------------------------------------------\n";
			outLog += " Code symbols size (bytes), module, area, segment\n";
			outLog += "-------------------------------------------------------------------------------\n";
			for (var i = 0; i < sortedSymb.length; i++)
			{
				const a = symbols[sortedSymb[i]];
				if ((a.area != "_DATA") && (a.area != "_INITIALIZED"))
				{
					outLog += `${a.name}\t\t${a.size}\t\t${a.module}\t\t${a.area}\t\t${a.segment}\n`;
					outCsv += `${a.name}${csvSeparator}${a.size}${csvSeparator}${a.module}${csvSeparator}${a.area}${csvSeparator}${a.segment}${csvSeparator}code\n`;
				}
			}
		}

		if(bReportVariable)
		{
			outLog += "-------------------------------------------------------------------------------\n";
			outLog += " Variable symbols size (bytes), module, area\n";
			outLog += "-------------------------------------------------------------------------------\n";
			for (var i = 0; i < sortedSymb.length; i++)
			{
				const a = symbols[sortedSymb[i]];
				if ((a.area == "_DATA") || (a.area == "_INITIALIZED"))
				{
					outLog += `${a.name}\t\t${a.size}\t\t${a.module}\t\t${a.area}\n`;
					outCsv += `${a.name}${csvSeparator}${a.size}${csvSeparator}${a.module}${csvSeparator}${a.area}${csvSeparator}${a.segment}${csvSeparator}var\n`;
				}
			}
		}
	}

	if((output == "both") || (output == "console"))
		console.log(outLog);
	
	if((output == "both") || (output == "file"))
	{
		log(`Exporting ${filename}.txt ...`);
		fs.writeFileSync(`${filename}.txt`, outLog);
	}

	if (bCSV)
	{
		log(`Exporting ${filename}.csv ...`);
		fs.writeFileSync(`${filename}.csv`, outCsv);
	}
}

//-----------------------------------------------------------------------------
// Display program usage and options
function analyzer()
{
	// Validate parameters
	report = report.toUpperCase();
	bReportArea = report.indexOf("A") != -1;
	bReportSegment = report.indexOf("S") != -1;
	bReportModule = report.indexOf("M") != -1;
	bReportCode = report.indexOf("C") != -1;
	bReportVariable = report.indexOf("V") != -1;

	if(!bReportArea && !bReportSegment && !bReportModule && !bReportCode && !bReportVariable)
	{
		console.log("Error: Nothing to report! Activate area, segment, module or symbol reporting.");
		return;
	}

	if((!bReportCode && !bReportVariable) && bCSV)
	{
		console.log("Warning: Report symbol option is need for CVS exporter! Symbol report will be activated.");
		bReportCode = true;
		bReportVariable = true;
	}

	output = output.toLowerCase();
	switch(output)
	{
	case "both":
	case "file":
	case "console":
		break;
	default:
		console.log(`Warning: Unknow '${output}' output method. 'both' output will be used instead`);
		output = "both";
	}

	sortMethod = sortMethod.toLowerCase();
	switch(sortMethod)
	{
	case "none":
	case "alpha":
	case "size":
		break;
	default:
		console.log(`Warning: Unknow '${sortMethod}' sort method. 'size' sort will be used instead`);
		sortMethod = "size";
	}

	log("-------------------------------------------------------------------------------");
	log(" Options");
	log("-------------------------------------------------------------------------------");
	log(`Verbose:       ${bVerbose}`);
	log(`Output:        ${output}`);
	log(`Sort method:   ${sortMethod}`);
	log(`Report:        ${report}`);
	log(` Areas:        ${bReportArea}`);
	log(` Segments:     ${bReportSegment}`);
	log(` Modules:      ${bReportModule}`);
	log(` Code symbols: ${bReportCode}`);
	log(` Variable sym: ${bReportVariable}`);
	log(`Generate CSV:  ${bCSV}`);
	log(`CSV separator: ${csvSeparator}`);
	log("-------------------------------------------------------------------------------");
	
	// Parse the input file
	for (var i = 0; i < inputFiles.length; i++)
	{
		parseFile(inputFiles[i]);
	
		const expFile = path.parse(inputFiles[i]);
		exportFile(`${expFile.dir}/${expFile.name}_analyze`);
	}
}

//-----------------------------------------------------------------------------
// Display program usage and options
function displayHelp()
{
	console.log("\nUsage:");
	console.log("\tmap_analyzer file1.map [file2.map [...]] [options]");

	console.log("\nOptions:");
	console.log("\t-h                           Display this help");
	console.log("\t--help                       Same as -h");
	console.log("\t-v                           Activate verbose mode");
	console.log("\t--verbose                    Same as -v");
	console.log("\t-o both|file|console         Outpout method (default: 'both')");
	console.log("\t--output                     Same as -o");
	console.log("\t-s none|size|alpha           Sorting method (default: 'size')");
	console.log("\t--sort                       Same as -s");
	console.log("\t-r [A][S][M][C][V]           Elements to report (default: 'ASMCV')");
	console.log("\t\tA                          Report areas");
	console.log("\t\tS                          Report segments");
	console.log("\t\tM                          Report modules");
	console.log("\t\tC                          Report code symbols");
	console.log("\t\tV                          Report variable symbols");
	console.log("\t--report                     Same as -r");
	console.log("\t-c                           Generate a .CSV file with all symbols");
	console.log("\t--csv                        Same as -c");
	console.log("\t--separator                  CSV separator (default: ',')");
}

//=============================================================================
// Main programm
//=============================================================================

//-----------------------------------------------------------------------------
//
function execCommandeLine()
{
	console.log(`MAP Analyzer ${version}`);

	//-- Setup command line overwrite parameters
	var cmdArgs = process.argv.slice(2);

	if (cmdArgs.length < 1)
	{
		displayHelp();
		return;
	}

	for (var i = 0; i < cmdArgs.length; i++)
	{
		const arg = cmdArgs[i];
		if (arg[0] == '-')
		{
			switch(arg.toLowerCase())
			{
			case "-?":
			case "-h":
			case "--help":
				displayHelp();
				return;
			case "-v":
			case "--verbose":
				bVerbose = true;
				break;
			case "-o":
			case "--output":
				output = cmdArgs[++i];
				break;
			case "-s":
			case "--sort":
				sortMethod = cmdArgs[++i];
				break;
			case "-r":
			case "--report":
				report = cmdArgs[++i];
				break;
			case "-c":
			case "--csv":
				bCSV = true;
				break;
			case "--separator":
				csvSeparator = cmdArgs[++i];
				break;
			}
		}
		else
			inputFiles.push(arg);
	}
	
	analyzer();
}

if (commandLine)
	execCommandeLine();

//-----------------------------------------------------------------------------
//
module.exports.exec = function (file, verb, out, sort, rep, csv, sep)
{
	inputFiles.push(file);

	bVerbose = verb;
	output = out;
	sortMethod = sort;
	report = rep;
	bCSV = csv;
	csvSeparator = sep;

	analyzer();
}