; ____________________________
; ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
;──────────────────────────────────────────────────────────────────────────────
; DM-system2  [Device Driver for あたっちゃって２５％]
;(C)1995 GIGAMIX/Influenza

;PCCM Ninja-Tap Driver v2.0  for MSX
;	(Primary & C-lib routine)
;Copyright 1994 1995 Knight`s Chamber,
;		 all rights reserved.
;	Written by Firm Knight.
;======================================
; DEVDRV	EQU	7800H	;0C000H
; NTPTBL	EQU	DEVDRV+1F6H	;10 Bytes
; DMMDEV	EQU	4300H+65

	; ORG	DEVDRV

; ;		 0....5....10...15...20...25...30...35...
	; DEFM	'DEV:あたっちゃって２５％    by Influenza'
	; DEFB	1AH,02H		;EOF,DEVDRV ID
	; DEFS	6

	; JP	DEVINI
	; JP	DEVMOU
	; JP	DEVSTK
	; JP	DEVTRG

	; JP	GET10P

; ;	JP	GTNTAP		;GetDAT
; ;	JP	CKNTAP		;Check

; ;======================================
; ; Data Area
; RESULT:	DEFB	0	;Connect Flag
; MAX:	DEFB	0	;Max Player
; ID1:	DEFB	0	;ID(Port1)
; ID2:	DEFB	0	;ID(Port2)

; ;======================================
; ; Get 4 Players Inkey
; ;[E]	None
; ;[R]	HL= Table Address

; GET10P:
	; CALL	GETALL
	; LD	HL,NTPTBL
	; RET

; ;======================================
; DEVINI:
	; LD	A,(002DH)
	; CP	3
	; JR	C,DEVIN1
	; LD	HL,GTNTA0
	; LD	(SWGETA+1),HL
; DEVIN1:
	; LD	HL,NTPTBL
	; LD	B,10
; DEVIN0:
	; LD	(HL),255
	; INC	HL
	; DJNZ	DEVIN0
	; JP	CKNTAP

; ;==================
; DEVMOU:
	; LD	HL,0000
	; RET

; ;==================
; DEVSTK:
	; CALL	GETALL
	; LD	C,0F0H
	; CALL	SRCH
	; RET	Z
	; AND	0FH
	; LD	HL,STKTBL
	; ADD	A,L
	; LD	L,A
	; LD	A,(HL)
	; RET

; STKTBL:
	; DEFB	0,7,3,0,1,8,2,1,5,6,4,5,0,7,3,0

; ;================
; DEVTRG:
	; CALL	GETALL
	; LD	C,3FH
	; CALL	SRCH
	; RET	Z
	; RLA
	; LD	A,9
	; RET	NC
	; INC	A
	; RET

; ;======================================
; ; Search a First Input
; ;[E]	C:Mask
; ;[R]	A=Inkey Condition
; ;	Z on=Nobody Input

; SRCH:
	; LD	HL,NTPTBL
	; LD	B,10
; SRCH0:
	; LD	A,(HL)
	; OR	C
	; INC	A
	; JR	NZ,SRCH1
	; INC	HL
	; DJNZ	SRCH0
	; LD	A,255
	; LD	(DMMDEV),A
	; XOR	A
	; RET
; SRCH1:
	; LD	A,10
	; SUB	B
	; LD	(DMMDEV),A
	; LD	A,(HL)
	; RET

; ;======================================
; ; Get All Device
; ;[E]	None
; ;[R]	(NTPTBL,10)=Result

; GETALL:
	; LD	HL,MATTBL
	; CALL	GETKEY
	; LD	A,B
	; LD	(NTPTBL),A
	; CALL	GETKEY
	; LD	HL,NTPTBL+1
	; LD	(HL),B
	; INC	HL
; SWGETA:
	; JP	GTNTAP

; ;======================================
; ; Get Key Matrix
; ;[E]	HL:MATTBL
; ;[R]	B:Inkey Condition

; GETKEY:
	; LD	D,0FBH
	; LD	BC,01FFH
; GETKE0:
	; LD	E,(HL)
	; INC	HL
	; LD	A,(DE)
	; AND	(HL)
	; INC	HL
	; ADD	A,C
	; RL	B
	; JR	NC,GETKE0
	; RET

;======================================
; Get Inkey
;[E]	hl :Pointer to NTAP Table
;[R]	None
;[M]	af,bc,de,hl
;ofset +0 (A)(B)( )( )(U)(D)(L)(R)

GTNTAP:
	di
	; ld		A,(RESULT)
	ld		a, (_g_NTap_Buffer) ; 
	rrca			;Port 1
	ld		de, #0xAF20
	call	nc, GETJOY
	ld		de, #0xBA3A
	call	c, GETNIN
	rrca			;Port 2
	ld		de, #0x9F50
	call	nc, GETJOY
	ld		de, #0xFA7A
	call	c, GETNIN
	ei
	ret

; ; For Turbo R
; GTNTA0:
	; CALL	0183H
	; PUSH	AF
	; XOR	A
	; CALL	0180H
	; CALL	GTNTAP
	; POP	AF
	; JP	0180H

;======================================
GETJOY:
	push	af
; Select Port1/Port2
	ld		a, #15
	out		(0xA0), a
	in		a, (0xA2)
	and		d
	or		e
	out		(0xA1), a
; Get key Status
	ld		a, #14
	out		(0xA0), a
	in		a, (0xA2)
; Write Status to (HL)
	ld		e, #0xFF
	bit		5, a
	jr		nz, GETJO0
	res		2, e		;Trig 2
GETJO0:
	bit		4, a
	jr		nz, GETJO1
	res		3, e		;Trig 1
GETJO1:
	rra
	rl		e
	rra
	rl		e
	rra
	rl		e
	rra
	rl		e		;lever
	ld		(hl),e
	inc		hl

	pop		af
	ret

;======================================
; NTAP Access
GETNIN:
	push	af
	push	hl
; select port and 8pin=h,6pin=L
	ld		a, #15
	out		(0xa0), a
	in		a, (0xa2)
	and		d
	or		e
	out		(0xA1), a
; 6pin=h (4021 data read)
	or		#0x05
	out		(0xA1), a
; 6pin=l (4021 transfer mode)
	and		#0xFA
	out		(0xA1), a
;/// get key status //////
	ld		b, #8
GETNI0:
; Send me Data
	ld		a, #15
	out		(0xA0), a
	in		a, (0xA2)
	or		#0x30
	out		(0xA1),a	;8=h
	and		#0xCF
	out		(0xA1),a	;8=l

	ld		a, #14
	out		(0xA0), a
	in		a, (0xA2)	;read
	rra
	rl		h		;joy1
	rra
	rl		e		;joy2
	rra
	rl		d		;joy3
	rra
	rl		c		;joy4
	djnz	GETNI0
;/////////////////////////
	ld		a,h
	pop		hl
	ld		(hl),a		;joy1
	inc		hl
	ld		(hl),e		;joy2
	inc		hl
	ld		(hl),d		;joy3
	inc		hl
	ld		(hl),c		;joy4
	INC		HL
; Restore Port (6,7,8=H)
	ld		de, #0xFF3F
	call	PORSEL
;	LD	A,15
;	OUT	(0xA0),A
;	IN	A,(0xA2)
;	OR	3FH
;	OUT	(0xA1),A
	pop		af
	ret

;======================================
; Connection Check
;[E]	None
;[R]	(NTAP Info)
;[M]	af,bc,de,hl,ix

;NTAP Info
;ofset	-1 Result(Internal Work)
;	+0 Max Player
;	+1 ID(Port1),0-15	(Dummy)
;	+2 ID(Port2),NotAP=255	(Dummy)

CKNTAP:
	di
	ld		b, #0	; b=Max Player
	ld		de, #0xBF00
	call	CHECK	; Port1 Check
	ld		c, a
	ld		e, #0x40
	call	CHECK	; Port2 Check
	rlca
	or		c
; Result Save
	ld		c, a
	; ld		(RESULT), bc
	ld		(_g_NTap_Buffer), bc
; Port1 ID Check
;	LD		H,0FFH
;	RRCA
;	LD		DE,0BA3AH
;	CALL	 C,GETID
;	RRCA
;	LD		A,H
;	LD		(RESULT+2),A
; Port2 ID Check
;	LD		H,0FFH
;	LD		DE,0FA7AH
;	CALL	 C,GETID
;	LD		A,H
;	LD		(RESULT+3),A
; Restore Port (6,7,8=H)
	ld		de, #0xFF3F
	jp		PORSEL
;	EI
;	RET

;======================================
; Connection Check Sub.
CHECK:
	call	PORSEL
	inc		b
	and		#0xC0
	out		(0xA1), a	; 678=L
	ex		af, af'		;'
	ld		a, #14
	out		(0xA0), a
	in		a, (0xA2)
	and		#0x20		; 7=H ?
	ret		z

	ld		a, #15
	out		(0xA0), a
	ex		af, af'		;'
	or		#0x30
	out		(0xA1), a	; 8=H
	ld		a, #14
	out		(0xA0), a
	in		a, (0xA2)
	and		#0x20		; 7=L ?
	jr		nz, CHECK1

	inc		a
	inc		b
	inc		b
	inc		b
	ret

; No NTAP
CHECK1:
	xor		a
	ret

;======================================
; Get ID
;GETID:
;	PUSH	AF
; 6=H 8=H
;	CALL	PORSEL
; 6=H 4021 Data Load
;	OR	05H
;	OUT	(0xA1),A
; 6=L 4021 Transfer Mode
;	AND	0FAH
;	OUT	(0xA1),A
;
;	LD	B,9
;/// Waste 8 Data and Get 9th Data ////
;GETID0:
;	LD	A,15
;	OUT	(0xA0),A
;	IN	A,(0xA2)
; 8=H
;	OR	30H
;	OUT	(0xA1),A
; 8=L (Send me Data)
;	AND	0CFH
;	OUT	(0xA1),A
; Data Read
;	LD	A,14
;	OUT	(0xA0),A
;	IN	A,(0xA2)
;	DJNZ	GETID0
;////////////////////////////////////
;	AND	0FH
;	LD	H,A
; Restore Port (6,7,8=H)
;	LD	A,15
;	OUT	(0xA0),A
;	IN	A,(0xA2)
;	OR	03FH
;	OUT	(0xA1),A
;
;	POP	AF
;	RET

;======================================
; Sub Routine
;[E]	D: Mask, E: Flags
PORSEL:
	ld		a, #15
	out		(0xA0), a
	in		a, (0xA2)
	and		d
	or		e
	out		(0xA1), a
	ret

; ;======================================
; MATTBL:
	; DEFB	0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH	;A B x x
	; DEFB	0+0E5H,02H, 0+0E5H,08H, 0+0E5H,10H, 0+0E5H,04H	;U D L R

	; DEFB	0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH	;A B x x
	; DEFB	2+0E5H,04H, 2+0E5H,10H, 2+0E5H,20H, 2+0E5H,08H	;U D L R

	; END