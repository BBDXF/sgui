#include "sgui_window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

// Window类的实现

Window::Window(int width, int height, const char* title, WindowManager* manager)
    : width_(width), height_(height), title_(title), manager_(manager), window_(nullptr) {
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

bool Window::Initialize() {
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

    // 设置鼠标和键盘回调
    glfwSetCursorPosCallback(window_, MousePosCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);
    glfwSetScrollCallback(window_, ScrollCallback);
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetCharCallback(window_, CharCallback);

    std::cout << "Created window: " << title_ << " (" << width_ << "x" << height_ << ")" << std::endl;
    return true;
}

void Window::Render() {
    if (!window_ || glfwWindowShouldClose(window_)) return;

    // 设置当前上下文
    glfwMakeContextCurrent(window_);

    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    // 交换缓冲区
    glfwSwapBuffers(window_);
}

bool Window::ShouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void* Window::GetWindow() const {
    return window_;
}

const char* Window::GetTitle() const {
    return title_;
}

// 窗口大小回调
void Window::WindowSizeCallback(GLFWwindow* window, int width, int height) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width_ = width;
        win->height_ = height;
        std::cout << "Window resized: " << win->title_ << " -> " << width << "x" << height << std::endl;
    }
}

// 窗口关闭回调
void Window::WindowCloseCallback(GLFWwindow* window) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Window close requested: " << win->title_ << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// 鼠标位置回调
void Window::MousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Mouse moved: " << win->title_ << " -> (" << xpos << ", " << ypos << ")" << std::endl;
    }
}

// 鼠标按钮回调
void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Mouse button: " << win->title_ << " -> button: " << button << ", action: " << action << ", mods: " << mods << std::endl;
    }
}

// 鼠标滚轮回调
void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Mouse scroll: " << win->title_ << " -> (" << xoffset << ", " << yoffset << ")" << std::endl;
    }
}

// 键盘按键回调
void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Key event: " << win->title_ << " -> key: " << key << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;
    }
}

// 字符输入回调
void Window::CharCallback(GLFWwindow* window, unsigned int codepoint) {
    auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Char input: " << win->title_ << " -> codepoint: " << codepoint << std::endl;
    }
}

// WindowManager类的实现

WindowManager::WindowManager() : glfw_initialized_(false) {
}

WindowManager::~WindowManager() {
    // 清理所有窗口
    windows_.clear();
    if (glfw_initialized_) {
        glfwTerminate();
    }
}

// 创建新窗口
std::shared_ptr<Window> WindowManager::CreateWindow(int width, int height, const char* title) {
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
void WindowManager::RemoveClosedWindows() {
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
void WindowManager::Run() {
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
size_t WindowManager::GetWindowCount() const {
    return windows_.size();
}
