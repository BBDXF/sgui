/**
 * ContainerEx.h - 扩展的GUI容器类
 * 
 * 继承自Container，添加背景、边框和文本样式功能
 */

#pragma once

#include "sgui_container.h"
#include <string>

namespace sgui {

// 前向声明
class SContainerEx;

// 智能指针类型定义
using ContainerExPtr = std::shared_ptr<SContainerEx>;

/**
 * ContainerEx类 - 扩展的容器，支持样式属性
 */
class SContainerEx : public SContainer {
public:
    SContainerEx();
    ~SContainerEx() override = default;
    
    // ====================================================================
    // 背景相关属性
    // ====================================================================
    
    /** 设置背景色 */
    void setBackgroundColor(const Color& color);
    /** 获取背景色 */
    Color getBackgroundColor() const { return m_backgroundColor; }
    
    /** 设置背景图片 */
    void setBackgroundImage(const std::string& imagePath);
    /** 获取背景图片路径 */
    const std::string& getBackgroundImage() const { return m_backgroundImage; }
    
    /** 设置渐变背景 */
    void setBackgroundGradient(const BackgroundGradient& gradient);
    /** 获取渐变背景 */
    const BackgroundGradient& getBackgroundGradient() const { return m_backgroundGradient; }
    
    /** 清除背景 */
    void clearBackground();
    
    // ====================================================================
    // 边框样式属性
    // ====================================================================
    
    /** 设置边框颜色 */
    void setBorderColor(const Color& color);
    /** 获取边框颜色 */
    Color getBorderColor() const { return m_borderColor; }
    
    /** 设置边框样式 */
    void setBorderStyle(BorderStyle style);
    /** 获取边框样式 */
    BorderStyle getBorderStyle() const { return m_borderStyle; }
    
    /** 设置圆角半径 */
    void setBorderRadius(const EdgeInsets& radius);
    /** 获取圆角半径 */
    const EdgeInsets& getBorderRadius() const { return m_borderRadius; }
    
    /** 设置阴影 */
    void setBoxShadow(const BoxShadow& shadow);
    /** 获取阴影 */
    const BoxShadow& getBoxShadow() const { return m_boxShadow; }
    
    /** 清除边框样式 */
    void clearBorderStyle();
    
    // ====================================================================
    // 文本样式属性
    // ====================================================================
    
    /** 设置文本颜色 */
    void setColor(const Color& color);
    /** 获取文本颜色 */
    Color getColor() const { return m_textColor; }
    
    /** 设置字体大小 */
    void setFontSize(float size);
    /** 获取字体大小 */
    float getFontSize() const { return m_fontSize; }
    
    /** 设置字体族 */
    void setFontFamily(const std::string& family);
    /** 获取字体族 */
    const std::string& getFontFamily() const { return m_fontFamily; }
    
    /** 设置字体粗细 */
    void setFontWeight(FontWeight weight);
    /** 获取字体粗细 */
    FontWeight getFontWeight() const { return m_fontWeight; }
    
    /** 设置字体样式 */
    void setFontStyle(FontStyle style);
    /** 获取字体样式 */
    FontStyle getFontStyle() const { return m_fontStyle; }
    
    /** 设置文本对齐 */
    void setTextAlign(TextAlign align);
    /** 获取文本对齐 */
    TextAlign getTextAlign() const { return m_textAlign; }
    
    /** 设置文本装饰 */
    void setTextDecoration(TextDecoration decoration);
    /** 获取文本装饰 */
    TextDecoration getTextDecoration() const { return m_textDecoration; }
    
    /** 设置文本溢出处理 */
    void setTextOverflow(TextOverflow overflow);
    /** 获取文本溢出处理 */
    TextOverflow getTextOverflow() const { return m_textOverflow; }
    
    /** 设置行高 */
    void setLineHeight(float height);
    /** 获取行高 */
    float getLineHeight() const { return m_lineHeight; }
    
    /** 设置文本缩进 */
    void setTextIndent(float indent);
    /** 获取文本缩进 */
    float getTextIndent() const { return m_textIndent; }
    
    /** 设置文本内容 */
    void setText(const std::string& text);
    /** 获取文本内容 */
    const std::string& getText() const { return m_text; }
    
    /** 清除文本样式 */
    void clearTextStyle();
    
    // ====================================================================
    // 重写基类函数
    // ====================================================================
    
    /** 重写绘制函数 */
    void render(cairo_t* cr) override;
    
    /** 重写测量函数 */
    void onMeasure(float width, float height, float& measuredWidth, float& measuredHeight) override;
    
    // ====================================================================
    // 样式管理
    // ====================================================================
    
    /** 应用所有样式（批量更新） */
    void applyStyles();
    
    /** 重置所有样式到默认值 */
    void resetStyles();
    
    /** 检查是否有背景 */
    bool hasBackground() const;
    
    /** 检查是否有边框样式 */
    bool hasBorderStyle() const;
    
    /** 检查是否有文本样式 */
    bool hasTextStyle() const;

private:
    // ====================================================================
    // 背景相关成员变量
    // ====================================================================
    Color m_backgroundColor;
    std::string m_backgroundImage;
    BackgroundGradient m_backgroundGradient;
    bool m_hasBackgroundColor = false;
    bool m_hasBackgroundImage = false;
    bool m_hasBackgroundGradient = false;
    
    // ====================================================================
    // 边框样式成员变量
    // ====================================================================
    Color m_borderColor;
    BorderStyle m_borderStyle = BorderStyle::Solid;
    EdgeInsets m_borderRadius;
    BoxShadow m_boxShadow;
    bool m_hasBorderColor = false;
    bool m_hasBorderStyle = false;
    bool m_hasBorderRadius = false;
    bool m_hasBoxShadow = false;
    
    // ====================================================================
    // 文本样式成员变量
    // ====================================================================
    Color m_textColor;
    float m_fontSize = 14.0f;
    std::string m_fontFamily = SGUI_DEFAULT_FONT_FAMILY;
    FontWeight m_fontWeight = FontWeight::Normal;
    FontStyle m_fontStyle = FontStyle::Normal;
    TextAlign m_textAlign = TextAlign::Left;
    TextDecoration m_textDecoration = TextDecoration::None;
    TextOverflow m_textOverflow = TextOverflow::Clip;
    float m_lineHeight = 1.2f;
    float m_textIndent = 0.0f;
    std::string m_text;
    bool m_hasTextColor = false;
    bool m_hasTextContent = false;
    
    // ====================================================================
    // 私有辅助函数
    // ====================================================================
    
    /** 测量文本尺寸 */
    void measureText(float& width, float& height);
    
    /** 标记样式已更改 */
    void markStylesDirty();
    
    // ====================================================================
    // Cairo绘制相关私有辅助函数
    // ====================================================================
    
    /** 使用Cairo绘制背景 */
    void drawBackgroundCairo(cairo_t* cr, float x, float y, float width, float height);
    
    /** 使用Cairo绘制边框 */
    void drawBorderCairo(cairo_t* cr, float x, float y, float width, float height);
    
    /** 使用Cairo绘制文本 */
    void drawTextCairo(cairo_t* cr, float x, float y, float width, float height);
    
    /** 样式更改标记 */
    bool m_stylesDirty = true;
};

} // namespace sgui
