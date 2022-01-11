rem -- Build ayFX data --
..\..\..\tools\CMSXbin\CMSXbin.exe datasrc\ayfx_bank.afb  -ad -ascii
..\..\..\tools\CMSXbin\CMSXbin.exe datasrc\ayfx_fx017.afx -ad -ascii
..\..\..\tools\CMSXbin\CMSXbin.exe datasrc\ayfx_fx019.afx -ad -ascii

rem -- Build VGM data --
if not exist vgm md vgm
for %%I in (datasrc\vgm\*.vgm) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -t g_VGM_%%~nI -ad  -o vgm\vgm_%%~nI.h

rem -- Build Arkos data --
if not exist arkos md arkos
for %%I in (datasrc\arkos\*.akg) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -t g_AKG_%%~nI -ad -o arkos\akg_%%~nI.h
for %%I in (datasrc\arkos\*.akm) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -t g_AKM_%%~nI -ad -o arkos\akm_%%~nI.h
for %%I in (datasrc\arkos\*.aky) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -t g_AKY_%%~nI -ad -o arkos\aky_%%~nI.h
for %%I in (datasrc\arkos\*.akl) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -t g_AKL_%%~nI -ad -o arkos\akl_%%~nI.h

rem -- Build pcmenc data --
REM if not exist pcm md pcm
REM for %%I in (datasrc\pcm\*.wav) do ..\..\..\tools\Audio\pcmenc\pcmenc.exe %%I
REM for %%I in (datasrc\pcm\*.bin) do ..\..\..\tools\CMSXbin\CMSXbin.exe %%I -ad -o pcm\pcm_%%~nI.h

rem -- Build CMSXimg data --
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_bmp_8b.h    -pos 16 32  -size 16 16 -num 1 1  -name g_DataBmp8b    -trans 0x8468a1 -bpc 8
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_bmp_4b.h    -pos 16 32  -size 16 16 -num 1 1  -name g_DataBmp4b    -trans 0x8468a1 -bpc 4 -pal msx1
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_bmp_2b.h    -pos 16 32  -size 16 16 -num 1 1  -name g_DataBmp2b    -trans 0x8468a1 -bpc 2 -pal custom
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_sprt_8.h    -pos 0 144  -size 8 8   -num 32 3 -name g_DataSprt8    -trans 0x8468a1 -bpc 1
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_sprt_16.h   -pos 0 80   -size 8 8   -num 12 4 -name g_DataSprt16   -trans 0x8468a1 -bpc 1
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_sprt_16or.h -pos 128 80 -size 8 8   -num 12 4 -name g_DataSprt16or -trans 0x8468a1 -bpc 1
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out data_sprt_16il.h -pos 0 160  -size 8 8   -num 12 8 -name g_DataSprt16il -trans 0x8468a1 -bpc 1

..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_no    -trans 0x8468a1 -bpc 8
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c16   -trans 0x8468a1 -bpc 8 -compress crop16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl16  -trans 0x8468a1 -bpc 8 -compress cropline16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c32   -trans 0x8468a1 -bpc 8 -compress crop32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl32  -trans 0x8468a1 -bpc 8 -compress cropline32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_c256  -trans 0x8468a1 -bpc 8 -compress crop256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_cl256 -trans 0x8468a1 -bpc 8 -compress cropline256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle0  -trans 0x8468a1 -bpc 8 -compress rle0
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle4  -trans 0x8468a1 -bpc 8 -compress rle4
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 8b\data_bmp_8b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp8b_rle8  -trans 0x8468a1 -bpc 8 -compress rle8

..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_no    -trans 0x8468a1 -bpc 4
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c16   -trans 0x8468a1 -bpc 4 -compress crop16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl16  -trans 0x8468a1 -bpc 4 -compress cropline16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c32   -trans 0x8468a1 -bpc 4 -compress crop32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl32  -trans 0x8468a1 -bpc 4 -compress cropline32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_c256  -trans 0x8468a1 -bpc 4 -compress crop256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_cl256 -trans 0x8468a1 -bpc 4 -compress cropline256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle0  -trans 0x8468a1 -bpc 4 -compress rle0
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle4  -trans 0x8468a1 -bpc 4 -compress rle4
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 4b\data_bmp_4b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp4b_rle8  -trans 0x8468a1 -bpc 4 -compress rle8

..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_no    -trans 0x8468a1 -bpc 2
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c16   -trans 0x8468a1 -bpc 2 -compress crop16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl16  -trans 0x8468a1 -bpc 2 -compress cropline16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c32   -trans 0x8468a1 -bpc 2 -compress crop32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl32  -trans 0x8468a1 -bpc 2 -compress cropline32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_c256  -trans 0x8468a1 -bpc 2 -compress crop256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_cl256 -trans 0x8468a1 -bpc 2 -compress cropline256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle0  -trans 0x8468a1 -bpc 2 -compress rle0
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle4  -trans 0x8468a1 -bpc 2 -compress rle4
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 2b\data_bmp_2b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp2b_rle8  -trans 0x8468a1 -bpc 2 -compress rle8

..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_no.h    -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_no    -trans 0x8468a1 -bpc 1
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_c16.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c16   -trans 0x8468a1 -bpc 1 -compress crop16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_cl16.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl16  -trans 0x8468a1 -bpc 1 -compress cropline16
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_c32.h   -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c32   -trans 0x8468a1 -bpc 1 -compress crop32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_cl32.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl32  -trans 0x8468a1 -bpc 1 -compress cropline32
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_c256.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_c256  -trans 0x8468a1 -bpc 1 -compress crop256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_cl256.h -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_cl256 -trans 0x8468a1 -bpc 1 -compress cropline256
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_rle0.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle0  -trans 0x8468a1 -bpc 1 -compress rle0
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_rle4.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle4  -trans 0x8468a1 -bpc 1 -compress rle4
..\..\..\tools\CMSXimg\CMSXimg.exe datasrc\data.png -copy datasrc\license.txt -out 1b\data_bmp_1b_rle8.h  -pos 16 0 -size 16 16 -num 1 1 -name g_DataBmp1b_rle8  -trans 0x8468a1 -bpc 1 -compress rle8

pause