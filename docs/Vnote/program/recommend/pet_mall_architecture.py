from diagrams import Diagram, Cluster, Edge
from diagrams.gcp.analytics import BigQuery, Dataflow, PubSub
from diagrams.gcp.compute import AppEngine, Functions, Run
from diagrams.gcp.database import Memorystore
from diagrams.gcp.analytics import Dataproc
from diagrams.gcp.ml import AIHub
from diagrams.c4 import Person, Container, SystemBoundary
from diagrams.gcp.storage import Storage
from diagrams.gcp.ml import AutoML

graph_attr = {
    "fontsize": "45",
    "bgcolor": "transparent",
    "splines": "spline",
}

# 创建图表，方向从上到下
with Diagram("宠物商城风控推荐系统架构", direction="TB", graph_attr=graph_attr):
    # 用户和前端
    customer = Person(
        name="用户",
        description="宠物商城用户"
    )

    # 系统边界
    with SystemBoundary("宠物商城系统"):
        # 前端应用
        frontend = Container(
            name="宠物商城前端",
            technology="React",
            description="用户界面"
        )
        
        # 后端服务
        backend = AppEngine("宠物商城后端")

    # 风控推荐核心
    with Cluster("风控推荐核心链路"):
        profile = AIHub("用户档案补全")
        recall = AutoML("召回商品")
        scoring = Functions("算法打分")
        ranking = Functions("排序、过滤、分页")

    # 特征构建
    with Cluster("特征构建"):
        with Cluster("离线特征"):
            user_features = Dataproc("用户特征")
            item_features = Dataproc("商品特征")
            ui_features = Dataproc("交互特征")

        with Cluster("实时特征"):
            view_stream = PubSub("实时浏览流")
            purchase_stream = PubSub("实时购买流")
            feature_fusion = Dataflow("特征实时融合")

    # 商品过滤
    with Cluster("商品过滤"):
        filter_engine = Run("过滤引擎")
        with Cluster("过滤规则"):
            fatigue = Functions("疲劳度")
            mutex = Functions("互斥")
            status = Functions("状态")

    # 缓存系统
    with Cluster("缓存系统"):
        cache_cluster = Memorystore("Redis集群")
        with Cluster("缓存类型"):
            fatigue_cache = Container(name="疲劳度\n12h", technology="Redis")
            rough_cache = Container(name="粗排\n12h", technology="Redis")
            rec_cache = Container(name="推荐\n1h", technology="Redis")
            item_cache = Container(name="商品\n10min", technology="Redis")

    # 缓存预热
    with Cluster("缓存预热"):
        scheduler = AppEngine("定时任务")
        pubsub = PubSub("任务订阅")
        task_queue = Run("任务队列")

    # 效果优化
    with Cluster("效果优化"):
        ab_platform = BigQuery("达尔文平台")
        monitor = Dataflow("效果监控")

    # 测试与日志
    with Cluster("系统支持"):
        unit_test = Container(name="单元测试", technology="测试框架")
        log_collect = Storage("日志采集")
        log_process = Dataflow("日志处理")

    # 连接关系
    customer >> Edge(color="black") >> frontend >> backend
    backend >> Edge(color="red", style="bold") >> profile
    
    # 特征和召回流程
    profile >> recall
    profile >> Edge(color="blue") >> [user_features, item_features, ui_features]
    [view_stream, purchase_stream] >> feature_fusion
    
    # 特征和召回结果一起进入打分环节
    recall >> Edge(color="green", label="召回商品") >> scoring
    [user_features, item_features, ui_features] >> Edge(color="blue", label="离线特征") >> scoring
    feature_fusion >> Edge(color="orange", label="实时特征") >> scoring
    
    # 后续处理流程
    scoring >> ranking >> filter_engine
    filter_engine >> [fatigue, mutex, status]
    
    # 缓存系统
    [fatigue, mutex, status] >> cache_cluster
    scheduler >> pubsub >> task_queue >> cache_cluster
    
    # 效果优化
    ranking >> ab_platform >> monitor
    
    # 日志系统
    [frontend, backend, profile, recall, scoring, ranking, 
     filter_engine, cache_cluster] >> Edge(style="dotted") >> log_collect >> log_process
