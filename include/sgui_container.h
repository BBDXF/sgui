/**
 * Container.h - GUI容器的基类
 * 
 * 基于Yoga Flexbox布局引擎的GUI容器基类，提供完整的布局功能
 * 所有GUI组件都应该继承自这个基类
 */

#pragma once

#include <yoga/YGNode.h>
#include <yoga/YGNodeStyle.h>
#include <yoga/YGNodeLayout.h>
#include <yoga/YGEnums.h>
#include <yoga/YGValue.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace sgui {

// 前向声明
class Container;

// 智能指针类型定义
using ContainerPtr = std::shared_ptr<Container>;
using ContainerWeakPtr = std::weak_ptr<Container>;

/**
 * 布局值类型，支持点、百分比、自动等不同单位
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
 * 边距/内边距设置
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
 * Container基类 - 所有GUI组件的基础
 */
class Container : public std::enable_shared_from_this<Container> {
public:
    // Flex方向枚举
    enum class FlexDirection {
        Row = YGFlexDirectionRow,
        RowReverse = YGFlexDirectionRowReverse,
        Column = YGFlexDirectionColumn,
        ColumnReverse = YGFlexDirectionColumnReverse
    };
    
    // 对齐方式枚举
    enum class Align {
        Auto = YGAlignAuto,
        FlexStart = YGAlignFlexStart,
        Center = YGAlignCenter,
        FlexEnd = YGAlignFlexEnd,
        Stretch = YGAlignStretch,
        Baseline = YGAlignBaseline,
        SpaceBetween = YGAlignSpaceBetween,
        SpaceAround = YGAlignSpaceAround,
        SpaceEvenly = YGAlignSpaceEvenly
    };
    
    // 位置类型枚举
    enum class PositionType {
        Static = YGPositionTypeStatic,
        Relative = YGPositionTypeRelative,
        Absolute = YGPositionTypeAbsolute
    };
    
    // 换行方式枚举
    enum class FlexWrap {
        NoWrap = YGWrapNoWrap,
        Wrap = YGWrapWrap,
        WrapReverse = YGWrapWrapReverse
    };
    
    // 溢出处理枚举
    enum class Overflow {
        Visible = YGOverflowVisible,
        Hidden = YGOverflowHidden,
        Scroll = YGOverflowScroll
    };
    
    // 显示类型枚举
    enum class Display {
        Flex = YGDisplayFlex,
        None = YGDisplayNone
    };
    
    // 方向枚举
    enum class Direction {
        Inherit = YGDirectionInherit,
        LTR = YGDirectionLTR,
        RTL = YGDirectionRTL
    };

public:
    Container();
    virtual ~Container();
    
    // 禁用拷贝构造和赋值
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    
    // ====================================================================
    // 子节点管理
    // ====================================================================
    
    /**
     * 添加子节点
     */
    void addChild(const ContainerPtr& child);
    
    /**
     * 在指定位置插入子节点
     */
    void insertChild(const ContainerPtr& child, size_t index);
    
    /**
     * 移除子节点
     */
    void removeChild(const ContainerPtr& child);
    
    /**
     * 移除所有子节点
     */
    void removeAllChildren();
    
    /**
     * 获取子节点数量
     */
    size_t getChildCount() const;
    
    /**
     * 获取指定索引的子节点
     */
    ContainerPtr getChildAt(size_t index) const;
    
    /**
     * 获取父节点
     */
    ContainerPtr getParent() const { return m_parent.lock(); }
    
    // ====================================================================
    // 布局属性设置
    // ====================================================================
    
    // --- 尺寸设置 ---
    void setWidth(const LayoutValue& width);
    void setHeight(const LayoutValue& height);
    void setMinWidth(const LayoutValue& minWidth);
    void setMinHeight(const LayoutValue& minHeight);
    void setMaxWidth(const LayoutValue& maxWidth);
    void setMaxHeight(const LayoutValue& maxHeight);
    
    LayoutValue getWidth() const;
    LayoutValue getHeight() const;
    LayoutValue getMinWidth() const;
    LayoutValue getMinHeight() const;
    LayoutValue getMaxWidth() const;
    LayoutValue getMaxHeight() const;
    
    // --- Flex属性 ---
    void setFlex(float flex);
    void setFlexGrow(float flexGrow);
    void setFlexShrink(float flexShrink);
    void setFlexBasis(const LayoutValue& flexBasis);
    
    float getFlex() const;
    float getFlexGrow() const;
    float getFlexShrink() const;
    LayoutValue getFlexBasis() const;
    
    // --- 布局方向和对齐 ---
    void setFlexDirection(FlexDirection direction);
    void setJustifyContent(Align justify);
    void setAlignItems(Align align);
    void setAlignSelf(Align align);
    void setAlignContent(Align align);
    
    FlexDirection getFlexDirection() const;
    Align getJustifyContent() const;
    Align getAlignItems() const;
    Align getAlignSelf() const;
    Align getAlignContent() const;
    
    // --- 位置和定位 ---
    void setPosition(PositionType positionType);
    void setPosition(EdgeInsets position);
    
    PositionType getPositionType() const;
    EdgeInsets getPosition() const;
    
    // --- 边距和内边距 ---
    void setMargin(const EdgeInsets& margin);
    void setPadding(const EdgeInsets& padding);
    void setBorder(const EdgeInsets& border);
    
    EdgeInsets getMargin() const;
    EdgeInsets getPadding() const;
    EdgeInsets getBorder() const;
    
    // --- 其他属性 ---
    void setFlexWrap(FlexWrap wrap);
    void setOverflow(Overflow overflow);
    void setDisplay(Display display);
    void setAspectRatio(float aspectRatio);
    void setDirection(Direction direction);
    
    FlexWrap getFlexWrap() const;
    Overflow getOverflow() const;
    Display getDisplay() const;
    float getAspectRatio() const;
    Direction getDirection() const;
    
    // ====================================================================
    // 布局计算和查询
    // ====================================================================
    
    /**
     * 计算布局
     * @param width 可用宽度，YGUndefined表示自动
     * @param height 可用高度，YGUndefined表示自动
     */
    void calculateLayout(float width = YGUndefined, float height = YGUndefined);
    
    /**
     * 获取计算后的布局位置
     */
    float getLeft() const;
    float getTop() const;
    float getRight() const;
    float getBottom() const;
    
    /**
     * 获取计算后的布局尺寸
     */
    float getLayoutWidth() const;
    float getLayoutHeight() const;
    
    /**
     * 获取计算后的边距
     */
    float getLayoutMarginLeft() const;
    float getLayoutMarginTop() const;
    float getLayoutMarginRight() const;
    float getLayoutMarginBottom() const;
    
    /**
     * 获取计算后的内边距
     */
    float getLayoutPaddingLeft() const;
    float getLayoutPaddingTop() const;
    float getLayoutPaddingRight() const;
    float getLayoutPaddingBottom() const;
    
    /**
     * 获取计算后的边框
     */
    float getLayoutBorderLeft() const;
    float getLayoutBorderTop() const;
    float getLayoutBorderRight() const;
    float getLayoutBorderBottom() const;
    
    /**
     * 检查布局是否需要重新计算
     */
    bool isDirty() const;
    
    /**
     * 标记需要重新计算布局
     */
    void markDirty();
    
    // ====================================================================
    // 虚函数接口
    // ====================================================================
    
    /**
     * 绘制函数 - 子类需要重写
     */
    virtual void draw() {}
    
    /**
     * 自定义测量函数 - 用于文本等需要测量的内容
     */
    virtual void onMeasure(float width, float height, float& measuredWidth, float& measuredHeight) {}
    
    /**
     * 布局变化回调
     */
    virtual void onLayoutChanged() {}
    
    // ====================================================================
    // 工具函数
    // ====================================================================
    
    /**
     * 打印布局树（调试用）
     */
    void printLayoutTree(int depth = 0) const;
    
    /**
     * 获取Yoga节点（高级用法）
     */
    YGNodeRef getYogaNode() const { return m_yogaNode; }
    
    /**
     * 设置用户数据
     */
    void setUserData(void* userData) { m_userData = userData; }
    
    /**
     * 获取用户数据
     */
    void* getUserData() const { return m_userData; }

protected:
    /**
     * 子类可以访问的Yoga节点
     */
    YGNodeRef m_yogaNode;
    
    /**
     * 子节点列表
     */
    std::vector<ContainerPtr> m_children;
    
    /**
     * 父节点弱引用
     */
    ContainerWeakPtr m_parent;
    
    /**
     * 用户数据指针
     */
    void* m_userData = nullptr;

private:
    /**
     * 将LayoutValue转换为YGValue
     */
    YGValue toYGValue(const LayoutValue& value) const;
    
    /**
     * 将YGValue转换为LayoutValue
     */
    LayoutValue fromYGValue(const YGValue& value) const;
    
    /**
     * 设置边距/内边距/边框的辅助函数
     */
    void setEdgeValues(void (*setter)(YGNodeRef, YGEdge, float), 
                      void (*setterPercent)(YGNodeRef, YGEdge, float),
                      const EdgeInsets& edge);
    
    /**
     * 获取边距/内边距/边框的辅助函数
     */
    EdgeInsets getEdgeValues(float (*getter)(YGNodeConstRef, YGEdge)) const;
    
    /**
     * 位置设置辅助函数
     */
    void setPositionValues(const EdgeInsets& position);
};

} // namespace sgui
