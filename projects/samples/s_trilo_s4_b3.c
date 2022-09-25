// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  ROM mapper sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

INCLUDE_ASM(sfx_wavetable, "content/trilo/SFX_TEST_WT.asm");
#include "content/trilo/SFX_TEST.h"

INCLUDE_ASM(tmu_kv2puzz,  "content/trilo/tmu_kv2puzz.asm");
INCLUDE_ASM(tmu_nem3st1,  "content/trilo/tmu_nem3st1.asm");