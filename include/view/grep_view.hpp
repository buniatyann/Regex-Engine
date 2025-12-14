#ifndef GREP_VIEW_HPP
#define GREP_VIEW_HPP

#include "../model/grep_model.hpp"
#include "../cli/cli_parser.hpp"
#include <iostream>

namespace view {

class grep_view {
public:
    grep_view(const cli::cli_options& options);

    void display_results(const std::vector<model::file_result>& results);
    void display_help();
    void display_version();
    void display_error(const std::string& error);

private:
    cli::cli_options options_;
    bool use_color_;

    void display_match(const model::match_result& match, bool show_filename);
    void display_count(const model::file_result& result);
    void display_filename_only(const model::file_result& result);
    std::string colorize(const std::string& text, const std::string& color) const;
    std::string highlight_match(const std::string& line, size_t start, size_t end) const;

    // ANSI color codes
    static constexpr const char* COLOR_RESET = "\033[0m";
    static constexpr const char* COLOR_RED = "\033[31m";
    static constexpr const char* COLOR_GREEN = "\033[32m";
    static constexpr const char* COLOR_YELLOW = "\033[33m";
    static constexpr const char* COLOR_BLUE = "\033[34m";
    static constexpr const char* COLOR_MAGENTA = "\033[35m";
    static constexpr const char* COLOR_CYAN = "\033[36m";
    static constexpr const char* COLOR_BOLD = "\033[1m";
};

} // namespace view

#endif // GREP_VIEW_HPP
