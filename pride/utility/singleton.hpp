
#pragma once

namespace pride
{
    template<typename Type>
    class singleton
    {
    protected:
        singleton() {}
    public:
        singleton(const singleton&) = delete;
        singleton(singleton&&) = delete;
        singleton& operator=(const singleton&) = delete;
        singleton& operator=(singleton&&) = delete;

        Type& instance()
        {
            static Type _instance;
            return _instance;
        }
    };
}
