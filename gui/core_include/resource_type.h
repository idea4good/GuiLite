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

typedef enum
{
	LANGUAGE_BEGIN = 0,
	ENGLISH = LANGUAGE_BEGIN,
	CHINESE,
	LANGUAGE_MAX
}LANGUAGE_TYPE;

typedef struct  struct_multi_lan_strings
{
	unsigned int    index;
	const char*     text[LANGUAGE_MAX];
}MULTI_LAN_STRINGS;

typedef struct  struct_unicode_lan_strings
{
	unsigned int    index;
	const char*    unicodes;
}MULTI_LAN_UNICODES;

#define ADD_STRING( index, english) \
{				\
	index,      \
	{           \
	english,    \
	0			\
	}			\
}

#endif
