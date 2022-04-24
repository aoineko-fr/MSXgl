# MSXgl

**MSXGL is a Game Library wrote in C and targeting MSX computers.**

The goal of this library is to offer to C programmers, the **whole set of functionalities to create a game** with high performances.
EvenEven if a C program can never be as efficient as a program written entirely in assembler, the goal is to try to get as close as possible.

To achieve this goal, the **library is dynamically compiled** to allow the user to configure the functionalities to best suit his needs.
Also, the most expensive features are written in assembler.

The other goal was to create a library that could be **totally independent of the BIOS**. This makes it much easier to use the 16K of page 0.
The library also provides interrupt handling code to create ROMs that always remain on page 0.

**Many sample programs** are available to demonstrate the different features of the library.
<div>
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_text.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_draw.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_psg.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_game.png" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_scroll.png" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_print.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_sprite.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_clock.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_swsprt.png" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_vdpcmd.jpg" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_wyz.png" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_math.jpg" />
</div>

The MSXgl Build Tool allows in 1 click to create a final program for a **large number of formats**:
* **Plain ROM** from 8K to 64K (interrupt handler can be install in page 0),
* **Mapped ROM** from 64K to 4096K (ASCII-8, ASCII-16, Konami and Konami SCC mappers),
* **MSX-DOS** 1 or 2 binary program,
* **BASIC** binary program.

The library supports the following devices:
* VDP (TMS9918 and V9938),
* PSG (AY-3-8910), MSX-Music, MSX-Audio, Konami SCC,
* Keyboard/Joystick, ROM mapper, slot manager, and Real Time Clock (RP-5C01).

Here are also the supported audio formats:
* PT3 (Vortex Tracker II)
* WYZ (WYZTracker)
* AKG, AKY and AKM (Arkos Tracker 2)
* ayFX
* VGM (for all supported audio chip)
* lVGM (a light-VGM format for PSG)
* PCM-Encoder (aka. _Crystal clean PCM 8bit samples on the poor PSG_)

Many thanks to the members of the [MSX Ressource Center](https://www.msx.org) and [MSX Village](https://msxvillage.fr) who have been very helpful.
The library is distribute under Creative Commons BY-SA license. Tools or sources from other authors could use other free license.

Documentation:
* Code documentation: /engine/doc/html/  (WIP)
* Library documentation: https://aoineko.org/msxgl (WIP)
