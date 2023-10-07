## 1. Basic Concepts

### 1.1. Developemnt of CI Pipeline
#### 1.1.1. Trditional CI Pipeline
![](https://gitee.com/istarwyh/images/raw/master/1594882447_20200713135521845_26635.png)
#### 1.1.2. Shift Left
Tested should be performed at early juncture of the **Software Development Life Cycle**, and CI/CD can be achieved with automation present.
![](https://gitee.com/istarwyh/images/raw/master/1594882450_20200713143758770_7801.png)
#### 1.1.3. Development Lifecycle With Diffblue
![](https://gitee.com/istarwyh/images/raw/master/1594882449_20200713141329757_7017.png)

#### 1.1.4. Unit Regression Tests
The primary difference between traditional unit tests and unit regression tests is that unit regression tests are **automatically created**(& maintzained) using Diffblue Cover.
The strength is in their numbers and the speed at which AI creates them—hundreds of times faster than the equivalents could be written by people.But When it comes to complex business logic, a human will often produce more accurate unit tests than AI-generated tests. The AI-generated tests can help save the time invested in keeping tests up-to-data[^up-to-date] so that developers can reinvest the time in writing more unit tests for **the highly complex business logic**.

### 1.2. The Judgement of Kingds of Fuzz
#### 1.2.1. Binary Fuzz
Binary fuzzing tools like AFL and libFuzzer treat the input as a sequence of bytes. If the test program expects highly structured inputs, such as XML documents or JavaScript programs, then mutating byte-arrays often results in syntactically invalid inputs; the core of the test program remains untested.[^BF]
#### 1.2.2. Structured Fuzzing
Structured fuzzing tools leverage domain-specific knowledge of the input format to produce inputs that are syntactically valid by construction. 
#### 1.2.3. Reinforcement Learning Test like Diffule
**For Companies:**
>create software velocity and reduce the time it takes to deliver high quality software--Diffblue

**For QA:**

>- Developers **create things for people to use**
>- Quality engineers **create things "that act like people doing things"** and then asserts the results --Trevor C

**So** needs these mertis:

- catch regressions early in the CI pipeline[^CI] most companies moved to a **DevOps model** 
- explore the program states effectively and thus significantly increase the code coverage and the number of triggered bugs and vulnerabilities

### 1.3. Links
**Products**

1. [CI Fuzz Get Started](https://docs.code-intelligence.com/getting_started/)

**Binary fuzzing**

1. [libFuzzer – a library for coverage-guided fuzz testing](https://llvm.org/docs/LibFuzzer.html)

**Semantic Fuzzing**

1. [Semantic Fuzzing](https://arxiv.org/abs/1812.00078)

**Complexity fuzzing**

1. [perffuzz](https://github.com/carolemieux/perffuzz)

**Reinforcement learning**

1. [Diffblue Basic concepts](http://docs.diffblue.com/cover/user-manual/topics/getting-started/basic-concepts.html)
2. [Getting started with the Diffblue Cover IntelliJ plugin](http://docs.diffblue.com/cover/user-manual/topics/getting-started/cover-intellij-plugin.html)
3. [How to apply AI to testing by Jeremias Rößler](https://www.youtube.com/watch?v=nvSqTlzivNM&pbjreload=101)

**Vulnerability Sample**

1. [WebGoat-A Vulnerability Sample](https://github.com/WebGoat/WebGoat)
2. [jqf-zest-example](https://github.com/rohanpadhye/jqf-zest-example)
3. [Fuzzing with Zest](https://github.com/rohanpadhye/jqf/wiki/Fuzzing-with-Zest)

**Others**
1. [Notion](https://www.notion.so/Java-3510eda6cc7e47aa9349bf314a30ca06#6dd89d2d6be84a71bf090d5964cc767d)

## 2. The Methodology of Them
### 2.1. How Does Diffblue Work?
1. It gets your project structure and dependencies from Maven or Gradle. 
2. Instead of the process of getting Object profile typing everything in going through underwriting talking to their creditors all of these types of actions, the `NN` has already discoverd those different paths so now the Object originator can use this neural network which is an `artifact` of the training and generation of our tests that is **the equivalent of a superuser**. Finally users can generate tests automatically without coding.[^NN]
### 2.2. How Does CI Fuzz Work?
CI Fuzz use docker to execute the fuzz tests, which makes it easier to migrate existing fuzz tests into cloud and continuous integration systems and prevents the tests from corrupting data on your local machine. And The **CI-Server**(`CI-Daemon`) is responsible for backend tasks such as project compilation, managing fuzz-targets and operating the docker infrastructure and interacts with the CI-Client and the UI[^CI-get]. 
In the backend it use **instrumentation**, the most promising `dynamic code analysis` technique.The software under test is executed with inputs, which are systematically mutated during the testing process. In this process, the mutation engine gets **coverage and data structure feedback** from executed code via instrumentation. An `initial static analysis` of the code allows the deduction of the input structure and is used to guide the fuzzer allowing to produce inputs reaching important states in the program.[^CIfuzz] Due to measuring code coverage, the fuzzer can monitor which parts of the program were reached with a given input and reach other program parts by generating similar inputs with random but small changes.

### 2.3. How Does Fuzzit+JQF+Zest Work?
CI fuzz may base on this.

1. A way to understand the input structure
    - by using declarative specifications of the input format such as context-free [grammars](https://embed.cs.utah.edu/csmith/) or [protocol buffers](https://github.com/google/libprotobuf-mutator)
2. A Generator<T> provides a method for producing random instances of type `T`
3. A algorithm which uses **code-coverage and input-validity feedback** to bias a `QuickCheck-style generator` towards generating structured inputs
    - bytecode instrumentation
    - input validity using JUnit's [Assume](https://junit.org/junit4/javadoc/4.12/org/junit/Assume.html) API

4. [Continuous Fuzzing for Java Example](https://github.com/fuzzitdev/example-java)

## 3. Others
### 3.1. About Diffblue Product & Team
#### 3.1.1. Background
They were founded in 2016, and in 2017 raised a $22 million Series A, the largest AI round in Europe that year. 
They are looking for a confident individual as product Management or Sales Engineering who has a background in Java software development.[^diffblue-job-java] And they are looking looking for a technical leader to manage our **Open Source team** working with other C++ developers and experts in AI, static and dynamic program analysis, and software verification.[^diffblue-job-c]

#### 3.1.2. Jugement
![](https://gitee.com/istarwyh/images/raw/master/1594882450_20200713185019671_14013.png)
### 3.2. Integrate Your JQF Targets with the Fuzzit Continuous Fuzzing Platform
### 3.3. Selenium with AI to Generate Automation Scripts
**Trevor E. Chandler**[^TC] at a meeting launched by `IIST`:[^IIST]
>A deeper meaning inside of QA(Quality Assurance) in general and some of the opportunities that QA brings us as QA engineers we have some very special skills as QA that are very relevant with AI and where the future of engineering is going are all very exciting.

MIT can take a snapshot of the communication of the brain at any given time. In the same way we run code coverage at unit tests or API tests or other things of the nature and we can capture a snapshot of every **conditional branch** and **line of code** that was used that we have there technology to do that same kind of thing with the brain so they exercise a thought or a series of thoughts and they could pull out a mapping.[^NN]
And NN used in tests like in path games:
![](https://gitee.com/istarwyh/images/raw/master/1594882450_20200714160257771_28608.png)

### 3.4. Reinforcement Learning
#### 3.4.1. Reinforcement Learning & NN
- Reinforcement learning tries **all combinations cycling all data**: 
    - prove without any doubt
    - the best way is generally the objective that took the least amount of steps

- Neural Network:[^NN]
    Every Little piece holds the **data** and its **weight** between 0~1 as a token of how confident it feels it is correct. What's the most important,every singal point can benefit to the other points of data with their communication about the `answer(?)`.--- The process must be helped with **Intelligent Data Structure** instead of traditional databases in face of massive queries & updates.
#### 3.4.2. How to Use NN as QA
QA may want to do `1` the first time as they're learning as the code is discovering all the functionality of the application but after that they just want to use the best way usually, so after the system runs a neural network is populated with all of the information that neural network in essence is **super user of QA's application** so QA can do some very impressive things with it --

- QA can tell it to **run back all the tests** using the most efficient way to solve each objective.
- QA could tell it to run all the positive every different way that the objective succeeded in none of the negative ways.
- QA could tell it to run all the ways that failed 
- QA can even pick a single thing and ....

**Requirements**

- The objective must have measurable actions, that are readonably believed, in some combination, to be able to fulfill the objective.
- The actions must be executable automatically through a system that also has access to the results, and , can persist and update the persisted information.

[^up-to-date]: A new baseline test suite is created each time Diffblue Cover is run, the tests are always up-to-date without manually maintaining them.  
[^BF]:[What is structured fuzzing?](https://github.com/rohanpadhye/jqf)
[^CI]: Continuous Integration process.
[^NN]:[Using Selenium and AI to Create Test Cases](https://www.youtube.com/watch?v=aZj1Y5P_YSg)
[^CI-get]:[CI Fuzz contains three main components:](https://docs.code-intelligence.com/getting_started/)
[^CIfuzz]:[Resources on Automated Security Testing](https://cdn2.hubspot.net/hubfs/7466322/Resources/CI_Onepager_tech_java_springboot.pdf)
[^diffblue-job-java]:[Developer Relations Engineer](https://apply.workable.com/diffblue/j/A517E1C81E/)
[^diffblue-job-c]:[Technical Team Leader - OpenSource Development](https://apply.workable.com/diffblue/j/DAE1107143/)
[^TC]:[Trevor C.](https://www.linkedin.com/in/trevorchandler/)
[^IIST]: International Institute for Softwae Testing