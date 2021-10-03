# Contibution guide:

- building [pranaOS](https://github.com/pranaOS/pranaOS/blob/master/docs/build.md)

## Creating cli app:

- creating files
```bash
mkdir nameofyourapp
cd nameofyourapp
touch main.cpp; touch BUILD.gn; touch .info.mk
```

- code:

- main.cpp
```c++
#include <stdio.h>

int main()
{
    /* code */

    return 0;
}

```

- BUILD.gn
```
import("//build/userland/TEMPLATE.gni")

pranaOS_executable("nameofyourapp") {
  install_path = "bin/"
  sources = [ "main.cpp" ]
  configs = [ "//build/userland:userland_flags" ]
  deplibs = [ "libc" ]
}
```

- .info.mk

```
APPS += NAMEOFAPP

NAMEOFAPP_NAME = nameofapp
NAMEOFAPP_LIBS = c
NAMEOFAPP_INSTALL_PATH = bin/
```

- after doing this steps navigate to build/userland

- BUILD.gn:
```
"//userland/utilities/nameofapp:nameofapp",
"//userland/utilities/touch:touch",
"//userland/utilities/sudo:sudo",
"//userland/utilities/uname:uname",
"//userland/utilities/whoami:whoami",
```

- to check that you have done everything correctly:
```
cd out/
./all.sh
```

- open an terminal in pranaOS and type the cli app you have created