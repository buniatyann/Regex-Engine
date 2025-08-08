#include "../../include/regex/regex.hpp"
#include "../../include/grammar/grammar.hpp"
#include "../../include/regex/conversion_builder.hpp"

namespace regex {

regex::regex(const std::string& pattern, engine e) : engine_(e) {
    grammar::parser parser(pattern);
    ast_ = parser.parse();
    if (engine_ == engine::nfa || engine_ == engine::dfa) {
        conversion::builder builder;
        nfa_ = builder.build(ast_);
        if (engine_ == engine::dfa) {
            dfa_ = std::make_unique<engine_dfa::dfa>(*nfa_);
        }
    }
}

match::match regex::match(const std::string& input) const {
    if (engine_ == engine::dfa && dfa_) {
        return dfa_->match(input);
    }
    if (nfa_) {
        return nfa_->match(input);
    }
   
    throw regex_error("No engine available", 0);
}

} // namespace regex