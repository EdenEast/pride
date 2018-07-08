
#pragma once

#include <exception>
#include <string>

#ifdef _WIN32
    const std::string LQUOTE("\'");
    const std::string RQUOTE("\'");
#else
    const std::string LQUOTE("‘");
    const std::string RQUOTE("’");
#endif

namespace pride::cmd
{
    class option_exception_t : public std::exception
    {
    public:
        option_exception_t(const std::string& message)
        : message(message) { }

        virtual const char* what() const noexcept
        {
            return message.c_str();
        }
    private:
        std::string message;
    };

    class option_spec_exception_t : public option_exception_t
    {
    public:
        option_spec_exception_t(const std::string& message)
        : option_exception_t(message) { }
    };

    class option_parse_exception_t : public option_exception_t
    {
    public:
        option_parse_exception_t(const std::string& message)
        : option_exception_t(message) { }
    };

    class option_exists_error_t : public option_spec_exception_t
    {
    public:
        option_exists_error_t(const std::string& option)
        : option_spec_exception_t(u8"Option " + LQUOTE + option + RQUOTE + u8" already exists") { }
    };

    class invalid_option_format_error_t : public option_spec_exception_t
    {
    public:
        invalid_option_format_error_t(const std::string& format)
        : option_spec_exception_t(u8"Invalid option format " + LQUOTE + format + RQUOTE) { }
    };

    class option_not_exists_exception_t : public option_parse_exception_t
    {
    public:
        option_not_exists_exception_t(const std::string& option)
        : option_parse_exception_t(u8"Option " + LQUOTE + option + RQUOTE + u8" does not exist") { }
    };

    class missing_argument_exception_t : public option_parse_exception_t
    {
    public:
        missing_argument_exception_t(const std::string& option)
        : option_parse_exception_t(
            u8"Option " + LQUOTE + option + RQUOTE + u8" is missing an argument"
        ) { }
    };

    class option_requires_argument_exception_t : public option_parse_exception_t
    {
    public:
        option_requires_argument_exception_t(const std::string& option)
        : option_parse_exception_t(
            u8"Option " + LQUOTE + option + RQUOTE + u8" requires an argument"
        ) { }
    };

    class option_not_has_argument_exception_t : public option_parse_exception_t
    {
    public:
        option_not_has_argument_exception_t(const std::string& option, const std::string& arg)
        : option_parse_exception_t(
            u8"Option " + LQUOTE + option + RQUOTE +
            u8" does not take an argument, but argument " +
            LQUOTE + arg + RQUOTE + " given"
        ) { }
    };

    class option_not_present_exception_t : public option_parse_exception_t
    {
    public:
        option_not_present_exception_t(const std::string& option)
        : option_parse_exception_t(u8"Option " + LQUOTE + option + RQUOTE + u8" not present") { }
    };

    class argument_incorrect_type_t : public option_parse_exception_t
    {
    public:
        argument_incorrect_type_t(const std::string& arg)
        : option_parse_exception_t(
            u8"Argument " + LQUOTE + arg + RQUOTE + u8" failed to parse"
        ) { }
    };

    class argument_incorrect_sign_t : public option_parse_exception_t
    {
    public:
        argument_incorrect_sign_t(const std::string& arg)
        : option_parse_exception_t(
            u8"Argument " + LQUOTE + arg + RQUOTE + u8" is a signed value and cannot be converted to unsigned"
        ) { }
    };

    class option_required_exception_t : public option_parse_exception_t
    {
    public:
        option_required_exception_t(const std::string& option)
        : option_parse_exception_t(
            u8"Option " + LQUOTE + option + RQUOTE + u8" is required but not present"
        ) { }
    };
}

#undef LQUOTE
#undef RQUOTE
