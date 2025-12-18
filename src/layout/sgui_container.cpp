/**
 * Container.cpp - GUI容器的基类实现
 */

#include "sgui_container.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace sgui {

// 静态回调函数用于测量
static YGSize measureFunc(YGNodeConstRef node, float width, YGMeasureMode widthMode, 
                         float height, YGMeasureMode heightMode) {
    // 从节点获取Container指针
    Container* container = static_cast<Container*>(YGNodeGetContext(node));
    if (!container) {
        return {0, 0};
    }
    
    float measuredWidth = 0, measuredHeight = 0;
    container->onMeasure(width, height, measuredWidth, measuredHeight);
    
    return {measuredWidth, measuredHeight};
}

// 静态回调函数用于布局变化
static void dirtiedFunc(YGNodeConstRef node) {
    Container* container = static_cast<Container*>(YGNodeGetContext(node));
    if (container) {
        container->onLayoutChanged();
    }
}

Container::Container() {
    // 创建Yoga节点
    m_yogaNode = YGNodeNew();
    
    // 设置上下文指针
    YGNodeSetContext(m_yogaNode, this);
    
    // 设置测量函数
    YGNodeSetMeasureFunc(m_yogaNode, measureFunc);
    
    // 设置脏标记回调
    YGNodeSetDirtiedFunc(m_yogaNode, dirtiedFunc);
}

Container::~Container() {
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

void Container::addChild(const ContainerPtr& child) {
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

void Container::insertChild(const ContainerPtr& child, size_t index) {
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

void Container::removeChild(const ContainerPtr& child) {
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

void Container::removeAllChildren() {
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

size_t Container::getChildCount() const {
    return m_children.size();
}

ContainerPtr Container::getChildAt(size_t index) const {
    if (index >= m_children.size()) return nullptr;
    return m_children[index];
}

// ====================================================================
// 布局属性设置
// ====================================================================

// --- 尺寸设置 ---
void Container::setWidth(const LayoutValue& width) {
    YGValue ygValue = toYGValue(width);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetWidthPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetWidthAuto(m_yogaNode);
    } else {
        YGNodeStyleSetWidth(m_yogaNode, ygValue.value);
    }
}

void Container::setHeight(const LayoutValue& height) {
    YGValue ygValue = toYGValue(height);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetHeightPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetHeightAuto(m_yogaNode);
    } else {
        YGNodeStyleSetHeight(m_yogaNode, ygValue.value);
    }
}

void Container::setMinWidth(const LayoutValue& minWidth) {
    YGValue ygValue = toYGValue(minWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinWidth(m_yogaNode, ygValue.value);
    }
}

void Container::setMinHeight(const LayoutValue& minHeight) {
    YGValue ygValue = toYGValue(minHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMinHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMinHeight(m_yogaNode, ygValue.value);
    }
}

void Container::setMaxWidth(const LayoutValue& maxWidth) {
    YGValue ygValue = toYGValue(maxWidth);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxWidthPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxWidth(m_yogaNode, ygValue.value);
    }
}

void Container::setMaxHeight(const LayoutValue& maxHeight) {
    YGValue ygValue = toYGValue(maxHeight);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetMaxHeightPercent(m_yogaNode, ygValue.value);
    } else {
        YGNodeStyleSetMaxHeight(m_yogaNode, ygValue.value);
    }
}

LayoutValue Container::getWidth() const {
    return fromYGValue(YGNodeStyleGetWidth(m_yogaNode));
}

LayoutValue Container::getHeight() const {
    return fromYGValue(YGNodeStyleGetHeight(m_yogaNode));
}

LayoutValue Container::getMinWidth() const {
    return fromYGValue(YGNodeStyleGetMinWidth(m_yogaNode));
}

LayoutValue Container::getMinHeight() const {
    return fromYGValue(YGNodeStyleGetMinHeight(m_yogaNode));
}

LayoutValue Container::getMaxWidth() const {
    return fromYGValue(YGNodeStyleGetMaxWidth(m_yogaNode));
}

LayoutValue Container::getMaxHeight() const {
    return fromYGValue(YGNodeStyleGetMaxHeight(m_yogaNode));
}

// --- Flex属性 ---
void Container::setFlex(float flex) {
    YGNodeStyleSetFlex(m_yogaNode, flex);
}

void Container::setFlexGrow(float flexGrow) {
    YGNodeStyleSetFlexGrow(m_yogaNode, flexGrow);
}

void Container::setFlexShrink(float flexShrink) {
    YGNodeStyleSetFlexShrink(m_yogaNode, flexShrink);
}

void Container::setFlexBasis(const LayoutValue& flexBasis) {
    YGValue ygValue = toYGValue(flexBasis);
    if (ygValue.unit == YGUnitPercent) {
        YGNodeStyleSetFlexBasisPercent(m_yogaNode, ygValue.value);
    } else if (ygValue.unit == YGUnitAuto) {
        YGNodeStyleSetFlexBasisAuto(m_yogaNode);
    } else {
        YGNodeStyleSetFlexBasis(m_yogaNode, ygValue.value);
    }
}

float Container::getFlex() const {
    return YGNodeStyleGetFlex(m_yogaNode);
}

float Container::getFlexGrow() const {
    return YGNodeStyleGetFlexGrow(m_yogaNode);
}

float Container::getFlexShrink() const {
    return YGNodeStyleGetFlexShrink(m_yogaNode);
}

LayoutValue Container::getFlexBasis() const {
    return fromYGValue(YGNodeStyleGetFlexBasis(m_yogaNode));
}

// --- 布局方向和对齐 ---
void Container::setFlexDirection(FlexDirection direction) {
    YGNodeStyleSetFlexDirection(m_yogaNode, static_cast<YGFlexDirection>(direction));
}

void Container::setJustifyContent(Align justify) {
    YGNodeStyleSetJustifyContent(m_yogaNode, static_cast<YGJustify>(justify));
}

void Container::setAlignItems(Align align) {
    YGNodeStyleSetAlignItems(m_yogaNode, static_cast<YGAlign>(align));
}

void Container::setAlignSelf(Align align) {
    YGNodeStyleSetAlignSelf(m_yogaNode, static_cast<YGAlign>(align));
}

void Container::setAlignContent(Align align) {
    YGNodeStyleSetAlignContent(m_yogaNode, static_cast<YGAlign>(align));
}

Container::FlexDirection Container::getFlexDirection() const {
    return static_cast<FlexDirection>(YGNodeStyleGetFlexDirection(m_yogaNode));
}

Container::Align Container::getJustifyContent() const {
    return static_cast<Align>(YGNodeStyleGetJustifyContent(m_yogaNode));
}

Container::Align Container::getAlignItems() const {
    return static_cast<Align>(YGNodeStyleGetAlignItems(m_yogaNode));
}

Container::Align Container::getAlignSelf() const {
    return static_cast<Align>(YGNodeStyleGetAlignSelf(m_yogaNode));
}

Container::Align Container::getAlignContent() const {
    return static_cast<Align>(YGNodeStyleGetAlignContent(m_yogaNode));
}

// --- 位置和定位 ---
void Container::setPosition(PositionType positionType) {
    YGNodeStyleSetPositionType(m_yogaNode, static_cast<YGPositionType>(positionType));
}

void Container::setPosition(EdgeInsets position) {
    setPositionValues(position);
}

Container::PositionType Container::getPositionType() const {
    return static_cast<PositionType>(YGNodeStyleGetPositionType(m_yogaNode));
}

EdgeInsets Container::getPosition() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        YGValue value = YGNodeStyleGetPosition(node, edge);
        return value.value;
    });
}

// --- 边距和内边距 ---
void Container::setMargin(const EdgeInsets& margin) {
    setEdgeValues(YGNodeStyleSetMargin, YGNodeStyleSetMarginPercent, margin);
}

void Container::setPadding(const EdgeInsets& padding) {
    setEdgeValues(YGNodeStyleSetPadding, YGNodeStyleSetPaddingPercent, padding);
}

void Container::setBorder(const EdgeInsets& border) {
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeLeft, border.left.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeTop, border.top.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeRight, border.right.value);
    YGNodeStyleSetBorder(m_yogaNode, YGEdgeBottom, border.bottom.value);
}

EdgeInsets Container::getMargin() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetMargin(node, edge);
    });
}

EdgeInsets Container::getPadding() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetPadding(node, edge);
    });
}

EdgeInsets Container::getBorder() const {
    return getEdgeValues([](YGNodeConstRef node, YGEdge edge) {
        return YGNodeLayoutGetBorder(node, edge);
    });
}

// --- 其他属性 ---
void Container::setFlexWrap(FlexWrap wrap) {
    YGNodeStyleSetFlexWrap(m_yogaNode, static_cast<YGWrap>(wrap));
}

void Container::setOverflow(Overflow overflow) {
    YGNodeStyleSetOverflow(m_yogaNode, static_cast<YGOverflow>(overflow));
}

void Container::setDisplay(Display display) {
    YGNodeStyleSetDisplay(m_yogaNode, static_cast<YGDisplay>(display));
}

void Container::setAspectRatio(float aspectRatio) {
    YGNodeStyleSetAspectRatio(m_yogaNode, aspectRatio);
}

void Container::setDirection(Direction direction) {
    YGNodeStyleSetDirection(m_yogaNode, static_cast<YGDirection>(direction));
}

Container::FlexWrap Container::getFlexWrap() const {
    return static_cast<FlexWrap>(YGNodeStyleGetFlexWrap(m_yogaNode));
}

Container::Overflow Container::getOverflow() const {
    return static_cast<Overflow>(YGNodeStyleGetOverflow(m_yogaNode));
}

Container::Display Container::getDisplay() const {
    return static_cast<Display>(YGNodeStyleGetDisplay(m_yogaNode));
}

float Container::getAspectRatio() const {
    return YGNodeStyleGetAspectRatio(m_yogaNode);
}

Container::Direction Container::getDirection() const {
    return static_cast<Direction>(YGNodeStyleGetDirection(m_yogaNode));
}

// ====================================================================
// 布局计算和查询
// ====================================================================

void Container::calculateLayout(float width, float height) {
    YGNodeCalculateLayout(m_yogaNode, width, height, YGDirectionLTR);
}

float Container::getLeft() const {
    return YGNodeLayoutGetLeft(m_yogaNode);
}

float Container::getTop() const {
    return YGNodeLayoutGetTop(m_yogaNode);
}

float Container::getRight() const {
    return YGNodeLayoutGetRight(m_yogaNode);
}

float Container::getBottom() const {
    return YGNodeLayoutGetBottom(m_yogaNode);
}

float Container::getLayoutWidth() const {
    return YGNodeLayoutGetWidth(m_yogaNode);
}

float Container::getLayoutHeight() const {
    return YGNodeLayoutGetHeight(m_yogaNode);
}

float Container::getLayoutMarginLeft() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeLeft);
}

float Container::getLayoutMarginTop() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeTop);
}

float Container::getLayoutMarginRight() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeRight);
}

float Container::getLayoutMarginBottom() const {
    return YGNodeLayoutGetMargin(m_yogaNode, YGEdgeBottom);
}

float Container::getLayoutPaddingLeft() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeLeft);
}

float Container::getLayoutPaddingTop() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeTop);
}

float Container::getLayoutPaddingRight() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeRight);
}

float Container::getLayoutPaddingBottom() const {
    return YGNodeLayoutGetPadding(m_yogaNode, YGEdgeBottom);
}

float Container::getLayoutBorderLeft() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeLeft);
}

float Container::getLayoutBorderTop() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeTop);
}

float Container::getLayoutBorderRight() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeRight);
}

float Container::getLayoutBorderBottom() const {
    return YGNodeLayoutGetBorder(m_yogaNode, YGEdgeBottom);
}

bool Container::isDirty() const {
    return YGNodeIsDirty(m_yogaNode);
}

void Container::markDirty() {
    YGNodeMarkDirty(m_yogaNode);
}

// ====================================================================
// 工具函数
// ====================================================================

void Container::printLayoutTree(int depth) const {
    std::string indent(depth * 2, ' ');
    
    std::cout << indent << "Container:\n";
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

YGValue Container::toYGValue(const LayoutValue& value) const {
    if (value.isAuto) {
        return YGValueAuto;
    } else if (value.isPercent) {
        return {value.value, YGUnitPercent};
    } else {
        return {value.value, YGUnitPoint};
    }
}

LayoutValue Container::fromYGValue(const YGValue& value) const {
    LayoutValue result;
    result.value = value.value;
    result.isPercent = (value.unit == YGUnitPercent);
    result.isAuto = (value.unit == YGUnitAuto);
    return result;
}

void Container::setEdgeValues(void (*setter)(YGNodeRef, YGEdge, float), 
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

EdgeInsets Container::getEdgeValues(float (*getter)(YGNodeConstRef, YGEdge)) const {
    EdgeInsets result;
    result.left.value = getter(m_yogaNode, YGEdgeLeft);
    result.top.value = getter(m_yogaNode, YGEdgeTop);
    result.right.value = getter(m_yogaNode, YGEdgeRight);
    result.bottom.value = getter(m_yogaNode, YGEdgeBottom);
    return result;
}

void Container::setPositionValues(const EdgeInsets& position) {
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
