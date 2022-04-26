; Song: KVII- Puzzle Stage              
; By:   Konami (Arr. by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	db $04					; Initial song speed.
	dw .waveform_start			; Start of the waveform data.
	dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
.restart:
	dw .track_000, .track_001, .track_002, .track_002, .track_004, .track_005, .track_006, .track_007	; Step:000 Pattern:000
	dw .track_008, .track_009, .track_010, .track_010, .track_012, .track_010, .track_010, .track_010	; Step:001 Pattern:001
	dw .track_008, .track_017, .track_010, .track_010, .track_020, .track_021, .track_022, .track_023	; Step:002 Pattern:002
	dw .track_024, .track_025, .track_010, .track_010, .track_028, .track_029, .track_030, .track_031	; Step:003 Pattern:003
	dw .track_032, .track_033, .track_034, .track_034, .track_036, .track_037, .track_038, .track_039	; Step:004 Pattern:004
	dw .track_200, .track_201, .track_201, .track_201, .track_204, .track_205, .track_206, .track_207	; Step:005 Pattern:025
	dw .track_040, .track_041, .track_010, .track_010, .track_044, .track_045, .track_046, .track_047	; Step:006 Pattern:005
	dw .track_048, .track_049, .track_010, .track_010, .track_052, .track_053, .track_054, .track_055	; Step:007 Pattern:006
	dw .track_000, .track_001, .track_002, .track_002, .track_004, .track_005, .track_006, .track_007	; Step:008 Pattern:000
	dw .track_008, .track_009, .track_010, .track_010, .track_012, .track_010, .track_010, .track_010	; Step:009 Pattern:001
	dw .track_008, .track_017, .track_010, .track_010, .track_020, .track_021, .track_022, .track_023	; Step:010 Pattern:002
	dw .track_024, .track_025, .track_010, .track_010, .track_028, .track_029, .track_030, .track_031	; Step:011 Pattern:003
	dw .track_032, .track_033, .track_034, .track_034, .track_036, .track_037, .track_038, .track_039	; Step:012 Pattern:004
	dw .track_200, .track_201, .track_201, .track_201, .track_204, .track_205, .track_206, .track_207	; Step:013 Pattern:025
	dw .track_056, .track_057, .track_010, .track_010, .track_060, .track_061, .track_062, .track_063	; Step:014 Pattern:007
	dw .track_064, .track_065, .track_010, .track_010, .track_068, .track_053, .track_054, .track_055	; Step:015 Pattern:008
	dw .track_008, .track_073, .track_010, .track_010, .track_076, .track_077, .track_078, .track_079	; Step:016 Pattern:009
	dw .track_080, .track_081, .track_010, .track_010, .track_084, .track_085, .track_086, .track_087	; Step:017 Pattern:010
	dw .track_008, .track_017, .track_010, .track_010, .track_092, .track_093, .track_094, .track_095	; Step:018 Pattern:011
	dw .track_096, .track_081, .track_010, .track_010, .track_084, .track_101, .track_102, .track_103	; Step:019 Pattern:012
	dw .track_104, .track_105, .track_010, .track_010, .track_108, .track_109, .track_110, .track_111	; Step:020 Pattern:013
	dw .track_104, .track_105, .track_010, .track_010, .track_108, .track_117, .track_118, .track_119	; Step:021 Pattern:014
	dw .track_104, .track_121, .track_010, .track_010, .track_124, .track_125, .track_126, .track_127	; Step:022 Pattern:015
	dw .track_080, .track_129, .track_010, .track_010, .track_132, .track_133, .track_134, .track_135	; Step:023 Pattern:016
	dw .track_136, .track_137, .track_010, .track_010, .track_140, .track_141, .track_142, .track_143	; Step:024 Pattern:017
	dw .track_136, .track_145, .track_010, .track_010, .track_148, .track_149, .track_150, .track_151	; Step:025 Pattern:018
	dw .track_152, .track_153, .track_010, .track_010, .track_156, .track_157, .track_158, .track_159	; Step:026 Pattern:019
	dw .track_160, .track_161, .track_010, .track_010, .track_164, .track_165, .track_166, .track_167	; Step:027 Pattern:020
	dw .track_136, .track_169, .track_010, .track_010, .track_140, .track_173, .track_142, .track_143	; Step:028 Pattern:021
	dw .track_136, .track_145, .track_010, .track_010, .track_148, .track_149, .track_150, .track_151	; Step:029 Pattern:022
	dw .track_184, .track_185, .track_010, .track_010, .track_156, .track_189, .track_158, .track_159	; Step:030 Pattern:023
	dw .track_192, .track_193, .track_010, .track_010, .track_164, .track_197, .track_198, .track_199	; Step:031 Pattern:024
	dw 0x0000, .restart				; End of sequence delimiter + restart address.

; [ Custom FM voices ]
.customvoice_start:

; [ SCC Waveforms ]
.waveform_start:
	db $80, $b0, $c0, $10, $1a, $2a, $2c, $1a, $00, $e0, $d0, $e0, $22, $53, $70, $75, $70, $31, $ea, $80, $88, $8a, $8c, $8e, $00, $7f, $75, $73, $62, $00, $c0, $90				; Waveform:1
	db $00, $40, $7f, $40, $01, $c0, $81, $c0, $01, $40, $7f, $40, $01, $c0, $01, $40, $01, $e0, $01, $20, $01, $f0, $01, $10, $01, $ff, $ff, $ff, $ff, $40, $40, $40				; Waveform:2
	db $a0, $90, $a0, $ac, $f0, $a0, $16, $00, $90, $00, $20, $40, $60, $50, $38, $1c, $f2, $e2, $d6, $d0, $d0, $da, $dc, $e0, $e2, $e2, $e2, $e0, $dc, $d8, $d0, $b0				; Waveform:7
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $80, $90, $a0, $b0, $c0, $d0, $e0, $f0, $00, $10, $20, $30, $40, $50, $60, $70				; Waveform:8
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $00, $e0, $c0, $a0, $80, $a0, $c0, $e0, $00, $20, $40, $60, $7f, $60, $40, $20				; Waveform:12
	db $30, $50, $50, $30, $00, $00, $10, $40, $60, $70, $60, $30, $f0, $e0, $e0, $00, $20, $20, $10, $c0, $a0, $90, $a0, $c0, $00, $00, $d0, $b0, $b0, $d0, $00, $00				; Waveform:13
	db $00, $00, $00, $00, $00, $70, $70, $00, $00, $80, $80, $80, $00, $00, $00, $00, $70, $70, $70, $00, $80, $80, $00, $00, $00, $00, $70, $70, $00, $00, $80, $80				; Waveform:15
	db $00, $68, $48, $18, $48, $68, $28, $00, $48, $77, $58, $08, $28, $38, $00, $b8, $08, $58, $00, $e8, $f8, $00, $b8, $98, $c8, $08, $e8, $a8, $c8, $f8, $c8, $a8				; Waveform:20
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $00, $e7, $cf, $b9, $a6, $96, $8b, $83, $80, $83, $8b, $96, $a6, $b9, $cf, $e7				; Waveform:21
	db $90, $88, $90, $9c, $b0, $c0, $e0, $26, $70, $7c, $70, $56, $40, $20, $10, $fc, $e0, $fc, $10, $fc, $40, $54, $72, $7f, $70, $26, $e0, $c0, $b0, $9c, $90, $88				; Waveform:30
	db $00, $4e, $62, $6d, $75, $7a, $7d, $7e, $7f, $7e, $7d, $7a, $75, $6d, $62, $4e, $00, $b1, $9d, $92, $8a, $85, $82, $81, $80, $81, $82, $85, $8a, $92, $9d, $b1				; Waveform:31

; [ FM Drum macros]
.drummacro_start:

; [ Instruments]
.instrument_start:
	dw .instrument_00				; BD G2           
	dw .instrument_01				; SD A3           
	dw .instrument_02				; HH CL A8        
	dw .instrument_03				; PSG BASS        
	dw .instrument_04				; HH OP           
	dw .instrument_05				; Short Snare     
	dw .instrument_06				; PSG flute 2     
	dw .instrument_07				; SCC BASS OUT    
	dw .instrument_08				; BASS LONG       
	dw .instrument_09				; Rombo-BASS      
	dw .instrument_10				; Flute KVII      
	dw .instrument_11				; Flute II-KV2    
	dw .instrument_12				; PSG Piano       
	dw .instrument_13				; SCC Piano       
	dw .instrument_14				; ROMBO - notes   
	dw .instrument_15				; ROMBOHIGH       
	dw .instrument_16				; CONT            

.instrument_00:					; BD G2           
	db $50						; Waveform 10
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
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
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0b						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0a						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$08						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$07						; Volume _
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

.instrument_03:					; PSG BASS        
	db $40						; Waveform 8
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
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $0c,$01						; Volume -
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
	db $00							; Volume same
	db $18,$e3						; Loop (-30)

.instrument_04:					; HH OP           
	db $10						; Waveform 2
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
								; --- Macro loop
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $0c,$01						; Volume -
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $18,$f0						; Loop (-17)

.instrument_05:					; Short Snare     
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

.instrument_06:					; PSG flute 2     
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
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

.instrument_07:					; SCC BASS OUT    
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$03						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_08:					; BASS LONG       
	db $20						; Waveform 4
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
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_09:					; Rombo-BASS      
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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

.instrument_10:					; Flute KVII      
	db $50						; Waveform 10
	db $02,$10						; Mixer (T)
	db $08,$03						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
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
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_11:					; Flute II-KV2    
	db $40						; Waveform 8
	db $02,$10						; Mixer (T)
	db $08,$03						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
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
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_12:					; PSG Piano       
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
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
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_13:					; SCC Piano       
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
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_14:					; ROMBO - notes   
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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

.instrument_15:					; ROMBOHIGH       
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_16:					; CONT            
	db $48						; Waveform 9
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $18,$fb						; Loop (-6)


; [ Song track data ]
.track_000:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $cb			;Wait 12
	db $29			;Note F-4
	db $6e			;Volume 13
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_001:
	db $10			;Note E-2
	db $6e			;Volume 13
	db $74			;Instrument 3
	db $9b			;CMD End 
	db $d1			;Wait 18
	db $13			;Note G-2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_002:
	db $70			;Volume 15
	db $71			;Instrument 0
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_004:
	db $10			;Note E-2
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $10			;Note E-2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $13			;Note G-2
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $13			;Note G-2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_005:
	db $2f			;Note B-4
	db $6e			;Volume 13
	db $7c			;Instrument 11
	db $c5			;Wait 6
	db $94,$78			;CMD Vibrato
	db $dd			;Wait 30
	db $bf			;[End-Of-Track]
.track_006:
	db $34			;Note E-5
	db $70			;Volume 15
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $dd			;Wait 30
	db $bf			;[End-Of-Track]
.track_007:
	db $60			;Release 96
	db $c5			;Wait 6
	db $34			;Note E-5
	db $6a			;Volume 9
	db $7b			;Instrument 10
	db $a1,$0a			;CMD Track detune
					;Wait Repeat 6
	db $94,$26			;CMD Vibrato
	db $d7			;Wait 24
	db $bf			;[End-Of-Track]
.track_008:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $29			;Note F-4
	db $73			;Instrument 2
	db $cb			;Wait 12
	db $29			;Note F-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_009:
	db $12			;Note F#2
	db $6e			;Volume 13
	db $74			;Instrument 3
	db $d1			;Wait 18
	db $17			;Note B-2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_010:
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_012:
	db $12			;Note F#2
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $11			;Note F-2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $17			;Note B-2
	db $79			;Instrument 8
	db $cf			;Wait 16
	db $17			;Note B-2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_017:
	db $16			;Note A#2
	db $74			;Instrument 3
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_020:
	db $16			;Note A#2
	db $79			;Instrument 8
	db $e1			;Wait 34
	db $16			;Note A#2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_021:
	db $2e			;Note A#4
	db $6e			;Volume 13
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$57			;CMD Vibrato
	db $dd			;Wait 30
	db $bf			;[End-Of-Track]
.track_022:
	db $37			;Note G-5
	db $70			;Volume 15
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$26			;CMD Vibrato
	db $dd			;Wait 30
	db $bf			;[End-Of-Track]
.track_023:
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $37			;Note G-5
	db $6a			;Volume 9
	db $7b			;Instrument 10
	db $9b			;CMD End 
					;Wait Repeat 6
	db $94,$26			;CMD Vibrato
	db $d7			;Wait 24
	db $bf			;[End-Of-Track]
.track_024:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $29			;Note F-4
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c8			;Wait 9
	db $29			;Note F-4
	db $6d			;Volume 12
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_025:
	db $17			;Note B-2
	db $74			;Instrument 3
	db $c5			;Wait 6
	db $6c			;Volume 11
	db $c1			;Wait 2
	db $6b			;Volume 10
	db $c9			;Wait 10
	db $24			;Note C-4
	db $69			;Volume 8
	db $77			;Instrument 6
	db $a1,$00			;CMD Track detune
	db $cc			;Wait 13
	db $23			;Note B-3
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_028:
	db $17			;Note B-2
	db $70			;Volume 15
	db $79			;Instrument 8
	db $c5			;Wait 6
	db $6f			;Volume 14
	db $c1			;Wait 2
	db $6e			;Volume 13
					;Wait Repeat 2
	db $6d			;Volume 12
	db $c7			;Wait 8
	db $13			;Note G-2
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c8			;Wait 9
	db $12			;Note F#2
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_029:
	db $2f			;Note B-4
	db $6e			;Volume 13
	db $7c			;Instrument 11
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $66			;Volume 5
	db $cb			;Wait 12
	db $24			;Note C-4
	db $69			;Volume 8
	db $7f			;Instrument 14
	db $a9,$08			; SCC Set Waveform 2
	db $c8			;Wait 9
	db $23			;Note B-3
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_030:
	db $36			;Note F#5
	db $70			;Volume 15
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $66			;Volume 5
	db $cb			;Wait 12
	db $28			;Note E-4
	db $69			;Volume 8
	db $7f			;Instrument 14
	db $a9,$30			; SCC Set Waveform 15
	db $c8			;Wait 9
	db $27			;Note D#4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_031:
	db $c5			;Wait 6
	db $36			;Note F#5
	db $65			;Volume 4
	db $81			;Instrument 16
	db $9b			;CMD End 
	db $cb			;Wait 12
	db $28			;Note E-4
	db $67			;Volume 6
	db $a1,$02			;CMD Track detune
	db $cc			;Wait 13
	db $27			;Note D#4
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_032:
	db $29			;Note F-4
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c8			;Wait 9
	db $29			;Note F-4
	db $c8			;Wait 9
	db $29			;Note F-4
	db $6e			;Volume 13
					;Wait Repeat 9
	db $29			;Note F-4
					;Wait Repeat 9
	db $29			;Note F-4
	db $c8			;Wait 9
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 9
	db $21			;Note A-3
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_033:
	db $c3			;Wait 4
	db $28			;Note E-4
	db $6a			;Volume 9
	db $77			;Instrument 6
	db $c8			;Wait 9
	db $27			;Note D#4
	db $c8			;Wait 9
	db $2d			;Note A-4
	db $6b			;Volume 10
					;Wait Repeat 9
	db $2c			;Note G#4
	db $c8			;Wait 9
	db $30			;Note C-5
	db $6c			;Volume 11
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $34			;Note E-5
	db $6b			;Volume 10
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_034:
	db $fe			;Wait 63
	db $bf			;[End-Of-Track]
.track_036:
	db $17			;Note B-2
	db $7a			;Instrument 9
	db $c8			;Wait 9
	db $16			;Note A#2
					;Wait Repeat 9
	db $1c			;Note E-3
					;Wait Repeat 9
	db $1b			;Note D#3
					;Wait Repeat 9
	db $1f			;Note G-3
					;Wait Repeat 9
	db $1e			;Note F#3
					;Wait Repeat 9
	db $23			;Note B-3
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_037:
	db $28			;Note E-4
	db $7f			;Instrument 14
	db $c8			;Wait 9
	db $27			;Note D#4
	db $c8			;Wait 9
	db $2d			;Note A-4
	db $6a			;Volume 9
					;Wait Repeat 9
	db $2c			;Note G#4
	db $c8			;Wait 9
	db $30			;Note C-5
	db $6b			;Volume 10
					;Wait Repeat 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $34			;Note E-5
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_038:
	db $2b			;Note G-4
	db $7f			;Instrument 14
	db $a9,$30			; SCC Set Waveform 15
	db $c8			;Wait 9
	db $2a			;Note F#4
	db $c8			;Wait 9
	db $30			;Note C-5
	db $6a			;Volume 9
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $34			;Note E-5
	db $6b			;Volume 10
					;Wait Repeat 9
	db $33			;Note D#5
					;Wait Repeat 9
	db $37			;Note G-5
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_039:
	db $c3			;Wait 4
	db $2b			;Note G-4
	db $c8			;Wait 9
	db $2a			;Note F#4
					;Wait Repeat 9
	db $30			;Note C-5
					;Wait Repeat 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $34			;Note E-5
					;Wait Repeat 9
	db $33			;Note D#5
					;Wait Repeat 9
	db $37			;Note G-5
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_040:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d7			;Wait 24
	db $29			;Note F-4
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $37			;Note G-5
	db $75			;Instrument 4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_041:
	db $18			;Note C-3
	db $6d			;Volume 12
	db $74			;Instrument 3
	db $a1,$04			;CMD Track detune
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_044:
	db $18			;Note C-3
	db $79			;Instrument 8
	db $e1			;Wait 34
	db $18			;Note C-3
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_045:
	db $39			;Note A-5
	db $6e			;Volume 13
	db $7c			;Instrument 11
	db $c5			;Wait 6
	db $94,$37			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_046:
	db $3c			;Note C-6
	db $6e			;Volume 13
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$26			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_047:
	db $36			;Note F#5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$16			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_048:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $d1			;Wait 18
	db $29			;Note F-4
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $1f			;Note G-3
	db $6c			;Volume 11
	db $76			;Instrument 5
					;Wait Repeat 6
	db $1f			;Note G-3
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_049:
	db $17			;Note B-2
	db $6d			;Volume 12
	db $74			;Instrument 3
	db $a1,$04			;CMD Track detune
	db $c7			;Wait 8
	db $6b			;Volume 10
	db $c0			;Wait 1
	db $92,$20			;CMD Portamento down
					;Wait Repeat 1
	db $6a			;Volume 9
					;Wait Repeat 1
	db $69			;Volume 8
					;Wait Repeat 1
	db $68			;Volume 7
	db $d7			;Wait 24
	db $bf			;[End-Of-Track]
.track_052:
	db $17			;Note B-2
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c8			;Wait 9
	db $92,$20			;CMD Portamento down
	db $c1			;Wait 2
	db $6a			;Volume 9
	db $c0			;Wait 1
	db $69			;Volume 8
	db $c5			;Wait 6
	db $60			;Release 96
	db $9b			;CMD End 
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_053:
	db $36			;Note F#5
	db $6e			;Volume 13
	db $7c			;Instrument 11
	db $c8			;Wait 9
	db $35			;Note F-5
	db $67			;Volume 6
	db $81			;Instrument 16
	db $a9,$40			; SCC Set Waveform 21
	db $c1			;Wait 2
	db $34			;Note E-5
	db $66			;Volume 5
	db $c0			;Wait 1
	db $33			;Note D#5
	db $65			;Volume 4
					;Wait Repeat 1
	db $32			;Note D-5
	db $64			;Volume 3
					;Wait Repeat 1
	db $31			;Note C#5
	db $63			;Volume 2
					;Wait Repeat 1
	db $30			;Note C-5
	db $62			;Volume 1
	db $c1			;Wait 2
	db $60			;Release 96
	db $d2			;Wait 19
	db $bf			;[End-Of-Track]
.track_054:
	db $3b			;Note B-5
	db $68			;Volume 7
	db $7b			;Instrument 10
	db $c8			;Wait 9
	db $3a			;Note A#5
	db $64			;Volume 3
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 31
	db $c1			;Wait 2
	db $39			;Note A-5
	db $c0			;Wait 1
	db $38			;Note G#5
	db $63			;Volume 2
					;Wait Repeat 1
	db $37			;Note G-5
					;Wait Repeat 1
	db $36			;Note F#5
	db $c0			;Wait 1
	db $35			;Note F-5
	db $62			;Volume 1
	db $c1			;Wait 2
	db $60			;Release 96
	db $d2			;Wait 19
	db $bf			;[End-Of-Track]
.track_055:
	db $3b			;Note B-5
	db $68			;Volume 7
	db $7b			;Instrument 10
	db $a1,$08			;CMD Track detune
	db $c8			;Wait 9
	db $3a			;Note A#5
	db $64			;Volume 3
	db $81			;Instrument 16
	db $a9,$50			; SCC Set Waveform 31
	db $c1			;Wait 2
	db $39			;Note A-5
	db $c0			;Wait 1
	db $38			;Note G#5
	db $63			;Volume 2
					;Wait Repeat 1
	db $37			;Note G-5
					;Wait Repeat 1
	db $36			;Note F#5
	db $c0			;Wait 1
	db $35			;Note F-5
	db $62			;Volume 1
	db $c1			;Wait 2
	db $60			;Release 96
	db $d2			;Wait 19
	db $bf			;[End-Of-Track]
.track_056:
	db $37			;Note G-5
	db $6d			;Volume 12
	db $75			;Instrument 4
	db $d7			;Wait 24
	db $29			;Note F-4
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $37			;Note G-5
	db $6d			;Volume 12
	db $75			;Instrument 4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_057:
	db $16			;Note A#2
	db $6d			;Volume 12
	db $74			;Instrument 3
	db $a1,$04			;CMD Track detune
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_060:
	db $16			;Note A#2
	db $79			;Instrument 8
	db $e1			;Wait 34
	db $16			;Note A#2
	db $70			;Volume 15
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_061:
	db $34			;Note E-5
	db $6e			;Volume 13
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$37			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_062:
	db $3a			;Note A#5
	db $6e			;Volume 13
	db $7b			;Instrument 10
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_063:
	db $37			;Note G-5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $a1,$00			;CMD Track detune
	db $c5			;Wait 6
	db $94,$16			;CMD Vibrato
	db $dc			;Wait 29
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_064:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $d1			;Wait 18
	db $29			;Note F-4
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $29			;Note F-4
	db $6d			;Volume 12
					;Wait Repeat 6
	db $1f			;Note G-3
	db $6c			;Volume 11
	db $76			;Instrument 5
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_065:
	db $17			;Note B-2
	db $6d			;Volume 12
	db $74			;Instrument 3
	db $a1,$04			;CMD Track detune
	db $c4			;Wait 5
	db $6c			;Volume 11
	db $c0			;Wait 1
	db $92,$30			;CMD Portamento down
					;Wait Repeat 1
	db $6b			;Volume 10
	db $c1			;Wait 2
	db $6a			;Volume 9
					;Wait Repeat 2
	db $69			;Volume 8
	db $c0			;Wait 1
	db $68			;Volume 7
	db $d7			;Wait 24
	db $bf			;[End-Of-Track]
.track_068:
	db $17			;Note B-2
	db $70			;Volume 15
	db $79			;Instrument 8
	db $c4			;Wait 5
	db $6f			;Volume 14
	db $c0			;Wait 1
	db $92,$30			;CMD Portamento down
					;Wait Repeat 1
	db $6e			;Volume 13
	db $c1			;Wait 2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $6c			;Volume 11
	db $c0			;Wait 1
	db $6b			;Volume 10
	db $c5			;Wait 6
	db $60			;Release 96
	db $9b			;CMD End 
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_073:
	db $18			;Note C-3
	db $6e			;Volume 13
	db $74			;Instrument 3
	db $9b			;CMD End 
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_076:
	db $18			;Note C-3
	db $70			;Volume 15
	db $79			;Instrument 8
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_077:
	db $38			;Note G#5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$57			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_078:
	db $3c			;Note C-6
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_079:
	db $3c			;Note C-6
	db $69			;Volume 8
	db $7b			;Instrument 10
	db $a1,$08			;CMD Track detune
	db $c2			;Wait 3
	db $94,$16			;CMD Vibrato
					;Wait Repeat 3
	db $67			;Volume 6
	db $c5			;Wait 6
	db $66			;Volume 5
					;Wait Repeat 6
	db $65			;Volume 4
	db $d0			;Wait 17
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_080:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $29			;Note F-4
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $29			;Note F-4
					;Wait Repeat 6
	db $29			;Note F-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_081:
	db $17			;Note B-2
	db $74			;Instrument 3
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_084:
	db $17			;Note B-2
	db $70			;Volume 15
	db $79			;Instrument 8
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_085:
	db $37			;Note G-5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$57			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_086:
	db $3b			;Note B-5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_087:
	db $c3			;Wait 4
	db $3b			;Note B-5
	db $68			;Volume 7
	db $7b			;Instrument 10
	db $a1,$08			;CMD Track detune
	db $c2			;Wait 3
	db $94,$16			;CMD Vibrato
					;Wait Repeat 3
	db $67			;Volume 6
	db $c5			;Wait 6
	db $66			;Volume 5
					;Wait Repeat 6
	db $65			;Volume 4
	db $cd			;Wait 14
	db $bf			;[End-Of-Track]
.track_092:
	db $16			;Note A#2
	db $70			;Volume 15
	db $79			;Instrument 8
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_093:
	db $36			;Note F#5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$57			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_094:
	db $3a			;Note A#5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_095:
	db $c3			;Wait 4
	db $3a			;Note A#5
	db $68			;Volume 7
	db $7b			;Instrument 10
	db $a1,$08			;CMD Track detune
	db $c2			;Wait 3
	db $94,$16			;CMD Vibrato
					;Wait Repeat 3
	db $67			;Volume 6
	db $c5			;Wait 6
	db $66			;Volume 5
					;Wait Repeat 6
	db $65			;Volume 4
	db $cd			;Wait 14
	db $bf			;[End-Of-Track]
.track_096:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $29			;Note F-4
	db $73			;Instrument 2
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $29			;Note F-4
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_101:
	db $37			;Note G-5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$57			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c4			;Wait 5
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_102:
	db $3b			;Note B-5
	db $6d			;Volume 12
	db $7b			;Instrument 10
	db $9b			;CMD End 
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $6c			;Volume 11
	db $c5			;Wait 6
	db $6b			;Volume 10
					;Wait Repeat 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
	db $c4			;Wait 5
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_103:
	db $c3			;Wait 4
	db $3b			;Note B-5
	db $68			;Volume 7
	db $7b			;Instrument 10
	db $a1,$08			;CMD Track detune
	db $c2			;Wait 3
	db $94,$16			;CMD Vibrato
					;Wait Repeat 3
	db $67			;Volume 6
	db $c5			;Wait 6
	db $66			;Volume 5
					;Wait Repeat 6
	db $65			;Volume 4
	db $cc			;Wait 13
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_104:
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $29			;Note F-4
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $29			;Note F-4
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $29			;Note F-4
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_105:
	db $24			;Note C-4
	db $6b			;Volume 10
	db $77			;Instrument 6
	db $c8			;Wait 9
	db $23			;Note B-3
					;Wait Repeat 9
	db $22			;Note A#3
					;Wait Repeat 9
	db $23			;Note B-3
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_108:
	db $18			;Note C-3
	db $79			;Instrument 8
	db $c8			;Wait 9
	db $17			;Note B-2
					;Wait Repeat 9
	db $16			;Note A#2
					;Wait Repeat 9
	db $17			;Note B-2
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_109:
	db $2d			;Note A-4
	db $6d			;Volume 12
	db $80			;Instrument 15
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $2a			;Note F#4
					;Wait Repeat 9
	db $28			;Note E-4
					;Wait Repeat 9
	db $27			;Note D#4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_110:
	db $30			;Note C-5
	db $6d			;Volume 12
	db $7f			;Instrument 14
	db $a9,$08			; SCC Set Waveform 2
	db $c8			;Wait 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $2e			;Note A#4
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_111:
	db $30			;Note C-5
	db $67			;Volume 6
	db $80			;Instrument 15
	db $a1,$09			;CMD Track detune
	db $cc			;Wait 13
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $2e			;Note A#4
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_117:
	db $2d			;Note A-4
	db $c8			;Wait 9
	db $2a			;Note F#4
					;Wait Repeat 9
	db $28			;Note E-4
					;Wait Repeat 9
	db $27			;Note D#4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_118:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7f			;Instrument 14
	db $a9,$08			; SCC Set Waveform 2
	db $c8			;Wait 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $2e			;Note A#4
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_119:
	db $c3			;Wait 4
	db $30			;Note C-5
	db $c8			;Wait 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $2e			;Note A#4
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_121:
	db $24			;Note C-4
	db $6b			;Volume 10
	db $77			;Instrument 6
	db $c8			;Wait 9
	db $23			;Note B-3
					;Wait Repeat 9
	db $24			;Note C-4
					;Wait Repeat 9
	db $23			;Note B-3
	db $c6			;Wait 7
	db $23			;Note B-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_124:
	db $18			;Note C-3
	db $79			;Instrument 8
	db $c8			;Wait 9
	db $17			;Note B-2
					;Wait Repeat 9
	db $18			;Note C-3
					;Wait Repeat 9
	db $17			;Note B-2
	db $c6			;Wait 7
	db $17			;Note B-2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_125:
	db $2d			;Note A-4
	db $c8			;Wait 9
	db $2a			;Note F#4
					;Wait Repeat 9
	db $2d			;Note A-4
					;Wait Repeat 9
	db $2a			;Note F#4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_126:
	db $30			;Note C-5
	db $6b			;Volume 10
	db $7f			;Instrument 14
	db $a9,$08			; SCC Set Waveform 2
	db $c8			;Wait 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $30			;Note C-5
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_127:
	db $c3			;Wait 4
	db $30			;Note C-5
	db $c8			;Wait 9
	db $2f			;Note B-4
					;Wait Repeat 9
	db $30			;Note C-5
					;Wait Repeat 9
	db $2f			;Note B-4
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_129:
	db $22			;Note A#3
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $e3			;Wait 36
	db $bf			;[End-Of-Track]
.track_132:
	db $16			;Note A#2
	db $70			;Volume 15
	db $79			;Instrument 8
	db $e1			;Wait 34
	db $16			;Note A#2
	db $78			;Instrument 7
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_133:
	db $29			;Note F-4
	db $80			;Instrument 15
	db $c3			;Wait 4
	db $94,$67			;CMD Vibrato
	db $cd			;Wait 14
	db $28			;Note E-4
	db $70			;Volume 15
	db $7e			;Instrument 13
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2b			;Note G-4
					;Wait Repeat 6
	db $2d			;Note A-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_134:
	db $2e			;Note A#4
	db $6c			;Volume 11
	db $7f			;Instrument 14
	db $a9,$08			; SCC Set Waveform 2
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $6b			;Volume 10
	db $c5			;Wait 6
	db $6a			;Volume 9
					;Wait Repeat 6
	db $69			;Volume 8
					;Wait Repeat 6
	db $2d			;Note A-4
	db $68			;Volume 7
	db $c0			;Wait 1
	db $2c			;Note G#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $2a			;Note F#4
					;Wait Repeat 1
	db $29			;Note F-4
					;Wait Repeat 1
	db $28			;Note E-4
	db $bf			;[End-Of-Track]
.track_135:
	db $c3			;Wait 4
	db $2e			;Note A#4
	db $68			;Volume 7
	db $80			;Instrument 15
	db $a9,$08			; SCC Set Waveform 2
	db $c2			;Wait 3
	db $94,$36			;CMD Vibrato
	db $d6			;Wait 23
	db $2d			;Note A-4
	db $c0			;Wait 1
	db $2c			;Note G#4
					;Wait Repeat 1
	db $2b			;Note G-4
					;Wait Repeat 1
	db $2a			;Note F#4
					;Wait Repeat 1
	db $29			;Note F-4
					;Wait Repeat 1
	db $28			;Note E-4
	db $bf			;[End-Of-Track]
.track_136:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $cb			;Wait 12
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_137:
	db $2e			;Note A#4
	db $6c			;Volume 11
	db $7d			;Instrument 12
	db $a1,$01			;CMD Track detune
	db $c3			;Wait 4
	db $2f			;Note B-4
	db $c7			;Wait 8
	db $94,$36			;CMD Vibrato
	db $cd			;Wait 14
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2b			;Note G-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_140:
	db $10			;Note E-2
	db $79			;Instrument 8
	db $cb			;Wait 12
	db $10			;Note E-2
	db $c5			;Wait 6
	db $12			;Note F#2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_141:
	db $2e			;Note A#4
	db $7e			;Instrument 13
	db $c1			;Wait 2
	db $2f			;Note B-4
	db $d5			;Wait 22
	db $2d			;Note A-4
	db $c5			;Wait 6
	db $2b			;Note G-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_142:
	db $60			;Release 96
	db $9b			;CMD End 
	db $cb			;Wait 12
	db $26			;Note D-4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $a9,$18			; SCC Set Waveform 8
	db $c5			;Wait 6
	db $60			;Release 96
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_143:
	db $60			;Release 96
	db $9b			;CMD End 
	db $cb			;Wait 12
	db $1f			;Note G-3
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c5			;Wait 6
	db $60			;Release 96
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_145:
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $c7			;Wait 8
	db $94,$36			;CMD Vibrato
	db $cf			;Wait 16
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2a			;Note F#4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_148:
	db $13			;Note G-2
	db $79			;Instrument 8
	db $d1			;Wait 18
	db $10			;Note E-2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_149:
	db $2d			;Note A-4
	db $7e			;Instrument 13
	db $d7			;Wait 24
	db $2b			;Note G-4
	db $c5			;Wait 6
	db $2a			;Note F#4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_150:
	db $26			;Note D-4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $a9,$18			; SCC Set Waveform 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c2			;Wait 3
	db $66			;Volume 5
	db $c0			;Wait 1
	db $64			;Volume 3
					;Wait Repeat 1
	db $63			;Volume 2
	db $bf			;[End-Of-Track]
.track_151:
	db $1f			;Note G-3
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c2			;Wait 3
	db $66			;Volume 5
	db $c0			;Wait 1
	db $64			;Volume 3
					;Wait Repeat 1
	db $63			;Volume 2
	db $bf			;[End-Of-Track]
.track_152:
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $37			;Note G-5
	db $75			;Instrument 4
	db $cb			;Wait 12
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 12
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_153:
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $2b			;Note G-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $28			;Note E-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $26			;Note D-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_156:
	db $15			;Note A-2
	db $79			;Instrument 8
	db $cb			;Wait 12
	db $15			;Note A-2
	db $c5			;Wait 6
	db $17			;Note B-2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_157:
	db $2b			;Note G-4
	db $7e			;Instrument 13
	db $c5			;Wait 6
	db $6f			;Volume 14
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $6e			;Volume 13
					;Wait Repeat 1
	db $6d			;Volume 12
					;Wait Repeat 1
	db $6b			;Volume 10
					;Wait Repeat 1
	db $60			;Release 96
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $70			;Volume 15
	db $7e			;Instrument 13
	db $cb			;Wait 12
	db $28			;Note E-4
	db $c5			;Wait 6
	db $26			;Note D-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_158:
	db $60			;Release 96
	db $9b			;CMD End 
	db $cb			;Wait 12
	db $25			;Note C#4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $a9,$18			; SCC Set Waveform 8
	db $c5			;Wait 6
	db $60			;Release 96
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_159:
	db $60			;Release 96
	db $9b			;CMD End 
	db $cb			;Wait 12
	db $21			;Note A-3
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c5			;Wait 6
	db $60			;Release 96
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_160:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c3			;Wait 4
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $29			;Note F-4
	db $6e			;Volume 13
	db $c3			;Wait 4
	db $29			;Note F-4
	db $6e			;Volume 13
	db $9a,$02			;CMD Note delay
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]
.track_161:
	db $c1			;Wait 2
	db $28			;Note E-4
	db $c7			;Wait 8
	db $94,$36			;CMD Vibrato
	db $c9			;Wait 10
	db $28			;Note E-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2b			;Note G-4
					;Wait Repeat 6
	db $2d			;Note A-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_164:
	db $15			;Note A-2
	db $79			;Instrument 8
	db $d1			;Wait 18
	db $12			;Note F#2
	db $d1			;Wait 18
	db $bf			;[End-Of-Track]
.track_165:
	db $28			;Note E-4
	db $7e			;Instrument 13
	db $d1			;Wait 18
	db $28			;Note E-4
	db $c5			;Wait 6
	db $2b			;Note G-4
					;Wait Repeat 6
	db $2d			;Note A-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_166:
	db $25			;Note C#4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $a9,$18			; SCC Set Waveform 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c2			;Wait 3
	db $66			;Volume 5
	db $c0			;Wait 1
	db $64			;Volume 3
					;Wait Repeat 1
	db $63			;Volume 2
	db $bf			;[End-Of-Track]
.track_167:
	db $21			;Note A-3
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c2			;Wait 3
	db $66			;Volume 5
	db $c0			;Wait 1
	db $64			;Volume 3
					;Wait Repeat 1
	db $63			;Volume 2
	db $bf			;[End-Of-Track]
.track_169:
	db $2e			;Note A#4
	db $6c			;Volume 11
	db $7d			;Instrument 12
	db $a1,$01			;CMD Track detune
	db $c3			;Wait 4
	db $2f			;Note B-4
	db $c5			;Wait 6
	db $94,$36			;CMD Vibrato
	db $cf			;Wait 16
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $2b			;Note G-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_173:
	db $2e			;Note A#4
	db $7e			;Instrument 13
	db $c2			;Wait 3
	db $2f			;Note B-4
	db $d4			;Wait 21
	db $2d			;Note A-4
	db $c5			;Wait 6
	db $2b			;Note G-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_184:
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $cb			;Wait 12
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 12
	db $29			;Note F-4
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_185:
	db $c1			;Wait 2
	db $28			;Note E-4
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $28			;Note E-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c4			;Wait 5
	db $28			;Note E-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $26			;Note D-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_189:
	db $28			;Note E-4
	db $7e			;Instrument 13
	db $c5			;Wait 6
	db $6f			;Volume 14
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $6e			;Volume 13
					;Wait Repeat 1
	db $6d			;Volume 12
					;Wait Repeat 1
	db $6b			;Volume 10
					;Wait Repeat 1
	db $60			;Release 96
	db $c1			;Wait 2
	db $28			;Note E-4
	db $70			;Volume 15
	db $7e			;Instrument 13
	db $cb			;Wait 12
	db $28			;Note E-4
	db $c5			;Wait 6
	db $26			;Note D-4
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_192:
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $d1			;Wait 18
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c8			;Wait 9
	db $37			;Note G-5
	db $6e			;Volume 13
	db $75			;Instrument 4
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_193:
	db $c1			;Wait 2
	db $28			;Note E-4
	db $7d			;Instrument 12
	db $c7			;Wait 8
	db $94,$36			;CMD Vibrato
	db $d9			;Wait 26
	db $bf			;[End-Of-Track]
.track_197:
	db $28			;Note E-4
	db $7e			;Instrument 13
	db $e2			;Wait 35
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_198:
	db $25			;Note C#4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $a9,$18			; SCC Set Waveform 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c4			;Wait 5
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_199:
	db $21			;Note A-3
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $c2			;Wait 3
	db $94,$65			;CMD Vibrato
	db $da			;Wait 27
	db $68			;Volume 7
	db $c4			;Wait 5
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_200:
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_201:
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_204:
	db $22			;Note A#3
	db $7a			;Instrument 9
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_205:
	db $33			;Note D#5
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_206:
	db $36			;Note F#5
	db $7f			;Instrument 14
	db $c8			;Wait 9
	db $bf			;[End-Of-Track]
.track_207:
	db $c3			;Wait 4
	db $36			;Note F#5
	db $81			;Instrument 16
	db $c4			;Wait 5
	db $bf			;[End-Of-Track]

