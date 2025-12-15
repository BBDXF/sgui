# SGUI
Simple GUI. 虽然叫simple GUI, 但是一点也不简单。

目标:
- 基于 `libdom, libcss, libxml, cairo, glfw` 创建的DirectUI GUI框架。
- 完全基于cairo框架和相关扩展，实现GUI控件的自绘制。
- 支持Flex Layout.
- 支持CSS美化Controls.
- 支持window menagement和builder.
- 实现基本的UI控件，比如`Container, Element, Input, Image, ScrollView`
- 使用C++ Proxy技术抽象class与实现。
- (暂时不实现) 最后，导出所有核心功能接口到C ABI, 方便其他语言调用。

## 规范
- 所有对外ABI的头文件放在include文件夹,如果内部使用，放在`src/internal`。
- 所有源代码分类放在src文件夹下。
- 对于每个主要的模块，都在examples下创建对应的核心测试demo, 方便学习和完善。


## TODO
- [ ] 基于glfw的窗口管理class和Demo
- [ ] 基于`libdom/libcss`实现的控件树和对应demo.
- [ ] 基于Cairo实现的基础Layout和Container,Element.
- [ ] Layout和控件支持CSS
- [ ] 完善的Layout flex/fixed，更多的控件。

