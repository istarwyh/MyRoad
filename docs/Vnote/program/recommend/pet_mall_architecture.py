from diagrams import Diagram, Cluster, Edge
from diagrams.gcp.analytics import BigQuery, Dataflow, PubSub
from diagrams.gcp.compute import AppEngine, Functions, Run, GKEOnPrem
from diagrams.gcp.database import Memorystore
from diagrams.gcp.analytics import Dataproc
from diagrams.gcp.ml import AIHub
from diagrams.c4 import C4Node, Person, Container, SystemBoundary
from diagrams.gcp.storage import Storage
from diagrams.gcp.ml import AutoML
from diagrams.programming.framework import React
from diagrams.saas.chat import Messenger

graph_attr = {
    "fontsize": "45",
    "splines": "ortho",
    "nodesep": "1.2",
    "ranksep": "1.5",
    "concentrate": "true",
    "compound": "true",     # 允许边连接到子图
    "clusterrank": "local", # 允许子图独立排序
    "constraint": "false"   # 减少边的约束
}

core_graph_attr = {
    "nodesep": "10.2",
    "ranksep": "10.5"
}

# 创建图表，方向从左到右
with Diagram("宠物商城风控推荐系统架构", direction="LR", graph_attr=graph_attr, outformat="png"):
    # 用户和前端
    customer = Messenger("宠物商城用户")

    # 创建主系统
    with Cluster("基础系统"):
        # 系统边界
        with SystemBoundary("宠物商城系统"):
            frontend = React("宠物商城前端")
            backend = AppEngine("宠物商城后端")

        # 风控推荐核心链路，与商城系统并排
        with Cluster("风控推荐核心链路", graph_attr=core_graph_attr):
            profile = AIHub("用户档案补全")
            recall = AutoML("HA3 召回商品")
            scoring = Functions("AI Studio算法打分")
            ranking = Functions("排序、过滤、分页")

            # 强制水平布局
            profile >> recall >> scoring >> ranking

    # 支撑系统组
    with Cluster("支撑系统"):
        # 效果优化
        with Cluster("效果优化"):
            ab_platform = BigQuery("达尔文平台")
            monitor = Dataflow("效果监控")

        # 测试与日志
        with Cluster("系统支持"):
            unit_test = GKEOnPrem("单元测试")
            log_collect = Storage("日志采集")
            log_process = Dataflow("日志处理")

    # 特征构建系统 - 使用水平布局
    with Cluster("特征工程", graph_attr={"rankdir": "LR", "rank": "same", "constraint": "false"}) as feature_eng:
        # 离线特征组
        with Cluster("离线特征组") as offline:
            user_features = Dataproc("用户特征")
            item_features = Dataproc("商品特征")
            ui_features = Dataproc("交互特征")

        # 实时特征组
        with Cluster("实时特征组") as realtime:
            view_stream = PubSub("实时浏览流")
            purchase_stream = PubSub("实时购买流")
            feature_fusion = Dataflow("特征实时融合")

        # 使用不可见边缘连接节点，强制水平布局
        user_features >> Edge(style="invis", constraint="false") >> view_stream
        item_features >> Edge(style="invis", constraint="false") >> purchase_stream
        ui_features >> Edge(style="invis", constraint="false") >> feature_fusion

    # 过滤和缓存系统
    with Cluster("过滤与缓存", graph_attr={"rank": "same", "constraint": "false"}):
        # # 商品过滤
        with Cluster("商品过滤"):
            filter_engine = Run("过滤模块")
            with Cluster("过滤规则"):
                fatigue = Functions("疲劳度")
                mutex = Functions("互斥")
                status = Functions("状态")

        # 缓存系统
        with Cluster("缓存系统"):
            cache_cluster = Memorystore("Redis集群")
            with Cluster("缓存预热"):
                scheduler = AppEngine("定时任务")
                pubsub = PubSub("任务订阅")
                task_queue = Run("任务队列")

    # 核心流程连接
    customer >> frontend >> backend >> profile >> recall >> scoring >> ranking >> filter_engine

    # 特征连接
    profile >> Edge(color="blue", style="dashed") >> user_features
    [view_stream, purchase_stream] >> feature_fusion >> scoring

    # 过滤规则连接
    filter_engine >> Edge(color="red") >> [fatigue, mutex, status]

    # 缓存系统连接
    [fatigue, mutex, status] >> cache_cluster
    scheduler >> pubsub >> task_queue >> cache_cluster

    # 效果优化连接
    ranking >> ab_platform >> monitor

    # 日志系统连接
    log_group = [frontend, backend, profile, recall, scoring, ranking, filter_engine]
    for component in log_group:
        component >> Edge(style="dotted", color="gray") >> log_collect

    log_collect >> log_process

    # 添加一个不可见的边来连接特征工程和过滤缓存系统
    feature_fusion >> Edge(style="invis", minlen="10", constraint="false") >> filter_engine
