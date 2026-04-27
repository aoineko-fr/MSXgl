BDOS     = 0xF37D
_DIRIO   = 0x06
_DIRIN   = 0x07
_FOPEN   = 0x0F
_SETDTA  = 0x1A
_RDBLK   = 0x27

org 0xC01E

boot_start:
	ret nc
	ld (0xC059), de        ; Address to jump after MSXDOS.SYS execution
	ld (buffer), a         ; ????
	ld (hl), 0x56          ; Write 0xC056 (callback_addr) at address pointed by HL
	inc hl
	ld (hl), 0xC0

loading_start:
	ld sp, 0xF51F          ; Set callstack address

	ld de, fcb_start
	ld c, _FOPEN           ; Open file (FCB)
	call BDOS              ; Open MSXDOS.SYS

	inc a
	jp z, handle_error     ; If open failed
	ld de, 0x0100
	ld c, _SETDTA          ; Set disk transfer address
	call BDOS

	ld hl, 0x0001
	ld (fcb_record), hl    ; Record size

	ld hl, 0x3F00          ; Number of records to read (16128 bytes! Max size?)
	ld de, fcb_start       ; Pointer to opened FCB
	ld c, _RDBLK           ; Random block read (FCB)
	call BDOS

	jp 0x0100              ; Execute MSXDOS.SYS
callback_addr:
	ld e, b
	ret nz
	call 0x0000            ; Call the address set in DE at startup

	ld a, c
	and 0xFE
	cp 0x02
	jp nz, label_01

handle_error:
	ld a, (buffer)
	and a
	jp z, 0x4022
label_01:
	ld de, error_message
	call print_str

	ld c, _DIRIN           ; Direct console input
	call BDOS              ; Wait for key press
	jr loading_start       ; Retry loading MSXDOS.SYS

print_str:                                                 ; Print null-terminted string in DE address
	ld a, (de)
	or a
	ret z                  ; Return from print_str
	push de
	ld e, a
	ld c, _DIRIO           ; Direct console I/O
	call BDOS
	pop de
	inc de
	jr print_str

error_message:
	db 0x42 ;'B'
	db 0x6F ;'o'
	db 0x6F ;'o'
	db 0x74 ;'t'
	db 0x20 ;' '
	db 0x65 ;'e'
	db 0x72 ;'r'
	db 0x72 ;'r'
	db 0x6F ;'o'
	db 0x72 ;'r'
	db 0x0D ;''
	db 0x0A ;''
	db 0x50 ;'P'
	db 0x72 ;'r'
	db 0x65 ;'e'
	db 0x73 ;'s'
	db 0x73 ;'s'
	db 0x20 ;' '
	db 0x61 ;'a'
	db 0x6E ;'n'
	db 0x79 ;'y'
	db 0x20 ;' '
	db 0x6B ;'k'
	db 0x65 ;'e'
	db 0x79 ;'y'
	db 0x20 ;' '
	db 0x66 ;'f'
	db 0x6F ;'o'
	db 0x72 ;'r'
	db 0x20 ;' '
	db 0x72 ;'r'
	db 0x65 ;'e'
	db 0x74 ;'t'
	db 0x72 ;'r'
	db 0x79 ;'y'
	db 0x0D ;''
	db 0x0A ;''
	db 0x00 ;

fcb_start:
	db 0x00 ; 00             Drive number. 0=default drive
	db 0x4D ; 01 'M'         Filename
	db 0x53 ; 02 'S'
	db 0x58 ; 03 'X'
	db 0x44 ; 04 'D'
	db 0x4F ; 05 'O'
	db 0x53 ; 06 'S'
	db 0x20 ; 07 ' '
	db 0x20 ; 08 ' '
	db 0x53 ; 09 'S'         Extension
	db 0x59 ; 0A 'Y'
	db 0x53 ; 0B 'S'
	db 0x00 ; 0C             Extent number (low byte)
	db 0x00 ; 0D             File attributes
fcb_record:
	db 0x00 ; 0E             Record size (low byte) / Extent number (high byte)
	db 0x00 ; 0F             Record size (high byte)
	db 0x00 ; 10             File size (32 bits)
	db 0x00 ; 11
	db 0x00 ; 12
	db 0x00 ; 13
	db 0x00 ; 14             Volume-id (32 bits)
	db 0x00 ; 15
	db 0x00 ; 16
	db 0x00 ; 17
	db 0x00 ; 18             Internal information
	db 0x00 ; 19
	db 0x00 ; 1A
	db 0x00 ; 1B
	db 0x00 ; 1C
	db 0x00 ; 1D
	db 0x00 ; 1E
	db 0x00 ; 1F
	db 0x00 ; 20             Current record within extent
	db 0x00 ; 21             Random record number
	db 0x00 ; 22
	db 0x00 ; 23
	db 0x00 ; 24

buffer:
	db 0x00 ;