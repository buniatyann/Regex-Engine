#include "../../include/cli/cli_parser.hpp"
#include "../../include/regex/regex_error.hpp"
#include <iostream>

namespace cli {

cli_parser::cli_parser(int argc, char* argv[])
    : pos_(0), current_state_(state::start) {
    for (int i = 1; i < argc; ++i) {
        args_.push_back(argv[i]);
    }
}

cli_options cli_parser::parse() {
    while (pos_ < args_.size() && current_state_ != state::done && current_state_ != state::error) {
        const std::string& arg = args_[pos_];
        switch (current_state_) {
            case state::start:
                if (is_option(arg)) {
                    transition(state::reading_option);
                } 
                else {
                    transition(state::reading_pattern);
                }
                
                break;

            case state::reading_option:
                if (is_option(arg)) {
                    if (!process_option(arg)) {
                        transition(state::error);
                    }
                
                    ++pos_;
                } 
                else {
                    transition(state::reading_pattern);
                }
                
                break;

            case state::reading_pattern:
                options_.pattern = arg;
                ++pos_;
                transition(state::reading_file);
                
                break;

            case state::reading_file:
                if (is_option(arg)) {
                    transition(state::reading_option);
                } 
                else {
                    options_.files.push_back(arg);
                    ++pos_;
                }
                
                break;

            case state::done:
            case state::error:
                break;
        }
    }

    // Validate that we have at least a pattern
    if (current_state_ != state::error && !options_.help && !options_.version) {
        if (options_.pattern.empty()) {
            set_error("No pattern specified");
            current_state_ = state::error;
        }
    }

    if (current_state_ == state::error) {
        throw regex::regex_error(error_message_, 0);
    }

    return options_;
}

void cli_parser::transition(state new_state) {
    current_state_ = new_state;
}

bool cli_parser::is_option(const std::string& arg) const {
    return !arg.empty() && arg[0] == '-';
}

bool cli_parser::process_option(const std::string& arg) {
    if (arg == "-i" || arg == "--ignore-case") {
        options_.ignore_case = true;
    } 
    else if (arg == "-n" || arg == "--line-number") {
        options_.line_numbers = true;
    } 
    else if (arg == "-v" || arg == "--invert-match") {
        options_.invert_match = true;
    } 
    else if (arg == "-c" || arg == "--count") {
        options_.count_only = true;
    } 
    else if (arg == "-l" || arg == "--files-with-matches") {
        options_.files_with_matches = true;
    } 
    else if (arg == "-r" || arg == "--recursive") {
        options_.recursive = true;
    } 
    else if (arg == "--dfa") {
        options_.use_dfa = true;
    } 
    else if (arg == "-h" || arg == "--help") {
        options_.help = true;
        transition(state::done);
    } 
    else if (arg == "--version") {
        options_.version = true;
        transition(state::done);
    } 
    else {
        set_error("Unknown option: " + arg);
        return false;
    }
    
    return true;
}

void cli_parser::set_error(const std::string& message) {
    error_message_ = message;
}

} // namespace cli
