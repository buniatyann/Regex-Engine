#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include "../ast/ast.hpp"
#include <string>

namespace grammar {

class parser {
public:
    parser(std::string pattern);
    ast::node_ptr parse();

private:
    std::string pattern_;
    size_t pos_;
    
    char peek() const;
    char consume();
    bool is_special(char c) const;
    ast::node_ptr parse_expr();
    ast::node_ptr parse_term();
    ast::node_ptr parse_factor();
    ast::node_ptr parse_atom();
    ast::node_ptr parse_char_class();
};

} // namespace grammar

#endif // GRAMMAR_HPP