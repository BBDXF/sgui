## sgui_container.h/cpp 已支持的属性分析

### 1. 布局相关属性（基于Yoga Flexbox）

#### 尺寸相关

- ✅ `width`, `height` - 基本尺寸
- ✅ `minWidth`, `minHeight` - 最小尺寸
- ✅ `maxWidth`, `maxHeight` - 最大尺寸
- ✅ `aspectRatio` - 宽高比

#### Flexbox布局

- ✅ `flexDirection` - 布局方向（row, column, row-reverse, column-reverse）
- ✅ `justifyContent` - 主轴对齐（flex-start, center, flex-end, space-between, space-around, space-evenly）
- ✅ `alignItems` - 交叉轴对齐（flex-start, center, flex-end, stretch, baseline）
- ✅ `alignSelf` - 单个元素对齐
- ✅ `alignContent` - 多行对齐
- ✅ `flexWrap` - 换行控制（nowrap, wrap, wrap-reverse）
- ✅ `flex` - 简写属性
- ✅ `flexGrow` - 扩展比例
- ✅ `flexShrink` - 收缩比例
- ✅ `flexBasis` - 基础尺寸

#### 定位相关

- ✅ `position` - 定位类型（static, relative, absolute）
- ✅ `left`, `top`, `right`, `bottom` - 位置偏移
- ✅ `display` - 显示类型（flex, none）

#### 间距相关

- ✅ `margin` - 外边距（支持四边独立设置）
- ✅ `padding` - 内边距（支持四边独立设置）
- ✅ `border` - 边框宽度（支持四边独立设置）

#### 其他布局属性

- ✅ `overflow` - 溢出处理（visible, hidden, scroll）
- ✅ `direction` - 文本方向（ltr, rtl, inherit）

### 2. 高级功能

- ✅ 自定义测量函数（`onMeasure`）
- ✅ 布局变化回调（`onLayoutChanged`）
- ✅ 绘制接口（`draw`）
- ✅ 用户数据支持（`setUserData`）
- ✅ 布局树调试（`printLayoutTree`）

## 缺少的CSS常见布局和美化属性

### 1. 视觉样式类

#### 背景相关

- ❌ `backgroundColor` - 背景色
- ❌ `backgroundImage` - 背景图片
- ❌ `backgroundSize` - 背景尺寸
- ❌ `backgroundPosition` - 背景位置
- ❌ `backgroundRepeat` - 背景重复
- ❌ `backgroundGradient` - 渐变背景

#### 边框样式

- ❌ `borderColor` - 边框颜色
- ❌ `borderStyle` - 边框样式（solid, dashed, dotted等）
- ❌ `borderRadius` - 圆角
- ❌ `borderWidth` - 边框宽度（已有基础支持，但缺少样式）
- ❌ `boxShadow` - 阴影

#### 文本样式

- ❌ `color` - 文本颜色
- ❌ `fontSize` - 字体大小
- ❌ `fontFamily` - 字体族
- ❌ `fontWeight` - 字体粗细
- ❌ `fontStyle` - 字体样式（italic, oblique）
- ❌ `textAlign` - 文本对齐
- ❌ `textDecoration` - 文本装饰（underline, line-through等）
- ❌ `textOverflow` - 文本溢出处理
- ❌ `lineHeight` - 行高
- ❌ `letterSpacing` - 字符间距
- ❌ `wordSpacing` - 单词间距
- ❌ `textIndent` - 文本缩进

### 2. 高级布局属性

#### Grid布局

- ❌ `display: grid` - 网格布局
- ❌ `gridTemplateColumns/Rows` - 网格模板
- ❌ `gridGap` - 网格间距
- ❌ `gridArea` - 网格区域
- ❌ `gridColumn/Row` - 网格位置

#### 多列布局

- ❌ `columnCount` - 列数
- ❌ `columnWidth` - 列宽
- ❌ `columnGap` - 列间距

### 3. 变换和动画

#### 2D/3D变换

- ❌ `transform` - 2D变换（translate, rotate, scale, skew）
- ❌ `transformOrigin` - 变换原点
- ❌ `perspective` - 透视

#### 过渡和动画

- ❌ `transition` - 属性过渡
- ❌ `animation` - 关键帧动画
- ❌ `transitionDuration/Property/Timing/Delay` - 过渡详细属性

### 4. 交互和状态

#### 鼠标样式

- ❌ `cursor` - 鼠标指针样式
- ❌ `pointerEvents` - 指针事件控制

#### 用户选择

- ❌ `userSelect` - 文本选择控制
- ❌ `resize` - 元素可调整大小

### 5. 滤镜和效果

- ❌ `opacity` - 透明度
- ❌ `filter` - CSS滤镜（blur, brightness等）
- ❌ `backdropFilter` - 背景滤镜
- ❌ `mixBlendMode` - 混合模式

### 6. 响应式设计

#### 媒体查询支持

- ❌ 媒体查询接口
- ❌ 断点管理
- ❌ 响应式属性切换

#### 容器查询

- ❌ `container-type` - 容器类型
- ❌ `container-queries` - 容器查询

### 7. 可访问性

- ❌ `aria` 属性支持
- ❌ `tabIndex` - Tab键导航
- ❌ `role` - 语义角色
- ❌ `accessibleName` - 可访问名称

### 8. 主题和变量

- ❌ CSS变量支持
- ❌ 主题切换机制
- ❌ 颜色系统
- ❌ 深色/浅色模式

### 9. 滚动行为

- ❌ `scrollBehavior` - 滚动行为（smooth, auto）
- ❌ `overscrollBehavior` - 过滚动行为
- ❌ `scrollSnapType/Align` - 滚动捕捉

### 10. 打印样式

- ❌ `print` 媒体样式支持
- ❌ `pageBreak` 相关属性

## 总结

当前的`sgui_container`已经实现&#x4E86;__&#x5B8C;整的Flexbox布局系统__，覆盖了所有核心的布局需求。但&#x5728;__&#x89C6;觉美化、交互效果、动画、响应式设&#x8BA1;__&#x7B49;方面还有很大扩展空间。

建议优先级：

1. __高优先级__: 背景色、边框样式、圆角、阴影等基础视觉样式
2. __中优先级__: 文本样式、透明度、基础变换
3. __低优先级__: 动画系统、Grid布局、高级滤镜

这样可以逐步构建一个功能完整、视觉丰富的GUI系统。
