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
PPI_B			= #0xA9
PPI_C			= #0xAA
PPI_MODE		= #0xAB
VDP_S			= #0x99
VDP_A			= #0x99
V9_P06			= #0x66

;------------------------------------------------------------------------------
; Main-ROM routines
ENASLT			= #0x0024 ; Switches indicated slot at indicated page on perpetually
ROMVER			= #0x002B ; Basic ROM version
MSXVER			= #0x002D ; MSX version number
INITXT			= #0x006C ; Switches to SCREEN 0
SETTXT			= #0x0078 ; Switches VDP to SCREEN 0 mode
CHGET			= #0x009F ; One character input (waiting)
CHPUT			= #0x00A2 ; Displays one character
CLS				= #0x00C3 ; Clears the screen
RSLREG			= #0x0138 ; Reads the primary slot register

;------------------------------------------------------------------------------
; RAM work area variables
LINL40			= #0xF3AE
HIMEM			= #0xFC4A
EXPTBL			= #0xFCC1
H_STKE			= #0xFEDA
SLTSL			= #0xFFFF

;------------------------------------------------------------------------------
; MSX version
MSX1			= 0 ; MSX 1
MSX2			= 1 ; MSX 2
MSX2P			= 2 ; MSX 2+
MSXTR			= 3 ; MSX turbo R
MSX2PP			= 4 ; MSX 2++
MSX2S			= 5 ; MSX 2#

;------------------------------------------------------------------------------
; Row #7 keys
KEY_F4			= #0b00000001
KEY_F5			= #0b00000010
KEY_ESC			= #0b00000100
KEY_TAB			= #0b00001000
KEY_STOP		= #0b00010000
KEY_BS			= #0b00100000
KEY_SELECT		= #0b01000000
KEY_RETURN		= #0b10000000

;------------------------------------------------------------------------------
; ROM types
ROM_PLAIN		= 0		; Plain ROM (no mapper)
ROM_MAPPER_8K	= 2		; Mapper 8 KB
ROM_ASCII8		= 2		;  ASCII-8 mapper
ROM_KONAMI		= 3		;  Konami without SCC mapper
ROM_KONAMI_SCC	= 4		;  Konami with SCC mapper
ROM_NEO8		= 5		;  NEO-8 mapper
ROM_YAMANOOTO	= 6		;  Yamanooto mapper
ROM_MAPPER_16K	= 10	; Mapper 16 KB
ROM_ASCII16		= 10	;  ASCII-16 mmaper
ROM_NEO16		= 11	;  NEO-16 mapper
ROM_ASCII16X	= 12	;  ASCII16-X mmaper

;------------------------------------------------------------------------------
; Yamanooto registers types
YAMA_ENAR		= #0x7FFF		; Features enable register
YAMA_ENAR_REGEN	= #0b00000001	; Registers enable. Setting this bit allows writing to all other registers and all register readability.
YAMA_ENAR_WREN	= #0b00010000	; Write enable. Set to 1 to enable writes to flash rom.
YAMA_OFFR		= #0x7FFE		; Mapper offset register
YAMA_CFGR		= #0x7FFD		; Configuration and control register
YAMA_CFGR_MDIS	= #0b00000001	; This bit disables mapping so you avoid mapper changes with small (up to 32 kbyte) roms that poke the switching area. Usually this is a problem only in K4 mode. Remember to reset this bit to make changes again.
YAMA_CFGR_ECHO	= #0b00000010	; This bit allows the built-in PSG to respond to the port number of the internal PSG of the MSX, causing music intended for the internal PSG to be played too in the Yamanooto and be heard through the stereo output of the cartridge. This is set only during boot when you press the HOME key.
YAMA_CFGR_ROMDIS= #0b00000100	; Setting this bit disables access to flash rom. This is automatically set during boot when the DEL key is pressed. You need to clear this bit in software to be able read/write the flash rom.
YAMA_CFGR_K4	= #0b00001000	; Changes mapper configuration from Konami5 (SCC) to Konami4 for compatibility with game compilations including non-SCC games.

;------------------------------------------------------------------------------
; ISR types
ISR_NONE		= 0				; No ISR
ISR_ALL			= 1				; Handle any interruption
ISR_VBLANK		= 2				; V-blank handler (V9938 or V9958)
ISR_VHBLANK		= 3				; V-blank and h-blank handler (V9958)
ISR_V9990		= 4				; V-blank, h-blank and command end handler (V9990)

;------------------------------------------------------------------------------
; Installation in RAM's page 0
RAM0_NONE		= 0				; Don't install anything in RAM 
RAM0_ISR		= 1				; Install only ISR
RAM0_SEGMENT	= 2				; Install ISR and segment data (for mapped-ROM)

;------------------------------------------------------------------------------
; Default values
START_ADDR		= 0x4000		; Program start address
ROM_MAPPER		= ROM_PLAIN		; ROM type
ROM_SKIP		= 0				; Skip boot with a key press
ROM_SKIP_KEY	= KEY_ESC		; Skip key to check
ROM_DELAY		= 0				; Install BDOS
ROM_BCALL		= 0				; Add banked call trampoline
ROM_ISR			= ISR_VBLANK	; ISR type
ROM_RAMISR		= RAM0_NONE		; Install ISR in RAM's page 0
ISR_SET_S0		= 0				; Reset statut register to S#0 in ISR
APP_SIGN		= 0				; Add application signature
APP_SIGN_NAME	= "GL"			; Application creator/group/company name
APP_SIGN_ID		= 0x0000		; Application number
DOS_PARSEARG	= 0				; Parse MSX-DOS command-line arguments
DOS_ISR			= 0				; Install ISR for MSX-DOS
CHECK_MSX		= MSX1			; Minimum MSX version required

;------------------------------------------------------------------------------
; Project overwrite values; File generated by the build tool
.include "crt0_config.asm"