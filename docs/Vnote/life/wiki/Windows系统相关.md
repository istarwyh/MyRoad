## 背景知识
- BIOS：Basic Input Output System，主要功能是为计算机提供最底层的、最直接的硬件设置和控制。
- x86:1978年6月8日，Intel发布了新款16位CPU“8086”，诞生x86架构，32位指令集（一套通用的计算机指令集合）。一般可认为这是“CPU+Linux”操作系统的架构，几乎可认为目前市面上所有主流服务器都是基于x86架构的CPU公版二次开发而来。
- X86_64、x64、AMD64:x86指令集的64位扩展

- 64位：CPU有64位整数寄存器和64位指针，便于单一程序使用更大内存。

- ARM64：移动端CPU架构，如高通骁龙系列。

- WinPE: Windows Preinstallation Environment, 可认为是简化版的自定义windows。

- FAT32：老格式

- NTFS:新版本分区格式，单个储存文件可大于4G；可以控制超过32G的文件系统。但是ntfs是日志式系统（就是干个啥都要写日志），不适合在U盘上使用。

- exFAT：微软专门推给U盘用的，但是比较新兼容性不太好。如果使用exFAT，则该启动盘将只能支持传统BIOS启动模式，无法支持UEFI模式启动

- UEFI：Unified Extensible Firmware Interface，这种接口用于操作系统自动从预启动的操作环境加载到一种操作系统上，被看做是BIOS的继任者。

## 准备工作

1.wi+S输入“系统信息”。传统对应BIOS，UEFI对应EFI启动模式。 
2.先[查阅](https://mp.weixin.qq.com/s?__biz=MTI2NzIyNzM0MQ==&mid=2650529491&idx=1&sn=0924942a434d41da503e913c6553d65b&chksm=7bb8d3714ccf5a67cf70796c050d927749cba469e7b119fc50fa961b5b1238ee715b448c30a1&scene=21#wechat_redirect)自己主板怎么进入BIOS，一般是看到开机logo就反复按f12键（大多台湾电脑）或del键或Esc键

### 原材料

ISO文件，微软官方或 http://msdn.itellyou.cn/

### USB介质

可选工具软件：WinUSB、软媒安装大师、软碟通、U盘装机大师、Etcher etc

## 应用

### 1.制作引导U盘

代码制作：

```shell
CMD Diskpart>List disk
Select disk U盘 
Clean 
Creat partition primary 
Select partition 1 
Active Format fs=exfat quick 
Assign 
Exit
```

之后将下载好的ISO系统镜像文件解压缩，然后全都拷贝到U盘里即可。如果U盘的图标变成了系统安装盘的图片就代表制作引导U盘成功！

### 2.WinPE维护：

1. 下载iso文件后，打开“虚拟光驱\sources”，其中的boot.wim即为WinPE系统核心文件，install.wim为Windows 10安装核心文件；

2. U盘格式化为FAT32格式，接着将虚拟光驱下加载的所有文件全部复制到U盘。此时因为EFI目录的存在，WinPE支持BIOS和UEFI双重启动

3. 重新启动电脑进入BIOS设置，将U盘UEFI启动设置为第一启动，这样即可从U盘启动到WinPE系统了。

4. 在出现安装界面时按下Shift+F10调出命令提示符界面，启动命令提示符后既可以进行常规的维护操作了，比如可以在其中启动记事本，使用“文件→打开”，使用这里的GUI界面进行文件复制、提取等操作，可以使用`Dism`命令进行系统重装或者恢复等

### 3.WinUSB

1.软件运行后提示插入并选择USB介质（Plug and select the USB drive）

2.随后点击软件右侧Add栏内的ISO光盘图标按钮，依次选择添加若干不同版本的Windows ISO文件（如果你需要制作多系统安装盘的话）

3.细心按提示操作即可

## 安装启动

可参考[10分钟学会装系统!](https://mp.weixin.qq.com/s?__biz=MTI2NzIyNzM0MQ==&mid=2650529881&idx=1&sn=2453f44d64b82491a0eecb3016dc06bf&chksm=7bb8edfb4ccf64ed9ea68bca186ab0656bb9ff6bb6b39180387aaaaf5b9802113502a1266052&scene=21#wechat_redirect)： 
1.进入BIOS后，Main->SATA Mode->IDE（新机器）or ACHI（老机器）—>enter+左右键->Boot->使emovable Devices或USB Devices优先级最高（f6+/f5-）or USB HDD->f10保存

2.进入Exit,并Exit Saving Changes

3.插入启动优盘->进入BIOS->选择USB启动

P.S.

如果没有特殊的需求，硬盘是完全[没必要分区](https://mp.weixin.qq.com/s?__biz=MTI2NzIyNzM0MQ==&mid=2650529002&idx=2&sn=6ab6dd021ece93f0045faa965a52849a&chksm=7bb8d1484ccf585e59d8f80392f523e2f0a8aeefa59cffa455c3ee50b732664255bada79dbd1&scene=21#wechat_redirect)的,转而使用库——虚拟文件夹（默认关闭，需查看-显示库）效率更高。

## Windows安装后
### 获取系统信息
FstFetch

### 1.Windows初始优化

- 任务管理器->关闭某些启动项
- 进入计算器服务关闭 `Superfetch`、`HomeGroup Listener`、`HomeGroup Provider` 这三项以及其他不必要的。建议Windows update开启，可在更新高级选项中略作调整。
- `dism++`根据自己使用习惯调整

常用内置工具或命令：
```shell

gpedit.msc 本地组策略编辑器-注册表的文字化编辑

Services.msc 本地服务

Sxstrace.exe 追踪程序生成日志

Regedit.exe 打开注册表

Inetcpl.cpl 打开Internet选项


ping 127.0.0.1 检查本地TCP/IP协议安装

chkdsk i: f 修复i盘

netstat -aon|findstr "8080" 查看占用端口

tasklist|findstr "5528" 根据PID查看进程

taskkill /pid xxxx /f
```
### 2.日常垃圾清理

- 在磁盘分区上鼠标右键点击“属性”，选择磁盘清理，对于日常没有权限删除的系统类文件夹可以通过“清理系统文件”的方式处理。在“设置-系统-存储”中，可以开启“存储感知”功能，让电脑自动删除“回收站”和“UWP应用”中存储的无用文件，释放磁盘空间。

• [windows文件夹清理](https://zhidao.baidu.com/question/325212639.html)

• [winsys文件夹清理](https://msdn.microsoft.com/zh-cn/library/windows/hardware/dn898501%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)(官方并不建议清理)

- wise care
- iobit installer

### 3.设置好全自动的任务计划

此电脑-管理-创建（基本）任务

### 4.细节决定舒适

#### 更干净的"此电脑"
[“此电脑”太乱 删掉就好](http://www.cfan.com.cn/2017/0831/129267.shtml):

##### 删除3D对象这x种文件夹

cmd->regedit定位到

```bash
HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace

```

1. 对着“NameSpace”项鼠标右键单击，选中“导出”，先将注册表项备份出来.
2. 删除下列项
```
{088e3905-0323-4b02-9826-5d99428e115f}下载文件夹 
{3dfdf296-dbec-4fb4-81d1-6a3438bcf4de}音乐文件夹 
{d3162b92-9365-467a-956b-92703aca08af}文档文件夹 
{0DB7E03F-FC29-4DC6-9020-FF41B59E513A}3D对象文件夹
```
##### 删除百度云这种盘符


同上定位

```bash
计算机\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace
```

删除这种对应的项即可
	![](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/ceda00a8-c246-47ec-b79e-9d4eecb65418/.1556104434726.png)
##### 关闭windows系统广告
- OFGB

#Command 