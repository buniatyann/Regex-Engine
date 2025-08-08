#ifndef REGEX_HPP
#define REGEX_HPP

#include "../ast/ast.hpp"
#include "../match/match.hpp"
#include "regex_error.hpp"
#include "../engine/engine_nfa.hpp"
#include "../engine/engine_dfa.hpp"
#include <memory>

namespace regex {

enum class engine {
    nfa,
    dfa
};

class regex {
public:
    regex(const std::string& pattern, engine e = engine::nfa);
    match::match match(const std::string& input) const;

private:
    ast::node_ptr ast_;
    engine engine_;
    std::shared_ptr<engine_nfa::nfa> nfa_;
    std::shared_ptr<engine_dfa::dfa> dfa_;
};

} // namespace regex

#endif // REGEX_HPP