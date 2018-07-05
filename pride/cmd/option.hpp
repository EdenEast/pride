
# pragma once

#include "value.hpp"
#include <string>
#include <memory>

namespace pride::cmd
{
    class option_t;
    class option_value_t;

    class option_t
    {
    public:
        option_t() = default;
        option_t(const std::string short_, const std::string long_,
            const std::string desc, std::shared_ptr<const value_t> value)
        : _short(short_)
        , _long(long_)
        , _desc(desc)
        , _value(value)
        , _count(0)
        { }

        option_t(const option_t& rhs)
        : _short(rhs._short)
        , _long(rhs._long)
        , _desc(rhs._desc)
        , _count(rhs._count)
        {
            _value = rhs._value->clone();
        }

        option_t(option_t&& rhs) = default;

        const std::string& description() const { return _desc; }
        const value_t& value() const { return *_value; }
        std::shared_ptr<value_t> make_storage() const { return _value->clone(); }
        const std::string& short_name() const { return _short; }
        const std::string& long_name() const { return _long; }

    private:
        std::string _short;
        std::string _long;
        std::string _desc;
        std::shared_ptr<const value_t> _value;
        int _count;
    };

    class option_value_t
    {
    public:
        void parse(std::shared_ptr<const option_t> details, const std::string& text)
        {
            ensure_value(details);
            ++_count;
            _value->parse(text);
        }

        void parse_default(std::shared_ptr<const option_t> details)
        {
            ensure_value(details);
            _value->parse();
        }

        size_t count() const
        {
            return _count;
        }

        template<typename T>
        const T& as() const
        {
            return static_cast<const standard_value<T>&>(*_value).get();
        }

    private:
        void ensure_value(std::shared_ptr<const option_t> details)
        {
            if (_value == nullptr)
                _value = details->make_storage();
        }

        std::shared_ptr<value_t> _value;
        size_t _count = 0;
    };
} // namespace pride::cmd
