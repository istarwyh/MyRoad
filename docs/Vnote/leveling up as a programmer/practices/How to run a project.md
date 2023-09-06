有时候，启动应用也不简单。
## Node应用
安装 nvm(node version manager)
nvm -v
安装 node:nvm install node node 为最新版的别名
安装 npm(node package manager)
查看 package.json 选择对应方式运行

## Java应用
🤔maven 经常下载不下来某个依赖但是清除缓存再试就好了？
- 非常怀疑其实是有一个找依赖都超时时间，清缓存延长了这个超时时间？
🤔maven 构建的时候会 OOM，默认是多大？给 maven 导入依赖指定多少内存合适？
🤔怎么给 maven 指定导入内存？
🤔项目运行测试 时显示 GC OOM，为什么会出现 这样的 bug？后续删了重来就好了。
🤔在 IDEA 中标记目录是测试目录或其他目录是做了什么？
🤔测试时 build 一直不停止，开启自动编译后显示找不到测试输出目录，之后取消指定测试目录和源目录再赖一边就好了？
🤔测试时显示绑定了多个日志依赖？真的就是一个个排包啊！是不是当初引入日志依赖的都应该加一个 scope 是 provided啊？

