# CodingStyle:

## headers:
- wrong
```c++
#ifndef NAMEOFTHEHEADER
#define NAMEOFTHEHEADER

int x = 1;

#endif
```

- right
```c++
#pragma once

int x = 1;
```

## class and functions name:
- wrong
```c++
class myclass {
public:
    static void myvoid();
};
```

- right
```c++
class myClass {
public:
    static void myVoid();
};
```