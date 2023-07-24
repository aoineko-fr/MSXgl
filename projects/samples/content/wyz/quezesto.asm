;-----------------------------------------------------------------------------
; WYZ file merged for MSXgl on 24/07/2023 23:08:29,43 
;-----------------------------------------------------------------------------
.module wyz_quezesto 
_quezesto_Song:: .DW SONG00 

; Tabla de instrumentos
_quezesto_Inst:: .DW PAUTA_0,PAUTA_1,PAUTA_2,PAUTA_3

; Tabla de efectos
_quezesto_FX:: .DW SONIDO0,SONIDO1,SONIDO2

;Pautas (instrumentos)
;Instrumento 'PIANO-REVERB'
PAUTA_1:	.DB	10,0,11,0,10,0,9,0,8,0,8,0,8,0,8,0,8,0,8,0,7,0,7,0,7,0,7,0,136
;Instrumento 'BAJO'
PAUTA_2:	.DB	44,0,11,0,10,0,9,0,8,0,129
;Instrumento 'ECO'
PAUTA_3:	.DB	9,0,8,0,7,0,6,0,129
;Instrumento 'Clipclop'
PAUTA_0:	.DB	13,0,12,0,11,0,10,0,10,0,10,0,10,0,10,0,9,0,9,0,9,0,9,0,9,0,138

;Efectos
;Efecto 'DRUM'
SONIDO0:	.DB	0,62,0,0,89,0,255
;Efecto 'SNARE'
SONIDO1:	.DB	209,46,0,46,59,1,0,87,7,255
;Efecto 'HIHAT'
SONIDO2:	.DB	0,10,2,255

;Frecuencias para las notas
_quezesto_Freq:: .DW 0,0
.DW 1711,1614,1524,1438,1358,1281,1210,1142,1078,1017
.DW 960,906,855,807,762,719,679,641,605,571
.DW 539,509,480,453,428,404,381,360,339,320
.DW 302,285,269,254,240,227,214,202,190,180
.DW 170,160,151,143,135,127,120,113,107,101
.DW 95,90,85,80,76,71,67,64,60,57
; Song data 
SONG00: 
; End of file 
_quezesto_End:: 
