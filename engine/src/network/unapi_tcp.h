// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄       ▄▄                 ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▀█ ▄███ ██▀ █ ██ ▄█▀▄ ██▄▀ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██ █ ▀█▄▄ ▀█▄ █▀█▀ ▀█▄▀ ██   ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
/*
   GR8NET internetworking adapter
   MSX TCP-IP UNAPI implementation version 1.0 for Fusion-C / header file
   Initial release by Eugeny Brychkov 2019-05-31
   License: keeps the same as TCP/IP UNAPI v.1.0 by Konamiman, plus requirement
            to cite original author when this work or its derivatives are used

   The workflow is the following:
	1. Perform enumeration of the implementations using tcpip_enumerate(), it will reset active_implementation to 0. The routine
	   will check for maximum 4 implementations (e.g. 4 GR8NETs installed in the system), and return this value;
	2. You set appropriate implementation number in the active_implementation, and work with this implementation getting its
	   capabilities, network properties etc.
	3. If you need to work with another implementation, change active_implementation, and then all calls will be performed
	   to the respective implementation. Note that structures must be different for each connection-implementation as they
	   are socket-specific. Any UNAPI-related routine will return integer error code
*/
#pragma once

// TCP/IP UNAPI management
extern	int	tcpip_enumerate(void);			// performs checking for installed implementation and gets calling information for 4 of them, returns number of implementations (max 4). Resets active_implementation to 0
extern	int	active_implementation;			// currently active implementation: 0 to 3, being reset to 0 by tcpip_enumerate()
extern	int	implementation_count;			// number of available implementations. Populated by tcpip_enumerate(), intially set as 0
extern	int	ram_helper_call_address;		// RAM helper jump table address in CPU bank 3, populated by tcpip_enumerate()
extern	int	tcpip_impl_getinfo(char** impl_string,	// UNAPI_GET_INFO: Obtain the implementation name and version. The implementation
			int* api_spec_version,		// is identified by the active_implementation (0..3). If active_implementation number
			int* api_impl_version);		// is wrong (not within 0..number of implementations-1) then ERR_INV_IMPL is returned

// Error codes, integer values
#define	ERR_OK					0	// Operation completed successfully
#define	ERR_NOT_IMP				1	// Capability not implemented
#define	ERR_NO_NETWORK			2	// No network connection available
#define	ERR_NO_DATA				3	// No incoming data available
#define	ERR_INV_PARAM			4	// Invalid input parameter
#define	ERR_QUERY_EXISTS		5	// Another query is already in progress
#define	ERR_INV_IP				6	// Invalid IP address
#define	ERR_NO_DNS				7	// No DNS servers are configured
#define	ERR_DNS					8	// Error returned by DNS server
#define	ERR_NO_FREE_CONN		9	// No free connections available
#define	ERR_CONN_EXISTS			10	// Connection already exists
#define	ERR_NO_CONN				11	// Connection does not exists
#define	ERR_CONN_STATE			12	// Invalid connection state
#define	ERR_BUFFER				13	// Insufficient output buffer space
#define	ERR_LARGE_DGRAM			14	// Datagram is too large
#define	ERR_INV_OPER			15	// Invalid operation
#define	ERR_INV_IMPL			255	// Invalid implementation set by the active_implementation variable

// Network media states
#define	MEDIA_STATE_CLOSED		0
#define	MEDIA_STATE_OPENING		1
#define	MEDIA_STATE_OPEN		2
#define	MEDIA_STATE_CLOSING		3
#define	MEDIA_STATE_UNKNOWN		255

// TCP communication states
#define	TCP_STATE_UNKNOWN		0
#define	TCP_STATE_LISTEN		1
#define	TCP_STATE_SYN_SENT		2
#define	TCP_STATE_SYN_RECEIVED	3
#define	TCP_STATE_ESTABLISHED	4
#define	TCP_STATE_FIN_WAIT_1	5
#define	TCP_STATE_FIN_WAIT_2	6
#define	TCP_STATE_CLOSE_WAIT	7
#define	TCP_STATE_CLOSING		8
#define	TCP_STATE_LAST_ACK		9
#define	TCP_STATE_TIME_WAIT		10

// TCP communication close reasons
#define	TCP_CLOSER_UNKNOWN		0
#define	TCP_CLOSER_NO_CONN		1	// This connection has never been used since the implementation was initialized
#define	TCP_CLOSER_CLOSED		2	// The TCPIP_TCP_CLOSE method was called
#define	TCP_CLOSER_ABORTED		3	// The TCPIP_TCP_ABORT method was called
#define	TCP_CLOSER_RST			4	// A RST segment was received (the connection was refused or aborted by the remote host)
#define	TCP_CLOSER_US_TOUT		5	// The user timeout expired
#define	TCP_CLOSER_CE_TOUT		6	// The connection establishment timeout expired
#define	TCP_CLOSER_LOST			7	// Network connection was lost while the TCP connection was open
#define	TCP_CLOSER_UNREACH		8	// ICMP "Destination unreachable" message received

// Connection types
#define	CONNTYPE_TRANSIENT		0
#define	CONNTYPE_RESIDENT		1

// Operation definitions for tcpip_config_ping
#define	CONFIG_PING_GET			0
#define	CONFIG_PING_SET			1

// tcpip_unapi_capab_flags_llproto
typedef	struct
{
	char	proto_used;		// Link level protocol used
	int		feat_flags;		// Features flags
	int		capab_flags;	// Capabilities flags
} tcpip_unapi_capab_flags_llproto;

// tcpip_unapi_capab_connections
typedef	struct
{
	char	max_tcp_conn;	// Maximum simultaneous TCP connections supported
	char	max_udp_conn;	// Maximum simultaneous UDP connections supported
	char	max_raw_conn;	// Maximum simultaneous raw IP connections supported
	char	free_tcp_conn;	// Free TCP connections currently available
	char	free_udp_conn;	// Free UDP connections currently available
	char	free_raw_conn;	// Free raw IP connections currently available
} tcpip_unapi_capab_connections;

// tcpip_unapi_capab_dtg_sizes
typedef	struct
{
	int	max_outgoing_dtg_size;	// Maximum outgoing datagram size supported
	int	max_incoming_dtg_size;	// Maximum incoming datagram size supported
} tcpip_unapi_capab_dtg_sizes;

// tcpip_unapi_ip_info
typedef	struct
{
	char	local_ip[4];		// Local IP address
	char	peer_ip[4];			// Peer IP address
	char	subnet_mask[4];		// Subnet mask
	char	gateway_ip[4];		// Default gateway
	char	dns_ip_pri[4];		// Primary DNS server IP address
	char	dns_ip_sec[4];		// Secondary DNS server IP address
} tcpip_unapi_ip_info;

// tcpip_unapi_echo
typedef struct
{
	char	dest_ip[4];		// +0 (4): IP address of the destination machine
	char	ttl;			// +4 (1): TTL for the datagram
	int		icmp_id;		// +5 (2): ICMP identifier
	int		seq_number;		// +7 (2): ICMP sequence number
	int		data_length;	// +9 (2): Data length, 0 to maximum datagram size - 28
} tcpip_unapi_echo;

// tcpip_unapi_dns_q
typedef	struct
{
	char	flags;
	char	state;			// note that state (reg B) and substate (reg C) have different meaning for tcpip_dns_q
	char	substate;		// and tcpip_dns_s. Please refer to the UNAPI description for more information
	char	host_ip[4];		// Resolved IP address (only if no error occurred and state of 1 or 2 is returned)} tcpip_unapi_dns_q;

// tcpip_unapi_udp_state
typedef struct
{
	char	num_of_pend_dtg;		// Number of pending incoming datagrams
	int		size_of_oldest_dtg;		// Size of oldest pending incoming datagram (data part only)
	int		port_number;			// Local port number
} tcpip_unapi_udp_state;

// tcpip_unapi_udp_dtg_parms
typedef	struct
{
	char	dest_ip[4];		// +0 (4): Destination IP address
	int		dest_port;		// +4 (2): Destination port
	int		data_length;	// +6 (2): Data length to send or actual data length when receiving)
} tcpip_unapi_udp_dtg_parms;

// tcpip_unapi_tcp_conn_parms
typedef	struct
{
	char	dest_ip[4];				// +0 (4): Remote IP address (0.0.0.0 for unespecified remote socket)
	int		dest_port;				// +4 (2): Remote port (ignored if unespecified remote socket)
	int		local_port;				// +6 (2): Local port, 0FFFFh for a random value
	int		user_timeout;			// +8 (2): Suggestion for user timeout value
	char	flags;					// +10 (1): Flags
	char	conn_state;				// +11 (1): Connection state updated by TCPIP_TCP_STATE
	char	close_reason;			// +12 (1): Close reason (only if ERR_NO_CONN is returned) by TCPIP_TCP_STATE
	int		incoming_bytes;			// +13 (2): Number of total available incoming bytes updated by TCPIP_TCP_STATE and TCPIP_TCP_RCV
	int		urgent_incoming_bytes;	// +15 (2): Number of urgent available incoming bytes updated by TCPIP_TCP_STATE and TCPIP_TCP_RCV
	int		send_free_bytes;		// +17 (2): Available free space in the output buffer (0FFFFh = infinite) updated by TCPIP_TCP_STATE
} tcpip_unapi_tcp_conn_parms;

// tcpip_unapi_ipraw_parms
typedef	struct
{
	char	ip[4];			// +0 (4): Destination IP address
	int		data_length;	// +4 (2): Data length
} tcpip_unapi_ipraw_parms;

// tcpip_unapi_ipraw_state
typedef	struct
{
	char	proto_code;			// +0 (1): Associated protocol code
	int		num_of_pend_dtg;	// +2 (2): Number of pending incoming datagrams
	int		size_of_oldest_dtg;	// +4 (2): Size of the oldest pending incoming datagram
} tcpip_unapi_ipraw_state;

/***** Network capabilities and state routines *****/

/* TCPIP_GET_CAPAB: Get information about the TCP/IP capabilities/features/link level protocol, connection pool and datagram sizes */
extern	int	tcpip_get_capab_flags_llproto(tcpip_unapi_capab_flags_llproto* capab_flags_llproto);
extern	int	tcpip_get_capab_connections(tcpip_unapi_capab_connections* capab_connections);
extern	int	tcpip_get_capab_dtg_sizes(tcpip_unapi_capab_dtg_sizes* capab_dtg_sizes);

/* TCPIP_GET_IPINFO: Get IP address. Always returns ERR_OK, if was unable to acquire specific IP address respective field is left as 0.0.0.0 */
extern	int	tcpip_get_ipinfo(tcpip_unapi_ip_info* ip_info);

/* TCPIP_NET_STATE: Get network state */
extern	int	tcpip_net_state(int* net_state);

/***** ICMP routines *****/

/* TCPIP_SEND_ECHO: Send ICMP echo message (PING). All values of the structure
   must be populated before performing the call */
extern	int	tcpip_send_echo(tcpip_unapi_echo* echo_param_block);

/* TCPIP_RCV_ECHO: Retrieve ICMP echo response message. The call populates
   the structure at the address passed */
extern	int	tcpip_rcv_echo(tcpip_unapi_echo* echo_param_block);

/***** Host name resoultion routines *****/

/* TCPIP_DNS_Q: Start a host name resolution query. Flags field in the structure
   must be defined before performing the call: 
   - bit 0: Only abort the query currently in progress, if there is any (other flags and registers are then ignored)
   - bit 1: Assume that the passed name is an IP address, and return an error if this is not true
   - bit 2: If there is a query in progress already, do NOT abort it and return an error instead */
extern	int	tcpip_dns_q(char* hostname, tcpip_unapi_dns_q* dns_q);

/* TCPIP_DNS_S: Obtains the host name resolution process state and result. Flags field
   in the structure must be defined perform performing the call:
   - bit 0: Clear any existing result or error condition after the execution (except if there is a query in progress) */
extern	int	tcpip_dns_s(tcpip_unapi_dns_q* dns_q);

/***** UDP protocol related routines *****/

/* TCPIP_UDP_OPEN: Open a UDP connection */
extern	int	tcpip_udp_open(int port_number, int conn_lifetime, int* conn_number);

/* TCPIP_UDP_CLOSE: Close a UDP connection */
extern	int	tcpip_udp_close(int conn_number);

/* TCPIP_UDP_STATE: Get the state of a UDP connection. Populates local port number,
   number of pending datagrams and size of oldest pending datagram fields */
extern	int	tcpip_udp_state(int conn_number, tcpip_unapi_udp_state* udp_state);

/* TCPIP_UDP_SEND: Send an UDP datagram. Destination IP address, destination
   port number and data size fields must be populated */
extern	int	tcpip_udp_send(int conn_number, tcpip_unapi_udp_dtg_parms* dtg_parms, char* data);

/* TCPIP_UDP_RCV: Retrieve an incoming UDP datagram. Populates IP address,
   source port and actual received data length fields */
extern	int	tcpip_udp_rcv(int conn_number, char* buffer, int dtg_maxsize, tcpip_unapi_udp_dtg_parms* dtg_parms);

/***** TCP protocol related routines *****/

/* TCPIP_TCP_OPEN: Open a TCP connection. Remote IP address, port numbers,
   timeout value and flags fields must be populated before performing the call */
extern	int	tcpip_tcp_open(tcpip_unapi_tcp_conn_parms* tcp_conn_params, int* conn_number);

/* TCPIP_TCP_CLOSE: Close a TCP connection */
extern	int	tcpip_tcp_close(int conn_number);

/* TCPIP_TCP_ABORT: Abort a TCP connection */
extern	int	tcpip_tcp_abort(int conn_number);

/* TCPIP_TCP_STATE: Get the state of a TCP connection. Puts information into connection state,
   close reason, both incoming and free bytes fields. It will also put IP address and port
   numbers in */
extern	int	tcpip_tcp_state(int conn_number, tcpip_unapi_tcp_conn_parms* tcp_conn_params);

/* TCPIP_TCP_SEND: Send data a TCP connection. Flags field must be defined
   beform performing the call (bit 0: Send the data PUSHed; bit 1: The data is urgent) */
extern	int	tcpip_tcp_send(int conn_number, char* data, int data_length, int flags);

/* TCPIP_TCP_RCV: Receive data from a TCP connection. Updates both received byte numbers
   within the structure */
extern	int	tcpip_tcp_rcv(int conn_number, char* buffer, int maxsize, tcpip_unapi_tcp_conn_parms* tcp_conn_params);

/* TCPIP_TCP_FLUSH: Flush the output buffer of a TCP */
extern	int	tcpip_tcp_flush(int conn_number);

/***** Raw IP connections related routines *****/

/* TCPIP_RAW_OPEN: Open a raw IP connection. IPRAW protocol list is here:
   http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml */
extern	int	tcpip_ipraw_open(int protocol_number, int conn_lifetime, int* conn_number);

/* TCPIP_RAW_CLOSE: Close a raw IP connection */
extern	int	tcpip_ipraw_close(int conn_number);

/* TCPIP_RAW_STATE: Get the state of a raw IP connection */
extern	int	tcpip_ipraw_state(int conn_number, tcpip_unapi_ipraw_state* ipraw_state);

/* TCPIP_RAW_SEND: Send a raw IP datagram. Structure passed must contain
   destination IP address and data size set before calling */
extern	int	tcpip_ipraw_send(int conn_number, char* data, tcpip_unapi_ipraw_parms* ipraw_params);

/* TCPIP_RAW_RCV: Retrieve an incoming raw IP datagram */
extern	int	tcpip_ipraw_rcv(int conn_number, char* buffer, int maxsize, tcpip_unapi_ipraw_parms* ipraw_params);

/***** Configuration related routines*****/

/* TCPIP_CONFIG_AUTOIP: Enable or disable the automatic IP addresses retrieval */
extern	int	tcpip_config_autoip_get(char* ip_mode);
extern	int	tcpip_config_autoip_set(char ip_mode);

/* TCPIP_CONFIG_IP: Manually configure an IP address */
extern	int	tcpip_config_ip(tcpip_unapi_ip_info* ip_info);

/* TCPIP_CONFIG_TTL: Get/set the value of TTL and TOS for outgoing datagrams */
extern	int	tcpip_config_ttltos_get(unsigned* ttltos_get);
extern	int	tcpip_config_ttltos_set(unsigned ttltos_set);

/* TCPIP_CONFIG_PING: Get/set the automatic PING reply flag */
extern	int	tcpip_config_ping_get(char* flag_value_get);
extern	int	tcpip_config_ping_set(char flag_value_set);

/* End of header file */