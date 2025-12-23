/**
 * SGUI Button 演示程序
 * 
 * 展示按钮控件的基本功能，包括：
 * - 鼠标悬停效果
 * - 点击事件处理
 * - 不同状态的样式变化
 */

#include <sgui_window.h>
#include <sgui_button.h>
#include <iostream>

using namespace sgui;

int main() {
    try {
        // 创建窗口管理器
        SWindowManager manager;
        
        // 创建窗口
        auto window = manager.CreateWindow(400, 300, "SGUI Button Demo");
        if (!window) {
            std::cerr << "Failed to create window" << std::endl;
            return -1;
        }
        
        // 创建根容器
        auto rootContainer = std::make_shared<SContainer>();
        rootContainer->setWidth(LayoutValue::Point(400));
        rootContainer->setHeight(LayoutValue::Point(300));
        rootContainer->setBackgroundColor(Color::White());
        
        // 创建一个按钮
        auto button = std::make_shared<SButton>("Click Me!");
        button->setPosition(EdgeInsets::Only(150, 120, 0, 0));
        button->setWidth(LayoutValue::Point(100));
        button->setHeight(LayoutValue::Point(40));
        
        // 设置按钮的点击回调
        static int clickCount = 0;
        button->setOnClick([&](const MouseEvent& event) {
            std::cout << "Button clicked!" << std::endl;
            clickCount++;
            
            // 更新按钮文本显示点击次数
            button->setButtonText("Clicked: " + std::to_string(clickCount));
        });
        
        // 自定义各状态的颜色
        button->setNormalBackgroundColor(Color::LightGray());
        button->setHoverBackgroundColor(Color::Gray());
        button->setPressedBackgroundColor(Color::DarkGray());
        button->setNormalBorderColor(Color::Gray());
        button->setHoverBorderColor(Color::DarkGray());
        button->setPressedBorderColor(Color::Black());
        
        // 创建第二个按钮，用于测试禁用功能
        auto toggleButton = std::make_shared<SButton>("Disable First Button");
        toggleButton->setPosition(EdgeInsets::Only(120, 180, 0, 0));
        toggleButton->setWidth(LayoutValue::Point(160));
        toggleButton->setHeight(LayoutValue::Point(35));
        toggleButton->setDisabled(true);
        toggleButton->setOnClick([&](const MouseEvent& event) {
            std::cout << "Disable Button is clicked! error!" << std::endl;
        });
        
        // 将按钮添加到根容器
        rootContainer->addChild(button);
        rootContainer->addChild(toggleButton);
        
        // 设置根容器
        window->SetRootContainer(rootContainer);
        
        // 运行主循环
        manager.Run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
