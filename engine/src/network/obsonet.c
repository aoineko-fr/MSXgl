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
#include "obsonet.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_ONET_Slot;
u8 g_ONET_Bios;
u8 g_ONET_Version[3];
u8 g_ONET_MAC[6];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// ObsoNET searching routine
u8 ONET_Initialize() __naked
{
	__asm

	NEXTSL = 0xFFFD // unused System variables address

		call	SRCHOBSO

		// Store values
		ld		(_g_ONET_Slot), a
		ld		a, #0				// don't use xor here to preserve carry flag
		adc		a					// store carry of shift as subtimer
		ld		(_g_ONET_Bios), a

		// Check validity
		ld		a, (_g_ONET_Slot)
		cp		#0xFF
		ret		z
		ld		a, (_g_ONET_Bios)
		cp		#0x00
		ret		z

		// Select the ROM page 0 by using WRSLT (see Section 3.3):
		ld		a, (_g_ONET_Slot)
		ld		e, #0xC0
		ld		hl, #0x7FE0
		call	R_WRSLT
		ld		a, (_g_ONET_Slot)
		ld		e, #0
		ld		hl, #0x7FE2
		call	R_WRSLT

		push	ix

		// Get BIOS version
		ld		l, #0
		ld		a, (_g_ONET_Slot)
		ld		h, a
		push	hl
		pop		iy
		ld		ix, #ONET_FUNC_GET_VERS
		call	R_CALSLT
		ld		(_g_ONET_Version+0), a
		ld		a, b
		ld		(_g_ONET_Version+1), a
		ld		a, c
		ld		(_g_ONET_Version+2), a

		// Get MAC address
		ld		ix, #ONET_FUNC_GET_HWAD
		call	R_CALSLT
		ld		(_g_ONET_MAC+0), hl
		ld		(_g_ONET_MAC+2), de
		ld		(_g_ONET_MAC+4), bc

		ei							// because CALSLT do DI
		pop		ix

		ld		a, (_g_ONET_Slot)	// Return value
		ret

	// SRCHOBSO - ObsoNET searching routine - by Nestor Soriano
	// ObsoNET (c) 2004
	// by Daniel Berdugo (hardware) and Nestor Soriano (software)
	// 
	// Input:   -
	// Output:  A  = ObsoNET slot (#FF if not found)
	//          Cy = 1 if the ObsoNET has BIOS
	//          ObsoNET slot connected on page 1
	//          (if not found, last available slot connected on page 1)
	SRCHOBSO:
		ld		a, #0xFF			// Initializes the NEXTSLOT routine
		ld		(NEXTSL), a

	SRCHLOOP:
		call	NEXTSLOT			// Obtains the next available slot...
		cp		#0xFF				// (if no more slots are availabe, terminates)
		ret		z

		push	af
		ld		h, #0x40			// ...and conects it.
		call	R_ENASLT
		pop		af

		ld		c, a
		call	CHKOBNET			// Checks if there is an ObsoNET...

		jr		nc, SRCHLOOP		// ...if not, go to the next slot.

		call	CHKBIOS				// Checks if it has BIOS and terminates.
		ld		a, c
		ret

	// --- Checks the presence of an ObsoNET on page 1.
	//     Input:    -
	//     Output:   Cy = 1 if there is an ObsoNET on page 1, 0 if not
	//     Modifies: AF, B
	CHKOBNET:
		ld		a, (0x7FE0)			// Save value to restore it
		ld		b, a				// if there is no ObsoNET

		xor		a					// Switches registers bank 0
		ld		(0x7FE0), a

		ld		a, (0x7FEA)			// Reads registers 10 and 11 (8019ID0/1),
		cp		#0x50				// they must return #50 and #70 respectively
		jr		nz, NO_OBNET
		ld		a, (0x7FEB)
		cp		#0x70
		jr		nz, NO_OBNET

		ld		a, #0x80			// Switches registers bank 0
		ld		(0x7FE0), a

		ld		a,(0x7FEA)			// Reads registers 10 and 11 (unused,
		cp		#0x50				// they must NOT return #50 and #70)
		scf
		ret		nz
		ld		a, (0x7FEB)
		cp		#0x70
		scf
		ret		nz

	NO_OBNET:
		ld		a, b				// No ObsoNET found: restores contents
		ld		(0x7FE0), a			// of #7FE0 in case it is RAM
		or		a
		ret

	// --- Checks if there is BIOS in the ObsoNET of page 1.
	//     Input:    -
	//     Output:   Cy = 1 if there is BIOS, 0 if not
	//     Modifies: AF, HL, DE
	CHKBIOS:
		ld		a, #0xC0			// Connects ROM page 0
		ld		(0x7FE0), a
		xor		a
		ld		(0x7FE2), a

		ld		hl, #0x7FD0
		ld		de, #OBSTRING

	CHKBLOOP: // Strings comparison loop
		ld		a, (de)
		cp		(hl)
		scf
		ccf
		ret		nz

		or		a
		scf
		ret		z

		inc		hl
		inc		de
		jr		CHKBLOOP

	OBSTRING:
		.asciz	"ObsoNET"

	// --- NEXTSLOT:
	//     Returns in A the next slot available on the system every time it is called.
	//     When no more slots are available it returns #FF.
	//     To initialize it, #FF must be written in NEXTSL.
	//     Modifies: AF, BC, HL
	NEXTSLOT:
		ld		a, (NEXTSL)
		cp		#0xFF
		jr		nz, NXTSL1
		ld		a, (M_EXPTBL)			// First slot
		and		#0b10000000
		ld		(NEXTSL), a
		ret

	NXTSL1:
		ld		a, (NEXTSL)
		cp		#0b10001111
		jr		z, NOMORESL			// No more slots?
		cp		#0b00000011
		jr		z, NOMORESL
		bit		7, a
		jr		nz, SLTEXP

	SLTSIMP:
		and		#0b00000011			// Simple slot
		inc		a
		ld		c, a
		ld		b, #0
		ld		hl, #M_EXPTBL
		add		hl, bc
		ld		a, (hl)
		and		#0b10000000
		or		c
		ld		(NEXTSL), a
		ret

	SLTEXP:
		ld		c, a				// Expanded slot
		and		#0b00001100
		cp		#0b00001100
		ld		a, c
		jr		z, SLTSIMP
		add		#0b00000100
		ld		(NEXTSL), a
		ret

	NOMORESL:
		ld		a, #0xFF
		ret

	// NEXTSL:
		// .db		#0xFF				// Last returned slot
	__endasm;
}

//-----------------------------------------------------------------------------
void ONET_Dummy() __naked
{
	__asm

	ObsoNET_BIOSCall::
		// Set ObsoNET cartridge slot
		ld______iyl_(0x00)
		ld		e, a				// Backup register A
		ld		a, (_g_ONET_Slot)
		ld______iyh_a
		ld		a, e				// Restore register A
		// Call
		call	R_CALSLT
		ei							// Because CALSLT do DI
		ret

	__endasm;
}

//-----------------------------------------------------------------------------
// Initializes the ObsoNET card
//  * ONET_RESET (#7FCD): Initializes the ObsoNET card
//    Input:  -
//    Output: -
//
//     This routine, which is automatically executed when the computer
//     boots, initializes the ObsoNET card, leaving it ready for use.
//     More precisely, it performs the following tasks:
//     1.  Sends a RESET command to the RTL8019AS controller.
//     2.  Aborts the current packed transmission, if there is one.
//     3.  Initializes the RTL8019AS registers to their appropriate
//         values.
//     4.  Configures reception to admit broadcast packets and reject
//         multicast packets and those with bad CRC, or not addressed to
//         ourselves (except broadcast).
//     5.  Reads the hardware address of the card from EEPROM and stores
//         it on registers PAR0 to PAR5.
//     6.  Configures the multicast mask as all ones.
void ONET_Reset()
{
	__asm
		push	ix
		ld		ix, #ONET_FUNC_RESET
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Obtains the network connection status
//  * GET_NETSTAT (#7FC4): Obtains the network connection status
//    Input:  -
//    Output: A = 0 if there is no network connection
//                1 if there is network connection
//
//     This routine checks that ObsoNET is physically connected to an
//     active network, that is, if it is possible to send and receive
//     packets.
//
//     To perform the checking, a test packet is sent (whose source and
//     destination physical addresses are 00-00-00-00-00-00), so the
//     routine execution takes certain amount of time.  Therefore, it is
//     not recommended to execute it too often.
bool ONET_GetStatus()
{
	__asm
		push	ix
		ld		ix, #ONET_FUNC_GET_NETSTAT
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Activates or deactivates ObsoNET
//  * ONET_ONOFF (#7FC1): Activates or deactivates ObsoNET
//    Input:  A = 0: Obtain the current state
//                1: Activate the card
//                2: Deactivate the card
//    Output: A = Card status
//                1: Active
//                2: Inactive
//    Preserves C, DE, HL
//
//     This routine activates or deactivates the RTL8019AS controller of
//     ObsoNET.  While inactive, all the received packets will be
//     ignored.
u8 ONET_Activation(u8 flag)
{
	flag; // A
	__asm
		push	ix
		ld		ix, #ONET_FUNC_ONOFF
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Configuration of the reception parameters
//  * CONF_RX (#7FBE): Configuration of the reception parameters
//    Input:  A = Flags that indicate the configuration when set:
//                bit 7: Only return the current configuration
//                       (the other flags are ignored)
//                bit 4: Accept all the incoming packets, regardless of
//                       their destination address ("promiscuous mode").
//                bit 3: Accept the incoming packets whose destination
//                       address is a multicast address.
//                bit 2: Accept the incoming packets whose destination
//                       address is the broadcast address
//                       (FF-FF-FF-FF-FF-FF)
//                bit 1: Accept the incoming packets whose length
//                       is smaller than 64 bytes
//    Output: A = Current configuración (same as the input except bit 7)
//    Preserves BC, DE, HL
//
//     When the RTL8019AS controller detects a new incoming packet
//     received from the network, it examines its physical destination
//     address to determine whether the packet should be captured (for
//     being obtained later, see routine GET_PACKET) or not.  This
//     routine configures the kind of addresses that will be considered
//     valid for performing the capture:
//     *  The packets whose destination address is equal to the address
//        of the card (according to PAR0-PAR5 registers) are always
//        captured, regardless of the reception configuration.
//     *  If bit 4 is set to 1 ("promiscuous mode"), all the packets that
//        traverse the network will be captured, regardless of their
//        destination address.  This mode is useful to develop network
//        analyzers.
//     *  If bit 3 is set to 1, all the packets whose destination address
//        is a multicast address will be captured, if the multicast mask
//        allows it (see the CONF_MCAST routine).
//     *  If bit 2 is set to 1, all the packets whose destination address
//        is the broadcast address (FF-FF-FF-FF-FF-FF) will be captured;
//        these packets are addressed to all the machines on the network
//        and in normal circumstances should be accepted.
//     *  If bit 1 is set to 1, packets whose length is smaller than 64
//        bytes will be captured.  The Ethernet standard forces all the
//        packets that traverse the network to have a size of at least 64
//        bytes, although there are operating systems that send smaller
//        packets.
//
//     The default configuration (established by RTL_RESET) is %00000110,
//     that is, broadcast packets and packets smaller than 64 bytes are
//     accepted and multicast packets are rejected; this configuration is
//     the most appropriate in normal circumstances.
u8 ONET_ReceptConfig(u8 flag)
{
	flag; // A
	__asm
		push	ix
		ld		ix, #ONET_FUNC_CONF_RX
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Configuration of the multicast mask
//  * CONF_MCAST (#7FBB): Configuration of the multicast mask
//    Input:  Cy = 0 to obtain the current mask,
//                 1 to set up the mask
//            HL = Address to write the mask to if Cy=0;
//                 address to read the mask from Cy=1
//    Output: -
//
//     This routine obtains or establishes the multicast mask, which
//     decides what multicast addresses are considered valid as the
//     destination address for captured packets (it is also necessary to
//     activate the reception of multicast packets; see the description
//     of the CONF_RX routine).
//
//     The multicast mask consists of eight bytes, which are treated as a
//     table of 64 bits.  When a packet arrives whose physical
//     destination address is multicast (the lowest bit of its first byte
//     is one), the RTL8019AS applies on the address a hash function that
//     gives a 6 bit number as a result.  This number is used as an index
//     in the multicast mask: if the corresponding bit is set to one, the
//     packet is captured, otherwise it is discarded.
//
//     The default value of the mask (established by RTL_RESET) is all
//     ones (eight #FF bytes).
void ONET_SetMulticastMask(u8* addr)
{
	addr; // HL
	__asm
		push	ix
		ld		ix, #ONET_FUNC_CONF_MCAST
		scf							// Set Carry
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Configuration of the multicast mask
void ONET_GetMulticastMask(u8* addr)
{
	addr; // HL
	__asm
		push	ix
		ld		ix, #ONET_FUNC_CONF_MCAST
		xor		a					// Reset Carry
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Obtains information about the oldest received packet
//  * GET_INSTAT (#7FB8): Obtains information about
//                        the oldest received packet
//    Input:  -
//    Output: A =  0: There are no incoming packets available
//                 1: There are incoming packets ready to be obtained
//            BC = Size of the oldest incoming packet
//            HL = Bytes 12 (H) and 13 (L) of the oldest incoming packet
//                 (length or Ether-Type)
//
//     This routine checks if there are captured incoming packets in the
//     RTL8019AS internal buffer.  If there is at least one, it returns
//     A=1, and in HL and BC it returns information about the oldest
//     packet, which is the one that will be obtained if GET_PACKET is
//     executed.
//
//     BC returns the total size of the packet (including the Ethernet
//     header), while HL returns bytes 12 and 13 of the packet; this
//     allows to distinguish the type of frame used by the packet,
//     Ethernet 2 o IEEE802.3, before obtaining it (see (see Appendix A)
//     for details about the Ethernet frame types).
//
//     The size of the buffer used to capture packets is 8K.  If it
//     overflows, no new packets will be captured, so it is convenient to
//     get the captured packets (with GET_PACKET) regularly.
bool ONET_GetPacketInfo(struct ONET_PacketInfo* info)
{
	info; // HL
	__asm
		push	ix
		ld		ix, #ONET_FUNC_GET_INSTAT
		push	hl
		call	ObsoNET_BIOSCall
		ld		e, l
		ld		d, h
		pop		hl
		ld		(hl), c
		inc		hl
		ld		(hl), b
		inc		hl
		ld		(hl), e
		inc		hl
		ld		(hl), d
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Obtains the oldest captured incoming packet
//  * GET_PACKET (#7FB5): Obtains the oldest captured incoming packet
//    Input:  HL = Address where to put the packet
//                 (0 to discard it)
//    Output: A  = 0 if a packet has been obtained (or discarded)
//                 1 if there were no captured packets
//            BC = Size of the obtained packet
//
//     This routine copies to the address passed in HL the oldest
//     captured incoming packet, then erases the packet from the
//     RTL8019AS reception buffer.  The passed address cannot be a page 1
//     address (#4000-#7FFF).
//
//     If HL=0 is specified, the packet is discarded: it is erased from
//     the buffer but it is not copied to memory.
//
//     To know in advance whether there are availavle packets before
//     trying to obtain them, the GET_INSTAT routine can be used.
bool ONET_GetPacket(u8* addr)
{
	addr; // HL
	__asm
		push	ix
		ld		ix, #ONET_FUNC_GET_PACKET
		push	hl
		inc		hl
		inc		hl
		call	ObsoNET_BIOSCall
		pop		hl
		ld		(hl), c
		inc		hl
		ld		(hl), b
		dec		a					// Invert boolean A value
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Discard the oldest captured incoming packet
void ONET_DiscardPacket()
{
	__asm
		push	ix
		ld		ix, #ONET_FUNC_GET_PACKET
		ld		hl, #0
		call	ObsoNET_BIOSCall
		dec		a					// Invert boolean A value
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Sends a packet to the network (Synchronous execution)
//  * SEND_PACKET (#7FB2): Sends a packet to the network
//    Input:  HL = Address of the packet to be sent
//            BC = Length of the packet to be sent (14 to 1514 bytes)
//            A  = 0: Synchronous execution
//                    (wait until the transmission finishes)
//                 1: Asynchronous execution
//                    (exit immediately after initiating the transmission)
//    Output: In synchronous execution:
//            A = 0: Packet successfully sent
//                1: Invalid packet length
//                3: Transmission aborted due to carrier loss
//                4: Transmission aborted due to excessive collisions
//            In asynchronous execution:
//            A = 0: The packet transmission has started
//                1: Invalid packet length
//
//     This routine sends (or starts sending) the packet stored in the
//     address passed in HL.  This address cannot be a page 1 address
//     (#4000-#7FFF).
//
//     The total packet size, including the Ethernet header, must be
//     passed in BC, and must be a value between 14 and 1514.  If a value
//     smaller than 64 is passed, then actually 64 bytes will be sent to
//     the wire: the packet, followed by random data.
//
//     If synchronous execution is selected, the routine will wait until
//     the packet transmission finishes (or fails), then it will return
//     the appropriate termination code in A.  Otherwise (if asynchronous
//     execution is selected), the routine will order the RTL8019AS to
//     start the packet transmission, then it will return immediately; to
//     know the transmission status (in progress, successfully finished,
//     or failed) the SEND_STATUS routine must be used.
//
//     Note that if there is already a transmission in progress when
//     SEND_PACKET is executed, the routine will wait until that
//     transmission finishes before starting the new one.  This happens
//     always, regardless of whether synchronous or asynchronous
//     transmission is requested.
u8 ONET_SendPacketSync(const u8* addr, u16 size)
{
	addr; // HL
	size; // DE
	__asm
		push	ix
		ld		ix, #ONET_FUNC_SEND_PACKET
		xor		a
		ld		b, d
		ld		c, e
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Sends a packet to the network (Asynchronous execution)
u8 ONET_SendPacketAsync(const u8* addr, u16 size)
{
	addr; // HL
	size; // DE
	__asm
		push	ix
		ld		ix, #ONET_FUNC_SEND_PACKET
		ld		a, #1
		ld		b, d
		ld		c, e
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Obtains the status of the packet transmission
//   * SEND_STATUS (#7FAF): Obtains the status of the packet transmission
//     Input:  -
//     Output: A = 0: No packets have been sent since the last execution of RTL_RESET
//                 1: Transmission in progress
//                 2: The packet has been successfully sent
//                 3: Transmission aborted due to carrier loss
//                 4: Transmission aborted due to excessive collisions
//     Preserves C, DE, HL
//
//      This routine returns the state of the last packet transmission
//      performed with the SEND_PACKET routine.  The returned state is
//      always valid, regardless of whether synchronous or asynchronous
//      transmission was requested; for example, if a synchronous
//      execution of SEND_PACKET returned A=0, then this routine will
//      return A=2.
//
//      The obtained information will not be erased: successive executions
//      of this routine will always return the same value until
//      SEND_PACKET (or RTL_RESET) is executed again.
u8 ONET_GetSendStatus()
{
	__asm
		push	ix
		ld		ix, #ONET_FUNC_SEND_STATUS
		call	ObsoNET_BIOSCall
		pop		ix
	__endasm;
}