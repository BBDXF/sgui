/**
 * CSS 到 Key-Value 转换器
 * 
 * 这个程序展示如何将 KatanaOutput 解析为 key-value 格式
 * 支持：
 * - 选择器到属性的映射
 * - CSS 属性的提取和格式化
 * - 媒体查询的处理
 * - @规则的解析
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

extern "C" {
#include "katana.h"
}

class CSSKeyValueConverter {
private:
    std::map<std::string, std::map<std::string, std::string>> cssRules;
    std::vector<std::string> mediaQueries;
    std::map<std::string, std::string> atRules;

    // 工具函数：将 KatanaValue 转换为字符串
    std::string valueToString(const KatanaValue* value) {
        if (!value) return "";
        
        std::ostringstream oss;
        
        switch (value->unit) {
            case KATANA_VALUE_STRING:
            case KATANA_VALUE_IDENT:
            case KATANA_VALUE_URI:
                oss << value->string;
                break;
            case KATANA_VALUE_NUMBER:
                if (value->isInt) {
                    oss << value->iValue;
                } else {
                    oss << value->fValue;
                }
                break;
            case KATANA_VALUE_PX:
                oss << (value->isInt ? value->iValue : value->fValue) << "px";
                break;
            case KATANA_VALUE_EMS:
                oss << (value->isInt ? value->iValue : value->fValue) << "em";
                break;
            case KATANA_VALUE_PERCENTAGE:
                oss << (value->isInt ? value->iValue : value->fValue) << "%";
                break;
            case KATANA_VALUE_RGBCOLOR:
                oss << "rgb(" << value->string << ")";
                break;
            case KATANA_VALUE_PARSER_FUNCTION:
                if (value->function) {
                    oss << value->function->name << "(";
                    for (unsigned int i = 0; i < value->function->args->length; i++) {
                        if (i > 0) oss << ", ";
                        KatanaValue* arg = (KatanaValue*)value->function->args->data[i];
                        oss << valueToString(arg);
                    }
                    oss << ")";
                }
                break;
            case KATANA_VALUE_PARSER_LIST:
                if (value->list) {
                    for (unsigned int i = 0; i < value->list->length; i++) {
                        if (i > 0) oss << " ";
                        KatanaValue* item = (KatanaValue*)value->list->data[i];
                        oss << valueToString(item);
                    }
                }
                break;
            default:
                if (value->raw) {
                    oss << value->raw;
                }
                break;
        }
        
        return oss.str();
    }

    // 工具函数：将选择器转换为字符串
    std::string selectorToString(const KatanaSelector* selector) {
        if (!selector) return "";
        
        std::string result;
        
        // 处理标签选择器
        if (selector->tag && selector->tag->local) {
            result += selector->tag->local;
        }
        
        // 处理类选择器
        if (selector->match == KatanaSelectorMatchClass && selector->data && selector->data->value) {
            result += "." + std::string(selector->data->value);
        }
        
        // 处理ID选择器
        if (selector->match == KatanaSelectorMatchId && selector->data && selector->data->value) {
            result += "#" + std::string(selector->data->value);
        }
        
        // 处理属性选择器
        if (selector->match >= KatanaSelectorMatchFirstAttribute && 
            selector->match <= KatanaSelectorMatchAttributeEnd &&
            selector->data && selector->data->attribute && selector->data->attribute->local) {
            
            result += "[" + std::string(selector->data->attribute->local);
            
            if (selector->data->value) {
                switch (selector->match) {
                    case KatanaSelectorMatchAttributeExact:
                        result += "=\"" + std::string(selector->data->value) + "\"";
                        break;
                    case KatanaSelectorMatchAttributeContain:
                        result += "*=\"" + std::string(selector->data->value) + "\"";
                        break;
                    case KatanaSelectorMatchAttributeBegin:
                        result += "^=\"" + std::string(selector->data->value) + "\"";
                        break;
                    case KatanaSelectorMatchAttributeEnd:
                        result += "$=\"" + std::string(selector->data->value) + "\"";
                        break;
                    default:
                        break;
                }
            }
            
            result += "]";
        }
        
        // 处理伪类和伪元素
        if (selector->pseudo != KatanaPseudoNotParsed && selector->pseudo != KatanaPseudoUnknown) {
            if (selector->pseudo < 100) {
                // 标准伪类/伪元素
                const char* pseudoNames[] = {
                    "empty", "first-child", "first-of-type", "last-child", "last-of-type",
                    "only-child", "only-of-type", "first-line", "first-letter", "nth-child",
                    "nth-of-type", "nth-last-child", "nth-last-of-type", "link", "visited",
                    "hover", "focus", "active", "checked", "disabled", "enabled", "first",
                    "last", "only", "root", "after", "before", "selection", "not"
                };
                
                if (selector->pseudo < sizeof(pseudoNames)/sizeof(pseudoNames[0])) {
                    if (selector->pseudo < KatanaPseudoFirstLine) {
                        result += ":" + std::string(pseudoNames[selector->pseudo]);
                    } else {
                        result += "::" + std::string(pseudoNames[selector->pseudo - KatanaPseudoFirstLine]);
                    }
                }
            }
        }
        
        // 递归处理组合选择器
        if (selector->tagHistory) {
            std::string parent = selectorToString(selector->tagHistory);
            if (!parent.empty()) {
                switch (selector->relation) {
                    case KatanaSelectorRelationChild:
                        result = parent + " > " + result;
                        break;
                    case KatanaSelectorRelationDescendant:
                        result = parent + " " + result;
                        break;
                    case KatanaSelectorRelationDirectAdjacent:
                        result = parent + " + " + result;
                        break;
                    case KatanaSelectorRelationIndirectAdjacent:
                        result = parent + " ~ " + result;
                        break;
                    case KatanaSelectorRelationSubSelector:
                    default:
                        result = parent + result;
                        break;
                }
            }
        }
        
        return result;
    }

    // 处理样式规则
    void processStyleRule(const KatanaStyleRule* styleRule) {
        if (!styleRule) return;
        
        // 处理选择器
        std::vector<std::string> selectors;
        if (styleRule->selectors) {
            for (unsigned int i = 0; i < styleRule->selectors->length; i++) {
                KatanaSelector* selector = (KatanaSelector*)styleRule->selectors->data[i];
                std::string selectorStr = selectorToString(selector);
                if (!selectorStr.empty()) {
                    selectors.push_back(selectorStr);
                }
            }
        }
        
        // 处理声明
        std::map<std::string, std::string> properties;
        if (styleRule->declarations) {
            for (unsigned int i = 0; i < styleRule->declarations->length; i++) {
                KatanaDeclaration* decl = (KatanaDeclaration*)styleRule->declarations->data[i];
                if (decl && decl->property) {
                    std::string value;
                    if (decl->values) {
                        for (unsigned int j = 0; j < decl->values->length; j++) {
                            if (j > 0) value += " ";
                            KatanaValue* val = (KatanaValue*)decl->values->data[j];
                            value += valueToString(val);
                        }
                    }
                    
                    if (decl->important) {
                        value += " !important";
                    }
                    
                    properties[std::string(decl->property)] = value;
                }
            }
        }
        
        // 将属性应用到所有选择器
        for (const auto& selector : selectors) {
            for (const auto& prop : properties) {
                cssRules[selector][prop.first] = prop.second;
            }
        }
    }

    // 处理 @import 规则
    void processImportRule(const KatanaImportRule* importRule) {
        if (!importRule) return;
        
        std::string rule = "@import";
        if (importRule->href) {
            rule += " url(" + std::string(importRule->href) + ")";
        }
        
        if (importRule->medias && importRule->medias->length > 0) {
            rule += " ";
            for (unsigned int i = 0; i < importRule->medias->length; i++) {
                if (i > 0) rule += ", ";
                // 处理媒体查询（简化版）
                KatanaMediaQuery* media = (KatanaMediaQuery*)importRule->medias->data[i];
                if (media && media->type) {
                    rule += std::string(media->type);
                }
            }
        }
        
        atRules["import"] = rule;
    }

    // 处理 @media 规则
    void processMediaRule(const KatanaMediaRule* mediaRule) {
        if (!mediaRule) return;
        
        std::string mediaQuery = "@media";
        if (mediaRule->medias && mediaRule->medias->length > 0) {
            mediaQuery += " ";
            for (unsigned int i = 0; i < mediaRule->medias->length; i++) {
                if (i > 0) mediaQuery += ", ";
                KatanaMediaQuery* media = (KatanaMediaQuery*)mediaRule->medias->data[i];
                if (media && media->type) {
                    mediaQuery += std::string(media->type);
                }
            }
        }
        
        mediaQueries.push_back(mediaQuery);
        
        // 处理媒体查询内部的规则
        if (mediaRule->rules) {
            for (unsigned int i = 0; i < mediaRule->rules->length; i++) {
                KatanaRule* rule = (KatanaRule*)mediaRule->rules->data[i];
                if (rule && rule->type == KatanaRuleStyle) {
                    KatanaStyleRule* styleRule = (KatanaStyleRule*)rule;
                    processStyleRule(styleRule);
                }
            }
        }
    }

public:
    // 解析 CSS 并转换为 key-value 格式
    void parseCSS(const char* css) {
        if (!css) return;
        
        KatanaOutput* output = katana_parse(css, strlen(css), KatanaParserModeStylesheet);
        if (!output || !output->stylesheet) {
            std::cerr << "解析 CSS 失败" << std::endl;
            return;
        }
        
        // 清空之前的数据
        cssRules.clear();
        mediaQueries.clear();
        atRules.clear();
        
        // 处理所有规则
        if (output->stylesheet->rules.data) {
            for (unsigned int i = 0; i < output->stylesheet->rules.length; i++) {
                KatanaRule* rule = (KatanaRule*)output->stylesheet->rules.data[i];
                if (!rule) continue;
                
                switch (rule->type) {
                    case KatanaRuleStyle:
                        processStyleRule((KatanaStyleRule*)rule);
                        break;
                    case KatanaRuleImport:
                        processImportRule((KatanaImportRule*)rule);
                        break;
                    case KatanaRuleMedia:
                        processMediaRule((KatanaMediaRule*)rule);
                        break;
                    case KatanaRuleFontFace:
                        atRules["font-face"] = "@font-face";
                        break;
                    case KatanaRuleCharset:
                        if (((KatanaCharsetRule*)rule)->encoding) {
                            atRules["charset"] = "@charset '" + std::string(((KatanaCharsetRule*)rule)->encoding) + "'";
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        
        katana_destroy_output(output);
    }

    // 打印 key-value 格式的结果
    void printKeyValueFormat() {
        std::cout << "\n=== CSS Key-Value 格式输出 ===\n" << std::endl;
        
        // 打印普通规则
        if (!cssRules.empty()) {
            std::cout << "【CSS 规则】" << std::endl;
            for (const auto& rule : cssRules) {
                std::cout << "选择器: " << rule.first << std::endl;
                for (const auto& prop : rule.second) {
                    std::cout << "  " << prop.first << ": " << prop.second << std::endl;
                }
                std::cout << std::endl;
            }
        }
        
        // 打印媒体查询
        if (!mediaQueries.empty()) {
            std::cout << "【媒体查询】" << std::endl;
            for (const auto& query : mediaQueries) {
                std::cout << query << std::endl;
            }
            std::cout << std::endl;
        }
        
        // 打印 @规则
        if (!atRules.empty()) {
            std::cout << "【@ 规则】" << std::endl;
            for (const auto& rule : atRules) {
                std::cout << rule.first << ": " << rule.second << std::endl;
            }
            std::cout << std::endl;
        }
    }

    // 获取指定选择器的属性
    std::map<std::string, std::string> getProperties(const std::string& selector) {
        auto it = cssRules.find(selector);
        if (it != cssRules.end()) {
            return it->second;
        }
        return {};
    }

    // 获取指定选择器的特定属性值
    std::string getPropertyValue(const std::string& selector, const std::string& property) {
        auto props = getProperties(selector);
        auto it = props.find(property);
        if (it != props.end()) {
            return it->second;
        }
        return "";
    }

    // 获取所有选择器
    std::vector<std::string> getAllSelectors() {
        std::vector<std::string> selectors;
        for (const auto& rule : cssRules) {
            selectors.push_back(rule.first);
        }
        return selectors;
    }
};

int main() {
    CSSKeyValueConverter converter;
    
    std::cout << "CSS 到 Key-Value 转换器演示\n" << std::endl;
    
    // 示例 CSS
    const char* css = R"(
/* 基础样式 */
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background-color: #f0f0f0;
}

.container {
    width: 100%;
    max-width: 1200px;
    margin: 0 auto;
}

.button {
    display: inline-block;
    padding: 10px 20px;
    background: linear-gradient(45deg, #007bff, #0056b3);
    color: white;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    transition: all 0.3s ease;
}

.button:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 8px rgba(0,0,0,0.2);
}

@import url('reset.css');

@media (min-width: 768px) {
    .container {
        padding: 0 15px;
    }
    .button {
        font-size: 16px;
    }
}

@font-face {
    font-family: 'MyFont';
    src: url('myfont.woff2');
}
    )";
    
    std::cout << "输入 CSS:" << std::endl;
    std::cout << css << std::endl;
    
    // 解析 CSS
    converter.parseCSS(css);
    
    // 打印 key-value 格式结果
    converter.printKeyValueFormat();
    
    // 演示如何查询特定属性
    std::cout << "=== 属性查询演示 ===\n" << std::endl;
    
    std::vector<std::string> testSelectors = {"body", ".container", ".button"};
    for (const auto& selector : testSelectors) {
        std::cout << "查询选择器: " << selector << std::endl;
        auto props = converter.getProperties(selector);
        if (!props.empty()) {
            std::cout << "找到 " << props.size() << " 个属性:" << std::endl;
            for (const auto& prop : props) {
                std::cout << "  " << prop.first << ": " << prop.second << std::endl;
            }
        } else {
            std::cout << "未找到该选择器的属性" << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 演示获取特定属性值
    std::cout << "特定属性值查询:" << std::endl;
    std::cout << "body 的 font-family: " << converter.getPropertyValue("body", "font-family") << std::endl;
    std::cout << ".button 的 background: " << converter.getPropertyValue(".button", "background") << std::endl;
    std::cout << ".container 的 max-width: " << converter.getPropertyValue(".container", "max-width") << std::endl;
    
    return 0;
}
