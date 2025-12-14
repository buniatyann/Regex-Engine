#include "../../include/model/grep_model.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace model {

grep_model::grep_model(const cli::cli_options& options)
    : options_(options) {

    std::string pattern = options_.pattern;

    // Check for anchors before case conversion
    bool has_start_anchor = !pattern.empty() && pattern[0] == '^';
    bool has_end_anchor = !pattern.empty() && pattern.back() == '$';

    // For case insensitive, convert pattern to lowercase after checking anchors
    if (options_.ignore_case) {
        pattern = to_lower(pattern);
    }

    // Wrap pattern with .* for substring matching (grep-like behavior)
    // unless it already has anchors
    if (!has_start_anchor) {
        pattern = ".*" + pattern;
    }
    if (!has_end_anchor) {
        pattern = pattern + ".*";
    }

    regex::engine engine_type = options_.use_dfa ? regex::engine::dfa : regex::engine::nfa;
    regex_ = std::make_unique<regex::regex>(pattern, engine_type);
}

std::vector<file_result> grep_model::search() {
    std::vector<file_result> results;

    if (options_.files.empty()) {
        // Read from stdin
        results.push_back(search_stdin());
    } 
    else {
        std::vector<std::string> all_files = get_all_files();
        for (const auto& filename : all_files) {
            results.push_back(search_file(filename));
        }
    }

    return results;
}

file_result grep_model::search_file(const std::string& filename) {
    file_result result;
    result.filename = filename;
    result.total_matches = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return result;
    }

    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        ++line_number;
        match_result match = match_line(line, line_number, filename);
        bool should_include = options_.invert_match ? !match.matched : match.matched;
        if (should_include) {
            result.matches.push_back(match);
            ++result.total_matches;
        }
    }

    return result;
}

file_result grep_model::search_stdin() {
    file_result result;
    result.filename = "(standard input)";
    result.total_matches = 0;
    std::string line;
    size_t line_number = 0;
    while (std::getline(std::cin, line)) {
        ++line_number;
        match_result match = match_line(line, line_number, result.filename);
        bool should_include = options_.invert_match ? !match.matched : match.matched;
        if (should_include) {
            result.matches.push_back(match);
            ++result.total_matches;
        }
    }

    return result;
}

match_result grep_model::match_line(const std::string& line, size_t line_number, const std::string& filename) {
    match_result result;
    result.filename = filename;
    result.line_number = line_number;
    result.line_content = line;
    result.matched = false;

    std::string search_line = options_.ignore_case ? to_lower(line) : line;
    match::match m = regex_->match(search_line);
    result.matched = m.is_matched();
    if (result.matched) {
        result.match_start = m.get_start();
        result.match_end = m.get_end();
    }

    return result;
}

std::string grep_model::to_lower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<std::string> grep_model::get_all_files() const {
    std::vector<std::string> all_files;
    for (const auto& file : options_.files) {
        if (options_.recursive && std::filesystem::is_directory(file)) {
            collect_files_recursive(file, all_files);
        } 
        else {
            all_files.push_back(file);
        }
    }

    return all_files;
}

void grep_model::collect_files_recursive(const std::string& path, std::vector<std::string>& files) const {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory " << path << ": " << e.what() << std::endl;
    }
}

} // namespace model
