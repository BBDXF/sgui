/**
 * sgui_common.h - SGUI库的公共结构体和类型定义
 * 
 * 包含所有模块共用的结构体、枚举和类型定义，
 * 避免重复定义并提供统一的接口
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>

// font default setting
#ifdef __linux__
#define SGUI_DEFAULT_FONT_FAMILY "Noto Sans CJK"
#elif _WIN32
#define SGUI_DEFAULT_FONT_FAMILY "Microsoft YaHei"
#elif __APPLE__
#define SGUI_DEFAULT_FONT_FAMILY "PingFang SC"
#endif


namespace sgui {

/**
 * @brief 颜色结构体
 * 
 * 统一的颜色定义，支持RGBA和十六进制格式
 */
struct Color {
    double r, g, b, a;
    
    Color() : r(0.0), g(0.0), b(0.0), a(1.0) {}
    Color(double red, double green, double blue, double alpha = 1.0)
        : r(red), g(green), b(blue), a(alpha) {}
    
    // 从RGB值创建（0-255范围）
    static Color fromRGB(double r, double g, double b, double a = 255.0) {
        return Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
    }
    
    // 从十六进制颜色创建
    static Color fromHex(uint32_t hex) {
        return Color(
            ((hex >> 16) & 0xFF) / 255.0,
            ((hex >> 8) & 0xFF) / 255.0,
            (hex & 0xFF) / 255.0,
            ((hex >> 24) & 0xFF) / 255.0
        );
    }
    
    // 转换为十六进制格式
    uint32_t toHex() const {
        return 
            (static_cast<uint32_t>(a * 255) << 24) |
            (static_cast<uint32_t>(r * 255) << 16) |
            (static_cast<uint32_t>(g * 255) << 8) |
            static_cast<uint32_t>(b * 255);
    }
};

/**
 * @brief 矩形结构体
 * 
 * 表示一个矩形区域，包含位置和尺寸信息
 */
struct Rect {
    double x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(double x, double y, double w, double h) : x(x), y(y), width(w), height(h) {}
    
    bool isEmpty() const { return width <= 0 || height <= 0; }
    bool contains(double px, double py) const {
        return px >= x && px < x + width && py >= y && py < y + height;
    }
    
    Rect intersect(const Rect& other) const {
        double x1 = std::max(x, other.x);
        double y1 = std::max(y, other.y);
        double x2 = std::min(x + width, other.x + other.width);
        double y2 = std::min(y + height, other.y + other.height);
        
        if (x2 < x1 || y2 < y1) return Rect();
        return Rect(x1, y1, x2 - x1, y2 - y1);
    }
};

/**
 * @brief 布局值类型
 * 
 * 支持点、百分比、自动等不同单位的布局值
 */
struct LayoutValue {
    float value = 0.0f;
    bool isPercent = false;
    bool isAuto = false;
    
    LayoutValue() = default;
    LayoutValue(float v) : value(v) {}
    LayoutValue(float v, bool percent) : value(v), isPercent(percent) {}
    
    static LayoutValue Auto() { LayoutValue v; v.isAuto = true; return v; }
    static LayoutValue Percent(float v) { return LayoutValue(v, true); }
    static LayoutValue Point(float v) { return LayoutValue(v, false); }
};

/**
 * @brief 边距/内边距设置
 * 
 * 统一的边距设置结构体，支持分别设置四个方向
 */
struct EdgeInsets {
    LayoutValue left, top, right, bottom;
    
    EdgeInsets() = default;
    EdgeInsets(float all) : left(all), top(all), right(all), bottom(all) {}
    EdgeInsets(float horizontal, float vertical) 
        : left(horizontal), top(vertical), right(horizontal), bottom(vertical) {}
    EdgeInsets(float l, float t, float r, float b) 
        : left(l), top(t), right(r), bottom(b) {}
    
    static EdgeInsets All(float value) { return EdgeInsets(value); }
    static EdgeInsets Horizontal(float value) { return EdgeInsets(value, 0); }
    static EdgeInsets Vertical(float value) { return EdgeInsets(0, value); }
    static EdgeInsets Symmetric(float horizontal, float vertical) { return EdgeInsets(horizontal, vertical); }
    static EdgeInsets Only(float left, float top, float right, float bottom) { return EdgeInsets(left, top, right, bottom); }
};

/**
 * @brief 渐变类型枚举
 */
enum class GradientType {
    Linear,    // 线性渐变
    Radial     // 径向渐变
};

/**
 * @brief 渐变停止点
 */
struct GradientStop {
    Color color;
    float position;  // 位置 [0-1]
    
    GradientStop() = default;
    GradientStop(const Color& c, float pos) : color(c), position(pos) {}
};

/**
 * @brief 渐变背景
 */
struct BackgroundGradient {
    GradientType type = GradientType::Linear;
    std::vector<GradientStop> stops;
    float angle = 0.0f;  // 线性渐变角度（度）
    
    BackgroundGradient() = default;
    BackgroundGradient(GradientType t) : type(t) {}
};

/**
 * @brief 边框样式枚举
 */
enum class BorderStyle {
    Solid,      // 实线
    Dashed,     // 虚线
    Dotted,     // 点线
    Double,     // 双线
    Groove,     // 凹槽
    Ridge,      // 脊状
    Inset,      // 内嵌
    Outset      // 外嵌
};

/**
 * @brief 阴影结构体
 */
struct BoxShadow {
    Color color = Color(0, 0, 0, 0.5f);  // 阴影颜色
    float offsetX = 0.0f;                // X轴偏移
    float offsetY = 0.0f;                // Y轴偏移
    float blurRadius = 0.0f;             // 模糊半径
    float spreadRadius = 0.0f;           // 扩散半径
    bool inset = false;                  // 是否为内阴影
    
    BoxShadow() = default;
    BoxShadow(const Color& c, float x, float y, float blur = 0.0f, float spread = 0.0f, bool in = false)
        : color(c), offsetX(x), offsetY(y), blurRadius(blur), spreadRadius(spread), inset(in) {}
};

/**
 * @brief 字体粗细枚举
 */
enum class FontWeight {
    Thin = 100,
    ExtraLight = 200,
    Light = 300,
    Normal = 400,
    Medium = 500,
    SemiBold = 600,
    Bold = 700,
    ExtraBold = 800,
    Black = 900
};

/**
 * @brief 字体样式枚举
 */
enum class FontStyle {
    Normal,
    Italic,
    Oblique
};

/**
 * @brief 文本对齐枚举
 */
enum class TextAlign {
    Left,
    Center,
    Right,
    Justify
};

/**
 * @brief 文本装饰枚举
 */
enum class TextDecoration {
    None,
    Underline,
    Overline,
    LineThrough
};

/**
 * @brief 文本溢出处理枚举
 */
enum class TextOverflow {
    Clip,        // 直接裁剪
    Ellipsis,    // 显示省略号
    Fade         // 淡出效果
};

// =============================================================================
// Yoga Flexbox 布局相关枚举
// =============================================================================

/**
 * @brief Flex方向枚举
 * 
 * 对应Yoga的YGFlexDirection枚举，定义flex容器的主轴方向
 */
enum class FlexDirection {
    Row = 0,            // 主轴为水平方向，起点在左端
    RowReverse = 1,       // 主轴为水平方向，起点在右端
    Column = 2,          // 主轴为垂直方向，起点在上沿
    ColumnReverse = 3      // 主轴为垂直方向，起点在下沿
};

/**
 * @brief 对齐方式枚举
 * 
 * 对应Yoga的YGAlign枚举，定义项目在交叉轴上的对齐方式
 */
enum class Align {
    Auto = 0,            // 自动选择对齐方式
    FlexStart = 1,         // 对齐到交叉轴的起点
    Center = 2,            // 在交叉轴上居中对齐
    FlexEnd = 3,           // 对齐到交叉轴的终点
    Stretch = 4,           // 拉伸以填充容器
    Baseline = 5,          // 沿基线对齐
    SpaceBetween = 6,        // 项目之间的间隔相等
    SpaceAround = 7,         // 项目周围的间隔相等
    SpaceEvenly = 8         // 项目之间的间隔相等，包括两端
};

/**
 * @brief 位置类型枚举
 * 
 * 对应Yoga的YGPositionType枚举，定义元素的定位方式
 */
enum class PositionType {
    Static = 0,      // 正常文档流中的位置
    Relative = 1,    // 相对于其正常位置的位置
    Absolute = 2      // 相对于其包含块的位置
};

/**
 * @brief 换行方式枚举
 * 
 * 对应Yoga的YGWrap枚举，定义flex容器是否换行
 */
enum class FlexWrap {
    NoWrap = 0,      // 不换行，所有项目在一行
    Wrap = 1,         // 换行，项目从上到下排列
    WrapReverse = 2    // 换行，项目从下到上排列
};

/**
 * @brief 溢出处理枚举
 * 
 * 对应Yoga的YGOverflow枚举，定义内容溢出容器时的处理方式
 */
enum class Overflow {
    Visible = 0,      // 内容溢出时仍然可见
    Hidden = 1,       // 内容溢出时隐藏
    Scroll = 2         // 内容溢出时显示滚动条
};

/**
 * @brief 显示类型枚举
 * 
 * 对应Yoga的YGDisplay枚举，定义元素的显示类型
 */
enum class Display {
    Flex = 0,         // 作为flex容器显示
    None = 1           // 不显示元素
};

/**
 * @brief 方向枚举
 * 
 * 对应Yoga的YGDirection枚举，定义文本的书写方向
 */
enum class Direction {
    Inherit = 0,      // 继承父元素的方向
    LTR = 1,          // 从左到右的方向
    RTL = 2           // 从右到左的方向
};

} // namespace sgui
