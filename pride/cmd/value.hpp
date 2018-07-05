
#pragma once

#include "exception.hpp"
#include "../ct/type.hpp"
#include <regex>
#include <string>
#include <sstream>
#include <limits>
#include <type_traits>

namespace pride::cmd
{
    namespace detail
    {
        // regex to parse a number. The capture groups are as follows:
        //   (-)?(?:0[xX])([a-fA-F0-9]+\\b)|(-)?(\\b[1-9][0-9]*\\b|0+\\b)
        //
        //   (-)?                      ; capture '-' and match between zero and one times. as many times as needed
        //   (?:0[xX])                 ; non-capture group for the 0x or 0X part of a hex value
        //   ([a-fA-F0-9]+\\b)         ; capture the hex characters after the 0x of the hex string
        //   |                         ; it can match what has already come before OR what is after
        //   (-)?                      ; capture '-' and match between zero and one times. as many times as needed
        //   (\\b[1-9][0-9]*\\b|0+\\b) ; capture values from 0-9 but the beginning value cant be 0 eg, 09
        // To get more information about this with live demo https://regex101.com/r/9CAUAV/1
        static std::basic_regex<char> integer_pattern("(-)?(?:0[xX])([a-fA-F0-9]+\\b)|(-)?(\\b[1-9][0-9]*\\b|0+\\b)");

        // Regex to capture true and false with option of capital first letter
        static std::basic_regex<char> true_pattern("(t|T)(rue)?");
        static std::basic_regex<char> false_pattern("((f|F)(alse)?)?");

        template <typename T, bool B>
        struct signed_check;

        template <typename T>
        struct signed_check<T, true>
        {
            template <typename U>
            void operator()(bool negative, U u, const std::string& text)
            {
                if (negative)
                    if (u < static_cast<U>(-std::numeric_limits<T>::min()))
                        throw argument_incorrect_type_t(text);
                else
                    if (u > static_cast<U>(std::numeric_limits<T>::max()))
                        throw argument_incorrect_type_t(text);
            }
        };

        template <typename T>
        struct signed_check<T, false>
        {
            template <typename U>
            void operator()(bool, U, const std::string&) {}
        };

        template <typename T, typename U>
        void check_signed_range(bool negative, U value, const std::string& text)
        {
            signed_check<T, std::numeric_limits<T>::is_signed>()(negative, value, text);
        }

        template <typename R, typename T>
        R checked_negate(T&& t, const std::string&, std::true_type)
        {
            // to avoid MSVC C4146
            return -static_cast<R>(t);
        }

        template <typename R, typename T>
        T checked_negate(T&&, const std::string& text, std::false_type)
        {
            throw argument_incorrect_type_t(text);
        }

        template<typename T>
        void integer_parser(const std::string& text, T& value)
        {
            std::smatch match;
            std::regex_match(text, match, integer_pattern);

            if (match.empty())
                throw argument_incorrect_type_t(text);

            using us_type = typename std::make_unsigned<T>::type;
            const auto umax = std::numeric_limits<us_type>::max();
            constexpr bool is_signed = std::numeric_limits<T>::is_signed;
            const bool negative = match.length(1) > 0 || match.length(3) > 0;
            const uint8_t base = match.length(2) > 0 ? 16 : 10;

            if (!is_signed && negative)
                throw argument_incorrect_sign_t(text);

            auto value_match = base == 16 ? match[2] : match[4];
            us_type result = 0;

            for (auto iter = value_match.first; iter != value_match.second; ++iter)
            {
                us_type digit = 0;
                if (*iter >= '0' && *iter <= '9')
                    digit = *iter - '0';
                else if (base == 16 && *iter >= 'a' && *iter <= 'f')
                    digit = *iter - 'a' + 10;
                else if (base == 16 && *iter >= 'A' && *iter <= 'F')
                    digit = *iter - 'A' + 10;
                else
                    throw argument_incorrect_type_t(text);

                if (umax - digit < result * base)
                    throw argument_incorrect_type_t(text);
                result = result * base + digit;
            }

            check_signed_range<T>(negative, result, text);
            value = negative ? checked_negate<T>(result, text, std::integral_constant<bool, is_signed>()) : result;
        }

        template<typename T>
        void string_stream_parser(const std::string& text, T& value)
        {
            std::stringstream in(text);
            in >> value;
            if (!in)
                throw argument_incorrect_type_t(text);
        }

        inline void bool_parser(const std::string& text, bool& value)
        {
            std::smatch result;
            std::regex_match(text, result, true_pattern);

            if (!result.empty())
            {
                value = true;
                return;
            }

            std::regex_match(text, result, false_pattern);
            if (!result.empty())
            {
                value = false;
                return;
            }

            throw argument_incorrect_type_t(text);
        }

        inline void parse_value(const std::string& text, uint8_t& value)  { integer_parser(text, value); }
        inline void parse_value(const std::string& text, int8_t& value)   { integer_parser(text, value); }
        inline void parse_value(const std::string& text, uint16_t& value) { integer_parser(text, value); }
        inline void parse_value(const std::string& text, int16_t& value)  { integer_parser(text, value); }
        inline void parse_value(const std::string& text, uint32_t& value) { integer_parser(text, value); }
        inline void parse_value(const std::string& text, int32_t& value)  { integer_parser(text, value); }
        inline void parse_value(const std::string& text, uint64_t& value) { integer_parser(text, value); }
        inline void parse_value(const std::string& text, int64_t& value)  { integer_parser(text, value); }
        inline void parse_value(const std::string& text, bool& value)     { bool_parser(text, value); }

        inline void parse_value(const std::string& text, std::string& value) { value = text; }

        template<typename T>
        void parse_value(const std::string& text, T& value)
        {
            string_stream_parser(text, value);
        }

        template<typename T>
        void parse_value(const std::string& text, std::vector<T>& value)
        {
            T result;
            parse_value(text, result);
            value.push_back(result);
        }

        template<typename T>
        struct is_container
        {
            static constexpr bool value = false;
        };

        template<typename T>
        struct is_container<std::vector<T>>
        {
            static constexpr bool value = true;
        };
    } // namespace detail

    class value_t : public std::enable_shared_from_this<value_t>
    {
    public:
        virtual ~value_t() = default;

        virtual std::shared_ptr<value_t> clone() const = 0;

        virtual void parse(const std::string& text) const = 0;
        virtual void parse() const = 0;

        virtual bool has_default() const = 0;
        virtual bool is_container() const = 0;
        virtual bool has_implicit() const = 0;

        virtual std::string default_value() const = 0;
        virtual std::string implicit_value() const = 0;
        virtual std::shared_ptr<value_t> default_value(const std::string& value) = 0;
        virtual std::shared_ptr<value_t> implicit_value(const std::string& value) = 0;
        virtual bool is_bool() const = 0;
    };

    template<typename T>
    class abstract_value : public value_t
    {
        using type = abstract_value<T>;

    public:
        abstract_value()
        : _result(std::make_shared<T>())
        , _store(_result.get()) { }

        abstract_value(T* t)
        : _store(t) { }

        virtual ~abstract_value() = default;

        abstract_value(const abstract_value& rhs)
        {
            if (rhs._result)
            {
                _result = std::make_shared<T>();
                _store = _result.get();
            }
            else
            {
                _store = rhs._store;
            }

            _default = rhs._default;
            _implicit = rhs._implicit;
            _default_value = rhs._default_value;
            _implicit_value = rhs._implicit_value;
        }

        void parse() const
        {
            detail::parse_value(_default_value, *_store);
        }
        void parse(const std::string& text) const
        {
            detail::parse_value(text, *_store);
        }

        bool is_container() const
        {
            return detail::is_container<T>::value;
        }

        bool has_default() const
        {
            return _default;
        }

        bool has_implicit() const
        {
            return _implicit;
        }

        std::shared_ptr<value_t> default_value(const std::string& value)
        {
            _default = true;
            _default_value = value;
            return shared_from_this();
        }

        std::shared_ptr<value_t> implicit_value(const std::string& value)
        {
            _implicit = true;
            _implicit_value = value;
            return shared_from_this();
        }

        std::string default_value() const
        {
            return _default_value;
        }

        std::string implicit_value() const
        {
            return _implicit_value;
        }

        bool is_bool() const
        {
            return std::is_same<T, bool>::value;
        }

        const T& get() const
        {
            if (_store == nullptr)
                return *_result;
            return *_store;
        }

    protected:
        std::shared_ptr<T> _result;
        T* _store;
        bool _default = false;
        bool _implicit = false;
        std::string _default_value;
        std::string _implicit_value;
    };

    template<typename T>
    class standard_value : public abstract_value<T>
    {
    public:
        using abstract_value<T>::abstract_value;

        std::shared_ptr<value_t> clone() const
        {
            return std::make_shared<standard_value<T>>(*this);
        }
    };

    template<>
    class standard_value<bool> : public abstract_value<bool>
    {
    public:
        ~standard_value() = default;
        standard_value()
        {
            set_defaults();
        }

        standard_value(bool* b)
        : abstract_value(b)
        {
            set_defaults();
        }

        std::shared_ptr<value_t> clone() const
        {
            return std::make_shared<standard_value<bool>>(*this);
        }
    private:
        void set_defaults()
        {
            _default = true;
            _default_value = "false";
            _implicit = true;
            _implicit_value = "true";
        }
    };

    template<typename T>
    std::shared_ptr<value_t> value()
    {
        return std::make_shared<standard_value<T>>();
    }

    template<typename T>
    std::shared_ptr<value_t> value(T& t)
    {
        return std::make_shared<standard_value<T>>(&t);
    }

    class key_value_t
    {
    public:
        key_value_t(std::string key, std::string value)
        : _key(std::move(key))
        , _value(std::move(value))
        {}

        const std::string key() const { return _key; }
        const std::string value() const { return _value; }

        template<typename T>
        T as() const
        {
            T result;
            detail::parse_value(_value, result);
            return result;
        }

    private:
        std::string _key;
        std::string _value;
    };
} // namespace pride::cml
