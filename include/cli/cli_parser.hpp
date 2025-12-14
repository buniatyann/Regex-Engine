#ifndef CLI_PARSER_HPP
#define CLI_PARSER_HPP

#include <string>
#include <vector>
#include <optional>

namespace cli {

struct cli_options {
    std::string pattern;
    std::vector<std::string> files;
    bool ignore_case = false;
    bool line_numbers = false;
    bool invert_match = false;
    bool count_only = false;
    bool files_with_matches = false;
    bool recursive = false;
    bool use_dfa = false;
    bool help = false;
    bool version = false;
};

class cli_parser {
public:
    cli_parser(int argc, char* argv[]);
    cli_options parse();

private:
    enum class state {
        start,
        reading_option,
        reading_pattern,
        reading_file,
        done,
        error
    };

    std::vector<std::string> args_;
    size_t pos_;
    state current_state_;
    cli_options options_;
    std::string error_message_;

    void transition(state new_state);
    bool is_option(const std::string& arg) const;
    bool process_option(const std::string& arg);
    void set_error(const std::string& message);
};

} // namespace cli

#endif // CLI_PARSER_HPP
