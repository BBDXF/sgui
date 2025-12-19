# Container基类设计

基于Yoga Flexbox布局引擎的GUI容器基类设计演示。

## 设计目标

创建一个基于Yoga的Container基类，作为所有GUI组件的基础，提供：
- 完整的Flexbox布局功能
- 类型安全的C++接口
- 易于扩展的架构
- 现代化的内存管理

## 核心组件

### 1. LayoutValue 结构
```cpp
struct LayoutValue {
    float value = 0.0f;
    bool isPercent = false;
    bool isAuto = false;
    
    static LayoutValue Auto();
    static LayoutValue Percent(float v);
    static LayoutValue Point(float v);
};
```

支持三种值类型：
- **Point**: 像素值（如 100px）
- **Percent**: 百分比值（如 50%）
- **Auto**: 自动计算

### 2. EdgeInsets 结构
```cpp
struct EdgeInsets {
    LayoutValue left, top, right, bottom;
    
    static EdgeInsets All(float value);
    static EdgeInsets Horizontal(float value);
    static EdgeInsets Vertical(float value);
    static EdgeInsets Symmetric(float horizontal, float vertical);
    static EdgeInsets Only(float left, float top, float right, float bottom);
};
```

统一管理边距、内边距和位置。

### 3. SContainer 基类
提供完整的布局功能：

#### 布局属性
- **尺寸**: width, height, minWidth, minHeight, maxWidth, maxHeight
- **Flex属性**: flex, flexGrow, flexShrink, flexBasis
- **布局方向**: flexDirection, justifyContent, alignItems, alignSelf, alignContent
- **定位**: positionType, position (left, top, right, bottom)
- **间距**: margin, padding, border
- **其他**: flexWrap, overflow, display, aspectRatio, direction

#### 子节点管理
```cpp
void addChild(const SContainerPtr& child);
void insertChild(const SContainerPtr& child, size_t index);
void removeChild(const SContainerPtr& child);
void removeAllChildren();
size_t getChildCount() const;
SContainerPtr getChildAt(size_t index) const;
```

#### 布局计算
```cpp
void calculateLayout(float width = YGUndefined, float height = YGUndefined);
float getLeft() const;
float getTop() const;
float getLayoutWidth() const;
float getLayoutHeight() const;
```

## 设计优势

### 1. 类型安全
- 使用强类型枚举而非字符串
- 编译时错误检查
- IDE自动补全支持

### 2. 现代C++特性
- 智能指针管理内存
- RAII资源管理
- 异常安全

### 3. 易于使用
- 链式调用支持
- 工厂函数简化创建
- 直观的API设计

### 4. 高性能
- 直接封装Yoga引擎
- 零拷贝设计
- 高效的布局计算

## 使用示例

### 基本布局
```cpp
auto root = sgui::createContainer("Root");
root->setWidth(sgui::LayoutValue::Point(300));
root->setHeight(sgui::LayoutValue::Point(200));
root->setPadding(sgui::EdgeInsets::All(10));

auto header = sgui::createRow("Header");
header->setHeight(sgui::LayoutValue::Point(50));

auto title = sgui::createContainer("Title");
title->setWidth(sgui::LayoutValue::Percent(70));

header->addChild(title);
root->addChild(header);
root->calculateLayout();
```

### 复杂布局
```cpp
auto main = sgui::createRow("MainContainer");
main->setWidth(sgui::LayoutValue::Point(800));
main->setPadding(sgui::EdgeInsets::All(20));

auto sidebar = sgui::createColumn("Sidebar");
sidebar->setWidth(sgui::LayoutValue::Percent(25));

auto content = sgui::createColumn("Content");
content->setFlexGrow(1.0);

main->addChild(sidebar);
main->addChild(content);
```

## 扩展性

### 自定义组件
```cpp
class Button : public sgui::SContainer {
public:
    Button(const std::string& text) {
        setPadding(sgui::EdgeInsets::Symmetric(16, 8));
        setAlignItems(sgui::SContainer::Align::Center);
        // 自定义样式和行为
    }
    
    void draw() override {
        // 自定义绘制逻辑
    }
};
```

### 响应式设计
```cpp
auto container = sgui::createContainer("Responsive");
container->setWidth(sgui::LayoutValue::Percent(100));

if (isMobile) {
    container->setFlexDirection(sgui::SContainer::FlexDirection::Column);
} else {
    container->setFlexDirection(sgui::SContainer::FlexDirection::Row);
}
```

## 与Yoga的集成

Container类完全封装了Yoga的所有功能：
- 自动管理YGNode生命周期
- 类型安全的属性设置
- 统一的错误处理
- 性能优化的布局计算

## 编译和运行

```bash
cd build
make container_demo
./bin/container_demo
```

## 未来扩展

1. **动画支持**: 属性过渡和关键帧动画
2. **事件系统**: 鼠标、键盘、触摸事件处理
3. **主题系统**: 颜色、字体、样式主题
4. **国际化**: 文本方向和本地化支持
5. **可访问性**: ARIA标签和屏幕阅读器支持

这个Container基类为构建现代化、高性能的GUI应用提供了坚实的基础。
