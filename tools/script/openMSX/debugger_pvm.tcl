proc DEBUG_HEX   {} { return 0 }
proc DEBUG_INT   {} { return 1 }
proc DEBUG_BIN   {} { return 2 }
proc DEBUG_CHAR  {} { return 3 }
proc DEBUG_PAUSE {} { return 0xff }

set debug_mode 1
set pos 0
set int 0

proc pause_on {} {
    #ext debugdevice
    set use_pause true
    debug set_watchpoint write_io {0x2e} {} {process_input $::wp_last_value}
}

proc pause_off {} {
    #ext debugdevice
    set use_pause false
    debug set_watchpoint write_io {0x2e} {} {}
}

proc process_input {{value 0}} {
    global use_pause
    global debug_mode
    switch $value {
        0xff    { if {$use_pause > 0} { debug break }} 
        default { set debug_mode $value }
    }
}

proc print_input {{value 0}} {
    global debug_mode
    global pos
    global int

    switch $debug_mode {
        0 {
            if {$pos == 1} {
                set int [expr {($value << 8) + $int}]
                puts stderr [format "0x%x" $int]
                set int 0
                set pos 0
            } else {
                set int $value
                set pos 1
            }
        }
        1 {
            if {$pos == 1} {
                set int [expr {($value << 8) + $int}]
                if {$int > 32767} {
                    set int [expr {$int - 65536}]
                }
                puts stderr $int
                set int 0
                set pos 0
            } else {
                set int $value
                set pos 1
            }
        }
        2 {
            if {$pos == 1} {
                set int [expr {($value << 8) + $int}]
                puts stderr [format "0b%b" $int]
                set int 0
                set pos 0
            } else {
                set int $value
                set pos 1
            }
        }
        3 { puts -nonewline stderr [format "%c" $value] }
        default {
            puts stderr "? Unknown debug_mode $debug_mode"
        }
    }
}

if { [info exists ::env(DEBUG)] && $::env(DEBUG) > 0 } {
    set use_pause $::env(DEBUG)
    #ext debugdevice
    debug set_watchpoint write_io {0x2e} {} {process_input $::wp_last_value}
    debug set_watchpoint write_io {0x2f} {} {print_input $::wp_last_value}
}

bind "mouse button2 down" toggle grabinput
plug joyporta mouse
