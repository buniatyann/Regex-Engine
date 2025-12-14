#ifndef GREP_MODEL_HPP
#define GREP_MODEL_HPP

#include "../regex/regex.hpp"
#include "../cli/cli_parser.hpp"
#include <string>
#include <vector>
#include <fstream>

namespace model {

struct match_result {
    std::string filename;
    size_t line_number;
    std::string line_content;
    size_t match_start;
    size_t match_end;
    bool matched;
};

struct file_result {
    std::string filename;
    std::vector<match_result> matches;
    size_t total_matches;
};

class grep_model {
public:
    grep_model(const cli::cli_options& options);

    std::vector<file_result> search();

private:
    cli::cli_options options_;
    std::unique_ptr<regex::regex> regex_;

    file_result search_file(const std::string& filename);
    file_result search_stdin();
    match_result match_line(const std::string& line, size_t line_number, const std::string& filename);
    std::string to_lower(const std::string& str) const;
    std::vector<std::string> get_all_files() const;
    void collect_files_recursive(const std::string& path, std::vector<std::string>& files) const;
};

} // namespace model

#endif // GREP_MODEL_HPP
