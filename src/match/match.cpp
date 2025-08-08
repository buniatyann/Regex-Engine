#include "../../include/match/match.hpp"

namespace match {
    match::match(bool matched, size_t start, size_t end) : matched_(matched), start_(start), end_(end) {}
} // namespace match