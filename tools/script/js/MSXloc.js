//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)

//=============================================================================
// DEFINES
//=============================================================================

const fs = require('fs');

//-----------------------------------------------------------------------------
// Contants

const version = "1.0.0";

const lineTypeSection = 1;
const lineTypeText    = 2;

//-----------------------------------------------------------------------------
// Variables

var bVerbose = true;
var outFilename = "./localization.h";
var structName = "g_LocData";
var bComment = true;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
function log(str)
{
	if(bVerbose)
		console.log(str);
}

//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)
function getFirstChar(str)
{
	for (var i = 0; i < str.length; i++)
		if((str[i] != ' ') && (str[i] != '\t'))
			return str[i];

	return 0;
}

//-----------------------------------------------------------------------------
// Parse a line and return a descriptor according to line type
function getLineType(str)
{
	var obj = {};
	const chr = getFirstChar(str);
	if(chr == '[')
	{
		const reg = /\[[ \t]*(?<section>[a-zA-Z0-9]+)[ \t]*\]/;
		const res = str.match(reg);
		if(res == null)
		{
			console.log(`Error: Invalid section! ${str}`);
			return null;
		}
		obj['type'] = lineTypeSection;
		obj['value'] = res.groups.section.toUpperCase();
	}
	else if(chr != 0)
	{
		const reg = /[ \t]*(?<key>[a-zA-Z0-9]+)[ \t]*\=[ \t]*"(?<value>[^"]+)"/;
		const res = str.match(reg);
		if(res == null)
		{
			console.log(`Error: Invalid text! ${str}`);
			return null;
		}
		obj['type'] = lineTypeText;
		obj['key'] = res.groups.key.toUpperCase();
		obj['value'] = res.groups.value;
	}
	else
		return null;

	return obj;
}

//-----------------------------------------------------------------------------
// Parse a localisation file
function parseLocFile(filename, locTable)
{
	log("Parsing " + filename + "...");

	var lang = null;
	const fileData = fs.readFileSync(filename, 'utf8');
	const lines = fileData.split('\n');
	for (var line = 0; line < lines.length; line++)
	{
		const obj = getLineType(lines[line]);
		if(obj != null)
		{
			switch(obj.type)
			{
				case lineTypeSection:
					if(obj.value != lang)
					{
						lang = obj.value;
					}
					break;

				case lineTypeText:
					if(locTable[lang] == null)
						locTable[lang] = [];
					locTable[lang][obj.key] = obj.value;
					break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Display program usage and options
function displayHelp()
{
	console.log("\nUsage:");
	console.log("\tMSXloc file1.loc [file2.loc [...]] [options]");

	console.log("\nOptions:");
	console.log("\t-h                           Display this help");
	console.log("\t--help");
	console.log(`\t-o {filename}                Select the output filename (default is '${outFilename}')`);
	console.log("\t--output {filename}");
	console.log("\t-v                           Activate verbose mode");
	console.log("\t--verbose");
	console.log(`\t-s {structname}              Select the structure name (default is '${structName}')`);
	console.log("\t--struct {structname}");
}

//=============================================================================
// Main programm
//=============================================================================

console.log(`MSXloc ${version}`);

//-- Setup command line overwrite parameters
var inputFiles = [];
var cmdArgs = process.argv.slice(2);
for(var i = 0; i < cmdArgs.length; i++)
{
	const arg = cmdArgs[i];
	if(arg[0] == '-')
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
			outFilename = cmdArgs[++i];
			break;
		case "-s":
		case "--struct":
			structName = cmdArgs[++i];
			break;
		}
	}
	else
		inputFiles.push(arg);
}




//-----------------------------------------------------------------------------
// Parse the input file
var locTable = [];
for(var i = 0; i < inputFiles.length; i++)
	parseLocFile(inputFiles[i], locTable);
// log(locTable);

//-----------------------------------------------------------------------------
// Analyze localisation data
var langIDs = [];
var textIDs = [];
var langKeys = Object.keys(locTable);
for(var i = 0; i < langKeys.length; i++)
{
	langIDs.push(langKeys[i]);
	var textKeys = Object.keys(locTable[langKeys[i]]);
	for(var j = 0; j < textKeys.length; j++)
		if(!textIDs.includes(textKeys[j]))
			textIDs.push(textKeys[j]);
}
// log(langIDs);
// log(textIDs);

//-----------------------------------------------------------------------------
// Generate header file
var i;
var headContent = `// Generated by MSXloc ${version}\n#pragma once\n#include "core.h"\n\n`;

headContent += "// Langage definition\n";
for(i = 0; i < langIDs.length; i++)
	headContent += `#define LANG_${langIDs[i]} ${i}\n`;
headContent += `#define LANG_MAX ${i}\n\n`;

headContent += "// Text ID definition\n";
for(i = 0; i < textIDs.length; i++)
	headContent += `#define TEXT_${textIDs[i]} ${i}\n`;
headContent += `#define TEXT_MAX ${i}\n\n`;

headContent += "// Localization data table\n";
headContent += `const c8* ${structName}[LANG_MAX][TEXT_MAX] =\n{\n`;

var langKeys = Object.keys(locTable);
for(var i = 0; i < langKeys.length; i++)
{
	headContent += `\t// ${langKeys[i]}\n\t{\n`;
	for(var j = 0; j < textIDs.length; j++)
	{
		if(locTable[langKeys[i]][textIDs[j]] == null)
		{
			headContent += `\t\tNULL,`;
			log(`Warning: Text ID '${textIDs[j]}' not defined for langage '${langKeys[i]}'`);
		}
		else
			headContent += `\t\t"${locTable[langKeys[i]][textIDs[j]]}",`;
		if(bComment)
			headContent += ` // TEXT_${textIDs[j]}`;
		headContent += "\n";
	}
	headContent += `\t},\n`;
}

headContent += `};`;

// log(headContent);
fs.writeFileSync(outFilename, headContent);

