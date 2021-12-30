#pragma once

#define ATTRIBUTE_PACKED __attribute__((packed))
#define ATTRIBUTE_NO_RETURN __attribute__((noreturn))
#define ATTRIBUTE_HIDDEN __attribute__((visibility("hidden")))
#define ATTRIBUTE_PROTECTED __attribute__((visibility("protected")))
#define ATTRIBUTE_ALIGNED(__alignment) __attribute__((aligned(__alignment)))

#define ALIGN_UP(__addr, __alignment) ((__addr + __alignment - 1) & ~(__alignment - 1))
#define ALIGN_DOWN(__addr, __alignment) (__addr & ~(__alignment - 1))