// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// DEFINES
//=============================================================================

// Ethernet framing types
enum ETHERFRAME
{
	ETHERFRAME_ETHER2 = 0, // Ethernet II framing
	ETHERFRAME_IEEE,       // IEEE 802.3 framing
};

#define GET_ETHERFRAME(a) (a > 1500) ? ETHERFRAME_ETHER2 : ETHERFRAME_IEEE

// Ethernet II frame header
struct EtherHeader
{
	u8  Dest[6];
	u8  Source[6];
	u16 Type;						// /!\ Big endian
};

// IEEE 802.3 frame header
struct EtherHeaderIEEE
{
	u8  Dest[6];
	u8  Source[6];
	u16 Length;						// /!\ Big endian
	u8  Payload[6];
	u16 Type;						// /!\ Big endian
};

// EtherType (see https://en.wikipedia.org/wiki/EtherType)
enum ETHERTYPE
{
	ETHERTYPE_IPV4			= 0x0800, // Internet Protocol version 4 (IPv4)
	ETHERTYPE_ARP			= 0x0806, // Address Resolution Protocol (ARP)
	ETHERTYPE_WOLAN			= 0x0842, // Wake-on-LAN
	ETHERTYPE_AVTP			= 0x22F0, // Audio Video Transport Protocol (AVTP)
	ETHERTYPE_IETF			= 0x22F3, // IETF TRILL Protocol
	ETHERTYPE_SRP			= 0x22EA, // Stream Reservation Protocol
	ETHERTYPE_DEC_MOP		= 0x6002, // DEC MOP RC
	ETHERTYPE_DECNET		= 0x6003, // DECnet Phase IV, DNA Routing
	ETHERTYPE_DEC_LAT		= 0x6004, // DEC LAT
	ETHERTYPE_RARP			= 0x8035, // Reverse Address Resolution Protocol (RARP)
	ETHERTYPE_ETHERTALK		= 0x809B, // AppleTalk (Ethertalk)
	ETHERTYPE_AARP			= 0x80F3, // AppleTalk Address Resolution Protocol (AARP)
	ETHERTYPE_VLANTF		= 0x8100, // VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility
	ETHERTYPE_SLPP			= 0x8102, // Simple Loop Prevention Protocol (SLPP)
	ETHERTYPE_VLACP			= 0x8103, // Virtual Link Aggregation Control Protocol (VLACP)
	ETHERTYPE_IPX			= 0x8137, // IPX
	ETHERTYPE_QNX			= 0x8204, // QNX Qnet
	ETHERTYPE_IPV6			= 0x86DD, // Internet Protocol Version 6 (IPv6)
	ETHERTYPE_EFC			= 0x8808, // Ethernet flow control
	ETHERTYPE_LACP			= 0x8809, // Ethernet Slow Protocols such as the Link Aggregation Control Protocol (LACP)
	ETHERTYPE_COBRA			= 0x8819, // CobraNet
	ETHERTYPE_MPLS_UNI		= 0x8847, // MPLS unicast
	ETHERTYPE_MPLS_MUL		= 0x8848, // MPLS multicast
	ETHERTYPE_PPPOE_DISC	= 0x8863, // PPPoE Discovery Stage
	ETHERTYPE_PPPOE_SES		= 0x8864, // PPPoE Session Stage
	ETHERTYPE_HOMEPLUG_MME	= 0x887B, // HomePlug 1.0 MME
	ETHERTYPE_EAP			= 0x888E, // EAP over LAN (IEEE 802.1X)
	ETHERTYPE_PROFINET		= 0x8892, // PROFINET Protocol
	ETHERTYPE_SCSI			= 0x889A, // HyperSCSI (SCSI over Ethernet)
	ETHERTYPE_ATA			= 0x88A2, // ATA over Ethernet
	ETHERTYPE_ETHERCAT		= 0x88A4, // EtherCAT Protocol
	ETHERTYPE_SVLAN			= 0x88A8, // Service VLAN tag identifier (S-Tag) on Q-in-Q tunnel.
	ETHERTYPE_POWERLINK		= 0x88AB, // Ethernet Powerlink
	ETHERTYPE_GOOSE			= 0x88B8, // GOOSE (Generic Object Oriented Substation event)
	ETHERTYPE_GSE			= 0x88B9, // GSE (Generic Substation Events) Management Services
	ETHERTYPE_SV			= 0x88BA, // SV (Sampled Value Transmission)
	ETHERTYPE_MIKROTIK		= 0x88BF, // MikroTik RoMON (unofficial)
	ETHERTYPE_LLDP			= 0x88CC, // Link Layer Discovery Protocol (LLDP)
	ETHERTYPE_SERCOS		= 0x88CD, // SERCOS III
	ETHERTYPE_HOMEPLUG_PHY	= 0x88E1, // HomePlug Green PHY
	ETHERTYPE_MRP			= 0x88E3, // Media Redundancy Protocol (IEC62439-2)
	ETHERTYPE_MACSEC		= 0x88E5, // IEEE 802.1AE MAC security (MACsec)
	ETHERTYPE_PBB			= 0x88E7, // Provider Backbone Bridges (PBB) (IEEE 802.1ah)
	ETHERTYPE_PTP			= 0x88F7, // Precision Time Protocol (PTP) over IEEE 802.3 Ethernet
	ETHERTYPE_NCSI			= 0x88F8, // NC-SI
	ETHERTYPE_PRP			= 0x88FB, // Parallel Redundancy Protocol (PRP)
	ETHERTYPE_CFM			= 0x8902, // IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
	ETHERTYPE_FCOE			= 0x8906, // Fibre Channel over Ethernet (FCoE)
	ETHERTYPE_FCOE_INI		= 0x8914, // FCoE Initialization Protocol
	ETHERTYPE_ROCE			= 0x8915, // RDMA over Converged Ethernet (RoCE)
	ETHERTYPE_TTE			= 0x891D, // TTEthernet Protocol Control Frame (TTE)
	ETHERTYPE_1905			= 0x893a, // 1905.1 IEEE Protocol
	ETHERTYPE_HSR			= 0x892F, // High-availability Seamless Redundancy (HSR)
	ETHERTYPE_ECTP			= 0x9000, // Ethernet Configuration Testing Protocol
	ETHERTYPE_RT			= 0xF1C1, // Redundancy Tag (IEEE 802.1CB Frame Replication and Elimination for Reliability)
};























