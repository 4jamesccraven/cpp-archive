#ifndef CPP_ARCHIVE_COMMON
#define CPP_ARCHIVE_COMMON

#include <cstdio>
#include <initializer_list>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>

using MainType = int(*)();

/// A struct for registering and distpatching each projects' main function.
struct ProjectDispatch
{
    static std::unordered_map<std::string, MainType>& get() {
        static std::unordered_map<std::string, MainType> map;
        return map;
    }

    ProjectDispatch(const std::string& name, MainType f)
    {
        get()[name] = f;
    }
};

/// A helper function that displays a prompt and sends user input to a buffer.
///
/// @param prompt_text The prompt to display
/// @param buffer Where to send user input to.
void prompt(std::string_view prompt_text, std::string& buffer);

/// Prompt the user for a value that is one of two specific values
/// note: true_vals and false_vals are case insensitive.
///
/// @param prompt_text The text displayed to the user
/// @param true_vals The strings that should be considered "true"
/// @param false_vals The strings that should be considered "false"
/// @param default_val What the value should be if the user doesn't
///                    enter anything. Not set by default.
bool prompt(std::string_view prompt_text,
            std::initializer_list<std::string_view> true_vals,
            std::initializer_list<std::string_view> false_vals,
            std::optional<bool> default_val = std::nullopt);

/// Prompt the user for one of a few discrete values.
///
/// @param prompt_text The text displayed to the user
/// @param choices Allowed values for user input
/// @param default_val What the value should be if the user doesn't
///                    enter anything. Not set by default.
std::string prompt(std::string_view prompt_text,
                   std::initializer_list<std::string_view> choices,
                   std::optional<std::string_view> default_val = std::nullopt);

/// Prompt the user for an integral type.
///
/// @param prompt_text The text displayed to the user
/// @param min The minimum valid value for which input is accepted
/// @param max The maximum valid value for which input is accepted
/// @param default_val What the value should be if the user doesn't
///                    enter anything. Not set by default.
template <std::integral T>
T prompt(std::string_view prompt_text,
         std::optional<T> min = std::nullopt,
         std::optional<T> max = std::nullopt,
         std::optional<T> default_val = std::nullopt)
{
    std::string input_buf{};

    while (true)
    {
        prompt(prompt_text, input_buf);

        // If the user says nothing, and the caller provides a default,
        // return the default value
        if (input_buf.empty() && default_val.has_value())
        {
            return default_val.value();
        }

        // Attempt conversion to the integral type
        T value{};
        auto [_, ec] = std::from_chars(
            input_buf.data(),
            input_buf.data() + input_buf.size(),
            value
        );
        if (ec != std::errc{})
        {
            std::println(stderr, "'{}' is not a valid number.", input_buf);
            continue;
        }

        // Ensure the value is in the correct range, if applicable.
        if (min.has_value() && value < min.value())
        {
            std::println(stderr, "{} is lower than minimum value {}", value, min.value());
            continue;
        }
        if (max.has_value() && value > max.value())
        {
            std::println(stderr, "{} is higher than maximum value {}", value, max.value());
            continue;
        }

        return value;
    }
    return 0;
}

#endif
