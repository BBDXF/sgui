# Katana Parser CSS 解析库使用指南

## 概述

Katana 是一个纯 C99 实现的 CSS 解析库，无外部依赖，专为构建其他工具和库（如 linter、验证器、模板语言、重构和分析工具）而设计。

## 主要特性

- **简单易用的 API**：可以轻松被其他语言包装
- **轻量级**：无外部依赖
- **支持片段解析**：可以解析 CSS 的各个部分
- **符合 CSS 语法规范**：实现 W3C CSS 语法算法

## 核心 API

### 1. 解析函数

```c
// 解析 CSS 字符串
KatanaOutput* katana_parse(const char* str, size_t len, KatanaParserMode mode);

// 解析 CSS 文件
KatanaOutput* katana_parse_in(FILE* fp);

// 释放解析结果
void katana_destroy_output(KatanaOutput* output);

// 打印格式化的 CSS
KatanaOutput* katana_dump_output(KatanaOutput* output);
```

### 2. 解析模式

```c
typedef enum KatanaParserMode {
    KatanaParserModeStylesheet,        // 完整 CSS 样式表
    KatanaParserModeRule,              // 单个 CSS 规则
    KatanaParserModeKeyframeRule,      // 关键帧规则
    KatanaParserModeKeyframeKeyList,   // 关键帧键列表
    KatanaParserModeMediaList,         // 媒体查询列表
    KatanaParserModeValue,             // CSS 属性值
    KatanaParserModeSelector,          // CSS 选择器
    KatanaParserModeDeclarationList,    // CSS 声明列表
} KatanaParserMode;
```

## 数据结构

### 1. 主要输出结构

```c
typedef struct KatanaOutput {
    KatanaStylesheet* stylesheet;  // 完整样式表
    union {
        KatanaRule* rule;         // 单个规则
        KatanaKeyframe* keyframe; // 关键帧
        KatanaArray* keyframe_keys;
        KatanaArray* values;      // 值列表
        KatanaArray* medias;      // 媒体查询列表
        KatanaArray* declarations; // 声明列表
        KatanaArray* selectors;   // 选择器列表
    };
    KatanaParserMode mode;        // 解析模式
    KatanaArray errors;          // 错误列表
} KatanaOutput;
```

### 2. 样式表结构

```c
typedef struct {
    const char* encoding;
    KatanaArray rules;    // KatanaRule 数组
    KatanaArray imports;  // KatanaImportRule 数组
} KatanaStylesheet;
```

### 3. 规则类型

```c
typedef enum {
    KatanaRuleUnkown,
    KatanaRuleStyle,      // 样式规则
    KatanaRuleImport,      // @import 规则
    KatanaRuleMedia,      // @media 规则
    KatanaRuleFontFace,   // @font-face 规则
    KatanaRuleSupports,   // @supports 规则
    KatanaRuleKeyframes,  // @keyframes 规则
    KatanaRuleCharset,    // @charset 规则
    KatanaRuleHost,      // @host 规则
} KatanaRuleType;
```

## 基本使用示例

### 1. 解析完整 CSS

```c
#include "katana.h"

int main() {
    const char* css = "body { margin: 0; padding: 0; }";
    
    // 解析 CSS
    KatanaOutput* output = katana_parse(css, strlen(css), KatanaParserModeStylesheet);
    
    if (output) {
        // 打印解析结果
        katana_dump_output(output);
        
        // 释放内存
        katana_destroy_output(output);
    }
    
    return 0;
}
```

### 2. 解析 CSS 选择器

```c
const char* selector = ".container > .item:nth-child(2n+1)";
KatanaOutput* output = katana_parse(selector, strlen(selector), KatanaParserModeSelector);
```

### 3. 解析 CSS 属性值

```c
const char* value = "linear-gradient(45deg, #007bff, #0056b3)";
KatanaOutput* output = katana_parse(value, strlen(value), KatanaParserModeValue);
```

## 高级用法：CSS 到 Key-Value 转换

我们创建了一个完整的 C++ 示例，展示如何将 KatanaOutput 转换为易于使用的 key-value 格式：

### 主要功能

1. **选择器解析**：支持所有 CSS 选择器类型
   - 标签选择器：`div`
   - 类选择器：`.class`
   - ID 选择器：`#id`
   - 属性选择器：`[attr="value"]`
   - 伪类选择器：`:hover`
   - 伪元素选择器：`::before`
   - 组合选择器：`div > p + span`

2. **属性值解析**：支持所有 CSS 值类型
   - 长度单位：`px`, `em`, `rem`, `%`
   - 颜色值：`#fff`, `rgb()`, `rgba()`
   - 函数值：`linear-gradient()`, `calc()`
   - 复合值：`margin: 10px 20px`

3. **@规则处理**：
   - `@import`
   - `@media`
   - `@font-face`
   - `@charset`

### 使用示例

```cpp
CSSKeyValueConverter converter;

// 解析 CSS
converter.parseCSS(css_string);

// 获取所有选择器
auto selectors = converter.getAllSelectors();

// 获取特定选择器的所有属性
auto props = converter.getProperties(".button");

// 获取特定属性值
std::string bgColor = converter.getPropertyValue("body", "background-color");

// 打印 key-value 格式
converter.printKeyValueFormat();
```

## 编译和安装

### 1. 构建库

```bash
git clone https://github.com/hackers-painters/katana-parser.git
cd katana-parser
./autogen.sh
./configure CFLAGS="-std=c99"
make
sudo make install
```

### 2. 使用 pkg-config

```bash
gcc your_program.c `pkg-config --cflags --libs katana` -o your_program
```

### 3. 集成到 CMake 项目

```cmake
# 添加 katana 库路径
set(KATANA_ROOT_DIR "${CMAKE_SOURCE_DIR}/katana-parser")
include_directories(${KATANA_ROOT_DIR}/src)
link_directories(${KATANA_ROOT_DIR}/.libs)

# 链接静态库
add_library(katana_static STATIC IMPORTED)
set_target_properties(katana_static PROPERTIES
    IMPORTED_LOCATION "${KATANA_ROOT_DIR}/.libs/libkatana.a"
)

# 创建可执行文件
add_executable(your_program your_program.cpp)
target_link_libraries(your_program katana_static)
```

## 解析结果示例

### 输入 CSS

```css
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background-color: #f0f0f0;
}

.button {
    display: inline-block;
    padding: 10px 20px;
    background: linear-gradient(45deg, #007bff, #0056b3);
    color: white;
}
```

### Key-Value 输出

```
【CSS 规则】
选择器: body
  font-family: Arial, sans-serif
  margin: 0
  padding: 20px
  background-color: #f0f0f0

选择器: .button
  display: inline-block
  padding: 10px 20px
  background: linear-gradient(45deg, #007bff, #0056b3)
  color: white
```

## 错误处理

```c
if (output->errors.length > 0) {
    for (unsigned int i = 0; i < output->errors.length; i++) {
        KatanaError* error = (KatanaError*)output->errors.data[i];
        printf("错误: 行 %d, 列 %d: %s\n", 
               error->first_line, error->first_column, error->message);
    }
}
```

## 性能考虑

- Katana 使用自己的内存管理，记得调用 `katana_destroy_output()` 释放内存
- 对于大型 CSS 文件，建议分块处理
- 静态链接可以减少运行时依赖

## 扩展和自定义

Katana 的设计允许：
- 添加自定义 CSS 规则类型
- 扩展选择器匹配逻辑
- 实现自定义验证规则
- 集成到现有工具链中

## 总结

Katana 是一个强大而灵活的 CSS 解析库，特别适合：

1. **CSS 工具开发**：linter、格式化器、压缩器
2. **构建系统集成**：CSS 预处理器、后处理器
3. **编辑器插件**：语法高亮、自动完成
4. **分析工具**：CSS 使用统计、依赖分析
5. **转换工具**：CSS 到其他格式的转换器

通过我们的 C++ 包装器，你可以轻松地将 CSS 解析结果转换为应用程序友好的 key-value 格式，便于进一步处理和使用。
