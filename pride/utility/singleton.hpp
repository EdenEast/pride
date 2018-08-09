
#pragma once

namespace pride
{
template<typename Type>
class singleton_t
{
protected:
    singleton_t() {}

public:
    singleton_t(const singleton_t&) = delete;
    singleton_t(singleton_t&&) = delete;
    singleton_t& operator=(const singleton_t&) = delete;
    singleton_t& operator=(singleton_t&&) = delete;

    static Type& instance()
    {
        static Type _instance;
        return _instance;
    }
};
} // namespace pride
