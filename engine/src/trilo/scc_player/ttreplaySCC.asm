;=================================
; TriloTracker Re-player SCC
;
; Functions:
; 	replay_init
;	replay_pause
;	replay_fade_out
;	replay_set_SCC_balance
;	replay_set_PSG_balance
;	replay_equalization
;	replay_loadsong
;================================
; ; Compile options:
; .define MSX2				; Used in speed equalization

; ;//.define TAIL_ON			; Limit volume to 1


; ;--- Only set 1 of the settings below for the period table
; ;//.define PERIOD_A440		; Modern 
; ;//.define PERIOD_A432		; Earth
; .define PERIOD_A445		; Konami
; ;//.define PERIOD_A448		; Classical

; ;//.define EXTERNAL_SCC 
; .define INTERNAL_SCC		; For internal no slot select is needed.

; .define SFXPLAY_ENABLED	; Enable the SFX functionality.

; ;---- Performance option
; ;//.define FPGA_SCC		; FPGA SCC can be written faster
						; ; as there are no artifacts when writing same values
; ;//.define TREMOLO_OFF		; removes tremolo code making the replayer a little bit faster
; ;===============================

_REL		= 96		; = release
_SUS		= 97		; = sustain
_VOL		= 98		; = volume 0
_INS		= 113+1	; = instrument 1
_CMD		= 144+1	; = effect 0
_SPC		= 184		; = special commands
_EOT		= 191		; = end of track
_WAIT		= 192		; = wait 1


;===========================================================
; ---	replay_init
; Initialize replayer data
; Only call this on start-up
; Input: none
;===========================================================
replay_init::
	ld		a, #4
	call	replay_set_SCC_balance
	ld		a, #4
	call	replay_set_PSG_balance

	xor		a
	ld		(replay_mode), a
	ld		(equalization_cnt), a
	ld		(equalization_flag), a
	ld		(equalization_freq), a
	ld		(replay_morph_speed), a
	inc		a
	ld		(replay_morph_type), a

	ret


;===========================================================
; ---	replay_pause
; Stop/Restart music playback
; 
; Input: none
;===========================================================
replay_pause::
	ld		a, (replay_mode)
	and		a
	jp		nz, _r_pause_disable
_r_pause_enable:
	;-- re-enable music decoding and processing
	ld		a, #1
	ld		(replay_mode), a
	ret

_r_pause_disable:
	;-- stop decoding and processing music data
	xor		a
	ld		(replay_mode), a
	;-- set mixers to silence.
	ld 		a, #0x3F
	ld		(PSG_regMIXER), a 
	xor		a
	ld		(SCC_regMIXER), a
	ret


;===========================================================
; ---	replay_fade_out
; Fade out the music. 
; Once the sound is silence the replayer is paused.
;
; in: [A] fade speed (1 - 255)
;===========================================================	
replay_fade_out::
	ld		(replay_fade), a
	ld		(replay_fade_timer), a
	xor		a
	ld		(replay_fade_vol), a
	ret


;===========================================================
; ---	replay_set_SCC_balance
; Set the main volume for the SCC chip. This enables for
; setting the balance between SCC and PSG as some MSX models 
; default balance differs. 
;
; in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
;===========================================================	
replay_set_SCC_balance::
	cp		#5	; limit 
	ret		nc
	call	_getnewbalancebase
	ld		(replay_mainSCCvol), hl	
	ret


;===========================================================
; ---	replay_set_PSG_balance
; Set the main volume for the PSG chip. This enables for
; setting the balance between SCC and PSG as some MSX models 
; default balance differs. 
;
; in: [A] master volume (0-7) 0=halve volume, 8=full volume. 
;===========================================================	
replay_set_PSG_balance::
	cp		#5	; limit 
	ret		nc
	call	_getnewbalancebase
	ld		(replay_mainPSGvol), hl	
	ret
	
_getnewbalancebase:
	add		a		; times 16
	add		a
	add		a
	add		a
	ld		hl, #_VOLUME_TABLE-64
	add		a, l
	ld		l, a
	ret 	nc
	inc		h
	ret


;===========================================================
; ---	replay_equalization
; Enables or disables the speed equalization. 
; This to enable same speed playback on 50 and 60Hz. 
;
; in: [A] setting of the equalization (0 = off, other values = on) 
;===========================================================	
;// MSXgl - Start
; replay_equalization::
; .ifdef MSX2
	; and		a
	; jp		z, replay_equalization.off
	; ;--- Only enable if in 60Hz mode
	; ld		a, (0xFFE8)	; get mirror of VDP reg# 9
	; and		#0b00000010
	; xor		#0b00000010
; replay_equalization.off:
; .endif
	; ld		(equalization_freq), a
	; ret
;// MSXgl - End


;===========================================================
; ---	replay_loadsong
; Initialize a song for playback
; 
; Input [HL] points to start song
;===========================================================
replay_loadsong::
	;--- Get the start speed.
	ld		a, (hl)	
	inc		hl
	ld		(replay_speed), a

	;--- Set waveform base, intrument base and track pointers 
	ld		de, #replay_wavebase
	ld		bc, #20
	ldir
	ld		(replay_orderpointer), hl		; store pointer for next set
								; of track pointers
	;--- Initialize replayer variables.
	xor		a
	ld		(replay_speed_subtimer), a
	
	;--- Set tonetable here as SCC and PSG share same tonetable
	;// MSXgl - Start
	; ld		hl, #TRACK_ToneTable_PSG			; Set the PSG note table
	; ld		(replay_tonetable), hl	
	;// MSXgl - End

	;--- Erase channel data	in RAM
	ld		bc, #TRACK_REC_SIZE*8-1
	ld		hl, #TRACK_Chan1
	ld		de, #TRACK_Chan1+1
	ld		(hl), a
	ldir

	ld		(replay_arp_speed), a
	;--- Silence the chips
	ld		(SCC_regMIXER), a
	ld		(PSG_regVOLA), a
	ld		(PSG_regVOLB), a
	ld		(PSG_regVOLC), a
	;//ld	(PSG_regNOISE), a
	ld		(replay_noise), a
	ld		a, #0x3f
	ld		(PSG_regMIXER), a

	ld		a, #1
	ld		(replay_speed_timer), a
;//	ld		(replay_morph_timer), a
;//	ld		(replay_morph_speed), a
	ld		(TRACK_Chan1+17+TRACK_Delay), a
	ld		(TRACK_Chan2+17+TRACK_Delay), a
	ld		(TRACK_Chan3+17+TRACK_Delay), a
	ld		(TRACK_Chan4+17+TRACK_Delay), a
	ld		(TRACK_Chan5+17+TRACK_Delay), a
	ld		(TRACK_Chan6+17+TRACK_Delay), a
	ld		(TRACK_Chan7+17+TRACK_Delay), a
	ld		(TRACK_Chan8+17+TRACK_Delay), a

	ld		a, #254
	ld		(TRACK_Chan1+17+TRACK_Instrument), a
	ld		(TRACK_Chan2+17+TRACK_Instrument), a
	ld		(TRACK_Chan3+17+TRACK_Instrument), a
	ld		(TRACK_Chan4+17+TRACK_Instrument), a
	ld		(TRACK_Chan5+17+TRACK_Instrument), a
	ld		(TRACK_Chan6+17+TRACK_Instrument), a
	ld		(TRACK_Chan7+17+TRACK_Instrument), a
	ld		(TRACK_Chan8+17+TRACK_Instrument), a

; .ifdef	EXTERNAL_SCC	
	; ;--- Init the SCC	(waveforms too)
	; ld		a, (SCC_slot)
	; ld		h, #0x80
	; call 	enaslt
; .endif

	ld		a, #255
	ld		(TRACK_Chan4+17+TRACK_Waveform), a
	ld		(TRACK_Chan5+17+TRACK_Waveform), a
	ld		(TRACK_Chan6+17+TRACK_Waveform), a
	ld		(TRACK_Chan7+17+TRACK_Waveform), a
	ld		a, #128
	ld		(TRACK_Chan4+17+TRACK_Flags), a
	ld		(TRACK_Chan5+17+TRACK_Flags), a
	ld		(TRACK_Chan6+17+TRACK_Flags), a
	ld		(TRACK_Chan7+17+TRACK_Flags), a
	ld		(TRACK_Chan8+17+TRACK_Flags), a
	
;//	call scc_reg_update			; Probably not needed.

; .ifdef	EXTERNAL_SCC	
	; ld		a, (mapper_slot)
	; ld		h, 0x80
	; call	enaslt
; .endif	

	call	replay_route

	; end is here
	ld		a, #1
	ld		(replay_mode), a
	ret


;===========================================================
; Set mixer values to silent.
;===========================================================
replay_play_no::
	; Silence PSG vol to avoid pops in sfx when music is not playing.
	xor		a
	ld		(PSG_regVOLA), a
	ld		(PSG_regVOLB), a
	ld		(PSG_regVOLC), a
	; Set mixers to silence
	ld		(SCC_regMIXER), a
	xor		#0x3f
	ld		(PSG_regMIXER), a
	ret


;===========================================================
; ---	replay_play
; Decode music data and process instruments and effects. 
; Music chip registers will be prepared for replay_route 
;
; Input none
;===========================================================	
replay_play::
	ld		a, (replay_mode)
	and		a
	jr z, replay_play_no
;//	ret	z		; replay mode = 0	; halted
				; replay mode = 1	; active

	;---- SPEED EQUALIZATION 
	ld		a, (equalization_freq)		; 0 = 50Hz, otherwise 60Hz
	and		a
	jr		z, PAL			 			; if PAL process at any interrupt;

NTSC:
	ld		hl, #equalization_cnt 		; if NTSC call 5 times out of 6
	dec		(hl)
	jr		nz, PAL						; skip music data processing one tic out of 6

	;--- Reset timer and raise equalization flag
	ld		a, #6
	ld		(hl), a
 	ld		(equalization_flag), a

	xor		a
	ld		(equalization_flag), a
	ret
PAL:
	;---- END SPEED EQUALIZATION

	;--- check for end of pattern
	ld		hl, (TRACK_pointer1)
	ld		a, (hl)
	cp		#191
	jp		nz, replay_play.skip_EOT

	ld 		a, (TRACK_Chan1+17+TRACK_Delay)
	dec		a
	jp		nz, replay_play.skip_EOT
	call	z, _replay_check_patternend	
replay_play.skip_EOT:
	;--- The speed timer
	ld		hl, #replay_speed_timer
	dec		(hl)
	jp		nz, process_data
	
	;--- Re-init Timer == 0
	xor		a
	ld		bc, (replay_speed)		; [b]	subtimer [c] speed
	srl		c						; bit	0 is halve speedstep
	adc		a, a
	xor		b						; alternate	speed	to have halve speed.
	ld		(replay_speed_subtimer), a
	add		c
	ld		(replay_speed_timer), a

;===========================================================
; ---	decode_data
; Process the patterndata 
;===========================================================
decode_data:
	;--- process the channels (tracks)
decode_data.decode1:
	ld 		hl, #TRACK_Chan1+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode2

	ld		a, (TRACK_Chan1+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan1+17+TRACK_Note)	
	ld		ix, #TRACK_Chan1+17
	ld		bc, (TRACK_pointer1)
	call	decode_data_chan
	ld		(TRACK_pointer1), bc
	ld		a, d
	ld		(TRACK_Chan1+17+TRACK_Flags), a

decode_data.decode2:	
	ld 		hl, #TRACK_Chan2+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode3

	ld		a, (TRACK_Chan2+17+TRACK_Flags)

	ld		d, a
	ld		a, (TRACK_Chan2+17+TRACK_Note)	
	ld		ix, #TRACK_Chan2+17
	ld		bc, (TRACK_pointer2)
	call	decode_data_chan
	ld		(TRACK_pointer2), bc
	ld		a, d
	ld		(TRACK_Chan2+17+TRACK_Flags), a	

decode_data.decode3:
	ld 		hl, #TRACK_Chan3+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode4

	ld		a, (TRACK_Chan3+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan3+17+TRACK_Note)	
	ld		ix, #TRACK_Chan3+17
	ld		bc, (TRACK_pointer3)
	call	decode_data_chan
	ld		(TRACK_pointer3), bc
	ld		a, d
	ld		(TRACK_Chan3+17+TRACK_Flags), a

decode_data.decode4:
	ld 		hl, #TRACK_Chan4+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode5

	; ld		iyh, #0x00
	.db		0xFD, 0x26, 0x00
	ld		a, (TRACK_Chan4+17+TRACK_Flags)
	ld 		d, a
	ld		a, (TRACK_Chan4+17+TRACK_Note)	
	ld		ix, #TRACK_Chan4+17
	ld		bc, (TRACK_pointer4)
	call	decode_data_chan
	ld		(TRACK_pointer4), bc
	ld		a, d
	ld		(TRACK_Chan4+17+TRACK_Flags), a

decode_data.decode5:
	ld 		hl, #TRACK_Chan5+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode6

	; ld		iyh, #0x20
	.db		0xFD, 0x26, 0x20
	ld		a, (TRACK_Chan5+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan5+17+TRACK_Note)	
	ld		ix, #TRACK_Chan5+17
	ld		bc, (TRACK_pointer5)
	call	decode_data_chan
	ld		(TRACK_pointer5), bc
	ld		a, d
	ld		(TRACK_Chan5+17+TRACK_Flags), a	

decode_data.decode6:
	ld 		hl, #TRACK_Chan6+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode7

	; ld		iyh, #0x40
	.db		0xFD, 0x26, 0x40
	ld		a, (TRACK_Chan6+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan6+17+TRACK_Note)	
	ld		ix, #TRACK_Chan6+17
	ld		bc, (TRACK_pointer6)
	call	decode_data_chan
	ld		(TRACK_pointer6), bc
	ld		a, d
	ld		(TRACK_Chan6+17+TRACK_Flags), a	

decode_data.decode7:
	ld 		hl, #TRACK_Chan7+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode8

	; ld		iyh, #0x60
	.db		0xFD, 0x26, 0x60
	ld		a, (TRACK_Chan7+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan7+17+TRACK_Note)	
	ld		ix, #TRACK_Chan7+17
	ld		bc, (TRACK_pointer7)
	call	decode_data_chan
	ld		(TRACK_pointer7), bc
	ld		a, d
	ld		(TRACK_Chan7+17+TRACK_Flags), a	

decode_data.decode8:
	ld 		hl, #TRACK_Chan8+17+TRACK_Delay
	dec		(hl)
	jp		nz, decode_data.decode_end

	ld		a, (TRACK_Chan8+17+TRACK_Flags)
	ld		d, a
	ld		a, (TRACK_Chan8+17+TRACK_Note)	
	ld		ix, #TRACK_Chan8+17
	ld		bc, (TRACK_pointer8)
	call	decode_data_chan
	ld		(TRACK_pointer8), bc
	ld		a, d
	ld		(TRACK_Chan8+17+TRACK_Flags), a

decode_data.decode_end:
	; continue to process data
;===========================================================
; ---	process_data
; Process changes.
; 
; 
;===========================================================
process_data:
	;---- morph routine here
	ld		a, (replay_morph_active)
	and		a
	call	nz, replay_process_morph

	;--- Initialize PSG Mixer and volume
	xor		a
	ld		(SCC_regMIXER), a

	;--- PSG balance
	ld		hl, (replay_mainPSGvol)
	ld		(replay_mainvol), hl

	;--------------------
	;--- Process track 1
	;--------------------
	ld		ix, #TRACK_Chan1+17
	ld		a, (TRACK_Chan1+17+TRACK_Flags)
	ld		d, a
	call	process_data_chan
	jp		nc, process_data.skipa
	ld		(PSG_regToneA), hl
process_data.skipa:
	ld		a, (SCC_regVOLE)
	ld		(PSG_regVOLA), a	

	;--------------------
	;--- Process track 2
	;--------------------
	ld		ix, #TRACK_Chan2+17
	ld		a, (TRACK_Chan2+17+TRACK_Flags)
	ld		d, a
	call	process_data_chan
	jp		nc, process_data.skipb
	ld		(PSG_regToneB), hl
process_data.skipb:
	ld		a, (SCC_regVOLE)
	ld		(PSG_regVOLB), a	

	;--------------------
	;--- Process track 3	
	;--------------------
	ld		ix, #TRACK_Chan3+17
	ld		a, (TRACK_Chan3+17+TRACK_Flags)
	ld		d, a
	call	process_data_chan
	jp		nc, process_data.skipc
	ld		(PSG_regToneC), hl
process_data.skipc:
	ld		a, (SCC_regVOLE)
	ld		(PSG_regVOLC), a
	
	;--- To disable track 3 just comment above lines (9 lines) and enable below 2 lines.
	;	This can be done for all tracks.
;//	ld		hl, SCC_regMIXER 
;//	srl		(hl)

	;-- Convert mixer to AY
	ld		a, (SCC_regMIXER)		
	srl		a
	srl		a
	xor		#0x3f
	ld		(PSG_regMIXER), a
	xor		a
;//	ld		a, 11100011b
	ld		(SCC_regMIXER), a
	
	;---	 set SCC balance
	ld		hl, (replay_mainSCCvol)
	ld		(replay_mainvol), hl

	
	; ld		iyh, 0			; iyh stores the SCC chan#
	.db		0xFD, 0x26, 0x00
					; used for waveform updates
	;--------------------
	;--- Process track 4
	;--------------------
	ld		ix, #TRACK_Chan4+17
	ld		a, (TRACK_Chan4+17+TRACK_Flags)
	ld		d, a
;//	ld		hl, SCC_regToneA
	call	process_data_chan
	jp		nc, process_data.skipd
	ld		(SCC_regToneA), hl
process_data.skipd:
	ld		a, (SCC_regVOLE)
	ld		(SCC_regVOLA), a	

	;--------------------
	;--- Process track 5
	;--------------------
	; ld	iyh, 0x20
	.db		0xFD, 0x26, 0x20
	ld		ix, #TRACK_Chan5+17
	ld		a, (TRACK_Chan5+17+TRACK_Flags)
	ld		d, a
;//	ld	hl, SCC_regToneB
	call	process_data_chan
	jp		nc, process_data.skipe
	ld		(SCC_regToneB), hl
process_data.skipe:
	ld		a, (SCC_regVOLE)
	ld		(SCC_regVOLB), a	


	;--------------------
	;--- Process track 6
	;--------------------
	; ld	iyh, 0x40
	.db		0xFD, 0x26, 0x40
	ld		ix, #TRACK_Chan6+17
	ld		a, (TRACK_Chan6+17+TRACK_Flags)
	ld		d, a
;//	ld		hl, SCC_regToneC
	call	process_data_chan
	jp		nc, process_data.skipf
	ld		(SCC_regToneC), hl
process_data.skipf:
	ld		a, (SCC_regVOLE)
	ld		(SCC_regVOLC), a	

	;--------------------
	;--- Process track 7
	;--------------------
	; ld	iyh, 0x60
	.db		0xFD, 0x26, 0x60
	ld		ix, #TRACK_Chan7+17
	ld		a, (TRACK_Chan7+17+TRACK_Flags)
	ld		d, a
;//	ld		hl, SCC_regToneD
	call	process_data_chan
	jp		nc, process_data.skipg
	ld		(SCC_regToneD), hl
process_data.skipg:
	ld		a, (SCC_regVOLE)
	ld		(SCC_regVOLD), a		

	;--------------------
	;--- Process track 8
	;--------------------
							; no waveform for SCC channel 5
		
	ld		ix, #TRACK_Chan8+17
	ld		a, (TRACK_Chan8+17+TRACK_Flags)
	ld		d, a
;//	ld		hl, SCC_regToneE
	call	process_data_chan
	jp		nc, process_data.skiph
	ld		(SCC_regToneE), hl
process_data.skiph:

	;--------------------
	; Fade out processing
	;--------------------
	ld		a, (replay_fade)
	and		a
	ret		z	; if replay fade = 0 then no fade active
	
	; decrease fade timer
	ld		a, (replay_fade_timer)
	dec		a
	jp		nz, process_data.no_new_step

	ld		a, (replay_fade_vol)
	inc		a
	cp		#16
	jp		c, process_data.cont_fadeing
	xor		a
	ld		(replay_fade), a
	jp		replay_pause
process_data.cont_fadeing:
	ld		(replay_fade_vol), a
	ld		a, (replay_fade)
process_data.no_new_step:
	ld		(replay_fade_timer), a

	ld		a, (replay_fade_vol)
	ld		c, a
	ld		b, #3
	ld		hl, #PSG_regVOLA
	call	process_data.calc_vol
	ld		b, #5
	ld		hl, #SCC_regVOLA

process_data.calc_vol:
	ld		a, (hl)
	sub		c
	jp		nc, process_data.no_carry
	xor		a
process_data.no_carry:
	ld		(hl), a
	inc		hl
	djnz	process_data.calc_vol
	ret


;--------------------
	
	
;--------------------
_replay_check_patternend:
	ld		a, (TRACK_Chan1+17+TRACK_Delay)
	dec		a
	jp		nz, process_data

	ld		hl, (TRACK_pointer1)
	ld		a, (hl)

	;--- check for end of pattern
	cp		#191	
	jp		nz, process_data

	;--- Set track pointers to start
	ld		hl, (replay_orderpointer)
	xor		a
	cp		(hl)
	jp		nz, _replay_check_patternend.no_restart
	inc		hl
	cp		(hl)
	dec		hl
	jp		nz, _replay_check_patternend.no_restart
	;--- next is restart order
	inc		hl
	inc		hl
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a
_replay_check_patternend.no_restart:
	ld		de, #TRACK_pointer1
	ld		bc, #16
	ldir
	ld		(replay_orderpointer), hl		; store pointer for next set
											; of strack pointers

	jp		process_data


;===========================================================
; ---	decode_data_chan
; Process the channel data
; 
; in BC is the pointer to the	data
;	D contains flags.
;===========================================================
decode_data_chan:

	;--- initialize data
	ld		(replay_previous_note), a

	;=============
	; Note 
	;=============
	ld		a, (bc)
	cp		#_REL
	jp		c, _replay_decode_note
	cp		#_SUS
	jp		c, _replay_decode_release
	jp		z, _replay_decode_sustain
_rdn2:
	cp		#_INS
	jp		c, _replay_decode_vol
_rdv2:
	cp		#_CMD
	jp		c, _replay_decode_ins
_rdi2:
	cp		#_EOT
	jp		c, _replaydecode_cmd
	jp		_replay_decode_delay

_rdn:
	cp		#_SUS+1		
	jp		c, _rd_delay
	jp		_rdn2

_rdi:
	cp		#_CMD
	jp		c, _rd_delay
	jp		_rdi2

_rdv:
	cp		#_INS
	jp		c, _rd_delay
	jp		_rdv2
_rdc:
	inc		bc
_rdc_noinc:	
	ld		a, (bc)
	cp		#_EOT
	jp		c, _rd_delay
	jp		_replay_decode_delay

	;--- re-init previous delay	
_rd_delay:
	ld		a, TRACK_prevDelay(ix)
	ld		TRACK_Delay(ix), a
	jp		_replay_decode_trigger_porttone_check

_rd_eot:
	inc		a
	ld		TRACK_Delay(ix), a
	jp		_replay_decode_trigger_porttone_check

_replay_decode_delay:
	sub		#_WAIT-1
	jp		z, _rd_eot			; EOT found 
	ld		TRACK_Delay(ix), a
	ld		TRACK_prevDelay(ix), a
	inc		bc

_replay_decode_trigger_porttone_check:
	;--- Check cmd active and note trigger?
	bit 	B_TRGNOT, d
	ret		z
	bit		B_TRGCMD, d
	ret		z
	;--- Check for cmd3 or cmd5 (value 0 or 1) to continue
	ld		a, TRACK_Command(ix)
	cp		#2
	ret		nc
	;-- trigger CMD
	res		B_TRGNOT, d
	set		B_ACTNOT, d
	ld		a, TRACK_cmd_3(ix)
	jp		decode_cmd3_port_tone_new_note

_replay_decode_note:
	ld		TRACK_Note(ix), a
	set		B_TRGNOT, d
	res		B_ACTMOR, d

	inc		bc
	ld		a, (bc)
	jp		_rdn

_replay_decode_release:
_replay_decode_sustain:
	res		B_ACTNOT, d				; reset note bit to	0
	inc		bc
	ld		a, (bc)
	jp		_rdn

_replay_decode_ins:
	res		B_ACTMOR, d				; reset morph slave mode
	sub		#_INS
	cp		TRACK_Instrument(ix)
	jp		z, .skip_ins

	;--- instrument change found
	ld		TRACK_Instrument(ix), a

	;--- set instrument pointer
	add		a
	ld		hl, (replay_insbase)
	add		a, l
	ld		l, a
	jp		nc, .skip
	inc		h
.skip:
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a

	;-- get waveform
	ld		a, (hl)
	inc		hl
	
	cp		TRACK_Waveform(ix)
	jp		z, .skip_wav_trig
	set		B_TRGWAV, d
	ld		TRACK_Waveform(ix), a

.skip_wav_trig:
	;--- Store the macro start
	ld		TRACK_MacroPointer(ix), l
	ld		TRACK_MacroPointer+1(ix), h
	;--- Store the macro re-start
	ld		TRACK_MacroStart(ix), l
	ld		TRACK_MacroStart+1(ix), h

.skip_ins:	
	inc		bc
	ld		a, (bc)
	jp		_rdi

_replay_decode_vol:
	sub		#_VOL
	add		a
	add		a
	add		a
	add		a

	;--- Set new base volume (high byte) but keep relative offset (low byte)
	ld		e, a
	ld		a, TRACK_Volume(ix)
	and		#0x0F
	or		e
	ld		TRACK_Volume(ix), a

	inc		bc
	ld		a, (bc)
	jp		_rdv


_replaydecode_cmd:
	sub		#_CMD

	ld		e, a				; store command for later
	ld		hl, #DECODE_CMDLIST
	add		a, a
	add		a, l
	ld		l, a
	jp		nc, _replaydecode_cmd.skip
	inc		h
_replaydecode_cmd.skip:
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a
	
	inc		bc
	ld		a, (bc)
	jp		(hl)


DECODE_CMDLIST:
	; Primary
	.dw		decode_cmd3_port_tone
	.dw		decode_cmd5_vibrato_port_tone
	.dw		decode_cmd2_port_down
	.dw		decode_cmd0_arpeggio
	.dw		decode_cmd4_vibrato
	.dw		decode_cmd1_port_up
	.dw		decode_cmd6_vibrato_vol
	.dw		decode_cmd7_vol_slide
	.dw		decode_cmd8_tremolo
	.dw		decode_cmd9_note_cut
	.dw		decode_cmd10_note_delay
	; Secondary
	.dw		decode_cmd11_command_end
	.dw		decode_cmd12_SCC_morph
	.dw		decode_cmd13_arp_speed
	.dw		decode_cmd14_fine_up
	.dw		decode_cmd15_fine_down
	.dw		decode_cmd16_notelink
	.dw		decode_cmd17_track_detune
	.dw		decode_cmd18_trigger
	.dw		decode_cmd19_speed
	; SoundChip Specific
	.dw		decode_cmd20_envelope_high
	.dw		decode_cmd21_envelope_low

	.dw		decode_cmd22_SCC_reset
	.dw		decode_cmd23_SCC_duty
	.dw		decode_cmd24_SCC_soften
	.dw		decode_cmd25_SCC_waveform
	.dw		decode_cmd26_SCC_morph_copy
	.dw		decode_cmd27_SCC_morph_type
	.dw		decode_cmd28_SCC_morph_speed

	.dw		decode_cmd29_SCC_sample


decode_cmd0_arpeggio:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	ld		TRACK_cmd_0(ix), a
	ld		TRACK_Command(ix), e	
	set		B_TRGCMD, d
	ld		TRACK_Timer(ix), #0
	ld		TRACK_Step(ix), #2
	jp		_rdc


decode_cmd1_port_up:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	ld		TRACK_Command(ix), e
	ld		TRACK_cmd_1(ix), a
	set		B_TRGCMD, d
	jp		_rdc


decode_cmd2_port_down:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	ld		TRACK_Command(ix), e
	ld		TRACK_cmd_2(ix), a
	set		B_TRGCMD, d
	jp 		_rdc


decode_cmd3_port_tone:
	; in:	[A] contains the param value
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	ld		TRACK_Command(ix), e
	set		B_TRGCMD, d
	set		B_ACTNOT, d
	
	ld		TRACK_cmd_3(ix), a
	ld		TRACK_Timer(ix), #2

	;--- Check if we have a	note on the	same event
	bit		B_TRGNOT, d
	jp		z, _rdc

	res 	B_TRGNOT, d
	
	call	decode_cmd3_port_tone_new_note
	jp		_rdc

decode_cmd3_port_tone_new_note:
	;-- remove deviation from parameter
	and 	#0x7f
	ex		af, af'		;'
	exx

	;-- get the	previous note freq
	ld		a, (replay_previous_note)
	add		a
	ld		hl, (replay_tonetable)	;TRACK_ToneTable
	add		a, l
	ld		l, a
	jp		nc, decode_cmd3_port_tone_new_note.skip
	inc		h
decode_cmd3_port_tone_new_note.skip:
	ld		e, (hl)
	inc		hl
	ld		d, (hl)

	; add	the toneadd
	ld		l, TRACK_cmd_ToneSlideAdd(ix)
	ld		h, TRACK_cmd_ToneSlideAdd+1(ix)

	add		hl, de	
	ex		de, hl				; store current freq in	[de]
	;--- get the current note freq
	ld		a, TRACK_Note(ix)
	add		a
	ld		hl, (replay_tonetable)	;TRACK_ToneTable
	add		a, l
	ld		l, a
	jp		nc, decode_cmd3_port_tone_new_note.skip2
	inc		h
	ccf
decode_cmd3_port_tone_new_note.skip2:
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a				; destination freq in [hl]
	
	;--- Calculate the delta
	ex		de, hl
	sbc		hl, de				; results in pos/neg delta
	
	ld		TRACK_cmd_ToneSlideAdd(ix), l
	ld		TRACK_cmd_ToneSlideAdd+1(ix), h	

	;--- re-apply deviation
	ex		af, af'			;'
	bit		7, h
	jp		nz, decode_cmd3_port_tone_new_note.skip3
	or 		#128
decode_cmd3_port_tone_new_note.skip3:
	ld 		TRACK_cmd_3(ix), a
	exx					; restore flags in D
	ret


decode_cmd8_tremolo:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------	


decode_cmd4_vibrato:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	; Vibrato with speed x and depth y.	This command 
	; will oscillate the frequency of the current note
	; with a sine wave. 
	;
	; high 3 bits is depth (0-7) (direct offset in sinetable)
	; low 5 bits is the speed. 1-16
	;--- Init values
	ld		TRACK_Command(ix), e
	ld		e, a

	;--- Set the speed
	and		#0x0F
	jp		z, decode_cmd4_vibrato.depth
	ld		TRACK_cmd_4_step(ix), a	
	neg	
	ld		TRACK_Step(ix), a
decode_cmd4_vibrato.depth:
	;-- set the depth
	ld		a, e
	and		#0xF0
	jp		z, decode_cmd4_vibrato.end		; set depth when 0 only when command was not active.

	sub		#16
	ld		hl, #TRACK_Vibrato_sine;-16
	add		a, a
	jp		nc, decode_cmd4_vibrato.skip1
	inc		h
decode_cmd4_vibrato.skip1:	
	add		a, l
	ld 		l, a
	jp		nc, decode_cmd4_vibrato.skip2
	inc		h
decode_cmd4_vibrato.skip2:
	ld		TRACK_cmd_4_depth(ix), l
	ld		TRACK_cmd_4_depth+1(ix), h

decode_cmd4_vibrato.end:	
	set		B_TRGCMD, d
	jp		_rdc	


decode_cmd5_vibrato_port_tone:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	; portTone	+ volumeslide
	;--- Init values
	ld		TRACK_Command(ix), e
	ld		TRACK_cmd_A(ix), a
	set		B_TRGCMD, d
	
	;-- Check if new note
	bit	B_TRGNOT, d
	jp		z, _rdc
	
	;-- Set new port tone value
	res		B_TRGNOT, d
	ld		a, TRACK_cmd_3(ix)
	jp		decode_cmd3_port_tone_new_note


decode_cmd6_vibrato_vol:
	; ; in:	[A] contains the paramvalue
	; ; 
	; ; ! do not change	[BC] this is the data pointer
	; ;--------------------------------------------------


decode_cmd7_vol_slide:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	ld		TRACK_Command(ix), e
	ld		TRACK_cmd_A(ix), a
	and 	#0x7F
	ld		TRACK_Timer(ix), a
	set		B_TRGCMD, d
	jp		_rdc


decode_cmd9_note_cut:
	set		B_TRGCMD, d
	ld		TRACK_Command(ix), e
	ld		TRACK_Timer(ix), a				; set	the timer to param y
	jp 		_rdc


decode_cmd10_note_delay:
	bit		B_TRGNOT, d						; is there a note	in this eventstep?
	jp		z, _rdc

	set		B_TRGCMD, d						; command active
	res		B_TRGNOT, d						; reset any	trigger note
	ld		TRACK_Command(ix), e
	
	ld		TRACK_Timer(ix), a				; set	the timer to param y
	ld		a, TRACK_Note(ix)
	ld		TRACK_cmd_E(ix), a				; store the	new note
	ld		a, (replay_previous_note)
	ld		TRACK_Note(ix), a				; restore the old	note
	jp		_rdc	


decode_cmd11_command_end:
	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	res		B_TRGCMD, d
	jp		_rdc_noinc


decode_cmd13_arp_speed:
	ld		(replay_arp_speed), a		; store the halve not to add
	jp		_rdc	


decode_cmd14_fine_up:
	ld		TRACK_cmd_ToneSlideAdd(ix), a
	ld		TRACK_cmd_ToneSlideAdd+1(ix), #0
	jp		_rdc


decode_cmd15_fine_down:
	ld		TRACK_cmd_ToneSlideAdd(ix), a
	ld		TRACK_cmd_ToneSlideAdd+1(ix), #0xff
	jp		_rdc


decode_cmd16_notelink:
	res		B_TRGNOT, d
	jp		_rdc_noinc


decode_cmd17_track_detune:
	; This command sets the	detune of the track.
	ld		e, a
	and		#0x07		; low	4 bits is value
	bit		3, e		; Center around 8
	jp		z, decode_cmd17_track_detune.pos
	cpl
	ld		TRACK_cmd_detune(ix), a
	ld		TRACK_cmd_detune+1(ix), #0xff
	jp		_rdc	
decode_cmd17_track_detune.pos:
	ld		TRACK_cmd_detune(ix), a
	ld		TRACK_cmd_detune+1(ix), #0x00	
	jp		_rdc	


decode_cmd18_trigger:
	ld		(replay_trigger), a
	jp		_rdc		


decode_cmd19_speed:
	ld		(replay_speed), a
	;--- Reset Timer == 0
	srl		a				; divide speed with 2
	ld		e, a
	ld		a, #0
	adc		a				; store carry of shift as subtimer
	ld		(replay_speed_subtimer), a
	add		a, e
	ld		(replay_speed_timer), a
	jp		_rdc	


	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	; This command set the envelope frequency using a
	; multiplier value (00-ff)
decode_cmd20_envelope_high:
	ld		(PSG_regEnvH), a
	jp		_rdc	


	; in:	[A] contains the paramvalue
	; 
	; ! do not change	[BC] this is the data pointer
	;--------------------------------------------------
	; This command set the envelope frequency using a
	; multiplier value (00-ff)
decode_cmd21_envelope_low:
	ld		(PSG_regEnvL), a
	jp		_rdc	


decode_cmd22_SCC_reset:
	set		B_TRGWAV, d
	res		B_ACTMOR, d
	;--- Look up the waveform form the instrument.
	ld		l, TRACK_MacroPointer(ix)
	ld		h, TRACK_MacroPointer+1(ix)
	inc		hl
	inc		hl
	ld		a, (hl)
	ld		TRACK_Waveform(ix), a
	ret
	jp		_rdc_noinc


decode_cmd23_SCC_duty:
	;=================
	; Waveform PWM / Duty Cycle
	; Do not overwrite [BC] and [D](flags)
	;=================
	res		B_ACTMOR, d			;TRACK_Flags(ix)	; reset morph
	res		B_TRGWAV, d			;TRACK_Flags(ix)	; reset normal wave update

	ld		e, a		; store in length in E for later		
	ex		af, af' ;'
	;get the waveform	start	in [DE]
	ld		hl, #_0x9800
	; ld		a, iyh		;ixh contains chan#
	.db		0xFD, 0x7C
	add		a, l
	ld		l, a
	jp		nc, decode_cmd23_SCC_duty.skip
	inc		h
decode_cmd23_SCC_duty.skip:
	ld		a, e		
	ex		af, af'	;' store for later
	inc		a
	ld		e, #0x77		; store for the other loop
decode_cmd23_SCC_duty.wspw_loop_h:
	ld		(hl), e
	inc		hl
	dec		a
	jp		nz, decode_cmd23_SCC_duty.wspw_loop_h

	ex		af, af'	;' restore the initial length
	sub		#31

	ld		e, #0x87
decode_cmd23_SCC_duty.wspw_loop_l:
	ld		(hl), e
	inc		hl
	dec		a
	jp		nz, decode_cmd23_SCC_duty.wspw_loop_l

	jp		_rdc


decode_cmd24_SCC_soften:
	;=================
	; Waveform Soften
	; Do not overwrite [BC] and [D](flags)
	;=================
	res		B_ACTMOR, d			;TRACK_Flags(ix)	; reset morph
	res		B_TRGWAV, d			;TRACK_Flags(ix)	; reset normal wave update

	;get the waveform	start	in [hl]
	ld		a, TRACK_Waveform(ix)
	ld		l, a
	ld		h, #0
	add		hl, hl
	add		hl, hl

	ld		a, d			; Preserve flags
	ex		af, af'		;'
	
	ld		de, (replay_wavebase)
	add		hl, de	

	;get the waveform destination address in [DE]
	ld		de, #_0x9800
	; ld		a, iyh		;ixh contains chan#
	.db		0xFD, 0x7C
	add		a, e
	ld		e, a
	jp		nc, decode_cmd24_SCC_soften.skip
	inc		d
decode_cmd24_SCC_soften.skip:
	; ld		iyl, #32
	.db		0xFD, 0x26, 32
decode_cmd24_SCC_soften.softloop:	
	ld		a, (hl)
	sra		a
	ld		(de), a
	; dec		iyl
	.db		0xFD, 0x2D
	jp		z, decode_cmd24_SCC_soften.end
	inc		hl
	inc		de
	jp		decode_cmd24_SCC_soften.softloop
decode_cmd24_SCC_soften.end:	
	ex		af, af'		;'
	ld		d, a		; restore flags
	jp		_rdc_noinc


	;=================
	; Waveform Set
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd25_SCC_waveform:
	ld		TRACK_Waveform(ix), a
	set		B_TRGWAV, d
	res		B_ACTMOR, d
	jp		_rdc


	;=================
	; Waveform Morph
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd12_SCC_morph:
	push	bc			; Preserve pointer

	ld		(replay_morph_waveform), a
	xor		a
	ld 		(replay_morph_counter), a
	inc		a
	ld		(replay_morph_timer), a
	ld		a, (replay_morph_type)
	and		a
	jp		z, decode_cmd12_SCC_morph.morph_fromtrack
decode_cmd12_SCC_morph.morph_fromregister:
	;--- Set HL to the buffer of the last written waveform
	ld		h, #0x98
	; ld		a, iyh
	.db		0xFD, 0x7C
	ld		l, a
	jp		decode_cmd12_SCC_morph.morph_copy

decode_cmd12_SCC_morph.morph_fromtrack:
	;--- Set HL to the waveform set by instrument or B1y or B2y
	ld		l, TRACK_Waveform(ix)
	ld		h, #0
	add		hl, hl
	add		hl, hl

	ld		bc, (replay_wavebase)
	add		hl, bc

decode_cmd12_SCC_morph.morph_copy:
	ld		bc, #replay_morph_buffer
	ld		a, #32
decode_cmd12_SCC_morph44:
	ex		af, af'	;'
	ld		a, (hl)
	inc		bc		; copy to value (skip delta value byte)
	ld		(bc), a	
	inc		hl
	inc		bc
	ex		af, af'	;'
	dec		a
	jp		nz, decode_cmd12_SCC_morph44 ; 44b

	;--- calculate the delta s
	ld		a, #255					; 255 triggers calc init
	ld		(replay_morph_active), a
	set		B_ACTMOR, d
	pop		bc						; Restore the pointer
	jp		_rdc


	;================= 
	; Waveform Morph copy /follow
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd26_SCC_morph_copy:
	set		B_ACTMOR, d
	jp		_rdc_noinc


	;=================
	; Waveform Morph type
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd27_SCC_morph_type:
	ld		(replay_morph_type), a
	jp		_rdc	


	;=================
	; Waveform Morph speed
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd28_SCC_morph_speed:
	ld		(replay_morph_speed), a
	jp		_rdc	

	;=================
	; Waveform Morph
	; Do not overwrite [BC] and [D](flags)
	;=================
decode_cmd29_SCC_sample:
	jp		_rdc	


;===========================================================
; ---process_data_chan
; Process the cmd/instrument/note and vol data 
; 
; in HL is the current tone freq
; in D is TRACK_FLAGS 
;===========================================================
process_data_chan:
	;-- set the	mixer	right
	ld		hl, #SCC_regMIXER 
	rrc		(hl)
		
	;=====
	; COMMAND
	;=====
	ld		TRACK_cmd_NoteAdd(ix), #0			; Always reset note add
	bit		B_TRGCMD, d	;TRACK_Flags(ix)
	jr		z, process_note
	
	ld		hl, #PROCESS_CMDLIST
	ld		a, TRACK_Command(ix)

	add		a
	add		a, l
	ld		l, a
	jp		nc, process_data_chan.skip
	inc		h
process_data_chan.skip:
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a	
	jp		(hl)


process_note:
process_commandEND:

	;=====
	; Note
	;=====
	;--- Check if we need to trigger a new note
	bit		B_TRGNOT, d	;TRACK_Flags(ix)
	jp		z, process_noNoteTrigger


process_triggerNote:
	;--- get new Note
	res		B_TRGNOT, d		;TRACK_Flags(ix)		; reset trigger note flag
	set		B_ACTNOT, d		;TRACK_Flags(ix)		; set	note active	flag

	ld		l, TRACK_MacroStart(ix)
	ld		h, TRACK_MacroStart+1(ix)
;	;--- Store the macro start	
	ld		TRACK_MacroPointer(ix), l
	ld		TRACK_MacroPointer+1(ix), h	

	ld		(_SP_Storage), sp
	ld		sp, ix				
	pop		af				; Move just after TRACK_cmd_ToneAdd
	pop		af	

	ld		hl, #0
	push	hl				; clear TRACK_cmd_ToneAdd
	push	hl				; clear TRACK_cmd_ToneSlideAdd
	push	hl				; clear TRACK_cmd_VolumeAdd + TRACK_Noise
	push	hl				; clear TRACK_VolumeAdd +TRACK_ToneAdd (high)
	push	hl				; clear TRACK_ToneAdd(low)+ TRACK_empty
	ld		sp, (_SP_Storage)

process_noNoteTrigger:
	;==============
	; Macro instrument
	;==============
	bit		B_ACTNOT, d			;TRACK_Flags(ix)
	ld		TRACK_Flags(ix), d
	jp		z, process_noNoteActive
	
	;--- Get the macro len and loop
	ld		e, TRACK_MacroPointer(ix)
	ld		d, TRACK_MacroPointer+1(ix)

process_macro:
	ld		a, (de)
	inc		de
	and		a
	jp		z, macro_volume_same

	;--- Get macro action address
	ld		hl, #MACROACTIONLIST-2
	add		a, l
	ld		l, a
	jp		nc, process_macro.skip
	inc		h
process_macro.skip:
	ld		a, (hl)
	inc		hl
	ld		h, (hl)
	ld		l, a
	jp		(hl)


MACROACTIONLIST:
	.dw 	macro_mixer				; 2
	.dw 	macro_tone_add			; 4
	.dw		0						; unused
;//	.dw 	macro_tone_sub			; 6
	.dw 	macro_vol_base			; 8
	.dw 	macro_vol_add			; a
	.dw 	macro_vol_sub			; c
	.dw 	macro_noise_base		; e
	.dw		0						;unused
;//	.dw 	macro_noise_add			; 10	; unused
	.dw 	macro_noise_add			; 12
	.dw 	macro_noise_vol			; 14
	.dw 	macro_voice				; 16
	.dw 	macro_loop				; 18
	.dw		macro_envelope			; 1a
	.dw		macro_envelope_shape	; 1c


macro_mixer:
	ld		a, (de)
	inc		de
	ld		b, a
	ld		a, (SCC_regMIXER) 
	or		b
	ld		(SCC_regMIXER), a
	jp		process_macro


macro_tone_add:
	ld		a, (de)
	inc		de
	add		TRACK_ToneAdd(ix)
	ld		TRACK_ToneAdd(ix), a
	ld		a, (de)
	adc 	TRACK_ToneAdd+1(ix)
	ld		TRACK_ToneAdd+1(ix), a
	inc		de
	jp		process_macro


;macro_tone_sub:
;	ld	a, (de)
;	ld	c, a
;	inc de
;	ld	a, (de)
;	ld	b, a
;	inc de
;	ld	l, TRACK_ToneAdd(ix)
;	ld	h, TRACK_ToneAdd+1(ix)
;	add hl, bc
;	ld	TRACK_ToneAdd(ix), l
;	ld	TRACK_ToneAdd+1(ix), h
;	jp	process_macro


macro_noise_base:
	ld		a, (de)
	inc 	de
	ld		TRACK_Noise(ix), a
	;ld		(PSG_regNOISE), a
	ld		(replay_noise), a
	jp		process_macro

;macro_noise_sub:
macro_noise_add:
	ld		a, (de)
	inc 	de
	add 	TRACK_Noise(ix)
	ld		TRACK_Noise(ix), a
	;ld		(PSG_regNOISE), a
	ld		(replay_noise), a
	jp		process_macro


macro_noise_vol:
	inc		de
	jp		process_macro


macro_voice:
	ld		a, (de)
	inc		de
	ld		TRACK_Waveform(ix), a
	;---- check this code perhaps this needs more checks.
	set		B_TRGWAV, TRACK_Flags(ix)
	res		B_ACTMOR, TRACK_Flags(ix)
	jp		process_macro


macro_envelope_shape:
	ld		a, (de)
	inc		de
	ld		(PSG_regEnvShape), a


macro_envelope:
	ld		a, #16			; set volume to 16 == envelope
	ld		(SCC_regVOLE), a
	jp		_macro_end


macro_loop:
	ex		de, hl
	ld		e, (hl)
	ld		d, #0xff
	add		hl, de
	ex		de, hl
	jp		process_macro


macro_volume_same:
	ld		a, TRACK_VolumeAdd(ix)
	jp		_macro_set_volume


macro_vol_base:
	ld		a, (de)
	inc 	de
	ld		TRACK_VolumeAdd(ix), a
	jp		_macro_set_volume


macro_vol_add:
	ld		a, (de)
	inc 	de
	add 	TRACK_VolumeAdd(ix)
	cp		#16
	jp		c, macro_vol_add.nolimit
	ld		a, #0x0f
macro_vol_add.nolimit:
	ld		TRACK_VolumeAdd(ix), a
	jp		_macro_set_volume


macro_vol_sub:
	ld		a, (de)
	ld		h, a
	inc 	de
	ld		a, TRACK_VolumeAdd(ix)
	sub 	h
	jp		nc, macro_vol_sub.nolimit
	xor 	a
macro_vol_sub.nolimit:
	ld		TRACK_VolumeAdd(ix), a

;-- Set the output volume directly after any update
_macro_set_volume:
	or		TRACK_Volume(ix)

.if TREMOLO_OFF
.else	
	ld 		b, TRACK_cmd_VolumeAdd(ix)
	sub		a, b
	jp 		nc, _macro_set_volume.skip2
	add		a, b
 	and		#0x0f
.endif
_macro_set_volume.skip2:
	;--- apply main volume balance
	ld		bc, (replay_mainvol)
	add		a, c
	ld		c, a
	jp		nc, _macro_set_volume.skip
	inc		b
_macro_set_volume.skip:
	ld		a, (bc)	
	; Test which CHIP.
	bit		B_PSGSCC, TRACK_Flags(ix)
	jp		nz, _macro_set_volume.skip3
	rra
	rra
	rra
	rra
_macro_set_volume.skip3:
	and		#0x0f
	ld		(SCC_regVOLE), a


_macro_end:
	;--- Store macro pointer
	ld		TRACK_MacroPointer(ix), e		;--- store pointer for next time
	ld		TRACK_MacroPointer+1(ix), d	

	;-- Get the current note
	ld		a, TRACK_Note(ix)
	add		a, TRACK_cmd_NoteAdd(ix)
	add		a

	ld		hl, (replay_tonetable)
	add		a, l
	ld		l, a
	jp		nc, _macro_end.skip
	inc		h
_macro_end.skip:
	ld		a, (hl)	;--- Store the note tone value in hl
	inc		hl
	ld		h, (hl)
	ld		l, a

	; NOTE Check deze BC nog eens goed na.	
	ld		c, TRACK_ToneAdd(ix)
	ld		b, TRACK_ToneAdd+1(ix)	
	add		hl, bc		;--- Store tone deviation		

	;-- set the detune.
	ld		(_SP_Storage), sp
	ld		sp, ix
	pop		bc		; cmd_ToneSlideAdd
	add		hl, bc
	pop		bc		; cmd_ToneAdd

	;--- Fix SCC persiod offset
	bit		B_PSGSCC, TRACK_Flags(ix)
	jp		z, _macro_end.tonePSG
_macro_end.toneSCC:
	dec		bc		; SCC tone is -1 of PSG tone
_macro_end.tonePSG:
	add		hl, bc
	pop		bc		; TRACK_cmd_detune
	add		hl, bc
	ld		sp, (_SP_Storage)

	;--- Return the tone register value
	scf		; no cary is update the register value
	ret


process_noNoteActive:
	;-- Silence
	xor		a			; also clears the carry flag.
	ld		(SCC_regVOLE), a
	ret


PROCESS_CMDLIST:
	; This list only contains the primary commands.
	.dw		process_cmd3_port_tone
	.dw		process_cmd5_vibrato_port_tone
	.dw		process_cmd2_port_down
	.dw		process_cmd0_arpeggio
	.dw		process_cmd4_vibrato
	.dw		process_cmd1_port_up
	.dw		process_cmd6_vibrato_vol
	.dw		process_cmd7_vol_slide
	.dw		process_cmd8_tremolo
	.dw		process_cmd9_note_cut
	.dw		process_cmd10_note_delay


process_cmd0_arpeggio:
	ld		a, TRACK_Timer(ix)
	and		a
	jp		z, process_cmd0_arpeggio.nextNote
	
	dec		a
	ld		TRACK_Timer(IX), a
	ld		a, TRACK_Step(ix)
	and		a
	jp		z, process_cmd0_arpeggio.skip
	ld		a, TRACK_cmd_0(IX)
	and		#0x0f
	ld		TRACK_cmd_NoteAdd(ix), a
	; jr		process_commandEND
	jp		process_commandEND
process_cmd0_arpeggio.skip:
	ld		TRACK_cmd_NoteAdd(ix), #0
	; jr		process_commandEND
	jp		process_commandEND

process_cmd0_arpeggio.nextNote:
	; re-init the speed.
	ld		a, (replay_arp_speed)
	ld		TRACK_Timer(IX), a
	
	ld		a, TRACK_Step(ix)
	and		a
	jr		nz, process_cmd0_arpeggio.skip1
	;--- set x
	ld		TRACK_Step(ix), #1
	ld		a, TRACK_cmd_0(ix)
	rlca
	rlca
	rlca
	rlca
	ld		TRACK_cmd_0(ix), a
	and		#0x0f
	ld		TRACK_cmd_NoteAdd(ix), a
	; jr		process_commandEND
	jp		process_commandEND

process_cmd0_arpeggio.skip1:
	dec		a
	jr		nz, process_cmd0_arpeggio.skip2
	;--- set y
	ld		TRACK_Step(ix), #2
	ld		a, TRACK_cmd_0(ix)
	rlca
	rlca
	rlca
	rlca
	ld		TRACK_cmd_0(ix), a
	and		#0x0f
	jp		nz, process_cmd0_arpeggio.zero
	;--- if zero then skip this note and set step to start
	ld		TRACK_Step(ix), #0
process_cmd0_arpeggio.zero:
	ld		TRACK_cmd_NoteAdd(ix), a
	; jr		process_commandEND
	jp		process_commandEND

process_cmd0_arpeggio.skip2:
	;--- set none
	ld		TRACK_Step(ix), #0
	ld		TRACK_cmd_NoteAdd(ix), #0
	; jr		process_commandEND
	jp		process_commandEND


process_cmd1_port_up:
	ld		a, TRACK_cmd_ToneSlideAdd(ix)
	sub		TRACK_cmd_1(ix)
	ld		TRACK_cmd_ToneSlideAdd(ix), a
	jp		nc, process_commandEND
	dec		TRACK_cmd_ToneSlideAdd+1(ix)
	jp		process_commandEND


process_cmd2_port_down:
	ld		a, TRACK_cmd_ToneSlideAdd(ix)
	add		TRACK_cmd_2(ix)
	ld		TRACK_cmd_ToneSlideAdd(ix), a
	jp		nc, process_commandEND
	inc		TRACK_cmd_ToneSlideAdd+1(ix)
	jp		process_commandEND


process_cmd3_port_tone:
	ld		a, TRACK_cmd_3(ix)
	ld		l, TRACK_cmd_ToneSlideAdd(ix)
	ld		h, TRACK_cmd_ToneSlideAdd+1(ix)
	bit		7, h
	jp		z, process_cmd3_sub
process_cmd3_add:
	;pos slide
	add		a, l
	ld		TRACK_cmd_ToneSlideAdd(ix), a
	jp		nc, process_cmd3_add.skip
	inc		h					
process_cmd3_add.skip:
	bit		7, h
	jp		z, process_cmd3_stop			; delta turned pos ?
	ld		TRACK_cmd_ToneSlideAdd+1(ix), h
	jp		process_commandEND
process_cmd3_sub:
	;negative slide	
	and		#0x7f
	ld		c, a
	xor		a
	ld		b, a
	sbc		hl, bc
	bit		7, h
	jp		nz, process_cmd3_stop			; delta turned neg ?
	ld		TRACK_cmd_ToneSlideAdd(ix), l
	ld		TRACK_cmd_ToneSlideAdd+1(ix), h
	jp		process_commandEND
process_cmd3_stop:	
	res		B_TRGCMD, d		;TRACK_Flags(ix)
	ld		TRACK_cmd_ToneSlideAdd(ix), #0
	ld		TRACK_cmd_ToneSlideAdd+1(ix), #0
	jp		process_commandEND



process_cmd8_tremolo:
.if TREMOLO_OFF
.else	
	;=================================
	;
	; tremolo	
	;
	;=================================	
	ld		l, TRACK_cmd_4_depth(ix)
	ld		h, TRACK_cmd_4_depth+1(ix)

	;--- Get next step
	ld		a, TRACK_Step(IX)
	add		TRACK_cmd_4_step(ix)
	and		#0x3F			; max	64
	ld		TRACK_Step(ix), a
	sra		a			; devide step by 2
	add		a, l
	ld		l, a
	jp		nc, process_cmd8_tremolo.skip
	inc		h
process_cmd8_tremolo.skip:
	ld		a, (hl)
	add		a
	add		a
	add		a
	add		a
	ld		TRACK_cmd_VolumeAdd(ix), a
.endif
	jp		process_commandEND



	;=================================
	;
	; Vibrato	
	;
	;=================================
process_cmd4_vibrato:
	ld		l, TRACK_cmd_4_depth(ix)
	ld		h, TRACK_cmd_4_depth+1(ix)

	;--- Get next step
	ld		a, TRACK_Step(IX)
	add		TRACK_cmd_4_step(ix)
	and		#0x3F			; max	64
	ld		TRACK_Step(ix), a
	
	bit		5, a			; step 32-63 the neg	
	jp		z, process_cmd4_vibrato.pos	
	
process_cmd4_vibrato.neg:
	and		#0x1f	; make it 32 steps again
	add		a, l
	ld		l, a
	jp		nc, process_cmd4_vibrato.skip1
	inc		h
process_cmd4_vibrato.skip1:
	ld		a, (hl)
	neg
	jp		z, process_cmd4_vibrato.zero			; 0xff00 gives strange result ;)	
	ld		TRACK_cmd_ToneAdd(ix), a
	ld		TRACK_cmd_ToneAdd+1(ix),# 0xff
	jp		process_commandEND	

process_cmd4_vibrato.pos:
	add		a, l
	ld		l, a
	jp		nc, process_cmd4_vibrato.skip2
	inc		h
process_cmd4_vibrato.skip2:
	ld		a, (hl)
process_cmd4_vibrato.zero:	
	ld		TRACK_cmd_ToneAdd(ix), a
	ld		TRACK_cmd_ToneAdd+1(ix), #0
	jp		process_commandEND


process_cmd5_vibrato_port_tone:
	call	process_cmdasub
	jp		process_cmd3_port_tone


process_cmd6_vibrato_vol:
	call	process_cmdasub
	jp		process_cmd4_vibrato	


process_cmd7_vol_slide:
	call	process_cmdasub
	jp		process_commandEND


process_cmdasub:
	dec		TRACK_Timer(ix)
	ret		nz
		
	; vol	slide
	ld		a, TRACK_cmd_A(ix)
	ld		c, a
	and		#0x7f
	ld		TRACK_Timer(ix), a

	ld		a, TRACK_Volume(IX)
	bit		7, c
	jp		z, process_cmda_inc
process_cmda_dec:
	and		a
	ret		z
	sub		#0x10
	ld		TRACK_Volume(ix), a
	ret
process_cmda_inc:
	cp		#0xf0
	ret		nc
	add		#0x10	
	ld		TRACK_Volume(ix), a
	ret


process_cmd9_note_cut:
	dec		TRACK_Timer(ix)
	jp		nz, process_commandEND
	
	; stop note
	res		B_TRGCMD, d	; set	note bit to	0
	res		B_ACTNOT, d
	jp		process_commandEND


process_cmd10_note_delay:
	; note delay
	dec		TRACK_Timer(ix)
	jp		nz, process_commandEND	; no delay yet

	; trigger note
	ld		a, TRACK_cmd_E(ix)
	ld		TRACK_Note(ix), a					; set	the note val
	set		B_TRGNOT, d	;TRACK_Flags(ix)		; set	trigger note flag
	res		B_TRGCMD, d	;TRACK_Flags(ix)		; reset trigger cmd flag
	
	jp		process_commandEND


;===========================================================
; ---replay_route
; Output the data	to the CHIP	registers
; 
; 
;===========================================================
replay_route::
;---------------
; P S G 
;---------------
	;--- Push values to AY HW
	ld		hl, #PSG_registers+13
	ld		bc, #0x0CA1			; 12 seq reg updates + port 0xa1

	;--- Envelope shape R#13
	ld		a, (hl)
	and		a				; Value to write?
	jp		z, _ptAY_noEnv	
	ld		d, a	
	ld		a, #0x0D
	out		(0xa0), a	
	out		(c), d
	ld		(hl), #0			;reset the envwrite	
_ptAY_noEnv:
	dec		hl
	ld		a, #0x0c				; Start at reg 0x0c 

	;--- Rolled out psg update 6 times
	out		(0xa0), a			; reg c
	dec		a
	outd
	out		(0xa0), a			; reg b
	dec		a
	outd
	out		(0xa0), a			; reg a
	dec		a
	outd
	out		(0xa0), a			; reg 9
	dec		a
	outd
	out		(0xa0), a			; reg 8
	dec		a
	outd
	out		(0xa0), a			; reg 7
	dec		a
	outd
	out		(0xa0), a			; reg 6
	outd
	dec		a
	ld		d, #0xff
_ptAY_loop:
	dec		a
	out		(0xa0), a
	out		(c), d
	inc		a
	out		(0xa0), a
	outd
	dec		a
	out		(0xa0), a
	outd
	dec		a
	jp		p, _ptAY_loop
;--------------
; S C C 
;--------------
	;// MSXgl - Start
	; ld		a, #0x3F				; enable SCC
	; ld		(0x9000), a
	;// MSXgl - End

	;--- Set the waveforms
	ld		a, (TRACK_Chan4+17+TRACK_Flags)
	bit		B_TRGWAV, a
	jp		z, _ptAY_loop.skip1

	;--- set wave form
	ld		de, #0x9800
	and		#0b10111111		;res	B_TRGWAV, a
	ld		(TRACK_Chan4+17+TRACK_Flags), a
	bit		B_ACTMOR, a
	call	nz, _write_SCC_special
	;--- write_SCC_special will handle the return to next skip [HACK]
	ld		a, (TRACK_Chan4+17+TRACK_Waveform)
	call	_write_SCC_wave
_ptAY_loop.skip1:
	ld		a, (TRACK_Chan5+17+TRACK_Flags)
	bit		B_TRGWAV, a
	jp		z, _ptAY_loop.skip2

	;--- set wave form
	ld		de, #0x9820
	and		#0b10111111		;res	B_TRGWAV, a
	ld		(TRACK_Chan5+17+TRACK_Flags), a
	bit		B_ACTMOR, a
	call	nz, _write_SCC_special
	;--- write_SCC_special will handle the return to next skip [HACK]
	ld		a, (TRACK_Chan5+17+TRACK_Waveform)
	call	_write_SCC_wave
_ptAY_loop.skip2:
	ld		a, (TRACK_Chan6+17+TRACK_Flags)
	bit		B_TRGWAV, a
	jp		z, _ptAY_loop.skip3

	;--- set wave form
	ld		de, #0x9840
	and		#0b10111111		;res	B_TRGWAV, a
	ld		(TRACK_Chan6+17+TRACK_Flags), a
	bit		B_ACTMOR, a
	call	nz, _write_SCC_special
	;--- write_SCC_special will handle the return to next skip [HACK]
	ld		a, (TRACK_Chan6+17+TRACK_Waveform)
	call	_write_SCC_wave
_ptAY_loop.skip3:
	ld		a, (TRACK_Chan7+17+TRACK_Flags)
	bit		B_TRGWAV, a
	jp		z, _ptAY_loop.skip4

	;--- set wave form
	ld		de, #0x9860
	and		#0b10111111		;res	B_TRGWAV, a
	ld		(TRACK_Chan7+17+TRACK_Flags), a
	bit		B_ACTMOR, a
	call	nz, _write_SCC_special
	;--- write_SCC_special will handle the return to next skip [HACK]
	ld		a, (TRACK_Chan7+17+TRACK_Waveform)
	call	_write_SCC_wave
_ptAY_loop.skip4:

scc_reg_update:
	
; .ifdef FPGA_SCC
	; ld		de, #0x9880
	; ld		hl, #SCC_registers
	; ;-- write 16 values
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
	; ldi
; .else

; ;--- Update changed SCC registers.
;	; fast update routine by SpaceMoai
	ld		hl, #oldregs+(3*5)
	ld		de, #SCC_registers+(3*5)
	ld		bc, #0x9880+(3*5)
scc_reg_update.loop:
	ld		a, (de)
	cp		(hl)
	jr		z, scc_reg_update.skip
	ld		(hl), a
	ld		(bc), a ; update scc registers
scc_reg_update.skip: 
	dec		de
	dec		hl
	dec		c
	jp		m, scc_reg_update.loop

	;--- Update changed SCC registers.
;//	ld hl, oldregs				
;//	ld de, SCC_registers
;//	ld bc, 0x9880
;//	ld a, (3*5)+1
;//.loop:
;//	ex af, af'	;'
;//	ld a, (de)
;//	cp (hl)					
;//	jr z, .skip					
;//	ld (hl), a		 				
;//	ld (bc), a		 				; update scc	registers
;//.skip:		
;//	inc hl					
;//	inc de
;//	inc bc
;//	ex af, af'		;'
;//	dec a
;//	jr nz, .loop
; .endif

	ret


;==================
; _write_SCC_wave
;
; Writes waveform data.
; [DE]	contains location for data
; [A]	contains waveform number + flags for special actions
; Data is not written to SCC but into RAM shadow registers.
;==================
_write_SCC_wave:
	;---- 000000SR	-> S = sfx waveform, R = RAM waveform
	bit		0, a
	jp		z, _write_SCC_wave.normalwave
.if SFXPLAY_ENABLED
	bit		1, a
	jp		nz, _write_SCC_wave.sfxwave
.endif
_write_SCC_wave.ramwave:
	dec		hl		; reset the special flag in the wave form number
	and		#0xFE
	ld		(hl), a

	ld		hl, #_0x9800
	ld		a, e
	add		a, l
	ld		l, a
	jp		nc, _write_SCC_wave.skip
	inc		h
_write_SCC_wave.skip:
	jp		copy_wave_fast

.if SFXPLAY_ENABLED
_write_SCC_wave.sfxwave:
	and		#0b11111000
	ld		l, a
	ld		h, #0
	add		hl, hl
	add		hl, hl
		
	;// MSXgl - Start
	;// ld		bc, #SFX_WAVEBASE
	ld		bc, (sfx_WAVETABLE)
	;// MSXgl - End
	add		hl, bc
	jp		copy_wave_fast
.endif

_write_SCC_wave.normalwave:
	ld		l, a
	ld		h, #0
	add		hl, hl
	add		hl, hl
		
	ld		bc, (replay_wavebase)
	add		hl, bc
copy_wave_fast:
	;--- Fastest way to copy waveform to SCC
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi		; 8
	ldi	
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi		; 16
	ldi	
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi		; 24
	ldi		
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi		; 32

	ret


_write_SCC_special:
	ld		hl, #replay_morph_buffer
;//	ld		bc, 32
_wss_l:
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	inc		hl
	ldi
	;--- Hack the return address
	pop		hl
	ld		bc, #6
	add		hl, bc
	jp		(hl)


;=============
; in [A] the morph active status
replay_process_morph:
	ld		hl, #replay_morph_timer
	dec		(hl)
	ret		nz

	;---- not sure what to do with this.
	; trigger any waveform updates
	ld		b, #4
	ld		de, #TRACK_REC_SIZE
	ld		hl, #TRACK_Chan4+17+TRACK_Flags
replay_process_morph.loop:	
	bit 	B_ACTMOR, (hl)
	jp		z, replay_process_morph.skip
	set		B_TRGWAV, (hl)
replay_process_morph.skip:
	add		hl, de
	djnz	replay_process_morph.loop	

	;---- timer ended.
	inc		a
	jp		nz, _rpm_next_step		; if status was !=255 then skip init

	;---- calculate offset
	inc		a
	ld		(replay_morph_active), a		; set status to 1

	ld		a, (replay_morph_speed)
	inc		a
	ld		(replay_morph_timer), a

	;--- calculate the delta s
	ld		a, (replay_morph_waveform)
	ld		l, a
	ld		h, #0
	add		hl, hl
	add		hl, hl
	ld		de, (replay_wavebase)
	add		hl, de
	ld		de, #replay_morph_buffer

	;---- start calculating
	ld		b, #32		; 32 values
_rpm_loop:
	inc		de
	ld		a, (de)
	dec		de
	add		a, #128
	ld		c, a
	ld		a, (hl)
	add		a, #128
	cp		c
	jp		c, _rpm_smaller			; dest is smaller

_rpm_larger:
	sub		c
	rrca
	rrca
	rrca
	rrca
	and		#0xEF					; reset bit 5
	ld		(de), a
	
	inc		de
	inc		de
	inc		hl
	djnz	_rpm_loop
	ret

_rpm_smaller:
	sub		c
	neg	
	rrca
	rrca
	rrca
	rrca
	or		#0x10					; set bit 5
	ld		(de), a
	
	inc		de
	inc		de
	inc		hl
	djnz	_rpm_loop
	ret


;============================
_rpm_next_step:
	ld		a, (replay_morph_speed)
	inc		a
	ld		(replay_morph_timer), a

	;-- apply the delta s
	ld		a, (replay_morph_counter)
	ld		c, a
	add		#16
	ld		(replay_morph_counter), a
	jp		nz, _rpm_next_step.skip
	;--- end morph
	ld		(replay_morph_active), a

_rpm_next_step.skip:
	dec		c
	ld		hl, #replay_morph_buffer
	ld		b, #32
_rpm_ns_loop:
	ld		a, (hl)
	bit 	4, a
	jp		z, _rmp_ns_add
_rmp_ns_sub:
	;--- handle corection
	and		#0xEF
	cp		c		; correction < counteR?
	jp		c, _rmp_ns_sub.skip
	inc		a		; if smaller C was set
_rmp_ns_sub.skip:
	and		#0b00011111	; keep lower 5 bits
	inc		hl
	ld		d, a
	ld		a, (hl)
	sub		d
	ld		(hl), a	; load new value
	inc		hl
	djnz	_rpm_ns_loop
	ret

_rmp_ns_add:
	;--- handle corection
	cp		c		; correction < counter?
	jp		c, _rmp_ns_add.skip
	inc		a		; if smaller C was set
_rmp_ns_add.skip:
	and		#0b00011111	; keep lower 5 bits
	inc		hl
	add		(hl)		; subtract waveform value
	ld		(hl), a	; load new value
	inc		hl
	djnz	_rpm_ns_loop
	ret
