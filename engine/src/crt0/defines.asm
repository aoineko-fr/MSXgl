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
PPI_A				= #0xA8
VDP_S				= #0x99

;------------------------------------------------------------------------------
; Main-ROM variables
ROMVER				= #0x002B
MSXVER				= #0x002D

;------------------------------------------------------------------------------
; RAM work area variables
EXPTBL				= #0xFCC1
H_STKE				= #0xFEDA
SLTSL				= #0xFFFF

;------------------------------------------------------------------------------
; ROM Mappers
ROM_PLAIN			= 0
ROM_ASCII8			= 1
ROM_ASCII16			= 2
ROM_KONAMI			= 3
ROM_KONAMI_SCC		= 4

;------------------------------------------------------------------------------
; Default values
ADDR_BOOT			= 0x4000
ROM_MAPPER			= ROM_PLAIN
ROM_BDOS			= 0				; Install BDOS
ROM_BCALL			= 0				; Add banked call trampoline
ROM_RAMISR			= 0				; Install ISR in RAM
