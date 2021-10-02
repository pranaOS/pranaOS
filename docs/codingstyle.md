# pranaOS Coding Style

### Headers:
Use `#pragma` once instead of `#define` and `#ifdef` for header guards.

###### Wrong:
```c++
#ifndef YOUR_HEADER_H
#define YOUR_HEADER_H

#include <stdio.h>

class myClass {
public:
    void myvoidfunction()
    {
        printf("hello world");
    }
};

#endif
```

###### Right:
```c++
#pragma once 

#include <stdio.h>

class myClass {
public:
    void myvoidfunction()
    {
        printf("hello world");
    }
};
```

### Defining Namespaces:

###### Right:
```c++
namespace myNamespace {
}
```

### Calling Namespaces:

###### Wrong:
```c++

#include <iostream>

using namespace std;

int main()
{
    cout << "hello world";
    return 0;
}

```

###### Right:
```c++

#include <iostream>

int main()
{
    std::cout << "hello world";
    return 0;
}

```
