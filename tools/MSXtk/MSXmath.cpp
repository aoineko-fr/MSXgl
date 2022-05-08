// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │            ▄▄   ▄▄  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▄█▄█ ▄▀██ ██▀  ██▄ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀▄██ ▀█▄▄ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
//  available on GitHub (https://github.com/aoineko-fr/MSXgl/tree/main/tools/MSXtk)
//  under CC-BY-AS free license (https://creativecommons.org/licenses/by-sa/2.0/)
//─────────────────────────────────────────────────────────────────────────────

// std
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctime>

// MSX Tool Kit
#include "MSXtk.h"


//-----------------------------------------------------------------------------
// D E F I N E S

const c8* VERSION = "1.6.1";

const f64 PI = 3.14159265358979323846f;
const f64 PI_2 = 2.0f * PI;
const f64 PIdiv2 = 0.5f * PI;

//#define BUFFER_SIZE 1024

// Operator functions
f64 ComputeSinus(f64 x);
f64 ComputeCosinus(f64 x);
f64 ComputeTangent(f64 x);
f64 ComputeArcSinus(f64 x);
f64 ComputeArcCosinus(f64 x);
f64 ComputeArcTangent(f64 x);
f64 ComputeSquare(f64 x);
f64 ComputeSquareRoot(f64 x);
f64 ComputeMap(f64 x);
f64 ComputePower(f64 x);

// Operator structure
struct Operation
{
	const c8* op;
	const c8* name;
	const c8* range;
	f64       range_min;
	f64       range_max;
	bool      inc_range;
	bool      need_sign;
	f64       (*func)(f64);
};

//-----------------------------------------------------------------------------
// G L O B A L   V A R I A B L E S

// Number of table entry
i16					Number = 128;					
// Bytes per number (default: 16-bits)
u8					Bytes = 2;
MSX::DataType		DataType = MSX::DATATYPE_U16;
// Data format
MSX::DataFormat	DataFormat = MSX::DATAFORMAT_HexaC;
// Number shift for fixed-point number (X * (1 << shift))
u8					Shift = 0;
// Prefix of the data table
const c8*			Prefix = "g_";			
// Data table starting address
bool				bUseStartAddr = false;
u32					StartAddr = 0;
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
MSX::FileFormat	Format = MSX::FILEFORMAT_C;
std::string         OutputFile;

// Operator list
const Operation OpTable[] =
{
	//        op      name          range         min:max        inc    sign   func
	/* 0 */ { "sin",  "Sinus",      "0:Pi*2",     0, PI_2,       false, false, ComputeSinus },
	/* 1 */ { "cos",  "Cosinus",    "0:Pi*2",     0, PI_2,       false, false, ComputeCosinus },
	/* 2 */ { "tan",  "Tangent",    "-Pi/2:Pi/2", -PI/2, PI_2/2, true,  false, ComputeTangent },
	/* 3 */ { "asin", "ArcSinus",   "-1:1",       -1, 1,         true,  false, ComputeArcSinus },
	/* 4 */ { "acos", "ArcCosinus", "-1:1",       -1, 1,         true,  false, ComputeArcCosinus },
	/* 5 */ { "atan", "ArcTangent", "0:N",        0, 0,          true,  false, ComputeArcTangent },
	/* 6 */ { "sq",   "Square",     "0:1",        0, 1,          true,  false, ComputeSquare },
	/* 7 */ { "sqrt", "SquareRoot", "0:N",        0, 0,          false, false, ComputeSquareRoot },
	/* 8 */ { "map",  "Map",        "0:N",        0, 0,          false, false, ComputeMap },
	/* 9 */ { "pow",  "Power",      "0:N",        0, 0,          false, false, ComputePower },
};

//-----------------------------------------------------------------------------
// U T I L I T Y   F U N C T I O N S

// Simple opetation functions
f64 ComputeSinus(f64 x)      { return sin(x); }
f64 ComputeCosinus(f64 x)    { return cos(x); }
f64 ComputeTangent(f64 x)    { return tan(x); }
f64 ComputeArcSinus(f64 x)   { return asin(x); }
f64 ComputeArcCosinus(f64 x) { return acos(x); }
f64 ComputeArcTangent(f64 x) { return atan(x); }
f64 ComputeSquare(f64 x)     { return pow(x, 2); }
f64 ComputeSquareRoot(f64 x) { return sqrt(x); }
f64 ComputeMap(f64 x)        { return (x / (Number - 1)) * (B - A) + A; }
f64 ComputePower(f64 x)      { return pow(x, A); }

// Print a generic table using operation pointer function
void PrintTable(i32 op)
{
	// Initialize variables
	f64 multi = pow(2, Shift);
	i32 maxNumber = Number;
	if (OpTable[op].inc_range)
		maxNumber++;
	f64 minRange = OpTable[op].range_min;
	f64 maxRange = OpTable[op].range_max;

	//c8 tmpStr[BUFFER_SIZE];

	// Handle special range cases
	if (minRange == 0 && maxRange == 0)
	{
		if (MSX::StrEqual(OpTable[op].range, "0:N")) // [0:N]
		{
			minRange = 0;
			maxRange = Number;
		}
		else if (MSX::StrEqual(OpTable[op].range, "0:N-1")) // [0:N-1]
		{
			minRange = 0;
			maxRange = Number - 1;
		}
		else if (MSX::StrEqual(OpTable[op].range, "-N:N")) // [-N:N]
		{
			minRange = -Number;
			maxRange = Number;
		}
		else if (MSX::StrEqual(OpTable[op].range, "-N/2:N/2")) // [-N/2:N/2]
		{
			minRange = -Number / 2;
			maxRange = Number / 2;
		}

	}

	// Add table
	Exporter->AddComment("");
	Exporter->StartSection(
		MSX::Format("%s%s%d", Prefix, OpTable[op].name, Number, maxNumber),
		MSX::Format("%s table. Range [%s%s\n", OpTable[op].name, OpTable[op].range, OpTable[op].inc_range ? "]" : "["));

	// Table content
	/*for (i32 i = 0; i < maxNumber; i++)
	{
		if ((i % 8 == 0))
			printf("\t");

		f64 x = (f64)i * (maxRange - minRange) / (f64)Number + minRange;
		x = OpTable[op].func(x);
		x *= multi;


		sprintf(tmpStr, "%s, ", MSX::GetDataFormat(MSX::DATAFORMAT_Hexa, Bytes));
		if (Bytes == 1)
			printf(tmpStr, 0xFF & (i32)x);
		else if (Bytes == 2)
			printf(tmpStr, 0xFFFF & (i32)x);
		else
			printf(tmpStr, (i32)x);

		StartAddr += Bytes;

		if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
			printf("\n");
	}*/



/*
	// Print table header
	if (Format == MSX::FILEFORMAT_C)
	{
		printf("\n// %s table. Range [%s%s\n", OpTable[op].name, OpTable[op].range, OpTable[op].inc_range ? "]" : "[");
		if (bUseStartAddr)
			printf("__at(0x%X) ", StartAddr);
		printf("const %s %s %s%s%d[%d] =\n{\n", OpTable[op].need_sign ? "signed" : "unsigned", (Bytes == 1) ? "char" : "short", Prefix, OpTable[op].name, Number, maxNumber);

		// Print table content
		for (i32 i = 0; i < maxNumber; i++)
		{
			if ((i % 8 == 0))
				printf("\t");

			f64 x = (f64)i * (maxRange - minRange) / (f64)Number + minRange;
			x = OpTable[op].func(x);
			x *= multi;

			
			sprintf(tmpStr, "%s, ", MSX::GetDataFormat(MSX::DATAFORMAT_Hexa, Bytes));
			if (Bytes == 1)
				printf(tmpStr, 0xFF & (i32)x);
			else if (Bytes == 2)
				printf(tmpStr, 0xFFFF & (i32)x);
			else
				printf(tmpStr, (i32)x);

			StartAddr += Bytes;

			if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
				printf("\n");
		}
		printf("};\n");
	}
	else if (Format == MSX::FILEFORMAT_Asm)
	{
		printf("\n; %s table. Range [%s%s\n", OpTable[op].name, OpTable[op].range, OpTable[op].inc_range ? "]" : "[");
		printf("%s%s%d:\n", Prefix, OpTable[op].name, Number);

		// Print table content
		for (i32 i = 0; i < maxNumber; i++)
		{
			if ((i % 8 == 0))
				printf("\t.%s ", (Bytes == 1) ? "db" : "dw");

			f64 x = (f64)i * (maxRange - minRange) / (f64)Number + minRange;
			x = OpTable[op].func(x);
			x *= multi;

			sprintf(tmpStr, "%s ", MSX::GetDataFormat(MSX::DATAFORMAT_Hexa, Bytes));
			printf(tmpStr, (i32)x);
			//if (Bytes == 1)
			//	printf("0x%02X", 0xFF & (i32)x);
			//else
			//	printf("0x%04X", 0xFFFF & (i32)x);

			if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
				printf("\n");
			else
				printf(", ");
		}
		printf("\n");
	}
*/
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
void Help()
{
	printf("MSXmath (v%s) - Math Table Generator\n", VERSION);
	printf("Usage: MSXmath [options] [tables]\n");
	printf("Options:\n");
	printf("  -o      X        Name of the output file\n");
	printf("  -num   <X>       Entries Number (Precision. Default=128)\n");
	printf("  -shift <X>       Shift value (Fixed-point position. Default=4)\n");
	printf("  -bytes <X>       Bytes Number (1: 8bits, 2: 16 bits. Default=2)\n");
	printf("  -prefix X        Table name prefix (0: Disable. Default=g_)\n");
	printf("  -format X        Output file format (C or ASM. Default=C)\n");
	printf("  -at     X        Data starting address (can be decimal or hexadecimal starting with '0x')\n");
	printf("  -help            Display this help\n");
	printf("Tables:\n");
	printf("  sin              Sinus table [0:Pi*2]\n");
	printf("  cos              Cosinus table [0:Pi*2]\n");
	printf("  tan              Tangente table [-Pi/2:Pi/2]\n");
	printf("  asin             Arc-sinus table [-1:1]\n");
	printf("  acos             Arc-cosinus table [-1:1]\n");
	printf("  atan             Arc-tangente table [-num:num]\n");
	printf("  sq               Square table [0:1]\n");
	printf("  sqrt             Square-root table [0:num]\n");
	printf("  pow  A           Power of A [0:num]\n");
	printf("  proj W H         3d projection tables (W/H: screen width/height)\n");
	printf("  rot              Rotation vector table\n");
	printf("  equa A B C D E   Equation of type y=A+B*(C+x*D)^E\n");
	printf("  map  A B         Map [0:num[ values to [A:B] space\n");
}

//const c8* ARGV[] = { "", "-num", "512", "-Bytes", "2",  "-Shift", "6","equa", "0", "1", "1", "-0.001953125", "1.5" };
//const c8* ARGV[] = { "", "-num", "16", "-Bytes", "1",  "-Shift", "0","map", "0", "100" };
//#define DEBUG_ARGS

//-----------------------------------------------------------------------------
// M A I N   L  O O P 
int main(int argc, const c8* argv[])
{
#ifdef DEBUG_ARGS
	argc = sizeof(ARGV) / sizeof(ARGV[0]); 
	argv = ARGV;
#endif

	// Error check
	if(argc < 2)
	{
		printf("Error: No enough parameters!\n");
		Help();
		return 0;
	}

	// Set paramters
	for(i32 argIndex = 1; argIndex < argc; argIndex++)
	{
		if(MSX::StrEqual(argv[argIndex], "-help"))
		{
			Help();
			return 0;
		}
		// Output filename
		else if (MSX::StrEqual(argv[argIndex], "-o"))
		{
			OutputFile = argv[++argIndex];
		}
		else if(MSX::StrEqual(argv[argIndex], "-num"))
		{
			Number = atoi(argv[++argIndex]);
		}
		else if(MSX::StrEqual(argv[argIndex], "-shift"))
		{
			Shift = atoi(argv[++argIndex]);
		}
		else if(MSX::StrEqual(argv[argIndex], "-bytes"))
		{
			Bytes = atoi(argv[++argIndex]);
			switch (Bytes)
			{
			case 1: DataType = MSX::DATATYPE_U8; break;
			case 2: DataType = MSX::DATATYPE_U16; break;
			case 4: DataType = MSX::DATATYPE_U32; break;
			case 8: DataType = MSX::DATATYPE_U64; break;
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
			if (MSX::StrEqual(format, "C"))
				Format = MSX::FILEFORMAT_C;
			else if (MSX::StrEqual(format, "ASM"))
				Format = MSX::FILEFORMAT_Asm;
		}
		else if (MSX::StrEqual(argv[argIndex], "-at")) // Starting address
		{
			bUseStartAddr = true;
			sscanf_s(argv[++argIndex], "%i", &StartAddr);
		}
	}

	MSX::ExporterConfig conf;
	conf.Format = DataFormat;
	conf.Type = DataType;
	if(bUseStartAddr)
		conf.Address = StartAddr;
	if (Format == MSX::FILEFORMAT_C)
		Exporter = new MSX::ExporterC(conf);
	/*else
		Exporter = new MSX::ExporterASM(DATA_HexaC);*/

	// Add header
	Exporter->AddComment("_____________________________________________________________________________");
	Exporter->AddComment(u8"   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄           ▄▄   ▄▄                                    ");
	Exporter->AddComment(u8"  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄█▄█ ▄▀██ ██▀  ██▄                                   ");
	Exporter->AddComment(u8"  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ █ ▀▄██ ▀█▄▄ ██ █                                  ");
	Exporter->AddComment("_____________________________________________________________________________");
	Exporter->AddComment(MSX::Format(" MSXmath %s by Guillaume \"Aoineko\" Blanchard", VERSION));
	Exporter->AddComment(" under CC-BY-AS free license");
	Exporter->AddComment("");
	Exporter->AddComment(" Generated: %s %s " __DATE__ " " __TIME__);
	Exporter->AddComment(MSX::Format(" Parameters: Entries=%d, Bytes=%d (%i-bits), Shift=%d (Q%i.%i)", Number, Bytes, Bytes * 8, Shift, Bytes * 8 - Shift, Shift));
	Exporter->AddComment("_____________________________________________________________________________");

	// Create tables
	for(i32 argIndex = 1; argIndex < argc; argIndex++)
	{
		if(MSX::StrEqual(argv[argIndex], "sin")) // Sinus
		{
			PrintTable(0);
		}
		else if(MSX::StrEqual(argv[argIndex], "cos")) // Cosinus
		{
			PrintTable(1);
		}
		else if(MSX::StrEqual(argv[argIndex], "tan")) // Tangent
		{
			PrintTable(2);
		}
		else if(MSX::StrEqual(argv[argIndex], "asin")) // Arc-sinus
		{
			PrintTable(3);
		}
		else if(MSX::StrEqual(argv[argIndex], "acos")) // Arc-cosinus
		{
			PrintTable(4);
		}
		else if(MSX::StrEqual(argv[argIndex], "atan")) // Arc-tangent
		{
			PrintTable(5);
		}
		else if (MSX::StrEqual(argv[argIndex], "sq")) // Square
		{
			PrintTable(6);
		}
		else if (MSX::StrEqual(argv[argIndex], "sqrt")) // Square-root
		{
			PrintTable(7);
		}
		else if (MSX::StrEqual(argv[argIndex], "map")) // Equation of type y=A+B*(C+x*D)^E
		{
			A = atof(argv[++argIndex]);
			B = atof(argv[++argIndex]);
			PrintTable(8);
		}
		else if(MSX::StrEqual(argv[argIndex], "proj")) // X/Y 3d projection according to Z value
		{
			// @todo Convert to generic function and add it to operation table

			f64 yScale = cos(PI * 0.25);
			f64 xScale = yScale * Height / Width;
			f64 zFar = 0.01;
			f64 zNear = 16;
			f64 K = zFar / (zFar - zNear) + 1;
			f64 multi = (f64)(1 << Shift);

			// X projection table
			printf("static const signed %s %sProjectionX%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(i32 i=0; i< Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				f64 z = i - (f64)Number/2;
				f64 x = multi * xScale / (z * K);

				if (Bytes == 1)
					printf("0x%02X, ", 0xFF & (i32)x);
				else
					printf("0x%04X, ", 0xFFFF & (i32)x);

				if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
					printf("\n");
			}
			printf("};\n");

			// Y projection table
			printf("static const signed %s %sProjectionY%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(i32 i=0; i< Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				f64 z = i - (f64)Number/2;
				f64 x = multi * yScale / (z * K);

				if (Bytes == 1)
					printf("0x%02X, ", 0xFF & (i32)x);
				else
					printf("0x%04X, ", 0xFFFF & (i32)x);

				if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
					printf("\n");
			}
			printf("};\n");
		}
		else if(MSX::StrEqual(argv[argIndex], "rot")) // Rotation table
		{
			// @todo Convert to generic function and add it to operation table

			f64 multi = (f64)(1 << Shift);
			printf("static const %s %sRotation%d[%d] =\n{\n\t", (Bytes == 1) ? "char" : "short", Prefix, Number, (2 * Number - 1) * (2 * Number - 1));
			for(i32 i = 1 - Number; i < Number; i++)
			{
				printf("/* x=%d */\n\t", i);
				for(i32 j = 1 - Number; j < Number; j++)
				{
					f64 x = multi * ComputeAngle((f64)i, (f64)j) / PI_2;
					x += 0.5; // for nearest approximation
					if(Bytes == 1)
						printf("0x%02X, ", 0xFF & (i32)x % (i32)multi);
					else
						printf("0x%04X, ", 0xFFFF & (i32)x % (i32)multi);
				}
				printf("\n\t");
			}
			printf("\n};\n");
		}
		else if(MSX::StrEqual(argv[argIndex], "equa")) // Equation of type y=A+B*(C+x*D)^E
		{
			A = atof(argv[++argIndex]);
			B = atof(argv[++argIndex]);
			C = atof(argv[++argIndex]);
			D = atof(argv[++argIndex]);
			E = atof(argv[++argIndex]);
			f64 multi = (f64)(1 << Shift);

			printf("static const signed %s %sEqua%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(i32 i=0; i<Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				f64 x = multi * (A + B * pow((C + (f64)i * D), E));
				if(Bytes == 1)
					printf("0x%02X, ", 0xFF & (i32)x);
				else
					printf("0x%04X, ", 0xFFFF & (i32)x);

				if((i % 8 == 7) && (i < Number - 1))
					printf("\n");
			}
			printf("};\n");
		}
		//else
		//{
		//	printf("\n/* Error: Unknow table type \'%s\'! */\n", argv[argIndex]);
		//}
	}

	Exporter->Export(OutputFile);

	return Exporter->GetTotalSize() + StartAddr;
}