/**
 * Cairo Demo - 演示Cairo渲染器
 * 直接绑定window id进行surface绘制
 */

#include "sgui_window.h"
#include "sgui_container.h"
#include "sgui_cairo_renderer.h"
#include <cairo/cairo.h>
#include <iostream>
#include <memory>

using namespace sgui;



int main() {
    std::cout << "SGUI Cairo渲染演示程序" << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "演示直接绑定window id的surface绘制" << std::endl;
    std::cout << std::endl;
    
    try {
        // 创建窗口管理器
        SWindowManager windowManager;
        
        // 创建主窗口
        auto window = windowManager.CreateWindow(800, 600, "SGUI Cairo Demo");
        if (!window) {
            std::cerr << "创建窗口失败" << std::endl;
            return -1;
        }
        
        // 创建根容器
        auto rootContainer = std::make_shared<SContainer>();
        rootContainer->setFlexDirection(FlexDirection::Column);
        // rootContainer->setBackgroundColor(Color::fromHex(0xFFFFFF));
        rootContainer->setDisplay(Display::Flex);
        
        // 创建标题容器
        auto titleContainer = std::make_shared<SContainer>();
        titleContainer->setText("Cairo渲染演示\n这是一个多行文本\n😃啊啊");
        titleContainer->setBackgroundColor(Color::fromRGB(52, 152, 219)); // 蓝色
        titleContainer->setHeight(80);
        titleContainer->setMargin(EdgeInsets::All(10.0));
        titleContainer->setPadding(EdgeInsets::All(8.0));
        titleContainer->setBorder(EdgeInsets::Only(4.0, 8.0, 0.0, 0.0));
        titleContainer->setBorderRadius(EdgeInsets::Radius(0, 40, 40, 16));
        
        // 创建内容容器（水平布局）
        auto contentContainer = std::make_shared<SContainer>();
        contentContainer->setFlex(1.0);
        contentContainer->setFlexDirection(FlexDirection::Row);
        contentContainer->setFlexWrap(FlexWrap::Wrap);
        contentContainer->setText("Body");
        contentContainer->setDisplay(Display::Flex);
        contentContainer->setGap(Gutter::All, LayoutValue::Point(10));
        
        // 创建三个彩色方块
        auto redBox = std::make_shared<SContainer>();
        redBox->setText("红色 - body");
        redBox->setBackgroundColor(Color::fromRGB(231, 76, 60)); // 红色
        redBox->setWidth(LayoutValue::Point(240));
        redBox->setHeight(LayoutValue::Point(200));
        
        auto greenBox = std::make_shared<SContainer>();
        greenBox->setText("绿色 - body");
        greenBox->setBackgroundColor(Color::fromRGB(46, 204, 113)); // 绿色
        greenBox->setWidth(LayoutValue::Point(240));
        greenBox->setHeight(LayoutValue::Point(200));
        
        auto blueBox = std::make_shared<SContainer>();
        blueBox->setText("蓝色 - body");
        blueBox->setBackgroundColor(Color::fromRGB(155, 89, 182)); // 紫色
        blueBox->setWidth(LayoutValue::Point(240));
        blueBox->setHeight(LayoutValue::Point(200));
        
        // 创建底部容器
        auto bottomContainer = std::make_shared<SContainer>();
        bottomContainer->setHeight(LayoutValue::Point(100));
        bottomContainer->setFlexDirection(FlexDirection::Row);
        bottomContainer->setBackgroundColor(Color(0xFF, 0x10, 0x1F));
        bottomContainer->setText("bottom");
        bottomContainer->setBorder(EdgeInsets::All(2.0));
        bottomContainer->setDisplay(Display::Flex);
        
        // 创建两个圆角矩形演示
        auto roundedBox1 = std::make_shared<SContainer>();
        roundedBox1->setText("黄色 - bot");
        roundedBox1->setBackgroundColor(Color::fromRGB(241, 196, 15)); // 黄色
        roundedBox1->setWidth(LayoutValue::Point(100));
        roundedBox1->setHeight(LayoutValue::Point(90));
        roundedBox1->setMargin(EdgeInsets::Only(0, 0, 20, 0));
        
        auto roundedBox2 = std::make_shared<SContainer>();
        roundedBox2->setText("橙色 - bot");
        roundedBox2->setBackgroundColor(Color::fromRGB(230, 126, 34)); // 橙色
        roundedBox2->setWidth(LayoutValue::Point(100));
        roundedBox2->setHeight(LayoutValue::Point(90));
        
        // 构建容器树
        contentContainer->addChild(redBox);
        contentContainer->addChild(greenBox);
        contentContainer->addChild(blueBox);
        
        bottomContainer->addChild(roundedBox1);
        bottomContainer->addChild(roundedBox2);
        
        rootContainer->addChild(titleContainer);
        rootContainer->addChild(contentContainer);
        rootContainer->addChild(bottomContainer);
        
        // 设置根容器到窗口
        window->SetRootContainer(rootContainer);
        
        std::cout << "窗口创建成功！" << std::endl;
        std::cout << std::endl;
        std::cout << "按ESC键或关闭窗口退出程序" << std::endl;
        
        // 运行主循环
        windowManager.Run();
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "程序正常退出" << std::endl;
    return 0;
}
