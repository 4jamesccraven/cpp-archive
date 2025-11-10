#include <cstdio>
#include <print>
#include <format>
#include <expected>
#include "common.h"

std::expected<MainType, std::string> parse_args(int argc, char* argv[]);

int main(int argc, char** argv)
{
    // Parse the command line and run the subproject if found.
    auto project_result = parse_args(argc, argv)
        .transform([](auto proj_main) { return proj_main(); });

    // Otherwise run report an error.
    if (!project_result.has_value())
    {
        std::println(stderr, "{}", project_result.error());
        return 1;
    }

    // Propagate return value of sub-project's main function.
    return project_result.value();
}

/// Parses the command line arguments to the program, returning the relevant
/// main function for a sub-project, or an error string.
std::expected<MainType, std::string> parse_args(int argc, char* argv[])
{
    if (argc != 2)
    {
        return std::unexpected("Usage: proj [-lh] PROJ_NAME");
    }

    std::string proj_name = argv[1];
    auto& projects = ProjectDispatch::get();

    if (!projects.contains(proj_name))
    {
        auto message = std::format(
            "Invalid project name: '{}'\nUse 'proj -l' to list valid project names.",
            proj_name
        );
        return std::unexpected(message);
    }

    return projects.at(proj_name);
}
