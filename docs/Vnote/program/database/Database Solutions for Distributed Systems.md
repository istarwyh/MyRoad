## Distributed Consensus
### [[Paxos]]
### [[Raft]]
## Distributed Event
### Clock Synchronization 
- Aurora Limitless
- Google Spanner
## High Scalability
### Sharding Cluster or Sharding and Partitioning
1. [[Possible Problems with Sharding and Partitioning]]

2. A Sharding Cluster is typically used when a single machine cannot handle a large workload or a vast amount of data, thus allowing you to scale horizontally. This process is particularly useful when dealing with very large amounts of data and offers near-limitless scaling to handle big data and intensive workloads

### [[Storage-Compute Architecture]]

In traditional database deployment, the master and slave each maintain an independent copy of the data and synchronize through Binlog. In a shared storage cloud database, the master and slave share a copy of the data.


## High Availability
### Master-Slave Switch
The [[Master-Slave-Switch]] in the database field refers to a process where the roles of the master database and the slave database are switched. This is usually done for purposes of failover, maintenance, or load balancing. The master database is typically the primary source of data, and it handles all write operations, while the slave databases are replicas of the master database and handle read operations.
### Replication

#### [[Binlog Master-Slave Data Synchronization]]

- TiDB uses the Raft consensus algorithm for data synchronization across its distributed nodes.
**Asynchronous**
- Redis master-slave replication
- ElasticSearch primary-backup model

**Synchronous**
Synchronous replication is a most reliable way comparing with other replication strategy, because the master node MUST WAIT for the slave node to confirm before sending data. Naturally, it can have a significant impact on the performance of the master node because it needs to wait for the slave node to confirm before continuing to process client requests.


**Semi-synchronous**
- MySQL master-slave replication

#### [[Redo-log Master-Slave Data Synchronization]]