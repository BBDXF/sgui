/**
 * GUI容器基类
 */

#include "sgui_container.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace sgui
{

SContainer::SContainer()
{
    // 初始化默认样式
    clearTextStyle();
    setDisplay(Display::Flex);
    setFlexDirection(FlexDirection::Column);
    setBackgroundColor(Color::White());
}

SContainer::~SContainer()
{
    // 清理背景资源
    cleanupBackgroundSource();
}

// ====================================================================
// 背景相关属性实现
// ====================================================================

void SContainer::setBackgroundColor(const Color &color)
{
    m_backgroundColor = color;
    markStylesDirty();
}

void SContainer::setBackgroundImage(const std::string &imagePath)
{
    m_backgroundImage = imagePath;
    m_hasBackgroundImage = !imagePath.empty();
    markStylesDirty();
}

void SContainer::setBackgroundGradient(const BackgroundGradient &gradient)
{
    m_backgroundGradient = gradient;
    m_hasBackgroundGradient = !gradient.stops.empty();
    markStylesDirty();
}

void SContainer::clearBackground()
{
    m_backgroundColor = Color(1, 1, 1, 0); // 透明背景
    m_backgroundImage.clear();
    m_backgroundGradient.stops.clear();
    m_hasBackgroundImage = false;
    m_hasBackgroundGradient = false;
    markStylesDirty();
}

// ====================================================================
// 边框样式属性实现
// ====================================================================

void SContainer::setBorderColor(const Color &color)
{
    m_borderColor = color;
    markStylesDirty();
}

void SContainer::setBorderStyle(BorderStyle style)
{
    m_borderStyle = style;
    markStylesDirty();
}

void SContainer::setBorderRadius(const EdgeInsets &radius)
{
    m_borderRadius = radius;
    markStylesDirty();
}

void SContainer::setBoxShadow(const BoxShadow &shadow)
{
    m_boxShadow = shadow;
    markStylesDirty();
}

void SContainer::clearBorderStyle()
{
    m_borderColor = Color(0, 0, 0, 0); //透明边框
    m_borderStyle = BorderStyle::Solid;
    m_borderRadius = EdgeInsets();
    m_boxShadow = BoxShadow();
    markStylesDirty();
}

// ====================================================================
// 文本样式属性实现
// ====================================================================

void SContainer::setColor(const Color &color)
{
    m_textColor = color;
    markStylesDirty();
}

void SContainer::setFontSize(float size)
{
    m_fontSize = std::max(1.0f, size); // 确保字体大小至少为1
    markStylesDirty();
}

void SContainer::setFontFamily(const std::string &family)
{
    m_fontFamily = family;
    markStylesDirty();
}

void SContainer::setFontWeight(FontWeight weight)
{
    m_fontWeight = weight;
    markStylesDirty();
}

void SContainer::setFontStyle(FontStyle style)
{
    m_fontStyle = style;
    markStylesDirty();
}

void SContainer::setTextAlign(TextAlign align)
{
    m_textAlign = align;
    markStylesDirty();
}

void SContainer::setTextDecoration(TextDecoration decoration)
{
    m_textDecoration = decoration;
    markStylesDirty();
}

void SContainer::setTextOverflow(TextOverflow overflow)
{
    m_textOverflow = overflow;
    markStylesDirty();
}

void SContainer::setLineHeight(float height)
{
    m_lineHeight = std::max(0.1f, height); // 确保行高至少为0.1
    markStylesDirty();
}

void SContainer::setTextIndent(float indent)
{
    m_textIndent = indent;
    markStylesDirty();
}

void SContainer::setText(const std::string &text)
{
    m_text = text;
    m_hasTextContent = !text.empty();
    markStylesDirty();
}

void SContainer::clearTextStyle()
{
    m_textColor = Color(0, 0, 0, 1); // 默认黑色文本
    m_fontSize = 14.0f;
    m_fontFamily = SGUI_DEFAULT_FONT_FAMILY;
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

// ====================================================================
// 重写基类函数实现
// render只绘制box内部区域（border+padding+content)
// 假定调用者已经将坐标平移到border边界(0,0)
// ====================================================================

void SContainer::render(cairo_t *cr)
{
    if (!cr)
        return;

    // 获取容器的布局信息
    float x = 0; // 相对于容器的坐标
    float y = 0;
    float width = getLayoutWidth();
    float height = getLayoutHeight();

    if (width <= 0 || height <= 0)
        return;

    // 保存Cairo状态
    cairo_save(cr);

    // 绘制背景
    drawBackgroundCairo(cr, x, y, width, height);

    // 绘制边框
    drawBorderCairo(cr, x, y, width, height);

    // 绘制文本
    drawTextCairo(cr, x, y, width, height);

    // 恢复Cairo状态
    cairo_restore(cr);
}

void SContainer::onMeasure(float width, float height, float &measuredWidth, float &measuredHeight)
{
    measuredWidth = 0;
    measuredHeight = 0;

    // 如果有文本内容，测量文本尺寸
    if (m_hasTextContent)
    {
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
    if (width == YGUndefined || width <= 0)
    {
        // 使用测量的宽度
    }
    else
    {
        measuredWidth = width;
    }

    if (height == YGUndefined || height <= 0)
    {
        // 使用测量的高度
    }
    else
    {
        measuredHeight = height;
    }
}

// ====================================================================
// 样式管理实现
// ====================================================================

void SContainer::applyStyles()
{
    // 这里可以添加样式应用的逻辑
    // 例如：通知渲染系统更新样式
    markStylesDirty();
}

void SContainer::resetStyles()
{
    clearBackground();
    clearBorderStyle();
    clearTextStyle();

    // 重置到默认值
    m_fontSize = 14.0f;
    m_fontFamily = SGUI_DEFAULT_FONT_FAMILY;
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

bool SContainer::hasBackground() const
{
    return (m_backgroundColor.a > 0) || m_hasBackgroundImage || m_hasBackgroundGradient;
}

bool SContainer::hasBorderStyle() const
{
    return (m_borderColor.a > 0) || (m_borderStyle != BorderStyle::Solid) ||
           (m_borderRadius.left.value > 0 || m_borderRadius.top.value > 0 || m_borderRadius.right.value > 0 || m_borderRadius.bottom.value > 0) ||
           (m_boxShadow.blurRadius > 0 || m_boxShadow.spreadRadius > 0 || m_boxShadow.offsetX != 0 || m_boxShadow.offsetY != 0 || m_boxShadow.color.a > 0);
}

bool SContainer::hasTextStyle() const
{
    return (m_textColor.a > 0) || m_hasTextContent || m_fontSize != 14.0f || m_fontFamily != SGUI_DEFAULT_FONT_FAMILY || m_fontWeight != FontWeight::Normal ||
           m_fontStyle != FontStyle::Normal || m_textAlign != TextAlign::Left || m_textDecoration != TextDecoration::None || m_textOverflow != TextOverflow::Clip ||
           m_lineHeight != 1.2f || m_textIndent != 0.0f;
}

// ====================================================================
// 私有辅助函数实现
// ====================================================================

void SContainer::measureText(float &width, float &height)
{
    if (!m_hasTextContent)
    {
        width = 0;
        height = 0;
        return;
    }

    // 简单的文本尺寸估算（实际实现需要使用字体库）
    float charWidth = m_fontSize * 0.6f;          // 估算字符宽度
    float charHeight = m_fontSize * m_lineHeight; // 考虑行高

    // 计算文本行数
    size_t lineCount = 1;
    for (char c : m_text)
    {
        if (c == '\n')
            lineCount++;
    }

    // 估算文本宽度（最长的行）
    size_t maxLineLength = 0;
    size_t currentLineLength = 0;
    for (char c : m_text)
    {
        if (c == '\n')
        {
            maxLineLength = std::max(maxLineLength, currentLineLength);
            currentLineLength = 0;
        }
        else
        {
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

// 创建圆角矩形路径的辅助函数
void SContainer::createRoundedRectanglePath(cairo_t *cr, float x, float y, float width, float height, float radius)
{
    if (radius <= 0)
    {
        cairo_rectangle(cr, x, y, width, height);
        return;
    }

    // 确保半径不会超过矩形的一半
    float maxRadius = std::min(width / 2.0f, height / 2.0f);
    radius = std::min(radius, maxRadius);

    cairo_move_to(cr, x + radius, y);
    cairo_line_to(cr, x + width - radius, y);
    cairo_arc(cr, x + width - radius, y + radius, radius, -M_PI / 2, 0);
    cairo_line_to(cr, x + width, y + height - radius);
    cairo_arc(cr, x + width - radius, y + height - radius, radius, 0, M_PI / 2);
    cairo_line_to(cr, x + radius, y + height);
    cairo_arc(cr, x + radius, y + height - radius, radius, M_PI / 2, M_PI);
    cairo_line_to(cr, x, y + radius);
    cairo_arc(cr, x + radius, y + radius, radius, M_PI, -M_PI / 2);
    cairo_close_path(cr);
}

// 创建复杂圆角矩形路径（支持不同方向的圆角）
void SContainer::createComplexRoundedRectanglePath(cairo_t *cr, float x, float y, float width, float height)
{
    float radiusTL = m_borderRadius.top.value;
    float radiusTR = m_borderRadius.right.value;
    float radiusBR = m_borderRadius.bottom.value;
    float radiusBL = m_borderRadius.left.value;

    // 参考 drawBorderCairo 的实现方式，从左上角开始
    cairo_move_to(cr, x + radiusTL, y);

    // 上边线到右上角圆弧的起点
    cairo_line_to(cr, x + width - radiusTR, y);

    // 右上角圆弧
    if (radiusTR > 0)
    {
        cairo_arc(cr, x + width - radiusTR, y + radiusTR, radiusTR, -M_PI / 2, 0);
    }

    // 右边线到右下角圆弧的起点
    cairo_line_to(cr, x + width, y + height - radiusBR);

    // 右下角圆弧
    if (radiusBR > 0)
    {
        cairo_arc(cr, x + width - radiusBR, y + height - radiusBR, radiusBR, 0, M_PI / 2);
    }

    // 下边线到左下角圆弧的起点
    cairo_line_to(cr, x + radiusBL, y + height);

    // 左下角圆弧
    if (radiusBL > 0)
    {
        cairo_arc(cr, x + radiusBL, y + height - radiusBL, radiusBL, M_PI / 2, M_PI);
    }

    // 左边线到左上角圆弧的起点
    cairo_line_to(cr, x, y + radiusTL);

    // 左上角圆弧
    if (radiusTL > 0)
    {
        cairo_arc(cr, x + radiusTL, y + radiusTL, radiusTL, M_PI, -M_PI / 2);
    }

    cairo_close_path(cr);
}

// 设置背景源
void SContainer::setupBackgroundSource(cairo_t *cr, float x, float y, float width, float height)
{
    if (m_hasBackgroundGradient && !m_backgroundGradient.stops.empty())
    {
        // 简单的线性渐变实现（后续可以扩展为更复杂的渐变）
        cairo_pattern_t *gradient = cairo_pattern_create_linear(x, y, x + width, y + height);

        for (const auto &stop : m_backgroundGradient.stops)
        {
            cairo_pattern_add_color_stop_rgba(gradient, stop.position, stop.color.r, stop.color.g, stop.color.b, stop.color.a);
        }

        cairo_set_source(cr, gradient);
        m_currentPattern = gradient;
    }
    else if (m_hasBackgroundImage && !m_backgroundImage.empty())
    {
        // 实现图片背景绘制
        // 检查图片路径是否有效
        if (!m_backgroundImage.empty())
        {
            // 尝试加载图片并创建Cairo表面
            cairo_surface_t *image_surface = cairo_image_surface_create_from_png(m_backgroundImage.c_str());
            if (cairo_surface_status(image_surface) == CAIRO_STATUS_SUCCESS)
            {
                // 创建图片图案
                cairo_pattern_t *pattern = cairo_pattern_create_for_surface(image_surface);

                // 设置图片缩放模式
                cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);

                cairo_set_source(cr, pattern);
                m_currentPattern = pattern;
                m_currentSurface = image_surface;
            }
        }
    }
    else if (m_backgroundColor.a > 0)
    {
        // 纯色背景
        cairo_set_source_rgba(cr, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
        m_currentPattern = nullptr;
        m_currentSurface = nullptr;
    }
}

// 清理背景资源
void SContainer::cleanupBackgroundSource()
{
    if (m_currentPattern)
    {
        cairo_pattern_destroy(m_currentPattern);
        m_currentPattern = nullptr;
    }
    if (m_currentSurface)
    {
        cairo_surface_destroy(m_currentSurface);
        m_currentSurface = nullptr;
    }
}

// 检查是否有圆角
bool SContainer::hasBorderRadius() const
{
    return (m_borderRadius.left.value > 0 || m_borderRadius.top.value > 0 || m_borderRadius.right.value > 0 || m_borderRadius.bottom.value > 0);
}

void SContainer::drawBackgroundCairo(cairo_t *cr, float x, float y, float width, float height)
{
    if (!hasBackground())
        return;

    // 获取边框信息用于背景绘制
    float borderLeft = getLayoutBorderLeft();
    float borderTop = getLayoutBorderTop();
    float borderRight = getLayoutBorderRight();
    float borderBottom = getLayoutBorderBottom();

    // 计算背景区域（排除边框）
    float bgX = x + borderLeft;
    float bgY = y + borderTop;
    float bgWidth = width - borderLeft - borderRight;
    float bgHeight = height - borderTop - borderBottom;

    if (bgWidth <= 0 || bgHeight <= 0)
        return;

    // 设置背景源
    setupBackgroundSource(cr, bgX, bgY, bgWidth, bgHeight);

    // 使用统一的圆角路径（与边框一致）
    if (hasBorderRadius())
    {
        // 使用复杂圆角路径支持不同方向的圆角
        createComplexRoundedRectanglePath(cr, bgX, bgY, bgWidth, bgHeight);
    }
    else
    {
        // 矩形背景
        cairo_rectangle(cr, bgX, bgY, bgWidth, bgHeight);
    }

    cairo_fill(cr);

    // 清理资源
    cleanupBackgroundSource();
}

void SContainer::drawBorderCairo(cairo_t *cr, float x, float y, float width, float height)
{
    if (!hasBorderStyle())
        return;

    // 边框应该绘制在整个容器的外层，包含padding但不包含margin
    // 边框路径应该围绕整个content+padding区域
    float borderX = x;
    float borderY = y;
    float borderRectWidth = width;
    float borderRectHeight = height;

    if (borderRectWidth <= 0 || borderRectHeight <= 0)
        return;

    // 获取边框宽度
    float borderLeft = getLayoutBorderLeft();
    float borderTop = getLayoutBorderTop();
    float borderRight = getLayoutBorderRight();
    float borderBottom = getLayoutBorderBottom();

    if (borderLeft == 0.0 && borderTop == 0.0 && borderRight == 0.0 && borderBottom == 0.0)
    {
        return;
    }

    // 设置边框颜色
    if (m_borderColor.a > 0)
    {
        cairo_set_source_rgba(cr, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
    }
    else
    {
        cairo_set_source_rgba(cr, 0, 0, 0, 1); // 默认黑色
    }

    // 线条末端形态
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    // 设置边框样式
    if (m_borderStyle != BorderStyle::Solid)
    {
        // 使用静态数组避免内存泄漏
        static double dashed_pattern[2] = {5.0, 5.0};
        static double dotted_pattern[2] = {2.0, 2.0};

        switch (m_borderStyle)
        {
        case BorderStyle::Dashed:
            cairo_set_dash(cr, dashed_pattern, 2, 0);
            break;
        case BorderStyle::Dotted:
            cairo_set_dash(cr, dotted_pattern, 2, 0);
            break;
        default:
            cairo_set_dash(cr, nullptr, 0, 0); // 实线
            break;
        }
    }

    // 检查是否所有边框宽度相同
    bool uniformBorder = (borderLeft == borderTop && borderTop == borderRight && borderRight == borderBottom);

    if (uniformBorder)
    {
        // 所有边框宽度相同，使用原有逻辑
        // 调整边框绘制区域，确保边框不会超出容器边界
        float halfBorder = borderLeft / 2.0f;
        float adjustedX = borderX + halfBorder;
        float adjustedY = borderY + halfBorder;
        float adjustedWidth = borderRectWidth - borderLeft;
        float adjustedHeight = borderRectHeight - borderLeft;

        if (adjustedWidth <= 0 || adjustedHeight <= 0)
            return;
        cairo_set_line_width(cr, borderLeft);
        // 绘制边框 - 使用统一的圆角路径
        if (hasBorderRadius())
        {
            createComplexRoundedRectanglePath(cr, adjustedX, adjustedY, adjustedWidth, adjustedHeight);
        }
        else
        {
            cairo_rectangle(cr, adjustedX, adjustedY, adjustedWidth, adjustedHeight);
        }
        cairo_stroke(cr);
    }
    else
    {
        // 边框宽度不同，使用stroke绘制四条边，支持圆角
        float radiusTL = m_borderRadius.top.value;
        float radiusTR = m_borderRadius.right.value;
        float radiusBR = m_borderRadius.bottom.value;
        float radiusBL = m_borderRadius.left.value;

        // 上边
        if (borderTop > 0)
        {
            cairo_set_line_width(cr, borderTop);
            cairo_move_to(cr, borderX + radiusTL, borderY + borderTop / 2);
            cairo_line_to(cr, borderX + borderRectWidth - radiusTR, borderY + borderTop / 2);
            if (radiusTR > 0)
            {
                // cairo_arc(cr, borderX + borderRectWidth - radiusTR, borderY + radiusTR, radiusTR, -M_PI / 2, 0);
            }
            cairo_stroke(cr);
        }

        // 右边
        if (borderRight > 0)
        {
            cairo_set_line_width(cr, borderRight);
            cairo_move_to(cr, borderX + borderRectWidth - borderRight / 2, borderY + radiusTR);
            cairo_line_to(cr, borderX + borderRectWidth - borderRight / 2, borderY + borderRectHeight - radiusBR);
            if (radiusBR > 0)
            {
                // cairo_arc(cr, borderX + borderRectWidth - radiusBR, borderY + borderRectHeight - radiusBR, radiusBR, 0, M_PI / 2);
            }
            cairo_stroke(cr);
        }

        // 下边
        if (borderBottom > 0)
        {
            cairo_set_line_width(cr, borderBottom);
            cairo_move_to(cr, borderX + borderRectWidth - radiusBR, borderY + borderRectHeight - borderBottom / 2);
            cairo_line_to(cr, borderX + radiusBL, borderY + borderRectHeight - borderBottom / 2);
            if (radiusBL > 0)
            {
                // cairo_arc(cr, borderX + radiusBL, borderY + borderRectHeight - radiusBL, radiusBL, M_PI / 2, M_PI);
            }
            cairo_stroke(cr);
        }

        // 左边
        if (borderLeft > 0)
        {
            cairo_set_line_width(cr, borderLeft);
            cairo_move_to(cr, borderX + borderLeft / 2, borderY + borderRectHeight - radiusBL);
            cairo_line_to(cr, borderX + borderLeft / 2, borderY + radiusTL);
            if (radiusTL > 0)
            {
                // cairo_arc(cr, borderX + radiusTL, borderY + radiusTL, radiusTL, M_PI, -M_PI / 2);
            }
            cairo_stroke(cr);
        }
    }

    // 重置虚线设置
    cairo_set_dash(cr, nullptr, 0, 0);
}

void SContainer::drawTextCairo(cairo_t *cr, float x, float y, float width, float height)
{
    if (!m_hasTextContent || m_text.empty())
        return;

    // 考虑内边距和边框
    float paddingLeft = getLayoutPaddingLeft();
    float paddingRight = getLayoutPaddingRight();
    float paddingTop = getLayoutPaddingTop();
    float paddingBottom = getLayoutPaddingBottom();

    float borderLeft = getLayoutBorderLeft();
    float borderRight = getLayoutBorderRight();
    float borderTop = getLayoutBorderTop();
    float borderBottom = getLayoutBorderBottom();

    float textAreaX = x + borderLeft + paddingLeft;
    float textAreaY = y + borderTop + paddingTop;
    float textAreaWidth = width - borderLeft - borderRight - paddingLeft - paddingRight;
    float textAreaHeight = height - borderTop - borderBottom - paddingTop - paddingBottom;

    if (textAreaWidth <= 0 || textAreaHeight <= 0)
        return;

    // 设置文本颜色
    if (m_textColor.a > 0)
    {
        cairo_set_source_rgba(cr, m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a);
    }
    else
    {
        cairo_set_source_rgba(cr, 0, 0, 0, 1); // 默认黑色
    }

    // 设置字体
    cairo_select_font_face(cr, m_fontFamily.c_str(), (m_fontStyle == FontStyle::Italic) ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                           (m_fontWeight >= FontWeight::Bold) ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, m_fontSize);

    // 处理多行文本
    std::vector<std::string> lines;
    std::string currentLine;
    for (char c : m_text)
    {
        if (c == '\n')
        {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else
        {
            currentLine += c;
        }
    }
    if (!currentLine.empty())
    {
        lines.push_back(currentLine);
    }

    // 绘制每一行文本
    float lineHeight = m_fontSize * m_lineHeight;
    float startY = textAreaY + m_fontSize; // 从字体大小开始计算

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const std::string &line = lines[i];
        if (line.empty())
        {
            startY += lineHeight;
            continue;
        }

        // 计算文本位置
        cairo_text_extents_t extents;
        cairo_text_extents(cr, line.c_str(), &extents);

        float textX = textAreaX + m_textIndent;
        float textY = startY + i * lineHeight;

        // 根据文本对齐方式调整X坐标
        switch (m_textAlign)
        {
        case TextAlign::Center:
            textX = textAreaX + (textAreaWidth - extents.width) / 2.0f + m_textIndent;
            break;
        case TextAlign::Right:
            textX = textAreaX + textAreaWidth - extents.width - m_textIndent;
            break;
        case TextAlign::Justify:
            // 简化处理：左对齐
            textX = textAreaX + m_textIndent;
            break;
        case TextAlign::Left:
        default:
            textX = textAreaX + m_textIndent;
            break;
        }

        // 应用文本装饰
        if (m_textDecoration == TextDecoration::Underline)
        {
            // 绘制下划线
            cairo_move_to(cr, textX, textY + 2);
            cairo_line_to(cr, textX + extents.width, textY + 2);
            cairo_set_line_width(cr, 1.0);
            cairo_stroke(cr);
        }
        else if (m_textDecoration == TextDecoration::LineThrough)
        {
            // 绘制删除线
            cairo_move_to(cr, textX, textY - extents.height / 2);
            cairo_line_to(cr, textX + extents.width, textY - extents.height / 2);
            cairo_set_line_width(cr, 1.0);
            cairo_stroke(cr);
        }

        // 绘制文本
        cairo_move_to(cr, textX, textY);
        cairo_show_text(cr, line.c_str());
    }
}

void SContainer::markStylesDirty()
{
    m_stylesDirty = true;
    // 标记需要重新绘制
    markDirty();
}

} // namespace sgui
