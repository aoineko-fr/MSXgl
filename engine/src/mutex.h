//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄                                                        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀                                                        
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █                                                        
//_____________________________________________________________________________
#pragma once

extern u8 g_Mutex; ///< @note Must be declared somewhere in the application code
#define MUTEX_DATA() u8 g_Mutex

/// Initialize mutex
inline void MutexInit() { g_Mutex = 0; }

/// Lock the given mutex (0-7)
inline void MutexLock(u8 mutex) { g_Mutex |= (1 << mutex); }

// Release the given mutex (0-7)
inline void MutexRelease(u8 mutex) { g_Mutex &= ~(1 << mutex); }

/// Wait for mutex release (0-7)
inline void MutexWait(u8 mutex) { while((g_Mutex & (1 << mutex)) != 0); }

/// Gate for mutex (0-7)
inline bool MutexGate(u8 mutex) { return ((g_Mutex & (1 << mutex)) == 0); }