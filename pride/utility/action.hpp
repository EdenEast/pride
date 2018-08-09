
#pragma once

#include <functional>
#include <tuple>

namespace pride
{
namespace util
{
    template<std::size_t... Ts>
    struct index_t
    {};

    template<std::size_t N, std::size_t... Ts>
    struct gen_seq_t : gen_seq_t<N - 1, N - 1, Ts...>
    {};

    template<std::size_t... Ts>
    struct gen_seq_t<0, Ts...> : index_t<Ts...>
    {};
} // namespace util

template<typename... Ts>
class action_t;

template<typename Func, typename... Args>
action_t<Args...> make_action(Func&& func, Args&&... args)
{
    return action_t<Args...>(std::forward<Func>(func), std::forward<Args>(args)...);
}

template<typename... Ts>
class action_t
{
public:
    using func_type = std::function<void(Ts...)>;
    using arg_type = std::tuple<Ts...>;

    action_t(func_type func, Ts&&... args)
        : _func(func)
        , _args(std::make_tuple(std::forward<Ts>(args)...))
    {}

    template<typename... Args, std::size_t... Is>
    void func(std::tuple<Args...>& tup, util::index_t<Is...>)
    {
        _func(std::get<Is>(tup)...);
    }

    template<typename... Args>
    void func(std::tuple<Args...>& tup)
    {
        func(tup, util::gen_seq_t<sizeof...(Args)>{});
    }

    void act()
    {
        func(_args);
    }

private:
    func_type _func;
    arg_type _args;
};
} // namespace pride
