
#pragma once

#include "../console.hpp"
#include "stringify.hpp"
#include <cstring>
#include <iterator>
#include <type_traits>

namespace pride::assert::internal
{
template<typename Type>
class expression_lhs;
class bool_expression_t;
class expresstion_decomposer_t;
struct violation_info_t;

class bool_expression_t
{
    template<typename>
    struct tfalse
    {
        static constexpr bool value = false;
    };

public:
    struct const_iterator : std::iterator<std::forward_iterator_tag, const stringifier_t>
    {
        const_iterator() noexcept;
        const_iterator(const stringifier_t* current) noexcept;

        const_iterator& operator++() noexcept;
        const_iterator operator++(int) noexcept;

        stringifier_t const& operator*() const noexcept;
        stringifier_t const* operator->() const noexcept;

        bool operator==(const const_iterator& rhs) const noexcept;
        bool operator!=(const const_iterator& rhs) const noexcept;

    private:
        const stringifier_t* current;
    };

    using iterator = const_iterator;

    template<typename Type>
    explicit bool_expression_t(expression_lhs<Type> expression);

    template<typename Type>
    explicit bool_expression_t(Type&& value);

    template<typename TL, typename TO, typename TR>
    bool_expression_t(TL&& lhs, TO&& op, TR&& rhs);

    bool_expression_t(bool_expression_t&& rhs) noexcept;
    ~bool_expression_t() noexcept = default;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    explicit operator bool() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const bool_expression_t& expr);

    // Makeing sure that only comparison operators are supported and all
    // other operators will static assert
    template<typename R>
    int operator&(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator^(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator|(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator&&(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator||(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator+=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator-=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator*=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator/=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator%=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator<<=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator>>=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator&=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator^=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }
    template<typename R>
    int operator|=(const R&)
    {
        static_assert(tfalse<R>::value, "Expression too complex: rewrite as binary comparison");
        return int();
    }

private:
    template<typename Type>
    static std::unique_ptr<stringifier_t> create_expression_list(Type&& value);

    template<typename TL, typename TO, typename TR>
    static std::unique_ptr<stringifier_t> create_expression_list(TL&& tl, TO&& op, TR&& tr);
    std::unique_ptr<stringifier_t> fail_expression;
};

template<typename Type>
class expression_lhs
{
public:
    expression_lhs(Type lhs)
        : value(std::move(lhs))
    {}

    template<typename R>
    friend bool_expression_t operator==(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_eq_t(), std::forward<R>(rhs)); }
    template<typename R>
    friend bool_expression_t operator!=(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_ne_t(), std::forward<R>(rhs)); }
    template<typename R>
    friend bool_expression_t operator<(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_lt_t(), std::forward<R>(rhs)); }
    template<typename R>
    friend bool_expression_t operator<=(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_le_t(), std::forward<R>(rhs)); }
    template<typename R>
    friend bool_expression_t operator>(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_gt_t(), std::forward<R>(rhs)); }
    template<typename R>
    friend bool_expression_t operator>=(expression_lhs<Type> lhs, R&& rhs) { return bool_expression_t(std::move(lhs.value), compare_ge_t(), std::forward<R>(rhs)); }

    template<typename R>
    friend auto operator<<(expression_lhs<Type> lhs, R&& rhs) -> expression_lhs<decltype(Type() << std::forward<R>(rhs))>
    {
        return std::move(lhs.value) << std::forward<R>(rhs);
    }

    template<typename R>
    friend auto operator>>(expression_lhs<Type> lhs, R&& rhs) -> expression_lhs<decltype(Type() >> std::forward<R>(rhs))>
    {
        return std::move(lhs.value) >> std::forward<R>(rhs);
    }

private:
    friend bool_expression_t;
    Type value;
};

// --------------------------------------------------------------------------------
// expression_decomposer

struct expression_decomposer_t
{
    template<typename T>
    expression_lhs<typename std::remove_reference<T>::type> operator<<(T&& lhs)
    {
        return expression_lhs<typename std::remove_reference<T>::type>(std::forward<T>(lhs));
    }
};

// --------------------------------------------------------------------------------
// expression_output

struct expression_output_t
{
    expression_output_t(const char* expr_str, const bool_expression_t& expr);
    friend std::ostream& operator<<(std::ostream& os, const expression_output_t& expr);

    const char* lhs;
    const char* end;
    const char* op;
    const char* rhs;
};

// --------------------------------------------------------------------------------
// bool_expression_t

template<typename Type>
inline bool_expression_t::bool_expression_t(expression_lhs<Type> expression)
    : bool_expression_t(std::move(expression.value))
{
}

template<typename Type>
inline bool_expression_t::bool_expression_t(Type&& val)
    : fail_expression(val ? nullptr : create_expression_list(std::forward<Type>(val)))
{
}

template<typename TL, typename TO, typename TR>
inline bool_expression_t::bool_expression_t(TL&& lhs, TO&& op, TR&& rhs)
    : fail_expression(op(lhs, rhs) ? nullptr : create_expression_list(std::forward<TL>(lhs), std::forward<TO>(op), std::forward<TR>(rhs)))
{
}

inline bool_expression_t::bool_expression_t(bool_expression_t&& rhs) noexcept
    : fail_expression(std::move(rhs.fail_expression))
{
}

inline bool_expression_t::const_iterator bool_expression_t::begin() const noexcept
{
    return fail_expression.get();
}

inline bool_expression_t::const_iterator bool_expression_t::end() const noexcept
{
    return const_iterator();
}

inline bool_expression_t::operator bool() const noexcept
{
    return !fail_expression;
}

inline std::ostream& operator<<(std::ostream& os, const bool_expression_t& expr)
{
    if (expr.begin() == expr.end())
    {
        os << console::fg_t::cyan;
        if (os.good())
            os << true;
    }

    bool is_operator = false;
    bool first = true;
    for (const auto& token : expr)
    {
        if (!first)
            os << os.fill();
        else
            first = false;

        os << (is_operator ? console::fg_t::yellow : console::fg_t::cyan);
        if (os.good())
            os << token;

        is_operator = !is_operator;
    }

    // return os back to reset
    return os << console::fg_t::reset;
}

template<typename Type>
std::unique_ptr<stringifier_t> bool_expression_t::create_expression_list(Type&& value)
{
    return std::unique_ptr<string_maker<Type>>(new string_maker<Type>(std::forward<Type>(value)));
}

template<typename TL, typename TO, typename TR>
std::unique_ptr<stringifier_t> bool_expression_t::create_expression_list(TL&& tl, TO&& op, TR&& tr)
{
    // Constructing in reverse order becuase this is built as a link list with nodes
    std::unique_ptr<stringifier_t> expr(new string_maker<TR>(std::forward<TR>(tr)));
    expr.reset((new string_maker<TO>)->set_next(std::move(expr)));
    expr.reset((new string_maker<TL>(std::forward<TL>(tl)))->set_next(std::move(expr)));
    return expr;
}

// --------------------------------------------------------------------------------
// bool_expression_t::const_iterator

inline bool_expression_t::const_iterator::const_iterator() noexcept
    : current(nullptr)
{
}

inline bool_expression_t::const_iterator::const_iterator(const stringifier_t* current) noexcept
    : current(current)
{
}

inline bool_expression_t::const_iterator& bool_expression_t::const_iterator::operator++() noexcept
{
    current = current->next.get();
    return *this;
}

inline bool_expression_t::const_iterator bool_expression_t::const_iterator::operator++(int) noexcept
{
    const_iterator prev(*this);
    ++*this;
    return prev;
}

inline stringifier_t const& bool_expression_t::const_iterator::operator*() const noexcept
{
    return *current;
}

inline stringifier_t const* bool_expression_t::const_iterator::operator->() const noexcept
{
    return current;
}

inline bool bool_expression_t::const_iterator::operator==(const const_iterator& rhs) const noexcept
{
    return current == rhs.current;
}

inline bool bool_expression_t::const_iterator::operator!=(const const_iterator& rhs) const noexcept
{
    return current != rhs.current;
}

// --------------------------------------------------------------------------------
// expression_output

inline expression_output_t::expression_output_t(const char* expr_str, const bool_expression_t& expr)
    : lhs(expr_str)
    , end(expr_str + strlen(expr_str))
    , op(end)
    , rhs(end)
{
    for (const auto& token : expr)
    {
        const auto opt = dynamic_cast<const string_maker_op_t*>(&token);
        if (!opt)
            continue;

        // there are mulltiple operators found in the expression
        // dont have to seperate them into their sides
        if (op != end)
        {
            rhs = op = end;
            return;
        }

        auto op_str = opt->str();
        op = strstr(lhs, op_str);
        if (!op)
        {
            op = end;
            continue;
        }

        rhs = op + strlen(op_str);
    }
}

inline std::ostream& operator<<(std::ostream& os, const expression_output_t& expr)
{
    os << console::fg_t::cyan;
    if (os.good())
        os.write(expr.lhs, expr.op - expr.lhs);

    os << console::fg_t::yellow;
    if (os.good())
        os.write(expr.op, expr.rhs - expr.op);

    os << console::fg_t::cyan;
    if (os.good())
        os.write(expr.rhs, expr.end - expr.rhs);

    return os << console::fg_t::reset;
}
} // namespace pride::assert::internal
