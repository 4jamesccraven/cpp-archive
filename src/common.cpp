#include <cctype>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <format>
#include <iostream>
#include <ranges>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <vector>
#include "common.h"

/// Shows a list of discrete options in brackets, e.g. [a|b|c].
///
/// @param opts An iterable of formattable items
template <std::ranges::range R>
requires std::formattable<std::ranges::range_value_t<R>, char>
std::string format_options(const R& opts)
{
    std::string buf = "[";
    bool first = true;

    for (const auto &opt : opts)
    {
        if (!first) buf += "|";
        std::format_to(std::back_inserter(buf), "{}", opt);
        first = false;
    }

    buf += "]";
    return buf;
}

/// Converts all strings in a range to lowercase.
///
/// @param r The input range.
/// @return A std::vector<std::string> containing the lowercased values.
template <std::ranges::range R>
requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
auto to_lower(const R& r)
{
    std::vector<std::string> result;
    result.resize(std::ranges::distance(r));

    size_t i = 0;
    for (auto && s : r)
    {
        std::string lower(s.size(), '\0');
        for (size_t j = 0; j < s.size(); j++)
        {
            lower[j] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[j])));
        }
        result[i++] = std::move(lower);
    }

    return result;
}

void prompt(std::string_view prompt_text, std::string& buffer)
{
    // Prompt the user
    std::println(stderr, "{}", prompt_text);
    std::print(stderr, "> ");
    std::cerr.flush();

    // Collect input.
    buffer.clear();
    std::getline(std::cin, buffer);
}

bool prompt(std::string_view prompt_text,
            std::initializer_list<std::string_view> true_vals,
            std::initializer_list<std::string_view> false_vals,
            std::optional<bool> default_val)
{
    // Holds user input.
    std::string input_buf{};
    // Map the caller's discrete options to lowercase values.
    auto lower_true = to_lower(true_vals);
    auto lower_false = to_lower(false_vals);
    // Precompute the error message
    auto error_message = std::format(
        "Invalid selection. Must be one of {} or {}",
        format_options(lower_true),
        format_options(lower_false)
    );

    // Continually prompt user until valid input is obtained.
    while (true)
    {
        prompt(prompt_text, input_buf);

        // If the user says nothing, and the caller provides a default,
        // return the default value
        if (input_buf.empty() && default_val.has_value())
        {
            return default_val.value();
        }

        // Check for false values first. This way, in the case of caller-side
        // ambiguity, it resolves to false by default.
        for (auto f_val : lower_false)
        {
            if (input_buf == f_val)
                return false;
        }
        // Then check for true values.
        for (auto t_val : lower_true)
        {
            if(input_buf == t_val)
                return true;
        }

        std::println(stderr, "{}", error_message);
    }
    // Unreachable.
    return (default_val.has_value()) ? default_val.value() : false;
}


std::string prompt(std::string_view prompt_text,
                   std::initializer_list<std::string_view> choices,
                   std::optional<std::string_view> default_val)
{
    std::string input_buf{};
    auto error_message = std::format(
        "Invalid selection. Must be one of {}",
        format_options(choices)
    );

    while (true)
    {
        prompt(prompt_text, input_buf);

        // If the user says nothing, and the caller provides a default,
        // return the default value
        if (input_buf.empty() && default_val.has_value())
        {
            return std::string(default_val.value());
        }

        // Check for matches
        for (const auto &choice : choices)
        {
            if (input_buf == choice)
            {
                return input_buf;
            }
        }

        std::println(stderr, "{}", error_message);
    }
    // Unreachable.
    return (default_val.has_value()) ? std::string(default_val.value()) : "error";
}

