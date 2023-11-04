SST is a file format used to store key-value pairs in which the keys are in sorted order. SST is used extensively in programming for storing large amounts of data. SST files are generally stored on disk, and can be quickly accessed by applications. It is used by LevelDB, Google's key-value storage engine, and in Bigtable for storing indices.

Here is an example of an SST directory structure:
```
event_year=2019 // Partition Keys 
	event_month=11 
		driver_no=12345 
			highway=101 
			000001_0_2019-02-22_08-05-19-123 //Data Files 
			000002_0_2019-02-22_08-05-19-123 000003_0```
```