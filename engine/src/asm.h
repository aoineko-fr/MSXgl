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
//-----------------------------------------------------------------------------

// Right shift of A register
#define RShift(a)			RShift_##a
// a >> 1
#define RShift_1			srl	a
// a >> 2
#define RShift_2			rrca			\
							rrca			\
							and a, #0x3F
// a >> 3
#define RShift_3			rrca			\
							rrca			\
							rrca			\
							and a, #0x1F	
// a >> 4
#define RShift_4			rlca			\
							rlca			\
							rlca			\
							rlca			\
							and a, #0x0F
// a >> 5
#define RShift_5			rlca			\
							rlca			\
							rlca			\
							and a, #0x07
// a >> 6
#define RShift_6			rlca			\
							rlca			\
							and a, #0x03
// a >> 7
#define RShift_7			rlca			\
							and a, #0x01

// Left shift of A register
#define LShift(a)			LShift_##a
// a << 1
#define LShift_1			add	a, a
// a << 2
#define LShift_2			add	a, a		\
							add	a, a
// a << 3
#define LShift_3			add	a, a		\
							add	a, a		\
							add	a, a
// a << 4
#define LShift_4			add	a, a		\
							add	a, a		\
							add	a, a		\
							add	a, a
// a << 5
#define LShift_5			rrca			\
							rrca			\
							rrca			\
							and a, #0xE0
// a << 6
#define LShift_6			rrca			\
							rrca			\
							and a, #0xC0
// a << 7
#define LShift_7			rrca			\
							and a, #0x80

//-----------------------------------------------------------------------------
// Misc.
//-----------------------------------------------------------------------------

// Convert carry to boolean (C:0xFF, NC:0)
#define CtoBool				sbc a, a

// Convert no-carry to boolean (C:0, NC:0xFF)
#define NCtoBool			ld a, #0 		\
							adc #0xFF

//-----------------------------------------------------------------------------
// Undocumented Z80 instruction
//-----------------------------------------------------------------------------

//.............................................................................
// CB Prefix

// SLL. This instruction shifts an 8-bit quantity left (logical), then inserts 1 into the low-order bit. /!\ Incompatible with turbo R's R800 processor
// #define sll_____b			.db 0xCB, 0x30		// sll b
// #define sll_____c			.db 0xCB, 0x31		// sll c
// #define sll_____d			.db 0xCB, 0x32		// sll d
// #define sll_____e			.db 0xCB, 0x33		// sll e
// #define sll_____h			.db 0xCB, 0x34		// sll h
// #define sll_____l			.db 0xCB, 0x35		// sll l
// #define sll_____(hl)		.db 0xCB, 0x36		// sll (hl)
// #define sll_____a			.db 0xCB, 0x37		// sll a

//.............................................................................
// DD Prefix

#define	inc_____ixh			.db 0xDD, 0x24		// inc ixh
#define	dec_____ixh			.db 0xDD, 0x25		// dec ixh
#define	ld______ixh_(nn)	.db 0xDD, 0x26, nn	// ld ixh,nn
#define	inc_____ixl			.db 0xDD, 0x2C		// inc ixl
#define	dec_____ixl			.db 0xDD, 0x2D		// dec ixl
#define	ld______ixl_(nn)	.db 0xDD, 0x2E, nn	// ld ixl,nn

#define	ld______b_ixh		.db 0xDD, 0x44		// ld b,ixh
#define	ld______b_ixl		.db 0xDD, 0x45		// ld b,ixl
#define	ld______c_ixh		.db 0xDD, 0x4C		// ld c,ixh
#define	ld______c_ixl		.db 0xDD, 0x4D		// ld c,ixl
#define	ld______d_ixh		.db 0xDD, 0x54		// ld d,ixh
#define	ld______d_ixl		.db 0xDD, 0x55		// ld d,ixl
#define	ld______e_ixh		.db 0xDD, 0x5C		// ld e,ixh
#define	ld______e_ixl		.db 0xDD, 0x5D		// ld e,ixl

#define	ld______ixh_b		.db 0xDD, 0x60		// ld ixh,b
#define	ld______ixh_c		.db 0xDD, 0x61		// ld ixh,c
#define	ld______ixh_d		.db 0xDD, 0x62		// ld ixh,d
#define	ld______ixh_e		.db 0xDD, 0x63		// ld ixh,e
#define	ld______ixh_ixh		.db 0xDD, 0x64		// ld ixh,ixh
#define	ld______ixh_ixl		.db 0xDD, 0x65		// ld ixh,ixl
#define	ld______ixh_a		.db 0xDD, 0x67		// ld ixh,a

#define	ld______ixl_b		.db 0xDD, 0x68		// ld ixl,b
#define	ld______ixl_c		.db 0xDD, 0x69		// ld ixl,c
#define	ld______ixl_d		.db 0xDD, 0x6A		// ld ixl,d
#define	ld______ixl_e		.db 0xDD, 0x6B		// ld ixl,e
#define	ld______ixl_ixh		.db 0xDD, 0x6C		// ld ixl,ixh
#define	ld______ixl_ixl		.db 0xDD, 0x6D		// ld ixl,ixl
#define	ld______ixl_a		.db 0xDD, 0x6F		// ld ixl,a

#define	ld______a_ixh		.db 0xDD, 0x7C		// ld a,ixh
#define	ld______a_ixl		.db 0xDD, 0x7D		// ld a,ixl

#define	add_____a_ixh		.db 0xDD, 0x84		// add a,ixh
#define	add_____a_ixl		.db 0xDD, 0x85		// add a,ixl
#define	adc_____a_ixh		.db 0xDD, 0x8C		// adc a,ixh
#define	adc_____a_ixl		.db 0xDD, 0x8D		// adc a,ixl
#define	sub_____ixh			.db 0xDD, 0x94		// sub ixh
#define	sub_____ixl			.db 0xDD, 0x95		// sub ixl
#define	sbc_____a_ixh		.db 0xDD, 0x9C		// sbc a,ixh
#define	sbc_____a_ixl		.db 0xDD, 0x9D		// sbc a,ixl
#define	and_____ixh			.db 0xDD, 0xA4		// and ixh
#define	and_____ixl			.db 0xDD, 0xA5		// and ixl
#define	xor_____ixh			.db 0xDD, 0xAC		// xor ixh
#define	xor_____ixl			.db 0xDD, 0xAD		// xor ixl
#define	or______ixh			.db 0xDD, 0xB4		// or ixh
#define	or______ixl			.db 0xDD, 0xB5		// or ixl
#define	cp______ixh			.db 0xDD, 0xBC		// cp ixh
#define	cp______ixl			.db 0xDD, 0xBD		// cp ixl

//.............................................................................
// ED Prefix

// Multiplication /!\ Only available on turbo R's R800 processor
#define mulub___a_a			.db 0xED, 0xF9		// mulub a,a	/!\ Not guaranteed to work properly
#define mulub___a_b			.db 0xED, 0xC1		// mulub a,b
#define mulub___a_c			.db 0xED, 0xC9		// mulub a,c
#define mulub___a_d			.db 0xED, 0xD1		// mulub a,d
#define mulub___a_e			.db 0xED, 0xD9		// mulub a,e
#define mulub___a_h			.db 0xED, 0xE1		// mulub a,h	/!\ Not guaranteed to work properly
#define mulub___a_L			.db 0xED, 0xE9		// mulub a,L	/!\ Not guaranteed to work properly

#define muluw___hl_bc		.db 0xED, 0xC3		// muluw hl,bc
#define muluw___hl_de		.db 0xED, 0xD3		// muluw hl,de	/!\ Not guaranteed to work properly
#define muluw___hl_hl		.db 0xED, 0xE3		// muluw hl,hl	/!\ Not guaranteed to work properly
#define muluw___hl_sp		.db 0xED, 0xF3		// muluw hl,sp

//.............................................................................
// FD Prefix

#define	inc_____iyh			.db 0xFD, 0x24		// inc iyh
#define	dec_____iyh			.db 0xFD, 0x25		// dec iyh
#define	ld______iyh_(nn)	.db 0xFD, 0x26, nn	// ld iyh,nn
#define	inc_____iyl			.db 0xFD, 0x2C		// inc iyl
#define	dec_____iyl			.db 0xFD, 0x2D		// dec iyl
#define	ld______iyl_(nn)	.db 0xFD, 0x2E, nn	// ld iyl,nn

#define	ld______b_iyh		.db 0xFD, 0x44		// ld b,iyh
#define	ld______b_iyl		.db 0xFD, 0x45		// ld b,iyl
#define	ld______c_iyh		.db 0xFD, 0x4C		// ld c,iyh
#define	ld______c_iyl		.db 0xFD, 0x4D		// ld c,iyl
#define	ld______d_iyh		.db 0xFD, 0x54		// ld d,iyh
#define	ld______d_iyl		.db 0xFD, 0x55		// ld d,iyl
#define	ld______e_iyh		.db 0xFD, 0x5C		// ld e,iyh
#define	ld______e_iyl		.db 0xFD, 0x5D		// ld e,iyl

#define	ld______iyh_b		.db 0xFD, 0x60		// ld iyh,b
#define	ld______iyh_c		.db 0xFD, 0x61		// ld iyh,c
#define	ld______iyh_d		.db 0xFD, 0x62		// ld iyh,d
#define	ld______iyh_e		.db 0xFD, 0x63		// ld iyh,e
#define	ld______iyh_iyh		.db 0xFD, 0x64		// ld iyh,iyh
#define	ld______iyh_iyl		.db 0xFD, 0x65		// ld iyh,iyl
#define	ld______iyh_a		.db 0xFD, 0x67		// ld iyh,a

#define	ld______iyl_b		.db 0xFD, 0x68		// ld iyl,b
#define	ld______iyl_c		.db 0xFD, 0x69		// ld iyl,c
#define	ld______iyl_d		.db 0xFD, 0x6A		// ld iyl,d
#define	ld______iyl_e		.db 0xFD, 0x6B		// ld iyl,e
#define	ld______iyl_iyh		.db 0xFD, 0x6C		// ld iyl,iyh
#define	ld______iyl_iyl		.db 0xFD, 0x6D		// ld iyl,iyl
#define	ld______iyl_a		.db 0xFD, 0x6F		// ld iyl,a

#define	ld______a_iyh		.db 0xFD, 0x7C		// ld a,iyh
#define	ld______a_iyl		.db 0xFD, 0x7D		// ld a,iyl

#define	add_____a_iyh		.db 0xFD, 0x84		// add a,iyh
#define	add_____a_iyl		.db 0xFD, 0x85		// add a,iyl
#define	adc_____a_iyh		.db 0xFD, 0x8C		// adc a,iyh
#define	adc_____a_iyl		.db 0xFD, 0x8D		// adc a,iyl
#define	sub_____iyh			.db 0xFD, 0x94		// sub iyh
#define	sub_____iyl			.db 0xFD, 0x95		// sub iyl
#define	sbc_____a_iyh		.db 0xFD, 0x9C		// sbc a,iyh
#define	sbc_____a_iyl		.db 0xFD, 0x9D		// sbc a,iyl
#define	and_____iyh			.db 0xFD, 0xA4		// and iyh
#define	and_____iyl			.db 0xFD, 0xA5		// and iyl
#define	xor_____iyh			.db 0xFD, 0xAC		// xor iyh
#define	xor_____iyl			.db 0xFD, 0xAD		// xor iyl
#define	or______iyh			.db 0xFD, 0xB4		// or iyh
#define	or______iyl			.db 0xFD, 0xB5		// or iyl
#define	cp______iyh			.db 0xFD, 0xBC		// cp iyh
#define	cp______iyl			.db 0xFD, 0xBD		// cp iyl

