# 断网急救助手 Network Checker
## 功能介绍
本产品灵感来源于 360 断网急救箱，旨在通过利用 Linux 中的常见指令实现对网络环境的简单诊断功能。本产品由 Microsoft Copilot 编写，基于 Gtk 3 框架。
本产品采用 GNU GPL V3 授权
## 使用手册
### 检测流程
同 360 断网急救箱一样，本产品首先会进行硬件连通性的检测，若此项检测未通过则终止检测过程，否则随后将对系统网络配置、DNS、DHCP、HOSTS 以及最终连通性依次进行检测。
检测通过的项目会提示 <span style="color: green;">正常</span>, 否则显示为 <span style="color: red;">异常</span>。
### 依赖
#### 基础依赖
下列工具包需要提前安装，若依赖项目未安装，系统会自动将检测项目<span style="color: orange;">忽略</span>处理。
* ifconfig 命令需要依赖 net-tools 工具包
* nslookup 命令需要安装 dnsutils (Ubuntu/Debian 等发行版) 或 bind-utils (Fedora/RHEL/CentOS 等发行版)
* dhclient 命令需要安装 isc-dhcp-client (Ubuntu/Debian 等发行版) 或 bind-utils (Fedora/RHEL/CentOS 等发行版)
* curl 命令需要通过安装 curl 工具包
#### Gtk 版本
需要系统中内置 Gtk 3 框架。
#### Qt 版本
Qt 版本正在筹划中，敬请期待!
#### NCurses 版本
该版本仍处于开发中，敬请期待。
#### 运行方法
直接运行 network_checker 即可开始检测。
### 已知问题
* 在 Wayland 环境下，程序可能会卡死，随后闪退、断开 Wayland 服务连接。
* NCurses 版本 存在乱码及缺少提示等问题
### 说明
本程序代码目前完全由 Microsoft Copilot 编写，并由本人独立维护，仍有大量维护改进空间。
时刻欢迎对此项目感兴趣的所有人加入到维护和开发中！
