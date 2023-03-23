; Song: NEM3- Air Battle                
; By:   Konami (Arr by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	.db 0x04					; Initial song speed
	.dw nem3airb_waveform_start			; Start of the waveform data
	.dw nem3airb_instrument_start			; Start of the instrument data

; [ Song order pointer list ]
	.dw nem3airb_track_000, nem3airb_track_000, nem3airb_track_000, nem3airb_track_003, nem3airb_track_000, nem3airb_track_000, nem3airb_track_000, nem3airb_track_000	; Step:000 Pattern:000
	.dw nem3airb_track_008, nem3airb_track_009, nem3airb_track_010, nem3airb_track_011, nem3airb_track_012, nem3airb_track_013, nem3airb_track_014, nem3airb_track_015	; Step:001 Pattern:001
	.dw nem3airb_track_016, nem3airb_track_017, nem3airb_track_018, nem3airb_track_018, nem3airb_track_020, nem3airb_track_021, nem3airb_track_014, nem3airb_track_015	; Step:002 Pattern:002
nem3airb_restart:
	.dw nem3airb_track_008, nem3airb_track_025, nem3airb_track_018, nem3airb_track_018, nem3airb_track_028, nem3airb_track_029, nem3airb_track_030, nem3airb_track_031	; Step:003 Pattern:003
	.dw nem3airb_track_008, nem3airb_track_033, nem3airb_track_018, nem3airb_track_018, nem3airb_track_036, nem3airb_track_037, nem3airb_track_038, nem3airb_track_039	; Step:004 Pattern:004
	.dw nem3airb_track_008, nem3airb_track_041, nem3airb_track_018, nem3airb_track_018, nem3airb_track_028, nem3airb_track_029, nem3airb_track_030, nem3airb_track_047	; Step:005 Pattern:005
	.dw nem3airb_track_016, nem3airb_track_049, nem3airb_track_018, nem3airb_track_018, nem3airb_track_052, nem3airb_track_037, nem3airb_track_054, nem3airb_track_055	; Step:006 Pattern:006
	.dw nem3airb_track_056, nem3airb_track_057, nem3airb_track_018, nem3airb_track_018, nem3airb_track_060, nem3airb_track_061, nem3airb_track_062, nem3airb_track_063	; Step:007 Pattern:007
	.dw nem3airb_track_056, nem3airb_track_065, nem3airb_track_018, nem3airb_track_018, nem3airb_track_068, nem3airb_track_069, nem3airb_track_070, nem3airb_track_071	; Step:008 Pattern:008
	.dw nem3airb_track_056, nem3airb_track_073, nem3airb_track_018, nem3airb_track_018, nem3airb_track_076, nem3airb_track_077, nem3airb_track_078, nem3airb_track_079	; Step:009 Pattern:009
	.dw nem3airb_track_080, nem3airb_track_081, nem3airb_track_018, nem3airb_track_018, nem3airb_track_084, nem3airb_track_085, nem3airb_track_086, nem3airb_track_087	; Step:010 Pattern:010
	.dw nem3airb_track_088, nem3airb_track_089, nem3airb_track_018, nem3airb_track_018, nem3airb_track_092, nem3airb_track_093, nem3airb_track_094, nem3airb_track_095	; Step:011 Pattern:011
	.dw nem3airb_track_096, nem3airb_track_097, nem3airb_track_018, nem3airb_track_018, nem3airb_track_100, nem3airb_track_101, nem3airb_track_102, nem3airb_track_103	; Step:012 Pattern:012
	.dw nem3airb_track_104, nem3airb_track_105, nem3airb_track_018, nem3airb_track_018, nem3airb_track_100, nem3airb_track_109, nem3airb_track_110, nem3airb_track_111	; Step:013 Pattern:013
	.dw nem3airb_track_096, nem3airb_track_113, nem3airb_track_018, nem3airb_track_018, nem3airb_track_100, nem3airb_track_117, nem3airb_track_102, nem3airb_track_103	; Step:014 Pattern:014
	.dw nem3airb_track_104, nem3airb_track_105, nem3airb_track_018, nem3airb_track_018, nem3airb_track_100, nem3airb_track_109, nem3airb_track_126, nem3airb_track_127	; Step:015 Pattern:015
	.dw nem3airb_track_096, nem3airb_track_129, nem3airb_track_018, nem3airb_track_018, nem3airb_track_132, nem3airb_track_133, nem3airb_track_134, nem3airb_track_135	; Step:016 Pattern:016
	.dw nem3airb_track_136, nem3airb_track_137, nem3airb_track_018, nem3airb_track_018, nem3airb_track_140, nem3airb_track_141, nem3airb_track_142, nem3airb_track_143	; Step:017 Pattern:017
	.dw nem3airb_track_144, nem3airb_track_145, nem3airb_track_018, nem3airb_track_018, nem3airb_track_148, nem3airb_track_149, nem3airb_track_150, nem3airb_track_151	; Step:018 Pattern:018
	.dw 0x0000, nem3airb_restart				; End of sequence delimiter + nem3airb_restart address


; [ SCC Waveforms ]
nem3airb_waveform_start:
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0x00, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x81, 0xc0, 0x01, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x01, 0x40, 0x01, 0xe0, 0x01, 0x20, 0x01, 0xf0, 0x01, 0x10, 0x01, 0xff, 0xff, 0xff, 0xff, 0x40, 0x40, 0x40				; Waveform:2
	.db 0x00, 0xf8, 0xf0, 0xe8, 0xe0, 0xd8, 0xd0, 0xc8, 0xc0, 0xb8, 0xb0, 0xa8, 0xa0, 0x98, 0x90, 0x88, 0x80, 0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08				; Waveform:6
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x80, 0xaa, 0xc8, 0x00, 0x24, 0x40, 0x5c, 0x70, 0x7f, 0x6a, 0x4a, 0x26, 0x00, 0xd0, 0xa8, 0x8c, 0x80, 0xaa, 0xc8, 0x00, 0x24, 0x40, 0x5c, 0x70, 0x7f, 0x6a, 0x4a, 0x26, 0x00, 0xd0, 0xa8, 0x8c				; Waveform:11
	.db 0x30, 0x50, 0x50, 0x30, 0x00, 0x00, 0x10, 0x40, 0x60, 0x70, 0x60, 0x30, 0xf0, 0xe0, 0xe0, 0x00, 0x20, 0x20, 0x10, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00, 0xd0, 0xb0, 0xb0, 0xd0, 0x00, 0x00				; Waveform:13
	.db 0x00, 0x7f, 0x00, 0x80, 0xa0, 0xc0, 0xd8, 0xf0, 0x08, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x78, 0x7c, 0x7f, 0x7c, 0x78, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x08, 0xf0, 0xd8, 0xc0, 0xa0, 0x80				; Waveform:17
	.db 0x00, 0x68, 0x40, 0x04, 0x00, 0x26, 0x40, 0xf5, 0x10, 0x40, 0x20, 0xd0, 0xf0, 0x00, 0xc0, 0x80, 0xd0, 0x30, 0xc0, 0xa0, 0xb0, 0xc0, 0x80, 0xd0, 0xf0, 0x40, 0x10, 0xc0, 0xd4, 0x00, 0xc0, 0xe0				; Waveform:19
	.db 0x00, 0x68, 0x48, 0x18, 0x48, 0x68, 0x28, 0x00, 0x48, 0x77, 0x58, 0x08, 0x28, 0x38, 0x00, 0xb8, 0x08, 0x58, 0x00, 0xe8, 0xf8, 0x00, 0xb8, 0x98, 0xc8, 0x08, 0xe8, 0xa8, 0xc8, 0xf8, 0xc8, 0xa8				; Waveform:20
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x00, 0xe7, 0xcf, 0xb9, 0xa6, 0x96, 0x8b, 0x83, 0x80, 0x83, 0x8b, 0x96, 0xa6, 0xb9, 0xcf, 0xe7				; Waveform:21
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60				; Waveform:25
	.db 0x90, 0x88, 0x90, 0x9c, 0xb0, 0xc0, 0xe0, 0x26, 0x70, 0x7c, 0x70, 0x56, 0x40, 0x20, 0x10, 0xfc, 0xe0, 0xfc, 0x10, 0xfc, 0x40, 0x54, 0x72, 0x7f, 0x70, 0x26, 0xe0, 0xc0, 0xb0, 0x9c, 0x90, 0x88				; Waveform:30
	.db 0x00, 0x00, 0x00, 0x80, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80				; Waveform:31

; [ Instruments]
nem3airb_instrument_start:
	.dw nem3airb_instrument_00				; BD G2           
	.dw nem3airb_instrument_01				; SD A3           
	.dw nem3airb_instrument_02				; HH CL A8        
	.dw nem3airb_instrument_03				; TOM             
	.dw nem3airb_instrument_04				; Short Snare     
	.dw nem3airb_instrument_05				; PAUSE K         
	.dw nem3airb_instrument_06				; SC Bass         
	.dw nem3airb_instrument_07				; Pause final     
	.dw nem3airb_instrument_08				; BASS LONG       
	.dw nem3airb_instrument_09				; Staccato BASS   
	.dw nem3airb_instrument_10				; ARP SQ          
	.dw nem3airb_instrument_11				; ARP SCC         
	.dw nem3airb_instrument_12				; ARP2 SCC        
	.dw nem3airb_instrument_13				; CHORDS END      
	.dw nem3airb_instrument_14				; ARP SCC3        
	.dw nem3airb_instrument_15				; THEME SCC       
	.dw nem3airb_instrument_16				; THEME LOW       
	.dw nem3airb_instrument_17				; CHORDS FINAL    
	.dw nem3airb_instrument_18				; SLIDE FINAL     
	.dw nem3airb_instrument_19				; CONT            

nem3airb_instrument_00:					; BD G2           
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x03						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3airb_instrument_01:					; SD A3           
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x01						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0b						; Volume _
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

nem3airb_instrument_02:					; HH CL A8        
	.db 0x18						; Waveform 3
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3airb_instrument_03:					; TOM             
	.db 0x48						; Waveform 9
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x10,0x00					; Tone add
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x10,0x00					; Tone add
	.db 0x08,0x0e						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x10,0x00					; Tone add
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x10,0x00					; Tone add
	.db 0x00							; Volume same
	.db 0x18,0xf2						; Loop (-15)

nem3airb_instrument_04:					; Short Snare     
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x06						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x06						; Noise _
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x18,0xed						; Loop (-20)

nem3airb_instrument_05:					; PAUSE K         
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_06:					; SC Bass         
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_07:					; Pause final     
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3airb_instrument_08:					; BASS LONG       
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_09:					; Staccato BASS   
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
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
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_10:					; ARP SQ          
	.db 0x38						; Waveform 7
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_11:					; ARP SCC         
	.db 0x40						; Waveform 8
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_12:					; ARP2 SCC        
	.db 0x40						; Waveform 8
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_13:					; CHORDS END      
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
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
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_14:					; ARP SCC3        
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_15:					; THEME SCC       
	.db 0x50						; Waveform 10
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_16:					; THEME LOW       
	.db 0x38						; Waveform 7
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
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
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
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
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xd0						; Loop (-49)

nem3airb_instrument_17:					; CHORDS FINAL    
	.db 0x30						; Waveform 6
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
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
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_18:					; SLIDE FINAL     
	.db 0x10						; Waveform 2
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
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
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3airb_instrument_19:					; CONT            
	.db 0x58						; Waveform 11
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x18,0xfb						; Loop (-6)


; [ Song track data ]
nem3airb_track_000:
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
nem3airb_track_003:
	.db 0x30			;Note C-5
	.db 0x6f			;Volume 13
	.db 0x77			;Instrument 5
	.db 0xa4,0x05			;CMD Speed
	.db 0xc1			;Wait 2
	.db 0x37			;Note G-5
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
					;Wait Repeat 2
	.db 0x3c			;Note C-6
	.db 0x79			;Instrument 7
	.db 0xc7			;Wait 8
	.db 0x60			;Release 96
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_008:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_009:
	.db 0x60			;Release 96
	.db 0xa6,0x80			;CMD Envelope multiplier low
	.db 0xc5			;Wait 6
	.db 0x32			;Note D-5
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_010:
	.db 0xa5,0x02			;CMD Envelope multiplier high
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
nem3airb_track_011:
	.db 0xa4,0x04			;CMD Speed
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
nem3airb_track_012:
	.db 0x0c			;Note C-2
	.db 0x6e			;Volume 12
	.db 0x78			;Instrument 6
	.db 0xc7			;Wait 8
	.db 0x18			;Note C-3
	.db 0xc3			;Wait 4
	.db 0x0c			;Note C-2
	.db 0xc7			;Wait 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
					;Wait Repeat 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
					;Wait Repeat 8
	.db 0x0c			;Note C-2
	.db 0xc3			;Wait 4
	.db 0x18			;Note C-3
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_013:
	.db 0x32			;Note D-5
	.db 0x69			;Volume 7
	.db 0x7d			;Instrument 11
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_014:
	.db 0x2b			;Note G-4
	.db 0x6f			;Volume 13
	.db 0x78			;Instrument 6
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xcb			;Wait 12
	.db 0x2b			;Note G-4
					;Wait Repeat 12
	.db 0x2b			;Note G-4
					;Wait Repeat 12
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x2a			;Note F#4
	.db 0x7f			;Instrument 13
	.db 0xc7			;Wait 8
	.db 0x2a			;Note F#4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_015:
	.db 0x26			;Note D-4
	.db 0x6f			;Volume 13
	.db 0x78			;Instrument 6
	.db 0xcb			;Wait 12
	.db 0x26			;Note D-4
					;Wait Repeat 12
	.db 0x26			;Note D-4
					;Wait Repeat 12
	.db 0x26			;Note D-4
	.db 0xcb			;Wait 12
	.db 0x26			;Note D-4
	.db 0x7f			;Instrument 13
	.db 0xc7			;Wait 8
	.db 0x26			;Note D-4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_016:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_017:
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x28			;Note E-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2a			;Note F#4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_018:
	.db 0xff			;Wait 64
	.db 0xbf			;[End-Of-Track]
nem3airb_track_020:
	.db 0x0c			;Note C-2
	.db 0x6e			;Volume 12
	.db 0x78			;Instrument 6
	.db 0xc7			;Wait 8
	.db 0x18			;Note C-3
	.db 0xc3			;Wait 4
	.db 0x0c			;Note C-2
	.db 0xc7			;Wait 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
					;Wait Repeat 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
	.db 0xc3			;Wait 4
	.db 0x0a			;Note A#1
					;Wait Repeat 4
	.db 0x16			;Note A#2
					;Wait Repeat 4
	.db 0x0b			;Note B-1
					;Wait Repeat 4
	.db 0x17			;Note B-2
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_021:
	.db 0x32			;Note D-5
	.db 0x69			;Volume 7
	.db 0x7d			;Instrument 11
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x28			;Note E-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_025:
	.db 0xc3			;Wait 4
	.db 0x2d			;Note A-4
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x2a			;Note F#4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xc7			;Wait 8
	.db 0x2d			;Note A-4
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0xc8			;Wait 9
	.db 0x95,0x47			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0xbf			;[End-Of-Track]
nem3airb_track_028:
	.db 0x0c			;Note C-2
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x18			;Note C-3
	.db 0xc3			;Wait 4
	.db 0x0c			;Note C-2
	.db 0xc7			;Wait 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
					;Wait Repeat 8
	.db 0x0c			;Note C-2
					;Wait Repeat 8
	.db 0x18			;Note C-3
					;Wait Repeat 8
	.db 0x0c			;Note C-2
	.db 0xc3			;Wait 4
	.db 0x18			;Note C-3
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_029:
	.db 0x32			;Note D-5
	.db 0x68			;Volume 6
	.db 0x7e			;Instrument 12
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_030:
	.db 0x2d			;Note A-4
	.db 0x6c			;Volume 10
	.db 0x81			;Instrument 15
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x2a			;Note F#4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xc7			;Wait 8
	.db 0x2d			;Note A-4
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0xcd			;Wait 14
	.db 0x95,0x57			;CMD Vibrato
	.db 0xd8			;Wait 25
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_031:
	.db 0x2a			;Note F#4
	.db 0x6c			;Volume 10
	.db 0x81			;Instrument 15
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
	.db 0xc7			;Wait 8
	.db 0x2a			;Note F#4
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0xcd			;Wait 14
	.db 0x95,0x57			;CMD Vibrato
	.db 0xd8			;Wait 25
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_033:
	.db 0xc2			;Wait 3
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
	.db 0xc7			;Wait 8
	.db 0x95,0x47			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xc8			;Wait 9
	.db 0x95,0x47			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0xbf			;[End-Of-Track]
nem3airb_track_036:
	.db 0x0a			;Note A#1
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x16			;Note A#2
	.db 0xc3			;Wait 4
	.db 0x0a			;Note A#1
	.db 0xc7			;Wait 8
	.db 0x0a			;Note A#1
					;Wait Repeat 8
	.db 0x16			;Note A#2
					;Wait Repeat 8
	.db 0x0a			;Note A#1
					;Wait Repeat 8
	.db 0x16			;Note A#2
					;Wait Repeat 8
	.db 0x0a			;Note A#1
	.db 0xc3			;Wait 4
	.db 0x16			;Note A#2
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_037:
	.db 0x30			;Note C-5
	.db 0x68			;Volume 6
	.db 0x7e			;Instrument 12
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_038:
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x81			;Instrument 15
	.db 0xaa,0x38			; SCC Set Waveform 19
	.db 0xc7			;Wait 8
	.db 0x95,0x57			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xc8			;Wait 9
	.db 0x95,0x57			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x29			;Note F-4
	.db 0x6c			;Volume 10
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc0			;Wait 1
	.db 0x2a			;Note F#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x2c			;Note G#4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_039:
	.db 0x1c			;Note E-3
	.db 0x6d			;Volume 11
	.db 0x82			;Instrument 16
	.db 0xd3			;Wait 20
	.db 0x1d			;Note F-3
	.db 0xc3			;Wait 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x24			;Note C-4
	.db 0xdf			;Wait 32
	.db 0x29			;Note F-4
	.db 0x68			;Volume 6
	.db 0xc0			;Wait 1
	.db 0x2a			;Note F#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x2c			;Note G#4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_041:
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 9
	.db 0x85			;Instrument 19
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x2a			;Note F#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x2c			;Note G#4
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0x6d			;Volume 11
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x2a			;Note F#4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xc7			;Wait 8
	.db 0x2d			;Note A-4
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0xc8			;Wait 9
	.db 0x95,0x47			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0xbf			;[End-Of-Track]
nem3airb_track_047:
	.db 0x2a			;Note F#4
	.db 0x6c			;Volume 10
	.db 0x81			;Instrument 15
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
	.db 0xc7			;Wait 8
	.db 0x2a			;Note F#4
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0xcd			;Wait 14
	.db 0x95,0x57			;CMD Vibrato
	.db 0xd8			;Wait 25
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_049:
	.db 0xc2			;Wait 3
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
	.db 0xc7			;Wait 8
	.db 0x95,0x47			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xc8			;Wait 9
	.db 0x95,0x47			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x2b			;Note G-4
	.db 0x9c			;CMD End 
	.db 0xc7			;Wait 8
	.db 0x29			;Note F-4
					;Wait Repeat 8
	.db 0x28			;Note E-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_052:
	.db 0x0a			;Note A#1
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x16			;Note A#2
	.db 0xc3			;Wait 4
	.db 0x0a			;Note A#1
	.db 0xc7			;Wait 8
	.db 0x0a			;Note A#1
					;Wait Repeat 8
	.db 0x16			;Note A#2
					;Wait Repeat 8
	.db 0x0a			;Note A#1
					;Wait Repeat 8
	.db 0x16			;Note A#2
	.db 0xc3			;Wait 4
	.db 0x0a			;Note A#1
					;Wait Repeat 4
	.db 0x0c			;Note C-2
					;Wait Repeat 4
	.db 0x0e			;Note D-2
					;Wait Repeat 4
	.db 0x0a			;Note A#1
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_054:
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x81			;Instrument 15
	.db 0xd3			;Wait 20
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xcb			;Wait 12
	.db 0x2b			;Note G-4
	.db 0x6c			;Volume 10
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc7			;Wait 8
	.db 0x29			;Note F-4
					;Wait Repeat 8
	.db 0x28			;Note E-4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_055:
	.db 0x24			;Note C-4
	.db 0x6d			;Volume 11
	.db 0x81			;Instrument 15
	.db 0x9c			;CMD End 
	.db 0xd3			;Wait 20
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x30			;Note C-5
	.db 0x82			;Instrument 16
	.db 0xc7			;Wait 8
	.db 0x32			;Note D-5
					;Wait Repeat 8
	.db 0x34			;Note E-5
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_056:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_057:
	.db 0x6b			;Volume 9
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
	.db 0xc6			;Wait 7
	.db 0x95,0x15			;CMD Vibrato
	.db 0xcc			;Wait 13
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xc9			;Wait 10
	.db 0x95,0x37			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0xbf			;[End-Of-Track]
nem3airb_track_060:
	.db 0x09			;Note A-1
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x15			;Note A-2
	.db 0xc3			;Wait 4
	.db 0x09			;Note A-1
	.db 0xc7			;Wait 8
	.db 0x09			;Note A-1
					;Wait Repeat 8
	.db 0x15			;Note A-2
					;Wait Repeat 8
	.db 0x09			;Note A-1
					;Wait Repeat 8
	.db 0x09			;Note A-1
	.db 0xc3			;Wait 4
	.db 0x0e			;Note D-2
					;Wait Repeat 4
	.db 0x10			;Note E-2
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x15			;Note A-2
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_061:
	.db 0x18			;Note C-3
	.db 0x6c			;Volume 10
	.db 0x80			;Instrument 14
	.db 0xc3			;Wait 4
	.db 0x1c			;Note E-3
					;Wait Repeat 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x34			;Note E-5
					;Wait Repeat 4
	.db 0x37			;Note G-5
					;Wait Repeat 4
	.db 0x34			;Note E-5
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x1c			;Note E-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_062:
	.db 0x28			;Note E-4
	.db 0x6c			;Volume 10
	.db 0x81			;Instrument 15
	.db 0xc6			;Wait 7
	.db 0x95,0x15			;CMD Vibrato
	.db 0xcc			;Wait 13
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xcb			;Wait 12
	.db 0x95,0x35			;CMD Vibrato
	.db 0xd3			;Wait 20
	.db 0x2b			;Note G-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_063:
	.db 0x34			;Note E-5
	.db 0x6b			;Volume 9
	.db 0x82			;Instrument 16
	.db 0xa2,0x08			;CMD Track detune
	.db 0xd3			;Wait 20
	.db 0x35			;Note F-5
	.db 0xc3			;Wait 4
	.db 0x37			;Note G-5
					;Wait Repeat 4
	.db 0x3c			;Note C-6
	.db 0xcb			;Wait 12
	.db 0x95,0x26			;CMD Vibrato
	.db 0xd3			;Wait 20
	.db 0x37			;Note G-5
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_065:
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0x6d			;Volume 11
	.db 0x9c			;CMD End 
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x33			;Note D#5
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xce			;Wait 15
	.db 0x95,0x57			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_068:
	.db 0x14			;Note G#2
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x14			;Note G#2
	.db 0xc7			;Wait 8
	.db 0x14			;Note G#2
					;Wait Repeat 8
	.db 0x20			;Note G#3
					;Wait Repeat 8
	.db 0x14			;Note G#2
					;Wait Repeat 8
	.db 0x20			;Note G#3
					;Wait Repeat 8
	.db 0x14			;Note G#2
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_069:
	.db 0x18			;Note C-3
	.db 0x6c			;Volume 10
	.db 0x80			;Instrument 14
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x37			;Note G-5
					;Wait Repeat 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_070:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 9
	.db 0x81			;Instrument 15
	.db 0xaa,0x60			; SCC Set Waveform 31
	.db 0xc3			;Wait 4
	.db 0x32			;Note D-5
					;Wait Repeat 4
	.db 0x33			;Note D#5
					;Wait Repeat 4
	.db 0x30			;Note C-5
	.db 0xcb			;Wait 12
	.db 0x95,0x67			;CMD Vibrato
	.db 0xe2			;Wait 35
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x32			;Note D-5
	.db 0x6d			;Volume 11
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_071:
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 9
	.db 0x81			;Instrument 15
	.db 0xa2,0x00			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0x2e			;Note A#4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x95,0x67			;CMD Vibrato
	.db 0xe3			;Wait 36
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
	.db 0x82			;Instrument 16
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_073:
	.db 0x32			;Note D-5
	.db 0x6d			;Volume 11
	.db 0x85			;Instrument 19
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x34			;Note E-5
	.db 0x6d			;Volume 11
	.db 0xcf			;Wait 16
	.db 0x30			;Note C-5
	.db 0xcb			;Wait 12
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x95,0x16			;CMD Vibrato
					;Wait Repeat 12
	.db 0x6a			;Volume 8
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_076:
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0xc7			;Wait 8
	.db 0x13			;Note G-2
					;Wait Repeat 8
	.db 0x1f			;Note G-3
					;Wait Repeat 8
	.db 0x13			;Note G-2
					;Wait Repeat 8
	.db 0x1f			;Note G-3
					;Wait Repeat 8
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x1f			;Note G-3
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_077:
	.db 0x18			;Note C-3
	.db 0x80			;Instrument 14
	.db 0xc3			;Wait 4
	.db 0x1c			;Note E-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x34			;Note E-5
					;Wait Repeat 4
	.db 0x37			;Note G-5
					;Wait Repeat 4
	.db 0x34			;Note E-5
					;Wait Repeat 4
	.db 0x30			;Note C-5
					;Wait Repeat 4
	.db 0x2b			;Note G-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1c			;Note E-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_078:
	.db 0x34			;Note E-5
	.db 0x6e			;Volume 12
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xcf			;Wait 16
	.db 0x30			;Note C-5
	.db 0xcb			;Wait 12
	.db 0x2b			;Note G-4
	.db 0xdb			;Wait 28
	.db 0x2b			;Note G-4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_079:
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 10
	.db 0x82			;Instrument 16
	.db 0xcf			;Wait 16
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x28			;Note E-4
	.db 0xdb			;Wait 28
	.db 0x28			;Note E-4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_080:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 11
	.db 0x76			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_081:
	.db 0xc3			;Wait 4
	.db 0x2d			;Note A-4
	.db 0x6c			;Volume 10
	.db 0x85			;Instrument 19
	.db 0x9c			;CMD End 
	.db 0xcf			;Wait 16
	.db 0x2f			;Note B-4
	.db 0xcb			;Wait 12
	.db 0x30			;Note C-5
	.db 0xd3			;Wait 20
	.db 0x30			;Note C-5
	.db 0xc7			;Wait 8
	.db 0x32			;Note D-5
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_084:
	.db 0x0e			;Note D-2
	.db 0x70			;Volume 14
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x10			;Note E-2
	.db 0xcb			;Wait 12
	.db 0x11			;Note F-2
	.db 0xd3			;Wait 20
	.db 0x12			;Note F#2
	.db 0xc7			;Wait 8
	.db 0x12			;Note F#2
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_085:
	.db 0x1a			;Note D-3
	.db 0x68			;Volume 6
	.db 0x80			;Instrument 14
	.db 0xaa,0x10			; SCC Set Waveform 6
	.db 0xc1			;Wait 2
	.db 0x1c			;Note E-3
					;Wait Repeat 2
	.db 0x1e			;Note F#3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x1e			;Note F#3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x23			;Note B-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x23			;Note B-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x28			;Note E-4
					;Wait Repeat 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x28			;Note E-4
					;Wait Repeat 2
	.db 0x2a			;Note F#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x34			;Note E-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x34			;Note E-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
					;Wait Repeat 2
	.db 0x37			;Note G-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_086:
	.db 0x2d			;Note A-4
	.db 0x6d			;Volume 11
	.db 0x7a			;Instrument 8
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xcf			;Wait 16
	.db 0x2f			;Note B-4
	.db 0x81			;Instrument 15
	.db 0xcb			;Wait 12
	.db 0x30			;Note C-5
	.db 0xc7			;Wait 8
	.db 0x95,0x44			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc7			;Wait 8
	.db 0x32			;Note D-5
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_087:
	.db 0x29			;Note F-4
	.db 0x6d			;Volume 11
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x2b			;Note G-4
	.db 0xcb			;Wait 12
	.db 0x2d			;Note A-4
	.db 0xc7			;Wait 8
	.db 0x95,0x44			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc7			;Wait 8
	.db 0x2d			;Note A-4
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_088:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_089:
	.db 0x32			;Note D-5
	.db 0x6c			;Volume 10
	.db 0x85			;Instrument 19
	.db 0xc3			;Wait 4
	.db 0x34			;Note E-5
	.db 0xc7			;Wait 8
	.db 0x95,0x26			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x35			;Note F-5
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x34			;Note E-5
					;Wait Repeat 4
	.db 0x32			;Note D-5
	.db 0xcb			;Wait 12
	.db 0x95,0x26			;CMD Vibrato
	.db 0xd3			;Wait 20
	.db 0xbf			;[End-Of-Track]
nem3airb_track_092:
	.db 0x07			;Note G-1
	.db 0x6f			;Volume 13
	.db 0x7b			;Instrument 9
	.db 0xc3			;Wait 4
	.db 0x07			;Note G-1
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x07			;Note G-1
	.db 0xc7			;Wait 8
	.db 0x07			;Note G-1
					;Wait Repeat 8
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x0e			;Note D-2
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x13			;Note G-2
					;Wait Repeat 4
	.db 0x0e			;Note D-2
					;Wait Repeat 4
	.db 0x0d			;Note C#2
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_093:
	.db 0x2b			;Note G-4
	.db 0x6c			;Volume 10
	.db 0x7a			;Instrument 8
	.db 0xaa,0x30			; SCC Set Waveform 17
	.db 0xc7			;Wait 8
	.db 0x95,0x46			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0x7b			;Instrument 9
	.db 0xaa,0x30			; SCC Set Waveform 17
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
	.db 0x7a			;Instrument 8
	.db 0xaa,0x30			; SCC Set Waveform 17
	.db 0xcb			;Wait 12
	.db 0x95,0x46			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_094:
	.db 0x34			;Note E-5
	.db 0x6c			;Volume 10
	.db 0x7a			;Instrument 8
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc7			;Wait 8
	.db 0x95,0x46			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x7b			;Instrument 9
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc3			;Wait 4
	.db 0x34			;Note E-5
	.db 0xc3			;Wait 4
	.db 0x32			;Note D-5
	.db 0x7a			;Instrument 8
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xcb			;Wait 12
	.db 0x95,0x46			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_095:
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 10
	.db 0x7a			;Instrument 8
	.db 0xc7			;Wait 8
	.db 0x95,0x46			;CMD Vibrato
	.db 0xcb			;Wait 12
	.db 0x32			;Note D-5
	.db 0x7b			;Instrument 9
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0xc3			;Wait 4
	.db 0x2f			;Note B-4
	.db 0x7a			;Instrument 8
	.db 0xcb			;Wait 12
	.db 0x95,0x46			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_096:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_097:
	.db 0x0e			;Note D-2
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_100:
	.db 0x0c			;Note C-2
	.db 0x6f			;Volume 13
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x0c			;Note C-2
	.db 0xcf			;Wait 16
	.db 0x0c			;Note C-2
					;Wait Repeat 16
	.db 0x0c			;Note C-2
	.db 0xc7			;Wait 8
	.db 0x0c			;Note C-2
	.db 0xc3			;Wait 4
	.db 0x0e			;Note D-2
					;Wait Repeat 4
	.db 0x10			;Note E-2
					;Wait Repeat 4
	.db 0x0c			;Note C-2
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_101:
	.db 0x13			;Note G-2
	.db 0x68			;Volume 6
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_102:
	.db 0x30			;Note C-5
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2e			;Note A#4
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_103:
	.db 0x2b			;Note G-4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0x2b			;Note G-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_104:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_105:
	.db 0x3e			;Note D-6
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_109:
	.db 0x11			;Note F-2
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x37			;Note G-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_110:
	.db 0x2d			;Note A-4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_111:
	.db 0x29			;Note F-4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_113:
	.db 0x37			;Note G-5
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_117:
	.db 0x13			;Note G-2
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_126:
	.db 0x2d			;Note A-4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2d			;Note A-4
					;Wait Repeat 4
	.db 0x2b			;Note G-4
	.db 0xc2			;Wait 3
	.db 0x95,0x45			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_127:
	.db 0x29			;Note F-4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
					;Wait Repeat 4
	.db 0x28			;Note E-4
	.db 0xc2			;Wait 3
	.db 0x95,0x35			;CMD Vibrato
	.db 0xe0			;Wait 33
	.db 0xbf			;[End-Of-Track]
nem3airb_track_129:
	.db 0x37			;Note G-5
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3f			;Note D#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3f			;Note D#6
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x19			;Note C#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x25			;Note C#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x31			;Note C#5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x38			;Note G#5
					;Wait Repeat 2
	.db 0x3d			;Note C#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x44			;Note G#6
					;Wait Repeat 2
	.db 0x49			;Note C#7
					;Wait Repeat 2
	.db 0x44			;Note G#6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_132:
	.db 0x0f			;Note D#2
	.db 0x6f			;Volume 13
	.db 0x7b			;Instrument 9
	.db 0xc7			;Wait 8
	.db 0x0f			;Note D#2
	.db 0xcf			;Wait 16
	.db 0x0f			;Note D#2
					;Wait Repeat 16
	.db 0x0f			;Note D#2
					;Wait Repeat 16
	.db 0x10			;Note E-2
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3airb_track_133:
	.db 0x16			;Note A#2
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x3f			;Note D#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x46			;Note A#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3f			;Note D#6
					;Wait Repeat 2
	.db 0x3a			;Note A#5
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x19			;Note C#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x25			;Note C#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x31			;Note C#5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x38			;Note G#5
					;Wait Repeat 2
	.db 0x3d			;Note C#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x44			;Note G#6
					;Wait Repeat 2
	.db 0x49			;Note C#7
					;Wait Repeat 2
	.db 0x44			;Note G#6
					;Wait Repeat 2
	.db 0x41			;Note F-6
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_134:
	.db 0x33			;Note D#5
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x45			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0x33			;Note D#5
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x31			;Note C#5
	.db 0xc2			;Wait 3
	.db 0x95,0x45			;CMD Vibrato
	.db 0xd8			;Wait 25
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_135:
	.db 0x2e			;Note A#4
	.db 0x6d			;Volume 11
	.db 0x83			;Instrument 17
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x45			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x2c			;Note G#4
	.db 0xc2			;Wait 3
	.db 0x95,0x45			;CMD Vibrato
	.db 0xd8			;Wait 25
	.db 0x2c			;Note G#4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3airb_track_136:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_137:
	.db 0x41			;Note F-6
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x45			;Note A-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x15			;Note A-2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_140:
	.db 0x11			;Note F-2
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xcb			;Wait 12
	.db 0x13			;Note G-2
	.db 0xe3			;Wait 36
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x18			;Note C-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_141:
	.db 0x0f			;Note D#2
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x45			;Note A-6
					;Wait Repeat 2
	.db 0x41			;Note F-6
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x11			;Note F-2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_142:
	.db 0x2e			;Note A#4
	.db 0x83			;Instrument 17
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xf0			;Wait 49
	.db 0xbf			;[End-Of-Track]
nem3airb_track_143:
	.db 0x2b			;Note G-4
	.db 0x83			;Instrument 17
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xf0			;Wait 49
	.db 0xbf			;[End-Of-Track]
nem3airb_track_144:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 12
					;Wait Repeat 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_145:
	.db 0x11			;Note F-2
	.db 0x7c			;Instrument 10
	.db 0xc1			;Wait 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x23			;Note B-3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x23			;Note B-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
					;Wait Repeat 2
	.db 0x17			;Note B-2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3airb_track_148:
	.db 0x13			;Note G-2
	.db 0x7b			;Instrument 9
	.db 0xcb			;Wait 12
	.db 0x15			;Note A-2
	.db 0xf3			;Wait 52
	.db 0xbf			;[End-Of-Track]
nem3airb_track_149:
	.db 0x11			;Note F-2
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x23			;Note B-3
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x47			;Note B-6
					;Wait Repeat 2
	.db 0x43			;Note G-6
					;Wait Repeat 2
	.db 0x3e			;Note D-6
					;Wait Repeat 2
	.db 0x3b			;Note B-5
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x71			;Volume 15
	.db 0x75			;Instrument 3
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
					;Wait Repeat 4
	.db 0x24			;Note C-4
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3airb_track_150:
	.db 0x30			;Note C-5
	.db 0x83			;Instrument 17
	.db 0xaa,0x50			; SCC Set Waveform 25
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x32			;Note D-5
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x35			;Note F-5
	.db 0x6a			;Volume 8
	.db 0x84			;Instrument 18
	.db 0xc0			;Wait 1
	.db 0x3e			;Note D-6
	.db 0x91,0x05			;CMD Portamento tone
	.db 0xc9			;Wait 10
	.db 0x95,0x35			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0xbf			;[End-Of-Track]
nem3airb_track_151:
	.db 0x2d			;Note A-4
	.db 0x83			;Instrument 17
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x25			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x32			;Note D-5
	.db 0x6a			;Volume 8
	.db 0x80			;Instrument 14
	.db 0xc0			;Wait 1
	.db 0x3b			;Note B-5
	.db 0x84			;Instrument 18
	.db 0x91,0x05			;CMD Portamento tone
	.db 0xc9			;Wait 10
	.db 0x95,0x35			;CMD Vibrato
	.db 0xd4			;Wait 21
	.db 0xbf			;[End-Of-Track]

