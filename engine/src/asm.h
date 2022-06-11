// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
// Optimized assmbler bits shift

// Right shift of A register
#define RShift(a)	RShift_##a
// a >> 1
#define RShift_1	srl	a
// a >> 2
#define RShift_2	rrca			\
					rrca			\
					and a, #0x3F
// a >> 3
#define RShift_3	rrca			\
					rrca			\
					rrca			\
					and a, #0x1F	
// a >> 4
#define RShift_4	rlca			\
					rlca			\
					rlca			\
					rlca			\
					and a, #0x0F
// a >> 5
#define RShift_5	rlca			\
					rlca			\
					rlca			\
					and a, #0x07
// a >> 6
#define RShift_6	rlca			\
					rlca			\
					and a, #0x03
// a >> 7
#define RShift_7	rlca			\
					and a, #0x01

// Left shift of A register
#define LShift(a)	LShift_##a
// a << 1
#define LShift_1	add	a, a
// a << 2
#define LShift_2	add	a, a		\
					add	a, a
// a << 3
#define LShift_3	add	a, a		\
					add	a, a		\
					add	a, a
// a << 4
#define LShift_4	add	a, a		\
					add	a, a		\
					add	a, a		\
					add	a, a
// a << 5
#define LShift_5	rrca			\
					rrca			\
					rrca			\
					and a, #0xE0
// a << 6
#define LShift_6	rrca			\
					rrca			\
					and a, #0xC0
// a << 7
#define LShift_7	rrca			\
					and a, #0x80

//-----------------------------------------------------------------------------
// Undocumented Z80 instruction

#define ld_______ixl_e		.db 0xDD, 0x6B
#define ld_______ixh_e		.db 0xDD, 0x63

#define ld_______b_iyh		.db 0xFD, 0x44
#define ld_______c_iyl		.db 0xFD, 0x4D

#define ld_______iyh_b		.db 0xFD, 0x60
#define ld_______iyl_c		.db 0xFD, 0x69