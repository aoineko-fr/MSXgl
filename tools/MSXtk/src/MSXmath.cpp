// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │            ▄▄   ▄▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▄█▄█ ▄▀██ ██▀  ██▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
//  available on GitHub (https://github.com/aoineko-fr/MSXgl/tree/main/tools/MSXtk)
//  under CC-BY-SA free license (https://creativecommons.org/licenses/by-sa/2.0/)
//─────────────────────────────────────────────────────────────────────────────

// std
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctime>
#include <limits>
#include <cfloat>

// MSX Tool Kit
#include "MSXtk.h"

//-----------------------------------------------------------------------------
// D E F I N E S

const c8* VERSION = "1.7.1";

const f64 PI = 3.14159265358979323846f;
const f64 PI_2 = 2.0f * PI;
const f64 PIdiv2 = 0.5f * PI;

//#define BUFFER_SIZE 1024

// Operator functions
f64 ComputeSquare(f64 x);
f64 ComputeMap(f64 x);
f64 ComputePower(f64 x);
f64 ComputeCoTan(f64 x);
f64 ComputeHypoDX(f64 x);
f64 ComputeHypoDY(f64 x);

// Operator structure
struct Operation
{
	const c8* op;
	const c8* Name;
	const c8* Range;
	f64       RangeMin;
	f64       RangeMax;
	bool      IncRange;
	bool      Signed;
	f64       (*Func)(f64);
};

//-----------------------------------------------------------------------------
// G L O B A L   V A R I A B L E S

// Number of table entry
i16					Number = 128;					
// Bytes per number (default: 16-bits)
u8					Bytes = 2;
MSX::DataSize		DataSize = MSX::DATASIZE_16bits;
// Number shift for fixed-point number (X * (1 << shift))
u8					Shift = 0;
// Prefix of the data table
const c8*			Prefix = "g_";			
// Constants used in some formula
f64					A = 0;					
f64					B = 0;
f64					C = 0;
f64					D = 0;
f64					E = 0;
// Screen size (for 3d projection)
i16					Width = 256;					
i16					Height = 212;
// Output file format
MSX::ExporterInterface* Exporter = NULL;
MSX::FileFormat		OutputFormat = MSX::FILEFORMAT_Auto;
std::string         OutputFile;

enum OPERATOR
{
	OP_SINUS = 0,
	OP_COSINUS,
	OP_TANGENT,
	OP_COTANGENT,
	OP_ARCSIN,
	OP_ARCCOS,
	OP_ARCTAN,
	OP_SQUARE,
	OP_SQUAREROOT,
	OP_MAP,
	OP_POWER,
	OP_EXP,
	OP_LOG,
	OP_LOG10,
	OP_HYPO_DX,
	OP_HYPO_DY,
};

// Operator list
const Operation OpTable[] =
{
	//         op       name           range         min:max        inc    sign   func
	/*  0 */ { "sin",   "Sinus",       "0:Pi*2",     0, PI_2,       false, true, sin },
	/*  1 */ { "cos",   "Cosinus",     "0:Pi*2",     0, PI_2,       false, true, cos },
	/*  2 */ { "tan",   "Tangent",     "-Pi/2:Pi/2", -PI/2, PI/2,   false, true, tan },
	/*  3 */ { "cot",   "Cotangent",   "-Pi/2:Pi/2", -PI/2, PI/2,   false, true, ComputeCoTan },
	/*  4 */ { "asin",  "ArcSinus",    "-1:1",       -1, 1,         true,  true, asin },
	/*  5 */ { "acos",  "ArcCosinus",  "-1:1",       -1, 1,         true,  true, acos },
	/*  6 */ { "atan",  "ArcTangent",  "0:N",        0, 0,          true,  true, atan },
	/*  7 */ { "sq",    "Square",      "0:1",        0, 1,          true,  true, ComputeSquare },
	/*  8 */ { "sqrt",  "SquareRoot",  "0:N",        0, 0,          false, true, sqrt },
	/*  9 */ { "map",   "Map",         "0:N",        0, 0,          false, true, ComputeMap },
	/* 10 */ { "pow",   "Power",       "0:N",        0, 0,          false, true, ComputePower },
	/* 11 */ { "exp",   "Exponential", "0:N",        0, 0,          false, true, exp },
	/* 12 */ { "log",   "Log",         "0:N",        0, 0,          false, true, log },
	/* 13 */ { "log10", "Log10",       "0:N",        0, 0,          false, true, log10 },
	/* 14 */ { "hdx",   "HypoDX",      "0:Pi*2",     0, PI_2,       false, true, ComputeHypoDX },
	/* 15 */ { "hdy",   "HypoDY",      "0:Pi*2",     0, PI_2,       false, true, ComputeHypoDY },
};

//-----------------------------------------------------------------------------
// U T I L I T Y   F U N C T I O N S

// Simple opetation functions
f64 ComputeCoTan(f64 x)      { return 1 / tan(x); }
f64 ComputeSquare(f64 x)     { return pow(x, 2); }
f64 ComputeMap(f64 x)        { return (x / (Number - 1)) * (B - A) + A; }
f64 ComputePower(f64 x)      { return pow(x, A); }
f64 ComputeHypoDX(f64 x)     { return (cos(x) == 0) ? DBL_MAX : sqrt(1.0 + pow(-sin(x) / cos(x), 2)); }
f64 ComputeHypoDY(f64 x)     { return (sin(x) == 0) ? DBL_MAX : sqrt(1.0 + pow(cos(x) / -sin(x), 2)); }

// Print a generic table using operation pointer function
f64 Clamp(f64 x, f64 min, f64 max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

// Print a generic table using operation pointer function
void ExportTable(i32 t)
{
	const Operation* op = &OpTable[t];

	// Initialize variables
	f64 multi = pow(2, Shift);
	i32 maxNumber = Number;
	if (op->IncRange)
		maxNumber++;
	f64 minRange = op->RangeMin;
	f64 maxRange = op->RangeMax;

	//c8 tmpStr[BUFFER_SIZE];

	// Handle special range cases
	if (minRange == 0 && maxRange == 0)
	{
		if (MSX::StrEqual(op->Range, "0:N")) // [0:N]
		{
			minRange = 0;
			maxRange = Number;
		}
		else if (MSX::StrEqual(op->Range, "0:N-1")) // [0:N-1]
		{
			minRange = 0;
			maxRange = Number - 1;
		}
		else if (MSX::StrEqual(op->Range, "-N:N")) // [-N:N]
		{
			minRange = -Number;
			maxRange = Number;
		}
		else if (MSX::StrEqual(op->Range, "-N/2:N/2")) // [-N/2:N/2]
		{
			minRange = -Number / 2;
			maxRange = Number / 2;
		}

	}

	// Add table
	Exporter->AddReturn();
	Exporter->AddComment(MSX::Format(" %s table. Range [%s%s", op->Name, op->Range, op->IncRange ? "]" : "["));
	Exporter->StartSection(MSX::Format("%s%s%d", Prefix, op->Name, Number), DataSize);

	// Table content
	for (i32 i = 0; i < Number; i++)
	{
		if ((i % 8 == 0))
			Exporter->StartLine();

		f64 x = (f64)i * (maxRange - minRange) / (f64)maxNumber + minRange;
		x = op->Func(x);
		x *= multi;
		x = round(x);

		switch (DataSize)
		{
		case MSX::DATASIZE_8bits:
			x = op->Signed ? Clamp(x, INT8_MIN, INT8_MAX) : Clamp(x, 0, UINT8_MAX);
			Exporter->AddByte(0xFF & (u32)x);
			break;
		case MSX::DATASIZE_16bits:
			x = op->Signed ? Clamp(x, INT16_MIN, INT16_MAX) : Clamp(x, 0, UINT16_MAX);
			Exporter->AddWord(0xFFFF & (u32)x);
			break;
		case MSX::DATASIZE_32bits:
			x = op->Signed ? Clamp(x, INT32_MIN, INT32_MAX) : Clamp(x, 0, UINT32_MAX);
			Exporter->AddDouble((u32)x);
			break;
		}

		if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
			Exporter->EndLine();
	}

	Exporter->EndSection();
}

// 
f64 ComputeAngle(f64 x, f64 y)
{
	f64 Angle;

	if(x == 0) // Particuliar case with Atn function (Atan in C)
	{
		if(y < 0)	
			Angle = -PI/2;
		else	
			Angle = PI/2;
	}
	else // "Regular" case
	{
		Angle = atan(y / x);
		if(x < 0)
			Angle = Angle + PI;
	}

	// Get a positive result
	if(Angle < 0)
		Angle = Angle + 2 * PI;
	return Angle;
}

// Display help information
void PrintHelp()
{
	printf("MSXmath %s - Mathematics table generator\n", VERSION);
	printf("Usage: MSXmath [options] [tables]\n");
	printf("Options:\n");
	printf("  -o <filename>    Filename of the output file (default: use input filename with .h/.asm/.bin extension)\n");
	printf("  -num   <N>       Entries Number (Precision. Default=128)\n");
	printf("  -shift <S>       Shift value (Fixed-point position. Default=4)\n");
	printf("  -bytes <B>       Bytes Number (1: 8bits, 2: 16 bits, 4: 32 bits. Default=2)\n");
	printf("  -prefix <str>    Table name prefix (0: Disable. Default=g_)\n");
	printf("  -format ?        Output file format\n");
	printf("    auto           Auto-detected using output file extension (default)\n");
	printf("    c              C header file format\n");
	printf("    asm            Assembler file format\n");
	printf("    bin            Raw binary data file\n");
	printf("  -at     X        Data base address\n");
	printf("  -help            Display this help\n");
	printf("Tables:\n");
	printf("  sin              Sinus table [0:Pi*2]\n");
	printf("  cos              Cosinus table [0:Pi*2]\n");
	printf("  tan              Tangente table [-Pi/2:Pi/2]\n");
	printf("  cot              Cotangente table [-Pi/2:Pi/2]\n");
	printf("  asin             Arc-sinus table [-1:1]\n");
	printf("  acos             Arc-cosinus table [-1:1]\n");
	printf("  atan             Arc-tangente table [-num:num]\n");
	printf("  sq               Square table [0:1]\n");
	printf("  sqrt             Square-root table [0:num]\n");
	printf("  pow  A           Power of A [0:num]\n");
	printf("  exp              Exponential\n");
	printf("  log              Natural logarithm (to the base e)\n");
	printf("  log10            Common logarithm (to the base 10)\n");
	printf("  hdx              Hypotenuse length when dX=1 [0:Pi*2]\n");
	printf("  hdy              Hypotenuse length when dY=1 [0:Pi*2]\n");
	printf("  proj N F         3d projection tables (N/F: near/far scale)\n");
	printf("  rot              Rotation vector table\n");
	printf("  equa A B C D E   Equation of type y=A+B*(C+x*D)^E\n");
	printf("  map  A B         Map [0:num[ values to [A:B] space\n");
	printf("  muls             Full 8-bit signed integer multiplication table (bytes should be 2\n");
	printf("  mulu             Full 8-bit unsigned integer multiplication table (bytes should be 2)\n");
	printf("  divs             Full 8-bit signed integer division table\n");
	printf("  divu             Full 8-bit unsigned integer division table\n");
	printf("Note: All number can be decimal or hexadecimal starting with '0x', '$' or '#'.\n");
}

//const c8* ARGV[] = { "", "-num", "512", "-Bytes", "2",  "-Shift", "6","equa", "0", "1", "1", "-0.001953125", "1.5" };
//const c8* ARGV[] = { "", "-num", "16", "-Bytes", "1",  "-Shift", "0","map", "0", "100" };
//const c8* ARGV[] = { "", "-o", "../testcases/sin.h", "-Shift", "12", "sin", "cos", "tan", "sq", "sqrt", "exp" };
//const c8* ARGV[] = { "", "-num", "256", "-bytes", "2",  "-shift", "8", "hdx", "hdy" };
//const c8* ARGV[] = { "", "-o", "../testcases/muls.h", "-num", "256", "-bytes", "2",  "-shift", "0", "muls" };
//#define DEBUG_ARGS


//-----------------------------------------------------------------------------
// M A I N   L  O O P 
int main(int argc, const c8* argv[])
{
#ifdef DEBUG_ARGS
	argc = numberof(ARGV); 
	argv = ARGV;
#endif

	// Search for -help option
	for (i32 i = 1; i < argc; ++i)
	{
		// Display help
		if (MSX::StrEqual(argv[i], "-help"))
		{
			PrintHelp();
			return 0;
		}
	}

	// Error check
	if(argc < 2)
	{
		printf("Error: No enough parameters!\n");
		PrintHelp();
		return 0;
	}

	MSX::ExportConfig conf;
	conf.Format = MSX::DATAFORMAT_Hexa;
	conf.Asm = MSX::ASMSYNTAX_Default;
	conf.Address = MSX::ADDRESS_INVALID;

	// Set paramters
	for(i32 argIndex = 1; argIndex < argc; argIndex++)
	{
		// Output filename
		if (MSX::StrEqual(argv[argIndex], "-o"))
		{
			OutputFile = argv[++argIndex];
		}
		else if(MSX::StrEqual(argv[argIndex], "-num"))
		{
			Number = MSX::StringToInt(argv[++argIndex]);
		}
		else if(MSX::StrEqual(argv[argIndex], "-shift"))
		{
			Shift = MSX::StringToInt(argv[++argIndex]);
		}
		else if(MSX::StrEqual(argv[argIndex], "-bytes"))
		{
			Bytes = MSX::StringToInt(argv[++argIndex]);
			switch (Bytes)
			{
			case 1: DataSize = MSX::DATASIZE_8bits;  break;
			case 2: DataSize = MSX::DATASIZE_16bits; break;
			case 4: DataSize = MSX::DATASIZE_32bits; break;
			}
		}
		else if (MSX::StrEqual(argv[argIndex], "-prefix"))
		{
			Prefix = argv[++argIndex];
			if (MSX::StrEqual(Prefix, "0"))
				Prefix = "";
		}
		else if (MSX::StrEqual(argv[argIndex], "-format"))
		{
			const c8* format = argv[++argIndex];
			if (MSX::StrEqual(format, "c"))
				OutputFormat = MSX::FILEFORMAT_C;
			else if (MSX::StrEqual(format, "asm"))
				OutputFormat = MSX::FILEFORMAT_Asm;
			else if (MSX::StrEqual(format, "bin"))
				OutputFormat = MSX::FILEFORMAT_Bin;
		}
		else if (MSX::StrEqual(argv[argIndex], "-at")) // Starting address
		{
			conf.Address = MSX::StringToInt(argv[++argIndex]);
		}
	}

	if (OutputFormat == MSX::FILEFORMAT_Auto)
		OutputFormat = MSX::File::GetFormat(OutputFile);

	switch (OutputFormat)
	{
	case MSX::FILEFORMAT_C:		Exporter = new MSX::ExporterC(conf);	break;
	case MSX::FILEFORMAT_Asm:	Exporter = new MSX::ExporterAsm(conf);	break;
	case MSX::FILEFORMAT_Bin:	Exporter = new MSX::ExporterBin(conf);	break;
	default:
		return -1;
	}

	// Date
	std::time_t result = std::time(nullptr);
	char* ltime = std::asctime(std::localtime(&result));
	ltime[strlen(ltime) - 1] = 0; // remove final '\n'

	// Add header
	Exporter->AddComment(u8" ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█            ▄▄   ▄▄");
	Exporter->AddComment(u8" ██  ▀  █▄  ▀██▄ ▀ ▄█  ▄█▄█ ▄▀██ ██▀  ██▄");
	Exporter->AddComment(u8" █  █ █  ▀▀  ▄█  █  █  ██ █ ▀▄██ ▀█▄▄ ██ █");
	Exporter->AddComment(u8" ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀");
	Exporter->AddComment(u8"─────────────────────────────────────────────────────────────────────────────");
	Exporter->AddComment(MSX::Format(" MSXmath %s by Guillaume \"Aoineko\" Blanchard", VERSION));
	Exporter->AddComment(" under CC-BY-SA free license");
	Exporter->AddComment("");
	Exporter->AddComment(MSX::Format(" Generated: %s", ltime));
	Exporter->AddComment(MSX::Format(" Parameters: Entries=%d, Bytes=%d (%i-bits), Shift=%d (Q%i.%i)", Number, Bytes, Bytes * 8, Shift, Bytes * 8 - Shift, Shift));
	Exporter->AddComment(u8"─────────────────────────────────────────────────────────────────────────────");
	
	// Create tables
	for(i32 argIndex = 1; argIndex < argc; argIndex++)
	{
		if(MSX::StrEqual(argv[argIndex], "sin")) // Sinus
		{
			ExportTable(OP_SINUS);
		}
		else if(MSX::StrEqual(argv[argIndex], "cos")) // Cosinus
		{
			ExportTable(OP_COSINUS);
		}
		else if(MSX::StrEqual(argv[argIndex], "tan")) // Tangent
		{
			ExportTable(OP_TANGENT);
		}
		else if (MSX::StrEqual(argv[argIndex], "cot")) // Cotangent
		{
			ExportTable(OP_COTANGENT);
		}
		else if(MSX::StrEqual(argv[argIndex], "asin")) // Arc-sinus
		{
			ExportTable(OP_ARCSIN);
		}
		else if(MSX::StrEqual(argv[argIndex], "acos")) // Arc-cosinus
		{
			ExportTable(OP_ARCCOS);
		}
		else if(MSX::StrEqual(argv[argIndex], "atan")) // Arc-tangent
		{
			ExportTable(OP_ARCTAN);
		}
		else if (MSX::StrEqual(argv[argIndex], "sq")) // Square
		{
			ExportTable(OP_SQUARE);
		}
		else if (MSX::StrEqual(argv[argIndex], "sqrt")) // Square-root
		{
			ExportTable(OP_SQUAREROOT);
		}
		else if (MSX::StrEqual(argv[argIndex], "map")) // Mapping
		{
			A = atof(argv[++argIndex]);
			B = atof(argv[++argIndex]);
			ExportTable(OP_MAP);
		}
		else if (MSX::StrEqual(argv[argIndex], "pow")) // Power: x^A
		{
			A = atof(argv[++argIndex]);
			ExportTable(OP_POWER);
		}
		else if (MSX::StrEqual(argv[argIndex], "exp")) // Exponential
		{
			ExportTable(OP_EXP);
		}
		else if (MSX::StrEqual(argv[argIndex], "log")) // Natural logarithm (to the base e)
		{
			ExportTable(OP_LOG);
		}
		else if (MSX::StrEqual(argv[argIndex], "log10")) // Common logarithm (to the base 10)
		{
			ExportTable(OP_LOG10);
		}
		else if (MSX::StrEqual(argv[argIndex], "hdx")) // Hypotenuse length when dX=1
		{
			ExportTable(OP_HYPO_DX);
		}
		else if (MSX::StrEqual(argv[argIndex], "hdy")) // Hypotenuse length when dY=1
		{
			ExportTable(OP_HYPO_DY);
		}
		else if (MSX::StrEqual(argv[argIndex], "muls")) // Full 8-bit unsigned integer multiplication table
		{
			// Add table
			Exporter->AddReturn();
			Exporter->AddComment("Multiplication table.");
			Exporter->StartSection(MSX::Format("%s%s%d", Prefix, "MulS", Number), DataSize);

			f64 multi = pow(2, Shift);
			for (int a = 0; a < Number; a++)
			{
				Exporter->AddComment(MSX::Format("A=%d", (i8)a));
				for (int b = 0; b < Number; b++)
				{
					if ((b % 8 == 0))
						Exporter->StartLine();

					f64 x = f64((i8)a) * f64((i8)b);
					x *= multi;
					x = round(x);

					switch (DataSize)
					{
					case MSX::DATASIZE_8bits:
						x = Clamp(x, INT8_MIN, INT8_MAX);
						Exporter->AddByte(0xFF & (u32)x);
						break;
					case MSX::DATASIZE_16bits:
						x = Clamp(x, INT16_MIN, INT16_MAX);
						Exporter->AddWord(0xFFFF & (u32)x);
						break;
					case MSX::DATASIZE_32bits:
						x = Clamp(x, INT32_MIN, INT32_MAX);
						Exporter->AddDouble((u32)x);
						break;
					}

					if (b % 8 == 7) // 8th column or last
						Exporter->EndLine();
				}
			}

			Exporter->EndSection();
		}
		else if (MSX::StrEqual(argv[argIndex], "proj")) // X/Y 3d projection according to Z value
		{
#define PROJ_FOCUS true

#if (!PROJ_FOCUS)
			f64 nearScale = atof(argv[++argIndex]);
			f64 farScale = atof(argv[++argIndex]);
			f64 power = atof(argv[++argIndex]);
#else
			f64 F = atof(argv[++argIndex]);
#endif

			// Add table
			Exporter->AddReturn();
			Exporter->AddComment("3D projection table");
#if (!PROJ_FOCUS)
			Exporter->AddComment(MSX::Format("Near: %0.3f, Far: %0.3f, Power: %0.3f", nearScale, farScale, power));
#else
			Exporter->AddComment(MSX::Format("Focal: %0.3f", F));
#endif
			Exporter->StartSection(MSX::Format("%s%s%d", Prefix, "Proj", Number), DataSize);

			f64 multi = pow(2, Shift);
			for (i32 j = 0; j < Number; j++)
			{
#if (!PROJ_FOCUS)
				f64 k0 = (f64)j / (f64)(Number - 1); // [0:1]
				k0 = pow(k0, power);
				f64 K = nearScale * (1.f - k0) + farScale * k0;
#else
				f64 K = F / (f64)(j + 1);
#endif

				Exporter->AddComment(MSX::Format("Z=%d, K=%0.3f", j, K));
				for (i32 i = 0; i < Number; i++)
				{
					if ((i % 8 == 0))
						Exporter->StartLine();

					f64 x = ((f64)i - ((f64)Number / 2));
					x *= K;
					x += (f64)(Number / 2);
					x *= multi;
					x = round(x);

					switch (DataSize)
					{
					case MSX::DATASIZE_8bits:
						x = Clamp(x, 0, UINT8_MAX);
						Exporter->AddByte(0xFF & (u32)x);
						break;
					case MSX::DATASIZE_16bits:
						x = Clamp(x, 0, UINT16_MAX);
						Exporter->AddWord(0xFFFF & (u32)x);
						break;
					case MSX::DATASIZE_32bits:
						x = Clamp(x, 0, UINT32_MAX);
						Exporter->AddDouble((u32)x);
						break;
					}

					if (i % 8 == 7) // 8th column or last
						Exporter->EndLine();
				}
			}

			Exporter->EndSection();
		}
		//else if(MSX::StrEqual(argv[argIndex], "proj")) // X/Y 3d projection according to Z value
		//{
		//	// @todo Convert to generic function and add it to operation table

		//	f64 yScale = cos(PI * 0.25);
		//	f64 xScale = yScale * Height / Width;
		//	f64 zFar = 0.01;
		//	f64 zNear = 16;
		//	f64 K = zFar / (zFar - zNear) + 1;
		//	f64 multi = (f64)(1 << Shift);

		//	// X projection table
		//	printf("static const signed %s %sProjectionX%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
		//	for(i32 i=0; i< Number; i++)
		//	{
		//		if ((i % 8 == 0))
		//			printf("\t");

		//		f64 z = i - (f64)Number/2;
		//		f64 x = multi * xScale / (z * K);

		//		if (Bytes == 1)
		//			printf("0x%02X, ", 0xFF & (i32)x);
		//		else
		//			printf("0x%04X, ", 0xFFFF & (i32)x);

		//		if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
		//			printf("\n");
		//	}
		//	printf("};\n");

		//	// Y projection table
		//	printf("static const signed %s %sProjectionY%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
		//	for(i32 i=0; i< Number; i++)
		//	{
		//		if ((i % 8 == 0))
		//			printf("\t");

		//		f64 z = i - (f64)Number/2;
		//		f64 x = multi * yScale / (z * K);

		//		if (Bytes == 1)
		//			printf("0x%02X, ", 0xFF & (i32)x);
		//		else
		//			printf("0x%04X, ", 0xFFFF & (i32)x);

		//		if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
		//			printf("\n");
		//	}
		//	printf("};\n");
		//}
		//else if(MSX::StrEqual(argv[argIndex], "rot")) // Rotation table
		//{
		//	// @todo Convert to generic function and add it to operation table

		//	f64 multi = (f64)(1 << Shift);
		//	printf("static const %s %sRotation%d[%d] =\n{\n\t", (Bytes == 1) ? "char" : "short", Prefix, Number, (2 * Number - 1) * (2 * Number - 1));
		//	for(i32 i = 1 - Number; i < Number; i++)
		//	{
		//		printf("/* x=%d */\n\t", i);
		//		for(i32 j = 1 - Number; j < Number; j++)
		//		{
		//			f64 x = multi * ComputeAngle((f64)i, (f64)j) / PI_2;
		//			x += 0.5; // for nearest approximation
		//			if(Bytes == 1)
		//				printf("0x%02X, ", 0xFF & (i32)x % (i32)multi);
		//			else
		//				printf("0x%04X, ", 0xFFFF & (i32)x % (i32)multi);
		//		}
		//		printf("\n\t");
		//	}
		//	printf("\n};\n");
		//}
		//else if(MSX::StrEqual(argv[argIndex], "equa")) // Equation of type y=A+B*(C+x*D)^E
		//{
		//	A = atof(argv[++argIndex]);
		//	B = atof(argv[++argIndex]);
		//	C = atof(argv[++argIndex]);
		//	D = atof(argv[++argIndex]);
		//	E = atof(argv[++argIndex]);
		//	f64 multi = (f64)(1 << Shift);

		//	printf("static const signed %s %sEqua%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
		//	for(i32 i=0; i<Number; i++)
		//	{
		//		if ((i % 8 == 0))
		//			printf("\t");

		//		f64 x = multi * (A + B * pow((C + (f64)i * D), E));
		//		if(Bytes == 1)
		//			printf("0x%02X, ", 0xFF & (i32)x);
		//		else
		//			printf("0x%04X, ", 0xFFFF & (i32)x);

		//		if((i % 8 == 7) && (i < Number - 1))
		//			printf("\n");
		//	}
		//	printf("};\n");
		//}
		//else
		//{
		//	printf("\n/* Error: Unknown table type \'%s\'! */\n", argv[argIndex]);
		//}
	}

	Exporter->AddComment(MSX::Format("Size: %d bytes", Exporter->GetTotalSize()));
	Exporter->Export(OutputFile);

	u32 retAddr = Exporter->GetTotalSize();
	if (conf.Address != MSX::ADDRESS_INVALID)
		retAddr += conf.Address;
	return retAddr;
}
