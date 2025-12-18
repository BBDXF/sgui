/**
 * Katana Parser CSS 解析 Demo
 * 
 * 这个 demo 展示了如何使用 katana-parser 库来解析 CSS
 * 包括完整的样式表解析、片段解析和各种解析模式
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

// 包含 katana parser 头文件
extern "C" {
#include "katana.h"
}

class KatanaCSSDemo {
private:
    void printSeparator(const char* title) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << " " << title << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }

    void demoBasicParsing() {
        printSeparator("基础 CSS 解析演示");
        
        const char* css = "body { margin: 0; padding: 0; }";
        std::cout << "输入 CSS: " << css << std::endl;
        
        KatanaOutput* output = katana_parse(css, strlen(css), KatanaParserModeStylesheet);
        if (output) {
            std::cout << "\n解析结果:" << std::endl;
            katana_dump_output(output);
            katana_destroy_output(output);
        }
    }

    void demoComplexCSS() {
        printSeparator("复杂 CSS 样式表解析");
        
        const char* complexCSS = R"(
/* 这是一个复杂的 CSS 示例 */
@import url("reset.css");

body {
    font-family: Arial, sans-serif;
    background-color: #f0f0f0;
    margin: 0;
    padding: 20px;
}

.container {
    width: 100%;
    max-width: 1200px;
    margin: 0 auto;
}

@media (min-width: 768px) {
    .container {
        padding: 0 15px;
    }
}

.button {
    display: inline-block;
    padding: 10px 20px;
    background: linear-gradient(45deg, #007bff, #0056b3);
    color: white;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    transition: all 0.3s ease;
}

.button:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 8px rgba(0,0,0,0.2);
}
        )";
        
        std::cout << "解析复杂 CSS 样式表..." << std::endl;
        
        KatanaOutput* output = katana_parse(complexCSS, strlen(complexCSS), KatanaParserModeStylesheet);
        if (output) {
            katana_dump_output(output);
            katana_destroy_output(output);
        }
    }

    void demoSelectorParsing() {
        printSeparator("CSS 选择器解析");
        
        const char* selectors[] = {
            "div",
            ".class-name",
            "#id-name", 
            "div.container",
            "ul.nav li.active",
            "input[type='text']:focus",
            "a:hover::before",
            "body > header h1",
            ":nth-child(2n+1)",
            "[data-attribute*='value']"
        };
        
        for (int i = 0; i < 10; i++) {
            std::cout << "\n选择器: " << selectors[i] << std::endl;
            KatanaOutput* output = katana_parse(selectors[i], strlen(selectors[i]), KatanaParserModeSelector);
            if (output) {
                katana_dump_output(output);
                katana_destroy_output(output);
            }
        }
    }

    void demoValueParsing() {
        printSeparator("CSS 属性值解析");
        
        const char* values[] = {
            "10px",
            "1.5em",
            "rgba(255, 0, 0, 0.5)",
            "calc(100% - 20px)",
            "#ff0000",
            "url('image.jpg')",
            "linear-gradient(45deg, red, blue)",
            "2rem 1.5rem 0 1rem",
            "bold 16px/1.5 Arial",
            "scale(1.2) rotate(45deg)"
        };
        
        for (int i = 0; i < 10; i++) {
            std::cout << "\n属性值: " << values[i] << std::endl;
            KatanaOutput* output = katana_parse(values[i], strlen(values[i]), KatanaParserModeValue);
            if (output) {
                katana_dump_output(output);
                katana_destroy_output(output);
            }
        }
    }

    void demoMediaQueryParsing() {
        printSeparator("媒体查询解析");
        
        const char* mediaQueries[] = {
            "(min-width: 768px)",
            "screen and (max-width: 1024px)",
            "only screen and (orientation: portrait)",
            "not print",
            "all and (color)",
            "(min-resolution: 2dppx)",
            "screen and (min-width: 320px) and (max-width: 768px)"
        };
        
        for (int i = 0; i < 7; i++) {
            std::cout << "\n媒体查询: " << mediaQueries[i] << std::endl;
            KatanaOutput* output = katana_parse(mediaQueries[i], strlen(mediaQueries[i]), KatanaParserModeMediaList);
            if (output) {
                katana_dump_output(output);
                katana_destroy_output(output);
            }
        }
    }

    void demoDeclarationListParsing() {
        printSeparator("CSS 声明列表解析");
        
        const char* declarations[] = {
            "color: red;",
            "margin: 10px 20px; padding: 5px;",
            "font: bold 14px/1.5 'Helvetica Neue', sans-serif;",
            "background: #fff url('bg.jpg') no-repeat center center;",
            "transition: all 0.3s ease-in-out;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1), inset 0 1px 0 rgba(255,255,255,0.5);"
        };
        
        for (int i = 0; i < 6; i++) {
            std::cout << "\n声明列表: " << declarations[i] << std::endl;
            KatanaOutput* output = katana_parse(declarations[i], strlen(declarations[i]), KatanaParserModeDeclarationList);
            if (output) {
                katana_dump_output(output);
                katana_destroy_output(output);
            }
        }
    }

    void demoAtRules() {
        printSeparator("@ 规则解析");
        
        const char* atRules[] = {
            "@import url('styles.css') screen;",
            "@font-face { font-family: 'MyFont'; src: url('font.woff2'); }",
            "@media (min-width: 768px) { .container { width: 750px; } }",
            "@keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }",
            "@charset 'UTF-8';",
            "@supports (display: grid) { .grid { display: grid; } }"
        };
        
        for (int i = 0; i < 6; i++) {
            std::cout << "\n@规则: " << atRules[i] << std::endl;
            KatanaOutput* output = katana_parse(atRules[i], strlen(atRules[i]), KatanaParserModeRule);
            if (output) {
                katana_dump_output(output);
                katana_destroy_output(output);
            }
        }
    }

    void demoErrorHandling() {
        printSeparator("错误处理演示");
        
        const char* invalidCSS[] = {
            "div { color: ; }",           // 缺少值
            ".class { prop val",           // 缺少 }
            "@import",                     // 不完整的 @import
            "rgba(255, 0, 0)",             // 不完整的 rgba
            "media (min-width: 768px) {}", // 错误的 @media
        };
        
        for (int i = 0; i < 5; i++) {
            std::cout << "\n无效 CSS: " << invalidCSS[i] << std::endl;
            KatanaOutput* output = katana_parse(invalidCSS[i], strlen(invalidCSS[i]), KatanaParserModeStylesheet);
            if (output) {
                std::cout << "解析结果:" << std::endl;
                katana_dump_output(output);
                
                // 检查是否有错误
                if (output->errors.length > 0) {
                    std::cout << "\n发现错误:" << std::endl;
                    for (unsigned int j = 0; j < output->errors.length; j++) {
                        KatanaError* error = (KatanaError*)output->errors.data[j];
                        std::cout << "  行 " << error->first_line << ", 列 " << error->first_column 
                                  << ": " << error->message << std::endl;
                    }
                }
                
                katana_destroy_output(output);
            }
        }
    }

public:
    void runAllDemos() {
        std::cout << "Katana Parser CSS 解析演示程序" << std::endl;
        std::cout << "这个程序展示了如何使用 katana-parser 库来解析各种 CSS 内容" << std::endl;
        
        demoBasicParsing();
        demoComplexCSS();
        demoSelectorParsing();
        demoValueParsing();
        demoMediaQueryParsing();
        demoDeclarationListParsing();
        demoAtRules();
        demoErrorHandling();
        
        printSeparator("演示完成");
        std::cout << "所有演示已完成！这个 demo 展示了 katana-parser 的主要功能：" << std::endl;
        std::cout << "✓ 完整 CSS 样式表解析" << std::endl;
        std::cout << "✓ CSS 选择器解析" << std::endl;
        std::cout << "✓ CSS 属性值解析" << std::endl;
        std::cout << "✓ 媒体查询解析" << std::endl;
        std::cout << "✓ CSS 声明列表解析" << std::endl;
        std::cout << "✓ @ 规则解析" << std::endl;
        std::cout << "✓ 错误处理" << std::endl;
        std::cout << "\nkatana-parser 是一个功能强大的纯 C CSS 解析库！" << std::endl;
    }
};

int main() {
    try {
        KatanaCSSDemo demo;
        demo.runAllDemos();
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
