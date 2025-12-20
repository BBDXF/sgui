/**
 * Cairo渲染器实现
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
    , m_frontSurface(nullptr), m_frontCairo(nullptr)
    , m_backSurface(nullptr), m_backCairo(nullptr) {
    
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
    if (!m_backCairo) {
        return;
    }
    
    // 重置变换矩阵
    cairo_identity_matrix(m_backCairo);
    
    // 清除后缓冲为白色背景
    cairo_save(m_backCairo);
    cairo_set_source_rgba(m_backCairo, 1.0, 1.0, 1.0, 1.0);
    cairo_fill(m_backCairo);
    cairo_restore(m_backCairo);
}

void SCairoRenderer::end() {
    if (!m_backSurface || !m_frontCairo) {
        return;
    }
    
    // 将后缓冲内容绘制到前缓冲
    cairo_save(m_frontCairo);
    cairo_set_source_surface(m_frontCairo, m_backSurface, 0, 0);
    cairo_paint(m_frontCairo);
    cairo_restore(m_frontCairo);
    
    // 刷新前缓冲到窗口
    cairo_surface_flush(m_frontSurface);
}


void SCairoRenderer::initCairoSurface() {
    // 1. 创建前缓冲（直接绑定到窗口）
    
#ifdef _WIN32
    // Windows平台：使用HWND创建直接绘制surface
    if (m_windowId) {
        HDC hdc = GetDC(static_cast<HWND>(m_windowId));
        m_frontSurface = cairo_win32_surface_create(hdc);
    }
#elif defined(__linux__)
    // X11平台：使用X11 Window创建直接绘制surface
    if (m_windowId) {
        ::Display* dpy = XOpenDisplay(nullptr);
        if (dpy) {
            ::Visual* visual = DefaultVisual(dpy, DefaultScreen(dpy));
            ::Drawable drawable = static_cast<Drawable>(reinterpret_cast<uintptr_t>(m_windowId));
            m_frontSurface = cairo_xlib_surface_create(dpy, drawable, visual, m_width, m_height);
        }
    }
#endif
    
    // 检查前缓冲创建是否成功
    if (!m_frontSurface || cairo_surface_status(m_frontSurface) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create front Cairo surface" << std::endl;
        m_frontSurface = nullptr;
        return;
    }
    
    // 创建前缓冲的Cairo上下文
    m_frontCairo = cairo_create(m_frontSurface);
    if (cairo_status(m_frontCairo) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create front Cairo context" << std::endl;
        cairo_surface_destroy(m_frontSurface);
        m_frontSurface = nullptr;
        m_frontCairo = nullptr;
        return;
    }
    
    // 2. 创建后缓冲（内存中的图像表面）
    m_backSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
    if (!m_backSurface || cairo_surface_status(m_backSurface) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create back Cairo surface" << std::endl;
        cairo_destroy(m_frontCairo);
        cairo_surface_destroy(m_frontSurface);
        m_frontSurface = nullptr;
        m_frontCairo = nullptr;
        m_backSurface = nullptr;
        return;
    }
    
    // 创建后缓冲的Cairo上下文
    m_backCairo = cairo_create(m_backSurface);
    if (cairo_status(m_backCairo) != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Failed to create back Cairo context" << std::endl;
        cairo_surface_destroy(m_frontSurface);
        cairo_surface_destroy(m_backSurface);
        cairo_destroy(m_frontCairo);
        cairo_destroy(m_backCairo);
        m_frontSurface = nullptr;
        m_frontCairo = nullptr;
        m_backSurface = nullptr;
        m_backCairo = nullptr;
        return;
    }
    
    // 设置抗锯齿
    cairo_set_antialias(m_backCairo, CAIRO_ANTIALIAS_SUBPIXEL);
    
    std::cout << "Created double-buffered Cairo renderer (" << m_width << "x" << m_height << ")" << std::endl;
}

void SCairoRenderer::cleanupCairoSurface() {
    if (m_backCairo) {
        cairo_destroy(m_backCairo);
        m_backCairo = nullptr;
    }
    
    if (m_backSurface) {
        cairo_surface_destroy(m_backSurface);
        m_backSurface = nullptr;
    }
    
    if (m_frontCairo) {
        cairo_destroy(m_frontCairo);
        m_frontCairo = nullptr;
    }
    
    if (m_frontSurface) {
        cairo_surface_destroy(m_frontSurface);
        m_frontSurface = nullptr;
    }
}


} // namespace sgui
