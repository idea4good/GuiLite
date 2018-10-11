#ifndef  _RESOURCE_H_
#define  _RESOURCE_H_

//BITMAP
typedef struct struct_bitmap_info
{
	unsigned short XSize;
	unsigned short YSize;
	unsigned short BytesPerLine;
	unsigned short BitsPerPixel;
	const unsigned char* pData; /* Pointer to picture data (indices) */
} BITMAP_INFO;

//FONT
typedef struct struct_lattice
{
	unsigned int			utf8_code;
	unsigned char			width;
	const unsigned char*	p_data;
} LATTICE;

typedef struct struct_font_info
{
	unsigned char	height;
	unsigned int	count;
	LATTICE*		lattice_array;
} FONT_INFO;

//SHAPE
#define INVALID_RGN 0xFFFFFF
#define COLOR_USERDEF GL_RGB(41,49,49)
typedef struct struct_color_rect
{
	int l;
	int t;
	int r;
	int b;
	unsigned int color;
}COLOR_RECT;

#endif
