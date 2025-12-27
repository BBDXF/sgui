/**
 * Input组件演示程序
 * 
 * 展示SInput组件的各种功能：
 * - 不同类型的输入框
 * - 文本输入和编辑
 * - 光标和选择
 * - 状态变化
 * - 事件回调
 */

#include "sgui_window.h"
#include "sgui_input.h"
#include "sgui_container.h"
#include <iostream>
#include <memory>

using namespace sgui;

int main()
{
    // 创建窗口管理器
    SWindowManager windowManager;
    
    // 创建窗口
    auto window = windowManager.CreateWindow(800, 600, "Input Demo - SGUI");
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    
    // 创建主容器
    auto mainContainer = std::make_shared<SContainer>();
    mainContainer->setWidth(LayoutValue::Percent(100.0f));
    mainContainer->setHeight(LayoutValue::Percent(100.0f));
    mainContainer->setFlexDirection(FlexDirection::Column);
    mainContainer->setPadding(EdgeInsets::All(20.0f));
    mainContainer->setGap(Gutter::All, LayoutValue::Point(10.0f));
    
    // 创建标题容器
    auto titleContainer = std::make_shared<SContainer>();
    titleContainer->setWidth(LayoutValue::Percent(100.0f));
    titleContainer->setHeight(LayoutValue::Point(60.0f));
    titleContainer->setJustifyContent(Align::Center);
    titleContainer->setAlignItems(Align::Center);
    titleContainer->setBackgroundColor(Color(0.2, 0.6, 1.0, 1.0));
    titleContainer->setBorderRadius(EdgeInsets::All(8.0f));
    
    // 设置标题文本
    titleContainer->setText("Input Component Demo");
    titleContainer->setFontSize(24.0f);
    titleContainer->setFontWeight(FontWeight::Bold);
    titleContainer->setColor(Color::White());
    
    // 创建文本输入框
    auto textInput = std::make_shared<SInput>("Enter your name...");
    textInput->setWidth(LayoutValue::Percent(100.0f));
    textInput->setHeight(LayoutValue::Point(40.0f));
    textInput->setInputType(InputType::Text);
    textInput->setFocusedBackgroundColor(Color(1.0, 1.0, 0.9, 1.0));
    textInput->setFocusedBorderColor(Color(0.2, 0.6, 1.0, 1.0));
    
    // 设置文本变化回调
    textInput->setOnTextChanged([](const std::string& text) {
        std::cout << "Text changed: " << text << std::endl;
    });
    
    // 设置焦点变化回调
    textInput->setOnFocusChanged([](bool focused) {
        std::cout << "Focus changed: " << (focused ? "gained" : "lost") << std::endl;
    });
    
    // 创建密码输入框
    auto passwordInput = std::make_shared<SInput>("Enter password...");
    passwordInput->setWidth(LayoutValue::Percent(100.0f));
    passwordInput->setHeight(LayoutValue::Point(40.0f));
    passwordInput->setInputType(InputType::Password);
    passwordInput->setMaxLength(20);
    
    passwordInput->setOnTextChanged([](const std::string& text) {
        std::cout << "Password changed: " << std::string(text.length(), '*') << std::endl;
    });
    
    // 创建数字输入框
    auto numberInput = std::make_shared<SInput>("Enter number...");
    numberInput->setWidth(LayoutValue::Percent(100.0f));
    numberInput->setHeight(LayoutValue::Point(40.0f));
    numberInput->setInputType(InputType::Number);
    numberInput->setPlaceholder("0.0");
    
    numberInput->setOnTextChanged([](const std::string& text) {
        std::cout << "Number changed: " << text << std::endl;
    });
    
    // 创建邮箱输入框
    auto emailInput = std::make_shared<SInput>("Enter email...");
    emailInput->setWidth(LayoutValue::Percent(100.0f));
    emailInput->setHeight(LayoutValue::Point(40.0f));
    emailInput->setInputType(InputType::Email);
    emailInput->setPlaceholder("user@example.com");
    
    emailInput->setOnTextChanged([](const std::string& text) {
        std::cout << "Email changed: " << text << std::endl;
    });
    
    // 创建只读输入框
    auto readonlyInput = std::make_shared<SInput>("Readonly input");
    readonlyInput->setWidth(LayoutValue::Percent(100.0f));
    readonlyInput->setHeight(LayoutValue::Point(40.0f));
    readonlyInput->setReadOnly(true);
    readonlyInput->setValue("This is a readonly input field");
    readonlyInput->setBackgroundColor(Color(0.95, 0.95, 0.95, 1.0));
    
    // 创建禁用输入框
    auto disabledInput = std::make_shared<SInput>("Disabled input");
    disabledInput->setWidth(LayoutValue::Percent(100.0f));
    disabledInput->setHeight(LayoutValue::Point(40.0f));
    disabledInput->setDisabled(true);
    disabledInput->setValue("This input is disabled");
    
    // 创建搜索输入框（带回车回调）
    auto searchInput = std::make_shared<SInput>("Search...");
    searchInput->setWidth(LayoutValue::Percent(100.0f));
    searchInput->setHeight(LayoutValue::Point(40.0f));
    searchInput->setInputType(InputType::Search);
    searchInput->setPlaceholder("Type and press Enter to search");
    
    searchInput->setOnEnterPressed([](const std::string& text) {
        std::cout << "Searching for: " << text << std::endl;
    });
    
    // 创建自定义样式的输入框
    auto customInput = std::make_shared<SInput>("Custom styled input");
    customInput->setWidth(LayoutValue::Percent(100.0f));
    customInput->setHeight(LayoutValue::Point(50.0f));
    customInput->setFontSize(16.0f);
    customInput->setFontWeight(FontWeight::Bold);
    customInput->setBorderRadius(EdgeInsets::All(25.0f));
    customInput->setBackgroundColor(Color(0.9, 0.95, 1.0, 1.0));
    customInput->setBorderColor(Color(0.3, 0.5, 0.8, 1.0));
    customInput->setCursorColor(Color(0.3, 0.5, 0.8, 1.0));
    customInput->setSelectionColor(Color(0.3, 0.5, 0.8, 0.3));
    
    // 将所有控件添加到主容器
    mainContainer->addChild(titleContainer);
    mainContainer->addChild(textInput);
    mainContainer->addChild(passwordInput);
    mainContainer->addChild(numberInput);
    mainContainer->addChild(emailInput);
    mainContainer->addChild(readonlyInput);
    mainContainer->addChild(disabledInput);
    mainContainer->addChild(searchInput);
    mainContainer->addChild(customInput);
    
    // 设置窗口的根容器
    window->SetRootContainer(mainContainer);
    
    // 计算布局
    mainContainer->calculateLayout(800, 600);
    
    std::cout << "=== SGUI Input Component Demo ===" << std::endl;
    std::cout << "This demo showcases various SInput component features:" << std::endl;
    std::cout << "- Text input with placeholder and events" << std::endl;
    std::cout << "- Password input with character masking" << std::endl;
    std::cout << "- Number input with validation" << std::endl;
    std::cout << "- Email input with basic validation" << std::endl;
    std::cout << "- Readonly input field" << std::endl;
    std::cout << "- Disabled input field" << std::endl;
    std::cout << "- Search input with Enter key callback" << std::endl;
    std::cout << "- Custom styled input" << std::endl;
    std::cout << "\nFeatures to try:" << std::endl;
    std::cout << "- Click on inputs to focus them" << std::endl;
    std::cout << "- Type text and see the callbacks" << std::endl;
    std::cout << "- Use arrow keys to move cursor" << std::endl;
    std::cout << "- Use Ctrl+A to select all text" << std::endl;
    std::cout << "- Use Ctrl+C/V/X for copy/paste/cut" << std::endl;
    std::cout << "- Use Ctrl+Z/Y for undo/redo" << std::endl;
    std::cout << "- Press Enter in search field" << std::endl;
    std::cout << "- Press Escape to unfocus" << std::endl;
    std::cout << "\nClose window to exit." << std::endl;
    
    // 运行主循环
    windowManager.Run();
    
    return 0;
}
