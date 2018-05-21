
#pragma once

namespace pride
{
    namespace detail
    {
        template<typename Tag>
        class tagged_bool
        {
            bool value;

            template<typename>
            friend class tagged_bool;

        public:
            constexpr explicit tagged_bool(bool v) : value(v) {}

            template<typename NotBool>
            constexpr explicit tagged_bool(NotBool) = delete;

            template<typename OtherTag>
            constexpr explicit tagged_bool(tagged_bool<OtherTag> b) : value(b.value) {}

            constexpr explicit operator bool() const { return value; }
            constexpr tagged_bool operator!() const { return tagged_bool(!value); }

            friend constexpr bool operator==(tagged_bool l, tagged_bool r) { return l.value == r.value; }
            friend constexpr bool operator!=(tagged_bool l, tagged_bool r) { return l.value != r.value; }
        };
    }

    using detail::tagged_bool;
}
