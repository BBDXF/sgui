# Yoga Demo

这个demo演示了如何使用Yoga Flexbox布局引擎进行UI布局计算。

## 什么是Yoga？

Yoga是Facebook开发的一个高性能、可嵌入的Flexbox布局引擎，它实现了CSS Flexbox布局规范。Yoga主要用于UI布局计算，支持多种语言绑定。

## 主要特性

- **完整的Flexbox支持**: 实现了CSS Flexbox布局规范
- **高性能**: 专为UI布局计算优化
- **跨平台**: 支持多种平台和语言
- **可嵌入**: 可以轻松集成到现有的应用程序中

## Demo内容

这个demo包含了四个示例，展示了Yoga的基本用法：

### 1. 基本水平布局
演示了如何创建一个水平方向的flex容器，包含三个具有不同flex-grow值的子节点。

### 2. 垂直布局和对齐方式
展示了垂直布局、居中对齐和子元素之间的间距分配。

### 3. 嵌套布局
演示了复杂的嵌套布局结构，包括水平容器中包含垂直子容器。

### 4. 绝对定位
展示了相对定位和绝对定位元素的使用。

## 编译和运行

### 编译
```bash
cd build
make yoga_demo
```

### 运行
```bash
cd build
./bin/yoga_demo
```

## Yoga基本用法

### 1. 创建节点
```cpp
YGNodeRef root = YGNodeNew();
YGNodeRef child = YGNodeNew();
```

### 2. 设置样式
```cpp
YGNodeStyleSetWidth(root, 300);
YGNodeStyleSetHeight(root, 100);
YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
YGNodeStyleSetPadding(root, YGEdgeAll, 10);
```

### 3. 构建节点树
```cpp
YGNodeInsertChild(root, child, 0);
```

### 4. 计算布局
```cpp
YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);
```

### 5. 获取布局结果
```cpp
float left = YGNodeLayoutGetLeft(node);
float top = YGNodeLayoutGetTop(node);
float width = YGNodeLayoutGetWidth(node);
float height = YGNodeLayoutGetHeight(node);
```

### 6. 清理内存
```cpp
YGNodeFreeRecursive(root);
```

## 支持的布局属性

- **Flex Direction**: row, column, row-reverse, column-reverse
- **Justify Content**: flex-start, center, flex-end, space-between, space-around, space-evenly
- **Align Items**: flex-start, center, flex-end, stretch, baseline
- **Position**: static, relative, absolute
- **尺寸单位**: point, percent, auto, max-content, fit-content
- **Margin, Padding, Border**: 完整的盒模型支持
- **Flex Wrap, Overflow, Display**: 等更多属性

## 输出说明

程序会输出每个示例的布局结果，包括：
- 节点位置 (left, top)
- 节点尺寸 (width, height)
- 边距 (margin)
- 内边距 (padding)

这些数值可以帮助你理解Yoga如何计算布局。

## 相关资源

- [Yoga官方文档](https://yogalayout.com/)
- [Flexbox规范](https://www.w3.org/TR/css-flexbox-1/)
- [Yoga GitHub仓库](https://github.com/facebook/yoga)
