To choose the suitable database, we should focus on the key differences between them, especially the main technology of them, because I believe none of a database can obtain all the advantages. When we encounter a new project, we should ask ourselves, "How much innovation in basic technology does this project achieve, and what combination of existing technologies does it involve?"


### Ask GPT4
>As an expert on databases, your task is to provide a summary of ten dimensions that illustrate the differences between databases such as MySQL, PostgreSQL, Redis, ElasticSearch, MongoDB, TiDB, ClickHouse, OSS, Simple Storage Service, and others. The dimensions should include factors such as data model, infrastructure, index structure, data-sync strategy, data store strategy and other technical factors. Your summary should explain the reasons why these differences exist and how they affect the suitability of each database for different use cases.
>Your response should be clear and concise, highlighting the most significant differences between each database and the advantages and disadvantages of each in relation to the ten dimensions mentioned above. Your response should be flexible enough to allow for various relevant and creative reasons for the differences between databases. For example, you could explain that MySQL uses a B+ Tree data structure, which is effective for storing large amounts of data on disk, while MongoDB uses a document-based data model that is more flexible but can be less efficient for certain types of queries. For example, you at least tell people that MySQL uses B+ Tree as the major index which is a Multiway Unbalanced Binary Tree existing in the disk, and it usually uses sub-library and sub-table strategy when it stores over 2000 thousand records.
>Your goal is to provide a comprehensive and informative overview of the major technical differences between these databases and the reasons why these differences exist, while also encouraging creative and relevant explanations to make the summary unique and engaging.


MySQL, to some extent, can be considered the "C language" of the database field. As a widely used relational database, MySQL is open-source, stable, and reliable, making it a preferred choice for many applications and websites. However, with the problems encountered in using relational databases like MySQL, a large number of NoSQL and NewSQL databases have emerged to address these issues. 

NoSQL (Not Only SQL) is a category of non-relational distributed databases that aims to overcome the limitations of traditional relational databases in handling large-scale data processing and high concurrency scenarios. NoSQL databases emphasize scalability, high performance, and flexibility, making them suitable for applications that deal with massive and rapidly changing data patterns. Well-known NoSQL databases include MongoDB, Cassandra, and Redis. 

NewSQL is an emerging database concept that combines traditional relational databases with [[Distributed Computing]]. It aims to provide the transactional consistency and data security of traditional relational databases, while also offering the scalability and high concurrency processing capabilities of distributed databases. NewSQL databases achieve data [[Horizontal Scaling]] and load balancing through innovative architectures and technologies such as sharding, replication, and intelligent routing. This combination of relational and distributed computing approaches satisfies the modern application's requirements for high performance, high availability, and strong consistency. Representative examples of NewSQL databases include Spanner, CockroachDB, and TiDB.


# Base Technology

## Data Persistence
If a computer wants to write data from memory to disk, it typically needs to perform at least two steps:

1. The program writes to the [[PageCache]]
2. The [[PageCache]] is flushed to the disk

## In-memory Database
### AOF(Append Only File)
Redis doesn't use a traditional [[Write-Ahead Logging|WAL]], but its AOF persistence mode has similarities with the concept of WAL.
The AOF and [[Write-Ahead Logging]] share the principle of logging changes to data before they are applied, providing a way to recover the state of the data in case of a crash. The AOF can be configured to sync to disk at different frequencies based on the fsync policy. This allows you to balance between write performance and data durability.
### [RDB(Redis Database)](https://redis.io/docs/management/persistence/).


### [[Write-Ahead Logging]]


RDB 


## Distributed  System 

### Consensus Algorithm
#### [[Raft]]



### Data Synchronization

#### [[Binlog Master-Slave Data Synchronization]]

#### [[Redo-log Master-Slave Data Synchronization]]
 
# Compare Items


https://mp.weixin.qq.com/s/xf7qb4oAVHyi4_U32FSKPA

https://aws.amazon.com/cn/startups/start-building/how-to-choose-a-database/

https://waverleysoftware.com/blog/how-to-choose-the-right-database/

>1. Data Model:
- MySQL and PostgreSQL use a relational data model with tables, rows, and columns. They support SQL queries and ACID transactions.
- Redis is an in-memory key-value store, supporting various data structures like strings, lists, sets, and hashes.
- ElasticSearch stores data in JSON documents and is designed for search and analytics.
- MongoDB is a document-based NoSQL database, storing data in BSON format.
- TiDB is a distributed NewSQL database that supports the MySQL protocol and provides horizontal scalability.
- ClickHouse is a columnar database designed for real-time analytics.
- OSS and Simple Storage Service are object storage services, storing data as objects in a flat address space.

2. Infrastructure:
- MySQL, PostgreSQL, Redis, ElasticSearch, MongoDB, and ClickHouse can be deployed on-premises or in the cloud.
- TiDB is designed for cloud-native environments, providing distributed storage and computing.
- OSS and Simple Storage Service are cloud-based storage services provided by Alibaba Cloud and Amazon Web Services, respectively.

3. Index Structure:
- MySQL uses B+ Tree as the major index, which is a multiway unbalanced binary tree existing on disk. It is effective for storing large amounts of data on disk.
- PostgreSQL also uses B+ Tree as its primary index structure, along with other index types like hash, GiST, and SP-GiST.
- Redis uses in-memory data structures for indexing, providing fast access to data.
- ElasticSearch uses an inverted index for full-text search and analytics.
- MongoDB uses B-Tree indexes, which are similar to B+ Tree but with some differences in node structure and traversal.
- TiDB uses a distributed index structure based on Google's Percolator model.
- ClickHouse uses a combination of primary and secondary indexes, including sparse and bitmap indexes.

4. Data-Sync Strategy:
- MySQL and PostgreSQL support replication for data synchronization, with various replication modes like asynchronous, synchronous, and semi-synchronous.
- Redis supports master-slave replication and Redis Cluster for data synchronization.
- ElasticSearch uses a primary-backup model for data replication and synchronization.
- MongoDB supports replica sets and sharding for data synchronization and distribution.
- TiDB uses the Raft consensus algorithm for data synchronization across its distributed nodes.
- ClickHouse supports replication through its distributed tables feature.

5. Data Store Strategy:
- MySQL uses a sub-library and sub-table strategy when storing over 2000 thousand records, improving performance and manageability.
- PostgreSQL uses tablespaces to manage the storage location of its data files.
- Redis stores data in memory, with optional persistence to disk using RDB snapshots or AOF logs.
- Elasticsearch stores data in shards, which are distributed across nodes in a cluster.
- MongoDB uses a storage engine called WiredTiger, which supports document-level concurrency control and compression.
- TiDB uses a distributed storage engine called TiKV, which is based on the Raft consensus algorithm.
- ClickHouse stores data in columns, improving query performance for analytical workloads.

6. Concurrency Control:
- MySQL and PostgreSQL use multi-version concurrency control (MVCC) to handle concurrent transactions.
- Redis uses single-threaded execution, processing one command at a time.
- Elasticsearch, MongoDB, and TiDB support optimistic concurrency control.
- ClickHouse uses a lock-free data structure for concurrent query execution.

7. Data Consistency:
- MySQL, PostgreSQL, and TiDB provide strong consistency through ACID transactions.
- Redis offers tunable consistency levels, depending on the configuration.
- Elasticsearch and MongoDB provide eventual consistency.
- ClickHouse focuses on read consistency for analytical queries.
- OSS and Simple Storage Service provide eventual consistency for object storage.

8. Query Language:
- MySQL and PostgreSQL use SQL as their query language.
- Redis has its own set of commands for data manipulation.
- Elasticsearch uses a JSON-based query language called Query DSL.
- MongoDB uses a JSON-like query language.
- TiDB supports SQL and is compatible with the MySQL protocol.
- ClickHouse uses SQL with extensions for analytical functions.

9. Scalability:
- MySQL and PostgreSQL provide vertical scalability, limited by the capacity of a single machine.
- Redis, Elasticsearch, MongoDB, and TiDB offer horizontal scalability, distributing data across multiple nodes.
- ClickHouse is designed for high scalability, supporting distributed query processing.
- OSS and Simple Storage Service provide virtually unlimited scalability, as they are cloud-based services.

10. Use Cases:
- MySQL and PostgreSQL are suitable for traditional transactional applications, such as web applications and e-commerce platforms.
- Redis is ideal for caching, session management, and real-time analytics.
- Elasticsearch is best for search and log analytics use cases.
- MongoDB is suitable for large-scale, schema-less applications, such as content management systems and IoT platforms.
- TiDB is suitable for hybrid transactional and analytical processing (HTAP) workloads.
- ClickHouse is ideal for real-time analytics and reporting.
- OSS and Simple Storage Service are suitable for storing large amounts of unstructured data, such as images, videos, and backups.

# References

1. [一名开发者眼中的 TiDB 与 MySQL 的选择丨TiDB Community](https://mp.weixin.qq.com/s/MrXitmIGnlsWY25aw1SRqw)
2. [数据库选型：MySQL、PolarDB、PolarDB-X、TableStore、MongoDB、TiDB、ClickHouse:](https://mp.weixin.qq.com/s/2oKTItUgvsbEHItiWiGtzA)