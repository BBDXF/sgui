#include "sgui_window.h"
#include <iostream>

int main() {
    // 创建窗口管理器
    WindowManager manager;

    // 创建一个窗口
    auto window = manager.CreateWindow(800, 600, "SGUI - Input Test Window");
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    std::cout << "Window created successfully. Try moving the mouse, clicking, scrolling, and pressing keys." << std::endl;
    std::cout << "Press ESC to exit." << std::endl;

    // 运行主循环
    manager.Run();

    return 0;
}
