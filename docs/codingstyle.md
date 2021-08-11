# Coding Style

- do not use this header style
```c
#ifndef NAME_HEADER_H
#define NAME_HEADER_H
```

- use this style:
```c
#pragma once
```

# Pointers and References

An out argument of a function should be passed by reference except rare cases where it is optional in which case it should be passed by pointer.

- do not use this style
```cpp
void MyClass::get_some_value(OutArgumentType* outArgument) const
{
    *out_argument = m_value;
}
```

- use this style:
```cpp
void MyClass::do_something(OutArgumentType* out_argument) const
{
    do_the_thing();
    if (out_argument)
        *out_argument = m_value;
}
```

