@echo off
setlocal EnableDelayedExpansion

:: Audio
set BuildArkos=0
set BuildTrilo=0
set BuildWYZ=0
set BuildayFX=0
set BuildVGM=0
set BuildlVGM=0
set BuildPCMEnc=0
set BuildPCMPlay=0
:: Image
set BuildBitmap=1
set BuildImage=0
set BuildCompress=0
set BuildTile=0
set BuildV9990=1
:: Misc
set BuildZip=0

:: Path
set Tools=..\..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set AudioMisc=%Tools%\audio\misc
set Dest=..\content
set FullPath=%~d0%~p0

if not exist %Dest% md %Dest%

::=============================================================================
:: AUDIO
::=============================================================================

::-----------------------------------------------------------------------------
:: Build Arkos data
if %BuildArkos%==1 (
	echo ----------------------------------------
	echo Building Arkos data...
	if not exist %Dest%\arkos md %Dest%\arkos
	for %%I in (arkos\*.akg) do ( echo %%I & %MSXtk%\MSXbin.exe %%I -t g_AKG_%%~nI -ad -o %Dest%\arkos\akg_%%~nI.h )
	for %%I in (arkos\*.akm) do ( echo %%I & %MSXtk%\MSXbin.exe %%I -t g_AKM_%%~nI -ad -o %Dest%\arkos\akm_%%~nI.h )
	for %%I in (arkos\*.aky) do ( echo %%I & %MSXtk%\MSXbin.exe %%I -t g_AKY_%%~nI -ad -o %Dest%\arkos\aky_%%~nI.h )
	for %%I in (arkos\*.akl) do ( echo %%I & %MSXtk%\MSXbin.exe %%I -t g_AKL_%%~nI -ad -o %Dest%\arkos\akl_%%~nI.h )
	for %%I in (arkos\*.akx) do ( echo %%I & %MSXtk%\MSXbin.exe %%I -t g_AKX_%%~nI -ad -o %Dest%\arkos\akx_%%~nI.h )
	for %%I in (arkos\*.asm) do ( echo %%I & %Tools%\audio\misc\rxrepl.exe -f %%I -o %Dest%\arkos\akg_%%~nI.asm -s "db \"AT20\"" -r "db 65, 84, 50, 48 ; AT20" -s "db" -r ".db" -s "dw" -r ".dw" -s "^[A-Za-z0-9_]+" -r "\0:" )
)

::-----------------------------------------------------------------------------
:: Build Trilo data
if %BuildTrilo%==1 (
	echo ----------------------------------------
	echo Building Trilo music data...
	if not exist %Dest%\trilo md %Dest%\trilo
	if not exist %Dest%\trilo\out md %Dest%\trilo\out
	for %%I in (trilo\*.tmu) do (
		echo ---- Compiling TMU file %%~nI...
		%Tools%\audio\trilo\tmucompile.exe %%I %Dest%\trilo\out\tmu_%%~nI.asm
		%Tools%\audio\misc\rxrepl -f %Dest%\trilo\out\tmu_%%~nI.asm -o %Dest%\trilo\tmu_%%~nI.asm -s "\." -r "" -s "\$" -r "0x" -s "db" -r ".db" -s "0x.db" -r "0xdb" -s "dw" -r ".dw" -s "waveform_" -r "%%~nI_waveform_" -s "instrument_" -r "%%~nI_instrument_" -s "track_" -r "%%~nI_track_" -s "restart" -r "%%~nI_restart" -s "customvoice_" -r "%%~nI_customvoice_" -s "drummacro_" -r "%%~nI_drummacro_" 
		echo.
	)
	
	echo Building Trilo SFX data...
	%MSXtk%\MSXbin.exe trilo\SFX_TEST.afb -ad -o %Dest%\trilo\SFX_TEST.h
	copy trilo\SFX_TEST_WT.asm %Dest%\trilo\SFX_TEST_WT.asm
)

::-----------------------------------------------------------------------------
:: Build WYZ data
if %BuildWYZ%==1 (
	echo ----------------------------------------
	echo Building WYZ data...
	if not exist %Dest%\wyz md %Dest%\wyz
	for %%I in (wyz\*.mus) do call %AudioMisc%\wyz_merge.bat %%I %Dest%\wyz content\wyz
)

::-----------------------------------------------------------------------------
:: Build ayFX data
if %BuildayFX%==1 (
	echo ----------------------------------------
	echo Building ayFX data...
	if not exist %Dest%\ayfx md %Dest%\ayfx
	%MSXtk%\MSXbin.exe ayfx\ayfx_bank.afb  -ad -ascii -o %Dest%\ayfx\ayfx_bank.h
	%MSXtk%\MSXbin.exe ayfx\ayfx_fx017.afx -ad -ascii -o %Dest%\ayfx\ayfx_fx017.h
	%MSXtk%\MSXbin.exe ayfx\ayfx_fx019.afx -ad -ascii -o %Dest%\ayfx\ayfx_fx019.h
)

::-----------------------------------------------------------------------------
:: Build VGM data
if %BuildVGM%==1 (
	echo ----------------------------------------
	echo Building VGM data...
	if not exist %Dest%\vgm md %Dest%\vgm
	for %%I in (vgm\*.vgm) do (
		echo Converting %%I...
		%MSXtk%\MSXbin.exe %%I -t g_VGM_%%~nI -ad -o %Dest%\vgm\vgm_%%~nI.h
	)
)

::-----------------------------------------------------------------------------
:: Build lVGM data
if %BuildlVGM%==1 (
	echo ----------------------------------------
	echo Building lVGM data...
	if not exist %Dest%\lvgm md %Dest%\lvgm
	for %%I in (vgm\*.vgm) do (
		echo Converting %%I...
		%MSXtk%\MSXzip.exe %%I -t g_lVGM_%%~nI -ad -lVGM --split 16K --freq 60 -c   -o %Dest%\lvgm\lvgm_%%~nI.h
		%MSXtk%\MSXzip.exe %%I -t g_lVGM_%%~nI -ad -lVGM --split 16K --freq 60 -bin -o %Dest%\lvgm\lvgm_%%~nI.bin
	)

	copy /Y %Dest%\lvgm\lvgm_psg_ds4_03.bin        %Dest%\lvgm\ds4.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_galious_05.bin    %Dest%\lvgm\galious.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_goemon_07.bin     %Dest%\lvgm\goemon.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_metalgear_05.bin  %Dest%\lvgm\mg.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_honotori_09.bin   %Dest%\lvgm\honotori.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_penguin_05.bin    %Dest%\lvgm\penguin.lvm
	copy /Y %Dest%\lvgm\lvgm_psg_yureikun_02.bin   %Dest%\lvgm\yureikun.lvm
	copy /Y %Dest%\lvgm\lvgm_mm_ff_03.bin          %Dest%\lvgm\ff.lvm
	copy /Y %Dest%\lvgm\lvgm_mm_laydock2_01.bin    %Dest%\lvgm\laydock2.lvm
	copy /Y %Dest%\lvgm\lvgm_mm_undeadline_03.bin  %Dest%\lvgm\undeadli.lvm
	copy /Y %Dest%\lvgm\lvgm_scc_f1spirit_01.bin   %Dest%\lvgm\f1spirit.lvm
	copy /Y %Dest%\lvgm\lvgm_scc_salamander_02.bin %Dest%\lvgm\salamand.lvm
	copy /Y %Dest%\lvgm\lvgm_scc_manbow_03.bin     %Dest%\lvgm\manbow.lvm
	copy /Y %Dest%\lvgm\lvgm_scc_metagear2_10.bin  %Dest%\lvgm\mg2.lvm
	copy /Y %Dest%\lvgm\lvgm_ma_proyakyu_10.bin    %Dest%\lvgm\proyakyu.lvm
	copy /Y %Dest%\lvgm\lvgm_ma_xevious_02.bin     %Dest%\lvgm\xevious.lvm
)

::-----------------------------------------------------------------------------
:: Build pcmenc data
if %BuildPCMEnc%==1 (
	echo ----------------------------------------
	echo Building PCM-encoder data...
	if not exist %Dest%\pcm md %Dest%\pcm
	::---- 8 KHz
	for %%I in (pcm\*.wav) do %Tools%\audio\pcmenc\pcmenc.exe -dt1 31 -dt2 27 -dt3 388 %%I
	move pcm\*.bin pcm\8K
	for %%I in (pcm\8K\*.bin) do %MSXtk%\MSXbin.exe %%I -t g_%%I_8K -ad -o %Dest%\pcm\pcm_8k_%%~nI.h
	::---- 11 KHz
	for %%I in (pcm\*.wav) do %Tools%\audio\pcmenc\pcmenc.exe -dt1 32 -dt2 27 -dt3 265 %%I
	move pcm\*.bin pcm\11K
	for %%I in (pcm\11K\*.bin) do %MSXtk%\MSXbin.exe %%I -t g_%%I_11K -ad -o %Dest%\pcm\pcm_11k_%%~nI.h
	::---- 22 KHz
	for %%I in (pcm\*.wav) do %Tools%\audio\pcmenc\pcmenc.exe -rto 2 -dt1 156 -dt2 27 -dt3 141 %%I
	move pcm\*.bin pcm\22K
	for %%I in (pcm\22K\*.bin) do %MSXtk%\MSXbin.exe %%I -t g_%%I_22K -ad -o %Dest%\pcm\pcm_22k_%%~nI.h
	::---- 44 KHz
	for %%I in (pcm\*.wav) do %Tools%\audio\pcmenc\pcmenc.exe -p 1 -rto 3 -dt1 73 -dt2 84 -dt3 87 %%I
	move pcm\*.bin pcm\44K
	for %%I in (pcm\44K\*.bin) do %MSXtk%\MSXbin.exe %%I -t g_%%I_44K -ad -o %Dest%\pcm\pcm_44k_%%~nI.h
)

::-----------------------------------------------------------------------------
:: Build pcmplay data
if %BuildPCMPlay%==1 (
	echo ----------------------------------------
	echo Building PCMPlayer data...
	for %%I in (pcm\pcmplay\*.pcm) do %MSXtk%\MSXbin.exe %%I  -o %Dest%\pcm\pp_%%~nI.h
)

::=============================================================================
:: IMAGE
::=============================================================================

::-----------------------------------------------------------------------------
:: Build bitmap data
if %BuildBitmap%==1 (
	echo ----------------------------------------
	echo Building bitmap data...
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format c -out %Dest%\img\data_sc5_msx1.h  -pal msx1
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format c -out %Dest%\img\data_sc5_cust.h  -pal custom
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format c -out %Dest%\img\data_sc5_pal24.h -pal custom --pal24
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format c -out %Dest%\img\data_sc5_input.h -pal input 8 0x000000 0x222222 0x444444 0x666666 0x888888 0xAAAAAA 0xCCCCCC 0xEEEEEE
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 8 --bload -format c -out %Dest%\img\data_sc8.h

	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format bin -out %Dest%\img\data_msx1.sc5  -pal msx1
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format bin -out %Dest%\img\data_cust.sc5  -pal custom
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 --bload -format bin -out %Dest%\img\data_pal24.sc5 -pal custom --pal24
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 8 --bload -format bin -out %Dest%\img\data.sc8

	%MSXtk%\MSXimg.exe img\image00.png -pos 0 0 -size 128 128 -bpc 4 -format bin -out %Dest%\img\data00_sc5.bin -pal msx1
	%MSXtk%\MSXimg.exe img\image00.png -pos 0 0 -size 128 128 -bpc 8 -format bin -out %Dest%\img\data00_sc8.bin

	%MSXtk%\MSXimg.exe img\image10.png -pos 0 0 -size 128 128 -bpc 4 -format bin -out %Dest%\img\data10_sc5.bin -pal msx1
	%MSXtk%\MSXimg.exe img\image10.png -pos 0 0 -size 128 128 -bpc 8 -format bin -out %Dest%\img\data10_sc8.bin
)

::-----------------------------------------------------------------------------
:: Build image data
if %BuildImage%==1 (
	echo ----------------------------------------
	echo Building image data...
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_bmp_8b.h     -pos 16 16  -size 16 16 -num 6 1  -name g_DataBmp8b    -trans 0xFF00FF -bpc 8
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_bmp_4b.h     -pos 16 16  -size 16 16 -num 6 1  -name g_DataBmp4b    -trans 0xFF00FF -bpc 4 -pal msx1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_bmp_2b.h     -pos 16 16  -size 16 16 -num 6 1  -name g_DataBmp2b    -trans 0xFF00FF -bpc 2 -pal custom
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_8.h     -pos 0 144  -size 8 8   -num 32 3 -name g_DataSprt8    -trans 0xFF00FF -bpc 1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_16.h    -pos 0 80   -size 8 8   -num 12 4 -name g_DataSprt16   -trans 0xFF00FF -bpc 1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_16or.h  -pos 128 80 -size 8 8   -num 12 4 -name g_DataSprt16or -trans 0xFF00FF -bpc 1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_16il.h  -pos 0 160  -size 8 8   -num 12 8 -name g_DataSprt16il -trans 0xFF00FF -bpc 1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_layer.h -pos 0 128  -size 16 16 -num 13 1 -name g_DataSprtLayer -mode sprt ^
		-l i16 0 0 1 1 0x311433 ^
		-l i16 0 0 1 1 0xFDF3F1 0xBAD6D2 0x7A9CAB ^
		-l i16 0 0 1 1 0xEB8929 0xC0524B ^
		-l i16 0 0 1 1 0x311433 0xBAD6D2 0x7A9CAB 0xC0524B
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\data_sprt_ball.h -pos 192 160 -size 16 16 -num 3 1 -name g_DataSprtBall -mode sprt ^
		-l i16 0 0 1 1 0xCCC35E 0xDED087 ^
		-l i16 0 0 1 1 0xCCC35E
	%MSXtk%\MSXimg.exe img\data.png -out %Dest%\data_bg.h -pos 128 160 -size 8 8 -num 8 3 -name g_DataBackground -mode sprt -l i8 0 0 1 1 0xFFFDFF
)

::-----------------------------------------------------------------------------
:: Build compressed image data
if %BuildCompress%==1 (
	echo ----------------------------------------
	echo Building compress data...
	if not exist %Dest%\8b md %Dest%\8b
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_no    -trans 0xFF00FF -bpc 8
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c16   -trans 0xFF00FF -bpc 8 -compress crop16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl16  -trans 0xFF00FF -bpc 8 -compress cropline16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c32   -trans 0xFF00FF -bpc 8 -compress crop32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl32  -trans 0xFF00FF -bpc 8 -compress cropline32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c256  -trans 0xFF00FF -bpc 8 -compress crop256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl256 -trans 0xFF00FF -bpc 8 -compress cropline256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle0  -trans 0xFF00FF -bpc 8 -compress rle0
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle4  -trans 0xFF00FF -bpc 8 -compress rle4
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\8b\data_bmp_8b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle8  -trans 0xFF00FF -bpc 8 -compress rle8
	if not exist %Dest%\4b md %Dest%\4b
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_no    -trans 0xFF00FF -bpc 4
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c16   -trans 0xFF00FF -bpc 4 -compress crop16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl16  -trans 0xFF00FF -bpc 4 -compress cropline16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c32   -trans 0xFF00FF -bpc 4 -compress crop32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl32  -trans 0xFF00FF -bpc 4 -compress cropline32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c256  -trans 0xFF00FF -bpc 4 -compress crop256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl256 -trans 0xFF00FF -bpc 4 -compress cropline256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle0  -trans 0xFF00FF -bpc 4 -compress rle0
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle4  -trans 0xFF00FF -bpc 4 -compress rle4
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\4b\data_bmp_4b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle8  -trans 0xFF00FF -bpc 4 -compress rle8
	if not exist %Dest%\2b md %Dest%\2b
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_no    -trans 0xFF00FF -bpc 2
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c16   -trans 0xFF00FF -bpc 2 -compress crop16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl16  -trans 0xFF00FF -bpc 2 -compress cropline16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c32   -trans 0xFF00FF -bpc 2 -compress crop32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl32  -trans 0xFF00FF -bpc 2 -compress cropline32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c256  -trans 0xFF00FF -bpc 2 -compress crop256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl256 -trans 0xFF00FF -bpc 2 -compress cropline256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle0  -trans 0xFF00FF -bpc 2 -compress rle0
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle4  -trans 0xFF00FF -bpc 2 -compress rle4
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\2b\data_bmp_2b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle8  -trans 0xFF00FF -bpc 2 -compress rle8
	if not exist %Dest%\1b md %Dest%\1b
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_no    -trans 0xFF00FF -bpc 1
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c16   -trans 0xFF00FF -bpc 1 -compress crop16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl16  -trans 0xFF00FF -bpc 1 -compress cropline16
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c32   -trans 0xFF00FF -bpc 1 -compress crop32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl32  -trans 0xFF00FF -bpc 1 -compress cropline32
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c256  -trans 0xFF00FF -bpc 1 -compress crop256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl256 -trans 0xFF00FF -bpc 1 -compress cropline256
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle0  -trans 0xFF00FF -bpc 1 -compress rle0
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle4  -trans 0xFF00FF -bpc 1 -compress rle4
	%MSXtk%\MSXimg.exe img\data.png -copy gk.txt -out %Dest%\1b\data_bmp_1b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle8  -trans 0xFF00FF -bpc 1 -compress rle8
)

::-----------------------------------------------------------------------------
:: Build tiles data
if %BuildTile%==1 (
	echo ----------------------------------------
	echo Building tiles data...
	if not exist %Dest%\tile md %Dest%\tile
	%MSXtk%\MSXimg.exe img\city.png -out %Dest%\tile\data_tile_gm2.h -mode gm2 -name g_DataTileGM2 -pos 0 256 -size 192  144 -offset 0
	%MSXtk%\MSXimg.exe img\city.png -out %Dest%\tile\data_map_gm2.h  -mode gm2 -name g_DataMapGM2  -pos 0 320 -size 1024 192 -offset 0
	REM %MSXtk%\MSXimg.exe img\city.png -out %Dest%\tile\data_tile_gm1.h -mode gm1 -name g_DataTileGM1 -pos 0 160 -size 192 144 -offset 0
)

::-----------------------------------------------------------------------------
:: Build V9990 data
if %BuildV9990%==1 (
	echo ----------------------------------------
	echo Building V9990 data...
	if not exist %Dest%\v9990 md %Dest%\v9990
	%MSXtk%\MSXimg.exe img\data.png    -copy gk.txt -out %Dest%\v9990\data_v9_chr.h  -pos 0 128 -size 256 16 -num 1 1 -name g_DataV9Chr  -trans 0xFF00FF -bpc 4 -pal custom --pal24
	%MSXtk%\MSXimg.exe img\data_bg.png -copy gk.txt -out %Dest%\v9990\data_v9_bg.h   -pos 0 0   -size 256 48 -num 1 1 -name g_DataV9BG   -trans 0xFF00FF -bpc 4 -pal custom --pal24
	%MSXtk%\MSXimg.exe img\font16.png  -copy gk.txt -out %Dest%\v9990\data_v9_font.h -pos 0 8   -size 256 24 -num 1 1 -name g_DataV9Font -trans 0xFF00FF -bpc 4 -pal custom --pal24
)




::=============================================================================
:: MISC
::=============================================================================

::-----------------------------------------------------------------------------
:: Generate Zip data
if %BuildZip%==1 (
	echo ----------------------------------------
	echo Building Zip data...
	if not exist %Dest%\zip md %Dest%\zip

	echo ---- No compression ----
	%MSXtk%\MSXimg.exe img\image10.png -pos 0 0 -size 128 128 -bpc 4 -format bin -out %Dest%\zip\data10.bin -pal msx1
	%MSXtk%\MSXimg.exe img\image01.png -pos 0 0 -size 256 192 -bpc 4 -format bin -out %Dest%\zip\data01.bin

	echo ---- RLEp compression ----
	%MSXtk%\MSXzip.exe %Dest%\zip\data10.bin -bin -rlep -def auto -incdef -o %Dest%\zip\data10.rlep
	%MSXtk%\MSXzip.exe %Dest%\zip\data01.bin -bin -rlep -def auto -incdef -o %Dest%\zip\data01.rlep

	echo ---- ZX0 compression ----
	%Tools%\compress\ZX0\zx0.exe -f %Dest%\zip\data10.bin %Dest%\zip\data10.zx0
	%Tools%\compress\ZX0\zx0.exe -f %Dest%\zip\data01.bin %Dest%\zip\data01.zx0

	echo ---- Bitbuster compression ----
	copy %Dest%\zip\data10.bin %Tools%\compress\Bitbuster\data10.tmp
	copy %Dest%\zip\data01.bin %Tools%\compress\Bitbuster\data01.tmp
	set PrevCD=!cd!
	cd %Tools%\compress\Bitbuster
	pack.exe data10.tmp
	pack.exe data01.tmp
	cd !PrevCD!
	copy %Tools%\compress\Bitbuster\data10.tmp.pck %Dest%\zip\data10.pck
	copy %Tools%\compress\Bitbuster\data01.tmp.pck %Dest%\zip\data01.pck
	del /Q %Tools%\compress\Bitbuster\*.tmp

	echo ---- Bitbuster 2 compression ----
	%Tools%\compress\Bitbuster\BitBuster2.exe %Dest%\zip\data10.bin -o %Dest%\zip\data10.bb2
	%Tools%\compress\Bitbuster\BitBuster2.exe %Dest%\zip\data01.bin -o %Dest%\zip\data01.bb2

	echo ---- Pletter compression ----
	%Tools%\compress\Pletter\pletter.exe %Dest%\zip\data10.bin %Dest%\zip\data10.pl5
	%Tools%\compress\Pletter\pletter.exe %Dest%\zip\data01.bin %Dest%\zip\data01.pl5
)

pause