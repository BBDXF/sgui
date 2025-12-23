/**
 * GUI按钮控件实现
 */

#include "sgui_button.h"
#include <iostream>

namespace sgui
{

SButton::SButton()
{
    // 设置按钮的基本样式
    setBackgroundColor(m_normalBackgroundColor);
    setBorderColor(m_normalBorderColor);
    setColor(m_normalTextColor);
    setBorderStyle(BorderStyle::Solid);
    setBorderRadius(EdgeInsets::All(4.0f));

    // 设置一些默认的内边距，让按钮看起来更美观
    setPadding(EdgeInsets::Symmetric(16.0f, 8.0f));

    // 设置文本居中对齐
    setTextAlign(TextAlign::Center);

    // 标记样式已更改，通过公共接口
    // 由于 markStylesDirty 是私有的，我们使用 markDirty() 来标记需要重新绘制
    markDirty();
}

SButton::SButton(const std::string &text) : SButton()
{
    setText(text);
}

void SButton::setButtonText(const std::string &text)
{
    setText(text);
}

void SButton::setOnClick(MouseEventCallback callback)
{
    m_onClick = callback;
}

void SButton::setDisabled(bool disabled)
{
    setState(ControlState::Disabled);
}

void SButton::setState(ControlState newState)
{
    if (m_state == newState)
    {
        return;
    }

    m_state = newState;
    updateAppearance();
}

void SButton::updateAppearance()
{
    switch (m_state)
    {
    case ControlState::Normal:
        setBackgroundColor(m_normalBackgroundColor);
        setBorderColor(m_normalBorderColor);
        setColor(m_normalTextColor);
        break;

    case ControlState::Hover:
        setBackgroundColor(m_hoverBackgroundColor);
        setBorderColor(m_hoverBorderColor);
        setColor(m_hoverTextColor);
        break;

    case ControlState::Pressed:
        setBackgroundColor(m_pressedBackgroundColor);
        setBorderColor(m_pressedBorderColor);
        setColor(m_pressedTextColor);
        break;

    case ControlState::Disabled:
        setBackgroundColor(m_disabledBackgroundColor);
        setBorderColor(m_disabledBorderColor);
        setColor(m_disabledTextColor);
        break;

    case ControlState::Focused:
        // 暂时不特别处理焦点状态，使用正常状态的样式
        setBackgroundColor(m_normalBackgroundColor);
        setBorderColor(m_normalBorderColor);
        setColor(m_normalTextColor);
        break;
    }

    // 标记样式已更改，通过公共接口
    markDirty();
}

void SButton::onMousePressed(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    if (event.button == MouseButton::Left)
    {
        setState(ControlState::Pressed);
    }
}

void SButton::onMouseReleased(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    if (event.button == MouseButton::Left)
    {
        if (m_state == ControlState::Pressed)
        {
            setState(ControlState::Hover);
        }
    }
}

void SButton::onMouseClicked(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    if (event.button == MouseButton::Left)
    {
        // std::cout << "Button clicked at (" << event.x << ", " << event.y << ")" << std::endl;
        // 调用点击回调函数
        if (m_onClick)
        {
            m_onClick(event);
        }
    }
}

void SButton::onMouseMoved(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    setState(ControlState::Hover);
}

void SButton::onMouseEntered(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    setState(ControlState::Hover);
}

void SButton::onMouseExited(const MouseEvent &event)
{
    if (m_state == ControlState::Disabled)
    {
        return;
    }

    setState(ControlState::Normal);
}

} // namespace sgui
