;-----------------------------------------------------------------------------
; WYZ file merged for MSXgl on 24/07/2023 23:08:28,94 
;-----------------------------------------------------------------------------
.module wyz_megalovania_6ch 
_megalovania_6ch_Song:: .DW SONG00 

; Tabla de instrumentos
_megalovania_6ch_Inst:: .DW PAUTA_0,PAUTA_1,PAUTA_2,PAUTA_3,PAUTA_4,PAUTA_5,PAUTA_6,PAUTA_7,PAUTA_8,PAUTA_9,PAUTA_10

; Tabla de efectos
_megalovania_6ch_FX:: .DW SONIDO0,SONIDO1,SONIDO2,SONIDO3,SONIDO4,SONIDO5,SONIDO6,SONIDO7,SONIDO8

;Pautas (instrumentos)
;Instrumento 'Piano1'
PAUTA_0:	.DB	12,0,12,0,12,0,12,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,136
;Instrumento 'Piano 2'
PAUTA_1:	.DB	9,-1,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,136
;Instrumento 'Piano 1 -1'
PAUTA_2:	.DB	42,0,10,-16,10,16,10,0,10,-2,10,2,130
;Instrumento 'Flute 1'
PAUTA_3:	.DB	9,0,11,-2,13,2,13,-2,13,-2,4,2,4,2,4,0,4,0,4,0,130
;Instrumento 'Piano +1oct'
PAUTA_4:	.DB	76,0,12,0,12,0,12,0,7,0,7,0,7,0,7,0,132
;Instrumento 'slide up 2'
PAUTA_5:	.DB	8,0,8,-6,129
;Instrumento 'slide down 2'
PAUTA_6:	.DB	8,0,8,7,129
;Instrumento 'Bass'
PAUTA_7:	.DB	44,0,12,-8,12,8,12,8,8,-7,8,0,8,0,8,0,0,0,129
;Instrumento 'slide oct+1'
PAUTA_8:	.DB	24,-1,24,-1,24,-2,24,-2,24,-3,24,-3,24,-4,24,-4,24,-5,24,-5,24,-6,24,-6,24,-7,24,-7,24,-8,24,-8,24,-9,24,-9,24,-10,24,-10,24,-11,24,-11,24,-12,129
;Instrumento 'Flute 2'
PAUTA_9:	.DB	9,0,11,-2,11,0,11,-1,11,0,10,0,9,1,9,0,8,0,8,-1,8,0,24,0,24,0,7,1,23,0,23,0,23,0,23,0,138
;Instrumento 'Med 2'
PAUTA_10:	.DB	7,-1,7,2,7,0,7,0,7,0,7,-2,7,0,7,0,7,0,7,2,7,0,7,-2,7,0,140

;Efectos
;Efecto 'bass drum'
SONIDO0:	.DB	128,48,128,0,1,8,0,76,0,130,74,0,0,89,0,255
;Efecto 'drum'
SONIDO1:	.DB	168,32,128,0,1,8,0,10,8,0,7,8,0,6,8,255
;Efecto 'hithat'
SONIDO2:	.DB	0,8,2,255
;Efecto 'bass drum vol 2'
SONIDO3:	.DB	232,56,0,208,116,0,255
;Efecto 'drum 2'
SONIDO4:	.DB	202,32,128,0,1,8,0,10,1,0,9,1,0,8,1,255
;Efecto 'hita hat 2'
SONIDO5:	.DB	0,8,1,255
;Efecto 'Bongo 1'
SONIDO6:	.DB	192,16,128,192,1,8,0,32,128,0,3,1,64,32,128,0,8,1,128,32,128,0,8,1,255
;Efecto 'Bongo 2'
SONIDO7:	.DB	64,32,128,190,1,8,128,32,128,0,3,1,192,32,128,0,8,1,0,48,128,0,8,1,255
;Efecto 'LARGE DRUM'
SONIDO8:	.DB	168,32,128,192,0,8,0,10,8,0,10,8,0,9,8,0,8,8,0,7,8,0,6,8,0,5,8,0,4,8,0,3,8,0,2,5,0,2,5,255

;Frecuencias para las notas
_megalovania_6ch_Freq:: .DW 0,0
.DW 1681,1586,1497,1413,1334,1259,1189,1122,1059,999
.DW 943,890,840,793,749,706,667,630,594,561
.DW 530,500,472,445,420,397,374,354,333,314
.DW 297,280,264,250,236,223,210,198,187,177
.DW 167,157,148,140,133,125,118,111,105,99
.DW 93,88,84,79,75,70,66,63,59,56
; Song data 
SONG00: 
; End of file 
_megalovania_6ch_End:: 
