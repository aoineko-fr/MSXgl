// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█		   │  ▄▄▄	▄▄	 ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ▀█▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▀█▄▀ ▄▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//	by Guillaume 'Aoineko' Blanchard under CC BY-SA license
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

#include "core.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

// DOS_USE_FCB
#ifndef DOS_USE_FCB
	#warning DOS_USE_FCB is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define DOS_USE_FCB				TRUE
#endif

// DOS_USE_HANDLE
#ifndef DOS_USE_HANDLE
	#warning DOS_USE_HANDLE is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define DOS_USE_HANDLE			TRUE
#endif

// DOS_USE_UTILITIES
#ifndef DOS_USE_UTILITIES
	#warning DOS_USE_UTILITIES is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define DOS_USE_UTILITIES		TRUE
#endif

// DOS_USE_VALIDATOR
#ifndef DOS_USE_VALIDATOR
	#warning DOS_USE_VALIDATOR is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define DOS_USE_VALIDATOR		TRUE
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Bios calls that can be called directly from MSX-DOS
//
// RDSLT (000CH) - read value at specified address of specified slot
// WRSLT (0014H) - write value at specified address of specified slot
// CALSLT (001CH) - call specified address of specified slot
// ENASLT (0024H) - make specified slot available
// CALLF (0030H) - call specified address of specified slot

// Start BASIC from DOS : https://www.msx.org/wiki/Disk-ROM_BIOS#4022H

#if ((TARGET_TYPE == TYPE_DOS) && ((TARGET != TARGET_DOS0)))
	#define BDOS					0x0005
	#define DOS_TPA					0x0006
#else
	#define BDOS					M_BDOS
#endif

//-----------------------------------------------------------------------------
// MSX-DOS ROUTINES
//-----------------------------------------------------------------------------

// MSX-DOS 1 routines
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
										 // 0x1C-0x20 no function
#define	DOS_FUNC_RDRND				0x21 // Write random file
#define	DOS_FUNC_WRRND				0x22 // Read random file
#define	DOS_FUNC_FSIZE				0x23 // Get file size
#define	DOS_FUNC_SETRND				0x24 // Set random record field
										 // 0x25 no function
#define	DOS_FUNC_WRBLK				0x26 // Write random block
#define	DOS_FUNC_RDBLK				0x27 // Read random block
#define	DOS_FUNC_WRZER				0x28 // Write random file (00H is set to unused portion)
										 // 0x29 no function
#define	DOS_FUNC_GDATE				0x2A // Get date
#define	DOS_FUNC_SDATE				0x2B // Set date
#define	DOS_FUNC_GTIME				0x2C // Get time
#define	DOS_FUNC_STIME				0x2D // Set time
#define	DOS_FUNC_VERIFY				0x2E // Set verify flag
#define	DOS_FUNC_RDABS				0x2F // Read logical sector
#define	DOS_FUNC_WRABS				0x30 // Write logical sector

// MSX-DOS 2 routines
#define DOS_FUNC_DPARM				0x31 // Get disk parameters
										 // 0x1C-0x20 no function
#define DOS_FUNC_FFIRST				0x40 // Find first entry
#define DOS_FUNC_FNEXT				0x41 // Find next entry
#define DOS_FUNC_FNEW				0x42 // Find new entry
#define DOS_FUNC_OPEN				0x43 // Open file handle
#define DOS_FUNC_CREATE				0x44 // Create file handle
#define DOS_FUNC_CLOSE				0x45 // Close file handle
#define DOS_FUNC_ENSURE				0x46 // Ensure file handle
#define DOS_FUNC_DUP				0x47 // Duplicate file handle
#define DOS_FUNC_READ				0x48 // Read from file handle
#define DOS_FUNC_WRITE				0x49 // Write to file handle
#define DOS_FUNC_SEEK				0x4A // Move file handle pointer
#define DOS_FUNC_IOCTL				0x4B // I/O control for devices
#define DOS_FUNC_HTEST				0x4C // Test file handle
#define DOS_FUNC_DELETE				0x4D // Delete file or subdirectory
#define DOS_FUNC_RENAME				0x4E // Rename file or subdirectory
#define DOS_FUNC_MOVE				0x4F // Move file or subdirectory
#define DOS_FUNC_ATTR				0x50 // Get/set file attributes
#define DOS_FUNC_FTIME				0x51 // Get/set file date and time
#define DOS_FUNC_HDELETE			0x52 // Delete file handle
#define DOS_FUNC_HRENAME			0x53 // Rename file handle
#define DOS_FUNC_HMOVE				0x54 // Move file handle
#define DOS_FUNC_HATTR				0x55 // Get/set file handle attributes
#define DOS_FUNC_HFTIME				0x56 // Get/set file handle date and time
#define DOS_FUNC_GETDTA				0x57 // Get disk transfer address
#define DOS_FUNC_GETVFY				0x58 // Get verify flag setting
#define DOS_FUNC_GETCD				0x59 // Get current directory
#define DOS_FUNC_CHDIR				0x5A // Change current directory
#define DOS_FUNC_PARSE				0x5B // Parse pathname
#define DOS_FUNC_PFILE				0x5C // Parse filename
#define DOS_FUNC_CHKCHR				0x5D // Check character
#define DOS_FUNC_WPATH				0x5E // Get whole path string
#define DOS_FUNC_FLUSH				0x5F // Flush disk buffers
#define DOS_FUNC_FORK				0x60 // Fork a child process
#define DOS_FUNC_JOIN				0x61 // Rejoin parent process
#define DOS_FUNC_TERM				0x62 // Terminate with error code
#define DOS_FUNC_DEFAB				0x63 // Define abort exit routine
#define DOS_FUNC_DEFER				0x64 // Define disk error handler routine
#define DOS_FUNC_ERROR				0x65 // Get previous error code
#define DOS_FUNC_EXPLAIN			0x66 // Explain error code
#define DOS_FUNC_FORMAT				0x67 // Format a disk
#define DOS_FUNC_RAMD				0x68 // Create or destroy RAM disk
#define DOS_FUNC_BUFFER				0x69 // Allocate sector buffers
#define DOS_FUNC_ASSIGN				0x6A // Logical drive assignment
#define DOS_FUNC_GENV				0x6B // Get environment item
#define DOS_FUNC_SENV				0x6C // Set environment item
#define DOS_FUNC_FENV				0x6D // Find environment item
#define DOS_FUNC_DSKCHK				0x6E // Get/set disk check status
#define DOS_FUNC_DOSVER				0x6F // Get MSX-DOS version number
#define DOS_FUNC_REDIR				0x70 // Get/set redirection status

//-----------------------------------------------------------------------------
// ERROR VALUES
//-----------------------------------------------------------------------------

// DISK ERRORS - The errors in this group are those which are usually passed to disk error handling routines. By default they will be reported as "Abort, Retry" errors. These errors except the one from "format disk" will be passed to the error handling routine, so they will not be returned as the return value from BDOS.
#define DOS_ERR_NCOMP				0xFF // Incompatible disk - The disk cannot be accessed in that drive (eg. a double sided disk in a single sided drive).
#define DOS_ERR_WRERR				0xFE // Write error - General error occurred during a disk write.
#define DOS_ERR_DISK				0xFD // Disk error - General unknown disk error occurred.
#define DOS_ERR_NRDY				0xFC // Not ready - Disk drive did not respond, usually means there is no disk in the drive.
#define DOS_ERR_VERFY				0xFB // Verify error - With VERIFY enabled, a sector could not be read correctly after being written.
#define DOS_ERR_DATA				0xFA // Data error - A disk sector could not be read because the CRC error checking was incorrect, usually indicating a damaged disk.
#define DOS_ERR_RNF					0xF9 // Sector not found - The required sector could not be found on the disk, usually means a damaged disk.
#define DOS_ERR_WPROT				0xF8 // Write protected disk - Attempt to write to a disk with the write protect tab on.
#define DOS_ERR_UFORM				0xF7 // Unformatted disk - The disk has not been formatted, or it is a disk using a different recording technique.
#define DOS_ERR_NDOS				0xF6 // Not a DOS disk - The disk is formatted for another operating system and cannot be accessed by MSX-DOS.
#define DOS_ERR_WDISK				0xF5 // Wrong disk - The disk has been changed while MSX-DOS was accessing it. Must replace the correct disk.
#define DOS_ERR_WFILE				0xF4 // Wrong disk for file - The disk has been changed while there is an open file on it. Must replace the correct disk.
#define DOS_ERR_SEEK				0xF3 // Seek error - The required track of the disk could not be found.
#define DOS_ERR_IFAT				0xF2 // Bad file allocation table - The file allocation table on the disk has got corrupted. CHKDSK may be able to recover some of the data on the disk.
#define DOS_ERR_NOUPB				0xF1 // This error has no message because it is always trapped internally in MSX-DOS as part of the disk change handling -
#define DOS_ERR_IFORM				0xF0 // Cannot format this drive - Attempt to format a drive which does not allow formatting. Usually as a result of trying to format the RAM disk.

// MSX-DOS FUNCTION ERRORS - The following errors are those which are normally returned from MSX-DOS function calls. See the Function Specification document for details of errors from particular MSX-DOS functions.
#define DOS_ERR_INTER				0xDF // Internal error - Should never occur.
#define DOS_ERR_NORAM				0xDE // Not enough memory - MSX-DOS has run out of memory in its 16k kernel data segment. Try reducing the number of sector buffers or removing some environment strings. Also occurs if there are no free segments for creating the RAMdisk.
#define DOS_ERR_IBDOS				0xDC // Invalid MSX-DOS call - An MSX-DOS call was made with an illegal function number. Most illegal function calls return no error, but this error may be returned if a "get previous error code" function call is made.
#define DOS_ERR_IDRV				0xDB // Invalid drive - A drive number parameter, or a drive letter in a drive/path/file string is one which does not exist in the current system.
#define DOS_ERR_IFNM				0xDA // Invalid filename - A filename string is illegal. This is only generated for pure filename strings, not drive/path/file strings.
#define DOS_ERR_IPATH				0xD9 // Invalid pathname - Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the syntax of the string is incorrect in some way.
#define DOS_ERR_PLONG				0xD8 // Pathname too long - Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the complete path being specified (including current directory if used) is longer than 63 characters.
#define DOS_ERR_NOFIL				0xD7 // File not found - Can be returned by any function which looks for files on a disk if it does not find one. This error is also returned if a directory was specified but not found. In other cases, .NODIR error (see below) will be returned.
#define DOS_ERR_NODIR				0xD6 // Directory not found - Returned if a directory item in a drive/path/file string could not be found.
#define DOS_ERR_DRFUL				0xD5 // Root directory full - Returned by "create" or "move" if a new entry is required in the root directory and it is already full. The root directory cannot be extended.
#define DOS_ERR_DKFUL				0xD4 // Disk full - Usually results from a write operation if there was insufficient room on the disk for the amount of data being written. May also result from trying to create or extend a sub-directory if the disk is completely full.
#define DOS_ERR_DUPF				0xD3 // Duplicate filename - Results from "rename" or "move" if the destination filename already exists in the destination directory.
#define DOS_ERR_DIRE				0xD2 // Invalid directory move - Results from an attempt to move a sub-directory into one of its own descendants. This is not allowed as it would create an isolated loop in the directory structure.
#define DOS_ERR_FILRO				0xD1 // Read only file - Attempt to write to or delete a file which has the "read only" attribute bit set.
#define DOS_ERR_DIRNE				0xD0 // Directory not empty - Attempt to delete a sub-directory which is not empty.
#define DOS_ERR_IATTR				0xCF // Invalid attributes - Can result from an attempt to change a file's attributes in an illegal way, or trying to do an operation on a file which is only possible on a sub-directory. Also results from illegal use of volume name fileinfo blocks.
#define DOS_ERR_DOT					0xCE // Invalid . or .. operation - Attempt to do an illegal operation on the "." or ".." entries in a sub-directory, such as rename or move them.
#define DOS_ERR_SYSX				0xCD // System file exists - Attempt to create a file or sub-directory of the same name as an existing system file. System files are not automatically deleted.
#define DOS_ERR_DIRX				0xCC // Directory exists - Attempt to create a file or sub-directory of the same name as an existing sub-directory. Sub-directories are not automatically deleted.
#define DOS_ERR_FILEX				0xCB // File exists - Attempt to create a sub-directory of the same name as an existing file. Files are not automatically deleted when creating sub-directories.
#define DOS_ERR_FOPEN				0xCA // File already in use - Attempt to delete, rename, move, or change the attributes or date and time of a file which has a file handle already open to it, other than by using the file handle itself.
#define DOS_ERR_OV64K				0xC9 // Cannot transfer above 64K - Disk transfer area would have extended above 0FFFFh.
#define DOS_ERR_FILE				0xC8 // File allocation error - The cluster chain for a file was corrupt. Use CHKDSK to recover as much of the file as possible.
#define DOS_ERR_EOF					0xC7 // End of file - Attempt to read from a file when the file pointer is already at or beyond the end of file.
#define DOS_ERR_ACCV				0xC6 // File access violation - Attempt to read or write to a file handle which was opened with the appropriate access bit set. Some of the standard file handles are opened in read only or write only mode.
#define DOS_ERR_IPROC				0xC5 // Invalid process id - Process id number passed to "join" function is invalid.
#define DOS_ERR_NHAND				0xC4 // No spare file handles - Attempt to open or create a file handle when all file handles are already in use. There are 64 file handles available in the current version.
#define DOS_ERR_IHAND				0xC3 // Invalid file handle - The specified file handle is greater than the maximum allowed file handle number.
#define DOS_ERR_NOPEN				0xC2 // File handle not open - The specified file handle is not currently open.
#define DOS_ERR_IDEV				0xC1 // Invalid device operation - Attempt to use a device file handle or fileinfo block for an invalid operation such as searching in it or moving it.
#define DOS_ERR_IENV				0xC0 // Invalid environment string - Environment item name string contains an invalid character.
#define DOS_ERR_ELONG				0xBF // Environment string too long - Environment item name or value string is either longer than the maximum allowed length of 255, or is too long for the user's buffer.
#define DOS_ERR_IDATE				0xBE // Invalid date - Date parameters passed to "set date" are invalid.
#define DOS_ERR_ITIME				0xBD // Invalid time - Time parameters passed to "set time" are invalid.
#define DOS_ERR_RAMDX				0xBC // RAM disk (drive H:) already exists - Returned from the "ramdisk" function if trying to create a RAM disk when one already exists.
#define DOS_ERR _NRAMD				0xBB // RAM disk does not exist - Attempt to delete the RAM disk when it does not currently exist. A function which tries to access a non-existent RAM disk will get a .IDRV error.
#define DOS_ERR_HDEAD				0xBA // File handle has been deleted - The file associate with a file handle has been deleted so the file handle can no longer be used.
#define DOS_ERR_EOL					0xB9 // - Internal error should never occur.
#define DOS_ERR_ISBFN				0xB8 // Invalid sub-function number - The sub-function number passed to the IOCTL function (function 4Bh) was invalid.

// PROGRAM TERMINATION ERRORS - The following errors are those which may be generated internally in the system and passed to "abort" routines. They will not normally be returned from function calls. Note that an abort routine can also be passed any error which a transient program passes to the "terminate with error code" function call.
#define DOS_ERR_STOP				0x9F // Ctrl-STOP pressed - The Ctrl-STOP key is tested in almost all places in the system including all character I/O.
#define DOS_ERR_CTRLC				0x9E // Ctrl-C pressed - Ctrl-C is only tested for on those character functions which specify status checks.
#define DOS_ERR_ABORT				0x9D // Disk operation aborted - This error occurs when any disk error is aborted by the user or automatically by the system. The original disk error code will be passed to the abort routine in B as the secondary error code.
#define DOS_ERR_OUTERR				0x9C // Error on standard output - Returned if any error occurred on a standard output channel while it was being accessed through the character functions (functions 01h...0Bh). The original error code is passed to the abort routine in register B as the secondary error code. This error will normally only occur if a program has altered the standard file handles.
#define DOS_ERR_INERR				0x9B // Error on standard input - Returned if any error occurred on a standard input channel while it was being accessed through the character functions (functions 01h...0Bh). The original error code is passed to the abort routine in register B as the secondary error code. The most likely error is end of file (.EOF). This error will normally only occur if a program has altered the standard file handles.

// COMMAND ERRORS - The following errors will not be returned from an MSX-DOS function call, but are used by the command interpreter. They are included here because a transient program may find it useful to return some of them. The "Command Specification" document gives more details of what these errors means from the command interpreter.
#define DOS_ERR_BADCOM				0x8F // Wrong version of COMMAND - COMMAND2.COM loaded it's transient part from disk but it's checksum was not what was expected.
#define DOS_ERR_BADCM				0x8E // Unrecognized command - A given command was not an internal command and a .COM or .BAT file was not found with the same name.
#define DOS_ERR_BUFUL				0x8D // Command too long - The command in a batch file exceeded 127 characters in length.
#define DOS_ERR_OKCMD				0x8C // - An internal error used after executing a command passed to COMMAND2.COM on the command line. (There is no message for this error code.)
#define DOS_ERR_IPARM				0x8B // Invalid parameter - The parameter to a command was invalid in some way eg. a number out of range.
#define DOS_ERR_INP					0x8A // Too many parameters - After parsing all the parameters required for a command, there were still more non-separator characters on the command line.
#define DOS_ERR_NOPAR				0x89 // Missing parameter - Where a parameter was expected the end of line was found.
#define DOS_ERR_IOPT				0x88 // Invalid option - The letter given after a / on the command line was invalid for that command.
#define DOS_ERR_BADNO				0x87 // Invalid number - Non-digit characters appeared where a number was expected.
#define DOS_ERR_NOHELP				0x86 // File for HELP not found - The help file was not found or the parameter was not a valid HELP parameter.
#define DOS_ERR_BADVER				0x85 // Wrong version of MSX-DOS - This error is never used by the command interpreter, it has its own internal message for this error. However it is provided for transient programs which may find it useful to return this error.
#define DOS_ERR_NOCAT				0x84 // Cannot concatenate destination file - The destination file in CONCAT is matched by the source specification.
#define DOS_ERR_BADEST				0x83 // Cannot create destination file - In COPY, creating the destination file would overwrite one of the source files (or another file that is already in use).
#define DOS_ERR_COPY				0x82 // File cannot be copied onto itself - In COPY, the destination file if created would overwrite the source file.
#define DOS_ERR_OVDEST				0x81 // Cannot overwrite previous destination file - In COPY, an ambiguous source was specified with a non-ambiguous, non-directory, non-device destination.

//-----------------------------------------------------------------------------
// MISC.
//-----------------------------------------------------------------------------

// DOS function return value
#define DOS_ERR_NONE				0x00
#define DOS_SUCCESS					DOS_ERR_NONE
#define HANDLE_INVALID				0xFF

// MSX-DOS version
#define DOS_VER_NONE				0
#define DOS_VER_1					1
#define DOS_VER_2					2
#define DOS_VER_NEXTOR				3

// Special control characters
#define CTRL_BEEP					0x07 // Produces a simple 'beep' PSG sound
#define CTRL_BS						0x08 // Back Space key
#define CTRL_TAB					0x09 // Tabulation key
#define CTRL_LF						0x0A // Descends the cursor of a line
#define CTRL_HOME					0x0B // Places the cursor at top left
#define CTRL_CLS					0x0C // Clears the screen
#define CTRL_RETURN					0x0D // Executes previous instruction

// MSX-DOS 2 Standard descriptors
#define DOS_STDIN					0
#define DOS_STDOUT					1
#define DOS_STDERR					2
#define DOS_AUX						3
#define DOS_PRN						4

// MSX-DOS 2 Open/create flags
#define O_RDONLY					0x01 // Open file for reading only
#define O_WRONLY					0x02 // Open file for writing only
#define O_RDWR						0x00 // Open file for reading and writing
#define O_INHERIT					0x04 // 

// MSX-DOS 2 Create attributes
#define ATTR_RDONLY					0x01 // If set then the file cannot be written to or deleted, but can be read, renamed or moved.
#define ATTR_HIDDEN					0x02 // If set then the file will only be found by the "Find First" function if the "hidden file" bit is set in the search attributes byte. All the commands implemented by the command interpreter that access files and directories on disk can take a "/H" option which allows the command to find hidden files.
#define ATTR_SYSTEM					0x04 // As far as MSX-DOS functions are concerned, this bit has exactly the same effect as the "HIDDEN FILE" bit except that the "Find New" and "Create" function calls will not automatically delete a system file. None of the commands implemented by the command interpreter allow system files to be accessed.
#define ATTR_VOLUME					0x08 // If set then this entry defines the name of the volume. Can only occur in the root directory, and only once. All other bits are ignored.
#define ATTR_FOLDER					0x10 // If set then the entry is a sub-directory rather than a file and so cannot be opened for reading and writing. Only the hidden bit has any meaning for sub-directories.
#define ATTR_ARCHIVE				0x20 // Whenever a file has been written to and closed this bit is set. This bit can be examined by, for example, the XCOPY command to determine whether the file has been changed.
#define ATTR_RESERVED				0x40 // Reserved (always 0).
#define ATTR_DEVICE					0x80 // This is set to indicate that the FIB refers to a character device (eg. "CON") rather than a disk file. All of the other attributes bits are ignored.

// MSX-DOS 2 Seek whence
#define SEEK_SET					0 // Relative to the beginning of the file
#define SEEK_CUR					1 // Relative to the current position
#define SEEK_END					2 // Relative to the end of the file

// Drive number
enum DOS_DRIVE
{
	DOS_DRIVE_DEFAULT = 0x00,
	DOS_DRIVE_A		  = 0x01,
	DOS_DRIVE_B		  = 0x02,
	DOS_DRIVE_C		  = 0x03,
	DOS_DRIVE_D		  = 0x04,
	DOS_DRIVE_E		  = 0x05,
	DOS_DRIVE_F		  = 0x06,
	DOS_DRIVE_G		  = 0x07,
	DOS_DRIVE_H		  = 0x08,
};

// Get driver number from letter
#define DRIVE_NUM(d)				((d) - 'A' + 1)

// Device ID
enum DOS_DEVICE
{
	// Disk devices
	DOS_DEVICE_DRIVE_A	 = 0x40,
	DOS_DEVICE_DRIVE_B	 = 0x41,
	DOS_DEVICE_DRIVE_C	 = 0x42,
	DOS_DEVICE_DRIVE_D	 = 0x43,
	DOS_DEVICE_DRIVE_E	 = 0x44,
	DOS_DEVICE_DRIVE_F	 = 0x45,
	DOS_DEVICE_DRIVE_G	 = 0x46,
	DOS_DEVICE_DRIVE_H	 = 0x47,
	// Special devices
	DOS_DEVICE_PRINTER	 = 0xFB,
	DOS_DEVICE_LIST		 = 0xFC,
	DOS_DEVICE_NULL		 = 0xFC,
	DOS_DEVICE_AUXILIARY = 0xFE,
	DOS_DEVICE_CONSOLE	 = 0xFF,
};

// C-style alias to manage file through handle
#define DOS_FOpen					DOS_OpenHandle
#define DOS_FCreate					DOS_CreateHandle
#define DOS_FClose					DOS_CloseHandle
#define DOS_FEnsure					DOS_EnsureHandle
#define DOS_FDuplicate				DOS_DuplicateHandle
#define DOS_FRead					DOS_ReadHandle
#define DOS_FWrite					DOS_WriteHandle
#define DOS_FSeek					DOS_SeekHandle
#define DOS_FDelete					DOS_DeleteHandle
#define DOS_FRename					DOS_RenameHandle
#define DOS_FMove					DOS_MoveHandle
#define DOS_FSetAttribute			DOS_SetAttributeHandle
#define DOS_FGetAttribute			DOS_GetAttributeHandle

// File Control Block structure
typedef struct
{
	u8	Drive;			// Drive number containing the file. (0 for Default drive, 1 for A, 2 for B, ..., 8 for H)
	c8	Name[11];		// Filename (format: 8.3)
	u16 CurrentBlock;	// Number of blocks from the top of the file to the current block
	u16 RecordSize;	
	u32 Size;
	u16 Date;			// The two bytes format for DATE are [YYYYYYYM|MMMDDDDD].
	u16 Time;
	u8	DeviceID;
	u8	Directory;
	u16 TopCluster;		// Top cluster number of the file
	u16 LastCluster;	// Last cluster number accessed
	u16 RelativeLoc;	// Relative location from top cluster of the file
	u8	CurrentRecord;
	u32 RandomRecord;	// Record order from the top of the file
} DOS_FCB;

// File info block
typedef struct
{
	u8	Ident;						//		0 - Always 0FFh
	u8	Filename[13];				//	1..13 - Filename as an ASCIIZ string
	u8	Attribute;					//	   14 - File attributes byte
	u16 Time;						// 15..16 - Time of last modification
	u16 Date;						// 17..18 - Date of last modification
	u16 Cluster;					// 19..20 - Start cluster
	u32 Size;						// 21..24 - File size
	u8	Drive;						//	   25 - Logical drive
	u8	Reserved[38];				// 26..63 - Internal information, must not be modified
} DOS_FIB;

#if (DOS_USE_VALIDATOR)
// Backup of the last error value
extern u8 g_DOS_LastError;
#endif

// Backup of the last file info block
extern DOS_FIB g_DOS_LastFIB;

// TPA (Transient Program Area) upper address
#if ((TARGET_TYPE == TYPE_DOS) && ((TARGET != TARGET_DOS0)))
__at(DOS_TPA) u16 DOS_TPAUpperAddr;
#endif

// DOS version structure
typedef struct
{
	u16 Kernel;
	u16 System;
} DOS_Version;

// MSX-DOS 2 time structure
typedef struct
{
	u16 Year;
	u8	Month;
	u8	Date;
	u8	Day;
	u8	Hour;
	u8	Minute;
	u8	Second;
} DOS_Time;

// MSX-DOS 2 disk parameters structure
typedef struct
{
	u8	DriveNum;					// Physical drive number (1=A: etc)
	u16 SectorSize;					// Sector size (always 512 currently)
	u8	ClusterSectors;				// Sectors per cluster (non-zero power of 2)
	u16 ReservedSectors;			// Number of reserved sectors (usually 1)
	u8	FATNum;						// Number of copies of the FAT (usually 2)
	u16 RootNum;					// Number of root directory entries
	u16 LogicalSectors;				// Total number of logical sectors
	u8	Media;						// Media descriptor byte
	u8	FATSectors;					// Number of sectors per FAT
	u16 RootSector;					// First root directory sector number
	u16 DataSector;					// First data sector number
	u16 MaxClusters;				// Maximum cluster number
	u8	DirtyFlag;					// Dirty disk flag
	u32 VolumeID;					// Volume id. (-1 => no volume id.)
	u8	Reserved[8];				// Reserved (currently always zero)
} DOS_DiskParam;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// MSX-DOS 1 FUNCTIONS
//-----------------------------------------------------------------------------

//.............................................................................
// Group: MSX-DOS 1 Core

// Function: DOS_Call
// Call a BDOS function
void DOS_Call(u8 func);

// Function: DOS_Exit0
// Exit program and return to DOS
// When this is called in MSX-DOS, the system is reset by jumping to 0000H.
// When MSX DISK-BASIC call this, it is “warm started”.
// That is, it returns to BASIC command level without destroying programs currently loaded.
// With MSX-DOS 2, this function terminates program with a zero return code.
void DOS_Exit0();

//.............................................................................
// Group: MSX-DOS 1 Console IO

// Function: DOS_CharInput
// Input character
// A character will be read from the standard input (file handle 0 - usually the keyboard) and echoed to the standard output (file handle 1 - usually the screen).
// If no character is ready then this function will wait for one.
// Various control characters, as specified for the "console status" function (function 0Bh), will be trapped by this function for various control purposes.
// If one of these characters is detected then it will be processed and this function will wait for another character.
// Thus these characters will never be returned to the user by this function.
//
// Return:
//   Character from keyboard
c8 DOS_CharInput();

// Function: DOS_CharOutput
// Output character
// The character passed in register E is written to the standard output (file handle 1 - usually the screen).
// If printer echo is enabled then the character is also written to the printer.
// Various control codes and escape sequences are interpreted as screen control codes.
// A list of these is included in the "Program Interface Specification", they are a sub-set of the standard VT-52 control codes.
// TABs will be expanded to every eighth column.
//
// Parameters:
//   chr - Character to be output
void DOS_CharOutput(c8 chr);

// Function: DOS_StringOutput
// The characters of the string will be output. The string is terminated by "$" (ASCII 24h).
//
// Parameters:
//   str - Address of string
void DOS_StringOutput(const c8* str);

// Function: DOS_Beep
// Play bip sound
inline void DOS_Beep() { DOS_CharOutput(CTRL_BEEP); }

// Function: DOS_ClearScreen
// Clear console screen
inline void DOS_ClearScreen() { DOS_CharOutput(CTRL_CLS); }

// Function: DOS_Return
// Carriage return
inline void DOS_Return() { DOS_StringOutput("\n\r$"); }

//.............................................................................
// Group: MSX-DOS 1 File Handling
#if (DOS_USE_FCB)

// Function: DOS_SetTransferAddr
// Set transfer address
// This function simply records the address passed in DE as the disk transfer address.
// This address will be used for all subsequent FCB read and write calls, for "search for first" and "search for next" calls to store the directory entry, and for absolute read and write calls.
// It is not used by the new MSX-DOS read and write functions. The address is set back to 80h by a DISK RESET call.
//
// Parameters:
//   data - Required Disk Transfer Address
void DOS_SetTransferAddr(void* data);

// Function: DOS_OpenFCB
// Open file
// The unopened FCB must contain a drive which may be zero to indicate the current drive and a filename and extension which may be ambiguous.
// The current directory of the specified drive will be searched for a matching file and if found it will be opened.
// Matching entries which are sub-directories or system files will be ignored, and if the filename is ambiguous then the first suitable matching entry will be opened.
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_OpenFCB(DOS_FCB* stream);

// Function: DOS_GetSizeFCB
// Get the size of an opened file
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   File size
inline u32 DOS_GetSizeFCB(DOS_FCB* stream) { return stream->Size; }

// Function: DOS_CloseFCB
// Close file
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_CloseFCB(DOS_FCB* stream);

// Function: DOS_CreateFCB
// Create file
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_CreateFCB(DOS_FCB* stream);

// Function: DOS_SequentialReadFCB
// Sequential read
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_SequentialReadFCB(DOS_FCB* stream);

// Function: DOS_SequentialWriteFCB
// Sequential write
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_SequentialWriteFCB(DOS_FCB* stream);

// Function: DOS_RandomBlockWriteFCB
// Random block write
//
// Parameters:
//   stream - Pointer to unopened FCB
//   records - Number of records to write
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_RandomBlockWriteFCB(DOS_FCB* stream, u16 records);

// Function: DOS_RandomBlockReadFCB
// Random block read
//
// Parameters:
//   stream - Pointer to unopened FCB
//   records - Number of records to write
//
// Return:
//   Number of records actually read
u16 DOS_RandomBlockReadFCB(DOS_FCB* stream, u16 records);

// Function: DOS_FindFirstFileFCB
// Search the first file matched with wildcard
//
// Parameters:
//   stream - Pointer to unopened FCB
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_FindFirstFileFCB(DOS_FCB* stream);

// Function: DOS_FindNextFileFCB
// Search the second and after the second file matched wildcard
//
// Return:
//   Error code (DOS_ERR_NONE if succeed)
u8 DOS_FindNextFileFCB();

#endif // (DOS_USE_FCB)

//-----------------------------------------------------------------------------
// MSX-DOS 2 FUNCTIONS
//-----------------------------------------------------------------------------
#if (TARGET == TARGET_DOS2)

//.............................................................................
// Group: MSX-DOS 2 File Handling
#if (DOS_USE_HANDLE)

// Function: DOS_OpenHandle
// Open file handle
//
// Parameters:
//   path - Drive/path/file ASCIIZ string
//   mode - Open mode
// > O_RDONLY     Open file for reading only
// > O_WRONLY     Open file for writing only
// > O_RDWR       Open file for reading and writing
// > O_INHERIT
//
// Return:
//   New file handle
u8 DOS_OpenHandle(const c8* path, u8 mode);

// Function: DOS_CreateHandle
// Create file handle
//
// Parameters:
//   path - Drive/path/file ASCIIZ string
//   mode - Open mode
// > O_RDONLY     Open file for reading only
// > O_WRONLY     Open file for writing only
// > O_RDWR       Open file for reading and writing
// > O_INHERIT
//   attr - Required attributes
//
// Return:
//   New file handle
u8 DOS_CreateHandle(const c8* path, u8 mode, u8 attr);

// Function: DOS_CloseHandle
// Close file handle
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_CloseHandle(u8 file);

// Function: DOS_EnsureHandle
// Ensure file handle
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_EnsureHandle(u8 file);

// Function: DOS_DuplicateHandle
// Duplicate file handle
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_DuplicateHandle(u8 file);

// Function: DOS_ReadHandle
// Read from file handle
//
// Parameters:
//   file - File handle
//   buffer - Buffer address
//   size - Number of bytes to read
//
// Return:
//   Number of bytes actually read
u16 DOS_ReadHandle(u8 file, void* buffer, u16 size);

// Function: DOS_WriteHandle
// Write to file handle
//
// Parameters:
//   file - File handle
//   buffer - Buffer address
//   size - Number of bytes to read
//
// Return:
//   Number of bytes actually written
u16 DOS_WriteHandle(u8 file, const void* buffer, u16 size);

// Function: DOS_SeekHandle
// Move file handle pointer
//
// Parameters:
//   file - File handle
//   offset - Signed offset
//   mode - Method code
// > SEEK_SET     Relative to the beginning of the file
// > SEEK_CUR     Relative to the current position
// > SEEK_END     Relative to the end of the file
//
// Return:
//   New file pointer
u32 DOS_SeekHandle(u8 file, u32 offset, u8 mode) __CALLEE;

// Function: DOS_DeleteHandle
// Delete file or subdirectory
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_DeleteHandle(u8 file);

// Function: DOS_RenameHandle
// Rename file or subdirectory
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_RenameHandle(u8 file, const c8* newPath);

// Function: DOS_MoveHandle
// Move the file associated with the specified file handle to the directory specified by the new path string.
// A file handle cannot be moved if there are any other separately opened file handles for this file (".FOPEN" error),
// although it can be moved if there are copies of this file handle, and in this case the copies will also be moved.
// Moving a file handle will not alter the file pointer but it will do an implicit "ensure" operation.
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_MoveHandle(u8 file, const c8* newPath);

// Function: DOS_SetAttributeHandle
// Set the attributes byte of the file associated with the specified file handle.
// A file handle cannot have its attributes changed (although they can be read) if there are any other separately opened file handles for this file (".FOPEN" error).
// The file pointer will not be altered but an implicit "ensure" operation will be done.
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_SetAttributeHandle(u8 file, u8 attr);

// Function: DOS_GetAttributeHandle
// Get the attributes byte of the file associated with the specified file handle.
// A file handle cannot have its attributes changed (although they can be read) if there are any other separately opened file handles for this file (".FOPEN" error).
// The file pointer will not be altered but an implicit "ensure" operation will be done.
//
// Parameters:
//   file - File handle
//
// Return:
//   Error code
u8 DOS_GetAttributeHandle(u8 file);

#endif // (DOS_USE_HANDLE)

//.............................................................................
// Group: MSX-DOS 2 Core

// Function: DOS_GetDiskParam
// Get disk parameters
u8 DOS_GetDiskParam(u8 drv, DOS_DiskParam* param);

// Function: DOS_Exit
// Terminate with error code
void DOS_Exit(u8 err);

// Function: DOS_Explain
// Explain error code
void DOS_Explain(u8 err, c8* str);

#if (DOS_USE_VALIDATOR)
// Function: DOS_GetLastError
// Get last error code
inline u8 DOS_GetLastError() { return g_DOS_LastError; }
#endif // (DOS_USE_VALIDATOR)

//.............................................................................
// Group: MSX-DOS 2 Utilities
#if (DOS_USE_UTILITIES)

// Function: DOS_FindFirstEntry
// Find first entry
DOS_FIB* DOS_FindFirstEntry(const c8* filename, u8 attr);

// Function: DOS_FindNextEntry
// Find next entry
DOS_FIB* DOS_FindNextEntry();

// Function: DOS_GetLastFileInfo
// Get last file info
inline DOS_FIB* DOS_GetLastFileInfo() { return &g_DOS_LastFIB; }

// Function: DOS_GetFileYear
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Year (from 1980)
inline u16 DOS_GetFileYear(const DOS_FIB* fib) { return 1980 + (fib->Date >> 9); }

// Function: DOS_GetFileMonth
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Month number
inline u8 DOS_GetFileMonth(const DOS_FIB* fib) { return (fib->Date & 0x01FF) >> 5; }

// Function: DOS_GetFileDay
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Day number
inline u8 DOS_GetFileDay(const DOS_FIB* fib) { return fib->Date & 0x1F; }

// Function: DOS_GetFileHour
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Hours
inline u8 DOS_GetFileHour(const DOS_FIB* fib) { return fib->Time >> 11; }

// Function: DOS_GetFileMinute
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Minutes
inline u8 DOS_GetFileMinute(const DOS_FIB* fib) { return (fib->Time & 0x07FF) >> 5; }

// Function: DOS_GetFileSecond
// Get last error code
//
// Parameters:
//   fib - Pointer to File Info Block structure
//
// Return:
//   Seconds
inline u8 DOS_GetFileSecond(const DOS_FIB* fib) { return (fib->Time & 0x1F) << 1; }

// Function: DOS_Delete
// Delete file or subdirectory
u8 DOS_Delete(const c8* path);

// Function: DOS_Rename
// Rename file or subdirectory
u8 DOS_Rename(const c8* path, const c8* newPath);

// Function: DOS_Move
// Move file or subdirectory
u8 DOS_Move(const c8* path, const c8* newPath);

// Function: DOS_SetAttribute
// Set file attributes
u8 DOS_SetAttribute(const c8* path, u8 attr);

// Function: DOS_GetAttribute
// Get file attributes
u8 DOS_GetAttribute(const c8* path);

#endif // (DOS_USE_UTILITIES)

#endif // (TARGET == TARGET_DOS2)

//.............................................................................
// Group: Misc

// Function: DOS_GetVersion
// Get MSX-DOS version number
u8 DOS_GetVersion(DOS_Version* ver);