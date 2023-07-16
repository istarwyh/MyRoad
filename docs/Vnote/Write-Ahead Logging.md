See wiki: https://en.wikipedia.org/wiki/Write-ahead_logging

[>](https://medium.com/@atri_iiita/what-is-wal-write-ahead-log-965bc6057a19)The concept of Write-Ahead Log (WAL) is not limited to PostgreSQL but is also used in other database systems. Here are some examples of where WAL or similar log-based mechanisms are employed:

1. Oracle Database: Oracle Database implements a similar concept called the “Redo Log.”
2. Microsoft SQL Server: SQL Server employs a transaction log, which serves as its implementation of the write-ahead logging mechanism.
3. MySQL: InnoDB, the default storage engine in MySQL, uses a transaction log known as the “InnoDB Log” or “Redo Log.” It follows a similar principle to WAL, where changes are written to the log before being applied to the data files.
4. MongoDB: MongoDB, a NoSQL database, uses an append-only write-ahead log called the “Oplog” (Operational Log). The Oplog captures all write operations in a capped collection.
5. Apache Kafka: Kafka, a distributed streaming platform, uses a write-ahead log to provide fault tolerance and durability for its messaging system. The log called the “Kafka Log” or “Kafka WAL,” records all published messages and guarantees their persistence.
