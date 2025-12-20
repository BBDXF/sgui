/**
 * ContainerEx Demo - 演示扩展容器的样式功能
 */

#include "sgui_container.h"
#include <iostream>
#include <memory>

using namespace sgui;

void demonstrateBackgroundFeatures() {
    std::cout << "=== 背景功能演示 ===" << std::endl;
    
    auto container = std::make_shared<SContainer>();
    container->setWidth(300);
    container->setHeight(200);
    
    // 设置背景色
    container->setBackgroundColor(Color::fromRGB(255, 200, 200)); // 浅红色
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置背景图片
    container->clearBackground();
    container->setBackgroundImage("background.jpg");
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置渐变背景
    container->clearBackground();
    BackgroundGradient gradient;
    gradient.type = GradientType::Linear;
    gradient.angle = 45.0f;
    gradient.stops.push_back(GradientStop(Color::fromRGB(255, 0, 0), 0.0f));    // 红色
    gradient.stops.push_back(GradientStop(Color::fromRGB(0, 255, 0), 0.5f));    // 绿色
    gradient.stops.push_back(GradientStop(Color::fromRGB(0, 0, 255), 1.0f));    // 蓝色
    container->setBackgroundGradient(gradient);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
}

void demonstrateBorderFeatures() {
    std::cout << "=== 边框功能演示 ===" << std::endl;
    
    auto container = std::make_shared<SContainer>();
    container->setWidth(300);
    container->setHeight(200);
    
    // 设置边框颜色和样式
    container->setBorderColor(Color::fromRGB(0, 0, 255)); // 蓝色边框
    container->setBorderStyle(BorderStyle::Dashed);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置圆角
    container->setBorderRadius(EdgeInsets::All(10.0f));
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置阴影
    BoxShadow shadow;
    shadow.color = Color::fromRGB(0, 0, 0, 0.5f);
    shadow.offsetX = 5.0f;
    shadow.offsetY = 5.0f;
    shadow.blurRadius = 10.0f;
    shadow.spreadRadius = 2.0f;
    container->setBoxShadow(shadow);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
}

void demonstrateTextFeatures() {
    std::cout << "=== 文本功能演示 ===" << std::endl;
    
    auto container = std::make_shared<SContainer>();
    container->setWidth(400);
    container->setHeight(150);
    container->setPadding(EdgeInsets::All(20));
    
    // 设置基本文本
    container->setText("Hello, ContainerEx!");
    container->setColor(Color::fromRGB(0, 0, 0)); // 黑色文本
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置字体大小和粗细
    container->setFontSize(24.0f);
    container->setFontWeight(FontWeight::Bold);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置字体样式和对齐
    container->setFontStyle(FontStyle::Italic);
    container->setTextAlign(TextAlign::Center);
    container->setFontFamily("Times New Roman");
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置行高和缩进
    container->setText("这是一段多行文本。\n它演示了行高和文本缩进的效果。\n第三行文本用于展示对齐方式。");
    container->setLineHeight(1.5f);
    container->setTextIndent(20.0f);
    container->setTextAlign(TextAlign::Left);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
    
    // 设置文本装饰
    container->setText("带装饰的文本");
    container->setTextDecoration(TextDecoration::Underline);
    container->setTextOverflow(TextOverflow::Ellipsis);
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
}

void demonstrateCombinedFeatures() {
    std::cout << "=== 综合功能演示 ===" << std::endl;
    
    auto container = std::make_shared<SContainer>();
    container->setWidth(400);
    container->setHeight(250);
    container->setPadding(EdgeInsets::All(20));
    
    // 背景
    container->setBackgroundColor(Color::fromRGB(240, 240, 240));
    
    // 边框
    container->setBorderColor(Color::fromRGB(100, 100, 100));
    container->setBorderStyle(BorderStyle::Solid);
    container->setBorderRadius(EdgeInsets::All(15.0f));
    
    // 阴影
    BoxShadow shadow;
    shadow.color = Color::fromRGB(0, 0, 0, 0.3f);
    shadow.offsetX = 3.0f;
    shadow.offsetY = 3.0f;
    shadow.blurRadius = 8.0f;
    container->setBoxShadow(shadow);
    
    // 文本
    container->setText("这是一个综合样式演示\n展示了背景、边框、阴影和文本样式的组合效果");
    container->setColor(Color::fromRGB(50, 50, 50));
    container->setFontSize(16.0f);
    container->setFontWeight(FontWeight::Medium);
    container->setTextAlign(TextAlign::Center);
    container->setLineHeight(1.6f);
    
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n";
}

void demonstrateStyleManagement() {
    std::cout << "=== 样式管理演示 ===" << std::endl;
    
    auto container = std::make_shared<SContainer>();
    container->setWidth(300);
    container->setHeight(100);
    
    // 设置多种样式
    container->setBackgroundColor(Color::fromRGB(255, 255, 200));
    container->setBorderColor(Color::fromRGB(200, 100, 100));
    container->setText("样式管理测试");
    container->setColor(Color::fromRGB(100, 100, 200));
    
    std::cout << "设置样式后:" << std::endl;
    std::cout << "  有背景: " << (container->hasBackground() ? "是" : "否") << std::endl;
    std::cout << "  有边框: " << (container->hasBorderStyle() ? "是" : "否") << std::endl;
    std::cout << "  有文本: " << (container->hasTextStyle() ? "是" : "否") << std::endl;
    
    container->calculateLayout();
    container->render(0);
    
    std::cout << "\n清除背景后:" << std::endl;
    container->clearBackground();
    std::cout << "  有背景: " << (container->hasBackground() ? "是" : "否") << std::endl;
    
    container->render(0);
    
    std::cout << "\n重置所有样式后:" << std::endl;
    container->resetStyles();
    std::cout << "  有背景: " << (container->hasBackground() ? "是" : "否") << std::endl;
    std::cout << "  有边框: " << (container->hasBorderStyle() ? "是" : "否") << std::endl;
    std::cout << "  有文本: " << (container->hasTextStyle() ? "是" : "否") << std::endl;
    
    container->render(0);
    
    std::cout << "\n";
}

void demonstrateColorUtilities() {
    std::cout << "=== 颜色工具演示 ===" << std::endl;
    
    // 从十六进制创建颜色
    Color color1 = Color::fromHex(0xFF808080); // 灰色
    std::cout << "从十六进制 0xFF808080 创建的颜色: RGBA(" 
              << color1.r << ", " << color1.g << ", " << color1.b << ", " << color1.a << ")" << std::endl;
    
    // 从RGB创建颜色
    Color color2 = Color::fromRGB(255, 128, 0, 200); // 橙色，透明度200
    std::cout << "从RGB(255, 128, 0, 200) 创建的颜色: RGBA(" 
              << color2.r << ", " << color2.g << ", " << color2.b << ", " << color2.a << ")" << std::endl;
    
    // 转换为十六进制
    uint32_t hex = color2.toHex();
    std::cout << "颜色转换为十六进制: 0x" << std::hex << hex << std::dec << std::endl;
    
    std::cout << "\n";
}

int main() {
    std::cout << "ContainerEx 功能演示程序" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "\n";
    
    try {
        demonstrateColorUtilities();
        demonstrateBackgroundFeatures();
        demonstrateBorderFeatures();
        demonstrateTextFeatures();
        demonstrateCombinedFeatures();
        demonstrateStyleManagement();
        
        std::cout << "所有功能演示完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
