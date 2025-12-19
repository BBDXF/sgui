/**
 * SCairoRenderer.cpp - Cairo渲染器实现
 * 直接绑定window id进行surface绘制
 */

#include "sgui_cairo_renderer.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#endif

namespace sgui {

SCairoRenderer::SCairoRenderer(void* windowId, int width, int height)
    : m_windowId(windowId), m_width(width), m_height(height)
    , m_surface(nullptr), m_cairo(nullptr) {
    
    initCairoSurface();
}

SCairoRenderer::~SCairoRenderer() {
    cleanupCairoSurface();
}

void SCairoRenderer::resize(int width, int height) {
    if (m_width == width && m_height == height) {
        return;
    }
    
    m_width = width;
    m_height = height;
    
    // 重新初始化Cairo表面
    cleanupCairoSurface();
    initCairoSurface();
}

void SCairoRenderer::begin() {
    if (!m_cairo) {
        return;
    }
    
    // 重置变换矩阵
    cairo_identity_matrix(m_cairo);
}

void SCairoRenderer::end() {
    // 直接绘制到窗口，无需额外的纹理更新
    cairo_surface_flush(m_surface);
}

void SCairoRenderer::setClipRect(const Rect& rect) {
    if (!m_cairo) return;
    
    cairo_rectangle(m_cairo, rect.x, rect.y, rect.width, rect.height);
    cairo_clip(m_cairo);
}

void SCairoRenderer::clearClip() {
    if (!m_cairo) return;
    cairo_reset_clip(m_cairo);
}

void SCairoRenderer::drawRect(const Rect& rect, const Color& color) {
    if (!m_cairo) return;
    
    cairo_save(m_cairo);
    cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
    cairo_rectangle(m_cairo, rect.x, rect.y, rect.width, rect.height);
    cairo_fill(m_cairo);
    cairo_restore(m_cairo);
}

void SCairoRenderer::drawRoundedRect(const Rect& rect, double radius, const Color& color) {
    if (!m_cairo) return;
    
    cairo_save(m_cairo);
    cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
    
    // 计算圆角矩形的路径
    double x = rect.x;
    double y = rect.y;
    double width = rect.width;
    double height = rect.height;
    double r = std::min(radius, std::min(width, height) / 2.0);
    
    cairo_move_to(m_cairo, x + r, y);
    cairo_line_to(m_cairo, x + width - r, y);
    cairo_arc(m_cairo, x + width - r, y + r, r, -M_PI/2, 0);
    cairo_line_to(m_cairo, x + width, y + height - r);
    cairo_arc(m_cairo, x + width - r, y + height - r, r, 0, M_PI/2);
    cairo_line_to(m_cairo, x + r, y + height);
    cairo_arc(m_cairo, x + r, y + height - r, r, M_PI/2, M_PI);
    cairo_line_to(m_cairo, x, y + r);
    cairo_arc(m_cairo, x + r, y + r, r, M_PI, -M_PI/2);
    cairo_close_path(m_cairo);
    
    cairo_fill(m_cairo);
    cairo_restore(m_cairo);
}

void SCairoRenderer::drawBorder(const Rect& rect, const Color& color, double lineWidth) {
    if (!m_cairo) return;
    
    cairo_save(m_cairo);
    cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
    cairo_set_line_width(m_cairo, lineWidth);
    cairo_rectangle(m_cairo, rect.x + lineWidth/2, rect.y + lineWidth/2, 
                    rect.width - lineWidth, rect.height - lineWidth);
    cairo_stroke(m_cairo);
    cairo_restore(m_cairo);
}

void SCairoRenderer::drawText(const std::string& text, double x, double y, 
                                   const Color& color, double fontSize,
                                   const std::string& fontFamily) {
    if (!m_cairo || text.empty()) return;
    
    cairo_save(m_cairo);
    
    // 设置字体
    cairo_select_font_face(m_cairo, fontFamily.c_str(), 
                           CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(m_cairo, fontSize);
    
    // 设置文本颜色
    cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
    
    // 移动到指定位置并绘制文本
    cairo_move_to(m_cairo, x, y);
    cairo_show_text(m_cairo, text.c_str());
    
    cairo_restore(m_cairo);
}

void SCairoRenderer::drawImage(const std::string& imagePath, const Rect& rect) {
    if (!m_cairo) return;
    
    // 加载图片到Cairo表面
    cairo_surface_t* imageSurface = cairo_image_surface_create_from_png(imagePath.c_str());
    if (!imageSurface || cairo_surface_status(imageSurface) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        if (imageSurface) {
            cairo_surface_destroy(imageSurface);
        }
        return;
    }
    
    cairo_save(m_cairo);
    
    // 计算缩放比例以适应目标矩形
    double imageWidth = cairo_image_surface_get_width(imageSurface);
    double imageHeight = cairo_image_surface_get_height(imageSurface);
    double scaleX = rect.width / imageWidth;
    double scaleY = rect.height / imageHeight;
    
    // 应用变换
    cairo_translate(m_cairo, rect.x, rect.y);
    cairo_scale(m_cairo, scaleX, scaleY);
    
    // 绘制图片
    cairo_set_source_surface(m_cairo, imageSurface, 0, 0);
    cairo_paint(m_cairo);
    
    cairo_restore(m_cairo);
    cairo_surface_destroy(imageSurface);
}

void SCairoRenderer::clear(const Color& color) {
    if (!m_cairo) return;
    
    cairo_save(m_cairo);
    cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
    cairo_rectangle(m_cairo, 0, 0, m_width, m_height);
    cairo_fill(m_cairo);
    cairo_restore(m_cairo);
}

void SCairoRenderer::initCairoSurface() {
    // 根据平台和窗口类型创建相应的Cairo表面
    
#ifdef _WIN32
    // Windows平台：使用HWND创建直接绘制surface
    if (m_windowId) {
        HDC hdc = GetDC(static_cast<HWND>(m_windowId));
        m_surface = cairo_win32_surface_create(hdc);
    }
#elif defined(__linux__)
    // X11平台：使用X11 Window创建直接绘制surface
    if (m_windowId) {
        ::Display* dpy = XOpenDisplay(nullptr);
        if (dpy) {
            ::Visual* visual = DefaultVisual(dpy, DefaultScreen(dpy));
            ::Drawable drawable = static_cast<Drawable>(reinterpret_cast<uintptr_t>(m_windowId));
            m_surface = cairo_xlib_surface_create(dpy, drawable, visual, m_width, m_height);
        }
    }
#endif
    
    // error check
    if (!m_surface || cairo_surface_status(m_surface) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create direct window surface, falling back to image surface" << std::endl;
        return;
    }
    
    if (cairo_surface_status(m_surface) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create Cairo surface" << std::endl;
        m_surface = nullptr;
        return;
    }
    
    // 创建Cairo上下文
    m_cairo = cairo_create(m_surface);
    if (cairo_status(m_cairo) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create Cairo context" << std::endl;
        cairo_surface_destroy(m_surface);
        m_surface = nullptr;
        m_cairo = nullptr;
        return;
    }
    
    // 设置抗锯齿
    cairo_set_antialias(m_cairo, CAIRO_ANTIALIAS_SUBPIXEL);
}

void SCairoRenderer::cleanupCairoSurface() {
    if (m_cairo) {
        cairo_destroy(m_cairo);
        m_cairo = nullptr;
    }
    
    if (m_surface) {
        cairo_surface_destroy(m_surface);
        m_surface = nullptr;
    }
}

} // namespace sgui
