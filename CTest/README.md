## Demo Command
```shell
mkdir ./algorithm/out;
gcc ./algorithm/bubbleSort.c -o ./algorithm/out/bubbleSort;
"./algorithm/out/"bubbleSort;
;
rm -rf ./algorithm/out/
```
## Detail
See your gcc info:
```shell
gcc -v
```
And maybe you can get this info:
```md
Apple clang version 13.1.6 (clang-1316.0.21.2.5)
Target: x86_64-apple-darwin21.6.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```
Then you can compile your C source file by using gcc:
```shell
mkdir ./algorithm/out
gcc ./algorithm/bubbleSort.c -o ./algorithm/out/bubbleSort
```
Finally, you can run the compiled result:
```shell
"./algorithm/out/"bubbleSort
```
Of course, you can clear the result by these operations:
```shell
rm -rf ./algorithm/out/
```