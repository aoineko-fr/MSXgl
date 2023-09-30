<img width="100%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/logo/msxgl-tate-logo.png" title="MSXgl" />

**MSXgl is a Game Library wrote in C and targeting MSX computers.**

The goal of this library is to offer to C programmers, the **whole set of functionalities to create a game** with high performances.
Even if a C program can never be as efficient as a program written entirely in assembler, the goal is to try to get as close as possible.

To achieve this goal, the **library is dynamically compiled** to allow the user to configure the functionalities to best suit his needs.
Also, the most expensive features are written in assembler.

The other goal was to create a library that could be **totally independent of the BIOS**. This makes it much easier to use the 16K of page 0.
The library also provides interrupt handling code to create ROMs that always remain on page 0.

**Many sample programs** are available to demonstrate the different features of the library.
<div>
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_text.jpg" title="Text mode sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_draw.jpg" title="Drawn function sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_psg.jpg" title="PSG control sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_game.png" title="Game template sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_scroll.png" title="Tile-based scrolling sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_print.jpg" title="Print text sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_sprite.jpg" title="MSX2 sprite sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_clock.jpg" title="Realtime Clock sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_swsprt.png" title="Software sprite sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_vdpcmd.jpg" title="VDP commands sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_wyz.png" title="WYZ replayer sample" />
<img width="24%" src="https://raw.githubusercontent.com/aoineko-fr/MSXgl/main/engine/doc/img/sample_math.jpg" title="Mathematics sample" />
</div>

The MSXgl Build Tool allows in 1 click to create a final program for a **large number of formats**:
* **Plain ROM** from 8K to 64K (interrupt handler can be install in page 0),
* **Mapped ROM** from 64K to 4096K (ASCII-8, ASCII-16, Konami and Konami SCC mappers),
* **MSX-DOS** 1 or 2 binary program,
* **BASIC** binary program.

The library supports the following devices:
* VDP (TMS9918, V9938 and V9958) and V9990,
* PSG (AY-3-8910), MSX-Music, MSX-Audio, Konami SCC,
* Keyboard, Joystick, Mouse and Ninja Tap (4 joysticks adaptator),
* ROM mapper, slot manager, and Real Time Clock (RP-5C01).

Here are also the supported audio formats:
* Arkos Tracker 2 (AKG, AKY and AKM)
* Trilo Tracker SCC
* Vortex Tracker II (PT3)
* WYZ Tracker (WYZ)
* ayFX
* VGM (for all supported audio chip)
* lVGM (a light-VGM format for PSG)
* PCM-Encoder (aka. _Crystal clean PCM 8bit samples on the poor PSG_)

Many thanks to the members of the [MSX Ressource Center](https://www.msx.org) and [MSX Village](https://msxvillage.fr) who have been very helpful.

The library is distribute under Creative Commons BY-SA license. Tools or sources from other authors could use other free license.

Documentation:
* Code documentation: /engine/doc/html/ or https://aoineko.org/msxgl-doc
* Library documentation: https://aoineko.org/msxgl

If you have any question, feel free to join the [MSXgl Discord server](https://discord.gg/pMeadGfv8E).

## Quick installation
### macOS
#### Dependencies
Install NVM (Node.js version manager) and SDCC (Small Device C Compiler):
```
brew install nvm sdcc
```
#### Quarantine
To be able to run the new binaries in the recent versions of macOS, you need to de-quarantine them:
```
xattr -r -d com.apple.quarantine ./MSXgl
```

