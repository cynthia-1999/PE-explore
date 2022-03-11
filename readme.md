PEviews 长期没有更新，且不支持 64 位，设计和实现一款 PE 文件浏览器，支持32位和64位。
1) 可以像 PEview 一样(图形界面)浏览所有 PE 结构，包括但不限于 header,section table, section。
2) 浏览(修改)每一节的权限。
3) 浏览编译器支持的安全指示，如 ASLR，DEP，stackguard, CFG 等。
4) 浏览和修改 IAT、EAT、重定位表、资源节、异常表、证书表。
5) 对其中关键字段可以修改，如 ImageBase, file alignment, section alignment等。
6) 验证 PE 文件的签名。
7) 支持代码节的反汇编，与重定位表一起，显示代码中的重定位项(可选)。
8)支持对任意进程的任意函数进行hooking or in line hooking，Hooking后的shellcode 支持弹出 CMD、或计算器、或一组文本字符串消息。
9) 支持对任意 PE 文件进行 Shellcode 注入，shellcode 支持弹出 CMD、或计算器、或一组文本字符串消息。
