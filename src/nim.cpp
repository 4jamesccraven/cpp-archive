/*
 *  Nim v2.0
 *  By James C. Craven
 *  impl 11th November, 2025
 *  See LICENSE for licensing information.
 */
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <print>
#include <random>
#include <string_view>
#include <thread>
#include "common.h"

enum class Player { player, computer };
/// Returns the other player
inline Player other(Player self);

struct GameState
{
    Player current_player;
    uint8_t heap;
    std::optional<Player> winner;
    std::string player_name;

    inline bool game_over() { return this->heap <= 0; };
    void take_turn();
};

/// Sleeps for 1 second.
inline void wait();

constexpr std::string_view RULES = R"(
The game begins with a pool of 23 objects. Players take turns removing one to three items at a time.
On each turn a player must take at least one object. Whoever is forced to remove the last object loses.
)";

int nim_main()
{
    GameState state {
        Player::player,
        23,
        std::nullopt,
        ""
    };

    prompt("Welcome to nim. Please enter your name.", state.player_name);

    bool display_help = prompt(
        "Display game rules? [y/N]",
        {"y", "yes"},
        {"n", "no"},
        false
    );

    if (display_help)
        std::println("{}", RULES);

    while (!state.game_over())
    {
        state.take_turn();

        state.current_player = other(state.current_player);;
        wait();
    }

    if (state.winner.has_value())
    {
        switch (state.winner.value())
        {
            case Player::player:
                std::println("{} has won!", state.player_name);
                break;
            case Player::computer:
                std::println("{} has been beaten by the computer.", state.player_name);
        }
    }
    else
    {
        std::println(stderr, "An unexpected error has occured.");
        return 1;
    }

    return 0;
}

void GameState::take_turn()
{
    // Determine who's playing, whether "object" should be plural, and
    // the maximum objects that can be removed.
    auto subject = (this->current_player == Player::player) ? this->player_name : "computer";
    auto plural_text = (this->heap != 1) ? "s" : "";
    std::uint8_t max = (this->heap < 3) ? this->heap : 3;

    // Announce that information to the player
    std::println("\n{}'s turn. {} object{} remaining.", subject, this->heap, plural_text);

    std::uint8_t to_remove;
    // The player chooses how many items to remove...
    if (this->current_player == Player::player)
    {
        to_remove = prompt<uint8_t>(
            "How many objects would you like to remove?",
            1, max
        );
    }
    // ... or the computer chooses a random amount.
    else
    {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<uint8_t> dist(1, max);
        to_remove = dist(rng);
    }
    auto removed_plural = (to_remove != 1) ? "s" : "";

    // Remove the items
    this->heap -= to_remove;
    std::println("{} has decided to remove {} object{}.", subject, to_remove, removed_plural);

    // Set the winner, if applicable.
    if (this->game_over())
        this->winner = other(this->current_player);
}

inline Player other(Player self)
{
    return (self == Player::player) ? Player::computer : Player::player;
}

inline void wait()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

ProjectDispatch reg_nim("nim", nim_main);
