/**
 * GUI按钮控件
 * 
 * 继承自SContainer，添加按钮特有的交互功能
 */

#pragma once

#include "sgui_container.h"
#include <functional>

namespace sgui {

// 前向声明
class SButton;

// 智能指针类型定义
using SButtonPtr = std::shared_ptr<SButton>;


/**
 * Button类 - 按钮控件
 * 
 * 支持鼠标悬停、点击等状态，并能够根据状态改变外观
 */
class SButton : public SContainer {
public:
    SButton();
    explicit SButton(const std::string& text);
    ~SButton() = default;
    
    // ====================================================================
    // 按钮特有属性
    // ====================================================================
    
    /** 设置按钮文本 */
    void setButtonText(const std::string& text);
    /** 获取按钮文本 */
    const std::string& getButtonText() const { return getText(); }
    
    /** 设置点击回调函数 */
    void setOnClick(MouseEventCallback callback);
    
    /** 设置是否禁用 */
    void setDisabled(bool disabled);
    /** 是否禁用 */
    bool isDisabled() const { return m_state == ControlState::Disabled; }
    
    /** 获取当前状态 */
    ControlState getState() const { return m_state; }
    
    // ====================================================================
    // 状态相关样式设置
    // ====================================================================
    
    /** 设置正常状态背景色 */
    void setNormalBackgroundColor(const Color& color) { m_normalBackgroundColor = color; updateAppearance(); }
    /** 设置悬停状态背景色 */
    void setHoverBackgroundColor(const Color& color) { m_hoverBackgroundColor = color; updateAppearance(); }
    /** 设置按下状态背景色 */
    void setPressedBackgroundColor(const Color& color) { m_pressedBackgroundColor = color; updateAppearance(); }
    /** 设置禁用状态背景色 */
    void setDisabledBackgroundColor(const Color& color) { m_disabledBackgroundColor = color; updateAppearance(); }
    
    /** 设置正常状态边框色 */
    void setNormalBorderColor(const Color& color) { m_normalBorderColor = color; updateAppearance(); }
    /** 设置悬停状态边框色 */
    void setHoverBorderColor(const Color& color) { m_hoverBorderColor = color; updateAppearance(); }
    /** 设置按下状态边框色 */
    void setPressedBorderColor(const Color& color) { m_pressedBorderColor = color; updateAppearance(); }
    /** 设置禁用状态边框色 */
    void setDisabledBorderColor(const Color& color) { m_disabledBorderColor = color; updateAppearance(); }
    
    /** 设置正常状态文本色 */
    void setNormalTextColor(const Color& color) { m_normalTextColor = color; updateAppearance(); }
    /** 设置悬停状态文本色 */
    void setHoverTextColor(const Color& color) { m_hoverTextColor = color; updateAppearance(); }
    /** 设置按下状态文本色 */
    void setPressedTextColor(const Color& color) { m_pressedTextColor = color; updateAppearance(); }
    /** 设置禁用状态文本色 */
    void setDisabledTextColor(const Color& color) { m_disabledTextColor = color; updateAppearance(); }

protected:
    // ====================================================================
    // 重写事件处理函数
    // ====================================================================
    
    void onMousePressed(const MouseEvent& event) override;
    void onMouseReleased(const MouseEvent& event) override;
    void onMouseClicked(const MouseEvent& event) override;
    void onMouseMoved(const MouseEvent& event) override;
    void onMouseEntered(const MouseEvent& event) override;
    void onMouseExited(const MouseEvent& event) override;

private:
    // ====================================================================
    // 私有辅助函数
    // ====================================================================
    
    /** 设置按钮状态 */
    void setState(ControlState newState);
    
    /** 更新外观（根据当前状态） */
    void updateAppearance();

private:
    // ====================================================================
    // 成员变量
    // ====================================================================
    
    /** 按钮状态 */
    ControlState m_state = ControlState::Normal;
    
    /** 点击回调函数 */
    MouseEventCallback m_onClick{nullptr};
    
    // ====================================================================
    // 各状态的颜色配置
    // ====================================================================
    
    /** 背景色 */
    Color m_normalBackgroundColor = Color::LightGray();
    Color m_hoverBackgroundColor = Color::Gray();
    Color m_pressedBackgroundColor = Color::DarkGray();
    Color m_disabledBackgroundColor = Color(0.8, 0.8, 0.8, 1.0);
    
    /** 边框色 */
    Color m_normalBorderColor = Color::Gray();
    Color m_hoverBorderColor = Color::DarkGray();
    Color m_pressedBorderColor = Color::Black();
    Color m_disabledBorderColor = Color(0.6, 0.6, 0.6, 1.0);
    
    /** 文本色 */
    Color m_normalTextColor = Color::Black();
    Color m_hoverTextColor = Color::Black();
    Color m_pressedTextColor = Color::White();
    Color m_disabledTextColor = Color(0.5, 0.5, 0.5, 1.0);
};

} // namespace sgui
