#ifndef SGUI_WINDOW_H
#define SGUI_WINDOW_H

#include <memory>
#include <vector>
#include <cstddef>
#include "sgui_common.h"
#include <GLFW/glfw3.h>

// 前向声明
namespace sgui {
    class SCairoRenderer;
    class SContainer;
    class SWindowManager;
}

/**
 * @class SWindow
 * @brief 使用简化Cairo渲染器的窗口类
 *
 * 该类使用CairoRenderer直接绑定window id进行surface绘制，
 * 避免了复杂的OpenGL纹理转换机制。
 */
namespace sgui {
class SWindow {
public:
    /**
     * @brief 构造函数
     * @param width 窗口宽度（像素）
     * @param height 窗口高度（像素）
     * @param title 窗口标题
     * @param manager 关联的窗口管理器指针
     */
    SWindow(int width, int height, const char* title, SWindowManager* manager);

    /**
     * @brief 析构函数
     * 清理窗口资源，销毁GLFW窗口
     */
    ~SWindow();

    /**
     * @brief 初始化窗口
     * @return 成功返回true，失败返回false
     *
     * 创建GLFW窗口，获取窗口ID，并创建简化的Cairo渲染器
     */
    bool Initialize();

    /**
     * @brief 渲染窗口
     *
     * 使用简化的Cairo渲染器直接绘制到窗口
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
    
    /**
     * @brief 设置根容器
     * @param root 根容器指针
     */
    void SetRootContainer(std::shared_ptr<sgui::SContainer> root);
    
    /**
     * @brief 获取根容器
     * @return 根容器指针
     */
    std::shared_ptr<sgui::SContainer> GetRootContainer() const;

    // 禁止复制和赋值
    SWindow(const SWindow&) = delete;
    SWindow& operator=(const SWindow&) = delete;

private:
    int width_;
    int height_;
    const char* title_;
    SWindowManager* manager_;
    GLFWwindow* window_; // GLFWwindow指针
    std::unique_ptr<sgui::SCairoRenderer> cairoRenderer_; // 简化的Cairo渲染器
    std::shared_ptr<sgui::SContainer> rootContainer_; // 根容器

    /**
     * @brief 获取平台特定的窗口ID
     * @return 窗口ID（HWND、X11 Window或NSWindow）
     */
    void* getWindowId();

    // 窗口大小回调函数
    static void WindowSizeCallback(GLFWwindow* window, int width, int height);

    // 窗口关闭回调函数
    static void WindowCloseCallback(GLFWwindow* window);

    // 鼠标位置回调函数
    static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);

    // 鼠标按钮回调函数
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // 鼠标滚轮回调函数
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // 键盘按键回调函数
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // 字符输入回调函数
    static void CharCallback(GLFWwindow* window, unsigned int codepoint);

    friend class SWindowManager;
};

/**
 * @class SWindowManager
 * @brief 简化的窗口管理器类
 *
 * 该类管理使用简化Cairo渲染器的多个窗口，
 * 处理窗口的创建、渲染和生命周期。
 */
class SWindowManager {
public:
    /**
     * @brief 构造函数
     */
    SWindowManager();

    /**
     * @brief 析构函数
     * 清理所有窗口资源并终止GLFW库
     */
    ~SWindowManager();

    /**
     * @brief 创建新窗口
     * @param width 窗口宽度（像素）
     * @param height 窗口高度（像素）
     * @param title 窗口标题
     * @return 成功返回SGUI_Window共享指针，失败返回nullptr
     *
     * 创建并初始化一个使用简化Cairo渲染器的新窗口实例
     */
    std::shared_ptr<SWindow> CreateWindow(int width, int height, const char* title);

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
    std::vector<std::shared_ptr<SWindow>> windows_;
    bool glfw_initialized_;

    // 禁止复制和赋值
    SWindowManager(const SWindowManager&) = delete;
    SWindowManager& operator=(const SWindowManager&) = delete;
};

} // namespace sgui

#endif // SGUI_WINDOW_H
