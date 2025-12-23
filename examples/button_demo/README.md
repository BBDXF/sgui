# SGUI Button Demo

这个演示程序展示了 SGUI 库中按钮控件的基本功能。

## 功能特性

### 鼠标交互
- **悬停效果**: 鼠标悬停在按钮上时，按钮背景色会变深
- **点击效果**: 点击按钮时，按钮会显示按下状态的样式
- **点击计数**: 第一个按钮会记录并显示点击次数
- **禁用功能**: 第二个按钮可以禁用/启用第一个按钮

### 状态样式
按钮支持以下状态，每种状态都有独立的样式设置：

- **Normal**: 正常状态（浅灰色背景）
- **Hover**: 鼠标悬停状态（灰色背景）
- **Pressed**: 鼠标按下状态（深灰色背景）
- **Disabled**: 禁用状态（灰白色背景）

## 编译和运行

```bash
cd build
make button_demo
./bin/button_demo
```

## 代码示例

### 创建按钮
```cpp
auto button = std::make_shared<SButton>("Click Me!");
```

### 设置点击回调
```cpp
button->setOnClick([]() {
    std::cout << "Button clicked!" << std::endl;
});
```

### 自定义状态颜色
```cpp
button->setNormalBackgroundColor(Color::LightGray());
button->setHoverBackgroundColor(Color::Gray());
button->setPressedBackgroundColor(Color::DarkGray());
```

### 禁用按钮
```cpp
button->setDisabled(true);
```

## 控件接口

### 主要方法
- `setButtonText(const std::string& text)`: 设置按钮文本
- `setOnClick(ButtonClickCallback callback)`: 设置点击回调函数
- `setDisabled(bool disabled)`: 设置禁用状态
- `isDisabled()`: 获取禁用状态
- `getState()`: 获取当前状态

### 样式设置方法
- `setNormalBackgroundColor(const Color& color)`
- `setHoverBackgroundColor(const Color& color)`
- `setPressedBackgroundColor(const Color& color)`
- `setDisabledBackgroundColor(const Color& color)`
- `setNormalBorderColor(const Color& color)`
- `setHoverBorderColor(const Color& color)`
- `setPressedBorderColor(const Color& color)`
- `setDisabledBorderColor(const Color& color)`
- `setNormalTextColor(const Color& color)`
- `setHoverTextColor(const Color& color)`
- `setPressedTextColor(const Color& color)`
- `setDisabledTextColor(const Color& color)`

## 事件处理

SButton 继承自 SContainer，重写了以下事件处理函数：
- `onMousePressed()`: 鼠标按下事件
- `onMouseReleased()`: 鼠标释放事件
- `onMouseClicked()`: 鼠标点击事件
- `onMouseMoved()`: 鼠标移动事件
- `onMouseEntered()`: 鼠标进入事件
- `onMouseExited()`: 鼠标离开事件

这些事件函数会自动处理按钮状态的变化，并触发相应的回调函数。
