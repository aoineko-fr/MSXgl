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
#pragma once

#include "core.h"
#include "bios.h"
#include "ethernet.h"

//=============================================================================
// DEFINES
//=============================================================================

// ObsoNET BIOS routines
#define ONET_FUNC_RESET				0x7FCD // Initializes the ObsoNET card
#define ONET_FUNC_GET_VERS			0x7FCA // Obtains the BIOS version.
#define ONET_FUNC_GET_HWAD			0x7FC7 // Obtains the ObsoNET physical address.
#define ONET_FUNC_GET_NETSTAT		0x7FC4 // Obtains the network connection status
#define ONET_FUNC_ONOFF				0x7FC1 // Activates or deactivates ObsoNET
#define ONET_FUNC_CONF_RX			0x7FBE // Configuration of the reception parameters
#define ONET_FUNC_CONF_MCAST		0x7FBB // Configuration of the multicast mask
#define ONET_FUNC_GET_INSTAT		0x7FB8 // Obtains information about the oldest received packet
#define ONET_FUNC_GET_PACKET		0x7FB5 // Obtains the oldest captured incoming packet
#define ONET_FUNC_SEND_PACKET		0x7FB2 // Sends a packet to the network
#define ONET_FUNC_SEND_STATUS		0x7FAF // Obtains the status of the packet transmission

// Synchronous execution status
#define ONET_SYNC_SUCCESS			0 // Packet successfully sent
#define ONET_SYNC_ERROR_LENGTH		1 // Invalid packet length
#define ONET_SYNC_ERROR_CARRIER		3 // Transmission aborted due to carrier loss
#define ONET_SYNC_ERROR_COLLISION	4 // Transmission aborted due to excessive collisions

// Asynchronous execution status
#define ONET_ASYNC_START			0 // The packet transmission has started
#define ONET_ASYNC_ERROR_LENGTH		1 // Invalid packet length

// Asynch send status
#define ONET_SEND_NONE				0 // No packets have been sent since the last execution of RTL_RESET
#define ONET_SEND_PROGRESS			1 // Transmission in progress
#define ONET_SEND_SUCESSS			2 // The packet has been successfully sent
#define ONET_SEND_ERROR_CARRIER		3 // Transmission aborted due to carrier loss
#define ONET_SEND_ERROR_COLLISION	4 // Transmission aborted due to excessive collisions 

// Activation flag
#define ONET_ACTIVATE_GET			0 // Obtain the current state
#define ONET_ACTIVATE_ON			1 // Activate the card
#define ONET_ACTIVATE_OFF			2 // Deactivate the card

// Reception flag
#define ONET_RECEPT_SMALL			0b00000001 // Accept the incoming packets whose length is smaller than 64 bytes
#define ONET_RECEPT_BROADCAST		0b00000010 // Accept the incoming packets whose destination address is the broadcast address (FF-FF-FF-FF-FF-FF)
#define ONET_RECEPT_MULTICAST		0b00000100 // Accept the incoming packets whose destination address is a multicast address.
#define ONET_RECEPT_ALL				0b00001000 // Accept all the incoming packets, regardless of their destination address ("promiscuous mode").
#define ONET_RECEPT_GET				0b10000000 // Only return the current configuration (the other flags are ignored)

// External data
extern u8 g_ONET_Slot;
extern u8 g_ONET_Bios;
extern u8 g_ONET_Version[3];
extern u8 g_ONET_MAC[6];

struct ONET_PacketInfo
{
	u16 Size;
	u16 Head;
};

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
bool ONET_GetStatus();

// Function: ONET_Activation
// Activates or deactivates ObsoNET
u8 ONET_Activation(u8 flag);

// Function: ONET_Activate
// Activates ObsoNET
inline void ONET_Activate() { ONET_Activation(ONET_ACTIVATE_ON); }

// Function: ONET_Disactivate
// Deactivates ObsoNET
inline void ONET_Disactivate() { ONET_Activation(ONET_ACTIVATE_OFF); }

// Function: ONET_IsActivate
// Check Activates or deactivates ObsoNET
inline bool ONET_IsActivate() { return ONET_Activation(ONET_ACTIVATE_GET) == ONET_ACTIVATE_ON; }

// Function: ONET_ReceptConfig
// Configuration of the reception parameters
u8 ONET_ReceptConfig(u8 flag);

// Function: ONET_SetReceptConfig
// Set Configuration of the reception parameters
inline void ONET_SetReceptConfig(u8 flag) { ONET_ReceptConfig(flag); }

// Function: ONET_SetReceptConfig
// Get Configuration of the reception parameters
inline u8 ONET_GetReceptConfig() { return ONET_ReceptConfig(ONET_RECEPT_GET); }

// Function: ONET_SetMulticastMask
// Set Configuration of the multicast mask
void ONET_SetMulticastMask(u8* addr);

// Function: ONET_GetMulticastMask
// Get Configuration of the multicast mask
void ONET_GetMulticastMask(u8* addr);

// Function: ONET_GetPacketInfo
// Obtains information about the oldest received packet
bool ONET_GetPacketInfo(struct ONET_PacketInfo* info);

// Function: ONET_GetPacket
// Obtains the oldest captured incoming packet
bool ONET_GetPacket(u8* addr);

// Function: ONET_DiscardPacket
// Discard the oldest captured incoming packet
void ONET_DiscardPacket();

// Function: ONET_SendPacketSync
// Sends a packet to the network (Synchronous execution)
u8 ONET_SendPacketSync(const u8* addr, u16 size);

// Function: ONET_SendPacketAsync
// Sends a packet to the network (Asynchronous execution)
u8 ONET_SendPacketAsync(const u8* addr, u16 size);

// Function: ONET_GetSendStatus
// Obtains the status of the packet transmission
u8 ONET_GetSendStatus();
