; Song: TT -TRIPLETS-B50-BASSDRUM       
; By:   Gryzor87 (c)2021                
; Period table: A440 Modern

; [ Song start data ]
	db $06					; Initial song speed.
	dw .waveform_start			; Start of the waveform data.
	dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
.restart:
	dw .track_000, .track_000, .track_002, .track_002, .track_004, .track_005, .track_000, .track_000	; Step:000 Pattern:000
	dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:001 Pattern:001
	dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:002 Pattern:001
	dw .track_016, .track_017, .track_018, .track_011, .track_012, .track_021, .track_022, .track_023	; Step:003 Pattern:002
	dw .track_016, .track_017, .track_018, .track_011, .track_012, .track_021, .track_022, .track_023	; Step:004 Pattern:002
	dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:005 Pattern:001
	dw .track_024, .track_025, .track_018, .track_027, .track_028, .track_029, .track_030, .track_031	; Step:006 Pattern:003
	dw 0x0000, .restart				; End of sequence delimiter + restart address.

; [ Custom FM voices ]
.customvoice_start:

; [ SCC Waveforms ]
.waveform_start:
	db $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00				; Waveform:0
	db $80, $b0, $c0, $10, $1a, $2a, $2c, $1a, $00, $e0, $d0, $e0, $22, $53, $70, $75, $70, $31, $ea, $80, $88, $8a, $8c, $8e, $00, $7f, $75, $73, $62, $00, $c0, $90				; Waveform:1
	db $a0, $90, $a0, $ac, $f0, $a0, $16, $00, $90, $00, $20, $40, $60, $50, $38, $1c, $f2, $e2, $d6, $d0, $d0, $da, $dc, $e0, $e2, $e2, $e2, $e0, $dc, $d8, $d0, $b0				; Waveform:7
	db $00, $19, $31, $47, $5a, $6a, $75, $7d, $7f, $7d, $75, $6a, $5a, $47, $31, $19, $80, $90, $a0, $b0, $c0, $d0, $e0, $f0, $00, $10, $20, $30, $40, $50, $60, $70				; Waveform:8
	db $00, $30, $50, $60, $70, $60, $50, $30, $00, $d0, $b0, $a0, $90, $a0, $b0, $d0, $00, $40, $60, $70, $60, $40, $00, $c0, $a0, $90, $a0, $c0, $00, $70, $00, $90				; Waveform:11
	db $28, $58, $58, $28, $00, $00, $18, $48, $68, $78, $68, $38, $e8, $d8, $e0, $00, $20, $20, $10, $c0, $a0, $90, $a0, $c0, $00, $00, $d0, $b0, $b0, $d0, $00, $00				; Waveform:13
	db $00, $68, $48, $18, $48, $68, $28, $00, $48, $77, $58, $08, $28, $38, $00, $b8, $08, $58, $00, $e8, $f8, $00, $b8, $98, $c8, $08, $e8, $a8, $c8, $f8, $c8, $a8				; Waveform:20
	db $00, $e0, $c0, $a0, $80, $a0, $c0, $e0, $00, $20, $40, $60, $7f, $60, $40, $20, $00, $e0, $c0, $a0, $80, $a0, $c0, $e0, $00, $20, $40, $60, $7f, $60, $40, $20				; Waveform:22
	db $10, $20, $28, $18, $f0, $d0, $b0, $d0, $f0, $00, $18, $50, $7f, $68, $50, $40, $20, $e0, $a8, $a0, $c8, $f0, $10, $30, $50, $38, $08, $e0, $b8, $a0, $b8, $e8				; Waveform:24
	db $07, $57, $37, $4f, $27, $e7, $9f, $2f, $6f, $b7, $ff, $e7, $1f, $ff, $4f, $d7, $e7, $ff, $f7, $df, $f7, $67, $47, $d7, $1f, $97, $b7, $e7, $d7, $c7, $f7, $ff				; Waveform:31

; [ FM Drum macros]
.drummacro_start:

; [ Instruments]
.instrument_start:
	dw .instrument_00				; BD C2           
	dw .instrument_01				; SD A3           
	dw .instrument_02				; HH CL A8        
	dw .instrument_03				; TOM             
	dw .instrument_04				; GTR CHORDS      
	dw .instrument_05				; SC Bass         
	dw .instrument_06				; SC Brass Satan  
	dw .instrument_07				; ARP SQ          
	dw .instrument_08				; Clarinet low    
	dw .instrument_09				; Response BELL   
	dw .instrument_10				; HH OP A8        
	dw .instrument_11				; SD-MAIN         

.instrument_00:					; BD C2           
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $06,$c0,$ff					; Tone sub
	db $1c,$03						; Envelope shape
	db $02,$10						; Mixer (T)
	db $06,$c0,$ff					; Tone sub
	db $1a						; Envelope
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_01:					; SD A3           
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$06						; Noise _
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0b						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0a						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$09						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$08						; Volume _
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
	db $02,$90						; Mixer (TN)
	db $0E,$00						; Noise _
	db $08,$0f						; Volume _
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_03:					; TOM             
	db $38						; Waveform 7
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $08,$0d						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $0c,$01						; Volume -
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $00							; Volume same
	db $18,$ec						; Loop (-21)

.instrument_04:					; GTR CHORDS      
	db $48						; Waveform 9
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$03						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$05						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_05:					; SC Bass         
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $0c,$01						; Volume -
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $18,$f5						; Loop (-12)

.instrument_06:					; SC Brass Satan  
	db $18						; Waveform 3
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
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
	db $08,$0b						; Volume _
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
	db $08,$09						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_07:					; ARP SQ          
	db $30						; Waveform 6
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
	db $00							; Volume same
	db $18,$fe						; Loop (-3)

.instrument_08:					; Clarinet low    
	db $40						; Waveform 8
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
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
	db $18,$e6						; Loop (-27)

.instrument_09:					; Response BELL   
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$0d						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
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
	db $0a,$01						; Volume +
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $0c,$01						; Volume -
	db $18,$d8						; Loop (-41)

.instrument_10:					; HH OP A8        
	db $00						; Waveform 0
	db $02,$90						; Mixer (TN)
	db $0E,$00						; Noise _
	db $08,$0e						; Volume _
								; --- Macro loop
	db $02,$90						; Mixer (TN)
	db $0E,$00						; Noise _
	db $0c,$01						; Volume -
	db $02,$90						; Mixer (TN)
	db $00							; Volume same
	db $02,$90						; Mixer (TN)
	db $00							; Volume same
	db $02,$90						; Mixer (TN)
	db $00							; Volume same
	db $18,$f0						; Loop (-17)

.instrument_11:					; SD-MAIN         
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$04						; Noise _
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$03						; Noise _
	db $08,$0a						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$02						; Noise _
	db $08,$09						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$01						; Noise _
	db $08,$08						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $08,$07						; Volume _
	db $02,$80						; Mixer (N)
	db $00							; Volume same
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)


; [ Song track data ]
.track_000:
	db $60			;Release 96
	db $70			;Volume 15
	db $71			;Instrument 0
	db $df			;Wait 32
	db $bf			;[End-Of-Track]
.track_002:
	db $70			;Volume 15
	db $71			;Instrument 0
	db $df			;Wait 32
	db $bf			;[End-Of-Track]
.track_004:
	db $46			;Note A#6
	db $6f			;Volume 14
	db $78			;Instrument 7
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $6b			;Volume 10
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $6b			;Volume 10
	db $a8				; SCC Soften Waveform
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $67			;Volume 6
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_005:
	db $60			;Release 96
	db $c2			;Wait 3
	db $46			;Note A#6
	db $67			;Volume 6
	db $78			;Instrument 7
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $66			;Volume 5
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $65			;Volume 4
	db $a8				; SCC Soften Waveform
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3a			;Note A#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $37			;Note G-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $46			;Note A#6
	db $64			;Volume 3
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $43			;Note G-6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $41			;Note F-6
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $3e			;Note D-6
	db $bf			;[End-Of-Track]
.track_008:
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $2f			;Note B-4
	db $6e			;Volume 13
	db $74			;Instrument 3
	db $c0			;Wait 1
	db $2f			;Note B-4
	db $c3			;Wait 4
	db $29			;Note F-4
	db $9a,$03			;CMD Note delay
	db $c4			;Wait 5
	db $26			;Note D-4
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 4
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6f			;Volume 14
	db $7b			;Instrument 10
					;Wait Repeat 2
	db $21			;Note A-3
	db $6d			;Volume 12
	db $72			;Instrument 1
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_009:
	db $a5,$00			;CMD Envelope multiplier low
	db $c2			;Wait 3
	db $47			;Note B-6
	db $69			;Volume 8
	db $7a			;Instrument 9
	db $a1,$08			;CMD Track detune
	db $c1			;Wait 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
	db $bf			;[End-Of-Track]
.track_010:
	db $a4,$04			;CMD Envelope multiplier high
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_011:
	db $47			;Note B-6
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $a1,$08			;CMD Track detune
	db $c1			;Wait 2
	db $42			;Note F#6
	db $6b			;Volume 10
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_012:
	db $0b			;Note B-1
	db $6e			;Volume 13
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $0b			;Note B-1
	db $c1			;Wait 2
	db $0b			;Note B-1
					;Wait Repeat 2
	db $17			;Note B-2
	db $c3			;Wait 4
	db $0b			;Note B-1
	db $c1			;Wait 2
	db $0b			;Note B-1
					;Wait Repeat 2
	db $0d			;Note C#2
	db $c4			;Wait 5
	db $15			;Note A-2
	db $9a,$02			;CMD Note delay
					;Wait Repeat 5
	db $19			;Note C#3
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $0e			;Note D-2
	db $c3			;Wait 4
	db $0e			;Note D-2
	db $c1			;Wait 2
	db $0e			;Note D-2
					;Wait Repeat 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $0e			;Note D-2
	db $c1			;Wait 2
	db $0e			;Note D-2
					;Wait Repeat 2
	db $0d			;Note C#2
	db $c3			;Wait 4
	db $0d			;Note C#2
	db $c1			;Wait 2
	db $0d			;Note C#2
					;Wait Repeat 2
	db $19			;Note C#3
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_013:
	db $21			;Note A-3
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $cb			;Wait 12
	db $21			;Note A-3
	db $c1			;Wait 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $23			;Note B-3
	db $c4			;Wait 5
	db $23			;Note B-3
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $23			;Note B-3
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $25			;Note C#4
	db $cb			;Wait 12
	db $25			;Note C#4
	db $c1			;Wait 2
	db $25			;Note C#4
					;Wait Repeat 2
	db $23			;Note B-3
	db $c4			;Wait 5
	db $23			;Note B-3
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $20			;Note G#3
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_014:
	db $26			;Note D-4
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $a9,$18			; SCC Set Waveform 8
	db $cb			;Wait 12
	db $26			;Note D-4
	db $c1			;Wait 2
	db $26			;Note D-4
					;Wait Repeat 2
	db $28			;Note E-4
	db $c4			;Wait 5
	db $28			;Note E-4
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $28			;Note E-4
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $2a			;Note F#4
	db $cb			;Wait 12
	db $2a			;Note F#4
	db $c1			;Wait 2
	db $2a			;Note F#4
					;Wait Repeat 2
	db $28			;Note E-4
	db $c4			;Wait 5
	db $28			;Note E-4
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $23			;Note B-3
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_015:
	db $2a			;Note F#4
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $c2			;Wait 3
	db $94,$66			;CMD Vibrato
	db $c8			;Wait 9
	db $2a			;Note F#4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2a			;Note F#4
					;Wait Repeat 2
	db $2c			;Note G#4
	db $c4			;Wait 5
	db $2d			;Note A-4
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $2f			;Note B-4
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $2d			;Note A-4
	db $c3			;Wait 4
	db $94,$66			;CMD Vibrato
	db $c7			;Wait 8
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $2c			;Note G#4
	db $c4			;Wait 5
	db $2a			;Note F#4
	db $9a,$03			;CMD Note delay
					;Wait Repeat 5
	db $28			;Note E-4
	db $9a,$05			;CMD Note delay
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_016:
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 4
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 4
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6f			;Volume 14
	db $7b			;Instrument 10
					;Wait Repeat 2
	db $21			;Note A-3
	db $6d			;Volume 12
	db $72			;Instrument 1
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $5d			;Note A-8
	db $6d			;Volume 12
					;Wait Repeat 2
	db $5d			;Note A-8
	db $6e			;Volume 13
	db $7b			;Instrument 10
					;Wait Repeat 2
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $7c			;Instrument 11
	db $c3			;Wait 4
	db $5d			;Note A-8
	db $6f			;Volume 14
	db $7b			;Instrument 10
	db $c1			;Wait 2
	db $21			;Note A-3
	db $6d			;Volume 12
	db $72			;Instrument 1
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_017:
	db $c2			;Wait 3
	db $47			;Note B-6
	db $69			;Volume 8
	db $7a			;Instrument 9
	db $a1,$08			;CMD Track detune
	db $c1			;Wait 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $47			;Note B-6
					;Wait Repeat 2
	db $42			;Note F#6
					;Wait Repeat 2
	db $3b			;Note B-5
	db $bf			;[End-Of-Track]
.track_018:
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_021:
	db $1a			;Note D-3
	db $6b			;Volume 10
	db $75			;Instrument 4
	db $c1			;Wait 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $1a			;Note D-3
	db $c5			;Wait 6
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1a			;Note D-3
					;Wait Repeat 2
	db $1c			;Note E-3
					;Wait Repeat 2
	db $1c			;Note E-3
	db $c5			;Wait 6
	db $1c			;Note E-3
	db $c1			;Wait 2
	db $1c			;Note E-3
	db $c5			;Wait 6
	db $1e			;Note F#3
	db $c1			;Wait 2
	db $1e			;Note F#3
					;Wait Repeat 2
	db $1e			;Note F#3
					;Wait Repeat 2
	db $1e			;Note F#3
	db $c5			;Wait 6
	db $1e			;Note F#3
	db $c1			;Wait 2
	db $1e			;Note F#3
	db $c1			;Wait 2
	db $1c			;Note E-3
	db $77			;Instrument 6
	db $c7			;Wait 8
	db $14			;Note G#2
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_022:
	db $34			;Note E-5
	db $70			;Volume 15
	db $79			;Instrument 8
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $36			;Note F#5
	db $90,$0a			;CMD Portamento tone
	db $c3			;Wait 4
	db $94,$26			;CMD Vibrato
	db $c4			;Wait 5
	db $34			;Note E-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $36			;Note F#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $34			;Note E-5
	db $9a,$03			;CMD Note delay
	db $c2			;Wait 3
	db $2f			;Note B-4
	db $9a,$02			;CMD Note delay
					;Wait Repeat 3
	db $34			;Note E-5
	db $c1			;Wait 2
	db $38			;Note G#5
	db $9a,$03			;CMD Note delay
	db $c2			;Wait 3
	db $3b			;Note B-5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 3
	db $38			;Note G#5
	db $c0			;Wait 1
	db $39			;Note A-5
	db $90,$04			;CMD Portamento tone
	db $c3			;Wait 4
	db $94,$36			;CMD Vibrato
					;Wait Repeat 4
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $38			;Note G#5
	db $c1			;Wait 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3d			;Note C#6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $3b			;Note B-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $39			;Note A-5
	db $c0			;Wait 1
	db $38			;Note G#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $36			;Note F#5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $34			;Note E-5
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $36			;Note F#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $38			;Note G#5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $39			;Note A-5
	db $c0			;Wait 1
	db $34			;Note E-5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $31			;Note C#5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_023:
	db $c2			;Wait 3
	db $34			;Note E-5
	db $68			;Volume 7
	db $79			;Instrument 8
	db $a1,$09			;CMD Track detune
	db $c0			;Wait 1
	db $36			;Note F#5
	db $90,$0a			;CMD Portamento tone
	db $c3			;Wait 4
	db $94,$26			;CMD Vibrato
	db $c4			;Wait 5
	db $34			;Note E-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $32			;Note D-5
					;Wait Repeat 2
	db $36			;Note F#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $34			;Note E-5
	db $9a,$03			;CMD Note delay
	db $c2			;Wait 3
	db $2f			;Note B-4
	db $9a,$02			;CMD Note delay
					;Wait Repeat 3
	db $34			;Note E-5
	db $c1			;Wait 2
	db $38			;Note G#5
	db $9a,$03			;CMD Note delay
	db $c2			;Wait 3
	db $3b			;Note B-5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 3
	db $38			;Note G#5
	db $c0			;Wait 1
	db $39			;Note A-5
	db $90,$04			;CMD Portamento tone
	db $c3			;Wait 4
	db $94,$36			;CMD Vibrato
					;Wait Repeat 4
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $38			;Note G#5
	db $c1			;Wait 2
	db $39			;Note A-5
					;Wait Repeat 2
	db $3b			;Note B-5
					;Wait Repeat 2
	db $3e			;Note D-6
	db $c0			;Wait 1
	db $3d			;Note C#6
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $3b			;Note B-5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $39			;Note A-5
	db $c0			;Wait 1
	db $38			;Note G#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $36			;Note F#5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $34			;Note E-5
	db $a1,$08			;CMD Track detune
	db $c0			;Wait 1
	db $36			;Note F#5
	db $9a,$02			;CMD Note delay
					;Wait Repeat 1
	db $38			;Note G#5
	db $9a,$03			;CMD Note delay
	db $c1			;Wait 2
	db $39			;Note A-5
	db $bf			;[End-Of-Track]
.track_024:
	db $11			;Note F-2
	db $70			;Volume 15
	db $71			;Instrument 0
			;CMD EE Not supported [Global transpose][WARNING]
	db $c0			;Wait 1
			;CMD EE Not supported [Global transpose][WARNING]
	db $fe			;Wait 63
	db $bf			;[End-Of-Track]
.track_025:
	db $a5,$42			;CMD Envelope multiplier low
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_027:
	db $47			;Note B-6
	db $6e			;Volume 13
	db $78			;Instrument 7
	db $a1,$08			;CMD Track detune
	db $ff			;Wait 64
	db $bf			;[End-Of-Track]
.track_028:
	db $0b			;Note B-1
	db $6e			;Volume 13
	db $76			;Instrument 5
	db $c2			;Wait 3
	db $ac,$06			; SCC Morph speed 6
	db $c0			;Wait 1
	db $aa				; SCC Morph foloow (Carbon C0poy) 0
	db $fb			;Wait 60
	db $bf			;[End-Of-Track]
.track_029:
	db $21			;Note A-3
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $c3			;Wait 4
	db $9c,$00			; SCC Morph to Waveform 0
	db $fb			;Wait 60
	db $bf			;[End-Of-Track]
.track_030:
	db $26			;Note D-4
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $c3			;Wait 4
	db $aa				; SCC Morph foloow (Carbon C0poy) 0
	db $fb			;Wait 60
	db $bf			;[End-Of-Track]
.track_031:
	db $2a			;Note F#4
	db $6c			;Volume 11
	db $77			;Instrument 6
	db $c2			;Wait 3
	db $94,$66			;CMD Vibrato
	db $c0			;Wait 1
	db $aa				; SCC Morph foloow (Carbon C0poy) 0
	db $fb			;Wait 60
	db $bf			;[End-Of-Track]

