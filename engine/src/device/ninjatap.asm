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
;Copyright 1994 1995 Knight's Chamber,
;		 all rights reserved.
;	Written by Firm Knight.
;======================================
DEVDRV	EQU	7800H	;0C000H
NTPTBL	EQU	DEVDRV+1F6H	;10 Bytes
DMMDEV	EQU	4300H+65

	ORG	DEVDRV

;		 0....5....10...15...20...25...30...35...
	DEFM	'DEV:あたっちゃって２５％    by Influenza'
	DEFB	1AH,02H		;EOF,DEVDRV ID
	DEFS	6

	JP	DEVINI
	JP	DEVMOU
	JP	DEVSTK
	JP	DEVTRG

	JP	GET10P

;	JP	GTNTAP		;GetDAT
;	JP	CKNTAP		;Check
;======================================
; Data Area
RESULT:	DEFB	0	;Connect Flag
MAX:	DEFB	0	;Max Player
ID1:	DEFB	0	;ID(Port1)
ID2:	DEFB	0	;ID(Port2)
;======================================
; Get 4 Players Inkey
;[E]	None
;[R]	HL= Table Address

GET10P:
	CALL	GETALL
	LD	HL,NTPTBL
	RET
;======================================
DEVINI:
	LD	A,(002DH)
	CP	3
	JR	C,DEVIN1
	LD	HL,GTNTA0
	LD	(SWGETA+1),HL
DEVIN1:
	LD	HL,NTPTBL
	LD	B,10
DEVIN0:	LD	(HL),255
	INC	HL
	DJNZ	DEVIN0
	JP	CKNTAP
;==================
DEVMOU:	LD	HL,0000
	RET
;==================
DEVSTK:	CALL	GETALL
	LD	C,0F0H
	CALL	SRCH
	RET	Z
	AND	0FH
	LD	HL,STKTBL
	ADD	A,L
	LD	L,A
	LD	A,(HL)
	RET

STKTBL:	DEFB	0,7,3,0,1,8,2,1,5,6,4,5,0,7,3,0
;================
DEVTRG:	CALL	GETALL
	LD	C,3FH
	CALL	SRCH
	RET	Z
	RLA
	LD	A,9
	RET	NC
	INC	A
	RET
;======================================
; Search a First Input
;[E]	C:Mask
;[R]	A=Inkey Condition
;	Z on=Nobody Input

SRCH:	LD	HL,NTPTBL
	LD	B,10
SRCH0:	LD	A,(HL)
	OR	C
	INC	A
	JR	NZ,SRCH1
	INC	HL
	DJNZ	SRCH0
	LD	A,255
	LD	(DMMDEV),A
	XOR	A
	RET
SRCH1:	LD	A,10
	SUB	B
	LD	(DMMDEV),A
	LD	A,(HL)
	RET
;======================================
; Get All Device
;[E]	None
;[R]	(NTPTBL,10)=Result

GETALL:	LD	HL,MATTBL
	CALL	GETKEY
	LD	A,B
	LD	(NTPTBL),A
	CALL	GETKEY
	LD	HL,NTPTBL+1
	LD	(HL),B
	INC	HL
SWGETA:	JP	GTNTAP
;======================================
; Get Key Matrix
;[E]	HL:MATTBL
;[R]	B:Inkey Condition

GETKEY:	LD	D,0FBH
	LD	BC,01FFH
GETKE0:	LD	E,(HL)
	INC	HL
	LD	A,(DE)
	AND	(HL)
	INC	HL
	ADD	A,C
	RL	B
	JR	NC,GETKE0
	RET
;======================================
; Get Inkey
;[E]	hl :Pointer to NTAP Table
;[R]	None
;[M]	af,bc,de,hl
;ofset +0 (A)(B)( )( )(U)(D)(L)(R)

GTNTAP:
	DI
	LD	A,(RESULT)
	RRCA			;Port 1
	LD	DE,0AF20H
	CALL	NC,GETJOY
	LD	DE,0BA3AH
	CALL	C,GETNIN
	RRCA			;Port 2
	LD	DE,09F50H
	CALL	NC,GETJOY
	LD	DE,0FA7AH
	CALL	C,GETNIN
	EI
	RET
; For Turbo R
GTNTA0:
	CALL	0183H
	PUSH	AF
	XOR	A
	CALL	0180H
	CALL	GTNTAP
	POP	AF
	JP	0180H
;======================================
GETJOY:
	PUSH	AF
; Select Port1/Port2
	LD	A,15
	OUT	(0A0H),A
	IN	A,(0A2H)
	AND	D
	OR	E
	OUT	(0A1H),A
; Get Key Status
	LD	A,14
	OUT	(0A0H),A
	IN	A,(0A2H)
; Write Status to (HL)
	LD	E,0FFH
	BIT	5,A
	JR	NZ,GETJO0
	RES	2,E		;Trig 2
GETJO0:	BIT	4,A
	JR	NZ,GETJO1
	RES	3,E		;Trig 1
GETJO1:	RRA
	RL	E
	RRA
	RL	E
	RRA
	RL	E
	RRA
	RL	E		;Lever
	LD	(HL),E
	INC	HL

	POP	AF
	RET
;======================================
; NTAP Access
GETNIN:
	PUSH	AF
	PUSH	HL
; Select Port and 8pin=H,6pin=L
	LD	A,15
	OUT	(0A0H),A
	IN	A,(0A2H)
	AND	D
	OR	E
	OUT	(0A1H),A
; 6pin=H (4021 Data Read)
	OR	05H
	OUT	(0A1H),A
; 6pin=L (4021 Transfer Mode)
	AND	0FAH
	OUT	(0A1H),A
;/// Get Key Status //////
	LD	B,8
GETNI0:
; Send me Data
	LD	A,15
	OUT	(0A0H),A
	IN	A,(0A2H)
	OR	030H
	OUT	(0A1H),A	;8=H
	AND	0CFH
	OUT	(0A1H),A	;8=L

	LD	A,14
	OUT	(0A0H),A
	IN	A,(0A2H)	;Read
	RRA
	RL	 H		;Joy1
	RRA
	RL	 E		;Joy2
	RRA
	RL	 D		;Joy3
	RRA
	RL	 C		;Joy4
	DJNZ	GETNI0
;/////////////////////////
	LD	A,H
	POP	HL
	LD	(HL),A		;Joy1
	INC	HL
	LD	(HL),E		;Joy2
	INC	HL
	LD	(HL),D		;Joy3
	INC	HL
	LD	(HL),C		;Joy4
	INC	HL
; Restore Port (6,7,8=H)
	LD	DE,0FF3FH
	CALL	PORSEL
;	LD	A,15
;	OUT	(0A0H),A
;	IN	A,(0A2H)
;	OR	3FH
;	OUT	(0A1H),A
	POP	AF
	RET
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

CKNTAP:	DI
	LD	B,0	; b=Max Player
	LD	DE,0BF00H
	CALL	CHECK	; Port1 Check
	LD	 C,A
	LD	 E,040H
	CALL	CHECK	; Port2 Check
	RLCA
	OR	 C
; Result Save
	LD	C,A
	LD	(RESULT),BC
; Port1 ID Check
;	LD	 H,0FFH
;	RRCA
;	LD	DE,0BA3AH
;	CALL	 C,GETID
;	RRCA
;	LD	A,H
;	LD	(RESULT+2),A
; Port2 ID Check
;	LD	H,0FFH
;	LD	DE,0FA7AH
;	CALL	 C,GETID
;	LD	A,H
;	LD	(RESULT+3),A
; Restore Port (6,7,8=H)
	LD	DE,0FF3FH
	JP	PORSEL
;	EI
;	RET
;======================================
; Connection Check Sub.
CHECK:	CALL	PORSEL
	INC	B
	AND	0C0H
	OUT	(0A1H),A	; 678=L
	EX	AF,AF'
	LD	A,14
	OUT	(0A0H),A
	IN	A,(0A2H)
	AND	20H		; 7=H ?
	RET	Z

	LD	A,15
	OUT	(0A0H),A
	EX	AF,AF'
	OR	030H
	OUT	(0A1H),A	; 8=H
	LD	A,14
	OUT	(0A0H),A
	IN	A,(0A2H)
	AND	20H		; 7=L ?
	JR	NZ,CHECK1

	INC	A
	INC	B
	INC	B
	INC	B
	RET
; No NTAP
CHECK1:	XOR	A
	RET
;======================================
; Get ID
;GETID:	PUSH	AF
; 6=H 8=H
;	CALL	PORSEL
; 6=H 4021 Data Load
;	OR	05H
;	OUT	(0A1H),A
; 6=L 4021 Transfer Mode
;	AND	0FAH
;	OUT	(0A1H),A
;
;	LD	B,9
;/// Waste 8 Data and Get 9th Data ////
;GETID0:	LD	A,15
;	OUT	(0A0H),A
;	IN	A,(0A2H)
; 8=H
;	OR	30H
;	OUT	(0A1H),A
; 8=L (Send me Data)
;	AND	0CFH
;	OUT	(0A1H),A
; Data Read
;	LD	A,14
;	OUT	(0A0H),A
;	IN	A,(0A2H)
;	DJNZ	GETID0
;////////////////////////////////////
;	AND	0FH
;	LD	H,A
; Restore Port (6,7,8=H)
;	LD	A,15
;	OUT	(0A0H),A
;	IN	A,(0A2H)
;	OR	03FH
;	OUT	(0A1H),A
;
;	POP	AF
;	RET
;======================================
; Sub Routine
PORSEL:	LD	A,15
	OUT	(0A0H),A
	IN	A,(0A2H)
	AND	D
	OR	E
	OUT	(0A1H),A
	RET
;======================================
MATTBL:	DEFB	0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH	;A B x x
	DEFB	0+0E5H,02H, 0+0E5H,08H, 0+0E5H,10H, 0+0E5H,04H	;U D L R

	DEFB	0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH, 0EEH,0FFH	;A B x x
	DEFB	2+0E5H,04H, 2+0E5H,10H, 2+0E5H,20H, 2+0E5H,08H	;U D L R

	END