/* An MSX-diskimage creation/extraction program

   Copyright (C) 2004 David Heremans <dhran@pi.be>
   Copyright (C) 2005 BouKiCHi

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any later
   version.
   As a sidenote: Please inform me about your modifications if you make any.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
   Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <getopt.h>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using std::string;
using std::cout;
using std::endl;
using std::hex;
using std::dec;


/** 4 bit integer */
typedef unsigned char nibble;

/** 8 bit signed integer */
typedef char signed_byte;
/** 8 bit unsigned integer */
typedef unsigned char byte;

/** 16 bit signed integer */
typedef short signed_word;
/** 16 bit unsigned integer */
typedef unsigned short word;

/** 32 bit unsigned integer */
typedef unsigned int uint32;

#define PRT_DEBUG(mes)				\
	do {					\
		if (show_debug) {		\
			cout << "DEBUG: "<< mes << endl;	\
		}				\
	} while (0)

#define PRT_VERBOSE(mes)			\
	do {					\
		if (verbose_option) {		\
			cout << mes << endl;	\
		}				\
	} while (0)

#define CRITICAL_ERROR(mes)			\
	do {					\
		cout << "FATAL ERROR:"<< mes << endl;	\
		exit(1);			\
	} while (0)


struct MSXBootSector {
	byte jumpcode[3]	;	/* 0xE5 to bootprogram */
	byte name[8];
	byte bpsector[2];		/* bytes per sector (always 512) */
	byte spcluster[1];		/* sectors per cluster (always 2) */
	byte reservedsectors[2];	/* amount of non-data sectors (ex bootsector) */
	byte nrfats[1];			/* number of fats */
	byte direntries[2];		/* max number of files in root directory  */
	byte nrsectors[2];		/* number of sectors on this disk */
	byte descriptor[1];		/* media descriptor */
	byte sectorsfat[2];		/* sectors per FAT */
	byte sectorstrack[2];		/* sectors per track */
	byte nrsides[2];		/* number of sides */
	byte hiddensectors[2];		/* not used */
	byte bootprogram[512-30];	/* actual bootprogram */
};

struct MSXDirEntry {
	byte filename[8];
	byte ext[3];
	byte attrib;
	byte reserved[10]; /* unused */
	byte time[2];
	byte date[2];
	byte startcluster[2];
	byte size[4];
};

//Modified struct taken over from Linux' fdisk.h
struct partition {
	byte boot_ind;         /* 0x80 - active */
	byte head;             /* starting head */
	byte sector;           /* starting sector */
	byte cyl;              /* starting cylinder */
	byte sys_ind;          /* What partition type */
	byte end_head;         /* end head */
	byte end_sector;       /* end sector */
	byte end_cyl;          /* end cylinder */
	byte start4[4];        /* starting sector counting from 0 */
	byte size4[4];         /* nr of sectors in partition */
};

struct pc98part {
	byte boota;
	byte bootb;
	byte reserveA[6];
	byte reserveB[2];
	byte startcyl[2];
	byte reserveC[2];
	byte endcyl[2];
	byte name[16];
};

#ifndef ACCESSPERMS
#define ACCESSPERMS 0777 // is this ok?
#endif


word EOF_FAT = 0x0FFF; /* signals EOF in FAT12 */

int SECTOR_SIZE = 512;

const byte T_MSX_REG  = 0x00; // Normal file
const byte T_MSX_READ = 0x01; // Read-Only file
const byte T_MSX_HID  = 0x02; // Hidden file
const byte T_MSX_SYS  = 0x04; // System file
const byte T_MSX_VOL  = 0x08; // filename is Volume Label
const byte T_MSX_DIR  = 0x10; // entry is a subdir
const byte T_MSX_ARC  = 0x20; // Archive bit

struct physDirEntry
{
	int sector;
	byte index;
};

#define NUMOFENT (SECTOR_SIZE/0x20) // number of entries in 1sector

char* program_name;

long sizeOfDskFile;
byte* dskImage;
byte* FSImage;
string MSXrootdir;
string MSXhostdir;
string inputFile;
string outputFile;
int nbSectors ;
int maxCluster;
int sectorsPerCluster ;
int sectorsPerTrack ;
int sectorsPerFat ;
int nbFats ;
int nbSides ;
byte nbSectorsPerCluster = 2;
int nbRootDirSectors ;
int RootDirStart ; // first sector from the root directory
int RootDirEnd ;   // last sector from the root directory
int MSXchrootSector;
int MSXchrootStartIndex = 0;
int MSXpartition = 0;
int globalargc;
char** globalargv;
int verbose_option = 0;
bool do_test = false; // reserved the flag for test how don't want to write to disk actually.
bool do_flat = false; // reserved the flag for MSX1ers who don't like to create subdirs.
bool do_extract = false;
bool do_subdirs = true;
bool do_singlesided = false;
bool touch_option = false;
bool keep_option = false;
bool msxdir_option = false;
bool msxpart_option = false;
bool msx_allpart = false;

static int show_version = 0; /* If nonzero, display version information and exit */
static int show_help = 0; /* If nonzero, display usage information and exit */
static int show_debug = 0; /* If nonzero, display debug information while running */
static int show_bootinfo = 0; /* If nonzero, display debug information while running */
static int do_fat16 = 0; /* Force FAT16 support, ide >32M automatically sets this */

enum
{
  DEBUG_OPTION = CHAR_MAX + 1,
  OTHER_OPTION
};

// bootblock created with regular nms8250 and '_format'
byte Dos1BootBlock[] =
{
	0xEB,0xFE,0x90,0x4E,0x4D,0x53,0x20,0x32,0x2E,0x30,0x50,0x00,0x02,0x02,0x01,0x00,
	0x02,0x70,0x00,0xA0,0x05,0xF9,0x03,0x00,0x09,0x00,0x02,0x00,0x00,0x00,0xD0,0xED,
	0x53,0x59,0xC0,0x32,0xD0,0xC0,0x36,0x56,0x23,0x36,0xC0,0x31,0x1F,0xF5,0x11,0xAB,
	0xC0,0x0E,0x0F,0xCD,0x7D,0xF3,0x3C,0xCA,0x63,0xC0,0x11,0x00,0x01,0x0E,0x1A,0xCD,
	0x7D,0xF3,0x21,0x01,0x00,0x22,0xB9,0xC0,0x21,0x00,0x3F,0x11,0xAB,0xC0,0x0E,0x27,
	0xCD,0x7D,0xF3,0xC3,0x00,0x01,0x58,0xC0,0xCD,0x00,0x00,0x79,0xE6,0xFE,0xFE,0x02,
	0xC2,0x6A,0xC0,0x3A,0xD0,0xC0,0xA7,0xCA,0x22,0x40,0x11,0x85,0xC0,0xCD,0x77,0xC0,
	0x0E,0x07,0xCD,0x7D,0xF3,0x18,0xB4,0x1A,0xB7,0xC8,0xD5,0x5F,0x0E,0x06,0xCD,0x7D,
	0xF3,0xD1,0x13,0x18,0xF2,0x42,0x6F,0x6F,0x74,0x20,0x65,0x72,0x72,0x6F,0x72,0x0D,
	0x0A,0x50,0x72,0x65,0x73,0x73,0x20,0x61,0x6E,0x79,0x20,0x6B,0x65,0x79,0x20,0x66,
	0x6F,0x72,0x20,0x72,0x65,0x74,0x72,0x79,0x0D,0x0A,0x00,0x00,0x4D,0x53,0x58,0x44,
	0x4F,0x53,0x20,0x20,0x53,0x59,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// bootblock created with nms8250 and MSX-DOS 2.20
byte Dos2BootBlock[] =
{
	0xEB,0xFE,0x90,0x4E,0x4D,0x53,0x20,0x32,0x2E,0x30,0x50,0x00,0x02,0x02,0x01,0x00,
	0x02,0x70,0x00,0xA0,0x05,0xF9,0x03,0x00,0x09,0x00,0x02,0x00,0x00,0x00,0x18,0x10,
	0x56,0x4F,0x4C,0x5F,0x49,0x44,0x00,0x71,0x60,0x03,0x19,0x00,0x00,0x00,0x00,0x00,
	0xD0,0xED,0x53,0x6A,0xC0,0x32,0x72,0xC0,0x36,0x67,0x23,0x36,0xC0,0x31,0x1F,0xF5,
	0x11,0xAB,0xC0,0x0E,0x0F,0xCD,0x7D,0xF3,0x3C,0x28,0x26,0x11,0x00,0x01,0x0E,0x1A,
	0xCD,0x7D,0xF3,0x21,0x01,0x00,0x22,0xB9,0xC0,0x21,0x00,0x3F,0x11,0xAB,0xC0,0x0E,
	0x27,0xCD,0x7D,0xF3,0xC3,0x00,0x01,0x69,0xC0,0xCD,0x00,0x00,0x79,0xE6,0xFE,0xD6,
	0x02,0xF6,0x00,0xCA,0x22,0x40,0x11,0x85,0xC0,0x0E,0x09,0xCD,0x7D,0xF3,0x0E,0x07,
	0xCD,0x7D,0xF3,0x18,0xB8,0x42,0x6F,0x6F,0x74,0x20,0x65,0x72,0x72,0x6F,0x72,0x0D,
	0x0A,0x50,0x72,0x65,0x73,0x73,0x20,0x61,0x6E,0x79,0x20,0x6B,0x65,0x79,0x20,0x66,
	0x6F,0x72,0x20,0x72,0x65,0x74,0x72,0x79,0x0D,0x0A,0x24,0x00,0x4D,0x53,0x58,0x44,
	0x4F,0x53,0x20,0x20,0x53,0x59,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// Direct disk boot using MSX-DOS 1 format
byte Dos0BootBlock[] =
{
	0xEB,0xFE,0x90,0x42,0x4F,0x4F,0x54,0x44,0x49,0x53,0x4B,0x00,0x02,0x02,0x01,0x00,
	0x02,0x70,0x00,0xA0,0x05,0xF9,0x03,0x00,0x09,0x00,0x02,0x00,0x00,0x00,0xD0,0x31,
	0x1F,0xF5,0xCD,0x6B,0xF3,0x11,0x4D,0xC0,0x0E,0x0F,0xCD,0x7D,0xF3,0x3C,0xCA,0x1E,
	0xC0,0x11,0x00,0x01,0x0E,0x1A,0xCD,0x7D,0xF3,0x21,0x01,0x00,0x22,0x5B,0xC0,0x21,
	0x00,0xC0,0x11,0x4D,0xC0,0x0E,0x27,0xCD,0x7D,0xF3,0xC3,0x00,0x01,0x00,0x42,0x4F,
	0x4F,0x54,0x44,0x49,0x53,0x4B,0x43,0x4F,0x4D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	// 0xEB,0xFE,0x90,0x42,0x4F,0x4F,0x54,0x44,0x49,0x53,0x4B,0x00,0x02,0x02,0x01,0x00,
	// 0x02,0x70,0x00,0xA0,0x05,0xF9,0x03,0x00,0x09,0x00,0x02,0x00,0x00,0x00,0xD0,0x31,
	// 0x1F,0xF5,0x11,0x4A,0xC0,0x0E,0x0F,0xCD,0x7D,0xF3,0x3C,0xCA,0x1E,0xC0,0x11,0x00,
	// 0x01,0x0E,0x1A,0xCD,0x7D,0xF3,0x21,0x01,0x00,0x22,0x58,0xC0,0x21,0x00,0xC0,0x11,
	// 0x4A,0xC0,0x0E,0x27,0xCD,0x7D,0xF3,0xC3,0x00,0x01,0x00,0x42,0x4F,0x4F,0x54,0x44,
	// 0x49,0x53,0x4B,0x43,0x4F,0x4D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

byte* DefaultBootBlock = Dos2BootBlock;

/* functions to change DirEntries */
/* #define setsh(x,y) {x[0]=y;x[1]=y>>8;} */
void setsh(byte* x,int y) 
{
	x[0] = (byte)(y & 255);
	x[1] = (byte)((y>>8)&255);
}
/* #define setlg(x,y) {x[0]=y;x[1]=y>>8;x[2]=y>>16;x[3]=y>>24;} */
void setlg(byte* x,int y)
{
	x[0] = (byte)(y & 255);
	x[1] = (byte)((y>>8)&255);
	x[2] = (byte)((y>>16)&255);
	x[3] = (byte)((y>>24)&255);
}

/* functions to read DirEntries */
/* #define rdsh(x) (x[0]+(x[1]<<8)) */
word rdsh(byte* x)
{
	return  (x[0]+(x[1]<<8));
}

/* #define rdlg(x) (x[0]+(x[1]<<8)+(x[2]<<16)+(x[3]<<24)) */
int rdlg(byte* x)
{
	return (x[0]+(x[1]<<8)+(x[2]<<16)+(x[3]<<24));
}

/** Transforms a clusternumber towards the first sector of this cluster
  * The calculation uses info read fom the bootsector
  */
int clusterToSector(int cluster)
{
	return 1+RootDirEnd + sectorsPerCluster*( cluster - 2 );
}

/** Transforms a sectornumber towards it containing cluster
  * The calculation uses info read fom the bootsector
  */
word sectorToCluster(int sector)
{
	return 2+(int)((sector-(1+RootDirEnd))/sectorsPerCluster);
}

/** Initialize global variables by reading info from the bootsector
  */
void readBootSector()
{
	struct MSXBootSector *boot = (struct MSXBootSector*)FSImage;

	nbSectors = rdsh(boot->nrsectors); // asume a DS disk is used
	SECTOR_SIZE = rdsh(boot->bpsector);
	sectorsPerTrack = rdsh(boot->nrsectors);
	nbSides = rdsh(boot->nrsides);
	nbFats = (byte)boot->nrfats[0];
	sectorsPerFat = rdsh(boot->sectorsfat);
	nbRootDirSectors = rdsh(boot->direntries)/(SECTOR_SIZE/32);
	sectorsPerCluster = (int)(byte)boot->spcluster[0] ;

	RootDirStart = 1 + nbFats*sectorsPerFat;
	MSXchrootSector = RootDirStart;

	RootDirEnd = RootDirStart+nbRootDirSectors-1;
	maxCluster = sectorToCluster(nbSectors);
	
	if (show_bootinfo)
	{
		cout <<"---------- Boot sector info -----"<<endl<<endl;
		cout <<"  bytes per sector:      "<< rdsh(boot->bpsector) <<endl;
		cout <<"  sectors per cluster:   "<< (int)(byte)boot->spcluster[0] <<endl;
		cout <<"  number of FAT's:       "<< (int)(byte)boot->nrfats[0] <<endl;
		cout <<"  direntries in rootdir: "<< rdsh(boot->direntries) <<endl;
		cout <<"  sectors on disk:       "<< rdsh(boot->nrsectors) <<endl;
		cout <<"  media descriptor:      "<< std::hex<<(int)boot->descriptor[0]<<std::dec <<endl;
		cout <<"  sectors per FAT:       "<< rdsh(boot->sectorsfat) <<endl;
		cout <<"  sectors per track:     "<< rdsh(boot->sectorstrack) <<endl;
		cout <<"  number of sides:       "<< rdsh(boot->nrsides) <<endl;
		cout <<endl<<"Calculated values"<<endl<<endl;
		cout <<"maxCluster   "<<maxCluster <<endl;
		cout <<"RootDirStart "<<RootDirStart <<endl;
		cout <<"RootDirEnd   "<<RootDirEnd <<endl;
		cout <<"---------------------------------"<< endl<<endl;
	}
}

/** Create a correct bootsector depending on the required size of the filesystem
  * Will implecitely call readbootsector for global var initialising
  */
void setBootSector(word nbSectors)
{
	// variables set to single sided disk by default
	word nbSides = 1;
	byte nbFats = 2;
	byte nbReservedSectors = 1; // Just copied from a 32MB IDE partition
	byte nbSectorsPerFat = 2;
	byte nbHiddenSectors = 1;
	word nbDirEntry = 112;
	byte descriptor = 0xf8;

	// now set correct info according to size of image (in sectors!)
	// and using the same layout as used by Jon in IDEFDISK v 3.1
	if (nbSectors >= 32733 )
	{
		nbSides = 2;		/*unknow yet */
		nbFats = 2;		/*unknow yet */
		nbSectorsPerFat = 12;	/*copied from a partition from an IDE HD */
		nbSectorsPerCluster = 16;
		nbDirEntry = 256;
		nbSides = 32;		/*copied from a partition from an IDE HD */
		nbHiddenSectors = 16;
		descriptor = 0xf0;
	}
	else if (nbSectors >= 16389 )
	{
		nbSides = 2;		/*unknow yet */
		nbFats = 2;		/*unknow yet */
		nbSectorsPerFat = 3;	/*unknow yet */
		nbSectorsPerCluster = 8;
		nbDirEntry = 256;
		descriptor = 0xf0;
	}
	else if (nbSectors >= 8213 )
	{
		nbSides = 2;		/*unknow yet */
		nbFats = 2;		/*unknow yet */
		nbSectorsPerFat = 3;	/*unknow yet */
		nbSectorsPerCluster = 4;
		nbDirEntry = 256;
		descriptor = 0xf0;
	}
	else if (nbSectors >= 4127 )
	{
		nbSides = 2;		/*unknow yet */
		nbFats=2;		/*unknow yet */
		nbSectorsPerFat=3;	/*unknow yet */
		nbSectorsPerCluster=2;
		nbDirEntry=256;
		descriptor=0xf0;
	}
	else if (nbSectors >= 2880 )
	{
		nbSides=2;		/*unknow yet */
		nbFats=2;		/*unknow yet */
		nbSectorsPerFat=3;	/*unknow yet */
		nbSectorsPerCluster=1;
		nbDirEntry=224;
		descriptor=0xf0;
	}
	else if (nbSectors >= 1441 )
	{
		nbSides=2;		/*unknow yet */
		nbFats=2;		/*unknow yet */
		nbSectorsPerFat=3;	/*unknow yet */
		nbSectorsPerCluster=2;
		nbDirEntry=112;
		descriptor=0xf0;
	}
	else if (nbSectors <= 720 )
	{
		// normal single sided disk
		nbSectors = 720 ;
	}
	else
	{
		// normal double sided disk
		nbSectors = 1440 ;
		nbSides=2;
		nbFats=2;
		nbSectorsPerFat=3;
		nbSectorsPerCluster=2;
		nbDirEntry=112;
		descriptor=0xf9;
	}
	struct MSXBootSector *boot=(struct MSXBootSector*)FSImage;

	setsh(boot->nrsectors,nbSectors);
	setsh(boot->nrsides,nbSides);
	boot->spcluster[0]=(byte)nbSectorsPerCluster;
	boot->nrfats[0]=nbFats;
	setsh(boot->sectorsfat,nbSectorsPerFat);
	setsh(boot->direntries,nbDirEntry);
	boot->descriptor[0]=descriptor;
	setsh(boot->reservedsectors,nbReservedSectors);
	setsh(boot->hiddensectors,nbHiddenSectors);

	readBootSector();
}

/** Get the next clusternumber from the FAT chain
  */
word ReadFAT(word clnr)
{
//	PRT_DEBUG("ReadFAT(cnlr= "<<clnr<<");");
	if (!do_fat16)
	{
		byte *P = FSImage+SECTOR_SIZE + (clnr * 3) / 2;
//		PRT_DEBUG("returning "<< ( (clnr & 1) ?  (P[0] >> 4) + (P[1] << 4) : P[0] + ((P[1] & 0x0F) << 8)) );
		return (clnr & 1) ?
		(P[0] >> 4) + (P[1] << 4) :
		P[0] + ((P[1] & 0x0F) << 8);
	}
	else
	{
		cout << "FAT size 16 not yet tested!!"<<endl;
		byte *P = FSImage+SECTOR_SIZE + (clnr * 2) ;
		return P[0] + (P[1] << 8);
	}
}

/** Write an entry to the FAT
  */
void WriteFAT(word clnr, word val)
{
	if (!do_fat16)
	{
		PRT_DEBUG("WriteFAT(cnlr= "<<clnr<<",val= "<<val<<");");
		byte* P=FSImage+SECTOR_SIZE + (clnr * 3) / 2;
		if (clnr & 1)
		{ 
			P[0] = (P[0] & 0x0F) + (val << 4);
			P[1] = val >> 4;
		}
		else
		{
			P[0] = val;
			P[1] = (P[1] & 0xF0) + ((val >> 8) & 0x0F);
		}
	}
	else
	{
		cout << "FAT size 16 not yet tested!!"<<endl;
		byte *P = FSImage+SECTOR_SIZE + (clnr * 2) ;
		P[0] = val & 0xFF;
		P[1] = (val >> 8) & 0xFF;
	}
}

/** Find the next clusternumber marked as free in the FAT
  */
word findFirstFreeCluster(void)
{
	int cluster=2;
	while ((cluster <= maxCluster) && ReadFAT(cluster))
	{
		cluster++;
	};
	return cluster;
}

void mkdir_ex(const char *name,mode_t perm)
{
#ifdef __WIN32__
	mkdir(name);
#else
	mkdir(name,perm);
#endif
}


/** Returns the index of a free (or with deleted file) entry
  * in the current dir sector
  * Out: index (is max 15)m if no index is found then 16 is returned
  */
byte findUsableIndexInSector(int sector)
{
	// find a not used (0x00) or delete entry (0xe5)
	byte* P=FSImage+SECTOR_SIZE*sector;
	byte i=0;
	for (; i<NUMOFENT && P[0]!=0x00 && P[0]!=0xe5; i++,P+=32);
	return i;
}

/** Get the next sector from a file or (sub)directory
  * If no next sector then 0 is returned
  */
int getNextSector(int sector)
{
	//if this sector is part of the root directory...
	if (sector==RootDirEnd) { return 0; }
	if (sector<RootDirEnd) { return sector + 1;}

	if ((nbSectorsPerCluster > 1) && ( sectorToCluster(sector) == sectorToCluster(sector+1)) )
	{
		return sector+1;
	}
	else
	{
		int nextcl=ReadFAT(sectorToCluster(sector));
		if (nextcl == EOF_FAT)
		{
			return 0;
		}
		else
		{
			return clusterToSector(nextcl);
		}
	}
}

/** if there are no more free entries in a subdirectory, the subdir is
  * expanded with an extra cluster, This function gets the free cluster,
  * clears it and updates the fat for the subdir
  * returns: the first sector in the newly appended cluster, or 0 in case of error
  */
int appendClusterToSubdir(int sector)
{
	word curcl=sectorToCluster(sector);
	if ( ReadFAT(curcl) != EOF_FAT)
	{
		CRITICAL_ERROR("appendClusterToSubdir called with sector in a not EOF_FAT cluster ");
	}
	word nextcl=findFirstFreeCluster();
	if (nextcl > maxCluster)
	{
		cout << "Disk full no more free clusters" << endl;
		return 0;
	}
	int logicalSector=clusterToSector(nextcl);
	//clear this cluster
	memset(FSImage+SECTOR_SIZE*logicalSector,0,SECTOR_SIZE*sectorsPerCluster);
	WriteFAT(curcl, nextcl);
	WriteFAT(nextcl, EOF_FAT);
	return logicalSector;
}

/** Find the dir entry for 'name' in subdir starting at the given 'sector'
  * with given 'index'
  * returns: a pointer to a MSXDirEntry struct if name was found
  *          a NULL pointer if no match was found
  */
struct MSXDirEntry* findEntryInDir(string name,int sector,byte direntryindex)
{
	byte* P=FSImage+SECTOR_SIZE*sector+32*direntryindex;
	byte i=0;
	do {
		for (i=0; i<NUMOFENT && strncmp(name.c_str(),(char*)P,11)!=0; i++,P+=32);
		if (i==NUMOFENT)
		{
			sector=getNextSector(sector);
			P=FSImage+SECTOR_SIZE*sector;
		};
		
	} while (i>NUMOFENT-1 && sector);
	return sector?(MSXDirEntry*)P:NULL ;
}

/** This function returns the sector and dirindex for a new directory entry
  * if needed the involved subdirectroy is expanded by an extra cluster
  * returns: a struct physDirEntry containing sector and index
  *          if failed then the sectornumber is 0
  */
struct physDirEntry addEntryToDir(int sector,byte direntryindex)
{
	//this routin adds the msxname to a directory sector, if needed (and possible) the directory is extened with an extra cluster
	struct physDirEntry newEntry;
	byte newindex=findUsableIndexInSector(sector);
	if (sector<=RootDirEnd)
	{
		// we are adding this to the root sector
		while (newindex>NUMOFENT-1 && sector<=RootDirEnd)
		{
			newindex=findUsableIndexInSector(++sector);
		}
		newEntry.sector=sector;
		newEntry.index=newindex;
	}
	else
	{
		// we are adding this to a subdir
		if  (newindex<NUMOFENT)
		{
			newEntry.sector=sector;
			newEntry.index=newindex;
		}
		else
		{
			int nextsector;
			while (newindex>NUMOFENT-1 && sector!=0 )
			{
				nextsector=getNextSector(sector);
				if (nextsector==0)
				{
					nextsector=appendClusterToSubdir(sector);
					cout <<"appendClusterToSubdir("<<sector<<") returns"<<nextsector <<endl;
					if (nextsector==0)
					{
						CRITICAL_ERROR("disk is full");
					}
				}
				sector=nextsector;
				newindex=findUsableIndexInSector(sector);
			};
			newEntry.sector=sector;
			newEntry.index=newindex;
			//cout <<"FATAL: subdir needs more then 16 entries"<<endl;
			//exit(1);
		}
	};
	return newEntry;
}

// create an MSX filename 8.3 format, if needed in vfat like abreviation
static char toMSXChr(char a)
{
	a = toupper(a);
	if (a == ' ' || a == '.')
	{
		a = '_';
	}
	return a;
}

/** Transform a long hostname in a 8.3 uppercase filename as used in the
 * direntries on an MSX
 */
string makeSimpleMSXFileName(const string& fullfilename)
{
	string::size_type pos = fullfilename.find_last_of('/');

	if (pos == string::npos)
		pos = fullfilename.find_last_of('\\'); // for DOS user :)

	string tmp;
	if (pos != string::npos)
	{
		tmp = fullfilename.substr(pos + 1);
	}
	else
	{
		tmp = fullfilename;
	}
	//PRT_DEBUG("filename before transform " << tmp);
	//PRT_DEBUG("filename after transform " << tmp);

	string file, ext;
	pos = tmp.find_last_of('.');
	if (pos != string::npos)
	{
		file = tmp.substr(0, pos);
		ext  = tmp.substr(pos + 1);
	}
	else
	{
		file = tmp;
		ext = "";
	}
	//remove trailing spaces
	while (file.find_last_of(' ')==(file.size()-1) && file.find_last_of(' ') != string::npos)
	{
		file=file.substr(0,file.size() -1 );
		cout << "shrinking file *"<<file<<"*"<<endl;
	};
	while (ext.find_last_of(' ')==(ext.size()-1) && ext.find_last_of(' ') != string::npos )
	{
		ext=ext.substr(0,ext.size() -1 );
		cout << "shrinking ext*"<<ext<<"*"<<endl;
	};
	// put in major case and create '_' if needed
	transform(file.begin(), file.end(), file.begin(), toMSXChr);
	transform(ext.begin(), ext.end(), ext.begin(), toMSXChr);

	PRT_DEBUG("adding correct amount of spaces");
	file += "        ";
	ext  += "   ";
	file = file.substr(0, 8);
	ext  = ext.substr(0, 3);

	return file + ext;
}

/** This function creates a new MSX subdir with given date 'd' and time 't'
  * in the subdir pointed at by 'sector' and 'direntryindex' in the newly
  * created subdir the entries for '.' and '..' are created
  * returns: the first sector of the new subdir
  *          0 in case no directory could be created
  */
int AddMSXSubdir(string msxName,int t,int d,int sector,byte direntryindex)
{
	// returns the sector for the first cluster of this subdir
	struct physDirEntry result;
	result=addEntryToDir(sector,direntryindex);
	if (result.index > NUMOFENT-1)
	{
		cout << "couldn't add entry"<<msxName <<endl;
		return 0;
	}
	struct MSXDirEntry* direntry =(MSXDirEntry*)(FSImage+SECTOR_SIZE*result.sector+32*result.index);
	direntry->attrib=T_MSX_DIR;
	setsh(direntry->time, t);
	setsh(direntry->date, d);
	memcpy(direntry,makeSimpleMSXFileName(msxName).c_str(),11);

	//direntry->filesize=fsize;
	word curcl = 2;
	curcl=findFirstFreeCluster();
	PRT_DEBUG("New subdir starting at cluster " << curcl );
	setsh(direntry->startcluster, curcl);
	WriteFAT(curcl, EOF_FAT);
	int logicalSector=clusterToSector(curcl);
	//clear this cluster
	memset(FSImage+SECTOR_SIZE*logicalSector,0,SECTOR_SIZE*sectorsPerCluster);
	// now add the '.' and '..' entries!!
	direntry =(MSXDirEntry*)(FSImage+SECTOR_SIZE*logicalSector);
	memset(direntry, 0, sizeof(struct MSXDirEntry));
	memset(direntry, 0x20, 11); //all spaces
	memset(direntry, '.', 1); 
	direntry->attrib=T_MSX_DIR;
	setsh(direntry->time, t);
	setsh(direntry->date, d);
	setsh(direntry->startcluster, curcl);

	direntry++;
	memset(direntry, 0, sizeof(struct MSXDirEntry));
	memset(direntry, 0x20, 11); //all spaces
	memset(direntry, '.', 2); 
	direntry->attrib=T_MSX_DIR;
	setsh(direntry->time, t);
	setsh(direntry->date, d);

	int parentCluster = sectorToCluster(sector);
	if (sector == RootDirStart) parentCluster=0;

	setsh(direntry->startcluster, parentCluster);

	return logicalSector;
}

void makeFATTime(const struct tm mtim,int *dt)
{
	dt[0] = (mtim.tm_sec >> 1) + (mtim.tm_min << 5) + (mtim.tm_hour << 11);
	dt[1] = mtim.tm_mday + ((mtim.tm_mon + 1) << 5) + ((mtim.tm_year + 1900 - 1980) << 9);
}



/** Add an MSXsubdir with the time properties from the HOST-OS subdir
 */
int AddSubdirtoDSK(string hostName,string msxName,int sector,byte direntryindex)
{
	//struct MSXDirEntry* direntry =(MSXDirEntry*)(FSImage+SECTOR_SIZE*result.sector+32*result.index);

	// compute time/date stamps
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));
	stat(hostName.c_str(), &fst);
	struct tm mtim = *localtime(&(fst.st_mtime));

	int td[2];

	makeFATTime(mtim,td);

	int logicalSector=AddMSXSubdir(msxName,td[0],td[1],sector,direntryindex);
	return logicalSector;
}

/** This file alters the filecontent of agiven file
  * It only changes the file content (and the filesize in the msxdirentry)
  * It doesn't changes timestamps nor filename, filetype etc.
  * Output: nothing usefull yet
  */
void AlterFileInDSK(struct MSXDirEntry* msxdirentry, string hostName)
{
	bool needsNew=false;
	int fsize;
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));
	stat(hostName.c_str(), &fst);
	fsize = fst.st_size;

	PRT_DEBUG("AlterFileInDSK: Filesize " << fsize);

	word curcl =rdsh(msxdirentry->startcluster) ;
	// if entry first used then no cluster assigned yet
	if (curcl==0)
	{
		curcl=findFirstFreeCluster();
		setsh(msxdirentry->startcluster, curcl);
		WriteFAT(curcl, EOF_FAT); 
		needsNew=true;
	}
	PRT_DEBUG("AlterFileInDSK: Starting at cluster " << curcl );

	int size = fsize;
	int prevcl = 0;
	//open file for reading
	FILE* file = fopen(hostName.c_str(),"rb");

	while (file && size && (curcl <= maxCluster))
	{
		int logicalSector= clusterToSector(curcl);
		byte* buf=FSImage+logicalSector*SECTOR_SIZE;
		for (int j=0 ; j < sectorsPerCluster ; j++)
		{
			if (size)
			{
				PRT_DEBUG("AlterFileInDSK: relative sector "<<j<<" in cluster "<<curcl);
				// more poitical correct:
				//We should read 'size'-bytes instead of 'SECTOR_SIZE' if it is the end of the file
				fread(buf,1,SECTOR_SIZE,file);
				buf+=SECTOR_SIZE;
			}
			size -= (size > SECTOR_SIZE  ? SECTOR_SIZE  : size);
		}

		if (prevcl) 
		{
			WriteFAT(prevcl, curcl);
		}
		prevcl = curcl;
		//now we check if we continue in the current clusterstring or need to allocate extra unused blocks
		//do {
		if (needsNew)
		{
			//need extra space for this file
			WriteFAT(curcl, EOF_FAT); 
			curcl=findFirstFreeCluster();
		}
		else
		{
			// follow cluster-Fat-stream
			curcl=ReadFAT(curcl);
			if (curcl == EOF_FAT)
			{
				curcl=findFirstFreeCluster();
				needsNew=true;
			}
		}
		//} while((curcl <= maxCluster) && ReadFAT(curcl));
		PRT_DEBUG("AlterFileInDSK: Continuing at cluster " << curcl);
	}
	if (file) fclose(file);

	if ((size == 0) && (curcl <= maxCluster))
	{
		// TODO: check what an MSX does with filesize zero and fat allocation;
		if(prevcl==0)
		{
			prevcl=curcl;
			curcl=ReadFAT(curcl);
		};
		WriteFAT(prevcl, EOF_FAT); 
		PRT_DEBUG("AlterFileInDSK: Ending at cluster " << prevcl);
		//cleaning rest of FAT chain if needed
		while (!needsNew)
		{
			prevcl=curcl;
			if (curcl != EOF_FAT)
			{
				curcl=ReadFAT(curcl);
			}
			else
			{
				needsNew=true;
			}
			PRT_DEBUG("AlterFileInDSK: Cleaning cluster " << prevcl << " from FAT");
			WriteFAT(prevcl, 0);
		}
		
	}
	else
	{
		//TODO: don't we need a EOF_FAT in this case as well ?
		// find out and adjust code here
		cout <<"Fake Diskimage full: "<< hostName <<" truncated.";
	}
	//write (possibly truncated) file size
	setlg(msxdirentry->size,fsize-size);

}


/** Add file to the MSX disk in the subdir pointed to by 'sector'
  * returns: nothing usefull yet :-)
  */
void AddFiletoDSK(string hostName,string msxName,int sector,byte direntryindex)
{
	//first find out if the filename already exists current dir
	struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(msxName),sector,direntryindex); 
	if (msxdirentry!=NULL)
	{
		PRT_VERBOSE("Preserving entry "<<msxName );
		return;
	};
	struct physDirEntry result;
	result=addEntryToDir(sector,direntryindex);
	if (result.index > NUMOFENT-1)
	{
		cout << "couldn't add entry"<<hostName <<endl;
		return;
	}
	struct MSXDirEntry* direntry =(MSXDirEntry*)(FSImage+SECTOR_SIZE*result.sector+32*result.index);
	direntry->attrib=T_MSX_REG;

	setsh(direntry->startcluster,0);

	PRT_VERBOSE(hostName<<" \t-> \""<<makeSimpleMSXFileName(msxName) << '"' );
	memcpy(direntry,makeSimpleMSXFileName(msxName).c_str(),11);

	// compute time/date stamps
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));
	stat(hostName.c_str(), &fst);
	struct tm mtim = *localtime(&(fst.st_mtime));
	int td[2];

	makeFATTime(mtim,td);
	setsh(direntry->time, td[0]);
	setsh(direntry->date, td[1]);

	AlterFileInDSK(direntry,hostName);
}

int check_stat(const string &name)
{
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));
	stat(name.c_str(), &fst);

	if (fst.st_mode & S_IFDIR)
		return 0; // it's a directory

	return 1; // if it's a file
}


/** transfer directory and all its subdirectories to the MSX diskimage
  */
void recurseDirFill(const string &DirName,int sector,int direntryindex)
{
	int result;

	PRT_DEBUG("Trying to read directory "<<DirName);

	DIR* dir = opendir(DirName.c_str());

	if (dir == NULL ) {
		PRT_DEBUG("Not a FDC_DirAsDSK image");
		//throw MSXException("Not a directory");
	}
	//read directory and fill the fake disk
	struct dirent* d = readdir(dir);
	while (d)
	{
		string name(d->d_name);
		PRT_DEBUG("reading name in dir :" << name);
		if ( check_stat(DirName + '/' + name) )
		{ // true if a file
			if (!name.empty() && name[0] != '.')
				AddFiletoDSK(DirName + '/' + name,name,sector,direntryindex); // used here to add file into fake dsk
			else
				cout << name << ": ignored file which first charactor is \".\"" << endl;
		}
		else if (name != string(".") && name != string("..") )
		{
			if (do_subdirs)
			{
				PRT_VERBOSE(DirName + '/' + name<<" \t-> \""<<makeSimpleMSXFileName(name) << '"' );
				struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(name),sector,direntryindex); 
				if (msxdirentry!=NULL)
				{
					PRT_VERBOSE("Dir entry "<<name <<" exists already ");
					result= clusterToSector(rdsh(msxdirentry->startcluster));
				}
				else
				{
					PRT_VERBOSE("Adding dir entry "<<name);
					result=AddSubdirtoDSK(DirName + '/' + name,name,sector,direntryindex); // used here to add file into fake dsk
				}

				recurseDirFill( DirName + '/' + name, result,0);
			}
			else
			{
				PRT_DEBUG("Skipping subdir:"<< DirName + "/" + name);
			}
		};
		d = readdir(dir);
	}
	closedir(dir);
}

/** Save the diskimage from memory to disk
  */
void writeImageToDisk(string filename)
{
	if (do_test)
	{
		PRT_VERBOSE("msxtar doesn't write to disk for test");
		return; // test is not to write.
	}

	FILE* file = fopen(filename.c_str(), "wb");
	if (file)
	{
		if (sizeOfDskFile)
			fwrite(dskImage, 1, sizeOfDskFile, file);
		else
			fwrite(dskImage, 1, nbSectors*SECTOR_SIZE, file);
		fclose(file);
	}
	else
	{
		cout << "Couldn't open file for writing!";
	}
}

void updatecreateDSK(const string fileName)
{
	int result;

	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));

	PRT_DEBUG("trying to stat : " << fileName );
	stat(fileName.c_str(), &fst);
	if (fst.st_mode & S_IFDIR )
	{
		// this should be a directory
		if (do_flat || !do_subdirs)
		{
			// put files in the directory to root
			recurseDirFill(fileName,MSXchrootSector,MSXchrootStartIndex);
		}
		else
		{
			PRT_VERBOSE("./" + fileName<<" \t-> \""<<makeSimpleMSXFileName(fileName) << '"' );
			struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(fileName),
							MSXchrootSector,MSXchrootStartIndex); 

			if (msxdirentry!=NULL)
			{
				PRT_VERBOSE("Dir entry "<<fileName <<" exists already ");
				result= clusterToSector(rdsh(msxdirentry->startcluster));
			}
			else
			{
				PRT_VERBOSE("Adding dir entry "<<fileName);
				result=AddSubdirtoDSK(fileName,fileName,MSXchrootSector,MSXchrootStartIndex);
				// used here to add file into fake dsk
			}

			recurseDirFill( fileName, result,0);
		}

	}
	else
	{
		// this should be a normal file
		PRT_VERBOSE("Updating file "<<fileName);
		//AddFiletoDSK(fileName,fileName,MSXchrootSector,MSXchrootStartIndex); // used here to add file into fake dsk in rootdir!!
		//first find out if the filename already exists current dir
		struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(fileName),MSXchrootSector,MSXchrootStartIndex);
		AlterFileInDSK(msxdirentry,fileName);
	};

}


void addcreateDSK(const string fileName)
{
	int result;

	// Here we create the fake diskimages based upon the files that can be
	// found in the 'fileName' directory or the single file
	//PRT_DEBUG("Trying FDC_DirAsDSK image");
	PRT_DEBUG("addcreateDSK( "<<fileName<<");");
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));

	PRT_DEBUG("addcreateDSK: trying to stat : " << fileName );
	stat(fileName.c_str(), &fst);
	if (fst.st_mode & S_IFDIR )
	{
		// this should be a directory
		PRT_VERBOSE("addcreateDSK: Adding directory "<<fileName);

		if (do_flat || !do_subdirs)
		{
			// put files in the directory to root
			MSXrootdir=fileName;
			recurseDirFill(fileName,MSXchrootSector,MSXchrootStartIndex);
		}
		else
		{
			PRT_VERBOSE("./" + fileName<<" \t-> \""<<makeSimpleMSXFileName(fileName) << '"' );
			struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(fileName), MSXchrootSector,MSXchrootStartIndex); 

			if (msxdirentry!=NULL)
			{
				PRT_VERBOSE("Dir entry "<<fileName <<" exists already ");
				result= clusterToSector(rdsh(msxdirentry->startcluster));
			}
			else
			{
				PRT_VERBOSE("Adding dir entry "<<fileName);
				result=AddSubdirtoDSK(fileName,fileName,MSXchrootSector,MSXchrootStartIndex);
				// used here to add file into fake dsk
			}
			recurseDirFill( fileName, result,0);
		}
	}
	else
	{
		// this should be a normal file
		PRT_DEBUG("addcreateDSK: Adding file "<<fileName);
		PRT_VERBOSE("Adding file "<<fileName);
		AddFiletoDSK(fileName,fileName,MSXchrootSector,MSXchrootStartIndex); // used here to add file into fake dsk in rootdir!!
	};
}

void updateInDSK(string name)
{
	// delete last charactor in the filename if it's a charactor to divide.

	if (name.length() > 0) {
		unsigned char ch = *(name.end()-1);
		if (ch == '/' && ch == '\\') 
			name.erase(name.length()-1);
		// Erased last charactor because it's a kind of slash :)
	}

	//first find the filename in the current 'root dir'
	struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(name),RootDirStart,0);

	if (msxdirentry==NULL)
	{
		PRT_VERBOSE("Couldn't find entry "<<name<<" to update, trying to create new entry");
		addcreateDSK(name);
	}
	else
	{
		if (keep_option)
		{
			PRT_VERBOSE("Preserving entry "<<name );
		}
		else
		{
			//PRT_VERBOSE("Updating entry not yet implemented "<<name );
			updatecreateDSK(name);
		}
	}
}

/** Create an empty diskimage with correct bootsector,FAT etc.
  */
void createEmptyDSK()
{
	// First create structure for the fake disk
	// Allocate dskImage in memmory
	dskImage=new byte[nbSectors*SECTOR_SIZE];
	FSImage=dskImage;
	if ( dskImage == NULL )
	{
		PRT_DEBUG("Not enough memory for disk image");
		// throw MSXException("No sufficient memory available");
	}

	// Assign default boot disk to this instance
	// give extra info on the bootsector
	// and get global parameters from them (implicit readBootSector)
	memcpy(FSImage, DefaultBootBlock, SECTOR_SIZE);
	setBootSector(nbSectors);

	// Assign default empty values to disk
	memset( FSImage+SECTOR_SIZE, 0x00, RootDirEnd*SECTOR_SIZE);
	memset( FSImage+((1+RootDirEnd)*SECTOR_SIZE), 0xE5, (nbSectors-(1+RootDirEnd))*SECTOR_SIZE);
	// for some reason the first 3bytes are used to indicate the end of a cluster, making the first available cluster nr 2
	// some sources say that this indicates the disk fromat and FAT[0]should 0xF7 for single sided disk, and 0xF9 for double sided disks
	// TODO: check this :-)
	// for now I simply repeat the media descriptor here
	{
		struct MSXBootSector *boot=(struct MSXBootSector*)FSImage;
		FSImage[SECTOR_SIZE+0] = boot->descriptor[0];
	}
	FSImage[SECTOR_SIZE+1] = 0xFF;
	FSImage[SECTOR_SIZE+2] = 0xFF;
}

string condensName(struct MSXDirEntry* direntry)
{
	char condensedname[13];
	int i;
	char *P=condensedname;
	for (i=0; i<8; i++)
	{
		if (direntry->filename[i] == ' ')
		{
			i=9;
		}
		else
		{
			*(P++)=tolower(direntry->filename[i]);
		}
	}
	if (direntry->ext[0] != ' ' || direntry->ext[1] != ' ' || direntry->ext[2] != ' ' )
	{
		*(P++)='.';
		for (i=0; i<3; i++)
		{
			*P=tolower(direntry->ext[i]);
			if (*P==' ')*P=(char)0;
			P++;
		}
	};
	*P=(char)0;
	return string(condensedname);
}

/**routine to make FSImage point to the correct part of dskImage
  * returns: true if succesfull, false if partition isn't valid
  */
bool chpart(int chpartition)
{
	int surf,sec,ssize,scyl;
	if (strncmp((char*)dskImage,"T98HDDIMAGE.R0",14)==0)
	{
		// 0x110 size of the header(long),cylinder(long),
		// surface(word),sector(word),secsize(word)
		cout << "T98header recognized" << endl;
		surf = rdsh(dskImage+0x110+8);
		sec  = rdsh(dskImage+0x110+10);
		ssize = rdsh(dskImage+0x110+12);

		SECTOR_SIZE=ssize;

		struct pc98part *P98=(struct pc98part *)(dskImage+0x400+(chpartition*16));
		scyl = rdsh(P98->startcyl);

		FSImage=dskImage + 0x200 + (ssize*scyl*surf*sec);
		readBootSector();
		return true;
	}

	if (strncmp((char*)dskImage,"\353\376\220MSX_IDE ",11) != 0)
	{
		cout << "Not an idefdisk compitable 0 sector" << endl;
		return false;
	}
	struct partition *P=(struct partition *)(dskImage+14+(30-chpartition)*16);
	if (rdlg(P->start4) == 0)
	{
		return false;
	}
	FSImage=dskImage+SECTOR_SIZE*rdlg(P->start4);
	readBootSector();
	return true;
	//P->size4
}

/** Routine to get the first sector of a given dir name
  * input: correctly MSXformatted subdirectory name
  * returns: 0 if given directory not found
  */
int findStartSectorOfDir(string dirname)
{
	string work=dirname;
	string totalpath=string(".");
	int MSXDirSector=MSXchrootSector;
	int MSXDirStartIndex=MSXchrootStartIndex;
	//if (!msxdir_option){return;};
	while (!work.empty() )
	{
		//cout <<"chroot 0: work=" <<work <<endl;
		//remove (multiple)leading '/'
		while (work.find_first_of("/\\")==0)
		{
			work.erase(0,1);
			//cout <<"chroot 1: work=" <<work <<endl;
		}
		string firstpart;
		string::size_type pos = work.find_first_of("/\\");
		if ( pos != string::npos)
		{
			firstpart=work.substr(0,pos);
			work=work.substr(pos + 1);
		}
		else
		{
			firstpart=work;
			work.clear();
		};
		// find firstpart directory 
		string simple=makeSimpleMSXFileName(firstpart);
		struct MSXDirEntry* msxdirentry=findEntryInDir(simple,MSXDirSector,MSXDirStartIndex);
		if (msxdirentry == NULL)
		{
			PRT_VERBOSE("Couldn't find directory: "<<dirname);
			return 0;
		}
		else
		{
			MSXDirSector=clusterToSector(rdsh(msxdirentry->startcluster));
			MSXDirStartIndex=2;
			totalpath+="/"+firstpart;
			mkdir_ex(totalpath.c_str(),ACCESSPERMS);
		}
	}
	return MSXDirSector;
}


//routine to update the global vars: MSXchrootSector,MSXchrootStartIndex
void chroot()
{
	string work=MSXhostdir;

	//if (!msxdir_option){return;};
	while (!work.empty() )
	{
		cout <<"chroot 0: work=" <<work <<endl;
		//remove (multiple)leading '/'
		while (work.find_first_of('/')==0)
		{
			work.erase(0,1);
			cout <<"chroot 1: work=" <<work <<endl;
		}
		string firstpart;
		string::size_type pos = work.find_first_of('/');
		if ( pos != string::npos)
		{
			firstpart=work.substr(0,pos);
			work=work.substr(pos + 1);
		}
		else
		{
			firstpart=work;
			work.clear();
		};
		// find firstpart directory or create it
		string simple=makeSimpleMSXFileName(firstpart);
		struct MSXDirEntry* msxdirentry=findEntryInDir(simple,MSXchrootSector,MSXchrootStartIndex);
		if (msxdirentry == NULL)
		{
			//creat new subdir
			time_t now;
			time( &now );
			struct tm mtim = *localtime(&now);
			int td[2];
			makeFATTime(mtim,td);

			cout <<"Create subdir "<< endl;
			MSXchrootSector=AddMSXSubdir(simple,td[0],td[1],MSXchrootSector,MSXchrootStartIndex);
			MSXchrootStartIndex=2;
			if (MSXchrootSector == 0){exit(0);};
		}
		else
		{
			MSXchrootSector=clusterToSector(rdsh(msxdirentry->startcluster));
			MSXchrootStartIndex=2;
		}
	}
}


void makeTimeFromDE(struct tm *ptm,int *td)
{
	ptm->tm_sec=( td[0] & 0x1f ) << 1 ;
	ptm->tm_min= ( td[0] & 0x03e0) >> 5;
	ptm->tm_hour= ( td[0] & 0xf800) >> 11;
	ptm->tm_mday= ( td[1] & 0x1f);
	ptm->tm_mon= ( td[1] & 0x01e0) >> 5;
	ptm->tm_year= (( td[1] & 0xfe00) >> 9) + 80;

}

/** Set the entries from direntry to the timestamp of resultFile
  */
void changeTime(string resultFile,struct MSXDirEntry* direntry)
{
	if (touch_option)
	{
		return;
	};
	int td[2];

	td[0]=rdsh(direntry->time);
	td[1]=rdsh(direntry->date);

	struct tm mtim;
	struct utimbuf utim;

	makeTimeFromDE(&mtim,td);

	utim.actime=mktime(&mtim);
	utim.modtime=mktime(&mtim);
	utime(resultFile.c_str(), &utim);
}

void fileExtract(string resultFile,struct MSXDirEntry* direntry)
{
	long size=rdlg(direntry->size);
	long savesize;
	int sector=clusterToSector(rdsh(direntry->startcluster));
	byte *buf;

	FILE* file = fopen(resultFile.c_str(), "wb");
	if (file == NULL)
	{
		CRITICAL_ERROR("Couldn't open file for writing!");
	}
	while (size && sector)
	{
		buf=FSImage+SECTOR_SIZE*sector;
		savesize = (size > SECTOR_SIZE  ? SECTOR_SIZE  : size);
		fwrite(buf, 1, savesize, file);
		size -= savesize;
		sector=getNextSector(sector);
	}
	if (sector==0 && size != 0)
	{
		cout << "no more sectors for file but file not ended ???"<<endl;
	}
	fclose(file);
	// now change the access time
	changeTime(resultFile,direntry);
}

void recurseDirExtract(const string &DirName,int sector,int direntryindex)
{
	int td[2];
	tm mtim;
	char os_buf[256];
	char ts_buf[32];

	int i=direntryindex;
	do {
		struct MSXDirEntry* direntry =(MSXDirEntry*)((FSImage+SECTOR_SIZE*sector)+32*i);
		if (direntry->filename[0] != 0xe5 && direntry->filename[0] != 0x00 )
		{
			string filename=condensName(direntry);
			string fullname=filename;
			if (! DirName.empty())
			{
			  fullname=DirName+"/"+filename;
			}
			td[0]=rdsh(direntry->time);
			td[1]=rdsh(direntry->date);

			makeTimeFromDE(&mtim,td);

			sprintf(ts_buf,"%04d/%02d/%02d %02d:%02d:%02d",
				mtim.tm_year+1900,mtim.tm_mon,mtim.tm_mday,
				mtim.tm_hour,mtim.tm_min,mtim.tm_sec);

			if (direntry->attrib & T_MSX_DIR)
				sprintf(os_buf,"%-32s %s %12s",fullname.c_str(),ts_buf,"<dir>");
			else
				sprintf(os_buf,"%-32s %s %12d",fullname.c_str(),ts_buf,rdlg(direntry->size));

			PRT_VERBOSE(os_buf);

			if (do_extract && direntry->attrib != T_MSX_DIR)
			{
				fileExtract(fullname,direntry);
			}
			if (direntry->attrib == T_MSX_DIR)
			{
				mkdir_ex(fullname.c_str(),ACCESSPERMS);
				// now change the access time
				changeTime(fullname,direntry);
				recurseDirExtract(
				  fullname,
				  clusterToSector(rdsh(direntry->startcluster)),
				  2); //read subdir and skip entries for '.' and '..'
			}
		}
		i++;
		direntry++;
		if (i == NUMOFENT)
		{
			if (sector<=RootDirEnd)
			{
				sector++;
				if (sector>RootDirEnd)
				{
					sector=0;
				}
			}
			else 
			{
				sector=getNextSector(sector);
			}
			i=0;
			direntry =(MSXDirEntry*)(FSImage+SECTOR_SIZE*sector);
		}
	} while (sector != 0);
}

void readDSK(const string fileName)
{
	// First read the diskimage into memory

	int fsize;
	struct stat fst;
	memset(&fst, 0, sizeof(struct stat));

	PRT_DEBUG("trying to stat : " << fileName );
	stat(fileName.c_str(), &fst);
	fsize = fst.st_size;

	dskImage=(byte*)malloc(fsize);
	sizeOfDskFile = fsize;

	FSImage=dskImage;
	if (dskImage == NULL)
	{
		CRITICAL_ERROR("Fatal error: Not enough memory to read image!");
	}
	//open file for reading
	PRT_DEBUG("open file for reading : " << fileName );
	FILE* file = fopen(fileName.c_str(),"rb");
	if (file == NULL)
	{
		CRITICAL_ERROR("Couldn't open "<<fileName<<" for reading!");
	}
	fread(dskImage,1,fsize,file);

	//Assuming normal diskimage means reading bootsector
	if (!msxpart_option)
	{
		if (strncmp((char*)dskImage,"T98HDDIMAGE.R0",14)==0 || strncmp((char*)dskImage,"\353\376\220MSX_IDE ",11) == 0)
		{
			cout << "Please specify a partition to use!"<<endl;
			exit(1);
		}
		readBootSector();
	}
}

void doSpecifiedExtraction(void)
{
	if (optind < globalargc )
	{
		PRT_VERBOSE("Going to extract only specified files/directories");
		string work=string(globalargv[optind++]);
		string fullname=work;
		//remove (multiple)leading '/'
		while (work.find_first_of("/\\")==0)
		{
			work.erase(0,1);
		};
		int MSXDirSector=MSXchrootSector;
		int MSXDirStartIndex=MSXchrootStartIndex;
		//now resolv directory if needed
		string::size_type pos = work.find_last_of("/\\");
		if ( pos != string::npos)
		{
			string firstpart=work.substr(0,pos);
			work=work.substr(pos + 1);
			MSXDirSector=findStartSectorOfDir(firstpart);
			if (MSXDirSector == 0)
			{
				cout << "Couldn't find "<< work<<endl;
				return;
			};
		};

		struct MSXDirEntry* msxdirentry=findEntryInDir(makeSimpleMSXFileName(work),MSXDirSector,MSXDirStartIndex);
		if (msxdirentry == NULL){return;};
		if (do_extract && msxdirentry->attrib != T_MSX_DIR)
		{
			PRT_VERBOSE(fullname);
			fileExtract(fullname,msxdirentry);
		}
		if (msxdirentry->attrib == T_MSX_DIR)
		{
			recurseDirExtract(
			work,
			clusterToSector(rdsh(msxdirentry->startcluster)),
			2); //read subdir and skip entries for '.' and '..'
		}
	}
	else
	{
		recurseDirExtract(string(""),MSXchrootSector,MSXchrootStartIndex);
	}
}

void display_usage(void)
{
	printf ("\
`msxtar' saves many files together into a single disk image to be used by\n\
emulators like openMSX, and can restore individual files from the archive.\n\
This tool supports single-sided, double-sized and IDE HD images (only FAT12)\n");

	printf ("\nUsage: %s [OPTION]... [FILE]...\n\
\n\
Examples:\n\
  %s -cf disk.dsk foo bar  # Create a diskimage from files/directories foo and bar.\n\
  %s -tvf disk.dsk         # List all files in disk.dsk verbosely.\n\
  %s -xf disk.dsk          # Extract all files from disk.dsk.\n",
		program_name, program_name, program_name, program_name);

	printf("\
\n\
If a long option shows an argument as mandatory, then it is mandatory\n\
for the equivalent short option also.  Similarly for optional arguments.\n");

	printf("\
\n\
Main operation mode:\n\
  -t, --list              list the contents of an archive\n\
  -x, --extract, --get    extract files from an archive\n\
  -c, --create            create a new archive\n\
  -r, --append            append files to the end of an archive\n\
  -u, --update            only append files newer than copy in archive\n\
  -A, --catenate          append tar files to an archive\n\
      --concatenate       same as -A\n");

	printf("\
\n\
Handling of file attributes:\n\
      --owner=NAME             force NAME as owner for added files\n\
      --group=NAME             force NAME as group for added files\n\
      --mode=CHANGES           force (symbolic) mode CHANGES for added files\n\
  -k, --keep                   keep existing files, do not overwrite \n\
  -m, --modification-time      don't extract file modified time\n");

	printf("\
\n\
Image selection and switching:\n\
  -f, --file=ARCHIVE             use archive file or device ARCHIVE\n\
                                 default name is 'msxdiskimage.dsk'\n\
  -S, --size=ARCHSIZE            ARCHSIZE can be nnnn[S|B|K|M]\n\
                                 The folowing simple sizes are predefinied\n\
                                 'single' equals 360K, 'double' equals 720K\n\
                                 and 'ide' equals 32M\n\
  -1, --dos1                     use MSX-DOS1 bootsector and no subdirs\n\
  -2, --dos2                     use MSX-DOS2 bootsector and use subdirs\n\
  -0, --dos0                     use direct boot disk (MSX-DOS1 format)\n\
  -M, --msxdir=SUBDIR            place new files in SUBDIR in the image\n\
  -P, --partition=PART           Use partition PART when handling files\n\
                                 PART can be 'all' to handle all partitions");

/*
      printf ("\
\n\
Archive format selection:\n\
  -j, --bzip2                        filter the archive through bzip2\n\
  -z, --gzip, --ungzip               filter the archive through gzip\n\
  -Z, --compress, --uncompress       filter the archive through compress\n\
      --use-compress-program=PROG    filter through PROG (must accept -d)\n");
*/

	printf ("\
\n\
Informative output:\n\
      --help            print this help, then exit\n\
      --version         print tar program version number, then exit\n\
  -v, --verbose         verbosely list files processed\n");

	printf ("\n\n");
}


#define OPTION_STRING "12rAP:jzkmMcxf:xzwktzuS:v"

static struct option long_options[] =
{
	{"diskboot", no_argument, 0, '0'},
	{"dos0", no_argument, 0, '0'},
	{"dos1", no_argument, 0, '1'},
	{"dos2", no_argument, 0, '2'},
	{"append", no_argument, 0, 'r'},
	{"catenate", no_argument, 0, 'A'},
	{"concatenate", no_argument, 0, 'A'},
	{"bzip2", no_argument, 0, 'j'},
	{"confirmation", no_argument, 0, 'w'},
	{"create", no_argument, 0, 'c'},
	{"list", no_argument, 0, 't'},
	{"extract", no_argument, 0, 'x'},
	{"get", no_argument, 0, 'x'},
	{"file", required_argument, 0, 'f'},
	{"msxdir", required_argument, 0, 'M'},
	{"partition", required_argument, 0, 'P'},
	{"keep", no_argument, 0, 'k'},
	{"size", required_argument, 0, 'S'},
	{"gunzip", no_argument, 0, 'z'},
	{"gzip", no_argument, 0, 'z'},
	{"help", no_argument, &show_help, 1},
	{"interactive", no_argument, 0, 'w'},
	{"modification-time", no_argument, 0, 'm'},
	{"keep-old-files", no_argument, 0, 'k'},
	{"ungzip", no_argument, 0, 'z'},
	{"update", no_argument, 0, 'u'},
	{"verbose", no_argument, 0, 'v'},
	{"version", no_argument, &show_version, 1},
	{"fat16", no_argument, &do_fat16, 1},
	{"debug", no_argument, 0, DEBUG_OPTION},
	{"bootinfo", no_argument, &show_bootinfo, 1 },
	{0, 0, 0, 0}
};

int main(int argc, char **argv)
{
	// code copied literaly from GNU-tar.c
	// this code is to be able to handle a command like : 'tar cvf name'
	// this will be translated to 'tar -c -v -f name'
	//

	program_name=argv[0];

	/* Convert old-style tar call by exploding option element and rearranging
	options accordingly.  */

	if (argc > 1 && argv[1][0] != '-')
	{
		PRT_DEBUG("reconverting commandline options");
		int new_argc;		/* argc value for rearranged arguments */
		char **new_argv;		/* argv value for rearranged arguments */
		char *const *in;		/* cursor into original argv */
		char **out;		/* cursor into rearranged argv */
		const char *letter;	/* cursor into old option letters */
		char buffer[3];		/* constructed option buffer */
		const char *cursor;	/* cursor in OPTION_STRING */

		/* Initialize a constructed option.  */

		buffer[0] = '-';
		buffer[2] = '\0';

		/* Allocate a new argument array, and copy program name in it.  */

		new_argc = argc - 1 + strlen (argv[1]);
		new_argv = (char**)malloc ((new_argc + 1) * sizeof (char *));
		in = argv;
		out = new_argv;
		*out++ = *in++;

		/* Copy each old letter option as a separate option, and have the
		corresponding argument moved next to it.  */

		for (letter = *in++; *letter; letter++)
		{
			buffer[1] = *letter;
			*out++ = strdup (buffer);
			cursor = strchr (OPTION_STRING, *letter);
			if (cursor && cursor[1] == ':')
			{
			if (in < argv + argc)
				*out++ = *in++;
			else
				//this is the original line
				//USAGE_ERROR ((0, 0, _("Old option `%c' requires an argument."),
				//	      *letter));
				exit(1);
			}
		}

		/* Copy all remaining options.  */

		while (in < argv + argc)
		*out++ = *in++;
		*out = 0;

		/* Replace the old option list by the new one.  */

		argc = new_argc;
		argv = new_argv;
	}

	/* Parse all options and non-options as they appear.  */

	//quick hack need to clear this one later
	globalargc=argc;
	globalargv=argv;

	//input_files = 0;

	//prepend_default_options (getenv ("TAR_OPTIONS"), &argc, &argv);
	enum {
		CREATE_COMMAND,
		LIST_COMMAND,
		EXTRACT_COMMAND,
		UPDATE_COMMAND,
		APPEND_COMMAND
	};

	int maincommand=LIST_COMMAND;
	int optchar;
	// default settings
	nbSectors = 1440; // asume a DS disk is used
	sectorsPerCluster=2; //set default value
	sizeOfDskFile = 0;


	outputFile=string("diskimage.dsk");

	while (optchar = getopt_long (argc, argv, OPTION_STRING, long_options, 0), optchar != -1 && optchar != 1)
	{
		char *optx;

		optx=optarg;
		if (optx && *optx=='=') optx++;

		switch (optchar)
		{
		case DEBUG_OPTION:
			show_debug=1;
			cout << "----------------------------------------------------------" << endl;
			cout << "This debug mode is intended for people who want to check  " << endl;
			cout << "the dataflow within the msxtar program.                   " << endl;
			cout << "Consider this mode verry unpractical for normal usage :-) " << endl;
			cout << "----------------------------------------------------------" << endl;
			break;

		case '?':
			display_usage();

		case 0:
			break;

		case 1:
			/* File name or non-parsed option, because of RETURN_IN_ORDER
			ordering triggered by the leading dash in OPTION_STRING.  */
			inputFile=string(optx);
			break;

		case '0':
			DefaultBootBlock = Dos0BootBlock;
			do_subdirs = false;
			break;

		case '1':
			DefaultBootBlock = Dos1BootBlock;
			do_subdirs = false;
			break;

		case '2':
			DefaultBootBlock = Dos2BootBlock;
			do_subdirs = true;
			break;

		case 'c':
			maincommand = CREATE_COMMAND;
			break;

		case 'f':
			outputFile = string(optx);
			break;

		case 'j':
			//set_use_compress_program_option ("bzip2");
			break;

		case 'k':
			/* Don't replace existing files.  */
			keep_option=true;
			break;

		case 'm':
			touch_option=true;
			break;

		case 'M':
			msxdir_option=true;
			MSXhostdir=string(optx);
			break;

		case 'P':
			msxpart_option=true;
			if (strncasecmp(optx,"all",3) == 0)
			{
				msx_allpart=true;
				MSXpartition=0;
			}
			else
			{
				MSXpartition=strtol(optx,&optx,10);
			};
			break;

		case 't':
			maincommand=LIST_COMMAND;
			do_extract=false;
			verbose_option++;
			break;

		case 'u':
			maincommand=UPDATE_COMMAND;
			break;

		case 'A':
		case 'r':
			maincommand=APPEND_COMMAND;
			break;

		case 'v':
			verbose_option++;
			break;

		case 'x':
			maincommand=EXTRACT_COMMAND;
			do_extract=true;
			break;

		case 'z':
			//set_use_compress_program_option ("gzip");
			break;

		case 'S':
			string imagesize=string(optx);
			if (strncasecmp(imagesize.c_str(),"single",6) == 0)
			{
				nbSectors = 720;
			}
			else if (strncasecmp(imagesize.c_str(),"double",6) == 0)
			{
				nbSectors = 1440;
			}
			else if (strncasecmp(imagesize.c_str(),"ide",3) == 0)
			{
				nbSectors = 65401;
			}
			else
			{
				// first find possible 'b','k' or 'm' end character
				char *P;
				int size=0;
				int scale=SECTOR_SIZE;

				P=optx;
				size=strtol(optx,&P,10);
				for (;*P != 0 ;P++);
				P--;
				cout << "Final letter" << *P << endl;
				cout << "value is " << size << endl;
				switch (*P)
				{
				case 'b':
				case 'B':
					scale=1;
					break;
				case 'k':
				case 'K':
					scale=1024;
					break;
				case 'm':
				case 'M':
					scale=1024*1024;
					break;
				case 's':
				case 'S':
					scale=SECTOR_SIZE;
					break;
				}
				nbSectors = (size*scale)/SECTOR_SIZE;
			};
			break;

		//  sprintf (cursor, "%d", device);
		}

	// end-of-copy
	}

	/* Process trivial options first.  */

	if (show_version)
	{
		printf ("msxtar 0.9\n");
		printf ("Copyright (C) 2004, the openMSX team.\n\n");
		printf("\
This program comes with NO WARRANTY, to the extent permitted by law.\n\
You may redistribute it under the terms of the GNU General Public License;\n\
see the file named COPYING for details.\n");

		printf ("Written by David Heremans.\n");
		printf ("Info provided by Jon De Schrijder and Wouter Vermaelen.\n\n");

		exit (0);
	}
	if (show_help)
	{
		display_usage();
	};
	if (do_fat16)
	{
		EOF_FAT=0xffff;
	}

	//int i=0;
	//for (; i<16 && i!=5 ;i++){cout <<"count: "<<i<<endl;};
	//cout << "final: "<<i<<endl;

	if (argc < 2)
	{
		cout << "Not enough arguments" << endl;
	}
	else
	{
		PRT_DEBUG("Testing switch coammnd");
		switch (maincommand)
		{
		case CREATE_COMMAND:
			createEmptyDSK();
			chroot();
			PRT_DEBUG("optind "<< optind << " argc "<<argc);
			while (optind < argc )
			{
				addcreateDSK(string(argv[optind++]));
			}
			writeImageToDisk(outputFile);
			break;
		case LIST_COMMAND:
		case EXTRACT_COMMAND:
			readDSK(outputFile);
			if (msxpart_option)
			{
				if (msx_allpart)
				{
					for (MSXpartition=0;MSXpartition<31;MSXpartition++)
					{
						PRT_VERBOSE("Handling partition "<<MSXpartition);
						if (chpart(MSXpartition))
						{
							char P[40];
							sprintf(P,"./PARTITION%02i",MSXpartition);
							string dirname=string(P);
							mkdir_ex(dirname.c_str(),ACCESSPERMS);
							recurseDirExtract(dirname,MSXchrootSector,MSXchrootStartIndex);
						}
					}
				}
				else
				{
					if (chpart(MSXpartition))
					{
						chroot();
						doSpecifiedExtraction();
					}
				}
			}
			else
			{
				chroot();
				doSpecifiedExtraction();
			}
			break;

		case APPEND_COMMAND:
			keep_option=true;

		case UPDATE_COMMAND:
			readDSK(outputFile);
			if (msxpart_option)
			{
				if (msx_allpart)
				{
					cout << "Specific partition only!" <<endl;
					exit(1);
				}
				else
				{
					chpart(MSXpartition);
				}
			}
			PRT_DEBUG("optind "<< optind << " argc "<<argc);
			chroot();
			while (optind < argc )
			{
				updateInDSK(string(argv[optind++]));
			}
			writeImageToDisk(outputFile);
			break;
		}
	}
}
