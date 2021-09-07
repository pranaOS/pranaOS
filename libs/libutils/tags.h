#pragma once

namespace Utils
{

// FIXME: add more enum struct tags

enum struct AdoptTag
{
    __value
};

enum struct WrapTag
{
    __value
};

// FIXME: call auto funcs of enum and implment them

constexpr auto ADOPT = AdoptTag::__value;

constexpr auto WRAPTAG = WrapTag::__value;

}
