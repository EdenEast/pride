
#pragma once

#include <type_traits>

namespace pride::ct
{
template<bool... b>
struct all_of;

template<bool... Tail>
struct all_of<true, Tail...> : all_of<Tail...>
{};

template<bool... Tail>
struct all_of<false, Tail...> : std::false_type
{};

template<>
struct all_of<> : std::true_type
{};
} // namespace pride::ct
