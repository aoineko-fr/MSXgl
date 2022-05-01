// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "string.h"

// Table use to quick decimal-to-hexadecimal conversion
static const c8 g_HexChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

//-----------------------------------------------------------------------------
// Group: String

//-----------------------------------------------------------------------------
// Function: String_Format
// Build a zero-terminated string
//
// Parameters:
//   dest - Destination string buffer (must big enough to contain the whole string)
//   format - Formating string
//   ... - Variable number of parameter (depends on the Formating string)
void String_Format(c8* dest, const c8* format, ...)
{
	va_list args;
	va_start(args, format);

	const c8* ptr = format;
	while(*ptr != 0)
	{
		if(*ptr == '%')
		{
			ptr++;
			
			// Parse length
			u8 len = 0;
			if((*ptr >= '0') && (*ptr <= '9'))
			{
				len = *ptr - '0';
				ptr++;
			}
			while((*ptr >= '0') && (*ptr <= '9'))
			{
				len *= 10;
				len += *ptr - '0';
				ptr++;
			}

			// Parse variable types
			if((*ptr == 'i') || (*ptr == 'd'))
			{
				i16 val = (i16)va_arg(args, i16);
				if(val < 0)
				{	
					*dest++ = '-';
					val = -val;
				}
				
				c8 str[8];
				c8* ptr = str;
				*ptr = 0;
				while(val >= 10)
				{
					*++ptr = '0' + (val % 10);
					val /= 10;
				}
				*++ptr = '0' + val;
				while(*ptr != 0)
					*dest++ = *ptr--;
			}
			/*else if(*ptr == 'u')
			{
				u16 val = (u16)va_arg(args, u16);
				// Print_DrawInt(val);
			}*/
			else if(*ptr == 'x')
			{
				if(len == 0)
					len = 4;
				u16 val = (u16)va_arg(args, u16);
				if(len > 3)
					*dest++ = g_HexChar[(val >> 12) & 0x000F];
				if(len > 2)
					*dest++ = g_HexChar[(val >> 8) & 0x000F];
				if(len > 1)
					*dest++ = g_HexChar[(val >> 4) & 0x000F];
				*dest++ = g_HexChar[val & 0x000F];
			}
			else if(*ptr == 'c')
			{
				c8 val = (c8)va_arg(args, c8);
				*dest++ = val;
			}
			else if(*ptr == 's')
			{
				const c8* val = (const c8*)va_arg(args, const c8*);
				u8 len = String_Length(val);
				for(u8 i = 0; i < len; i++)
					*dest++ = *val++;
			}
		}
		else
			*dest++ = *ptr;
		
		ptr++;
	}
	*dest = 0;

	va_end(args);
}