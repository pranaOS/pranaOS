# CodingStyle:

## headers:
- creating a header for a .cpp:
```cpp 
#pragma once

namespace somenamespace {
    // your code goes here...
}
```

- creating headers for .c:
```c
#ifndef FOLDER_FILENAME_H
#define FOLDER_FILENAME_H

// your code goes here..

#endif
```

## Including a universal header:
- wrong
```c++
#include "myheadername.h"
```

- right
```c++
#include <myheadername.h>
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

## classes:
- if your going to create more classes in a header make sure to group them inside a namespace;
- wrong
```c++
class myClassOne {
    // your code
};

class myClassTwo {
    // your code  
};
```

- right
```c++
namespace NameOFYourNamespace {
    class myClassOne {
        // your code
    };

    class myClassTwo {
        // your code  
    };
}
```

## Namespace names:
- if your working on Kernel then name of the namespace should be Kernel
```c++
namespace Kernel {
    class ACPI {
        public:
            // some code
    };
}
```

- if your working on a library make sure to keep the name of the library as namespace name
- Libc:
```c++
namespace LibC {
    class stdio {
        public:
            // your code
    };
}
```
