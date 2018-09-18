#include "core_include/api.h"
#include "core_include/resource.h"
#include "core_include/rect.h"
#include "core_include/word.h"
#include "core_include/surface.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/table.h"


void c_table::pre_create_wnd()
{
	m_style = GLT_ATTR_VISIBLE | (~GLT_ATTR_FOCUS);
}

int c_table::set_item(int row, int col, char* str, unsigned int with_bg_color)
{
	draw_item( row, col, str, with_bg_color);
	return 1;
}

void c_table::draw_item(int row, int col, const char* str, unsigned int with_bg_color)
{
	c_rect rect = get_item_rect(row, col);

	unsigned int back_color = 0;
	if (with_bg_color == 0xFFFFFFFF)
	{
		back_color = m_bg_color;
	}
	else
	{
		back_color = with_bg_color;
	}
	fill_rect(rect.m_left+1, rect.m_top+1, rect.m_right-1, rect.m_bottom-1, back_color);

	c_word::draw_string_in_rect(m_surface, m_z_order, str, rect, m_font_type, m_font_color, COLOR_TRANPARENT, m_align_type);
}

void c_table::set_row_height(unsigned int height)
{
	for (unsigned int i = 0; i < m_row_num; i ++)
	{
		m_row_height[i] = height;
	}
}

void c_table::set_col_width(unsigned int width)
{
	for (unsigned int i = 0; i < m_col_num; i ++)
	{
		m_col_width[i] = width;
	}
}

int c_table::set_row_height(unsigned int index, unsigned int height)
{
	if (m_row_num > index)
	{
		m_row_height[index] = height;
		return index;
	}
	return -1;
}

int c_table::set_col_width(unsigned int index, unsigned int width)
{
	if (m_col_num > index)
	{
		m_col_width[index] = width;
		return index;
	}
	return -1;
}

c_rect c_table::get_item_rect(int row, int col)
{
	static c_rect rect;
	if (row >= MAX_ROW_NUM || col >= MAX_COL_NUM)
	{
		return rect;
	}

	unsigned int width = 0;
	unsigned int height = 0;
	for (int i = 0; i < col; i++)
	{
		width += m_col_width[i];
	}
	for (int j = 0; j < row; j++)
	{
		height += m_row_height[j];
	}

	c_rect wRect;
	get_screen_rect(wRect);

	rect.m_left = wRect.m_left + width;
	rect.m_right = rect.m_left + m_col_width[col];
	if (rect.m_right > wRect.m_right)
	{
		rect.m_right = wRect.m_right;
	}
	rect.m_top = wRect.m_top + height;
	rect.m_bottom = rect.m_top + m_row_height[row];
	if (rect.m_bottom > wRect.m_bottom)
	{
		rect.m_bottom = wRect.m_bottom;
	}
	return rect;
}
