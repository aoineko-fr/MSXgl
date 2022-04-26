; Song: NEM3- Air Battle                
; By:   Konami (Arr. by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	db $04					; Initial song speed.
	dw .waveform_start			; Start of the waveform data.
	dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
	dw .track_000, .track_000, .track_000, .track_003, .track_000, .track_000, .track_000, .track_000	; Step:000 Pattern:000
	dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:001 Pattern:001
	dw .track_016, .track_017, .track_018, .track_018, .track_020, .track_021, .track_014, .track_015	; Step:002 Pattern:002
.restart:
	dw .track_008, .track_025, .track_018, .track_018, .track_028, .track_029, .track_030, .track_031	; Step:003 Pattern:003
	dw .track_008, .track_033, .track_018, .track_018, .track_036, .track_037, .track_038, .track_039	; Step:004 Pattern:004
	dw .track_008, .track_041, .track_018, .track_018, .track_028, .track_029, .track_030, .track_047	; Step:005 Pattern:005
	dw .track_016, .track_049, .track_018, .track_018, .track_052, .track_037, .track_054, .track_055	; Step:006 Pattern:006
	dw .track_056, .track_057, .track_018, .track_018, .track_060, .track_061, .track_062, .track_063	; Step:007 Pattern:007
	dw .track_056, .track_065, .track_018, .track_018, .track_068, .track_069, .track_070, .track_071	; Step:008 Pattern:008
	dw .track_056, .track_073, .track_018, .track_018, .track_076, .track_077, .track_078, .track_079	; Step:009 Pattern:009
	dw .track_080, .track_081, .track_018, .track_018, .track_084, .track_085, .track_086, .track_087	; Step:010 Pattern:010
	dw .track_088, .track_089, .track_018, .track_018, .track_092, .track_093, .track_094, .track_095	; Step:011 Pattern:011
	dw .track_096, .track_097, .track_018, .track_018, .track_100, .track_101, .track_102, .track_103	; Step:012 Pattern:012
	dw .track_104, .track_105, .track_018, .track_018, .track_100, .track_109, .track_110, .track_111	; Step:013 Pattern:013
	dw .track_096, .track_113, .track_018, .track_018, .track_100, .track_117, .track_102, .track_103	; Step:014 Pattern:014
	dw .track_104, .track_105, .track_018, .track_018, .track_100, .track_109, .track_126, .track_127	; Step:015 Pattern:015
	dw .track_096, .track_129, .track_018, .track_018, .track_132, .track_133, .track_134, .track_135	; Step:016 Pattern:016
	dw .track_136, .track_137, .track_018, .track_018, .track_140, .track_141, .track_142, .track_143	; Step:017 Pattern:017
	dw .track_144, .track_145, .track_018, .track_018, .track_148, .track_149, .track_150, .track_151	; Step:018 Pattern:018
	dw 0x0000, .restart				; End of sequence delimiter + restart address.

; [ Custom FM voices ]
.customvoice_start:

; [ SCC Waveforms ]
.waveform_start:
	db $80, $b0, $c0, $10, $1a, $2a, $2c, $1a, $00, $e0, $d0, $e0, $22, $53, $70, $75, $70, $31, $ea, $80, $88, $8a, $8c, $8e, $00, $7f, $75, $73, $62, $00, $c0, $90				; Waveform:1
	db $00, $40, $7f, $40, $01, $c0, $81, $c0, $01, $40, $7f, $40, $01, $c0, $01, $40, $01, $e0, $01, $20, $01, $f0, $01, $10, $01, $ff, $ff, $ff, $ff, $40, $40, $40				; Waveform:2
	db $00, $f8, $f0, $e8, $e0, $d8, $d0, $c8, $c0, $b8, $b0, $a8, $a0, $98, $90, $88, $80, $78, $70, $68, $60, $58, $50, $48, $40, $38, $30, $28, $20, $18, $10, $08				; Waveform:6
	db $a0, $90, $a0, $ac, $f0, $a0, $16, $00, $90, $00, $20, $40, $60, $50, $38, $1c, $f2, $e2, $d6, $d0, $d0, $da, $dc, $e0, $e2, $e2, $e2, $e0, $dc, $d8, $d0, $b0				; Waveform:7
	db $80, $aa, $c8, $00, $24, $40, $5c, $70, $7f, $6a, $4a, $26, $00, $d0, $a8, $8c, $80, $aa, $c8, $00, $24, $40, $5c, $70, $7f, $6a, $4a, $26, $00, $d0, $a8, $8c				; Waveform:11
	db $30, $50, $50, $30, $00, $00, $10, $40, $60, $70, $60, $30, $f0, $e0, $e0, $00, $20, $20, $10, $c0, $a0, $90, $a0, $c0, $00, $00, $d0, $b0, $b0, $d0, $00, $00				; Waveform:13
	db $00, $7f, $00, $80, $a0, $c0, $d8, $f0, $08, $20, $30, $40, $50, $60, $70, $78, $7c, $7f, $7c, $78, $70, $60, $50, $40, $30, $20, $08, $f0, $d8, $c0, $a0, $80				; Waveform:17
	db $00, $68, $40, $04, $00, $26, $40, $f5, $10, $40, $20, $d0, $f0, $00, $c0, $80, $d0, $30, $c0, $a0, $b0, $c0, $80, $d0, $f0, $40, $10, $c0, $d4, $00, $c0, $e0				; Waveform:19
	db $00, $68, $48, $18, $48, $68, $28, $00, $48, $77, $58, $08, $28, $38, $00, $b8, $08, $58, $00, $e8, $f8, $00, $b8, $98, $c8, $08, $e8, $a8, $c8, $f8, $c8, $a8				; Waveform:20
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $00, $e7, $cf, $b9, $a6, $96, $8b, $83, $80, $83, $8b, $96, $a6, $b9, $cf, $e7				; Waveform:21
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $80, $a0, $c0, $e0, $00, $20, $40, $60, $80, $a0, $c0, $e0, $00, $20, $40, $60				; Waveform:25
	db $90, $88, $90, $9c, $b0, $c0, $e0, $26, $70, $7c, $70, $56, $40, $20, $10, $fc, $e0, $fc, $10, $fc, $40, $54, $72, $7f, $70, $26, $e0, $c0, $b0, $9c, $90, $88				; Waveform:30
	db $00, $00, $00, $80, $00, $70, $70, $70, $00, $00, $00, $80, $00, $00, $00, $80, $80, $80, $80, $00, $80, $00, $00, $00, $00, $80, $80, $80, $00, $80, $80, $80				; Waveform:31

; [ FM Drum macros]
.drummacro_start:

; [ Instruments]
.instrument_start:
	dw .instrument_00				; BD G2           
	dw .instrument_01				; SD A3           
	dw .instrument_02				; HH CL A8        
	dw .instrument_03				; TOM             
	dw .instrument_04				; Short Snare     
	dw .instrument_05				; PAUSE K         
	dw .instrument_06				; SC Bass         
	dw .instrument_07				; Pause final     
	dw .instrument_08				; BASS LONG       
	dw .instrument_09				; Staccato BASS   
	dw .instrument_10				; ARP SQ          
	dw .instrument_11				; ARP SCC         
	dw .instrument_12				; ARP2 SCC        
	dw .instrument_13				; CHORDS END      
	dw .instrument_14				; ARP SCC3        
	dw .instrument_15				; THEME SCC       
	dw .instrument_16				; THEME LOW       
	dw .instrument_17				; CHORDS FINAL    
	dw .instrument_18				; SLIDE FINAL     
	dw .instrument_19				; CONT            

.instrument_00:					; BD G2           
	db $28						; Waveform 5
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
	db $18						; Waveform 3
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_03:					; TOM             
	db $48						; Waveform 9
	db $02,$10						; Mixer (T)
	db $04,$10,$00					; Tone add
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $04,$10,$00					; Tone add
	db $08,$0e						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $04,$10,$00					; Tone add
	db $0c,$01						; Volume -
	db $02,$10						; Mixer (T)
	db $04,$10,$00					; Tone add
	db $00							; Volume same
	db $18,$f2						; Loop (-15)

.instrument_04:					; Short Snare     
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

.instrument_05:					; PAUSE K         
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_06:					; SC Bass         
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

.instrument_07:					; Pause final     
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_08:					; BASS LONG       
	db $00						; Waveform 0
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
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
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_09:					; Staccato BASS   
	db $00						; Waveform 0
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
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_10:					; ARP SQ          
	db $38						; Waveform 7
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

.instrument_11:					; ARP SCC         
	db $40						; Waveform 8
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
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_12:					; ARP2 SCC        
	db $40						; Waveform 8
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_13:					; CHORDS END      
	db $08						; Waveform 1
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
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_14:					; ARP SCC3        
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_15:					; THEME SCC       
	db $50						; Waveform 10
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
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_16:					; THEME LOW       
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
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $18,$d0						; Loop (-49)

.instrument_17:					; CHORDS FINAL    
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
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_18:					; SLIDE FINAL     
	db $10						; Waveform 2
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_19:					; CONT            
	db $58						; Waveform 11
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $18,$fb						; Loop (-6)


; [ Song track data ]
.track_000:
	db $70			;Volume 15
	db $71			;Instrument 0
	db $d7			;Wait 24
	db $bf			;[End-Of-Track]
.track_003:
	db $30			;Note C-5
	db $6e			;Volume 13
	db $76			;Instrument 5
	db $a3,$05			;CMD Speed
	db $c1			;Wait 2
	db $37			;Note G-5
	db $c1			;Wait 2
	db $34			;Note E-5
	db $70			;Volume 15
					;Wait Repeat 2
	db $37			;Note G-5
	db $6e			;Volume 13
					;Wait Repeat 2
	db $3c			;Note C-6
	db $78			;Instrument 7
	db $c7			;Wait 8
	db $60			;Release 96
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_008:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_009:
	db $60			;Release 96
	db $a5,$80			;CMD Envelope multiplier low
	db $c5			;Wait 6
	db $32			;Note D-5
	db $6e			;Volume 13
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_010:
	db $a4,$02			;CMD Envelope multiplier high
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_011:
	db $a3,$04			;CMD Speed
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_012:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $77			;Instrument 6
	db $c7			;Wait 8
	db $18			;Note C-3
	db $c3			;Wait 4
	db $0c			;Note C-2
	db $c7			;Wait 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
					;Wait Repeat 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
					;Wait Repeat 8
	db $0c			;Note C-2
	db $c3			;Wait 4
	db $18			;Note C-3
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_013:
	db $32			;Note D-5
	db $68			;Volume 7
	db $7c			;Instrument 11
	db $c3			;Wait 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_014:
	db $2b			;Note G-4
	db $6e			;Volume 13
	db $77			;Instrument 6
	db $a9,$08			; SCC Set Waveform 2
	db $cb			;Wait 12
	db $2b			;Note G-4
					;Wait Repeat 12
	db $2b			;Note G-4
					;Wait Repeat 12
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $2a			;Note F#4
	db $7e			;Instrument 13
	db $c7			;Wait 8
	db $2a			;Note F#4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_015:
	db $26			;Note D-4
	db $6e			;Volume 13
	db $77			;Instrument 6
	db $cb			;Wait 12
	db $26			;Note D-4
					;Wait Repeat 12
	db $26			;Note D-4
					;Wait Repeat 12
	db $26			;Note D-4
	db $cb			;Wait 12
	db $26			;Note D-4
	db $7e			;Instrument 13
	db $c7			;Wait 8
	db $26			;Note D-4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_016:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_017:
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6e			;Volume 13
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $26			;Note D-4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $28			;Note E-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2a			;Note F#4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_018:
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_020:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $77			;Instrument 6
	db $c7			;Wait 8
	db $18			;Note C-3
	db $c3			;Wait 4
	db $0c			;Note C-2
	db $c7			;Wait 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
					;Wait Repeat 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
	db $c3			;Wait 4
	db $0a			;Note A#1
					;Wait Repeat 4
	db $16			;Note A#2
					;Wait Repeat 4
	db $0b			;Note B-1
					;Wait Repeat 4
	db $17			;Note B-2
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_021:
	db $32			;Note D-5
	db $68			;Volume 7
	db $7c			;Instrument 11
	db $c3			;Wait 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $26			;Note D-4
	db $c1			;Wait 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $28			;Note E-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2a			;Note F#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $2d			;Note A-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_025:
	db $c3			;Wait 4
	db $2d			;Note A-4
	db $6c			;Volume 11
	db $84			;Instrument 19
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $2a			;Note F#4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $c7			;Wait 8
	db $2d			;Note A-4
	db $c3			;Wait 4
	db $26			;Note D-4
	db $c8			;Wait 9
	db $94,$47			;CMD Vibrato
	db $da			;Wait 27
	db $bf			;[End-Of-Track]
.track_028:
	db $0c			;Note C-2
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $18			;Note C-3
	db $c3			;Wait 4
	db $0c			;Note C-2
	db $c7			;Wait 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
					;Wait Repeat 8
	db $0c			;Note C-2
					;Wait Repeat 8
	db $18			;Note C-3
					;Wait Repeat 8
	db $0c			;Note C-2
	db $c3			;Wait 4
	db $18			;Note C-3
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_029:
	db $32			;Note D-5
	db $67			;Volume 6
	db $7d			;Instrument 12
	db $c3			;Wait 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $26			;Note D-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_030:
	db $2d			;Note A-4
	db $6b			;Volume 10
	db $80			;Instrument 15
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $2a			;Note F#4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $c7			;Wait 8
	db $2d			;Note A-4
	db $c3			;Wait 4
	db $26			;Note D-4
	db $cd			;Wait 14
	db $94,$57			;CMD Vibrato
	db $d8			;Wait 25
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_031:
	db $2a			;Note F#4
	db $6b			;Volume 10
	db $80			;Instrument 15
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $28			;Note E-4
	db $c7			;Wait 8
	db $2a			;Note F#4
	db $c3			;Wait 4
	db $21			;Note A-3
	db $cd			;Wait 14
	db $94,$57			;CMD Vibrato
	db $d8			;Wait 25
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_033:
	db $c2			;Wait 3
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $28			;Note E-4
	db $6c			;Volume 11
	db $84			;Instrument 19
	db $c7			;Wait 8
	db $94,$47			;CMD Vibrato
	db $cb			;Wait 12
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $c8			;Wait 9
	db $94,$47			;CMD Vibrato
	db $d6			;Wait 23
	db $bf			;[End-Of-Track]
.track_036:
	db $0a			;Note A#1
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $16			;Note A#2
	db $c3			;Wait 4
	db $0a			;Note A#1
	db $c7			;Wait 8
	db $0a			;Note A#1
					;Wait Repeat 8
	db $16			;Note A#2
					;Wait Repeat 8
	db $0a			;Note A#1
					;Wait Repeat 8
	db $16			;Note A#2
					;Wait Repeat 8
	db $0a			;Note A#1
	db $c3			;Wait 4
	db $16			;Note A#2
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_037:
	db $30			;Note C-5
	db $67			;Volume 6
	db $7d			;Instrument 12
	db $c3			;Wait 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $24			;Note C-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_038:
	db $28			;Note E-4
	db $6c			;Volume 11
	db $80			;Instrument 15
	db $a9,$38			; SCC Set Waveform 19
	db $c7			;Wait 8
	db $94,$57			;CMD Vibrato
	db $cb			;Wait 12
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $c8			;Wait 9
	db $94,$57			;CMD Vibrato
	db $d6			;Wait 23
	db $29			;Note F-4
	db $6b			;Volume 10
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 25
	db $c0			;Wait 1
	db $2a			;Note F#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $2c			;Note G#4
	db $bf			;[End-Of-Track]
.track_039:
	db $1c			;Note E-3
	db $6c			;Volume 11
	db $81			;Instrument 16
	db $d3			;Wait 20
	db $1d			;Note F-3
	db $c3			;Wait 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $24			;Note C-4
	db $df			;Wait 32
	db $29			;Note F-4
	db $67			;Volume 6
	db $c0			;Wait 1
	db $2a			;Note F#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $2c			;Note G#4
	db $bf			;[End-Of-Track]
.track_041:
	db $29			;Note F-4
	db $6a			;Volume 9
	db $84			;Instrument 19
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $2a			;Note F#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $2c			;Note G#4
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $6c			;Volume 11
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $2a			;Note F#4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $c7			;Wait 8
	db $2d			;Note A-4
	db $c3			;Wait 4
	db $26			;Note D-4
	db $c8			;Wait 9
	db $94,$47			;CMD Vibrato
	db $da			;Wait 27
	db $bf			;[End-Of-Track]
.track_047:
	db $2a			;Note F#4
	db $6b			;Volume 10
	db $80			;Instrument 15
	db $c3			;Wait 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $28			;Note E-4
	db $c7			;Wait 8
	db $2a			;Note F#4
	db $c3			;Wait 4
	db $21			;Note A-3
	db $cd			;Wait 14
	db $94,$57			;CMD Vibrato
	db $d8			;Wait 25
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_049:
	db $c2			;Wait 3
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $28			;Note E-4
	db $6c			;Volume 11
	db $84			;Instrument 19
	db $c7			;Wait 8
	db $94,$47			;CMD Vibrato
	db $cb			;Wait 12
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $c8			;Wait 9
	db $94,$47			;CMD Vibrato
	db $c2			;Wait 3
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c7			;Wait 8
	db $29			;Note F-4
					;Wait Repeat 8
	db $28			;Note E-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_052:
	db $0a			;Note A#1
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $16			;Note A#2
	db $c3			;Wait 4
	db $0a			;Note A#1
	db $c7			;Wait 8
	db $0a			;Note A#1
					;Wait Repeat 8
	db $16			;Note A#2
					;Wait Repeat 8
	db $0a			;Note A#1
					;Wait Repeat 8
	db $16			;Note A#2
	db $c3			;Wait 4
	db $0a			;Note A#1
					;Wait Repeat 4
	db $0c			;Note C-2
					;Wait Repeat 4
	db $0e			;Note D-2
					;Wait Repeat 4
	db $0a			;Note A#1
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_054:
	db $28			;Note E-4
	db $6c			;Volume 11
	db $80			;Instrument 15
	db $d3			;Wait 20
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $cb			;Wait 12
	db $2b			;Note G-4
	db $6b			;Volume 10
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 25
	db $c7			;Wait 8
	db $29			;Note F-4
					;Wait Repeat 8
	db $28			;Note E-4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_055:
	db $24			;Note C-4
	db $6c			;Volume 11
	db $80			;Instrument 15
	db $9b			;CMD End 
	db $d3			;Wait 20
	db $26			;Note D-4
	db $c3			;Wait 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $30			;Note C-5
	db $81			;Instrument 16
	db $c7			;Wait 8
	db $32			;Note D-5
					;Wait Repeat 8
	db $34			;Note E-5
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_056:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_057:
	db $6a			;Volume 9
	db $c3			;Wait 4
	db $28			;Note E-4
	db $6c			;Volume 11
	db $84			;Instrument 19
	db $c6			;Wait 7
	db $94,$15			;CMD Vibrato
	db $cc			;Wait 13
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $c9			;Wait 10
	db $94,$37			;CMD Vibrato
	db $d5			;Wait 22
	db $bf			;[End-Of-Track]
.track_060:
	db $09			;Note A-1
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $15			;Note A-2
	db $c3			;Wait 4
	db $09			;Note A-1
	db $c7			;Wait 8
	db $09			;Note A-1
					;Wait Repeat 8
	db $15			;Note A-2
					;Wait Repeat 8
	db $09			;Note A-1
					;Wait Repeat 8
	db $09			;Note A-1
	db $c3			;Wait 4
	db $0e			;Note D-2
					;Wait Repeat 4
	db $10			;Note E-2
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $15			;Note A-2
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_061:
	db $18			;Note C-3
	db $6b			;Volume 10
	db $7f			;Instrument 14
	db $c3			;Wait 4
	db $1c			;Note E-3
					;Wait Repeat 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $34			;Note E-5
					;Wait Repeat 4
	db $37			;Note G-5
					;Wait Repeat 4
	db $34			;Note E-5
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $1c			;Note E-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_062:
	db $28			;Note E-4
	db $6b			;Volume 10
	db $80			;Instrument 15
	db $c6			;Wait 7
	db $94,$15			;CMD Vibrato
	db $cc			;Wait 13
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
	db $cb			;Wait 12
	db $94,$35			;CMD Vibrato
	db $d3			;Wait 20
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_063:
	db $34			;Note E-5
	db $6a			;Volume 9
	db $81			;Instrument 16
	db $a1,$08			;CMD Track detune
	db $d3			;Wait 20
	db $35			;Note F-5
	db $c3			;Wait 4
	db $37			;Note G-5
					;Wait Repeat 4
	db $3c			;Note C-6
	db $cb			;Wait 12
	db $94,$26			;CMD Vibrato
	db $d3			;Wait 20
	db $37			;Note G-5
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_065:
	db $2b			;Note G-4
	db $6c			;Volume 11
	db $84			;Instrument 19
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $30			;Note C-5
	db $6c			;Volume 11
	db $9b			;CMD End 
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $33			;Note D#5
					;Wait Repeat 4
	db $30			;Note C-5
	db $ce			;Wait 15
	db $94,$57			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_068:
	db $14			;Note G#2
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $20			;Note G#3
	db $c3			;Wait 4
	db $14			;Note G#2
	db $c7			;Wait 8
	db $14			;Note G#2
					;Wait Repeat 8
	db $20			;Note G#3
					;Wait Repeat 8
	db $14			;Note G#2
					;Wait Repeat 8
	db $20			;Note G#3
					;Wait Repeat 8
	db $14			;Note G#2
	db $c3			;Wait 4
	db $20			;Note G#3
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_069:
	db $18			;Note C-3
	db $6b			;Volume 10
	db $7f			;Instrument 14
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $37			;Note G-5
					;Wait Repeat 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_070:
	db $30			;Note C-5
	db $6a			;Volume 9
	db $80			;Instrument 15
	db $a9,$60			; SCC Set Waveform 31
	db $c3			;Wait 4
	db $32			;Note D-5
					;Wait Repeat 4
	db $33			;Note D#5
					;Wait Repeat 4
	db $30			;Note C-5
	db $cb			;Wait 12
	db $94,$67			;CMD Vibrato
	db $e2			;Wait 35
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $32			;Note D-5
	db $6c			;Volume 11
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 25
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_071:
	db $2c			;Note G#4
	db $6a			;Volume 9
	db $80			;Instrument 15
	db $a1,$00			;CMD Track detune
	db $c3			;Wait 4
	db $2e			;Note A#4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $94,$67			;CMD Vibrato
	db $e3			;Wait 36
	db $26			;Note D-4
	db $6b			;Volume 10
	db $81			;Instrument 16
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_073:
	db $32			;Note D-5
	db $6c			;Volume 11
	db $84			;Instrument 19
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $34			;Note E-5
	db $6c			;Volume 11
	db $cf			;Wait 16
	db $30			;Note C-5
	db $cb			;Wait 12
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $94,$16			;CMD Vibrato
					;Wait Repeat 12
	db $69			;Volume 8
	db $c3			;Wait 4
	db $2b			;Note G-4
	db $6c			;Volume 11
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_076:
	db $13			;Note G-2
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $13			;Note G-2
	db $c7			;Wait 8
	db $13			;Note G-2
					;Wait Repeat 8
	db $1f			;Note G-3
					;Wait Repeat 8
	db $13			;Note G-2
					;Wait Repeat 8
	db $1f			;Note G-3
					;Wait Repeat 8
	db $13			;Note G-2
	db $c3			;Wait 4
	db $1f			;Note G-3
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_077:
	db $18			;Note C-3
	db $7f			;Instrument 14
	db $c3			;Wait 4
	db $1c			;Note E-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $34			;Note E-5
					;Wait Repeat 4
	db $37			;Note G-5
					;Wait Repeat 4
	db $34			;Note E-5
					;Wait Repeat 4
	db $30			;Note C-5
					;Wait Repeat 4
	db $2b			;Note G-4
					;Wait Repeat 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1c			;Note E-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_078:
	db $34			;Note E-5
	db $6d			;Volume 12
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 25
	db $cf			;Wait 16
	db $30			;Note C-5
	db $cb			;Wait 12
	db $2b			;Note G-4
	db $db			;Wait 28
	db $2b			;Note G-4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_079:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $81			;Instrument 16
	db $cf			;Wait 16
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $28			;Note E-4
	db $db			;Wait 28
	db $28			;Note E-4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_080:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6c			;Volume 11
	db $75			;Instrument 4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_081:
	db $c3			;Wait 4
	db $2d			;Note A-4
	db $6b			;Volume 10
	db $84			;Instrument 19
	db $9b			;CMD End 
	db $cf			;Wait 16
	db $2f			;Note B-4
	db $cb			;Wait 12
	db $30			;Note C-5
	db $d3			;Wait 20
	db $30			;Note C-5
	db $c7			;Wait 8
	db $32			;Note D-5
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_084:
	db $0e			;Note D-2
	db $6f			;Volume 14
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $10			;Note E-2
	db $cb			;Wait 12
	db $11			;Note F-2
	db $d3			;Wait 20
	db $12			;Note F#2
	db $c7			;Wait 8
	db $12			;Note F#2
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_085:
	db $1a			;Note D-3
	db $67			;Volume 6
	db $7f			;Instrument 14
	db $a9,$10			; SCC Set Waveform 6
	db $c1			;Wait 2
	db $1c			;Note E-3
					;Wait Repeat 2
	db $1e			;Note F#3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $1e			;Note F#3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $23			;Note B-3
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $23			;Note B-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $28			;Note E-4
					;Wait Repeat 2
	db $2a			;Note F#4
					;Wait Repeat 2
	db $28			;Note E-4
					;Wait Repeat 2
	db $2a			;Note F#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $34			;Note E-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $34			;Note E-5
					;Wait Repeat 2
	db $36			;Note F#5
					;Wait Repeat 2
	db $37			;Note G-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_086:
	db $2d			;Note A-4
	db $6c			;Volume 11
	db $79			;Instrument 8
	db $a9,$50			; SCC Set Waveform 25
	db $cf			;Wait 16
	db $2f			;Note B-4
	db $80			;Instrument 15
	db $cb			;Wait 12
	db $30			;Note C-5
	db $c7			;Wait 8
	db $94,$44			;CMD Vibrato
	db $cb			;Wait 12
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c7			;Wait 8
	db $32			;Note D-5
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_087:
	db $29			;Note F-4
	db $6c			;Volume 11
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $2b			;Note G-4
	db $cb			;Wait 12
	db $2d			;Note A-4
	db $c7			;Wait 8
	db $94,$44			;CMD Vibrato
	db $cb			;Wait 12
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c7			;Wait 8
	db $2d			;Note A-4
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_088:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_089:
	db $32			;Note D-5
	db $6b			;Volume 10
	db $84			;Instrument 19
	db $c3			;Wait 4
	db $34			;Note E-5
	db $c7			;Wait 8
	db $94,$26			;CMD Vibrato
	db $cb			;Wait 12
	db $35			;Note F-5
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $34			;Note E-5
					;Wait Repeat 4
	db $32			;Note D-5
	db $cb			;Wait 12
	db $94,$26			;CMD Vibrato
	db $d3			;Wait 20
	db $bf			;[End-Of-Track]
.track_092:
	db $07			;Note G-1
	db $6e			;Volume 13
	db $7a			;Instrument 9
	db $c3			;Wait 4
	db $07			;Note G-1
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $07			;Note G-1
	db $c7			;Wait 8
	db $07			;Note G-1
					;Wait Repeat 8
	db $13			;Note G-2
	db $c3			;Wait 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $0e			;Note D-2
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $13			;Note G-2
					;Wait Repeat 4
	db $0e			;Note D-2
					;Wait Repeat 4
	db $0d			;Note C#2
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_093:
	db $2b			;Note G-4
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $a9,$30			; SCC Set Waveform 17
	db $c7			;Wait 8
	db $94,$46			;CMD Vibrato
	db $ca			;Wait 11
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $7a			;Instrument 9
	db $a9,$30			; SCC Set Waveform 17
	db $c3			;Wait 4
	db $2b			;Note G-4
	db $c3			;Wait 4
	db $2b			;Note G-4
	db $79			;Instrument 8
	db $a9,$30			; SCC Set Waveform 17
	db $cb			;Wait 12
	db $94,$46			;CMD Vibrato
	db $d6			;Wait 23
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_094:
	db $34			;Note E-5
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $a9,$50			; SCC Set Waveform 25
	db $c7			;Wait 8
	db $94,$46			;CMD Vibrato
	db $ca			;Wait 11
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $35			;Note F-5
	db $7a			;Instrument 9
	db $a9,$50			; SCC Set Waveform 25
	db $c3			;Wait 4
	db $34			;Note E-5
	db $c3			;Wait 4
	db $32			;Note D-5
	db $79			;Instrument 8
	db $a9,$50			; SCC Set Waveform 25
	db $cb			;Wait 12
	db $94,$46			;CMD Vibrato
	db $d6			;Wait 23
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_095:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $c7			;Wait 8
	db $94,$46			;CMD Vibrato
	db $cb			;Wait 12
	db $32			;Note D-5
	db $7a			;Instrument 9
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $30			;Note C-5
	db $c3			;Wait 4
	db $2f			;Note B-4
	db $79			;Instrument 8
	db $cb			;Wait 12
	db $94,$46			;CMD Vibrato
	db $d6			;Wait 23
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_096:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_097:
	db $0e			;Note D-2
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $16			;Note A#2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_100:
	db $0c			;Note C-2
	db $6e			;Volume 13
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $0c			;Note C-2
	db $cf			;Wait 16
	db $0c			;Note C-2
					;Wait Repeat 16
	db $0c			;Note C-2
	db $c7			;Wait 8
	db $0c			;Note C-2
	db $c3			;Wait 4
	db $0e			;Note D-2
					;Wait Repeat 4
	db $10			;Note E-2
					;Wait Repeat 4
	db $0c			;Note C-2
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_101:
	db $13			;Note G-2
	db $67			;Volume 6
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $16			;Note A#2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3e			;Note D-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_102:
	db $30			;Note C-5
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d4			;Wait 21
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2e			;Note A#4
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_103:
	db $2b			;Note G-4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d4			;Wait 21
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $29			;Note F-4
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_104:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_105:
	db $3e			;Note D-6
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_109:
	db $11			;Note F-2
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $24			;Note C-4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $30			;Note C-5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $37			;Note G-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_110:
	db $2d			;Note A-4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_111:
	db $29			;Note F-4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $29			;Note F-4
					;Wait Repeat 4
	db $28			;Note E-4
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_113:
	db $37			;Note G-5
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $16			;Note A#2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_117:
	db $13			;Note G-2
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $18			;Note C-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
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
	db $37			;Note G-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $16			;Note A#2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3e			;Note D-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_126:
	db $2d			;Note A-4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2d			;Note A-4
					;Wait Repeat 4
	db $2b			;Note G-4
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_127:
	db $29			;Note F-4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $29			;Note F-4
					;Wait Repeat 4
	db $28			;Note E-4
	db $c2			;Wait 3
	db $94,$35			;CMD Vibrato
	db $e0			;Wait 33
	db $bf			;[End-Of-Track]
.track_129:
	db $37			;Note G-5
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3f			;Note D#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3f			;Note D#6
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $19			;Note C#3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $25			;Note C#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $31			;Note C#5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $38			;Note G#5
					;Wait Repeat 2
	db $3d			;Note C#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $44			;Note G#6
					;Wait Repeat 2
	db $49			;Note C#7
					;Wait Repeat 2
	db $44			;Note G#6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_132:
	db $0f			;Note D#2
	db $6e			;Volume 13
	db $7a			;Instrument 9
	db $c7			;Wait 8
	db $0f			;Note D#2
	db $cf			;Wait 16
	db $0f			;Note D#2
					;Wait Repeat 16
	db $0f			;Note D#2
					;Wait Repeat 16
	db $10			;Note E-2
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_133:
	db $16			;Note A#2
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $3f			;Note D#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $46			;Note A#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3f			;Note D#6
					;Wait Repeat 2
	db $3a			;Note A#5
					;Wait Repeat 2
	db $14			;Note G#2
					;Wait Repeat 2
	db $19			;Note C#3
					;Wait Repeat 2
	db $1d			;Note F-3
					;Wait Repeat 2
	db $20			;Note G#3
					;Wait Repeat 2
	db $25			;Note C#4
					;Wait Repeat 2
	db $29			;Note F-4
					;Wait Repeat 2
	db $2c			;Note G#4
					;Wait Repeat 2
	db $31			;Note C#5
					;Wait Repeat 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $38			;Note G#5
					;Wait Repeat 2
	db $3d			;Note C#6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $44			;Note G#6
					;Wait Repeat 2
	db $49			;Note C#7
					;Wait Repeat 2
	db $44			;Note G#6
					;Wait Repeat 2
	db $41			;Note F-6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_134:
	db $33			;Note D#5
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $d4			;Wait 21
	db $33			;Note D#5
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $31			;Note C#5
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $d8			;Wait 25
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_135:
	db $2e			;Note A#4
	db $6c			;Volume 11
	db $82			;Instrument 17
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $d4			;Wait 21
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $c3			;Wait 4
	db $2c			;Note G#4
	db $c2			;Wait 3
	db $94,$45			;CMD Vibrato
	db $d8			;Wait 25
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_136:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_137:
	db $41			;Note F-6
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $0f			;Note D#2
					;Wait Repeat 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $45			;Note A-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $35			;Note F-5
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
	db $15			;Note A-2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_140:
	db $11			;Note F-2
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $cb			;Wait 12
	db $13			;Note G-2
	db $e3			;Wait 36
	db $13			;Note G-2
	db $c3			;Wait 4
	db $18			;Note C-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_141:
	db $0f			;Note D#2
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $13			;Note G-2
					;Wait Repeat 2
	db $16			;Note A#2
					;Wait Repeat 2
	db $1b			;Note D#3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $22			;Note A#3
					;Wait Repeat 2
	db $27			;Note D#4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
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
	db $35			;Note F-5
					;Wait Repeat 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $45			;Note A-6
					;Wait Repeat 2
	db $41			;Note F-6
					;Wait Repeat 2
	db $3c			;Note C-6
					;Wait Repeat 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $35			;Note F-5
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
	db $15			;Note A-2
					;Wait Repeat 2
	db $11			;Note F-2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_142:
	db $2e			;Note A#4
	db $82			;Instrument 17
	db $a9,$50			; SCC Set Waveform 25
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $f0			;Wait 49
	db $bf			;[End-Of-Track]
.track_143:
	db $2b			;Note G-4
	db $82			;Instrument 17
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $f0			;Wait 49
	db $bf			;[End-Of-Track]
.track_144:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 4
	db $13			;Note G-2
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
					;Wait Repeat 4
	db $21			;Note A-3
	db $6d			;Volume 12
					;Wait Repeat 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $21			;Note A-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_145:
	db $11			;Note F-2
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $11			;Note F-2
					;Wait Repeat 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
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
	db $32			;Note D-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $2b			;Note G-4
					;Wait Repeat 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $23			;Note B-3
					;Wait Repeat 2
	db $1f			;Note G-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $17			;Note B-2
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_148:
	db $13			;Note G-2
	db $7a			;Instrument 9
	db $cb			;Wait 12
	db $15			;Note A-2
	db $f3			;Wait 52
	db $bf			;[End-Of-Track]
.track_149:
	db $11			;Note F-2
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $15			;Note A-2
					;Wait Repeat 2
	db $18			;Note C-3
					;Wait Repeat 2
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
	db $32			;Note D-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $43			;Note G-6
					;Wait Repeat 2
	db $3e			;Note D-6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $37			;Note G-5
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $70			;Volume 15
	db $74			;Instrument 3
	db $c3			;Wait 4
	db $28			;Note E-4
					;Wait Repeat 4
	db $24			;Note C-4
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_150:
	db $30			;Note C-5
	db $82			;Instrument 17
	db $a9,$50			; SCC Set Waveform 25
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $32			;Note D-5
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $35			;Note F-5
	db $69			;Volume 8
	db $83			;Instrument 18
	db $c0			;Wait 1
	db $3e			;Note D-6
	db $90,$05			;CMD Portamento tone
	db $c9			;Wait 10
	db $94,$35			;CMD Vibrato
	db $d4			;Wait 21
	db $bf			;[End-Of-Track]
.track_151:
	db $2d			;Note A-4
	db $82			;Instrument 17
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $c8			;Wait 9
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$25			;CMD Vibrato
	db $d0			;Wait 17
	db $32			;Note D-5
	db $69			;Volume 8
	db $7f			;Instrument 14
	db $c0			;Wait 1
	db $3b			;Note B-5
	db $83			;Instrument 18
	db $90,$05			;CMD Portamento tone
	db $c9			;Wait 10
	db $94,$35			;CMD Vibrato
	db $d4			;Wait 21
	db $bf			;[End-Of-Track]

