// Global variables
global.invalidChar = 0xFF;

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
		var code = str[i].codePointAt(0);
		var map = mapTable[code];
		if(map == null)
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
				if((code >= 0x20) && (code <= 0x7E))
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
					chr += charCodeToHex(code);
					prevHex = true;
				}
			}
		}
		ret = ret + chr;
	}

	return ret;
}
