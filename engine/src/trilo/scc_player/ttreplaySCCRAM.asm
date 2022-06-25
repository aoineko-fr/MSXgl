;================================
; The new replayer.
;
; Persistent RAM unswappable
;
;================================

TRACK_Instrument			= 0-17
TRACK_Command				= 1-17
TRACK_MacroPointer			= 2-17
TRACK_MacroStart			= 4-17
;TRACK_MacroRestart 		= 6-17		; Delete
TRACK_Note					= 7-17
TRACK_Volume				= 8-17
TRACK_Waveform				= 9-17
TRACK_Flags					= 10-17
	; 0 = note trigger
	; 1 = note active
	; 4 = morph active		;-< for SCC when 1 then waveform is following morph buffer
	; 3 = command trigger
	; 2 = envelope trigger
	; 5 = instrument trigger
	; 6 = waveform trigger
	; 7 = PSG/SCC
TRACK_empty				= 11-17		; needed for pushing 0 at note start
TRACK_ToneAdd			= 12-17		; reset after note set
TRACK_VolumeAdd			= 14-17		; reset after note set
TRACK_Noise				= 15-17		; reset after note set
TRACK_cmd_VolumeAdd		= 16-17		; reset after note set - For tremelo
TRACK_cmd_ToneSlideAdd	= 17-17		; reset after note set
TRACK_cmd_ToneAdd		= 19-17		; reset after note set

TRACK_cmd_detune		= 21-17
TRACK_cmd_0				= 23-17
TRACK_cmd_1				= 24-17
TRACK_cmd_2				= 25-17
TRACK_cmd_3				= 26-17
TRACK_cmd_4_depth		= 27-17
TRACK_cmd_4_step		= 29-17
TRACK_cmd_NoteAdd		= 30-17		; reset after note set
TRACK_cmd_A				= 31-17		
TRACK_cmd_B				= 32-17		
TRACK_cmd_E				= 33-17
TRACK_Timer				= 34-17		; used for timing by all cmd s
TRACK_Step				= 35-17		; only for VIBRATO???
TRACK_Delay				= 36-17		; rows to wait till next data
;TRACK_Retrig			= 35-17		; rows to retrigger command
TRACK_prevDelay			= 37-17
TRACK_cmd_A_add			= 38-17	;<< Still in use???

TRACK_REC_SIZE			= 39


B_TRGNOT				= 0			; note trigger
B_ACTNOT				= 1			; note active
B_TRGENV				= 2			; envelope trigger
B_TRGCMD				= 3			; command active
B_ACTMOR				= 4			; morph active
B_TRGINS				= 5			; instrument trigger
B_TRGWAV				= 6			; waveform trigger
B_PSGSCC				= 7			; chip type (PSG or SCC)


_SP_Storage:			.ds 2			; to store the SP

replay_trigger:			.ds 1			; trigger byte.
replay_mainPSGvol:		.ds 2			; volume mixer for PSG SCC balance
replay_mainSCCvol:		.ds 2			; volume mixer for PSG SCC balance
;replay_songbase:		.ds 2			; pointer to song data


; Do not move/reorder these
replay_wavebase:		.ds 2			; pointer to waveform data
replay_insbase:			.ds 2			; pointer to instrument data
TRACK_pointer1:			.ds 2
TRACK_pointer2:			.ds 2
TRACK_pointer3:			.ds 2
TRACK_pointer4:			.ds 2
TRACK_pointer5:			.ds 2
TRACK_pointer6:			.ds 2
TRACK_pointer7:			.ds 2
TRACK_pointer8:			.ds 2
; / Do not move/reorder above

replay_orderpointer:	.ds 2			; pointer to the order track list pointers

replay_speed:			.ds 1			; speed to replay (get from song)
replay_speed_subtimer:	.ds 1			; counter for finer speed
replay_speed_timer:		.ds 1 			; counter for speed
replay_mode:			.ds 1			; Replayer status
	; mode 0  = no sound output
	; mode 1  = replay song 
replay_arp_speed:		.ds 1			; arpeggio speed ( 0 = fast, $f = slowest)
replay_fade:			.ds 1			; Fade active (value = fade speed)
replay_fade_timer:		.ds 1			; Timer for fade
replay_fade_vol:		.ds 1			; fade volume to lower the channel volume.

replay_previous_note:	.ds 1			; previous note played
replay_mainvol:			.ds 2			; the volume correction.

;replay_vib_table:		.ds 2			; pointer to the vibrato table
;// MSXgl - Start
; replay_tonetable:		.ds 2			; ToneTable (affected by transpose);
replay_tonetable		= _g_TriloSCC_ToneTable
;// MSXgl - End

replay_morph_active:	.ds 1			; flag to indicate morphing is active
;replay_morph_update:	.ds 1			; flag to indicate a new waveform is ready
replay_morph_timer:		.ds 1			; step timer between morphs
replay_morph_type:		.ds 1			; Type of morph from set (0) or from buffer (1)
replay_morph_speed:		.ds 1 			; tics to wait between steps.
replay_morph_counter:	.ds 1			; counter till end morph
replay_morph_buffer:	.ds 64			; interleaved buffer with morphed waveform and morph delta values
replay_morph_waveform:	.ds 1 			; waveform we are morphing to.
;replay_envelope_shape:	.ds 1			; current envelope shape

;// MSXgl - Start
;// equalization_freq:		.ds 1	; vdp type for correct playback on 60hz 0=50Hx, >0=60Hz
equalization_freq		= _g_TriloSCC_Freq
;// MSXgl - End
equalization_cnt:		.ds 1	; counter for correct playback on 60hz
equalization_flag:		.ds 1	; flag indicating if only instruments need to be processed.

TRACK_Chan1:			.ds TRACK_REC_SIZE
TRACK_Chan2:			.ds TRACK_REC_SIZE
TRACK_Chan3:			.ds TRACK_REC_SIZE
TRACK_Chan4:			.ds TRACK_REC_SIZE
TRACK_Chan5:			.ds TRACK_REC_SIZE
TRACK_Chan6:			.ds TRACK_REC_SIZE
TRACK_Chan7:			.ds TRACK_REC_SIZE
TRACK_Chan8:			.ds TRACK_REC_SIZE

.if SFXPLAY_ENABLED

;--- AY SPECIFIC
PSG_registers:			.ds 0 
PSG_regToneA:			.ds 2	; Tone A freq low (8bit)
								; Tone A freq high (4bit)
PSG_regToneB:			.ds 2	; Tone B freq low
								; Tone B freq high
PSG_regToneC:			.ds 2	; Tone C freq low
								; Tone C freq high
PSG_regNOISE:			.ds 1	; Noise freq (5bit)
PSG_regMIXER:			.ds 1	;0x38	;x3f	; Mixer control (1 = off, 0 = on)
PSG_regVOLA:			.ds 1	; Chan A volume
PSG_regVOLB:			.ds 1	; Chan B volume
PSG_regVOLC:			.ds 1	; Chan C volume
PSG_regEnvL:			.ds 1	; Volume Env Freq low (8bit)	
PSG_regEnvH:			.ds 1	; Volume Env Freq high (4bit)
PSG_regEnvShape:		.ds 1	; Volume Env Shape (4bit)
replay_noise:			.ds 1	; replayer_noise

.else

;--- AY SPECIFIC
PSG_registers:			.ds 0 
PSG_regToneA:			.ds 2	; Tone A freq low (8bit)
								; Tone A freq high (4bit)
PSG_regToneB:			.ds 2	; Tone B freq low
								; Tone B freq high
PSG_regToneC:			.ds 2	; Tone C freq low
								; Tone C freq high
replay_noise:
PSG_regNOISE:			.ds 1	; Noise freq (5bit)
PSG_regMIXER:			.ds 1	;0x38	;x3f	; Mixer control (1 = off, 0 = on)
PSG_regVOLA:			.ds 1	; Chan A volume
PSG_regVOLB:			.ds 1	; Chan B volume
PSG_regVOLC:			.ds 1	; Chan C volume
PSG_regEnvL:			.ds 1	; Volume Env Freq low (8bit)	
PSG_regEnvH:			.ds 1	; Volume Env Freq high (4bit)
PSG_regEnvShape:		.ds 1	; Volume Env Shape (4bit)

.endif


;--- SCC SPECIFIC
ttsfx_waveform:			.ds 2 	; Pointer to sfx waveform
_0x9800:				.ds 32			; Waveform data
_0x9820:				.ds 32
_0x9840:				.ds 32
_0x9860:				.ds 32
SCC_registers:			.ds 0
SCC_regToneA:			.ds 2	; Tone A freq low (8bit)
								; Tone A freq high (4bit)
SCC_regToneB:			.ds 2	; Tone B freq low
								; Tone B freq high
SCC_regToneC:			.ds 2	; Tone C freq low
								; Tone C freq high
SCC_regToneD:			.ds 2	; Tone D freq low
								; Tone D freq high
SCC_regToneE:			.ds 2	; Tone E freq low
								; Tone E freq high
SCC_regVOLA:			.ds 1	; Chan A volume
SCC_regVOLB:			.ds 1	; Chan B volume
SCC_regVOLC:			.ds 1	; Chan C volume
SCC_regVOLD:			.ds 1	; Chan D volume
SCC_regVOLE:			.ds 1	; Chan E volume
SCC_regMIXER:			.ds 1	; x3f	; Mixer control (1 = off, 0 = on)


;-- SCC registers
oldregs:				.ds 144; ; (32*4)+(3*5)+1	; a way to int the SCC


