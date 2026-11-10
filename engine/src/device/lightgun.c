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

#if ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)

// Light gun detection frames duration 
#if (LIGHTGUN_FRAME_DURATION == LIGHTGUN_CUSTOM)
u8 g_LightGun_Duration = 2;
#define LGUN_FRAME_DURATION			g_LightGun_Duration
#else
#define LGUN_FRAME_DURATION			LIGHTGUN_FRAME_DURATION
#endif

// Light gun detection frames latency
#if (LIGHTGUN_FRAME_LATENCY == LIGHTGUN_CUSTOM)
u8 g_LightGun_Latency = 4;
#define LGUN_FRAME_LATENCY			g_LightGun_Latency
#else
#define LGUN_FRAME_LATENCY			LIGHTGUN_FRAME_LATENCY
#endif

#define LGUN_COUNT_BLACK1			(0)
#define LGUN_COUNT_WHITE			(LGUN_FRAME_DURATION)
#define LGUN_COUNT_BLACK2			(LGUN_COUNT_WHITE + LGUN_FRAME_DURATION)
#define LGUN_COUNT_GAME				(LGUN_COUNT_BLACK2 + LGUN_FRAME_DURATION)

//=============================================================================
// VARIABLES
//=============================================================================

// Function to be called if hit occurs
LGun_EventCB g_LGun_CB = NULL;

// Joystick port to check
u8 g_LGun_Port;

// Current lightgun raw value
u8 g_LGun_Value = 0;

// Previous trigger value
bool g_LGun_TrigPrev = FALSE;

// Light detection counter
u8 g_LGun_Counter = 0;

// Light detection state
u8 g_LGun_State = LGUN_STATE_IDLE;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update the Phenix light gun.
void LightGun_Phenix_Update()
{
	g_LGun_Value = LightGun_Read(g_LGun_Port);
	u8 trig = LightGun_Phenix_GetTriggerA();

	// Detect trigger press
	if ((g_LGun_State == LGUN_STATE_IDLE) && trig && !g_LGun_TrigPrev)
	{
		g_LGun_State = LGUN_STATE_DETECT;
		g_LGun_Counter = 0;
	}
	g_LGun_TrigPrev = trig;

	// Detection sequence
	if (g_LGun_State >= LGUN_STATE_DETECT)
	{
		// Notify game to display black/white screen at specific frame
		if (g_LGun_Counter == LGUN_COUNT_BLACK1) // Display full black screen
			g_LGun_CB(LGUN_EVENT_REF1);
		else if (g_LGun_Counter == LGUN_COUNT_WHITE) // Display black screen + white square
			g_LGun_CB(LGUN_EVENT_DETECT);
		else if (g_LGun_Counter == LGUN_COUNT_BLACK2) // Display full black screen
			g_LGun_CB(LGUN_EVENT_REF2);
		else if (g_LGun_Counter == LGUN_COUNT_GAME) // Restore game screen
			g_LGun_CB(LGUN_EVENT_GAME);

		// Detect the light color changes
		switch (g_LGun_State)
		{
		case LGUN_STATE_DETECT: // Wait for black screen
			if (!LightGun_Phenix_GetLight())
				g_LGun_State = LGUN_STATE_BLACK;
			else if (g_LGun_Counter > LGUN_COUNT_WHITE + LGUN_FRAME_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LGUN_STATE_BLACK:
			if (LightGun_Phenix_GetLight())
				g_LGun_State = LGUN_STATE_WHITE;
			else if (g_LGun_Counter > LGUN_COUNT_BLACK2 + LGUN_FRAME_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LGUN_STATE_WHITE:		
			if (!LightGun_Phenix_GetLight())
			{
				g_LGun_State = LGUN_STATE_HIT;
				g_LGun_CB(LGUN_EVENT_HIT);
			}
			else if (g_LGun_Counter > LGUN_COUNT_GAME + LGUN_FRAME_LATENCY)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LGUN_STATE_HIT:
			goto LightGun_Phenix_Update_Timeout;
		}

		g_LGun_Counter++;
	}
	return;

LightGun_Phenix_Update_Timeout:
	g_LGun_State = LGUN_STATE_IDLE;
	if (g_LGun_Counter < LGUN_COUNT_GAME)
		g_LGun_CB(LGUN_EVENT_GAME);
	return;
}

#endif // ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)
