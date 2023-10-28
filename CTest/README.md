## Demo Command
```shell
mkdir src/algorithm/out;
gcc src/algorithm/bubbleSort.c -o src/algorithm/out/bubbleSort;
"src/algorithm/out/"bubbleSort;
;
rm -rf src/algorithm/out/
```
## Detail
### Steps
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
Then you can compile your C source file by using `gcc` Or your C++ source file with `g++`:
```shell
mkdir src/algorithm/out
gcc src/algorithm/bubbleSort.c -o ./algorithm/out/bubbleSort
```
Finally, you can run the compiled result:
```shell
"src/algorithm/out/"bubbleSort
```
Of course, you can clear the result by these operations:
```shell
rm -rf src/algorithm/out/
```
### What does the gcc do?
1. Pre-processing
```shell
cd src/algorithm
mkdir src/algorithm/out
cd out 
gcc -E ../bubbleSort.c -o bubbleSort.i
```
2. Compiling to get assemble language file
```shell
gcc -S bubbleSort.i -o bubbleSort.s
```
3. Assembling to get the binary file
```shell
gcc -c bubbleSort.s -o bubbleSort.o
```
4. Linking
```shell
gcc bubbleSort.o -o bubbleSort
```
Finally, you can run the compiled result:
```shell
"src/algorithm/out/"bubbleSort
```

## Test Framwork
- [boost](https://www.boost.org/doc/libs/1_80_0/libs/test/doc/html/boost_test/section_glossary.html#test_setup)
- [Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#top)