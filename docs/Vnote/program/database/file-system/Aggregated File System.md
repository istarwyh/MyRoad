## AGFS

在 Unix 的设计哲学里，有一条核心原则：Everything is a file。设备、进程、网络连接，都可以通过文件接口来访问。

Linux 的 VFS（虚拟文件系统）也是这个思路：底层不同的文件系统通过一套抽象的 VFS 接口，把能力通过标准接口往上提供。

AGFS 延续了这个理念：Unix Pipeline + File System ，能够以最小的代价组合出最多的可能性。只要把工具通过文件系统接口提供，就可以直接串联到 bash 上面。

## Context、Context File System、Memory 三者的关系

传统文件系统有两个缺陷：

1. 1. 只适合非结构化数据：唯一的结构就是文件夹目录
2. 2. 难以支持消息投递：Agent A 想给 Agent B 发消息，在传统文件系统上实现起来非常别扭

现在各种各样的新服务、新框架、新工具超级多。每一个单看都挺好的，但如果把自己当做一个 AI 去想象一下：手头上这么多武器，每一个要用还得学习一遍。所以有没有办法能够有一个 unified 的 interface 去统一所有这些？

比如 Manus 的 Wide Research 功能，应该给 Agent 一个 Meta Tool，而不是一个个具体的工具。与其给它一个叫 Wide Research 的 Tool，不如给它文件系统 + bash + 命令行，让它自己组合出用户要的东西。

### 虚拟文件系统

### Redis 作为文件系统
