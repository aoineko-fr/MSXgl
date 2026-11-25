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
#include "system.h"

//=============================================================================
// DEFINES
//=============================================================================

#if ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)

#define LIGHTGUN_COUNT_BLACK1		(0)
#define LIGHTGUN_COUNT_WHITE		(g_LightGun_Duration)
#define LIGHTGUN_COUNT_BLACK2		(LIGHTGUN_COUNT_WHITE + g_LightGun_Duration)
#define LIGHTGUN_COUNT_GAME			(LIGHTGUN_COUNT_BLACK2 + g_LightGun_Duration)

#define LIGHTGUN_CALIB_TIMEOUT		60

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

// Light gun detection frames duration 
u8 g_LightGun_Duration = LIGHTGUN_FRAME_DURATION;

// Light gun detection frames latency
u8 g_LightGun_Latency = LIGHTGUN_FRAME_LATENCY;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update the Phenix light gun.
void LightGun_Phenix_Update()
{
	g_LightGun_Value = LightGun_Read(g_LightGun_Port);
	bool trig = LightGun_Phenix_GetTriggerA();

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
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_WHITE + g_LightGun_Latency)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LIGHTGUN_STATE_BLACK:
			if (LightGun_Phenix_GetLight())
				g_LightGun_State = LIGHTGUN_STATE_WHITE;
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_BLACK2 + g_LightGun_Latency)
				goto LightGun_Phenix_Update_Timeout;
			break;

		case LIGHTGUN_STATE_WHITE:		
			if (!LightGun_Phenix_GetLight())
			{
				g_LightGun_State = LIGHTGUN_STATE_HIT;
				g_LightGun_CB(LIGHTGUN_EVENT_HIT);
			}
			else if (g_LightGun_Counter > LIGHTGUN_COUNT_GAME + g_LightGun_Latency)
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


#if (LIGHTGUN_USE_CALIBRATE)
//-----------------------------------------------------------------------------
// Calibrate the Phenix light gun.
u8 LightGun_Phenix_Calibrate()
{
	// Set black screen
	g_LightGun_CB(LIGHTGUN_EVENT_CALIB_BLACK);

	// Wait for trigger
	bool trig = FALSE;
	while (!trig)
	{
		g_LightGun_Value = LightGun_Read(g_LightGun_Port);
		trig = LightGun_Phenix_GetTriggerA();
	}

	// Set black screen
	g_LightGun_CB(LIGHTGUN_EVENT_CALIB_WHITE);

	// Set for lightgun seeing the white screen
	bool light = FALSE;
	u8 count = 0;
	while (!light)
	{
		Halt();
		g_LightGun_Value = LightGun_Read(g_LightGun_Port);
		light = LightGun_Phenix_GetLight();
		count++;
		if (count > LIGHTGUN_CALIB_TIMEOUT)
		{
			g_LightGun_CB(LIGHTGUN_EVENT_GAME);
			return FALSE;
		}
	}

	// Set new latency value
	LightGun_SetFrameLatency(count);
	g_LightGun_CB(LIGHTGUN_EVENT_GAME);
	return TRUE;
}
#endif // (LIGHTGUN_USE_CALIBRATE)

#endif // ((LIGHTGUN_DRIVER & LIGHTGUN_PHENIX) && LIGHTGUN_USE_UPDATE)
