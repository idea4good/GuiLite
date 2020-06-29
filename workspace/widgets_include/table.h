#ifndef GUILITE_WIDGETS_INCLUDE_TABLE_H
#define GUILITE_WIDGETS_INCLUDE_TABLE_H

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"

#define  MAX_COL_NUM  30
#define  MAX_ROW_NUM  30

class c_table: public c_wnd
{
public:
	void set_sheet_align(unsigned int align_type){ m_align_type = align_type;}
	void set_row_num(unsigned int row_num){ m_row_num = row_num;}
	void set_col_num(unsigned int col_num){ m_col_num = col_num;}
	void set_row_height(unsigned int height)
	{
		for (unsigned int i = 0; i < m_row_num; i++)
		{
			m_row_height[i] = height;
		}
	}
	void set_col_width(unsigned int width)
	{
		for (unsigned int i = 0; i < m_col_num; i++)
		{
			m_col_width[i] = width;
		}
	}
	int set_row_height(unsigned int index, unsigned int height)
	{
		if (m_row_num > index)
		{
			m_row_height[index] = height;
			return index;
		}
		return -1;
	}
	int set_col_width(unsigned int index, unsigned int width)
	{
		if (m_col_num > index)
		{
			m_col_width[index] = width;
			return index;
		}
		return -1;
	}
	void set_item(int row, int col, char* str, unsigned int color)
	{
		draw_item(row, col, str, color);
	}

	unsigned int get_row_num(){ return m_row_num;}
	unsigned int get_col_num(){ return m_col_num;}
	c_rect get_item_rect(int row, int col)
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
protected:
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE);
		m_font_type = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
	}
	void draw_item(int row, int col, const char* str, unsigned int color)
	{
		c_rect rect = get_item_rect(row, col);
		m_surface->fill_rect(rect.m_left + 1, rect.m_top + 1, rect.m_right - 1, rect.m_bottom - 1, color, m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, str, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_align_type);
	}

	unsigned int m_align_type;	
	unsigned int m_row_num;
	unsigned int m_col_num;
	unsigned int m_row_height[MAX_ROW_NUM];
	unsigned int m_col_width[MAX_COL_NUM];
};
#endif
