#include "../../include/match/matchers.hpp"

namespace matchers {

    literal_matcher::literal_matcher(char value) : value_(value) {}

    bool literal_matcher::matches(char c) const {
        return c == value_;
    }

    bool literal_matcher::is_anchor() const {
        return false;
    }

    bool dot_matcher::matches(char c) const {
        return c != '\n';
    }

    bool dot_matcher::is_anchor() const {
        return false;
    }

    char_class_matcher::char_class_matcher(const std::set<char>& chars, bool negated)
        : chars_(chars), negated_(negated) {}

    bool char_class_matcher::matches(char c) const {
        bool in_class = chars_.count(c);
        return negated_ ? !in_class : in_class;
    }

    bool char_class_matcher::is_anchor() const {
        return false;
    }

    bool anchor_start_matcher::matches(char) const {
        return false;
    }

    bool anchor_start_matcher::is_anchor() const {
        return true;
    }

    bool anchor_end_matcher::matches(char) const {
        return false;
    }

    bool anchor_end_matcher::is_anchor() const {
        return true;
    }

} // namespace matchers