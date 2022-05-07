; Song: NEM3 -Stage 1                   
; By:   Konami (Arr by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	.db 0x07					; Initial song speed
	.dw nem3st1_waveform_start			; Start of the waveform data
	.dw nem3st1_instrument_start			; Start of the instrument data

; [ Song order pointer list ]
nem3st1_restart:
	.dw nem3st1_track_000, nem3st1_track_001, nem3st1_track_002, nem3st1_track_003, nem3st1_track_004, nem3st1_track_005, nem3st1_track_006, nem3st1_track_007	; Step:000 Pattern:000
	.dw nem3st1_track_008, nem3st1_track_009, nem3st1_track_010, nem3st1_track_010, nem3st1_track_012, nem3st1_track_013, nem3st1_track_014, nem3st1_track_015	; Step:001 Pattern:001
	.dw nem3st1_track_000, nem3st1_track_017, nem3st1_track_010, nem3st1_track_010, nem3st1_track_004, nem3st1_track_005, nem3st1_track_006, nem3st1_track_007	; Step:002 Pattern:002
	.dw nem3st1_track_024, nem3st1_track_009, nem3st1_track_010, nem3st1_track_010, nem3st1_track_012, nem3st1_track_013, nem3st1_track_014, nem3st1_track_015	; Step:003 Pattern:003
	.dw nem3st1_track_032, nem3st1_track_033, nem3st1_track_010, nem3st1_track_010, nem3st1_track_036, nem3st1_track_037, nem3st1_track_038, nem3st1_track_039	; Step:004 Pattern:004
	.dw nem3st1_track_032, nem3st1_track_041, nem3st1_track_010, nem3st1_track_010, nem3st1_track_044, nem3st1_track_045, nem3st1_track_046, nem3st1_track_047	; Step:005 Pattern:005
	.dw nem3st1_track_032, nem3st1_track_049, nem3st1_track_010, nem3st1_track_010, nem3st1_track_052, nem3st1_track_053, nem3st1_track_054, nem3st1_track_055	; Step:006 Pattern:006
	.dw nem3st1_track_056, nem3st1_track_057, nem3st1_track_010, nem3st1_track_010, nem3st1_track_060, nem3st1_track_061, nem3st1_track_062, nem3st1_track_063	; Step:007 Pattern:007
	.dw nem3st1_track_064, nem3st1_track_065, nem3st1_track_010, nem3st1_track_010, nem3st1_track_068, nem3st1_track_069, nem3st1_track_070, nem3st1_track_071	; Step:008 Pattern:008
	.dw nem3st1_track_072, nem3st1_track_073, nem3st1_track_010, nem3st1_track_010, nem3st1_track_076, nem3st1_track_077, nem3st1_track_078, nem3st1_track_079	; Step:009 Pattern:009
	.dw nem3st1_track_064, nem3st1_track_081, nem3st1_track_010, nem3st1_track_010, nem3st1_track_068, nem3st1_track_069, nem3st1_track_086, nem3st1_track_087	; Step:010 Pattern:010
	.dw nem3st1_track_088, nem3st1_track_089, nem3st1_track_010, nem3st1_track_010, nem3st1_track_092, nem3st1_track_093, nem3st1_track_094, nem3st1_track_095	; Step:011 Pattern:011
	.dw 0x0000, nem3st1_restart				; End of sequence delimiter + nem3st1_restart address

; [ Custom FM voices ]
nem3st1_customvoice_start:

; [ SCC Waveforms ]
nem3st1_waveform_start:
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0x00, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x81, 0xc0, 0x01, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x01, 0x40, 0x01, 0xe0, 0x01, 0x20, 0x01, 0xf0, 0x01, 0x10, 0x01, 0xff, 0xff, 0xff, 0xff, 0x40, 0x40, 0x40				; Waveform:2
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70				; Waveform:8
	.db 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x80, 0x80				; Waveform:15
	.db 0x00, 0x7f, 0x00, 0x80, 0xa0, 0xc0, 0xd8, 0xf0, 0x08, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x78, 0x7c, 0x7f, 0x7c, 0x78, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x08, 0xf0, 0xd8, 0xc0, 0xa0, 0x80				; Waveform:17
	.db 0x00, 0x68, 0x40, 0x04, 0x00, 0x26, 0x40, 0xf5, 0x10, 0x40, 0x20, 0xd0, 0xf0, 0x00, 0xc0, 0x80, 0xd0, 0x30, 0xc0, 0xa0, 0xb0, 0xc0, 0x80, 0xd0, 0xf0, 0x40, 0x10, 0xc0, 0xd4, 0x00, 0xc0, 0xe0				; Waveform:19
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60				; Waveform:25
	.db 0x90, 0x88, 0x90, 0x9c, 0xb0, 0xc0, 0xe0, 0x26, 0x70, 0x7c, 0x70, 0x56, 0x40, 0x20, 0x10, 0xfc, 0xe0, 0xfc, 0x10, 0xfc, 0x40, 0x54, 0x72, 0x7f, 0x70, 0x26, 0xe0, 0xc0, 0xb0, 0x9c, 0x90, 0x88				; Waveform:30

; [ FM Drum macros]
nem3st1_drummacro_start:

; [ Instruments]
nem3st1_instrument_start:
	.dw nem3st1_instrument_00				; BD G2           
	.dw nem3st1_instrument_01				; SD A3           
	.dw nem3st1_instrument_02				; HH CL A8        
	.dw nem3st1_instrument_03				; Short Snare     
	.dw nem3st1_instrument_04				; SCC Rhythm      
	.dw nem3st1_instrument_05				; SC Bass         
	.dw nem3st1_instrument_06				; Remate ARP      
	.dw nem3st1_instrument_07				; ARP SQ          
	.dw nem3st1_instrument_08				; ARP SCC         
	.dw nem3st1_instrument_09				; ARP END SCC     
	.dw nem3st1_instrument_10				; THEME SCC       
	.dw nem3st1_instrument_11				; THEME STACC     
	.dw nem3st1_instrument_12				; ARP SEMI FINAL  
	.dw nem3st1_instrument_13				; ARP semi ch5    
	.dw nem3st1_instrument_14				; CONT            

nem3st1_instrument_00:					; BD G2           
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x1c,0x03						; Envelope shape
	.db 0x02,0x10						; Mixer (T)
	.db 0x1a						; Envelope
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3st1_instrument_01:					; SD A3           
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

nem3st1_instrument_02:					; HH CL A8        
	.db 0x10						; Waveform 2
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3st1_instrument_03:					; Short Snare     
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

nem3st1_instrument_04:					; SCC Rhythm      
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
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_05:					; SC Bass         
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

nem3st1_instrument_06:					; Remate ARP      
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_07:					; ARP SQ          
	.db 0x30						; Waveform 6
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_08:					; ARP SCC         
	.db 0x28						; Waveform 5
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_09:					; ARP END SCC     
	.db 0x18						; Waveform 3
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_10:					; THEME SCC       
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_11:					; THEME STACC     
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

nem3st1_instrument_12:					; ARP SEMI FINAL  
	.db 0x38						; Waveform 7
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_13:					; ARP semi ch5    
	.db 0x38						; Waveform 7
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3st1_instrument_14:					; CONT            
	.db 0x40						; Waveform 8
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x18,0xfb						; Loop (-6)


; [ Song track data ]
nem3st1_track_000:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6c			;Volume 11
	.db 0x74			;Instrument 3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6c			;Volume 11
	.db 0x74			;Instrument 3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_001:
	.db 0x24			;Note C-4
	.db 0x6c			;Volume 11
	.db 0x7f			;Instrument 14
	.db 0xa5,0x50			;CMD Envelope multiplier low
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0xa1,0x01			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0x94,0x17			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x24			;Note C-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x94,0x17			;CMD Vibrato
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
nem3st1_track_002:
	.db 0x70			;Volume 15
	.db 0x71			;Instrument 0
	.db 0xa4,0x02			;CMD Envelope multiplier high
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
nem3st1_track_003:
	.db 0x70			;Volume 15
	.db 0x71			;Instrument 0
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
nem3st1_track_004:
	.db 0x0c			;Note C-2
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
	.db 0xc5			;Wait 6
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
	.db 0xc5			;Wait 6
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_005:
	.db 0x24			;Note C-4
	.db 0x69			;Volume 8
	.db 0x75			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_006:
	.db 0x24			;Note C-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0x94,0x26			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x24			;Note C-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x94,0x36			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
nem3st1_track_007:
	.db 0x1f			;Note G-3
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x94,0x26			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x1f			;Note G-3
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x94,0x36			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
nem3st1_track_008:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6c			;Volume 11
	.db 0x74			;Instrument 3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0x72			;Instrument 1
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0xbf			;[End-Of-Track]
nem3st1_track_009:
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x6c			;Volume 11
	.db 0x7f			;Instrument 14
	.db 0x9b			;CMD End 
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x94,0x17			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x18			;Note C-3
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_010:
	.db 0xef			;Wait 48
	.db 0xbf			;[End-Of-Track]
nem3st1_track_012:
	.db 0x0c			;Note C-2
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
	.db 0xc5			;Wait 6
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_013:
	.db 0x24			;Note C-4
	.db 0x69			;Volume 8
	.db 0x75			;Instrument 4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x6a			;Volume 9
	.db 0x77			;Instrument 6
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_014:
	.db 0x24			;Note C-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x94,0x46			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x18			;Note C-3
	.db 0x6a			;Volume 9
	.db 0x75			;Instrument 4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_015:
	.db 0x1f			;Note G-3
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x94,0x36			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x13			;Note G-2
	.db 0x6a			;Volume 9
	.db 0x75			;Instrument 4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_017:
	.db 0x2e			;Note A#4
	.db 0x6c			;Volume 11
	.db 0x7f			;Instrument 14
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0x94,0x17			;CMD Vibrato
	.db 0xd1			;Wait 18
	.db 0x24			;Note C-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x94,0x17			;CMD Vibrato
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
nem3st1_track_024:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6c			;Volume 11
	.db 0x74			;Instrument 3
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0x72			;Instrument 1
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_032:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_033:
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x6c			;Volume 11
	.db 0x7f			;Instrument 14
	.db 0xc5			;Wait 6
	.db 0x94,0x37			;CMD Vibrato
	.db 0xd7			;Wait 24
	.db 0x24			;Note C-4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x29			;Note F-4
					;Wait Repeat 6
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_036:
	.db 0x09			;Note A-1
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
					;Wait Repeat 2
	.db 0x09			;Note A-1
					;Wait Repeat 2
	.db 0x15			;Note A-2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_037:
	.db 0x18			;Note C-3
	.db 0x69			;Volume 8
	.db 0x75			;Instrument 4
	.db 0xc1			;Wait 2
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
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
	.db 0x68			;Volume 7
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x69			;Volume 8
					;Wait Repeat 2
	.db 0x2d			;Note A-4
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
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_038:
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x94,0x46			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x66			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x24			;Note C-4
	.db 0x6c			;Volume 11
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x66			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_039:
	.db 0x24			;Note C-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x94,0x46			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x66			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6c			;Volume 11
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x24			;Note C-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_041:
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0x6d			;Volume 12
	.db 0x7f			;Instrument 14
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x2e			;Note A#4
	.db 0x9b			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x2d			;Note A-4
	.db 0x9b			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x29			;Note F-4
	.db 0x9b			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x94,0x37			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_044:
	.db 0x0a			;Note A#1
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
					;Wait Repeat 2
	.db 0x0a			;Note A#1
					;Wait Repeat 2
	.db 0x16			;Note A#2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_045:
	.db 0x1a			;Note D-3
	.db 0x69			;Volume 8
	.db 0x75			;Instrument 4
	.db 0xc1			;Wait 2
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
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
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
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x1d			;Note F-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_046:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2e			;Note A#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
nem3st1_track_047:
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x29			;Note F-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x26			;Note D-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
nem3st1_track_049:
	.db 0xc3			;Wait 4
	.db 0x2c			;Note G#4
	.db 0x6d			;Volume 12
	.db 0x7f			;Instrument 14
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2c			;Note G#4
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x27			;Note D#4
					;Wait Repeat 6
	.db 0x2c			;Note G#4
					;Wait Repeat 6
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_052:
	.db 0x08			;Note G#1
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x08			;Note G#1
					;Wait Repeat 2
	.db 0x14			;Note G#2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_053:
	.db 0x1b			;Note D#3
	.db 0x75			;Instrument 4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x20			;Note G#3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_054:
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x7c			;Instrument 11
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x27			;Note D#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_055:
	.db 0x27			;Note D#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x27			;Note D#4
	.db 0x6b			;Volume 10
	.db 0x7c			;Instrument 11
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x24			;Note C-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x27			;Note D#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0xc4			;Wait 5
	.db 0x64			;Volume 3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_056:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_057:
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xcc			;Wait 13
	.db 0x30			;Note C-5
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
	.db 0xc4			;Wait 5
	.db 0x94,0x27			;CMD Vibrato
	.db 0xce			;Wait 15
	.db 0xbf			;[End-Of-Track]
nem3st1_track_060:
	.db 0x07			;Note G-1
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x07			;Note G-1
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0e			;Note D-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x18			;Note C-3
					;Wait Repeat 2
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_061:
	.db 0x1a			;Note D-3
	.db 0x75			;Instrument 4
	.db 0xc1			;Wait 2
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
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x1a			;Note D-3
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
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_062:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7c			;Instrument 11
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0x2f			;Note B-4
	.db 0x7b			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x64			;Volume 3
	.db 0x9b			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3st1_track_063:
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 10
	.db 0x7b			;Instrument 10
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x64			;Volume 3
	.db 0xc0			;Wait 1
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 10
	.db 0x7c			;Instrument 11
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x7b			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x94,0x36			;CMD Vibrato
	.db 0xd0			;Wait 17
	.db 0x64			;Volume 3
	.db 0x9b			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3st1_track_064:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_065:
	.db 0x60			;Release 96
	.db 0xc0			;Wait 1
	.db 0x9b			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x30			;Note C-5
	.db 0x6e			;Volume 13
	.db 0x78			;Instrument 7
	.db 0xa1,0x03			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_068:
	.db 0x0c			;Note C-2
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
	.db 0xc3			;Wait 4
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
	.db 0xc3			;Wait 4
	.db 0x0f			;Note D#2
	.db 0xc1			;Wait 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
	.db 0xc3			;Wait 4
	.db 0x0f			;Note D#2
	.db 0xc1			;Wait 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_069:
	.db 0x24			;Note C-4
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xa9,0x08			; SCC Set Waveform 2
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc3			;Wait 4
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x22			;Note A#3
	.db 0xc3			;Wait 4
	.db 0x22			;Note A#3
	.db 0xc1			;Wait 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
	.db 0xc3			;Wait 4
	.db 0x22			;Note A#3
	.db 0xc1			;Wait 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
					;Wait Repeat 2
	.db 0x22			;Note A#3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_070:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7d			;Instrument 12
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_071:
	.db 0x2b			;Note G-4
	.db 0x69			;Volume 8
	.db 0x7e			;Instrument 13
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_072:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_073:
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 13
	.db 0x78			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x6f			;Volume 14
	.db 0xa1,0x03			;CMD Track detune
	.db 0xc4			;Wait 5
	.db 0x94,0x35			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x2c			;Note G#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x30			;Note C-5
					;Wait Repeat 6
	.db 0x2f			;Note B-4
	.db 0xc3			;Wait 4
	.db 0x94,0x35			;CMD Vibrato
					;Wait Repeat 4
	.db 0x2f			;Note B-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc3			;Wait 4
	.db 0x94,0x35			;CMD Vibrato
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3st1_track_076:
	.db 0x14			;Note G#2
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x14			;Note G#2
	.db 0xc1			;Wait 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
	.db 0xc3			;Wait 4
	.db 0x14			;Note G#2
	.db 0xc1			;Wait 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x0e			;Note D-2
					;Wait Repeat 2
	.db 0x0c			;Note C-2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_077:
	.db 0x27			;Note D#4
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xa9,0x08			; SCC Set Waveform 2
	.db 0xc3			;Wait 4
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
	.db 0xc3			;Wait 4
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_078:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x79			;Instrument 8
	.db 0x9b			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc7			;Wait 8
	.db 0x2c			;Note G#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x30			;Note C-5
					;Wait Repeat 6
	.db 0x2f			;Note B-4
	.db 0xc2			;Wait 3
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x2f			;Note B-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
	.db 0x6c			;Volume 11
	.db 0xc2			;Wait 3
	.db 0x94,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
nem3st1_track_079:
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x79			;Instrument 8
	.db 0x9b			;CMD End 
	.db 0xc3			;Wait 4
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc7			;Wait 8
	.db 0x27			;Note D#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2c			;Note G#4
					;Wait Repeat 6
	.db 0x2b			;Note G-4
	.db 0xc2			;Wait 3
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x2b			;Note G-4
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0xc2			;Wait 3
	.db 0x94,0x25			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
nem3st1_track_081:
	.db 0xa5,0x08			;CMD Envelope multiplier low
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0x6e			;Volume 13
	.db 0x78			;Instrument 7
	.db 0xa1,0x03			;CMD Track detune
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_086:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x7d			;Instrument 12
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_087:
	.db 0x2b			;Note G-4
	.db 0x69			;Volume 8
	.db 0x7e			;Instrument 13
	.db 0x9b			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x24			;Note C-4
					;Wait Repeat 2
	.db 0x26			;Note D-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_088:
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6d			;Volume 12
	.db 0x73			;Instrument 2
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0x71			;Instrument 0
					;Wait Repeat 2
	.db 0x29			;Note F-4
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0x72			;Instrument 1
	.db 0xc0			;Wait 1
	.db 0x21			;Note A-3
	.db 0x6e			;Volume 13
					;Wait Repeat 1
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 14
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_089:
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 13
	.db 0x78			;Instrument 7
	.db 0xa5,0x08			;CMD Envelope multiplier low
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
	.db 0x6f			;Volume 14
	.db 0xa1,0x03			;CMD Track detune
	.db 0xc4			;Wait 5
	.db 0x94,0x35			;CMD Vibrato
	.db 0xc6			;Wait 7
	.db 0x2c			;Note G#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 11
	.db 0xc1			;Wait 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0xc1			;Wait 2
	.db 0x3e			;Note D-6
	.db 0xa1,0x00			;CMD Track detune
					;Wait Repeat 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
	.db 0xc1			;Wait 2
	.db 0x3a			;Note A#5
	.db 0x6c			;Volume 11
	.db 0x78			;Instrument 7
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x6b			;Volume 10
					;Wait Repeat 1
	.db 0x2e			;Note A#4
	.db 0x6a			;Volume 9
					;Wait Repeat 1
	.db 0x27			;Note D#4
					;Wait Repeat 1
	.db 0x22			;Note A#3
					;Wait Repeat 1
	.db 0x1b			;Note D#3
					;Wait Repeat 1
	.db 0x38			;Note G#5
					;Wait Repeat 1
	.db 0x31			;Note C#5
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
	.db 0x94,0x35			;CMD Vibrato
					;Wait Repeat 1
	.db 0x25			;Note C#4
					;Wait Repeat 1
	.db 0x20			;Note G#3
					;Wait Repeat 1
	.db 0x19			;Note C#3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_092:
	.db 0x14			;Note G#2
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x14			;Note G#2
	.db 0xc1			;Wait 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
	.db 0xc3			;Wait 4
	.db 0x14			;Note G#2
	.db 0xc1			;Wait 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x14			;Note G#2
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x1f			;Note G-3
					;Wait Repeat 2
	.db 0x13			;Note G-2
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x1d			;Note F-3
					;Wait Repeat 2
	.db 0x11			;Note F-2
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x1b			;Note D#3
					;Wait Repeat 2
	.db 0x0f			;Note D#2
					;Wait Repeat 2
	.db 0x0d			;Note C#2
					;Wait Repeat 2
	.db 0x19			;Note C#3
					;Wait Repeat 2
	.db 0x0d			;Note C#2
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_093:
	.db 0x27			;Note D#4
	.db 0x6d			;Volume 12
	.db 0x76			;Instrument 5
	.db 0xa9,0x08			; SCC Set Waveform 2
	.db 0xc3			;Wait 4
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
	.db 0xc3			;Wait 4
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x6c			;Volume 11
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x2b			;Note G-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x29			;Note F-4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x27			;Note D#4
					;Wait Repeat 2
	.db 0x25			;Note C#4
					;Wait Repeat 2
	.db 0x25			;Note C#4
					;Wait Repeat 2
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3st1_track_094:
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 10
	.db 0x79			;Instrument 8
	.db 0xc3			;Wait 4
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc7			;Wait 8
	.db 0x2c			;Note G#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x30			;Note C-5
	.db 0xc5			;Wait 6
	.db 0x3e			;Note D-6
	.db 0x69			;Volume 8
	.db 0x7a			;Instrument 9
	.db 0xc1			;Wait 2
	.db 0x37			;Note G-5
					;Wait Repeat 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x3c			;Note C-6
					;Wait Repeat 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x3a			;Note A#5
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
					;Wait Repeat 1
	.db 0x2e			;Note A#4
					;Wait Repeat 1
	.db 0x27			;Note D#4
					;Wait Repeat 1
	.db 0x22			;Note A#3
					;Wait Repeat 1
	.db 0x1b			;Note D#3
					;Wait Repeat 1
	.db 0x38			;Note G#5
					;Wait Repeat 1
	.db 0x31			;Note C#5
					;Wait Repeat 1
	.db 0x2c			;Note G#4
					;Wait Repeat 1
	.db 0x25			;Note C#4
					;Wait Repeat 1
	.db 0x20			;Note G#3
					;Wait Repeat 1
	.db 0x19			;Note C#3
	.db 0xbf			;[End-Of-Track]
nem3st1_track_095:
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 10
	.db 0x79			;Instrument 8
	.db 0xc3			;Wait 4
	.db 0x94,0x45			;CMD Vibrato
	.db 0xc7			;Wait 8
	.db 0x27			;Note D#4
	.db 0x9b			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2c			;Note G#4
	.db 0xc5			;Wait 6
	.db 0x3b			;Note B-5
	.db 0x69			;Volume 8
	.db 0x7a			;Instrument 9
	.db 0xc1			;Wait 2
	.db 0x32			;Note D-5
					;Wait Repeat 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x39			;Note A-5
					;Wait Repeat 2
	.db 0x30			;Note C-5
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x37			;Note G-5
	.db 0xc0			;Wait 1
	.db 0x2e			;Note A#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x27			;Note D#4
					;Wait Repeat 1
	.db 0x22			;Note A#3
					;Wait Repeat 1
	.db 0x1f			;Note G-3
					;Wait Repeat 1
	.db 0x35			;Note F-5
					;Wait Repeat 1
	.db 0x2c			;Note G#4
					;Wait Repeat 1
	.db 0x29			;Note F-4
					;Wait Repeat 1
	.db 0x25			;Note C#4
					;Wait Repeat 1
	.db 0x20			;Note G#3
					;Wait Repeat 1
	.db 0x1d			;Note F-3
	.db 0xbf			;[End-Of-Track]

