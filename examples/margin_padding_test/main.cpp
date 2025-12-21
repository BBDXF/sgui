/**
 * Margin 和 Padding 测试程序
 * 专门测试容器边距和内边距的布局效果
 * 新版本：root下挂载三个子元素，分别测试不同的margin/padding组合
 */

#include "sgui_window.h"
#include "sgui_container.h"
#include "sgui_cairo_renderer.h"
#include <cairo/cairo.h>
#include <iostream>
#include <memory>

using namespace sgui;

int main() {
    std::cout << "SGUI Margin & Padding 测试程序" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "测试容器的边距和内边距布局效果" << std::endl;
    std::cout << std::endl;
    
    try {
        // 创建窗口管理器
        SWindowManager windowManager;
        
        // 创建主窗口
        auto window = windowManager.CreateWindow(900, 700, "Margin & Padding Test");
        if (!window) {
            std::cerr << "创建窗口失败" << std::endl;
            return -1;
        }
        
        // 创建根容器 - 使用浅灰色背景便于观察
        auto rootContainer = std::make_shared<SContainer>();
        rootContainer->setFlexDirection(FlexDirection::Column);
        rootContainer->setBackgroundColor(Color::fromRGB(240, 240, 240)); // 浅灰色
        rootContainer->setDisplay(Display::Flex);
        rootContainer->setPadding(EdgeInsets::All(10.0)); // 根容器有少量内边距
        
        // 创建第一个子元素 - flex布局，margin padding为0
        auto firstChild = std::make_shared<SContainer>();
        firstChild->setFlexDirection(FlexDirection::Row);
        firstChild->setDisplay(Display::Flex);
        firstChild->setMargin(EdgeInsets::All(0.0)); // margin为0
        firstChild->setPadding(EdgeInsets::All(0.0)); // padding为0
        firstChild->setBackgroundColor(Color::fromRGB(200, 200, 200)); // 深灰色背景，便于观察
        firstChild->setHeight(200);
        firstChild->setBorder(EdgeInsets::All(1.0)); // 边框便于观察
        firstChild->setBorderColor(Color::fromRGB(100, 100, 100));
        
        // 第一个子元素的内部三个子容器
        // 第一个：设置padding
        auto firstGrandChild1 = std::make_shared<SContainer>();
        firstGrandChild1->setText("有Padding\n无Margin\nPadding: 15px");
        firstGrandChild1->setBackgroundColor(Color::fromRGB(231, 76, 60)); // 红色
        firstGrandChild1->setFlex(1.0);
        firstGrandChild1->setPadding(EdgeInsets::All(15.0)); // 设置padding
        firstGrandChild1->setBorder(EdgeInsets::All(2.0));
        firstGrandChild1->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第二个：不设置margin和padding
        auto firstGrandChild2 = std::make_shared<SContainer>();
        firstGrandChild2->setText("无Padding\n无Margin");
        firstGrandChild2->setBackgroundColor(Color::fromRGB(46, 204, 113)); // 绿色
        firstGrandChild2->setFlex(1.0);
        firstGrandChild2->setBorder(EdgeInsets::All(2.0));
        firstGrandChild2->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第三个：设置margin
        auto firstGrandChild3 = std::make_shared<SContainer>();
        firstGrandChild3->setText("无Padding\n有Margin\nMargin: 10px");
        firstGrandChild3->setBackgroundColor(Color::fromRGB(155, 89, 182)); // 紫色
        firstGrandChild3->setFlex(1.0);
        firstGrandChild3->setMargin(EdgeInsets::All(10.0)); // 设置margin
        firstGrandChild3->setBorder(EdgeInsets::All(2.0));
        firstGrandChild3->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 创建第二个子元素 - flex布局，设置padding
        auto secondChild = std::make_shared<SContainer>();
        secondChild->setFlexDirection(FlexDirection::Row);
        secondChild->setDisplay(Display::Flex);
        secondChild->setPadding(EdgeInsets::All(20.0)); // 设置padding
        secondChild->setBackgroundColor(Color::fromRGB(220, 220, 180)); // 浅黄色背景
        secondChild->setHeight(200);
        secondChild->setBorder(EdgeInsets::All(1.0));
        secondChild->setBorderColor(Color::fromRGB(100, 100, 100));
        
        // 第二个子元素的内部三个子容器（与第一个子元素内部相同）
        // 第一个：设置padding
        auto secondGrandChild1 = std::make_shared<SContainer>();
        secondGrandChild1->setText("有Padding\n无Margin\nPadding: 15px");
        secondGrandChild1->setBackgroundColor(Color::fromRGB(231, 76, 60)); // 红色
        secondGrandChild1->setFlex(1.0);
        secondGrandChild1->setPadding(EdgeInsets::All(15.0)); // 设置padding
        secondGrandChild1->setBorder(EdgeInsets::All(2.0));
        secondGrandChild1->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第二个：不设置margin和padding
        auto secondGrandChild2 = std::make_shared<SContainer>();
        secondGrandChild2->setText("无Padding\n无Margin");
        secondGrandChild2->setBackgroundColor(Color::fromRGB(46, 204, 113)); // 绿色
        secondGrandChild2->setFlex(1.0);
        secondGrandChild2->setBorder(EdgeInsets::All(2.0));
        secondGrandChild2->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第三个：设置margin
        auto secondGrandChild3 = std::make_shared<SContainer>();
        secondGrandChild3->setText("无Padding\n有Margin\nMargin: 10px");
        secondGrandChild3->setBackgroundColor(Color::fromRGB(155, 89, 182)); // 紫色
        secondGrandChild3->setFlex(1.0);
        secondGrandChild3->setMargin(EdgeInsets::All(10.0)); // 设置margin
        secondGrandChild3->setBorder(EdgeInsets::All(2.0));
        secondGrandChild3->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 创建第三个子元素 - 类似第二个，但只设置margin
        auto thirdChild = std::make_shared<SContainer>();
        thirdChild->setFlexDirection(FlexDirection::Row);
        thirdChild->setDisplay(Display::Flex);
        thirdChild->setMargin(EdgeInsets::All(20.0)); // 只设置margin
        thirdChild->setBackgroundColor(Color::fromRGB(180, 220, 220)); // 浅蓝色背景
        thirdChild->setHeight(200);
        thirdChild->setBorder(EdgeInsets::All(1.0));
        thirdChild->setBorderColor(Color::fromRGB(100, 100, 100));
        
        // 第三个子元素的内部三个子容器（与前面相同）
        // 第一个：设置padding
        auto thirdGrandChild1 = std::make_shared<SContainer>();
        thirdGrandChild1->setText("有Padding\n无Margin\nPadding: 15px");
        thirdGrandChild1->setBackgroundColor(Color::fromRGB(231, 76, 60)); // 红色
        thirdGrandChild1->setFlex(1.0);
        thirdGrandChild1->setPadding(EdgeInsets::All(15.0)); // 设置padding
        thirdGrandChild1->setBorder(EdgeInsets::All(2.0));
        thirdGrandChild1->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第二个：不设置margin和padding
        auto thirdGrandChild2 = std::make_shared<SContainer>();
        thirdGrandChild2->setText("无Padding\n无Margin");
        thirdGrandChild2->setBackgroundColor(Color::fromRGB(46, 204, 113)); // 绿色
        thirdGrandChild2->setFlex(1.0);
        thirdGrandChild2->setBorder(EdgeInsets::All(2.0));
        thirdGrandChild2->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 第三个：设置margin
        auto thirdGrandChild3 = std::make_shared<SContainer>();
        thirdGrandChild3->setText("无Padding\n有Margin\nMargin: 10px");
        thirdGrandChild3->setBackgroundColor(Color::fromRGB(155, 89, 182)); // 紫色
        thirdGrandChild3->setFlex(1.0);
        thirdGrandChild3->setMargin(EdgeInsets::All(10.0)); // 设置margin
        thirdGrandChild3->setBorder(EdgeInsets::All(2.0));
        thirdGrandChild3->setBorderColor(Color::fromRGB(0, 0, 0));
        
        // 构建容器树 - 第一层的子元素
        firstChild->addChild(firstGrandChild1);
        firstChild->addChild(firstGrandChild2);
        firstChild->addChild(firstGrandChild3);
        
        secondChild->addChild(secondGrandChild1);
        secondChild->addChild(secondGrandChild2);
        secondChild->addChild(secondGrandChild3);
        
        thirdChild->addChild(thirdGrandChild1);
        thirdChild->addChild(thirdGrandChild2);
        thirdChild->addChild(thirdGrandChild3);
        
        // 添加到根容器
        rootContainer->addChild(firstChild);
        rootContainer->addChild(secondChild);
        rootContainer->addChild(thirdChild);
        
        // 设置根容器到窗口
        window->SetRootContainer(rootContainer);
        
        std::cout << "窗口创建成功！" << std::endl;
        std::cout << std::endl;
        std::cout << "测试说明：" << std::endl;
        std::cout << "1. 根容器有10px内边距（浅灰色背景）" << std::endl;
        std::cout << "2. 第一个子容器：margin=0, padding=0（深灰色背景）" << std::endl;
        std::cout << "   - 红色子容器：只有padding=15px" << std::endl;
        std::cout << "   - 绿色子容器：无margin无padding" << std::endl;
        std::cout << "   - 紫色子容器：只有margin=10px" << std::endl;
        std::cout << "3. 第二个子容器：padding=20px（浅黄色背景）" << std::endl;
        std::cout << "   - 内部三个子容器与第一个子容器相同" << std::endl;
        std::cout << "4. 第三个子容器：margin=20px（浅蓝色背景）" << std::endl;
        std::cout << "   - 内部三个子容器与第一个子容器相同" << std::endl;
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
