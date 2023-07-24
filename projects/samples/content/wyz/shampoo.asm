;-----------------------------------------------------------------------------
; WYZ file merged for MSXgl on 24/07/2023 23:08:29,79 
;-----------------------------------------------------------------------------
.module wyz_shampoo 
_shampoo_Song:: .DW SONG00 

; Tabla de instrumentos
_shampoo_Inst:: .DW PAUTA_0,PAUTA_1,PAUTA_2,PAUTA_3,PAUTA_4,PAUTA_5,PAUTA_6

; Tabla de efectos
_shampoo_FX:: .DW SONIDO0,SONIDO1,SONIDO2,SONIDO3

;Pautas (instrumentos)
;Instrumento 'Bajo'
PAUTA_0:	.DB	46,0,13,0,12,0,11,0,129
;Instrumento 'triririn'
PAUTA_1:	.DB	8,0,74,0,11,0,43,0,10,0,72,0,8,0,40,0,8,0,132
;Instrumento 'chip1'
PAUTA_2:	.DB	4,0,71,0,8,0,40,0,8,0,70,0,5,0,37,0,5,0,69,0,132
;Instrumento 'onda1'
PAUTA_3:	.DB	72,0,10,0,11,0,12,0,11,0,10,0,9,0,8,0,129
;Instrumento 'Onda2'
PAUTA_4:	.DB	69,0,6,0,7,0,7,0,5,0,5,0,129
;Instrumento 'Chip2'
PAUTA_5:	.DB	3,0,69,0,5,0,36,0,3,0,2,0,129
;Instrumento 'Onda3'
PAUTA_6:	.DB	43,0,12,0,12,0,12,0,12,0,11,0,11,0,11,0,11,0,10,0,10,0,10,0,10,0,11,0,11,0,11,0,11,0,144

;Efectos
;Efecto 'bass drum'
SONIDO0:	.DB	0,63,0,162,95,0,93,142,0,255
;Efecto 'drum'
SONIDO1:	.DB	232,47,0,0,94,5,23,109,5,232,123,5,255
;Efecto 'hithat'
SONIDO2:	.DB	0,9,1,0,7,1,255
;Efecto 'bass drum vol 2'
SONIDO3:	.DB	186,58,0,0,102,0,162,131,0,255

;Frecuencias para las notas
_shampoo_Freq:: .DW 0,0
.DW 1711,1614,1524,1438,1358,1281,1210,1142,1078,1017
.DW 960,906,855,807,762,719,679,641,605,571
.DW 539,509,480,453,428,404,381,360,339,320
.DW 302,285,269,254,240,227,214,202,190,180
.DW 170,160,151,143,135,127,120,113,107,101
.DW 95,90,85,80,76,71,67,64,60,57
; Song data 
SONG00: 
; End of file 
_shampoo_End:: 
