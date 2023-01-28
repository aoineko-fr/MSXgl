; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 header for MSX-DOS program
;──────────────────────────────────────────────────────────────────────────────
; Based on work from 'Konamiman' and 'Avelino' (11/2004)
;  https://github.com/Konamiman/MSX/blob/master/SRC/SDCC/crt0-msxdos/crt0msx_msxdos.asm
;
; Code address: 0x0100
; Data address: 0		(right after code)
;------------------------------------------------------------------------------
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

	.area	_HOME
	.area	_CODE

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
	call	SETRAM

	di
	; Set stack address at the top of free memory
	; ld		sp, (DOS_TPA)
	
	; Initialize globals
	INIT_GLOBALS

	.if DOS_PARSEARG
		;--- Step 2: Build the parameter pointers table on 0x100,
		;    and terminate each parameter with 0.
		;    MSX-DOS places the command line length at 0x80 (one byte),
		;    and the command line itself at 0x81 (up to 127 characters).

		;* Check if there are any parameters at all
		ld		a, (#0x0080)			; Get parameters line length
		or		a
		ld		c, #0					; Initialize parameters counter
		jr		z, crt0_start			; Skip parameters parsing

		;* Terminate command line with 0
		;  (DOS 2 does this automatically but DOS 1 does not)
		ld		hl, #0x0081
		ld		bc, (#0x0080)
		ld		b, #0
		add		hl, bc
		ld		(hl), #0

		;* Copy the command line processing code to 0xC000 and
		;  execute it from there, this way the memory of the original code
		;  can be recycled for the parameter pointers table.
		;  (The space from 0x100 up to "cont" can be used,
		;   this is room for about 40 parameters.
		;   No real world application will handle so many parameters.)
		ld		hl, #crt0_param_start
		ld		de, #0xC000
		ld		bc, #crt0_param_end-#crt0_param_start
		ldir

		;* Initialize registers and jump to the loop routine
		ld		hl, #0x0081				; Command line pointer
		ld		c, #0					; Number of params found
		ld		ix, #0x100				; Params table pointer

		ld		de, #crt0_start			; To continue execution at "cont"
		push	de						; when the routine RETs
		jp		0xC000

		;>>> Command line processing routine begin
	crt0_param_start:

		;* Loop over the command line: skip spaces
	crt0_param_loop:
		ld		a, (hl)
		or		a						; Command line end found?
		ret		z

		cp		#32
		jr		nz, crt0_param_found
		inc		hl
		jr		crt0_param_loop

		;* Parameter found: add its address to params table...
	crt0_param_found:
		ld		0(ix), l
		ld		1(ix), h
		inc		ix
		inc		ix
		inc		c

		ld		a, c					; protection against too many parameters
		cp		#40
		ret		nc

		;* ...and skip chars until finding a space or command line end
	crt0_param_loop2:
		ld		a, (hl)
		or		a						; Command line end found?
		ret		z

		cp		#32
		jr		nz, crt0_param_nospc	; If space found, set it to 0
										; (string terminator)...
		ld		(hl), #0
		inc		hl
		jr		crt0_param_loop			;...and return to space skipping loop

	crt0_param_nospc:
		inc		hl
		jr		crt0_param_loop2

	crt0_param_end:
		;>>> Command line processing routine end

	crt0_start:
		; Start main(argc, argv) function
		ei
		ld		a, c					; Parameters number
		ld		de, #0x100				; Parameters 
		call	_main

	.else ; !DOS_PARSEARG

	crt0_start:
		; Start main() function
		ei
		call	_main

	.endif

	; Exit program and return value from register A
	ld		b, a
	ld		c, #DOS_TERM
	call	BDOS					; Try the DOS-2 termination function (_TERM)
	ld		c, #DOS_TERM0
	jp		BDOS					; Otherwise, try the DOS-1 termination function (0)

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_RODATA
	.area	_INITIALIZER 
	.area   _GSINIT
	.area   _GSFINAL
_g_LastAddr::

	.area	_DATA
_g_HeapStartAddress::
	.dw		s__HEAP

;------------------------------------------------------------------------------
; Ordering of segments for the linker
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP