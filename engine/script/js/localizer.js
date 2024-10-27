//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)

//=============================================================================
// DEFINES
//=============================================================================

//-- Node.js libraries
const fs = require('fs');

//-- MSXgl JS libraries
const util = require("./util.js"); 
const locUtil = require("./localize/localize_util.js"); 
const locArb = require("./localize/localize_arb.js"); 
const locCyr = require("./localize/localize_cyr.js"); 
const locLat = require("./localize/localize_lat.js"); 
const locJap = require("./localize/localize_jap.js"); 
const locKor = require("./localize/localize_kor.js"); 

//-----------------------------------------------------------------------------
// Contants

const version = "1.0.0";

const lineTypeSection = 1;
const lineTypeText    = 2;
const lineTypeSpecial = 3;

//-----------------------------------------------------------------------------
// Variables

var bComment = true;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Parse a line and return a descriptor according to line type
function getLineType(str)
{
	var obj = {};
	const chr = locUtil.getFirstChar(str);
	if((chr == '#') || (chr == ';')) // Comment
		return null;
	else if(chr == '[') // Language code
	{
		const reg = /\[\s*(?<section>[\w]+)\s*\]/;
		const res = str.match(reg);
		if(res == null)
		{
			console.log(`Error: Invalid section! ${str}`);
			return null;
		}
		obj['type'] = lineTypeSection;
		obj['value'] = res.groups.section.toUpperCase();
	}
	else if(chr != 0) // Text entry
	{
		const reg = /\s*(?<key>[\w]+)\s*\=\s*"(?<value>[^"]*)"/;
		const res = str.match(reg);
		if(res == null)
		{
			console.log(`Error: Invalid text! ${str}`);
			return null;
		}
		obj['type'] = (chr == '@') ? lineTypeSpecial : lineTypeText;
		obj['key'] = res.groups.key.toUpperCase();
		obj['value'] = res.groups.value;
	}
	else
		return null;

	return obj;
}

//-----------------------------------------------------------------------------
// Parse a localisation file
function parseLocFile(filename, locTable, langTable)
{
	locUtil.log("Parsing " + filename + "...");

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
				// Start a new language section
				case lineTypeSection:
					if(obj.value != lang)
					{
						lang = obj.value;
					}
					locUtil.resetSpecial(); // Reset language parameters
					break;

				// Special character entry
				case lineTypeSpecial:
					if(langTable[lang] == null)
						langTable[lang] = {};
					
					langTable[lang][obj.key] = obj.value;
					locUtil.handleSpecial(obj.key, obj.value);
					break;

				// Localization entry
				case lineTypeText:
					if(locTable[lang] == null)
						locTable[lang] = [];

					if(locTable[lang][obj.key] != null)
						locUtil.log(`Warning: Text ID '${obj.key}' already defined for language '${lang}'`);
					else
					{
						switch(lang)
						{
						case "INT": // International
						case "ENG": // English
						case "EN":
						case "FRA": // French
						case "FRE":
						case "FR":
						case "GER": // German
						case "DEU":
						case "DE":
						case "SPA": // Spanish
						case "ES":
						case "DAN": // Danish
						case "DA":
						case "NO":  // Norwegian
						case "NOR":
						case "NL":  // Dutch
						case "NLD":
						case "DUT":
						case "FI":  // Finnish
						case "FIN":
						case "DE":  // German
						case "DEU":
						case "GER":
						case "IS":  // Icelandic
						case "ISL":
						case "ICE":
						case "GA":  // Irish
						case "GLE":
						case "IT":  // Italian
						case "ITA":
						case "PL":  // Polish
						case "POL":
						case "PT":  // Portuguese
						case "POR":
						case "ES":  // Spanish, Castilian
						case "SPA":
						case "SK":  // Slovak
						case "SLK":
						case "SLO":
						case "SV":  // Swedish
						case "SWE":
						case "TR":  // Turkish
						case "TUR":
							locTable[lang][obj.key] = locLat.convertToMSX(obj.value);
							break;

						case "JA":  // Japanese
						case "JPN":
							locTable[lang][obj.key] = locJap.convertToMSX(obj.value);
							break;

						case "KO":  // Korean
						case "KOR":
							locTable[lang][obj.key] = locKor.convertToMSX(obj.value);
							break;

						case "AR": // Arabic
						case "ARA":
							locTable[lang][obj.key] = locArb.convertToMSX(obj.value);
							break;

						case "UK":  // Ukrainian
						case "UKR":
						case "RU":  // Russian
						case "RUS":
							locTable[lang][obj.key] = locCyr.convertToMSX(obj.value);
							break;

						default:
							locTable[lang][obj.key] = obj.value;
						}
					}
					break;
			}
		}
	}
}

//=============================================================================
// Main programm
//=============================================================================

module.exports.generate = function (inputFiles, outFilename, structName, verbose)
{
	invalidChar = 0x1C; // 0x3F
	bVerbose = verbose;

	//-----------------------------------------------------------------------------
	// Parse the input file
	var locTable = [];
	var langTable = [];
	for(var i = 0; i < inputFiles.length; i++)
		parseLocFile(inputFiles[i], locTable, langTable);

	// locUtil.log("locTable:");
	// locUtil.log(locTable);
	// locUtil.log("langTable:");
	// locUtil.log(langTable);

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

	// locUtil.log("langIDs:");
	// locUtil.log(langIDs);
	// locUtil.log("textIDs:");
	// locUtil.log(textIDs);

	//-----------------------------------------------------------------------------
	// Generate header file
	var i;
	var headContent = `// Generated by MSXgl on ${util.getDateString()}\n`
	headContent += `#pragma once\n#include "core.h"\n\n`;

	headContent += "// Language definition\n";
	for(i = 0; i < langIDs.length; i++)
		headContent += `#define LANG_${langIDs[i]} ${i}\n`;
	headContent += `#define LANG_MAX ${i}\n\n`;

	headContent += "// Text ID definition\n";
	for(i = 0; i < textIDs.length; i++)
		headContent += `#define TEXT_${textIDs[i]} ${i}\n`;
	headContent += `#define TEXT_MAX ${i}\n\n`;

	headContent += "// Localization data table\n";
	headContent += `const c8* const ${structName}[LANG_MAX][TEXT_MAX] =\n{\n`;

	var langKeys = Object.keys(locTable);
	for(var i = 0; i < langKeys.length; i++)
	{
		headContent += `\t// ${langKeys[i]}\n\t{\n`;
		for(var j = 0; j < textIDs.length; j++)
		{
			if(locTable[langKeys[i]][textIDs[j]] == null)
			{
				headContent += `\t\tNULL,`;
				locUtil.log(`Warning: Text ID '${textIDs[j]}' not defined for language '${langKeys[i]}'`);
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

	// locUtil.log("headContent:");
	// locUtil.log(headContent);

	fs.writeFileSync(outFilename, headContent);
}