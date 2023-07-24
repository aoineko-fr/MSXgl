;-----------------------------------------------------------------------------
; WYZ file merged for MSXgl on 24/07/2023 23:08:29,14 
;-----------------------------------------------------------------------------
.module wyz_pt10 
_pt10_Song:: .DW SONG00 

; Tabla de instrumentos
_pt10_Inst:: .DW PAUTA_0,PAUTA_1,PAUTA_2

; Tabla de efectos
_pt10_FX:: .DW SONIDO0,SONIDO1

;Pautas (instrumentos)
;Instrumento 'Piano'
PAUTA_0:	.DB	13,0,11,0,9,0,5,0,129
;Instrumento 'blip'
PAUTA_1:	.DB	11,0,8,0,9,0,7,0,5,0,0,129
;Instrumento 'piano 2'
PAUTA_2:	.DB	8,0,5,0,6,0,4,0,2,0,0,129

;Efectos
;Efecto 'bass'
SONIDO0:	.DB	69,47,0,69,92,0,255
;Efecto 'snare'
SONIDO1:	.DB	46,47,0,0,107,6,255

;Frecuencias para las notas
_pt10_Freq:: .DW 0,0
.DW 1711,1614,1524,1438,1358,1281,1210,1142,1078,1017
.DW 960,906,855,807,762,719,679,641,605,571
.DW 539,509,480,453,428,404,381,360,339,320
.DW 302,285,269,254,240,227,214,202,190,180
.DW 170,160,151,143,135,127,120,113,107,101
.DW 95,90,85,80,76,71,67,64,60,57
; Song data 
SONG00: 
; End of file 
_pt10_End:: 
