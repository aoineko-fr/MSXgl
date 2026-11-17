// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄   ▄       ▄▄   ▄▄      ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██   ▄  ▄▀██ ██▄  ██▀    ██   ██ █ ██▀▄
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▄ ██  ▀██ ██ █ ▀█▄    ▀█▄█ ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘           ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Light gun driver module
//─────────────────────────────────────────────────────────────────────────────

#include "core.h"
#include "lightgun.h"

//=============================================================================
// DEFINES
//=============================================================================

#if ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)

// Light gun detection frames duration 
#if (LIGHTGUN_FRAME_DURATION == LIGHTGUN_CUSTOM)
u8 g_LightGun_Duration = 2;
#define LIGHTGUN_DURATION			g_LightGun_Duration
#else
#define LIGHTGUN_DURATION			LIGHTGUN_FRAME_DURATION
#endif

// Light gun detection frames latency
#if (LIGHTGUN_FRAME_LATENCY == LIGHTGUN_CUSTOM)
u8 g_LightGun_Latency = 4;
#define LIGHTGUN_LATENCY			g_LightGun_Latency
#else
#define LIGHTGUN_LATENCY			LIGHTGUN_FRAME_LATENCY
#endif

#define LIGHTGUN_COUNT_BLACK1		(0)
#define LIGHTGUN_COUNT_WHITE		(LIGHTGUN_DURATION)
#define LIGHTGUN_COUNT_BLACK2		(LIGHTGUN_COUNT_WHITE + LIGHTGUN_DURATION)
#define LIGHTGUN_COUNT_GAME			(LIGHTGUN_COUNT_BLACK2 + LIGHTGUN_DURATION)


//=============================================================================
// VARIABLES
//=============================================================================

// Function to be called if hit occurs
LightGun_EventCB g_LightGun_CB = NULL;

// Joystick port to check
u8 g_LightGun_Port;

// Current lightgun raw value
u8 g_LightGun_Value = 0;

// Previous trigger value
bool g_LightGun_TrigPrev = FALSE;

// Light detection counter
u8 g_LightGun_Counter = 0;

// Light detection state
u8 g_LightGun_State = LIGHTGUN_STATE_IDLE;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update the Phenix light gun.
void LightGun_Phenix_Update()
{
	g_LightGun_Value = LightGun_Read(g_LightGun_Port);
	u8 trig = LightGun_Phenix_GetTriggerA();

	// Detect trigger press
	if ((g_LightGun_State == LIGHTGUN_STATE_IDLE) && trig && !g_LightGun_TrigPrev)
	{
		g_LightGun_State = LIGHTGUN_STATE_DETECT;
		g_LightGun_Counter = 0;
	}
	g_LightGun_TrigPrev = trig;

	// Detection sequence
	if (g_LightGun_State >= LIGHTGUN_STATE_DETECT)
	{
		// Notify game to display black/white screen at specific frame
		if (g_LightGun_Counter == LIGHTGUN_COUNT_BLACK1) // Display full black screen
			g_LightGun_CB(LIGHTGUN_EVENT_REF1);
		else if (g_LightGun_Counter == LIGHTGUN_COUNT_WHITE) // Display black screen + white square
			g_LightGun_CB(LIGHTGUN_EVENT_DETECT);
		else if (g_LightGun_Counter == LIGHTGUN_COUNT_BLACK2) // Display full black screen
			g_LightGun_CB(LIGHTGUN_EVENT_REF2);
		else if (g_LightGun_Counter == LIGHTGUN_COUNT_GAME) // Restore game screen
			g_LightGun_CB(LIGHTGUN_EVENT_GAME);

		// Detect the light color changes
		switch (g_LightGun_State)
		{
		case LIGHTGUN_STATE_DETECT: // Wait for black screen
			if (!LightGun_Phenix_GetLight())
				g_LightGun_State = LIGHTGUN_STATE_BLACK;
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_WHITE + LIGHTGUN_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LIGHTGUN_STATE_BLACK:
			if (LightGun_Phenix_GetLight())
				g_LightGun_State = LIGHTGUN_STATE_WHITE;
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_BLACK2 + LIGHTGUN_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LIGHTGUN_STATE_WHITE:		
			if (!LightGun_Phenix_GetLight())
			{
				g_LightGun_State = LIGHTGUN_STATE_HIT;
				g_LightGun_CB(LIGHTGUN_EVENT_HIT);
			}
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_GAME + LIGHTGUN_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LIGHTGUN_STATE_HIT:
			goto LightGun_Phenix_Update_Timeout;
		}

		g_LightGun_Counter++;
	}
	return;

LightGun_Phenix_Update_Timeout:
	g_LightGun_State = LIGHTGUN_STATE_IDLE;
	if (g_LightGun_Counter < LIGHTGUN_COUNT_GAME)
		g_LightGun_CB(LIGHTGUN_EVENT_GAME);
	return;
}

#endif // ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)
