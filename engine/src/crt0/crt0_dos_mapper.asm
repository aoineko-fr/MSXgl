; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; 
;──────────────────────────────────────────────────────────────────────────────
; Code address: 0x0100 (bootload) 0x4000 (program)
; Data address: 0xC000
;──────────────────────────────────────────────────────────────────────────────
.module	crt0

.include "defines.asm"
.include "macros.asm"

DOS_TPA   = 0x0006
DOS_TERM0 = 0x00
DOS_TERM  = 0x62
BDOS      = 0x0005
SETRAM    = 0xF36B

;==============================================================================
; RAM
;==============================================================================
	.area	_HEADER (ABS)
	.org	0x0100 ; MSX-DOS .com program start address

_g_FirstAddr::
_g_HeaderAddr::

.if APP_SIGN
	jr		crt0_init
_g_AppSignature::
	.dw		APP_SIGN_NAME
	.dw		APP_SIGN_ID

.endif
; .ifdef APP_SIGN_EXTRA
	; .db		APP_SIGN_EXTRA
; .endif

;------------------------------------------------------------------------------
; Initialization code
crt0_init:
	; Initialize
	call	SETRAM
	di

	; Initialize globals
	INIT_GLOBALS

;------------------------------------------------------------------------------
; Load application
crt0_load:

	ld		c, #1					; file counter (start at the 2nd file)
crt0_load_loop:
	call	crt0_load_file
	jp		z, crt0_start			; we reached the last segment if Z is true
	; Save current counter segment index (stored in A)
	ld		hl, #_g_SegTable
	ld		b, #0
	add		hl, bc
	ld		(hl), a
	; select 2 first segments in RAM
	; ...
	; check next file
	jp		crt0_load_loop

; load segment nmber C file into RAM mapper
crt0_load_file:
	call	crt0_getfilename
	ret

; build the file name according to C counter
crt0_getfilename:
	ret



;------------------------------------------------------------------------------
; Start application
crt0_start:
	; start main() function
	ei
	call	_main

	; Exit program and return value from register A
	ld		b, a
	ld		c, #DOS_TERM
	call	BDOS					; Try the MSX-DOS 2 termination function (_TERM)
	rst		0						; Otherwise, reset computer

	; List of application segment index in the RAM mpper
_g_SegTable::
	.ds		256

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_HOME
	.area	_CODE
	.area	_RODATA
	.area	_INITIALIZER 
	.area	_GSINIT
	.area	_GSFINAL
_g_LastAddr::

	.area	_DATA
_g_HeapStartAddress::
	.ds		2

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP