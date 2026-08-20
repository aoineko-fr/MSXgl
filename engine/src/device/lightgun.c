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

#if (LIGHTGUN_USE_PHENIX && LIGHTGUN_USE_UPDATE)

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

// Light black dot detection counter
u8 g_LGun_Black = 0;

// Light detection state
u8 g_LGun_State = LGUN_STATE_DEFAULT;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update the Phenix light gun.
void LightGun_Phenix_Update()
{
	g_LGun_Value = LightGun_Read(g_LGun_Port);
	bool trig = LightGun_Phenix_GetTriggerA();
	u8   light = LightGun_Phenix_GetLight();

	if (trig && !g_LGun_TrigPrev)
	{
		g_LGun_State = LGUN_STATE_DETECT;
		g_LGun_Counter = 0;
		g_LGun_Black = 0;
		g_LGun_CB(LGUN_EVENT_REF1); // Program must display a black screen
	}
	g_LGun_TrigPrev = trig;

	if (g_LGun_State >= LGUN_STATE_DETECT)
	{
		switch (g_LGun_State)
		{
		case LGUN_STATE_DETECT:
			if (light <= LGUN_BLACK_THRESHOLD)
				g_LGun_State = LGUN_STATE_BLACK;
			break;
		case LGUN_STATE_BLACK:
			if (light >= LGUN_WHITE_THRESHOLD)
				g_LGun_State = LGUN_STATE_WHITE;
			else
			{
				g_LGun_Black++;
				if (g_LGun_Black >= 4)
				{
					g_LGun_State = LGUN_STATE_DEFAULT;
					g_LGun_CB(LGUN_EVENT_GAME);
				}
			}
			break;
		case LGUN_STATE_WHITE:		
			if (light < LGUN_WHITE_THRESHOLD)
			{
				g_LGun_State = LGUN_STATE_HIT;
				g_LGun_CB(LGUN_EVENT_HIT);
			}
			break;
		case LGUN_STATE_HIT:		
			g_LGun_State = LGUN_STATE_DEFAULT;
			g_LGun_CB(LGUN_EVENT_GAME);
			break;
		}
		g_LGun_Counter++;
		if (g_LGun_Counter == LGUN_COUNT_WHITE)
			g_LGun_CB(LGUN_EVENT_DETECT);
		else if (g_LGun_Counter == LGUN_COUNT_BLACK2)
			g_LGun_CB(LGUN_EVENT_REF2);
		else if (g_LGun_Counter == LGUN_COUNT_DEFAULT)
			g_LGun_CB(LGUN_EVENT_GAME);
		else if (g_LGun_Counter > LGUN_COUNT_MAX)
			g_LGun_State = LGUN_STATE_DEFAULT;
	}
}

#endif // (LIGHTGUN_USE_PHENIX && LIGHTGUN_USE_UPDATE)
