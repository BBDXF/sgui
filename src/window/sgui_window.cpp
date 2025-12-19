/**
 * Window.cpp - 使用简化Cairo渲染器的窗口实现
 */

#include "sgui_window.h"
#include "sgui_containerex.h"
#include "sgui_cairo_renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

// 为了使用glfwGetX11Window函数，需要包含GLFW的头文件
#ifdef __linux__
#include <X11/Xlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#endif

// SGUI_Window类的实现

namespace sgui {

SWindow::SWindow(int width, int height, const char* title, SWindowManager* manager)
    : width_(width), height_(height), title_(title), manager_(manager), window_(nullptr) {
    // 注意：CairoRenderer需要在窗口创建后才能初始化
}

SWindow::~SWindow() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

bool SWindow::Initialize() {
    // 设置窗口提示，不创建OpenGL上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
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

    // 获取窗口ID并创建简化的Cairo渲染器
    void* windowId = getWindowId();
    if (windowId) {
        cairoRenderer_ = std::make_unique<sgui::SCairoRenderer>(windowId, width_, height_);
        std::cout << "Created simplified Cairo renderer for window: " << title_ << std::endl;
    } else {
        std::cerr << "Failed to get window ID for: " << title_ << std::endl;
        return false;
    }

    std::cout << "Created window: " << title_ << " (" << width_ << "x" << height_ << ")" << std::endl;
    return true;
}

void SWindow::Render() {
    if (!window_ || glfwWindowShouldClose(window_)) return;

    // 如果有根容器，使用简化的Cairo渲染
    if (rootContainer_ && cairoRenderer_) {
        // 开始Cairo绘制
        cairoRenderer_->begin();
        
        // 计算布局
        if(rootContainer_->isDirty()){
            rootContainer_->setWidth(sgui::LayoutValue::Point(width_));
            rootContainer_->setHeight(sgui::LayoutValue::Point(height_));
            rootContainer_->calculateLayout(width_, height_);
            rootContainer_->clearDirty();
        }
        
        // 渲染容器树到Cairo上下文
        cairo_t* cr = cairoRenderer_->getContext();
        if (cr) {
            rootContainer_->renderTree(cr);
        }
        
        // 结束Cairo绘制（直接刷新到窗口）
        cairoRenderer_->end();
    }

    // 刷新窗口显示（不使用OpenGL缓冲区交换）
    // Cairo渲染器已经直接绘制到窗口，无需额外的缓冲区操作
}

bool SWindow::ShouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void* SWindow::GetWindow() const {
    return window_;
}

const char* SWindow::GetTitle() const {
    return title_;
}

void SWindow::SetRootContainer(std::shared_ptr<sgui::SContainerEx> root) {
    rootContainer_ = root;
}

std::shared_ptr<sgui::SContainerEx> SWindow::GetRootContainer() const {
    return rootContainer_;
}

void* SWindow::getWindowId() {
    if (!window_) return nullptr;
    
    // 根据平台返回相应的窗口ID
#ifdef _WIN32
    // Windows平台：返回HWND
    return glfwGetWin32Window(window_);
#elif defined(__linux__)
    // Linux平台：返回X11 Window
    return (void*)glfwGetX11Window(window_);
#elif defined(__APPLE__)
    // macOS平台：返回NSWindow
    return glfwGetCocoaWindow(window_);
#else
    // 其他平台暂不支持直接绘制
    std::cerr << "Platform not supported for direct Cairo rendering" << std::endl;
    return nullptr;
#endif
}

// 窗口大小回调
void SWindow::WindowSizeCallback(GLFWwindow* window, int width, int height) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width_ = width;
        win->height_ = height;
        
        // 调整简化的Cairo渲染器大小
        if (win->cairoRenderer_) {
            win->cairoRenderer_->resize(width, height);
        }
        
        std::cout << "Window resized: " << win->title_ << " -> " << width << "x" << height << std::endl;
    }
}

// 窗口关闭回调
void SWindow::WindowCloseCallback(GLFWwindow* window) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Window close requested: " << win->title_ << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// 鼠标位置回调
void SWindow::MousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        // std::cout << "Mouse moved: " << win->title_ << " -> (" << xpos << ", " << ypos << ")" << std::endl;
    }
}

// 鼠标按钮回调
void SWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        // std::cout << "Mouse button: " << win->title_ << " -> button: " << button << ", action: " << action << ", mods: " << mods << std::endl;
    }
}

// 鼠标滚轮回调
void SWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Mouse scroll: " << win->title_ << " -> (" << xoffset << ", " << yoffset << ")" << std::endl;
    }
}

// 键盘按键回调
void SWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        std::cout << "Key event: " << win->title_ << " -> key: " << key << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;
    }
}

// 字符输入回调
void SWindow::CharCallback(GLFWwindow* window, unsigned int codepoint) {
    auto win = static_cast<SWindow*>(glfwGetWindowUserPointer(window));
    if (win) {
        // std::cout << "Char input: " << win->title_ << " -> codepoint: " << codepoint << std::endl;
    }
}

// WindowManager类的实现

SWindowManager::SWindowManager() : glfw_initialized_(false) {
}

SWindowManager::~SWindowManager() {
    // 清理所有窗口
    windows_.clear();
    if (glfw_initialized_) {
        glfwTerminate();
    }
}

// 创建新窗口
std::shared_ptr<SWindow> SWindowManager::CreateWindow(int width, int height, const char* title) {
    // 初始化GLFW（如果还没有初始化）
    if (!glfw_initialized_) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return nullptr;
        }
        glfw_initialized_ = true;
    }

    auto window = std::make_shared<SWindow>(width, height, title, this);
    if (window->Initialize()) {
        windows_.push_back(window);
        return window;
    }
    return nullptr;
}

// 移除关闭的窗口
void SWindowManager::RemoveClosedWindows() {
    auto it = windows_.begin();
    while (it != windows_.end()) {
        if ((*it)->ShouldClose()) {
            std::cout << "SWindowManager: Removing closed window: " << (*it)->GetTitle() << std::endl;
            it = windows_.erase(it);
        } else {
            ++it;
        }
    }
}

// 运行所有窗口
void SWindowManager::Run() {
    if (windows_.empty()) {
        std::cout << "No windows to run" << std::endl;
        return;
    }

    std::cout << "Created " << windows_.size() << " windows with simplified Cairo rendering." << std::endl;
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
size_t SWindowManager::GetWindowCount() const {
    return windows_.size();
}

} // namespace sgui
