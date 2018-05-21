
#pragma once

#include <type_traits>
#include <utility>

namespace pride
{
    namespace detail
    {
        template<typename T, template<typename, typename> class TypePred>
        class only_when
        {
            T value;

        public:
            template<typename U, typename  ::std::enable_if<(TypePred<T, U>::value), bool>::type = true>
            only_when(U&& v) : value(::std::forward<U>(v)) {}

            template<typename U, typename ::std::enable_if<(!TypePred<T, U>::value), bool>::type = true>
            only_when(U&&) = delete;

            T get() const { return value; }
        };
    }

    using detail::only_when;
}
