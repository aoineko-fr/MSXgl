#
# Profiling utility
#
# Usage:
#
# Start profiling a section with in a,(2ch), end with out (2ch),a. ID in A.
# Start profiling a frame with in a,(2dh), end with out (2dh),a.
# Every time a frame starts, all section times are reset.
#
# Details:
#
# With this utility you can profile the time spent in up to 257 sections of your
# code. To measure a section you need to instrument it with I/O instructions to
# indicate the start and end of the section.
#
# The first 256 sections are instrumented with I/O port 2CH, read with “in” to
# start the section and write with “out” to end it. The section number is
# specified as the MSB of the 16-bit I/O address, a little known Z80 feature
# (see the Z80 manual). With out (n),a and in a,(n) the section number is
# specified in register A, with out (c),r and in r,(c) in register B.
#
# The remaining section is the “frame section”, instrumented with I/O port 2DH.
# It is started with “in” and ended with “out”, but does not need a section
# number. This section is treated as a frame delimiter, every time this section
# starts all section times are reset.
#
# Console commands:
#
# profile_osd - show the on-screen display of the current frame time.
# profile_ms - toggle the OSD values between percentage and milliseconds.
# profile - show the detailed current profiling status in the console.
#
# Legend: the “frame” value indicates the time spent in the last frame, the
# “current” value shows the currently accumulated time, the “count” value shows
# the number of times a section was started, and finally the “balance” value
# shows whether the CPU is currently in a section. If sections are not started
# or ended consistently, this value will ever-increase or ever-decrease.
#
namespace eval profile {
	variable slots [dict create]
	variable frame_start_time 0
	variable frame_total_time 0
	variable width 32
	variable height 8
	variable percent true
	
	debug set_watchpoint read_io 0x2C {} {
		profile::slot_begin [expr $::wp_last_address >> 8] [machine_info time]
	}
	
	debug set_watchpoint write_io 0x2C {} {
		profile::slot_end [expr $::wp_last_address >> 8] [machine_info time]
	}
	
	debug set_watchpoint read_io 0x2D {} {
		profile::frame_begin [machine_info time]
		profile::slot_begin x [machine_info time]
	}
	
	debug set_watchpoint write_io 0x2D {} {
		profile::slot_end x [machine_info time]
	}
	
	proc slot_ensure {slotid} {
		variable slots
		if {![dict exists $slots $slotid]} {
			dict set slots $slotid [dict create total_time 0 start_time -1 count 0 balance 0 frame_time 0]
		}
	}
	
	proc slot_with {slotid body} {
		variable slots
		slot_ensure $slotid
		dict with slots $slotid $body
	}
	
	proc slot_begin {slotid current_time} {
		slot_with $slotid {
			upvar current_time current_time
			if {$balance == 0} {
				set start_time [expr $current_time]
			}
			incr balance
			incr count
		}
	}
	
	proc slot_end {slotid current_time} {
		slot_with $slotid {
			upvar current_time current_time
			if {$balance == 1} {
				set total_time [expr $total_time + $current_time - $start_time]
			}
			incr balance -1
		}
	}
	
	proc slot_flush_frame {slotid current_time} {
		slot_with $slotid {
			upvar current_time current_time
			set frame_time [expr $total_time + ($balance > 0 ? $current_time - $start_time : 0)]
			set total_time 0
			set start_time $current_time
		}
	}
	
	proc frame_begin {current_time} {
		variable slots
		variable frame_start_time
		variable frame_total_time
		set frame_total_time [expr $current_time - $frame_start_time]
		set frame_start_time $current_time
		foreach slotid [dict keys $slots] {
			slot_flush_frame $slotid $current_time
		}
		osd_update
	}
	
	proc osd_update {} {
		if {![osd exists profile]} {
			return
		}
		variable slots
		foreach slotid [dict keys $slots] {
			slot_with $slotid {
				set index [expr {$slotid eq {x} ? 0 : [expr $slotid + 1]}]
				variable width
				variable height
				variable frame_total_time
				variable percent
				
				set fraction [expr $frame_total_time != 0 ? $frame_time / $frame_total_time : 0]
				
				if {![osd exists profile.$slotid]} {
					set rgba [osd_hya [expr $index * 0.14] 0.5 1.0]
					osd create rectangle profile.$slotid -x 0 -y 0 -w $width -h $height -scaled true -clip true -rgba 0x00000088
					osd create rectangle profile.$slotid.bar -x 0 -y 0 -w 0 -h $height -scaled true -rgba $rgba
					osd create text profile.$slotid.text -x 2 -y 1 -size 5 -scaled true -rgba 0xffffffff
				}
				
				osd configure profile.$slotid -x [expr ($index * $height / 240) * $width] -y [expr $index * $height % 240]
				osd configure profile.$slotid.bar -w [expr ($fraction < 0 ? 0 : $fraction > 1 ? 1 : $fraction) * $width]
				
				if {$percent} {
					osd configure profile.$slotid.text -text [format "%s: %00.2f%%" $slotid [expr $fraction * 100]]
				} else {
					osd configure profile.$slotid.text -text [format "%s: %.2f ms" $slotid [expr $frame_time * 1000]]
				}
			}
		}
	}
	
	proc osd_hya {h y a} {
		set h [expr ($h - floor($h)) * 8.0]
		osd_yuva $y [expr $h < 2.0 ? -1.0 : $h < 4.0 ? $h - 3.0 : $h < 6.0 ? 1.0 : 7.0 - $h] \
		            [expr $h < 2.0 ? $h - 1.0 : $h < 4.0 ? 1.0 : $h < 6.0 ? 5.0 - $h : -1.0] $a
	}
	
	proc osd_yuva {y u v a} {
		set r [fraction_to_uint8 [expr $y + 1.28033 * 0.615 * $v]]
		set g [fraction_to_uint8 [expr $y - 0.21482 * 0.436 * $u - 0.38059 * 0.615 * $v]]
		set b [fraction_to_uint8 [expr $y + 2.12798 * 0.436 * $u]]
		set a [fraction_to_uint8 $a]
		expr $r << 24 | $g << 16 | $b << 8 | $a
	}
	
	proc fraction_to_uint8 {value} {
		set value [expr round($value * 255)]
		expr $value > 255 ? 255 : $value < 0 ? 0 : $value
	}
	
	proc get_cpu_freq {} {
		expr {[cpuregs::get_active_cpu] eq {z80}} ? [machine_info z80_freq] : [machine_info r800_freq]
	}
	
	proc profile {} {
		variable slots
		foreach slotid [dict keys $slots] {
			slot_with $slotid {
				variable frame_total_time
				set percentage [expr $frame_total_time != 0 ? $frame_time * 100 / $frame_total_time : 0]
				set current [expr $total_time + ($balance > 0 ? [machine_info time] - $start_time : 0)]
				puts [format {%s :: frame: %.8f (%05.2f%%), current: %.8f, count: %d, balance: %d} \
					$slotid $frame_time $percentage $current $count $balance]
			}
		}
	}
	
	proc profile_osd {} {
		if {![osd exists profile]} {
			osd create rectangle profile
		} else {
			osd destroy profile
		}
		return
	}
	
	proc profile_ms {} {
		variable percent
		set percent [expr !$percent]
		return
	}
	
	namespace export profile
	namespace export profile_osd
	namespace export profile_ms
}

namespace import profile::*

puts "Start Grauw Profiler"