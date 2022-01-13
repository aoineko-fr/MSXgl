                  
                  DSKTOOL v1.3 by Ricardo Bittencourt
                  Updated by Tony Cruise 2010
                  Updated by Natalia Pujol 2017
                  
---------------------------------------------------------------------------

                             Index

                        1. Introduction
                        2. Syntax
                        3. Examples
                        4. Suggestions
                        5. What's new
                        6. Greetings
                        7. The author

---------------------------------------------------------------------------

1. Introduction

        Hello, world.

        Most of the MSX emulator users know the .DSK file format. It's
a single file containing all the information from an entire floppy disk.
These files are also used by real MSX users to trade programs not in 
MSX-DOS format, like most of the Compile games.

        The usual way to make a .DSK archive is by using a program called
"DCOPY". This program copy the contents of floppy disk to a .DSK archive.
But, what if you want to add files to your .DSK archive?

        Until now, the only way would be copy the entire .DSK to a floppy,
then copy the files to the floppy, and then copy the floppy back to .DSK
format. This is a slow method and requires a temporary floppy disk.

        To make things easy, I made the "DSKTOOL" program. With DSKTOOL,
you can add files to your .DSK archive, without all the slow steps required
in the previous method. With DSKTOOL you can also list the contents of a
.DSK archive and extract or delete files from an archive.

        The current version is compiled to the Win32 platform, compiled
using Visual Studio 2010. 

        The executable and source code are under GNU GPL (support free 
software!!)

---------------------------------------------------------------------------

2. Syntax

        The syntax of DSKTOOL is very similar to the ARJ compressor:

        DSKTOOL command archive [files]

        "command" is one of the four supported commands:

        C n     create a new disk (where 'n' is 360, 720, 1440, 2880)
        I       show floppy boot sector info
        L[H]    list the contents of the archive
        E[H]    extract files from the archive
        A[H]    add files to the archive
        D       delete files from the archive
        F       show file clusters list
        O[H]    get file info for a raw disk offsett
        
	Some commands can use H suffix to change to ADVH Filesystem mode.

        [files] is a list of files. The "*" wildcard is supported.

        If you try to add files to a non-existent archive, DSKTOOL will
create a new archive and initialize the .DSK with a MSX-DOS 1 boot.

        The only type of .DSK supported is the 720kb one (80 tracks, 
9 sectors per track, 2 sides).

---------------------------------------------------------------------------

3. Examples

3.1. List the contents of TALKING.DSK:

        DSKTOOL L TALKING.DSK

3.2. Extract all the .TXT files from AMDTOOLS.DSK:

        DSKTOOL E AMDTOOLS.DSK *.TXT

3.3. Add the game ZANAC to GAMEPACK.DSK          

        DSKTOOL A GAMEPACK.DSK ZANAC.BAS ZANAC*.BIN

3.4. Delete all the .BIN files from ZORAX.DSK

        DSKTOOL D ZORAX.DSK *.BIN

3.5. Show floppy info

        DSKTOOL I BACKUP.DSK

3.6. Show file clusters list

        DSKTOOL F BACKUP.DSK FAIL.TXT

3.7. Get file info for a raw disk offset

        DSKTOOL O BACKUP.DSK 307712

3.8. Create a new disk

        DSKTOOL C 720 NEWDISK.DSK

---------------------------------------------------------------------------

4. Suggestions

        This program does everything I wanted it to do. This means I will
not make any extension to this version (unless I find some bug).
        But, with the source code, you can make improvements to the program.
Some suggestions are:

        4.1. A GUI, maybe like the Norton Commander, or a full Win95 GUI.
        4.2. Support to directories and MSX-DOS 2 disks.
        4.3. Include other types of archives, like DDI or IMG.
        4.4. Port to other platforms (Mac, Amiga, MSX-SCSI, etc.)

        It would be nice if you tell me of any new version.

---------------------------------------------------------------------------

5. What's new

        [1.4]
        - Added support to create/read 360Kb, 720Kb, 1440Kb and 2880Kb disks.
        - Bug fixes
        [1.3]
        - ported to GCC Linux/MinGW
        - support to 180Kb and 360Kb (new images 720Kb only)
        - read of ADVH propietary filesystem
        - added option to show floppy boot sector info (I)
        - added option to show file clusters list (F)
        - added option to get file info for a raw disk offset (O)
        - set date/time when added new files
        - show file attribs in file list
        - improved some functions
        - makefile created
        [1.2]
        - ported to Win32
        - fixed memory leak in add_single_file
        [1.1] 
        - fixed a bug with files greater than 64kb

---------------------------------------------------------------------------

6. Greetings

        All the information needed to make this program was obtained from
the books:

        "Guia do Programador MSX" - by Eduardo A. Barbosa
        "MSX Top Secret" - by Edison A. Pires de Moraes

        Many thanks to the authors!!
        I also want to thanks DJ Delorie, Charles Sandmann and all the 
people behind the DJGPP project. Of course, many thanks to Richard Stallman 
and all the GNU people!! I couldn't forget the people from msxbr-l mailing
list, and remember: MSX still alive!!!

---------------------------------------------------------------------------

7. The author

        If you find any bug, want to make any comment, or have problems 
to understand the source code, send a e-mail to:

        tony.cruise@cruiseresearch.net

        You can also reach me through my home page:

        http://www.electricadventures.net

        Hope you find this program useful.


Tony Cruise

        Github for latest version:
        https://github.com/nataliapc/MSX_devs

NataliaPC (@ishwin74)
