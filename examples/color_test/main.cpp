#include "sgui_common.h"
#include <iostream>

int main() {
    std::cout << "测试 SGUI Color 结构体的常见颜色功能" << std::endl;
    
    // 测试基本颜色
    sgui::Color red = sgui::Color::Red();
    sgui::Color green = sgui::Color::Green();
    sgui::Color blue = sgui::Color::Blue();
    
    std::cout << "红色: R=" << red.r << " G=" << red.g << " B=" << red.b << " A=" << red.a << std::endl;
    std::cout << "绿色: R=" << green.r << " G=" << green.g << " B=" << green.b << " A=" << green.a << std::endl;
    std::cout << "蓝色: R=" << blue.r << " G=" << blue.g << " B=" << blue.b << " A=" << blue.a << std::endl;
    
    // 测试其他常见颜色
    sgui::Color white = sgui::Color::White();
    sgui::Color black = sgui::Color::Black();
    sgui::Color yellow = sgui::Color::Yellow();
    sgui::Color cyan = sgui::Color::Cyan();
    sgui::Color magenta = sgui::Color::Magenta();
    sgui::Color gray = sgui::Color::Gray();
    sgui::Color orange = sgui::Color::Orange();
    sgui::Color purple = sgui::Color::Purple();
    
    std::cout << "\n其他常见颜色:" << std::endl;
    std::cout << "白色: " << white.toHex() << std::endl;
    std::cout << "黑色: " << black.toHex() << std::endl;
    std::cout << "黄色: " << yellow.toHex() << std::endl;
    std::cout << "青色: " << cyan.toHex() << std::endl;
    std::cout << "品红: " << magenta.toHex() << std::endl;
    std::cout << "灰色: " << gray.toHex() << std::endl;
    std::cout << "橙色: " << orange.toHex() << std::endl;
    std::cout << "紫色: " << purple.toHex() << std::endl;
    
    // 测试RGB转换
    sgui::Color rgbColor = sgui::Color::fromRGB(255, 128, 64);
    std::cout << "\nRGB(255, 128, 64): R=" << rgbColor.r << " G=" << rgbColor.g << " B=" << rgbColor.b << std::endl;
    
    // 测试十六进制转换
    sgui::Color hexColor = sgui::Color::fromHex(0xFF8040);
    std::cout << "十六进制 0xFF8040: R=" << hexColor.r << " G=" << hexColor.g << " B=" << hexColor.b << std::endl;
    
    // 测试透明度
    sgui::Color transparent = sgui::Color::Transparent();
    std::cout << "\n透明色: A=" << transparent.a << std::endl;
    
    std::cout << "\n所有颜色功能测试完成！" << std::endl;
    
    return 0;
}
