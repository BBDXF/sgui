#include "sgui_window.h"
#include <iostream>

/**
 * @brief 窗口管理库Demo程序
 *
 * 该程序演示如何使用SGUI窗口管理库创建和管理多个窗口。
 * 它展示了基本的窗口创建、渲染和生命周期管理功能。
 */
int main() {
    std::cout << "SGUI Window Management Library Demo" << std::endl;
    std::cout << "==================================" << std::endl;

    // 创建窗口管理器实例
    WindowManager manager;

    std::cout << "Creating multiple windows..." << std::endl;

    // 创建多个不同大小和标题的窗口
    auto mainWindow = manager.CreateWindow(800, 600, "SGUI - Main Window");
    auto secondaryWindow = manager.CreateWindow(600, 400, "SGUI - Secondary Window");
    auto toolWindow = manager.CreateWindow(400, 300, "SGUI - Tool Window");

    // 检查窗口是否成功创建
    if (!mainWindow || !secondaryWindow || !toolWindow) {
        std::cerr << "Failed to create all windows!" << std::endl;
        return 1;
    }

    std::cout << "Successfully created " << manager.GetWindowCount() << " windows." << std::endl;
    std::cout << "Window titles:" << std::endl;
    std::cout << "  - " << mainWindow->GetTitle() << std::endl;
    std::cout << "  - " << secondaryWindow->GetTitle() << std::endl;
    std::cout << "  - " << toolWindow->GetTitle() << std::endl;

    std::cout << std::endl;
    std::cout << "Instructions:" << std::endl;
    std::cout << "  - Each window can be moved, resized, and closed independently" << std::endl;
    std::cout << "  - Try resizing windows to see the resize callback in action" << std::endl;
    std::cout << "  - Close windows one by one to see the window management" << std::endl;
    std::cout << "  - The program will exit when all windows are closed" << std::endl;
    std::cout << std::endl;

    // 运行主循环 - 这将阻塞直到所有窗口被关闭
    manager.Run();

    std::cout << "Demo completed successfully!" << std::endl;
    return 0;
}
