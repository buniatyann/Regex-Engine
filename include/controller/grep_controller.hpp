#ifndef GREP_CONTROLLER_HPP
#define GREP_CONTROLLER_HPP

#include "../cli/cli_parser.hpp"
#include "../model/grep_model.hpp"
#include "../view/grep_view.hpp"
#include <memory>

namespace controller {

class grep_controller {
public:
    grep_controller(int argc, char* argv[]);

    int run();

private:
    int argc_;
    char** argv_;
    std::unique_ptr<cli::cli_parser> parser_;
    std::unique_ptr<model::grep_model> model_;
    std::unique_ptr<view::grep_view> view_;
    cli::cli_options options_;

    void initialize();
    int execute();
};

} // namespace controller

#endif // GREP_CONTROLLER_HPP
