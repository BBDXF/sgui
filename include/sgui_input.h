/**
 * GUI输入框控件
 * 
 * 继承自SContainer，添加文本输入、光标显示、焦点管理等输入框特有功能
 */

#pragma once

#include "sgui_container.h"
#include <functional>
#include <string>
#include <chrono>

namespace sgui {

// 前向声明
class SInput;

// 智能指针类型定义
using SInputPtr = std::shared_ptr<SInput>;

/**
 * 输入类型枚举
 */
enum class InputType
{
    Text,        // 文本输入
    Password,    // 密码输入（显示为*或●）
    Number,      // 数字输入
    Email,       // 邮箱输入
    Search,      // 搜索输入
    Url,         // URL输入
    Tel,         // 电话号码输入
    MultiLine    // 多行文本输入
};

/**
 * 文本变化回调函数类型
 */
using TextChangedCallback = std::function<void(const std::string&)>;

/**
 * 焦点变化回调函数类型
 */
using FocusChangedCallback = std::function<void(bool)>;

/**
 * 回车键回调函数类型
 */
using EnterPressedCallback = std::function<void(const std::string&)>;

/**
 * Input类 - 输入框控件
 * 
 * 支持文本输入、光标显示、文本选择、输入验证等功能
 */
class SInput : public SContainer {
public:
    SInput();
    explicit SInput(const std::string& placeholder);
    ~SInput() override;
    
    // ====================================================================
    // 基本属性设置
    // ====================================================================
    
    /** 设置输入类型 */
    void setInputType(InputType type);
    /** 获取输入类型 */
    InputType getInputType() const { return m_inputType; }
    
    /** 设置占位符文本 */
    void setPlaceholder(const std::string& placeholder);
    /** 获取占位符文本 */
    const std::string& getPlaceholder() const { return m_placeholder; }
    
    /** 设置输入框的值 */
    void setValue(const std::string& value);
    /** 获取输入框的值 */
    const std::string& getValue() const { return getText(); }
    
    /** 设置是否只读 */
    void setReadOnly(bool readOnly);
    /** 是否只读 */
    bool isReadOnly() const { return m_readOnly; }
    
    /** 设置最大字符长度 */
    void setMaxLength(int maxLength);
    /** 获取最大字符长度 */
    int getMaxLength() const { return m_maxLength; }
    
    /** 设置是否禁用 */
    void setDisabled(bool disabled);
    /** 是否禁用 */
    bool isDisabled() const { return m_state == ControlState::Disabled; }
    
    /** 获取当前状态 */
    ControlState getState() const { return m_state; }
    
    // ====================================================================
    // 光标和选择相关
    // ====================================================================
    
    /** 设置光标位置 */
    void setCursorPosition(int position);
    /** 获取光标位置 */
    int getCursorPosition() const { return m_cursorPosition; }
    
    /** 选择全部文本 */
    void selectAll();
    /** 清除选择 */
    void clearSelection();
    /** 是否有选中文本 */
    bool hasSelection() const;
    /** 获取选中的文本 */
    std::string getSelectedText() const;
    
    // ====================================================================
    // 样式相关设置
    // ====================================================================
    
    /** 设置占位符文本颜色 */
    void setPlaceholderColor(const Color& color) { m_placeholderColor = color; markDirty(); }
    /** 获取占位符文本颜色 */
    const Color& getPlaceholderColor() const { return m_placeholderColor; }
    
    /** 设置光标颜色 */
    void setCursorColor(const Color& color) { m_cursorColor = color; markDirty(); }
    /** 获取光标颜色 */
    const Color& getCursorColor() const { return m_cursorColor; }
    
    /** 设置选择高亮颜色 */
    void setSelectionColor(const Color& color) { m_selectionColor = color; markDirty(); }
    /** 获取选择高亮颜色 */
    const Color& getSelectionColor() const { return m_selectionColor; }
    
    /** 设置光标宽度 */
    void setCursorWidth(float width) { m_cursorWidth = width; markDirty(); }
    /** 获取光标宽度 */
    float getCursorWidth() const { return m_cursorWidth; }
    
    // ====================================================================
    // 状态相关样式设置
    // ====================================================================
    
    /** 设置正常状态边框色 */
    void setNormalBorderColor(const Color& color) { m_normalBorderColor = color; updateAppearance(); }
    /** 设置焦点状态边框色 */
    void setFocusedBorderColor(const Color& color) { m_focusedBorderColor = color; updateAppearance(); }
    /** 设置悬停状态边框色 */
    void setHoverBorderColor(const Color& color) { m_hoverBorderColor = color; updateAppearance(); }
    /** 设置禁用状态边框色 */
    void setDisabledBorderColor(const Color& color) { m_disabledBorderColor = color; updateAppearance(); }
    
    /** 设置正常状态背景色 */
    void setNormalBackgroundColor(const Color& color) { m_normalBackgroundColor = color; updateAppearance(); }
    /** 设置焦点状态背景色 */
    void setFocusedBackgroundColor(const Color& color) { m_focusedBackgroundColor = color; updateAppearance(); }
    /** 设置悬停状态背景色 */
    void setHoverBackgroundColor(const Color& color) { m_hoverBackgroundColor = color; updateAppearance(); }
    /** 设置禁用状态背景色 */
    void setDisabledBackgroundColor(const Color& color) { m_disabledBackgroundColor = color; updateAppearance(); }
    
    // ====================================================================
    // 回调函数设置
    // ====================================================================
    
    /** 设置文本变化回调 */
    void setOnTextChanged(TextChangedCallback callback);
    /** 设置焦点变化回调 */
    void setOnFocusChanged(FocusChangedCallback callback);
    /** 设置回车键回调 */
    void setOnEnterPressed(EnterPressedCallback callback);
    
    // ====================================================================
    // 公共方法
    // ====================================================================
    
    /** 清空输入框 */
    void clear();
    /** 撤销操作 */
    void undo();
    /** 重做操作 */
    void redo();
    /** 复制选中文本 */
    void copy();
    /** 剪切选中文本 */
    void cut();
    /** 粘贴文本 */
    void paste();
    /** 插入文本 */
    void insertText(const std::string& text);
    /** 删除选中文本 */
    void deleteSelected();

protected:
    // ====================================================================
    // 重写事件处理函数
    // ====================================================================
    
    void onMousePressed(const MouseEvent& event) override;
    void onMouseReleased(const MouseEvent& event) override;
    void onMouseMoved(const MouseEvent& event) override;
    void onMouseEntered(const MouseEvent& event) override;
    void onMouseExited(const MouseEvent& event) override;
    void onKeyPressed(const KeyEvent& event) override;
    void onKeyReleased(const KeyEvent& event) override;
    
    // ====================================================================
    // 重写绘制函数
    // ====================================================================
    
    void render(cairo_t* cr) override;

private:
    // ====================================================================
    // 私有辅助函数
    // ====================================================================
    
    /** 设置输入框状态 */
    void setState(ControlState newState);
    
    /** 更新外观（根据当前状态） */
    void updateAppearance();
    
    /** 获取显示的文本（考虑密码模式） */
    std::string getDisplayText() const;
    
    /** 处理字符输入 */
    void handleCharInput(unsigned int codepoint);
    
    /** 处理特殊键输入 */
    void handleSpecialKey(const KeyEvent& event);
    
    /** 移动光标 */
    void moveCursor(int delta);
    
    /** 移动光标到指定位置 */
    void moveCursorTo(int position);
    
    /** 选择文本 */
    void selectText(int start, int end);
    
    /** 删除字符 */
    void deleteChar(int position);
    
    /** 插入字符到指定位置 */
    void insertCharAt(int position, char c);
    
    /** 检查位置是否在选择范围内 */
    bool isPositionSelected(int position) const;
    
    /** 获取文本在指定位置的像素坐标 */
    float getTextPositionAt(int charIndex) const;
    
    /** 根据像素坐标获取字符位置 */
    int getCharIndexAt(float x) const;
    
    /** 更新光标闪烁状态 */
    void updateCursorBlink();
    
    /** 添加到撤销历史 */
    void addToHistory();
    
    /** 触发文本变化事件 */
    void triggerTextChanged();
    
    /** 触发焦点变化事件 */
    void triggerFocusChanged(bool focused);

private:
    // ====================================================================
    // 成员变量
    // ====================================================================
    
    /** 输入框状态 */
    ControlState m_state = ControlState::Normal;
    
    /** 输入类型 */
    InputType m_inputType = InputType::Text;
    
    /** 占位符文本 */
    std::string m_placeholder;
    
    /** 只读标志 */
    bool m_readOnly = false;
    
    /** 最大字符长度 */
    int m_maxLength = -1; // -1表示无限制
    
    // ====================================================================
    // 光标和选择相关
    // ====================================================================
    
    /** 光标位置 */
    int m_cursorPosition = 0;
    
    /** 选择起始位置 */
    int m_selectionStart = -1;
    
    /** 选择结束位置 */
    int m_selectionEnd = -1;
    
    /** 光标是否显示（用于闪烁效果） */
    bool m_cursorVisible = true;
    
    /** 上次光标闪烁时间 */
    std::chrono::steady_clock::time_point m_lastBlinkTime;
    
    // ====================================================================
    // 样式配置
    // ====================================================================
    
    /** 占位符颜色 */
    Color m_placeholderColor = Color(0.6, 0.6, 0.6, 1.0);
    
    /** 光标颜色 */
    Color m_cursorColor = Color(0, 0, 0, 1.0);
    
    /** 选择高亮颜色 */
    Color m_selectionColor = Color(0.2, 0.6, 1.0, 0.3);
    
    /** 光标宽度 */
    float m_cursorWidth = 1.0f;
    
    // ====================================================================
    // 各状态的颜色配置
    // ====================================================================
    
    /** 背景色 */
    Color m_normalBackgroundColor = Color::White();
    Color m_focusedBackgroundColor = Color::White();
    Color m_hoverBackgroundColor = Color(0.98, 0.98, 0.98, 1.0);
    Color m_disabledBackgroundColor = Color(0.95, 0.95, 0.95, 1.0);
    
    /** 边框色 */
    Color m_normalBorderColor = Color::LightGray();
    Color m_focusedBorderColor = Color(0.2, 0.6, 1.0, 1.0);
    Color m_hoverBorderColor = Color::Gray();
    Color m_disabledBorderColor = Color(0.8, 0.8, 0.8, 1.0);
    
    // ====================================================================
    // 回调函数
    // ====================================================================
    
    /** 文本变化回调 */
    TextChangedCallback m_onTextChanged{nullptr};
    
    /** 焦点变化回调 */
    FocusChangedCallback m_onFocusChanged{nullptr};
    
    /** 回车键回调 */
    EnterPressedCallback m_onEnterPressed{nullptr};
    
    // ====================================================================
    // 历史记录（用于撤销/重做）
    // ====================================================================
    
    /** 撤销历史 */
    std::vector<std::string> m_undoHistory;
    
    /** 重做历史 */
    std::vector<std::string> m_redoHistory;
    
    /** 最大历史记录数 */
    static const int MAX_HISTORY_SIZE = 50;
};

} // namespace sgui
