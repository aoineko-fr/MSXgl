;______________________________________________________________________________
;   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄                                                        
;  ██ ▀ ██▀█ ▀█▄  ▀█▄▀                                                        
;  ▀█▄▀ ██ █ ▄▄█▀ ██ █                                                        
;______________________________________________________________________________
.module	crt0

;==============================================================================
; HELPER
;==============================================================================

;------------------------------------------------------------------------------
; Initialize globals
;------------------------------------------------------------------------------
.macro INIT_GLOBALS

	crt0_init_globals:
		ld		bc, #l__INITIALIZER
		ld		a, b
		or		a, c
		jp		z, crt0_skip_globals	
		ld		de, #s__INITIALIZED
		ld		hl, #s__INITIALIZER
		ldir
	crt0_skip_globals:

.endm

;------------------------------------------------------------------------------
; Set page #2 at the same slot than the page #1 (for 32K ROM)
;------------------------------------------------------------------------------
.macro INIT_P1_TO_P2

	crt0_p1_to_p2:
		; Set pages #1 & #3 primary slot equal to page #1 one
		in		a, (PPI_A)				; A=[P3|P2|P1|P0] Get primary slots info 
		ld		d, a					; D=[P3|P2|P1|P0] Backup original primary slots info
		and		a, #0b00001100			; A=[00|00|P1|00] Mask all pages slots but P1 
		ld		c, a					; C=[00|00|P1|00] Backup
		add		a, a					;                 A<<1
		add		a, a					; A=[00|P1|00|00] A<<1
		or		a, c					; A=[00|P1|P1|00] Merge P1 in P2
		add		a, a					;                 A<<1
		add		a, a					; A=[P1|P1|00|00] A<<1
		or		a, c					; A=[P1|P1|P1|00] Merge P1 in P2 & P3
		ld		c, a					; C=[P1|P1|P1|00] Backup
		ld		a, d					; A=[P3|P2|P1|P0] Restore primary slots info 
		and		a, #0b00000011			; A=[00|00|00|P0] Mask all pages slots but P0 
		or		a, c					; A=[P1|P1|P1|P0] Merge original P0 with P1 in all other pages
		out		(PPI_A), a				;                 Set primary slots info
		ld		e, a					; E=[P1|P1|P1|P0] Backup new primary slots

		; Set page #2 seconday slot equal to page #1 one
		ld		a, (SLTSL)				; A=[~3|~2|~1|~0] Read secondary slots register of selected primary slot
		cpl								; A=[S3|S2|S1|S0] Reverses the bits
		ld		b, a					; B=[S3|S2|S1|S0] Backup secondary slot
		and		a, #0b00001100			; A=[00|00|S1|00] Mask all pages slots but P1 
		ld		c, a					; C=[00|00|S1|00] Backup P1
		add		a, a					;                 P1<<1
		add		a, a					; A=[00|S1|00|00] P1<<1
		or		a, c					; A=[00|S1|S1|00] Merge P1 in P2
		ld		c, a					; C=[00|S1|S1|00]
		ld		a, b					; A=[S3|S2|S1|S0] Restore primary slots info 
		and		a, #0b11000011			; A=[S3|00|00|S0] Mask all pages slots but P0 
		or		a, c					; A=[S3|S1|S1|S0] Merge original S0 and S3 with S1 in other pages
		ld		(SLTSL), a				;                 Set secondary slot info

		; Restore initial page #3 primary slot
		ld		a, d					; A=[P3|P2|P1|P0] Restore initiale primary slots
		and		a, #0b11000011			; A=[P3|00|00|P0] Keep pages #0 and #3 slot
		ld		c, a					; B=[P3|00|00|P0] Backup
		ld		a, e					; A=[P1|P1|P1|P0] Restore new promary slots
		and		a, #0b00111100			; A=[00|P1|P1|00] Keep pages #1 and #2 slot
		or		a, c					; A=[P3|P1|P1|P0] Merge
		out		(PPI_A), a
.endm

;------------------------------------------------------------------------------
; Set pages #0 and #2 at the same slot than the page #1 (for 48K ROM)
;------------------------------------------------------------------------------
.macro INIT_P1_TO_P02

	crt0_p1_to_p02:
		; Set all pages primary slot equal to page #1 one
		in		a, (PPI_A)				; A=[P3|P2|P1|P0] Get primary slots info
		ld		d, a					; D=[P3|P2|P1|P0] Backup full slots info
		and		a, #0b00001100			; A=[00|00|P1|00] Mask all pages slots but P1 
		ld		c, a					; C=[00|00|P1|00] Backup P1
		rrca							;                 A>>1
		rrca							; A=[00|00|00|P1] A>>1
		or		a, c					; A=[00|00|P1|P1]
		ld		c, a					; C=[00|00|P1|P1] Backup 
		add		a, a					;                 A<<1
		add		a, a					; A=[00|P1|P1|00] A<<1
		add		a, a					;                 A<<1
		add		a, a					; A=[P1|P1|00|00] A<<1
		or		a, c					; A=[P1|P1|P1|P1] Merge
		out		(PPI_A), a				;                 Set primary slots info
		ld		e, a					; E=[P1|P1|P1|P1] Backup new slots

		; Set page #0 and 2 at the same secondary slot than page #1 one
		ld		a, (SLTSL)				; A=[~3|~2|~1|~0] Read secondary slots register of selected primary slot
		cpl								; A=[S3|S2|S1|S0] Reverses the bits
		ld		b, a					; B=[S3|S2|S1|S0] Backup secondary slot
		and		a, #0b00001100			; A=[00|00|S1|00] Mask all pages secondary slots but P1 
		ld		c, a					; C=[00|00|S1|00] Backup
		rrca							;                 A>>1
		rrca							; A=[00|00|00|S1] A>>1
		or		a, c					; A=[00|00|S1|S1] Merge
		ld		c, a					; C=[00|00|S1|S1] Backup
		add		a, a					;                 A<<1
		add		a, a					; A=[00|S1|S1|00] A<<1
		or		a, c					; A=[00|S1|S1|S1] Merge
		ld		c, a					; C=[00|S1|S1|S1] Backup
		ld		a, b					; A=[S3|S2|S1|S0] Restore primary slots info 
		and		a, #0b11000000			; A=[S3|00|00|00] Mask all pages slots but P0 
		or		a, c					; A=[S3|S1|S1|S0] Merge original P0 and P1 in all other page
		ld		(SLTSL), a				;                 Set secondary slot info

		; Restore initial Page 3 slot
		ld		a, d					; A=[P3|P2|P1|P0] Restore initiale primary slots
		and		a, #0b11000000			; A=[P3|00|00|00] Keep page #3 primary slot
		ld		c, a					; C=[P3|00|00|00] Backup
		ld		a, e					; A=[P1|P1|P1|P1] Restore new primary slots
		and		a, #0b00111111			; A=[00|P1|P1|P1] Keep all pages but page #3
		or		a, c					; A=[P3|P1|P1|P1] Merge
		out		(PPI_A), a				;                 Set primary slots info
.endm

;------------------------------------------------------------------------------
; Set page #0 at the same slot than the page #3 (replace Main-ROM by RAM)
; /!\ Need 64 KB of RAM
;------------------------------------------------------------------------------
.macro INIT_P3_TO_P0

	crt0_p3_to_p0:
		; Set pages #0 primary slot equal to page #3 one
		in		a, (PPI_A)				; A=[P3|P2|P1|P0] Get primary slots info 
		and		a, #0b11111100			; A=[P3|P2|P1|00] Mask P0 slot 
		ld		b, a					; B=[P3|P2|P1|00] Backup
		and		a, #0b11000000			; A=[P3|00|00|00] Mask all pages slots but P3 
		rlca							;                 A<<1
		rlca							; A=[00|00|00|P3] A<<1
		or		a, b					; A=[P3|P2|P1|P3] Merge
		out		(PPI_A), a				;                 Set primary slots info

		; Set page #0 seconday slot equal to page #3 one
		ld		a, (SLTSL)				; A=[~3|~2|~1|~0] Read secondary slots register of selected primary slot
		cpl								; A=[S3|S2|S1|S0] Reverses the bits
		and		a, #0b11111100			; A=[S3|S2|S1|00] Mask S0 slot 
		ld		b, a					; B=[S3|S2|S1|00] Backup
		and		a, #0b11000000			; A=[S3|00|00|00] Mask all pages slots but S3 
		rlca							;                 A<<1
		rlca							; A=[00|00|00|S3] A<<1
		or		a, b					; A=[S3|S2|S1|S3] Merge
		ld		(SLTSL), a				;                 Set secondary slot info
.endm

;------------------------------------------------------------------------------
; Install BDOS
;------------------------------------------------------------------------------
.macro INSTALL_BDOS
	.if ROM_BDOS
	; Setup the hook H.STKE to run the ROM with disk support
	crt0_bdos_install:
		ld		a, c					; Get the ROM slot number
		ld		hl, #crt0_bdos_hook
		ld		de, #H_STKE
		ld		bc, #4
		ldir							; Copy the routine to execute the ROM to the hook
		ld		(H_STKE+1), a			; Put the ROM slot number to the hook
		ret								; Back to slots scanning

	; Routine to execute the ROM
	crt0_bdos_hook:
		rst		0x30					; Inter-slot call
		.db		1						; This byte will be replaced by the slot number of ROM
		.dw		crt0_bdos_end			; Address to execute the ROM

	; Remove the hook and resume the ROM boot sequence
	crt0_bdos_end:
		ld		a, #0xC9				; 'ret' instruction
		ld		(H_STKE), a
	.endif
.endm

;------------------------------------------------------------------------------
; Install ISR in RAM
; (needs 64 KB of RAM in Page #3's slot)
;------------------------------------------------------------------------------
.macro INSTALL_RAM_ISR
	.if ROM_RAMISR
		.globl	_VDP_InterruptHandler

		di
		jp		crt0_interrupt_end
		
	_VDP_InterruptHandler::
		ret

	crt0_interrupt_start::
		; Skip interruptions that do not come from the VDP.
		push	af
		in		a, (VDP_S)
		and		a
		jr		nz, crt0_interrupt_skip
		; Backup registers
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
		; Call VDP interruption handler
		call    _VDP_InterruptHandler
		; Restore registers
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
		; Restore registers
	crt0_interrupt_skip:
		pop		af
		ei
		reti
	crt0_interrupt_end:

	; Switch page 0 to RAM
		INIT_P3_TO_P0

	; Copy ISR to RAM
		ld		bc, #crt0_interrupt_end-crt0_interrupt_start
		ld		hl, #crt0_interrupt_start
		ld		de, #0x0038
		ldir
		
		ei

	.endif
.endm


;------------------------------------------------------------------------------
; Add banked call trampoline
;------------------------------------------------------------------------------
.macro SUPPORT_BANKED_CALL
	.if ROM_BCALL
	; Set segment (get value from register A)
	set_bank::
		.ifeq ROM_MAPPER-ROM_ASCII16
		ld		(BANK1_ADDR), a ; Bank 1 (8000h)
		.else
		ld		(BANK2_ADDR), a ; Bank 2 (8000h)
		.endif
		ld		(_g_CurrentSegment), a
		ret

	; Get segment (return value in register A)
	get_bank::
		ld		a, (_g_CurrentSegment)
		ret
	.endif
.endm


;==============================================================================
; ROM MAPPER
;==============================================================================

;------------------------------------------------------------------------------
; ROM_PLAIN
;------------------------------------------------------------------------------
.ifeq ROM_MAPPER-ROM_PLAIN
	.macro INIT_MAPPER
	.endm
.endif
;------------------------------------------------------------------------------
; ROM_ASCII8
;------------------------------------------------------------------------------
.ifeq ROM_MAPPER-ROM_ASCII8
	BANK0_ADDR = #0x6000
	BANK1_ADDR = #0x6800
	BANK2_ADDR = #0x7000
	BANK3_ADDR = #0x7800

	.macro INIT_MAPPER
		xor		a
		ld		(BANK0_ADDR), a ; Segment 0 in Bank 0
		inc		a
		ld		(BANK1_ADDR), a ; Segment 1 in Bank 1
		inc		a
		ld		(BANK2_ADDR), a ; Segment 2 in Bank 2
		.if ROM_BCALL
		ld		(_g_CurrentSegment), a
		.endif
		inc		a
		ld		(BANK3_ADDR), a ; Segment 3 in Bank 3

	.endm
.endif
;------------------------------------------------------------------------------
; ROM_ASCII16
;------------------------------------------------------------------------------
.ifeq ROM_MAPPER-ROM_ASCII16
	BANK0_ADDR = #0x6000
	BANK1_ADDR = #0x77FF

	.macro INIT_MAPPER
		xor		a
		ld		(BANK0_ADDR), a ; Segment 0 in Bank 0
		inc		a
		ld		(BANK1_ADDR), a ; Segment 1 in Bank 1
		.if ROM_BCALL
		ld		(_g_CurrentSegment), a
		.endif
	.endm
.endif
;------------------------------------------------------------------------------
; ROM_KONAMI
;------------------------------------------------------------------------------
.ifeq ROM_MAPPER-ROM_KONAMI
	BANK1_ADDR = #0x6000
	BANK2_ADDR = #0x8000
	BANK3_ADDR = #0xA000

	.macro INIT_MAPPER
		ld		a, #1
		ld		(BANK1_ADDR), a ; Segment 1 in Bank 1
		inc		a
		ld		(BANK2_ADDR), a ; Segment 2 in Bank 2
		.if ROM_BCALL
		ld		(_g_CurrentSegment), a
		.endif
		inc		a
		ld		(BANK3_ADDR), a ; Segment 3 in Bank 3
	.endm
.endif
;------------------------------------------------------------------------------
; ROM_KONAMI_SCC
;------------------------------------------------------------------------------
.ifeq ROM_MAPPER-ROM_KONAMI_SCC
	BANK0_ADDR = #0x5000
	BANK1_ADDR = #0x7000
	BANK2_ADDR = #0x9000
	BANK3_ADDR = #0xB000

	.macro INIT_MAPPER
		xor		a
		ld		(BANK0_ADDR), a ; Segment 0 in Bank 0
		inc		a
		ld		(BANK1_ADDR), a ; Segment 1 in Bank 1
		inc		a
		ld		(BANK2_ADDR), a ; Segment 2 in Bank 2
		.if ROM_BCALL
		ld		(_g_CurrentSegment), a
		.endif
		inc		a
		ld		(BANK3_ADDR), a ; Segment 3 in Bank 3
	.endm
.endif
