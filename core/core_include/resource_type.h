#ifndef  _RESOURCE_TYPE_H_
#define  _RESOURCE_TYPE_H_

typedef struct struct_gui_bitmap
{
	unsigned short XSize;
	unsigned short YSize;
	unsigned short BytesPerLine;
	unsigned short BitsPerPixel;
	const unsigned char* pData; /* Pointer to picture data (indices) */
} GUI_BITMAP;

typedef struct 
{
	unsigned char XSize;
	unsigned char XDist;
	unsigned char BytesPerLine;
	void* pData;
} GUI_CHARINFO;

typedef struct 
{
	unsigned short First;         /* first character               */
	unsigned short Last;          /* last character                */
	const GUI_CHARINFO* paCharInfo;    /* address of first character    */
	void* pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef struct struct_gui_font
{
	unsigned char YSize;
	unsigned char YDist;
	unsigned char XMag;
	unsigned char YMag;
	union 
	{
		void  *pFontData;
		const GUI_FONT_PROP* pProp;
	} p;
	unsigned char Baseline;
} GUI_FONT;

typedef struct struct_custom_shape
{
	int l;
	int t;
	int r;
	int b;
	unsigned int color;
}CUSTOM_SHAPE;

#endif
