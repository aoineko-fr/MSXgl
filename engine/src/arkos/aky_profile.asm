; ____________________________
; ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;─────────────────────────────────────────────────────────────────────────────
; Arkos AKY replayer (Fastest)
;
; Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
;  https://www.julien-nevo.com/arkostracker/index.php/credits/
;─────────────────────────────────────────────────────────────────────────────

; Setup replayer
PLY_AKY_HARDWARE_MSX = 1
PLY_AKY_ROM = 1
PLY_AKY_ROM_Buffer = #F000

; Include replayer
include "playerAky/sources/z80/PlayerAky.asm"