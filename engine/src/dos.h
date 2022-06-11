// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// MSX-DOS routines handler 
//
// References:
// - http://map.grauw.nl/resources/dos2_environment.php
// - http://map.grauw.nl/resources/dos2_functioncalls.php
// - https://konamiman.github.io/MSX2-Technical-Handbook/md/Chapter3.html#11-features-of-msx-dos
// - http://msx.avelinoherrera.com/index_en.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

#include "core.h"

// Bios calls that can be called directly from MSX-DOS
//
// RDSLT (000CH) - read value at specified address of specified slot
// WRSLT (0014H) - write value at specified address of specified slot
// CALSLT (001CH) - call specified address of specified slot
// ENASLT (0024H) - make specified slot available
// CALLF (0030H) - call specified address of specified slot

// Start BASIC from DOS : https://www.msx.org/wiki/Disk-ROM_BIOS#4022H

#if (TARGET_TYPE == TYPE_DOS)
#define BDOS						0x0005
#define DOS_TPA						0x0006
#else
#define BDOS						0xF37D	
#endif

#define	DOS_FUNC_TERM0				0x00 // System reset
#define	DOS_FUNC_CONIN				0x01 // Get one character from console (input wait, echo back, control code check)
#define	DOS_FUNC_CONOUT				0x02 // Send one character to console
#define	DOS_FUNC_AUXIN				0x03 // Get one character from auxiliary device
#define	DOS_FUNC_AUXOUT				0x04 // Send one character to auxiliary device
#define	DOS_FUNC_LSTOUT				0x05 // Send one character to printer
#define	DOS_FUNC_DIRIO				0x06 // Get one character from console (no input wait, no echo back, no control code check) / one character output
#define	DOS_FUNC_DIRIN				0x07 // Get one character from console (input wait, no echo back, no control code check)
#define	DOS_FUNC_INNOE				0x08 // Get one character from console (input wait, no echo back, control code check)
#define	DOS_FUNC_STROUT				0x09 // Send string
#define	DOS_FUNC_BUFIN				0x0A // Get string
#define	DOS_FUNC_CONST				0x0B // Check input from console
#define	DOS_FUNC_CPMVER				0x0C // Get version number
#define	DOS_FUNC_DSKRST				0x0D // Disk reset
#define	DOS_FUNC_SELDSK				0x0E // Select default drive
#define	DOS_FUNC_FOPEN				0x0F // Open file
#define	DOS_FUNC_FCLOSE				0x10 // Close file
#define	DOS_FUNC_SFIRST				0x11 // Search the first file matched with wildcard
#define	DOS_FUNC_SNEXT				0x12 // Search the second and after the second file matched wildcard
#define	DOS_FUNC_FDEL				0x13 // Delete file
#define	DOS_FUNC_RDSEQ				0x14 // Read sequential file
#define	DOS_FUNC_WRSEQ				0x15 // Write sequential file
#define	DOS_FUNC_FMAKE				0x16 // Create file
#define	DOS_FUNC_FREN				0x17 // Rename file
#define	DOS_FUNC_LOGIN				0x18 // Get login vector
#define	DOS_FUNC_CURDRV				0x19 // Get default drive name
#define	DOS_FUNC_SETDTA				0x1A // Set DMA address
#define	DOS_FUNC_ALLOC				0x1B // Get disk information
//                      			0x1C-0x20 no function
#define	DOS_FUNC_RDRND				0x21 // Write random file
#define	DOS_FUNC_WRRND				0x22 // Read random file
#define	DOS_FUNC_FSIZE				0x23 // Get file size
#define	DOS_FUNC_SETRND				0x24 // Set random record field
//                      			0x25 no function
#define	DOS_FUNC_WRBLK				0x26 // Write random block
#define	DOS_FUNC_RDBLK				0x27 // Read random block
#define	DOS_FUNC_WRZER				0x28 // Write random file (00H is set to unused portion)
//									0x29 no function
#define	DOS_FUNC_GDATE				0x2A // Get date
#define	DOS_FUNC_SDATE				0x2B // Set date
#define	DOS_FUNC_GTIME				0x2C // Get time
#define	DOS_FUNC_STIME				0x2D // Set time
#define	DOS_FUNC_VERIFY				0x2E // Set verify flag
#define	DOS_FUNC_RDABS				0x2F // Read logical sector
#define	DOS_FUNC_WRABS				0x30 // Write logical sector

#define CTRL_BEEP					0x07 // Produces a simple 'beep' PSG sound
#define CTRL_BS						0x08 // Back Space key
#define CTRL_TAB					0x09 // Tabulation key
#define CTRL_LF						0x0A // Descends the cursor of a line
#define CTRL_HOME					0x0B // Places the cursor at top left
#define CTRL_CLS					0x0C // Clears the screen
#define CTRL_RETURN					0x0D // Executes previous instruction

//=============================================================================
// MSX-DOS 1
//=============================================================================

// DOS function return value
#define DOS_ERROR					0xFF
#define DOS_NO_ERROR				0x00

// Drive number
enum DOS_DRIVE
{
	DOS_DRIVE_DEFAULT = 0x00,
	DOS_DRIVE_A       = 0x01,
	DOS_DRIVE_B       = 0x02,
	DOS_DRIVE_C       = 0x03,
	DOS_DRIVE_D       = 0x04,
	DOS_DRIVE_E       = 0x05,
	DOS_DRIVE_F       = 0x06,
	DOS_DRIVE_G       = 0x07,
	DOS_DRIVE_H       = 0x08,
};


// Device ID
enum DOS_DEVICE
{
	// Disk devices
	DOS_DEVICE_DRIVE_A   = 0x40,
	DOS_DEVICE_DRIVE_B   = 0x41,
	DOS_DEVICE_DRIVE_C   = 0x42,
	DOS_DEVICE_DRIVE_D   = 0x43,
	DOS_DEVICE_DRIVE_E   = 0x44,
	DOS_DEVICE_DRIVE_F   = 0x45,
	DOS_DEVICE_DRIVE_G   = 0x46,
	DOS_DEVICE_DRIVE_H   = 0x47,
	// Special devices
	DOS_DEVICE_PRINTER   = 0xFB,
	DOS_DEVICE_LIST      = 0xFC,
	DOS_DEVICE_NULL      = 0xFC,
	DOS_DEVICE_AUXILIARY = 0xFE,
	DOS_DEVICE_CONSOLE   = 0xFF,
};

// File Control Block structure
typedef struct {
	u8   Drive;			// Drive number containing the file. (0 for Default drive, 1 for A, 2 for B, ..., 8 for H)
	c8   Name[11];		// Filename (format: 8.3)
	u16  CurrentBlock;	// Number of blocks from the top of the file to the current block
	u16  RecordSize;	
	u32  Size;
	u16  Date;			// The two bytes format for DATE are [YYYYYYYM|MMMDDDDD].
	u16  Time;
	u8   DeviceID;
	u8   Directory;
	u16  TopCluster;	// Top cluster number of the file
	u16  LastCluster;	// Last cluster number accessed
	u16  RelativeLoc;	// Relative location from top cluster of the file
	u8   CurrentRecord;
	u32  RandomRecord;	// Record order from the top of the file
} FCB;

// Backup of the last error value
extern i8 g_DOS_LastError;

// TPA (Transient Program Area) upper address
#if (TARGET_TYPE == TYPE_DOS)
__at(DOS_TPA) u16 DOS_TPAUpperAddr;
#endif

//-----------------------------------------------------------------------------
// Goup: Console IO

// Function: DOS_Call
// Call a BDOS function
void DOS_Call(u8 func);

// Function: DOS_CharacterOutput
// Output character
void DOS_CharOutput(c8 chr);

// Function: DOS_CharInput
// Input character
c8 DOS_CharInput();

// Function: DOS_StringOutput
// The characters of the string will be output. The string is terminated by "$" (ASCII 24h).
void DOS_StringOutput(const c8* str);

// Function: DOS_Exit
// Exit program and return to DOS
void DOS_Exit();

// Function: DOS_Beep
// Play bip sound
inline void DOS_Beep() { DOS_CharOutput(CTRL_BEEP); }

// Function: DOS_CharacterOutput
// Clear console screen
inline void DOS_ClearScreen() { DOS_CharOutput(CTRL_CLS); }

// Function: DOS_Return
// Carriage return
inline void DOS_Return() { DOS_StringOutput("\n\r$"); }

//-----------------------------------------------------------------------------
// Goup: File Handling

// Function: DOS_SetTransferAddr
// Set transfer address
i8 DOS_SetTransferAddr(void* data);

// Function: DOS_Open
// Open file
i8 DOS_Open(FCB* stream);

// Function: DOS_GetFileSize
// Get the size of an opened file
inline u32 DOS_GetSize(FCB* stream) { return stream->Size; }

// Function: DOS_Create
// Create file
i8 DOS_Create(FCB* stream);

// Function: DOS_Close
// Close file
i8 DOS_Close(FCB* stream);

// Function: DOS_SequentialRead
// Sequential read
i8 DOS_SequentialRead(FCB* stream);

// Function: DOS_SequentialWrite
// Sequential write
i8 DOS_SequentialWrite(FCB* stream);

// Function: DOS_RandomBlockRead
// Random block read
u16 DOS_RandomBlockRead(FCB* stream, u16 records);

// Function: DOS_RandomBlockWrite
// Random block write
u16 DOS_RandomBlockWrite(FCB* stream, u16 records);

// Function: DOS_FindFirstFile
// Search the first file matched with wildcard
i8 DOS_FindFirstFile(FCB* stream);

// Function: DOS_FindNextFile
// Search the second and after the second file matched wildcard
i8 DOS_FindNextFile();

//=============================================================================
// MSX-DOS 2
//=============================================================================
#if (TARGET == TARGET_DOS2)

// Standard descriptors
#define  STDIN   			0
#define  STDOUT  			1
#define  STDERR  			2
#define  AUX     			3
#define  PRN     			4

// Open/create flags
#define  O_RDONLY			0x01
#define  O_WRONLY			0x02
#define  O_RDWR				0x00
#define  O_INHERIT			0x04

// Create attributes
#define  ATTR_RDONLY		0x01
#define  ATTR_HIDDEN		0x02
#define  ATTR_SYSTEM		0x04
#define  ATTR_VOLUME		0x08
#define  ATTR_FOLDER		0x10
#define  ATTR_ARCHIV		0x20
#define  ATTR_DEVICE		0x80

// Seek whence
#define  SEEK_SET			0
#define  SEEK_CUR			1
#define  SEEK_END			2

//
i8 DOS2_Open(char *, u8) __sdcccall(0);

//
i8 DOS2_Create(char *, u8, u8) __sdcccall(0);

//
i8 DOS2_Close(i8) __sdcccall(0);

//
i8 DOS2_Duplicate(i8) __sdcccall(0);

//
i16 DOS2_Read(i8, void *, i16) __sdcccall(0);

//
i16 DOS2_Write(i8, void *, i16) __sdcccall(0);

//
u32 DOS2_LSeek(i8, u32, u8) __sdcccall(0);

//
void DOS2_Exit(i8) __sdcccall(0);

#define DOS_FUNC_DPARM		0x31 // Get disk parameters
//							0x1C-0x20 no function
#define DOS_FUNC_FFIRST		0x40 // Find first entry
#define DOS_FUNC_FNEXT		0x41 // Find next entry
#define DOS_FUNC_FNEW		0x42 // Find new entry
#define DOS_FUNC_OPEN		0x43 // Open file handle
#define DOS_FUNC_CREATE		0x44 // Create file handle
#define DOS_FUNC_CLOSE		0x45 // Close file handle
#define DOS_FUNC_ENSURE		0x46 // Ensure file handle
#define DOS_FUNC_DUP		0x47 // Duplicate file handle
#define DOS_FUNC_READ		0x48 // Read from file handle
#define DOS_FUNC_WRITE		0x49 // Write to file handle
#define DOS_FUNC_SEEK		0x4A // Move file handle pointer
#define DOS_FUNC_IOCTL		0x4B // I/O control for devices
#define DOS_FUNC_HTEST		0x4C // Test file handle
#define DOS_FUNC_DELETE		0x4D // Delete file or subdirectory
#define DOS_FUNC_RENAME		0x4E // Rename file or subdirectory
#define DOS_FUNC_MOVE		0x4F // Move file or subdirectory
#define DOS_FUNC_ATTR		0x50 // Get/set file attributes
#define DOS_FUNC_FTIME		0x51 // Get/set file date and time
#define DOS_FUNC_HDELETE	0x52 // Delete file handle
#define DOS_FUNC_HRENAME	0x53 // Rename file handle
#define DOS_FUNC_HMOVE		0x54 // Move file handle
#define DOS_FUNC_HATTR		0x55 // Get/set file handle attributes
#define DOS_FUNC_HFTIME		0x56 // Get/set file handle date and time
#define DOS_FUNC_GETDTA		0x57 // Get disk transfer address
#define DOS_FUNC_GETVFY		0x58 // Get verify flag setting
#define DOS_FUNC_GETCD		0x59 // Get current directory
#define DOS_FUNC_CHDIR		0x5A // Change current directory
#define DOS_FUNC_PARSE		0x5B // Parse pathname
#define DOS_FUNC_PFILE		0x5C // Parse filename
#define DOS_FUNC_CHKCHR		0x5D // Check character
#define DOS_FUNC_WPATH		0x5E // Get whole path string
#define DOS_FUNC_FLUSH		0x5F // Flush disk buffers
#define DOS_FUNC_FORK		0x60 // Fork a child process
#define DOS_FUNC_JOIN		0x61 // Rejoin parent process
#define DOS_FUNC_TERM		0x62 // Terminate with error code
#define DOS_FUNC_DEFAB		0x63 // Define abort exit routine
#define DOS_FUNC_DEFER		0x64 // Define disk error handler routine
#define DOS_FUNC_ERROR		0x65 // Get previous error code
#define DOS_FUNC_EXPLAIN	0x66 // Explain error code
#define DOS_FUNC_FORMAT		0x67 // Format a disk
#define DOS_FUNC_RAMD		0x68 // Create or destroy RAM disk
#define DOS_FUNC_BUFFER		0x69 // Allocate sector buffers
#define DOS_FUNC_ASSIGN		0x6A // Logical drive assignment
#define DOS_FUNC_GENV		0x6B // Get environment item
#define DOS_FUNC_SENV		0x6C // Set environment item
#define DOS_FUNC_FENV		0x6D // Find environment item
#define DOS_FUNC_DSKCHK		0x6E // Get/set disk check status
#define DOS_FUNC_DOSVER		0x6F // Get MSX-DOS version number
#define DOS_FUNC_REDIR		0x70 // Get/set redirection status

//-----------------------------------------------------------------------------
// ERROR VALUES
//-----------------------------------------------------------------------------

// DISK ERRORS - The errors in this group are those which are usually passed to disk error handling routines. By default they will be reported as "Abort, Retry" errors. These errors except the one from "format disk" will be passed to the error handling routine, so they will not be returned as the return value from BDOS.
#define DOS_ERR_NCOMP		0xFF // Incompatible disk - The disk cannot be accessed in that drive (eg. a double sided disk in a single sided drive).
#define DOS_ERR_WRERR		0xFE // Write error - General error occurred during a disk write.
#define DOS_ERR_DISK		0xFD // Disk error - General unknown disk error occurred.
#define DOS_ERR_NRDY		0xFC // Not ready - Disk drive did not respond, usually means there is no disk in the drive.
#define DOS_ERR_VERFY		0xFB // Verify error - With VERIFY enabled, a sector could not be read correctly after being written.
#define DOS_ERR_DATA		0xFA // Data error - A disk sector could not be read because the CRC error checking was incorrect, usually indicating a damaged disk.
#define DOS_ERR_RNF			0xF9 // Sector not found - The required sector could not be found on the disk, usually means a damaged disk.
#define DOS_ERR_WPROT		0xF8 // Write protected disk - Attempt to write to a disk with the write protect tab on.
#define DOS_ERR_UFORM		0xF7 // Unformatted disk - The disk has not been formatted, or it is a disk using a different recording technique.
#define DOS_ERR_NDOS		0xF6 // Not a DOS disk - The disk is formatted for another operating system and cannot be accessed by MSX-DOS.
#define DOS_ERR_WDISK		0xF5 // Wrong disk - The disk has been changed while MSX-DOS was accessing it. Must replace the correct disk.
#define DOS_ERR_WFILE		0xF4 // Wrong disk for file - The disk has been changed while there is an open file on it. Must replace the correct disk.
#define DOS_ERR_SEEK		0xF3 // Seek error - The required track of the disk could not be found.
#define DOS_ERR_IFAT		0xF2 // Bad file allocation table - The file allocation table on the disk has got corrupted. CHKDSK may be able to recover some of the data on the disk.
#define DOS_ERR_NOUPB		0xF1 // This error has no message because it is always trapped internally in MSX-DOS as part of the disk change handling -
#define DOS_ERR_IFORM		0xF0 // Cannot format this drive - Attempt to format a drive which does not allow formatting. Usually as a result of trying to format the RAM disk.

// MSX-DOS FUNCTION ERRORS - The following errors are those which are normally returned from MSX-DOS function calls. See the Function Specification document for details of errors from particular MSX-DOS functions.
#define DOS_ERR_INTER		0xDF // Internal error - Should never occur.
#define DOS_ERR_NORAM		0xDE // Not enough memory - MSX-DOS has run out of memory in its 16k kernel data segment. Try reducing the number of sector buffers or removing some environment strings. Also occurs if there are no free segments for creating the RAMdisk.
#define DOS_ERR_IBDOS		0xDC // Invalid MSX-DOS call - An MSX-DOS call was made with an illegal function number. Most illegal function calls return no error, but this error may be returned if a "get previous error code" function call is made.
#define DOS_ERR_IDRV		0xDB // Invalid drive - A drive number parameter, or a drive letter in a drive/path/file string is one which does not exist in the current system.
#define DOS_ERR_IFNM		0xDA // Invalid filename - A filename string is illegal. This is only generated for pure filename strings, not drive/path/file strings.
#define DOS_ERR_IPATH		0xD9 // Invalid pathname - Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the syntax of the string is incorrect in some way.
#define DOS_ERR_PLONG		0xD8 // Pathname too long - Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the complete path being specified (including current directory if used) is longer than 63 characters.
#define DOS_ERR_NOFIL		0xD7 // File not found - Can be returned by any function which looks for files on a disk if it does not find one. This error is also returned if a directory was specified but not found. In other cases, .NODIR error (see below) will be returned.
#define DOS_ERR_NODIR		0xD6 // Directory not found - Returned if a directory item in a drive/path/file string could not be found.
#define DOS_ERR_DRFUL		0xD5 // Root directory full - Returned by "create" or "move" if a new entry is required in the root directory and it is already full. The root directory cannot be extended.
#define DOS_ERR_DKFUL		0xD4 // Disk full - Usually results from a write operation if there was insufficient room on the disk for the amount of data being written. May also result from trying to create or extend a sub-directory if the disk is completely full.
#define DOS_ERR_DUPF		0xD3 // Duplicate filename - Results from "rename" or "move" if the destination filename already exists in the destination directory.
#define DOS_ERR_DIRE		0xD2 // Invalid directory move - Results from an attempt to move a sub-directory into one of its own descendants. This is not allowed as it would create an isolated loop in the directory structure.
#define DOS_ERR_FILRO		0xD1 // Read only file - Attempt to write to or delete a file which has the "read only" attribute bit set.
#define DOS_ERR_DIRNE		0xD0 // Directory not empty - Attempt to delete a sub-directory which is not empty.
#define DOS_ERR_IATTR		0xCF // Invalid attributes - Can result from an attempt to change a file's attributes in an illegal way, or trying to do an operation on a file which is only possible on a sub-directory. Also results from illegal use of volume name fileinfo blocks.
#define DOS_ERR_DOT			0xCE // Invalid . or .. operation - Attempt to do an illegal operation on the "." or ".." entries in a sub-directory, such as rename or move them.
#define DOS_ERR_SYSX		0xCD // System file exists - Attempt to create a file or sub-directory of the same name as an existing system file. System files are not automatically deleted.
#define DOS_ERR_DIRX		0xCC // Directory exists - Attempt to create a file or sub-directory of the same name as an existing sub-directory. Sub-directories are not automatically deleted.
#define DOS_ERR_FILEX		0xCB // File exists - Attempt to create a sub-directory of the same name as an existing file. Files are not automatically deleted when creating sub-directories.
#define DOS_ERR_FOPEN		0xCA // File already in use - Attempt to delete, rename, move, or change the attributes or date and time of a file which has a file handle already open to it, other than by using the file handle itself.
#define DOS_ERR_OV64K		0xC9 // Cannot transfer above 64K - Disk transfer area would have extended above 0FFFFh.
#define DOS_ERR_FILE		0xC8 // File allocation error - The cluster chain for a file was corrupt. Use CHKDSK to recover as much of the file as possible.
#define DOS_ERR_EOF			0xC7 // End of file - Attempt to read from a file when the file pointer is already at or beyond the end of file.
#define DOS_ERR_ACCV		0xC6 // File access violation - Attempt to read or write to a file handle which was opened with the appropriate access bit set. Some of the standard file handles are opened in read only or write only mode.
#define DOS_ERR_IPROC		0xC5 // Invalid process id - Process id number passed to "join" function is invalid.
#define DOS_ERR_NHAND		0xC4 // No spare file handles - Attempt to open or create a file handle when all file handles are already in use. There are 64 file handles available in the current version.
#define DOS_ERR_IHAND		0xC3 // Invalid file handle - The specified file handle is greater than the maximum allowed file handle number.
#define DOS_ERR_NOPEN		0xC2 // File handle not open - The specified file handle is not currently open.
#define DOS_ERR_IDEV		0xC1 // Invalid device operation - Attempt to use a device file handle or fileinfo block for an invalid operation such as searching in it or moving it.
#define DOS_ERR_IENV		0xC0 // Invalid environment string - Environment item name string contains an invalid character.
#define DOS_ERR_ELONG		0xBF // Environment string too long - Environment item name or value string is either longer than the maximum allowed length of 255, or is too long for the user's buffer.
#define DOS_ERR_IDATE		0xBE // Invalid date - Date parameters passed to "set date" are invalid.
#define DOS_ERR_ITIME		0xBD // Invalid time - Time parameters passed to "set time" are invalid.
#define DOS_ERR_RAMDX 		0xBC // RAM disk (drive H:) already exists - Returned from the "ramdisk" function if trying to create a RAM disk when one already exists.
#define DOS_ERR _NRAMD		0xBB // RAM disk does not exist - Attempt to delete the RAM disk when it does not currently exist. A function which tries to access a non-existent RAM disk will get a .IDRV error.
#define DOS_ERR_HDEAD		0xBA // File handle has been deleted - The file associate with a file handle has been deleted so the file handle can no longer be used.
#define DOS_ERR_EOL			0xB9 // - Internal error should never occur.
#define DOS_ERR_ISBFN		0xB8 // Invalid sub-function number - The sub-function number passed to the IOCTL function (function 4Bh) was invalid.

// PROGRAM TERMINATION ERRORS - The following errors are those which may be generated internally in the system and passed to "abort" routines. They will not normally be returned from function calls. Note that an abort routine can also be passed any error which a transient program passes to the "terminate with error code" function call.
#define DOS_ERR_STOP		0x9F // Ctrl-STOP pressed - The Ctrl-STOP key is tested in almost all places in the system including all character I/O.
#define DOS_ERR_CTRLC		0x9E // Ctrl-C pressed - Ctrl-C is only tested for on those character functions which specify status checks.
#define DOS_ERR_ABORT		0x9D // Disk operation aborted - This error occurs when any disk error is aborted by the user or automatically by the system. The original disk error code will be passed to the abort routine in B as the secondary error code.
#define DOS_ERR_OUTERR		0x9C // Error on standard output - Returned if any error occurred on a standard output channel while it was being accessed through the character functions (functions 01h...0Bh). The original error code is passed to the abort routine in register B as the secondary error code. This error will normally only occur if a program has altered the standard file handles.
#define DOS_ERR_INERR		0x9B // Error on standard input - Returned if any error occurred on a standard input channel while it was being accessed through the character functions (functions 01h...0Bh). The original error code is passed to the abort routine in register B as the secondary error code. The most likely error is end of file (.EOF). This error will normally only occur if a program has altered the standard file handles.

// COMMAND ERRORS - The following errors will not be returned from an MSX-DOS function call, but are used by the command interpreter. They are included here because a transient program may find it useful to return some of them. The "Command Specification" document gives more details of what these errors means from the command interpreter.
#define DOS_ERR_BADCOM		0x8F // Wrong version of COMMAND - COMMAND2.COM loaded it's transient part from disk but it's checksum was not what was expected.
#define DOS_ERR_BADCM		0x8E // Unrecognized command - A given command was not an internal command and a .COM or .BAT file was not found with the same name.
#define DOS_ERR_BUFUL		0x8D // Command too long - The command in a batch file exceeded 127 characters in length.
#define DOS_ERR_OKCMD		0x8C // - An internal error used after executing a command passed to COMMAND2.COM on the command line. (There is no message for this error code.)
#define DOS_ERR_IPARM		0x8B // Invalid parameter - The parameter to a command was invalid in some way eg. a number out of range.
#define DOS_ERR_INP			0x8A // Too many parameters - After parsing all the parameters required for a command, there were still more non-separator characters on the command line.
#define DOS_ERR_NOPAR		0x89 // Missing parameter - Where a parameter was expected the end of line was found.
#define DOS_ERR_IOPT		0x88 // Invalid option - The letter given after a / on the command line was invalid for that command.
#define DOS_ERR_BADNO		0x87 // Invalid number - Non-digit characters appeared where a number was expected.
#define DOS_ERR_NOHELP		0x86 // File for HELP not found - The help file was not found or the parameter was not a valid HELP parameter.
#define DOS_ERR_BADVER		0x85 // Wrong version of MSX-DOS - This error is never used by the command interpreter, it has its own internal message for this error. However it is provided for transient programs which may find it useful to return this error.
#define DOS_ERR_NOCAT		0x84 // Cannot concatenate destination file - The destination file in CONCAT is matched by the source specification.
#define DOS_ERR_BADEST		0x83 // Cannot create destination file - In COPY, creating the destination file would overwrite one of the source files (or another file that is already in use).
#define DOS_ERR_COPY		0x82 // File cannot be copied onto itself - In COPY, the destination file if created would overwrite the source file.
#define DOS_ERR_OVDEST		0x81 // Cannot overwrite previous destination file - In COPY, an ambiguous source was specified with a non-ambiguous, non-directory, non-device destination.

#endif