@echo on

set MSXimg=..\..\tools\MSXtk\bin\MSXimg
set MSXmath=..\..\tools\MSXtk\bin\MSXmath

set BuildMath=1
set BuildFont=1
set BuildCursor=1

::-----------------------------------------------------------------------------
:: Build precompute math tables
if %BuildMath%==1 (

	echo ---- BUILD PRECOMPUTE MATH TABLES ----

	%MSXmath% -num 32  -bytes 2 -shift 12 sin cos -o ..\content\mathtable\mt_trigo_32.inc
	%MSXmath% -num 64  -bytes 2 -shift 12 sin cos -o ..\content\mathtable\mt_trigo_64.inc
	%MSXmath% -num 128 -bytes 2 -shift 12 sin cos -o ..\content\mathtable\mt_trigo_128.inc
	%MSXmath% -num 256 -bytes 2 -shift 12 sin cos -o ..\content\mathtable\mt_trigo_256.inc
	%MSXmath% -num 360 -bytes 2 -shift 12 sin cos -o ..\content\mathtable\mt_trigo_360.inc

	%MSXmath% -num 64  -bytes 1 -shift 6 sin cos -o ..\content\mathtable\mt_trigo_Q2.6_64.inc
	%MSXmath% -num 64  -bytes 2 -shift 6 sin cos -o ..\content\mathtable\mt_trigo_Q10.6_64.inc

	%MSXmath% -num 32  -bytes 2 -shift 8 tan asin acos atan -o ..\content\mathtable\mt_trigo_ext_32.inc
	%MSXmath% -num 64  -bytes 2 -shift 8 tan asin acos atan -o ..\content\mathtable\mt_trigo_ext_64.inc
	%MSXmath% -num 128 -bytes 2 -shift 8 tan asin acos atan -o ..\content\mathtable\mt_trigo_ext_128.inc
	%MSXmath% -num 256 -bytes 2 -shift 8 tan asin acos atan -o ..\content\mathtable\mt_trigo_ext_256.inc

	%MSXmath% -num 64 -bytes 2 -shift 8 sin  -o ..\content\mathtable\mt_sin_64.inc
	%MSXmath% -num 64 -bytes 2 -shift 8 cos  -o ..\content\mathtable\mt_cos_64.inc
	%MSXmath% -num 64 -bytes 2 -shift 8 tan  -o ..\content\mathtable\mt_tan_64.inc
	%MSXmath% -num 64 -bytes 2 -shift 8 asin -o ..\content\mathtable\mt_asin_64.inc
	%MSXmath% -num 64 -bytes 2 -shift 8 acos -o ..\content\mathtable\mt_acos_64.inc
	%MSXmath% -num 64 -bytes 2 -shift 8 atan -o ..\content\mathtable\mt_atan_64.inc

	%MSXmath% -num 256  -bytes 1 -shift 4 sqrt -o ..\content\mathtable\mt_sqrt_256.inc
	%MSXmath% -num 1024 -bytes 2 -shift 8 sqrt -o ..\content\mathtable\mt_sqrt_1024.inc

	%MSXmath% -num 64  -bytes 1 -shift 6  sq -o ..\content\mathtable\mt_sq_64.inc
	%MSXmath% -num 256 -bytes 2 -shift 12 sq -o ..\content\mathtable\mt_sq_256.inc

	%MSXmath% -num 32  -bytes 1 -shift 4  map 0 100 -o ..\content\mathtable\mt_sq_64.inc
)

::-----------------------------------------------------------------------------
:: Build font data
if %BuildFont%==1 (

	echo ---- BUILD FONT DATA ----

	REM -------- MSXGL FONTS --------
	%MSXimg% font\font_mgl_big1.png     -out ..\content\font\font_mgl_big1.h     -pos 0 0   -size 8 11 -gap 0 5  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Big1     -copy msxgl.txt -skip  -font 8 11 ! _
	%MSXimg% font\font_mgl_curs1.png    -out ..\content\font\font_mgl_curs1.h    -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Curs1    -copy msxgl.txt -skip  -font 8 8 ! ~
	%MSXimg% font\font_mgl_curs1b.png   -out ..\content\font\font_mgl_curs1b.h   -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Curs1B   -copy msxgl.txt -skip  -font 8 8 ! ~
	%MSXimg% font\font_mgl_digit1.png   -out ..\content\font\font_mgl_digit1.h   -pos 0 24  -size 8 8  -gap 0 0  -num 16 3  -trans 0x000000  -bpc 1  -name g_Font_MGL_Digit1   -copy msxgl.txt        -font 6 8 0 _
	%MSXimg% font\font_mgl_digit2.png   -out ..\content\font\font_mgl_digit2.h   -pos 0 24  -size 8 8  -gap 0 0  -num 16 3  -trans 0x000000  -bpc 1  -name g_Font_MGL_Digit2   -copy msxgl.txt        -font 6 8 0 _
	%MSXimg% font\font_mgl_mini1.png    -out ..\content\font\font_mgl_mini1.h    -pos 0 16  -size 8 5  -gap 0 3  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Mini1    -copy msxgl.txt -skip  -font 4 6 ! ~
	%MSXimg% font\font_mgl_mini1b.png   -out ..\content\font\font_mgl_mini1b.h   -pos 0 16  -size 8 5  -gap 0 3  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Mini1B   -copy msxgl.txt -skip  -font 4 6 ! ~
	%MSXimg% font\font_mgl_mini2.png    -out ..\content\font\font_mgl_mini2.h    -pos 0 16  -size 8 6  -gap 0 2  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Mini2    -copy msxgl.txt -skip  -font 5 6 ! ~
	%MSXimg% font\font_mgl_mini3.png    -out ..\content\font\font_mgl_mini3.h    -pos 0 16  -size 8 6  -gap 0 2  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Mini3    -copy msxgl.txt -skip  -font 5 6 ! ~
	%MSXimg% font\font_mgl_neon1.png    -out ..\content\font\font_mgl_neon1.h    -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Neon1    -copy msxgl.txt -skip  -font 8 8 ! ~
	%MSXimg% font\font_mgl_neon1b.png   -out ..\content\font\font_mgl_neon1b.h   -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Neon1B   -copy msxgl.txt -skip  -font 8 8 ! ~
	%MSXimg% font\font_mgl_neon2.png    -out ..\content\font\font_mgl_neon2.h    -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Neon2    -copy msxgl.txt -skip  -font 8 8 ! ~
	%MSXimg% font\font_mgl_rune2.png    -out ..\content\font\font_mgl_rune2.h    -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Rune2    -copy msxgl.txt -skip  -font 8 8 ! _
	%MSXimg% font\font_mgl_rune2b.png   -out ..\content\font\font_mgl_rune2b.h   -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Rune2B   -copy msxgl.txt -skip  -font 8 8 ! _
	%MSXimg% font\font_mgl_std0.png     -out ..\content\font\font_mgl_std0.h     -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Std0     -copy msxgl.txt -skip  -font 6 8 ! ~
	%MSXimg% font\font_mgl_std1.png     -out ..\content\font\font_mgl_std1.h     -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Std1     -copy msxgl.txt -skip  -font 6 8 ! ~
	%MSXimg% font\font_mgl_std2.png     -out ..\content\font\font_mgl_std2.h     -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Std2     -copy msxgl.txt -skip  -font 6 8 ! ~
	%MSXimg% font\font_mgl_std3.png     -out ..\content\font\font_mgl_std3.h     -pos 0 16  -size 8 7  -gap 0 1  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_MGL_Std3     -copy msxgl.txt -skip  -font 6 8 ! }
	%MSXimg% font\font_mgl_symbol1.png  -out ..\content\font\font_mgl_symbol1.h  -pos 0 0   -size 8 8  -gap 0 0  -num 16 14 -trans 0x000000  -bpc 1  -name g_Font_MGL_Symbol1  -copy msxgl.txt        -font 8 8 0x00 0xE0
	%MSXimg% font\font_mgl_sys1.png     -out ..\content\font\font_mgl_sys1.h     -pos 0 16  -size 8 8  -gap 0 0  -num 16 4  -trans 0x000000  -bpc 1  -name g_Font_MGL_Sys1     -copy msxgl.txt -skip  -font 8 8 ! _0
	%MSXimg% font\font_mgl_sample6.png  -out ..\content\font\font_mgl_sample6.h  -pos 0 0   -size 8 8  -gap 0 0  -num 16 12 -trans 0x000000  -bpc 1  -name g_Font_MGL_Sample6  -copy msxgl.txt        -font 6 8 0x00 0xBF
	%MSXimg% font\font_mgl_sample8.png  -out ..\content\font\font_mgl_sample8.h  -pos 0 0   -size 8 8  -gap 0 0  -num 16 12 -trans 0x000000  -bpc 1  -name g_Font_MGL_Sample8  -copy msxgl.txt        -font 8 8 0x00 0xBF
	%MSXimg% font\font_mgl_kana8.png    -out ..\content\font\font_mgl_kana8.h    -pos 0 0   -size 8 8  -gap 0 0  -num 16 16 -trans 0x000000  -bpc 1  -name g_Font_MGL_Kana8    -copy msxgl.txt        -font 8 8 0x00 0xFD

	REM -------- MISC FONTS --------
	%MSXimg% font\font_carwar.png        -out ..\content\font\font_carwar.h        -pos 0 0   -size 8 8  -gap 0 0  -num 16 4  -trans 0xDA48B6  -bpc 1  -name g_Font_Carwar        -copy           -skip  -font 8 8 ! _
	REM %MSXimg% -in font\font_darkrose.png	-out font_darkrose.h    -pos 0 16 -size 8 8  -gap 0 0 -num 16 6  -trans 0x000000 -bpc 1 -name g_Font_Darkrose		-skip -font 8 8 ! ~
	REM %MSXimg% -in font\oxygene.png			-out font_oxygene.h     -pos 0 16 -size 8 8  -gap 0 0 -num 16 6  -trans 0x000000 -bpc 1 -name g_Font_Oxygene		-skip -font 8 8 ! _
	%MSXimg% font\font_room5.png         -out ..\content\font\font_room5.h         -pos 0 16  -size 8 8  -gap 0 0  -num 16 4  -trans 0x000000  -bpc 1  -name g_Font_Room5         -copy msxgl.txt -skip -font 8 8 ! _

	REM -------- SSYTEM FONTS --------
	%MSXimg% font\font_tsm9900.png       -out ..\content\font\font_tsm9900.h       -pos 0 16  -size 8 8  -gap 0 0  -num 16 6  -trans 0x000000  -bpc 1  -name g_Font_TMS9900       -copy           -skip  -font 6 8 ! ~
	REM %MSXimg% -in font\IBMFont.bmp			-out font_ibm.h         -pos 0 0  -size 8 8  -gap 0 0 -num 16 16 -trans 0x040404 -bpc 1 -name g_Font_IBM			-skip -font 8 8 0x01 0xFE
	%MSXimg% font\typo\Apple2.png        -out ..\content\font\font_apple2.h        -pos 0 0   -size 7 8  -gap 0 0  -num 32 3  -trans 0x000000  -bpc 1  -name g_Font_Apple2        -copy           -skip -font 8 8 ! 0x7F
	%MSXimg% font\typo\Atari.png         -out ..\content\font\font_atari.h         -pos 0 0   -size 8 8  -gap 0 0  -num 32 3  -trans 0x005D8E  -bpc 1  -name g_Font_Atari         -copy           -skip -font 8 8 ! 0x7C
	%MSXimg% font\font_ascii_int.png     -out ..\content\font\font_ascii_int.h     -pos 0 0   -size 8 8  -gap 0 0  -num 32 8  -trans 0x000000  -bpc 1  -name g_Font_ASCII_INT     -copy font\ascii.txt  -font 8 8 0x00 0xFE
	%MSXimg% font\font_ascii_jpn.png     -out ..\content\font\font_ascii_jpn.h     -pos 0 0   -size 8 8  -gap 0 0  -num 16 16 -trans 0x000000  -bpc 1  -name g_Font_ASCII_JPN     -copy font\ascii.txt  -font 8 8 0x00 0xFD


	REM -------- GFX FONTS --------
	%MSXimg% font\font_gfx_future1.png   -out ..\content\font\font_gfx_future1.h   -pos 0 0   -size 8 8  -gap 0 0  -num 16 4  -trans 0x000000  -bpc 1  -name g_Font_GFX_Future1   -copy font\gfx.txt -skip  -font 8 8 ! _
	%MSXimg% font\font_gfx_tennis1.png   -out ..\content\font\font_gfx_tennis1.h   -pos 0 0   -size 8 8  -gap 0 0  -num 16 4  -trans 0x000000  -bpc 1  -name g_Font_GFX_Tennis1   -copy font\gfx.txt -skip  -font 8 8 ! _
	%MSXimg% font\font_gfx_tennis2.png   -out ..\content\font\font_gfx_tennis2.h   -pos 0 0   -size 8 8  -gap 0 0  -num 16 4  -trans 0x000000  -bpc 1  -name g_Font_GFX_Tennis2   -copy font\gfx.txt -skip  -font 8 8 ! _

)

::-----------------------------------------------------------------------------
:: Build cursor data
if %BuildCursor%==1 (

	echo ---- BUILD CURSOR DATA ----

	%MSXimg% cursor\cur_mgl1.png        -out ..\content\cursor\cur_mgl1.h        -pos 0 0   -size 8 8  -gap 0 0  -num 16 2  -trans 0x000000  -bpc 1  -name g_Cursor_MGL1       -copy msxgl.txt 

)

pause