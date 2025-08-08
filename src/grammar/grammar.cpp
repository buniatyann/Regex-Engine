#include "../../include/grammar/grammar.hpp"
#include "../../include/regex/regex_error.hpp"

namespace grammar {

parser::parser(std::string p) : pattern_(std::move(p)), pos_(0) {
    if (pattern_.empty()) {
        throw regex::regex_error("Empty pattern", 0);
    }
}

char parser::peek() const {
    return pos_ < pattern_.size() ? pattern_[pos_] : '\0';
}

char parser::consume() {
    return pos_ < pattern_.size() ? pattern_[pos_++] : '\0';
}

bool parser::is_special(char c) const {
    return c == '|' || c == '*' || c == '+' || c == '?' || c == '(' || c == ')' ||
           c == '.' || c == '[' || c == ']' || c == '^' || c == '$' || c == '\\';
}

ast::node_ptr parser::parse() {
    bool has_start_anchor = peek() == '^';
    if (has_start_anchor) consume();
    auto node = parse_expr();
    if (peek() == '$') {
        consume();
        auto end_node = std::make_shared<ast::node>(ast::node_type_t::anchor_end);
        auto concat = std::make_shared<ast::node>(ast::node_type_t::concat);
        concat->set_children({node, end_node});
        node = concat;
    }
    if (pos_ < pattern_.size()) {
        throw regex::regex_error("Unexpected character", pos_);
    }
    if (has_start_anchor) {
        auto start_node = std::make_shared<ast::node>(ast::node_type_t::anchor_start);
        auto concat = std::make_shared<ast::node>(ast::node_type_t::concat);
        concat->set_children({start_node, node});
        node = concat;
    }
    
    return node;
}

ast::node_ptr parser::parse_expr() {
    auto term = parse_term();
    if (peek() == '|') {
        consume();
        auto expr = parse_expr();
        auto node = std::make_shared<ast::node>(ast::node_type_t::alternate);
        node->set_children({term, expr});
    
        return node;
    }
    
    return term;
}

ast::node_ptr parser::parse_term() {
    std::vector<ast::node_ptr> factors;
    while (peek() != '\0' && peek() != '|' && peek() != ')') {
        factors.push_back(parse_factor());
    }
    if (factors.empty()) {
        throw regex::regex_error("Empty term", pos_);
    }
    if (factors.size() == 1) {
        return factors[0];
    }
    
    auto node = std::make_shared<ast::node>(ast::node_type_t::concat);
    node->set_children(factors);
    
    return node;
}

ast::node_ptr parser::parse_factor() {
    auto atom = parse_atom();
    if (peek() == '*') {
        consume();
        auto node = std::make_shared<ast::node>(ast::node_type_t::star);
        node->set_children({atom});
    
        return node;
    }
    if (peek() == '+') {
        consume();
        auto node = std::make_shared<ast::node>(ast::node_type_t::plus);
        node->set_children({atom});
    
        return node;
    }
    if (peek() == '?') {
        consume();
        auto node = std::make_shared<ast::node>(ast::node_type_t::optional);
        node->set_children({atom});
    
        return node;
    }
    
    return atom;
}

ast::node_ptr parser::parse_atom() {
    if (peek() == '(') {
        consume();
        auto expr = parse_expr();
        if (consume() != ')') {
            throw regex::regex_error("Unclosed parenthesis", pos_);
        }
    
        auto node = std::make_shared<ast::node>(ast::node_type_t::group);
        node->set_children({expr});
    
        return node;
    }
    if (peek() == '.') {
        consume();
        return std::make_shared<ast::node>(ast::node_type_t::dot);
    }
    if (peek() == '[') {
        return parse_char_class();
    }
    if (peek() == '\\') {
        consume();
        if (peek() == '\0') {
            throw regex::regex_error("Incomplete escape", pos_);
        }
    
        return std::make_shared<ast::node>(ast::node_type_t::literal, consume());
    }
    if (peek() == '\0' || is_special(peek())) {
        throw regex::regex_error("Unexpected character", pos_);
    }
    
    return std::make_shared<ast::node>(ast::node_type_t::literal, consume());
}

ast::node_ptr parser::parse_char_class() {
    consume(); // '['
    bool negated = peek() == '^';
    if (negated) consume();
    std::set<char> chars;
    if (peek() == ']') {
        throw regex::regex_error("Empty character class", pos_);
    }
    
    while (peek() != ']' && peek() != '\0') {
        if (peek() == '\\') {
            consume();
            if (peek() == '\0') {
                throw regex::regex_error("Incomplete escape in character class", pos_);
            }
        
            chars.insert(consume());
        } 
        else if (peek() == '-' && !chars.empty() && peek() + 1 != ']' && peek() + 1 != '\0') {
            char start = *chars.rbegin();
            consume(); // '-'
            char end = consume();
            if (end < start) {
                throw regex::regex_error("Invalid range in character class", pos_);
            }
            
            for (char c = start + 1; c <= end; ++c) {
                chars.insert(c);
            }
        } 
        else {
            chars.insert(consume());
        }
    }
    
    if (consume() != ']') {
        throw regex::regex_error("Unclosed character class", pos_);
    }
    
    return std::make_shared<ast::node>(ast::node_type_t::char_class, chars, negated);
}

} // namespace grammar