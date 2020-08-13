# 1. <center>Online Judge</center>

## 1.1. 成员分工工作量
本项目主要实现以下功能与设计：
1. 完成前端与后端交互，完成核心判题功能
2. 从数据库读题目名称,从in/out文件读测试用例
3. 从文件读题目描述
4. 添加用户
5. 前端基本设计
6. 前端优化设计
7. 程序运行计时并正确显示
8. 添加题目与测试用例
9. 将报错信息显示在页面上并正确显示
其中1、2、4、9由主要由黄佳威同学实现，3、6主要由刘勇琦同学实现，5、7主要由王艺辉同学实现。8共同实现。
工作量百分比分别是:
## 1.2.  OJ关键流程图
<img src="https://i.loli.net/2019/12/08/rJU6wtIC2ZDfxec.png"/>

## 1.3. 核心业务的实现方式
### 1.3.1. 读入读出
对于用户要提交的代码有主要三种处理方式:
- 从文件读入输出
- 从控制台读入输出
- 从网页端读入网页端输出
我们选择了对用户最友好的第三种,主要通过Spring Boot的controller层与前端交互.
### 1.3.2. 编译运行与测试
请见1.4节的核心代码介绍.
### 1.3.3. 运行时间与限制
通过设定开始与结束时间可以得到程序运行时间.
**没有实现的**
再开一个线程对这个运行的程序进行计时，若程序超出时间，自动kill它。否则，评判机将可能因为运行程序死循环而停止工作下一步的工作或者同时运行过多死循环的程序而崩溃。
具体实现思路:
在Windows上使用Windows API监测程序的内存和运行时间(在Linux下对应的使用POSIX API).并且因为需要做操作系统API的调用，而那些接口是C语言的，还需要用C语言调用。<sup>[1](https://www.zhihu.com/question/20343652/answer/94145691)</sup>
### 1.3.4. 返回运行结果
一般的编译与运行错误信息或者运行时间在Controller层放到status中,然后封装进message中,再在前端调用这个message输出.此外通过`  final ByteArrayOutputStream outErr = new ByteArrayOutputStream();System.setErr(new PrintStream(outErr));`可以将原本输出在控制台的错误信息放到outErr中最后在网页端输出.
**另一种实现思路**
在Web使用Server Sent Event返回实时的评测结果。假设一个题有多个测试点，那么每评测一个测试点评测机会向消息队列丢一个消息。Web也在监听消息队列，然后将评测机发来的消息返回给对应的用户。
## 1.4. 核心代码介绍

### 1.4.1. Controller层

```java
		@ResponseBody
    @RequestMapping(value = "/context")
    public ModelAndView title(Model model,@RequestParam("answer") String answer, HttpSession session) throws FileNotFoundException {
        String status ="等待判题";
        File solution = new File("./repository/answer/Solution.java");

        try{
            Files.write(solution.toPath(), answer.getBytes(StandardCharsets.UTF_8));
            System.out.println("创建成功");
        }
        catch (IOException e){
            e.printStackTrace();
            System.out.println("创建失败");
        }
        long Runtime = 0;
        long startTime = System.currentTimeMillis();
        if(userService.compile(solution).equals("编译成功")){
            status= userService.run(pid);
            Runtime = System.currentTimeMillis() - startTime;
        }
        else{
            status= "编译失败";
        }
        ModelAndView modelAndView = new ModelAndView();
        Problem problem = userService.ProblemContext(pid);
        modelAndView.setViewName("/context");
        List<String> activproblemContext =userService.getcontext(problem.getId());
        problem.setProblemcontext(activproblemContext);
        modelAndView.addObject("problem",problem);
        modelAndView.addObject("useranswer",answer);
        session.setAttribute("message",status+ "\t\n"+"运行时间:" + Runtime+"ms");
        return modelAndView;
    }
}
```



**逻辑如下：**

1. controller层从前端页面得到用户的答案输入
2. 将输入的字符串写入java文件并存在项目根目录repository/answer文件夹下
3. 得到当前的系统时间，并进行编译，运行，用例测试三步操作
4. 在页面输出判题状态，还有运行时间



### 1.4.2. Service层

```java
public String compile(File solution){
        try {
            JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
            if(compiler == null){
                System.out.println("JDK required");
                return "编译错误";
            }else{
                System.out.println("got it");
            }

            final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
            System.setErr(new PrintStream(outContent));
            int compilationResult = compiler.run(null, null, null, solution.getPath());
            if (compilationResult == 0) {
                System.out.println("Compilation is successful");
            } else {
                System.out.println("Compilation Failed");
                return "编译错误\n"
                        + outContent.toString();
            }
        }
        catch (Exception e){
            e.printStackTrace();
        }

        return "编译成功";
    }

   public String run(int problemid) {
        File folder = new File("./repository/answer");
        final ByteArrayOutputStream outErr = new ByteArrayOutputStream();
        System.setErr(new PrintStream(outErr));

        try {
            URL[] urls = new URL[]{folder.toURI().toURL()};
            URLClassLoader loader = new URLClassLoader(urls);
            Class c = loader.loadClass("Solution");
            Method m = c.getDeclaredMethod("main", String[].class);

            File inAndout = new File("./repository/in_out/" + problemid);
            int length = inAndout.listFiles().length;
            int count = 0;


            for (int i = 1; i <= length / 2; i++) {
                File in = new File("./repository/in_out/" + problemid + "/" + i + ".in");
                StringBuffer intemp = new StringBuffer();
                Scanner input = new Scanner(in);
                while (input.hasNext()) {
                    String s = input.nextLine();
                    intemp.append(s + "\n");
                }
                String data = intemp.toString();
                InputStream stdin = System.in;
                System.setIn(new ByteArrayInputStream(data.getBytes()));
                final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
                System.setOut(new PrintStream(outContent));


                m.invoke(null, (Object) new String[]{});

                File out = new File("./repository/in_out/" + problemid + "/" + i + ".out");
                Scanner output = new Scanner(out);
                StringBuffer outtemp = new StringBuffer();
                while (output.hasNext()) {
                    String s = output.nextLine();
                    outtemp.append(s);
                }
                String answer = outtemp.toString();

                input.close();
                output.close();

                if (outContent.toString().trim().equals(answer)) count++;
                outContent.close();

            }

            if (count == length / 2) return "答案正确" ;
            else return "答案错误" ;
        } catch (Exception e) {
            e.printStackTrace();
            return "运行错误\n"+outErr.toString().substring(outErr.toString().lastIndexOf("Caused")+10,outErr.toString().length()-10);
        }
    }
```



**compiler方法逻辑如下：**

1. 通过ToolProvider类的静态方法getSystemJavaCompiler来得到一个JavaCompiler接口的实例。
   　　JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();

通过调用 ToolProvider 的 getSystemJavaCompiler 方法，JDK 提供了将当前平台的编译器映射到内存中的一个对象。这样使用者可以在运行时操纵编译器。JavaCompiler 是一个接口，它继承了javax.tools.Tool 接口。

2. 如果compiler为null，则找不到编译器，则返回编译错误
3. 获得编译器对象之后，可以调用 Tool.run 方法对源文件进行编译。Run 方法的前三个参数，分别可以用来重定向标准输入、标准输出和标准错误输出。如果run编译成功，返回0。
   　　int run(InputStream in, OutputStream out, OutputStream err, String… arguments)
   如果前3个参数传入的是null，那么run方法将以标准的输入、输出代替，即System.in、System.out和System.err
4. 编译成功后开始运行程序



**run方法逻辑如下：**

通过反射机制运行java文件，java反射机制是在运行状态中，对于任意一个类，都能够知道这个类的所有属性和方法；对于任意一个对象，都能够调用它的任意方法和属性；这种动态获取信息以及动态调用对象方法的功能称为java语言的反射机制。

用途：在日常的第三方应用开发过程中，经常会遇到某个类的某个成员变量、方法或是属性是私有的或是只对系统应用开放，这时候就可以利用Java的反射机制通过反射来获取所需的私有成员或是方法。

1. 获得answer文件夹的url

2. 通过URLClassLoader从文件夹下的class文件

   URLClassLoader提供了这个功能，它让我们可以通过以下几种方式进行加载：
     \* 文件: (从文件系统目录加载)
     \* jar包: (从Jar包进行加载)
     \* Http: (从远程的Http服务进行加载)

![](https://i.loli.net/2019/12/07/ScdCGOUswvufz6l.png)
3. `Class c = loader.loadClass("Solution");`获得Solution类
4. `Method m = c.getDeclaredMethod("main", String[].class);`获得main方法，之后就可以利用` m.invoke(null, (Object)new String[]{});`运行代码
5. 读取repository/in_out/下对应题号的文件夹，读出文件夹下的in和out文件，作为代码的输入和输出

6. 利用System.setIn将标准输入System.in改变为从文件读入输入，输出放在ByteArrayOutputStream一个实例内，并和文件夹内正确输出做比对，若一致则当前样例通过
7. 判断完当前题目文件夹下的所有测试用例后，若通过测试用例次数等于文件夹下测试用例，则返回答案正确，若通过次数不等于测试用例数，则返回答案错误
8. 若运行中出现错误，则返回运行错误

