; ____________________________
; ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄       ▄▄            
; ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄█ ██▄▀ ██▄▀ ▄█▀▄  ██▀
; █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ██   ██ █ ▀█▄▀ ▄██ 
; ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
;  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
;─────────────────────────────────────────────────────────────────────────────
; Arkos AKG replayer (Generic)
;
; Arkos Tracker and the Z80 players are coded and designed by Julien Névo a.k.a Targhan/Arkos.
;  https://www.julien-nevo.com/arkostracker/index.php/credits/
;─────────────────────────────────────────────────────────────────────────────

; Setup replayer
PLY_AKG_HARDWARE_MSX = 1
PLY_AKG_MANAGE_SOUND_EFFECTS = 1
PLY_AKG_Rom = 1
PLY_AKG_ROM_Buffer = #F000

; Include replayer
include "playerAkg/sources/PlayerAkg.asm"