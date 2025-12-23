/**
 * Window.cpp - 使用简化Cairo渲染器的窗口实现
 */

#include "sgui_window.h"
#include "sgui_cairo_renderer.h"
#include "sgui_container.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

// 为了使用glfwGetX11Window函数，需要包含GLFW的头文件
#ifdef __linux__
#include <X11/Xlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

// SGUI_Window类的实现

namespace sgui
{

SWindow::SWindow(int width, int height, const char *title, SWindowManager *manager) : width_(width), height_(height), title_(title), manager_(manager), window_(nullptr)
{
    // 注意：CairoRenderer需要在窗口创建后才能初始化
}

SWindow::~SWindow()
{
    if (window_)
    {
        glfwDestroyWindow(window_);
    }
}

bool SWindow::Initialize()
{
    // 设置窗口提示，不创建OpenGL上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // 创建窗口
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (!window_)
    {
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
    void *windowId = getWindowId();
    if (windowId)
    {
        cairoRenderer_ = std::make_unique<sgui::SCairoRenderer>(windowId, width_, height_);
        std::cout << "Created simplified Cairo renderer for window: " << title_ << std::endl;
    }
    else
    {
        std::cerr << "Failed to get window ID for: " << title_ << std::endl;
        return false;
    }

    std::cout << "Created window: " << title_ << " (" << width_ << "x" << height_ << ")" << std::endl;
    return true;
}

void SWindow::Render()
{
    if (!window_ || glfwWindowShouldClose(window_))
        return;

    // 如果有根容器，使用双缓冲Cairo渲染
    if (rootContainer_ && cairoRenderer_)
    {
        // 开始双缓冲绘制 - 清除后缓冲并准备绘制
        // 所有绘制操作将先在内存中的后缓冲进行，避免直接绘制到窗口造成闪烁
        cairoRenderer_->begin();

        // 计算布局
        if (rootContainer_->isDirty())
        {
            rootContainer_->setWidth(sgui::LayoutValue::Point(width_));
            rootContainer_->setHeight(sgui::LayoutValue::Point(height_));
            rootContainer_->calculateLayout(width_, height_);
            rootContainer_->clearDirty();
        }

        // 渲染容器树到后缓冲（双缓冲：先绘制到内存）
        // 此时所有绘制操作都在内存中的后缓冲进行，用户看不到绘制过程
        cairo_t *cr = cairoRenderer_->getContext();
        if (cr)
        {
            rootContainer_->renderTree(cr);
        }

        // 结束双缓冲绘制 - 将后缓冲内容一次性复制到前缓冲（窗口）
        // 这样可以避免绘制过程中的闪烁，所有绘制操作在内存中完成后一次性显示
        cairoRenderer_->end();
    }
}

bool SWindow::ShouldClose() const
{
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void *SWindow::GetWindow() const
{
    return window_;
}

const char *SWindow::GetTitle() const
{
    return title_;
}

void SWindow::SetRootContainer(std::shared_ptr<sgui::SContainer> root)
{
    rootContainer_ = root;
    rootContainer_->markDirty();
}

std::shared_ptr<sgui::SContainer> SWindow::GetRootContainer() const
{
    return rootContainer_;
}

void *SWindow::getWindowId()
{
    if (!window_)
        return nullptr;

        // 根据平台返回相应的窗口ID
#ifdef _WIN32
    // Windows平台：返回HWND
    return glfwGetWin32Window(window_);
#elif defined(__linux__)
    // Linux平台：返回X11 Window
    return (void *)glfwGetX11Window(window_);
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
void SWindow::WindowSizeCallback(GLFWwindow *window, int width, int height)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win)
    {
        win->width_ = width;
        win->height_ = height;

        // 调整简化的Cairo渲染器大小
        if (win->cairoRenderer_)
        {
            win->cairoRenderer_->resize(width, height);
        }
        if (win->rootContainer_ != nullptr)
        {
            win->rootContainer_->markDirty();
        }
        std::cout << "Window resized: " << win->title_ << " -> " << width << "x" << height << std::endl;
    }
}

// 窗口关闭回调
void SWindow::WindowCloseCallback(GLFWwindow *window)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win)
    {
        std::cout << "Window close requested: " << win->title_ << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// 全局状态记录，用于跟踪鼠标当前所在的控件（最深层）
static sgui::SContainer *g_lastMouseInsideContainer = nullptr;

// 辅助函数：查找鼠标位置下的最深层子节点
static sgui::SContainer *findDeepestContainerAtPosition(sgui::SContainer *container, float x, float y, float &subx, float &suby)
{
    if (!container)
        return nullptr;

    // 获取容器的布局信息
    float containerX = container->getLeft();
    float containerY = container->getTop();
    float containerWidth = container->getLayoutWidth();
    float containerHeight = container->getLayoutHeight();

    // 检查鼠标是否在容器内
    if (x < containerX || x >= containerX + containerWidth || y < containerY || y >= containerY + containerHeight)
    {
        return nullptr;
    }

    // 转换为相对于容器的坐标
    subx = x - containerX;
    suby = y - containerY;

    // 从后往前遍历子控件（后添加的在上层，优先处理）
    for (int i = static_cast<int>(container->getChildCount()) - 1; i >= 0; --i)
    {
        auto child = container->getChildAt(i);
        if (child)
        {
            auto childContainer = std::dynamic_pointer_cast<sgui::SContainer>(child);
            if (childContainer)
            {
                sgui::SContainer *deepestChild = findDeepestContainerAtPosition(childContainer.get(), x, y, subx, suby);
                if (deepestChild)
                {
                    return deepestChild;
                }
            }
        }
    }

    // 如果没有更深的子节点包含该位置，返回当前容器
    return container;
}

// 辅助函数：将鼠标事件分发到控件树
static void dispatchMouseEvent(sgui::SContainer *rootContainer, const MouseEvent &event)
{
    if (!rootContainer)
        return;

    // std::cout << "MouseEvent: " << g_lastMouseInsideContainer << ", type: " << (int)event.type << ". x,y: " << event.x << "," << event.y << std::endl;

    // 查找鼠标位置下的最深层子节点
    float subx{0}, suby{0};
    sgui::SContainer *targetContainer = findDeepestContainerAtPosition(rootContainer, event.x, event.y, subx, suby);

    // std::cout << "Found ele: " << targetContainer << ". x,y: " << subx << "," << suby << ". w,h: " << targetContainer->getWidth().value << "," <<
    // targetContainer->getHeight().value
    //   << std::endl;
    // 处理 enter/leave 事件：如果目标容器改变了
    if (targetContainer != g_lastMouseInsideContainer)
    {
        // 给旧容器发送 leave 事件
        if (g_lastMouseInsideContainer)
        {
            MouseEvent leaveEvent = event;
            leaveEvent.type = MouseEventType::Leaving;
            g_lastMouseInsideContainer->onMouseExited(leaveEvent);
        }

        // 更新当前容器
        g_lastMouseInsideContainer = targetContainer;

        // 给新容器发送 enter 事件
        if (targetContainer)
        {
            MouseEvent enterEvent = event;
            enterEvent.x = subx;
            enterEvent.y = suby;
            enterEvent.type = MouseEventType::Entering;
            targetContainer->onMouseEntered(enterEvent);
        }
    }

    // 如果鼠标移动且在容器内，发送移动事件
    if (event.isMoving() && targetContainer)
    {
        MouseEvent moveEvent = event;
        moveEvent.x = subx;
        moveEvent.y = suby;
        targetContainer->onMouseMoved(moveEvent);
    }

    // 处理其他事件类型（只在目标容器上处理）
    if (targetContainer)
    {
        MouseEvent relativeEvent = event;
        relativeEvent.x = subx;
        relativeEvent.y = suby;

        if (event.isPressed())
        {
            targetContainer->onMousePressed(relativeEvent);
        }
        if (event.isReleased())
        {
            targetContainer->onMouseReleased(relativeEvent);
        }
        if (event.isClicked())
        {
            targetContainer->onMouseClicked(relativeEvent);
        }
        if (event.isDoubleClicked())
        {
            targetContainer->onMouseDoubleClicked(relativeEvent);
        }
        if (event.isScrolling())
        {
            relativeEvent.scrollX = event.scrollX;
            relativeEvent.scrollY = event.scrollY;
            // 滚轮事件暂时没有对应的处理函数，但可以在这里添加
            std::cout << "Mouse scroll event at (" << event.x << ", " << event.y << ") delta: (" << event.scrollX << ", " << event.scrollY << ")" << std::endl;
        }
    }
}

// 辅助函数：将键盘事件分发到控件树
static void dispatchKeyEvent(SContainer *container, const KeyEvent &event)
{
    if (!container)
        return;

    if (g_lastMouseInsideContainer != nullptr)
    {
        // 然后处理容器自身的键盘事件
        if (event.isPressed())
        {
            g_lastMouseInsideContainer->onKeyPressed(event);
        }
        if (event.isReleased())
        {
            g_lastMouseInsideContainer->onKeyReleased(event);
        }

        // 调试输出
        std::cout << "Key event: " << static_cast<int>(event.type) << " key: " << event.keyCode << " mods: " << event.mods;
        if (event.codepoint != 0)
        {
            std::cout << " codepoint: " << event.codepoint;
        }
        std::cout << std::endl;
    }
}

// 鼠标位置回调
void SWindow::MousePosCallback(GLFWwindow *window, double xpos, double ypos)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win && win->rootContainer_)
    {
        MouseEvent event;
        event.x = xpos;
        event.y = ypos;
        event.type = MouseEventType::Moving;

        dispatchMouseEvent(win->rootContainer_.get(), event);
    }
}

// 鼠标按钮回调
void SWindow::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win && win->rootContainer_)
    {
        // 获取当前鼠标位置
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        MouseEvent event;
        event.x = xpos;
        event.y = ypos;
        event.button = static_cast<MouseButton>(button);

        if (action == GLFW_PRESS)
        {
            event.type = MouseEventType::Pressed;
        }
        else if (action == GLFW_RELEASE)
        {
            event.type = MouseEventType::Released | MouseEventType::Clicked; // 简化处理：释放时视为点击
        }

        dispatchMouseEvent(win->rootContainer_.get(), event);
    }
}

// 鼠标滚轮回调
void SWindow::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win && win->rootContainer_)
    {
        // 获取当前鼠标位置
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        MouseEvent event(xpos, ypos, static_cast<float>(xoffset), static_cast<float>(yoffset));

        dispatchMouseEvent(win->rootContainer_.get(), event);
    }
}

// 键盘按键回调
void SWindow::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win && win->rootContainer_)
    {
        KeyEventType type = KeyEventType::Null;
        if (action == GLFW_PRESS)
        {
            type = KeyEventType::Pressed;
        }
        else if (action == GLFW_RELEASE)
        {
            type = KeyEventType::Released;
        }
        else if (action == GLFW_REPEAT)
        {
            type = KeyEventType::Repeat;
        }

        KeyEvent event(key, type, mods);
        dispatchKeyEvent(win->rootContainer_.get(), event);
    }
}

// 字符输入回调
void SWindow::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    auto win = static_cast<SWindow *>(glfwGetWindowUserPointer(window));
    if (win && win->rootContainer_)
    {
        KeyEvent event(codepoint);
        dispatchKeyEvent(win->rootContainer_.get(), event);
    }
}

// WindowManager类的实现

SWindowManager::SWindowManager() : glfw_initialized_(false)
{
}

SWindowManager::~SWindowManager()
{
    // 清理所有窗口
    windows_.clear();
    if (glfw_initialized_)
    {
        glfwTerminate();
    }
}

// 创建新窗口
std::shared_ptr<SWindow> SWindowManager::CreateWindow(int width, int height, const char *title)
{
    // 初始化GLFW（如果还没有初始化）
    if (!glfw_initialized_)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return nullptr;
        }
        glfw_initialized_ = true;
    }

    auto window = std::make_shared<SWindow>(width, height, title, this);
    if (window->Initialize())
    {
        windows_.push_back(window);
        return window;
    }
    return nullptr;
}

// 移除关闭的窗口
void SWindowManager::RemoveClosedWindows()
{
    auto it = windows_.begin();
    while (it != windows_.end())
    {
        if ((*it)->ShouldClose())
        {
            std::cout << "SWindowManager: Removing closed window: " << (*it)->GetTitle() << std::endl;
            it = windows_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// 运行所有窗口
void SWindowManager::Run()
{
    if (windows_.empty())
    {
        std::cout << "No windows to run" << std::endl;
        return;
    }

    std::cout << "Created " << windows_.size() << " windows with simplified Cairo rendering." << std::endl;
    std::cout << "Each window can be closed independently. Program exits when all windows are closed." << std::endl;

    // // 使用高精度时钟实现帧率控制
    // const auto target_frame_duration = std::chrono::nanoseconds(16666667); // 60 FPS = 16.666667ms
    // auto next_frame_time = std::chrono::high_resolution_clock::now();

    while (!windows_.empty())
    {
        // 渲染所有打开的窗口
        for (auto &window : windows_)
        {
            window->Render();
        }

        // 移除关闭的窗口
        RemoveClosedWindows();

        // 处理事件
        glfwPollEvents();

        // // 计算下一帧时间点
        // next_frame_time += target_frame_duration;

        // // 等待到下一帧时间点
        // auto now = std::chrono::high_resolution_clock::now();
        // if (next_frame_time > now)
        // {
        //     std::this_thread::sleep_until(next_frame_time);
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "All windows closed. Exiting program." << std::endl;
}

// 获取窗口数量
size_t SWindowManager::GetWindowCount() const
{
    return windows_.size();
}

} // namespace sgui
