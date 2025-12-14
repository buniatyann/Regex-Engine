#include "../../include/view/grep_view.hpp"
#include <unistd.h>

namespace view {

grep_view::grep_view(const cli::cli_options& options)
    : options_(options) {
    // Enable color if output is a terminal
    use_color_ = isatty(STDOUT_FILENO);
}

void grep_view::display_results(const std::vector<model::file_result>& results) {
    bool multiple_files = results.size() > 1 || (!results.empty() && !results[0].filename.empty() && results[0].filename != "(standard input)");

    for (const auto& result : results) {
        if (options_.count_only) {
            display_count(result);
        } 
        else if (options_.files_with_matches) {
            display_filename_only(result);
        } 
        else {
            for (const auto& match : result.matches) {
                display_match(match, multiple_files);
            }
        }
    }
}

void grep_view::display_match(const model::match_result& match, bool show_filename) {
    std::string output;

    // Add filename if needed
    if (show_filename) {
        output += colorize(match.filename, COLOR_MAGENTA);
        output += ":";
    }

    // Add line number if needed
    if (options_.line_numbers) {
        output += colorize(std::to_string(match.line_number), COLOR_GREEN);
        output += ":";
    }

    // Add the line content with highlighted match
    if (match.matched && use_color_) {
        output += highlight_match(match.line_content, match.match_start, match.match_end);
    } 
    else {
        output += match.line_content;
    }

    std::cout << output << std::endl;
}

void grep_view::display_count(const model::file_result& result) {
    if (result.total_matches > 0 || !result.filename.empty()) {
        if (!result.filename.empty() && result.filename != "(standard input)") {
            std::cout << colorize(result.filename, COLOR_MAGENTA) << ":";
        }

        std::cout << result.total_matches << std::endl;
    }
}

void grep_view::display_filename_only(const model::file_result& result) {
    if (result.total_matches > 0) {
        std::cout << colorize(result.filename, COLOR_MAGENTA) << std::endl;
    }
}

std::string grep_view::colorize(const std::string& text, const std::string& color) const {
    if (!use_color_) {
        return text;
    }
    
    return color + text + COLOR_RESET;
}

std::string grep_view::highlight_match(const std::string& line, size_t start, size_t end) const {
    if (start >= line.length()) {
        return line;
    }

    std::string result;
    result += line.substr(0, start);
    result += COLOR_BOLD;
    result += COLOR_RED;
    result += line.substr(start, end - start);
    result += COLOR_RESET;
    result += line.substr(end);

    return result;
}

void grep_view::display_help() {
    std::cout << "Usage: regex_grep [OPTIONS] PATTERN [FILE...]\n\n"
              << "Search for PATTERN in each FILE or standard input.\n\n"
              << "Options:\n"
              << "  -i, --ignore-case        Ignore case distinctions\n"
              << "  -n, --line-number        Print line numbers with output\n"
              << "  -v, --invert-match       Select non-matching lines\n"
              << "  -c, --count              Print only a count of matching lines\n"
              << "  -l, --files-with-matches Print only names of files with matches\n"
              << "  -r, --recursive          Search directories recursively\n"
              << "      --dfa                Use DFA engine (default: NFA)\n"
              << "  -h, --help               Display this help and exit\n"
              << "      --version            Display version information and exit\n\n"
              << "Regex Syntax:\n"
              << "  Literals: a, b, ...\n"
              << "  Dot (.):  Matches any character except newline\n"
              << "  Character classes: [abc], [^abc] (negated)\n"
              << "  Alternation: a|b\n"
              << "  Quantifiers: * (zero or more), + (one or more), ? (optional)\n"
              << "  Groups: (ab)\n"
              << "  Anchors: ^ (start), $ (end)\n\n"
              << "Examples:\n"
              << "  regex_grep 'error' logfile.txt\n"
              << "  regex_grep -i 'warning|error' *.log\n"
              << "  regex_grep -n '^[0-9]+$' data.txt\n"
              << "  regex_grep -r 'TODO' src/\n";
}

void grep_view::display_version() {
    std::cout << "regex_grep version 1.0.0\n"
              << "High-performance C++ Regex Engine with NFA/DFA support\n";
}

void grep_view::display_error(const std::string& error) {
    std::cerr << colorize("Error: ", COLOR_RED) << error << std::endl;
    std::cerr << "Try 'regex_grep --help' for more information." << std::endl;
}

} // namespace view
