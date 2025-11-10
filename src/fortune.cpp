/*
 *  Fortune Teller v2.0
 *  By James C. Craven
 *  impl 10th November, 2025
 *  See LICENSE for licensing information.
 */
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <print>
#include <random>
#include <string_view>
#include <utility>
#include "common.h"

/// Represents a season
/// Note: the numbers are to make their values compatible with the original
enum class Season { spring = 2, summer = 3, fall = 4, winter = 1};

/// Converts the a northern hemisphere season to its southern
/// equivalent and vice versa.
constexpr Season invert(Season s);

/// Gets the user's current season, accounting for their vertical hemisphere.
Season current_season(bool northern_hemisphere);

/// Interprets the user's fortune, giving them a message.
std::string_view interpret_fortune(unsigned int fortune_val);

constexpr std::string_view INTRO = R"(
This fortune teller program magically reads fate, using your astro-temporal coordinates!
To begin, we must find your magnetic signature in relation to the poles.
)";

constexpr std::string_view DICE_ART = R"(
      ____
     /\' .\    _____
    /: \___\  / .  /\
    \' / . / /____/..\
     \/___/  \'  '\  /
              \'__'\/
)";

int fort_main()
{
    // Print the introduction.
    std::println(stderr, "{}", INTRO);

    // Determine the user's vertical hemisphere, assuming north.
    bool in_northern_hemisphere = prompt(
        "Are you in the northern or southern hemisphere? [default: north]",
        {"n", "north", "northern"},
        {"s", "south", "southern"},
        true
    );

    // Obtain information about the user's date of birth.
    // (and SSN, mother's maiden name, first pet, etc. /j)
    std::println(stderr, "Next your horoscope must be considered.");
    unsigned int birth_month = prompt<unsigned int>(
        "What is your birth month? (enter a number)",
        1, 12 // month in [1, 12]
    );
    unsigned int birth_year = prompt<unsigned int>(
        "And your birth year? (enter a number)"
    );

    // Have the user roll a die (or pick a number, whatever works).
    std::println(stderr, "Now roll a d6 (six-sided die).");
    unsigned int die_roll = prompt<unsigned int>(
        "What did you get?",
        1, 6 // die result in [1, 6]
    );

    // Convert the season to its underlying value
    auto season_val = static_cast<unsigned int>(current_season(in_northern_hemisphere));

    // Calculate a value to seed the RNG based on user input.
    // This is different from the original and uses an FNV hash. This is
    // to avoid overflows with pow and other similar issues.
    constexpr auto FNV_OFFSET = 0xcbf29ce484222325;
    constexpr auto FNV_PRIME = 0x100000001b3;
    std::array<unsigned int, 4> inputs = {{
        birth_month, birth_year, die_roll, season_val
    }};

    std::uint64_t seed = FNV_OFFSET;
    for (auto inp : inputs)
    {
        seed ^= inp;
        seed *= FNV_PRIME;
    }

    // Create and seed a random number distribution and seed it
    // with our fortune inputs.
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<unsigned int> dist(1, 120);

    // Obtain the fortune.
    auto fortune = dist(rng);

    std::println(stderr, "{}", DICE_ART);
    std::println("{}", interpret_fortune(fortune));

    return 0;
}

constexpr Season invert(Season s)
{
    switch (s)
    {
        case Season::winter:
            return Season::summer;
        case Season::spring:
            return Season::fall;
        case Season::summer:
            return Season::winter;
        case Season::fall:
            return Season::spring;
    }

    std::unreachable();
}

Season current_season(bool northern_hemisphere)
{
    using namespace std::chrono;

    // Use the system clock to figure out what day today is,
    // and when January 1st was this year.
    auto today = floor<days>(system_clock::now());
    year_month_day ymd{today};
    sys_days start_of_year{ymd.year()/January/1};

    // Calculate the approximate progress through the year (as a percent)
    unsigned int day_of_year = (today - start_of_year).count();
    double year_percent = day_of_year / 365.0;

    // Approximate transition points of the seasons
    // Note: these aren't perfect, but they're close enough
    // for our purposes.
    constexpr double winter_spring = 0.22;
    constexpr double spring_summer = 0.47;
    constexpr double summer_fall = 0.73;
    constexpr double fall_winter = 0.98;

    // Determine the season via approximation.
    std::optional<Season> current_season = std::nullopt;
    if (year_percent < winter_spring || year_percent >= fall_winter)
    {
        current_season = Season::winter;
    }
    else if (year_percent >= winter_spring && year_percent < spring_summer)
    {
        current_season = Season::spring;
    }
    else if (year_percent >= spring_summer && year_percent < summer_fall)
    {
        current_season = Season::summer;
    }
    else
    {
        current_season = Season::fall;
    }

    return (northern_hemisphere) ? current_season.value() : invert(current_season.value());
}

std::string_view interpret_fortune(unsigned int fortune_val)
{
    static constexpr std::array<std::pair<int, int>, 10> fortune_buckets = {{
        {0,  0}, {40, 1}, {50,  2}, {60,  3}, {70,  4},
        {80, 5}, {90, 6}, {100, 7}, {110, 8}, {120, 9}
    }};
    static constexpr std::array<std::string_view, 10> fortunes = {{
        "https://youtu.be/dQw4w9WgXcQ",              // Rarest (it's a Rick Roll)
        "Your future is blurry. You may try again.", // Grey loot.
        "Your character seems suspicious to others. Try smiling more. With excessive eye contact.",
        "You owe me a soda.",
        "You will overcome your fears.",
        "You will see an octopus today.", // Changed because the original was an inside joke with someone I detest
        "Your next test, be it academic, medical, or otherwise will be positive, for better or for worse.",
        "You will trip and fall today.",
        "Beware the ides of the coming month.",
        "Try a new hobby. (Might I suggest Warframe?)" // Changed to something I play more frequently
    }};

    auto it = std::ranges::find_if(fortune_buckets, [fortune_val](auto r)
        {
            return fortune_val <= r.first;
        });

    // We'll use the common response if this fails for some reason
    // though it shouldn't under any circumstance.
    auto idx = it == fortune_buckets.end() ? 1 : it->second;

    return fortunes[idx];
}

// Register the main function to the top-level binary
ProjectDispatch reg("fortune", fort_main);
