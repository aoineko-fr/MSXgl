; Song: TT Vs Replayer ULTIMATE  TESTv12
; By:   Gryzor87 (c)2021                
; Period table: A445 Konami

; [ Song start data ]
	.db 0x0d					; Initial song speed.
	.dw .waveform_start			; Start of the waveform data.
	.dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
.restart:
	.dw .track_000, .track_001, .track_002, .track_003, .track_004, .track_004, .track_004, .track_004	; Step:000 Pattern:000
	.dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:001 Pattern:001
	.dw .track_016, .track_017, .track_018, .track_019, .track_020, .track_010, .track_010, .track_010	; Step:002 Pattern:002
	.dw .track_024, .track_025, .track_026, .track_027, .track_027, .track_010, .track_010, .track_010	; Step:003 Pattern:003
	.dw .track_032, .track_032, .track_032, .track_035, .track_036, .track_037, .track_032, .track_032	; Step:004 Pattern:004
	.dw .track_040, .track_041, .track_042, .track_043, .track_044, .track_044, .track_044, .track_044	; Step:005 Pattern:005
	.dw .track_040, .track_049, .track_050, .track_051, .track_052, .track_053, .track_010, .track_010	; Step:006 Pattern:006
	.dw .track_040, .track_057, .track_027, .track_027, .track_027, .track_027, .track_027, .track_010	; Step:007 Pattern:007
	.dw .track_064, .track_065, .track_066, .track_066, .track_066, .track_066, .track_066, .track_066	; Step:008 Pattern:008
	.dw .track_072, .track_073, .track_010, .track_010, .track_010, .track_010, .track_010, .track_010	; Step:009 Pattern:009
	.dw .track_080, .track_081, .track_082, .track_010, .track_010, .track_010, .track_010, .track_010	; Step:010 Pattern:010
	.dw .track_072, .track_027, .track_027, .track_091, .track_092, .track_093, .track_094, .track_010	; Step:011 Pattern:011
	.dw .track_072, .track_010, .track_010, .track_099, .track_100, .track_101, .track_102, .track_010	; Step:012 Pattern:012
	.dw .track_104, .track_105, .track_106, .track_107, .track_027, .track_027, .track_027, .track_027	; Step:013 Pattern:013
	.dw .track_112, .track_027, .track_027, .track_115, .track_010, .track_010, .track_010, .track_010	; Step:014 Pattern:014
	.dw .track_120, .track_027, .track_027, .track_027, .track_027, .track_027, .track_027, .track_027	; Step:015 Pattern:015
	.dw 0x0000, .restart				; End of sequence delimiter + restart address.


; [ SCC Waveforms ]
.waveform_start:
	.db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00				; Waveform:0
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0x00, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x81, 0xc0, 0x01, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x01, 0x40, 0x01, 0xe0, 0x01, 0x20, 0x01, 0xf0, 0x01, 0x10, 0x01, 0xff, 0xff, 0xff, 0xff, 0x40, 0x40, 0x40				; Waveform:2
	.db 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x7f, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0xff, 0xf0, 0xe0, 0xd0, 0xc0, 0xb0, 0xa0, 0x90, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0				; Waveform:3
	.db 0x00, 0x30, 0x50, 0x60, 0x70, 0x60, 0x50, 0x30, 0x00, 0xd0, 0xb0, 0xa0, 0x90, 0xa0, 0xb0, 0xd0, 0x00, 0x40, 0x60, 0x70, 0x60, 0x40, 0x00, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x70, 0x00, 0x90				; Waveform:4
	.db 0x80, 0x8e, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x3f, 0x3e, 0x3c, 0x3a, 0x37, 0x31, 0x29, 0x20, 0x1c, 0x10, 0x00, 0xe6, 0xc0, 0xd0, 0x00, 0x20, 0x3f, 0x10, 0xe0, 0x80, 0xc0, 0x00, 0x20, 0x00, 0x90				; Waveform:5
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70				; Waveform:8
	.db 0x00, 0xf8, 0xe8, 0xd8, 0xc8, 0xb8, 0xac, 0x9c, 0x90, 0x8c, 0x8a, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x77, 0x68, 0x58, 0x48, 0x38, 0x28, 0x18, 0x08				; Waveform:9
	.db 0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00, 0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00				; Waveform:10
	.db 0x00, 0x30, 0x50, 0x60, 0x70, 0x60, 0x50, 0x30, 0x00, 0xd0, 0xb0, 0xa0, 0x90, 0xa0, 0xb0, 0xd0, 0x00, 0x40, 0x60, 0x70, 0x60, 0x40, 0x00, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x70, 0x00, 0x90				; Waveform:11
	.db 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87				; Waveform:12
	.db 0x28, 0x58, 0x58, 0x28, 0x00, 0x00, 0x18, 0x48, 0x68, 0x78, 0x68, 0x38, 0xe8, 0xd8, 0xe0, 0x00, 0x20, 0x20, 0x10, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00, 0xd0, 0xb0, 0xb0, 0xd0, 0x00, 0x00				; Waveform:13
	.db 0x00, 0x40, 0x7f, 0x40, 0x10, 0x01, 0xea, 0xce, 0xbb, 0xb1, 0xa7, 0x9c, 0x94, 0x8d, 0x87, 0x82, 0x81, 0x80, 0x80, 0x81, 0x81, 0x83, 0x87, 0x8d, 0x94, 0x9c, 0xa7, 0xb1, 0xbb, 0xce, 0xea, 0xff				; Waveform:16
	.db 0x27, 0x5f, 0x4f, 0x4f, 0x5f, 0x67, 0x7f, 0x7f, 0x6f, 0x6f, 0x7f, 0x6f, 0x5f, 0x4f, 0x67, 0x5f, 0x37, 0xe7, 0xd7, 0xc7, 0xd7, 0xff, 0xe7, 0xcf, 0xc7, 0xc7, 0xbf, 0x97, 0x87, 0xaf, 0xd7, 0xff				; Waveform:18
	.db 0x90, 0xd0, 0x20, 0x0c, 0xbc, 0xaa, 0xe6, 0xdc, 0xbc, 0xe3, 0x40, 0x30, 0xe0, 0xd0, 0x10, 0x06, 0xe4, 0x0c, 0x70, 0x50, 0xe0, 0xd0, 0x10, 0x00, 0xd0, 0xf0, 0x40, 0x20, 0xc4, 0xc0, 0xf0, 0x90				; Waveform:19
	.db 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x7f, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0xff, 0xf0, 0xe0, 0xd0, 0xc0, 0xb0, 0xa0, 0x90, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0				; Waveform:20
	.db 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20, 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20				; Waveform:22
	.db 0x08, 0x20, 0x48, 0x68, 0x7f, 0x30, 0x08, 0x30, 0x60, 0x70, 0x7f, 0x28, 0xb8, 0x80, 0xc8, 0x20, 0xf8, 0xc8, 0xb8, 0xc0, 0xd0, 0xc8, 0xd8, 0xe0, 0x00, 0xe0, 0xd0, 0xb8, 0xc8, 0xe0, 0xf0, 0x00				; Waveform:28
	.db 0x07, 0x57, 0x37, 0x4f, 0x27, 0xe7, 0x9f, 0x2f, 0x6f, 0xb7, 0xff, 0xe7, 0x1f, 0xff, 0x4f, 0xd7, 0xe7, 0xff, 0xf7, 0xdf, 0xf7, 0x67, 0x47, 0xd7, 0x1f, 0x97, 0xb7, 0xe7, 0xd7, 0xc7, 0xf7, 0xff				; Waveform:31

; [ Instruments]
.instrument_start:
	.dw .instrument_00				; BD C2           
	.dw .instrument_01				; LAZER           
	.dw .instrument_02				; HH CL A8        
	.dw .instrument_03				; Volume          
	.dw .instrument_04				; VOL  CONT       
	.dw .instrument_05				; ENV  A          
	.dw .instrument_06				; ENV   8         
	.dw .instrument_07				; SC Bass         
	.dw .instrument_08				; VOL MELODY cont 
	.dw .instrument_09				; Piano Tremolo HI
	.dw .instrument_10				; PIANO           
	.dw .instrument_11				; SHAKER II G7    
	.dw .instrument_12				; SHAKER III      
	.dw .instrument_13				; NOISE  SWEEP    

.instrument_00:					; BD C2           
	.db 0x50						; Waveform 10
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x09						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
	.db 0x18,0xfc						; Loop (-5)

.instrument_01:					; LAZER           
	.db 0x80						; Waveform 16
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xfd,0xff					; Tone sub
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xff,0xff					; Tone sub
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x01						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x04,0x00					; Tone add
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xff,0xff					; Tone sub
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x01,0x00					; Tone add
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xff,0xff					; Tone sub
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x08,0x00					; Tone add
	.db 0x1c,0x0e						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x0c,0x00					; Tone add
	.db 0x1c,0x0e						; Envelope shape
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

.instrument_02:					; HH CL A8        
	.db 0x30						; Waveform 6
	.db 0x02,0x90						; Mixer (TN)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

.instrument_03:					; Volume          
	.db 0x58						; Waveform 11
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

.instrument_04:					; VOL  CONT       
	.db 0x58						; Waveform 11
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x18,0xfb						; Loop (-6)

.instrument_05:					; ENV  A          
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x0a						; Envelope shape
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
	.db 0x18,0xfc						; Loop (-5)

.instrument_06:					; ENV   8         
	.db 0x70						; Waveform 14
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x08						; Envelope shape
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
	.db 0x18,0xfc						; Loop (-5)

.instrument_07:					; SC Bass         
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

.instrument_08:					; VOL MELODY cont 
	.db 0x38						; Waveform 7
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfb						; Loop (-6)

.instrument_09:					; Piano Tremolo HI
	.db 0x60						; Waveform 12
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
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
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
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
	.db 0x0a,0x01						; Volume +
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xd7						; Loop (-42)

.instrument_10:					; PIANO           
	.db 0x10						; Waveform 2
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
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
	.db 0x18,0xec						; Loop (-21)

.instrument_11:					; SHAKER II G7    
	.db 0x00						; Waveform 0
	.db 0x02,0x90						; Mixer (TN)
	.db 0x0E,0x1a						; Noise _
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x18						; Noise _
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x10						; Noise _
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0e						; Noise _
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0b						; Noise _
	.db 0x08,0x09						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0a						; Noise _
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x07						; Noise _
	.db 0x08,0x07						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x04						; Noise _
	.db 0x08,0x06						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x02						; Noise _
	.db 0x08,0x05						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x00							; Volume same
	.db 0x18,0xfe						; Loop (-3)

.instrument_12:					; SHAKER III      
	.db 0x00						; Waveform 0
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x15						; Noise _
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x11						; Noise _
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0e						; Noise _
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x09						; Noise _
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x03						; Noise _
	.db 0x08,0x09						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x02						; Noise _
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x01						; Noise _
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfd						; Loop (-4)

.instrument_13:					; NOISE  SWEEP    
	.db 0x00						; Waveform 0
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x1f						; Noise _
	.db 0x08,0x0e						; Volume _
								; --- Macro loop
	.db 0x02,0x80						; Mixer (N)
	.db 0x12,0xff						; Noise -
	.db 0x08,0x0d						; Volume _
	.db 0x18,0xf9						; Loop (-8)


; [ Song track data ]
.track_000:
	.db 0x2b			;Note G-4
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x2b			;Note G-4
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6a			;Volume 8
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x69			;Volume 7
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x66			;Volume 4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
.track_001:
	.db 0x60			;Release 96
	.db 0xa4,0x0d			;CMD Speed
	.db 0xcf			;Wait 16
	.db 0x37			;Note G-5
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6d			;Volume 11
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6a			;Volume 8
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x69			;Volume 7
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x66			;Volume 4
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x37			;Note G-5
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
.track_002:
	.db 0x60			;Release 96
	.db 0xdf			;Wait 32
	.db 0x43			;Note G-6
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6d			;Volume 11
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6a			;Volume 8
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x69			;Volume 7
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x66			;Volume 4
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x43			;Note G-6
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
.track_003:
	.db 0x60			;Release 96
	.db 0xef			;Wait 48
	.db 0x2b			;Note G-4
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x2b			;Note G-4
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x6a			;Volume 8
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x69			;Volume 7
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x66			;Volume 4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0xbf			;[End-Of-Track]
.track_004:
	.db 0x60			;Release 96
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
.track_008:
	.db 0xe7			;Wait 40
	.db 0x4f			;Note G-7
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x4f			;Note G-7
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6e			;Volume 12
	.db 0xc4			;Wait 5
	.db 0x37			;Note G-5
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0xc1			;Wait 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x4f			;Note G-7
					;Wait Repeat 2
	.db 0x5b			;Note G-8
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
.track_009:
	.db 0xeb			;Wait 44
	.db 0x4f			;Note G-7
	.db 0x6d			;Volume 11
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x4f			;Note G-7
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6a			;Volume 8
	.db 0xd0			;Wait 17
	.db 0xbf			;[End-Of-Track]
.track_010:
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
.track_011:
	.db 0x60			;Release 96
	.db 0xf7			;Wait 56
	.db 0x37			;Note G-5
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0xc1			;Wait 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x4f			;Note G-7
					;Wait Repeat 2
	.db 0x5b			;Note G-8
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
.track_012:
	.db 0x37			;Note G-5
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0xcc			;Wait 13
	.db 0x43			;Note G-6
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6e			;Volume 12
	.db 0xcc			;Wait 13
	.db 0x4f			;Note G-7
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x4f			;Note G-7
	.db 0x70			;Volume 14
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6e			;Volume 12
	.db 0xdc			;Wait 29
	.db 0xbf			;[End-Of-Track]
.track_013:
	.db 0xc3			;Wait 4
	.db 0x37			;Note G-5
	.db 0x6d			;Volume 11
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x6a			;Volume 8
	.db 0xcc			;Wait 13
	.db 0x43			;Note G-6
	.db 0x6d			;Volume 11
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x6a			;Volume 8
	.db 0xcc			;Wait 13
	.db 0x4f			;Note G-7
	.db 0x6d			;Volume 11
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x4f			;Note G-7
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x4f			;Note G-7
	.db 0x6a			;Volume 8
	.db 0xd8			;Wait 25
	.db 0xbf			;[End-Of-Track]
.track_014:
	.db 0xc7			;Wait 8
	.db 0x37			;Note G-5
	.db 0x69			;Volume 7
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x66			;Volume 4
	.db 0xcc			;Wait 13
	.db 0x43			;Note G-6
	.db 0x69			;Volume 7
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x68			;Volume 6
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x67			;Volume 5
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x66			;Volume 4
	.db 0xe4			;Wait 37
	.db 0xbf			;[End-Of-Track]
.track_015:
	.db 0xcb			;Wait 12
	.db 0x37			;Note G-5
	.db 0x65			;Volume 3
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x37			;Note G-5
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xcb			;Wait 12
	.db 0x43			;Note G-6
	.db 0x65			;Volume 3
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x63			;Volume 1
					;Wait Repeat 1
	.db 0x43			;Note G-6
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
.track_016:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x01			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_017:
	.db 0x24			;Note C-4
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x94,0x47			;CMD Arpeggio
	.db 0xc7			;Wait 8
	.db 0x1f			;Note G-3
	.db 0x70			;Volume 14
	.db 0x94,0x47			;CMD Arpeggio
					;Wait Repeat 8
	.db 0x2e			;Note A#4
	.db 0x6f			;Volume 13
	.db 0x94,0x47			;CMD Arpeggio
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x1d			;Note F-3
	.db 0x6f			;Volume 13
	.db 0x94,0x7c			;CMD Arpeggio
	.db 0xc7			;Wait 8
	.db 0x11			;Note F-2
	.db 0x6f			;Volume 13
	.db 0x94,0x7c			;CMD Arpeggio
					;Wait Repeat 8
	.db 0x05			;Note F-1
	.db 0x6f			;Volume 13
	.db 0x94,0x7c			;CMD Arpeggio
	.db 0xc1			;Wait 2
	.db 0x06			;Note F#1
					;Wait Repeat 2
	.db 0x05			;Note F-1
					;Wait Repeat 2
	.db 0x04			;Note E-1
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
.track_018:
	.db 0x9e,0x08			;CMD Arpegio speed
	.db 0xc7			;Wait 8
	.db 0x9e,0x04			;CMD Arpegio speed
					;Wait Repeat 8
	.db 0x9e,0x01			;CMD Arpegio speed
	.db 0xcf			;Wait 16
	.db 0x30			;Note C-5
	.db 0x6e			;Volume 12
	.db 0x76			;Instrument 4
	.db 0x94,0x7c			;CMD Arpeggio
	.db 0xc1			;Wait 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x28			;Note E-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x6e			;Volume 12
	.db 0x94,0x7c			;CMD Arpeggio
					;Wait Repeat 2
	.db 0x1e			;Note F#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x1c			;Note E-3
	.db 0xc1			;Wait 2
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x94,0x7c			;CMD Arpeggio
					;Wait Repeat 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x28			;Note E-4
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
.track_019:
	.db 0x60			;Release 96
	.db 0xf7			;Wait 56
	.db 0x05			;Note F-1
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xaa,0x38			; SCC Set Waveform 8
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
.track_020:
	.db 0x60			;Release 96
	.db 0xf5			;Wait 54
	.db 0xac,0x01			; SCC Morph follow (Carbon C0poy) 1
	.db 0xc0			;Wait 1
	.db 0xad,0x01			; SCC Morph speed 1
					;Wait Repeat 1
	.db 0x05			;Note F-1
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xaa,0x20			; SCC Set Waveform 4
					;Wait Repeat 1
	.db 0x9d,0x98			; SCC Morph to Waveform 31
	.db 0xc6			;Wait 7
	.db 0xbf			;[End-Of-Track]
.track_024:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_025:
	.db 0x18			;Note C-3
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0x96,0x0c			;CMD Portamento up
	.db 0xc3			;Wait 4
	.db 0x93,0x0c			;CMD Portamento down
					;Wait Repeat 4
	.db 0x24			;Note C-4
	.db 0x6f			;Volume 13
	.db 0x96,0x06			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x06			;CMD Portamento down
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0x6f			;Volume 13
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 4
	.db 0x3c			;Note C-6
	.db 0x6f			;Volume 13
	.db 0x96,0x02			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x02			;CMD Portamento down
					;Wait Repeat 4
	.db 0x93,0x04			;CMD Portamento down
					;Wait Repeat 4
	.db 0x93,0x08			;CMD Portamento down
					;Wait Repeat 4
	.db 0x93,0x0f			;CMD Portamento down
					;Wait Repeat 4
	.db 0x93,0x20			;CMD Portamento down
					;Wait Repeat 4
	.db 0x93,0x40			;CMD Portamento down
					;Wait Repeat 4
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x2d			;Note A-4
	.db 0x71			;Volume 15
	.db 0x93,0xa0			;CMD Portamento down
	.db 0xc2			;Wait 3
	.db 0x60			;Release 96
	.db 0xc0			;Wait 1
	.db 0x05			;Note F-1
	.db 0x96,0xa0			;CMD Portamento up
	.db 0xc2			;Wait 3
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_026:
	.db 0x9f,0x0f			;CMD Portamento fine up
	.db 0xc3			;Wait 4
	.db 0xa0,0xff			;CMD Portamento fine up
	.db 0xd3			;Wait 20
	.db 0x9f,0x01			;CMD Portamento fine up
	.db 0xe7			;Wait 40
	.db 0xbf			;[End-Of-Track]
.track_027:
	.db 0x60			;Release 96
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
.track_032:
	.db 0x60			;Release 96
	.db 0xeb			;Wait 44
	.db 0xbf			;[End-Of-Track]
.track_035:
	.db 0x0e			;Note D-2
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0xaa,0x80			; SCC Set Waveform 20
	.db 0xc0			;Wait 1
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc3			;Wait 4
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 4
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 4
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 4
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 4
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 4
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_036:
	.db 0x15			;Note A-2
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0xaa,0x80			; SCC Set Waveform 20
	.db 0xc0			;Wait 1
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc4			;Wait 5
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 5
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 5
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 5
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 5
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 5
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 5
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 5
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_037:
	.db 0x1c			;Note E-3
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0xaa,0x80			; SCC Set Waveform 20
	.db 0xc0			;Wait 1
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc6			;Wait 7
	.db 0x96,0x0a			;CMD Portamento up
					;Wait Repeat 7
	.db 0x93,0x0a			;CMD Portamento down
					;Wait Repeat 7
	.db 0x96,0x0a			;CMD Portamento up
	.db 0xc5			;Wait 6
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x4d			;Note F-7
	.db 0x71			;Volume 15
	.db 0xaa,0x18			; SCC Set Waveform 3
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x96,0x03			;CMD Portamento up
					;Wait Repeat 1
	.db 0x93,0x03			;CMD Portamento down
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_040:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_041:
	.db 0x26			;Note D-4
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0x91,0x0a			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x32			;Note D-5
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x91,0x05			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x3e			;Note D-6
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x40			;Note E-6
	.db 0x91,0x03			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0x1c			;Note E-3
	.db 0x91,0x10			;CMD Portamento tone
					;Wait Repeat 4
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc0			;Wait 1
	.db 0x23			;Note B-3
					;Wait Repeat 1
	.db 0x24			;Note C-4
	.db 0xc0			;Wait 1
	.db 0x26			;Note D-4
	.db 0x70			;Volume 14
	.db 0x7a			;Instrument 8
	.db 0xc0			;Wait 1
	.db 0x28			;Note E-4
	.db 0x76			;Instrument 4
	.db 0x91,0x03			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x6f			;Volume 13
	.db 0x95,0x65			;CMD Vibrato
					;Wait Repeat 4
	.db 0x6e			;Volume 12
					;Wait Repeat 4
	.db 0x6d			;Volume 11
					;Wait Repeat 4
	.db 0x6c			;Volume 10
	.db 0xc2			;Wait 3
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_042:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xec			;Wait 45
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
	.db 0x76			;Instrument 4
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc0			;Wait 1
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x91,0x03			;CMD Portamento tone
	.db 0xc3			;Wait 4
	.db 0x6c			;Volume 10
	.db 0x95,0x65			;CMD Vibrato
					;Wait Repeat 4
	.db 0x6b			;Volume 9
					;Wait Repeat 4
	.db 0x6a			;Volume 8
					;Wait Repeat 4
	.db 0x69			;Volume 7
	.db 0xc0			;Wait 1
	.db 0x60			;Release 96
	.db 0xbf			;[End-Of-Track]
.track_043:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0xaa,0x58			; SCC Set Waveform 12
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0x91,0x0a			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x32			;Note D-5
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x91,0x05			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x3e			;Note D-6
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x40			;Note E-6
	.db 0x91,0x03			;CMD Portamento tone
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0x1a			;Note D-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0x1c			;Note E-3
	.db 0x91,0x10			;CMD Portamento tone
					;Wait Repeat 4
	.db 0x60			;Release 96
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
.track_044:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
.track_049:
	.db 0x33			;Note D#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x91,0x04			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
					;Wait Repeat 1
	.db 0x31			;Note C#5
					;Wait Repeat 1
	.db 0x30			;Note C-5
					;Wait Repeat 1
	.db 0x33			;Note D#5
					;Wait Repeat 1
	.db 0x2c			;Note G#4
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x76			;CMD Vibrato
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc0			;Wait 1
	.db 0x2e			;Note A#4
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
					;Wait Repeat 1
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x95,0x77			;CMD Vibrato
					;Wait Repeat 2
	.db 0x6f			;Volume 13
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x03			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x66			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x6f			;Volume 13
	.db 0xc1			;Wait 2
	.db 0x6e			;Volume 12
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x31			;Note C#5
	.db 0x91,0x06			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x2e			;Note A#4
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0x7a			;Instrument 8
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
					;Wait Repeat 1
	.db 0x31			;Note C#5
	.db 0x91,0x06			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x31			;Note C#5
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x01			;CMD Portamento up
					;Wait Repeat 1
	.db 0x35			;Note F-5
	.db 0x91,0x08			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc0			;Wait 1
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc1			;Wait 2
	.db 0x31			;Note C#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x04			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x74			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x6f			;Volume 13
	.db 0xc2			;Wait 3
	.db 0x6e			;Volume 12
	.db 0xbf			;[End-Of-Track]
.track_050:
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x33			;Note D#5
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x91,0x04			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x75			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x33			;Note D#5
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x31			;Note C#5
					;Wait Repeat 1
	.db 0x30			;Note C-5
					;Wait Repeat 1
	.db 0x33			;Note D#5
					;Wait Repeat 1
	.db 0x2c			;Note G#4
	.db 0xc1			;Wait 2
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0x95,0x77			;CMD Vibrato
	.db 0xc5			;Wait 6
	.db 0x30			;Note C-5
	.db 0x96,0x03			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x66			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x30			;Note C-5
	.db 0x96,0x02			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x31			;Note C#5
	.db 0x91,0x06			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x2e			;Note A#4
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
					;Wait Repeat 1
	.db 0x31			;Note C#5
	.db 0x76			;Instrument 4
	.db 0x91,0x06			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x31			;Note C#5
	.db 0x76			;Instrument 4
					;Wait Repeat 1
	.db 0x33			;Note D#5
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x76			;Instrument 4
	.db 0x96,0x01			;CMD Portamento up
					;Wait Repeat 1
	.db 0x35			;Note F-5
	.db 0x76			;Instrument 4
	.db 0x91,0x08			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x5a			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x93,0x0a			;CMD Portamento down
	.db 0xc1			;Wait 2
	.db 0x31			;Note C#5
	.db 0x76			;Instrument 4
	.db 0x96,0x04			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x76			;Instrument 4
	.db 0x91,0x0a			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x95,0x74			;CMD Vibrato
	.db 0xbf			;[End-Of-Track]
.track_051:
	.db 0x0a			;Note A#1
	.db 0x71			;Volume 15
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0x0a			;Note A#1
	.db 0xc0			;Wait 1
	.db 0x0a			;Note A#1
					;Wait Repeat 1
	.db 0x0a			;Note A#1
	.db 0xc1			;Wait 2
	.db 0x0a			;Note A#1
	.db 0xc0			;Wait 1
	.db 0x0a			;Note A#1
					;Wait Repeat 1
	.db 0x09			;Note A-1
					;Wait Repeat 1
	.db 0x0a			;Note A#1
					;Wait Repeat 1
	.db 0x0a			;Note A#1
	.db 0xc1			;Wait 2
	.db 0x09			;Note A-1
	.db 0xc0			;Wait 1
	.db 0x0a			;Note A#1
					;Wait Repeat 1
	.db 0x0a			;Note A#1
	.db 0xc1			;Wait 2
	.db 0x05			;Note F-1
					;Wait Repeat 2
	.db 0x05			;Note F-1
	.db 0xc0			;Wait 1
	.db 0x05			;Note F-1
					;Wait Repeat 1
	.db 0x05			;Note F-1
	.db 0xc1			;Wait 2
	.db 0x05			;Note F-1
	.db 0xc0			;Wait 1
	.db 0x05			;Note F-1
					;Wait Repeat 1
	.db 0x08			;Note G#1
	.db 0xc1			;Wait 2
	.db 0x08			;Note G#1
	.db 0xc0			;Wait 1
	.db 0x08			;Note G#1
					;Wait Repeat 1
	.db 0x08			;Note G#1
	.db 0xc1			;Wait 2
	.db 0x08			;Note G#1
	.db 0xc0			;Wait 1
	.db 0x08			;Note G#1
					;Wait Repeat 1
	.db 0x06			;Note F#1
	.db 0xc1			;Wait 2
	.db 0x06			;Note F#1
	.db 0xc0			;Wait 1
	.db 0x06			;Note F#1
					;Wait Repeat 1
	.db 0x06			;Note F#1
	.db 0xc1			;Wait 2
	.db 0x06			;Note F#1
	.db 0xc0			;Wait 1
	.db 0x06			;Note F#1
					;Wait Repeat 1
	.db 0x06			;Note F#1
	.db 0xc1			;Wait 2
	.db 0x06			;Note F#1
	.db 0xc0			;Wait 1
	.db 0x06			;Note F#1
					;Wait Repeat 1
	.db 0x06			;Note F#1
	.db 0xc1			;Wait 2
	.db 0x06			;Note F#1
	.db 0xc0			;Wait 1
	.db 0x08			;Note G#1
					;Wait Repeat 1
	.db 0x0d			;Note C#2
	.db 0xc1			;Wait 2
	.db 0x0d			;Note C#2
	.db 0xc0			;Wait 1
	.db 0x0d			;Note C#2
					;Wait Repeat 1
	.db 0x0d			;Note C#2
					;Wait Repeat 1
	.db 0x14			;Note G#2
					;Wait Repeat 1
	.db 0x19			;Note C#3
					;Wait Repeat 1
	.db 0x19			;Note C#3
					;Wait Repeat 1
	.db 0x08			;Note G#1
	.db 0xc1			;Wait 2
	.db 0x08			;Note G#1
	.db 0xc0			;Wait 1
	.db 0x06			;Note F#1
					;Wait Repeat 1
	.db 0x08			;Note G#1
					;Wait Repeat 1
	.db 0x0f			;Note D#2
					;Wait Repeat 1
	.db 0x14			;Note G#2
					;Wait Repeat 1
	.db 0x14			;Note G#2
	.db 0xbf			;[End-Of-Track]
.track_052:
	.db 0xef			;Wait 48
	.db 0x2c			;Note G#4
	.db 0x68			;Volume 6
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_053:
	.db 0xef			;Wait 48
	.db 0x31			;Note C#5
	.db 0x68			;Volume 6
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0x2c			;Note G#4
					;Wait Repeat 4
	.db 0x2c			;Note G#4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_057:
	.db 0x33			;Note D#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x96,0x02			;CMD Portamento up
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x91,0x04			;CMD Portamento tone
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x6e			;Volume 12
					;Wait Repeat 3
	.db 0x6d			;Volume 11
	.db 0xc2			;Wait 3
	.db 0x38			;Note G#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0x44			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x6e			;Volume 12
	.db 0xc0			;Wait 1
	.db 0x37			;Note G-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
	.db 0xc1			;Wait 2
	.db 0x6e			;Volume 12
	.db 0xc2			;Wait 3
	.db 0x36			;Note F#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
					;Wait Repeat 3
	.db 0x6f			;Volume 13
	.db 0x95,0x75			;CMD Vibrato
					;Wait Repeat 3
	.db 0x6e			;Volume 12
	.db 0xc1			;Wait 2
	.db 0x31			;Note C#5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
					;Wait Repeat 1
	.db 0x35			;Note F-5
	.db 0xc1			;Wait 2
	.db 0x95,0x6b			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x6e			;Volume 12
	.db 0xc2			;Wait 3
	.db 0x6d			;Volume 11
	.db 0xc3			;Wait 4
	.db 0x1b			;Note D#3
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x1d			;Note F-3
					;Wait Repeat 1
	.db 0x1e			;Note F#3
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
	.db 0x95,0xa8			;CMD Vibrato Extended
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x6f			;Volume 13
	.db 0x95,0xa8			;CMD Vibrato Extended
	.db 0xc1			;Wait 2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x6d			;Volume 11
	.db 0xbf			;[End-Of-Track]
.track_064:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_065:
	.db 0x20			;Note G#3
	.db 0x71			;Volume 15
	.db 0x76			;Instrument 4
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x22			;Note A#3
	.db 0x91,0x01			;CMD Portamento tone
					;Wait Repeat 1
	.db 0x92,0x89			;CMD Portamento tone + Volume slide
	.db 0xcd			;Wait 14
	.db 0x2e			;Note A#4
	.db 0x71			;Volume 15
	.db 0xc1			;Wait 2
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x97,0x89			;CMD Vibrato + Volume slide
	.db 0xcc			;Wait 13
	.db 0x20			;Note G#3
	.db 0x71			;Volume 15
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x22			;Note A#3
	.db 0x91,0x04			;CMD Portamento tone
					;Wait Repeat 1
	.db 0x92,0x83			;CMD Portamento tone + Volume slide
	.db 0xc5			;Wait 6
	.db 0x2e			;Note A#4
	.db 0x71			;Volume 15
	.db 0xc1			;Wait 2
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x97,0x83			;CMD Vibrato + Volume slide
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
.track_066:
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
.track_072:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_073:
	.db 0x2c			;Note G#4
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0x99,0x7b			;CMD Tremolo
	.db 0xcf			;Wait 16
	.db 0x38			;Note G#5
	.db 0x99,0x3f			;CMD Tremolo
					;Wait Repeat 16
	.db 0x20			;Note G#3
	.db 0x70			;Volume 14
	.db 0x99,0x83			;CMD Tremolo
					;Wait Repeat 16
	.db 0x20			;Note G#3
	.db 0x71			;Volume 15
	.db 0x99,0x87			;CMD Tremolo
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
.track_080:
	.db 0xdf			;Wait 32
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa6,0xaf			;CMD Envelope multiplier low
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_081:
	.db 0x0c			;Note C-2
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xa5,0x01			;CMD Envelope multiplier high
	.db 0xc2			;Wait 3
	.db 0xa5,0x00			;CMD Envelope multiplier high
	.db 0xc0			;Wait 1
	.db 0x3c			;Note C-6
	.db 0xa6,0x50			;CMD Envelope multiplier low
	.db 0xc3			;Wait 4
	.db 0x54			;Note C-8
	.db 0xa6,0x10			;CMD Envelope multiplier low
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xa5,0x30			;CMD Envelope multiplier high
	.db 0xc9			;Wait 10
	.db 0xa6,0x00			;CMD Envelope multiplier low
	.db 0xc0			;Wait 1
	.db 0xa5,0x00			;CMD Envelope multiplier high
					;Wait Repeat 1
	.db 0x59			;Note F-8
	.db 0x73			;Instrument 1
	.db 0xa6,0x09			;CMD Envelope multiplier low
	.db 0xc6			;Wait 7
	.db 0xa6,0x00			;CMD Envelope multiplier low
	.db 0xc0			;Wait 1
	.db 0x1d			;Note F-3
	.db 0x77			;Instrument 5
	.db 0xa5,0x04			;CMD Envelope multiplier high
	.db 0xcf			;Wait 16
	.db 0x10			;Note E-2
	.db 0x78			;Instrument 6
	.db 0xa5,0x0f			;CMD Envelope multiplier high
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
.track_082:
	.db 0x60			;Release 96
	.db 0xa6,0x00			;CMD Envelope multiplier low
	.db 0xc0			;Wait 1
	.db 0x9c			;CMD End 
	.db 0xfe			;Wait 63
	.db 0xbf			;[End-Of-Track]
.track_091:
	.db 0x09			;Note A-1
	.db 0x71			;Volume 15
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x09			;Note A-1
	.db 0x71			;Volume 15
	.db 0xc1			;Wait 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x09			;Note A-1
	.db 0x71			;Volume 15
	.db 0xc1			;Wait 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0x09			;Note A-1
	.db 0x71			;Volume 15
	.db 0xc1			;Wait 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_092:
	.db 0xcf			;Wait 16
	.db 0x26			;Note D-4
	.db 0x6b			;Volume 9
	.db 0x76			;Instrument 4
	.db 0xaa,0x38			; SCC Set Waveform 8
	.db 0xc2			;Wait 3
	.db 0x98,0x8b			;CMD Volume slide up
	.db 0xcc			;Wait 13
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x7b			;Instrument 9
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x98,0x8f			;CMD Volume slide up
	.db 0xcc			;Wait 13
	.db 0x39			;Note A-5
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xaa,0x58			; SCC Set Waveform 12
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xa9				; SCC Soften Waveform
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xaa,0x58			; SCC Set Waveform 12
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xa9				; SCC Soften Waveform
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0x71			;Volume 15
	.db 0xaa,0x78			; SCC Set Waveform 19
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xa9				; SCC Soften Waveform
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xaa,0x78			; SCC Set Waveform 19
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xa9				; SCC Soften Waveform
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
.track_093:
	.db 0x1f			;Note G-3
	.db 0x6c			;Volume 10
	.db 0x76			;Instrument 4
	.db 0xaa,0x40			; SCC Set Waveform 9
	.db 0xc1			;Wait 2
	.db 0xaa,0x38			; SCC Set Waveform 8
					;Wait Repeat 2
	.db 0xaa,0x48			; SCC Set Waveform 10
					;Wait Repeat 2
	.db 0xaa,0x00			; SCC Set Waveform 0
					;Wait Repeat 2
	.db 0xaa,0x78			; SCC Set Waveform 19
					;Wait Repeat 2
	.db 0xaa,0x80			; SCC Set Waveform 20
					;Wait Repeat 2
	.db 0xaa,0x88			; SCC Set Waveform 22
					;Wait Repeat 2
	.db 0xaa,0x30			; SCC Set Waveform 7
					;Wait Repeat 2
	.db 0x2d			;Note A-4
	.db 0x6b			;Volume 9
	.db 0xaa,0x38			; SCC Set Waveform 8
					;Wait Repeat 2
	.db 0x95,0x75			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x97,0x8b			;CMD Vibrato + Volume slide
	.db 0xcc			;Wait 13
	.db 0x28			;Note E-4
	.db 0x71			;Volume 15
	.db 0x7b			;Instrument 9
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x98,0x8f			;CMD Volume slide up
	.db 0xdc			;Wait 29
	.db 0xbf			;[End-Of-Track]
.track_094:
	.db 0xcf			;Wait 16
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 9
	.db 0x76			;Instrument 4
	.db 0xaa,0x38			; SCC Set Waveform 8
	.db 0xc1			;Wait 2
	.db 0x95,0x75			;CMD Vibrato
	.db 0xc0			;Wait 1
	.db 0x97,0x8b			;CMD Vibrato + Volume slide
	.db 0xcc			;Wait 13
	.db 0x2b			;Note G-4
	.db 0x71			;Volume 15
	.db 0x7b			;Instrument 9
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x98,0x8f			;CMD Volume slide up
	.db 0xdc			;Wait 29
	.db 0xbf			;[End-Of-Track]
.track_099:
	.db 0x05			;Note F-1
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xaa,0x38			; SCC Set Waveform 8
	.db 0xcf			;Wait 16
	.db 0x2b			;Note G-4
	.db 0x6f			;Volume 13
	.db 0xaa,0x58			; SCC Set Waveform 12
	.db 0xc0			;Wait 1
	.db 0xad,0x00			; SCC Morph speed 0
					;Wait Repeat 1
	.db 0xac,0x01			; SCC Morph follow (Carbon C0poy) 1
	.db 0xc1			;Wait 2
	.db 0x9d,0x10			; SCC Morph to Waveform 2
	.db 0xc3			;Wait 4
	.db 0x9d,0x58			; SCC Morph to Waveform 12
					;Wait Repeat 4
	.db 0x9d,0x10			; SCC Morph to Waveform 2
					;Wait Repeat 4
	.db 0x26			;Note D-4
	.db 0x6d			;Volume 11
	.db 0x9d,0x58			; SCC Morph to Waveform 12
					;Wait Repeat 4
	.db 0x9d,0x10			; SCC Morph to Waveform 2
					;Wait Repeat 4
	.db 0x9d,0x58			; SCC Morph to Waveform 12
					;Wait Repeat 4
	.db 0x9d,0x10			; SCC Morph to Waveform 2
					;Wait Repeat 4
	.db 0x9d,0x58			; SCC Morph to Waveform 12
					;Wait Repeat 4
	.db 0x9d,0x90			; SCC Morph to Waveform 28
					;Wait Repeat 4
	.db 0x9d,0x68			; SCC Morph to Waveform 16
					;Wait Repeat 4
	.db 0x9d,0x00			; SCC Morph to Waveform 0
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_100:
	.db 0x05			;Note F-1
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xaa,0x20			; SCC Set Waveform 4
	.db 0xc0			;Wait 1
	.db 0x9d,0x98			; SCC Morph to Waveform 31
	.db 0xce			;Wait 15
	.db 0x60			;Release 96
	.db 0xcf			;Wait 16
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
	.db 0xab				; SCC Morph foloow (Carbon C0poy) 0
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
.track_101:
	.db 0xad,0x02			; SCC Morph speed 2
	.db 0xcf			;Wait 16
	.db 0x60			;Release 96
	.db 0xcf			;Wait 16
	.db 0x2f			;Note B-4
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
	.db 0xab				; SCC Morph foloow (Carbon C0poy) 0
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
.track_102:
	.db 0xcf			;Wait 16
	.db 0x60			;Release 96
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
.track_104:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x0b			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x0a			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x09			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x08			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x07			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x06			;CMD Speed
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_105:
	.db 0x60			;Release 96
	.db 0xd9			;Wait 26
	.db 0x1f			;Note G-3
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
	.db 0xc0			;Wait 1
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x70			;Volume 14
	.db 0x75			;Instrument 3
	.db 0xa2,0x07			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x41			;Note F-6
	.db 0xa2,0x06			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x05			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x04			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x03			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x02			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x01			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x08			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x09			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0a			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0b			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0c			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0d			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0e			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0f			;CMD Track detune
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xc1			;Wait 2
	.db 0x41			;Note F-6
	.db 0x76			;Instrument 4
	.db 0xa2,0x00			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_106:
	.db 0x60			;Release 96
	.db 0xdb			;Wait 28
	.db 0x41			;Note F-6
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
					;Wait Repeat 4
	.db 0x41			;Note F-6
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_107:
	.db 0x37			;Note G-5
	.db 0x70			;Volume 14
	.db 0x76			;Instrument 4
	.db 0xaa,0x58			; SCC Set Waveform 12
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
					;Wait Repeat 1
	.db 0x34			;Note E-5
					;Wait Repeat 1
	.db 0x32			;Note D-5
					;Wait Repeat 1
	.db 0x29			;Note F-4
	.db 0xc0			;Wait 1
	.db 0x2b			;Note G-4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 1
	.db 0x2d			;Note A-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0x9b,0x03			;CMD Note delay
					;Wait Repeat 1
	.db 0x2b			;Note G-4
	.db 0x9b,0x05			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x7c			;Instrument 10
	.db 0xaa,0x58			; SCC Set Waveform 12
					;Wait Repeat 2
	.db 0x2e			;Note A#4
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x2d			;Note A-4
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x22			;Note A#3
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
					;Wait Repeat 2
	.db 0x1f			;Note G-3
	.db 0x7c			;Instrument 10
	.db 0xa1			;CMD Note link
	.db 0xc0			;Wait 1
	.db 0x9e,0x00			;CMD Arpegio speed
					;Wait Repeat 1
	.db 0x41			;Note F-6
	.db 0x70			;Volume 14
	.db 0x75			;Instrument 3
	.db 0xa2,0x07			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x41			;Note F-6
	.db 0xa2,0x06			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x05			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x04			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x03			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x02			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x01			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x08			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x09			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0a			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0b			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0c			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0d			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0e			;CMD Track detune
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xa2,0x0f			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0x41			;Note F-6
	.db 0x76			;Instrument 4
	.db 0xa2,0x00			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_112:
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xa4,0x06			;CMD Speed
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xa4,0x05			;CMD Speed
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
					;Wait Repeat 4
	.db 0x5d			;Note A-8
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
.track_115:
	.db 0x3c			;Note C-6
	.db 0x76			;Instrument 4
	.db 0xaa,0x58			; SCC Set Waveform 12
	.db 0xc1			;Wait 2
	.db 0x37			;Note G-5
	.db 0x76			;Instrument 4
					;Wait Repeat 2
	.db 0x34			;Note E-5
	.db 0x76			;Instrument 4
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0x76			;Instrument 4
					;Wait Repeat 2
	.db 0x3c			;Note C-6
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x3b			;Note B-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x39			;Note A-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x37			;Note G-5
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x32			;Note D-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x34			;Note E-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x37			;Note G-5
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3c			;Note C-6
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x40			;Note E-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x48			;Note C-7
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x47			;Note B-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x45			;Note A-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x43			;Note G-6
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x41			;Note F-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x3e			;Note D-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3c			;Note C-6
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x40			;Note E-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x43			;Note G-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x48			;Note C-7
	.db 0x76			;Instrument 4
	.db 0xc0			;Wait 1
	.db 0x43			;Note G-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x02			;CMD Note delay
					;Wait Repeat 1
	.db 0x40			;Note E-6
	.db 0x76			;Instrument 4
	.db 0x9b,0x03			;CMD Note delay
	.db 0xc1			;Wait 2
	.db 0x3c			;Note C-6
	.db 0x76			;Instrument 4
					;Wait Repeat 2
	.db 0x60			;Release 96
	.db 0xd5			;Wait 22
	.db 0xbf			;[End-Of-Track]
.track_120:
	.db 0x24			;Note C-4
	.db 0x70			;Volume 14
	.db 0x7f			;Instrument 13
	.db 0xa4,0x0c			;CMD Speed
	.db 0xd4			;Wait 21
	.db 0x60			;Release 96
	.db 0xc2			;Wait 3
	.db 0x4f			;Note G-7
	.db 0x71			;Volume 15
	.db 0x7d			;Instrument 11
	.db 0xc3			;Wait 4
	.db 0x4f			;Note G-7
					;Wait Repeat 4
	.db 0x4f			;Note G-7
					;Wait Repeat 4
	.db 0x4f			;Note G-7
	.db 0xc3			;Wait 4
	.db 0x18			;Note C-3
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x18			;Note C-3
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x18			;Note C-3
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x18			;Note C-3
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]

