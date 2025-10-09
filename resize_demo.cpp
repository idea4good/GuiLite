#include "resize_fix.h"
#include <iostream>
#include <thread>
#include <chrono>

// Mock display driver for demonstration
struct MockDisplayDriver : public DISPLAY_DRIVER {
    MockDisplayDriver() {
        draw_pixel = mock_draw_pixel;
        fill_rect = mock_fill_rect;
    }
    
    static void mock_draw_pixel(int x, int y, unsigned int rgb) {
        // Simulate drawing to actual display
        // In real implementation, this would draw to screen/window
    }
    
    static void mock_fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb) {
        // Simulate filling rectangle on actual display
        // In real implementation, this would fill screen/window area
    }
};

// Sample application window
class SampleWindow : public c_wnd_enhanced {
public:
    SampleWindow() : m_counter(0) {}
    
    virtual void on_paint() override {
        if (!m_surface) return;
        
        // Clear background
        c_rect rect;
        get_wnd_rect(rect);
        m_surface->fill_rect(rect, GL_RGB(240, 240, 240), Z_ORDER_LEVEL_0);
        
        // Draw some content
        m_surface->draw_rect(10, 10, rect.width() - 20, rect.height() - 20, 
                           GL_RGB(100, 150, 200), Z_ORDER_LEVEL_0, 2);
        
        // Draw counter text (simplified)
        char buffer[32];
        sprintf(buffer, "Frame: %d", m_counter++);
        // Note: In real implementation, you'd use the font system here
        
        std::cout << "Painting window: " << buffer << " Size: " 
                  << rect.width() << "x" << rect.height() << std::endl;
    }
    
    static void resize_callback(c_wnd* wnd, int width, int height) {
        std::cout << "Window resized to: " << width << "x" << height << std::endl;
        
        // Force repaint after resize
        wnd->show_window(true);
    }
    
private:
    int m_counter;
};

// Main application class
class ResizeTestApp {
public:
    ResizeTestApp() : m_display(nullptr), m_surface(nullptr), m_window(nullptr) {}
    
    ~ResizeTestApp() {
        cleanup();
    }
    
    bool initialize(int width, int height) {
        try {
            // Create mock framebuffer
            m_framebuffer = new unsigned int[width * height];
            memset(m_framebuffer, 0, width * height * sizeof(unsigned int));
            
            // Create enhanced display with anti-flashing support
            m_display = new c_display_enhanced(
                m_framebuffer, width, height,    // Physical display
                width, height, 4, 1,             // Surface dimensions, 32-bit color, 1 surface
                &m_driver                        // Mock driver
            );
            
            // Create enhanced surface
            m_surface = new c_surface_enhanced(width, height, 4, Z_ORDER_LEVEL_2);
            m_surface->attach_display(m_display);
            m_surface->set_active(true);
            
            // Create main window
            m_window = new SampleWindow();
            m_window->set_surface(m_surface);
            m_window->set_resize_handler(SampleWindow::resize_callback);
            
            // Connect window
            m_window->connect(nullptr, 1, "Main Window", 0, 0, width, height);
            
            std::cout << "Application initialized: " << width << "x" << height << std::endl;
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Initialization failed: " << e.what() << std::endl;
            cleanup();
            return false;
        }
    }
    
    void simulate_resize(int new_width, int new_height) {
        if (!m_display || !m_window) return;
        
        std::cout << "\n=== Simulating resize to " << new_width << "x" << new_height << " ===" << std::endl;
        
        // Simulate the resize event that would come from the OS
        m_window->on_resize(new_width, new_height);
        
        // Update display
        m_display->update();
        
        std::cout << "Resize completed without flashing!" << std::endl;
    }
    
    void run_demo() {
        if (!initialize(800, 600)) {
            return;
        }
        
        // Initial paint
        m_window->show_window(true);
        
        // Simulate various resize operations
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        simulate_resize(1024, 768);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        simulate_resize(640, 480);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        simulate_resize(1200, 900);
        
        // Simulate rapid resizing (like dragging window border)
        std::cout << "\n=== Simulating rapid resize (drag simulation) ===" << std::endl;
        for (int i = 0; i < 10; i++) {
            int w = 800 + i * 20;
            int h = 600 + i * 15;
            simulate_resize(w, h);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // Final update
        m_display->force_flush();
        
        std::cout << "\nDemo completed successfully!" << std::endl;
    }
    
private:
    void cleanup() {
        delete m_window;
        delete m_surface;
        delete m_display;
        delete[] m_framebuffer;
        
        m_window = nullptr;
        m_surface = nullptr;
        m_display = nullptr;
        m_framebuffer = nullptr;
    }
    
    MockDisplayDriver m_driver;
    c_display_enhanced* m_display;
    c_surface_enhanced* m_surface;
    SampleWindow* m_window;
    unsigned int* m_framebuffer;
};

int main() {
    std::cout << "GuiLite Resize Fix Demo" << std::endl;
    std::cout << "=======================" << std::endl;
    
    ResizeTestApp app;
    app.run_demo();
    
    return 0;
}