		; --- TT sfx player v0.1 ---

		; --- THIS FILE MUST BE COMPILED IN RAM ---

sfx_PRIORITY:			.ds 1			; Current sfx_PSG stream priority
sfx_STATUS:				.ds 1			; 0 = inactive, 1 or 2 is one or more sfx beeing played.
sfx_PSG_POINTER:		.ds 2			; Pointer to the current sfx_PSG stream
sfx_PSG_TONE:			.ds 2			; Current tone of the sfx_PSG stream
sfx_PSG_NOISE:			.ds 1			; Current noise of the sfx_PSG stream
;sfx_PSG_VOLUME:		.ds 1			; Current volume of the sfx_PSG stream
sfx_PSG_BALANCE:		.ds 2			; Pointer to the volume table

;sfx_SCC_PRIORITY:		.ds 1			; Current sfx_SCC stream priority
sfx_SCC_POINTER:		.ds 2			; Pointer to the current sfx_SCC stream
sfx_SCC_TONE:	    	.ds 2			; Current tone of the sfx_SCC stream
;sfx_SCC_WAVE:	    	.ds 2			; Current waveform of the sfx_SCC stream
;sfx_SCC_VOLUME:		.ds 1			; Current volume of the sfx_SCC stream
sfx_SCC_BALANCE:		.ds 2			; Pointer to the volume table

;// MSXgl - Start
sfx_BANK				= _g_TriloSFX_Bank ; Pointer to bank data
sfx_WAVETABLE			= _g_TriloSFX_WaveTable ; Pointer to wave-table data
;// MSXgl - End

