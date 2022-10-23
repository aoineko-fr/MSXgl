; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; crt0 helper defines
;------------------------------------------------------------------------------
; By Guillaume 'Aoineko' Blanchard for MSX Game Library 
; (ɔ) 2022 under CC-BY-AS license
;------------------------------------------------------------------------------
.module	crt0

;------------------------------------------------------------------------------
; Global symbols
.globl _main
.globl l__INITIALIZER
.globl s__INITIALIZED
.globl s__INITIALIZER
.globl s__HEADER
.globl s__GSFINAL
.globl s__HEAP

;------------------------------------------------------------------------------
; I/O ports
PPI_A			= #0xA8
VDP_S			= #0x99
VDP_A			= #0x99
V9_P06			= #0x66

;------------------------------------------------------------------------------
; Main-ROM routines
ENASLT			= #0x0024
ROMVER			= #0x002B
MSXVER			= #0x002D
RSLREG			= #0x0138

;------------------------------------------------------------------------------
; RAM work area variables
HIMEM			= #0xFC4A
EXPTBL			= #0xFCC1
H_STKE			= #0xFEDA
SLTSL			= #0xFFFF

;------------------------------------------------------------------------------
; ROM Mappers
ROM_PLAIN		= 0
ROM_ASCII8		= 1
ROM_ASCII16		= 2
ROM_KONAMI		= 3
ROM_KONAMI_SCC	= 4

;------------------------------------------------------------------------------
; ISR
ISR_VBLANK		= 0				; V-blank handler (V9938 or V9958)
ISR_VHBLANK		= 1				; V-blank and h-blank handler (V9958)
ISR_V9990		= 2				; V-blank, h-blank and command end handler (V9990)

;------------------------------------------------------------------------------
; Default values
START_ADDR		= 0x4000
ROM_MAPPER		= ROM_PLAIN
ROM_BDOS		= 0				; Install BDOS
ROM_BCALL		= 0				; Add banked call trampoline
ROM_ISR			= ISR_VBLANK
ROM_RAMISR		= 0				; Install ISR in RAM
ISR_SET_S0		= 0				; Reset statut register to S#0 in ISR
APP_SIGN		= 0				; Add application signature
APP_SIGN_NAME	= "GL"			; Application creator/group/company name
APP_SIGN_ID		= 0x0000		; Application number