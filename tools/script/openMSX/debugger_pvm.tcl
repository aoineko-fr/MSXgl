# Next generation debugger with OpenMSX
# by Pedro de Medeiros "PVMM", May 7 2024
# https://github.com/pvmm/msx-dev/tree/main/src/debugv2

set debug_mode 0
set pos        0
set addr       0
set ppos       0

proc pause_on {} {
    set use_pause true
    debug set_watchpoint write_io {0x2e} {} {process_ctrl $::wp_last_value}
}

proc pause_off {} {
    set use_pause false
    debug set_watchpoint write_io {0x2e} {} {}
}

proc process_ctrl {{value 0}} {
    global use_pause
    global debug_mode
    switch $value {
        255 {
            if {$use_pause > 0} {
                debug break
            }
        }
        default { set debug_mode $value }
    }
}

proc addr2string {addr} {
    set str ""
    for {set byte [peek $addr]} {$byte > 0} {incr addr; set byte [peek $addr]} {
        append str [format "%c" $byte]
    }
    return $str
}

proc peek32 {addr {m memory}} {
    expr {[peek16 $addr] + 65536 * [peek16 [expr {$addr + 2}] $m]}
}

# formatting commands
proc printf__c   {mod addr} { puts -nonewline stderr [format "%${mod}c"  [peek8  $addr]]; return 2 }
proc printf__s   {mod addr} { puts -nonewline stderr [format "%${mod}s"  [addr2string [peek16 $addr]]]; return 2 }
proc printf__S   {mod addr} { puts -nonewline stderr [format "%${mod}s"  [string toupper [addr2string [peek16 $addr]]]]; return 2 }
proc printf__hhi {mod addr} { puts -nonewline stderr [format "%${mod}hi" [peek8  $addr]]; return 2 }
proc printf__hi  {mod addr} { puts -nonewline stderr [format "%${mod}hi" [peek16 $addr]]; return 2 }
proc printf__i   {mod addr} { return [printf__hi $mod $addr] }
proc printf__hhu {mod addr} { puts -nonewline stderr [format "%${mod}hu" [peek8  $addr]]; return 2 }
proc printf__hu  {mod addr} { puts -nonewline stderr [format "%${mod}hu" [peek16 $addr]]; return 2 }
proc printf__u   {mod addr} { return [printf__hu $mod $addr] }
proc printf__hhx {mod addr} { puts -nonewline stderr [format "%${mod}hx" [peek8  $addr]]; return 2 }
proc printf__hx  {mod addr} { puts -nonewline stderr [format "%${mod}hx" [peek16 $addr]]; return 2 }
proc printf__x   {mod addr} { return [printf__hx $mod $addr] }
proc printf__hhX {mod addr} { puts -nonewline stderr [format "%${mod}hX" [peek8  $addr]]; return 2 }
proc printf__hX  {mod addr} { puts -nonewline stderr [format "%${mod}hX" [peek16 $addr]]; return 2 }
proc printf__X   {mod addr} { return [printf__hX $mod $addr] }
proc printf__hho {mod addr} { puts -nonewline stderr [format "%${mod}ho" [peek8  $addr]]; return 2 }
proc printf__ho  {mod addr} { puts -nonewline stderr [format "%${mod}ho" [peek16 $addr]]; return 2 }
proc printf__o   {mod addr} { return [printf__ho $mod $addr] }
proc printf__hhb {mod addr} { puts -nonewline stderr [format "%${mod}hb" [peek8  $addr]]; return 2 }
proc printf__hb  {mod addr} { puts -nonewline stderr [format "%${mod}hb" [peek16 $addr]]; return 2 }
proc printf__b   {mod addr} { return [printf__hb $mod $addr ] }
proc printf__f   {mod addr} { puts -nonewline stderr [format "%${mod}s"  [parse_basic_float 3 [peek16 $addr]]]; return 4 }
proc printf__lf  {mod addr} { puts -nonewline stderr [format "%${mod}s"  [parse_basic_float 7 [peek16 $addr]]]; return 8 }
proc printf__hf  {mod addr} { puts -nonewline stderr [format "%${mod}s"  [parse_sdcc_float    [peek16 $addr]]]; return 4 }
#proc printf__hhf {mod addr} { puts -nonewline stderr [format "%${mod}s"  [parse_fp_float      [peek16 $addr]]]; return 4 }
proc printf__li  {mod addr} { puts -nonewline stderr [format "%${mod}li" [parse_int32 [peek32 $addr]]]; return 4 }
proc printf__lu  {mod addr} { puts -nonewline stderr [format "%${mod}lu" [peek32 $addr]]; return 4 }
proc printf__lx  {mod addr} { puts -nonewline stderr [format "%${mod}lx" [peek32 $addr]]; return 4 }
proc printf__lX  {mod addr} { puts -nonewline stderr [format "%${mod}lX" [peek32 $addr]]; return 4 }
proc printf__?       {addr} { puts -nonewline stderr [format        "%s" [print_debug_mode [peek16 $addr]]]; return 2 }
proc printf__p   {mod addr} { puts -nonewline stderr [format      "0x%X" [peek16 $addr]]; return 2 }
proc printf__z   {mod addr} { puts stderr "mod=$mod, val=$addr"; return 2 } ;# debug

proc parse_int32 {value} {
    return [expr $value > 2147483647 ? $value - 4294967296 : $value]
}

# MSX-BASIC single is 1-bit signal + 7-bit exponent + (3|7) bytes packed BCD (n*2 digits)
proc parse_basic_float {size addr} {
    set buf "0."
    set tmp [peek8 $addr]
    set signal [expr $tmp & 0x80 ? {"-"} : {"+"}]
    set exponent [expr ($tmp & 0x7f) - 0x40]
    set mantissa [debug read_block memory [expr $addr + 1] $size] ;# read_block returns a string
    for {set b 0} {$b < $size} {incr b} {
        set i [scan [string index $mantissa $b] %c]
        append buf [format %x $i]
    }
    append buf "e$signal$exponent"
    return $buf
}

# format is unknown (even searching in the manual)
proc parse_sdcc_float {value} {
    return "?.????" ;# [format %f $value]
}

# compatibility with old debugging code
proc print_debug_mode {value} {
    global debug_mode
    switch $debug_mode {
        0 { return [format %x $value] }
        1 { return [format %i $value] }
        2 { return [format %b $value] }
        3 { return [format %c [expr $value & 0xff]] }
        default { return "" }
    }
}

# empty lots of variables at once
proc empty-> {args} {
    for {set len 0} {$len < [llength $args]} {incr len} {
        upvar [lindex $args $len] arg; set arg ""
    }
}

proc printf {addr} {
    global ppos

    set tysz  2  ;# default type size
    set neg  ""  ;# negative/positive sign?
    set lpad ""  ;# pad size in characters
    set tdot ""  ;# truncated dot?
    set rpad ""  ;# truncated size in characters
    set tcs  ""  ;# type category suffix
    set raw  ""

    set fmt_addr [peek16 $addr]
    incr addr $tysz

    for {set byte [peek $fmt_addr]} {$byte > 0} {incr fmt_addr; set byte [peek $fmt_addr]} {
        set c [format %c $byte]
        switch $c {
            "%" { if {$ppos eq 1} { set ppos 0; append raw $c } else { incr ppos } }
            "c" { if {$ppos > 0}  { set ppos 0; set cmd "printf__c       {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "S" { if {$ppos > 0}  { set ppos 0; set cmd "printf__S       {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "s" { if {$ppos > 0}  { set ppos 0; set cmd "printf__s       {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "i" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}i {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "d" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}i {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "u" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}u {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "x" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}x {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "X" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}X {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "o" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}o {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "b" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}b {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "f" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}f {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "F" { if {$ppos > 0}  { set ppos 0; set cmd "printf__${tcs}F {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "p" { if {$ppos > 0}  { set ppos 0; set cmd "printf__p       {$neg$lpad$tdot$rpad} $addr"; empty-> neg lpad tdot rpad tcs } else { append raw $c } }
            "?" { if {$ppos > 0}  { set ppos 0; set cmd "printf__? $addr"; empty-> neg lpad tdot rpad tcs; incr addr 2 } else { append raw $c } }
            "h" { if {$ppos > 0}  { append tcs $c; incr ppos; set tysz 2 } else { append raw $c } }
            "l" { if {$ppos > 0}  { append tcs $c; incr ppos; set tysz 4 } else { append raw $c } }
            default {
                if {$ppos > 0} {
                    if {$c eq "-" || $c eq "+"} {
                        append neg $c
                    } elseif {$ppos > 0 && $c eq "."} {
                        append tdot $c
                    } elseif {$ppos > 0 && $byte >= 48 && $byte <= 57} {
                        if {$tdot eq ""} { append lpad $c } else { append rpad $c }
                    }
                    incr ppos
                } else {
                    ;# fall through
                    set ppos 0; append raw $c
                }
            }
        }
        if {$ppos eq 0} {
            if {[info exists cmd]} {
                puts -nonewline stderr $raw
                set raw ""
                incr addr [eval $cmd]
                unset cmd
                set tysz 2 ;# go back to default type size
            } else {
                puts -nonewline stderr $raw; set raw ""
            }
        }
    }
}

proc debug_printf {value} {
    global pos
    global addr

    if {$pos == 1} {
        set addr [expr ($value << 8) + $addr]
        printf $addr
        set addr 0
        incr pos -1
    } else {
        set addr $value
        incr pos
    }
}

# if { [info exists ::env(DEBUG)] && $::env(DEBUG) > 0 } { # removed the need of setting system variable -- Aoineko (May 9 2024)
#    set use_pause $::env(DEBUG)
    set use_pause 1
    #ext debugdevice
    debug set_watchpoint write_io {0x2e} {} {process_ctrl $::wp_last_value}
    debug set_watchpoint write_io {0x2f} {} {debug_printf $::wp_last_value}
# }

# ext debugdevice