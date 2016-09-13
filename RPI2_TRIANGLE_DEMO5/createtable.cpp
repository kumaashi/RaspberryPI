#include <stdio.h>
#include <stdint.h>

//https://github.com/Microsoft/graphics-driver-samples/blob/e494e687f909138d6cf4272858887c6ba5d945e1/render-only-sample/roscommon/Vc4Hw.h#L1014
#define VC4_1KB_SUB_TILE_WIDTH       16
#define VC4_1KB_SUB_TILE_HEIGHT      16
#define VC4_1KB_SUB_TILE_WIDTH_BYTES 64
#define VC4_1KB_SUB_TILE_SIZE_BYTES  1024
#define VC4_4KB_TILE_WIDTH           32
#define VC4_4KB_TILE_HEIGHT          32
#define VC4_4KB_TILE_WIDTH_BYTES     128
#define VC4_4KB_TILE_SIZE_BYTES      4096
#define VC4_MICRO_TILE_WIDTH_BYTES   16
#define VC4_MICRO_TILE_HEIGHT        4
#define VC4_MICRO_TILE_SIZE_BYTES    64

// Form 1k sub-tile block
int index = 0;
uint8_t *Form1kSubTileBlock(uint8_t *pInputBuffer, uint8_t *pOutBuffer, uint32_t rowStride)
{
    // 1k sub-tile block is formed from micro-tiles blocks
    for (uint32_t h = 0; h < VC4_1KB_SUB_TILE_HEIGHT; h += 4)
    {
        uint8_t *currentBufferPos = pInputBuffer + h*rowStride;

        // Process row of 4 micro-tiles blocks
        for (uint32_t w = 0; w < VC4_1KB_SUB_TILE_WIDTH_BYTES; w+= VC4_MICRO_TILE_WIDTH_BYTES)
        {
            uint8_t *microTileOffset = currentBufferPos + w;

            // Process micro-tile block (4x16 bytes)
            for (int t = 0; t < VC4_MICRO_TILE_HEIGHT; t++)
            {
                //memcpy(pOutBuffer, microTileOffset, VC4_MICRO_TILE_WIDTH_BYTES);
            	for(int k = 0 ; k < VC4_MICRO_TILE_WIDTH_BYTES; k+=4) {
            		if((index % 16) == 0) printf("\n");
            		uint32_t idx = (uint32_t)microTileOffset;
            		uint32_t offset = (idx + k) / 4;
            		printf("0x%08X, ", offset);
            		index++;
            	}
                pOutBuffer += VC4_MICRO_TILE_WIDTH_BYTES;
                microTileOffset += rowStride;
            }
        }
    }
    return pOutBuffer;
}

// Form one 4k tile block from pInputBuffer and store in pOutBuffer
uint8_t *Form4kTileBlock(uint8_t *pInputBuffer, uint8_t *pOutBuffer, uint32_t rowStride, uint32_t OddRow)
{
    uint8_t *currentTileOffset = 0;

    if (OddRow)
    {
        // For even rows, process sub-tile blocks in ABCD order, where
        // each sub-tile is stored in memory as follows:
        //
        //  [C  B]
        //  [D  A]
        //

        // Get A block
        currentTileOffset = pInputBuffer + rowStride * VC4_1KB_SUB_TILE_HEIGHT + VC4_1KB_SUB_TILE_WIDTH_BYTES;
        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // Get B block
        currentTileOffset = pInputBuffer + VC4_1KB_SUB_TILE_WIDTH_BYTES;

        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // Get C block
        pOutBuffer = Form1kSubTileBlock(pInputBuffer, pOutBuffer, rowStride);

        // Get D block
        currentTileOffset = pInputBuffer + rowStride * VC4_1KB_SUB_TILE_HEIGHT;
        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // return current position in out buffer
        return pOutBuffer;

    }
    else
    {
        // For even rows, process sub-tile blocks in ABCD order, where
        // each sub-tile is stored in memory as follows:
        //
        //  [A  D]
        //  [B  C]
        //

        // Get A block
        pOutBuffer = Form1kSubTileBlock(pInputBuffer, pOutBuffer, rowStride);

        /// Get B block
        currentTileOffset = pInputBuffer + rowStride * VC4_1KB_SUB_TILE_HEIGHT;
        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // Get C Block
        currentTileOffset = pInputBuffer + rowStride * VC4_1KB_SUB_TILE_HEIGHT + VC4_1KB_SUB_TILE_WIDTH_BYTES;
        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // Get D block
        currentTileOffset = pInputBuffer + VC4_1KB_SUB_TILE_WIDTH_BYTES;
        pOutBuffer = Form1kSubTileBlock(currentTileOffset, pOutBuffer, rowStride);

        // return current position in out buffer
        return pOutBuffer;
    }
}

// Form (CountX * CountY) tile blocks from InputBuffer and store them in OutBuffer
void ConvertBitmapTo4kTileBlocks(uint32_t Width, uint32_t Height, uint8_t *InputBuffer, uint8_t *OutBuffer, uint32_t rowStride)
{
    // [todo] Currently only 32bpp mode is supported
	/*
    uint32_t CountX = this->WidthInTiles();
    uint32_t CountY = this->HeightInTiles();
	*/
    uint32_t CountX = Width  / VC4_4KB_TILE_WIDTH;
    uint32_t CountY = Height / VC4_4KB_TILE_WIDTH;
	

    for (uint32_t k = 0; k < CountY; k++)
    {
        uint32_t oddRow = k & 1;
        if (oddRow)
        {
            // Build 4k blocks from right to left for odd rows
            for (int i = CountX - 1; i >= 0; i--)
            {
                uint8_t *blockStartOffset = InputBuffer + k * rowStride * VC4_4KB_TILE_HEIGHT + i * VC4_4KB_TILE_WIDTH_BYTES;
                OutBuffer = Form4kTileBlock(blockStartOffset, OutBuffer, rowStride, oddRow);
            }
        }
        else
        {
            // Build 4k blocks from left to right for even rows
            for (uint32_t i = 0; i < CountX; i++)
            {
                uint8_t *blockStartOffset = InputBuffer + k * rowStride * VC4_4KB_TILE_HEIGHT + i * VC4_4KB_TILE_WIDTH_BYTES;
                OutBuffer = Form4kTileBlock(blockStartOffset, OutBuffer, rowStride, oddRow);
            }
        }
    }
}

int main() {
	ConvertBitmapTo4kTileBlocks(256, 256, 0, 0, 256 * 4);
}
