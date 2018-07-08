
#pragma once

#include "option.hpp"
#include "value.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pride::cmd
{
    class cmd_t;
    class parse_result_t;
    class opt_adder_t;

    namespace detail
    {
        // regex to parse a command line option. The capture groups are as follows:
        //   --([a-zA-Z0-9][-_a-zA-Z0-9]*)(?:=(.*))?|-([a-zA-Z0-9]+)
        //
        //   --([a-zA-Z0-9][-_a-zA-Z0-9]*) ; match '--' then match [a-zA-Z0-9] for the first character and everything and [-_]
        //   (?:=(.*))?                    ; if there is a = then capture the rest of the characters
        //   |                             ; it can match what has already come before OR what is after
        //   -([a-zA-Z0-9]+)               ; match - and then at least [a-zA-Z0-9]
        // To get more information about this with live demo https://regex101.com/r/Ip4VXu/2
        static std::basic_regex<char> arg_pattern("--([a-zA-Z0-9][-_a-zA-Z0-9]*)(?:=(.*))?|-([a-zA-Z0-9]+)");
        static std::basic_regex<char> option_pattern("(([a-zA-Z0-9]),)?[ ]*([a-zA-Z0-9][-_a-zA-Z0-9]*)?");

        constexpr size_t longest_option = 30;
        constexpr size_t description_gap = 2;
        constexpr size_t widest_description = 80;

        struct help_option_info_t
        {
            std::string s;
            std::string l;
            std::string desc;
            bool has_default;
            std::string default_value;
            bool has_implicit;
            std::string implicit_value;
            std::string arg_help;
            bool is_container;
            bool is_bool;
        };

        struct help_group_info_t
        {
            std::string name;
            std::string description;
            std::vector<help_option_info_t> info;
        };

        inline std::string format_option(const help_option_info_t& info);
        inline std::string format_description(const help_option_info_t& info, size_t start, size_t width);
    }

    class cmd_t
    {
    public:
        cmd_t(std::string program, std::string help_string = "");

        cmd_t& positional_help(std::string help_string) { _positional_help = std::move(help_string); return *this; }
        cmd_t& custom_help(std::string help_string) { _help_string = std::move(help_string); return *this; }
        cmd_t& show_positional_help() { _show_positional = true; return *this; }
        cmd_t& allow_unrecognised_options() { _allow_unrecognised = true; return *this; }

        parse_result_t parse(int& argc, char**& argv);
        opt_adder_t add_options(std::string group = "");
        void add_option(const std::string& group, const std::string& s, const std::string& l,
            const std::string& desc, std::shared_ptr<const value_t> value, const std::string& arg_help);

        void parse_positional(const std::string& option);
        void parse_positional(const std::vector<std::string> options);
        void parse_positional(const std::initializer_list<std::string> options);

        std::string help(const std::vector<std::string>& groups = {""}) const;

    private:
        void add_one_option(const std::string& option, std::shared_ptr<option_t> details);
        std::string help_one_group(const std::string& group) const;
        void generate_group_help(std::string& result, const std::vector<std::string>& groups) const;
        void generate_all_help(std::string& result) const;

    private:
        std::string _program;
        std::string _help_string;
        std::string _custom_help;
        std::string _positional_help;
        bool _show_positional = false;
        bool _allow_unrecognised = false;

        std::unordered_map<std::string, std::shared_ptr<option_t>> _options;
        std::vector<std::string> _positional;
        std::vector<std::string>::iterator _next_positional;
        std::unordered_set<std::string> _positional_set;

        std::unordered_map<std::string, detail::help_group_info_t> _help;
    };

    class parse_result_t
    {
    public:
        parse_result_t(
            const std::unordered_map<std::string, std::shared_ptr<option_t>>&,
            std::vector<std::string> positoinal,
            bool allow_unrecognised,
            int& argc, char**& argv
        );

        size_t count(const std::string& opt) const;
        const option_value_t operator[](const std::string& option) const;
        const std::vector<key_value_t>& arguments() const { return _sequential; }

    private:
        // option_value_t& get_option(std::shared_ptr<option_t>);
        void add_to_option(const std::string& option, const std::string& args);
        bool consume_positional(std::string p);

        void parse(int& argc, char**& argv);
        void parse_option(std::shared_ptr<option_t> value, const std::string& name, const std::string& arg);
        void parse_default(std::shared_ptr<option_t> opt);

        void checked_parse_arg(int argc, char* argv[], int& current, std::shared_ptr<option_t> value, const std::string& name);

        const std::unordered_map<std::string, std::shared_ptr<option_t>>& _options;
        std::vector<std::string> _positional;
        std::vector<std::string>::iterator _next_positional;
        std::unordered_set<std::string> _positional_set;
        std::unordered_map<std::shared_ptr<option_t>, option_value_t> _results;
        bool _allow_unrecognised;
        std::vector<key_value_t> _sequential;
    };

    class opt_adder_t
    {
    public:
        opt_adder_t(cmd_t& cmd, std::string group)
        :_cmd(cmd)
        , _group(std::move(group))
        { }

        opt_adder_t& operator()(
            const std::string& opts,
            const std::string& desc,
            std::shared_ptr<const value_t> _value = pride::cmd::value<bool>(),
            std::string help_str = ""
        )
        {
        std::match_results<const char*> result;
        std::regex_match(opts.c_str(), result, detail::option_pattern);

        if (result.empty())
            throw invalid_option_format_error_t(opts);

        const auto& short_match = result[2];
        const auto& long_match = result[3];

        if (!short_match.length() && !long_match.length())
            throw invalid_option_format_error_t(opts);
        else if (long_match.length() == 1 && short_match.length())
            throw invalid_option_format_error_t(opts);

        auto option_name = long_match.length() == 1 ?
            std::make_pair(long_match.str(), short_match.str()) : std::make_pair(short_match.str(), long_match.str());

        _cmd.add_option(_group, option_name.first, option_name.second, desc, _value, help_str);
        return *this;
    }

    private:
        cmd_t& _cmd;
        std::string _group;
    };

    //
    // IMPLEMENTATIONS   ---------------------------------------------------------------------
    //

    namespace detail
    {
        inline std::string format_option(const help_option_info_t& info)
        {
            auto& s = info.s;
            auto& l = info.l;

            std::string result = "  ";

            if (s.size() > 0)
                result += "-" + s + ",";
            else
                result += "   ";

            if (l.size() > 0)
                result += " --" + l;

            auto arg = info.arg_help.size() > 0 ? info.arg_help : "arg";

            if (info.is_bool)
            {
                if (info.has_implicit)
                    result += " [=" + arg + "(=" + info.implicit_value + ")]";
                else
                    result += " " + arg;
            }
            return result;
        }

        inline std::string format_description(const help_option_info_t& info, size_t start, size_t width)
        {
            auto desc = info.desc;

            if (info.has_default && (!info.is_bool || info.default_value != "false"))
                desc += " (default: " + info.default_value + ")";

            std::string result;
            auto current = std::begin(desc);
            auto startline = current;
            auto lastspace = current;
            auto size = size_t{};

            while (current != std::end(desc))
            {
                if (*current == ' ')
                    lastspace = current;

                if (size > width)
                {
                    if (lastspace == startline)
                    {
                        result.append(startline, current + 1);
                        result.append("\n");
                        result.append(start, ' ');
                        startline = current + 1;
                        lastspace = startline;
                    }
                    else
                    {
                        result.append(startline, lastspace);
                        result.append("\n");
                        result.append(start, ' ');
                        startline = lastspace + 1;
                    }
                    size = 0;
                }
                else
                {
                    ++size;
                }
                ++current;
            }
            result.append(startline, current);
            return result;
        }
    }

    // cmd implementation   ----------------------------------------------------

    inline cmd_t::cmd_t(std::string program, std::string help_string)
    : _program(std::move(program))
    , _help_string(std::move(help_string))
    , _custom_help("[OPTION...]")
    , _positional_help("positional params")
    , _next_positional(_positional.end())
    {
    }

    inline parse_result_t cmd_t::parse(int& argc, char**& argv)
    {
        parse_result_t result(_options, _positional, _allow_unrecognised, argc, argv);
        return result;
    }

    inline opt_adder_t cmd_t::add_options(std::string group)
    {
        return opt_adder_t(*this, std::move(group));
    }

    inline void cmd_t::add_option(const std::string& group, const std::string& s, const std::string& l,
        const std::string& desc, std::shared_ptr<const value_t> value, const std::string& arg_help)
    {
        auto option = std::make_shared<option_t>(s, l, desc, value);

        if (s.size() > 0)
            add_one_option(s, option);

        if (l.size() > 0)
            add_one_option(l, option);

        auto& help_group = _help[group];
        help_group.info.emplace_back(detail::help_option_info_t{
            s, l, desc,
            value->has_default(), value->default_value(),
            value->has_implicit(), value->implicit_value(),
            std::move(arg_help),
            value->is_container(), value->is_bool()
        });
    }
    inline void cmd_t::parse_positional(const std::string& option)
    {
        parse_positional(std::vector<std::string>{std::move(option)});
    }

    inline void cmd_t::parse_positional(const std::vector<std::string> options)
    {
        _positional = std::move(options);
        _next_positional = _positional.begin();
        _positional_set.insert(_positional.begin(), _positional.end());
    }

    inline void cmd_t::parse_positional(const std::initializer_list<std::string> options)
    {
        parse_positional(std::vector<std::string>(std::move(options)));
    }

    inline std::string cmd_t::help(const std::vector<std::string>& groups) const
    {
        std::string result = _help_string + "\nUsage:\n  " + _program + " " + _custom_help;
        if (_positional.size() > 0 && _positional_help.size() > 0)
            result += " " + _positional_help;

        result += "\n\n";

        if (groups.size() == 0)
            generate_all_help(result);
        else
            generate_group_help(result, groups);

        return result;
    }

    inline void cmd_t::add_one_option(const std::string& option, std::shared_ptr<option_t> details)
    {
        auto in = _options.emplace(option, details);
        if (!in.second)
            throw option_exists_error_t(option);
    }

    inline std::string cmd_t::help_one_group(const std::string& g) const
    {
        auto group = _help.find(g);
        if (group == _help.end())
            return "";

        std::vector<std::pair<std::string, std::string>> format;
        size_t longest = 0;
        std::string result;

        if (g.empty())
            result += " " + g + " options:\n";

        for (const auto& opt : group->second.info)
        {
            if (opt.is_container && _positional_set.find(opt.l) != _positional_set.end() && !_show_positional)
                continue;

            // auto s format_option
            auto s = detail::format_option(opt);
            longest = std::max(longest, s.size());
            format.push_back(std::make_pair(s, std::string{}));
        }

        longest = std::min(longest, detail::longest_option);
        auto allowed_width = detail::widest_description - longest - detail::description_gap;

        auto filter = format.begin();
        for (const auto& opt : group->second.info)
        {
            if (opt.is_container && _positional_set.find(opt.l) != _positional_set.end() && !_show_positional)
                continue;

            auto d = format_description(opt, longest + detail::description_gap, allowed_width);
            result += filter->first;

            if (filter->first.length() > longest)
            {
                result += '\n';
                result += std::string(longest + detail::description_gap, ' ');
            }
            else
            {
                result += std::string(longest + detail::description_gap - filter->first.length(), ' ');
            }

            result += d;
            result += '\n';
            ++filter;
        }
        return result;
    }

    inline void cmd_t::generate_group_help(std::string& result, const std::vector<std::string>& groups) const
    {
        for (size_t i = 0; i != groups.size(); ++i)
        {
            const std::string& group_help_text = help_one_group(groups[i]);
            if (group_help_text.empty())
                continue;

            result += group_help_text;
            if (i < groups.size() - 1)
                result += '\n';
        }
    }

    inline void cmd_t::generate_all_help(std::string& result) const
    {
        std::vector<std::string> all_groups;
        all_groups.reserve(_help.size());
        for (auto& group : _help)
            all_groups.push_back(group.first);

        generate_group_help(result, all_groups);
    }

    // parse_result_t implementation   -------------------------------------------

    inline parse_result_t::parse_result_t(
        const std::unordered_map<std::string, std::shared_ptr<option_t>>& option,
        std::vector<std::string> positoinal,
        bool allow_unrecognised,
        int& argc, char**& argv
    )
    : _options(option)
    , _positional(std::move(positoinal))
    , _next_positional(_positional.end())
    , _allow_unrecognised(allow_unrecognised)
    {
        parse(argc, argv);
    }

    inline size_t parse_result_t::count(const std::string& opt) const
    {
        auto iter = _options.find(opt);
        if (iter == _options.end())
            return 0;
        return _results.find(iter->second)->second.count();
    }

    inline const option_value_t parse_result_t::operator[](const std::string& opt) const
    {
        auto iter = _options.find(opt);
        if (iter == _options.end())
            throw option_not_present_exception_t(opt);
        return _results.find(iter->second)->second;
    }

    inline void parse_result_t::add_to_option(const std::string& option, const std::string& arg)
    {
        auto iter = _options.find(option);
        if (iter == _options.end())
            throw option_not_exists_exception_t(option);

        parse_option(iter->second, option, arg);
    }

    inline bool parse_result_t::consume_positional(std::string p)
    {
        while (_next_positional != _positional.end())
        {
            auto iter = _options.find(*_next_positional);
            if (iter != _options.end())
            {
                auto& result = _results[iter->second];
                if (!iter->second->value().is_container())
                {
                    if (result.count() == 0)
                    {
                        add_to_option(*_next_positional, p);
                        ++_next_positional;
                        return true;
                    }
                    else
                    {
                        ++_next_positional;
                        continue;
                    }
                }
                else
                {
                    add_to_option(*_next_positional, p);
                    return true;
                }
            }
            ++_next_positional;
        }
        return false;
    }

    inline void parse_result_t::parse(int& argc, char**& argv)
    {
        int current = 1;
        int next_keep = 1;
        bool consume_remaining = false;

        while (current != argc)
        {
            if (strcmp(argv[current], "--") == 0)
            {
                consume_remaining = true;
                ++current;
                break;
            }

            std::match_results<const char*> result;
            std::regex_match(argv[current], result, detail::arg_pattern);

            if (result.empty())
            {
                // this is not an optional flag with -|--
                // if consume_positional is true then it was consumed, otherwise it is ignored
                if (!consume_positional(argv[current]))
                {
                    argv[next_keep] = argv[current];
                    ++next_keep;
                }
            }
            else
            {
                // is it short or long?
                if (result[3].length() != 0)
                {
                    const std::string& s = result[3];

                    for (std::size_t i = 0; i != s.size(); ++i)
                    {
                        std::string name(1, s[i]);
                        auto iter = _options.find(name);

                        if (iter == _options.end())
                        {
                            if (_allow_unrecognised)
                                continue;
                            else
                                throw option_not_exists_exception_t(name);
                        }

                        auto value = iter->second;
                        if (i + 1 == s.size())
                            checked_parse_arg(argc, argv, current, value, name);
                        else if (value->value().has_implicit())
                            parse_option(value, name, value->value().implicit_value());
                        else
                            throw option_requires_argument_exception_t(name);
                    }
                }
                else if (result[1].length() != 0)
                {
                    const std::string& name = result[1];
                    auto iter = _options.find(name);

                    if (iter == _options.end())
                    {
                        if (_allow_unrecognised)
                        {
                            // keep unrecongnised option in argument list, then move to the next one
                            argv[next_keep] = argv[current];
                            ++next_keep;
                            ++current;
                            continue;
                        }
                        else
                        {
                            throw option_not_exists_exception_t(name);
                        }
                    }

                    auto opt = iter->second;

                    if (result[2].length() != 0)
                        parse_option(opt, name, result[3]);
                    else
                        checked_parse_arg(argc, argv, current, opt, name);
                }
            }
            ++current;
        }

        for (auto& opt : _options)
        {
            auto& info = opt.second;
            auto& value = info->value();
            auto& store = _results[info];

            if (!store.count() && value.has_default())
                parse_default(info);
        }

        if (consume_remaining)
        {
            while (current < argc)
            {
                if (!consume_positional(argv[current]))
                    break;
                ++current;
            }

            while (current != argc)
            {
                argv[next_keep] = argv[current];
                ++next_keep;
                ++current;
            }
        }

        argc = next_keep;
    }

    inline void parse_result_t::parse_option(std::shared_ptr<option_t> value, const std::string& name, const std::string& arg)
    {
        auto& result = _results[value];
        result.parse(value, arg);
        _sequential.emplace_back(value->long_name(), arg);
    }

    inline void parse_result_t::parse_default(std::shared_ptr<option_t> opt)
    {
        _results[opt].parse_default(opt);
    }

    inline void parse_result_t::checked_parse_arg(int argc, char* argv[], int& current, std::shared_ptr<option_t> value, const std::string& name)
    {
        if (current + 1 >= argc)
        {
            if (value->value().has_implicit())
                parse_option(value, name, value->value().implicit_value());
            else
                throw missing_argument_exception_t(name);
        }
        else
        {
            if (value->value().has_implicit())
            {
                parse_option(value, name, value->value().implicit_value());
            }
            else
            {
                parse_option(value, name, argv[current + 1]);
                ++current;
            }
        }
    }
}
