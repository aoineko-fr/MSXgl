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

.globl	_VDP_InterruptHandler

HIMEM  = #0xFC4A

;------------------------------------------------------------------------------
.area	_HEADER (ABS)

	.org	0x0000

_g_FirstAddr::

;------------------------------------------------------------------------------
.area   _DRIVER (ABS)

	.org	0x0038

_interrupt_start::

	push	af

	in		a, (VDP_S)
	and		a
	jp		p, _interrupt_end

	push	hl
	push	de
	push	bc
	exx
	ex		af, af'
	push	af
	push	hl
	push	de
	push	bc
	push	iy
	push	ix

	call    _VDP_InterruptHandler

	pop		ix
	pop		iy
	pop		bc
	pop		de
	pop		hl
	pop		af
	ex		af, af'
	exx
	pop		bc
	pop		de
	pop		hl
	
_interrupt_end:		
	pop		af

	ei
	reti


;------------------------------------------------------------------------------
.area	_CODE

_g_HeaderAddr::
	; ROM header
	.db		0x41 ; A
	.db		0x42 ; B
	.dw		crt0_init
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000
	.dw		0x0000

crt0_init:
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

;-- ROM --
.area	_HOME
.area	_CODE
.area	_RODATA
.area	_INITIALIZER 
.area   _GSINIT
.area   _GSFINAL
_g_LastAddr::

;-- RAM --
.area	_DATA
_g_HeapStartAddress::
	.ds 2
_g_VersionROM::
	.ds 1
_g_VersionMSX::
	.ds 1

.area	_INITIALIZED
.area	_BSEG
.area   _BSS
.area   _HEAP
