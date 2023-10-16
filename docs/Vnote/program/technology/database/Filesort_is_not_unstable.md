
不同的分页查询批次存在重复数据.
MySQL在执行排序操作时，主要[这样选择排序算法](https://github.com/mysql/mysql-server/blob/trunk/sql/filesort.cc)：

1. 如果数据可以完全适应内存，那么MySQL会使用快速排序算法（Quick sort）。

2. 当数据无法完全适应内存，MySQL会使用一种称为外部合并排序（External merge sort）的算法，这是快速排序和合并排序算法的结合。

3. 如果数据可以适应内存，但我们只需要获取部分结果（如使用LIMIT语句），那么MySQL会使用堆排序算法（Heap sort）。

4. 在某些情况下，MySQL也会利用索引来提高排序性能。这并不是一种排序算法，而是一种优化策略。如果查询的排序顺序与索引的顺序相符，那么MySQL可以直接使用索引返回结果，无需再进行排序。

相比于优化策略的使用索引返回结果，MySQL使用的其他排序方式统称为filesort。关于 filesort [Mysql 官方文档](https://dev.mysql.com/doc/refman/8.0/en/limit-optimization.html) 强调：
>If you combine `LIMIT`  _`row_count`_  with `ORDER BY`, MySQL **stops sorting as soon as it has found the first _`row_count`_ rows of the sorted result**, rather than sorting the entire result.
>If multiple rows have identical values in the `ORDER BY` columns, the server is free to return those rows in any order, and may do so differently depending on the overall execution plan. In other words,** the sort order of those rows is nondeterministic with respect to the nonordered columns.**

为了确保行的顺序稳定，我们可以在查询中添加一个额外的非重复的排序条件，例如按 id 列进行排序。事实上，在不添加额外的非重复的排序条件情况下，MySQL是没有能力做到稳定排序的，因为可以理解数据是存在一个Set中，而Set 本身是无序的。