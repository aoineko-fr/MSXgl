// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - AY-3-8910 Programmable Sound Generator Data Manual
//     http://map.grauw.nl/resources/sound/generalinstrument_ay-3-8910.pdf
//  - YM2149 Programmable Software-Controlled Sound Generator Manual
//     http://map.grauw.nl/resources/sound/yamaha_ym2149.pdf
//  - The Revised MSX Red Book - PSG
//     https://www.angelfire.com/art2/unicorndreams/msx/RR-PSG.html
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "system_port.h"
#include "psg_reg.h"
#include "audio/audio.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// PSG_USE_CHANNEL
#ifndef PSG_USE_CHANNEL
	#warning PSG_USE_CHANNEL is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define PSG_USE_CHANNEL			TRUE
#endif

// PSG_DEVICE
#ifndef PSG_DEVICE
	#warning PSG_DEVICE is not defined in "msxgl_config.h"! Default value will be used: PSG_DEVICE_INTERN
	#define PSG_DEVICE				PSG_DEVICE_INTERN
#endif

#if (PSG_DEVICE & PSG_DEVICE_DARKY)
#include "device/darky.h"
#endif
#if (PSG_DEVICE & PSG_DEVICE_OPNA)
#include "audio/opna.h"
#endif

//=============================================================================
// DEFINES
//=============================================================================

// Devices number and index (in the order: internal, external, Darky x2, OPNA)
#if (PSG_DEVICE & PSG_DEVICE_INTERN)
	#define PSG_INTERN_NUM		1
#else
	#define PSG_INTERN_NUM		0
#endif
#if (PSG_DEVICE & PSG_DEVICE_EXTERN)
	#define PSG_EXTERN_NUM		1
#else
	#define PSG_EXTERN_NUM		0
#endif
#if (PSG_DEVICE & PSG_DEVICE_DARKY)
	#define PSG_DARKY_NUM		2
#else
	#define PSG_DARKY_NUM		0
#endif
#if (PSG_DEVICE & PSG_DEVICE_OPNA)
	#define PSG_OPNA_NUM		1
#else
	#define PSG_OPNA_NUM		0
#endif

#define PSG_INTERN_IDX			0
#define PSG_EXTERN_IDX			(PSG_INTERN_IDX + PSG_INTERN_NUM)
#define PSG_DARKY_IDX			(PSG_EXTERN_IDX + PSG_EXTERN_NUM)
#define PSG_OPNA_IDX			(PSG_DARKY_IDX + PSG_DARKY_NUM)

// PSG device number
#define PSG_DEVICE_NUM			(PSG_INTERN_NUM + PSG_EXTERN_NUM + PSG_DARKY_NUM + PSG_OPNA_NUM)
#if (PSG_DEVICE_NUM == 0)
	#error Error: PSG module need at least 1 device!
#endif

// Register index for a given device (device index in bits 4-7, register in bits 0-3)
#define PSG_DEVICE_REG(dev, reg)	(((dev) << 4) | (reg))

// Ports of the 1st device
#if (PSG_DEVICE & PSG_DEVICE_INTERN)
	#define PSG_PORT_REG		P_PSG_REGS
	#define PSG_PORT_WRITE		P_PSG_DATA
	#define PSG_PORT_READ		P_PSG_STAT
	#define PSG_VAR_REG			io_PSG_Reg
	#define PSG_VAR_WRITE		io_PSG_Data
	#define PSG_VAR_READ		io_PSG_Stat
#elif (PSG_DEVICE & PSG_DEVICE_EXTERN)
	#define PSG_PORT_REG		P_PSG_EXT_REGS
	#define PSG_PORT_WRITE		P_PSG_EXT_DATA
	#define PSG_PORT_READ		P_PSG_EXT_STAT
	#define PSG_VAR_REG			io_PSG_Ext_Reg
	#define PSG_VAR_WRITE		io_PSG_Ext_Data
	#define PSG_VAR_READ		io_PSG_Ext_Stat
#elif (PSG_DEVICE & PSG_DEVICE_DARKY)
	#define PSG_PORT_REG		P_DARKY_PSG1_IDX
	#define PSG_PORT_WRITE		P_DARKY_PSG1_DATA
	#define PSG_PORT_READ		P_DARKY_PSG1_READ
	#define PSG_VAR_REG			io_Darky_PSG1_Reg
	#define PSG_VAR_WRITE		io_Darky_PSG1_Data
	#define PSG_VAR_READ		io_Darky_PSG1_Read
#else // (PSG_DEVICE & PSG_DEVICE_OPNA)
	#define PSG_PORT_REG		P_OPNA_ADDR0
	#define PSG_PORT_WRITE		P_OPNA_DATA0
	#define PSG_PORT_READ		P_OPNA_DATA0
	#define PSG_VAR_REG			io_OPNA_Reg0
	#define PSG_VAR_WRITE		io_OPNA_Data0
	#define PSG_VAR_READ		io_OPNA_Data0
#endif

// Channels
#define PSG_CHANNEL_A			0
#define PSG_CHANNEL_B			1
#define PSG_CHANNEL_C			2

// Mixer
#define PSG_TONE_A_ON			0b00000001
#define PSG_TONE_B_ON			0b00000010
#define PSG_TONE_C_ON			0b00000100
#define PSG_NOISE_A_ON			0b00001000
#define PSG_NOISE_B_ON			0b00010000
#define PSG_NOISE_C_ON			0b00100000
#define PSG_TONE_A_OFF			0
#define PSG_TONE_B_OFF			0
#define PSG_TONE_C_OFF			0
#define PSG_NOISE_A_OFF			0
#define PSG_NOISE_B_OFF			0
#define PSG_NOISE_C_OFF			0

#define PSG_USE_ENVELOPE		PSG_F_ENV

#if (PSG_USE_NOTES)
enum NOTE
{
	// Latin notation
	NOTE_DO = 0, // Do
	NOTE_DOd,    // Do#
	NOTE_RE,     // Re
	NOTE_REd,    // Re#
	NOTE_MI,     // Mi
	NOTE_FA,     // Fa
	NOTE_FAd,    // Fa#
	NOTE_SOL,    // Sol
	NOTE_SOLd,   // Sol#
	NOTE_LA,     // La
	NOTE_LAd,    // La#
	NOTE_SI,     // Si
	// Anglo-Germanic notation
	NOTE_C = 0,  // C 
	NOTE_Cd,     // C#
	NOTE_D,      // D
	NOTE_Dd,     // D#
	NOTE_E,      // E
	NOTE_F,      // F
	NOTE_Fd,     // F#
	NOTE_G,      // G
	NOTE_Gd,     // G#
	NOTE_A,      // A
	NOTE_Ad,     // A#
	NOTE_B       // B
};

extern const u16 g_PSG_Notes[];
#endif // (PSG_USE_NOTES)

//-----------------------------------------------------------------------------
typedef struct PSG_Data
{
	u16		Tone[3];	// 6
	u8		Noise;		// 1
	u8		Mixer;		// 1
	u8		Volume[3];	// 3
	u16		Envelope;	// 2
	u8		Shape;		// 1
	u8		IOPortA;	// 1
	u8		IOPortB;	// 1
} PSG_Data;

#if (PSG_ACCESS == PSG_INDIRECT)
extern u8 g_PSG_Regs[PSG_DEVICE_NUM][16];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

// Group: Core

// Function: PSG_Initialize
// Initialize the PSG
void PSG_Initialize();

// Function: PSG_SetRegister
// Set the value of a given register
//
// Parameters:
//   reg   - Register to set (device index in bits 4-7, register in bits 0-3; see <PSG_DEVICE_REG>)
//   value - Value to set
void PSG_SetRegister(u8 reg, u8 value);

// Function: PSG_GetRegister
// Get the value of a given register
//
// Parameters:
//   reg - Register to get (device index in bits 4-7, register in bits 0-3; see <PSG_DEVICE_REG>)
//
// Return:
//   Value of the register
u8 PSG_GetRegister(u8 reg);

// Function: PSG_GetDeviceNum
// Get number of supported PSG devices
//
// Return:
//   PSG devices number
inline u8 PSG_GetDeviceNum() { return PSG_DEVICE_NUM; }

#if (PSG_DEVICE_NUM > 1)
// Function: PSG_SetDeviceRegister
// Get the value of a given register from the given PSG device
//
// Parameters:
//   dev - PSG device index
//   reg - Register to set
//   val - Value to write
inline void PSG_SetDeviceRegister(u8 dev, u8 reg, u8 val) { PSG_SetRegister(PSG_DEVICE_REG(dev, reg), val); }

// Function: PSG_GetDeviceRegister
// Get the value of a given register from the given PSG device
//
// Parameters:
//   dev - PSG device index
//   reg - Register to get
//
// Return:
//   Value of the register
inline u8 PSG_GetDeviceRegister(u8 dev, u8 reg) { return PSG_GetRegister(PSG_DEVICE_REG(dev, reg)); }
#endif

// Group: Helper
// Note: Helper functions only act on the 1st PSG device
#if (PSG_USE_EXTRA)
// Function: PSG_SetTone
// Set the tone period of a given channel (tone generator control register) (1st PSG device only)
//
// Parameters:
//   chan   - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//   period - Period to set (12-bits value)
void PSG_SetTone(u8 chan, u16 period);

// Function: PSG_SetNoise
// Set the noise period (noise generator control register) (1st PSG device only)
//
// Parameters:
//   period - Period to set (5-bits value)
void PSG_SetNoise(u8 period);

// Function: PSG_SetMixer
// Setup mixer by enabling tune and noise generators for each channel (mixer control enable register) (1st PSG device only)
//
// Parameters:
//   mix - Mixer value to set
void PSG_SetMixer(u8 mix);

// Function: PSG_SetVolume
// Set the volume of a given channel (Amplitude control register) (1st PSG device only)
//
// Parameters:
//   chan - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//   vol  - Volume to set (4-bits value)
void PSG_SetVolume(u8 chan, u8 vol);

// Function: PSG_SetEnvelope
// Set the envelope period (Envelope priod control register) (1st PSG device only)
//
// Parameters:
//   period - Period to set (16-bits value)
void PSG_SetEnvelope(u16 period);

// Function: PSG_SetShape
// Set the envelope shape (Envelope shape control register) (1st PSG device only)
//
// Parameters:
//   shape - Shape to set (4-bits value)
void PSG_SetShape(u8 shape);

// Function: PSG_EnableTone
// Enable/disable tone on the given channel (1st PSG device only)
//
// Parameters:
//   chan - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//   val  - TRUE to enable, FALSE to disable
void PSG_EnableTone(u8 chan, bool val);

// Function: PSG_EnableNoise
// Enable/disable noise on the given channel (1st PSG device only)
//
// Parameters:
//   chan - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//   val  - TRUE to enable, FALSE to disable
void PSG_EnableNoise(u8 chan, bool val);

// Function: PSG_EnableEnvelope
// Enable/disable envelope on the given channel (1st PSG device only)
//
// Parameters:
//   chan - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)

void PSG_EnableEnvelope(u8 chan, bool val);

// Function: PSG_GetTone
// Get the tone period of a given channel (1st PSG device only)
//
// Parameters:
//   chan - Channel to get (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//
// Return:
//   Tone period (12-bits value)
u16 PSG_GetTone(u8 chan);

// Function: PSG_GetNoise
// Get the noise period (1st PSG device only)
//
// Return:
//   Noise period (5-bits value)
inline u8 PSG_GetNoise() { return PSG_GetRegister(PSG_REG_NOISE); }

// Function: PSG_GetMixer
// Get the mixer value (same format than <PSG_SetMixer>: bit set to 1 when tone/noise generator is enabled) (1st PSG device only)
//
// Return:
//   Mixer value
inline u8 PSG_GetMixer() { return ~PSG_GetRegister(PSG_REG_MIXER) & 0b00111111; }

// Function: PSG_GetVolume
// Get the volume of a given channel (including envelope flag <PSG_F_ENV>) (1st PSG device only)
//
// Parameters:
//   chan - Channel to get (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//
// Return:
//   Volume value
inline u8 PSG_GetVolume(u8 chan) { return PSG_GetRegister(PSG_REG_AMP_A + chan); }

// Function: PSG_GetEnvelope
// Get the envelope period (1st PSG device only)
//
// Return:
//   Envelope period (16-bits value)
u16 PSG_GetEnvelope();

// Function: PSG_GetShape
// Get the envelope shape (1st PSG device only)
//
// Return:
//   Envelope shape (4-bits value)
inline u8 PSG_GetShape() { return PSG_GetRegister(PSG_REG_SHAPE) & 0x0F; }

#endif //(PSG_USE_EXTRA)

// Function: PSG_Mute
// Silent all PSG devices (set according registers)
void PSG_Mute();

#if (PSG_USE_PAUSE)
// Function: PSG_Resume
// Resume all PSG devices sound
void PSG_Resume();
#endif

#if (PSG_ACCESS == PSG_INDIRECT)

// Group: Indirect

// Function: PSG_Apply
// Send data to registers #0 to #13 of all PSG devices
void PSG_Apply();

#endif

#if (PSG_USE_NOTES)
// Function: PSG_SetNote
// Set a note on the given channel
// Note: PSG_USE_NOTES need to be set to TRUE to use this function
//
// Parameters:
//   chan   - Channel to set (0 to 2; or PSG_CHANNEL_A, PSG_CHANNEL_B, PSG_CHANNEL_C)
//   note   - Note to set (0 to 11; or NOTE_DO, NOTE_RE, NOTE_MI, NOTE_FA, NOTE_SOL, NOTE_LA, NOTE_SI)
//   octave - Octave to play (0 to 7)
inline void PSG_SetNote(u8 chan, u8 note, u8 octave) { PSG_SetTone(chan, g_PSG_Notes[(octave * 12) + note]); }
#endif

#if (PSG_USE_CHANNEL)
// Group: Channel

// Function: PSG_GetChannelCount
// Get the number of PSG channels (3 per device)
// Note: PSG_USE_CHANNEL need to be set to TRUE to use this function
//
// Return:
//   Number of channels
inline u8 PSG_GetChannelCount() { return PSG_DEVICE_NUM * 3; }

// Function: PSG_GetChannelVolume
// Get the current volume of a given channel
// Note: PSG_USE_CHANNEL need to be set to TRUE to use this function
//
// Parameters:
//   chan - Channel index (0 to 2; 3 to 5 for the secondary PSG, etc.)
//
// Return:
//   Volume from 0 (mute) to 15 (max). 0 if neither tone nor noise is enabled on the channel. 15 if the channel volume is controlled by the envelope
u8 PSG_GetChannelVolume(u8 chan);

// Function: PSG_GetChannelType
// Get the type of a given channel
// Note: PSG_USE_CHANNEL need to be set to TRUE to use this function
//
// Parameters:
//   chan - Channel index (0 to 2; 3 to 5 for the secondary PSG, etc.)
//
// Return:
//   Channel type (always CHANNEL_TYPE_TONE)
inline u8 PSG_GetChannelType(u8 chan) { chan; return CHANNEL_TYPE_TONE; }
#endif