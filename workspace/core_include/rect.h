#ifndef GUILITE_CORE_INCLUDE_RECT_H
#define GUILITE_CORE_INCLUDE_RECT_H

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
	bool PtInRect(int x, int y) const ;
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
