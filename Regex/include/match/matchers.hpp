#ifndef MATCHERS_HPP
#define MATCHERS_HPP

#include <memory>
#include <set>

namespace matchers {

class matcher {
public:
    virtual ~matcher() = default;
    virtual bool matches(char c) const = 0;
    virtual bool is_anchor() const = 0;
};

using matcher_ptr = std::shared_ptr<matcher>;

class literal_matcher : public matcher {
public:
    literal_matcher(char value);
    bool matches(char c) const override;
    bool is_anchor() const override;

private:
    char value_;
};

class dot_matcher : public matcher {
public:
    bool matches(char c) const override;
    bool is_anchor() const override;
};

class char_class_matcher : public matcher {
public:
    char_class_matcher(const std::set<char>& chars, bool negated);
    bool matches(char c) const override;
    bool is_anchor() const override;

private:
    std::set<char> chars_;
    bool negated_;
};

class anchor_start_matcher : public matcher {
public:
    bool matches(char c) const override;
    bool is_anchor() const override;
};

class anchor_end_matcher : public matcher {
public:
    bool matches(char c) const override;
    bool is_anchor() const override;
};

} // namespace matchers

#endif // MATCHERS_HPP