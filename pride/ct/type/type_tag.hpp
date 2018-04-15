
#pragma once

namespace pride { namespace ct
{
    template<typename T>
    struct type_tag
    {
        constexpr type_tag() = default;
        using type = T;
    };
}}
