#include "../core_include/rect.h"
#include "../core_include/api.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

c_rect::c_rect(const c_rect& rect)
{
	SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
}

c_rect& c_rect::operator=(const c_rect& rect)
{
	SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
	return *this;
}

void c_rect::SetRect( int Left, int Top, int Right, int Bottom)
{ 
	m_left = MIN(Left, Right);
	m_top = MIN(Top, Bottom);
	m_right = MAX(Left, Right);
	m_bottom = MAX(Top, Bottom);
}

c_rect  c_rect::operator&(const c_rect& rect) const
{
	c_rect  dst;
	dst.m_left = MAX(m_left, rect.m_left);
	dst.m_top = MAX(m_top, rect.m_top);
	dst.m_right = MIN(m_right, rect.m_right);
	dst.m_bottom = MIN(m_bottom, rect.m_bottom);
	if(dst.m_left >= dst.m_right || dst.m_top >= dst.m_bottom)
		dst.Empty();
	return dst;	
}

void c_rect::Empty()
{
	m_left = m_top = m_right = m_bottom = 0;
}

void c_rect::Offset(int x, int y)
{
	m_left +=x;
	m_right +=x;
	m_top += y;
	m_bottom += y;
}

int c_rect::IsEmpty() const
{
	return m_top == m_bottom || m_left == m_right;
}

bool c_rect::PtInRect(int x, int y) const 
{
	return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
}

int c_rect::operator==(const c_rect& rect) const
{
	return (m_left == rect.m_left) && (m_top==rect.m_top) &&
		(m_right == rect.m_right) && (m_bottom==rect.m_bottom);
}
