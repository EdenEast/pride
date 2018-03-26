
#pragma once

#include "terminal.hpp"
#include <atomic>
#include <iostream>

namespace pride { namespace console
{
    enum class style_t
    {
        reset     = 0,
        bold      = 1,
        dim       = 2,
        italic    = 3,
        underline = 4,
        blink     = 5,
        rblink    = 6,
        reversed  = 7,
        conceal   = 8,
        crossed   = 9
    };

    enum class fg_t
    {
        black   = 30,
        red     = 31,
        green   = 32,
        yellow  = 33,
        blue    = 34,
        magenta = 35,
        cyan    = 36,
        grey    = 37,
        reset   = 39
    };

    enum class bg_t
    {
        black   = 40,
        red     = 41,
        green   = 42,
        yellow  = 43,
        blue    = 44,
        magenta = 45,
        cyan    = 46,
        grey    = 47,
        reset   = 49
    };

    enum class fgb_t
    {
        black   = 90,
        red     = 91,
        green   = 92,
        yellow  = 93,
        blue    = 94,
        magenta = 95,
        cyan    = 96,
        grey    = 97
    };

    enum class bgb_t
    {
        black   = 100,
        red     = 101,
        green   = 102,
        yellow  = 103,
        blue    = 104,
        magenta = 105,
        cyan    = 106,
        grey    = 107
    };

    enum class control_t
    {
        off       = 0,
        automatic = 1,
        force     = 2
    };

    enum class win_term_t
    {
        automatic = 0,
        ansi      = 1,
        native    = 2
    };

    namespace impl
    {
        // A check to see if the type is a color enum class type
        template <typename T>
        using enable_std_t = typename std::enable_if<
            std::is_same<T, style_t>::value || std::is_same<T, fg_t>::value
            || std::is_same<T, bg_t>::value || std::is_same<T, fgb_t>::value
            || std::is_same<T, bgb_t>::value,
            std::ostream &>::type;

        template<typename Type>
        enable_std_t<Type> set_color(std::ostream& os, const Type value);

        inline std::atomic<control_t> &control_mode() noexcept
        {
            static std::atomic<control_t> value(control_t::automatic);
            return value;
        }

        inline std::atomic<win_term_t> &win_term_mode() noexcept
        {
            static std::atomic<win_term_t> value(win_term_t::automatic);
            return value;
        }

#if defined(PRI_OS_WINDOWS)
        struct sgr_t
        {
            BYTE fg_color;
            BYTE bg_color;
            BYTE bold;
            BYTE underline;
            BOOLEAN inverse;
            BOOLEAN conceal;
        };

        enum class attr_color_t
        {
            black   = 0,
            red     = 4,
            green   = 2,
            yellow  = 6,
            blue    = 1,
            magenta = 5,
            cyan    = 3,
            grey    = 7
        };

        inline const sgr_t &default_state() noexcept
        {
            static const sgr_t default_sgr = []() {
                CONSOLE_SCREEN_BUFFER_INFO info;
                WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)
                    || GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &info))
                    att = info.wAttributes;

                sgr_t sgr = { 0, 0, 0, 0, FALSE, FALSE };
                sgr.fg_color = att & 0x0F;
                sgr.bg_color = (att & 0xF0) >> 4;
                return sgr;
            }();
            return default_sgr;
        }

        inline BYTE ansi_2_attr(BYTE rgb) noexcept
        {
            static const attr_color_t rev[8] = {
                attr_color_t::black, attr_color_t::red,
                attr_color_t::green, attr_color_t::yellow,
                attr_color_t::blue, attr_color_t::magenta,
                attr_color_t::cyan, attr_color_t::grey
            };
            return static_cast<BYTE>(rev[rgb]);
        }

        inline void set_win_sgr(fg_t color, sgr_t& state) noexcept
        {
            if (color != fg_t::reset)
                state.fg_color = ansi_2_attr(static_cast<BYTE>(color) - 30);
            else
                state.fg_color = default_state().fg_color;
        }

        inline void set_win_sgr(bg_t color, sgr_t& state) noexcept
        {
            if (color != bg_t::reset)
                state.bg_color = ansi_2_attr(static_cast<BYTE>(color) - 40);
            else
                state.bg_color = default_state().bg_color;
        }

        inline void set_win_sgr(fgb_t color, sgr_t& state) noexcept
        {
            state.fg_color = FOREGROUND_INTENSITY | ansi_2_attr(static_cast<BYTE>(color) - 90);
        }

        inline void set_win_sgr(bgb_t color, sgr_t& state) noexcept
        {
            state.bg_color = (BACKGROUND_INTENSITY >> 4) | ansi_2_attr(static_cast<BYTE>(color) - 100);
        }

        inline void set_win_sgr(style_t style, sgr_t& state) noexcept
        {
            switch (style)
            {
            case style_t::reset: state = default_state(); break;
            case style_t::bold: state.bold = FOREGROUND_INTENSITY; break;
            case style_t::underline:
            case style_t::blink:
                state.underline = BACKGROUND_INTENSITY; break;
            case style_t::reversed: state.inverse = TRUE; break;
            case style_t::conceal: state.conceal = TRUE; break;
            default: break;
            }
        }

        inline sgr_t& current_state() noexcept
        {
            static sgr_t state = default_state();
            return state;
        }

        inline WORD sgr_2_attr(const sgr_t& state) noexcept
        {
            WORD att = 0;
            if (state.conceal) 
            {
                if (state.inverse) 
                {
                    att = (state.fg_color << 4) | state.fg_color;
                    if (state.bold)
                        att |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
                }
                else
                {
                    att = (state.bg_color << 4) | state.bg_color;
                    if (state.underline)
                        att |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
                }
            }
            else if (state.inverse)
            {
                att = (state.fg_color << 4) | state.bg_color;
                if (state.bold) att |= BACKGROUND_INTENSITY;
                if (state.underline) att |= FOREGROUND_INTENSITY;
            }
            else
            {
                att = state.fg_color | (state.bg_color << 4) | state.bold | state.underline;
            }
            return att;
        }

        template<typename Type>
        inline void set_win_color_ansi(std::ostream& os, const Type value)
        {
            os << "\033[" << static_cast<int>(value) << 'm';
        }

        template<typename Type>
        inline void set_win_color_native(std::ostream& os, const Type value)
        {
            const HANDLE h = get_console_handle(os.rdbuf());
            if (h != INVALID_HANDLE_VALUE)
            {
                set_win_sgr(value, current_state());

                // flush all buffered text to console with prev settings
                os.flush();
                SetConsoleTextAttribute(h, sgr_2_attr(current_state()));
            }
        }

        template<typename Type>
        inline enable_std_t<Type> set_color(std::ostream& os, const Type value)
        {
            if (win_term_mode() == win_term_t::automatic)
            {
                if (support_ansi(os.rdbuf()))
                    set_win_color_ansi(os, value);
                else
                    set_win_color_native(os, value);
            }
            else if (win_term_mode() == win_term_t::ansi)
            {
                set_win_color_ansi(os, value);
            }
            else
            {
                set_win_color_native(os, value);
            }
            return os;
        }
#else
    template<typename Type>
    inline enable_std_t<Type> set_color(std::ostream& os, const Type value)
    {
        return os << "\033[" << static_cast<int>(value) << 'm';
    }
#endif
    } // namespace impl

    template<typename Type>
    impl::enable_std_t<Type> operator<<(std::ostream& os, const Type value)
    {
        const control_t control = impl::control_mode();
        switch (control)
        {
        case control_t::automatic: 
            return has_color_support() && is_terminal(os.rdbuf()) ?
                impl::set_color(os, value) : os;
        case control_t::force:
            return impl::set_color(os, value);
        default:
            return os;
        }
    }

    inline void set_control_mode(const control_t mode) noexcept
    {
        impl::control_mode() = mode;
    }
    inline void set_win_term_mode(const win_term_t mode) noexcept
    {
        impl::win_term_mode() = mode;
    }
}}
