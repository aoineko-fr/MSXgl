//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//_______________________________▀▀____________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)

// FreeImage
#include "FreeImage.h"
// MSXi
#include "image.h"

//-----------------------------------------------------------------------------
// FreeImage interface
//-----------------------------------------------------------------------------

/** Generic image loader
	@param lpszPathName Pointer to the full file name
	@param flag Optional load flag constant
	@return Returns the loaded dib if successful, returns NULL otherwise
*/
FIBITMAP* LoadImage(const char* lpszPathName)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// check the file signature and deduce its format (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(lpszPathName, 0);
	if (fif == FIF_UNKNOWN)
	{
		// no signature ? try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		// ok, let's load the file
		FIBITMAP* dib = FreeImage_Load(fif, lpszPathName);
		// unless a bad file format, we are done !
		return dib;
	}
	return NULL;
}

/** Generic image writer
	@param dib Pointer to the dib to be saved
	@param lpszPathName Pointer to the full file name
	@param flag Optional save flag constant
	@return Returns true if successful, returns false otherwise
*/
bool SaveImage(FIBITMAP* dib, const char* lpszPathName)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	BOOL bSuccess = FALSE;
	if (dib)
	{
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
		if (fif != FIF_UNKNOWN)
		{
			// check that the plugin has sufficient writing and export capabilities ...
			WORD bpp = FreeImage_GetBPP(dib);
			if (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp))
			{
				// ok, we can save the file
				bSuccess = FreeImage_Save(fif, dib, lpszPathName);
				// unless an abnormal bug, we are done !
			}
		}
	}
	return (bSuccess == TRUE) ? true : false;
}