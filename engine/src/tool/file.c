#include "msxgl.h"
#include "dos.h"

//-----------------------------------------------------------------------------
// File copy function using MSX-DOS2 read/write handle routines
bool File_Copy(const c8* src, const c8* dst)
{
	u8 sFD, dFD;
	if((sFD = DOS_OpenHandle(src, O_RDONLY)) == 0)
		return FALSE;
	if((dFD = DOS_CreateHandle(dst, O_WRONLY, 0)) == 0)
		return FALSE;

	u8 err = DOS_ERR_NONE;
	u16 wrc = 0;
	while(1)
	{
		u16 rd = DOS_ReadHandle(sFD, Mem_GetHeapAddress(), 256);
		if(rd == 0xFFFF)
		{
			err = DOS_GetLastError();
			break;
		}
		u16 wr = DOS_WriteHandle(dFD, Mem_GetHeapAddress(), rd);

		// Print_DrawFormat("session:%d Bytes read:%d, wrote:%d\n", wrc++, rd, wr);
	}

	DOS_CloseHandle(sFD);
	DOS_CloseHandle(dFD); 
	return err == DOS_ERR_EOF;
}