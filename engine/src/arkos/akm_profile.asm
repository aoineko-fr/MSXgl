; ____________________________
; ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;─────────────────────────────────────────────────────────────────────────────
; Arkos AKM replayer (Minimalist)
;
; Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
;  https://www.julien-nevo.com/arkostracker/index.php/credits/
;─────────────────────────────────────────────────────────────────────────────

; Setup replayer
PLY_AKM_HARDWARE_MSX = 1
PLY_AKM_MANAGE_SOUND_EFFECTS = 1
PLY_AKM_Rom = 1
PLY_AKM_ROM_Buffer = #F000

; Include replayer
include "playerAkm/sources/z80/PlayerAkm.asm"