/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Utils
{

enum struct AdoptTag
{
    __value
};

enum struct CopyTag
{
    __value
};

enum struct WrapTag
{
    __value
};
enum struct NoneTag
{
    __value
};

constexpr auto ADOPT = AdoptTag::__value;
constexpr auto COPY = CopyTag::__value;
constexpr auto WRAP = WrapTag::__value;
constexpr auto NONE = NoneTag::__value;

}