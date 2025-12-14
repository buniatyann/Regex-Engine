#include "../../include/controller/grep_controller.hpp"
#include "../../include/regex/regex_error.hpp"

namespace controller {

grep_controller::grep_controller(int argc, char* argv[])
    : argc_(argc), argv_(argv) {
}

int grep_controller::run() {
    try {
        initialize();
        return execute();
    } catch (const regex::regex_error& e) {
        if (view_) {
            view_->display_error(std::string(e.what()) + " at position " + std::to_string(e.get_position()));
        } else {
            std::cerr << "Error: " << e.what() << " at position " << e.get_position() << std::endl;
        }
        return 2;
    } catch (const std::exception& e) {
        if (view_) {
            view_->display_error(e.what());
        } else {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        return 2;
    }
}

void grep_controller::initialize() {
    // Parse command-line arguments
    parser_ = std::make_unique<cli::cli_parser>(argc_, argv_);
    options_ = parser_->parse();

    // Create view first (needed for help/version)
    view_ = std::make_unique<view::grep_view>(options_);

    // Handle help and version flags
    if (options_.help) {
        view_->display_help();
        std::exit(0);
    }

    if (options_.version) {
        view_->display_version();
        std::exit(0);
    }

    // Create model
    model_ = std::make_unique<model::grep_model>(options_);
}

int grep_controller::execute() {
    // Execute the search
    auto results = model_->search();

    // Display results
    view_->display_results(results);

    // Return exit code based on whether matches were found
    bool found_matches = false;
    for (const auto& result : results) {
        if (result.total_matches > 0) {
            found_matches = true;
            break;
        }
    }

    return found_matches ? 0 : 1;
}

} // namespace controller
