
#pragma once

#include <iostream>
#include <memory>

namespace pride::assert::internal
{
struct stringifier_t
{
    virtual ~stringifier_t() noexcept;
    virtual std::ostream& convert(std::ostream& os) const = 0;

private:
    friend class bool_expression_t;
    stringifier_t* set_next(std::unique_ptr<stringifier_t> next) noexcept;
    std::unique_ptr<stringifier_t> next;
};

inline std::ostream& operator<<(std::ostream& os, const stringifier_t& str)
{
    return str.convert(os);
}

template<typename Type>
struct string_maker : stringifier_t
{
    explicit string_maker(Type val);
    virtual std::ostream& convert(std::ostream& os) const override;

private:
    Type value;
};

// This is for the operators
struct string_maker_op_t : stringifier_t
{
    virtual std::ostream& convert(std::ostream& os) const override;
    virtual const char* str() const noexcept = 0;
};

// Create string maker for built in types
// template struct string_maker<bool>;
template struct string_maker<short>;
template struct string_maker<unsigned short>;
template struct string_maker<int>;
template struct string_maker<unsigned int>;
template struct string_maker<long>;
template struct string_maker<unsigned long>;
template struct string_maker<long long>;
template struct string_maker<unsigned long long>;
template struct string_maker<float>;
template struct string_maker<double>;
template struct string_maker<long double>;
template struct string_maker<void*>;
template struct string_maker<const void*>;
template struct string_maker<char>;
template struct string_maker<signed char>;
template struct string_maker<unsigned char>;
template struct string_maker<const char*>;
template struct string_maker<const signed char*>;
template struct string_maker<const unsigned char*>;

// comparitor functions
struct compare_eq_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs == rhs; }
};
struct compare_ne_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs != rhs; }
};
struct compare_lt_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs < rhs; }
};
struct compare_le_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs <= rhs; }
};
struct compare_gt_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs > rhs; }
};
struct compare_ge_t
{
    template<class TL, class TR>
    bool operator()(const TL& lhs, const TR& rhs) const { return lhs >= rhs; }
};

template<>
struct string_maker<compare_eq_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return "=="; }
};
template<>
struct string_maker<compare_ne_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return "!="; }
};
template<>
struct string_maker<compare_lt_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return "<"; }
};
template<>
struct string_maker<compare_le_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return "<="; }
};
template<>
struct string_maker<compare_gt_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return ">"; }
};
template<>
struct string_maker<compare_ge_t> : string_maker_op_t
{
    virtual const char* str() const noexcept override { return ">="; }
};

// --------------------------------------------------------------------------------
// stringifier_t

inline stringifier_t::~stringifier_t() noexcept
{
    while (next)
    {
        std::unique_ptr<stringifier_t> const to_delete(std::move(next));

        // delete on the next interation and prevent recursion of unknown depth
        next = std::move(to_delete->next);
    }
}

inline stringifier_t* stringifier_t::set_next(std::unique_ptr<stringifier_t> next) noexcept
{
    this->next = std::move(next);
    return this;
}

// --------------------------------------------------------------------------------
// string_maker

template<typename Type>
string_maker<Type>::string_maker(Type val)
    : value(std::move(val))
{
}

template<typename Type>
inline std::ostream& string_maker<Type>::convert(std::ostream& os) const
{
    return os << value;
}

// make string return true or false for bool
template<>
inline std::ostream& string_maker<bool>::convert(std::ostream& os) const
{
    return value ? os << "true" : os << "false";
}

// --------------------------------------------------------------------------------
// string_maker_op_t

inline std::ostream& string_maker_op_t::convert(std::ostream& os) const
{
    return os << str();
}
} // namespace pride::assert::internal
