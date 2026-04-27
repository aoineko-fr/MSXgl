; Song: NEM3- Weapon Select             
; By:   Konami (Arr by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	.db 0x07					; Initial song speed
	.dw nem3sel_waveform_start			; Start of the waveform data
	.dw nem3sel_instrument_start			; Start of the instrument data

; [ Song order pointer list ]
nem3sel_restart:
	.dw nem3sel_track_000, nem3sel_track_001, nem3sel_track_002, nem3sel_track_003, nem3sel_track_004, nem3sel_track_005, nem3sel_track_006, nem3sel_track_007	; Step:000 Pattern:000
	.dw nem3sel_track_008, nem3sel_track_009, nem3sel_track_010, nem3sel_track_011, nem3sel_track_012, nem3sel_track_013, nem3sel_track_014, nem3sel_track_015	; Step:001 Pattern:001
	.dw nem3sel_track_000, nem3sel_track_017, nem3sel_track_018, nem3sel_track_019, nem3sel_track_020, nem3sel_track_021, nem3sel_track_022, nem3sel_track_023	; Step:002 Pattern:002
	.dw nem3sel_track_024, nem3sel_track_025, nem3sel_track_026, nem3sel_track_027, nem3sel_track_028, nem3sel_track_029, nem3sel_track_030, nem3sel_track_031	; Step:003 Pattern:003
	.dw 0x0000, nem3sel_restart				; End of sequence delimiter + nem3sel_restart address


; [ SCC Waveforms ]
nem3sel_waveform_start:
	.db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00				; Waveform:0
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x00, 0x7f, 0x00, 0x80, 0xa0, 0xc0, 0xd8, 0xf0, 0x08, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x78, 0x7c, 0x7f, 0x7c, 0x78, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x08, 0xf0, 0xd8, 0xc0, 0xa0, 0x80				; Waveform:17
	.db 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20, 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20				; Waveform:22
	.db 0x90, 0x88, 0x90, 0x9c, 0xb0, 0xc0, 0xe0, 0x26, 0x70, 0x7c, 0x70, 0x56, 0x40, 0x20, 0x10, 0xfc, 0xe0, 0xfc, 0x10, 0xfc, 0x40, 0x54, 0x72, 0x7f, 0x70, 0x26, 0xe0, 0xc0, 0xb0, 0x9c, 0x90, 0x88				; Waveform:30
	.db 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x70, 0x70, 0x70, 0x80, 0x80, 0x80, 0x70, 0x70, 0x70, 0x70, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80				; Waveform:31

; [ Instruments]
nem3sel_instrument_start:
	.dw nem3sel_instrument_00				; BD C2           
	.dw nem3sel_instrument_01				; SD A3           
	.dw nem3sel_instrument_02				; HH CL A8        
	.dw nem3sel_instrument_03				; TOM             
	.dw nem3sel_instrument_04				; HH roll         
	.dw nem3sel_instrument_05				; Short Snare     
	.dw nem3sel_instrument_06				; SC Bass         
	.dw nem3sel_instrument_07				; SD-MAIN         
	.dw nem3sel_instrument_08				; Melody          
	.dw nem3sel_instrument_09				; CONT            
	.dw nem3sel_instrument_10				; ARP             
	.dw nem3sel_instrument_11				; ARP 2           
	.dw nem3sel_instrument_12				; PSG ARP         

nem3sel_instrument_00:					; BD C2           
	.db 0x18						; Waveform 3
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xc0,0xff					; Tone sub
	.db 0x1c,0x01						; Envelope shape
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0xc0,0xff					; Tone sub
	.db 0x1a						; Envelope
	.db 0x18,0xf9						; Loop (-8)

nem3sel_instrument_01:					; SD A3           
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x01						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0a						; Volume _
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

nem3sel_instrument_02:					; HH CL A8        
	.db 0x10						; Waveform 2
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3sel_instrument_03:					; TOM             
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x04,0x40,0x00					; Tone add
	.db 0x08,0x0e						; Volume _
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

nem3sel_instrument_04:					; HH roll         
	.db 0x10						; Waveform 2
								; --- Macro loop
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
	.db 0x08,0x00						; Volume _
	.db 0x18,0xf3						; Loop (-14)

nem3sel_instrument_05:					; Short Snare     
	.db 0x08						; Waveform 1
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

nem3sel_instrument_06:					; SC Bass         
	.db 0x30						; Waveform 6
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xfc						; Loop (-5)

nem3sel_instrument_07:					; SD-MAIN         
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x05						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x06						; Noise _
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x07						; Noise _
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x08						; Noise _
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x09						; Noise _
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0a						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0b						; Noise _
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0a						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0b						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0c						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0b						; Noise _
	.db 0x08,0x09						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0c						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0b						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0c						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0d						; Noise _
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0c						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0d						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0e						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0d						; Noise _
	.db 0x08,0x07						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0e						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0f						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0e						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0f						; Noise _
	.db 0x08,0x06						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x10						; Noise _
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x0f						; Noise _
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

nem3sel_instrument_08:					; Melody          
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
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
	.db 0x08,0x0b						; Volume _
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
	.db 0x08,0x09						; Volume _
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
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3sel_instrument_09:					; CONT            
	.db 0x28						; Waveform 5
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3sel_instrument_10:					; ARP             
	.db 0x18						; Waveform 3
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3sel_instrument_11:					; ARP 2           
	.db 0x18						; Waveform 3
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

nem3sel_instrument_12:					; PSG ARP         
	.db 0x18						; Waveform 3
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
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
	.db 0x08,0x0b						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x18,0xfb						; Loop (-6)


; [ Song track data ]
nem3sel_track_000:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xa6,0x00			;CMD Envelope multiplier low
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x11			;Note F-2
	.db 0x6e			;Volume 12
	.db 0x76			;Instrument 4
					;Wait Repeat 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0xc7			;Wait 8
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_001:
	.db 0x60			;Release 96
	.db 0xa5,0x04			;CMD Envelope multiplier high
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
	.db 0x6d			;Volume 11
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_002:
	.db 0x60			;Release 96
	.db 0xc2			;Wait 3
	.db 0x32			;Note D-5
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc4			;Wait 5
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0xc3			;Wait 4
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd7			;Wait 24
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3sel_track_003:
	.db 0x07			;Note G-1
	.db 0x6e			;Volume 12
	.db 0x78			;Instrument 6
	.db 0xdf			;Wait 32
	.db 0x05			;Note F-1
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
nem3sel_track_004:
	.db 0x32			;Note D-5
	.db 0x6e			;Volume 12
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
	.db 0xc4			;Wait 5
	.db 0x95,0x66			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x30			;Note C-5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0xc5			;Wait 6
	.db 0x95,0x66			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0x2e			;Note A#4
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_005:
	.db 0x32			;Note D-5
	.db 0x6c			;Volume 10
	.db 0x7a			;Instrument 8
	.db 0xa2,0x08			;CMD Track detune
	.db 0xcb			;Wait 12
	.db 0x95,0x68			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0x30			;Note C-5
	.db 0x6e			;Volume 12
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x2f			;Note B-4
					;Wait Repeat 2
	.db 0x30			;Note C-5
	.db 0xc5			;Wait 6
	.db 0x95,0x68			;CMD Vibrato
	.db 0xd3			;Wait 20
	.db 0xbf			;[End-Of-Track]
nem3sel_track_006:
	.db 0x1a			;Note D-3
	.db 0x6d			;Volume 11
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_007:
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
	.db 0x7d			;Instrument 11
	.db 0xaa,0x18			; SCC Set Waveform 17
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_008:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x73			;Instrument 1
	.db 0xc7			;Wait 8
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
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x73			;Instrument 1
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc7			;Wait 8
	.db 0x1f			;Note G-3
	.db 0x71			;Volume 15
	.db 0x79			;Instrument 7
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3sel_track_009:
	.db 0x1f			;Note G-3
	.db 0x6e			;Volume 12
	.db 0x7e			;Instrument 12
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1b			;Note D#3
	.db 0xc3			;Wait 4
	.db 0x1b			;Note D#3
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x1b			;Note D#3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_010:
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2c			;Note G#4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
nem3sel_track_011:
	.db 0x0a			;Note A#1
	.db 0x78			;Instrument 6
	.db 0xdf			;Wait 32
	.db 0x09			;Note A-1
	.db 0xcf			;Wait 16
	.db 0x08			;Note G#1
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
nem3sel_track_012:
	.db 0x2e			;Note A#4
	.db 0x6e			;Volume 12
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x2c			;Note G#4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0xbf			;[End-Of-Track]
nem3sel_track_013:
	.db 0xc1			;Wait 2
	.db 0x2e			;Note A#4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x2e			;Note A#4
	.db 0xc4			;Wait 5
	.db 0x95,0x68			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x29			;Note F-4
	.db 0x9c			;CMD End 
	.db 0xc7			;Wait 8
	.db 0x95,0x68			;CMD Vibrato
					;Wait Repeat 8
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2c			;Note G#4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_014:
	.db 0x1a			;Note D-3
	.db 0x6d			;Volume 11
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0xc3			;Wait 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
					;Wait Repeat 4
	.db 0x1a			;Note D-3
					;Wait Repeat 4
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1a			;Note D-3
	.db 0xc3			;Wait 4
	.db 0x1b			;Note D#3
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x1b			;Note D#3
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_015:
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2b			;Note G-4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2d			;Note A-4
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
	.db 0xc1			;Wait 2
	.db 0x27			;Note D#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x2c			;Note G#4
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x2e			;Note A#4
					;Wait Repeat 2
	.db 0x33			;Note D#5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_017:
	.db 0x60			;Release 96
	.db 0xa5,0x06			;CMD Envelope multiplier high
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
	.db 0x6d			;Volume 11
	.db 0x7e			;Instrument 12
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_018:
	.db 0x60			;Release 96
	.db 0xc2			;Wait 3
	.db 0x38			;Note G#5
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc4			;Wait 5
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x36			;Note F#5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
	.db 0xc3			;Wait 4
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd7			;Wait 24
	.db 0x34			;Note E-5
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3sel_track_019:
	.db 0x0d			;Note C#2
	.db 0x6e			;Volume 12
	.db 0x78			;Instrument 6
	.db 0xdf			;Wait 32
	.db 0x0b			;Note B-1
	.db 0xdf			;Wait 32
	.db 0xbf			;[End-Of-Track]
nem3sel_track_020:
	.db 0x38			;Note G#5
	.db 0x6e			;Volume 12
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x56			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0x36			;Note F#5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
	.db 0xc5			;Wait 6
	.db 0x95,0x66			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0x34			;Note E-5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x33			;Note D#5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_021:
	.db 0xc4			;Wait 5
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0x6d			;Volume 11
	.db 0x7a			;Instrument 8
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc5			;Wait 6
	.db 0x95,0x68			;CMD Vibrato
	.db 0xd5			;Wait 22
	.db 0x36			;Note F#5
	.db 0x9c			;CMD End 
	.db 0xc1			;Wait 2
	.db 0x35			;Note F-5
					;Wait Repeat 2
	.db 0x36			;Note F#5
	.db 0xc5			;Wait 6
	.db 0x95,0x68			;CMD Vibrato
	.db 0xd3			;Wait 20
	.db 0xbf			;[End-Of-Track]
nem3sel_track_022:
	.db 0x20			;Note G#3
	.db 0x6d			;Volume 11
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_023:
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_024:
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x77			;Instrument 5
	.db 0xc5			;Wait 6
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0x71			;Volume 15
	.db 0x73			;Instrument 1
	.db 0xc7			;Wait 8
	.db 0x13			;Note G-2
	.db 0x72			;Instrument 0
	.db 0xc3			;Wait 4
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x6e			;Volume 12
	.db 0x75			;Instrument 3
	.db 0xc0			;Wait 1
	.db 0x2d			;Note A-4
	.db 0xc0			;Wait 1
	.db 0x2b			;Note G-4
	.db 0x6e			;Volume 12
	.db 0xc1			;Wait 2
	.db 0x29			;Note F-4
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x28			;Note E-4
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x24			;Note C-4
	.db 0x71			;Volume 15
					;Wait Repeat 2
	.db 0x1f			;Note G-3
	.db 0xc1			;Wait 2
	.db 0x1f			;Note G-3
	.db 0x6f			;Volume 13
	.db 0xc5			;Wait 6
	.db 0x1f			;Note G-3
	.db 0x70			;Volume 14
	.db 0x79			;Instrument 7
	.db 0xc7			;Wait 8
	.db 0xbf			;[End-Of-Track]
nem3sel_track_025:
	.db 0x25			;Note C#4
	.db 0x6e			;Volume 12
	.db 0x7e			;Instrument 12
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x21			;Note A-3
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
nem3sel_track_026:
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x33			;Note D#5
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x32			;Note D-5
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
nem3sel_track_027:
	.db 0x10			;Note E-2
	.db 0x78			;Instrument 6
	.db 0xdf			;Wait 32
	.db 0x0f			;Note D#2
	.db 0xcf			;Wait 16
	.db 0x0e			;Note D-2
	.db 0xcf			;Wait 16
	.db 0xbf			;[End-Of-Track]
nem3sel_track_028:
	.db 0x34			;Note E-5
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x2f			;Note B-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x33			;Note D#5
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x32			;Note D-5
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x76			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0xbf			;[End-Of-Track]
nem3sel_track_029:
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0x9c			;CMD End 
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x34			;Note E-5
	.db 0xc4			;Wait 5
	.db 0x95,0x68			;CMD Vibrato
	.db 0xca			;Wait 11
	.db 0x2f			;Note B-4
	.db 0x9c			;CMD End 
	.db 0xc7			;Wait 8
	.db 0x95,0x68			;CMD Vibrato
					;Wait Repeat 8
	.db 0x33			;Note D#5
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x32			;Note D-5
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x68			;CMD Vibrato
	.db 0xc2			;Wait 3
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
nem3sel_track_030:
	.db 0x20			;Note G#3
	.db 0x6d			;Volume 11
	.db 0x7c			;Instrument 10
	.db 0xc3			;Wait 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
	.db 0xc1			;Wait 2
	.db 0x25			;Note C#4
	.db 0xc3			;Wait 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
					;Wait Repeat 4
	.db 0x20			;Note G#3
					;Wait Repeat 4
	.db 0x25			;Note C#4
	.db 0xc1			;Wait 2
	.db 0x20			;Note G#3
	.db 0xc3			;Wait 4
	.db 0x21			;Note A-3
					;Wait Repeat 4
	.db 0x26			;Note D-4
					;Wait Repeat 4
	.db 0x21			;Note A-3
	.db 0xc1			;Wait 2
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
nem3sel_track_031:
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
	.db 0x7d			;Instrument 11
	.db 0xc1			;Wait 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2c			;Note G#4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x31			;Note C#5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x33			;Note D#5
					;Wait Repeat 2
	.db 0x38			;Note G#5
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x34			;Note E-5
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0x6c			;Volume 10
					;Wait Repeat 2
	.db 0x32			;Note D-5
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x34			;Note E-5
					;Wait Repeat 2
	.db 0x39			;Note A-5
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]

