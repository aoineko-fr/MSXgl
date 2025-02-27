// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"

//=============================================================================
// DEFINES
//=============================================================================

// Macro: Q
// Generate a fixed point number ID
#define Q(m, n)						(((m) << 8) + (n))

//-----------------------------------------------------------------------------
// 8-BIT FIXED POINT
//-----------------------------------------------------------------------------

// Group: 8-bit 
// 8-bit fixed point numbers

//.............................................................................
// Q8.0 [-128:127]
//.............................................................................

// Macro: Q8_0
// Generate ID for a Q8.0 fixed point number
#define Q8_0						Q(8, 0)

// Macro: Q8_0_TYPE
// Data type of a Q8.0 fixed point number
#define Q8_0_TYPE					i8

// Macro: Q8_0_SET
// Convert source unit value to a Q8.0 fixed point number
#define Q8_0_SET(a)					(i8)(a)

// Macro: Q8_0_GET
// Convert a Q8.0 fixed point number to source unit value
#define Q8_0_GET(a)					(i8)(a)

// Macro: Q8_0_FRAC
// Get the fractional part of a Q8.0 fixed point number
#define Q8_0_FRAC(a)				0

//.............................................................................
// Q7.1 [-64:63.5]
//.............................................................................

// Macro: Q7_1
// Generate ID for a Q7.1 fixed point number
#define Q7_1						Q(7, 1)

// Macro: Q7_1_TYPE
// Data type of a Q7.1 fixed point number
#define Q7_1_TYPE					i8

// Macro: Q7_1_SET
// Convert source unit value to a Q7.1 fixed point number
#define Q7_1_SET(a)					(i8)((a) * 2)

// Macro: Q7_1_GET
// Convert a Q7.1 fixed point number to source unit value
#define Q7_1_GET(a)					(i8)((a) / 2)

// Macro: Q7_1_FRAC
// Get the fractional part of a Q7.1 fixed point number
#define Q7_1_FRAC(a)				(((a) >= 0) ? (a) & 0x01 : (a) | 0xFE)

//.............................................................................
// Q6.2 [-32:31.75]
//.............................................................................

// Macro: Q6_2
// Generate ID for a Q6.2 fixed point number
#define Q6_2						Q(6, 2)

// Macro: Q6_2_TYPE
// Data type of a Q6.2 fixed point number
#define Q6_2_TYPE					i8

// Macro: Q6_2_SET
// Convert source unit value to a Q6.2 fixed point number
#define Q6_2_SET(a)					(i8)((a) * 4)

// Macro: Q6_2_GET
// Convert a Q6.2 fixed point number to source unit value
#define Q6_2_GET(a)					(i8)((a) / 4)

// Macro: Q6_2_FRAC
// Get the fractional part of a Q6.2 fixed point number
#define Q6_2_FRAC(a)				(((a) >= 0) ? (a) & 0x03 : (a) | 0xFC)

//.............................................................................
// Q5.3 [-16:15.87]
//.............................................................................

// Macro: Q5_3
// Generate ID for a Q5.3 fixed point number
#define Q5_3						Q(5, 3)

// Macro: Q5_3_TYPE
// Data type of a Q5.3 fixed point number
#define Q5_3_TYPE					i8

// Macro: Q5_3_SET
// Convert source unit value to a Q5.3 fixed point number
#define Q5_3_SET(a)					(i8)((a) * 8)

// Macro: Q5_3_GET
// Convert a Q5.3 fixed point number to source unit value
#define Q5_3_GET(a)					(i8)((a) / 8)

// Macro: Q5_3_FRAC
// Get the fractional part of a Q5.3 fixed point number
#define Q5_3_FRAC(a)				(((a) >= 0) ? (a) & 0x07 : (a) | 0xF8)

//.............................................................................
// Q4.4 [-8:7.94]
//.............................................................................

// Macro: Q4_4
// Generate ID for a Q4.4 fixed point number
#define Q4_4						Q(4, 4)

// Macro: Q4_4_TYPE
// Data type of a Q4.4 fixed point number
#define Q4_4_TYPE					i8

// Macro: Q4_4_SET
// Convert source unit value to a Q4.4 fixed point number
#define Q4_4_SET(a)					(i8)((a) * 16)

// Macro: Q4_4_GET
// Convert a Q4.4 fixed point number to source unit value
#define Q4_4_GET(a)					(i8)((a) / 16)

// Macro: Q4_4_FRAC
// Get the fractional part of a Q4.4 fixed point number
#define Q4_4_FRAC(a)				(((a) >= 0) ? (a) & 0x0F : (a) | 0xF0)

//.............................................................................
// Q3.5 [-4:3.97]
//.............................................................................

// Macro: Q3_5
// Generate ID for a Q3.5 fixed point number
#define Q3_5						Q(3, 5)

// Macro: Q3_5_TYPE
// Data type of a Q3.5 fixed point number
#define Q3_5_TYPE					i8

// Macro: Q3_5_SET
// Convert source unit value to a Q3.5 fixed point number
#define Q3_5_SET(a)					(i8)((a) * 32)

// Macro: Q3_5_GET
// Convert a Q3.5 fixed point number to source unit value
#define Q3_5_GET(a)					(i8)((a) / 32)

// Macro: Q3_5_FRAC
// Get the fractional part of a Q3.5 fixed point number
#define Q3_5_FRAC(a)				(((a) >= 0) ? (a) & 0x1F : (a) | 0xE0)

//.............................................................................
// Q2.6 [-2:1.98]
//.............................................................................

// Macro: Q2_6
// Generate ID for a Q2.6 fixed point number
#define Q2_6						Q(2, 6)

// Macro: Q2_6_TYPE
// Data type of a Q2.6 fixed point number
#define Q2_6_TYPE					i8

// Macro: Q2_6_SET
// Convert source unit value to a Q2.6 fixed point number
#define Q2_6_SET(a)					(i8)((a) * 64)

// Macro: Q2_6_GET
// Convert a Q2.6 fixed point number to source unit value
#define Q2_6_GET(a)					(i8)((a) / 64)

// Macro: Q2_6_FRAC
// Get the fractional part of a Q2.6 fixed point number
#define Q2_6_FRAC(a)				(((a) >= 0) ? (a) & 0x3F : (a) | 0xC0)

//.............................................................................
// Q1.7 [-1:0.99]
//.............................................................................

// Macro: Q1_7
// Generate ID for a Q1.7 fixed point number
#define Q1_7						Q(1, 7)

// Macro: Q1_7_TYPE
// Data type of a Q1.7 fixed point number
#define Q1_7_TYPE					i8

// Macro: Q1_7_SET
// Convert source unit value to a Q1.7 fixed point number
#define Q1_7_SET(a)					(i8)((a) * 128)

// Macro: Q1_7_GET
// Convert a Q1.7 fixed point number to source unit value
#define Q1_7_GET(a)					(i8)((a) / 128)

// Macro: Q1_7_FRAC
// Get the fractional part of a Q1.7 fixed point number
#define Q1_7_FRAC(a)				(((a) >= 0) ? (a) & 0x7F : (a) | 0x80)

// Function: QMN_Set8
// Convert source unit 8-bit value to a Qm.n fixed point number
//
// Parameters:
//   q - Fixed point number ID
//   a - Source unit value
//
// Return:
//   Qm.n fixed point number
inline i8 QMN_Set8(u16 q, i8 a) { return a * (i8)(1 << (q & 0x00FF)); }

// Function: QMN_Get8
// Convert a Qm.n fixed point number to source unit 8-bit value
//
// Parameters:
//   q - Fixed point number ID
//   a - Qm.n fixed point number
//
// Return:
//   Source unit value
inline i8 QMN_Get8(u16 q, i8 a) { return a / (i8)(1 << (q & 0x00FF)); }

//-----------------------------------------------------------------------------
// 16-BIT FIXED POINT
//-----------------------------------------------------------------------------

// Group: 16-bit
// 16-bit fixed point numbers

//.............................................................................
// Q16.0 [-32768:32767]
//.............................................................................

// Macro: Q16_0
// Generate ID for a Q16.0 fixed point number
#define Q16_0						Q(16, 0)

// Macro: Q16_0_TYPE
// Data type of a Q16.0 fixed point number
#define Q16_0_TYPE					i16

// Macro: Q16_0_SET
// Convert source unit value to a Q16.0 fixed point number
#define Q16_0_SET(a)				(i16)(a)

// Macro: Q16_0_GET
// Convert a Q16.0 fixed point number to source unit value
#define Q16_0_GET(a)				(i16)(a)

// Macro: Q16_0_FRAC
// Get the fractional part of a Q16_0 fixed point number
#define Q16_0_FRAC(a)				0

//.............................................................................
// Q15.1 [-16384:16383.5]
//.............................................................................

// Macro: Q15_1
// Generate ID for a Q15.1 fixed point number
#define Q15_1						Q(15, 1)

// Macro: Q15_1_TYPE
// Data type of a Q15.1 fixed point number
#define Q15_1_TYPE					i16

// Macro: Q15_1_SET
// Convert source unit value to a Q15.1 fixed point number
#define Q15_1_SET(a)				(i16)((a) * 2)

// Macro: Q15_1_GET
// Convert a Q15.1 fixed point number to source unit value
#define Q15_1_GET(a)				(i16)((a) / 2)

// Macro: Q15_1_FRAC
// Get the fractional part of a Q15.1 fixed point number
#define Q15_1_FRAC(a)				(((a) >= 0) ? (a) & 0x0001 : (a) | 0xFFFE)

//.............................................................................
// Q14.2 [-8192:8191.75]
//.............................................................................

// Macro: Q14_2
// Generate ID for a Q14.2 fixed point number
#define Q14_2						Q(14, 2)

// Macro: Q14_2_TYPE
// Data type of a Q14.2 fixed point number
#define Q14_2_TYPE					i16

// Macro: Q14_2_SET
// Convert source unit value to a Q14.2 fixed point number
#define Q14_2_SET(a)				(i16)((a) * 4)

// Macro: Q14_2_GET
// Convert a Q14.2 fixed point number to source unit value
#define Q14_2_GET(a)				(i16)((a) / 4)

// Macro: Q14_2_FRAC
// Get the fractional part of a Q14.2 fixed point number
#define Q14_2_FRAC(a)				(((a) >= 0) ? (a) & 0x0003 : (a) | 0xFFFC)

//.............................................................................
// Q13.3 [-4096:4095.87]
//.............................................................................

// Macro: Q13_3
// Generate ID for a Q13.3 fixed point number
#define Q13_3						Q(13, 3)

// Macro: Q13_3_TYPE
// Data type of a Q13.3 fixed point number
#define Q13_3_TYPE					i16

// Macro: Q13_3_SET
// Convert source unit value to a Q13.3 fixed point number
#define Q13_3_SET(a)				(i16)((a) * 8)

// Macro: Q13_3_GET
// Convert a Q13.3 fixed point number to source unit value
#define Q13_3_GET(a)				(i16)((a) / 8)

// Macro: Q13_3_FRAC
// Get the fractional part of a Q13.3 fixed point number
#define Q13_3_FRAC(a)				(((a) >= 0) ? (a) & 0x0007 : (a) | 0xFFF8)

//.............................................................................
// Q12.4 [-2048:2047.94]
//.............................................................................

// Macro: Q12_4
// Generate ID for a Q12.4 fixed point number
#define Q12_4						Q(12, 4)

// Macro: Q12_4_TYPE
// Data type of a Q12.4 fixed point number
#define Q12_4_TYPE					i16

// Macro: Q12_4_SET
// Convert source unit value to a Q12.4 fixed point number
#define Q12_4_SET(a)				(i16)((a) * 16)

// Macro: Q12_4_GET
// Convert a Q12.4 fixed point number to source unit value
#define Q12_4_GET(a)				(i16)((a) / 16)

// Macro: Q12_4_FRAC
// Get the fractional part of a Q12.4 fixed point number
#define Q12_4_FRAC(a)				(((a) >= 0) ? (a) & 0x000F : (a) | 0xFFF0)

//.............................................................................
// Q11.5 [-1024:1023.97]
//.............................................................................

// Macro: Q11_5
// Generate ID for a Q11.5 fixed point number
#define Q11_5						Q(11, 5)

// Macro: Q11_5_TYPE
// Data type of a Q11.5 fixed point number
#define Q11_5_TYPE					i16

// Macro: Q11_5_SET
// Convert source unit value to a Q11.5 fixed point number
#define Q11_5_SET(a)				(i16)((a) * 32)

// Macro: Q11_5_GET
// Convert a Q11.5 fixed point number to source unit value
#define Q11_5_GET(a)				(i16)((a) / 32)

// Macro: Q11_5_FRAC
// Get the fractional part of a Q11.5 fixed point number
#define Q11_5_FRAC(a)				(((a) >= 0) ? (a) & 0x001F : (a) | 0xFFE0)

//.............................................................................
// Q10.6 [-512:511.98]
//.............................................................................

// Macro: Q10_6
// Generate ID for a Q10.6 fixed point number
#define Q10_6						Q(10, 6)

// Macro: Q10_6_TYPE
// Data type of a Q10.6 fixed point number
#define Q10_6_TYPE					i16

// Macro: Q10_6_SET
// Convert source unit value to a Q10.6 fixed point number
#define Q10_6_SET(a)				(i16)((a) * 64)

// Macro: Q10_6_GET
// Convert a Q10.6 fixed point number to source unit value
#define Q10_6_GET(a)				(i16)((a) / 64)

// Macro: Q10_6_FRAC
// Get the fractional part of a Q10.6 fixed point number
#define Q10_6_FRAC(a)				(((a) >= 0) ? (a) & 0x003F : (a) | 0xFFC0)

//.............................................................................
// Q9.7 [-256:255.99]
//.............................................................................

// Macro: Q9_7
// Generate ID for a Q9.7 fixed point number
#define Q9_7						Q(9, 7)

// Macro: Q9_7_TYPE
// Data type of a Q9.7 fixed point number
#define Q9_7_TYPE					i16

// Macro: Q9_7_SET
// Convert source unit value to a Q9.7 fixed point number
#define Q9_7_SET(a)					(i16)((a) * 128)

// Macro: Q9_7_GET
// Convert a Q9.7 fixed point number to source unit value
#define Q9_7_GET(a)					(i16)((a) / 128)

// Macro: Q9_7_FRAC
// Get the fractional part of a Q9.7 fixed point number
#define Q9_7_FRAC(a)				(((a) >= 0) ? (a) & 0x007F : (a) | 0xFF80)

//.............................................................................
// Q8.8 [-128:127.99]
//.............................................................................

// Macro: Q8_8
// Generate ID for a Q8.8 fixed point number
#define Q8_8						Q(8, 8)

// Macro: Q8_8_TYPE
// Data type of a Q8.8 fixed point number
#define Q8_8_TYPE					i16

// Macro: Q8_8_SET
// Convert source unit value to a Q8.8 fixed point number
#define Q8_8_SET(a)					(i16)((a) * 256)

// Macro: Q8_8_GET
// Convert a Q8.8 fixed point number to source unit value
#define Q8_8_GET(a)					(i16)((a) / 256)

// Macro: Q8_8_FRAC
// Get the fractional part of a Q8.8 fixed point number
#define Q8_8_FRAC(a)				(((a) >= 0) ? (a) & 0x00FF : (a) | 0xFF00)

//.............................................................................
// Q7.9 [-64:63.99]
//.............................................................................

// Macro: Q7_9
// Generate ID for a Q7.9 fixed point number
#define Q7_9						Q(7, 9)

// Macro: Q7_9_TYPE
// Data type of a Q7.9 fixed point number
#define Q7_9_TYPE					i16

// Macro: Q7_9_SET
// Convert source unit value to a Q7.9 fixed point number
#define Q7_9_SET(a)					(i16)((a) * 512)

// Macro: Q7_9_GET
// Convert a Q7.9 fixed point number to source unit value
#define Q7_9_GET(a)					(i16)((a) / 512)

// Macro: Q7_9_FRAC
// Get the fractional part of a Q7.9 fixed point number
#define Q7_9_FRAC(a)				(((a) >= 0) ? (a) & 0x01FF : (a) | 0xFE00)

//.............................................................................
// Q6.10 [-32:31.99]
//.............................................................................

// Macro: Q6_10
// Generate ID for a Q6.10 fixed point number
#define Q6_10						Q(6, 10)

// Macro: Q6_10_TYPE
// Data type of a Q6.10 fixed point number
#define Q6_10_TYPE					i16

// Macro: Q6_10_SET
// Convert source unit value to a Q6.10 fixed point number
#define Q6_10_SET(a)				(i16)((a) * 1024)

// Macro: Q6_10_GET
// Convert a Q6.10 fixed point number to source unit value
#define Q6_10_GET(a)				(i16)((a) / 1024)

// Macro: Q6_10_FRAC
// Get the fractional part of a Q6.10 fixed point number
#define Q6_10_FRAC(a)				(((a) >= 0) ? (a) & 0x03FF : (a) | 0xFC00)

//.............................................................................
// Q5.11 [-16:15.99]
//.............................................................................

// Macro: Q5_11
// Generate ID for a Q5.11 fixed point number
#define Q5_11						Q(5, 11)

// Macro: Q5_11_TYPE
// Data type of a Q5.11 fixed point number
#define Q5_11_TYPE					i16

// Macro: Q5_11_SET
// Convert source unit value to a Q5.11 fixed point number
#define Q5_11_SET(a)				(i16)((a) * 2048)

// Macro: Q5_11_GET
// Convert a Q5.11 fixed point number to source unit value
#define Q5_11_GET(a)				(i16)((a) / 2048)

// Macro: Q5_11_FRAC
// Get the fractional part of a Q5.11 fixed point number
#define Q5_11_FRAC(a)				(((a) >= 0) ? (a) & 0x07FF : (a) | 0xF800)

//.............................................................................
// Q4.12 [-8:7.99]
//.............................................................................

// Macro: Q4_12
// Generate ID for a Q4.12 fixed point number
#define Q4_12						Q(4, 12)

// Macro: Q4_12_TYPE
// Data type of a Q4.12 fixed point number
#define Q4_12_TYPE					i16

// Macro: Q4_12_SET
// Convert source unit value to a Q4.12 fixed point number
#define Q4_12_SET(a)				(i16)((a) * 4096)

// Macro: Q4_12_GET
// Convert a Q4.12 fixed point number to source unit value
#define Q4_12_GET(a)				(i16)((a) / 4096)

// Macro: Q4_12_FRAC
// Get the fractional part of a Q4.12 fixed point number
#define Q4_12_FRAC(a)				(((a) >= 0) ? (a) & 0x0FFF : (a) | 0xF000)

//.............................................................................
// Q3.13 [-4:3.99]
//.............................................................................

// Macro: Q3_13
// Generate ID for a Q3.13 fixed point number
#define Q3_13						Q(3, 13)

// Macro: Q3_13_TYPE
// Data type of a Q3.13 fixed point number
#define Q3_13_TYPE					i16

// Macro: Q3_13_SET
// Convert source unit value to a Q3.13 fixed point number
#define Q3_13_SET(a)				(i16)((a) * 8192)

// Macro: Q3_13_GET
// Convert a Q3.13 fixed point number to source unit value
#define Q3_13_GET(a)				(i16)((a) / 8192)

// Macro: Q3_13_FRAC
// Get the fractional part of a Q3.13 fixed point number
#define Q3_13_FRAC(a)				(((a) >= 0) ? (a) & 0x1FFF : (a) | 0xE000)

//.............................................................................
// Q2.14 [-2:1.99]
//.............................................................................

// Macro: Q2_14
// Generate ID for a Q2.14 fixed point number
#define Q2_14						Q(2, 14)

// Macro: Q2_14_TYPE
// Data type of a Q2.14 fixed point number
#define Q2_14_TYPE					i16

// Macro: Q2_14_SET
// Convert source unit value to a Q2.14 fixed point number
#define Q2_14_SET(a)				(i16)((a) * 16384)

// Macro: Q2_14_GET
// Convert a Q2.14 fixed point number to source unit value
#define Q2_14_GET(a)				(i16)((a) / 16384)

// Macro: Q2_14_FRAC
// Get the fractional part of a Q2.14 fixed point number
#define Q2_14_FRAC(a)				(((a) >= 0) ? (a) & 0x3FFF : (a) | 0xC000)

//.............................................................................
// Q1.15 [-1:0.99]
//.............................................................................

// Macro: Q1_15
// Generate ID for a Q1.15 fixed point number
#define Q1_15						Q(1, 15)

// Macro: Q1_15_TYPE
// Data type of a Q1.15 fixed point number
#define Q1_15_TYPE					i16

// Macro: Q1_15_SET
// Convert source unit value to a Q1.15 fixed point number
#define Q1_15_SET(a)				(i16)((a) * 32768)

// Macro: Q1_15_GET
// Convert a Q1.15 fixed point number to source unit value
#define Q1_15_GET(a)				(i16)((a) / 32768)

// Macro: Q1_15_FRAC
// Get the fractional part of a Q1.15 fixed point number
#define Q1_15_FRAC(a)				(((a) >= 0) ? (a) & 0x7FFF : (a) | 0x8000)

// Function: QMN_Set16
// Convert source unit 16-bit value to a Qm.n fixed point number
//
// Parameters:
//   q - Fixed point number ID
//   a - Source unit value
//
// Return:
//   Qm.n fixed point number
inline i16 QMN_Set16(u16 q, i16 a) { return a * (i16)(1 << (q & 0x00FF)); }

// Function: QMN_Get16
// Convert a Qm.n fixed point number to source unit 16-bit value
//
// Parameters:
//   q - Fixed point number ID
//   a - Qm.n fixed point number
//
// Return:
//   Source unit value
inline i16 QMN_Get16(u16 q, i16 a) { return a / (i16)(1 << (q & 0x00FF)); }
