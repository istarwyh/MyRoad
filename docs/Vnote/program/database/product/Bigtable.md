Google Cloud Bigtable is a NoSQL wide-column database. The wide-column data model is a type of columnar storage, but it's not quite the same as traditional column storage databases.[^BigtableWiki]

In a traditional column storage database, data is stored by column rather than by row. This can be beneficial for analytical queries that aggregate values in a column because the database can read the column data in a continuous disk scan.

However, in a wide-column database like Bigtable, data is still stored in rows, but it's the columns that can be grouped together into column families. Column families are a way of grouping related columns together in a single row. Within a column family, data is stored in a format that is similar to a two-dimensional array where each row can have an arbitrary number of columns. This allows for much more flexibility than in a traditional relational database where each row has the same set of columns.

Here is how it works:

- Each row in a Bigtable table is identified by a row key.
- Columns in Bigtable are grouped into column families.
- Each column is identified by a combination of a column family and a column qualifier (which is essentially the column name within the family).
- Each cell in a Bigtable table has a timestamp.
- Within a single row, columns are sorted lexicographically by column family and column qualifier.
- Within a single column of a single row, versions are sorted in reverse chronological order.

The wide-column model allows for efficient storage and retrieval of data by row key, column family, and a range of column qualifiers. It also allows for efficient storage and retrieval of multiple versions of a cell's data.

So, while Bigtable does use a form of column storage, it's not the same as traditional column storage. **It's a hybrid model that combines aspects of both row storage and column storage.**



Bigtable support null-query instead of exist-query, because Bigtable cannot store null data. Null in Bigtable represents two semantic meaning:
1. null field is absent
2. the field value is actually null
So it is confusing. And exist query actually define the field is absent in a row.

[^BigtableWiki]:https://en.wikipedia.org/wiki/Bigtable