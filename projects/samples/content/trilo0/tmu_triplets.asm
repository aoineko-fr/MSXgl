; Song: TT -TRIPLETS-B50-BASSDRUM       
; By:   Gryzor87 (c)2021                
; Period table: A440 Modern

; [ Song start data ]
	.db 0x06					; Initial song speed
	.dw triplets_waveform_start			; Start of the waveform data
	.dw triplets_instrument_start			; Start of the instrument data

; [ Song order pointer list ]
triplets_restart:
	.dw triplets_track_000, triplets_track_000, triplets_track_002, triplets_track_002, triplets_track_004, triplets_track_005, triplets_track_000, triplets_track_000	; Step:000 Pattern:000
	.dw triplets_track_008, triplets_track_009, triplets_track_010, triplets_track_011, triplets_track_012, triplets_track_013, triplets_track_014, triplets_track_015	; Step:001 Pattern:001
	.dw triplets_track_008, triplets_track_009, triplets_track_010, triplets_track_011, triplets_track_012, triplets_track_013, triplets_track_014, triplets_track_015	; Step:002 Pattern:001
	.dw triplets_track_016, triplets_track_017, triplets_track_018, triplets_track_011, triplets_track_012, triplets_track_021, triplets_track_022, triplets_track_023	; Step:003 Pattern:002
	.dw triplets_track_016, triplets_track_017, triplets_track_018, triplets_track_011, triplets_track_012, triplets_track_021, triplets_track_022, triplets_track_023	; Step:004 Pattern:002
	.dw triplets_track_008, triplets_track_009, triplets_track_010, triplets_track_011, triplets_track_012, triplets_track_013, triplets_track_014, triplets_track_015	; Step:005 Pattern:001
	.dw triplets_track_024, triplets_track_025, triplets_track_018, triplets_track_027, triplets_track_028, triplets_track_029, triplets_track_030, triplets_track_031	; Step:006 Pattern:003
	.dw 0x0000, triplets_restart				; End of sequence delimiter + triplets_restart address


; [ SCC Waveforms ]
triplets_waveform_start:
	.db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00				; Waveform:0
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70				; Waveform:8
	.db 0x00, 0x30, 0x50, 0x60, 0x70, 0x60, 0x50, 0x30, 0x00, 0xd0, 0xb0, 0xa0, 0x90, 0xa0, 0xb0, 0xd0, 0x00, 0x40, 0x60, 0x70, 0x60, 0x40, 0x00, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x70, 0x00, 0x90				; Waveform:11
	.db 0x28, 0x58, 0x58, 0x28, 0x00, 0x00, 0x18, 0x48, 0x68, 0x78, 0x68, 0x38, 0xe8, 0xd8, 0xe0, 0x00, 0x20, 0x20, 0x10, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00, 0xd0, 0xb0, 0xb0, 0xd0, 0x00, 0x00				; Waveform:13
	.db 0x00, 0x68, 0x48, 0x18, 0x48, 0x68, 0x28, 0x00, 0x48, 0x77, 0x58, 0x08, 0x28, 0x38, 0x00, 0xb8, 0x08, 0x58, 0x00, 0xe8, 0xf8, 0x00, 0xb8, 0x98, 0xc8, 0x08, 0xe8, 0xa8, 0xc8, 0xf8, 0xc8, 0xa8				; Waveform:20
	.db 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20, 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20				; Waveform:22
	.db 0x10, 0x20, 0x28, 0x18, 0xf0, 0xd0, 0xb0, 0xd0, 0xf0, 0x00, 0x18, 0x50, 0x7f, 0x68, 0x50, 0x40, 0x20, 0xe0, 0xa8, 0xa0, 0xc8, 0xf0, 0x10, 0x30, 0x50, 0x38, 0x08, 0xe0, 0xb8, 0xa0, 0xb8, 0xe8				; Waveform:24
	.db 0x07, 0x57, 0x37, 0x4f, 0x27, 0xe7, 0x9f, 0x2f, 0x6f, 0xb7, 0xff, 0xe7, 0x1f, 0xff, 0x4f, 0xd7, 0xe7, 0xff, 0xf7, 0xdf, 0xf7, 0x67, 0x47, 0xd7, 0x1f, 0x97, 0xb7, 0xe7, 0xd7, 0xc7, 0xf7, 0xff				; Waveform:31

; [ Instruments]
triplets_instrument_start:
	.dw triplets_instrument_00				; BD C2           
	.dw triplets_instrument_01				; SD A3           
	.dw triplets_instrument_02				; HH CL A8        
	.dw triplets_instrument_03				; TOM             
	.dw triplets_instrument_04				; GTR CHORDS      
	.dw triplets_instrument_05				; SC Bass         
	.dw triplets_instrument_06				; SC Brass Satan  
	.dw triplets_instrument_07				; ARP SQ          
	.dw triplets_instrument_08				; Clarinet low    
	.dw triplets_instrument_09				; Response BELL   
	.dw triplets_instrument_10				; HH OP A8        
	.dw triplets_instrument_11				; SD-MAIN         

triplets_instrument_00:					; BD C2           
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xc0,0xff					; Tone sub
	.db 0x1c,0x03						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xc0,0xff					; Tone sub
	.db 0x1a						; Envelope
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

triplets_instrument_01:					; SD A3           
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x06						; Noise _
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x04						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

triplets_instrument_02:					; HH CL A8        
	.db 0x10						; Waveform 2
	.db 0x02,0x90						; Mixer (TN)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

triplets_instrument_03:					; TOM             
	.db 0x38						; Waveform 7
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x08,0x0d						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x00							; Volume same
	.db 0x18,0xec						; Loop (-21)

triplets_instrument_04:					; GTR CHORDS      
	.db 0x48						; Waveform 9
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x03						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

triplets_instrument_05:					; SC Bass         
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xf5						; Loop (-12)

triplets_instrument_06:					; SC Brass Satan  
	.db 0x18						; Waveform 3
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

triplets_instrument_07:					; ARP SQ          
	.db 0x30						; Waveform 6
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
								; --- Macro loop
	.db 0x00							; Volume same
	.db 0x18,0xfe						; Loop (-3)

triplets_instrument_08:					; Clarinet low    
	.db 0x40						; Waveform 8
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xe6						; Loop (-27)

triplets_instrument_09:					; Response BELL   
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x0a,0x01						; Volume +
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
	.db 0x18,0xd8						; Loop (-41)

triplets_instrument_10:					; HH OP A8        
	.db 0x00						; Waveform 0
	.db 0x02,0x90						; Mixer (TN)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0e						; Volume _
								; --- Macro loop
	.db 0x02,0x90						; Mixer (TN)
	.db 0x0E,0x00						; Noise _
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x90						; Mixer (TN)
	.db 0x00							; Volume same
	.db 0x02,0x90						; Mixer (TN)
	.db 0x00							; Volume same
	.db 0x02,0x90						; Mixer (TN)
	.db 0x00							; Volume same
	.db 0x18,0xf0						; Loop (-17)

triplets_instrument_11:					; SD-MAIN         
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x04						; Noise _
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x03						; Noise _
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x02						; Noise _
	.db 0x08,0x09						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x01						; Noise _
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x07						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)


; [ Song track data ]
triplets_track_000:
	.db 0x60			;Release 96
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
triplets_track_002:
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
triplets_track_004:
	.db 0x46			;Note A#6
	.db 0x70			;Volume 14
	.db 0x79			;Instrument 7
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x6c			;Volume 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x6c			;Volume 10
	.db 0xa9				; SCC Soften Waveform
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x68			;Volume 6
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
triplets_track_005:
	.db 0x60			;Release 96
	.db 0xc2			;Wait 3
	.db 0x46			;Note A#6
	.db 0x68			;Volume 6
	.db 0x79			;Instrument 7
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x67			;Volume 5
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x66			;Volume 4
	.db 0xa9				; SCC Soften Waveform
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3a			;Note A#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x46			;Note A#6
	.db 0x65			;Volume 3
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xbf			;[End-Of-Track]
triplets_track_008:
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x2f			;Note B-4
	.db 0x6f			;Volume 13
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x2f			;Note B-4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x26			;Note D-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x70			;Volume 14
	.db 0x7c			;Instrument 10
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 12
	.db 0x73			;Instrument 1
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
triplets_track_009:
	.db 0xa6,0x00			;CMD Envelope multiplier low
	.db 0xc2			;Wait 3
	.db 0x47			;Note B-6
	.db 0x6a			;Volume 8
	.db 0x7b			;Instrument 9
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
	.db 0xbf			;[End-Of-Track]
triplets_track_010:
	.db 0xa5,0x04			;CMD Envelope multiplier high
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
triplets_track_011:
	.db 0x47			;Note B-6
	.db 0x6f			;Volume 13
	.db 0x79			;Instrument 7
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x42			;Note F#6
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
triplets_track_012:
	.db 0x0b			;Note B-1
	.db 0x6f			;Volume 13
	.db 0x77			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x0b			;Note B-1
	.db 0xc1			;Wait 2
	.db 0x0b			;Note B-1
					;Wait Repeat 2
	.db 0x17			;Note B-2
	.db 0xc3			;Wait 4
	.db 0x0b			;Note B-1
	.db 0xc1			;Wait 2
	.db 0x0b			;Note B-1
					;Wait Repeat 2
	.db 0x0d			;Note C#2
	.db 0xc4			;Wait 5
	.db 0x15			;Note A-2
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 5
	.db 0x19			;Note C#3
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0x0e			;Note D-2
	.db 0xc3			;Wait 4
	.db 0x0e			;Note D-2
	.db 0xc1			;Wait 2
	.db 0x0e			;Note D-2
					;Wait Repeat 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x0e			;Note D-2
	.db 0xc1			;Wait 2
	.db 0x0e			;Note D-2
					;Wait Repeat 2
	.db 0x0d			;Note C#2
	.db 0xc3			;Wait 4
	.db 0x0d			;Note C#2
	.db 0xc1			;Wait 2
	.db 0x0d			;Note C#2
					;Wait Repeat 2
	.db 0x19			;Note C#3
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
triplets_track_013:
	.db 0x21			;Note A-3
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xcb			;Wait 12
	.db 0x21			;Note A-3
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x23			;Note B-3
	.db 0xc4			;Wait 5
	.db 0x23			;Note B-3
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x23			;Note B-3
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0x25			;Note C#4
	.db 0xcb			;Wait 12
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
					;Wait Repeat 2
	.db 0x23			;Note B-3
	.db 0xc4			;Wait 5
	.db 0x23			;Note B-3
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x20			;Note G#3
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
triplets_track_014:
	.db 0x26			;Note D-4
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xcb			;Wait 12
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x28			;Note E-4
	.db 0xc4			;Wait 5
	.db 0x28			;Note E-4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x28			;Note E-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0x2a			;Note F#4
	.db 0xcb			;Wait 12
	.db 0x2a			;Note F#4
	.db 0xc1			;Wait 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x28			;Note E-4
	.db 0xc4			;Wait 5
	.db 0x28			;Note E-4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x23			;Note B-3
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
triplets_track_015:
	.db 0x2a			;Note F#4
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xc2			;Wait 3
	.db 0x95,0x66			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0xc4			;Wait 5
	.db 0x2d			;Note A-4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x2f			;Note B-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0x2d			;Note A-4
	.db 0xc3			;Wait 4
	.db 0x95,0x66			;CMD Vibrato
	.db 0xc7			;Wait 8
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0xc4			;Wait 5
	.db 0x2a			;Note F#4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 5
	.db 0x28			;Note E-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
triplets_track_016:
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x70			;Volume 14
	.db 0x7c			;Instrument 10
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 12
	.db 0x73			;Instrument 1
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x5d			;Note A-8
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x70			;Volume 14
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 12
	.db 0x73			;Instrument 1
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
triplets_track_017:
	.db 0xc2			;Wait 3
	.db 0x47			;Note B-6
	.db 0x6a			;Volume 8
	.db 0x7b			;Instrument 9
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x42			;Note F#6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
	.db 0xbf			;[End-Of-Track]
triplets_track_018:
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
triplets_track_021:
	.db 0x1a			;Note D-3
	.db 0x6c			;Volume 10
	.db 0x76			;Instrument 4
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
	.db 0xc5			;Wait 6
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1c			;Note E-3
					;Wait Repeat 2
	.db 0x1c			;Note E-3
	.db 0xc5			;Wait 6
	.db 0x1c			;Note E-3
	.db 0xc1			;Wait 2
	.db 0x1c			;Note E-3
	.db 0xc5			;Wait 6
	.db 0x1e			;Note F#3
	.db 0xc1			;Wait 2
	.db 0x1e			;Note F#3
					;Wait Repeat 2
	.db 0x1e			;Note F#3
					;Wait Repeat 2
	.db 0x1e			;Note F#3
	.db 0xc5			;Wait 6
	.db 0x1e			;Note F#3
	.db 0xc1			;Wait 2
	.db 0x1e			;Note F#3
	.db 0xc1			;Wait 2
	.db 0x1c			;Note E-3
	.db 0x78			;Instrument 6
	.db 0xc7			;Wait 8
	.db 0x14			;Note G#2
	.db 0x78			;Instrument 6
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
triplets_track_022:
	.db 0x34			;Note E-5
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x36			;Note F#5
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x95,0x26			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x34			;Note E-5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc2			;Wait 3
	.db 0x2f			;Note B-4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 3
	.db 0x34			;Note E-5
	.db 0xc1			;Wait 2
	.db 0x38			;Note G#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc2			;Wait 3
	.db 0x3b			;Note B-5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 3
	.db 0x38			;Note G#5
	.db 0xc0			;Wait 1
	.db 0x39			;Note A-5
	.db 0x91,0x04			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x95,0x36			;CMD Vibrato
					;Wait Repeat 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3d			;Note C#6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x3b			;Note B-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x36			;Note F#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x36			;Note F#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x38			;Note G#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xc0			;Wait 1
	.db 0x34			;Note E-5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x31			;Note C#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
triplets_track_023:
	.db 0xc2			;Wait 3
	.db 0x34			;Note E-5
	.db 0x69			;Volume 7
	.db 0x7a			;Instrument 8
	.db 0xa2,0x09			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x36			;Note F#5
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x95,0x26			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x34			;Note E-5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc2			;Wait 3
	.db 0x2f			;Note B-4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 3
	.db 0x34			;Note E-5
	.db 0xc1			;Wait 2
	.db 0x38			;Note G#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc2			;Wait 3
	.db 0x3b			;Note B-5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 3
	.db 0x38			;Note G#5
	.db 0xc0			;Wait 1
	.db 0x39			;Note A-5
	.db 0x91,0x04			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x95,0x36			;CMD Vibrato
					;Wait Repeat 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
	.db 0xc0			;Wait 1
	.db 0x3d			;Note C#6
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x3b			;Note B-5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x36			;Note F#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x36			;Note F#5
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x38			;Note G#5
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xbf			;[End-Of-Track]
triplets_track_024:
	.db 0x11			;Note F-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
			;CMD EE Not supported [Global transpose][WARNING]
	.db 0xc0			;Wait 1
			;CMD EE Not supported [Global transpose][WARNING]
	.db 0xfe			;Wait 63
	.db 0xbf			;[End-Of-Track]
triplets_track_025:
	.db 0xa6,0x42			;CMD Envelope multiplier low
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
triplets_track_027:
	.db 0x47			;Note B-6
	.db 0x6f			;Volume 13
	.db 0x79			;Instrument 7
	.db 0xa2,0x08			;CMD Track detune
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
triplets_track_028:
	.db 0x0b			;Note B-1
	.db 0x6f			;Volume 13
	.db 0x77			;Instrument 5
	.db 0xc2			;Wait 3
	.db 0xad,0x06			; SCC Morph speed 6
	.db 0xc0			;Wait 1
	.db 0xab				; SCC Morph foloow (Carbon C0poy) 0
	.db 0xfb			;Wait 60
	.db 0xbf			;[End-Of-Track]
triplets_track_029:
	.db 0x21			;Note A-3
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xc3			;Wait 4
	.db 0x9d,0x00			; SCC Morph to Waveform 0
	.db 0xfb			;Wait 60
	.db 0xbf			;[End-Of-Track]
triplets_track_030:
	.db 0x26			;Note D-4
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xc3			;Wait 4
	.db 0xab				; SCC Morph foloow (Carbon C0poy) 0
	.db 0xfb			;Wait 60
	.db 0xbf			;[End-Of-Track]
triplets_track_031:
	.db 0x2a			;Note F#4
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xc2			;Wait 3
	.db 0x95,0x66			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0xab				; SCC Morph foloow (Carbon C0poy) 0
	.db 0xfb			;Wait 60
	.db 0xbf			;[End-Of-Track]

