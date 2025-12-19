/**
 * SCairoRenderer.h - Cairo渲染器
 * 
 * 直接绑定window id进行surface绘制，避免了复杂的OpenGL纹理转换机制
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
 * 直接绑定window id进行surface绘制，简化了原有的OpenGL纹理转换机制
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
     * @return Cairo上下文指针
     */
    cairo_t* getContext() const { return m_cairo; }
    
    /**
     * 开始绘制
     * 清除表面并准备绘制
     */
    void begin();
    
    /**
     * 结束绘制
     * 直接刷新到窗口，无需纹理更新
     */
    void end();
    
    /**
     * 获取渲染器宽度
     */
    int getWidth() const { return m_width; }
    
    /**
     * 获取渲染器高度
     */
    int getHeight() const { return m_height; }
    
    /**
     * 设置裁剪区域
     * @param rect 裁剪矩形
     */
    void setClipRect(const Rect& rect);
    
    /**
     * 清除裁剪区域
     */
    void clearClip();
    
    /**
     * 绘制矩形
     * @param rect 矩形区域
     * @param color 填充颜色
     */
    void drawRect(const Rect& rect, const Color& color);
    
    /**
     * 绘制圆角矩形
     * @param rect 矩形区域
     * @param radius 圆角半径
     * @param color 填充颜色
     */
    void drawRoundedRect(const Rect& rect, double radius, const Color& color);
    
    /**
     * 绘制边框
     * @param rect 矩形区域
     * @param color 边框颜色
     * @param lineWidth 线宽
     */
    void drawBorder(const Rect& rect, const Color& color, double lineWidth = 1.0);
    
    /**
     * 绘制文本
     * @param text 文本内容
     * @param x X坐标
     * @param y Y坐标
     * @param color 文本颜色
     * @param fontSize 字体大小
     * @param fontFamily 字体族名
     */
    void drawText(const std::string& text, double x, double y, 
                  const Color& color, double fontSize = 12.0,
                  const std::string& fontFamily = SGUI_DEFAULT_FONT_FAMILY);
    
    /**
     * 绘制图片
     * @param imagePath 图片路径
     * @param rect 目标矩形
     */
    void drawImage(const std::string& imagePath, const Rect& rect);
    
    /**
     * 清除表面
     * @param color 清除颜色
     */
    void clear(const Color& color);

private:
    void* m_windowId;  // 窗口ID
    int m_width;
    int m_height;
    
    // Cairo相关
    cairo_surface_t* m_surface;
    cairo_t* m_cairo;
    
    /**
     * 初始化Cairo表面
     * 根据平台选择合适的surface类型
     */
    void initCairoSurface();
    
    /**
     * 清理Cairo表面
     */
    void cleanupCairoSurface();
};

} // namespace sgui
