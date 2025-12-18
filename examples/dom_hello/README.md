# libcss和libdom Demo程序

## libcss和libdom库介绍

### libcss
libcss是NetSurf项目中的一个CSS解析和选择器引擎库，主要功能包括：
- CSS样式表的解析
- CSS选择器的匹配和计算
- 样式级联和继承处理
- CSS属性的计算和解析

### libdom  
libdom是NetSurf项目中的DOM实现库，主要功能包括：
- DOM文档树的构建和操作
- DOM事件处理
- HTML/XML文档的解析
- DOM API的实现（类似浏览器中的DOM接口）

### Demo程序
本demo展示了如何使用libcss和libdom库来：
1. 解析HTML文档并构建DOM树
2. 解析CSS样式表
3. 将CSS样式应用到DOM元素
4. 遍历DOM树并显示元素及其样式信息

## 编译和运行

```bash
mkdir build && cd build
cmake ..
make
./bin/dom_hello
