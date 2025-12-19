#include <GLFW/glfw3.h>
#include <iostream>

// 简单的窗口管理类
class SWindowManager {
public:
    SWindowManager(int width, int height, const char* title)
        : width_(width), height_(height), title_(title), window_(nullptr) {}

    ~SWindowManager() {
        if (window_) {
            glfwDestroyWindow(window_);
        }
        glfwTerminate();
    }

    bool Initialize() {
        // 初始化GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // 创建窗口
        window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
        if (!window_) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        // 设置当前上下文
        glfwMakeContextCurrent(window_);

        // 设置窗口回调
        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowSizeCallback(window_, WindowSizeCallback);
        glfwSetWindowCloseCallback(window_, WindowCloseCallback);

        return true;
    }

    void Run() {
        if (!window_) return;

        // 主循环
        while (!glfwWindowShouldClose(window_)) {
            // 清除屏幕
            glClear(GL_COLOR_BUFFER_BIT);

            // 交换缓冲区
            glfwSwapBuffers(window_);

            // 污染事件
            glfwPollEvents();
        }
    }

    GLFWwindow* GetWindow() const { return window_; }

private:
    int width_;
    int height_;
    const char* title_;
    GLFWwindow* window_;

    // 窗口大小回调
    static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
        auto manager = static_cast<SWindowManager*>(glfwGetWindowUserPointer(window));
        if (manager) {
            manager->width_ = width;
            manager->height_ = height;
            std::cout << "Window resized to: " << width << "x" << height << std::endl;
        }
    }

    // 窗口关闭回调
    static void WindowCloseCallback(GLFWwindow* window) {
        std::cout << "Window close requested" << std::endl;
    }
};

int main() {
    // 创建窗口管理器
    SWindowManager manager(800, 600, "SGUI - GLFW Window Demo");

    // 初始化
    if (!manager.Initialize()) {
        return -1;
    }

    std::cout << "Window created successfully. Press ESC to exit." << std::endl;

    // 运行主循环
    manager.Run();

    return 0;
}
