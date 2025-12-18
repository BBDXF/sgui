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

## Note
```bash
## 实施时间线

### 第一阶段 (4-6周)

- [ ] Virtual DOM 系统实现
- [ ] QuickJS 集成
- [ ] 基础组件系统
- [ ] 简单示例应用

### 第二阶段 (4-6周)

- [ ] Flexbox 布局引擎
- [ ] Cairo 渲染引擎
- [ ] 基础原生组件
- [ ] 事件处理系统

### 第三阶段 (3-4周)

- [ ] CSS 解析引擎
- [ ] 样式系统完善
- [ ] 高级组件实现
- [ ] 性能优化

### 第四阶段 (2-3周)

- [ ] 调试工具开发
- [ ] 文档和示例
- [ ] 测试完善
- [ ] 发布准备

## 技术风险和解决方案

### 主要风险

1. __性能问题__: Virtual DOM 可能带来性能开销
2. __内存管理__: JavaScript/C++ 内存管理复杂性
3. __布局精度__: Flexbox 在 Cairo 上的精确实现
4. __事件处理__: 跨平台事件一致性

### 解决方案

1. __差分优化__: 实现高效的 diff 算法
2. __智能GC__: 自定义内存管理策略
3. __测试驱动__: 大量布局测试用例
4. __统一抽象__: 事件处理抽象层

## 成功指标

### 功能指标

- [ ] 支持所有基础 React Native 组件
- [ ] 完整的 Flexbox 布局支持
- [ ] CSS 兼容性达到 90%+
- [ ] 应用启动时间 < 2秒

### 性能指标

- [ ] 60fps 流畅渲染
- [ ] 内存使用 < 100MB
- [ ] 组件渲染延迟 < 16ms
- [ ] 热重载时间 < 1秒

这个方案将 sgui 转变为一个现代化的 React Native 风格的 GUI 框架，既保持了原有的 Cairo/GLFW 基础，又引入了现代化的开发体验。

```