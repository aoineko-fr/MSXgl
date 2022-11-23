// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄  ▄▄▄▄  ▄▄   ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀  ██  ██ █ ▀█▄  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ▄██▄ ▀█▄▀ ▄▄█▀ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// BIOS hooks routines handler 
//
// References:
// - https://www.msx.org/wiki/System_hooks
// - Pratique du MSX2
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "memory.h"

//-----------------------------------------------------------------------------
// Hooks handler

// Function: Bios_SetHookCallback
// Set a safe hook interslot jump to a given function
//
// Parameters:
//   hook - Hook address
//   cb - Function to be called
void Bios_SetHookCallback(u16 hook, callback cb);

// Function: Bios_SetHookDirectCallback
// Set a Hook to jump directly to a given function
//
// Parameters:
//   hook - Hook address
//   cb - Function to be called
inline void Bios_SetHookDirectCallback(u16 hook, callback cb)
{
	*((u8*)hook) = 0xC3; // JUMP
	*((callback*)++hook) = cb; // Callback address
}

// Function: Bios_SetHookInterSlotCallback
// Set a Hook to jump to a given function in a given slot ID
//
// Parameters:
//   hook - Hook address
//   slot - Slot ID
//   cb - Function to be called
inline void Bios_SetHookInterSlotCallback(u16 hook, u32 slot, callback cb)
{
	*((u8*)hook) = 0xF7; // RST #30
	*((u8*)++hook) = slot; // Slot ID
	*((callback*)++hook) = cb; // Callback address
}

// Function: Bios_ClearHook
// Clear a Hook (set RET asm code)
//
// Parameters:
//   hook - Hook address
inline void Bios_ClearHook(u16 hook)
{
	*((u8*)hook) = 0xC9; // RET
}

// Function: Bios_BackupHook
// Backup hook content into a buffer (must be 5 bytes length). The buffer content can been call at any time using Call() function
//
// Parameters:
//   hook - Hook address
//   buffer - Destination buffer in RAM
inline void Bios_BackupHook(u16 hook, void* buffer)
{
	Mem_Copy((void*)hook, (void*)buffer, 5);
}

//-----------------------------------------------------------------------------
// Deferred Hooks handler

// #define MAX_DEFERRED_HOOK 16
// extern u8 g_DeferredHookCount;
// extern callback g_DeferredHooks[MAX_DEFERRED_HOOK];
// extern callback g_LastCallback;

// // inline void AddDeferredHook()
// {
// 	if(g_DeferredHookCount < MAX_DEFERRED_HOOK)
// 	{
// 		DeferredHooks[DeferredHookCount] = g_LastCallback;
// 		DeferredHookCount++;
// 	}
// }

// // Set a Hook to jump to given function
// inline void SetDeferredHook(u16 hook, callback cb)
// {
// 	g_LastCallback = cb;
// 	*((u8*)hook) = 0xC3; // JUMP
// 	*((callback*)++hook) = AddDeferredHook;
// }

//-----------------------------------------------------------------------------
// Hooks

#define H_KEYI	0xFD9A
// Call: Called at the beginning of the KEYINT interrupt routine (Main-ROM at 0038h).
// Usage: Used to test whether the interruption was caused by a device other than the VDP. (RS-232C, MSX-Midi, etc)
// Note: The program must back to the interrupt routine if no device has caused the interrupt.

#define H_TIMI	0xFD9F
// Call: Called by the KEYINT interrupt routine (Main-ROM at 0038h) just after reading the status register 0 of the VDP (Must be specified by the control register 15 on MSX2 and newer).
// Usage: Synchronize the graphic display. You can add graphics during the Vblank to have a clean display.
// Note: This hook is supposed to be only called when Vblank interrupt occurs (control register 15 must contain 0 on MSX2 and newer). The register A contains the value read to status register 0 when this Hook is called in order to know.

#define H_CHPU	0xFDA4
// Call: At the beginning of the routine CHPUT (Main-ROM at 00A2h) to display a character on the screen.
// Usage: Allows the use of a device other than a screen.
// Note: Register A contains the character code when this Hook is called.

#define H_DSPC	0xFDA9	
// Call: At the beginning of internal routine DSPCSR that displays the cursor.
// Usage: Allow access to other devices than the screen.

#define H_ERAC	0xFDAE	
// Call: At the beginning of internal routine ERACSR that erases the cursor.
// Usage: Allow access to other devices than the screen.

#define H_DSPF	0xFDB3	
// Call: At the beginning of the routines DSPFNK (Main-ROM at 00CFh) that displays the contents of the function keys.
// Usage: Allow access to other devices than the screen.

#define H_ERAF	0xFDB8	
// Call: At the beginning of the routine ERAFNK (Main-ROM at 00CCh) to erase the text of fonction keys.
// Usage: Allow access to other devices than the screen.

#define H_TOTE	0xFDBD	
// Call: At the beginning of the routine TOTEXT (Main-ROM at 00D2h) to back into text mode.
// Usage: Allow access to other devices than the screen.

#define H_CHGE	0xFDC2	
// Call: At the beginning of the routine CHGET (Main-ROM at 009Fh) to read a character on the keyboard.
// Usage: Allow access to other devices than the MSX keyboard.

#define H_INIP	0xFDC7	
// Call: At the beginning of internal routine INIPAT that initializes characters font in VRAM.
// Usage: Change the characters font when back to text mode (after a graphic mode).

#define H_KEYC	0xFDCC	
// Call: At the beginning of internal routine KEYCOD of keyboard reading.
// Usage: Intercept the keyboard inputs.
// Note: When the Hook is called, the accumulator contains: (line number) x 8 + column number of the key pressed in the keyboard matrix.

#define H_KYEA	0xFDD1	
// Call: At the beginning of internal routine KYEASY that decodes the keyboard.
// Usage: Change the way a key is interpreted.

#define H_NMI	0xFDD6	
// Call: At the beginning of non-maskable interrupts routine (Main-ROM at 0066h).
// Usage: Designed for handling non-maskable interrupts.
// Note: Unused on MSX except perhaps in the mode R800 of MSX turbo R in option.

#define H_PINL	0xFDDB	
// Call: At the beginning of Bios routine PINLIN (Main-ROM at 00AEh) that manages the input of a program line to the keyboard.
// Usage: Use the 80 columns mode, or another input device than the keyboard for example.

#define H_QINL	0xFDE0	
// Call: At the beginning of Bios routine QINLIN (Main-ROM at 00B4h) that manages the line input with LINEINPUT instruction of Basic.
// Usage: Use the 80 columns mode, or another input device than the keyboard for example.

#define H_INLI	0xFDE5	
// Call: At the beginning of Bios routine INLIN (Main-ROM at 000B1h) that store the entered characters until STOP or RET key is pressed.
// Usage: Use the 80 columns mode, or another input device than the keyboard for example.

#define H_ONGO	0xFDEA	
// Call: At the beginning of the system routine that manages the Basic instructions like ON GOTO, ON GOSUB.
// Usage: Divert access to these instructions.

#define H_DSKO	0xFDEF	
// Call: At the beginning of Basic instruction DSKO$.
// Usage: The Disk-ROM uses this Hook to write the sector of a disk.
// Note: Used by the Disk-ROM.

#define H_SETS	0xFDF4	
// Call: At the beginning of Basic instruction SET.
// Usage: Divert access to the instruction or extend it.
// Note: On MSX1, the instruction SET has no other effect than calling this hook and returning an error. On MSX2 or newer, the instructions SET SCREEN, SET ADJUST, SET TIME, etc call this hook to be treated.

#define H_NAME	0xFDF9	
// Call: At the beginning of Basic instruction NAME.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_KILL	0xFDFE	
// Call: At the beginning of Basic instruction KILL.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_IPL	0xFE03	
// Call: At the beginning of Basic instruction IPL.
// Usage: Reserved (never used).
// Note: The basic instruction IPL only calls this hook. It can be used to create its own instruction.

#define H_COPY	0xFE08	
// Call: At the beginning of Basic instruction COPY.
// Usage: Divert/Modify/Process file copie to disk
// Note: Used by the Disk-ROM.

#define H_CMD	0xFE0D	
// Call: At the beginning of Basic instruction CMD.
// Usage: Reserved (never used).
// Note: The basic instruction CMD only calls this hook. It can be used to create its own instruction.

#define H_DSKF	0xFE12	
// Call: At the beginning of Basic instruction DSKF$.
// Usage: Divert/Modify/Process the instruction
// Note: Used by the Disk-ROM.

#define H_DSKI	0xFE17	
// Call: At the beginning of Basic instruction DSKI$.
// Usage: Divert/Modify/Process sector reading from the records
// Note: Used by the Disk-ROM.

#define H_ATTR	0xFE1C	
// Call: At the beginning of Basic instruction ATTR$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_LSET	0xFE21	
// Call: At the beginning of Basic instruction LSET$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_RSET	0xFE26	
// Call: At the beginning of Basic instruction RSET$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_FIEL	0xFE2B	
// Call: At the beginning of Basic instruction FIELD.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_MKI0xFE30	
// $	Call: At the beginning of Basic instruction MKI$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_MKS0xFE35	
// $	Call: At the beginning of Basic instruction MKS$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_MKD0xFE3A	
// $	Call: At the beginning of Basic instruction MKD$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_CVI	0xFE3F	
// Call: At the beginning of Basic instruction CVI$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_CVS	0xFE44	
// Call: At the beginning of Basic instruction CVS$.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_CVD	0xFE49	
// Call: At the beginning of Basic instruction CVD.
// Usage: Process the instruction.
// Note:Used by the Disk-ROM.

#define H_GETP	0xFE4E	
// Call: At the beginning of the routine GETPTR ("GET file PoinTeR"), positioning on a file.
// Usage: Process the routine.
// Note: Used by the Disk-ROM.

#define H_SETF	0xFE53	
// Call: At the beginning of the routine SETFIL ("SET FILe"), position a pointer on a previously opened file.
// Usage: Process the routine.

#define H_NOFO	0xFE58	
// Call: At the beginning of the routine NOFOR ("NO FOR clause"), used by Basic instruction OPEN of Basic.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM to process.

#define H_NULO	0xFE5D	
// Call: At the beginning of the routine NULOPN ("NULl file OPen"), used by Basic instructions LOAD, KILL, MERGE, etc.
// Usage: Process the routine.
// Note: Used by the Disk-ROM.

#define H_NTFL	0xFE62	
// Call: At the beginning of the routine NTFL0 ("NoT FiLe number 0"), used by Basic instructions LOAD, KILL, MERGE, etc.
// Usage: Process the routine.
// Note: Used by the Disk-ROM.

#define H_MERG	0xFE67	
// Call: At the beginning of the MERGE ("MERGE program files") routine, used by Basic instructions MERGE and LOAD.
// Usage: Process the routine.

#define H_SAVE	0xFE6C	
// Call: At the beginning of the routine SAVE, used by Basic instruction SAVE.
// Usage: Process the instruction.

#define H_BINS	0xFE71	
// Call: At the beginning of the routine BINSAR ("BINary SAVe") used by the instruction SAVE of Basic.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_BINL	0xFE76	
// Call: At the beginning of internal routine BINLOD ("BINary LOaD") used by the instruction LOAD of Basic.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_FILE	0xFE7B	
// Call: At the beginning of Basic instruction FILES.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_DGET	0xFE80	
// Call: At the beginning of internal routine DGET, used by the basic instruction GET/PUT.
// Usage: Process the instruction.
// Note: Used by the Disk-ROM.

#define H_FILO	0xFE85	
// Call: At the beginning of internal routine FILOU1 of file output.
// Usage: Process the device name.
// Note: Used by the Disk-ROM.

#define H_INDS	0xFE8A	
// Call: At the beginning of the internal routine INDSKC of disk attribute input.
// Usage: Process the routine.
// Note: Used by the Disk-ROM.

#define H_RSLF	0xFE8F	
// Call: At the beginning of the internal routine of re-selection of previous drive.
// Usage: Process the routine.

#define H_SAVD	0xFE94	
// Call: At the beginning of the internal routine of store the current drive, used by the LOF, LOC, EOF, FPOS instructions, etc.
// Usage: Process the routine.

#define H_LOC	0xFE99	
// Call: At the beginning of the internal routine LOC, used by the LOC instruction of the Basic.
// Usage:
// Note: Used by the Disk-ROM.

#define H_LOF	0xFE9E	
// Call: At the beginning of the internal routine LOF, used by the LOF instruction of the Basic.
// Usage: Process the instruction.

#define H_EOF	0xFEA3	
// Call: At the beginning of the internal routine EOF, used by the EOF instruction of the Basic.
// Usage: Process the instruction.

#define H_FPOS	0xFEA8	
// Call: At the beginning of the internal routine FPOS, used by the FPOS instruction of the Basic.
// Usage: Process the instruction.

#define H_BAKU	0xFEAD	
// Call: At the beginning of the internal routine BAKUPT.
// Usage: Process the instruction.

#define H_PARD	0xFEB2	
// Call: At the beginning of the internal routine PARDEV that parses the device name.
// Usage: Add a device name.

#define H_NODE	0xFEB7	
// Call: At the beginning of the internal routine NODEVN that is called when no name has been found in the device name table.
// Usage: Add a device name.

#define H_POSD	0xFEBC	
// Call: At the beginning of the internal routine POSDSK.
// Usage: Process the routine.

#define H_DEVN	0xFEC1	
// Call: At the beginning of the internal routine DEVNAM to process the device name.
// Usage: Process the routine.

#define H_GEND	0xFEC6	
// Call: At the beginning of the internal routine GENDSP to assign the device name.
// Usage: Process the routine.

#define H_RUNC	0xFECB	
// Call: At the beginning of the routine RUNC, used by the Basic instructions NEW and RUN.
// Usage: Allows the diversion of the instructions NEW and RUN.

#define H_CLEA	0xFED0	
// Call: At the beginning of the CLEARC routine that initializes the variables table, used by the CLEAR instruction of the Basic.
// Usage: Avoid erasing Basic variables, for example.

#define H_LOPD	0xFED5	
// Call: At the beginning of the internal routine LOPDFT, initialize the variable table, used by the CLEAR instruction of the Basic.
// Usage: Process the routine.

#define H_STKE	0xFEDA	
// Call: At the beginning of the routine STKERR (stack error), used by the CLEAR instruction of the Basic.
// Usage: Process the routine.
// Note: This hook is called after scaninging for executable ROMs in each slot when initializing the MSX, just before the system starts the Basic or DOS environment. This hook can therefore allow you to automatically re-execute your ROM after the disks are installed by placing the following routine.
// H.STKE:	
	// rst	030h		; Inter-slot call
	// db	1		; Slot number of the ROM
	// dw	ROM_Exe		; Address to execute the ROM with disks support
// Put this hook back to its original state once the ROM has been executed thanks to this hook.

#define H_ISFL	0xFEDF	
// Call: At the beginning of the internal routine ISFLIO that tests whether the file to write or read.
// Usage: Process the routine.

#define H_OUTD	0xFEE4	
// Call: At the beginning of the Bios routine OUTDO, output a screen character or printer.
// Usage: Process the routine.

#define H_CRDO	0xFEE9	
// Call: At the beginning of the CRDO routine that sends a CR (0Dh) and a LF (code 0Ah).
// Usage: Allows you to use a printer with an automatic line feed, for example.

#define H_DSKC	0xFEEE	
// Call: At the beginning of the internal routine DSKCHI, for the entry of the attribute of a disk.
// Usage: Process the routine.

#define H_DOGR	0xFEF3	
// Call: At the beginning of the internal routine DOGRPH, used by the instructions of the Basic of graphical tracing. (LINE, CIRCLE, ...)
// Usage: Process the graphical instructions.

#define H_PRGE	0xFEF8	
// Call: At the end of Basic program execution.
// Usage: Add a routine after the execution of Basic program.

#define H_ERRP	0xFEFD	
// Call: At the beginning of the routine ERRPRT that displays the error message under Basic.
// Change or add an error message.

#define H_ERRF	0xFF02	
// Call: At the end of the routine that displays the error message under Basic.
// Add a routine after the display of message.

#define H_READ	0xFF07	
// Call: At the beginning of the routine READY that displays the message "Ok" (or the one defined by the user on MSX2 or newer)
// Usage: Add a routine after the display of prompt ("OK").

#define H_MAIN	0xFF0C	
// Call: At the beginning of the MAIN routine, used at each access to the Basic interpreter.
// Usage: Allows you to program an interruption for example.

#define H_DIRD	0xFF11	
// Call: At the beginning of the DIRDO routine which is called when executing instructions in direct mode.
// Usage: Add a routine or prevent the executions.

#define H_FINI	0xFF16	
// Call: At the beginning of the routine FININT that initializes the interpretation of a basic instruction.
// Usage: Divert the processing of Basic instructions.

#define H_FINE	0xFF1B	
// Call: At the end of the routine that initializes the interpretation of a basic instruction.
// Usage: Add a routine.

#define H_CRUN	0xFF20	
// Call: At the beginning of the routine CRUNCH that transforms a Basic line into keywords.
// Usage: Process the routine.

#define H_CRUS	0xFF25	
// Call: At the beginning of the CRUSH routine that searches for a keyword in the alphabetical list in Rom.
// Usage: Process the routine.

#define H_ISRE	0xFF2A	
// Call: At the beginning of the ISRESV routine when a keyword is found by the CRUSH routine.
// Usage: Process the routine.

#define H_NTFN	0xFF2F	
// Call: At the beginning of the NTFN2 routine when a keyword is followed by a line number.
// Usage: Process the routine.

#define H_NOTR	0xFF34	
// Call: At the beginning of the NOTRSV routine when the sequence of characters examined by the CRUNCH routine is not a keyword.
// Usage: Process the routine.

#define H_SNGF	0xFF39	
// Call: At the beginning of basic instruction FOR.
// Usage: Process the instruction.

#define H_NEWS	0xFF3E	
// Call: At the end of process of a Basic instruction.
// Usage: Prepare the process of next instruction.

#define H_GONE	0xFF43	
// Call: At the beginning of the GONE2 routine, used by the jump instructions (GOTO, THEN, ...)
// Usage: Add a routine.

#define H_CNRG	0xFF48	
// Call: At the beginning of the CHRGET routine, enter a character on the keyboard.
// Usage: Use another Keyboard.

#define H_RETU	0xFF4D	
// Call: At the beginning of basic instruction RETURN.
// Usage: Change the instruction process.

#define H_PRTF	0xFF52	
// Call: At the beginning of basic instruction PRINT.
// Usage: Change the instruction process.

#define H_COMP	0xFF57	
// Call: At the beginning of internal routine COMPRT of basic instruction PRINT.
// Usage: Change the instruction process.

#define H_FINP	0xFF5C	
// Call: At the end of the text display under Basic.
// Usage: Add a routine after displaying a text.

#define H_TRMN	0xFF61	
// Call: When an input error with basic instruction READ/INPUT.
// Usage: Process the error.

#define H_FRME	0xFF66	
// Call: At the beginning of expression evaluator routine of Basic interpreter.
// Usage: Allows you to install new math routines.
// Entry: HL = Text pointer.
// Output: HL = Pointer to the found expression.
	// VALTYP (F663h) = Type of expression value.
	// DAC (F7F6h) = Value found.

#define H_NTPL	0xFF6B	
// Call: At the beginning of expression evaluator routine of Basic interpreter.
// Usage: Allows you to install new math functions.

#define H_EVAL	0xFF70	
// Call: At the beginning of expression evaluator routine of Basic interpreter.
// Usage: Allows you to install new math functions.

#define H_OKNO	0xFF75	
// Call: At the beginning of transcendental function routine of Basic interpreter.
// Usage: Allows you to install new math functions.
// Note: Removed from MSX Turbo R.

#define H_MDIN	0xFF75	
// Call: At the beginning of interruptions routine of Midi interface input.
// Usage: Process the routine.
// Note: Used on MSX Turbo R with internal Midi interface.

#define H_FING	0xFF7A	
// Call: At the beginning of factor evaluator routine of Basic interpreter.
// Usage:

#define H_ISMI	0xFF7F	
// Call: At the beginning of basic instruction MID$.
// Usage: Runloop execute

#define H_WIDT	0xFF84	
// Call: At the beginning of basic instruction WIDTH.
// Usage: Process the instruction.

#define H_LIST	0xFF89	
// Call: At the beginning of basic instruction LIST/LLIST.
// Usage: Process the instruction.

#define H_BUFL	0xFF8E	
// Call: When instructions LIST detokenise a basic instruction.
// Usage: Process the routine.

#define H_FRQINT	0xFF93	
// Call: At the beginning of frequency interrupt routine
// Usage: Process the routine.
// Note: Removed from MSX Turbo R.

#define H_MDTM	0xFF93	
// Call: At the beginning of the routine of Midi interface timer.
// Usage: Process the routine.
// Note: Only on MSX Turbo R with internal Midi interface.

#define H_SCNE	0xFF98	
// Call: At the beginning of the routine SCNEX2 from Basic interpretor, conversion of a line number to a memory address and vice versa.
// Usage: Process the routine.

#define H_FRET	0xFF9D	
// Call: At the beginning of the routine FRETMP from Basic interpretor, search for a free location to store the descriptor an alphanumeric variable.
// Usage: Process the routine.

#define H_PTRG	0xFFA2	
// Call: At the beginning of the routine PTRGET from Basic interpretor, to get the pointer to found a variable.
// Usage: Use other types of variables.

#define H_PHYD	0xFFA7	
// Call: At the beginning of the routine PHYDIO (Main-ROM at 0144h).
// Usage: Process the routine.
// Note: Used by the Disk-ROM.

#define H_FORM	0xFFAC	
// Call: At the beginning of the routine FORMAT (Main-ROM at 0147h).
// Usage: Process the routine to format a disk.
// Note: Used by the Disk-ROM.

#define H_ERRO	0xFFB1	
// Call: At the beginning of the error handler routine of Basic.
// Usage: Handle errors with your own routine.

#define H_LPTO	0xFFB6	
// Call: At the beginning of the routine LPTOUT (Main-ROM at 00A5h).
// Usage: Use of a non-MSX printer.

#define H_LPTS	0xFFBB	
// Call: At the beginning of the routine LPTSTT (Main-ROM at 00A5h).
// Usage: Use of a non-MSX printer.

#define H_SCRE	0xFFC0	
// Call: At the beginning of the instruction SCREEN of Basic.
// Usage: Extend the instruction for new SCREEN modes.

#define H_PLAY	0xFFC5	
// Call: Called at the beginning of the instruction PLAY of Basic.
// Usage: Extend the instruction for another sound chip.

#define E_TBIO	0xFFCA	
// Call: Nothing.
// Usage: This is not a hook. (See Extended Bios)

#define H_BGFD	0xFFCF	
// Call: Before physical operation with disk. (MSX-DOS1 only)
// Usage: Divert disk access.
// Note: Removed from MSX-DOS2 v2.3x. Replaced by the routine DISINT (See Extended Bios).

#define H_ENFD	0xFFD4	
// Call: After physical operation with disk. (MSX-DOS1 only)
// Usage: Add a routine after disk access.
// Note: Removed from MSX-DOS2 v2.3x. Replaced by the routine ENAINT (See Extended Bios).