#ifndef _RECT_H
#define _RECT_H

#define ALIGN_HCENTER		0x00000000L
#define ALIGN_LEFT			0x01000000L
#define ALIGN_RIGHT			0x02000000L
#define ALIGN_HMASK			0x03000000L

#define ALIGN_VCENTER		0x00000000L
#define ALIGN_TOP			0x00100000L
#define ALIGN_BOTTOM		0x00200000L
#define ALIGN_VMASK			0x00300000L

class c_rect
{
public:
	c_rect(){Empty();}
	c_rect(int left, int top, int right, int bottom){m_left = left;m_top = top;m_right = right;m_bottom = bottom;};
	void SetRect( int left, int top, int right, int bottom);
	c_rect(const c_rect&);
	c_rect& operator=(const c_rect&);
	void Empty();
	void Offset(int x, int y);
	int IsEmpty() const ;
	int PtInRect(int x, int y) const ;
	int operator==(const c_rect& ) const;
	c_rect  operator&(const c_rect& aRect) const;
	int Width() const   {return m_right - m_left + 1;}
	int Height() const {return m_bottom - m_top + 1;}

	int	    m_left;
	int     m_top;
	int     m_right;
	int     m_bottom;
};

#endif
