#ifndef GUI_BMP_H
#define GUI_BMP_H

#include <libgui/layout.h>
#include <stddef.h>
#include <stdint.h>

#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3
#define BI_JPEG 4
#define BI_PNG 5

#define BITMAPCOREHEADER 12
#define BITMAPCOREHEADER2 64
#define OS22XBITMAPHEADER 16
#define BITMAPINFOHEADER 40
#define BITMAPV2INFOHEADER 52
#define BITMAPV3INFOHEADER 56
#define BITMAPV4HEADER 108
#define BITMAPV5HEADER 124

struct bmp_header {
	int16_t signature;
	uint32_t file_size;
	uint32_t reserved;
	uint32_t data_offset;
} __attribute__((packed));

/**
 * @brief bmp draw
 * 
 * @param dgraph 
 * @param bmph 
 * @param px 
 * @param py 
 */
void bmp_draw(struct graphic *dgraph, char *bmph, int32_t px, int32_t py);

#endif