#ifndef REGEX_ERROR_HPP
#define REGEX_ERROR_HPP

#include <stdexcept>
#include <string>

namespace regex {

class regex_error : public std::exception {
public:
    regex_error(const std::string& message, size_t position);
    const char* what() const noexcept override;
    size_t get_position() const noexcept;

private:
    std::string message_;
    size_t position_;
};

} // namespace regex

#endif // REGEX_ERROR_HPP