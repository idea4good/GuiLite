#pragma once

#include "GuiLite.h"

// Enhanced display class with anti-flashing resize support
class c_display_enhanced : public c_display {
public:
    c_display_enhanced(void* phy_fb, int display_width, int display_height, 
                      int surface_width, int surface_height, unsigned int color_bytes, 
                      int surface_cnt, DISPLAY_DRIVER* driver = 0)
        : c_display(phy_fb, display_width, display_height, surface_width, surface_height, 
                   color_bytes, surface_cnt, driver)
        , m_resize_in_progress(false)
        , m_deferred_flush_enabled(false)
        , m_flush_timer(0)
        , m_last_width(display_width)
        , m_last_height(display_height)
    {
    }

    // Override flush_screen to implement deferred rendering
    virtual int flush_screen(int left, int top, int right, int bottom, void* fb, int fb_width) override
    {
        if (m_resize_in_progress || m_deferred_flush_enabled) {
            // Store the flush request for later
            m_pending_flush.left = left;
            m_pending_flush.top = top;
            m_pending_flush.right = right;
            m_pending_flush.bottom = bottom;
            m_pending_flush.fb = fb;
            m_pending_flush.fb_width = fb_width;
            m_pending_flush.pending = true;
            return 0;
        }
        
        return c_display::flush_screen(left, top, right, bottom, fb, fb_width);
    }

    // Handle resize events
    void handle_resize(int new_width, int new_height)
    {
        if (new_width == m_last_width && new_height == m_last_height) {
            return; // No actual resize
        }

        m_resize_in_progress = true;
        
        // Update dimensions
        m_last_width = new_width;
        m_last_height = new_height;
        m_width = new_width;
        m_height = new_height;

        // Clear any pending flushes during resize
        m_pending_flush.pending = false;
        
        // Enable deferred flushing for a short period after resize
        enable_deferred_flush(50); // 50ms delay
    }

    // Enable deferred flushing for specified milliseconds
    void enable_deferred_flush(int delay_ms)
    {
        m_deferred_flush_enabled = true;
        m_flush_timer = get_time_in_second() * 1000 + delay_ms;
    }

    // Update method to be called in main loop
    void update()
    {
        long current_time = get_time_in_second() * 1000;
        
        // Check if resize is complete
        if (m_resize_in_progress && current_time > m_flush_timer) {
            m_resize_in_progress = false;
        }
        
        // Check if deferred flush period is over
        if (m_deferred_flush_enabled && current_time > m_flush_timer) {
            m_deferred_flush_enabled = false;
            
            // Execute pending flush if any
            if (m_pending_flush.pending) {
                c_display::flush_screen(m_pending_flush.left, m_pending_flush.top,
                                      m_pending_flush.right, m_pending_flush.bottom,
                                      m_pending_flush.fb, m_pending_flush.fb_width);
                m_pending_flush.pending = false;
            }
        }
    }

    // Force immediate flush (for critical updates)
    void force_flush()
    {
        if (m_pending_flush.pending) {
            c_display::flush_screen(m_pending_flush.left, m_pending_flush.top,
                                  m_pending_flush.right, m_pending_flush.bottom,
                                  m_pending_flush.fb, m_pending_flush.fb_width);
            m_pending_flush.pending = false;
        }
        m_resize_in_progress = false;
        m_deferred_flush_enabled = false;
    }

private:
    struct PendingFlush {
        int left, top, right, bottom;
        void* fb;
        int fb_width;
        bool pending;
        
        PendingFlush() : pending(false) {}
    };

    bool m_resize_in_progress;
    bool m_deferred_flush_enabled;
    long m_flush_timer;
    int m_last_width;
    int m_last_height;
    PendingFlush m_pending_flush;
};

// Enhanced surface class with resize awareness
class c_surface_enhanced : public c_surface {
public:
    c_surface_enhanced(unsigned int width, unsigned int height, unsigned int color_bytes, 
                      Z_ORDER_LEVEL max_zorder = Z_ORDER_LEVEL_0, c_rect overlap_rect = c_rect())
        : c_surface(width, height, color_bytes, max_zorder, overlap_rect)
        , m_batch_mode(false)
        , m_batch_count(0)
    {
    }

    // Override draw_pixel to support batching
    virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order) override
    {
        if (m_batch_mode) {
            m_batch_count++;
            // Only update framebuffer, don't flush to screen
            if (x >= m_width || y >= m_height || x < 0 || y < 0) {
                return;
            }
            
            if (z_order > (unsigned int)m_max_zorder) {
                return;
            }

            // Update internal buffers only
            if (z_order == m_max_zorder) {
                if (m_fb) {
                    (m_color_bytes == 2) ? 
                        ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb) : 
                        ((unsigned int*)m_fb)[y * m_width + x] = rgb;
                }
                return;
            }

            // Handle layer updates
            if (m_layers[z_order].rect.pt_in_rect(x, y)) {
                c_rect layer_rect = m_layers[z_order].rect;
                if (m_color_bytes == 2) {
                    ((unsigned short*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + 
                        (y - layer_rect.m_top) * layer_rect.width()] = GL_RGB_32_to_16(rgb);
                } else {
                    ((unsigned int*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + 
                        (y - layer_rect.m_top) * layer_rect.width()] = rgb;
                }
            }
            return;
        }
        
        // Normal operation
        c_surface::draw_pixel(x, y, rgb, z_order);
    }

    // Start batch mode to prevent individual pixel flushes
    void begin_batch_draw()
    {
        m_batch_mode = true;
        m_batch_count = 0;
    }

    // End batch mode and flush all changes at once
    void end_batch_draw()
    {
        if (m_batch_mode && m_batch_count > 0) {
            m_batch_mode = false;
            
            // Flush the entire surface
            if (m_is_active && m_display) {
                c_rect full_rect(0, 0, m_width, m_height);
                flush_screen(full_rect.m_left, full_rect.m_top, 
                           full_rect.m_right, full_rect.m_bottom);
            }
        }
        m_batch_mode = false;
    }

private:
    bool m_batch_mode;
    int m_batch_count;
};

// Window class with resize handling
class c_wnd_enhanced : public c_wnd {
public:
    c_wnd_enhanced() : c_wnd(), m_resize_handler(nullptr) {}

    // Set resize handler callback
    void set_resize_handler(void (*handler)(c_wnd*, int, int))
    {
        m_resize_handler = handler;
    }

    // Handle resize event
    virtual void on_resize(int new_width, int new_height)
    {
        // Update window rectangle if this is root window
        if (!m_parent) {
            m_wnd_rect.m_right = m_wnd_rect.m_left + new_width - 1;
            m_wnd_rect.m_bottom = m_wnd_rect.m_top + new_height - 1;
        }

        // Call user-defined resize handler
        if (m_resize_handler) {
            m_resize_handler(this, new_width, new_height);
        }

        // Notify display about resize
        if (m_surface && m_surface->get_display()) {
            c_display_enhanced* enhanced_display = 
                dynamic_cast<c_display_enhanced*>(m_surface->get_display());
            if (enhanced_display) {
                enhanced_display->handle_resize(new_width, new_height);
            }
        }

        // Propagate resize to children
        c_wnd* child = m_top_child;
        while (child) {
            c_wnd_enhanced* enhanced_child = dynamic_cast<c_wnd_enhanced*>(child);
            if (enhanced_child) {
                enhanced_child->on_resize(new_width, new_height);
            }
            child = child->m_next_sibling;
        }
    }

    // Enhanced show_window with batch drawing
    virtual void show_window(bool pre_paint = false) override
    {
        if (ATTR_VISIBLE == (m_attr & ATTR_VISIBLE)) {
            c_surface_enhanced* enhanced_surface = 
                dynamic_cast<c_surface_enhanced*>(m_surface);
            
            if (enhanced_surface) {
                enhanced_surface->begin_batch_draw();
            }

            if (pre_paint) {
                on_paint();
            } else if (m_parent == 0) { // root wnd
                c_rect rc;
                get_screen_rect(rc);
                if (enhanced_surface) {
                    enhanced_surface->end_batch_draw();
                } else {
                    m_surface->flush_screen(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
                }
                return;
            }

            c_wnd* child = m_top_child;
            if (0 != child) {
                while (child) {
                    child->show_window(pre_paint);
                    child = child->m_next_sibling;
                }
            }

            if (enhanced_surface) {
                enhanced_surface->end_batch_draw();
            }
        }
    }

private:
    void (*m_resize_handler)(c_wnd*, int, int);
};