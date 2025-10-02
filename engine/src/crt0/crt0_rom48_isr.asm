; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for 48/64KB ROM program with ISR replacement
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0x4000
; Data address: 0xC000
;──────────────────────────────────────────────────────────────────────────────
.module	crt0

.include "defines.asm"
.include "macros.asm"

;==============================================================================
; ROM
;==============================================================================
	.area	_HEADER (ABS)
	.org	0x0000

_g_FirstAddr::

;------------------------------------------------------------------------------
; ISR code
	.area   _DRIVER (ABS)
	.org	0x0038

	; ISR
	INCLUDE_ISR

;------------------------------------------------------------------------------
; Header
	.area	_HOME
	.area	_CODE

_g_HeaderAddr::
	; ROM header
	ROM_HEADER crt0_init

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
	; Backup ROM's slot ID
	ld		a, c
	ld		(#_g_ROMSlotID), a

	; Check for skip key press (if ROM_SKIP is set to 1)
	SKIP_BOOT

	; Delay startup using STKE hook (if ROM_DELAY is set to 1)
	DELAY_BOOT

	di
	; Set stack address at the top of free memory
	ld		sp, (HIMEM)

	; Initialize heap address
	ld		hl, #s__HEAP
	ld		(#_g_HeapStartAddress), hl

	; Backup Page 0 (Main-ROM) information
	BACKUP_ROMINFO

	; Set Page 0 & 2 at the same slot than the Page 1 one
	INIT_P1_TO_P02

	; Initialize globals
	INIT_GLOBALS

crt0_start:
	; start main() function
	ei
	call	_main
	rst		0

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_RODATA
	.area	_INITIALIZER 
	.area	_GSINIT
	.area	_GSFINAL
_g_LastAddr::

;==============================================================================
; RAM
;==============================================================================
	.area	_DATA

_g_HeapStartAddress::
	.ds		2

_g_ROMSlotID::
	.ds		1

	ALLOC_ROMINFO

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP