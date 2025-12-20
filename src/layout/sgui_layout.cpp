/**
 * GUI Layout类
 */

#include "sgui_layout.h"
#include <cairo.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace sgui {

// 静态回调函数用于测量
static YGSize measureFunc(YGNodeConstRef node, float width, YGMeasureMode widthMode, 
                         float height, YGMeasureMode heightMode) {
    // 从节点获取Container指针
    SLayout* container = static_cast<SLayout*>(YGNodeGetContext(node));
    if (!container) {
        return {0, 0};
    }
    
    float measuredWidth = 0, measuredHeight = 0;
    container->onMeasure(width, height, measuredWidth, measuredHeight);
    
    return {measuredWidth, measuredHeight};
}

// 静态回调函数用于布局变化
static void dirtiedFunc(YGNodeConstRef node) {
    SLayout* container = static_cast<SLayout*>(YGNodeGetContext(node));
    if (container) {
        container->onLayoutChanged();
    }
}

SLayout::SLayout() {
    // 创建Yoga节点
    m_yogaNode = YGNodeNew();
    
    // 设置上下文指针
    YGNodeSetContext(m_yogaNode, this);
    
    // 完全不设置测量函数，让Yoga自动处理布局
    // YGNodeSetMeasureFunc(m_yogaNode, measureFunc);
    
    // 设置脏标记回调
    YGNodeSetDirtiedFunc(m_yogaNode, dirtiedFunc);
}

SLayout::~SLayout() {
    // 移除所有子节点
    removeAllChildren();
    
    // 释放Yoga节点
    if (m_yogaNode) {
        YGNodeFree(m_yogaNode);
        m_yogaNode = nullptr;
    }
}

// ====================================================================
// 子节点管理
// ====================================================================

void SLayout::addChild(const SLayoutPtr& child) {
    if (!child) return;
    
    // 移除子节点原来的父节点
    auto oldParent = child->getParent();
    if (oldParent) {
        oldParent->removeChild(child);
    }
    
    // 添加到子节点列表
    m_children.push_back(child);
    
    // 设置父节点
    child->m_parent = shared_from_this();
    
    // 添加到Yoga节点树
    YGNodeInsertChild(m_yogaNode, child->m_yogaNode, YGNodeGetChildCount(m_yogaNode));
    
    // 标记需要重新计算布局
    markDirty();
}

void SLayout::insertChild(const SLayoutPtr& child, size_t index) {
    if (!child || index > m_children.size()) return;
    
    // 移除子节点原来的父节点
    auto oldParent = child->getParent();
    if (oldParent) {
        oldParent->removeChild(child);
    }
    
    // 插入到子节点列表
    m_children.insert(m_children.begin() + index, child);
    
    // 设置父节点
    child->m_parent = shared_from_this();
    
    // 添加到Yoga节点树
    YGNodeInsertChild(m_yogaNode, child->m_yogaNode, index);
    
    // 标记需要重新计算布局
    markDirty();
}

void SLayout::removeChild(const SLayoutPtr& child) {
    if (!child) return;
    
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        // 从Yoga节点树移除
        YGNodeRemoveChild(m_yogaNode, child->m_yogaNode);
        
        // 清除父节点引用
        child->m_parent.reset();
        
        // 从子节点列表移除
        m_children.erase(it);
        
        // 标记需要重新计算布局
        markDirty();
    }
}

void SLayout::removeAllChildren() {
    // 移除所有Yoga子节点
    YGNodeRemoveAllChildren(m_yogaNode);
    
    // 清除所有子节点的父节点引用
    for (auto& child : m_children) {
        child->m_parent.reset();
    }
    
    // 清空子节点列表
    m_children.clear();
    
    // 标记需要重新计算布局
    markDirty();
}

size_t SLayout::getChildCount() const {
    return m_children.size();
}

SLayoutPtr SLayout::getChildAt(size_t index) const {
    if (index >= m_children.size()) return nullptr;
    return m_children[index];
}

// ====================================================================
// 布局属性设置
// ====================================================================

// --- 尺寸设置 ---
void SLayout::setWidth(const LayoutValue& width) {
    YGValue ygValue = toYGValue(width);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetWidthPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetWidthAuto(m_yogaNode);
    } else {
        YGNodeStyleSetWidth(m_yogaNode, ygValue.value);
    }
}

void SLayout::setHeight(const LayoutValue& height) {
    YGValue ygValue = toYGValue(height);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetHeightPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetHeightAuto(m_yogaNode);
    } else {
        YGNodeStyleSetHeight(m_yogaNode, ygValue.value);
    }
}

void SLayout::setMinWidth(const LayoutValue& minWidth) {
    YGValue ygValue = toYGValue(minWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinWidth(m_yogaNode, ygValue.value);
    }
}

void SLayout::setMinHeight(const LayoutValue& minHeight) {
    YGValue ygValue = toYGValue(minHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinHeight(m_yogaNode, ygValue.value);
    }
}

void SLayout::setMaxWidth(const LayoutValue& maxWidth) {
    YGValue ygValue = toYGValue(maxWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxWidth(m_yogaNode, ygValue.value);
    }
}

void SLayout::setMaxHeight(const LayoutValue& maxHeight) {
    YGValue ygValue = toYGValue(maxHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxHeight(m_yogaNode, ygValue.value);
    }
}

LayoutValue SLayout::getWidth() const {
    return fromYGValue(YGNodeStyleGetWidth(m_yogaNode));
}

LayoutValue SLayout::getHeight() const {
    return fromYGValue(YGNodeStyleGetHeight(m_yogaNode));
}

LayoutValue SLayout::getMinWidth() const {
    return fromYGValue(YGNodeStyleGetMinWidth(m_yogaNode));
}

LayoutValue SLayout::getMinHeight() const {
    return fromYGValue(YGNodeStyleGetMinHeight(m_yogaNode));
}

LayoutValue SLayout::getMaxWidth() const {
    return fromYGValue(YGNodeStyleGetMaxWidth(m_yogaNode));
}

LayoutValue SLayout::getMaxHeight() const {
    return fromYGValue(YGNodeStyleGetMaxHeight(m_yogaNode));
}

// --- Flex属性 ---
void SLayout::setFlex(float flex) {
    YGNodeStyleSetFlex(m_yogaNode, flex);
}

void SLayout::setFlexGrow(float flexGrow) {
    YGNodeStyleSetFlexGrow(m_yogaNode, flexGrow);
}

void SLayout::setFlexShrink(float flexShrink) {
    YGNodeStyleSetFlexShrink(m_yogaNode, flexShrink);
}

void SLayout::setFlexBasis(const LayoutValue& flexBasis) {
    YGValue ygValue = toYGValue(flexBasis);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetFlexBasisPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetFlexBasisAuto(m_yogaNode);
    } else {
        YGNodeStyleSetFlexBasis(m_yogaNode, ygValue.value);
    }
}

float SLayout::getFlex() const {
    return YGNodeStyleGetFlex(m_yogaNode);
}

float SLayout::getFlexGrow() const {
    return YGNodeStyleGetFlexGrow(m_yogaNode);
}

float SLayout::getFlexShrink() const {
    return YGNodeStyleGetFlexShrink(m_yogaNode);
}

LayoutValue SLayout::getFlexBasis() const {
    return fromYGValue(YGNodeStyleGetFlexBasis(m_yogaNode));
}

// --- 布局方向和对齐 ---
void SLayout::setFlexDirection(FlexDirection direction) {
    YGNodeStyleSetFlexDirection(m_yogaNode, static_cast<YGFlexDirection>(static_cast<int>(direction)));
}

void SLayout::setJustifyContent(Align justify) {
    YGNodeStyleSetJustifyContent(m_yogaNode, static_cast<YGJustify>(static_cast<int>(justify)));
}

void SLayout::setAlignItems(Align align) {
    YGNodeStyleSetAlignItems(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

void SLayout::setAlignSelf(Align align) {
    YGNodeStyleSetAlignSelf(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

void SLayout::setAlignContent(Align align) {
    YGNodeStyleSetAlignContent(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

FlexDirection SLayout::getFlexDirection() const {
    return static_cast<sgui::FlexDirection>(static_cast<int>(YGNodeStyleGetFlexDirection(m_yogaNode)));
}

Align SLayout::getJustifyContent() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetJustifyContent(m_yogaNode)));
}

Align SLayout::getAlignItems() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignItems(m_yogaNode)));
}

Align SLayout::getAlignSelf() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignSelf(m_yogaNode)));
}

Align SLayout::getAlignContent() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignContent(m_yogaNode)));
}

// --- 位置和定位 ---
void SLayout::setPosition(PositionType positionType) {
    YGNodeStyleSetPositionType(m_yogaNode, static_cast<YGPositionType>(positionType));
}

void SLayout::setPosition(EdgeInsets position) {
    setPositionValues(position);
}

PositionType SLayout::getPositionType() const {
    return static_cast<sgui::PositionType>(YGNodeStyleGetPositionType(m_yogaNode));
}

EdgeInsets SLayout::getPosition() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        YGValue value = YGNodeStyleGetPosition(node, edge);
        return value.value;
    });
}

// --- 边距和内边距 ---
void SLayout::setMargin(const EdgeInsets& margin) {
    setEdgeValues(YGNodeStyleSetMargin, YGNodeStyleSetMarginPercent, margin);
}

void SLayout::setPadding(const EdgeInsets& padding) {
    setEdgeValues(YGNodeStyleSetPadding, YGNodeStyleSetPaddingPercent, padding);
}

void SLayout::setBorder(const EdgeInsets& border) {
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeLeft, border.left.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeTop, border.top.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeRight, border.right.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeBottom, border.bottom.value);
}

EdgeInsets SLayout::getMargin() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetMargin(node, edge);
    });
}

EdgeInsets SLayout::getPadding() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetPadding(node, edge);
    });
}

EdgeInsets SLayout::getBorder() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetBorder(node, edge);
    });
}

// --- 其他属性 ---
void SLayout::setFlexWrap(FlexWrap wrap) {
    YGNodeStyleSetFlexWrap(m_yogaNode, static_cast<YGWrap>(static_cast<int>(wrap)));
}

void SLayout::setOverflow(Overflow overflow) {
    YGNodeStyleSetOverflow(m_yogaNode, static_cast<YGOverflow>(static_cast<int>(overflow)));
}

void SLayout::setDisplay(Display display) {
    YGNodeStyleSetDisplay(m_yogaNode, static_cast<YGDisplay>(static_cast<int>(display)));
}

void SLayout::setAspectRatio(float aspectRatio) {
    YGNodeStyleSetAspectRatio(m_yogaNode, aspectRatio);
}

void SLayout::setDirection(Direction direction) {
    YGNodeStyleSetDirection(m_yogaNode, static_cast<YGDirection>(static_cast<int>(direction)));
}

// ====================================================================
// Gap 属性实现
// ====================================================================

void SLayout::setGap(Gutter gutter, const LayoutValue& gap) {
    YGGutter ygGutter = static_cast<YGGutter>(static_cast<int>(gutter));
    
    if (gap.isPercent) {
        YGNodeStyleSetGapPercent(m_yogaNode, ygGutter, gap.value);
    } else {
        YGNodeStyleSetGap(m_yogaNode, ygGutter, gap.value);
    }
    markDirty();
}

void SLayout::setColumnGap(const LayoutValue& gap) {
    setGap(Gutter::Column, gap);
}

void SLayout::setRowGap(const LayoutValue& gap) {
    setGap(Gutter::Row, gap);
}

void SLayout::setGapAll(const LayoutValue& gap) {
    setGap(Gutter::All, gap);
}

// ====================================================================
// Box Sizing 属性实现
// ====================================================================

void SLayout::setBoxSizing(BoxSizing boxSizing) {
    YGNodeStyleSetBoxSizing(m_yogaNode, static_cast<YGBoxSizing>(static_cast<int>(boxSizing)));
    markDirty();
}

// ====================================================================
// Gap 属性获取实现
// ====================================================================

LayoutValue SLayout::getGap(Gutter gutter) const {
    YGGutter ygGutter = static_cast<YGGutter>(static_cast<int>(gutter));
    YGValue ygValue = YGNodeStyleGetGap(m_yogaNode, ygGutter);
    return fromYGValue(ygValue);
}

LayoutValue SLayout::getColumnGap() const {
    return getGap(Gutter::Column);
}

LayoutValue SLayout::getRowGap() const {
    return getGap(Gutter::Row);
}

LayoutValue SLayout::getGapAll() const {
    return getGap(Gutter::All);
}

// ====================================================================
// Box Sizing 属性获取实现
// ====================================================================

BoxSizing SLayout::getBoxSizing() const {
    return static_cast<sgui::BoxSizing>(YGNodeStyleGetBoxSizing(m_yogaNode));
}

FlexWrap SLayout::getFlexWrap() const {
    return static_cast<sgui::FlexWrap>(YGNodeStyleGetFlexWrap(m_yogaNode));
}

Overflow SLayout::getOverflow() const {
    return static_cast<sgui::Overflow>(YGNodeStyleGetOverflow(m_yogaNode));
}

Display SLayout::getDisplay() const {
    return static_cast<sgui::Display>(YGNodeStyleGetDisplay(m_yogaNode));
}

float SLayout::getAspectRatio() const {
    return YGNodeStyleGetAspectRatio(m_yogaNode);
}

Direction SLayout::getDirection() const {
    return static_cast<sgui::Direction>(YGNodeStyleGetDirection(m_yogaNode));
}

// ====================================================================
// 布局计算和查询
// ====================================================================

void SLayout::calculateLayout(float width, float height) {
    YGNodeCalculateLayout(m_yogaNode, width, height, YGDirectionLTR);
}

float SLayout::getLeft() const {
    return YGNodeLayoutGetLeft(m_yogaNode);
}

float SLayout::getTop() const {
    return YGNodeLayoutGetTop(m_yogaNode);
}

float SLayout::getRight() const {
    return YGNodeLayoutGetRight(m_yogaNode);
}

float SLayout::getBottom() const {
    return YGNodeLayoutGetBottom(m_yogaNode);
}

float SLayout::getLayoutWidth() const {
    return YGNodeLayoutGetWidth(m_yogaNode);
}

float SLayout::getLayoutHeight() const {
    return YGNodeLayoutGetHeight(m_yogaNode);
}

float SLayout::getLayoutMarginLeft() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeLeft);
}

float SLayout::getLayoutMarginTop() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeTop);
}

float SLayout::getLayoutMarginRight() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeRight);
}

float SLayout::getLayoutMarginBottom() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeBottom);
}

float SLayout::getLayoutPaddingLeft() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeLeft);
}

float SLayout::getLayoutPaddingTop() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeTop);
}

float SLayout::getLayoutPaddingRight() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeRight);
}

float SLayout::getLayoutPaddingBottom() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeBottom);
}

float SLayout::getLayoutBorderLeft() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeLeft);
}

float SLayout::getLayoutBorderTop() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeTop);
}

float SLayout::getLayoutBorderRight() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeRight);
}

float SLayout::getLayoutBorderBottom() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeBottom);
}

bool SLayout::isDirty() const {
    // return YGNodeIsDirty(m_yogaNode);
    return m_dirty;
}

void SLayout::markDirty() {
    // YGNodeMarkDirty(m_yogaNode);
    m_dirty = true;
}

void SLayout::clearDirty() {
    m_dirty = false;
}


// ====================================================================
// 工具函数
// ====================================================================

void SLayout::renderTree(cairo_t* cr) {
    if (!cr) return;
    
    // 保存当前状态
    cairo_save(cr);
    
    // 获取布局信息
    float left = getLeft();
    float top = getTop();
    float width = getLayoutWidth();
    float height = getLayoutHeight();
    float paddingLeft = getLayoutPaddingLeft();
    float paddingTop = getLayoutPaddingTop();
    float paddingRight = getLayoutPaddingRight();
    float paddingBottom = getLayoutPaddingBottom();
    
    // 移动到容器位置
    cairo_translate(cr, left, top);
    
    // 设置裁剪区域（考虑溢出设置）
    Overflow overflow = getOverflow();
    if (overflow == Overflow::Hidden) {
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_clip(cr);
    }
    
    // 绘制背景
    cairo_save(cr);
    render(cr); // 调用子类的render方法
    cairo_restore(cr);
    
    // 移动到内容区域（考虑内边距）
    cairo_translate(cr, paddingLeft, paddingTop);
    
    // 绘制子节点
    float contentWidth = width - paddingLeft - paddingRight;
    float contentHeight = height - paddingTop - paddingBottom;
    
    for (const auto& child : m_children) {
        if (child->getDisplay() != Display::None) {
            child->renderTree(cr);
        }
    }
    
    // 恢复状态
    cairo_restore(cr);
}

void SLayout::printLayoutTree(int depth) const {
    std::string indent(depth * 2, ' ');
    
    std::cout << indent << "SLayout:\n";
    std::cout << indent << "  位置: (" << getLeft() << ", " << getTop() << ")\n";
    std::cout << indent << "  尺寸: " << getLayoutWidth() << " x " << getLayoutHeight() << "\n";
    std::cout << indent << "  边距: L=" << getLayoutMarginLeft()
              << ", T=" << getLayoutMarginTop()
              << ", R=" << getLayoutMarginRight()
              << ", B=" << getLayoutMarginBottom() << "\n";
    std::cout << indent << "  内边距: L=" << getLayoutPaddingLeft()
              << ", T=" << getLayoutPaddingTop()
              << ", R=" << getLayoutPaddingRight()
              << ", B=" << getLayoutPaddingBottom() << "\n";
    
    // 递归打印子节点
    for (const auto& child : m_children) {
        child->printLayoutTree(depth + 1);
    }
}

// ====================================================================
// 私有辅助函数
// ====================================================================

YGValue SLayout::toYGValue(const LayoutValue& value) const {
    if (value.isAuto) {
        return YGValueAuto;
    } else if (value.isPercent) {
        return {value.value, YGUnitPercent};
    } else {
        return {value.value, YGUnitPoint};
    }
}

LayoutValue SLayout::fromYGValue(const YGValue& value) const {
    LayoutValue result;
    result.value = value.value;
    result.isPercent = (value.unit == YGUnitPercent);
    result.isAuto = (value.unit == YGUnitAuto);
    return result;
}

void SLayout::setEdgeValues(void (*setter)(YGNodeRef, YGEdge, float), 
                             void (*setterPercent)(YGNodeRef, YGEdge, float),
                             const EdgeInsets& edge) {
    // Left
    if (edge.left.isPercent) {
        setterPercent(m_yogaNode, YGEdgeLeft, edge.left.value);
    } else if (edge.left.isAuto) {
        YGNodeStyleSetMarginAuto(m_yogaNode, YGEdgeLeft);
    } else {
        setter(m_yogaNode, YGEdgeLeft, edge.left.value);
    }
    
    // Top
    if (edge.top.isPercent) {
        setterPercent(m_yogaNode, YGEdgeTop, edge.top.value);
    } else if (edge.top.isAuto) {
        YGNodeStyleSetMarginAuto(m_yogaNode, YGEdgeTop);
    } else {
        setter(m_yogaNode, YGEdgeTop, edge.top.value);
    }
    
    // Right
    if (edge.right.isPercent) {
        setterPercent(m_yogaNode, YGEdgeRight, edge.right.value);
    } else if (edge.right.isAuto) {
        YGNodeStyleSetMarginAuto(m_yogaNode, YGEdgeRight);
    } else {
        setter(m_yogaNode, YGEdgeRight, edge.right.value);
    }
    
    // Bottom
    if (edge.bottom.isPercent) {
        setterPercent(m_yogaNode, YGEdgeBottom, edge.bottom.value);
    } else if (edge.bottom.isAuto) {
        YGNodeStyleSetMarginAuto(m_yogaNode, YGEdgeBottom);
    } else {
        setter(m_yogaNode, YGEdgeBottom, edge.bottom.value);
    }
}

EdgeInsets SLayout::getEdgeValues(float (*getter)(YGNodeConstRef, YGEdge)) const {
    EdgeInsets result;
    result.left.value = getter(m_yogaNode, YGEdgeLeft);
    result.top.value = getter(m_yogaNode, YGEdgeTop);
    result.right.value = getter(m_yogaNode, YGEdgeRight);
    result.bottom.value = getter(m_yogaNode, YGEdgeBottom);
    return result;
}

void SLayout::setPositionValues(const EdgeInsets& position) {
    // Left
    if (position.left.isPercent) {
        YGNodeStyleSetPositionPercent(m_yogaNode, YGEdgeLeft, position.left.value);
    } else if (position.left.isAuto) {
        YGNodeStyleSetPositionAuto(m_yogaNode, YGEdgeLeft);
    } else {
        YGNodeStyleSetPosition(m_yogaNode, YGEdgeLeft, position.left.value);
    }
    
    // Top
    if (position.top.isPercent) {
        YGNodeStyleSetPositionPercent(m_yogaNode, YGEdgeTop, position.top.value);
    } else if (position.top.isAuto) {
        YGNodeStyleSetPositionAuto(m_yogaNode, YGEdgeTop);
    } else {
        YGNodeStyleSetPosition(m_yogaNode, YGEdgeTop, position.top.value);
    }
    
    // Right
    if (position.right.isPercent) {
        YGNodeStyleSetPositionPercent(m_yogaNode, YGEdgeRight, position.right.value);
    } else if (position.right.isAuto) {
        YGNodeStyleSetPositionAuto(m_yogaNode, YGEdgeRight);
    } else {
        YGNodeStyleSetPosition(m_yogaNode, YGEdgeRight, position.right.value);
    }
    
    // Bottom
    if (position.bottom.isPercent) {
        YGNodeStyleSetPositionPercent(m_yogaNode, YGEdgeBottom, position.bottom.value);
    } else if (position.bottom.isAuto) {
        YGNodeStyleSetPositionAuto(m_yogaNode, YGEdgeBottom);
    } else {
        YGNodeStyleSetPosition(m_yogaNode, YGEdgeBottom, position.bottom.value);
    }
}

} // namespace sgui
