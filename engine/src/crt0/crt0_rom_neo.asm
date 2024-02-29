; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for mapped ROM program
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0x4010 (right after the header)
; Data address: 0xC000
;──────────────────────────────────────────────────────────────────────────────
.module	crt0

.include "defines.asm"
.include "macros.asm"

;==============================================================================
; ROM
;==============================================================================
	.area	_HEADER (ABS)
	.org	START_ADDR

;------------------------------------------------------------------------------
; Header
	.area	_HOME
	.area	_CODE

_g_FirstAddr::
_g_HeaderAddr::
	; ROM header
	ROM_HEADER crt0_init

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
	; Delay startup using STKE hook (if ROM_DELAY is 1)
	DELAY_BOOT

	di
	; Set stack address at the top of free memory
	ld		sp, (HIMEM)

	; Initialize heap address
	ld		hl, #s__HEAP
	ld		(#_g_HeapStartAddress), hl

	; Backup Page 0 (Main-ROM) information
	ld		a, (ROMVER)
	ld		(#_g_VersionROM), a
	ld		a, (MSXVER)
	ld		(#_g_VersionMSX), a

	; ; Set Page 2 slot equal to Page 1 slot
	; INIT_P1_TO_P2
	; Set Page 0 & 2 at the same slot than the Page 1 one
	INIT_P1_TO_P02
	
	; Initialize ROM mapper segment
	INIT_MAPPER

	; ; Install ISR in RAM
	; INSTALL_RAM_ISR

	; Initialize globals
	INIT_GLOBALS

crt0_start:
	; start main() function
	ei
	call	_main
	rst		0

	; Add banked call trampoline
	SUPPORT_BANKED_CALL

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_RODATA
	.area	_INITIALIZER 
	.area	_GSINIT
	.area	_GSFINAL
_g_LastAddr::

;------------------------------------------------------------------------------
; ISR code
	.area   _DRIVER (ABS)
.ifeq ROM_MAPPER-ROM_NEO8
	.org	0x00040038
.endif
.ifeq ROM_MAPPER-ROM_NEO16
	.org	0x00020038
.endif

	; ISR
	INCLUDE_ISR
	
;==============================================================================
; RAM
;==============================================================================
	.area	_DATA

_g_HeapStartAddress::
	.ds		2
_g_VersionROM::
	.ds		1
_g_VersionMSX::
	.ds		1

	ALLOC_MAPPER

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP