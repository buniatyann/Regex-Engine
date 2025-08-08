#include "../../include/ast/ast.hpp"

namespace ast {
    node::node(node_type_t type, char value, bool negated) : type_(type), value_(value), negated_(negated) {}

    node::node(node_type_t type, const std::set<char>& chars, bool negated) : type_(type), chars_(chars), negated_(negated) {}
} // namespace ast