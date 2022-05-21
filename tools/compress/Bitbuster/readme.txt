
Bitbuster v1.2 - (c) 2002-2003  Team Bomba
------------------------------------------


Bitbuster is a new set of (de)compression-tools for MSX and PC. It was created because I was getting frustrated by POPCOM; it made crash Bombaman. That's why Bitbuster was created.


Bitbuster vs POPCOM
-------------------
Bitbuster looks a lot like POPCOM, they both use a similar compression scheme. The compression ratio is pretty much the same, sometimes bitbuster is better than popcom and vice versa. Here's a little comparison (files mostly taken from Bombaman). The lengths stated for POPCOM are the lengths of the compressed data without decompression code.

			POPCOM		Bitbuster	Original
bossapp2.mwm		849		815		2752
ccompress.cpp		979		929		2197
cdecompress.cpp		1536		1197		3358
cmatch.cpp		2310		2323		6335
crash.raw		15096		14315		15463
deepb.raw		5334		5254		10085
game.com		4741		4800		8320
level11.ge5		6532		6628		30375
level16.bls		1795		1739		2670
level4.ge5		7869		7720		30375
level5.ge5		8131		8162		30375
multi.cpm		9931		10002		17536
ruins2.mbm		1490		1420		5130
swamps1.mbm		1688		1735		4557
win.mbm			818		779		3193
world1a.mwm		2131		2312		5743
world4b.mwm		3652		3600		9243
			----------  	----------  	----------
			74882		73729		187707



Files in this package
---------------------
-readme.txt - you're reading this now!
-pack.exe - the cruncher for use in windows
-depack.exe - the decruncher for use in windows
-pack\*.* - sourcecode of cruncher (c++)
-depackpc\*.* - sourcecode of decruncher (c++)
-depackz80\depack.gen - source of decruncher (z80)
-depackarm\depack.s - source of decruncher (arm)


How to use Bitbuster packer
--------------------------
To compress a file, simply type the following on the commandline:
pack <sourcefile> [sourcefile1] ...
This will compress all specified files. You can also use wildcards in filenames. The new files will get the extension .pck. 


How to use Bitbuster depacker (executables)
-----------------------------
To decompress a file (on PC), simple type the following on the commandline:
depack <sourcefile> <destfile>
This will decrunch sourcefile and write the original data to destfile.


Stuff to do
-----------
-file i/o error handling
-optimize for speed!
-msx-version of packer, anyone?


Legal information
-----------------
Read license.txt.


Contact info
------------
Please send fanmail to bombaman@generation-msx.nl. You can also send mail to ask questions of give some suggestions :)
Also, visit our site, http://www.bombaman.generation-msx.nl/ and download more cool stuff from us, like games, wallpapers and music!


Version history
---------------
1.2	Bitbuster can be compiled with other compilers than MSVC (thanks kanima!)
	Added support for compressing multiple files at onces (thanks kanima!)
	Speed up compressing process (still too slow though...)
	Speed up Z80 version of decompressor a lot (thanks to GuyveR800 for some tips)
	Added new license (MIT style)
	Smaller executables
	Display execution time
1.1	Added support for wildcards
	Added ARM version of decompressor
1.0	Initial release

