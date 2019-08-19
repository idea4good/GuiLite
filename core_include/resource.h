#ifndef  GUILITE_CORE_INCLUDE_RESOURCE_H
#define  GUILITE_CORE_INCLUDE_RESOURCE_H

//BITMAP
typedef struct struct_bitmap_info
{
	unsigned short XSize;
	unsigned short YSize;
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

#endif
