
#pragma once

#include "../../../config/detection.hpp"
#include "../../../preprocessor/function.hpp"
#include "../../cstring.hpp"

#if defined(PRI_COMPILER_CLANG)
    #define PRI_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(PRI_COMPILER_GCC)
    #define PRI_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(PRI_COMPILER_MSVC)
    #define PRI_PRETTY_FUNCTION __FUNCSIG__
#endif

namespace pride { namespace ct { namespace detail
{
    namespace pretty_function
    {
        template<typename T>
        constexpr pride::ct::cstring_t type()
        {
            return { PRI_PRETTY_FUNCTION };
        }

        template<typename T, T Value>
        constexpr pride::ct::cstring_t value()
        {
            return { PRI_PRETTY_FUNCTION };
        }
    }
}}}

#if defined(PRI_COMPILER_CLANG)
    #define PRI_TYPE_PRETTY_FUNCTION_PREFIX "pride::ct::cstring_t pride::ct::detail::pretty_function::type() [T = "
    #define PRI_TYPE_PRETTY_FUNCTION_SUFFIX "]"
#elif defined(PRI_COMPILER_GCC) && !defined(PRI_COMPILER_CLANG)
    #define PRI_TYPE_PRETTY_FUNCTION_PREFIX "constexpr pride::ct::cstring_t pride::ct::detail::pretty_function::type() [with T = "
    #define PRI_TYPE_PRETTY_FUNCTION_SUFFIX "]"
#elif defined(PRI_COMPILER_MSVC)
    #define PRI_TYPE_PRETTY_FUNCTION_PREFIX "class pride::ct::cstring_t __cdecl pride::ct::detail::pretty_function::type<"
    #define PRI_TYPE_PRETTY_FUNCTION_SUFFIX ">(void)"
#else
    #error "No support for this compiler."
#endif

#define PRI_TYPE_PRETTY_FUNCTION_LEFT (sizeof(PRI_TYPE_PRETTY_FUNCTION_PREFIX) - 1)
#define PRI_TYPE_PRETTY_FUNCTION_RIGHT (sizeof(PRI_TYPE_PRETTY_FUNCTION_SUFFIX) - 1)

#if defined(PRI_COMPILER_CLANG)
    #define PRI_VALUE_PRETTY_FUNCTION_PREFIX "pride::ct::cstring_t pride::ct::detail::pretty_function::value() [T = "
    #define PRI_VALUE_PRETTY_FUNCTION_SEPARATOR "; Value = "
    #define PRI_VALUE_PRETTY_FUNCTION_SUFFIX "]"
#elif defined(PRI_COMPILER_GCC) && !defined(PRI_COMPILER_CLANG)
    #define PRI_VALUE_PRETTY_FUNCTION_PREFIX "constexpr pride::ct::cstring_t pride::ct::detail::pretty_function::value() [with T = "
    #define PRI_VALUE_PRETTY_FUNCTION_SEPARATOR "; T Value = "
    #define PRI_VALUE_PRETTY_FUNCTION_SUFFIX "]"
#elif defined(PRI_COMPILER_MSVC)
    #define PRI_VALUE_PRETTY_FUNCTION_PREFIX "class pride::ct::cstring_t __cdecl pride::ct::detail::pretty_function::value<"
    #define PRI_VALUE_PRETTY_FUNCTION_SEPARATOR "; T Value = "
    #define PRI_VALUE_PRETTY_FUNCTION_SUFFIX ">(void)"
#else
    #error "No support for this compiler."
#endif

#define PRI_VALUE_PRETTY_FUNCTION_LEFT (sizeof(PRI_VALUE_PRETTY_FUNCTION_PREFIX) - 1)
#define PRI_VALUE_PRETTY_FUNCTION_SEPARATION (sizeof(PRI_VALUE_PRETTY_FUNCTION_SEPARATOR) - 1)
#define PRI_VALUE_PRETTY_FUNCTION_RIGHT (sizeof(PRI_VALUE_PRETTY_FUNCTION_SUFFIX) - 1)
