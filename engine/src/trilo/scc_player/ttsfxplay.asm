; --- TT sfx player v0.14 ---
; --- Build uppon ayFX replayer, the work of Shiru, Z80st, ARTRAG


; --- plays SFX over PSG(chan3) and SCC (chan1)
; --- Works with a wavetable list (SFX_WAVEBASE) and afb file.

;===========================================================
; ---	ttsfx_init
; Initialise the sfx player. Sets initial priorities, volume
; balancce and initial SCC waveform.
;
;===========================================================
ttsfx_init::
	ld		a, #15
	call	ttsfx_scc_balance
	ld		a, #15
	call	ttsfx_psg_balance
	xor		a
	ld		(sfx_STATUS), a
	dec		a
	ld		(sfx_PRIORITY), a
	ret


; ;===========================================================
; ; ---	ttsfx_load
; ;
; ; in: [HL] Bank binary data
; ;===========================================================
; ttsfx_load::
	; ;// MSXgl - Start
	; ld		(sfx_BANK), hl
	; ;// MSXgl - End
	; ret

;===========================================================
; ---	ttsfx_scc_balance
; Set the main SFX volume for the SCC chip.
;
; in: [A] master volume (0-15) 0=silenced, 15=full volume. 
;===========================================================	
ttsfx_scc_balance::
	call	_getsfxbalancebase
	ld		(sfx_SCC_BALANCE), hl
	ret


;===========================================================
; ---	ttsfx_psg_balance
; Set the main SFXvolume for the PSG chip.
;
; in: [A] master volume (0-15) 0=silenced, 15=full volume. 
;===========================================================	
ttsfx_psg_balance::
	call	_getsfxbalancebase
	ld		(sfx_PSG_BALANCE), hl
	ret

_getsfxbalancebase:
	rlca
	rlca
	rlca
	rlca
	ld		hl, #_VOLUME_TABLE			; Volume table from TT replayer
	and		#0xf0
	add		a, l
	ld		l, a
	ret 	nc
	inc		h
	ret


;===========================================================
;===========================================================	
ttsfx_start::
	;--- Start a new SFX
	;--- Input B -> sfx number
	;--- Input C -> priority 0 = lowest, 255 is highest

	;--- Test priority
	ld		a, (sfx_PRIORITY)		; a:=Current sfx_SCC stream priority
	cp		c						; If new sfx_SCC stream priority is higher than currently one...
	ret		c						; ...we don t start the new sfx_SCC stream

	;--- Set priority
	ld		a, c					; a:=New priority
	ld		(sfx_PRIORITY), a		; new priority saved in RAM
	ld		a, #0b00000011
	ld		(sfx_STATUS), a			; 2 chnnels playing

	;// MSXgl - Start
	;// ld		de, #sfxbank+1		; Start of SFX offset list
	ld		de, (sfx_BANK)
	inc		de
	;// MSXgl - End
	ld		l, b
	ld		h, #0
	add		hl, hl					; offset is 2 sfx (PSG+SCC)
	add		hl, hl					; x2 to get item offset in bank
	add		hl, de
	ld		a, b
	add		a						; offset is 2 sfx (PSG+SCC)
	add		a
	add		a, l
	ld		l, a
	jp		nc, ttsfx_start.skip
	inc		h
ttsfx_start.skip:
	ld		c, (hl)
	inc		hl
	ld		b, (hl)
;//	inc		hl
	push	hl						; store for second sfx
	add		hl, bc					; Pointer to the first sfx
	ld		(sfx_PSG_POINTER), hl
	pop		hl
	inc		hl
	ld		c, (hl)
	inc		hl
	ld		b, (hl)
	add		hl, bc					; Pointer to the second sfx
	ld		(sfx_SCC_POINTER), hl
	ret


;===========================================================
;===========================================================	
ttsfx_play::
	;--------------------------------
	; Play both sfx streams
	;--------------------------------
	ld		a, (sfx_STATUS)	; a:=Current sfx_PSG stream priority
	bit		0, a			; PSG active?
	jp		nz, ttsfx_play.psg_play
	and		a			; test if scc is active
	
	;--- If not active just use re-player noise and mixer
	ld		a, (replay_noise)
	ld		(PSG_regNOISE), a

	jp		nz, _ttsfx_scc_play.scc_play
	ret

ttsfx_play.psg_play:
	; --- PLAY A FRAME OF AN sfx_PSG STREAM ---
	; --- Extract control byte from stream ---
	ld		hl, (sfx_PSG_POINTER)	; Pointer to the current sfx_PSG stream
	ld		c, (hl)			; c:=Control byteb
	ld		a, (PSG_regMIXER)
	ld		b, a				; b = mixer 
	inc		hl				; Increment pointer

_ayCHECK_NT:
	; --- Check if there s new tone on stream ---
	bit		5, c				; If bit 5 c is off...
	jp		z, _ayCHECK_NN		; ...jump to _ayCHECK_NN (no new tone)
	; --- Extract new tone from stream ---
	ld		e, (hl)			; e:=lower byte of new tone
	inc		hl				; Increment pointer
	ld		d, (hl)			; d:=higher byte of new tone
	inc		hl				; Increment pointer
	ld		(sfx_PSG_TONE), de		; sfx_PSG tone updated

_ayCHECK_NN:	
	; --- Check if there s new noise on stream ---
	bit		6, c				; if bit 6 c is off...
	jp		z, _aySETPOINTER		; ...jump to _aySETPOINTER (no new noise)
	; --- Extract new noise from stream ---
	ld		a, (hl)			; a:=New noise
	inc		hl				; Increment pointer
	cp		#0x20				; If it s an illegal value of noise (used to mark end of stream)...
	jp		z, _ayEND
_ayNOISE:
	ld		(sfx_PSG_NOISE), a		; sfx_PSG noise updated
_aySETPOINTER:
	; --- Update sfx_PSG pointer ---
	ld		(sfx_PSG_POINTER), hl	; Update sfx_PSG stream pointer

	; --- Set noise register if enabled ---
	bit		7, c						; If noise is off...
	jp		nz, _ayMUSNOISE			; ...jump to _aySETMASKS
	ld		a, b				; mixer mask for no music noise
	or		#0b00011000
	and		#0b00011111
	ld		b, a
	ld		a, (sfx_PSG_NOISE)		; sfx_PSG noise value
	jp		_ayNOISESET
_ayMUSNOISE:
	;-- If no sfx noise then use music noise value
	ld		a, (replay_noise)		; replaer PSG noise value
_ayNOISESET:
	ld		(PSG_regNOISE), a		; copied in to AYREGS (noise channel)

	; --- Extract and set volume if tone or noise is enabled ---
	bit		4, c						; If tone is off...
	jp		nz, _ayVOLUME			; ...jump to _ayMIXER
	ld		hl, (sfx_PSG_TONE)
	ld		(PSG_regToneC), hl
	res		2, b				; enable tone on chan 3

_ayVOLUME:	
	;--- Only update volume if tone or noise is active
	ld		a, #0b00100100
	and		b
	cp		#0b00100100
	jp		z, _ayMIXER

	ld		a, c					; a:=Control byte
	and		#0x0F					; lower nibble
	; --- Fix the volume using TT Volume Table ---
	ld		hl, (sfx_PSG_BALANCE)	; hl:=Pointer to relative volume table
	add		a, l
	ld		l, a
	jp		nc, _ayVOLUME.skip
	inc		h
_ayVOLUME.skip:
	ld		a, (hl)				; a:=sfx_PSG relative volume
	and		#0x0f					; mask only ay volume
	ld		(PSG_regVOLC), a		; Diretly update the TT register value

_ayMIXER:
	;--- Set Mixer
	ld		a, b
	ld		(PSG_regMIXER), a		; Directly write the TT register value

_ttsfx_scc_play:
	ld		a, (sfx_STATUS)	; a:=Current sfx_PSG stream priority
	and		#0b00000010		; SCC active?
	ret		z


_ttsfx_scc_play.scc_play:
	; --- Extract control byte from stream ---
	ld		hl, (sfx_SCC_POINTER)	; Pointer to the current sfx_SCC stream
	ld		c, (hl)			; c:=Control byte
	ld		a, (SCC_regMIXER)
	ld		b, a				; b = mixer 
	inc		hl				; Increment pointer

_sccCHECK_NT:
	; --- Check if there s new tone on stream ---
	bit		5, c				; If bit 5 c is off...
	jp		z, _sccCHECK_NW		; ...jump to _sccCHECK_NN (no new tone)
	; --- Extract new tone from stream ---
	ld		e, (hl)			; e:=lower byte of new tone
	inc		hl				; Increment pointer
	ld		d, (hl)			; d:=higher byte of new tone
	inc		hl				; Increment pointer
	ld		(sfx_SCC_TONE), de		; sfx_SCC tone updated

_sccCHECK_NW:	
	; --- Check if there s the end of the sfx ---
	bit		6, c				; if bit 6 c is off...
	jp		z, _sccSETPOINTER		; ...jump to _sccSETPOINTER (no end)
	; --- Extract new waveform from stream ---
	ld		a, (hl)			; a:=New waveform
	inc		hl
	add		a				; *2
	jp		z, _sccSETPOINTER		; wave form 0 is no waveform
	cp		#0x40				; And marker is now 2*0x20 due to the add
	jp		z, _sccEND		


_sccWAVE:
	add		a				; *4
	add		a				; *8
	or		#3				; set SFX wave flag

	ld		(sfx_SCC_POINTER), hl	; Update sfx_SCC stream pointer
	;--- Set waveform trigger in TT replayer
	ld		hl, #TRACK_Chan4+17+TRACK_Waveform
	cp		(hl)
	jp		z, _sccVOLUME

	ld		(TRACK_Chan4+17+TRACK_Waveform), a		; Set a new waveform trigger, ttreplay will handle this
	ld		hl, #TRACK_Chan4+17+TRACK_Flags
	set 	B_TRGWAV, (hl)
	jp		_sccVOLUME


_sccSETPOINTER:	; --- Update sfx_SCC pointer ---
	ld		(sfx_SCC_POINTER), hl	; Update sfx_SCC stream pointer


_sccVOLUME:
	; --- Extract and set volume if tone is enabled ---
	bit		4, c						; If tone is off...
	ret		nz							; end

	set		0, b						; enable sound chan1
	ld		a, c						; a:=Control byte
	and		#0x0F						; lower nibble
	; --- Fix the volume using TT Volume Table ---
	ld		hl, (sfx_SCC_BALANCE)		; hl:=Pointer to relative volume table
	ld		e, a						; e:=a (sfx_SCC volume)
	ld		d, #0						; d:=0
	add		hl, de						; hl:=hl+de (hl points to the relative volume of this frame
	ld		a, (hl)						; a:=sfx_SCC relative volume
	rra									; SCC relative volume is in the higher 4 bits
	rra
	rra
	rra
	and		#0x0f						; mask only scc volume
	ld		(SCC_regVOLA), a			; Volume copied in to SCCREGS (channel 1 volume)

	ld		hl, (sfx_SCC_TONE)
	ld		(SCC_regToneA), hl


_sccMIXER:
	;--- Set Mixer
	ld		a, b
	ld		(SCC_regMIXER), a		; Directly write the TT register value
	ret


_ayEND:		
	ld		a, (sfx_STATUS)		; remove bit 0
	and		#0b00000010
	ld		(sfx_STATUS), a
	jp		nz, _ttsfx_scc_play.scc_play
	dec		a					; reset prio (255)
	ld		(sfx_PRIORITY), a
	ret


_sccEND:
	ld		a, (sfx_STATUS)		; remove bit 0
	and		#0b00000001
	ld		(sfx_STATUS), a
	ret		nz
	dec		a					; reset prio (255)
	ld		(sfx_PRIORITY), a
	ret
