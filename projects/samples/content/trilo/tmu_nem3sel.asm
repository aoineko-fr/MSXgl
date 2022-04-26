; Song: NEM3- Weapon Select             
; By:   Konami (Arr. by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	db $07					; Initial song speed.
	dw .waveform_start			; Start of the waveform data.
	dw .instrument_start			; Start of the instrument data.

; [ Song order pointer list ]
.restart:
	dw .track_000, .track_001, .track_002, .track_003, .track_004, .track_005, .track_006, .track_007	; Step:000 Pattern:000
	dw .track_008, .track_009, .track_010, .track_011, .track_012, .track_013, .track_014, .track_015	; Step:001 Pattern:001
	dw .track_000, .track_017, .track_018, .track_019, .track_020, .track_021, .track_022, .track_023	; Step:002 Pattern:002
	dw .track_024, .track_025, .track_026, .track_027, .track_028, .track_029, .track_030, .track_031	; Step:003 Pattern:003
	dw 0x0000, .restart				; End of sequence delimiter + restart address.

; [ Custom FM voices ]
.customvoice_start:

; [ SCC Waveforms ]
.waveform_start:
	db $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00				; Waveform:0
	db $80, $b0, $c0, $10, $1a, $2a, $2c, $1a, $00, $e0, $d0, $e0, $22, $53, $70, $75, $70, $31, $ea, $80, $88, $8a, $8c, $8e, $00, $7f, $75, $73, $62, $00, $c0, $90				; Waveform:1
	db $a0, $90, $a0, $ac, $f0, $a0, $16, $00, $90, $00, $20, $40, $60, $50, $38, $1c, $f2, $e2, $d6, $d0, $d0, $da, $dc, $e0, $e2, $e2, $e2, $e0, $dc, $d8, $d0, $b0				; Waveform:7
	db $00, $7f, $00, $80, $a0, $c0, $d8, $f0, $08, $20, $30, $40, $50, $60, $70, $78, $7c, $7f, $7c, $78, $70, $60, $50, $40, $30, $20, $08, $f0, $d8, $c0, $a0, $80				; Waveform:17
	db $00, $e0, $c0, $a0, $80, $a0, $c0, $e0, $00, $20, $40, $60, $7f, $60, $40, $20, $00, $e0, $c0, $a0, $80, $a0, $c0, $e0, $00, $20, $40, $60, $7f, $60, $40, $20				; Waveform:22
	db $90, $88, $90, $9c, $b0, $c0, $e0, $26, $70, $7c, $70, $56, $40, $20, $10, $fc, $e0, $fc, $10, $fc, $40, $54, $72, $7f, $70, $26, $e0, $c0, $b0, $9c, $90, $88				; Waveform:30
	db $70, $70, $70, $70, $70, $70, $70, $70, $80, $80, $80, $80, $80, $80, $80, $80, $70, $70, $70, $80, $80, $80, $70, $70, $70, $70, $80, $80, $80, $80, $80, $80				; Waveform:31

; [ FM Drum macros]
.drummacro_start:

; [ Instruments]
.instrument_start:
	dw .instrument_00				; BD C2           
	dw .instrument_01				; SD A3           
	dw .instrument_02				; HH CL A8        
	dw .instrument_03				; TOM             
	dw .instrument_04				; HH roll         
	dw .instrument_05				; Short Snare     
	dw .instrument_06				; SC Bass         
	dw .instrument_07				; SD-MAIN         
	dw .instrument_08				; Melody          
	dw .instrument_09				; CONT            
	dw .instrument_10				; ARP             
	dw .instrument_11				; ARP 2           
	dw .instrument_12				; PSG ARP         

.instrument_00:					; BD C2           
	db $18						; Waveform 3
	db $02,$10						; Mixer (T)
	db $06,$c0,$ff					; Tone sub
	db $1c,$01						; Envelope shape
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $06,$c0,$ff					; Tone sub
	db $1a						; Envelope
	db $18,$f9						; Loop (-8)

.instrument_01:					; SD A3           
	db $08						; Waveform 1
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$01						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0d						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0a						; Volume _
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

.instrument_03:					; TOM             
	db $20						; Waveform 4
	db $02,$10						; Mixer (T)
	db $04,$40,$00					; Tone add
	db $08,$0e						; Volume _
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

.instrument_04:					; HH roll         
	db $10						; Waveform 2
								; --- Macro loop
	db $02,$80						; Mixer (N)
	db $0E,$00						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $08,$0c						; Volume _
	db $08,$00						; Volume _
	db $18,$f3						; Loop (-14)

.instrument_05:					; Short Snare     
	db $08						; Waveform 1
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

.instrument_06:					; SC Bass         
	db $30						; Waveform 6
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
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $18,$fc						; Loop (-5)

.instrument_07:					; SD-MAIN         
	db $00						; Waveform 0
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$05						; Noise _
	db $08,$0f						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$06						; Noise _
	db $08,$0e						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$07						; Noise _
	db $08,$0d						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$08						; Noise _
	db $08,$0c						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$09						; Noise _
	db $08,$0b						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$0a						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0b						; Noise _
	db $08,$0a						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$0a						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0b						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0c						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0b						; Noise _
	db $08,$09						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$0c						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0b						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0c						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0d						; Noise _
	db $08,$08						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$0c						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0d						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0e						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0d						; Noise _
	db $08,$07						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$0e						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0f						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0e						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0f						; Noise _
	db $08,$06						; Volume _
	db $02,$80						; Mixer (N)
	db $0E,$10						; Noise _
	db $00							; Volume same
	db $02,$80						; Mixer (N)
	db $0E,$0f						; Noise _
	db $00							; Volume same
								; --- Macro loop
	db $08,$00						; Volume _
	db $18,$fd						; Loop (-4)

.instrument_08:					; Melody          
	db $28						; Waveform 5
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
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
	db $08,$0b						; Volume _
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
	db $08,$09						; Volume _
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
	db $08,$05						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_09:					; CONT            
	db $28						; Waveform 5
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_10:					; ARP             
	db $18						; Waveform 3
	db $02,$10						; Mixer (T)
	db $08,$0f						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0c						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$04						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$06						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$08						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0a						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_11:					; ARP 2           
	db $18						; Waveform 3
	db $02,$10						; Mixer (T)
	db $08,$0e						; Volume _
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$09						; Volume _
	db $02,$10						; Mixer (T)
	db $00							; Volume same
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$07						; Volume _
	db $18,$fb						; Loop (-6)

.instrument_12:					; PSG ARP         
	db $18						; Waveform 3
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
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
								; --- Macro loop
	db $02,$10						; Mixer (T)
	db $08,$0b						; Volume _
	db $18,$fb						; Loop (-6)


; [ Song track data ]
.track_000:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $a5,$00			;CMD Envelope multiplier low
	db $c1			;Wait 2
	db $13			;Note G-2
	db $c5			;Wait 6
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $11			;Note F-2
	db $6d			;Volume 12
	db $75			;Instrument 4
					;Wait Repeat 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
					;Wait Repeat 2
	db $21			;Note A-3
	db $c1			;Wait 2
	db $13			;Note G-2
	db $71			;Instrument 0
					;Wait Repeat 2
	db $13			;Note G-2
	db $c5			;Wait 6
	db $13			;Note G-2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $72			;Instrument 1
					;Wait Repeat 4
	db $21			;Note A-3
	db $6f			;Volume 14
	db $c7			;Wait 8
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_001:
	db $60			;Release 96
	db $a4,$04			;CMD Envelope multiplier high
	db $c3			;Wait 4
	db $1a			;Note D-3
	db $6c			;Volume 11
	db $7d			;Instrument 12
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_002:
	db $60			;Release 96
	db $c2			;Wait 3
	db $32			;Note D-5
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c4			;Wait 5
	db $94,$36			;CMD Vibrato
	db $d6			;Wait 23
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
	db $c3			;Wait 4
	db $94,$36			;CMD Vibrato
	db $d7			;Wait 24
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_003:
	db $07			;Note G-1
	db $6d			;Volume 12
	db $77			;Instrument 6
	db $df			;Wait 32
	db $05			;Note F-1
	db $df			;Wait 32
	db $bf			;[End-Of-Track]
.track_004:
	db $32			;Note D-5
	db $6d			;Volume 12
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c4			;Wait 5
	db $94,$66			;CMD Vibrato
	db $d6			;Wait 23
	db $30			;Note C-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
	db $c5			;Wait 6
	db $94,$66			;CMD Vibrato
	db $d5			;Wait 22
	db $2e			;Note A#4
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_005:
	db $32			;Note D-5
	db $6b			;Volume 10
	db $79			;Instrument 8
	db $a1,$08			;CMD Track detune
	db $cb			;Wait 12
	db $94,$68			;CMD Vibrato
	db $d5			;Wait 22
	db $30			;Note C-5
	db $6d			;Volume 12
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $2f			;Note B-4
					;Wait Repeat 2
	db $30			;Note C-5
	db $c5			;Wait 6
	db $94,$68			;CMD Vibrato
	db $d3			;Wait 20
	db $bf			;[End-Of-Track]
.track_006:
	db $1a			;Note D-3
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_007:
	db $26			;Note D-4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $a9,$18			; SCC Set Waveform 17
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_008:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $c5			;Wait 6
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c5			;Wait 6
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $72			;Instrument 1
	db $c7			;Wait 8
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
	db $13			;Note G-2
	db $6d			;Volume 12
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $72			;Instrument 1
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
					;Wait Repeat 2
	db $21			;Note A-3
	db $70			;Volume 15
					;Wait Repeat 2
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c7			;Wait 8
	db $1f			;Note G-3
	db $70			;Volume 15
	db $78			;Instrument 7
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_009:
	db $1f			;Note G-3
	db $6d			;Volume 12
	db $7d			;Instrument 12
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1b			;Note D#3
	db $c3			;Wait 4
	db $1b			;Note D#3
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $1b			;Note D#3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_010:
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $c1			;Wait 2
	db $2e			;Note A#4
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_011:
	db $0a			;Note A#1
	db $77			;Instrument 6
	db $df			;Wait 32
	db $09			;Note A-1
	db $cf			;Wait 16
	db $08			;Note G#1
	db $cf			;Wait 16
	db $bf			;[End-Of-Track]
.track_012:
	db $2e			;Note A#4
	db $6d			;Volume 12
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $bf			;[End-Of-Track]
.track_013:
	db $c1			;Wait 2
	db $2e			;Note A#4
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $9b			;CMD End 
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $2e			;Note A#4
	db $c4			;Wait 5
	db $94,$68			;CMD Vibrato
	db $ca			;Wait 11
	db $29			;Note F-4
	db $9b			;CMD End 
	db $c7			;Wait 8
	db $94,$68			;CMD Vibrato
					;Wait Repeat 8
	db $2d			;Note A-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$68			;CMD Vibrato
	db $c8			;Wait 9
	db $2c			;Note G#4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$68			;CMD Vibrato
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_014:
	db $1a			;Note D-3
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $c3			;Wait 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
					;Wait Repeat 4
	db $1a			;Note D-3
					;Wait Repeat 4
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1a			;Note D-3
	db $c3			;Wait 4
	db $1b			;Note D#3
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $1b			;Note D#3
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_015:
	db $26			;Note D-4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $26			;Note D-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2b			;Note G-4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2d			;Note A-4
					;Wait Repeat 2
	db $32			;Note D-5
	db $c1			;Wait 2
	db $27			;Note D#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2c			;Note G#4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $33			;Note D#5
	db $c1			;Wait 2
	db $27			;Note D#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $2c			;Note G#4
	db $6a			;Volume 9
					;Wait Repeat 2
	db $2e			;Note A#4
					;Wait Repeat 2
	db $33			;Note D#5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_017:
	db $60			;Release 96
	db $a4,$06			;CMD Envelope multiplier high
	db $c3			;Wait 4
	db $20			;Note G#3
	db $6c			;Volume 11
	db $7d			;Instrument 12
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $25			;Note C#4
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_018:
	db $60			;Release 96
	db $c2			;Wait 3
	db $38			;Note G#5
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c4			;Wait 5
	db $94,$36			;CMD Vibrato
	db $d6			;Wait 23
	db $36			;Note F#5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $36			;Note F#5
	db $c3			;Wait 4
	db $94,$36			;CMD Vibrato
	db $d7			;Wait 24
	db $34			;Note E-5
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_019:
	db $0d			;Note C#2
	db $6d			;Volume 12
	db $77			;Instrument 6
	db $df			;Wait 32
	db $0b			;Note B-1
	db $df			;Wait 32
	db $bf			;[End-Of-Track]
.track_020:
	db $38			;Note G#5
	db $6d			;Volume 12
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$56			;CMD Vibrato
	db $d5			;Wait 22
	db $36			;Note F#5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $36			;Note F#5
	db $c5			;Wait 6
	db $94,$66			;CMD Vibrato
	db $d5			;Wait 22
	db $34			;Note E-5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $33			;Note D#5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_021:
	db $c4			;Wait 5
	db $9b			;CMD End 
	db $c0			;Wait 1
	db $38			;Note G#5
	db $6c			;Volume 11
	db $79			;Instrument 8
	db $a1,$08			;CMD Track detune
	db $c5			;Wait 6
	db $94,$68			;CMD Vibrato
	db $d5			;Wait 22
	db $36			;Note F#5
	db $9b			;CMD End 
	db $c1			;Wait 2
	db $35			;Note F-5
					;Wait Repeat 2
	db $36			;Note F#5
	db $c5			;Wait 6
	db $94,$68			;CMD Vibrato
	db $d3			;Wait 20
	db $bf			;[End-Of-Track]
.track_022:
	db $20			;Note G#3
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $25			;Note C#4
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $25			;Note C#4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_023:
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_024:
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $c5			;Wait 6
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6e			;Volume 13
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $13			;Note G-2
	db $6d			;Volume 12
	db $76			;Instrument 5
	db $c5			;Wait 6
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c1			;Wait 2
	db $21			;Note A-3
	db $70			;Volume 15
	db $72			;Instrument 1
	db $c7			;Wait 8
	db $13			;Note G-2
	db $71			;Instrument 0
	db $c3			;Wait 4
	db $13			;Note G-2
	db $6d			;Volume 12
	db $73			;Instrument 2
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $6d			;Volume 12
	db $74			;Instrument 3
	db $c0			;Wait 1
	db $2d			;Note A-4
	db $c0			;Wait 1
	db $2b			;Note G-4
	db $6d			;Volume 12
	db $c1			;Wait 2
	db $29			;Note F-4
	db $70			;Volume 15
					;Wait Repeat 2
	db $28			;Note E-4
	db $70			;Volume 15
					;Wait Repeat 2
	db $24			;Note C-4
	db $70			;Volume 15
					;Wait Repeat 2
	db $1f			;Note G-3
	db $c1			;Wait 2
	db $1f			;Note G-3
	db $6e			;Volume 13
	db $c5			;Wait 6
	db $1f			;Note G-3
	db $6f			;Volume 14
	db $78			;Instrument 7
	db $c7			;Wait 8
	db $bf			;[End-Of-Track]
.track_025:
	db $25			;Note C#4
	db $6d			;Volume 12
	db $7d			;Instrument 12
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $25			;Note C#4
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $21			;Note A-3
	db $c3			;Wait 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $21			;Note A-3
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]
.track_026:
	db $c0			;Wait 1
	db $33			;Note D#5
	db $c1			;Wait 2
	db $34			;Note E-5
	db $6d			;Volume 12
	db $7a			;Instrument 9
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $33			;Note D#5
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c8			;Wait 9
	db $32			;Note D-5
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$36			;CMD Vibrato
	db $c5			;Wait 6
	db $bf			;[End-Of-Track]
.track_027:
	db $10			;Note E-2
	db $77			;Instrument 6
	db $df			;Wait 32
	db $0f			;Note D#2
	db $cf			;Wait 16
	db $0e			;Note D-2
	db $cf			;Wait 16
	db $bf			;[End-Of-Track]
.track_028:
	db $34			;Note E-5
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $33			;Note D#5
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $32			;Note D-5
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$76			;CMD Vibrato
	db $c9			;Wait 10
	db $bf			;[End-Of-Track]
.track_029:
	db $c1			;Wait 2
	db $34			;Note E-5
	db $6e			;Volume 13
	db $79			;Instrument 8
	db $9b			;CMD End 
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $34			;Note E-5
	db $c4			;Wait 5
	db $94,$68			;CMD Vibrato
	db $ca			;Wait 11
	db $2f			;Note B-4
	db $9b			;CMD End 
	db $c7			;Wait 8
	db $94,$68			;CMD Vibrato
					;Wait Repeat 8
	db $33			;Note D#5
	db $9b			;CMD End 
	db $c6			;Wait 7
	db $94,$68			;CMD Vibrato
	db $c8			;Wait 9
	db $32			;Note D-5
	db $9b			;CMD End 
	db $c5			;Wait 6
	db $94,$68			;CMD Vibrato
	db $c2			;Wait 3
	db $9b			;CMD End 
	db $bf			;[End-Of-Track]
.track_030:
	db $20			;Note G#3
	db $6c			;Volume 11
	db $7b			;Instrument 10
	db $c3			;Wait 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
	db $c1			;Wait 2
	db $25			;Note C#4
	db $c3			;Wait 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
					;Wait Repeat 4
	db $20			;Note G#3
					;Wait Repeat 4
	db $25			;Note C#4
	db $c1			;Wait 2
	db $20			;Note G#3
	db $c3			;Wait 4
	db $21			;Note A-3
					;Wait Repeat 4
	db $26			;Note D-4
					;Wait Repeat 4
	db $21			;Note A-3
	db $c1			;Wait 2
	db $26			;Note D-4
	db $c3			;Wait 4
	db $bf			;[End-Of-Track]
.track_031:
	db $2c			;Note G#4
	db $6b			;Volume 10
	db $7c			;Instrument 11
	db $c1			;Wait 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2c			;Note G#4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $31			;Note C#5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $33			;Note D#5
					;Wait Repeat 2
	db $38			;Note G#5
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $32			;Note D-5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $34			;Note E-5
					;Wait Repeat 2
	db $39			;Note A-5
	db $c1			;Wait 2
	db $2d			;Note A-4
	db $6b			;Volume 10
					;Wait Repeat 2
	db $32			;Note D-5
	db $6a			;Volume 9
					;Wait Repeat 2
	db $34			;Note E-5
					;Wait Repeat 2
	db $39			;Note A-5
	db $c1			;Wait 2
	db $bf			;[End-Of-Track]

