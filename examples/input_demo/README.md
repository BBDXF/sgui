# Input Component Demo

这个演示程序展示了SGUI框架中SInput组件的各种功能和特性。

## 功能特性

### 输入类型支持
- **Text**: 标准文本输入
- **Password**: 密码输入，字符会被掩码显示
- **Number**: 数字输入，只允许数字和小数点
- **Email**: 邮箱输入，基本邮箱格式验证
- **Search**: 搜索输入，支持回车键回调

### 交互功能
- **文本编辑**: 插入、删除、选择文本
- **光标控制**: 使用方向键移动光标
- **文本选择**: 支持全选（Ctrl+A）和鼠标选择
- **撤销/重做**: 支持Ctrl+Z撤销和Ctrl+Y重做
- **复制/粘贴/剪切**: 支持Ctrl+C/Ctrl+V/Ctrl+X操作
- **键盘导航**: Home/End键快速移动到行首/行尾

### 状态管理
- **Normal**: 正常状态
- **Hover**: 鼠标悬停状态
- **Focused**: 获得焦点状态
- **Pressed**: 鼠标按下状态
- **Disabled**: 禁用状态
- **Readonly**: 只读状态

### 样式定制
- **背景色**: 支持不同状态的背景色
- **边框色**: 支持不同状态的边框色
- **文本色**: 可自定义文本颜色
- **占位符**: 支持自定义占位符文本和颜色
- **光标**: 可自定义光标颜色和宽度
- **选择高亮**: 可自定义文本选择高亮颜色
- **圆角**: 支持边框圆角设置

### 事件回调
- **文本变化**: `setOnTextChanged()` - 文本内容改变时触发
- **焦点变化**: `setOnFocusChanged()` - 获得/失去焦点时触发
- **回车按键**: `setOnEnterPressed()` - 按下回车键时触发

## 编译和运行

### 编译
```bash
# 在项目根目录下
mkdir build
cd build
cmake ..
make input_demo
```

### 运行
```bash
# 在build目录下
./bin/input_demo
```

## 使用示例

### 基本文本输入
```cpp
auto input = std::make_shared<SInput>("Enter text here...");
input->setWidth(LayoutValue::Percent(100.0f));
input->setHeight(LayoutValue::Point(40.0f));
input->setInputType(InputType::Text);

// 设置文本变化回调
input->setOnTextChanged([](const std::string& text) {
    std::cout << "Text: " << text << std::endl;
});
```

### 密码输入
```cpp
auto passwordInput = std::make_shared<SInput>("Password");
passwordInput->setInputType(InputType::Password);
passwordInput->setMaxLength(20);
```

### 自定义样式
```cpp
auto styledInput = std::make_shared<SInput>("Custom styled");
styledInput->setFontSize(16.0f);
styledInput->setFontWeight(FontWeight::Bold);
styledInput->setBorderRadius(EdgeInsets::All(25.0f));
styledInput->setBackgroundColor(Color(0.9, 0.95, 1.0, 1.0));
styledInput->setBorderColor(Color(0.3, 0.5, 0.8, 1.0));
styledInput->setCursorColor(Color(0.3, 0.5, 0.8, 1.0));
```

### 搜索输入框
```cpp
auto searchInput = std::make_shared<SInput>("Search...");
searchInput->setInputType(InputType::Search);
searchInput->setOnEnterPressed([](const std::string& text) {
    std::cout << "Searching: " << text << std::endl;
    // 执行搜索操作
});
```

## 键盘快捷键

| 快捷键 | 功能 |
|--------|------|
| `←` `→` | 移动光标 |
| `Home` `End` | 移动到行首/行尾 |
| `Ctrl+A` | 全选文本 |
| `Ctrl+C` | 复制选中文本 |
| `Ctrl+V` | 粘贴文本 |
| `Ctrl+X` | 剪切选中文本 |
| `Ctrl+Z` | 撤销 |
| `Ctrl+Y` | 重做 |
| `Backspace` | 删除前一个字符 |
| `Delete` | 删除后一个字符 |
| `Enter` | 回车键（触发回调） |
| `Escape` | 取消焦点 |

## 架构设计

SInput组件继承自SContainer，最大化利用了现有的基础设施：

- **样式系统**: 复用SContainer的背景、边框、文本样式
- **事件处理**: 复用鼠标和键盘事件处理框架
- **布局系统**: 完全兼容Yoga Flexbox布局
- **渲染系统**: 基于Cairo的高质量文本渲染
- **状态管理**: 参考SButton的状态管理模式

这种设计确保了与现有系统的完美集成，同时提供了丰富的输入功能。
