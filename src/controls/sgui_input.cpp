/**
 * GUI输入框控件实现
 */

#include "sgui_input.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace sgui
{

SInput::SInput()
{
    // 设置输入框的基本样式
    setBackgroundColor(m_normalBackgroundColor);
    setBorderColor(m_normalBorderColor);
    setBorderStyle(BorderStyle::Solid);
    setBorderRadius(EdgeInsets::All(4.0f));
    
    // 设置内边距
    setPadding(EdgeInsets::Symmetric(12.0f, 8.0f));
    
    // 设置文本样式
    setFontSize(14.0f);
    setFontFamily(SGUI_DEFAULT_FONT_FAMILY);
    setTextAlign(TextAlign::Left);
    
    // 初始化光标闪烁时间
    m_lastBlinkTime = std::chrono::steady_clock::now();
    
    markDirty();
}

SInput::SInput(const std::string& placeholder) : SInput()
{
    m_placeholder = placeholder;
}

SInput::~SInput() = default;

// ====================================================================
// 基本属性设置实现
// ====================================================================

void SInput::setInputType(InputType type)
{
    if (m_inputType != type)
    {
        m_inputType = type;
        markDirty();
    }
}

void SInput::setPlaceholder(const std::string& placeholder)
{
    m_placeholder = placeholder;
    markDirty();
}

void SInput::setValue(const std::string& value)
{
    addToHistory();
    setText(value);
    m_cursorPosition = std::min(m_cursorPosition, static_cast<int>(value.length()));
    clearSelection();
    triggerTextChanged();
    markDirty();
}

void SInput::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
    if (readOnly && m_state == ControlState::Focused)
    {
        setState(ControlState::Normal);
    }
}

void SInput::setMaxLength(int maxLength)
{
    m_maxLength = maxLength;
    if (maxLength > 0 && getText().length() > static_cast<size_t>(maxLength))
    {
        std::string truncated = getText().substr(0, maxLength);
        setText(truncated);
        m_cursorPosition = std::min(m_cursorPosition, maxLength);
        triggerTextChanged();
    }
}

void SInput::setDisabled(bool disabled)
{
    setState(disabled ? ControlState::Disabled : ControlState::Normal);
}

// ====================================================================
// 光标和选择相关实现
// ====================================================================

void SInput::setCursorPosition(int position)
{
    int textLength = static_cast<int>(getText().length());
    m_cursorPosition = std::max(0, std::min(position, textLength));
    clearSelection();
    markDirty();
}

void SInput::selectAll()
{
    int textLength = static_cast<int>(getText().length());
    selectText(0, textLength);
}

void SInput::clearSelection()
{
    m_selectionStart = -1;
    m_selectionEnd = -1;
    markDirty();
}

bool SInput::hasSelection() const
{
    return m_selectionStart >= 0 && m_selectionEnd >= 0 && m_selectionStart != m_selectionEnd;
}

std::string SInput::getSelectedText() const
{
    if (!hasSelection())
        return "";
    
    const std::string& text = getText();
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    
    return text.substr(start, end - start);
}

// ====================================================================
// 回调函数设置实现
// ====================================================================

void SInput::setOnTextChanged(TextChangedCallback callback)
{
    m_onTextChanged = std::move(callback);
}

void SInput::setOnFocusChanged(FocusChangedCallback callback)
{
    m_onFocusChanged = std::move(callback);
}

void SInput::setOnEnterPressed(EnterPressedCallback callback)
{
    m_onEnterPressed = std::move(callback);
}

// ====================================================================
// 公共方法实现
// ====================================================================

void SInput::clear()
{
    if (!getText().empty())
    {
        addToHistory();
        setText("");
        m_cursorPosition = 0;
        clearSelection();
        triggerTextChanged();
    }
}

void SInput::undo()
{
    if (!m_undoHistory.empty())
    {
        m_redoHistory.push_back(getText());
        setText(m_undoHistory.back());
        m_undoHistory.pop_back();
        m_cursorPosition = static_cast<int>(getText().length());
        clearSelection();
        triggerTextChanged();
    }
}

void SInput::redo()
{
    if (!m_redoHistory.empty())
    {
        m_undoHistory.push_back(getText());
        setText(m_redoHistory.back());
        m_redoHistory.pop_back();
        m_cursorPosition = static_cast<int>(getText().length());
        clearSelection();
        triggerTextChanged();
    }
}

void SInput::copy()
{
    if (hasSelection())
    {
        // 这里应该将选中的文本复制到剪贴板
        // 由于没有剪贴板接口，这里只是示例
        std::string selectedText = getSelectedText();
        std::cout << "Copy to clipboard: " << selectedText << std::endl;
    }
}

void SInput::cut()
{
    if (hasSelection())
    {
        copy();
        deleteSelected();
    }
}

void SInput::paste()
{
    // 这里应该从剪贴板获取文本
    // 由于没有剪贴板接口，这里只是示例
    std::string clipboardText = "pasted text"; // 示例文本
    insertText(clipboardText);
}

void SInput::insertText(const std::string& text)
{
    if (text.empty())
        return;
    
    addToHistory();
    
    // 如果有选中文本，先删除
    deleteSelected();
    
    const std::string& currentText = getText();
    
    // 检查长度限制
    if (m_maxLength > 0)
    {
        int availableLength = m_maxLength - static_cast<int>(currentText.length());
        if (availableLength <= 0)
            return;
        
        std::string insertText = text.substr(0, availableLength);
        std::string newText = currentText.substr(0, m_cursorPosition) + insertText + 
                             currentText.substr(m_cursorPosition);
        setText(newText);
        m_cursorPosition += static_cast<int>(insertText.length());
    }
    else
    {
        std::string newText = currentText.substr(0, m_cursorPosition) + text + 
                             currentText.substr(m_cursorPosition);
        setText(newText);
        m_cursorPosition += static_cast<int>(text.length());
    }
    
    triggerTextChanged();
}

void SInput::deleteSelected()
{
    if (!hasSelection())
        return;
    
    addToHistory();
    
    const std::string& currentText = getText();
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    
    std::string newText = currentText.substr(0, start) + currentText.substr(end);
    setText(newText);
    m_cursorPosition = start;
    clearSelection();
    triggerTextChanged();
}

// ====================================================================
// 重写事件处理函数实现
// ====================================================================

void SInput::onMousePressed(const MouseEvent& event)
{
    if (m_state == ControlState::Disabled)
        return;
    
    if (event.button == MouseButton::Left)
    {
        setState(ControlState::Pressed);
        
        // 计算光标位置
        float x = event.x - getLayoutPaddingLeft() - getLayoutBorderLeft();
        m_cursorPosition = getCharIndexAt(x);
        
        // 如果有选中文本，检查是否点击在选择区域内
        if (!hasSelection() || !isPositionSelected(m_cursorPosition))
        {
            clearSelection();
        }
        
        markDirty();
    }
}

void SInput::onMouseReleased(const MouseEvent& event)
{
    if (m_state == ControlState::Disabled)
        return;
    
    if (event.button == MouseButton::Left)
    {
        if (m_state == ControlState::Pressed)
        {
            setState(ControlState::Focused);
        }
    }
}

void SInput::onMouseMoved(const MouseEvent& event)
{
    if (m_state == ControlState::Disabled || m_state == ControlState::Focused)
        return;
    
    setState(ControlState::Hover);
}

void SInput::onMouseEntered(const MouseEvent& event)
{
    if (m_state == ControlState::Disabled)
        return;
    
    setState(ControlState::Hover);
}

void SInput::onMouseExited(const MouseEvent& event)
{
    if (m_state == ControlState::Disabled)
        return;
    
    if (m_state == ControlState::Hover || m_state == ControlState::Pressed)
    {
        setState(ControlState::Normal);
    }
}

void SInput::onKeyPressed(const KeyEvent& event)
{
    if (m_state == ControlState::Disabled || m_readOnly)
        return;
    
    handleSpecialKey(event);
}

void SInput::onKeyReleased(const KeyEvent& event)
{
    // 处理字符输入
    if (event.codepoint > 0)
    {
        handleCharInput(event.codepoint);
    }
}

// ====================================================================
// 重写绘制函数实现
// ====================================================================

void SInput::render(cairo_t* cr)
{
    if (!cr)
        return;
    
    // 先调用基类的render方法绘制背景、边框等
    SContainer::render(cr);
    
    // 更新光标闪烁状态
    updateCursorBlink();
    
    // 获取绘制区域
    float x = getLeft();
    float y = getTop();
    float width = getLayoutWidth() - getLayoutBorderLeft() - getLayoutBorderRight();
    float height = getLayoutHeight() - getLayoutBorderTop() - getLayoutBorderBottom();
    
    if (width <= 0 || height <= 0)
        return;
    
    // 考虑内边距
    float paddingLeft = getLayoutPaddingLeft();
    float paddingTop = getLayoutPaddingTop();
    float paddingRight = getLayoutPaddingRight();
    float paddingBottom = getLayoutPaddingBottom();
    
    float textAreaX = x + paddingLeft;
    float textAreaY = y + paddingTop;
    float textAreaWidth = width - paddingLeft - paddingRight;
    float textAreaHeight = height - paddingTop - paddingBottom;
    
    if (textAreaWidth <= 0 || textAreaHeight <= 0)
        return;
    
    // 保存Cairo状态
    cairo_save(cr);
    
    // 绘制选择高亮
    if (hasSelection())
    {
        int start = std::min(m_selectionStart, m_selectionEnd);
        int end = std::max(m_selectionStart, m_selectionEnd);
        
        float startX = textAreaX + getTextPositionAt(start);
        float endX = textAreaX + getTextPositionAt(end);
        
        cairo_set_source_rgba(cr, m_selectionColor.r, m_selectionColor.g, m_selectionColor.b, m_selectionColor.a);
        cairo_rectangle(cr, startX, textAreaY, endX - startX, textAreaHeight);
        cairo_fill(cr);
    }
    
    // 绘制文本
    const std::string& text = getText();
    std::string displayText = getDisplayText();
    
    if (displayText.empty())
    {
        // 显示占位符
        if (!m_placeholder.empty())
        {
            cairo_set_source_rgba(cr, m_placeholderColor.r, m_placeholderColor.g, 
                                m_placeholderColor.b, m_placeholderColor.a);
            cairo_select_font_face(cr, getFontFamily().c_str(), 
                                 (getFontStyle() == FontStyle::Italic) ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                                 (getFontWeight() >= FontWeight::Bold) ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, getFontSize());
            
            cairo_move_to(cr, textAreaX, textAreaY + getFontSize());
            cairo_show_text(cr, m_placeholder.c_str());
        }
    }
    else
    {
        // 显示实际文本
        cairo_set_source_rgba(cr, getColor().r, getColor().g, getColor().b, getColor().a);
        cairo_select_font_face(cr, getFontFamily().c_str(), 
                             (getFontStyle() == FontStyle::Italic) ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                             (getFontWeight() >= FontWeight::Bold) ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, getFontSize());
        
        cairo_move_to(cr, textAreaX, textAreaY + getFontSize());
        cairo_show_text(cr, displayText.c_str());
    }
    
    // 绘制光标
    if (m_state == ControlState::Focused && m_cursorVisible)
    {
        float cursorX = textAreaX + getTextPositionAt(m_cursorPosition);
        
        cairo_set_source_rgba(cr, m_cursorColor.r, m_cursorColor.g, m_cursorColor.b, m_cursorColor.a);
        cairo_set_line_width(cr, m_cursorWidth);
        cairo_move_to(cr, cursorX, textAreaY + 2);
        cairo_line_to(cr, cursorX, textAreaY + textAreaHeight - 2);
        cairo_stroke(cr);
    }
    
    // 恢复Cairo状态
    cairo_restore(cr);
}

// ====================================================================
// 私有辅助函数实现
// ====================================================================

void SInput::setState(ControlState newState)
{
    if (m_state == newState)
        return;
    
    // 处理焦点变化
    if (m_state == ControlState::Focused && newState != ControlState::Focused)
    {
        triggerFocusChanged(false);
        clearSelection();
    }
    else if (m_state != ControlState::Focused && newState == ControlState::Focused)
    {
        triggerFocusChanged(true);
    }
    
    m_state = newState;
    updateAppearance();
}

void SInput::updateAppearance()
{
    switch (m_state)
    {
    case ControlState::Normal:
        setBackgroundColor(m_normalBackgroundColor);
        setBorderColor(m_normalBorderColor);
        break;
        
    case ControlState::Hover:
        setBackgroundColor(m_hoverBackgroundColor);
        setBorderColor(m_hoverBorderColor);
        break;
        
    case ControlState::Pressed:
        setBackgroundColor(m_hoverBackgroundColor);
        setBorderColor(m_hoverBorderColor);
        break;
        
    case ControlState::Focused:
        setBackgroundColor(m_focusedBackgroundColor);
        setBorderColor(m_focusedBorderColor);
        break;
        
    case ControlState::Disabled:
        setBackgroundColor(m_disabledBackgroundColor);
        setBorderColor(m_disabledBorderColor);
        break;
    }
    
    markDirty();
}

std::string SInput::getDisplayText() const
{
    const std::string& text = getText();
    
    if (m_inputType == InputType::Password)
    {
        return std::string(text.length(), '*'); // 使用*代替●避免编译错误
    }
    
    return text;
}

void SInput::handleCharInput(unsigned int codepoint)
{
    if (m_state != ControlState::Focused || m_readOnly)
        return;
    
    // 检查是否为可打印字符
    if (codepoint < 32 || codepoint == 127)
        return;
    
    // 输入验证
    char c = static_cast<char>(codepoint);
    
    switch (m_inputType)
    {
    case InputType::Number:
        if (!std::isdigit(c) && c != '-' && c != '.')
            return;
        break;
        
    case InputType::Email:
        // 简单的邮箱验证（实际应该更复杂）
        if (std::isspace(c))
            return;
        break;
        
    case InputType::Tel:
        if (!std::isdigit(c) && c != '+' && c != '-' && c != '(' && c == ')' && c != ' ')
            return;
        break;
        
    default:
        break;
    }
    
    insertText(std::string(1, c));
}

void SInput::handleSpecialKey(const KeyEvent& event)
{
    if (event.mods != 0)
    {
        // 处理组合键
        if (event.mods & 1) // Ctrl键
        {
            switch (event.keyCode)
            {
            case 65: // Ctrl+A
                selectAll();
                return;
            case 67: // Ctrl+C
                copy();
                return;
            case 86: // Ctrl+V
                paste();
                return;
            case 88: // Ctrl+X
                cut();
                return;
            case 90: // Ctrl+Z
                undo();
                return;
            case 89: // Ctrl+Y
                redo();
                return;
            }
        }
    }
    
    switch (event.keyCode)
    {
    case 259: // Backspace
        if (hasSelection())
        {
            deleteSelected();
        }
        else if (m_cursorPosition > 0)
        {
            deleteChar(m_cursorPosition - 1);
        }
        break;
        
    case 261: // Delete
        if (hasSelection())
        {
            deleteSelected();
        }
        else if (m_cursorPosition < static_cast<int>(getText().length()))
        {
            deleteChar(m_cursorPosition);
        }
        break;
        
    case 256: // Escape
        if (m_state == ControlState::Focused)
        {
            setState(ControlState::Normal);
        }
        break;
        
    case 257: // Enter
        if (m_onEnterPressed)
        {
            m_onEnterPressed(getText());
        }
        break;
        
    case 260: // Left
        if (event.mods & 1) // Ctrl+Left (跳过一个单词)
        {
            moveCursor(-1); // 简化实现
        }
        else
        {
            moveCursor(-1);
        }
        break;
        
    case 262: // Right
        if (event.mods & 1) // Ctrl+Right (跳过一个单词)
        {
            moveCursor(1); // 简化实现
        }
        else
        {
            moveCursor(1);
        }
        break;
        
    case 263: // Home
        moveCursorTo(0);
        break;
        
    case 264: // End
        moveCursorTo(static_cast<int>(getText().length()));
        break;
    }
}

void SInput::moveCursor(int delta)
{
    int newPosition = m_cursorPosition + delta;
    moveCursorTo(newPosition);
}

void SInput::moveCursorTo(int position)
{
    int textLength = static_cast<int>(getText().length());
    m_cursorPosition = std::max(0, std::min(position, textLength));
    markDirty();
}

void SInput::selectText(int start, int end)
{
    int textLength = static_cast<int>(getText().length());
    m_selectionStart = std::max(0, std::min(start, textLength));
    m_selectionEnd = std::max(0, std::min(end, textLength));
    markDirty();
}

void SInput::deleteChar(int position)
{
    addToHistory();
    
    const std::string& currentText = getText();
    std::string newText = currentText.substr(0, position) + currentText.substr(position + 1);
    setText(newText);
    m_cursorPosition = position;
    triggerTextChanged();
}

void SInput::insertCharAt(int position, char c)
{
    addToHistory();
    
    const std::string& currentText = getText();
    std::string newText = currentText.substr(0, position) + c + currentText.substr(position);
    setText(newText);
    m_cursorPosition = position + 1;
    triggerTextChanged();
}

bool SInput::isPositionSelected(int position) const
{
    if (!hasSelection())
        return false;
    
    int start = std::min(m_selectionStart, m_selectionEnd);
    int end = std::max(m_selectionStart, m_selectionEnd);
    
    return position >= start && position < end;
}

float SInput::getTextPositionAt(int charIndex) const
{
    std::string displayText = getDisplayText();
    std::string prefix = displayText.substr(0, charIndex);
    
    // 简单的文本宽度估算
    float charWidth = getFontSize() * 0.6f;
    return static_cast<float>(prefix.length()) * charWidth;
}

int SInput::getCharIndexAt(float x) const
{
    std::string displayText = getDisplayText();
    
    // 简单的字符位置估算
    float charWidth = getFontSize() * 0.6f;
    int index = static_cast<int>(x / charWidth);
    
    return std::max(0, std::min(index, static_cast<int>(displayText.length())));
}

void SInput::updateCursorBlink()
{
    if (m_state != ControlState::Focused)
    {
        m_cursorVisible = true;
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastBlinkTime);
    
    if (duration.count() > 500) // 500ms闪烁间隔
    {
        m_cursorVisible = !m_cursorVisible;
        m_lastBlinkTime = now;
        markDirty();
    }
}

void SInput::addToHistory()
{
    if (m_undoHistory.size() >= MAX_HISTORY_SIZE)
    {
        m_undoHistory.erase(m_undoHistory.begin());
    }
    
    m_undoHistory.push_back(getText());
    m_redoHistory.clear();
}

void SInput::triggerTextChanged()
{
    if (m_onTextChanged)
    {
        m_onTextChanged(getText());
    }
}

void SInput::triggerFocusChanged(bool focused)
{
    if (m_onFocusChanged)
    {
        m_onFocusChanged(focused);
    }
}

} // namespace sgui
