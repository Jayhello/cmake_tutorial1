1. single file cmake demo

2. test debug and release version profile

build debug version:   cmake .. -DCMAKE_BUILD_TYPE=DEBUG

build release version: cmake .. -DCMAKE_BUILD_TYPE=RELEASE

output as below
```
xy@xy:~/cmake_practice/demo1/build$ ./demo1 
Hello, World!
after loop count is: 100000001
 Time difference = 193

```

