/**
 * SGUI 按钮背景功能演示程序
 * 
 * 展示 SButton 支持的所有背景类型：
 * - 纯色背景（原有功能）
 * - 渐变背景（新功能）
 * - 图片背景（新功能）
 * - 混合背景类型（新功能）
 * - 便捷方法（新功能）
 */

#include <sgui_window.h>
#include <sgui_button.h>
#include <sgui_container.h>
#include <iostream>
#include <memory>

using namespace sgui;

int main() {
    try {
        std::cout << "=== SGUI 按钮背景功能演示 ===\n\n";
        
        // 创建窗口管理器
        SWindowManager manager;
        
        // 创建主窗口
        auto window = manager.CreateWindow(800, 600, "SGUI Button Background Demo");
        if (!window) {
            std::cerr << "Failed to create window" << std::endl;
            return -1;
        }
        
        // 创建根容器
        auto rootContainer = std::make_shared<SContainer>();
        rootContainer->setBackgroundColor(Color::White());
        
        // 创建标题容器
        auto titleContainer = std::make_shared<SContainer>("SGUI 按钮背景功能演示");
        titleContainer->setHeight(LayoutValue::Point(80));
        titleContainer->setBackgroundColor(Color::fromRGB(52, 152, 219)); // 蓝色背景
        titleContainer->setPadding(EdgeInsets::All(20.0f));
        titleContainer->setFontSize(21.0);
        
        // 创建主要内容容器
        auto contentContainer = std::make_shared<SContainer>();
        contentContainer->setWidth(LayoutValue::Point(800));
        contentContainer->setHeight(LayoutValue::Point(400));
        contentContainer->setPadding(EdgeInsets::All(30.0f));
        contentContainer->setFlexDirection(FlexDirection::Row);
        contentContainer->setAlignItems(Align::Center);
        
        // === 第一列：传统按钮和渐变按钮 ===
        auto column1 = std::make_shared<SContainer>();
        column1->setWidth(LayoutValue::Point(260));
        column1->setHeight(LayoutValue::Point(400));
        column1->setFlexDirection(FlexDirection::Column);
        column1->setGap(Gutter::Column, 15.0f);
        
        // 传统纯色按钮
        auto solidButton = std::make_shared<SButton>("传统纯色");
        solidButton->setWidth(LayoutValue::Point(240));
        solidButton->setHeight(LayoutValue::Point(45));
        solidButton->setNormalBackgroundColor(Color::LightGray());
        solidButton->setHoverBackgroundColor(Color::Gray());
        solidButton->setPressedBackgroundColor(Color::DarkGray());
        solidButton->setBorderRadius(EdgeInsets::All(6.0f));
        solidButton->setOnClick([](const MouseEvent& event) {
            std::cout << "✅ 传统纯色按钮被点击！\n";
        });
        
        // 渐变背景按钮
        auto gradientButton = std::make_shared<SButton>("渐变背景");
        gradientButton->setWidth(LayoutValue::Point(240));
        gradientButton->setHeight(LayoutValue::Point(45));
        gradientButton->setNormalBackgroundGradient(BackgroundGradient::linear(Color::Blue(), Color::Purple(), 45.0f));
        gradientButton->setHoverBackgroundGradient(BackgroundGradient::sunset());
        gradientButton->setPressedBackgroundGradient(BackgroundGradient::fire());
        gradientButton->setBorderRadius(EdgeInsets::All(6.0f));
        gradientButton->setOnClick([](const MouseEvent& event) {
            std::cout << "🌈 渐变背景按钮被点击！\n";
        });
        
        // 便捷方法演示按钮
        auto convenienceButton = std::make_shared<SButton>("便捷渐变");
        convenienceButton->setWidth(LayoutValue::Point(240));
        convenienceButton->setHeight(LayoutValue::Point(45));
        convenienceButton->setNormalBackgroundGradient(BackgroundGradient::rainbow(90.0f));
        convenienceButton->setBorderRadius(EdgeInsets::All(6.0f));
        convenienceButton->setOnClick([](const MouseEvent& event) {
            std::cout << "🎨 便捷方法按钮被点击！\n";
        });
        
        column1->addChild(solidButton);
        column1->addChild(gradientButton);
        column1->addChild(convenienceButton);
        
        // === 第二列：图片背景和混合类型 ===
        auto column2 = std::make_shared<SContainer>();
        column2->setWidth(LayoutValue::Point(260));
        column2->setHeight(LayoutValue::Point(400));
        column2->setFlexDirection(FlexDirection::Column);
        column2->setGap(Gutter::Column, 15.0f);
        
        // 图片背景按钮
        auto imageButton = std::make_shared<SButton>("图片背景");
        imageButton->setWidth(LayoutValue::Point(240));
        imageButton->setHeight(LayoutValue::Point(45));
        imageButton->setNormalBackgroundImage("button_normal.png");
        imageButton->setHoverBackgroundImage("button_hover.png");
        imageButton->setPressedBackgroundImage("button_pressed.png");
        imageButton->setBorderRadius(EdgeInsets::All(6.0f));
        imageButton->setOnClick([](const MouseEvent& event) {
            std::cout << "🖼️ 图片背景按钮被点击！\n";
        });
        
        // 混合背景类型按钮
        auto mixedButton = std::make_shared<SButton>("混合类型");
        mixedButton->setWidth(LayoutValue::Point(240));
        mixedButton->setHeight(LayoutValue::Point(45));
        mixedButton->setNormalBackgroundColor(Color::LightGray());        // 纯色
        mixedButton->setHoverBackgroundGradient(BackgroundGradient::ocean());   // 渐变
        mixedButton->setPressedBackgroundImage("button_pressed.png");        // 图片
        mixedButton->setBorderRadius(EdgeInsets::All(6.0f));
        mixedButton->setOnClick([](const MouseEvent& event) {
            std::cout << "🎭 混合类型按钮被点击！\n";
        });
        
        // 各种预设渐变按钮
        auto presetButton1 = std::make_shared<SButton>("天空渐变");
        presetButton1->setWidth(LayoutValue::Point(240));
        presetButton1->setHeight(LayoutValue::Point(45));
        presetButton1->setNormalBackgroundGradient(BackgroundGradient::skyBlue());
        presetButton1->setBorderRadius(EdgeInsets::All(6.0f));
        presetButton1->setOnClick([](const MouseEvent& event) {
            std::cout << "☁️ 天空渐变按钮被点击！\n";
        });
        
        column2->addChild(imageButton);
        column2->addChild(mixedButton);
        column2->addChild(presetButton1);
        
        // === 第三列：控制按钮 ===
        auto column3 = std::make_shared<SContainer>();
        column3->setWidth(LayoutValue::Point(260));
        column3->setHeight(LayoutValue::Point(400));
        column3->setFlexDirection(FlexDirection::Column);
        column3->setGap(Gutter::Column, 15.0f);
        
        // 控制按钮
        auto controlButton = std::make_shared<SButton>("控制面板");
        controlButton->setWidth(LayoutValue::Point(240));
        controlButton->setHeight(LayoutValue::Point(45));
        controlButton->setNormalBackgroundColor(Color::Orange());
        controlButton->setHoverBackgroundColor(Color::Red());
        controlButton->setPressedBackgroundColor(Color::fromRGB(139, 0, 0)); // Dark Red
        controlButton->setBorderRadius(EdgeInsets::All(6.0f));
        controlButton->setOnClick([&](const MouseEvent& event) {
            static bool disabled = false;
            disabled = !disabled;
            
            // 切换其他按钮的禁用状态
            gradientButton->setDisabled(disabled);
            convenienceButton->setDisabled(disabled);
            imageButton->setDisabled(disabled);
            mixedButton->setDisabled(disabled);
            presetButton1->setDisabled(disabled);
            
            if (disabled) {
                controlButton->setButtonText("启用所有");
                controlButton->setNormalBackgroundColor(Color::Green());
                std::cout << "🔒 其他按钮已禁用\n";
            } else {
                controlButton->setButtonText("禁用所有");
                controlButton->setNormalBackgroundColor(Color::Orange());
                std::cout << "🔓 其他按钮已启用\n";
            }
        });
        
        // 重置按钮
        auto resetButton = std::make_shared<SButton>("重置渐变");
        resetButton->setWidth(LayoutValue::Point(240));
        resetButton->setHeight(LayoutValue::Point(45));
        resetButton->setNormalBackgroundColor(Color::Purple());
        resetButton->setHoverBackgroundColor(Color::Pink());
        resetButton->setPressedBackgroundColor(Color::fromRGB(75, 0, 130)); // Dark Purple
        resetButton->setBorderRadius(EdgeInsets::All(6.0f));
        resetButton->setOnClick([&](const MouseEvent& event) {
            // 随机生成新的渐变背景
            static int gradientIndex = 0;
            BackgroundGradient gradients[] = {
                BackgroundGradient::rainbow(),
                BackgroundGradient::sunset(),
                BackgroundGradient::ocean(),
                BackgroundGradient::forest(),
                BackgroundGradient::fire()
            };
            
            gradientIndex = (gradientIndex + 1) % 6;
            convenienceButton->setNormalBackgroundGradient(gradients[gradientIndex]);
            
            const char* gradientNames[] = {"彩虹", "日落", "海洋", "森林", "火焰", "自定义"};
            std::cout << "🎲 渐变已切换为: " << gradientNames[gradientIndex] << "\n";
        });
        
        column3->addChild(controlButton);
        column3->addChild(resetButton);
        
        // 组装界面
        contentContainer->addChild(column1);
        contentContainer->addChild(column2);
        contentContainer->addChild(column3);
        
        rootContainer->addChild(titleContainer);
        rootContainer->addChild(contentContainer);
        
        // 设置根容器
        window->SetRootContainer(rootContainer);
        
        // 显示功能说明
        std::cout << "\n=== 功能说明 ===\n";
        std::cout << "🎯 纯色背景：传统的按钮背景色（原有功能）\n";
        std::cout << "🌈 渐变背景：支持线性/径向渐变，多种预设样式（新功能）\n";
        std::cout << "🖼️ 图片背景：支持PNG图片作为背景（新功能）\n";
        std::cout << "🎭 混合类型：每个按钮状态可使用不同背景类型（新功能）\n";
        std::cout << "🎨 便捷方法：快速设置统一背景（新功能）\n";
        std::cout << "🔄 优先级系统：渐变 > 图片 > 纯色\n";
        std::cout << "✅ 向后兼容：原有代码无需修改\n\n";
        std::cout << "💡 提示：点击不同按钮体验各种背景效果！\n\n";
        
        // 运行主循环
        manager.Run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
