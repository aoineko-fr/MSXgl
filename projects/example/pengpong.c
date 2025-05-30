// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Example game
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

// MSXgl
#include "msxgl.h"
#include "game/game.h"
#include "game/pawn.h"
#include "game/menu.h"
#include "clock.h"
#include "math.h"
#include "string.h"
#include "fixed_point.h"
#include "version.h"
#include "compress/pletter.h"
#include "arkos/akm_player.h"

// Lib
#include "lib/phenix.h"

//=============================================================================
// DEFINES
//=============================================================================

// Fixed point format
//
// Format	Prec.	Min		Max
//-----------------------------------
// Q4.4		0.06	-8		7.94
// Q12.4	0.06	-2048	2047.94

// Gameplay value
#define BALL_GRAVITY				Q4_4_SET(0.15f)
#define FALL_MAX_SPEED				Q4_4_SET(3.0f)

#define JUMP_FORCE					Q4_4_SET(-3.0f)
#define GRAVITY						Q4_4_SET(0.2f)
#define MOVE_FRICTION				Q4_4_SET(0.1f)
#define MOVE_ACCEL					Q4_4_SET(0.33f)
#define MOVE_MAX_SPEED				Q4_4_SET(1.5f)

#define BOUNCE_FORCE				Q4_4_SET(-2.5f)
#define COL_DIST					16

#define SHADOW_COLOR				COLOR_LIGHT_BLUE

// Background
#define HORIZON_H					11
#define NET_H						7

// VRAM Tables Address
#define VRAM_PATTERN_TABLE			0x0000
#define VRAM_COLOR_TABLE			0x2000
#define VRAM_LAYOUT_TABLE			0x3800
#define VRAM_SPRITE_PATTERN			0x1800
#define VRAM_SPRITE_ATTRIBUTE		0x3E00

#define MUSIC_ADDRESS				0xE800
#define SFX_ADDRESS					0xE000

// Index of all menu pages
enum MENU_PAGES
{
	MENU_MAIN = 0, // Main page
	MENU_VERSUS,   // Start page
	MENU_SOLO,     // Start page
	MENU_RULES,    // Rules page
	MENU_OPTION,   // Options page
	MENU_AUDIO,    // Audio options page
	MENU_GRAPH,    // Graphical options page
	MENU_CREDITS1, // Credits page
	MENU_CREDITS2,    // Infos page
//.............................
	MENU_MAX,      // Number of menu
};

// Input set enumeration
enum INPUT_SET
{
	INPUT_SET_KB1 = 0,
	INPUT_SET_KB2,
	INPUT_SET_JOY1,
	INPUT_SET_JOY2,
//.............................
	INPUT_SET_MAX,
};

// Input action flag
enum INPUT_ACTION
{
	INPUT_NONE    = 0,
	INPUT_UP      = 0b00000001,
	INPUT_DOWN    = 0b00000010,
	INPUT_LEFT    = 0b00000100,
	INPUT_RIGHT   = 0b00001000,
	INPUT_BUTTON1 = 0b00010000,
	INPUT_BUTTON2 = 0b00100000,
	INPUT_ACTION  = 0b01000000,
	INPUT_7       = 0b10000000,
};

// Actions id
enum ACTION_PLAYER_ID
{
	ACTION_PLAYER_IDLE = 0,
	ACTION_PLAYER_MOVE,
	ACTION_PLAYER_JUMP,
	ACTION_PLAYER_FALL,
	ACTION_PLAYER_HIT,
	ACTION_PLAYER_WIN,
	ACTION_PLAYER_LOOSE,
//.............................
	ACTION_PLAYER_MAX,
};

// Actions id
enum ACTION_BALL_ID
{
	ACTION_BALL_IDLE = 0,
	ACTION_BALL_BUMP,
//.............................
ACTION_BALL_MAX,
};

// Score type
enum SCORE_ID
{
	SCORE_OUT = 0,
	SCORE_BOUNCE,
	SCORE_PASS,
	//.............................
	SCORE_MAX
};

// AI level
enum AI_LEVEL
{
	AI_EASY = 0,
	AI_MEDIUM,
	AI_HARD,
//.............................
	AI_MAX,
};

// Palette enumaration
enum PAL_ID
{
	PAL_CUSTOM = 0,
	PAL_MSX1,
	PAL_MSX2,
	PAL_GRAY,
//.............................
	PAL_MAX,
};

// Frequence enumaration
enum FREQ_MODE
{
	FREQ_AUTO,							// Use auto-detected frequency
	FREQ_60HZ,							// Force 60 Hz
	FREQ_50HZ,							// Force 50 Hz
//.............................
	FREQ_MAX,
};

// Sprite index
enum SPRITE_ID
{
	SPRITE_BALL_DARK,					// 0		Ball shadow
	SPRITE_BALL_LIGHT,					// 1		Ball light
	SPRITE_PLY1_BLACK,					// 2		Player 1 black
	SPRITE_PLY2_BLACK,					// 3		Player 2 black
	SPRITE_PLY1_RED,					// 4 		Player 1 red
	SPRITE_PLY2_RED,					// 5		Player 2 red
	SPRITE_WIN_BACK,					// 6
	SPRITE_WIN_FRONT,					// 7
	SPRITE_BALL_SHADOW,					// 8
	SPRITE_PLY1_SHADOW,					// 9
	SPRITE_PLY2_SHADOW,					// 10
	SPRITE_HIT_MARKER,					// 11
	SPRITE_CLOUD,						// 12-27
//.............................
	SPRITE_MAX,
};

// Musics index
enum MUSIC_ID
{
	MUSIC_EMPTY = 0,
	MUSIC_MAIN,
	MUSIC_VICTORY,
//.............................
	MUSIC_MAX,
};

// SFX index
enum SFX_ID
{
	SFX_CLICK = 0,
	SFX_BUMP,
	SFX_JUMP,
	SFX_BUMP2,
	SFX_DING,
	SFX_PUING,
	SFX_CUICUI,
	SFX_CUICUI2,
	SFX_CLICK2,
	SFX_TAC,
	SFX_TIC,
	SFX_SPEAK,
	SFX_LAND,
	SFX_DEBUG,
	SFX_SEA,
//.............................
	SFX_MAX,
};

// Gameplay character stricture
struct Character
{
	u8			ID;
	bool		bFreeze;
	bool		bMoving;
	bool		bInAir;
	i8			VelocityX;	// Format Q4.4 [-8:7.94]
	i8			VelocityY;	// Format Q4.4 [-8:7.94]
	i8			RestX;	// Format Q4.4 [-8:7.94]
	i8			RestY;	// Format Q4.4 [-8:7.94]
	u8			Input;
	u8			Score;
	Pawn		Pawn;
	u8			Shadow;
};

// Gameplay rule structure
struct Rule
{
	u8			GamePoints;
	u8			MaxBounce;
	u8			MaxPass;
};

// Option data structure
struct Option
{
	bool		Music;				// 1
	bool		SFX;				// 1
	bool		Blend;				// 1		Color blending
	bool		Feedback;			// 1		Ball rule feedback
	u8			Freq;				// 2
	u8			Palette;			// 3
	u8   		InputSet[3];		// 3 x 2
	u8			AILevel;			// 2		AI level for each player
	struct Rule	Rule;				// 6 + 4
};

// Cloud data structure
struct Cloud
{
	u8			X;
	u8			Y;
	u8			Pattern;
	u8			Sprite;
	u8			Mask;
};

// Functions
typedef u8 (*cbInputCheck)(void);				// Callback default signature

// States prototype
bool State_AppInit();
bool State_LogoInit();
bool State_LogoUpdate();
bool State_MenuInit();
bool State_MenuUpdate();
bool State_GameInit();
bool State_KickOff();
bool State_Game();
bool State_Pause();
bool State_Point();
bool State_VictoryInit();
bool State_VictoryUpdate();

void DrawScore();
void DrawInfo(u8 event);
void UpdateBallColor();
void SaveOptions();
void LoadOptions();
void SetSprite(u8 idx, u8 x, u8 y, u8 shape, u8 color);
void SetSpriteColor(u8 idx, u8 color);

// Menu function callback
void ApplyPaletteOption();
void ApplyFreqOption();
void ApplyMusicOption();
const c8* MenuAction_Start(u8 op, i8 value);
const c8* MenuAction_Input(u8 op, i8 value);
const c8* MenuAction_AI(u8 op, i8 value);
const c8* MenuAction_Freq(u8 op, i8 value);
const c8* MenuAction_Palette(u8 op, i8 value);
const c8* MenuAction_Music(u8 op, i8 value);
const c8* MenuAction_Save(u8 op, i8 value);
const c8* MenuAction_Reset(u8 op, i8 value);

// Input function callback
u8 CheckKB1();
u8 CheckKB2();
u8 CheckJoy1();
u8 CheckJoy2();
u8 CheckAI();
u8 GetBallHitX_Easy();
u8 GetBallHitX_Medium();
u8 GetBallHitX_Hard();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font by Ludo 'GFX'
#include "content/data_font.h"

// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_player.h"
#include "content/data_sprt_ball.h"
#include "content/data_sprt_extra.h"

// Background by Yaz
#include "content/data_bg2.h"

// Audio data
#include "content/music_empty.h"
#include "content/music_main.h"
#include "content/music_victory.h"
#include "content/sfx.h"

//.............................................................................
// Player 1 data

// Player's sprite layers
// This character is made by 3 layers (3 sprites) but the first two are special: One is only visible on even frames while the second only on odd frames.
// So, on the 3 layers, only two are visible at a given frame. The blinking of the first two black layers is done to create shaded colors.
// The counterpart is the flickering effect. The character white color comes from the background and is not in a sprite.
const Pawn_Sprite g_SpriteLayers[] =
{
//	  Sprite ID
//    |                  X offset from pawn's position
//    |                  |  Y offset
//    |                  |  |  Pattern offset from current animation key
//    |                  |  |  |  Layer's color
//    |                  |  |  |  |                Layer option
	{ SPRITE_PLY1_RED,   0, 0, 0, COLOR_LIGHT_RED, 0 },
	{ SPRITE_PLY1_BLACK, 0, 0, 4, COLOR_BLACK,     PAWN_SPRITE_BLEND }, // Only visible on even frame number
};

// Idle animation frames
// Each line describes an animation key
const Pawn_Frame g_FramesIdle[] =
{
//	  Pattern offset of this animation key in the sprite data
//    |     Animation key duration (in frame number)
//    |     |   Event to trigger during this animation key (function pointer)
	{ 6*12, 64, NULL },
	{ 7*12, 24, NULL },
};

// Move animation frames
const Pawn_Frame g_FramesMove[] =
{
	{ 0*12, 4, NULL },
	{ 1*12, 4, NULL },
	{ 2*12, 4, NULL },
	{ 3*12, 4, NULL },
};

// Jump animation frames
const Pawn_Frame g_FramesJump[] =
{
	{ 8*12, 4, NULL },
	{ 3*12, 4, NULL },
};

// Fall animation frames
const Pawn_Frame g_FramesFall[] =
{
	{ 9*12,	4, NULL },
};

// 
const Pawn_Frame g_FramesHit[] =
{
	{ 12*12, 12, NULL },
};

// 
const Pawn_Frame g_FramesWin[] =
{
	{ 14*12, 25, NULL },
	{ 15*12, 25, NULL },
};

// 
const Pawn_Frame g_FramesLost[] =
{
	{ 13*12, 4,	NULL },
};

// List of all player actions
const Pawn_Action g_AnimActions[ACTION_PLAYER_MAX] =
{ //  Frames        Number               Loop? Interrupt?
	{ g_FramesIdle, numberof(g_FramesIdle), 1, 1 }, // ACTION_PLAYER_IDLE
	{ g_FramesMove, numberof(g_FramesMove), 1, 1 }, // ACTION_PLAYER_MOVE
	{ g_FramesJump, numberof(g_FramesJump), 1, 1 }, // ACTION_PLAYER_JUMP
	{ g_FramesFall, numberof(g_FramesFall), 1, 1 }, // ACTION_PLAYER_FALL
	{ g_FramesHit,  numberof(g_FramesHit),  0, 0 }, // ACTION_PLAYER_HIT
	{ g_FramesWin,  numberof(g_FramesWin),  1, 0 }, // ACTION_PLAYER_WIN
	{ g_FramesLost, numberof(g_FramesLost), 1, 0 }, // ACTION_PLAYER_LOOSE
};

//.............................................................................
// Player 2 data

// Pawn sprite layers
const Pawn_Sprite g_SpriteLayers2[] =
{
	{ SPRITE_PLY2_RED,   0, 0, 0, COLOR_LIGHT_RED, 0 },
	{ SPRITE_PLY2_BLACK, 0, 0, 4, COLOR_BLACK,     PAWN_SPRITE_BLEND },
};

//.............................................................................
// Ball data

// Pawn sprite layers
const Pawn_Sprite g_BallLayers[] =
{
	{ SPRITE_BALL_DARK,  0, 0, 4, COLOR_MEDIUM_RED, 0 },
	{ SPRITE_BALL_LIGHT, 0, 0, 0, COLOR_LIGHT_RED,  0 },
};

// Idle animation frames
const Pawn_Frame g_BallIdle[] =
{
	{ 0*8,	4,	NULL },
};

// Bump animation frames
const Pawn_Frame g_BallBump[] =
{
	{ 1*8,	1,	NULL },
	{ 2*8,	5,	NULL },
	{ 1*8,	2,	NULL },
};

// List of all ball actions
const Pawn_Action g_BallActions[ACTION_BALL_MAX] =
{ //  Frames      Number             Loop? Interrupt?
	{ g_BallIdle, numberof(g_BallIdle), 1, 1 }, // ACTION_BALL_IDLE
	{ g_BallBump, numberof(g_BallBump), 0, 0 }, // ACTION_BALL_BUMP
};

//.............................................................................

// Clouds data
const struct Cloud g_Cloud[] =
{
	{  30,  8,  80 + 64, SPRITE_CLOUD + 0,  0b00000111 },
	{  46,  8,  88 + 64, SPRITE_CLOUD + 2,  0b00000111 },
	{ 140, 20,  96 + 64, SPRITE_CLOUD + 4,  0b00001111 },
	{ 156, 20, 104 + 64, SPRITE_CLOUD + 6,  0b00001111 },
	{   4, 30,  96 + 64, SPRITE_CLOUD + 8,  0b00011111 },
	{  20, 30, 104 + 64, SPRITE_CLOUD + 10, 0b00011111 },
	{ 124, 37, 112 + 64, SPRITE_CLOUD + 12, 0b00111111 },
	{ 248, 38, 120 + 64, SPRITE_CLOUD + 14, 0b01111111 },
};

// Custom palette
const u16 g_CustomPalette[15] =
{
	RGB16(0, 0, 0), // black				RGB16(0, 0, 0),
	RGB16(0, 4, 4), // medium green			RGB16(1, 5, 1),
	RGB16(3, 5, 6), // light green			RGB16(3, 6, 3),
	RGB16(2, 2, 6), // dark blue			RGB16(2, 2, 6),
	RGB16(1, 3, 6), // light blue			RGB16(3, 3, 7),
	RGB16(5, 2, 2), // dark red				RGB16(5, 2, 2),
	RGB16(3, 5, 7), // *cyan				RGB16(2, 6, 7),
	RGB16(6, 3, 3), // *medium red			RGB16(6, 2, 2),
	RGB16(7, 4, 4), // *light red			RGB16(6, 3, 3),
	RGB16(5, 5, 3), // *dark yellow			RGB16(5, 5, 2),
	RGB16(6, 6, 4), // *light yellow		RGB16(6, 6, 3),
	RGB16(2, 4, 5), // dark green			RGB16(1, 4, 1),
	RGB16(5, 2, 4), // *magenta				RGB16(5, 2, 5),
	RGB16(5, 5, 6), // gray					RGB16(5, 5, 5),
	RGB16(7, 7, 7)  // white				RGB16(7, 7, 7) 
};

// Gray scale palette
const u16 g_GrayPalette[15] =
{
	RGB16(0, 0, 0), // black				RGB16(0, 0, 0),
	RGB16(3, 3, 3), // medium green			RGB16(1, 5, 1),
	RGB16(6, 6, 6), // light green			RGB16(3, 6, 3),
	RGB16(2, 2, 2), // dark blue			RGB16(2, 2, 6),
	RGB16(3, 3, 3), // light blue			RGB16(3, 3, 7),
	RGB16(1, 1, 1), // dark red				RGB16(5, 2, 2),
	RGB16(5, 5, 5), // *cyan				RGB16(2, 6, 7),
	RGB16(3, 3, 3), // *medium red			RGB16(6, 2, 2),
	RGB16(5, 5, 5), // *light red			RGB16(6, 3, 3),
	RGB16(3, 3, 3), // *dark yellow			RGB16(5, 5, 2),
	RGB16(6, 6, 6), // *light yellow		RGB16(6, 6, 3),
	RGB16(1, 1, 1), // dark green			RGB16(1, 4, 1),
	RGB16(3, 3, 3), // *magenta				RGB16(5, 2, 5),
	RGB16(4, 4, 4), // gray					RGB16(5, 5, 5),
	RGB16(7, 7, 7)  // white				RGB16(7, 7, 7) 
};

// 
const c8* g_InputSetName[INPUT_SET_MAX] =
{
	"DFG",
	"\\]",
	"[1",
	"[2",
};

// 
const cbInputCheck g_InputCheck[INPUT_SET_MAX] = { CheckKB1, CheckKB2, CheckJoy1, CheckJoy2 };


// GM2 font color (1 color per line)
const u8 g_FontColorNormal[8] =
{
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
};

// GM2 font color (1 color per line)
const u8 g_FontColorSelect[8] =
{
	COLOR_MERGE(COLOR_LIGHT_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_MEDIUM_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_DARK_RED, COLOR_WHITE),
	COLOR_MERGE(COLOR_DARK_RED, COLOR_WHITE),
};

// 
const u8 g_ShadowPattern[] =
{
	0b00111111, 0b11111111,
	0b00000111, 0b11111110,
	0b00000000, 0b11111100,
};

// 
const u8 g_ShadowPatternId[24] =
{ // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
	36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 32, 32, 32, 32, 28, 28, 28, 24, 24, 24, 24
};

// Liste des animations
const u8 *const g_MusicTable[MUSIC_MAX] = { g_MusicEmpty, g_MusicMain, g_Musicvictory };

// Default options
const struct Option g_OptionDefault = { TRUE, TRUE, TRUE, TRUE, FREQ_AUTO, PAL_CUSTOM, { INPUT_SET_KB1, INPUT_SET_KB2, INPUT_SET_KB2 }, AI_MEDIUM, { 11, 1, 3 } };

// AI reception position offset
const i8 g_AIReceptOffset[] = { 8, 12, 16, 16, 16, 16, 16, 16 };

//=============================================================================
// MEMORY DATA
//=============================================================================

u16  g_StateTimer = 0;
u8   g_VersionVDP;
u8   g_FreqCurrent;
u8   g_FreqDetected;

u8   g_SaveData[6];
bool g_Saved = FALSE;

// Characters
struct Character g_Player[2];
struct Character g_Ball;
u8   g_CollisionMap[32*24];
bool g_BallHit;
u8   g_BallGroundX;

i16  g_CloudX[numberof(g_Cloud)];

// AI
bool g_AIGame;
u8   g_AIWait;

// Rules
u8   g_Field = 0;
u8   g_BounceNum = 0;
u8   g_PassNum = 0;
u8   g_ChangeNum = 0;
u8   g_LastTouch = 0;
u8   g_Victorious = 0;
cbInputCheck g_InputCheck1;
cbInputCheck g_InputCheck2;
cbInputCheck g_AIPredict;

// Audio
u8   g_CurrentMusic = 0xFF;
u8   g_NextMusic = 0xFF;
u8   g_NextSFX[3] = { 0xFF, 0xFF, 0xFF };
u8   g_NextSFXVol[3];

// Options parameters
struct Option g_Option;

//.............................................................................
// Menu

const MenuItemMinMax g_MenuPointsMinMax = { 1, 60, 1 };
const MenuItemMinMax g_MenuBouncesMinMax =  { 0, 10, 1 };

// Entries description for the Main menu
const MenuItem g_MenuMain[] =
{
	{ "PLY VS PLY",          MENU_ITEM_GOTO, NULL, MENU_VERSUS },	// Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ "PLY VS CPU",          MENU_ITEM_GOTO, NULL, MENU_SOLO },		// Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ "OPTIONS",             MENU_ITEM_GOTO, NULL, MENU_OPTION },	// Entry to go to Option menu page
	{ "CREDITS",             MENU_ITEM_GOTO, NULL, MENU_CREDITS1 },	// Entry to go to Option menu page
	#if ((TARGET == TARGET_BIN_DISK) || (TARGET == TARGET_BIN_TAPE) || (TARGET == TARGET_DOS1) || (TARGET == TARGET_DOS2))
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },			// Blank entry to create a gap
	{ "EXIT",                MENU_ITEM_ACTION, MenuAction_Start, 0 },			// Entry to exit the game (will trigger MenuAction_Start with value equal to '0')
	#endif
};

// Entries description for the Start menu
const MenuItem g_MenuVersus[] =
{
	{ "START>",              MENU_ITEM_ACTION, MenuAction_Start, 1 }, // Entry to change the screen mode (will trigger MenuAction_Screen)
	{ "PLAYER 1",            MENU_ITEM_ACTION, MenuAction_Input, 0 }, // Entry display a text aligned to left
	{ "PLAYER 2",            MENU_ITEM_ACTION, MenuAction_Input, 1 }, // Entry display a text aligned to center
	{ "RULES>",              MENU_ITEM_GOTO, NULL, MENU_RULES },	// Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_GOTO, NULL, MENU_MAIN },        // Entry to go back to the main menu
};

// Entries description for the Start menu
const MenuItem g_MenuSolo[] =
{
	{ "START>",              MENU_ITEM_ACTION, MenuAction_Start, 2 }, // Entry to change the screen mode (will trigger MenuAction_Screen)
	{ "CONTROL",             MENU_ITEM_ACTION, MenuAction_Input, 2 }, // Entry display a text aligned to center
	{ "CPU",                 MENU_ITEM_ACTION, MenuAction_AI,    0 }, // Entry display a text aligned to left
	{ "RULES>",              MENU_ITEM_GOTO, NULL, MENU_RULES },	// Entry to start a game (will trigger MenuAction_Start with value equal to '1')
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_GOTO, NULL, MENU_MAIN },        // Entry to go back to the main menu
};

// Entries description for the Start menu
const MenuItem g_MenuRules[] =
{
	{ "POINTS",              MENU_ITEM_INT, &g_Option.Rule.GamePoints, (i16)&g_MenuPointsMinMax },
	{ "BOUNCES",             MENU_ITEM_INT, &g_Option.Rule.MaxBounce, (i16)&g_MenuBouncesMinMax },
	{ "PASSES",              MENU_ITEM_INT, &g_Option.Rule.MaxPass,   (i16)&g_MenuBouncesMinMax },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },               // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_BACK, NULL, 0 },        // Entry to go back to the main menu
};

// Entries description for the Option menu
/*const*/ MenuItem g_MenuOptions[] =
{
	{ "AUDIO>",              MENU_ITEM_GOTO, NULL, MENU_AUDIO },
	{ "VIDEO>",              MENU_ITEM_GOTO, NULL, MENU_GRAPH },
	{ "SAVE",                MENU_ITEM_ACTION, MenuAction_Save, 0 },
	{ "RESET",               MENU_ITEM_ACTION, MenuAction_Reset, 0 },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_GOTO, NULL, MENU_MAIN },                // Entry to go back to the main menu
};

// Entries description for the Audio options menu
const MenuItem g_MenuAudio[] =
{
	{ "MUSIC",               MENU_ITEM_ACTION, MenuAction_Music, 0 },
	{ "SFX",                 MENU_ITEM_BOOL, &g_Option.SFX, NULL },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_GOTO, NULL, MENU_OPTION },                // Entry to go back to the main menu
};

// Entries description for the Graphical options menu
/*const*/ MenuItem g_MenuGraph[] =
{
	{ "CLR MIX",             MENU_ITEM_BOOL, &g_Option.Blend, NULL },
	{ "FEEDBACK",            MENU_ITEM_BOOL, &g_Option.Feedback, NULL },
	{ "FREQ",                MENU_ITEM_ACTION, MenuAction_Freq, 0 },
	{ "PALETTE",             MENU_ITEM_ACTION, MenuAction_Palette, 0 },
	{ NULL,                  MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "BACK",                MENU_ITEM_GOTO, NULL, MENU_OPTION },                // Entry to go back to the main menu
};

// Entries description for the Credits menu
const MenuItem g_MenuCredits1[] =
{
	{ "CODE        AOINEKO",     MENU_ITEM_TEXT, NULL, -3 }, // Entry display a text aligned to left
	{ "SPRITES     GRAFXKID",    MENU_ITEM_TEXT, NULL, -3 }, // Entry display a text aligned to center
	{ "GRAPHICS    YAZ",         MENU_ITEM_TEXT, NULL, -3 }, // Entry display a text aligned to center
	{ "FONT        LUDO 'GFX'",  MENU_ITEM_TEXT, NULL, -3 }, // Entry display a text aligned to center
	{ "MUSIC       MAKOTO",      MENU_ITEM_TEXT, NULL, -3 }, // Entry display a text aligned to center
	{ NULL,                      MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "NEXT>",                   MENU_ITEM_GOTO, NULL, MENU_CREDITS2 },	// Entry to go to Option menu page
};

// Entries description for the Credits menu
const MenuItem g_MenuCredits2[] =
{
	{ "VERSION 1.0 (FD50)",      MENU_ITEM_TEXT, NULL, -2 }, // Entry display a text aligned to center
	{ "MADE WITH MSXGL",         MENU_ITEM_TEXT, NULL, -2 }, // Entry display a text aligned to center
	{ "PIXEL PHENIX & 2025",     MENU_ITEM_TEXT, NULL, -2 }, // Entry display a text aligned to center
	{ NULL,                      MENU_ITEM_EMPTY, NULL, 0 },                       // Blank entry to create a gap
	{ "BACK",                    MENU_ITEM_GOTO, NULL, MENU_MAIN },                // Entry to go back to the main menu
};

// List of all menus
const Menu g_Menus[MENU_MAX] =
{
	{ NULL,	g_MenuMain,     numberof(g_MenuMain),     NULL }, // MENU_MAIN
	{ NULL,	g_MenuVersus,   numberof(g_MenuVersus),   NULL }, // MENU_VERSUS
	{ NULL,	g_MenuSolo,     numberof(g_MenuSolo),     NULL }, // MENU_SOLO
	{ NULL,	g_MenuRules,    numberof(g_MenuRules),    NULL }, // MENU_RULES
	{ NULL,	g_MenuOptions,  numberof(g_MenuOptions),  NULL }, // MENU_OPTION
	{ NULL,	g_MenuAudio,    numberof(g_MenuAudio),    NULL }, // MENU_AUDIO
	{ NULL,	g_MenuGraph,    numberof(g_MenuGraph),    NULL }, // MENU_GRAPH
	{ NULL,	g_MenuCredits1, numberof(g_MenuCredits1), NULL }, // MENU_CREDITS1
	{ NULL,	g_MenuCredits2, numberof(g_MenuCredits2), NULL }, // MENU_CREDITS2
};

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// AUDIO
//-----------------------------------------------------------------------------

// Set the new music to be player
inline void PlayMusic(u8 id) { if (g_Option.Music) g_NextMusic = id; }

// Stop music
inline void StopMusic() { g_NextMusic = MUSIC_EMPTY; }

// Set the new music to be player
inline void PlaySFX(u8 id, u8 chan, u8 vol) { g_NextSFX[chan] = id; g_NextSFXVol[chan] = vol; }

//-----------------------------------------------------------------------------
// MENU
//-----------------------------------------------------------------------------

u8 g_PrevInput = INPUT_NONE;

//-----------------------------------------------------------------------------
//
u8 MenuInputHandler()
{
	u8 in = CheckKB2() | CheckJoy1() | CheckJoy2(); // Get input from keyboard and joystick (the game use the same input value than menu)
	u8 ret = in & ~g_PrevInput; // Only return new input
	g_PrevInput = in;
	return ret;
}

//-----------------------------------------------------------------------------
//
void MenuEventHandler(u8 event)
{
	switch (event)
	{
	case MENU_EVENT_UP:
		PlaySFX(SFX_TIC, ARKOS_CHANNEL_C, 0x0F);
		break;

	case MENU_EVENT_DOWN:
		PlaySFX(SFX_TAC, ARKOS_CHANNEL_C, 0x0F);
		break;

	case MENU_EVENT_SET:
	case MENU_EVENT_INC:
	case MENU_EVENT_DEC:
		PlaySFX(SFX_CLICK, ARKOS_CHANNEL_C, 0x0F);
		switch (Menu_GetCurrentItem()->Type)
		{
		case MENU_ITEM_INT:
		case MENU_ITEM_BOOL:
			g_Saved = FALSE;
			break;
		}
		break;

	// case MENU_EVENT_DRAW_TITLE:
	// 	break;

	case MENU_EVENT_DRAW_ENTRY:
		Print_SetPatternOffset(128);
		break;
	case MENU_EVENT_DRAW_SELECTED:
		Print_SetPatternOffset(192);
		break;
	}
}

//-----------------------------------------------------------------------------
// Callback to handle game start
const c8* MenuAction_Start(u8 op, i8 value)
{
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	case MENU_ACTION_DEC:
		if (value == 0)
			Bios_Exit(0);
		else if (value == 1) // Versus
		{
			g_AIGame = FALSE;
			g_InputCheck1 = g_InputCheck[g_Option.InputSet[0]];
			g_InputCheck2 = g_InputCheck[g_Option.InputSet[1]];
			Game_SetState(State_GameInit);
		}
		else if (value == 2) // Solo
		{
			g_AIGame = TRUE;
			g_InputCheck1 = CheckAI;
			g_InputCheck2 = g_InputCheck[g_Option.InputSet[2]];
			switch (g_Option.AILevel)
			{
			case AI_EASY:
				g_AIPredict = GetBallHitX_Easy;
				break;
			case AI_MEDIUM:
				g_AIPredict = GetBallHitX_Medium;
				break;
			case AI_HARD:
				g_AIPredict = GetBallHitX_Hard;
				break;
			}	
			Game_SetState(State_GameInit);
		}
		break;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Callback to handle input
const c8* MenuAction_Input(u8 op, i8 value)
{
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	inc_input_set:
		g_Option.InputSet[value] = (g_Option.InputSet[value] + 1) % INPUT_SET_MAX;
		if ((value < 2) && (g_Option.InputSet[value] == g_Option.InputSet[1 - value]))
			goto inc_input_set;
		g_Saved = FALSE;
		break;
	case MENU_ACTION_DEC:
	dec_input_set:
		g_Option.InputSet[value] = (g_Option.InputSet[value] + (INPUT_SET_MAX - 1)) % INPUT_SET_MAX;
		if ((value < 2) && (g_Option.InputSet[value] == g_Option.InputSet[1 - value]))
			goto dec_input_set;
		g_Saved = FALSE;
		break;
	}

	return g_InputSetName[g_Option.InputSet[value]];
}

//-----------------------------------------------------------------------------
// Callback to handle input
const c8* MenuAction_AI(u8 op, i8 value)
{
	value;
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
		if (g_Option.AILevel < AI_MAX - 1)
			g_Option.AILevel++;
		else
			g_Option.AILevel = 0;
		g_Saved = FALSE;
		break;
	case MENU_ACTION_DEC:
		if (g_Option.AILevel > 0)
			g_Option.AILevel--;
		else
			g_Option.AILevel = AI_MAX - 1;
		g_Saved = FALSE;
		break;
	}

	switch (g_Option.AILevel)
	{
	case AI_EASY:
		return "EASY";
	case AI_MEDIUM:
		return "MEDIUM";
	case AI_HARD:
		return "HARD";
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Callback to handle frequency change
const c8* MenuAction_Freq(u8 op, i8 value)
{
	value;
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
		if (g_Option.Freq < FREQ_MAX - 1)
			g_Option.Freq++;
		else
			g_Option.Freq = 0;
		g_Saved = FALSE;
		break;
	case MENU_ACTION_DEC:
		if (g_Option.Freq > 0)
			g_Option.Freq--;
		else
			g_Option.Freq = FREQ_MAX - 1;
		g_Saved = FALSE;
		break;
	}

	ApplyFreqOption();
	if (g_Option.Freq == FREQ_60HZ) 
		return "60HZ";
	else if (g_Option.Freq == FREQ_50HZ)
		return "50HZ";
	else // if (g_Option.Freq == FREQ_AUTO) 
	{
		if (g_FreqCurrent == FREQ_50HZ)
			return "AUTO:50HZ";
		else
			return "AUTO:60HZ";
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Callback to handle palette change
const c8* MenuAction_Palette(u8 op, i8 value)
{
	value;
	if (g_VersionVDP == VDP_VERSION_TMS9918A)
		return "(FOR MSX2)";

	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
		g_Option.Palette = (g_Option.Palette + 1) % PAL_MAX;
		g_Saved = FALSE;
		break;
	case MENU_ACTION_DEC:
		g_Option.Palette = (g_Option.Palette + (PAL_MAX - 1)) % PAL_MAX;
		g_Saved = FALSE;
		break;
	}

	ApplyPaletteOption();
	switch(g_Option.Palette)
	{
	case PAL_CUSTOM:
		return "CUSTOM";
	case PAL_MSX1: 
		return "MSX1";
	case PAL_MSX2: 
		return "MSX2";
	case PAL_GRAY:
		return "GRAY";
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Callback to handle music enabling/disabling
const c8* MenuAction_Music(u8 op, i8 value)
{
	value;
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	case MENU_ACTION_DEC:
		TOGGLE(g_Option.Music);
		g_Saved = FALSE;
		ApplyMusicOption();
		break;
	}
	return g_Option.Music ? "#" : "$";
}

//-----------------------------------------------------------------------------
// Callback to handle otion save on RTC
const c8* MenuAction_Save(u8 op, i8 value)
{
	value;
	if (g_VersionVDP == VDP_VERSION_TMS9918A)
		return "(FOR MSX2)";

	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	case MENU_ACTION_DEC:
		SaveOptions();
		break;
	}

	RTC_SetMode(RTC_MODE_BLOCK_3);
	bool bValid = ((RTC_Read(0) == RTC_DATA_SIGNSAVE) && (RTC_LoadDataSigned(g_SaveData)));
	return g_Saved ? "SAVED" : (bValid) ? "MODIFIED" : "EMPTY";
}

//-----------------------------------------------------------------------------
// Callback to reset options
const c8* MenuAction_Reset(u8 op, i8 value)
{
	value;
	switch(op)
	{
	case MENU_ACTION_SET:
	case MENU_ACTION_INC:
	case MENU_ACTION_DEC:
		Mem_Copy((u8*)&g_OptionDefault, (u8*)&g_Option, sizeof(struct Option));
		g_Saved = FALSE;
		ApplyFreqOption();
		ApplyPaletteOption();
		ApplyMusicOption();
		Menu_SetDirty();
		break;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// RULES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle change field rule
void Rules_ChangeField(u8 field)
{
	g_Field = field;
	g_BounceNum = 0;
	g_PassNum = 0;
	UpdateBallColor();
}

//-----------------------------------------------------------------------------
// Handle match starting rule
void Rules_Init()
{
	g_Player[0].Score = 0;
	g_Player[1].Score = 0;
	DrawScore();
	Rules_ChangeField(g_AIGame ? 1 : 0);
}	

//-----------------------------------------------------------------------------
// Handle score rule
void Rules_Score(u8 ply, u8 event)
{
	g_Victorious = ply;
	g_Player[ply].Score++;
	if ((g_Player[ply].Score >= g_Option.Rule.GamePoints) && (g_Player[ply].Score > g_Player[1 - ply].Score + 1))
	{
		Game_SetState(State_VictoryInit);
	}
	else
	{
		g_StateTimer = 20;
		VDP_SetColor(COLOR_WHITE);
		Game_SetState(State_Point);
	}
	PlaySFX(SFX_CUICUI, ARKOS_CHANNEL_C, 0x0F);
	DrawScore();
	DrawInfo(event);
}	

//-----------------------------------------------------------------------------
// Handle bounces rule
void Rules_Bounce()
{
	g_BounceNum++;
	if ((g_Option.Rule.MaxBounce != 0xFF) && (g_BounceNum > g_Option.Rule.MaxBounce))
		Rules_Score(1 - g_Field, SCORE_BOUNCE);
	g_LastTouch = g_Field;
	UpdateBallColor();
}

//-----------------------------------------------------------------------------
// Handle pass rule
void Rules_Pass()
{
	g_PassNum++;
	if ((g_Option.Rule.MaxPass != 0) && (g_PassNum > g_Option.Rule.MaxPass))
		Rules_Score(1 - g_Field, SCORE_PASS);
	g_LastTouch = g_Field;
	UpdateBallColor();
}

//-----------------------------------------------------------------------------
// Handle out-of-field rule
void Rules_Out()
{
	Rules_Score(1 - g_LastTouch, SCORE_OUT);
}

//-----------------------------------------------------------------------------
// PHYSICS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Physics hnadle for player
void PlayerPhysics(u8 event, struct Character* ply)
{
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		if (ply->bInAir)
		{
			ply->bInAir = FALSE;
			PlaySFX(SFX_LAND, ply->ID, 0x0C);
		}
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		if (!ply->bInAir)
		{
			ply->bInAir = TRUE;
			ply->VelocityY = 0;
		}
		break;
	};
}

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer1(u8 event, u8 tile)
{
	tile;
	PlayerPhysics(event, &g_Player[0]);
}

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventPlayer2(u8 event, u8 tile)
{
	tile;
	PlayerPhysics(event, &g_Player[1]);
}

//-----------------------------------------------------------------------------
// Physics callback
void PhysicsEventBall(u8 event, u8 tile)
{
	tile;
	switch(event)
	{
	case PAWN_PHYSICS_BORDER_DOWN: // Handle downward collisions 
	case PAWN_PHYSICS_COL_DOWN:
		PlaySFX(SFX_BUMP2, ARKOS_CHANNEL_C, ((g_Ball.VelocityY >> 3) & 0x03) + 11); // Make bounce volume depend on vertical velocity
		g_Ball.VelocityY *= -1; // Reverse
		Pawn_SetAction(&g_Ball.Pawn, ACTION_BALL_BUMP);
		if (g_Ball.Pawn.PositionY > 140)
			Rules_Bounce();
		break;

	case PAWN_PHYSICS_COL_RIGHT: // Handle net
	case PAWN_PHYSICS_COL_LEFT:
		g_Ball.VelocityX *= -1;
		break;
	
	case PAWN_PHYSICS_BORDER_RIGHT: // Handle border
	case PAWN_PHYSICS_BORDER_LEFT:
		Rules_Out();
		break;

	case PAWN_PHYSICS_FALL: // Handle falling
		// if (!g_Ball.bInAir)
		// {
		// 	g_Ball.bInAir = TRUE;
		// 	g_Ball.VelocityY = 0;
		// }
		break;
	};
}

//-----------------------------------------------------------------------------
// Collision callback
//
// Parameters:
//   tile - Number o the tile to check
//
// Return:
//   TRUE if the tile is a blocker, FALSE otherwise
bool PhysicsCollision(u8 tile)
{
	return tile;
}

//-----------------------------------------------------------------------------
// INPUT
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Check key press
bool PressKey()
{
	if (Keyboard_IsKeyPressed(KEY_SPACE))
		return TRUE;

	if ((Joystick_Read(JOY_PORT_1) & JOY_INPUT_TRIGGER_A) == 0)
		return TRUE;
	
	if ((Joystick_Read(JOY_PORT_2) & JOY_INPUT_TRIGGER_A) == 0)
		return TRUE;
		
	return FALSE;
}


//-----------------------------------------------------------------------------
// Check keyboard configuration 1
u8 CheckKB1()
{
	u8 row3 = Keyboard_Read(3);
	u8 ret = INPUT_NONE;
	if (IS_KEY_PRESSED(row3, KEY_D))
		ret |= INPUT_LEFT;
	else if (IS_KEY_PRESSED(row3, KEY_G))
		ret |= INPUT_RIGHT;
	if (IS_KEY_PRESSED(row3, KEY_F))
		ret |= INPUT_ACTION;
	return ret;
}

//-----------------------------------------------------------------------------
// Check keyboard configuration 2
u8 CheckKB2()
{
	u8 row8 = Keyboard_Read(8);
	u8 ret = INPUT_NONE;
	if (IS_KEY_PRESSED(row8, KEY_RIGHT))
		ret |= INPUT_RIGHT;
	else if (IS_KEY_PRESSED(row8, KEY_LEFT))
		ret |= INPUT_LEFT;
	if (IS_KEY_PRESSED(row8, KEY_UP))
		ret |= INPUT_UP | INPUT_ACTION;
	else if (IS_KEY_PRESSED(row8, KEY_DOWN))
		ret |= INPUT_DOWN;
	if (IS_KEY_PRESSED(row8, KEY_SPACE))
		ret |= INPUT_BUTTON1 | INPUT_ACTION;
	if (IS_KEY_PRESSED(row8, KEY_ESC))
		ret |= INPUT_BUTTON2;
	return ret;
}

//-----------------------------------------------------------------------------
// Check joystick port
u8 CheckJoystick(u8 port)
{
	u8 joy = Joystick_Read(port);
	u8 ret = INPUT_NONE;
	if ((joy & JOY_INPUT_DIR_LEFT) == 0)
		ret |= INPUT_LEFT;
	else if ((joy & JOY_INPUT_DIR_RIGHT) == 0)
		ret |= INPUT_RIGHT;
	if ((joy & JOY_INPUT_DIR_UP) == 0)
		ret |= INPUT_UP;
	else if ((joy & JOY_INPUT_DIR_DOWN) == 0)
		ret |= INPUT_DOWN;
	if ((joy & JOY_INPUT_TRIGGER_A) == 0)
		ret |= INPUT_BUTTON1 | INPUT_ACTION;
	if ((joy & JOY_INPUT_TRIGGER_B) == 0)
		ret |= INPUT_BUTTON2;
	return ret;
}

//-----------------------------------------------------------------------------
// Check joystick port 1
u8 CheckJoy1()
{
	return CheckJoystick(JOY_PORT_1);
}

//-----------------------------------------------------------------------------
// Check joystick port 2
u8 CheckJoy2()
{
	return CheckJoystick(JOY_PORT_2);
}

//-----------------------------------------------------------------------------
//
u8 GetBallHitX_Easy()
{
	const Pawn* ballPawn = &g_Ball.Pawn;

	i16 hitX = ballPawn->PositionX + Q4_4_GET(g_Ball.VelocityX * ((168 - ballPawn->PositionY) / 4));
	if (hitX < 0)
		hitX = 0;
	else if (hitX > 255)
		hitX = 255;
	return (u8)hitX;
}

//-----------------------------------------------------------------------------
//
u8 GetBallHitX_Medium()
{
	const Pawn* ballPawn = &g_Ball.Pawn;

	i16 hitX = ballPawn->PositionX + Q4_4_GET(g_Ball.VelocityX * ((168 - ballPawn->PositionY) / 3));
	if (hitX < 0)
		hitX = 0;
	else if (hitX > 255)
		hitX = 255;
	return (u8)hitX;
}

//-----------------------------------------------------------------------------
//
u8 GetBallHitX_Hard()
{
	const Pawn* ballPawn = &g_Ball.Pawn;

	u8 hitT = 0;
	i8 hitV = g_Ball.VelocityY;
	i16 hitH = Q12_4_SET(ballPawn->PositionY);
	while (hitH < Q12_4_SET(168))
	{
		hitV += BALL_GRAVITY; // Apply gravity
		if (hitV > FALL_MAX_SPEED) // Clamp fall speed
			hitV = FALL_MAX_SPEED;
		hitH += hitV;
		hitT++;
	}
	i16 hitX = ballPawn->PositionX + Q4_4_GET(g_Ball.VelocityX * hitT);
	if (hitX < 0)
		hitX = 0;
	else if (hitX > 255)
		hitX = 255;
	return (u8)hitX;
}

//-----------------------------------------------------------------------------
//
u8 CheckAI()
{
	const Pawn* ballPawn = &g_Ball.Pawn;
	const Pawn* plyPawn = &g_Player[0].Pawn;

// VDP_SetColor(COLOR_BLACK);

	// Serve
	if ((ballPawn->PositionX < 128) && (g_Ball.bFreeze == TRUE))
	{
		if (g_AIWait > 0) // Wait a bit before serving
		{
			g_AIWait--;
			return INPUT_NONE;
		}

		if (plyPawn->PositionX < 40)
			return INPUT_ACTION | INPUT_RIGHT;
		else
			return INPUT_LEFT;
	}

// VDP_SetColor(COLOR_DARK_GREEN);

	g_BallGroundX = g_AIPredict();
	// SetSprite(SPRITE_HIT_MARKER, g_BallGroundX, 176, 124, COLOR_MEDIUM_RED);

	// Repositionning
	if ((ballPawn->PositionX > 128) && (g_BallGroundX > 128))
	{
		if (plyPawn->PositionX < 32)
				return INPUT_RIGHT;
		else if (plyPawn->PositionX > 64)
			return INPUT_LEFT;
		else
			return INPUT_NONE;
	}	

// VDP_SetColor(COLOR_DARK_RED);

	// Shoot
#if (1)
	i8 dx = (g_BallGroundX - plyPawn->PositionX);
	i8 sx = (ballPawn->PositionX - plyPawn->PositionX);
	if ((dx > 0) && (sx < 24) && (ballPawn->PositionX < 128) && (ballPawn->PositionY > 128))
	{
		if (sx > g_AIReceptOffset[plyPawn->PositionX / 16])
			return INPUT_ACTION | INPUT_RIGHT;
		else
			return INPUT_ACTION;
	}
#else
	i8 dx = ballPawn->PositionX - plyPawn->PositionX;
	i8 dy = plyPawn->PositionY - ballPawn->PositionY;
	if ((dx + dy < 40) && (ballPawn->PositionX < 128))
	{
		if (dx > g_AIReceptOffset[plyPawn->PositionX / 16])
			return INPUT_ACTION | INPUT_RIGHT;
		else
			return INPUT_ACTION;
	}
#endif

// VDP_SetColor(COLOR_DARK_BLUE);

	// Reception
	if (plyPawn->PositionX < g_BallGroundX)
		return INPUT_RIGHT;
	else
		return INPUT_LEFT;
}

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Apply palette option (for MSX2)
void ApplyPaletteOption()
{
	if (g_VersionVDP == VDP_VERSION_TMS9918A)
		return;

	switch(g_Option.Palette)
	{
	case PAL_CUSTOM:
		VDP_SetPalette((u8*)g_CustomPalette);
		return;
	case PAL_MSX1: 
		VDP_SetMSX1Palette();
		return;
	case PAL_MSX2: 
		VDP_SetDefaultPalette();
		return;
	case PAL_GRAY:
		VDP_SetPalette((u8*)g_GrayPalette);
		return;
	}
}

//-----------------------------------------------------------------------------
// Apply frequency option
void ApplyFreqOption()
{
	// Set gameplay frequency according to option
	if (g_Option.Freq == FREQ_60HZ) 
		g_FreqCurrent = FREQ_60HZ;
	else if (g_Option.Freq == FREQ_50HZ)
		g_FreqCurrent = FREQ_50HZ;
	else // if (g_Option.Freq == FREQ_AUTO)
		g_FreqCurrent = g_FreqDetected;

	// Change VDP frequency
	if (g_VersionVDP > VDP_VERSION_TMS9918A)
	{
		VDP_SetFrequency((g_FreqCurrent == FREQ_50HZ) ? VDP_FREQ_50HZ : VDP_FREQ_60HZ);
	}
	Game_SetIs60Hz(g_FreqCurrent == FREQ_60HZ);
}

//-----------------------------------------------------------------------------
// Apply music option
void ApplyMusicOption()
{
	if (g_Option.Music)
		PlayMusic(MUSIC_MAIN);
	else
		StopMusic();
}	

//-----------------------------------------------------------------------------
// Initialize sprite according to sprite model
void SetSprite(u8 idx, u8 x, u8 y, u8 shape, u8 color)
{
	if (g_VersionVDP == VDP_VERSION_TMS9918A)
		VDP_SetSpriteSM1(idx, x, y, shape, color);
	else
		VDP_SetSpriteExUniColor(idx, x, y, shape, color);
}

//-----------------------------------------------------------------------------
// Set sprite color
void SetSpriteColor(u8 idx, u8 color)
{
	if (g_VersionVDP == VDP_VERSION_TMS9918A)
		VDP_SetSpriteColorSM1(idx, color);
	else
		VDP_SetSpriteUniColor(idx, color);
}

//-----------------------------------------------------------------------------
// Draw the updated score
void DrawScore()
{
	Print_SetPatternOffset(192);
	Print_SetPosition(13, 0);
	if (g_Player[0].Score < 10)
		Print_DrawChar('0');
	Print_DrawInt(g_Player[0].Score);

	Print_SetPosition(17, 0);
	if (g_Player[1].Score < 10)
		Print_DrawChar('0');
	Print_DrawInt(g_Player[1].Score);
}

//-----------------------------------------------------------------------------
// Draw point info
void DrawInfo(u8 event)
{
	Print_SetPatternOffset(128);
	switch (event)
	{
	case SCORE_OUT:
		if (g_PassNum == 0)
		{
			if (g_BounceNum == 0)
				Print_DrawTextAt(14, 15, "OUT!");
			else if (g_ChangeNum == 1)
				Print_DrawTextAt(14, 15, "ACE!");
			else
				Print_DrawTextAt(12, 15, "PASSING!");
		}
		return;
	case SCORE_BOUNCE:
	case SCORE_PASS:
		Print_DrawTextAt(13, 15, "FAULT!");
		return;
	}
}

//-----------------------------------------------------------------------------
// Clear point info
void ClearInfo()
{
	Print_SetPatternOffset(128);
	Print_SetPosition(8, 15);
	Print_DrawCharX(' ', 16); // Clear line
}

//-----------------------------------------------------------------------------
// Load in all 3 screen sections
void Pletter_LoadGM2(const u8* src, u16 dst)
{
	Pletter_UnpackToVRAM(src, dst);
	dst += 0x800;
	Pletter_UnpackToVRAM(src, dst);
	dst += 0x800;
	Pletter_UnpackToVRAM(src, dst);
}

//-----------------------------------------------------------------------------
// Initialize collision map
void CollisionInit()
{
	Mem_Set(0, g_CollisionMap, sizeof(g_CollisionMap));
	
	// Ground
	// Mem_Set(0xFF, g_CollisionMap + 23 * 32, 32);

	// "Net"
	loop(i, NET_H)
	{
		u8* ptr = g_CollisionMap + 15 + (i + (23 - NET_H)) * 32;
		*ptr++ = 0xFF;
		*ptr   = 0xFF;
	}

	Pawn_SetTileMap(g_CollisionMap);
}

//-----------------------------------------------------------------------------
// Draw level background
void DrawLevel(bool bTitle)
{
	// Background
	Pletter_LoadGM2(g_DataBackground_Patterns, VDP_GetPatternTable());
	Pletter_LoadGM2(g_DataBackground_Colors, VDP_GetColorTable());
	Pletter_UnpackToVRAM(bTitle ? g_DataBackgroundL1_Names : g_DataBackgroundL0_Names, VDP_GetLayoutTable());
	// VDP_FillVRAM_16K(0x01, VDP_GetLayoutTable(), 32);
}


//-----------------------------------------------------------------------------
// Save options to RTC
void SaveOptions()
{
	// Byte #0
	g_SaveData[0] = g_Option.Palette & 0x07; // 0b00000111
	if (g_Option.Music)    g_SaveData[0] |= 0b00010000;
	if (g_Option.SFX)      g_SaveData[0] |= 0b00100000;
	if (g_Option.Blend)    g_SaveData[0] |= 0b01000000;
	if (g_Option.Feedback) g_SaveData[0] |= 0b10000000;

	// Byte #1
	g_SaveData[1]  = (g_Option.InputSet[0] & 0x03) << 0; // 0b00000011
	g_SaveData[1] |= (g_Option.InputSet[1] & 0x03) << 2; // 0b00001100
	g_SaveData[1] |= (g_Option.InputSet[2] & 0x03) << 4; // 0b00110000
	g_SaveData[1] |= (g_Option.AILevel     & 0x03) << 6; // 0b11000000

	// Byte #2
	g_SaveData[2] = g_Option.Freq & 0x03;                // 0b00000011

	// Bytes #3-5
	g_SaveData[3] = g_Option.Rule.GamePoints;   // 0b00111111
	g_SaveData[4] = g_Option.Rule.MaxBounce;    // 0b00001111
	g_SaveData[5] = g_Option.Rule.MaxPass;      // 0b00001111

	RTC_SaveDataSigned(g_SaveData);
	g_Saved = TRUE;
}

//-----------------------------------------------------------------------------
// Load options from RTC
void LoadOptions()
{
	RTC_SetMode(RTC_MODE_BLOCK_3);
	if (RTC_Read(0) != RTC_DATA_SIGNSAVE)
		return;

	if (!RTC_LoadDataSigned(g_SaveData))
		return;

	// Byte #0
	g_Option.Palette  = (g_SaveData[0] & 0b00000111);
	g_Option.Music    = (g_SaveData[0] & 0b00010000);
	g_Option.SFX      = (g_SaveData[0] & 0b00100000);
	g_Option.Blend    = (g_SaveData[0] & 0b01000000);
	g_Option.Feedback = (g_SaveData[0] & 0b10000000);

	// Byte #1
	g_Option.InputSet[0] = (g_SaveData[1] & 0b00000011) >> 0;
	g_Option.InputSet[1] = (g_SaveData[1] & 0b00001100) >> 2;
	g_Option.InputSet[2] = (g_SaveData[1] & 0b00110000) >> 4;
	g_Option.AILevel     = (g_SaveData[1] & 0b11000000) >> 6;

	// Byte #2
	g_Option.Freq        = g_SaveData[2] & 0x03;

	// Bytes #3-5
	g_Option.Rule.GamePoints = g_SaveData[3];
	g_Option.Rule.MaxBounce  = g_SaveData[4];
	g_Option.Rule.MaxPass    = g_SaveData[5];

	g_Saved = TRUE;
}

//-----------------------------------------------------------------------------
// PLAYER
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize player
void InitPlayerPosition(u8 id)
{
	struct Character* ply = &g_Player[id];
	Pawn* pawn = &ply->Pawn;
	#if (1)
	if (id == 0)
		Pawn_SetPosition(pawn, 32, 168);
	else
		Pawn_SetPosition(pawn, (u8)(255 - 32 - 16), 168);
	#else
	if (id == 0)
		Pawn_SetPosition(pawn, Q4_4_SET(32), Q4_4_SET(168));
	else
		Pawn_SetPosition(pawn, Q4_4_SET(255 - 32 - 16), Q4_4_SET(168));
	#endif
}

//-----------------------------------------------------------------------------
// Initialize player
void InitPlayer(u8 id)
{
	struct Character* ply = &g_Player[id];
	Mem_Set(0, ply, sizeof(struct Character)); // Set all paramters to 0

	ply->ID = id;
	Pawn* pawn = &ply->Pawn;
	if (id == 0)
	{
		Pawn_Initialize(pawn, g_SpriteLayers, numberof(g_SpriteLayers), 0, g_AnimActions);
		Pawn_InitializePhysics(pawn, PhysicsEventPlayer1, PhysicsCollision, 16, 16);
	}
	else
	{
		Pawn_Initialize(pawn, g_SpriteLayers2, numberof(g_SpriteLayers2), 0, g_AnimActions);
		Pawn_InitializePhysics(pawn, PhysicsEventPlayer2, PhysicsCollision, 16, 16);
		Pawn_SetSpriteFX(pawn, PAWN_SPRITE_FX_FLIP_X);
	}
	Pawn_SetPatternAddress(pawn, g_DataSprtPlayer);
	Pawn_SetColorBlend(pawn, g_Option.Blend);

	InitPlayerPosition(id);

	Pawn_Update(pawn);
	
	// Shadow
	ply->Shadow = (id == 0) ? SPRITE_PLY1_SHADOW : SPRITE_PLY2_SHADOW; 
	SetSprite(ply->Shadow, ply->Pawn.PositionX, 183, 24, SHADOW_COLOR);
}

//-----------------------------------------------------------------------------
// Update character gameplay and display
void UpdateCharacter(struct Character* ply)
{
	Pawn* pawn = &ply->Pawn;

	if (!ply->bFreeze)
	{
#if (1)
		// Compute delta movement and store conversion rest
		i8 dx = Q4_4_GET(ply->VelocityX);
		ply->RestX += ply->VelocityX - dx;
		dx += Q4_4_GET(ply->RestX);
		ply->RestX = Q4_4_FRAC(ply->RestX);

		// Compute delta movement and store conversion rest
		i8 dy = Q4_4_GET(ply->VelocityY);
		ply->RestY += ply->VelocityY - dy;
		dy += Q4_4_GET(ply->RestY);
		ply->RestY = Q4_4_FRAC(ply->RestY);
		
		Pawn_SetMovement(pawn, dx, dy);
#else
		Pawn_SetMovement(pawn, ply->VelocityX, ply->VelocityY);
#endif
	}
	Pawn_Update(pawn);
}

//-----------------------------------------------------------------------------
// Update player gameplay and display
void UpdatePlayer(struct Character* ply)
{
	// HORIZONTAL MOVEMENT

	// Check movement input
	if (ply->Input & INPUT_RIGHT)
	{
		ply->VelocityX += MOVE_ACCEL;
		ply->bMoving = TRUE;
	}
	else if (ply->Input & INPUT_LEFT)
	{
		ply->VelocityX -= MOVE_ACCEL;
		ply->bMoving = TRUE;
	}
	else // Apply friction
	{
		ply->bMoving = FALSE;
		// if(ply->VelocityX > MOVE_FRICTION)
		// 	ply->VelocityX -= MOVE_FRICTION;
		// else if(ply->VelocityX < -MOVE_FRICTION)
		// 	ply->VelocityX += MOVE_FRICTION;
		// else
			ply->VelocityX = 0;
	}
	// Clamp speed
	ply->VelocityX = CLAMP8(ply->VelocityX, -MOVE_MAX_SPEED, MOVE_MAX_SPEED);

	// VERTICAL MOVEMENT
	
	if (ply->bInAir) // Handle in air state (jump or fall)
	{
		ply->VelocityY += GRAVITY; // Apply gravity
		if (ply->VelocityY > FALL_MAX_SPEED) // Clamp fall speed
			ply->VelocityY = FALL_MAX_SPEED;
	}
	else if (ply->Input & INPUT_ACTION) // Initialize jump force
	{
		PlaySFX(SFX_JUMP, ply->ID, 0x0E);
		ply->bInAir = TRUE;
		ply->VelocityY = JUMP_FORCE;
	}
	else
		ply->VelocityY = 0;

	// UPDATE

	// Update player animation & physics
	u8 act = ACTION_PLAYER_IDLE;
	if (ply->bInAir && (ply->VelocityY < 0)) // Up
		act = ACTION_PLAYER_JUMP;
	else if (ply->bInAir) // Down
		act = ACTION_PLAYER_FALL;
	else if (ply->bMoving)
		act = ACTION_PLAYER_MOVE;

	Pawn* pawn = &ply->Pawn;
	Pawn_SetAction(pawn, act);
	UpdateCharacter(ply);

	// Player shadow
	VDP_SetSpritePositionX(ply->Shadow, pawn->PositionX);
	VDP_SetSpritePattern(ply->Shadow, g_ShadowPatternId[pawn->PositionY >> 3]);
}

//-----------------------------------------------------------------------------
// BALL
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize ball position
void InitBallPosition()
{
	Pawn* pawn = &g_Ball.Pawn;
	#if (1)
	Pawn_SetPosition(pawn, (g_Field == 0) ? 56 : 184, 128);
	#else
	Pawn_SetPosition(pawn, (g_Field == 0) ? Q4_4_SET(56) : Q4_4_SET(184), Q4_4_SET(128));
	#endif
	g_Ball.bFreeze = TRUE;
}

//-----------------------------------------------------------------------------
// Initialize ball structure
void InitBall()
{
	Mem_Set(0, &g_Ball, sizeof(struct Character));
	
	Pawn* pawn = &g_Ball.Pawn;
	Pawn_Initialize(pawn, g_BallLayers, numberof(g_BallLayers), 6, g_BallActions);
	Pawn_InitializePhysics(pawn, PhysicsEventBall, PhysicsCollision, 16, 16);
	Pawn_SetAction(pawn, ACTION_BALL_IDLE);
	Pawn_SetPatternAddress(pawn, g_DataSprtBall);
	
	InitBallPosition();
	SetSprite(SPRITE_BALL_SHADOW, pawn->PositionX, 183, 24, SHADOW_COLOR);
	g_BallHit = FALSE;
}

//-----------------------------------------------------------------------------
// Change ball color according to game rules
void UpdateBallColor()
{
	if (!g_Option.Feedback)
		return;

	bool bWarning = (g_BounceNum >= g_Option.Rule.MaxBounce);
	if (g_Option.Rule.MaxPass != 0)
		bWarning |= (g_PassNum >= g_Option.Rule.MaxPass - 1);

	if (bWarning)
	{
		SetSpriteColor(SPRITE_BALL_DARK, COLOR_DARK_RED);
		SetSpriteColor(SPRITE_BALL_LIGHT, COLOR_MEDIUM_RED);
	}
	else
	{
		SetSpriteColor(SPRITE_BALL_DARK, COLOR_MEDIUM_RED);
		SetSpriteColor(SPRITE_BALL_LIGHT, COLOR_LIGHT_RED);
	}
}

//-----------------------------------------------------------------------------
// Update ball gameplay and display
void UpdateBall()
{
	// Apply gravity
	g_Ball.VelocityY += BALL_GRAVITY; // Apply gravity
	if (g_Ball.VelocityY > FALL_MAX_SPEED) // Clamp fall speed
		g_Ball.VelocityY = FALL_MAX_SPEED;

	// Select the player to tests
	Pawn* ballPawn = &g_Ball.Pawn;
	struct Character* ply = (ballPawn->PositionX < 128 - 8) ? &g_Player[0] : &g_Player[1];

	// Compute distance
	Pawn* plyPawn = &ply->Pawn;
	i8 dx = ballPawn->PositionX - plyPawn->PositionX;
	i8 dy = ballPawn->PositionY - plyPawn->PositionY;

	u16 sqrtDist = (dx * dx) + (dy * dy);
	if (sqrtDist < COL_DIST * COL_DIST)
	{
		if (!g_BallHit)
		{
			g_BallHit = TRUE;
			PlaySFX(SFX_BUMP2, ARKOS_CHANNEL_C, 0x0E);
			Rules_Pass();
		}

		g_Ball.VelocityX = dx * 2;
		g_Ball.VelocityX += ply->VelocityX / 2;

		g_Ball.VelocityY = Q4_4_SET(-1.5f);
		if (ply->VelocityY < 0)
			g_Ball.VelocityY += ply->VelocityY / 2;

		g_Ball.bFreeze = FALSE;
		Pawn_SetAction(ballPawn, ACTION_BALL_BUMP);
		Pawn_SetAction(plyPawn, ACTION_PLAYER_HIT);
	}
	else
		g_BallHit = FALSE;

	// Update ball animation & physics
	u8 x = ballPawn->PositionX;
	UpdateCharacter(&g_Ball);

	// Ball shadow
	VDP_SetSpritePositionX(SPRITE_BALL_SHADOW, ballPawn->PositionX);
	VDP_SetSpritePattern(SPRITE_BALL_SHADOW, g_ShadowPatternId[ballPawn->PositionY >> 3]);

	if ((x <= 120) && (ballPawn->PositionX > 120))
	{
		Rules_ChangeField(1);
		g_ChangeNum++;
	}
	else if ((x > 120) && (ballPawn->PositionX <= 120))
	{
		Rules_ChangeField(0);
		g_ChangeNum++;
	}
}

//-----------------------------------------------------------------------------
// SHADOWS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize shadows sprites
void InitShadows()
{
	u16 addr = VDP_GetSpritePatternTable() + 8 * 24;
	loop(i, numberof(g_ShadowPattern))
	{
		VDP_Poke_16K(g_ShadowPattern[i], addr);
		addr += 16;
	}
}

//-----------------------------------------------------------------------------
// CLOUDS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Initialize cloud sprites
void InitClouds()
{
	// Unpack sprite data to VRAM
	Pletter_UnpackToVRAM(g_DataSprtExtra, VDP_GetSpritePatternTable() + 8 * 64);

	// Initialize cloud sprites
	loop(id, numberof(g_Cloud))
	{
		const struct Cloud* cloud = &g_Cloud[id];
	
		u8 sprt = cloud->Sprite;
		u8 pat = cloud->Pattern;
		SetSprite(sprt++, cloud->X, cloud->Y, pat + 0, COLOR_GRAY);
		SetSprite(sprt,   cloud->X, cloud->Y, pat + 4, COLOR_WHITE);
		g_CloudX[id] = cloud->X;
	}
}

//-----------------------------------------------------------------------------
// Update cloud sprites position
void UpdateClouds()
{
	u8 frame = Game_GetFrameCount();
	loop(id, numberof(g_Cloud))
	{
		const struct Cloud* cloud = &g_Cloud[id];

		if ((frame & cloud->Mask) != 0)
			continue;

		g_CloudX[id]--;
		u8 x = g_CloudX[id];
		if (g_CloudX[id] == -16)
		{
			u8 sprt = cloud->Sprite;
			SetSpriteColor(sprt++, COLOR_GRAY);
			SetSpriteColor(sprt,   COLOR_WHITE);
			x = g_CloudX[id] = 255;
		}
		else if (g_CloudX[id] < 0)
		{
			u8 sprt = cloud->Sprite;
			SetSpriteColor(sprt++, VDP_SPRITE_EC | COLOR_GRAY);
			SetSpriteColor(sprt,   VDP_SPRITE_EC | COLOR_WHITE);
			x = g_CloudX[id] + 32;
		}

		u8 sprt = cloud->Sprite;
		VDP_SetSpritePositionX(sprt++, x);
		VDP_SetSpritePositionX(sprt,   x);
	}
}

//-----------------------------------------------------------------------------
// STATES
//-----------------------------------------------------------------------------

// Game initialization state
bool State_AppInit()
{
	Bios_SetKeyClick(FALSE);

	Mem_Copy((u8*)&g_OptionDefault, (u8*)&g_Option, sizeof(struct Option)); // Set default options

	// Get VDP version
	if (Keyboard_IsKeyPressed(KEY_1))
		g_VersionVDP = VDP_VERSION_TMS9918A;
	else if (Keyboard_IsKeyPressed(KEY_2))
		g_VersionVDP = VDP_VERSION_V9938;
	else
		g_VersionVDP = VDP_GetVersion();

	// MSX2 specific settings
	if (g_VersionVDP > VDP_VERSION_TMS9918A)
	{
		// Initialize the RTC module
		RTC_Initialize();
		LoadOptions();
				
		// Initialize palette
		ApplyPaletteOption();

		// Get video frequency from VDP
		g_FreqDetected = (VDP_GetFrequency() == VDP_FREQ_50HZ) ? FREQ_50HZ : FREQ_60HZ;
	}
	else // MSX1 specific settings
	{
		// Invalidate MSX2 only options
		g_MenuOptions[2].Type |= MENU_ITEM_DISABLE; // Save
		g_MenuGraph[3].Type |= MENU_ITEM_DISABLE; // Palette

		// Get video frequency from BIOS
		if (Sys_GetBASICVersion() & 0x80) // 50 Hz flag
			g_FreqDetected = FREQ_50HZ;
		else
			g_FreqDetected = FREQ_60HZ;
	}
	ApplyFreqOption();
	StopMusic();

	Game_SetState(State_LogoInit);
	return FALSE;
}

//-----------------------------------------------------------------------------
// Logo initialization state
bool State_LogoInit()
{
	Logo_Initialize();
	Game_SetState(State_LogoUpdate);
	return FALSE;
}

//-----------------------------------------------------------------------------
// Logo update state
bool State_LogoUpdate()
{
	Halt();

	bool bFinish = Logo_Update();

	if (bFinish || PressKey())
		Game_SetState(State_MenuInit);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Menu initialization state
bool State_MenuInit()
{
	// Initialize display
	VDP_SetMode((g_VersionVDP == VDP_VERSION_TMS9918A) ? VDP_MODE_SCREEN2 : VDP_MODE_SCREEN4);
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();
	VDP_RegWrite(14, 0);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	
	// Setup VRAM tables
	VDP_SetPatternTable(VRAM_PATTERN_TABLE);
	VDP_SetColorTable(VRAM_COLOR_TABLE);
	VDP_SetLayoutTable(VRAM_LAYOUT_TABLE);
	VDP_SetSpritePatternTable(VRAM_SPRITE_PATTERN);
	VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE);
	
	// Draw background
	DrawLevel(TRUE);

	// Initialize clouds
	VDP_HideAllSprites();
	SetSprite(0,   2, 173, 68, COLOR_GRAY); // MSXgl
	SetSprite(1,  18, 173, 76, COLOR_GRAY);
	SetSprite(2, 223, 167, 84, COLOR_GRAY); // FutureDisk
	SetSprite(3, 239, 167, 92, COLOR_GRAY);
	InitClouds();
		
	// Initialize font
	Print_SetTextFont(g_Font, 192);
	for(u16 i = 192; i < 256; i++)
	{
		VDP_WriteVRAM_16K(g_FontColorSelect, VDP_GetColorTable() + 0x0800 + (i * 8), 8);
		VDP_WriteVRAM_16K(g_FontColorSelect, VDP_GetColorTable() + 0x1000 + (i * 8), 8);
	}
	// Duplicate font patterns for shaded characters
	VDP_WriteVRAM_16K(Print_GetFontInfo()->FontPatterns, VDP_GetPatternTable() + 0x0800 + (128 * 8), Print_GetFontInfo()->CharCount * 8);
	VDP_WriteVRAM_16K(Print_GetFontInfo()->FontPatterns, VDP_GetPatternTable() + 0x1000 + (128 * 8), Print_GetFontInfo()->CharCount * 8);
	for(u16 i = 128; i < 192; i++)
	{
		VDP_WriteVRAM_16K(g_FontColorNormal, VDP_GetColorTable() + 0x0800 + (i * 8), 8);
		VDP_WriteVRAM_16K(g_FontColorNormal, VDP_GetColorTable() + 0x1000 + (i * 8), 8);
	}

	// Initialize the menu
	Menu_Initialize(g_Menus);
	Menu_SetEventCallback(MenuEventHandler);
	Menu_SetInputCallback(MenuInputHandler);
	Menu_DrawPage(MENU_MAIN); // Display the first page
	
	// Play main music
	PlayMusic(MUSIC_MAIN);

	Game_SetState(State_MenuUpdate);

	VDP_EnableDisplay(TRUE);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Menu update state
bool State_MenuUpdate()
{
	// Update menu
	Menu_Update();
	UpdateClouds();

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
// Data initialization state
bool State_GameInit()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_BLACK);

	// Play main music
	StopMusic();

	// Initialize background tiles
	DrawLevel(FALSE);
	CollisionInit();

	// Initialize text
	Print_SetTextFont(g_Font, 192);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	// Duplicate font patterns for shaded characters
	VDP_WriteVRAM_16K(Print_GetFontInfo()->FontPatterns, VDP_GetPatternTable() + 0x0800 + (128 * 8), Print_GetFontInfo()->CharCount * 8);
	VDP_WriteVRAM_16K(Print_GetFontInfo()->FontPatterns, VDP_GetPatternTable() + 0x1000 + (128 * 8), Print_GetFontInfo()->CharCount * 8);
	for(u16 i = 128; i < 192; i++)
	{
		VDP_WriteVRAM_16K(g_FontColorNormal, VDP_GetColorTable() + 0x0800 + (i * 8), 8);
		VDP_WriteVRAM_16K(g_FontColorNormal, VDP_GetColorTable() + 0x1000 + (i * 8), 8);
	}

	// Initialize sprite
	VDP_HideAllSprites();
	VDP_DisableSpritesFrom(SPRITE_MAX); // hide

	// Init player 1 pawn (left)
	InitPlayer(0);

	// MSX2 specific settings
	if ((g_AIGame) && (g_VersionVDP > VDP_VERSION_TMS9918A))
	{
		VDP_SetSpriteUniColor(SPRITE_PLY1_BLACK, COLOR_MAGENTA);
		switch (g_Option.AILevel)
		{
		case AI_EASY:
			VDP_SetPaletteEntry(COLOR_MAGENTA, RGB16(0, 0, 1));
			break;
		case AI_MEDIUM:
			VDP_SetPaletteEntry(COLOR_MAGENTA, RGB16(1, 0, 1));
			break;
		case AI_HARD:
			VDP_SetPaletteEntry(COLOR_MAGENTA, RGB16(1, 0, 0));
			break;
		}
	}

	// Init player 2 pawn (right)
	InitPlayer(1);
	
	// Init ball
	InitBall();
	
	// Init misc graphics
	InitClouds();
	InitShadows();
	
	Rules_Init();

	VDP_EnableDisplay(TRUE);
	// PlaySFX(SFX_SEA, ARKOS_CHANNEL_C, 0x08);

	Game_SetState(State_KickOff);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Kick-off state
bool State_KickOff()
{
	// Init player 1 pawn (left)
	InitPlayerPosition(0);
	g_AIWait = 30; // If AI, wait a bit before serving

	// Init player 2 pawn (right)
	InitPlayerPosition(1);

	// Init ball
	InitBallPosition();

	g_ChangeNum = 0;

	Game_SetState(State_Game);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Game update state
bool State_Game()
{
// VDP_SetColor(COLOR_DARK_RED);

	Pawn_Draw(&g_Ball.Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);

// VDP_SetColor(COLOR_DARK_BLUE);

	UpdatePlayer(&g_Player[0]);
	UpdatePlayer(&g_Player[1]);
	UpdateBall();
	UpdateClouds();

// VDP_SetColor(COLOR_DARK_GREEN);

	// Update input
	g_Player[0].Input = g_InputCheck1();
	g_Player[1].Input = g_InputCheck2();
	
	if (Keyboard_IsKeyPressed(KEY_ESC))
		Game_SetState(State_MenuInit);

// VDP_SetColor(COLOR_BLACK);

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
// Pause state
bool State_Pause()
{
	Game_SetState(State_Game);
	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
// Point won state
bool State_Point()
{
	Pawn_SetDirty(&g_Ball.Pawn);
	Pawn_Draw(&g_Ball.Pawn);
	Pawn_SetDirty(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	Pawn_SetDirty(&g_Player[1].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);

	Pawn_Update(&g_Ball.Pawn);
	Pawn_Update(&g_Player[0].Pawn);
	Pawn_Update(&g_Player[1].Pawn);
	UpdateClouds();

	g_StateTimer--;
	if ((g_StateTimer == 0) || PressKey())
	{
		ClearInfo();
		VDP_SetColor(COLOR_BLACK);
		Rules_ChangeField(1 - g_Victorious);
		Game_SetState(State_KickOff);
	}

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
// Victory scene initialization state
bool State_VictoryInit()
{
	g_StateTimer = 500;

	#if (1)
	Pawn_SetPosition(&g_Player[0].Pawn, g_Player[0].Pawn.PositionX, 168);
	Pawn_SetPosition(&g_Player[1].Pawn, g_Player[1].Pawn.PositionX, 168);
	#else
	Pawn_SetPosition(&g_Player[0].Pawn, g_Player[0].Pawn.PositionX, Q4_4_SET(168));
	Pawn_SetPosition(&g_Player[1].Pawn, g_Player[1].Pawn.PositionX, Q4_4_SET(168));
	#endif

	Pawn_SetEnable(&g_Ball.Pawn, FALSE);
	Pawn_ForceSetAction(&g_Player[1 - g_Victorious].Pawn, ACTION_PLAYER_LOOSE);

	Pawn* winner = &g_Player[g_Victorious].Pawn;
	Pawn_ForceSetAction(winner, ACTION_PLAYER_WIN);
	SetSprite(SPRITE_WIN_BACK,  winner->PositionX, winner->PositionY - 24, 112, COLOR_LIGHT_RED);
	SetSprite(SPRITE_WIN_FRONT, winner->PositionX, winner->PositionY - 24, 116, COLOR_DARK_RED);
	
	PlayMusic(MUSIC_VICTORY);
	PlaySFX(SFX_CUICUI, ARKOS_CHANNEL_C, 0x0F);
	ClearInfo();

	Game_SetState(State_VictoryUpdate);
	return FALSE; // Frame finished
}

//-----------------------------------------------------------------------------
// Victory scene update state
bool State_VictoryUpdate()
{
	Pawn_SetDirty(&g_Player[0].Pawn);
	Pawn_Draw(&g_Player[0].Pawn);
	
	Pawn_SetDirty(&g_Player[1].Pawn);
	Pawn_Draw(&g_Player[1].Pawn);

	Pawn_Update(&g_Player[0].Pawn);
	Pawn_Update(&g_Player[1].Pawn);
	UpdateClouds();

	if (Game_GetFrameCount() & 0b00010000)
	{
		VDP_SetSpritePositionY(SPRITE_WIN_BACK, g_Player[g_Victorious].Pawn.PositionY - 24);
		VDP_SetSpritePositionY(SPRITE_WIN_FRONT, g_Player[g_Victorious].Pawn.PositionY - 24);
	}
	else
	{
		VDP_HideSprite(SPRITE_WIN_BACK);
		VDP_HideSprite(SPRITE_WIN_FRONT);
	}

	g_StateTimer--;
	if ((g_StateTimer == 0) || Keyboard_IsKeyPressed(KEY_ESC))
	{
		StopMusic();
		Game_SetState(State_MenuInit);
	}

	return TRUE; // Frame finished
}

//=============================================================================
// INTERRUPTION
//=============================================================================

//-----------------------------------------------------------------------------
// Vertical synchronization callback (always synchronized at 50 Hz)
void VSynch()
{
	if (g_NextMusic != 0xFF)
	{
		if (g_NextMusic != g_CurrentMusic)
		{
			// Load music data
			Pletter_UnpackToRAM((const void *)g_MusicTable[g_NextMusic], (void *)MUSIC_ADDRESS);
			AKM_Init((const void *)MUSIC_ADDRESS, 0);
	
			// Load SFX data
			Pletter_UnpackToRAM((const void *)g_SoundFX, (void *)SFX_ADDRESS);
			AKM_InitSFX((const void *)SFX_ADDRESS);
	
			g_CurrentMusic = g_NextMusic;
		}
		if (g_Option.SFX)
		{
			loop (i, 3)
			{
				if (g_NextSFX[i] != 0xFF)
				{
					AKM_PlaySFX(g_NextSFX[i], i, 0xF - g_NextSFXVol[i]);
					g_NextSFX[i] = 0xFF;
				}
			}
		}
	
		AKM_Decode();
	}
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Programme entry point
void main()
{
	// Start game loop
	Game_SetVSyncCallback(VSynch);
	Game_SetState(State_AppInit);
	Game_Start(VDP_MODE_SCREEN2, g_FreqCurrent == FREQ_60HZ);
}