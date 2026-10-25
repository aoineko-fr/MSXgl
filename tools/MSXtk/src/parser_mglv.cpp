//_____________________________________________________________________________
//   ▄ ▄  ▄▄▄ ▄▄ ▄ ▄                                                      
//  ██▀█ ▀█▄  ▀█▄▀ ▄  ▄█▄█ ▄▀██                                           
//  ██ █ ▄▄█▀ ██ █ ██ ██ █  ▀██                                           
//__________________________▀▀_________________________________________________
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// available on GitHub (https://github.com/aoineko-fr/MSXimg)
// under CC BY-SA license (https://creativecommons.org/licenses/by-sa/2.0/)

// std
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <array>
#include <assert.h>

// MSXi
#include "parser.h"
#include "MGLV.h"

//#############################################################################
//
// EXPORT MGLV
// 
//#############################################################################

// MGLV data chunk
struct MGLVChunk
{
	MGLV_COMAND Command;
	std::vector<u8> Value;
	u16 Number;
	u16 Size;

	MGLVChunk() : Command(MGLV_CMD_END_VIDEO), Number(0), Size(0) {}
};

//-----------------------------------------------------------------------------
// Count the number of unchanged digits
u32 MGLVGetSkipCount(const std::vector<u8>& curData, const std::vector<u8>& prevData, u32 it)
{
	u32 i = it;
	while ((i < (u32)curData.size()) && (i < (u32)prevData.size()) && (curData[i] == prevData[i]))
	{
		i++;
	}
	return i - it;
}

//-----------------------------------------------------------------------------
// Count the number of identical digits
u32 MGLVGetFillCount(const std::vector<u8>& curData, u32 it)
{
	u8 val = curData[it];
	u32 i = it;
	while ((i < (u32)curData.size()) && (curData[i] == val))
	{
		i++;
	}
	return i - it;
}

//-----------------------------------------------------------------------------
/** Convert multiple images into MGLV video data */
MGLVChunk MGLVGetNextChunk(ExportParameters* param, const std::vector<u8>& curData, const std::vector<u8>& prevData, u32& it, u32 remain)
{
	MGLVChunk chunk;

	//......................................................................... 
	// Check end of frame
	if (it == (u32)curData.size())
	{
		chunk.Command = MGLV_CMD_SKIP_FRAME;
		chunk.Size = 1; // cmd
		return chunk;
	}
		
	//......................................................................... 
	// Check skip chunks
	u32 skip = MGLVGetSkipCount(curData, prevData, it);
	if (skip >= param->mglvConfig.minSkip)
	{
		chunk.Number = skip;
		if (skip < 16)
		{
			chunk.Command = MGLV_CMD_SKIP_4B;
			chunk.Size = 1; // cmd|n
		}
		else if (skip < 256)
		{
			chunk.Command = MGLV_CMD_SKIP_8B;
			chunk.Size = 1 + 1; // cmd + nn
		}
		else if (skip != curData.size())
		{
			chunk.Command = MGLV_CMD_SKIP_16B;
			chunk.Size = 1 + 2; // cmd + nnnn
		}
		else
		{
			chunk.Command = MGLV_CMD_SKIP_FRAME;
			chunk.Size = 1; // cmd
		}

		// Check that chunk don't overflow the current segment
		if (chunk.Size < remain)
		{
			it += skip;
			return chunk;
		}
	}

	//......................................................................... 
	// Check fill chunks
	u32 fill = MGLVGetFillCount(curData, it);
	if (fill >= param->mglvConfig.minFill)
	{
		chunk.Value.resize(1);
		chunk.Value[0] = curData[it];
		chunk.Number = fill;

		if (fill < 16)
		{
			chunk.Command = MGLV_CMD_FILL_4B;
			chunk.Size = 1 + 1; // cmd|n + val
		}
		else if (fill < 256)
		{
			chunk.Command = MGLV_CMD_FILL_8B;
			chunk.Size = 1 + 1 + 1; // cmd + nn + val
		}
		else if (fill != curData.size())
		{
			chunk.Command = MGLV_CMD_FILL_16B;
			chunk.Size = 1 + 2 + 1; // cmd + nnnn + val
		}
		else
		{
			chunk.Command = MGLV_CMD_FILL_FRAME;
			chunk.Size = 1 + 1; // cmd + val
		}

		// Check that chunk don't overflow the current segment
		if (chunk.Size < remain)
		{
			it += fill;
			return chunk;
		}
	}

	//......................................................................... 
	// Check end of segment
	if (remain <= 2)
	{
		chunk.Command = MGLV_CMD_END_SEGMENT;
		chunk.Size = 1; // cmd
		return chunk;
	}

	//......................................................................... 
	// Check copy chunks
	std::vector<u8> data;
	for (u32 i = it; i < (u32)curData.size(); i++)
	{
		if ((i - it) >= remain - 3)
			break;

		if (MGLVGetSkipCount(curData, prevData, i) >= param->mglvConfig.minSkip)
			break;

		if (MGLVGetFillCount(curData, i) >= param->mglvConfig.minFill)
			break;

		data.push_back(curData[i]);
	}
	u32 copy = (u32)data.size();
	
	chunk.Number = copy;
	chunk.Value = data;

	if (copy < 16)
	{
		chunk.Command = MGLV_CMD_COPY_4B;
		chunk.Size = 1 + copy; // cmd|n + val[n]
	}
	else if (copy < 256)
	{
		chunk.Command = MGLV_CMD_COPY_8B;
		chunk.Size = 1 + 1 + copy; // cmd + nn + val[nn]
	}
	else if (copy != curData.size())
	{
		chunk.Command = MGLV_CMD_COPY_16B;
		chunk.Size = 1 + 2 + copy; // cmd + nnnn + val[nnnn]
	}
	else
	{
		chunk.Command = MGLV_CMD_COPY_FRAME;
		chunk.Size = 1 + copy; // cmd + val[]
	}

	it += (u32)data.size();
	return chunk;
}

//-----------------------------------------------------------------------------
// Convert multiple images into MGLV video data
bool ExportMGLV(ExportParameters* param, ExporterInterface* exp)
{
	// Individual image exporter parameters
	ExportParameters imgParam = *param;
	imgParam.mode = MODE_Bitmap;

	u32 segIdx = 0; // current segment index
	u32 segRemain = param->mglvConfig.segmentSize * 1024; // current segment index

	exp->WriteTableBegin(TABLE_U8, param->tabName, "MGLV commands buffer");

	// Write header
	MGLV_Header head;
	memcpy(&head.Sign, "MGV", 3);
	head.Flag = MGLV_VERSION;
	if (param->mglvConfig.segmentSize == 16)
		head.Flag |= MGLV_SEGMENT_16K;
	head.Image = MGLV_SCR_MODE_BITMAP | MGLV_SCR_WIDTH_256 | MGLV_SCR_BPP_4;
	head.Replay = param->mglvConfig.frameSkip;
	if (param->mglvConfig.isLooping)
		head.Replay |= MGLV_LOOP;
	if (param->mglvConfig.freq == 60)
		head.Replay |= MGLV_FREQ_60HZ;
	head.Width = (head.Image & MGLV_SCR_WIDTH_512) ? param->mglvConfig.width / 2 - 1 : param->mglvConfig.width - 1;
	head.Height = (u8)param->mglvConfig.height;
	switch (param->mglvConfig.headerMode)
	{
	case MGLV_HEADER_NONE:
		break;
	case MGLV_HEADER_SHORT:
		exp->WriteBytesList(std::vector<u8>({ 'M', 'G', 'V' }), "Signature");
		exp->Write1ByteLine(head.Flag, "Flag");
		segRemain -= 4;
		break;
	case MGLV_HEADER_FULL:
		exp->WriteBytesList(std::vector<u8>({ 'M', 'G', 'V' }), "Signature");
		exp->Write1ByteLine(head.Flag | MGLV_IMAGE_HEADER, "Flag");
		exp->Write1ByteLine(head.Image, "Image");
		exp->Write1ByteLine(head.Replay, "Replay");
		exp->Write1ByteLine(head.Width, "Width");
		exp->Write1ByteLine(head.Height, "Height");
		segRemain -= sizeof(MGLV_Header);
		break;
	};

	// Parse all video files
	bool bStart = false;
	u32 i = 0;
	std::vector<u8> prevData;
	while (true) // Parse all images
	{
		// Check if file exists
		std::string filename = MSX::Format("%s/%04d.png", param->inFile.c_str(), i);
		if (MSX::File::Exists(filename))
		{
			printf("Converting %s...\n", filename.c_str());
			exp->WriteCommentLine(MSX::Format("Image %i", i));

			if (!bStart) // First image
			{
				bStart = true;
			}
			else // 
			{
			}

			// Get data from source image
			imgParam.inFile = filename;
			ExporterBinNoSave curImg(param->format, &imgParam); // convert image to binary
			ExportBitmap(&imgParam, &curImg);
			std::vector<u8> curData = curImg.GetData();

			u32 imgIt = 0;

			// Export image to video data
			while (1)
			{
				MGLVChunk chunk = MGLVGetNextChunk(param, curData, prevData, imgIt, segRemain);
				switch (chunk.Command)
				{
				//---- Events ----//
				case MGLV_CMD_END_VIDEO: // End of data
					exp->Write1ByteLine(chunk.Command, "END_VIDEO");
					break;
				case MGLV_CMD_END_SEGMENT: // End of segment(increment segment index and reset data pointer)
					exp->Write1ByteLine(chunk.Command, "END_SEGMENT");
					break;
				case MGLV_CMD_END_LINE: // End of line
					exp->Write1ByteLine(chunk.Command, "END_LINE");
					break;
				case MGLV_CMD_END_TABLE: // End table and switch to next one
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Value[0] << 4), "END_TABLE");
					break;
				//---- Skip ----//
				case MGLV_CMD_SKIP_4B: // Skip n bytes(1 - 15)
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("SKIP_4B (%i)", chunk.Number));
					break;
				case MGLV_CMD_SKIP_8B: // Skip nn bytes(1 - 255)
					exp->Write1ByteLine(chunk.Command, MSX::Format("SKIP_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					break;
				case MGLV_CMD_SKIP_16B: // Skip nnnn bytes(1 - 65535)
					exp->Write1ByteLine(chunk.Command, MSX::Format("SKIP_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					break;
				case MGLV_CMD_SKIP_FRAME: // Skip a frame / End of frame
					exp->Write1ByteLine(chunk.Command, "SKIP_FRAME");
					break;
				//---- Fill ----//
				case MGLV_CMD_FILL_4B: // Fill n bytes(1 - 15) with vv value
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("FILL_4B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_8B: // Fill nn bytes(1 - 255) with vv value
					exp->Write1ByteLine(chunk.Command, MSX::Format("FILL_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_16B: // Fill nnnn bytes(1 - 65535) with vv value
					exp->Write1ByteLine(chunk.Command, MSX::Format("FILL_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				case MGLV_CMD_FILL_FRAME: // Fill a full frame with vv value
					exp->Write1ByteLine(chunk.Command, "FILL_FRAME");
					exp->Write1ByteLine((u8)chunk.Value[0]);
					break;
				//---- Copy ----//
				case MGLV_CMD_COPY_4B: // Copy n bytes(1 - 15) from vv[n] data table
					exp->Write1ByteLine(chunk.Command + (u8)(chunk.Number << 4), MSX::Format("COPY_4B (%i)", chunk.Number));
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_8B: // Copy nn bytes(1 - 255) from vv[nn] data table
					exp->Write1ByteLine(chunk.Command, MSX::Format("COPY_8B (%i)", chunk.Number));
					exp->Write1ByteLine((u8)chunk.Number);
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_16B: // Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
					exp->Write1ByteLine(chunk.Command, MSX::Format("COPY_16B (%i)", chunk.Number));
					exp->Write1WordLine((u16)chunk.Number);
					exp->WriteBytesList(chunk.Value);
					break;
				case MGLV_CMD_COPY_FRAME: // Copy a full frame from data table (raw frame)
					exp->Write1ByteLine(chunk.Command, "COPY_FRAME");
					exp->WriteBytesList(chunk.Value);
					break;
				default:
					break;
				}

				segRemain -= chunk.Size;
				assert(segRemain >= 0);
				if (segRemain == 0)
				{
					segRemain = param->mglvConfig.segmentSize * 1024;
					segIdx++;
					exp->WriteCommentLine(MSX::Format("Switch to segment +%i (%i)", segIdx, exp->GetTotalBytes()));
				}

				if (chunk.Command == MGLV_CMD_END_FRAME)
					break;
			}
			prevData = curImg.GetData();
		}
		else
		{
			if (bStart) // Reached last file
			{
				break;
			}
		}
		i++;
	}

	exp->Write1ByteLine(MGLV_CMD_END_VIDEO, "END_VIDEO");
	exp->WriteTableEnd();

	bool bSaved = exp->Export();
	return bSaved;
}
