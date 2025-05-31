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
- The number of points required to win a match (default: 11)
- The number of ground bounces allowed before the ball must cross the "net" (default: 1)
- The number of times a player can hit the ball before the ball must cross the "net" (default: 3)

A player scores a point when:
- The ball goes out on the opponent’s side after bouncing at least once on the ground (if opponent don't touch the ball, it's a "passing")
- The ball goes out on their side without touching them and without bouncing on the ground (the opponent made a "out")
- The opponent lets the ball bounce too many times on their side of the court, or hits it more times than allowed (this is considered a "fault")

### Modes
- PLY vs PLY: Face off against a friend (or maybe a future ex-friend?)
- PLY vs CPU: Challenge an AI opponent with three difficulty levels (Easy, Medium, and Hard)

### Options
- 

### MSX2 enhancements
If the game is 100% compatible with MSX1 standard, it bring some ehancement when played on a MSX2, MSX2+ or MSX turbo R:
- Use of Screen 4 (same as Screen 2 but with MSX2 sprites: no more flickers)
- Enable custom palette (4 palettes available)
- Enable video processor frequency switch (50 <> 60 Hz)

## Startup keys
You can press those keys when starting the game to force some special behavior:
- `[1]`: Force MSX1 mode
- `[2]`: Force MSX2 mode
- `[3]`: Reset options (including the video processor frequency)
