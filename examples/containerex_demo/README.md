# ContainerEx Demo

这个示例程序演示了 `ContainerEx` 类的所有样式功能，包括背景、边框和文本样式。

## 功能特性

### 背景相关
- ✅ `backgroundColor` - 背景色
- ✅ `backgroundImage` - 背景图片
- ✅ `backgroundGradient` - 渐变背景

### 边框样式
- ✅ `borderColor` - 边框颜色
- ✅ `borderStyle` - 边框样式（solid, dashed, dotted等）
- ✅ `borderRadius` - 圆角
- ✅ `borderWidth` - 边框宽度（通过基类支持）
- ✅ `boxShadow` - 阴影

### 文本样式
- ✅ `color` - 文本颜色
- ✅ `fontSize` - 字体大小
- ✅ `fontFamily` - 字体族
- ✅ `fontWeight` - 字体粗细
- ✅ `fontStyle` - 字体样式（italic, oblique）
- ✅ `textAlign` - 文本对齐
- ✅ `textDecoration` - 文本装饰（underline, line-through等）
- ✅ `textOverflow` - 文本溢出处理
- ✅ `lineHeight` - 行高
- ✅ `textIndent` - 文本缩进

## 编译和运行

### 编译
```bash
cd examples/containerex_demo
mkdir build
cd build
cmake ..
make
```

### 运行
```bash
./containerex_demo
```

## 示例说明

程序包含以下演示部分：

1. **颜色工具演示** - 展示 `Color` 结构体的静态方法
2. **背景功能演示** - 演示背景色、背景图片和渐变背景
3. **边框功能演示** - 演示边框颜色、样式、圆角和阴影
4. **文本功能演示** - 演示各种文本样式属性
5. **综合功能演示** - 演示所有样式的组合效果
6. **样式管理演示** - 演示样式的设置、清除和重置

## 输出示例

程序运行时会输出详细的绘制信息，包括：

- 背景绘制信息（颜色、图片路径、渐变参数）
- 边框绘制信息（颜色、样式、圆角、阴影参数）
- 文本绘制信息（内容、颜色、字体属性、对齐方式）
- 布局测量信息（文本尺寸估算）

## 代码结构

- `main.cpp` - 主演示程序
- `CMakeLists.txt` - 构建配置

## 依赖

- Yoga 布局引擎
- C++17 或更高版本
- CMake 3.10 或更高版本

## 注意事项

当前实现使用控制台输出演示绘制过程，实际图形渲染需要集成具体的渲染库（如OpenGL、DirectX等）。

文本尺寸测量使用简单的估计算法，精确测量需要集成字体渲染库（如FreeType）。

背景图片加载和渐变渲染需要相应的图形处理支持。
