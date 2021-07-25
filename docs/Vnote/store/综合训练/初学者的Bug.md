# 初学者的Bug
### 1.4.1. 正则匹配
#### 1.4.1.1. 对于方法的了解---document.write()
**先自己跑一遍**
**确认代码无误**
但还是有问题,So?

- 尝试1
是不是`let`替换了`var`的缘故?-->结果未变,并不是
- 尝试2
是不是英文替换了中文的缘故?-->结果未变,并不是
- 尝试3

是不是得在那个环境下才可以?-->结果你发现是你把`str3`少到了一个`3`,自然无法匹配-->改好后结果正常了
**所以你一开始真的确认代码无误了吗?**
**但其实你又发现了**
`.`的匹配不应该是匹配`除换行符（\n、\r）之外的任何单个字符`吗?这样不是应该把`</span>`前面的都给匹配出来吗?

- Try 4
把`.`换成`[\s\S]`会怎么样-->没有效果
- Try5
那么可能是`+`的问题?你又翻看了定义,`匹配前面的子表达式一次或多次`.觉得不会是这个问题,但你还是改一下成`*`-->![](https://gitee.com/istarwyh/images/raw/master/1618155656_20191215101234089_18272.png =94x),你发现多了个`,`号,这个是啥?-->**你猜**这个是document自己加的,其后面为空,代表0次出现.**但是因为和主问题无关,所以先不管了**

**那么似乎已经尝试完了,你不能理解**
这个时候你去问人是恰当的,然后别人告诉你去console自己调试一下,于是你发现:![](https://gitee.com/istarwyh/images/raw/master/1618155658_20191215101725942_10223.png =268x),哈???
那为什么页面上显示的不一样?**你猜**可能是`document.write()`方法会忽略某些输出.
**于是你搜索了一下**
发现有js中有三种输出方式,于是你都试了一下,发现只有`document.write()`这个在页面上显示的不完整.于是你继续搜索document.write()的使用方法,你发现![](https://gitee.com/istarwyh/images/raw/master/1618155659_20191215103044402_21130.png =265x),果然像`<p>`这种标签式的,document.write()是不会打印给html页面的.
**所以这个问题才算基本解决了**
### 1.4.2. P-mission小程序后端
#### 1.4.2.1. 删掉数据后发现无法加载出页面了
- Try1
猜想可能是改掉的序号乱了它的遍历-->把序号`id`改成连续的1/2/3,同时发现了标签没有闭合(不知道为啥vscode不提示了)-->改好后没有作用
- Try2
发现还有`publish_user_id`,以及`cover`这个标签中还有个`id`,于是都改成一样的-->可以显示一部分,点进去的页面显示非法参数
- Try3
猜想可能是`publish_user_id`,改回去-->显示不出来了
- Try4
全部改成1/2/3-->原本的一部分也显示不出来了
- Try5
于是全部回退,拿到原本的静态数据-->可以并且删掉一部分-->可以-->于是再改一下格式-->可以-->再加一个改完格式的,也可以,当时只能显示部分-->删掉两个改了格式的-->可以了
**结论**:这个不支持改格式,尽管不知道为什么
- Try6
改掉一个`id`与`publish_user_id`-->不行-->把一个干掉再试---->可以
**结论**:前面一个`id`是给进入的页面的参数;后面对于返回页面的查询(`https://ganzhiqiang.wang/ares/goods/detail/?goods_id=50`)也证实了这一点
#### 1.4.2.2. 试验数据库不返回对应值
- Try 1
删掉一些字段-->可以
#### 1.4.2.3. 数据库表创建总提示语法不对
- Try 1
上网搜-->没用但是发现了后面多打了一个`,`号,改了-->报了另一个错误---->发现本质还是打了`,`或`;`什么的
#### 1.4.2.4. 不知道该怎么返回对应格式的数据
- Try1
看后端代码并在后端代码里面搜,关键词`select`-->发现sql有关的语句并不在`Dao层`中,而是在`.xml`文件中,并且他似乎用了Mybatis的手段
于是我想到直接改他的数据库,如果能跑起来应该也就没有问题了-->不过后端似乎用了docker和tomcat,他会显示拒绝访问
- Try2
因为这个项目前后端分离通过接口连接的方式,后端只要能把对应的页面跑出来就好.所以还是回到原疑惑,我怎么返回对应格式的数据?
几个选择:
1. 继续研究怎么返回对应格式
2. 转向学习怎么返回页面,然后自己写sql尽量返回对应格式,如果返回不了就改前端
3. 把所有页面都改成静态的,不过这个也似乎不好解决对应返回页面的问题

选2吧,2自己还有点主动设计,不完全逆向;1是逆向学习,不确定性太大;3的话只能是下下策.
**不得不说**
赵泽龙直接说猜大概像`json`这样的肯定已经有人写了包了
還有就是像`@getter``@Setter`這些也的確應當是有人寫了包
#### 1.4.2.5. 報"無法訪問"
問題找了一圈是**OneDrive同步的時候鎖定**了,但是似乎同步時間還很長?-->考慮要不要換
#### 1.4.2.6. 函數空指針異常
原因居然是注釋掉了`@AutoWired`...因爲上面原本有紅點點,我就把它注釋掉了
2020年6月27日
这是因为没有把对应的属性作为`Bean`自动注入,也就没有实例化,等同于没有
```
private final Properties bean
private constructor(Bean bean){
    this.bean = bean;
}
```
#### 1.4.2.7. 返回的页面无法达到同样效果
最开始自然是怀疑返回的不对,然后改前端逻辑,都失败了于是决定完全返回对应页面,依然不对....
只好怀疑是超出认知之外的问题,于是改掉url返回完全一样的页面,并考虑**排版**的问题,终于发现:
**即使** 是html页面中有注释的东西或者多些根本不显示的标签,都无法成功达到效果!!!
**原因**
大概是因为返回的东西要求是ResPonseBody,会被某些功能类格式化

最后发现我原本以为返回的和它原本的

- 第一版
```json
{
"message":"success","status":1,
"data":
    {
    "goods":
        [
            {
            "id":50,"publish_user_id":57,"title":"卫龙 八成新","describe":"","publish_address":"北京 大兴","price":2.0,"publish_date":"2019-12-19","look_count":55,
            "cover":
                {"id":50,"image_id":76,"path":""},
            "comment_count":5
            }
        ],
    "user_vo":
        [
            {
            "telephone":"15955311246","school":"软微学院","publish_count":2,"look_count":33,
            "user_base":
                {"id":50,"nickname":"Mbin","gender":1,"avatar":"","address":"北京 大兴"}
            }
        ]
    }
}
```
- 第二版
```json
{
"message":"success","status":1,
"data":
    [
        {
        "id":50,"publish_user_id":57,"title":"卫龙 八成新","describe":"","publish_address":"北京 大兴","price":2.0,"publish_date":"2019-12-19","look_count":55,

        "cover":{"id":50,"image_id":76,"path":""},

        "comment_count":5,
        "user_vo":
               {
                "telephone":"15955311246","school":"软微学院","publish_count":2,"look_count":33,
                "user_base":{"id":50,"nickname":"Mbin","gender":1,"avatar":"","address":"北京 大兴"}
                }
        }
    ]
}
```
- **原本的:**
```json
{
"status":1,"message":"success",
"data":
    {
    "id":50,
    "user_vo":
        {
        "user_base":{"id":57,"nickname":"高华峰","gender":1,"avatar":"","address":"Anhui Bozhou"},
        "telephone":null,"school":1,"publish_count":0,"look_count":83
        },
    "title":"卫龙 八成新","descible":" ",
    "photos":
       [
        {"id":76,"path":""}
       ],
    "price":2.0,"publish_address":"安徽 芜湖","publish_date":"2018年4月11日 22:29","look_count":153,"comment_count":2
    }
}
```
**早一点了解啊...模仿也是基于知识的**
[JSON 语法规则](https://www.runoob.com/json/json-syntax.html)
- 数据在名称/值对中
- 数据由`,`分隔
- 大括号`{}`保存对象,对象可以包含多个 key/value（键/值）对,JSON 对象中可以包含另外一个 JSON 对象
- 中括号`[]`保存数组
这样明白了之后甚至可以先用[json在线解析](https://c.runoob.com/front-end/53)看清结构啊!(包括还有看`json`的插件)
#### 1.4.2.8. 构造函数内部不能那个啥?
```java
List<Cover> photos = new ArrayList<>();photos.add(new Cover(id, image_id, path));
    photos.add(cover);
```
事实上,我试了一下,好像这样都不行(price就是从数据库拿出来的值)
```java
this.price = 100000+price;
```
只有在底下添加`get方法`,修改的约定才会生效
但即使是这样依然达不到效果
```java
public  List<Cover> addPlus(){
        List<Cover> photos = new ArrayList<>();photos.add(cover);
        return photos;
}
public  List<Cover> getPhotos(){
        return this.addPlus();}
```

**最后的解决办法**

```java
this.cover = new Cover(id,image_id,path);
this.photos=new ArrayList<>();photos.add(this.cover);
//调用它自己就好了!初始化也是调用自己
```
#### 1.4.2.9. 报avatar格式异常
找了很久还是别人发现的,我删掉了一些传入的值,导致了这些值之间对不上,但是我一直以为自己已经改回来了,而拒绝看是不是没有对上!!!!
多少bug是自己给自己下绊子呵!!!!
#### 1.4.2.10. `describe`始终无法显示
这个是...我单词拼错了,自己原本写成了`descrble`???!!!
呵......望君珍重
### 1.4.3. Bitcron啥啥
`Traceback (most recent call last):  File "D:\Python\Scripts\bitcron-script.py", line 11, in <module>
    load_entry_point('bitcron==0.0.9', 'console_scripts', 'bitcron')()`
上面这个Google了好像没得啥办法...

### 1.4.4. Hadoop云盘
#### 1.4.4.1. JAVA_HOME is not defined correctly.
**输入**
`ant -version`
**返回**
```bash
Error: JAVA_HOME is not defined correctly.
  We cannot execute /usr/lib/jvm/jdk1.7.0-71/bin/java
```
**背景**:
`java -version`可以识别

**解决思路**
[有人说](https://community.oracle.com/thread/1318002)权限问题
有人说jre子目录混淆`bin`
[有人说](https://askubuntu.com/questions/157229/java-home-is-not-defined-correctly)名字格式可能不对,用`ls -al`检查
有人是因为JAVA_HOME多加了一个`bin`
最后有人说使用`find . -exec grep -l "jdk1.7.0_71" {} \`找到真正定义你JAVA_HOME的文件,我才发现自己`JAVA_HOME`中的路径曾经因为疑惑是不是不认`_`而将其改为了`-`.
