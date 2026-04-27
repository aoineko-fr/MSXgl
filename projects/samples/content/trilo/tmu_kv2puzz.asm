; Song: KVII- Puzzle Stage              
; By:   Konami (Arr by Gryzor87)       
; Period table: A445 Konami

; [ Song start data ]
	.db 0x04					; Initial song speed
	.dw kv2puzz_waveform_start			; Start of the waveform data
	.dw kv2puzz_instrument_start			; Start of the instrument data

; [ Song order pointer list ]
kv2puzz_restart:
	.dw kv2puzz_track_000, kv2puzz_track_001, kv2puzz_track_002, kv2puzz_track_002, kv2puzz_track_004, kv2puzz_track_005, kv2puzz_track_006, kv2puzz_track_007	; Step:000 Pattern:000
	.dw kv2puzz_track_008, kv2puzz_track_009, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_012, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_010	; Step:001 Pattern:001
	.dw kv2puzz_track_008, kv2puzz_track_017, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_020, kv2puzz_track_021, kv2puzz_track_022, kv2puzz_track_023	; Step:002 Pattern:002
	.dw kv2puzz_track_024, kv2puzz_track_025, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_028, kv2puzz_track_029, kv2puzz_track_030, kv2puzz_track_031	; Step:003 Pattern:003
	.dw kv2puzz_track_032, kv2puzz_track_033, kv2puzz_track_034, kv2puzz_track_034, kv2puzz_track_036, kv2puzz_track_037, kv2puzz_track_038, kv2puzz_track_039	; Step:004 Pattern:004
	.dw kv2puzz_track_200, kv2puzz_track_201, kv2puzz_track_201, kv2puzz_track_201, kv2puzz_track_204, kv2puzz_track_205, kv2puzz_track_206, kv2puzz_track_207	; Step:005 Pattern:025
	.dw kv2puzz_track_040, kv2puzz_track_041, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_044, kv2puzz_track_045, kv2puzz_track_046, kv2puzz_track_047	; Step:006 Pattern:005
	.dw kv2puzz_track_048, kv2puzz_track_049, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_052, kv2puzz_track_053, kv2puzz_track_054, kv2puzz_track_055	; Step:007 Pattern:006
	.dw kv2puzz_track_000, kv2puzz_track_001, kv2puzz_track_002, kv2puzz_track_002, kv2puzz_track_004, kv2puzz_track_005, kv2puzz_track_006, kv2puzz_track_007	; Step:008 Pattern:000
	.dw kv2puzz_track_008, kv2puzz_track_009, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_012, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_010	; Step:009 Pattern:001
	.dw kv2puzz_track_008, kv2puzz_track_017, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_020, kv2puzz_track_021, kv2puzz_track_022, kv2puzz_track_023	; Step:010 Pattern:002
	.dw kv2puzz_track_024, kv2puzz_track_025, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_028, kv2puzz_track_029, kv2puzz_track_030, kv2puzz_track_031	; Step:011 Pattern:003
	.dw kv2puzz_track_032, kv2puzz_track_033, kv2puzz_track_034, kv2puzz_track_034, kv2puzz_track_036, kv2puzz_track_037, kv2puzz_track_038, kv2puzz_track_039	; Step:012 Pattern:004
	.dw kv2puzz_track_200, kv2puzz_track_201, kv2puzz_track_201, kv2puzz_track_201, kv2puzz_track_204, kv2puzz_track_205, kv2puzz_track_206, kv2puzz_track_207	; Step:013 Pattern:025
	.dw kv2puzz_track_056, kv2puzz_track_057, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_060, kv2puzz_track_061, kv2puzz_track_062, kv2puzz_track_063	; Step:014 Pattern:007
	.dw kv2puzz_track_064, kv2puzz_track_065, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_068, kv2puzz_track_053, kv2puzz_track_054, kv2puzz_track_055	; Step:015 Pattern:008
	.dw kv2puzz_track_008, kv2puzz_track_073, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_076, kv2puzz_track_077, kv2puzz_track_078, kv2puzz_track_079	; Step:016 Pattern:009
	.dw kv2puzz_track_080, kv2puzz_track_081, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_084, kv2puzz_track_085, kv2puzz_track_086, kv2puzz_track_087	; Step:017 Pattern:010
	.dw kv2puzz_track_008, kv2puzz_track_017, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_092, kv2puzz_track_093, kv2puzz_track_094, kv2puzz_track_095	; Step:018 Pattern:011
	.dw kv2puzz_track_096, kv2puzz_track_081, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_084, kv2puzz_track_101, kv2puzz_track_102, kv2puzz_track_103	; Step:019 Pattern:012
	.dw kv2puzz_track_104, kv2puzz_track_105, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_108, kv2puzz_track_109, kv2puzz_track_110, kv2puzz_track_111	; Step:020 Pattern:013
	.dw kv2puzz_track_104, kv2puzz_track_105, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_108, kv2puzz_track_117, kv2puzz_track_118, kv2puzz_track_119	; Step:021 Pattern:014
	.dw kv2puzz_track_104, kv2puzz_track_121, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_124, kv2puzz_track_125, kv2puzz_track_126, kv2puzz_track_127	; Step:022 Pattern:015
	.dw kv2puzz_track_080, kv2puzz_track_129, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_132, kv2puzz_track_133, kv2puzz_track_134, kv2puzz_track_135	; Step:023 Pattern:016
	.dw kv2puzz_track_136, kv2puzz_track_137, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_140, kv2puzz_track_141, kv2puzz_track_142, kv2puzz_track_143	; Step:024 Pattern:017
	.dw kv2puzz_track_136, kv2puzz_track_145, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_148, kv2puzz_track_149, kv2puzz_track_150, kv2puzz_track_151	; Step:025 Pattern:018
	.dw kv2puzz_track_152, kv2puzz_track_153, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_156, kv2puzz_track_157, kv2puzz_track_158, kv2puzz_track_159	; Step:026 Pattern:019
	.dw kv2puzz_track_160, kv2puzz_track_161, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_164, kv2puzz_track_165, kv2puzz_track_166, kv2puzz_track_167	; Step:027 Pattern:020
	.dw kv2puzz_track_136, kv2puzz_track_169, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_140, kv2puzz_track_173, kv2puzz_track_142, kv2puzz_track_143	; Step:028 Pattern:021
	.dw kv2puzz_track_136, kv2puzz_track_145, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_148, kv2puzz_track_149, kv2puzz_track_150, kv2puzz_track_151	; Step:029 Pattern:022
	.dw kv2puzz_track_184, kv2puzz_track_185, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_156, kv2puzz_track_189, kv2puzz_track_158, kv2puzz_track_159	; Step:030 Pattern:023
	.dw kv2puzz_track_192, kv2puzz_track_193, kv2puzz_track_010, kv2puzz_track_010, kv2puzz_track_164, kv2puzz_track_197, kv2puzz_track_198, kv2puzz_track_199	; Step:031 Pattern:024
	.dw 0x0000, kv2puzz_restart				; End of sequence delimiter + kv2puzz_restart address


; [ SCC Waveforms ]
kv2puzz_waveform_start:
	.db 0x80, 0xb0, 0xc0, 0x10, 0x1a, 0x2a, 0x2c, 0x1a, 0x00, 0xe0, 0xd0, 0xe0, 0x22, 0x53, 0x70, 0x75, 0x70, 0x31, 0xea, 0x80, 0x88, 0x8a, 0x8c, 0x8e, 0x00, 0x7f, 0x75, 0x73, 0x62, 0x00, 0xc0, 0x90				; Waveform:1
	.db 0x00, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x81, 0xc0, 0x01, 0x40, 0x7f, 0x40, 0x01, 0xc0, 0x01, 0x40, 0x01, 0xe0, 0x01, 0x20, 0x01, 0xf0, 0x01, 0x10, 0x01, 0xff, 0xff, 0xff, 0xff, 0x40, 0x40, 0x40				; Waveform:2
	.db 0xa0, 0x90, 0xa0, 0xac, 0xf0, 0xa0, 0x16, 0x00, 0x90, 0x00, 0x20, 0x40, 0x60, 0x50, 0x38, 0x1c, 0xf2, 0xe2, 0xd6, 0xd0, 0xd0, 0xda, 0xdc, 0xe0, 0xe2, 0xe2, 0xe2, 0xe0, 0xdc, 0xd8, 0xd0, 0xb0				; Waveform:7
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70				; Waveform:8
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x00, 0xe0, 0xc0, 0xa0, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x7f, 0x60, 0x40, 0x20				; Waveform:12
	.db 0x30, 0x50, 0x50, 0x30, 0x00, 0x00, 0x10, 0x40, 0x60, 0x70, 0x60, 0x30, 0xf0, 0xe0, 0xe0, 0x00, 0x20, 0x20, 0x10, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00, 0xd0, 0xb0, 0xb0, 0xd0, 0x00, 0x00				; Waveform:13
	.db 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x80, 0x80				; Waveform:15
	.db 0x00, 0x68, 0x48, 0x18, 0x48, 0x68, 0x28, 0x00, 0x48, 0x77, 0x58, 0x08, 0x28, 0x38, 0x00, 0xb8, 0x08, 0x58, 0x00, 0xe8, 0xf8, 0x00, 0xb8, 0x98, 0xc8, 0x08, 0xe8, 0xa8, 0xc8, 0xf8, 0xc8, 0xa8				; Waveform:20
	.db 0x00, 0x19, 0x31, 0x47, 0x5a, 0x6a, 0x75, 0x7d, 0x7f, 0x7d, 0x75, 0x6a, 0x5a, 0x47, 0x31, 0x19, 0x00, 0xe7, 0xcf, 0xb9, 0xa6, 0x96, 0x8b, 0x83, 0x80, 0x83, 0x8b, 0x96, 0xa6, 0xb9, 0xcf, 0xe7				; Waveform:21
	.db 0x90, 0x88, 0x90, 0x9c, 0xb0, 0xc0, 0xe0, 0x26, 0x70, 0x7c, 0x70, 0x56, 0x40, 0x20, 0x10, 0xfc, 0xe0, 0xfc, 0x10, 0xfc, 0x40, 0x54, 0x72, 0x7f, 0x70, 0x26, 0xe0, 0xc0, 0xb0, 0x9c, 0x90, 0x88				; Waveform:30
	.db 0x00, 0x4e, 0x62, 0x6d, 0x75, 0x7a, 0x7d, 0x7e, 0x7f, 0x7e, 0x7d, 0x7a, 0x75, 0x6d, 0x62, 0x4e, 0x00, 0xb1, 0x9d, 0x92, 0x8a, 0x85, 0x82, 0x81, 0x80, 0x81, 0x82, 0x85, 0x8a, 0x92, 0x9d, 0xb1				; Waveform:31

; [ Instruments]
kv2puzz_instrument_start:
	.dw kv2puzz_instrument_00				; BD G2           
	.dw kv2puzz_instrument_01				; SD A3           
	.dw kv2puzz_instrument_02				; HH CL A8        
	.dw kv2puzz_instrument_03				; PSG BASS        
	.dw kv2puzz_instrument_04				; HH OP           
	.dw kv2puzz_instrument_05				; Short Snare     
	.dw kv2puzz_instrument_06				; PSG flute 2     
	.dw kv2puzz_instrument_07				; SCC BASS OUT    
	.dw kv2puzz_instrument_08				; BASS LONG       
	.dw kv2puzz_instrument_09				; Rombo-BASS      
	.dw kv2puzz_instrument_10				; Flute KVII      
	.dw kv2puzz_instrument_11				; Flute II-KV2    
	.dw kv2puzz_instrument_12				; PSG Piano       
	.dw kv2puzz_instrument_13				; SCC Piano       
	.dw kv2puzz_instrument_14				; ROMBO - notes   
	.dw kv2puzz_instrument_15				; ROMBOHIGH       
	.dw kv2puzz_instrument_16				; CONT            

kv2puzz_instrument_00:					; BD G2           
	.db 0x50						; Waveform 10
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

kv2puzz_instrument_01:					; SD A3           
	.db 0x00						; Waveform 0
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x01						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

kv2puzz_instrument_02:					; HH CL A8        
	.db 0x10						; Waveform 2
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
								; --- Macro loop
	.db 0x08,0x00						; Volume _
	.db 0x18,0xfd						; Loop (-4)

kv2puzz_instrument_03:					; PSG BASS        
	.db 0x40						; Waveform 8
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
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
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x0c,0x01						; Volume -
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
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x18,0xe3						; Loop (-30)

kv2puzz_instrument_04:					; HH OP           
	.db 0x10						; Waveform 2
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x80						; Mixer (N)
	.db 0x0E,0x00						; Noise _
	.db 0x0c,0x01						; Volume -
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x02,0x80						; Mixer (N)
	.db 0x00							; Volume same
	.db 0x18,0xf0						; Loop (-17)

kv2puzz_instrument_05:					; Short Snare     
	.db 0x00						; Waveform 0
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

kv2puzz_instrument_06:					; PSG flute 2     
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_07:					; SCC BASS OUT    
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x03						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_08:					; BASS LONG       
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_09:					; Rombo-BASS      
	.db 0x20						; Waveform 4
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_10:					; Flute KVII      
	.db 0x50						; Waveform 10
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x03						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_11:					; Flute II-KV2    
	.db 0x40						; Waveform 8
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x03						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_12:					; PSG Piano       
	.db 0x38						; Waveform 7
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
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_13:					; SCC Piano       
	.db 0x28						; Waveform 5
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x05						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
	.db 0x02,0x10						; Mixer (T)
	.db 0x00							; Volume same
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x04						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_14:					; ROMBO - notes   
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_15:					; ROMBOHIGH       
	.db 0x08						; Waveform 1
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x06						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x07						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x08						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x09						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0a						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0b						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0c						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0f						; Volume _
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0d						; Volume _
	.db 0x18,0xfb						; Loop (-6)

kv2puzz_instrument_16:					; CONT            
	.db 0x48						; Waveform 9
								; --- Macro loop
	.db 0x02,0x10						; Mixer (T)
	.db 0x08,0x0e						; Volume _
	.db 0x18,0xfb						; Loop (-6)


; [ Song track data ]
kv2puzz_track_000:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_001:
	.db 0x10			;Note E-2
	.db 0x6f			;Volume 13
	.db 0x75			;Instrument 3
	.db 0x9c			;CMD End 
	.db 0xd1			;Wait 18
	.db 0x13			;Note G-2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_002:
	.db 0x71			;Volume 15
	.db 0x72			;Instrument 0
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_004:
	.db 0x10			;Note E-2
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x10			;Note E-2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0x13			;Note G-2
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x13			;Note G-2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_005:
	.db 0x2f			;Note B-4
	.db 0x6f			;Volume 13
	.db 0x7d			;Instrument 11
	.db 0xc5			;Wait 6
	.db 0x95,0x78			;CMD Vibrato
	.db 0xdd			;Wait 30
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_006:
	.db 0x34			;Note E-5
	.db 0x71			;Volume 15
	.db 0x7c			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x95,0x36			;CMD Vibrato
	.db 0xdd			;Wait 30
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_007:
	.db 0x60			;Release 96
	.db 0xc5			;Wait 6
	.db 0x34			;Note E-5
	.db 0x6b			;Volume 9
	.db 0x7c			;Instrument 10
	.db 0xa2,0x0a			;CMD Track detune
					;Wait Repeat 6
	.db 0x95,0x26			;CMD Vibrato
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_008:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x74			;Instrument 2
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_009:
	.db 0x12			;Note F#2
	.db 0x6f			;Volume 13
	.db 0x75			;Instrument 3
	.db 0xd1			;Wait 18
	.db 0x17			;Note B-2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_010:
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_012:
	.db 0x12			;Note F#2
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x11			;Note F-2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0x17			;Note B-2
	.db 0x7a			;Instrument 8
	.db 0xcf			;Wait 16
	.db 0x17			;Note B-2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_017:
	.db 0x16			;Note A#2
	.db 0x75			;Instrument 3
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_020:
	.db 0x16			;Note A#2
	.db 0x7a			;Instrument 8
	.db 0xe1			;Wait 34
	.db 0x16			;Note A#2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_021:
	.db 0x2e			;Note A#4
	.db 0x6f			;Volume 13
	.db 0x7d			;Instrument 11
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x57			;CMD Vibrato
	.db 0xdd			;Wait 30
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_022:
	.db 0x37			;Note G-5
	.db 0x71			;Volume 15
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x95,0x26			;CMD Vibrato
	.db 0xdd			;Wait 30
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_023:
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x37			;Note G-5
	.db 0x6b			;Volume 9
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
					;Wait Repeat 6
	.db 0x95,0x26			;CMD Vibrato
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_024:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc8			;Wait 9
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_025:
	.db 0x17			;Note B-2
	.db 0x75			;Instrument 3
	.db 0xc5			;Wait 6
	.db 0x6d			;Volume 11
	.db 0xc1			;Wait 2
	.db 0x6c			;Volume 10
	.db 0xc9			;Wait 10
	.db 0x24			;Note C-4
	.db 0x6a			;Volume 8
	.db 0x78			;Instrument 6
	.db 0xa2,0x00			;CMD Track detune
	.db 0xcc			;Wait 13
	.db 0x23			;Note B-3
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_028:
	.db 0x17			;Note B-2
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xc5			;Wait 6
	.db 0x70			;Volume 14
	.db 0xc1			;Wait 2
	.db 0x6f			;Volume 13
					;Wait Repeat 2
	.db 0x6e			;Volume 12
	.db 0xc7			;Wait 8
	.db 0x13			;Note G-2
	.db 0x6e			;Volume 12
	.db 0x7b			;Instrument 9
	.db 0xc8			;Wait 9
	.db 0x12			;Note F#2
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_029:
	.db 0x2f			;Note B-4
	.db 0x6f			;Volume 13
	.db 0x7d			;Instrument 11
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
	.db 0xcb			;Wait 12
	.db 0x24			;Note C-4
	.db 0x6a			;Volume 8
	.db 0x80			;Instrument 14
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc8			;Wait 9
	.db 0x23			;Note B-3
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_030:
	.db 0x36			;Note F#5
	.db 0x71			;Volume 15
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
	.db 0xcb			;Wait 12
	.db 0x28			;Note E-4
	.db 0x6a			;Volume 8
	.db 0x80			;Instrument 14
	.db 0xaa,0x30			; SCC Set Waveform 15
	.db 0xc8			;Wait 9
	.db 0x27			;Note D#4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_031:
	.db 0xc5			;Wait 6
	.db 0x36			;Note F#5
	.db 0x66			;Volume 4
	.db 0x82			;Instrument 16
	.db 0x9c			;CMD End 
	.db 0xcb			;Wait 12
	.db 0x28			;Note E-4
	.db 0x68			;Volume 6
	.db 0xa2,0x02			;CMD Track detune
	.db 0xcc			;Wait 13
	.db 0x27			;Note D#4
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_032:
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc8			;Wait 9
	.db 0x29			;Note F-4
	.db 0xc8			;Wait 9
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
					;Wait Repeat 9
	.db 0x29			;Note F-4
					;Wait Repeat 9
	.db 0x29			;Note F-4
	.db 0xc8			;Wait 9
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 9
	.db 0x21			;Note A-3
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_033:
	.db 0xc3			;Wait 4
	.db 0x28			;Note E-4
	.db 0x6b			;Volume 9
	.db 0x78			;Instrument 6
	.db 0xc8			;Wait 9
	.db 0x27			;Note D#4
	.db 0xc8			;Wait 9
	.db 0x2d			;Note A-4
	.db 0x6c			;Volume 10
					;Wait Repeat 9
	.db 0x2c			;Note G#4
	.db 0xc8			;Wait 9
	.db 0x30			;Note C-5
	.db 0x6d			;Volume 11
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0x34			;Note E-5
	.db 0x6c			;Volume 10
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_034:
	.db 0xfe			;Wait 63
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_036:
	.db 0x17			;Note B-2
	.db 0x7b			;Instrument 9
	.db 0xc8			;Wait 9
	.db 0x16			;Note A#2
					;Wait Repeat 9
	.db 0x1c			;Note E-3
					;Wait Repeat 9
	.db 0x1b			;Note D#3
					;Wait Repeat 9
	.db 0x1f			;Note G-3
					;Wait Repeat 9
	.db 0x1e			;Note F#3
					;Wait Repeat 9
	.db 0x23			;Note B-3
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_037:
	.db 0x28			;Note E-4
	.db 0x80			;Instrument 14
	.db 0xc8			;Wait 9
	.db 0x27			;Note D#4
	.db 0xc8			;Wait 9
	.db 0x2d			;Note A-4
	.db 0x6b			;Volume 9
					;Wait Repeat 9
	.db 0x2c			;Note G#4
	.db 0xc8			;Wait 9
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 10
					;Wait Repeat 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x34			;Note E-5
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_038:
	.db 0x2b			;Note G-4
	.db 0x80			;Instrument 14
	.db 0xaa,0x30			; SCC Set Waveform 15
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
	.db 0xc8			;Wait 9
	.db 0x30			;Note C-5
	.db 0x6b			;Volume 9
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0x34			;Note E-5
	.db 0x6c			;Volume 10
					;Wait Repeat 9
	.db 0x33			;Note D#5
					;Wait Repeat 9
	.db 0x37			;Note G-5
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_039:
	.db 0xc3			;Wait 4
	.db 0x2b			;Note G-4
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
					;Wait Repeat 9
	.db 0x30			;Note C-5
					;Wait Repeat 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x34			;Note E-5
					;Wait Repeat 9
	.db 0x33			;Note D#5
					;Wait Repeat 9
	.db 0x37			;Note G-5
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_040:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd7			;Wait 24
	.db 0x29			;Note F-4
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x37			;Note G-5
	.db 0x76			;Instrument 4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_041:
	.db 0x18			;Note C-3
	.db 0x6e			;Volume 12
	.db 0x75			;Instrument 3
	.db 0xa2,0x04			;CMD Track detune
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_044:
	.db 0x18			;Note C-3
	.db 0x7a			;Instrument 8
	.db 0xe1			;Wait 34
	.db 0x18			;Note C-3
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_045:
	.db 0x39			;Note A-5
	.db 0x6f			;Volume 13
	.db 0x7d			;Instrument 11
	.db 0xc5			;Wait 6
	.db 0x95,0x37			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_046:
	.db 0x3c			;Note C-6
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x95,0x26			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_047:
	.db 0x36			;Note F#5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x95,0x16			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_048:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x1f			;Note G-3
	.db 0x6d			;Volume 11
	.db 0x77			;Instrument 5
					;Wait Repeat 6
	.db 0x1f			;Note G-3
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_049:
	.db 0x17			;Note B-2
	.db 0x6e			;Volume 12
	.db 0x75			;Instrument 3
	.db 0xa2,0x04			;CMD Track detune
	.db 0xc7			;Wait 8
	.db 0x6c			;Volume 10
	.db 0xc0			;Wait 1
	.db 0x93,0x20			;CMD Portamento down
					;Wait Repeat 1
	.db 0x6b			;Volume 9
					;Wait Repeat 1
	.db 0x6a			;Volume 8
					;Wait Repeat 1
	.db 0x69			;Volume 7
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_052:
	.db 0x17			;Note B-2
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc8			;Wait 9
	.db 0x93,0x20			;CMD Portamento down
	.db 0xc1			;Wait 2
	.db 0x6b			;Volume 9
	.db 0xc0			;Wait 1
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_053:
	.db 0x36			;Note F#5
	.db 0x6f			;Volume 13
	.db 0x7d			;Instrument 11
	.db 0xc8			;Wait 9
	.db 0x35			;Note F-5
	.db 0x68			;Volume 6
	.db 0x82			;Instrument 16
	.db 0xaa,0x40			; SCC Set Waveform 21
	.db 0xc1			;Wait 2
	.db 0x34			;Note E-5
	.db 0x67			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x33			;Note D#5
	.db 0x66			;Volume 4
					;Wait Repeat 1
	.db 0x32			;Note D-5
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x31			;Note C#5
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x30			;Note C-5
	.db 0x63			;Volume 1
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
	.db 0xd2			;Wait 19
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_054:
	.db 0x3b			;Note B-5
	.db 0x69			;Volume 7
	.db 0x7c			;Instrument 10
	.db 0xc8			;Wait 9
	.db 0x3a			;Note A#5
	.db 0x65			;Volume 3
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 31
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x37			;Note G-5
					;Wait Repeat 1
	.db 0x36			;Note F#5
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x63			;Volume 1
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
	.db 0xd2			;Wait 19
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_055:
	.db 0x3b			;Note B-5
	.db 0x69			;Volume 7
	.db 0x7c			;Instrument 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc8			;Wait 9
	.db 0x3a			;Note A#5
	.db 0x65			;Volume 3
	.db 0x82			;Instrument 16
	.db 0xaa,0x50			; SCC Set Waveform 31
	.db 0xc1			;Wait 2
	.db 0x39			;Note A-5
	.db 0xc0			;Wait 1
	.db 0x38			;Note G#5
	.db 0x64			;Volume 2
					;Wait Repeat 1
	.db 0x37			;Note G-5
					;Wait Repeat 1
	.db 0x36			;Note F#5
	.db 0xc0			;Wait 1
	.db 0x35			;Note F-5
	.db 0x63			;Volume 1
	.db 0xc1			;Wait 2
	.db 0x60			;Release 96
	.db 0xd2			;Wait 19
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_056:
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0x76			;Instrument 4
	.db 0xd7			;Wait 24
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0x76			;Instrument 4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_057:
	.db 0x16			;Note A#2
	.db 0x6e			;Volume 12
	.db 0x75			;Instrument 3
	.db 0xa2,0x04			;CMD Track detune
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_060:
	.db 0x16			;Note A#2
	.db 0x7a			;Instrument 8
	.db 0xe1			;Wait 34
	.db 0x16			;Note A#2
	.db 0x71			;Volume 15
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_061:
	.db 0x34			;Note E-5
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x95,0x37			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_062:
	.db 0x3a			;Note A#5
	.db 0x6f			;Volume 13
	.db 0x7c			;Instrument 10
	.db 0xc5			;Wait 6
	.db 0x95,0x36			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_063:
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0xa2,0x00			;CMD Track detune
	.db 0xc5			;Wait 6
	.db 0x95,0x16			;CMD Vibrato
	.db 0xdc			;Wait 29
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_064:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x29			;Note F-4
	.db 0x6e			;Volume 12
					;Wait Repeat 6
	.db 0x1f			;Note G-3
	.db 0x6d			;Volume 11
	.db 0x77			;Instrument 5
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_065:
	.db 0x17			;Note B-2
	.db 0x6e			;Volume 12
	.db 0x75			;Instrument 3
	.db 0xa2,0x04			;CMD Track detune
	.db 0xc4			;Wait 5
	.db 0x6d			;Volume 11
	.db 0xc0			;Wait 1
	.db 0x93,0x30			;CMD Portamento down
					;Wait Repeat 1
	.db 0x6c			;Volume 10
	.db 0xc1			;Wait 2
	.db 0x6b			;Volume 9
					;Wait Repeat 2
	.db 0x6a			;Volume 8
	.db 0xc0			;Wait 1
	.db 0x69			;Volume 7
	.db 0xd7			;Wait 24
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_068:
	.db 0x17			;Note B-2
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xc4			;Wait 5
	.db 0x70			;Volume 14
	.db 0xc0			;Wait 1
	.db 0x93,0x30			;CMD Portamento down
					;Wait Repeat 1
	.db 0x6f			;Volume 13
	.db 0xc1			;Wait 2
	.db 0x6e			;Volume 12
					;Wait Repeat 2
	.db 0x6d			;Volume 11
	.db 0xc0			;Wait 1
	.db 0x6c			;Volume 10
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_073:
	.db 0x18			;Note C-3
	.db 0x6f			;Volume 13
	.db 0x75			;Instrument 3
	.db 0x9c			;CMD End 
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_076:
	.db 0x18			;Note C-3
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_077:
	.db 0x38			;Note G#5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x57			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_078:
	.db 0x3c			;Note C-6
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_079:
	.db 0x3c			;Note C-6
	.db 0x6a			;Volume 8
	.db 0x7c			;Instrument 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc2			;Wait 3
	.db 0x95,0x16			;CMD Vibrato
					;Wait Repeat 3
	.db 0x68			;Volume 6
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
					;Wait Repeat 6
	.db 0x66			;Volume 4
	.db 0xd0			;Wait 17
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_080:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x29			;Note F-4
					;Wait Repeat 6
	.db 0x29			;Note F-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_081:
	.db 0x17			;Note B-2
	.db 0x75			;Instrument 3
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_084:
	.db 0x17			;Note B-2
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_085:
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x57			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_086:
	.db 0x3b			;Note B-5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_087:
	.db 0xc3			;Wait 4
	.db 0x3b			;Note B-5
	.db 0x69			;Volume 7
	.db 0x7c			;Instrument 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc2			;Wait 3
	.db 0x95,0x16			;CMD Vibrato
					;Wait Repeat 3
	.db 0x68			;Volume 6
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
					;Wait Repeat 6
	.db 0x66			;Volume 4
	.db 0xcd			;Wait 14
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_092:
	.db 0x16			;Note A#2
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_093:
	.db 0x36			;Note F#5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x57			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_094:
	.db 0x3a			;Note A#5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_095:
	.db 0xc3			;Wait 4
	.db 0x3a			;Note A#5
	.db 0x69			;Volume 7
	.db 0x7c			;Instrument 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc2			;Wait 3
	.db 0x95,0x16			;CMD Vibrato
					;Wait Repeat 3
	.db 0x68			;Volume 6
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
					;Wait Repeat 6
	.db 0x66			;Volume 4
	.db 0xcd			;Wait 14
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_096:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x29			;Note F-4
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x29			;Note F-4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_101:
	.db 0x37			;Note G-5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x57			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc4			;Wait 5
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_102:
	.db 0x3b			;Note B-5
	.db 0x6e			;Volume 12
	.db 0x7c			;Instrument 10
	.db 0x9c			;CMD End 
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6d			;Volume 11
	.db 0xc5			;Wait 6
	.db 0x6c			;Volume 10
					;Wait Repeat 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
	.db 0xc4			;Wait 5
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_103:
	.db 0xc3			;Wait 4
	.db 0x3b			;Note B-5
	.db 0x69			;Volume 7
	.db 0x7c			;Instrument 10
	.db 0xa2,0x08			;CMD Track detune
	.db 0xc2			;Wait 3
	.db 0x95,0x16			;CMD Vibrato
					;Wait Repeat 3
	.db 0x68			;Volume 6
	.db 0xc5			;Wait 6
	.db 0x67			;Volume 5
					;Wait Repeat 6
	.db 0x66			;Volume 4
	.db 0xcc			;Wait 13
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_104:
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x29			;Note F-4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x29			;Note F-4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x29			;Note F-4
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_105:
	.db 0x24			;Note C-4
	.db 0x6c			;Volume 10
	.db 0x78			;Instrument 6
	.db 0xc8			;Wait 9
	.db 0x23			;Note B-3
					;Wait Repeat 9
	.db 0x22			;Note A#3
					;Wait Repeat 9
	.db 0x23			;Note B-3
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_108:
	.db 0x18			;Note C-3
	.db 0x7a			;Instrument 8
	.db 0xc8			;Wait 9
	.db 0x17			;Note B-2
					;Wait Repeat 9
	.db 0x16			;Note A#2
					;Wait Repeat 9
	.db 0x17			;Note B-2
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_109:
	.db 0x2d			;Note A-4
	.db 0x6e			;Volume 12
	.db 0x81			;Instrument 15
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
					;Wait Repeat 9
	.db 0x28			;Note E-4
					;Wait Repeat 9
	.db 0x27			;Note D#4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_110:
	.db 0x30			;Note C-5
	.db 0x6e			;Volume 12
	.db 0x80			;Instrument 14
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x2e			;Note A#4
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_111:
	.db 0x30			;Note C-5
	.db 0x68			;Volume 6
	.db 0x81			;Instrument 15
	.db 0xa2,0x09			;CMD Track detune
	.db 0xcc			;Wait 13
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0x2e			;Note A#4
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_117:
	.db 0x2d			;Note A-4
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
					;Wait Repeat 9
	.db 0x28			;Note E-4
					;Wait Repeat 9
	.db 0x27			;Note D#4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_118:
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 10
	.db 0x80			;Instrument 14
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x2e			;Note A#4
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_119:
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x2e			;Note A#4
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_121:
	.db 0x24			;Note C-4
	.db 0x6c			;Volume 10
	.db 0x78			;Instrument 6
	.db 0xc8			;Wait 9
	.db 0x23			;Note B-3
					;Wait Repeat 9
	.db 0x24			;Note C-4
					;Wait Repeat 9
	.db 0x23			;Note B-3
	.db 0xc6			;Wait 7
	.db 0x23			;Note B-3
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_124:
	.db 0x18			;Note C-3
	.db 0x7a			;Instrument 8
	.db 0xc8			;Wait 9
	.db 0x17			;Note B-2
					;Wait Repeat 9
	.db 0x18			;Note C-3
					;Wait Repeat 9
	.db 0x17			;Note B-2
	.db 0xc6			;Wait 7
	.db 0x17			;Note B-2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_125:
	.db 0x2d			;Note A-4
	.db 0xc8			;Wait 9
	.db 0x2a			;Note F#4
					;Wait Repeat 9
	.db 0x2d			;Note A-4
					;Wait Repeat 9
	.db 0x2a			;Note F#4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_126:
	.db 0x30			;Note C-5
	.db 0x6c			;Volume 10
	.db 0x80			;Instrument 14
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x30			;Note C-5
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_127:
	.db 0xc3			;Wait 4
	.db 0x30			;Note C-5
	.db 0xc8			;Wait 9
	.db 0x2f			;Note B-4
					;Wait Repeat 9
	.db 0x30			;Note C-5
					;Wait Repeat 9
	.db 0x2f			;Note B-4
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_129:
	.db 0x22			;Note A#3
	.db 0x6d			;Volume 11
	.db 0x78			;Instrument 6
	.db 0xe3			;Wait 36
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_132:
	.db 0x16			;Note A#2
	.db 0x71			;Volume 15
	.db 0x7a			;Instrument 8
	.db 0xe1			;Wait 34
	.db 0x16			;Note A#2
	.db 0x79			;Instrument 7
	.db 0xc1			;Wait 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_133:
	.db 0x29			;Note F-4
	.db 0x81			;Instrument 15
	.db 0xc3			;Wait 4
	.db 0x95,0x67			;CMD Vibrato
	.db 0xcd			;Wait 14
	.db 0x28			;Note E-4
	.db 0x71			;Volume 15
	.db 0x7f			;Instrument 13
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
					;Wait Repeat 6
	.db 0x2d			;Note A-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_134:
	.db 0x2e			;Note A#4
	.db 0x6d			;Volume 11
	.db 0x80			;Instrument 14
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc8			;Wait 9
	.db 0x6c			;Volume 10
	.db 0xc5			;Wait 6
	.db 0x6b			;Volume 9
					;Wait Repeat 6
	.db 0x6a			;Volume 8
					;Wait Repeat 6
	.db 0x2d			;Note A-4
	.db 0x69			;Volume 7
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x2a			;Note F#4
					;Wait Repeat 1
	.db 0x29			;Note F-4
					;Wait Repeat 1
	.db 0x28			;Note E-4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_135:
	.db 0xc3			;Wait 4
	.db 0x2e			;Note A#4
	.db 0x69			;Volume 7
	.db 0x81			;Instrument 15
	.db 0xaa,0x08			; SCC Set Waveform 2
	.db 0xc2			;Wait 3
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd6			;Wait 23
	.db 0x2d			;Note A-4
	.db 0xc0			;Wait 1
	.db 0x2c			;Note G#4
					;Wait Repeat 1
	.db 0x2b			;Note G-4
					;Wait Repeat 1
	.db 0x2a			;Note F#4
					;Wait Repeat 1
	.db 0x29			;Note F-4
					;Wait Repeat 1
	.db 0x28			;Note E-4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_136:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xcb			;Wait 12
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_137:
	.db 0x2e			;Note A#4
	.db 0x6d			;Volume 11
	.db 0x7e			;Instrument 12
	.db 0xa2,0x01			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0x2f			;Note B-4
	.db 0xc7			;Wait 8
	.db 0x95,0x36			;CMD Vibrato
	.db 0xcd			;Wait 14
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_140:
	.db 0x10			;Note E-2
	.db 0x7a			;Instrument 8
	.db 0xcb			;Wait 12
	.db 0x10			;Note E-2
	.db 0xc5			;Wait 6
	.db 0x12			;Note F#2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_141:
	.db 0x2e			;Note A#4
	.db 0x7f			;Instrument 13
	.db 0xc1			;Wait 2
	.db 0x2f			;Note B-4
	.db 0xd5			;Wait 22
	.db 0x2d			;Note A-4
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_142:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xcb			;Wait 12
	.db 0x26			;Note D-4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_143:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xcb			;Wait 12
	.db 0x1f			;Note G-3
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_145:
	.db 0xc1			;Wait 2
	.db 0x2d			;Note A-4
	.db 0xc7			;Wait 8
	.db 0x95,0x36			;CMD Vibrato
	.db 0xcf			;Wait 16
	.db 0x2b			;Note G-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2a			;Note F#4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_148:
	.db 0x13			;Note G-2
	.db 0x7a			;Instrument 8
	.db 0xd1			;Wait 18
	.db 0x10			;Note E-2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_149:
	.db 0x2d			;Note A-4
	.db 0x7f			;Instrument 13
	.db 0xd7			;Wait 24
	.db 0x2b			;Note G-4
	.db 0xc5			;Wait 6
	.db 0x2a			;Note F#4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_150:
	.db 0x26			;Note D-4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc2			;Wait 3
	.db 0x67			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x64			;Volume 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_151:
	.db 0x1f			;Note G-3
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc2			;Wait 3
	.db 0x67			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x64			;Volume 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_152:
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x37			;Note G-5
	.db 0x76			;Instrument 4
	.db 0xcb			;Wait 12
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 12
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_153:
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x2b			;Note G-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x28			;Note E-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_156:
	.db 0x15			;Note A-2
	.db 0x7a			;Instrument 8
	.db 0xcb			;Wait 12
	.db 0x15			;Note A-2
	.db 0xc5			;Wait 6
	.db 0x17			;Note B-2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_157:
	.db 0x2b			;Note G-4
	.db 0x7f			;Instrument 13
	.db 0xc5			;Wait 6
	.db 0x70			;Volume 14
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xc1			;Wait 2
	.db 0x2b			;Note G-4
	.db 0x71			;Volume 15
	.db 0x7f			;Instrument 13
	.db 0xcb			;Wait 12
	.db 0x28			;Note E-4
	.db 0xc5			;Wait 6
	.db 0x26			;Note D-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_158:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xcb			;Wait 12
	.db 0x25			;Note C#4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_159:
	.db 0x60			;Release 96
	.db 0x9c			;CMD End 
	.db 0xcb			;Wait 12
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc5			;Wait 6
	.db 0x60			;Release 96
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_160:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0xc3			;Wait 4
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x9b,0x02			;CMD Note delay
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_161:
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0xc7			;Wait 8
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc9			;Wait 10
	.db 0x28			;Note E-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
					;Wait Repeat 6
	.db 0x2d			;Note A-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_164:
	.db 0x15			;Note A-2
	.db 0x7a			;Instrument 8
	.db 0xd1			;Wait 18
	.db 0x12			;Note F#2
	.db 0xd1			;Wait 18
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_165:
	.db 0x28			;Note E-4
	.db 0x7f			;Instrument 13
	.db 0xd1			;Wait 18
	.db 0x28			;Note E-4
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
					;Wait Repeat 6
	.db 0x2d			;Note A-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_166:
	.db 0x25			;Note C#4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc2			;Wait 3
	.db 0x67			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x64			;Volume 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_167:
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc2			;Wait 3
	.db 0x67			;Volume 5
	.db 0xc0			;Wait 1
	.db 0x65			;Volume 3
					;Wait Repeat 1
	.db 0x64			;Volume 2
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_169:
	.db 0x2e			;Note A#4
	.db 0x6d			;Volume 11
	.db 0x7e			;Instrument 12
	.db 0xa2,0x01			;CMD Track detune
	.db 0xc3			;Wait 4
	.db 0x2f			;Note B-4
	.db 0xc5			;Wait 6
	.db 0x95,0x36			;CMD Vibrato
	.db 0xcf			;Wait 16
	.db 0x2d			;Note A-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_173:
	.db 0x2e			;Note A#4
	.db 0x7f			;Instrument 13
	.db 0xc2			;Wait 3
	.db 0x2f			;Note B-4
	.db 0xd4			;Wait 21
	.db 0x2d			;Note A-4
	.db 0xc5			;Wait 6
	.db 0x2b			;Note G-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_184:
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xcb			;Wait 12
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
					;Wait Repeat 12
	.db 0x29			;Note F-4
	.db 0x6f			;Volume 13
	.db 0x74			;Instrument 2
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_185:
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x28			;Note E-4
	.db 0x9c			;CMD End 
	.db 0xc6			;Wait 7
	.db 0x95,0x36			;CMD Vibrato
	.db 0xc4			;Wait 5
	.db 0x28			;Note E-4
	.db 0x9c			;CMD End 
	.db 0xc5			;Wait 6
	.db 0x26			;Note D-4
	.db 0xc3			;Wait 4
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_189:
	.db 0x28			;Note E-4
	.db 0x7f			;Instrument 13
	.db 0xc5			;Wait 6
	.db 0x70			;Volume 14
	.db 0x9c			;CMD End 
	.db 0xc0			;Wait 1
	.db 0x6f			;Volume 13
					;Wait Repeat 1
	.db 0x6e			;Volume 12
					;Wait Repeat 1
	.db 0x6c			;Volume 10
					;Wait Repeat 1
	.db 0x60			;Release 96
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0x71			;Volume 15
	.db 0x7f			;Instrument 13
	.db 0xcb			;Wait 12
	.db 0x28			;Note E-4
	.db 0xc5			;Wait 6
	.db 0x26			;Note D-4
	.db 0xc5			;Wait 6
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_192:
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xd1			;Wait 18
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc8			;Wait 9
	.db 0x37			;Note G-5
	.db 0x6f			;Volume 13
	.db 0x76			;Instrument 4
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_193:
	.db 0xc1			;Wait 2
	.db 0x28			;Note E-4
	.db 0x7e			;Instrument 12
	.db 0xc7			;Wait 8
	.db 0x95,0x36			;CMD Vibrato
	.db 0xd9			;Wait 26
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_197:
	.db 0x28			;Note E-4
	.db 0x7f			;Instrument 13
	.db 0xe2			;Wait 35
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_198:
	.db 0x25			;Note C#4
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xaa,0x18			; SCC Set Waveform 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc4			;Wait 5
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_199:
	.db 0x21			;Note A-3
	.db 0x6f			;Volume 13
	.db 0x7a			;Instrument 8
	.db 0xc2			;Wait 3
	.db 0x95,0x65			;CMD Vibrato
	.db 0xda			;Wait 27
	.db 0x69			;Volume 7
	.db 0xc4			;Wait 5
	.db 0x9c			;CMD End 
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_200:
	.db 0x21			;Note A-3
	.db 0x70			;Volume 14
	.db 0x73			;Instrument 1
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_201:
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_204:
	.db 0x22			;Note A#3
	.db 0x7b			;Instrument 9
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_205:
	.db 0x33			;Note D#5
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_206:
	.db 0x36			;Note F#5
	.db 0x80			;Instrument 14
	.db 0xc8			;Wait 9
	.db 0xbf			;[End-Of-Track]
kv2puzz_track_207:
	.db 0xc3			;Wait 4
	.db 0x36			;Note F#5
	.db 0x82			;Instrument 16
	.db 0xc4			;Wait 5
	.db 0xbf			;[End-Of-Track]

