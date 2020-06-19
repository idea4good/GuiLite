#ifndef GUILITE_CORE_INCLUDE_RECT_H
#define GUILITE_CORE_INCLUDE_RECT_H

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

class c_rect
{
public:
	c_rect(){Empty();}
	c_rect(int left, int top, int right, int bottom){m_left = left;m_top = top;m_right = right;m_bottom = bottom;};
	void SetRect(int Left, int Top, int Right, int Bottom)
	{
		m_left = MIN(Left, Right);
		m_top = MIN(Top, Bottom);
		m_right = MAX(Left, Right);
		m_bottom = MAX(Top, Bottom);
	}
	c_rect(const c_rect& rect)
	{
		SetRect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
	}
	void Empty()
	{
		m_left = m_top = m_right = m_bottom = -1;
	}
	int IsEmpty() const
	{
		return m_top == m_bottom || m_left == m_right;
	}
	bool PtInRect(int x, int y) const
	{
		return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
	}
	int operator==(const c_rect& rect) const
	{
		return (m_left == rect.m_left) && (m_top == rect.m_top) &&
			(m_right == rect.m_right) && (m_bottom == rect.m_bottom);
	}
	int Width() const   {return m_right - m_left + 1;}
	int Height() const {return m_bottom - m_top + 1;}

	int	    m_left;
	int     m_top;
	int     m_right;
	int     m_bottom;
};

#endif
