// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCCM Ninja-Tap Driver v2.0 for MSX (Primary & C-lib routine)
// Copyright 1994 1995 Knight's Chamber, all rights reserved.
// 	Written by Firm Knight.
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "bios.h"

//=============================================================================
// DEFINES
//=============================================================================

// ObsoNET BIOS routines
#define ONET_RESET					0x7FCD // Initializes the ObsoNET card
#define ONET_GET_VERS				0x7FCA // Obtains the BIOS version.
#define ONET_GET_HWAD				0x7FC7 // Obtains the ObsoNET physical address.
#define ONET_GET_NETSTAT			0x7FC4 // Obtains the network connection status
#define ONET_ONET_ONOFF				0x7FC1 // Activates or deactivates ObsoNET
#define ONET_CONF_RX				0x7FBE // Configuration of the reception parameters
#define ONET_CONF_MCAST				0x7FBB // Configuration of the multicast mask
#define ONET_GET_INSTAT				0x7FB8 // Obtains information about
#define ONET_GET_PACKET				0x7FB5 // Obtains the oldest captured incoming packet
#define ONET_SEND_PACKET			0x7FB2 // Sends a packet to the network
#define ONET_SEND_STATUS			0x7FAF // Obtains the status of the packet transmission

// Synchronous execution status
#define SYNCH_SUCCESS				0 // Packet successfully sent
#define SYNCH_ERROR_LENGTH			1 // Invalid packet length
#define SYNCH_ERROR_CARRIER			3 // Transmission aborted due to carrier loss
#define SYNCH_ERROR_COLLISION		4 // Transmission aborted due to excessive collisions

// Asynchronous execution status
#define ASYNCH_START				0 // The packet transmission has started
#define ASYNCH_ERROR_LENGTH			1 // Invalid packet length

// Asynch send status
#define SEND_NONE					0 // No packets have been sent since the last execution of RTL_RESET
#define SEND_PROGRESS				1 // Transmission in progress
#define SEND_SUCESSS				2 // The packet has been successfully sent
#define SEND_ERROR_CARRIER			3 // Transmission aborted due to carrier loss
#define SEND_ERROR_COLLISION		4 // Transmission aborted due to excessive collisions 

extern u8 g_ONET_Slot;
extern u8 g_ONET_Bios;
extern u8 g_ONET_Version[3];
extern u8 g_ONET_MAC[6];

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: ONET_Initialize
// ObsoNET searching routine
// /!\ Must not be call from page 1
// /!\ Must not be call before any other function from this module
u8 ONET_Initialize();

// Function: ONET_GetSlot
// Obtains the ObsoNET slot (or 0xFF is not found)
inline u8 ONET_GetSlot() { return g_ONET_Slot; }

// Function: ONET_HasBIOS
// Check if ObsoNET card has a BIOS
inline bool ONET_HasBIOS() { return g_ONET_Bios; }

// Function: ONET_Reset
// Initializes the ObsoNET card
void ONET_Reset();

// Function: ONET_GetVersion
// Obtains the BIOS version
inline const u8* ONET_GetVersion() { return g_ONET_Version; }

// Function: ONET_GetPhysicalAddr
// Obtains the ObsoNET physical address
inline const u8* ONET_GetPhysicalAddr() { return g_ONET_MAC; }

// Function: ONET_GetMACAddress
// Obtains the ObsoNET physical address
// See <ONET_GetPhysicalAddr>
inline const u8* ONET_GetMACAddress() { return ONET_GetPhysicalAddr(); }

// Function: ONET_GetStatus
// Obtains the network connection status
u8 ONET_GetStatus();

// Function: ONET_Activate
// Activates or deactivates ObsoNET
void ONET_Activate(bool activate);

// Function: ONET_IsActive
// Activates or deactivates ObsoNET / Obtain the current state
bool ONET_IsActive();

// Function: ONET_SetReception
// Configuration of the reception parameters
void ONET_SetReception(u8 flag);

// Function: ONET_SetMulticastMask
// Configuration of the multicast mask
void ONET_SetMulticastMask(u16 addr);

// Function: ONET_GetMulticastMask
// Configuration of the multicast mask
u16 ONET_GetMulticastMask();

// Function: ONET_GetPacketInfo
// Obtains information about the oldest received packet
const c8* ONET_GetPacketInfo();

// Function: ONET_GetPacket
// Obtains the oldest captured incoming packet
u8 ONET_GetPacket(u16 addr, u16* size);

// Function: ONET_SendPacketSynch
// Sends a packet to the network (Synchronous execution)
u8 ONET_SendPacketSynch(u16 addr, u16 size);

// Function: ONET_SendPacketAsynch
// Sends a packet to the network (Asynchronous execution)
u8 ONET_SendPacketAsynch(u16 addr, u16 size);

// Function: ONET_GetSendStatus
// Obtains the status of the packet transmission
u8 ONET_GetSendStatus();
