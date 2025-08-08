#include "../../include/regex/regex_error.hpp"

namespace regex {

regex_error::regex_error(const std::string& message, size_t position)
    : message_(message), position_(position) {}

const char* regex_error::what() const noexcept {
    return message_.c_str();
}

size_t regex_error::get_position() const noexcept {
    return position_;
}

} // namespace regex