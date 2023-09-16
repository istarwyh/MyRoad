To choose the suitable database, we should focus on the key differences between them, especially the main technology of them, because I believe none of a database can obtain all the advantages. When we encounter a new project, we should ask ourselves, "How much innovation in basic technology does this project achieve, and what combination of existing technologies does it involve?"

## Ask GPT4
>As an expert on databases, your task is to provide a summary of ten dimensions that illustrate the differences between databases such as MySQL, PostgreSQL, Redis, ElasticSearch, MongoDB, TiDB, ClickHouse, OSS, Simple Storage Service, and others. The dimensions should include factors such as data model, infrastructure, index structure, data-sync strategy, data store strategy and other technical factors. Your summary should explain the reasons why these differences exist and how they affect the suitability of each database for different use cases.
>Your response should be clear and concise, highlighting the most significant differences between each database and the advantages and disadvantages of each in relation to the ten dimensions mentioned above. Your response should be flexible enough to allow for various relevant and creative reasons for the differences between databases. For example, you could explain that MySQL uses a B+ Tree data structure, which is effective for storing large amounts of data on disk, while MongoDB uses a document-based data model that is more flexible but can be less efficient for certain types of queries. For example, you at least tell people that MySQL uses B+ Tree as the major index which is a Multiway Unbalanced Binary Tree existing in the disk, and it usually uses sub-library and sub-table strategy when it stores over 2000 thousand records.
>Your goal is to provide a comprehensive and informative overview of the major technical differences between these databases and the reasons why these differences exist, while also encouraging creative and relevant explanations to make the summary unique and engaging.


MySQL, to some extent, can be considered the "C language" of the database field. As a widely used relational database, MySQL is open-source, stable, and reliable, making it a preferred choice for many applications and websites. However, with the problems encountered in using relational databases like MySQL, a large number of NoSQL and NewSQL databases have emerged to address these issues. 

NoSQL (Not Only SQL) is a category of non-relational distributed databases that aims to overcome the limitations of traditional relational databases in handling large-scale data processing and high concurrency scenarios. NoSQL databases emphasize scalability, high performance, and flexibility, making them suitable for applications that deal with massive and rapidly changing data patterns. Well-known NoSQL databases include MongoDB, Cassandra, and Redis. 

NewSQL is an emerging database concept that combines traditional relational databases with [[Distributed Computing]]. It aims to provide the transactional consistency and data security of traditional relational databases, while also offering the scalability and high concurrency processing capabilities of distributed databases. NewSQL databases achieve data [[Horizontal Scaling]] and load balancing through innovative architectures and technologies such as sharding, replication, and intelligent routing. This combination of relational and distributed computing approaches satisfies the modern application's requirements for high performance, high availability, and strong consistency. Representative examples of NewSQL databases include Spanner, CockroachDB, and TiDB.


## Data Create、Persistence & Backup
### Index Structure
- ...traditional data structure like list、array
- bitmap
- dictionary encoding

 MySQL uses B+ Tree as the major index, which is a multiway unbalanced binary tree existing on disk. It is effective for storing large amounts of data on disk.
- PostgreSQL also uses B+ Tree as its primary index structure, along with other index types like hash, GiST, and SP-GiST.
- Redis uses in-memory data structures for indexing, providing fast access to data.
- ElasticSearch uses an inverted index for full-text search and analytics.
- MongoDB uses B-Tree indexes, which are similar to B+ Tree but with some differences in node structure and traversal.
- TiDB uses a distributed index structure based on Google's Percolator model.
- ClickHouse uses a combination of primary and secondary indexes, including sparse and bitmap indexes.
### Data Persistence Format
If a computer wants to write data from memory to disk, it typically needs to perform at least two steps:

1. The program writes to the [[PageCache]]
2. The [[PageCache]] is flushed to the disk
#### Column
Examples: [[ORC (Optimized Row Columnar)]]
Column storage is suitable for OLAP(Online Analytical Processing) scenarios:
- data scanning
- filtering
- statistical analysis
#### Row

Row storage is suitable for OLTP(Online Transaction Processing) scenarios:
- Primary Key select
- ACID
##### Examples:
- [[SST(Sorted String Table)]]
- [[Inverted Index of Lucene and B+Tree#2.1. B+Tree]]
#### Coexistence of Rows and Columns
In a traditional column storage database, data is stored by column rather than by row. This can be beneficial for analytical queries that aggregate values in a column because the database can read the column data in a continuous disk scan.
##### Examples:
- [[Hologres]] Coexistence of Rows and Columns Style
- [[Bigtable]]
#### LSM
Examples: Inverted Index
### Backup Strategy
#### [[Write-Ahead Logging]]

#### AOF(Append Only File)
Redis doesn't use a traditional [[Write-Ahead Logging|WAL]], but its AOF persistence mode has similarities with the concept of [[Write-Ahead Logging]].
The AOF and [[Write-Ahead Logging]] share the principle of logging changes to data before they are applied, providing a way to recover the state of the data in case of a crash. The AOF can be configured to sync to disk at different frequencies based on the fsync policy. This allows you to balance between write performance and data durability.
#### [RDB(Redis Database)](https://redis.io/docs/management/persistence/)



## Conventional System Implementation Method
### Data Model
- MySQL and PostgreSQL use a relational data model with tables, rows, and columns. They support SQL queries and ACID transactions.
- Redis is an in-memory key-value store, supporting various data structures like strings, lists, sets, and hashes.
- ElasticSearch stores data in JSON documents and is designed for search and analytics.
- MongoDB is a document-based NoSQL database, storing data in BSON format.
- TiDB is a distributed NewSQL database that supports the MySQL protocol and provides horizontal scalability.
- ClickHouse is a columnar database designed for real-time analytics.
- OSS and Simple Storage Service are object storage services, storing data as objects in a flat address space.
### Connection Model
### Spawning a New Process on Each Connection
The process model provides better isolation, for example, an invalid memory access error can only crash a single process, rather than the entire database server. The process model, on the other hand, consumes more resources.
#### Example
- PostgreSQL
### Spawns a New Thread on Each Connection
- MySQL

### Concurrency Control
- MySQL and PostgreSQL use multi-version concurrency control (MVCC) to handle concurrent transactions.
- Redis uses single-threaded execution, processing one command at a time.
- Elasticsearch, MongoDB, and TiDB support optimistic concurrency control.
- ClickHouse uses a lock-free data structure for concurrent query execution.
## Database Solutions for Distributed Systems

### Consensus Algorithm
#### [[Paxos]]
#### [[Raft]]

### Data Synchronization

#### [[Binlog Master-Slave Data Synchronization]]

- TiDB uses the Raft consensus algorithm for data synchronization across its distributed nodes.
##### Asynchronous
- Redis master-slave replication
- ElasticSearch primary-backup model
##### Synchronous
Synchronous replication is a most reliable way comparing with other replication strategy, because the master node MUST WAIT for the slave node to confirm before sending data. Naturally, it can have a significant impact on the performance of the master node because it needs to wait for the slave node to confirm before continuing to process client requests.
##### Semi-synchronous
- MySQL master-slave replication

#### [[Redo-log Master-Slave Data Synchronization]]

### [[Storage-Compute Architecture]]
In traditional database deployment, the master and slave each maintain an independent copy of the data and synchronize through Binlog. In a shared storage cloud database, the master and slave share a copy of the data.

## License
- MySQL Community Edition is licensed under the GPL.
- Postgres is released under the PostgreSQL license, a free and open source license similar to BSD or MIT.

## **Ecosystem Ecological Ecosystem**
## Features of Common Database

### **Performance**

### Query Optimizer
### Data Consistency
- MySQL, PostgreSQL, and TiDB provide strong consistency through ACID transactions.
- Redis offers tunable consistency levels, depending on the configuration.
- Elasticsearch and MongoDB provide eventual consistency.
- ClickHouse focuses on read consistency for analytical queries.
- OSS and Simple Storage Service provide eventual consistency for object storage.
### **Usability**
#### Query Language
- MySQL and PostgreSQL use `SQL` as their query language.
- Redis has its own `set of commands` for data manipulation.
- Elasticsearch uses a JSON-based query language called `Query DSL`.
- MongoDB uses a `JSON-like query language`.
- TiDB supports `SQL` and is compatible with the MySQL protocol.
- ClickHouse uses `SQL` with extensions for analytical functions.
#### JSON Support
Both Postgres and MySQL supports JSON column. Postgres supports more features, like allowing indexes to be created on JSON fields.
### **CTE (Common Table Expression)**
Postgres has more comprehensive support for CTE:
- SELECT, UPDATE, INSERT, DELETE operations within the CTE
- SELECT, UPDATE, INSERT, DELETE operations after CTE
### Advanced Function or User Defined Function Support

### **Operability**


### Extensibility
Postgres supports several extensions. The most outstanding is PostGIS, which brings geospatial capabilities to Postgres. In addition, there is Foreign Data Wrapper (FDW), which supports querying other data systems, pg_stat_statements for tracking planning and execution statistics, and pgvector for vector searches for AI applications.[^PostgrevsMysql]


### Security

## Use Cases
- MySQL and PostgreSQL are suitable for traditional transactional applications, such as web applications and e-commerce platforms.
- Redis is ideal for caching, session management, and real-time analytics.
- Elasticsearch is best for search and log analytics use cases.
- MongoDB is suitable for large-scale, schema-less applications, such as content management systems and IoT platforms.
- TiDB is suitable for hybrid transactional and analytical processing (HTAP) workloads.
- ClickHouse is ideal for real-time analytics and reporting.
- OSS and Simple Storage Service are suitable for storing large amounts of unstructured data, such as images, videos, and backups.

### Other case
1. [一名开发者眼中的 TiDB 与 MySQL 的选择丨TiDB Community](https://mp.weixin.qq.com/s/MrXitmIGnlsWY25aw1SRqw)
2. [数据库选型：MySQL、PolarDB、PolarDB-X、TableStore、MongoDB、TiDB、ClickHouse:](https://mp.weixin.qq.com/s/2oKTItUgvsbEHItiWiGtzA)

## References

https://mp.weixin.qq.com/s/xf7qb4oAVHyi4_U32FSKPA

https://aws.amazon.com/cn/startups/start-building/how-to-choose-a-database/

https://waverleysoftware.com/blog/how-to-choose-the-right-database/


 [^PostgrevsMysql]: [全方位对比 Postgres 和 MySQL (2023 版) ](https://mp.weixin.qq.com/s/xf7qb4oAVHyi4_U32FSKPA)