---
tags: []
---
# PageCache   
   
PageCache is a transparent cache for the pages originating from a secondary storage device such as a hard disk drive (HDD) or a solid-state drive (SSD). The operating system keeps a page cache in otherwise unused portions of the main memory (RAM).   
The primary purpose of the Page Cache is to improve the IO latency of read and write operations. When compared to main memory, hard disk drive read/writes are slow and random accesses require expensive disk seeks; as a result, larger amounts of main memory bring performance improvements as more data can be cached in memory.