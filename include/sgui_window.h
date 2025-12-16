#ifndef SGUI_WINDOW_H
#define SGUI_WINDOW_H

#include <memory>
#include <vector>
#include <cstddef>

// 前向声明
class WindowManager;

/**
 * @class Window
 * @brief 单个窗口类，用于管理GLFW窗口的创建、渲染和生命周期
 *
 * 该类封装了GLFW窗口的基本操作，包括窗口创建、大小调整回调、
 * 关闭回调等功能。每个窗口实例都与一个WindowManager关联。
 */
class Window {
public:
    /**
     * @brief 构造函数
     * @param width 窗口宽度（像素）
     * @param height 窗口高度（像素）
     * @param title 窗口标题
     * @param manager 关联的窗口管理器指针
     */
    Window(int width, int height, const char* title, WindowManager* manager);

    /**
     * @brief 析构函数
     * 清理窗口资源，销毁GLFW窗口
     */
    ~Window();

    /**
     * @brief 初始化窗口
     * @return 成功返回true，失败返回false
     *
     * 创建GLFW窗口并设置相关回调函数
     */
    bool Initialize();

    /**
     * @brief 渲染窗口
     *
     * 设置当前OpenGL上下文，清除屏幕，并交换缓冲区
     */
    void Render();

    /**
     * @brief 检查窗口是否应该关闭
     * @return 窗口应该关闭返回true，否则返回false
     */
    bool ShouldClose() const;

    /**
     * @brief 获取GLFW窗口指针
     * @return GLFWwindow指针
     */
    void* GetWindow() const;

    /**
     * @brief 获取窗口标题
     * @return 窗口标题字符串
     */
    const char* GetTitle() const;

    // 禁止复制和赋值
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

private:
    int width_;
    int height_;
    const char* title_;
    WindowManager* manager_;
    struct GLFWwindow* window_; // GLFWwindow指针

    // 窗口大小回调函数
    static void WindowSizeCallback(struct GLFWwindow* window, int width, int height);

    // 窗口关闭回调函数
    static void WindowCloseCallback(struct GLFWwindow* window);

    // 鼠标位置回调函数
    static void MousePosCallback(struct GLFWwindow* window, double xpos, double ypos);

    // 鼠标按钮回调函数
    static void MouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);

    // 鼠标滚轮回调函数
    static void ScrollCallback(struct GLFWwindow* window, double xoffset, double yoffset);

    // 键盘按键回调函数
    static void KeyCallback(struct GLFWwindow* window, int key, int scancode, int action, int mods);

    // 字符输入回调函数
    static void CharCallback(struct GLFWwindow* window, unsigned int codepoint);

    friend class WindowManager;
};

/**
 * @class WindowManager
 * @brief 窗口管理器类，用于管理多个窗口的创建、渲染和生命周期
 *
 * 该类负责初始化GLFW库，创建和管理多个Window实例，
 * 处理窗口的渲染循环，并清理关闭的窗口。
 */
class WindowManager {
public:
    /**
     * @brief 构造函数
     */
    WindowManager();

    /**
     * @brief 析构函数
     * 清理所有窗口资源并终止GLFW库
     */
    ~WindowManager();

    /**
     * @brief 创建新窗口
     * @param width 窗口宽度（像素）
     * @param height 窗口高度（像素）
     * @param title 窗口标题
     * @return 成功返回Window共享指针，失败返回nullptr
     *
     * 创建并初始化一个新的窗口实例，自动初始化GLFW库（如果尚未初始化）
     */
    std::shared_ptr<Window> CreateWindow(int width, int height, const char* title);

    /**
     * @brief 移除已关闭的窗口
     *
     * 从窗口列表中移除所有ShouldClose()返回true的窗口
     */
    void RemoveClosedWindows();

    /**
     * @brief 运行主循环
     *
     * 执行窗口的主渲染循环，直到所有窗口被关闭
     */
    void Run();

    /**
     * @brief 获取当前窗口数量
     * @return 当前管理的窗口数量
     */
    size_t GetWindowCount() const;

private:
    std::vector<std::shared_ptr<Window>> windows_;
    bool glfw_initialized_;

    // 禁止复制和赋值
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

#endif // SGUI_WINDOW_H
