#pragma once

#include "../src/core/api.h"
#include "../src/core/wnd.h"
#include "../src/core/resource.h"
#include "../src/core/word.h"
#include "../src/core/display.h"
#include "../src/core/theme.h"

#include <stdio.h>

typedef enum
{
	PROGRESS_HORIZONTAL,
	PROGRESS_VERTICAL
} PROGRESS_ORIENTATION;

typedef void (*PROGRESS_CALLBACK)(int value);

class c_progress_bar : public c_wnd
{
public:
	c_progress_bar()
		: m_min(0)
		, m_max(100)
		, m_value(0)
		, m_orientation(PROGRESS_HORIZONTAL)
		, m_color_start(GL_RGB(0, 128, 0))    // green
		, m_color_end(GL_RGB(255, 0, 0))      // red
		, m_bg_color_bar(GL_RGB(50, 50, 50))
		, m_border_color(GL_RGB(180, 180, 180))
		, m_text_color(GL_RGB(255, 255, 255))
		, m_show_percentage(true)
		, m_animation_speed(1)
		, m_animated_value(0)
		, m_on_complete(0)
		, m_completion_fired(false)
	{
	}

	void set_range(int min_val, int max_val)
	{
		if (min_val >= max_val)
		{
			ASSERT(false);
			return;
		}
		m_min = min_val;
		m_max = max_val;
		// Re-clamp current value to new range
		clamp_value(m_value);
		clamp_value(m_animated_value);
	}

	int get_min() const { return m_min; }
	int get_max() const { return m_max; }

	void set_value(int value)
	{
		// Bug-fix: Bounds clamping - clamp value between min and max
		clamp_value(value);

		if (value == m_value)
		{
			return;
		}

		m_value = value;

		// Check for completion callback
		if (m_value >= m_max && m_on_complete && !m_completion_fired)
		{
			m_completion_fired = true;
			m_on_complete(m_value);
		}
		else if (m_value < m_max)
		{
			m_completion_fired = false;
		}

		on_paint();
	}

	int get_value() const { return m_value; }

	void set_orientation(PROGRESS_ORIENTATION orientation)
	{
		if (m_orientation == orientation)
		{
			return;
		}
		m_orientation = orientation;
		on_paint();
	}

	PROGRESS_ORIENTATION get_orientation() const { return m_orientation; }

	void set_colors(unsigned int start_color, unsigned int end_color)
	{
		m_color_start = start_color;
		m_color_end = end_color;
		on_paint();
	}

	unsigned int get_color_start() const { return m_color_start; }
	unsigned int get_color_end() const { return m_color_end; }

	void set_bg_color_bar(unsigned int color)
	{
		m_bg_color_bar = color;
	}

	void set_border_color(unsigned int color)
	{
		m_border_color = color;
	}

	void set_text_color(unsigned int color)
	{
		m_text_color = color;
	}

	void set_show_percentage(bool show)
	{
		m_show_percentage = show;
	}

	bool get_show_percentage() const { return m_show_percentage; }

	void set_animation_speed(int speed)
	{
		if (speed < 1)
		{
			speed = 1;
		}
		else if (speed > 100)
		{
			speed = 100;
		}
		m_animation_speed = speed;
	}

	int get_animation_speed() const { return m_animation_speed; }

	void set_on_complete(PROGRESS_CALLBACK callback)
	{
		m_on_complete = callback;
	}

	// Call this periodically to animate the fill towards the target value
	void animate_step()
	{
		if (m_animated_value == m_value)
		{
			return;
		}

		if (m_animated_value < m_value)
		{
			m_animated_value += m_animation_speed;
			if (m_animated_value > m_value)
			{
				m_animated_value = m_value;
			}
		}
		else
		{
			m_animated_value -= m_animation_speed;
			if (m_animated_value < m_value)
			{
				m_animated_value = m_value;
			}
		}

		on_paint();
	}

	int get_animated_value() const { return m_animated_value; }

	// Calculate the current percentage (0-100)
	int get_percentage() const
	{
		if (m_max == m_min)
		{
			return 0;
		}
		return ((m_animated_value - m_min) * 100) / (m_max - m_min);
	}

protected:
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE);
		m_font = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
	}

	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);

		if (!m_surface)
		{
			return;
		}

		// Draw background
		m_surface->fill_rect(rect, m_bg_color_bar, m_z_order);

		// Draw border
		m_surface->draw_rect(rect, m_border_color, 1, m_z_order);

		// Calculate fill area (inset by 1 pixel for border)
		int fill_left = rect.m_left + 1;
		int fill_top = rect.m_top + 1;
		int fill_right = rect.m_right - 1;
		int fill_bottom = rect.m_bottom - 1;

		int fill_width = fill_right - fill_left + 1;
		int fill_height = fill_bottom - fill_top + 1;

		if (fill_width <= 0 || fill_height <= 0)
		{
			return;
		}

		int pct = get_percentage();

		if (m_orientation == PROGRESS_HORIZONTAL)
		{
			draw_horizontal_fill(fill_left, fill_top, fill_width, fill_height, pct);
		}
		else
		{
			draw_vertical_fill(fill_left, fill_top, fill_width, fill_height, pct);
		}

		// Draw percentage text
		if (m_show_percentage && m_font)
		{
			char buf[8];
			sprintf(buf, "%d%%", pct);
			c_word::draw_string_in_rect(m_surface, m_z_order, buf, rect,
				m_font, m_text_color, GL_ARGB(0, 0, 0, 0),
				ALIGN_HCENTER | ALIGN_VCENTER);
		}
	}

private:
	void clamp_value(int& value)
	{
		if (value < m_min)
		{
			value = m_min;
		}
		else if (value > m_max)
		{
			value = m_max;
		}
	}

	unsigned int interpolate_color(unsigned int color_a, unsigned int color_b, int ratio_256)
	{
		// ratio_256: 0 = fully color_a, 256 = fully color_b
		int inv = 256 - ratio_256;
		unsigned int r = (GL_RGB_R(color_a) * inv + GL_RGB_R(color_b) * ratio_256) >> 8;
		unsigned int g = (GL_RGB_G(color_a) * inv + GL_RGB_G(color_b) * ratio_256) >> 8;
		unsigned int b = (GL_RGB_B(color_a) * inv + GL_RGB_B(color_b) * ratio_256) >> 8;
		return GL_RGB(r, g, b);
	}

	void draw_horizontal_fill(int left, int top, int width, int height, int pct)
	{
		int filled_width = (width * pct) / 100;
		if (filled_width <= 0)
		{
			return;
		}

		// Draw gradient fill column by column
		for (int x = 0; x < filled_width; x++)
		{
			int ratio = (x * 256) / (width > 1 ? width - 1 : 1);
			unsigned int color = interpolate_color(m_color_start, m_color_end, ratio);

			m_surface->draw_vline(left + x, top, top + height - 1, color, m_z_order);
		}
	}

	void draw_vertical_fill(int left, int top, int width, int height, int pct)
	{
		int filled_height = (height * pct) / 100;
		if (filled_height <= 0)
		{
			return;
		}

		// Vertical fills from bottom to top
		int start_y = top + height - filled_height;

		for (int y = 0; y < filled_height; y++)
		{
			int ratio = (y * 256) / (height > 1 ? height - 1 : 1);
			unsigned int color = interpolate_color(m_color_start, m_color_end, ratio);

			m_surface->draw_hline(left, left + width - 1, start_y + y, color, m_z_order);
		}
	}

	int                    m_min;
	int                    m_max;
	int                    m_value;
	int                    m_animated_value;
	PROGRESS_ORIENTATION   m_orientation;
	unsigned int           m_color_start;
	unsigned int           m_color_end;
	unsigned int           m_bg_color_bar;
	unsigned int           m_border_color;
	unsigned int           m_text_color;
	bool                   m_show_percentage;
	int                    m_animation_speed;
	PROGRESS_CALLBACK      m_on_complete;
	bool                   m_completion_fired;
};
