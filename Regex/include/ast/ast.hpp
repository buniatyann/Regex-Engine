#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <set>
#include <vector>

namespace ast {

enum class node_type_t {
    literal,
    dot,
    char_class,
    group,
    concat,
    alternate,
    star,
    plus,
    optional,
    anchor_start,
    anchor_end
};

class node;

using node_ptr = std::shared_ptr<node>;

class node {
public:
    node(node_type_t type, char value = '\0', bool negated = false);
    node(node_type_t type, const std::set<char>& chars, bool negated);

    node_type_t get_type() const { 
        return type_; 
    }
    
    char get_value() const { 
        return value_;
    }

    const std::set<char>& get_chars() const { 
        return chars_; 
    }
    
    bool is_negated() const { 
        return negated_; 
    }
    
    const std::vector<node_ptr>& get_children() const { 
        return children_; 
    }
    
    void set_children(const std::vector<node_ptr>& children) { 
        children_ = children; 
    }

private:
    node_type_t type_;
    char value_;
    std::set<char> chars_;
    bool negated_;
    std::vector<node_ptr> children_;
};

} // namespace ast

#endif // AST_HPP