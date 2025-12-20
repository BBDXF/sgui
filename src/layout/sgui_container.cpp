/**
 * SContainer.cpp - GUI容器的基类实现
 */

#include "sgui_container.h"
#include <cairo.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace sgui {

// 静态回调函数用于测量
static YGSize measureFunc(YGNodeConstRef node, float width, YGMeasureMode widthMode, 
                         float height, YGMeasureMode heightMode) {
    // 从节点获取Container指针
    SContainer* container = static_cast<SContainer*>(YGNodeGetContext(node));
    if (!container) {
        return {0, 0};
    }
    
    float measuredWidth = 0, measuredHeight = 0;
    container->onMeasure(width, height, measuredWidth, measuredHeight);
    
    return {measuredWidth, measuredHeight};
}

// 静态回调函数用于布局变化
static void dirtiedFunc(YGNodeConstRef node) {
    SContainer* container = static_cast<SContainer*>(YGNodeGetContext(node));
    if (container) {
        container->onLayoutChanged();
    }
}

SContainer::SContainer() {
    // 创建Yoga节点
    m_yogaNode = YGNodeNew();
    
    // 设置上下文指针
    YGNodeSetContext(m_yogaNode, this);
    
    // 完全不设置测量函数，让Yoga自动处理布局
    // YGNodeSetMeasureFunc(m_yogaNode, measureFunc);
    
    // 设置脏标记回调
    YGNodeSetDirtiedFunc(m_yogaNode, dirtiedFunc);
}

SContainer::~SContainer() {
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

void SContainer::addChild(const SContainerPtr& child) {
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

void SContainer::insertChild(const SContainerPtr& child, size_t index) {
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

void SContainer::removeChild(const SContainerPtr& child) {
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

void SContainer::removeAllChildren() {
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

size_t SContainer::getChildCount() const {
    return m_children.size();
}

SContainerPtr SContainer::getChildAt(size_t index) const {
    if (index >= m_children.size()) return nullptr;
    return m_children[index];
}

// ====================================================================
// 布局属性设置
// ====================================================================

// --- 尺寸设置 ---
void SContainer::setWidth(const LayoutValue& width) {
    YGValue ygValue = toYGValue(width);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetWidthPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetWidthAuto(m_yogaNode);
    } else {
        YGNodeStyleSetWidth(m_yogaNode, ygValue.value);
    }
}

void SContainer::setHeight(const LayoutValue& height) {
    YGValue ygValue = toYGValue(height);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetHeightPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetHeightAuto(m_yogaNode);
    } else {
        YGNodeStyleSetHeight(m_yogaNode, ygValue.value);
    }
}

void SContainer::setMinWidth(const LayoutValue& minWidth) {
    YGValue ygValue = toYGValue(minWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinWidth(m_yogaNode, ygValue.value);
    }
}

void SContainer::setMinHeight(const LayoutValue& minHeight) {
    YGValue ygValue = toYGValue(minHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinHeight(m_yogaNode, ygValue.value);
    }
}

void SContainer::setMaxWidth(const LayoutValue& maxWidth) {
    YGValue ygValue = toYGValue(maxWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxWidth(m_yogaNode, ygValue.value);
    }
}

void SContainer::setMaxHeight(const LayoutValue& maxHeight) {
    YGValue ygValue = toYGValue(maxHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxHeight(m_yogaNode, ygValue.value);
    }
}

LayoutValue SContainer::getWidth() const {
    return fromYGValue(YGNodeStyleGetWidth(m_yogaNode));
}

LayoutValue SContainer::getHeight() const {
    return fromYGValue(YGNodeStyleGetHeight(m_yogaNode));
}

LayoutValue SContainer::getMinWidth() const {
    return fromYGValue(YGNodeStyleGetMinWidth(m_yogaNode));
}

LayoutValue SContainer::getMinHeight() const {
    return fromYGValue(YGNodeStyleGetMinHeight(m_yogaNode));
}

LayoutValue SContainer::getMaxWidth() const {
    return fromYGValue(YGNodeStyleGetMaxWidth(m_yogaNode));
}

LayoutValue SContainer::getMaxHeight() const {
    return fromYGValue(YGNodeStyleGetMaxHeight(m_yogaNode));
}

// --- Flex属性 ---
void SContainer::setFlex(float flex) {
    YGNodeStyleSetFlex(m_yogaNode, flex);
}

void SContainer::setFlexGrow(float flexGrow) {
    YGNodeStyleSetFlexGrow(m_yogaNode, flexGrow);
}

void SContainer::setFlexShrink(float flexShrink) {
    YGNodeStyleSetFlexShrink(m_yogaNode, flexShrink);
}

void SContainer::setFlexBasis(const LayoutValue& flexBasis) {
    YGValue ygValue = toYGValue(flexBasis);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetFlexBasisPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetFlexBasisAuto(m_yogaNode);
    } else {
        YGNodeStyleSetFlexBasis(m_yogaNode, ygValue.value);
    }
}

float SContainer::getFlex() const {
    return YGNodeStyleGetFlex(m_yogaNode);
}

float SContainer::getFlexGrow() const {
    return YGNodeStyleGetFlexGrow(m_yogaNode);
}

float SContainer::getFlexShrink() const {
    return YGNodeStyleGetFlexShrink(m_yogaNode);
}

LayoutValue SContainer::getFlexBasis() const {
    return fromYGValue(YGNodeStyleGetFlexBasis(m_yogaNode));
}

// --- 布局方向和对齐 ---
void SContainer::setFlexDirection(FlexDirection direction) {
    YGNodeStyleSetFlexDirection(m_yogaNode, static_cast<YGFlexDirection>(static_cast<int>(direction)));
}

void SContainer::setJustifyContent(Align justify) {
    YGNodeStyleSetJustifyContent(m_yogaNode, static_cast<YGJustify>(static_cast<int>(justify)));
}

void SContainer::setAlignItems(Align align) {
    YGNodeStyleSetAlignItems(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

void SContainer::setAlignSelf(Align align) {
    YGNodeStyleSetAlignSelf(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

void SContainer::setAlignContent(Align align) {
    YGNodeStyleSetAlignContent(m_yogaNode, static_cast<YGAlign>(static_cast<int>(align)));
}

FlexDirection SContainer::getFlexDirection() const {
    return static_cast<sgui::FlexDirection>(static_cast<int>(YGNodeStyleGetFlexDirection(m_yogaNode)));
}

Align SContainer::getJustifyContent() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetJustifyContent(m_yogaNode)));
}

Align SContainer::getAlignItems() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignItems(m_yogaNode)));
}

Align SContainer::getAlignSelf() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignSelf(m_yogaNode)));
}

Align SContainer::getAlignContent() const {
    return static_cast<sgui::Align>(static_cast<int>(YGNodeStyleGetAlignContent(m_yogaNode)));
}

// --- 位置和定位 ---
void SContainer::setPosition(PositionType positionType) {
    YGNodeStyleSetPositionType(m_yogaNode, static_cast<YGPositionType>(positionType));
}

void SContainer::setPosition(EdgeInsets position) {
    setPositionValues(position);
}

PositionType SContainer::getPositionType() const {
    return static_cast<sgui::PositionType>(YGNodeStyleGetPositionType(m_yogaNode));
}

EdgeInsets SContainer::getPosition() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        YGValue value = YGNodeStyleGetPosition(node, edge);
        return value.value;
    });
}

// --- 边距和内边距 ---
void SContainer::setMargin(const EdgeInsets& margin) {
    setEdgeValues(YGNodeStyleSetMargin, YGNodeStyleSetMarginPercent, margin);
}

void SContainer::setPadding(const EdgeInsets& padding) {
    setEdgeValues(YGNodeStyleSetPadding, YGNodeStyleSetPaddingPercent, padding);
}

void SContainer::setBorder(const EdgeInsets& border) {
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeLeft, border.left.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeTop, border.top.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeRight, border.right.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeBottom, border.bottom.value);
}

EdgeInsets SContainer::getMargin() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetMargin(node, edge);
    });
}

EdgeInsets SContainer::getPadding() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetPadding(node, edge);
    });
}

EdgeInsets SContainer::getBorder() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetBorder(node, edge);
    });
}

// --- 其他属性 ---
void SContainer::setFlexWrap(FlexWrap wrap) {
    YGNodeStyleSetFlexWrap(m_yogaNode, static_cast<YGWrap>(static_cast<int>(wrap)));
}

void SContainer::setOverflow(Overflow overflow) {
    YGNodeStyleSetOverflow(m_yogaNode, static_cast<YGOverflow>(static_cast<int>(overflow)));
}

void SContainer::setDisplay(Display display) {
    YGNodeStyleSetDisplay(m_yogaNode, static_cast<YGDisplay>(static_cast<int>(display)));
}

void SContainer::setAspectRatio(float aspectRatio) {
    YGNodeStyleSetAspectRatio(m_yogaNode, aspectRatio);
}

void SContainer::setDirection(Direction direction) {
    YGNodeStyleSetDirection(m_yogaNode, static_cast<YGDirection>(static_cast<int>(direction)));
}

// ====================================================================
// Gap 属性实现
// ====================================================================

void SContainer::setGap(Gutter gutter, const LayoutValue& gap) {
    YGGutter ygGutter = static_cast<YGGutter>(static_cast<int>(gutter));
    
    if (gap.isPercent) {
        YGNodeStyleSetGapPercent(m_yogaNode, ygGutter, gap.value);
    } else {
        YGNodeStyleSetGap(m_yogaNode, ygGutter, gap.value);
    }
    markDirty();
}

void SContainer::setColumnGap(const LayoutValue& gap) {
    setGap(Gutter::Column, gap);
}

void SContainer::setRowGap(const LayoutValue& gap) {
    setGap(Gutter::Row, gap);
}

void SContainer::setGapAll(const LayoutValue& gap) {
    setGap(Gutter::All, gap);
}

// ====================================================================
// Box Sizing 属性实现
// ====================================================================

void SContainer::setBoxSizing(BoxSizing boxSizing) {
    YGNodeStyleSetBoxSizing(m_yogaNode, static_cast<YGBoxSizing>(static_cast<int>(boxSizing)));
    markDirty();
}

// ====================================================================
// Gap 属性获取实现
// ====================================================================

LayoutValue SContainer::getGap(Gutter gutter) const {
    YGGutter ygGutter = static_cast<YGGutter>(static_cast<int>(gutter));
    YGValue ygValue = YGNodeStyleGetGap(m_yogaNode, ygGutter);
    return fromYGValue(ygValue);
}

LayoutValue SContainer::getColumnGap() const {
    return getGap(Gutter::Column);
}

LayoutValue SContainer::getRowGap() const {
    return getGap(Gutter::Row);
}

LayoutValue SContainer::getGapAll() const {
    return getGap(Gutter::All);
}

// ====================================================================
// Box Sizing 属性获取实现
// ====================================================================

BoxSizing SContainer::getBoxSizing() const {
    return static_cast<sgui::BoxSizing>(YGNodeStyleGetBoxSizing(m_yogaNode));
}

FlexWrap SContainer::getFlexWrap() const {
    return static_cast<sgui::FlexWrap>(YGNodeStyleGetFlexWrap(m_yogaNode));
}

Overflow SContainer::getOverflow() const {
    return static_cast<sgui::Overflow>(YGNodeStyleGetOverflow(m_yogaNode));
}

Display SContainer::getDisplay() const {
    return static_cast<sgui::Display>(YGNodeStyleGetDisplay(m_yogaNode));
}

float SContainer::getAspectRatio() const {
    return YGNodeStyleGetAspectRatio(m_yogaNode);
}

Direction SContainer::getDirection() const {
    return static_cast<sgui::Direction>(YGNodeStyleGetDirection(m_yogaNode));
}

// ====================================================================
// 布局计算和查询
// ====================================================================

void SContainer::calculateLayout(float width, float height) {
    YGNodeCalculateLayout(m_yogaNode, width, height, YGDirectionLTR);
}

float SContainer::getLeft() const {
    return YGNodeLayoutGetLeft(m_yogaNode);
}

float SContainer::getTop() const {
    return YGNodeLayoutGetTop(m_yogaNode);
}

float SContainer::getRight() const {
    return YGNodeLayoutGetRight(m_yogaNode);
}

float SContainer::getBottom() const {
    return YGNodeLayoutGetBottom(m_yogaNode);
}

float SContainer::getLayoutWidth() const {
    return YGNodeLayoutGetWidth(m_yogaNode);
}

float SContainer::getLayoutHeight() const {
    return YGNodeLayoutGetHeight(m_yogaNode);
}

float SContainer::getLayoutMarginLeft() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeLeft);
}

float SContainer::getLayoutMarginTop() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeTop);
}

float SContainer::getLayoutMarginRight() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeRight);
}

float SContainer::getLayoutMarginBottom() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeBottom);
}

float SContainer::getLayoutPaddingLeft() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeLeft);
}

float SContainer::getLayoutPaddingTop() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeTop);
}

float SContainer::getLayoutPaddingRight() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeRight);
}

float SContainer::getLayoutPaddingBottom() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeBottom);
}

float SContainer::getLayoutBorderLeft() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeLeft);
}

float SContainer::getLayoutBorderTop() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeTop);
}

float SContainer::getLayoutBorderRight() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeRight);
}

float SContainer::getLayoutBorderBottom() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeBottom);
}

bool SContainer::isDirty() const {
    // return YGNodeIsDirty(m_yogaNode);
    return m_dirty;
}

void SContainer::markDirty() {
    // YGNodeMarkDirty(m_yogaNode);
    m_dirty = true;
}

void SContainer::clearDirty() {
    m_dirty = false;
}


// ====================================================================
// 工具函数
// ====================================================================

void SContainer::renderTree(cairo_t* cr) {
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

void SContainer::printLayoutTree(int depth) const {
    std::string indent(depth * 2, ' ');
    
    std::cout << indent << "SContainer:\n";
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

YGValue SContainer::toYGValue(const LayoutValue& value) const {
    if (value.isAuto) {
        return YGValueAuto;
    } else if (value.isPercent) {
        return {value.value, YGUnitPercent};
    } else {
        return {value.value, YGUnitPoint};
    }
}

LayoutValue SContainer::fromYGValue(const YGValue& value) const {
    LayoutValue result;
    result.value = value.value;
    result.isPercent = (value.unit == YGUnitPercent);
    result.isAuto = (value.unit == YGUnitAuto);
    return result;
}

void SContainer::setEdgeValues(void (*setter)(YGNodeRef, YGEdge, float), 
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

EdgeInsets SContainer::getEdgeValues(float (*getter)(YGNodeConstRef, YGEdge)) const {
    EdgeInsets result;
    result.left.value = getter(m_yogaNode, YGEdgeLeft);
    result.top.value = getter(m_yogaNode, YGEdgeTop);
    result.right.value = getter(m_yogaNode, YGEdgeRight);
    result.bottom.value = getter(m_yogaNode, YGEdgeBottom);
    return result;
}

void SContainer::setPositionValues(const EdgeInsets& position) {
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
