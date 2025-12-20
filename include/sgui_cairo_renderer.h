/**
 * Cairo渲染器
 * 支持双缓冲以减少闪烁
 */

#pragma once

#include <cairo/cairo.h>
#include <memory>
#include <string>
#include "sgui_common.h"


namespace sgui {

/**
 * Cairo渲染器类
 * 
 * 使用双缓冲技术：先绘制到内存中的后缓冲，然后一次性复制到窗口表面
 * 这样可以避免绘制过程中的闪烁问题
 */
class SCairoRenderer {
public:
    /**
     * 构造函数
     * @param windowId 窗口ID（HWND或X11 Window）
     * @param width 渲染器宽度
     * @param height 渲染器高度
     */
    SCairoRenderer(void* windowId, int width, int height);
    
    /**
     * 析构函数
     */
    ~SCairoRenderer();
    
    /**
     * 禁用拷贝构造和赋值
     */
    SCairoRenderer(const SCairoRenderer&) = delete;
    SCairoRenderer& operator=(const SCairoRenderer&) = delete;
    
    /**
     * 调整渲染器大小
     * @param width 新宽度
     * @param height 新高度
     */
    void resize(int width, int height);
    
    /**
     * 获取Cairo绘制上下文
     * @return Cairo上下文指针（指向后缓冲）
     */
    cairo_t* getContext() const { return m_backCairo; }
    
    /**
     * 开始绘制
     * 清除后缓冲并准备绘制
     */
    void begin();
    
    /**
     * 结束绘制
     * 将后缓冲内容复制到前缓冲（窗口）
     */
    void end();
    
    

private:
    void* m_windowId;  // 窗口ID
    int m_width;
    int m_height;
    
    // 前缓冲（直接绑定到窗口）
    cairo_surface_t* m_frontSurface;
    cairo_t* m_frontCairo;
    
    // 后缓冲（内存中的图像表面）
    cairo_surface_t* m_backSurface;
    cairo_t* m_backCairo;
    
    /**
     * 初始化Cairo表面
     * 创建前缓冲和后缓冲
     */
    void initCairoSurface();
    
    /**
     * 清理Cairo表面
     */
    void cleanupCairoSurface();
    
};

} // namespace sgui
