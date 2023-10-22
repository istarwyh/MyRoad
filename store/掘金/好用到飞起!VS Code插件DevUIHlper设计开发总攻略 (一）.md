# 1. 好用到飞起!VS Code插件DevUIHlper设计开发总攻略 (一）
## 1.1. 开发背景
为了提升[DevUI组件](https://devui.design/home)对于使用者的易用性:

1. 针对DevUI组件库目前只有插入代码块的插件，用户可在html文件中输入关键字生成代码块
例如输入`d-button`，vscode会自动联想，选中后会出现代码块：
`<d-button (btnClick)=‘’> </d-button>`[^snippets]
[^snippets]:[Snippets in VSCode](https://code.visualstudio.com/docs/editor/userdefinedsnippets)

2. 避免在使用组件的过程中不停地切换到组件库网站查询Demo和API
即提高开发者写出`<d-button type=‘‘primary‘’></d-button>`的效率,当写完后鼠标悬浮到type上时，应该出现type的相关描述.
而类型，默认值与说明，应与网站中API页面type的描述保持一致.拆分一下可以有这两个需求:
- 自动补全,输入`<d-`这样的**组件标签**,可以提示`API`并补全,同时可以提示**参数**并补全.
- 对已有代码进行相关悬浮提示:用户悬停到关键字上，显示相关的描述.


目前似乎许多UI库还没有实现将api文档从网站搬到代码中,我们或许可以实现"**api文档赋能页面**"。基于此可以开发一款基于VS Code的组件库开发助手插件，起名`DevUIHelper`。
## 1.2. 开发概述
### 1.2.1. 功能设想
原始版本功能设想如下,鱼骨图靠近鱼头的部分意味着优先级较高,应优先开发。
![](https://gitee.com/istarwyh/images/raw/master/1587179601_20200417234150557_1578.png)
做着做着伴随着解bug,新想法又冒出来:依赖查找报警/补全时图片提示/提供GUI的自定义图片....

### 1.2.2. 已有成果展示
![small](https://gitee.com/istarwyh/images/raw/master/1592892269_20200623140417156_10776.gif)
### 1.2.3. 怎么实现的?
本插件站在了[前辈](https://www.cnblogs.com/liuxianan/p/vscode-plugin-overview.html)的[基础](https://github.com/bauhausr/antd-intellisense)上,以及参考了Angular Language Service插件的解析器部分进行开发,总的逻辑是**获取素材+合适工具=功能实现**.插件开发细节如`函数注册`/`package配置`等请见前辈文章,非常感谢他们的分享!
#### 1.2.3.1. 各版本思路

- **1.0**版本思路:通过字符串匹配查找导出的素材模块,在合适的时候调用对应的api完成功能
- **2.0**版本思路:面向对象封装功能与参数,以便后续扩展,并集成素材模块,重构其形式,为3.0资源模块对象化做铺垫
- **3.0**版本思路:引入Server端的`LSP`,对标签句经过词法/语法/语义分析生成`AST`代替原本的正则匹配,并将原本的Local API替换为Server API
- **4.0**版本思路:面向接口编程进一步降低各模块耦合度,优化`parser`,尝试新增对指令的全功能支持以及自定义命令实现快速开发等

*题外话:工具怎么找到呢?事先学到了是一个, 读文档看API是一个,读源码溯源是一个.*

#### 1.2.3.2. 目前总架构
目前的架构如下图所示：
![](https://gitee.com/istarwyh/images/raw/master/1592675751_20200621010623425_20952.png)

1. **控制层** :以`server.ts`为入口，通过LSP提供的TCP连接建立与语言服务器的连接以便`HOST`统筹调用功能API，同时激活（初始化）`Parser`与资源模块，一个依据源代码生成抽象语法树，一个依据根据API文档对应生成的资源文件（`.json`等格式）生成资源树。

2. **模型层**:之后`Hunter`节点树与资源树之间架起一座桥梁，一方面从抽象语法树中得到节点信息后去资源树中查询对应资源，一方面也采用缓存的办法提高下一次的查询效率。

3. **业务层**:得到的节点信息如`Position`和对应资源（可能还需要处理资源）作为参数传入LSP提供的功能API，进而实现对应功能。

总体上架构未离开**获取素材+合适工具=功能实现**的总逻辑，不过值得注意`Hunter`类的存在解决了节点资源树一同刷新的问题，也赋予了两者各自更多的可能性。这也正是高内聚低耦合的好处。
## 1.3. 功能开发
### 1.3.1. 获取素材
为供api调用,预计网络传输开销大于本地预处理,因此素材可

- 爬虫结果处理后得到素材
- 素材文档在本地由`Java`处理后导出成module模块
- `node_modules`中模块中已集成

素材示例:
```ts
export const HTML_SCHEMA=[
"accordion||这是一个accordion组件",
"data||Array<any>或AccordionMenuType||null||数据源||必选，数据源，可以自定义数组或者使用预设的AccordionMenuType||true||false||[]",
"titleKey||string||title||标题的属性名||可选，标题的属性名，item[titleKey]类型为string，为标题显示内容||false||false||[]",
"loadingKey||string||loading||子菜单是否加载中的判断属性名||可选，子菜单是否加载中的判断属性名，item[loadingKey]类型为boolean||false||false||[]",
"childrenKey||string||children||子菜单的属性名||可选，子菜单的属性名，item[childrenKey]类型为Array<any>||false||false||[]"
]
```
如果api文档不够规范影响素材处理,可与组件团队协调解决或者建议团队采用`Angular doc`这样的业界规范。而为了解决版本化的问题，最好的方案则是将素材模块集成入对应组件库的`node_modules`中,也可以考虑产品自带模块处理脚本。
### 1.3.2. 使用工具
#### 1.3.2.1. 正则表达式
正则表达式(regular expression)描述了一种字符串匹配的模式（pattern），可以用来检查一个串是否含有某种子串、将匹配的子串替换或者从某个串中取出符合某个条件的子串等。
对于组件的特征,如

- DevUI的组件的`d-button`的`d-`,Antdesign的`Button`大写的`B`

以及引入的模块

- `import { DevUIModule } from 'ng-devui'`

开发过程中的操作

- 敲空格,`[]`(属性),`()`(事件)等

都可以作为正则匹配条件来实现条件限制或触发,代码示例:
```ts
//devui的使用以d-开头,如d-button,以此做触发
const componentRegex = /<(d-[a-zA-Z0-9-]*)\b[^<>]***/g;
// 匹配"",而不是""和空格或者""和>,以此做""不触发而""外可触发的条件限制
const attributeValue= /^=\"[\s\S]*\"(?! |(>)\1)/;
```

#### 1.3.2.2. 调用API
通过看vscode自己的API文档[^api],可以打开实现各种功能的新世界。有可能得耐心看,可以逐层了解接口与返回值,以`registerHoverProvider`为例:
[^api]:[Programmatic Language Features](https://code.visualstudio.com/api/language-extensions/programmatic-language-features)


![](https://gitee.com/istarwyh/images/raw/master/1592897428_20200623151611056_18717.png)
#### 1.3.2.3. [Language Server Protocol](https://code.visualstudio.com/api/language-extensions/language-server-extension-guide)
##### 1.3.2.3.1. 什么是LSP?
微软推出LSP，意图标准化语言工具和代码编辑器之间的通信:
![2](https://code.visualstudio.com/assets/api/language-extensions/language-server-extension-guide/lsp-languages-editors.png)
其实在Angular框架和Jdk中都自带了`LSP`的实现.在VS Code中，语言服务器包含两个部分：

- 语言客户端：用`JavaScript `/ `TypeScript`编写的普通VS Code扩展。该扩展可以访问所有VS Code命名空间API。
- 语言服务器：在单独的进程中运行的语言分析工具。


依托于**语言或语言工具开发者提供的**素材库/符合规范的接口,只要C／S之间建立起可靠的连接(如`TCP`),**Language Server**就可以执行静态程序分析为指定特征代码(如组件库代码)或全部代码建立抽象语法树，来得到`LSP`所提供API的接口所需的参数,代码编辑器便可以实现Client端包括自动补全、悬浮提示和自动纠错在内的许多功能.

##### 1.3.2.3.2. 为什么利用AST?
抽象语法树是程序源代码结构的树状表示。程序源代码经过词法分析器（`Lexer`）得到各种不同种类的单词（`Token`），再由语法分析器（`Parser`[^parser]）分析和语法检查后得到抽象语法树(`AST`)。对于如下`C`语言代码：
```c
while(i < n){  
    sum += A[i++];
}
```
可以生成如图的AST结构：
![AST2](https://gitee.com/istarwyh/images/raw/master/1593424935_20200629180207233_47.png)
通常AST的根节点表示整个程序，内部节点是抽象语法结构或者单词。AST的核心在于它能与输入源代码中的各个语法元素**一一对应**。正好事实上,LSP的大部分请求都是在表达"**在指定位置执行规定动作**"---这意味着插件开发者只需要考虑何时触发以及何处触发指定操作即可,因此常见的参数即

- 文件的`URI`
- 文件的`change.document`
- 单词的`position`

语法树便可以**准确地**提供`Token`(包括单词)的`position`.这正是通过正则表达式的匹配无法达到的优点。

当然,当深入语言的编译原理时,其实也可以创建自己的语言定义了,想象空间十分大.

[^parser]:parser除了转化程序文本成AST这样的数据结构，也有用于处理 CSV，JSON，XML 之类的格式。

##### 1.3.2.3.3. 引入Server端的LSP[^LSPGuide]
[^LSPGuide]:[Language Server Extension Guide](https://code.visualstudio.com/api/language-extensions/language-server-extension-guide)

当在server文件夹的`package.json`中引入`vscode-languageserver`:
```
"dependencies": {
    "vscode-languageserver": "^4.1.3"
}
```
之后开发者就可以使用server的API了.
##### 1.3.2.3.4. LSP类插件基本结构
```
.
├── client // Language Client: able to use vscode API
│   ├── src
│   │   ├── test // End to End tests for Language Client / Server
│   │   └── extension.ts // Language Client entry point
├── package.json // The extension manifest
└── server // Language Server
    └── src
        └── server.ts // Language Server entry point
```
#### 1.3.2.4. 组件库的语法树
如前对AST的介绍，`html`/`css`/`ts`/`.d.ts`的文件都可以解析成语法树。以html文件为例,可以将每一级标签作为一个`DOM`节点，组件语句则经过词法/语法/语义分析打碎成`token`。
比如对`<d-button bsStyle="primary"></button>`分割成

- **标签**(如`d-button`)
- **api**(如`style`)
- **属性**(如`primary`)
- **符号**(如`=""`),

而后对每一分割的元素周围划定识别的范围(`keySpan`),压栈后经序列化最终生成**AST语法树**。如对下列使用了`DevUI`组件库的代码：
```html
<div class="main">
  <div class="left">
    <d-accordion 
        [data]="menu" 
        class="menu" 
        [restrictOneOpen]="restrictOneOpen" 
        (itemClick)="itemClick(**event)"
        (menuToggle)="menuToggle(**event)">
    hello
    </d-accordion>
  </div>
  <div class="content">
    <app-table></app-table>
  </div>
</div>
```
本插件生成的AST可部分表示为：
![AST1](https://gitee.com/istarwyh/images/raw/master/1593424713_20200629174235034_12724.png)
>不过必须**注意**,类似的工作已经有[posthtml](https://posthtml.org/)这样的工具做了[^posthtml],并且如果要解析`markdown`之类也有对应的库,自己在开发之前"不要重复造轮子”.

[^posthtml]: 但是`posthtml`转成的节点树对于位置信息不敏感,即上述的`keySpan`.因此本项目选择了重写`htmlparser`,后续可能会考虑在规范的`posthtml`基础上添加节点的开始与结束信息.

每一个组件标签最后可以实现一棵单树,在这样的森林里最后可以对光标周围的某个节点(`position`)读取后对其父亲和孩子查找来实现节点树于资源树节点的匹配,理论上还可以`autofix`或者依赖检查的功能.
这样的森林可以通过初始化分析建立`Snapshot`来保存,当局部树改变时,作为`dirty data`处理局部刷新快照.在早期(指[2.0版本](https://github.com/istarwyh/DevUIHelper))的时候,可以借鉴这个思路做成Map,也即认为是单节点的树来查询.

##### 1.3.2.4.1. **几个问题**
>- 与Angular或者其他工具的AST生成有何不同?

答:主要在于分析的范围。以Angular为例，它是全文的,并且依赖于ts的解析器;而分析组件库就只有devui的部分:`d-button -- bsStyle -- primary`这样的三级.

>- 可以复用其他工具的AST吗?

答:理论上可以,但是要实现如Angular的AST对自己的DevUI不好扩展,且显得臃肿.

>- 可以在本地调用markdown的解析包,或者本地实现markdown的解析展示吗?

答:有准备尝试!

>- 可以适配其他组件库吗？

答：可以！**获取素材+合适工具=功能实现**是通用的，而对于某些特殊情况，除了单独建树也可以采用类似于限定某种功能仅当使用组件的同时使用`[]`绑定变量或者`()`绑定函数时被激活的操作,就像`Angular`对基于它自身的`devui`组件做的。事实上，业界[Rome](https://jasonformat.com/rome-javascript-toolchain/#cliusage)更进一步，希望把基于AST的所有功能都统一起来，不要每个工具自己做一次AST解析。

## 1.4. 代码与性能优化
1. 面向接口编程，以及包括将重复使用的模块如`getName()`导出成模块解耦
2. 考虑插件不必要时不必激活
  - 是否引入了对应依赖,如`@angualr`
  - 是否写了标志性的符号,如`<`或`d-`
  - 是否时对应的文件,`html`或`ts`

值得注意的是,在早期这个或许只能考虑读取本地文件[^FileReader]使用正则表达式检测`app.module.ts`中是否引入了`devui`的依赖从而避免无效启动插件浪费资源;当升级了工具--使用AST来获取元素`Token`时,问题就迎刃而解了.

[^FileReader]:基于Electron开发的vscode可以使用nodejs随意读写本地文件、跨域请求、甚至创建一个本地server.

3. 其他设计模式
## 1.5. 上线接受检验!
使用`vsce`打包发布到vscode插件市场,打补丁更新的时候`vsce publish patch`即可.注意可通过`.vscodeignore`省略`test`和`out`等部分.

目前[^pre]`DevUIHelper`[4.0版本](https://github.com/sspku-yqLiu/DevUIHelper-LSP)已在VSCode插件市场上线,欢迎`star`,欢迎`issue & pr`,欢迎使用! :-)
[^pre]:`2.0`版本及之前可见这个[链接](https://github.com/istarwyh/DevUIHelper)



----
作者: [幕宾](https://juejin.im/user/5eae69b36fb9a0436c3ddb5e)
责编: 华为云DevUI团队

《好用到飞起!VS Code插件DevUIHlper设计开发总攻略(二）》即将出炉，敬请期待~
