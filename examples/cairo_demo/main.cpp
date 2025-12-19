/**
 * Cairo Demo - 演示Cairo渲染器
 * 直接绑定window id进行surface绘制
 */

#include "sgui_window.h"
#include "sgui_containerex.h"
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
        auto rootContainer = std::make_shared<SContainerEx>();
        rootContainer->setWidth(LayoutValue::Point(800));
        rootContainer->setHeight(LayoutValue::Point(600));
        rootContainer->setFlexDirection(FlexDirection::Column);
        rootContainer->setPadding(EdgeInsets::All(20));
        
        // 创建标题容器
        auto titleContainer = std::make_shared<SContainerEx>();
        titleContainer->setText("Cairo渲染演示");
        titleContainer->setBackgroundColor(Color::fromRGB(52, 152, 219)); // 蓝色
        titleContainer->setWidth(LayoutValue::Point(760));
        titleContainer->setHeight(LayoutValue::Point(80));
        titleContainer->setMargin(EdgeInsets::Only(0, 0, 0, 10));
        
        // 创建内容容器（水平布局）
        auto contentContainer = std::make_shared<SContainerEx>();
        contentContainer->setWidth(LayoutValue::Point(760));
        contentContainer->setHeight(LayoutValue::Point(200));
        contentContainer->setFlexDirection(FlexDirection::Row);
        contentContainer->setMargin(EdgeInsets::Only(0, 0, 0, 10));
        
        // 创建三个彩色方块
        auto redBox = std::make_shared<SContainerEx>();
        redBox->setText("红色");
        redBox->setBackgroundColor(Color::fromRGB(231, 76, 60)); // 红色
        redBox->setWidth(LayoutValue::Point(240));
        redBox->setHeight(LayoutValue::Point(200));
        redBox->setMargin(EdgeInsets::Only(0, 0, 10, 0));
        
        auto greenBox = std::make_shared<SContainerEx>();
        greenBox->setText("绿色");
        greenBox->setBackgroundColor(Color::fromRGB(46, 204, 113)); // 绿色
        greenBox->setWidth(LayoutValue::Point(240));
        greenBox->setHeight(LayoutValue::Point(200));
        greenBox->setMargin(EdgeInsets::Only(0, 0, 10, 0));
        
        auto blueBox = std::make_shared<SContainerEx>();
        blueBox->setText("蓝色");
        blueBox->setBackgroundColor(Color::fromRGB(155, 89, 182)); // 紫色
        blueBox->setWidth(LayoutValue::Point(240));
        blueBox->setHeight(LayoutValue::Point(200));
        
        // 创建底部容器
        auto bottomContainer = std::make_shared<SContainerEx>();
        bottomContainer->setWidth(LayoutValue::Point(760));
        bottomContainer->setHeight(LayoutValue::Point(240));
        bottomContainer->setFlexDirection(FlexDirection::Row);
        
        // 创建两个圆角矩形演示
        auto roundedBox1 = std::make_shared<SContainerEx>();
        roundedBox1->setText("黄色");
        roundedBox1->setBackgroundColor(Color::fromRGB(241, 196, 15)); // 黄色
        roundedBox1->setWidth(LayoutValue::Point(370));
        roundedBox1->setHeight(LayoutValue::Point(240));
        roundedBox1->setMargin(EdgeInsets::Only(0, 0, 20, 0));
        
        auto roundedBox2 = std::make_shared<SContainerEx>();
        roundedBox2->setText("橙色");
        roundedBox2->setBackgroundColor(Color::fromRGB(230, 126, 34)); // 橙色
        roundedBox2->setWidth(LayoutValue::Point(370));
        roundedBox2->setHeight(LayoutValue::Point(240));
        
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
