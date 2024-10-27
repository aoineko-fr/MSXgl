//-----------------------------------------------------------------------------
// Global variables

// Invalid character vlaue
global.invalidChar = 0xFF;

// Invalid character vlaue
global.charOffset = 0;

// Invalid character vlaue
global.charSet = "";

// Log details about conversion processing
global.bVerbose = false;

//-----------------------------------------------------------------------------
// Log string or object
module.exports.log = function (str)
{
	if(bVerbose)
		console.log(str);
}

//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)
module.exports.getFirstChar = function (str)
{
	for (var i = 0; i < str.length; i++)
		if((str[i] != ' ') && (str[i] != '\t'))
			return str[i];

	return 0;
}


//-----------------------------------------------------------------------------
// Reset language parameters
module.exports.resetSpecial = function ()
{
	charOffset = 0;
	charSet = "";
}

//-----------------------------------------------------------------------------
// Handle special entries
module.exports.handleSpecial = function (key, value)
{
	switch(key)
	{
		case "OFFSET":
			charOffset = parseInt(value);
			break;

		case "CHARSET":
			charSet = value;
			break;
	}
}

//-----------------------------------------------------------------------------
// 
function charCodeToHex(code)
{
	return "\\x" + code.toString(16).padStart(2, '0').toUpperCase();
}

//-----------------------------------------------------------------------------
// 
function isHexCode(code)
{
	var chr = String.fromCodePoint(code);
	return (((chr >= "a") && (chr <= "f")) || ((chr >= "A") && (chr <= "F")) || ((chr >= "0") && (chr <= "9")))
}

// 
module.exports.mapCharacter = function (str, mapTable)
{
	var prevHex = false;
	var ret = "";
	for(var i = 0; i < str.length; i++)
	{
		var chr = str[i];
		if (chr === "\\" && str[i])
		{
		}

		var code = str[i].codePointAt(0);
		if (charSet.length)
		{
			var idx = charSet.indexOf(chr);
			if(idx == -1) // character not found ; keep original
				chr = charCodeToHex(code);
			else
				chr = charCodeToHex(idx + charOffset);
		}
		else
		{
			var map = mapTable[code];
			if(map == null) // character not found
			{
				chr = charCodeToHex(invalidChar);
				prevHex = true;
			}
			else
			{
				chr = "";
				for(var j = 0; j < map.length; j++)
				{
					code = map[j];
					if((code >= 0x20) && (code <= 0x7E) && (charOffset === 0) && (charSet.length === 0)) // ASCII alpha-numeric
					{
						if(prevHex && isHexCode(code))
						{
							chr += charCodeToHex(code);
							prevHex = true;
						}
						else
						{
							chr += String.fromCharCode(code);
							prevHex = false;
						}
					}
					else
					{
						code += charOffset;
						chr += charCodeToHex(code);
						prevHex = true;
					}
				}
			}
		}
		ret = ret + chr;
	}

	return ret;
}
