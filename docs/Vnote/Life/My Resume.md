honor:  
  - icon: fa-star  
    date: 2023.3  
    name: 阿里巴巴 | 代码英雄奖 | 8/222 | 代码质量优秀  
  -   
    icon: fa-star  
    date: 2020.10  
    name: 北大 | 学习优秀奖 | 15/139 | GPA3.789  
  -   
    icon: fa-trophy  
    date: 2017.11  
    name: 石大 | 国家奖学金 | 1/69 | GPA3.89  
  
# Tech Stack
  -  
    name: Java  
    percentage: 80%  
  -   
    name: Mysql  
    percentage: 70%  
  
  - name: Junit5  
    percentage: 70%  
  
  - name: AIOS  
    percentage: 50%  
  
  - name: MaxCompute  
    percentage: 50%  
  
  - name: BigTable  
    percentage: 50%  
  
  - name: Flink  
    percentage: 30%  
  -  
    name: LLM  
    percentage: 30%  
  -  
    name: React  
    percentage: 30%  
  -  
    name: C/C++  
    percentage: 20%  
# Project Experience
## 开源贡献  
    project:  
      -  
        name: WritingHelper  
        date: 2021.02 - present  
        img: https://istarwyh.gallerycdn.vsassets.io/extensions/istarwyh/writinghelper/0.1.8/1636288775600/Microsoft.VisualStudio.Services.Icons.Default  
        website:  
        source_code: https://github.com/istarwyh/writingHelper  
        des: !!md |  
          本人学习雅思写作时意识到市场上缺少针对雅思地道词伙写作的工具,于是自己着手开发了WritingHelper。  
                    项目基于Node.js环境开发,通过字典树与责任链借助LSP的API实现实现了对英语词伙的自动补全、汉语翻译以及计时、计数等功能.  
            
          项目上线后使用人数不断增加,目前**4000**+用户使用.  
  
      -   
        name: 其他    
date:   
        source_code:   
        des: !!md |   
          - [learn-with-open-source](https://github.com/zhuangbiaowei/learn-with-open-source.git) collaborator  
            
          - 为Jvm-book提过[issue](https://github.com/fenixsoft/jvm_book/issues/38#issue-647177396)被接收,以及给[ng-devui](https://github.com/DevCloudFE/ng-devui)提过[PR](https://github.com/DevCloudFE/ng-devui/commit/9cde6c49061fdfbeb9a071303db9558bc93c8213)被接收  
  
          - 本人写作的[好用到飞起!VSCode插件DevUIHelper设计开发全功略(一)](https://juejin.im/post/5efaf7f1f265da22c966f445)目前被阅读10000+次,获得点赞90+.  
  
  
 ## 工作项目  
    expert:  
      - company: 淘天集团淘工厂  
        date: 2022.10 - present  
        desc: 本人在淘工厂财务团队主要负责"钱账票一体化"业务。"算得准、接的快、对的清、行的稳"是财务域不变的追求。  
        project:  
          -  
            name: 钱账票一体化  
            img:  
            website:  
            source_code:  
            des: !!md |  
              本人作为账单域owner,主牵淘工厂内部"结算"、"账单"与"供应商开票"三个业务的自动化与一体化。项目涉及十多个计费业务，数据量达百亿，本人深度使用了MaxCompute、PostgreSQL、BigTable等数据库。为提高新业务接入效率，开发了DSL。项目上线后将原本烟囱式开发的效率缩短到一周。  
      -   
        company: 阿里巴巴淘特事业部  
        date: 2021.06 - 2022.10  
        desc: 本人淘特供应链技术团队主要负责补货&库存计划业务。供应链是连接供应商与最终消费者的信息链、物流链与资金链,其中补货&库存计划是保证物流链中货物以正确的数量在正确的时间与地点上架恰好满足订单的关键.  
        project:  
          - name: 补货计划链路重构与迭代  
            img:  
            website:  
            source_code:  
            des: !!md |  
              随着补货计划源头的150万货品池越发庞大,从数据集成开始的整条链路几乎每周出现一次故障,必须重构:1. 品池削减;2. 链路优化; 3. 主动故障监控  
  
              重建整个商家仓链路是从0开始的,选型搜索引擎Ha3,引入SARO数据源和Swift增量;自己从0开始基于http封装client以及ORM的代码,并综合使用了异步、并发、切片等手段优化代码,特别地还从业务逻辑上分策略和冗余优化  
  
              项目的进展立竿见影地减少了工单和故障,并将计算链路的重算时间缩短了**93%**,获得部门内表彰"让用户爽"。  
          -  
            name: 大库存归因  
            img:  
            website:   
            source_code:  
            des: !!md |  
              链路归因类产品通常面临3大问题:1.链路节点技术栈不统一;2.已有数据以及在线与离线逻辑复用问题;3.归因逻辑需要快速反复试错。  
  
              本人作为Inbound域技术PM,通过在应用以及调度平台都打通串联各异构数据源(MySQL、PostgreSQL、Flink、MaxCompute),以及对归因逻辑从大到小的责任链切分组合,顺利解决了以上问题,也为归因类产品探索了一个通用的解决方案。  
  
  
# Skills
      -  
        des: !!md |  
          掌握Java,熟悉JVM特性原理,曾配置ParallelGC、初始堆与方法区内存等使蛋白质建模运行流畅  
      -  
        des: !!md |  
          熟练运用 SpringBoot+Mybatis与中间件HSF、MetaQ,大数据工具 MaxCompute、Flink等  
          - des: !!md |  
          熟悉MySQL;了解Redis、Lucene  
      - des: !!md |  
          熟悉React使用,本人独立承接自己域内全栈需求  
      -  
        des: !!md |  
         不断追求高质量、高效率的工程开发实践,包括领域建模方法(如DDD)、代码规范、code review、git版本控制、日志治理以及**单元测试**等