#pragma once

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)

static constexpr bool TODO = false;
#define TODO() VERIFY(TODO)