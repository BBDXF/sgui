/**
 * Yoga Demo - 演示Yoga Flexbox布局引擎的基本用法
 * 
 * 这个demo展示了如何使用Yoga创建一个简单的UI布局，包括：
 * 1. 基本的flexbox容器和子节点
 * 2. 不同的布局方向和对齐方式
 * 3. 边距、内边距和尺寸设置
 * 4. 嵌套布局
 */

#include <iostream>
#include <string>
#include <iomanip>

// 直接包含Yoga的核心头文件
#include <yoga/YGNode.h>
#include <yoga/YGNodeStyle.h>
#include <yoga/YGNodeLayout.h>
#include <yoga/YGEnums.h>

// 辅助函数：打印节点的布局信息
void printNodeLayout(YGNodeRef node, const std::string& name, int depth = 0) {
    std::string indent(depth * 2, ' ');
    
    float left = YGNodeLayoutGetLeft(node);
    float top = YGNodeLayoutGetTop(node);
    float width = YGNodeLayoutGetWidth(node);
    float height = YGNodeLayoutGetHeight(node);
    
    std::cout << indent << name << ":\n";
    std::cout << indent << "  位置: (" << left << ", " << top << ")\n";
    std::cout << indent << "  尺寸: " << width << " x " << height << "\n";
    std::cout << indent << "  边距: L=" << YGNodeLayoutGetMargin(node, YGEdgeLeft)
              << ", T=" << YGNodeLayoutGetMargin(node, YGEdgeTop)
              << ", R=" << YGNodeLayoutGetMargin(node, YGEdgeRight)
              << ", B=" << YGNodeLayoutGetMargin(node, YGEdgeBottom) << "\n";
    std::cout << indent << "  内边距: L=" << YGNodeLayoutGetPadding(node, YGEdgeLeft)
              << ", T=" << YGNodeLayoutGetPadding(node, YGEdgeTop)
              << ", R=" << YGNodeLayoutGetPadding(node, YGEdgeRight)
              << ", B=" << YGNodeLayoutGetPadding(node, YGEdgeBottom) << "\n\n";
    
    // 递归打印子节点
    for (size_t i = 0; i < YGNodeGetChildCount(node); i++) {
        YGNodeRef child = YGNodeGetChild(node, i);
        printNodeLayout(child, name + "_child_" + std::to_string(i), depth + 1);
    }
}

// 示例1：基本水平布局
void demoBasicHorizontalLayout() {
    std::cout << "=== 示例1：基本水平布局 ===\n";
    
    // 创建根容器
    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, 300);
    YGNodeStyleSetHeight(root, 100);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
    YGNodeStyleSetPadding(root, YGEdgeAll, 10);
    
    // 创建三个子节点
    YGNodeRef child1 = YGNodeNew();
    YGNodeStyleSetFlexGrow(child1, 1);
    YGNodeStyleSetWidth(child1, 50);
    YGNodeStyleSetMargin(child1, YGEdgeRight, 5);
    YGNodeInsertChild(root, child1, 0);
    
    YGNodeRef child2 = YGNodeNew();
    YGNodeStyleSetFlexGrow(child2, 2);
    YGNodeStyleSetWidth(child2, 50);
    YGNodeStyleSetMargin(child2, YGEdgeRight, 5);
    YGNodeInsertChild(root, child2, 1);
    
    YGNodeRef child3 = YGNodeNew();
    YGNodeStyleSetFlexGrow(child3, 1);
    YGNodeStyleSetWidth(child3, 50);
    YGNodeInsertChild(root, child3, 2);
    
    // 计算布局
    YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);
    
    // 打印结果
    printNodeLayout(root, "HorizontalContainer");
    
    // 清理内存
    YGNodeFreeRecursive(root);
}

// 示例2：垂直布局和对齐方式
void demoVerticalLayoutWithAlignment() {
    std::cout << "=== 示例2：垂直布局和对齐方式 ===\n";
    
    // 创建根容器
    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, 200);
    YGNodeStyleSetHeight(root, 200);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionColumn);
    YGNodeStyleSetAlignItems(root, YGAlignCenter);
    YGNodeStyleSetJustifyContent(root, YGJustifySpaceBetween);
    YGNodeStyleSetPadding(root, YGEdgeAll, 15);
    
    // 创建不同尺寸的子节点
    YGNodeRef child1 = YGNodeNew();
    YGNodeStyleSetWidth(child1, 80);
    YGNodeStyleSetHeight(child1, 40);
    YGNodeInsertChild(root, child1, 0);
    
    YGNodeRef child2 = YGNodeNew();
    YGNodeStyleSetWidth(child2, 120);
    YGNodeStyleSetHeight(child2, 30);
    YGNodeInsertChild(root, child2, 1);
    
    YGNodeRef child3 = YGNodeNew();
    YGNodeStyleSetWidth(child3, 60);
    YGNodeStyleSetHeight(child3, 50);
    YGNodeInsertChild(root, child3, 2);
    
    // 计算布局
    YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);
    
    // 打印结果
    printNodeLayout(root, "VerticalContainer");
    
    // 清理内存
    YGNodeFreeRecursive(root);
}

// 示例3：嵌套布局
void demoNestedLayout() {
    std::cout << "=== 示例3：嵌套布局 ===\n";
    
    // 创建根容器
    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, 400);
    YGNodeStyleSetHeight(root, 300);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
    YGNodeStyleSetPadding(root, YGEdgeAll, 10);
    
    // 创建左侧列容器
    YGNodeRef leftColumn = YGNodeNew();
    YGNodeStyleSetFlexGrow(leftColumn, 1);
    YGNodeStyleSetFlexDirection(leftColumn, YGFlexDirectionColumn);
    YGNodeStyleSetMargin(leftColumn, YGEdgeRight, 10);
    
    // 左侧列的子节点
    YGNodeRef leftChild1 = YGNodeNew();
    YGNodeStyleSetHeight(leftChild1, 80);
    YGNodeStyleSetMargin(leftChild1, YGEdgeBottom, 10);
    YGNodeInsertChild(leftColumn, leftChild1, 0);
    
    YGNodeRef leftChild2 = YGNodeNew();
    YGNodeStyleSetFlexGrow(leftChild2, 1);
    YGNodeInsertChild(leftColumn, leftChild2, 1);
    
    // 创建右侧列容器
    YGNodeRef rightColumn = YGNodeNew();
    YGNodeStyleSetFlexGrow(rightColumn, 1);
    YGNodeStyleSetFlexDirection(rightColumn, YGFlexDirectionColumn);
    
    // 右侧列的子节点
    YGNodeRef rightChild1 = YGNodeNew();
    YGNodeStyleSetHeight(rightChild1, 60);
    YGNodeStyleSetMargin(rightChild1, YGEdgeBottom, 10);
    YGNodeInsertChild(rightColumn, rightChild1, 0);
    
    YGNodeRef rightChild2 = YGNodeNew();
    YGNodeStyleSetHeight(rightChild2, 60);
    YGNodeStyleSetMargin(rightChild2, YGEdgeBottom, 10);
    YGNodeInsertChild(rightColumn, rightChild2, 1);
    
    YGNodeRef rightChild3 = YGNodeNew();
    YGNodeStyleSetFlexGrow(rightChild3, 1);
    YGNodeInsertChild(rightColumn, rightChild3, 2);
    
    // 将左右列添加到根容器
    YGNodeInsertChild(root, leftColumn, 0);
    YGNodeInsertChild(root, rightColumn, 1);
    
    // 计算布局
    YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);
    
    // 打印结果
    printNodeLayout(root, "NestedContainer");
    
    // 清理内存
    YGNodeFreeRecursive(root);
}

// 示例4：绝对定位
void demoAbsolutePositioning() {
    std::cout << "=== 示例4：绝对定位 ===\n";
    
    // 创建根容器
    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, 300);
    YGNodeStyleSetHeight(root, 200);
    YGNodeStyleSetPadding(root, YGEdgeAll, 20);
    
    // 创建一个相对定位的子节点
    YGNodeRef relativeChild = YGNodeNew();
    YGNodeStyleSetWidth(relativeChild, 100);
    YGNodeStyleSetHeight(relativeChild, 50);
    YGNodeStyleSetMargin(relativeChild, YGEdgeLeft, 20);
    YGNodeStyleSetMargin(relativeChild, YGEdgeTop, 30);
    YGNodeInsertChild(root, relativeChild, 0);
    
    // 创建一个绝对定位的子节点
    YGNodeRef absoluteChild = YGNodeNew();
    YGNodeStyleSetPositionType(absoluteChild, YGPositionTypeAbsolute);
    YGNodeStyleSetWidth(absoluteChild, 80);
    YGNodeStyleSetHeight(absoluteChild, 40);
    YGNodeStyleSetPosition(absoluteChild, YGEdgeRight, 10);
    YGNodeStyleSetPosition(absoluteChild, YGEdgeBottom, 10);
    YGNodeInsertChild(root, absoluteChild, 1);
    
    // 计算布局
    YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);
    
    // 打印结果
    printNodeLayout(root, "AbsoluteContainer");
    
    // 清理内存
    YGNodeFreeRecursive(root);
}

int main() {
    std::cout << "Yoga Flexbox布局引擎演示程序\n";
    std::cout << "================================\n\n";
    
    try {
        // 运行各种示例
        demoBasicHorizontalLayout();
        std::cout << "\n";
        
        demoVerticalLayoutWithAlignment();
        std::cout << "\n";
        
        demoNestedLayout();
        std::cout << "\n";
        
        demoAbsolutePositioning();
        
        std::cout << "所有演示完成！\n";
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
