// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
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
