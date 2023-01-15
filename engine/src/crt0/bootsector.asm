; ____________________________
; ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄   ▄▄
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██▄▀ ██▀ █ ██
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ██   ▀█▄ ▀▄█▀
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;──────────────────────────────────────────────────────────────────────────────
; boot sector for direct boot application
;──────────────────────────────────────────────────────────────────────────────
.module boot

BDOS	= 0xF37D
_DIRIO	= 0x06
_DIRIN	= 0x07
_FOPEN	= 0x0F
_SETDTA	= 0x1A
_RDBLK	= 0x27

	.area	_HEADER (ABS)
	.org	0xC000

header: ; 0xC000
header_jump:                    ; Jump instruction in machine language 8088 (IBM)
	.db		0xEB ; 00
	.db		0xFE ; 01
	.db		0x90 ; 02
header_label:                   ; Manufacturer's label and software version
	.ascii	"BOOTDISK"
	; .db		0x4E ; 03 'N'
	; .db		0x4D ; 04 'M'
	; .db		0x53 ; 05 'S'
	; .db		0x20 ; 06 ' '
	; .db		0x32 ; 07 '2'
	; .db		0x2E ; 08 '.'
	; .db		0x30 ; 09 '0'
	; .db		0x50 ; 0A 'P'
header_sectorsize:              ; Number of bytes per sector
	.db		0x00 ; 0B
	.db		0x02 ; 0C
header_sectorpercluster:        ; Number of sectors per cluster
	.db		0x02 ; 0D
header_firstsector:             ; Number of the first sector of the FAT
	.db		0x01 ; 0E
	.db		0x00 ; 0F
header_fatondisk:               ; Number of FATs stored on the disk
	.db		0x02 ; 10
header_maxdir:                  ; Maximum size of the Directory in files
	.db		0x70 ; 11 'p'
	.db		0x00 ; 12
header_totalsector:             ; Total number of sectors on the disk
	.db		0xA0 ; 13
	.db		0x05 ; 14
header_typecode:                ; Disc type code
	.db		0xF9 ; 15
header_reservedsector:          ; Number of sectors reserved for a FAT.
	.db		0x03 ; 16
	.db		0x00 ; 17
header_sectorpertrack:          ; Number of sectors per track
	.db		0x09 ; 18 '	'
	.db		0x00 ; 19
header_headsnum:                ; Number of read/write heads
	.db		0x02 ; 1A
	.db		0x00 ; 1B
header_logical:                 ; Logical disk shift in number of sectors
	.db		0x00 ; 1C
	.db		0x00 ; 1D

boot_start: ; 0xC01E
	ret		nc
	ld		sp, #0xF51F         ; Set callstack address

	ld		de, #fcb
	ld		c, #_FOPEN          ; Open file (FCB)
	call	BDOS                ; Open MSXDOS.SYS

	inc		a
	jp		z, boot_start       ; If open failed
	ld		de, #0x0100
	ld		c, #_SETDTA         ; Set disk transfer address
	call	BDOS

	ld		hl, #0x0001
	ld		(fcb_record), hl    ; Record size

	ld		hl, #0x3F00         ; Number of records to read (16128 bytes! Max size?)
	ld		de, #fcb            ; Pointer to opened FCB
	ld		c, #_RDBLK          ; Random block read (FCB)
	call	BDOS

	jp		0x0100              ; Execute MSXDOS.SYS

fcb:
	.db		0x00 ; 00             Drive number. 0=default drive
fcb_filename:
	.ascii	"BOOTDISKCOM"
	; .db		0x4D ; 01 'M'         Filename
	; .db		0x53 ; 02 'S'
	; .db		0x58 ; 03 'X'
	; .db		0x44 ; 04 'D'
	; .db		0x4F ; 05 'O'
	; .db		0x53 ; 06 'S'
	; .db		0x20 ; 07 ' '
	; .db		0x20 ; 08 ' '
	; .db		0x53 ; 09 'S'         Extension
	; .db		0x59 ; 0A 'Y'
	; .db		0x53 ; 0B 'S'
	.db		0x00 ; 0C             Extent number (low byte)
	.db		0x00 ; 0D             File attributes
fcb_record:
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