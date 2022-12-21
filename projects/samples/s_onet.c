// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "network/obsonet.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// Modes
#define MODE_SNIFFER				0
#define MODE_GAME					1

// Message ID
enum MESSAGE
{
	MESSAGE_INVITATION = 0,			//						Sent at regular intervals by the server to inform of the existence of a game session
	MESSAGE_JOIN_REQUEST,			//						Sent by the player who wishes to join a game session
	MESSAGE_JOIN_VALID,				// + PlayerID			Sent by the server to accept a join request
	MESSAGE_UPDATE,					// + struct Player		Sent by the player during the game session
};

// Message ID
enum STATE
{
	STATE_MENU_INIT = 0,
	STATE_MENU,
	STATE_HOSTING_INIT,
	STATE_HOSTING,
	STATE_BROWSE_INIT,
	STATE_BROWSE,
	STATE_JOINING_INIT,
	STATE_JOINING,
	STATE_PLAYING_INIT,
	STATE_PLAYING,
};

// Functions prototypes
void DisplaySniffer();
void UpdateSniffer();
void DisplayGame();
void UpdateGame();

// Process mode structure
struct Process
{
	callback Display;
	callback Update;
};

// Player structure
struct Player
{
	u8 Y;
	u8 X;
	u8 Shape;
	u8 Color;
	//---------- End of Sprite buffer
	u8 ID;
	u8 Direction;
};

// Vector structure
struct Vector
{
	i8 X;
	i8 Y;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample8.h"

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Process mode table
const struct Process g_ModeTab[2] =
{
	{ DisplaySniffer, UpdateSniffer },
	{ DisplayGame,    UpdateGame },
};

// Broadcast MAC address
const u8 g_BroadcastMAC[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Vector structure
const struct Vector g_MoveTab[8] =
{
	{  0,  1 },
	{ -1,  1 },
	{ -1,  0 },
	{ -1, -1 },
	{  0, -1 },
	{  1, -1 },
	{  1,  0 },
	{  1,  1 },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8   g_Buffer[1514+2]; // Max Ethernet frame size + 2 bytes for size value
bool g_Pending;
u8   g_Mode;
u8   g_Server[6];
u8   g_Frame;
u8   g_State;
u8   g_PlayerNum;
u8   g_Slot;
struct Player g_Player[32];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Erase(u8 sx, u8 sy, u16 num)
{
	u16 dst = g_ScreenLayoutLow + (sy * g_PrintData.ScreenWidth) + sx;
	VDP_FillVRAM(0, dst, g_ScreenLayoutHigh, num);
}

//-----------------------------------------------------------------------------
//
const c8* GetEtherTypeName(u16 id)
{
	switch(id)
	{
	case ETHERTYPE_IPV4			: return "IPv4";
	case ETHERTYPE_ARP			: return "ARP";
	case ETHERTYPE_WOLAN		: return "WoLAN";
	case ETHERTYPE_AVTP			: return "AVTP";
	case ETHERTYPE_IETF			: return "IETF";
	case ETHERTYPE_SRP			: return "SRP";
	case ETHERTYPE_DEC_MOP		: return "DEC MOP";
	case ETHERTYPE_DECNET		: return "DECnet";
	case ETHERTYPE_DEC_LAT		: return "DEC LAT";
	case ETHERTYPE_RARP			: return "RARP";
	case ETHERTYPE_ETHERTALK	: return "Eth Talk";
	case ETHERTYPE_AARP			: return "AARP";
	case ETHERTYPE_VLANTF		: return "VLANtf";
	case ETHERTYPE_SLPP			: return "SLPP";
	case ETHERTYPE_VLACP		: return "VLACP";
	case ETHERTYPE_IPX			: return "IPX";
	case ETHERTYPE_QNX			: return "QNX Qnet";
	case ETHERTYPE_IPV6			: return "IPv6";
	case ETHERTYPE_EFC			: return "EFC";
	case ETHERTYPE_LACP			: return "LACP";
	case ETHERTYPE_COBRA		: return "CobraNet";
	case ETHERTYPE_MPLS_UNI		: return "MPLS uni";
	case ETHERTYPE_MPLS_MUL		: return "MPLS mul";
	case ETHERTYPE_PPPOE_DISC	: return "PPPoE D";
	case ETHERTYPE_PPPOE_SES	: return "PPPoE S";
	case ETHERTYPE_HOMEPLUG_MME	: return "HomePlug";
	case ETHERTYPE_EAP			: return "EAPoLAN";
	case ETHERTYPE_PROFINET		: return "PROFINET";
	case ETHERTYPE_SCSI			: return "H-SCSI";
	case ETHERTYPE_ATA			: return "ATA";
	case ETHERTYPE_ETHERCAT		: return "EtherCAT";
	case ETHERTYPE_SVLAN		: return "S-Tag";
	case ETHERTYPE_POWERLINK	: return "Powerlnk";
	case ETHERTYPE_GOOSE		: return "GOOSE";
	case ETHERTYPE_GSE			: return "GSE";
	case ETHERTYPE_SV			: return "SV";
	case ETHERTYPE_MIKROTIK		: return "MikroTik";
	case ETHERTYPE_LLDP			: return "LLDP";
	case ETHERTYPE_SERCOS		: return "SERCOS3";
	case ETHERTYPE_HOMEPLUG_PHY	: return "HP PHY";
	case ETHERTYPE_MRP			: return "IEC62439";
	case ETHERTYPE_MACSEC		: return "MACsec";
	case ETHERTYPE_PBB			: return "PBB";
	case ETHERTYPE_PTP			: return "PTP";
	case ETHERTYPE_NCSI			: return "NC-SI";
	case ETHERTYPE_PRP			: return "PRP";
	case ETHERTYPE_CFM			: return "CFM/OAM";
	case ETHERTYPE_FCOE			: return "FCoE";
	case ETHERTYPE_FCOE_INI		: return "FCoE Ini";
	case ETHERTYPE_ROCE			: return "RoCE";
	case ETHERTYPE_TTE			: return "TTE";
	case ETHERTYPE_1905			: return "1905.1";
	case ETHERTYPE_HSR			: return "HSR";
	case ETHERTYPE_ECTP			: return "ECTP";
	case ETHERTYPE_RT			: return "802.1CB";
	};
	return "Unknown";
}

//-----------------------------------------------------------------------------
//
const c8* GetSendResultName(u8 val)
{
	switch(val)
	{
	case ONET_SYNC_SUCCESS			: return "Success"; // Packet successfully sent
	case ONET_SYNC_ERROR_LENGTH		: return "Error length"; // Invalid packet length
	case ONET_SYNC_ERROR_CARRIER	: return "Error carrier"; // Transmission aborted due to carrier loss
	case ONET_SYNC_ERROR_COLLISION	: return "Error collision"; // Transmission aborted due to excessive collisions
	};
	return "Unknown";
}

//-----------------------------------------------------------------------------
//
const c8* GetSendStatusName(u8 val)
{
	switch(val)
	{
	case ONET_SEND_NONE				: return "None"; // No packets have been sent since the last execution of RTL_RESET
	case ONET_SEND_PROGRESS			: return "Progress"; // Transmission in progress
	case ONET_SEND_SUCESSS			: return "Sucess"; // The packet has been successfully sent
	case ONET_SEND_ERROR_CARRIER	: return "Error carrier"; // Transmission aborted due to carrier loss
	case ONET_SEND_ERROR_COLLISION	: return "Error collision"; // Transmission aborted due to excessive collisions 
	};
	return "Unknown";
}

//-----------------------------------------------------------------------------
//
void Print_DrawSlot(u8 slot)
{
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if(Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
//
void Print_DrawMAC(const u8* addr)
{
	Print_DrawHex8(*addr++);
	Print_DrawChar('.');
	Print_DrawHex8(*addr++);
	Print_DrawChar('.');
	Print_DrawHex8(*addr++);
	Print_DrawChar('.');
	Print_DrawHex8(*addr++);
	Print_DrawChar('.');
	Print_DrawHex8(*addr++);
	Print_DrawChar('.');
	Print_DrawHex8(*addr++);
}

//-----------------------------------------------------------------------------
//
void DisplayHead()
{
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " ObsoNET Sample");
	Print_DrawLineH(0, 1, 32);
}

//-----------------------------------------------------------------------------
//
void DisplayFoot()
{
	Print_DrawLineH(0, 22, 32);
	Erase(0, 23, 32);
	Print_SetPosition(0, 23);
	if(g_Mode == MODE_SNIFFER)
		Print_DrawFormat("F1:Game R:Reset%s", (g_Pending) ? " G:Get D:Discard" : "");
	else
		Print_DrawFormat("F1:Sniffer");
}

//-----------------------------------------------------------------------------
// 
void DisplaySniffer()
{
	g_Pending = false;
	VDP_DisableSpritesFrom(0);

	DisplayHead();
	DisplayFoot();

	Print_SetPosition(0, 3);
	Print_DrawText("Slot   :");
	g_Slot = ONET_Initialize();
	if(g_Slot == 0xFF)
	{
		Print_DrawText("Not found!");
		return;
	}
	Print_DrawSlot(g_Slot);
	Print_DrawFormat("\nBIOS   :%s", ONET_HasBIOS() ? "Yes" : "No");
	Print_DrawFormat("\nVer.   :%i.%i.%i", g_ONET_Version[0], g_ONET_Version[1], g_ONET_Version[2]);
	Print_DrawText(  "\nMAC    :");
	Print_DrawMAC(g_ONET_MAC);
	Print_DrawFormat("\nConn.  :%s", ONET_GetStatus() ? "Yes" : "No");
	Print_DrawFormat("\nActive :%s", ONET_IsActivate() ? "Yes" : "No");

	u8 rec = ONET_GetReceptConfig();
	Print_DrawFormat("\nRecept :%2xh Sm%s BC%s MC%s All%s", rec,
		(rec & ONET_RECEPT_SMALL) ? "\x0C" : "\x0B",
		(rec & ONET_RECEPT_BROADCAST) ? "\x0C" : "\x0B",
		(rec & ONET_RECEPT_MULTICAST) ? "\x0C" : "\x0B",
		(rec & ONET_RECEPT_ALL) ? "\x0C" : "\x0B");

	Print_DrawText("\nPacket :");
}

//-----------------------------------------------------------------------------
// 
void UpdateSniffer()
{
	struct ONET_PacketInfo info;
	
	if(ONET_GetSlot() == 0xFF)
		return;

	if(!g_Pending)
	{
		Erase(8, 10, 32-8);
		bool bInf = ONET_GetPacketInfo(&info);
		Print_SetPosition(8, 10);
		if(bInf)
		{
			Print_DrawFormat("Yes S=%i H=%4x %s", info.Size, info.Head, GetEtherTypeName(info.Head));
			g_Pending = TRUE;
			DisplayFoot();
		}
		else
		{
			Print_DrawText("No");
			DisplayFoot();
		}
	}
	if(g_Pending)
	{
		if(Keyboard_IsKeyPressed(KEY_G)) // Get Packet
		{
			if(ONET_GetPacket(g_Buffer))
			{
				Erase(0, 11, 32*11);
				Print_SetPosition(0, 11);
				Print_DrawCharX(0x1C, 32);
				Print_DrawFormat("\nSize    %i bytes", *(i16*)g_Buffer);
				Print_DrawText("\nDest.   "); Print_DrawMAC(g_Buffer + 2);
				Print_DrawText("\nSource  "); Print_DrawMAC(g_Buffer + 8);
				u8* buf;
				if(info.Head > 1500)
				{
					buf = g_Buffer + 14;
				}
				else
				{
					Print_DrawFormat("\nLength  %i", *(i16*)(g_Buffer + 14));
					buf = g_Buffer + 22;
				}
				Print_DrawFormat("\nType    %2x%2x (%s)", buf[0], buf[1], GetEtherTypeName(Math_Swap(*(u16*)buf)));

				buf += 2;
				Print_DrawText("\nRaw     ");
				u16 size = MIN(info.Size, 8*6);
				for(u8 i = 0; i < size; ++i)
				{
					if((i > 0) && ((i % 8) == 0))
					{
						g_PrintData.CursorX = 8;
						g_PrintData.CursorY++;
					}
					Print_DrawHex8(*buf++);
					if((i % 8) != 7)
						g_PrintData.CursorX++;
				}
				g_Pending = FALSE;
				DisplayFoot();
			}
		}
		else if(Keyboard_IsKeyPressed(KEY_D)) // Discard Packet
		{
			ONET_GetPacket(0);
			g_Pending = FALSE;
			DisplayFoot();
		}
	}
}

//-----------------------------------------------------------------------------
// 
void DisplayGame()
{
	VDP_DisableSpritesFrom(0);

	g_State = STATE_MENU_INIT;

	DisplayHead();
	DisplayFoot();
}

//-----------------------------------------------------------------------------
// 
void InitPlayer()
{
	struct Player* ply = &g_Player[0];
	ply->ID = 0;
	ply->X = Math_GetRandom8();
	ply->Y = Math_GetRandom8() / 2 + 32;
	ply->Direction = Math_GetRandom8() % 8;
	ply->Shape = 0xB0 + ply->Direction;
	ply->Color = 2 + Math_GetRandom8() % 14;
	VDP_SetSpriteSM1(ply->ID, ply->X, ply->Y, ply->Shape, ply->Color);
}

//-----------------------------------------------------------------------------
// 
inline void SetMAC(u8* dest, const u8* src) { Mem_Copy(src, dest, 6); }

//-----------------------------------------------------------------------------
// 
void SendBroadcast(u8 msg, const u8* data, u16 size)
{
	// Prepare header
	struct EtherHeader* head = (struct EtherHeader*)g_Buffer;
	SetMAC(head->Dest, g_BroadcastMAC);
	SetMAC(head->Source, ONET_GetMACAddress());
	head->Type = Math_Swap(ETHERTYPE_IPV4);

	// Prepare data
	g_Buffer[14] = msg;
	if(size)
		Mem_Copy(data, g_Buffer + 15, size);

	// Send data
	ONET_SendPacketAsync(g_Buffer, size + 15);
}

//-----------------------------------------------------------------------------
// Pseudo game FSM
void UpdateGame()
{
	switch(g_State)
	{
	case STATE_MENU_INIT:
		Erase(0, 2, 20*32);
		Print_SetPosition(0, 3);
		Print_DrawText("State: Menu");
		Print_DrawText("\nMAC:   ");
		Print_DrawMAC(g_ONET_MAC);
		Print_DrawText("\n\n1. Host");
		Print_DrawText("\n2. Join");
		InitPlayer();
		g_State = STATE_MENU;

	case STATE_MENU:
		if(Keyboard_IsKeyPressed(KEY_1))
			g_State = STATE_HOSTING_INIT;
		if(Keyboard_IsKeyPressed(KEY_2))
			g_State = STATE_BROWSE_INIT;
		break;

	case STATE_HOSTING_INIT:
		Erase(0, 2, 20*32);
		Print_SetPosition(0, 3);
		Print_DrawText("State:  Hosting");
		Print_DrawText("\nLocal:  ");
		Print_DrawMAC(g_ONET_MAC);
		Print_DrawText("\nServer: ");
		Print_DrawMAC(g_ONET_MAC);
		g_PlayerNum = 1;
		g_State = STATE_HOSTING;
		break;

	case STATE_BROWSE_INIT:
		Erase(0, 2, 20*32);
		Print_SetPosition(0, 3);
		Print_DrawText("State: Browsing");
		Print_DrawText("\nMAC:   ");
		Print_DrawMAC(g_ONET_MAC);
		break;

	case STATE_BROWSE:
		break;

	case STATE_JOINING_INIT:
		Erase(0, 2, 20*32);
		Print_SetPosition(0, 3);
		Print_DrawText("State: Joining");
		break;

	case STATE_JOINING:
		break;

	case STATE_PLAYING_INIT:
		Erase(0, 2, 20*32);
		Print_SetPosition(0, 3);
		Print_DrawText("State: Playing");
		g_State = STATE_PLAYING;
		break;

	case STATE_HOSTING:
		if((g_Frame % 32) == 0)
			SendBroadcast(MESSAGE_INVITATION, NULL, 0);

		// u8 res = ONET_GetSendStatus();
		// Erase(0, 20, 32);
		// Print_SetPosition(0, 20);
		// Print_DrawText("Status: ");
		// Print_DrawText(GetSendStatusName(res));

	case STATE_PLAYING:
		{
			// Handle local player
			struct Player* ply = &g_Player[0];
			if(Keyboard_IsKeyPressed(KEY_RIGHT))
			{
				ply->Direction++;
				ply->Direction %= 32;
				ply->Shape = 0xB0 + (ply->Direction / 4);
			}
			else if(Keyboard_IsKeyPressed(KEY_LEFT))
			{
				ply->Direction--;
				ply->Direction %= 32;
				ply->Shape = 0xB0 + (ply->Direction / 4);
			}
			if(Keyboard_IsKeyPressed(KEY_UP))
			{
				const struct Vector* vec = &g_MoveTab[ply->Direction / 4];
				ply->X += vec->X;
				ply->Y += vec->Y;
			}

			// Handle local player send info
			if((g_Frame % 8) == 0)
				SendBroadcast(MESSAGE_UPDATE, (const u8*)ply, sizeof(struct Player));

			// u8 res = ONET_GetSendStatus();
			// Erase(0, 20, 32);
			// Print_SetPosition(0, 20);
			// Print_DrawText("Status: ");
			// Print_DrawText(GetSendStatusName(res));

			// Update distant players

			// Draw players
			for(u8 i = 0; i < g_PlayerNum; ++i)
				VDP_WriteVRAM((const u8*)&g_Player[i], g_SpriteAtributeLow + (i * 4), g_SpriteAtributeHigh, 3);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN1);
	VDP_SetColor(0);
	VDP_ClearVRAM();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1);
	Print_SetColor(0xF, 0x0);

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, sizeof(g_Font_MGL_Sample8) / 8 - 4);

	g_Mode = MODE_SNIFFER;
	g_ModeTab[g_Mode].Display();

	g_Pending = false;
	bool bPressed = FALSE;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[g_Frame & 0x03]);

		g_ModeTab[g_Mode].Update();

		if(Keyboard_IsKeyPressed(KEY_F1))
		{
			if((!bPressed) && (g_Slot != 0xFF))
			{
				g_Mode = 1 - g_Mode;
				g_ModeTab[g_Mode].Display();
				bPressed = TRUE;
			}
		}
		else
			bPressed = FALSE;

		if(Keyboard_IsKeyPressed(KEY_R))
			g_ModeTab[g_Mode].Display();

		g_Frame++;
	}
}