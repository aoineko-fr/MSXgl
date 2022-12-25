//*****************************************************************************
// PROJECT OVERWRITE
//*****************************************************************************

// RAM in slot 0 and install ISR there (0 = false, 1 = true)
InstallRAMISR = true;

// Type of custom ISR (for RAM or ROM)
// - VBLANK		V-blank handler
// - VHBLANK	V-blank and h-blank handler (V9938 or V9958)
// - V9990		v-blank, h-blank and command end handler (V9990)
CustomISR = "VBLANK";