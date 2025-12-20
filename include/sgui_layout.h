/**
 * GUI Layout类
 * 
 * 基于Yoga Flexbox布局引擎的GUI容器基类，提供完整的布局功能
 * 这里只实现Layout功能，具体绘制功能由子类实现
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
#include "sgui_common.h"
#include <cairo/cairo.h>

namespace sgui {

// 前向声明
class SLayout;

// 智能指针类型定义
using SLayoutPtr = std::shared_ptr<SLayout>;
using SLayoutWeakPtr = std::weak_ptr<SLayout>;

/**
 * Container基类 - 所有GUI组件的基础
 */
class SLayout : public std::enable_shared_from_this<SLayout> {
public:

public:
    SLayout();
    virtual ~SLayout();
    
    // 禁用拷贝构造和赋值
    SLayout(const SLayout&) = delete;
    SLayout& operator=(const SLayout&) = delete;
    
    // ====================================================================
    // 子节点管理
    // ====================================================================
    
    /**
     * 添加子节点
     */
    void addChild(const SLayoutPtr& child);
    
    /**
     * 在指定位置插入子节点
     */
    void insertChild(const SLayoutPtr& child, size_t index);
    
    /**
     * 移除子节点
     */
    void removeChild(const SLayoutPtr& child);
    
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
    SLayoutPtr getChildAt(size_t index) const;
    
    /**
     * 获取父节点
     */
    SLayoutPtr getParent() const { return m_parent.lock(); }
    
    // ====================================================================
    // 布局属性设置
    // ====================================================================
    
    // --- 尺寸设置 ---
    /**
     * 设置容器的宽度
     * 在布局中定义容器的水平尺寸，可以是固定值、百分比或自动
     * @param width 宽度值，支持像素、百分比、自动等模式
     */
    void setWidth(const LayoutValue& width);
    
    /**
     * 设置容器的高度
     * 在布局中定义容器的垂直尺寸，可以是固定值、百分比或自动
     * @param height 高度值，支持像素、百分比、自动等模式
     */
    void setHeight(const LayoutValue& height);
    
    /**
     * 设置容器的最小宽度
     * 在布局中限制容器的最小水平尺寸，防止容器收缩到小于指定值
     * @param minWidth 最小宽度值，支持像素、百分比等模式
     */
    void setMinWidth(const LayoutValue& minWidth);
    
    /**
     * 设置容器的最小高度
     * 在布局中限制容器的最小垂直尺寸，防止容器收缩到小于指定值
     * @param minHeight 最小高度值，支持像素、百分比等模式
     */
    void setMinHeight(const LayoutValue& minHeight);
    
    /**
     * 设置容器的最大宽度
     * 在布局中限制容器的最大水平尺寸，防止容器扩展超过指定值
     * @param maxWidth 最大宽度值，支持像素、百分比等模式
     */
    void setMaxWidth(const LayoutValue& maxWidth);
    
    /**
     * 设置容器的最大高度
     * 在布局中限制容器的最大垂直尺寸，防止容器扩展超过指定值
     * @param maxHeight 最大高度值，支持像素、百分比等模式
     */
    void setMaxHeight(const LayoutValue& maxHeight);
    
    LayoutValue getWidth() const;
    LayoutValue getHeight() const;
    LayoutValue getMinWidth() const;
    LayoutValue getMinHeight() const;
    LayoutValue getMaxWidth() const;
    LayoutValue getMaxHeight() const;
    
    // --- Flex属性 ---
    /**
     * 设置Flex属性的简写
     * 在布局中同时设置flex-grow、flex-shrink和flex-basis三个属性
     * 控制容器在Flex容器中的伸缩行为和基础尺寸
     * @param flex Flex值，影响容器的伸缩比例
     */
    void setFlex(float flex);
    
    /**
     * 设置Flex增长系数
     * 在布局中定义容器在Flex容器中的放大比例
     * 当容器有剩余空间时，按此比例分配空间
     * @param flexGrow 增长系数，默认为0，负值无效
     */
    void setFlexGrow(float flexGrow);
    
    /**
     * 设置Flex收缩系数
     * 在布局中定义容器在Flex容器中的缩小比例
     * 当容器空间不足时，按此比例缩小容器
     * @param flexShrink 收缩系数，默认为1，负值无效
     */
    void setFlexShrink(float flexShrink);
    
    /**
     * 设置Flex基础尺寸
     * 在布局中定义容器在分配剩余空间之前的基础尺寸
     * 可以是固定值、百分比或auto
     * @param flexBasis 基础尺寸值
     */
    void setFlexBasis(const LayoutValue& flexBasis);
    
    float getFlex() const;
    float getFlexGrow() const;
    float getFlexShrink() const;
    LayoutValue getFlexBasis() const;
    
    // --- 布局方向和对齐 ---
    /**
     * 设置Flex方向
     * 在布局中定义子元素的排列方向（水平或垂直）
     * 控制主轴的方向，影响子元素的排列方式
     * @param direction Flex方向：ROW(水平)、COLUMN(垂直)等
     */
    void setFlexDirection(FlexDirection direction);
    
    /**
     * 设置主轴对齐方式
     * 在布局中定义子元素在主轴上的对齐方式
     * 控制子元素在主轴方向上的分布和间距
     * @param justify 主轴对齐方式：FLEX_START、CENTER、FLEX_END、SPACE_BETWEEN等
     */
    void setJustifyContent(Align justify);
    
    /**
     * 设置交叉轴对齐方式
     * 在布局中定义子元素在交叉轴上的对齐方式
     * 控制所有子元素在交叉轴方向上的对齐
     * @param align 交叉轴对齐方式：FLEX_START、CENTER、FLEX_END、STRETCH等
     */
    void setAlignItems(Align align);
    
    /**
     * 设置单个容器的交叉轴对齐方式
     * 在布局中覆盖父容器的align-items设置
     * 只影响当前容器在交叉轴上的对齐方式
     * @param align 交叉轴对齐方式：FLEX_START、CENTER、FLEX_END、STRETCH、AUTO等
     */
    void setAlignSelf(Align align);
    
    /**
     * 设置多行容器的交叉轴对齐方式
     * 在布局中定义多行Flex容器中行的对齐方式
     * 当flex-wrap为wrap时，控制多行在交叉轴上的分布
     * @param align 多行对齐方式：FLEX_START、CENTER、FLEX_END、STRETCH、SPACE_BETWEEN等
     */
    void setAlignContent(Align align);
    
    FlexDirection getFlexDirection() const;
    Align getJustifyContent() const;
    Align getAlignItems() const;
    Align getAlignSelf() const;
    Align getAlignContent() const;
    
    // --- 位置和定位 ---
    /**
     * 设置定位类型
     * 在布局中定义容器的定位方式（相对定位或绝对定位）
     * 影响容器在父容器中的位置计算方式
     * @param positionType 定位类型：RELATIVE(相对)、ABSOLUTE(绝对)
     */
    void setPosition(PositionType positionType);
    
    /**
     * 设置容器的位置偏移
     * 在布局中定义容器相对于父容器的位置偏移量
     * 支持上、右、下、左四个方向的独立设置
     * @param position 位置偏移值，包含top、right、bottom、left四个方向
     */
    void setPosition(EdgeInsets position);
    
    PositionType getPositionType() const;
    EdgeInsets getPosition() const;
    
    // --- 边距和内边距 ---
    /**
     * 设置外边距
     * 在布局中定义容器与相邻元素之间的空间
     * 影响容器与其他元素的距离和整体布局间距
     * @param margin 外边距值，包含top、right、bottom、left四个方向
     */
    void setMargin(const EdgeInsets& margin);
    
    /**
     * 设置内边距
     * 在布局中定义容器内容与边框之间的空间
     * 影响容器内部内容的显示区域和布局
     * @param padding 内边距值，包含top、right、bottom、left四个方向
     */
    void setPadding(const EdgeInsets& padding);
    
    /**
     * 设置边框宽度
     * 在布局中定义容器边框的宽度
     * 影响容器的总尺寸和内部内容的可用空间
     * @param border 边框宽度值，包含top、right、bottom、left四个方向
     */
    void setBorder(const EdgeInsets& border);
    
    EdgeInsets getMargin() const;
    EdgeInsets getPadding() const;
    EdgeInsets getBorder() const;
    
    // --- 其他属性 ---
    /**
     * 设置Flex换行方式
     * 在布局中定义子元素是否允许换行
     * 控制当容器空间不足时子元素的排列方式
     * @param wrap 换行方式：NOWRAP(不换行)、WRAP(换行)、WRAP_REVERSE(反向换行)
     */
    void setFlexWrap(FlexWrap wrap);
    
    /**
     * 设置溢出处理方式
     * 在布局中定义当内容超出容器尺寸时的处理方式
     * 控制是否显示滚动条或裁剪超出内容
     * @param overflow 溢出方式：VISIBLE(可见)、HIDDEN(隐藏)、SCROLL(滚动)
     */
    void setOverflow(Overflow overflow);
    
    /**
     * 设置显示类型
     * 在布局中定义容器的显示类型
     * 控制容器是否参与布局计算和显示
     * @param display 显示类型：FLEX(Flex布局)、NONE(不显示)
     */
    void setDisplay(Display display);
    
    /**
     * 设置宽高比
     * 在布局中定义容器的宽高比例
     * 用于保持容器的特定比例，常用于图片和视频元素
     * @param aspectRatio 宽高比例值，宽度/高度
     */
    void setAspectRatio(float aspectRatio);
    
    /**
     * 设置文本方向
     * 在布局中定义文本的书写方向
     * 影响文本的排列方向和对齐方式
     * @param direction 文本方向：INHERIT(继承)、LTR(从左到右)、RTL(从右到左)
     */
    void setDirection(Direction direction);
    
    // --- Gap 属性 ---
    /**
     * 设置间距
     * 在布局中定义子元素之间的间距
     * @param gutter 间距类型：COLUMN(列间距)、ROW(行间距)、ALL(所有间距)
     * @param gap 间距值，支持像素、百分比等
     */
    void setGap(Gutter gutter, const LayoutValue& gap);
    
    /**
     * 设置列间距
     * 在布局中定义子元素在列方向（水平）的间距
     * @param gap 列间距值
     */
    void setColumnGap(const LayoutValue& gap);
    
    /**
     * 设置行间距
     * 在布局中定义子元素在行方向（垂直）的间距
     * @param gap 行间距值
     */
    void setRowGap(const LayoutValue& gap);
    
    /**
     * 设置所有方向的间距
     * 在布局中定义子元素在所有方向的间距
     * @param gap 间距值
     */
    void setGapAll(const LayoutValue& gap);
    
    // --- Box Sizing 属性 ---
    /**
     * 设置盒子尺寸计算方式
     * 在布局中定义width和height是否包含padding和border
     * @param boxSizing 计算方式：BORDER_BOX(包含边框和内边距)、CONTENT_BOX(仅内容区域)
     */
    void setBoxSizing(BoxSizing boxSizing);
    
    FlexWrap getFlexWrap() const;
    Overflow getOverflow() const;
    Display getDisplay() const;
    float getAspectRatio() const;
    Direction getDirection() const;
    
    // --- Gap 属性获取 ---
    LayoutValue getGap(Gutter gutter) const;
    LayoutValue getColumnGap() const;
    LayoutValue getRowGap() const;
    LayoutValue getGapAll() const;
    
    // --- Box Sizing 属性获取 ---
    BoxSizing getBoxSizing() const;
    
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

    /**
     * 清除dirty标记
     */
    void clearDirty();
    
    // ====================================================================
    // 虚函数接口
    // ====================================================================
    
    /**
     * Cairo渲染函数 - 子类可以重写以实现自定义绘制
     * @param cr Cairo绘制上下文
     */
    virtual void render(cairo_t* cr) {
        // 避免未使用参数警告
        (void)cr;
    }
    
    /**
     * 自定义测量函数 - 用于文本等需要测量的内容
     */
    virtual void onMeasure(float width, float height, float& measuredWidth, float& measuredHeight) {
        // 避免未使用参数警告
        (void)width;
        (void)height;
        (void)measuredWidth;
        (void)measuredHeight;
    }
    
    /**
     * 布局变化回调
     */
    virtual void onLayoutChanged() {}
    
    /**
     * 渲染容器及其所有子节点
     * @param cr Cairo绘制上下文
     */
    void renderTree(cairo_t* cr);
    
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
    std::vector<SLayoutPtr> m_children;
    
    /**
     * 父节点弱引用
     */
    SLayoutWeakPtr m_parent;
    
    /**
     * 用户数据指针
     */
    void* m_userData = nullptr;

    /**
     * Dirty 标记
     */
    bool m_dirty = true;

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
