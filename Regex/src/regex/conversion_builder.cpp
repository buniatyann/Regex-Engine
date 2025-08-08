#include "../../include/regex/conversion_builder.hpp"
#include "../../include/match/matchers.hpp"
#include "../../include/regex/regex_error.hpp"

namespace conversion {

std::shared_ptr<engine_nfa::nfa> builder::build(const ast::node_ptr& node) {
    if (!node) {
        throw regex::regex_error("Null AST node", 0);
    }
    
    auto [start, accept] = build_node(node);
    states_.clear();
    
    return std::make_shared<engine_nfa::nfa>(start, accept);
}

std::pair<builder::nfa_state_ptr, builder::nfa_state_ptr>
builder::build_node(const ast::node_ptr& node) {
    // using matcher_ptr = matchers::matcher_ptr;
    switch (node->get_type()) {
        case ast::node_type_t::literal: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            start->transitions_[std::make_shared<matchers::literal_matcher>(node->get_value())] = {accept};
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::dot: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            start->transitions_[std::make_shared<matchers::dot_matcher>()] = {accept};
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::char_class: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            start->transitions_[std::make_shared<matchers::char_class_matcher>(node->get_chars(), node->is_negated())] = {accept};
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::group: {
            return build_node(node->get_children()[0]);
        }
        case ast::node_type_t::concat: {
            auto first = build_node(node->get_children()[0]);
            auto current_accept = first.second;
            for (size_t i = 1; i < node->get_children().size(); ++i) {
                auto next = build_node(node->get_children()[i]);
                current_accept->epsilon_transitions_.insert(next.first);
                current_accept = next.second;
            }
            
            return {first.first, current_accept};
        }
        case ast::node_type_t::alternate: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            for (const auto& child : node->get_children()) {
                auto sub = build_node(child);
                start->epsilon_transitions_.insert(sub.first);
                sub.second->epsilon_transitions_.insert(accept);
            }
            
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::star: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            auto sub = build_node(node->get_children()[0]);
            start->epsilon_transitions_.insert(sub.first);
            start->epsilon_transitions_.insert(accept);
            sub.second->epsilon_transitions_.insert(sub.first);
            sub.second->epsilon_transitions_.insert(accept);
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::plus: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            auto sub = build_node(node->get_children()[0]);
            start->epsilon_transitions_.insert(sub.first);
            sub.second->epsilon_transitions_.insert(sub.first);
            sub.second->epsilon_transitions_.insert(accept);
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::optional: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            auto sub = build_node(node->get_children()[0]);
            start->epsilon_transitions_.insert(sub.first);
            start->epsilon_transitions_.insert(accept);
            sub.second->epsilon_transitions_.insert(accept);
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::anchor_start: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            start->transitions_[std::make_shared<matchers::anchor_start_matcher>()] = {accept};
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        case ast::node_type_t::anchor_end: {
            auto start = std::make_shared<engine_nfa::nfa_state>();
            auto accept = std::make_shared<engine_nfa::nfa_state>();
            start->transitions_[std::make_shared<matchers::anchor_end_matcher>()] = {accept};
            states_.push_back(start);
            states_.push_back(accept);
            
            return {start, accept};
        }
        default:
            throw regex::regex_error("Unknown AST node type", 0);
    }
}
}