// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Advanced input manager
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "input.h"
#include "input_manager.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================
#if (!INPUT_USE_JOYSTICK || !INPUT_USE_KEYBOARD)
	#error Input manager need both Joystick and Keyboard support! Please include 'input' module in your module list and set INPUT_USE_JOYSTICK and INPUT_USE_KEYBOARD to TRUE.
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Default configuration
const IPM_Config g_DefaultConfig =
{
	// DeviceSupport
	{	TRUE,	TRUE,	TRUE,	TRUE },
	// Timer
	0x10, 0x04,
	// Keyboard set
	{
		{ KEY_UP,	KEY_RIGHT,	KEY_DOWN,	KEY_LEFT,	KEY_SPACE,	KEY_N },
		{ KEY_W,	KEY_D,		KEY_S,		KEY_A,		KEY_CTRL,	KEY_SHIFT },
	}
};

//=============================================================================
// MEMORY DATA
//=============================================================================

IPM_Data g_IPM;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Check if a click event occured
u8 CheckClick(u8 dev, u8 input)
{
	return (g_IPM.Process[dev].State[input] == IPM_STATE_PRESS);
}

//-----------------------------------------------------------------------------
// Check if a release event occured
u8 CheckRelease(u8 dev, u8 input)
{
	return (g_IPM.Process[dev].State[input] == IPM_STATE_RELEASE);
}

//-----------------------------------------------------------------------------
// Check if a hold event occured
u8 CheckHold(u8 dev, u8 input)
{
	return (g_IPM.Process[dev].State[input] == (IPM_STATE_ON + IPM_STATE_HOLD));
}

//-----------------------------------------------------------------------------
// Check if a double-click event occured
u8 CheckDoubleClick(u8 dev, u8 input)
{
	return (g_IPM.Process[dev].State[input] == (IPM_STATE_PRESS + IPM_STATE_DOUBLE));
}

//-----------------------------------------------------------------------------
// Check if a double-click-hold event occured
u8 CheckDoubleClickHold(u8 dev, u8 input)
{
	return (g_IPM.Process[dev].State[input] == (IPM_STATE_ON + IPM_STATE_DOUBLE + IPM_STATE_HOLD));
}

//-----------------------------------------------------------------------------
// Initialize input manager
// Inupts:		config		The manager configuration (NULL: default config will be used)
void IPM_Initialize(IPM_Config* config)
{
	for (u8 dev = 0; dev < IPM_DEVICE_MAX; ++dev)
	{
		IPM_Process* proc = &g_IPM.Process[dev];
		proc->CurrentStatus  = 0xFF;
		proc->PreviousStatus = 0xFF;

		for (u8 in = 0; in < IPM_INPUT_MAX; ++in)
		{
			proc->State[in] = 0;
			proc->Timer[in] = 0xFF;
		}
	}

	g_IPM.EventsNum = 0;
	g_IPM.Checker[IPM_EVENT_CLICK]             = CheckClick;
	g_IPM.Checker[IPM_EVENT_RELEASE]           = CheckRelease;
	g_IPM.Checker[IPM_EVENT_HOLD]              = CheckHold;
	g_IPM.Checker[IPM_EVENT_DOUBLE_CLICK]      = CheckDoubleClick;
	g_IPM.Checker[IPM_EVENT_DOUBLE_CLICK_HOLD] = CheckDoubleClickHold;

	if (config == NULL)
		config = (IPM_Config*)&g_DefaultConfig;

	Mem_Copy(config, &g_IPM.Config, sizeof(g_IPM.Config));
}

//-----------------------------------------------------------------------------
// Update input manager
void IPM_Update()
{
	// Update device status
	for (u8 dev = 0; dev < IPM_DEVICE_MAX; ++dev)
	{
		IPM_Process* proc = &g_IPM.Process[dev];
		proc->PreviousStatus = proc->CurrentStatus;
		
		if (dev <= IPM_DEVICE_JOYSTICK_2) // Get joystick current status
		{
			proc->CurrentStatus = Joystick_Read((dev == IPM_DEVICE_JOYSTICK_1) ? JOY_PORT_1 : JOY_PORT_2);
		}
		else // Get keyboard-set current status
		{
			u8 ks = dev - IPM_DEVICE_KEYBOARD_1;
			IPM_KeySet* keySet = &g_IPM.Config.KeySet[ks];

			u8 flag = 0xFF;
			if (Keyboard_IsKeyPressed(keySet->Up))
				flag &= ~JOY_INPUT_DIR_UP;
			if (Keyboard_IsKeyPressed(keySet->Right))
				flag &= ~JOY_INPUT_DIR_RIGHT;
			if (Keyboard_IsKeyPressed(keySet->Down))
				flag &= ~JOY_INPUT_DIR_DOWN;
			if (Keyboard_IsKeyPressed(keySet->Left))
				flag &= ~JOY_INPUT_DIR_LEFT;
			if (Keyboard_IsKeyPressed(keySet->TriggerA))
				flag &= ~JOY_INPUT_TRIGGER_A;
			if (Keyboard_IsKeyPressed(keySet->TriggerB))
				flag &= ~JOY_INPUT_TRIGGER_B;

			proc->CurrentStatus = flag;
		}

		u8 in;
		for (in = 0; in < IPM_INPUT_MAX; ++in)
		{
			// Increment previous state counter
			if (proc->Timer[in] < 0xFF)
				proc->Timer[in]++;
			
			// Check state tranition
			if ((proc->State[in] & IPM_STATE_PRESSMASK) == IPM_STATE_PRESS) // PRESS >> ON
			{
				proc->State[in] &= ~IPM_STATE_PRESSMASK;
				proc->State[in] |= IPM_STATE_ON;
			}			
			else if ((proc->State[in] & IPM_STATE_PRESSMASK) == IPM_STATE_RELEASE) // RELEASE >> OFF
			{
				proc->State[in] &= ~IPM_STATE_PRESSMASK;
				proc->State[in] &= ~IPM_STATE_HOLDMASK;
				proc->State[in] |= IPM_STATE_OFF;
			}
			else if (((proc->State[in] & IPM_STATE_HOLDMASK) == 0) && ((proc->State[in] & IPM_STATE_PRESSMASK) == IPM_STATE_ON) && (proc->Timer[in] > g_IPM.Config.HoldTimer)) // ON >> ON + HOLD
			{
				proc->State[in] |= IPM_STATE_HOLD;
			}
			else if (((proc->State[in] & IPM_STATE_HOLDMASK) == IPM_STATE_HOLD)) // ON + HOLD >> ON + HOLDING
			{
				proc->State[in] &= ~IPM_STATE_HOLDMASK;
				proc->State[in] |= IPM_STATE_HOLDING;
			}
			else if (((proc->State[in] & IPM_STATE_PRESSMASK) == IPM_STATE_OFF) && (proc->Timer[in] > g_IPM.Config.DoubleClickTimer)) // OFF + DBL >> OFF
			{
				proc->State[in] &= ~IPM_STATE_DOUBLE;
			}

			u8 curOn;
			u8 prevOn;
			if (in == IPM_INPUT_STICK)
			{
				curOn = (proc->CurrentStatus & JOY_INPUT_DIR_MASK) != JOY_INPUT_DIR_MASK;
				prevOn = (proc->PreviousStatus & JOY_INPUT_DIR_MASK) != JOY_INPUT_DIR_MASK;
			}
			else
			{
				u8 mask = (in == IPM_INPUT_BUTTON_A) ? JOY_INPUT_TRIGGER_A : JOY_INPUT_TRIGGER_B;
				curOn = (proc->CurrentStatus & mask) == 0;
				prevOn = (proc->PreviousStatus & mask) == 0;
			}

			if (curOn && !prevOn) // OFF >> PRESS
			{
				proc->State[in] &= ~IPM_STATE_PRESSMASK;
				proc->State[in] |= IPM_STATE_PRESS;
				if (proc->Timer[in] <= g_IPM.Config.DoubleClickTimer)
					proc->State[in] |= IPM_STATE_DOUBLE;
				proc->Timer[in] = 0;
			}
			else if (!curOn && prevOn) // ON >> RELEASE
			{
				proc->State[in] = IPM_STATE_RELEASE;
				proc->Timer[in] = 0;
			}
		}
	}

	// Check registered events
	for (u8 i = 0; i < g_IPM.EventsNum; ++i)
	{
		IPM_Event* entry = &g_IPM.Events[i];
		
		IPM_cb cb = entry->Callback;
		if (cb == NULL)
			continue;

		if (entry->Device < IPM_DEVICE_MAX)
		{	
			u8 dev = entry->Device;
			if (entry->Event < IPM_EVENT_MAX)
			{
				u8 ev = entry->Event;
				if (entry->Input < IPM_INPUT_MAX)
				{
					u8 in = entry->Input;
					if (g_IPM.Checker[ev](dev, in))
						cb(dev, in, ev);
				}
				else // IPM_INPUT_ANY
				{
					for (u8 in = 0; in < IPM_INPUT_MAX; ++in)
						if (g_IPM.Checker[ev](dev, in))
							cb(dev, in, ev);
				}
			}
			else // IPM_EVENT_ANY
			{
				for (u8 ev = 0; ev < IPM_EVENT_MAX; ++ev)
				{
					if (entry->Input < IPM_INPUT_MAX)
					{
						u8 in = entry->Input;
						if (g_IPM.Checker[ev](dev, in))
							cb(dev, in, ev);
					}
					else // IPM_INPUT_ANY
					{
						for (u8 in = 0; in < IPM_INPUT_MAX; ++in)
							if (g_IPM.Checker[ev](dev, in))
								cb(dev, in, ev);
					}
				}
			}
		}	
		else // IPM_DEVICE_ANY
		{	
			for (u8 dev = 0; dev < IPM_DEVICE_MAX; ++dev)
			{
				if (entry->Event < IPM_EVENT_MAX)
				{
					u8 ev = entry->Event;
					if (entry->Input < IPM_INPUT_MAX)
					{
						u8 in = entry->Input;
						if (g_IPM.Checker[ev](dev, in))
							cb(dev, in, ev);
					}
					else // IPM_INPUT_ANY
					{
						for (u8 in = 0; in < IPM_INPUT_MAX; ++in)
							if (g_IPM.Checker[ev](dev, in))
								cb(dev, in, ev);
					}
				}
				else // IPM_EVENT_ANY
				{
					for (u8 ev = 0; ev < IPM_EVENT_MAX; ++ev)
					{
						if (entry->Input < IPM_INPUT_MAX)
						{
							u8 in = entry->Input;
							if (g_IPM.Checker[ev](dev, in))
								cb(dev, in, ev);
						}
						else // IPM_INPUT_ANY
						{
							for (u8 in = 0; in < IPM_INPUT_MAX; ++in)
								if (g_IPM.Checker[ev](dev, in))
									cb(dev, in, ev);
						}
					}
				}
			}
		}	
	}
}

//-----------------------------------------------------------------------------
// Register a callback to a given device manager's event
bool IPM_RegisterEvent(u8 dev, u8 input, u8 event, IPM_cb cb)
{
	if (g_IPM.EventsNum < IPM_EVENT_TAB_SIZE - 1)
	{
		u8 i = g_IPM.EventsNum++;
		IPM_Event* ev = &g_IPM.Events[i];
		ev->Device = dev;
		ev->Input = input;
		ev->Event = event;
		ev->Callback = cb;
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Get current direction of the given device
u8 IPM_GetStickDirection(u8 dev)
{
	u8 in = g_IPM.Process[dev].CurrentStatus;
	in = ~in;
	in &= JOY_INPUT_DIR_MASK;
	switch (in)
	{
	case JOY_INPUT_DIR_UP:			return 0;
	case JOY_INPUT_DIR_UP_RIGHT:	return 1;
	case JOY_INPUT_DIR_RIGHT:		return 2;
	case JOY_INPUT_DIR_DOWN_RIGHT:	return 3;
	case JOY_INPUT_DIR_DOWN:		return 4;
	case JOY_INPUT_DIR_DOWN_LEFT:	return 5;
	case JOY_INPUT_DIR_LEFT:		return 6;
	case JOY_INPUT_DIR_UP_LEFT:		return 7;
	default:						break;
	}

	return 0xFF;
}

//-----------------------------------------------------------------------------
// Get event name
const c8* IPM_GetEventName(u8 ev)
{
	switch (ev)
	{
	case IPM_EVENT_CLICK:
		return "Clck";
	case IPM_EVENT_RELEASE:
		return "Rel.";
	case IPM_EVENT_HOLD:
		return "Hold";
	case IPM_EVENT_DOUBLE_CLICK:
		return "DClk";
	case IPM_EVENT_DOUBLE_CLICK_HOLD:
		return "DCHo";
	default:
		break;
	}
	return "???";
}
