# PENG-PONG
A penguin volley game!

## Game Sheet
- <ins>Machine</ins>: **MSX1** (enhanced on **MSX2** or above)
- <ins>RAM</ins>:     **16 KB**
- <ins>Format</ins>:  **32 KB ROM**
- <ins>Input</ins>:   **Keyboard** and/or **Joystick**
- <ins>Players</ins>: **1** or **2**

## Gameplay
Peng-Pong is a fun sports game where two penguins face off using rules inspired by ping-pong and volleyball.

### Rules
Several settings can be customized to tailor the rules:
- `POINTS`: The number of points required to win a match (default: 11)
- `BOUNCES`: The number of ground bounces allowed before the ball must cross the "net" (default: 1)
- `PASSES`: The number of times a player can hit the ball before the ball must cross the "net" (default: 3)

A player scores a point when:
- The ball goes out on the opponent’s side after bouncing at least once on the ground (if opponent don't touch the ball, it's a "passing")
- The ball goes out on their side without touching them and without bouncing on the ground (the opponent made a "out")
- The opponent lets the ball bounce too many times on their side of the court, or hits it more times than allowed (this is considered a "fault")

### Modes
- `PLY VS PLY`: Face off against a friend (or maybe a future ex-friend?)
- `PLY VS CPU`: Challenge an AI opponent with three difficulty levels (Easy, Medium, and Hard)

Note: You can exit a match at any time by pressing the `[Esc]` key.

### Input
Les menus peuvent être controlés depuis le clavier (flêches de direction et touche `[Space]`) ou une manette standard MSX branché sur les ports 1 ou 2.
En jeu, vous avez 4 configurations possibles :
- `DFG`:
  - `[D]`: Left
  - `[G]`: Right
  - `[F]`: Jump
- `⯇⯅⯈`:
  - `[⯇]`: Left
  - `[⯈]`: Right
  - `[⯅]` or `[Space]`: Jump
- `🕹️1` or `🕹️2`:
  - `[⯇]`: Left
  - `[⯈]`: Right
  - `[Button 1]`: Jump

## Options
- `AUDIO`: Audio settings
  - `MUSIC`: Activate/desactivate music playback
  - `SFX`: Activate/desactivate SFX playback
- VIDEO: 
  - `CLR MIX`: Activate/desactivate color mix (this generates more color, but can cause flickering on some displays)
  - `RULE CLR`: Enables/disables ball color change to reflect critical moments
  - `FREQ`:
    - `50 HZ`: Force 50 HZ for gameplay (on MSX2, VDP is also set to PAL)
    - `60 HZ`: Force 60 HZ for gameplay (on MSX2, VDP is also set to NTSC)
    - `AUTO`: Uses MSX default frequency
  - `PALETTE`: Changes the game palette (MSX2 only)
- `SAVE`: Saves options in the Real-time clock's memory (MSX2 only)
- `RESET`: Reset options to default

## MSX2 enhancements
If the game is 100% compatible with MSX1 standard, it bring some ehancement when played on a MSX2, MSX2+ or MSX turbo R:
- Use of Screen 4 (same as Screen 2 but with MSX2 sprites: no more flickers)
- Enable custom palette (4 palettes available)
- Enable video processor frequency switch (50 <> 60 Hz)

## Startup keys
You can press those keys when starting the game to force some special behavior:
- `[1]`: Force MSX1 mode
- `[2]`: Force MSX2 mode
- `[3]`: Reset options (including the video processor frequency)
