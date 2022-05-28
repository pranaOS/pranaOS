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

struct dip_bitmapcoreheader {
	uint32_t header_size;	  
	int32_t width_px;		  
	int32_t height_px;		  
	uint16_t num_planes;	  
	uint16_t bits_per_pixel;  
	uint32_t compression;
	uint32_t image_size_bytes;
};

struct dip_bitmapinfoheader	{
	uint32_t header_size;		
	int32_t width_px;			
	int32_t height_px;			
	uint16_t num_planes;		
	uint16_t bits_per_pixel;	
	uint32_t compression;		
	uint32_t image_size_bytes;	
	int32_t x_resolution_ppm;	
	int32_t y_resolution_ppm;	
	uint32_t num_colors;		
	uint32_t important_colors;	
};

struct ciexyz {
	uint32_t ciexyzX;  
	uint32_t ciexyzY;  
	uint32_t ciexyzZ;  
};

struct ciexyztriple {
	struct ciexyz ciexyzRed;	
	struct ciexyz ciexyzGreen;	
	struct ciexyz ciexyzBlue;	
} ciexyztriple;

struct dip_bitmapv4header {
	uint32_t bV4Size;
	int32_t bV4Width;
	int32_t bV4Height;
	uint16_t bV4Planes;
	uint16_t bV4BitCount;
	uint32_t bV4V4Compression;
	uint32_t bV4SizeImage;
	int32_t bV4XPelsPerMeter;
	int32_t bV4YPelsPerMeter;
	uint32_t bV4ClrUsed;
	uint32_t bV4ClrImportant;
	uint32_t bV4RedMask;
	uint32_t bV4GreenMask;
	uint32_t bV4BlueMask;
	uint32_t bV4AlphaMask;
	uint32_t bV4CSType;
	struct ciexyztriple bV4Endpoints;
	uint32_t bV4GammaRed;
	uint32_t bV4GammaGreen;
	uint32_t bV4GammaBlue;
} dip_bitmapv4header;

struct dip_bitmapv5header {
	uint32_t bV5Size;
	int32_t bV5Width;
	int32_t bV5Height;
	uint16_t bV5Planes;
	uint16_t bV5BitCount;
	uint32_t bV5Compression;
	uint32_t bV5SizeImage;
	int32_t bV5XPelsPerMeter;
	int32_t bV5YPelsPerMeter;
	uint32_t bV5ClrUsed;
	uint32_t bV5ClrImportant;
	uint32_t bV5RedMask;
	uint32_t bV5GreenMask;
	uint32_t bV5BlueMask;
	uint32_t bV5AlphaMask;
	uint32_t bV5CSType;
	struct ciexyztriple bV5Endpoints;
	uint32_t bV5GammaRed;
	uint32_t bV5GammaGreen;
	uint32_t bV5GammaBlue;
	uint32_t bV5Intent;
	uint32_t bV5ProfileData;
	uint32_t bV5ProfileSize;
	uint32_t bV5Reserved;
};

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