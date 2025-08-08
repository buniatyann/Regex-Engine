#ifndef CONVERSION_BUILDER_HPP
#define CONVERSION_BUILDER_HPP

#include "../ast/ast.hpp"
#include "../engine/engine_nfa.hpp"
#include <memory>

namespace conversion {

class builder {
public:
    std::shared_ptr<engine_nfa::nfa> build(const ast::node_ptr& node);

private:
    using nfa_state_ptr = std::shared_ptr<engine_nfa::nfa_state>;
    std::vector<nfa_state_ptr> states_;
    std::pair<nfa_state_ptr, nfa_state_ptr> build_node(const ast::node_ptr& node);
};

} // namespace conversion

#endif // CONVERSION_BUILDER_HPP