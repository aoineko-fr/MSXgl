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

; load segment number C file into RAM mapper
crt0_load_file:
	call	crt0_setfilename	; Set filename according to C counter "DATA??.SEG"

	ld		de, #crt0_fcb
	ld		c, #_FOPEN          ; Open file (FCB)
	call	BDOS                ; Open MSXDOS.SYS

	inc		a
	ret		z					; If open failed
	ld		de, #0x0100
	ld		c, #_SETDTA         ; Set disk transfer address
	call	BDOS

	ld		hl, #0x0001
	ld		(crt0_fcb_record), hl    ; Record size

	ld		hl, #0xC000         ; Number of records to read (48 KB)
	ld		de, #crt0_fcb       ; Pointer to opened FCB
	ld		c, #_RDBLK          ; Random block read (FCB)
	call	BDOS

	ret

; build the file name according to C counter
crt0_setfilename:
	ld		a, c					; Get current count (MSB)
	rlca
	rlca
	rlca
	rlca
	and		a, #0x0F
	ld		e, a
	ld		d, #0
	ld		hl, #crt0_hexatab
	add		hl, de
	ld		a, (hl)
	ld		hl, #crt0_fcb_filename + 4
	ld		(hl), a

	ld		a, c					; Get current count (LSB)
	and		a, #0x0F
	ld		e, a
	ld		hl, #crt0_hexatab
	add		hl, de
	ld		a, (hl)
	ld		hl, #crt0_fcb_filename + 5
	ld		(hl), a

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

;------------------------------------------------------------------------------
; Data

crt0_hexatab:
	.ascii	"0123456789ABCDEF"

	; file FCB
crt0_fcb:
	.db		0x00 ; 00             Drive number. 0=default drive
crt0_fcb_filename:
	.ascii	"DATA00  SEG" ; 8+3 characters
	.db		0x00 ; 0C             Extent number (low byte)
	.db		0x00 ; 0D             File attributes
crt0_fcb_record:
	.db		0x00 ; 0E             Record size (low byte) / Extent number (high byte)
	.db		0x00 ; 0F             Record size (high byte)
	.db		0x00 ; 10             File size (32 bits)
	.db		0x00 ; 11
	.db		0x00 ; 12
	.db		0x00 ; 13
	.db		0x00 ; 14             Volume-id (32 bits)
	.db		0x00 ; 15
	.db		0x00 ; 16
	.db		0x00 ; 17
	.db		0x00 ; 18             Internal information
	.db		0x00 ; 19
	.db		0x00 ; 1A
	.db		0x00 ; 1B
	.db		0x00 ; 1C
	.db		0x00 ; 1D
	.db		0x00 ; 1E
	.db		0x00 ; 1F
	.db		0x00 ; 20             Current record within extent
	.db		0x00 ; 21             Random record number
	.db		0x00 ; 22
	.db		0x00 ; 23
	.db		0x00 ; 24
	
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