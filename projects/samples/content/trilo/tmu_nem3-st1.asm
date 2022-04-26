; Song: NEM3 -Stage 1                   
; By:   Konami (Arr. by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	db $07					; Initial song speed.
	dw .waveform_start			; Start of the waveform data.
	dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
.restart:
	dw .track_000, .track_001, .track_002, .track_003, .track_004, .track_005, .track_006, .track_007	; Step:000 Pattern:000
	dw .track_008, .track_009, .track_010, .track_010, .track_012, .track_013, .track_014, .track_015	; Step:001 Pattern:001
	dw .track_000, .track_017, .track_010, .track_010, .track_004, .track_005, .track_006, .track_007	; Step:002 Pattern:002
	dw .track_024, .track_009, .track_010, .track_010, .track_012, .track_013, .track_014, .track_015	; Step:003 Pattern:003
	dw .track_032, .track_033, .track_010, .track_010, .track_036, .track_037, .track_038, .track_039	; Step:004 Pattern:004
	dw .track_032, .track_041, .track_010, .track_010, .track_044, .track_045, .track_046, .track_047	; Step:005 Pattern:005
	dw .track_032, .track_049, .track_010, .track_010, .track_052, .track_053, .track_054, .track_055	; Step:006 Pattern:006
	dw .track_056, .track_057, .track_010, .track_010, .track_060, .track_061, .track_062, .track_063	; Step:007 Pattern:007
	dw .track_064, .track_065, .track_010, .track_010, .track_068, .track_069, .track_070, .track_071	; Step:008 Pattern:008
	dw .track_072, .track_073, .track_010, .track_010, .track_076, .track_077, .track_078, .track_079	; Step:009 Pattern:009
	dw .track_064, .track_081, .track_010, .track_010, .track_068, .track_069, .track_086, .track_087	; Step:010 Pattern:010
	dw .track_088, .track_089, .track_010, .track_010, .track_092, .track_093, .track_094, .track_095	; Step:011 Pattern:011
	dw 0x0000, .restart				; End of sequence delimiter + restart address.

; [ Custom FM voices ]
.customvoice_start:

; [ SCC Waveforms ]
.waveform_start:
	db $80, $b0, $c0, $10, $1a, $2a, $2c, $1a, $00, $e0, $d0, $e0, $22, $53, $70, $75, $70, $31, $ea, $80, $88, $8a, $8c, $8e, $00, $7f, $75, $73, $62, $00, $c0, $90				; Waveform:1
	db $00, $40, $7f, $40, $01, $c0, $81, $c0, $01, $40, $7f, $40, $01, $c0, $01, $40, $01, $e0, $01, $20, $01, $f0, $01, $10, $01, $ff, $ff, $ff, $ff, $40, $40, $40				; Waveform:2
	db $a0, $90, $a0, $ac, $f0, $a0, $16, $00, $90, $00, $20, $40, $60, $50, $38, $1c, $f2, $e2, $d6, $d0, $d0, $da, $dc, $e0, $e2, $e2, $e2, $e0, $dc, $d8, $d0, $b0				; Waveform:7
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $80, $90, $a0, $b0, $c0, $d0, $e0, $f0, $00, $10, $20, $30, $40, $50, $60, $70				; Waveform:8
	db $00, $00, $00, $00, $00, $70, $70, $00, $00, $80, $80, $80, $00, $00, $00, $00, $70, $70, $70, $00, $80, $80, $00, $00, $00, $00, $70, $70, $00, $00, $80, $80				; Waveform:15
	db $00, $7f, $00, $80, $a0, $c0, $d8, $f0, $08, $20, $30, $40, $50, $60, $70, $78, $7c, $7f, $7c, $78, $70, $60, $50, $40, $30, $20, $08, $f0, $d8, $c0, $a0, $80				; Waveform:17
	db $00, $68, $40, $04, $00, $26, $40, $f5, $10, $40, $20, $d0, $f0, $00, $c0, $80, $d0, $30, $c0, $a0, $b0, $c0, $80, $d0, $f0, $40, $10, $c0, $d4, $00, $c0, $e0				; Waveform:19
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $80, $a0, $c0, $e0, $00, $20, $40, $60, $80, $a0, $c0, $e0, $00, $20, $40, $60				; Waveform:25
	db $90, $88, $90, $9c, $b0, $c0, $e0, $26, $70, $7c, $70, $56, $40, $20, $10, $fc, $e0, $fc, $10, $fc, $40, $54, $72, $7f, $70, $26, $e0, $c0, $b0, $9c, $90, $88				; Waveform:30

; [ FM Drum macros]
.drummacro_start:

; [ Instruments]
.instrument_start:
	dw .instrument_00				; BD G2           
	dw .instrument_01				; SD A3           
	dw .instrument_02				; HH CL A8        
	dw .instrument_03				; Short Snare     
	dw .instrument_04				; SCC Rhythm      
	dw .instrument_05				; SC Bass         
	dw .instrument_06				; Remate ARP      
	dw .instrument_07				; ARP SQ          
	dw .instrument_08				; ARP SCC         
	dw .instrument_09				; ARP END SCC     
	dw .instrument_10				; THEME SCC       
	dw .instrument_11				; THEME STACC     
	dw .instrument_12				; ARP SEMI FINAL  
	dw .instrument_13				; ARP semi ch5    
	dw .instrument_14				; CONT            

.instrument_00:					; BD G2           
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $1c,$03						; Envelope shape
	db $02,$10						; Mixer (T)
	db $1a						; Envelope
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_01:					; SD A3           
	db $00						; Waveform 0
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$01						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0d						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0b						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$07						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$06						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$05						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$04						; Volume _
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_02:					; HH CL A8        
	db $10						; Waveform 2
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_03:					; Short Snare     
	db $00						; Waveform 0
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$06						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0b						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
								; --- Macro loop
	db $02,$80						; Mixer (N)
	db $0E,$06						; Noise _
	db $0c,$01						; Volume -
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $18,$ed						; Loop (-20)

.instrument_04:					; SCC Rhythm      
	db $30						; Waveform 6
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_05:					; SC Bass         
	db $00						; Waveform 0
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_06:					; Remate ARP      
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_07:					; ARP SQ          
	db $30						; Waveform 6
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_08:					; ARP SCC         
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_09:					; ARP END SCC     
	db $18						; Waveform 3
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_10:					; THEME SCC       
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_11:					; THEME STACC     
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_12:					; ARP SEMI FINAL  
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_13:					; ARP semi ch5    
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_14:					; CONT            
	db $40						; Waveform 8
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $18,$fb						; Loop (-6)


; [ Song track data ]
.track_000:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6c			;Volume 11
	db $74			;Instrument 3
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6c			;Volume 11
	db $74			;Instrument 3
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_001:
	db $24			;Note C-4
	db $6c			;Volume 11
	db $7f			;Instrument 14
	db $a5,$50			;CMD Envelope multiplier low
	db $c3			;Wait 4
	db $30			;Note C-5
	db $a1,$01			;CMD Track detune
	db $c1			;Wait 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2e			;Note A#4
	db $94,$17			;CMD Vibrato
	db $d1			;Wait 18
	db $24			;Note C-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $94,$17			;CMD Vibrato
	db $cf			;Wait 16
	db $bf			;[End-Of-Track]
.track_002:
	db $70			;Volume 15
	db $71			;Instrument 0
	db $a4,$02			;CMD Envelope multiplier high
	db $ef			;Wait 48
	db $bf			;[End-Of-Track]
.track_003:
	db $70			;Volume 15
	db $71			;Instrument 0
	db $ef			;Wait 48
	db $bf			;[End-Of-Track]
.track_004:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
	db $c5			;Wait 6
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
	db $c5			;Wait 6
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_005:
	db $24			;Note C-4
	db $69			;Volume 8
	db $75			;Instrument 4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_006:
	db $24			;Note C-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2e			;Note A#4
	db $94,$26			;CMD Vibrato
	db $d1			;Wait 18
	db $24			;Note C-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $94,$36			;CMD Vibrato
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_007:
	db $1f			;Note G-3
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $24			;Note C-4
	db $94,$26			;CMD Vibrato
	db $d1			;Wait 18
	db $1f			;Note G-3
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $24			;Note C-4
	db $94,$36			;CMD Vibrato
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_008:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6c			;Volume 11
	db $74			;Instrument 3
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6e			;Volume 13
	db $72			;Instrument 1
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
	db $bf			;[End-Of-Track]
.track_009:
	db $c1			;Wait 2
	db $24			;Note C-4
	db $6c			;Volume 11
	db $7f			;Instrument 14
	db $9b			;CMD End 
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $94,$17			;CMD Vibrato
	db $d1			;Wait 18
	db $18			;Note C-3
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_010:
	db $ef			;Wait 48
	db $bf			;[End-Of-Track]
.track_012:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
	db $c5			;Wait 6
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_013:
	db $24			;Note C-4
	db $69			;Volume 8
	db $75			;Instrument 4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
	db $6a			;Volume 9
	db $77			;Instrument 6
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_014:
	db $24			;Note C-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $94,$46			;CMD Vibrato
	db $d1			;Wait 18
	db $18			;Note C-3
	db $6a			;Volume 9
	db $75			;Instrument 4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2e			;Note A#4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_015:
	db $1f			;Note G-3
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $24			;Note C-4
	db $94,$36			;CMD Vibrato
	db $d1			;Wait 18
	db $13			;Note G-2
	db $6a			;Volume 9
	db $75			;Instrument 4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_017:
	db $2e			;Note A#4
	db $6c			;Volume 11
	db $7f			;Instrument 14
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2e			;Note A#4
	db $94,$17			;CMD Vibrato
	db $d1			;Wait 18
	db $24			;Note C-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $94,$17			;CMD Vibrato
	db $cf			;Wait 16
	db $bf			;[End-Of-Track]
.track_024:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6c			;Volume 11
	db $74			;Instrument 3
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6e			;Volume 13
	db $72			;Instrument 1
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6e			;Volume 13
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_032:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_033:
	db $c3			;Wait 4
	db $29			;Note F-4
	db $6c			;Volume 11
	db $7f			;Instrument 14
	db $c5			;Wait 6
	db $94,$37			;CMD Vibrato
	db $d7			;Wait 24
	db $24			;Note C-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $29			;Note F-4
					;Wait Repeat 6
	db $30			;Note C-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_036:
	db $09			;Note A-1
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $09			;Note A-1
					;Wait Repeat 2
	db $15			;Note A-2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_037:
	db $18			;Note C-3
	db $69			;Volume 8
	db $75			;Instrument 4
	db $c1			;Wait 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $21			;Note A-3
	db $c1			;Wait 2
	db $24			;Note C-4
	db $68			;Volume 7
					;Wait Repeat 2
	db $29			;Note F-4
	db $69			;Volume 8
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $1d			;Note F-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_038:
	db $29			;Note F-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$46			;CMD Vibrato
	db $d6			;Wait 23
	db $66			;Volume 5
	db $c0			;Wait 1
	db $24			;Note C-4
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $66			;Volume 5
	db $c0			;Wait 1
	db $29			;Note F-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $bf			;[End-Of-Track]
.track_039:
	db $24			;Note C-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$46			;CMD Vibrato
	db $d6			;Wait 23
	db $66			;Volume 5
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $24			;Note C-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $bf			;[End-Of-Track]
.track_041:
	db $c3			;Wait 4
	db $30			;Note C-5
	db $6d			;Volume 12
	db $7f			;Instrument 14
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $c6			;Wait 7
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $c6			;Wait 7
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $c6			;Wait 7
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $94,$37			;CMD Vibrato
	db $c2			;Wait 3
	db $bf			;[End-Of-Track]
.track_044:
	db $0a			;Note A#1
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $0a			;Note A#1
					;Wait Repeat 2
	db $16			;Note A#2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_045:
	db $1a			;Note D-3
	db $69			;Volume 8
	db $75			;Instrument 4
	db $c1			;Wait 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $1d			;Note F-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_046:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2e			;Note A#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $29			;Note F-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_047:
	db $2b			;Note G-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $29			;Note F-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $29			;Note F-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $26			;Note D-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_049:
	db $c3			;Wait 4
	db $2c			;Note G#4
	db $6d			;Volume 12
	db $7f			;Instrument 14
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $c6			;Wait 7
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2c			;Note G#4
	db $c1			;Wait 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $2c			;Note G#4
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $27			;Note D#4
					;Wait Repeat 6
	db $2c			;Note G#4
					;Wait Repeat 6
	db $30			;Note C-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_052:
	db $08			;Note G#1
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $08			;Note G#1
					;Wait Repeat 2
	db $14			;Note G#2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_053:
	db $1b			;Note D#3
	db $75			;Instrument 4
	db $c1			;Wait 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $20			;Note G#3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_054:
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $ca			;Wait 11
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $2c			;Note G#4
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $27			;Note D#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $bf			;[End-Of-Track]
.track_055:
	db $27			;Note D#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $ca			;Wait 11
	db $64			;Volume 3
	db $c0			;Wait 1
	db $27			;Note D#4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
	db $c1			;Wait 2
	db $27			;Note D#4
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $24			;Note C-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $27			;Note D#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $c4			;Wait 5
	db $64			;Volume 3
	db $bf			;[End-Of-Track]
.track_056:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6e			;Volume 13
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 2
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 1
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_057:
	db $c3			;Wait 4
	db $30			;Note C-5
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $cc			;Wait 13
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2f			;Note B-4
	db $c4			;Wait 5
	db $94,$27			;CMD Vibrato
	db $ce			;Wait 15
	db $bf			;[End-Of-Track]
.track_060:
	db $07			;Note G-1
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $07			;Note G-1
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0e			;Note D-2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_061:
	db $1a			;Note D-3
	db $75			;Instrument 4
	db $c1			;Wait 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $23			;Note B-3
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_062:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $ca			;Wait 11
	db $64			;Volume 3
	db $c0			;Wait 1
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $30			;Note C-5
	db $c1			;Wait 2
	db $2f			;Note B-4
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $d0			;Wait 17
	db $64			;Volume 3
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_063:
	db $2b			;Note G-4
	db $6b			;Volume 10
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $ca			;Wait 11
	db $64			;Volume 3
	db $c0			;Wait 1
	db $2b			;Note G-4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $d0			;Wait 17
	db $64			;Volume 3
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_064:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_065:
	db $60			;Release 96
	db $c0			;Wait 1
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $30			;Note C-5
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $a1,$03			;CMD Track detune
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_068:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0c			;Note C-2
	db $c3			;Wait 4
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0c			;Note C-2
					;Wait Repeat 2
	db $0f			;Note D#2
	db $c3			;Wait 4
	db $0f			;Note D#2
	db $c1			;Wait 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0f			;Note D#2
	db $c3			;Wait 4
	db $0f			;Note D#2
	db $c1			;Wait 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0f			;Note D#2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_069:
	db $24			;Note C-4
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $a9,$08			; SCC Set Waveform 2
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c3			;Wait 4
	db $24			;Note C-4
	db $c1			;Wait 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $22			;Note A#3
	db $c3			;Wait 4
	db $22			;Note A#3
	db $c1			;Wait 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $22			;Note A#3
	db $c3			;Wait 4
	db $22			;Note A#3
	db $c1			;Wait 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $22			;Note A#3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_070:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7d			;Instrument 12
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_071:
	db $2b			;Note G-4
	db $69			;Volume 8
	db $7e			;Instrument 13
	db $c1			;Wait 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_072:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_073:
	db $29			;Note F-4
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $30			;Note C-5
	db $6f			;Volume 14
	db $a1,$03			;CMD Track detune
	db $c4			;Wait 5
	db $94,$35			;CMD Vibrato
	db $c6			;Wait 7
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $30			;Note C-5
					;Wait Repeat 6
	db $2f			;Note B-4
	db $c3			;Wait 4
	db $94,$35			;CMD Vibrato
					;Wait Repeat 4
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $32			;Note D-5
	db $c3			;Wait 4
	db $94,$35			;CMD Vibrato
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_076:
	db $14			;Note G#2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $14			;Note G#2
	db $c1			;Wait 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
	db $c3			;Wait 4
	db $14			;Note G#2
	db $c1			;Wait 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $13			;Note G-2
	db $c3			;Wait 4
	db $13			;Note G-2
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $13			;Note G-2
	db $c3			;Wait 4
	db $13			;Note G-2
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $0e			;Note D-2
					;Wait Repeat 2
	db $0c			;Note C-2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_077:
	db $27			;Note D#4
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $a9,$08			; SCC Set Waveform 2
	db $c3			;Wait 4
	db $27			;Note D#4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
	db $c3			;Wait 4
	db $27			;Note D#4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $26			;Note D-4
	db $c3			;Wait 4
	db $26			;Note D-4
	db $c1			;Wait 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $26			;Note D-4
	db $c3			;Wait 4
	db $26			;Note D-4
	db $c1			;Wait 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $26			;Note D-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_078:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $94,$45			;CMD Vibrato
	db $c7			;Wait 8
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $30			;Note C-5
					;Wait Repeat 6
	db $2f			;Note B-4
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $c4			;Wait 5
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $30			;Note C-5
	db $c1			;Wait 2
	db $32			;Note D-5
	db $6c			;Volume 11
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_079:
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $94,$45			;CMD Vibrato
	db $c7			;Wait 8
	db $27			;Note D#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2c			;Note G#4
					;Wait Repeat 6
	db $2b			;Note G-4
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $c4			;Wait 5
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2b			;Note G-4
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_081:
	db $a5,$08			;CMD Envelope multiplier low
	db $c3			;Wait 4
	db $30			;Note C-5
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $a1,$03			;CMD Track detune
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_086:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7d			;Instrument 12
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_087:
	db $2b			;Note G-4
	db $69			;Volume 8
	db $7e			;Instrument 13
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_088:
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $29			;Note F-4
	db $71			;Instrument 0
					;Wait Repeat 2
	db $29			;Note F-4
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c0			;Wait 1
	db $21			;Note A-3
	db $6e			;Volume 13
					;Wait Repeat 1
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_089:
	db $29			;Note F-4
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $a5,$08			;CMD Envelope multiplier low
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $c1			;Wait 2
	db $30			;Note C-5
	db $6f			;Volume 14
	db $a1,$03			;CMD Track detune
	db $c4			;Wait 5
	db $94,$35			;CMD Vibrato
	db $c6			;Wait 7
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $30			;Note C-5
	db $6c			;Volume 11
	db $c1			;Wait 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $30			;Note C-5
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $a1,$00			;CMD Track detune
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $3c			;Note C-6
	db $c1			;Wait 2
	db $3a			;Note A#5
	db $6c			;Volume 11
	db $78			;Instrument 7
	db $c0			;Wait 1
	db $33			;Note D#5
	db $6b			;Volume 10
					;Wait Repeat 1
	db $2e			;Note A#4
	db $6a			;Volume 9
					;Wait Repeat 1
	db $27			;Note D#4
					;Wait Repeat 1
	db $22			;Note A#3
					;Wait Repeat 1
	db $1b			;Note D#3
					;Wait Repeat 1
	db $38			;Note G#5
					;Wait Repeat 1
	db $31			;Note C#5
	db $c0			;Wait 1
	db $2c			;Note G#4
	db $94,$35			;CMD Vibrato
					;Wait Repeat 1
	db $25			;Note C#4
					;Wait Repeat 1
	db $20			;Note G#3
					;Wait Repeat 1
	db $19			;Note C#3
	db $bf			;[End-Of-Track]
.track_092:
	db $14			;Note G#2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $14			;Note G#2
	db $c1			;Wait 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
	db $c3			;Wait 4
	db $14			;Note G#2
	db $c1			;Wait 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $0d			;Note C#2
					;Wait Repeat 2
	db $19			;Note C#3
					;Wait Repeat 2
	db $0d			;Note C#2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_093:
	db $27			;Note D#4
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $a9,$08			; SCC Set Waveform 2
	db $c3			;Wait 4
	db $27			;Note D#4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
	db $c3			;Wait 4
	db $27			;Note D#4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $6c			;Volume 11
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $25			;Note C#4
					;Wait Repeat 2
	db $25			;Note C#4
					;Wait Repeat 2
	db $25			;Note C#4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_094:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $c3			;Wait 4
	db $94,$45			;CMD Vibrato
	db $c7			;Wait 8
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $30			;Note C-5
	db $c5			;Wait 6
	db $3e			;Note D-6
	db $69			;Volume 8
	db $7a			;Instrument 9
	db $c1			;Wait 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $3a			;Note A#5
	db $c0			;Wait 1
	db $33			;Note D#5
					;Wait Repeat 1
	db $2e			;Note A#4
					;Wait Repeat 1
	db $27			;Note D#4
					;Wait Repeat 1
	db $22			;Note A#3
					;Wait Repeat 1
	db $1b			;Note D#3
					;Wait Repeat 1
	db $38			;Note G#5
					;Wait Repeat 1
	db $31			;Note C#5
					;Wait Repeat 1
	db $2c			;Note G#4
					;Wait Repeat 1
	db $25			;Note C#4
					;Wait Repeat 1
	db $20			;Note G#3
					;Wait Repeat 1
	db $19			;Note C#3
	db $bf			;[End-Of-Track]
.track_095:
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $c3			;Wait 4
	db $94,$45			;CMD Vibrato
	db $c7			;Wait 8
	db $27			;Note D#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2c			;Note G#4
	db $c5			;Wait 6
	db $3b			;Note B-5
	db $69			;Volume 8
	db $7a			;Instrument 9
	db $c1			;Wait 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $37			;Note G-5
	db $c0			;Wait 1
	db $2e			;Note A#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $27			;Note D#4
					;Wait Repeat 1
	db $22			;Note A#3
					;Wait Repeat 1
	db $1f			;Note G-3
					;Wait Repeat 1
	db $35			;Note F-5
					;Wait Repeat 1
	db $2c			;Note G#4
					;Wait Repeat 1
	db $29			;Note F-4
					;Wait Repeat 1
	db $25			;Note C#4
					;Wait Repeat 1
	db $20			;Note G#3
					;Wait Repeat 1
	db $1d			;Note F-3
	db $bf			;[End-Of-Track]

