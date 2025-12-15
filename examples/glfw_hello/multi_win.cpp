#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

// 前向声明
class WindowManager;

// 单个窗口类
class Window {
public:
    Window(int width, int height, const char* title, WindowManager* manager)
        : width_(width), height_(height), title_(title), manager_(manager), window_(nullptr) {}

    friend class WindowManager; // 允许WindowManager访问私有成员

    ~Window() {
        if (window_) {
            glfwDestroyWindow(window_);
        }
    }

    bool Initialize() {
        // 创建窗口
        window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
        if (!window_) {
            std::cerr << "Failed to create GLFW window: " << title_ << std::endl;
            return false;
        }

        // 设置窗口回调
        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowSizeCallback(window_, WindowSizeCallback);
        glfwSetWindowCloseCallback(window_, WindowCloseCallback);

        std::cout << "Created window: " << title_ << " (" << width_ << "x" << height_ << ")" << std::endl;
        return true;
    }

    void Render() {
        if (!window_ || glfwWindowShouldClose(window_)) return;

        // 设置当前上下文
        glfwMakeContextCurrent(window_);

        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区
        glfwSwapBuffers(window_);
    }

    bool ShouldClose() const {
        return window_ ? glfwWindowShouldClose(window_) : true;
    }

    GLFWwindow* GetWindow() const { return window_; }
    const char* GetTitle() const { return title_; }

private:
    int width_;
    int height_;
    const char* title_;
    WindowManager* manager_;
    GLFWwindow* window_;

    // 窗口大小回调
    static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
        auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            win->width_ = width;
            win->height_ = height;
            std::cout << "Window resized: " << win->title_ << " -> " << width << "x" << height << std::endl;
        }
    }

    // 窗口关闭回调
    static void WindowCloseCallback(GLFWwindow* window) {
        auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
            std::cout << "Window close requested: " << win->title_ << std::endl;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
};

// 窗口管理器类
class WindowManager {
public:
    WindowManager() : glfw_initialized_(false) {}

    ~WindowManager() {
        // 清理所有窗口
        windows_.clear();
        if (glfw_initialized_) {
            glfwTerminate();
        }
    }

    // 创建新窗口
    std::shared_ptr<Window> CreateWindow(int width, int height, const char* title) {
        // 初始化GLFW（如果还没有初始化）
        if (!glfw_initialized_) {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                return nullptr;
            }
            glfw_initialized_ = true;
        }

        auto window = std::make_shared<Window>(width, height, title, this);
        if (window->Initialize()) {
            windows_.push_back(window);
            return window;
        }
        return nullptr;
    }

    // 移除关闭的窗口
    void RemoveClosedWindows() {
        auto it = windows_.begin();
        while (it != windows_.end()) {
            if ((*it)->ShouldClose()) {
                std::cout << "WindowManager: Removing closed window: " << (*it)->GetTitle() << std::endl;
                it = windows_.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 运行所有窗口
    void Run() {
        if (windows_.empty()) {
            std::cout << "No windows to run" << std::endl;
            return;
        }

        std::cout << "Created " << windows_.size() << " windows successfully." << std::endl;
        std::cout << "Each window can be closed independently. Program exits when all windows are closed." << std::endl;

        // 主循环
        while (!windows_.empty()) {
            // 渲染所有打开的窗口
            for (auto& window : windows_) {
                window->Render();
            }

            // 移除关闭的窗口
            RemoveClosedWindows();

            // 污染事件
            glfwPollEvents();
        }

        std::cout << "All windows closed. Exiting program." << std::endl;
    }

    // 获取窗口数量
    size_t GetWindowCount() const {
        return windows_.size();
    }

private:
    std::vector<std::shared_ptr<Window>> windows_;
    bool glfw_initialized_;
};


int main() {
    // 创建窗口管理器
    WindowManager manager;

    // 创建多个窗口
    manager.CreateWindow(800, 600, "SGUI - Main Window");
    manager.CreateWindow(600, 400, "SGUI - Secondary Window");
    manager.CreateWindow(400, 300, "SGUI - Tool Window");

    // 运行主循环
    manager.Run();

    return 0;
}
