/**
 * ContainerEx.cpp - 扩展的GUI容器类实现
 */

#include "sgui_containerex.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace sgui {

ContainerEx::ContainerEx() {
    // 初始化默认样式
    m_textColor = Color(0, 0, 0, 1);  // 默认黑色文本
    m_borderColor = Color(0, 0, 0, 1); // 默认黑色边框
    m_backgroundColor = Color(1, 1, 1, 0); // 默认透明背景
}

// ====================================================================
// 背景相关属性实现
// ====================================================================

void ContainerEx::setBackgroundColor(const Color& color) {
    m_backgroundColor = color;
    m_hasBackgroundColor = true;
    markStylesDirty();
}

void ContainerEx::setBackgroundImage(const std::string& imagePath) {
    m_backgroundImage = imagePath;
    m_hasBackgroundImage = !imagePath.empty();
    markStylesDirty();
}

void ContainerEx::setBackgroundGradient(const BackgroundGradient& gradient) {
    m_backgroundGradient = gradient;
    m_hasBackgroundGradient = !gradient.stops.empty();
    markStylesDirty();
}

void ContainerEx::clearBackground() {
    m_hasBackgroundColor = false;
    m_hasBackgroundImage = false;
    m_hasBackgroundGradient = false;
    m_backgroundImage.clear();
    m_backgroundGradient.stops.clear();
    markStylesDirty();
}

// ====================================================================
// 边框样式属性实现
// ====================================================================

void ContainerEx::setBorderColor(const Color& color) {
    m_borderColor = color;
    m_hasBorderColor = true;
    markStylesDirty();
}

void ContainerEx::setBorderStyle(BorderStyle style) {
    m_borderStyle = style;
    m_hasBorderStyle = true;
    markStylesDirty();
}

void ContainerEx::setBorderRadius(const EdgeInsets& radius) {
    m_borderRadius = radius;
    m_hasBorderRadius = true;
    markStylesDirty();
}

void ContainerEx::setBoxShadow(const BoxShadow& shadow) {
    m_boxShadow = shadow;
    m_hasBoxShadow = true;
    markStylesDirty();
}

void ContainerEx::clearBorderStyle() {
    m_hasBorderColor = false;
    m_hasBorderStyle = false;
    m_hasBorderRadius = false;
    m_hasBoxShadow = false;
    markStylesDirty();
}

// ====================================================================
// 文本样式属性实现
// ====================================================================

void ContainerEx::setColor(const Color& color) {
    m_textColor = color;
    m_hasTextColor = true;
    markStylesDirty();
}

void ContainerEx::setFontSize(float size) {
    m_fontSize = std::max(1.0f, size); // 确保字体大小至少为1
    markStylesDirty();
}

void ContainerEx::setFontFamily(const std::string& family) {
    m_fontFamily = family;
    markStylesDirty();
}

void ContainerEx::setFontWeight(FontWeight weight) {
    m_fontWeight = weight;
    markStylesDirty();
}

void ContainerEx::setFontStyle(FontStyle style) {
    m_fontStyle = style;
    markStylesDirty();
}

void ContainerEx::setTextAlign(TextAlign align) {
    m_textAlign = align;
    markStylesDirty();
}

void ContainerEx::setTextDecoration(TextDecoration decoration) {
    m_textDecoration = decoration;
    markStylesDirty();
}

void ContainerEx::setTextOverflow(TextOverflow overflow) {
    m_textOverflow = overflow;
    markStylesDirty();
}

void ContainerEx::setLineHeight(float height) {
    m_lineHeight = std::max(0.1f, height); // 确保行高至少为0.1
    markStylesDirty();
}

void ContainerEx::setTextIndent(float indent) {
    m_textIndent = indent;
    markStylesDirty();
}

void ContainerEx::setText(const std::string& text) {
    m_text = text;
    m_hasTextContent = !text.empty();
    markStylesDirty();
}

void ContainerEx::clearTextStyle() {
    m_hasTextColor = false;
    m_textColor = Color(0, 0, 0, 1);
    m_fontSize = 14.0f;
    m_fontFamily = "Arial";
    m_fontWeight = FontWeight::Normal;
    m_fontStyle = FontStyle::Normal;
    m_textAlign = TextAlign::Left;
    m_textDecoration = TextDecoration::None;
    m_textOverflow = TextOverflow::Clip;
    m_lineHeight = 1.2f;
    m_textIndent = 0.0f;
    markStylesDirty();
}

// ====================================================================
// 重写基类函数实现
// ====================================================================

void ContainerEx::draw() {
    // 绘制背景
    drawBackground();
    
    // 绘制边框
    drawBorder();
    
    // 绘制文本
    drawText();
    
    // 调用基类绘制（如果需要）
    // Container::draw();
}

void ContainerEx::onMeasure(float width, float height, float& measuredWidth, float& measuredHeight) {
    measuredWidth = 0;
    measuredHeight = 0;
    
    // 如果有文本内容，测量文本尺寸
    if (m_hasTextContent) {
        measureText(measuredWidth, measuredHeight);
    }
    
    // 考虑内边距
    float paddingLeft = getLayoutPaddingLeft();
    float paddingRight = getLayoutPaddingRight();
    float paddingTop = getLayoutPaddingTop();
    float paddingBottom = getLayoutPaddingBottom();
    
    measuredWidth += paddingLeft + paddingRight;
    measuredHeight += paddingTop + paddingBottom;
    
    // 考虑边框
    float borderLeft = getLayoutBorderLeft();
    float borderRight = getLayoutBorderRight();
    float borderTop = getLayoutBorderTop();
    float borderBottom = getLayoutBorderBottom();
    
    measuredWidth += borderLeft + borderRight;
    measuredHeight += borderTop + borderBottom;
    
    // 如果没有指定尺寸，使用测量的尺寸
    if (width == YGUndefined || width <= 0) {
        // 使用测量的宽度
    } else {
        measuredWidth = width;
    }
    
    if (height == YGUndefined || height <= 0) {
        // 使用测量的高度
    } else {
        measuredHeight = height;
    }
}

// ====================================================================
// 样式管理实现
// ====================================================================

void ContainerEx::applyStyles() {
    // 这里可以添加样式应用的逻辑
    // 例如：通知渲染系统更新样式
    markStylesDirty();
}

void ContainerEx::resetStyles() {
    clearBackground();
    clearBorderStyle();
    clearTextStyle();
    
    // 重置到默认值
    m_fontSize = 14.0f;
    m_fontFamily = "Arial";
    m_fontWeight = FontWeight::Normal;
    m_fontStyle = FontStyle::Normal;
    m_textAlign = TextAlign::Left;
    m_textDecoration = TextDecoration::None;
    m_textOverflow = TextOverflow::Clip;
    m_lineHeight = 1.2f;
    m_textIndent = 0.0f;
    m_text.clear();
    m_hasTextContent = false;
    
    markStylesDirty();
}

bool ContainerEx::hasBackground() const {
    return m_hasBackgroundColor || m_hasBackgroundImage || m_hasBackgroundGradient;
}

bool ContainerEx::hasBorderStyle() const {
    return m_hasBorderColor || m_hasBorderStyle || m_hasBorderRadius || m_hasBoxShadow;
}

bool ContainerEx::hasTextStyle() const {
    return m_hasTextColor || m_hasTextContent || 
           m_fontSize != 14.0f || m_fontFamily != "Arial" ||
           m_fontWeight != FontWeight::Normal || m_fontStyle != FontStyle::Normal ||
           m_textAlign != TextAlign::Left || m_textDecoration != TextDecoration::None ||
           m_textOverflow != TextOverflow::Clip || m_lineHeight != 1.2f || m_textIndent != 0.0f;
}

// ====================================================================
// 私有辅助函数实现
// ====================================================================

void ContainerEx::drawBackground() {
    if (!hasBackground()) return;
    
    // 获取布局信息
    float left = getLeft();
    float top = getTop();
    float width = getLayoutWidth();
    float height = getLayoutHeight();
    
    // 按照优先级绘制背景：渐变 > 图片 > 颜色
    if (m_hasBackgroundGradient) {
        drawBackgroundGradient();
    } else if (m_hasBackgroundImage) {
        drawBackgroundImage();
    } else if (m_hasBackgroundColor) {
        drawBackgroundColor();
    }
    
    std::cout << "Drawing background at (" << left << ", " << top 
              << ") size: " << width << "x" << height << std::endl;
}

void ContainerEx::drawBackgroundColor() {
    std::cout << "Drawing background color: RGBA(" 
              << m_backgroundColor.r << ", " << m_backgroundColor.g << ", " 
              << m_backgroundColor.b << ", " << m_backgroundColor.a << ")" << std::endl;
}

void ContainerEx::drawBackgroundImage() {
    std::cout << "Drawing background image: " << m_backgroundImage << std::endl;
}

void ContainerEx::drawBackgroundGradient() {
    std::cout << "Drawing background gradient with " << m_backgroundGradient.stops.size() 
              << " stops, angle: " << m_backgroundGradient.angle << "°" << std::endl;
}

void ContainerEx::drawBorder() {
    if (!hasBorderStyle()) return;
    
    // 获取布局信息
    float left = getLeft();
    float top = getTop();
    float width = getLayoutWidth();
    float height = getLayoutHeight();
    
    std::cout << "Drawing border at (" << left << ", " << top 
              << ") size: " << width << "x" << height << std::endl;
    
    if (m_hasBorderColor) {
        std::cout << "  Border color: RGBA(" 
                  << m_borderColor.r << ", " << m_borderColor.g << ", " 
                  << m_borderColor.b << ", " << m_borderColor.a << ")" << std::endl;
    }
    
    if (m_hasBorderStyle) {
        std::cout << "  Border style: ";
        switch (m_borderStyle) {
            case BorderStyle::Solid: std::cout << "Solid"; break;
            case BorderStyle::Dashed: std::cout << "Dashed"; break;
            case BorderStyle::Dotted: std::cout << "Dotted"; break;
            case BorderStyle::Double: std::cout << "Double"; break;
            case BorderStyle::Groove: std::cout << "Groove"; break;
            case BorderStyle::Ridge: std::cout << "Ridge"; break;
            case BorderStyle::Inset: std::cout << "Inset"; break;
            case BorderStyle::Outset: std::cout << "Outset"; break;
        }
        std::cout << std::endl;
    }
    
    if (m_hasBorderRadius) {
        std::cout << "  Border radius: L=" << m_borderRadius.left.value
                  << ", T=" << m_borderRadius.top.value
                  << ", R=" << m_borderRadius.right.value
                  << ", B=" << m_borderRadius.bottom.value << std::endl;
    }
    
    if (m_hasBoxShadow) {
        std::cout << "  Box shadow: offset(" << m_boxShadow.offsetX 
                  << ", " << m_boxShadow.offsetY 
                  << ") blur=" << m_boxShadow.blurRadius
                  << " spread=" << m_boxShadow.spreadRadius
                  << " inset=" << (m_boxShadow.inset ? "true" : "false") << std::endl;
    }
}

void ContainerEx::drawText() {
    if (!m_hasTextContent) return;
    
    // 获取布局信息
    float left = getLeft();
    float top = getTop();
    float width = getLayoutWidth();
    float height = getLayoutHeight();
    
    // 考虑内边距
    float paddingLeft = getLayoutPaddingLeft();
    float paddingTop = getLayoutPaddingTop();
    
    float textX = left + paddingLeft + m_textIndent;
    float textY = top + paddingTop;
    
    // 根据文本对齐方式调整X坐标
    switch (m_textAlign) {
        case TextAlign::Center:
            textX = left + width / 2.0f;
            break;
        case TextAlign::Right:
            textX = left + width - getLayoutPaddingRight();
            break;
        case TextAlign::Justify:
            // 两端对齐需要更复杂的逻辑
            textX = left + paddingLeft + m_textIndent;
            break;
        case TextAlign::Left:
        default:
            textX = left + paddingLeft + m_textIndent;
            break;
    }
    
    std::cout << "Drawing text at (" << textX << ", " << textY 
              << "): \"" << m_text << "\"" << std::endl;
    
    if (m_hasTextColor) {
        std::cout << "  Text color: RGBA(" 
                  << m_textColor.r << ", " << m_textColor.g << ", " 
                  << m_textColor.b << ", " << m_textColor.a << ")" << std::endl;
    }
    
    std::cout << "  Font: " << m_fontFamily << " " << m_fontSize << "px";
    
    switch (m_fontWeight) {
        case FontWeight::Thin: std::cout << " Thin"; break;
        case FontWeight::ExtraLight: std::cout << " ExtraLight"; break;
        case FontWeight::Light: std::cout << " Light"; break;
        case FontWeight::Normal: std::cout << " Normal"; break;
        case FontWeight::Medium: std::cout << " Medium"; break;
        case FontWeight::SemiBold: std::cout << " SemiBold"; break;
        case FontWeight::Bold: std::cout << " Bold"; break;
        case FontWeight::ExtraBold: std::cout << " ExtraBold"; break;
        case FontWeight::Black: std::cout << " Black"; break;
    }
    
    switch (m_fontStyle) {
        case FontStyle::Italic: std::cout << " Italic"; break;
        case FontStyle::Oblique: std::cout << " Oblique"; break;
        case FontStyle::Normal:
        default: break;
    }
    
    std::cout << std::endl;
    
    std::cout << "  Line height: " << m_lineHeight << std::endl;
    std::cout << "  Text indent: " << m_textIndent << std::endl;
}

void ContainerEx::measureText(float& width, float& height) {
    if (!m_hasTextContent) {
        width = 0;
        height = 0;
        return;
    }
    
    // 简单的文本尺寸估算（实际实现需要使用字体库）
    float charWidth = m_fontSize * 0.6f; // 估算字符宽度
    float charHeight = m_fontSize * m_lineHeight; // 考虑行高
    
    // 计算文本行数
    size_t lineCount = 1;
    for (char c : m_text) {
        if (c == '\n') lineCount++;
    }
    
    // 估算文本宽度（最长的行）
    size_t maxLineLength = 0;
    size_t currentLineLength = 0;
    for (char c : m_text) {
        if (c == '\n') {
            maxLineLength = std::max(maxLineLength, currentLineLength);
            currentLineLength = 0;
        } else {
            currentLineLength++;
        }
    }
    maxLineLength = std::max(maxLineLength, currentLineLength);
    
    width = static_cast<float>(maxLineLength) * charWidth + m_textIndent;
    height = static_cast<float>(lineCount) * charHeight;
    
    std::cout << "Measured text: " << m_text << std::endl;
    std::cout << "  Estimated size: " << width << " x " << height << std::endl;
    std::cout << "  Lines: " << lineCount << ", Max line length: " << maxLineLength << std::endl;
}

void ContainerEx::markStylesDirty() {
    m_stylesDirty = true;
    // 标记需要重新绘制
    markDirty();
}

} // namespace sgui
