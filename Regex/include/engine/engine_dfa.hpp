#ifndef ENGINE_DFA_HPP
#define ENGINE_DFA_HPP

#include "engine_nfa.hpp"
#include "../match/match.hpp"
#include <map>
#include <set>
#include <vector>

namespace engine_dfa {

class dfa_state;

using dfa_state_ptr = std::shared_ptr<dfa_state>;

class dfa_state {
public:
    std::set<engine_nfa::nfa_state_ptr> nfa_states_;
    bool is_accept_;
    std::map<matchers::matcher_ptr, dfa_state_ptr> transitions_;
};

class dfa {
public:
    dfa(const engine_nfa::nfa& nfa);
    match::match match(const std::string& input) const;

private:
    dfa_state_ptr start_;
    std::vector<dfa_state_ptr> states_;
    
    dfa_state_ptr get_or_create_state(
        const std::set<engine_nfa::nfa_state_ptr>& nfa_states,
        std::map<std::set<engine_nfa::nfa_state_ptr>, dfa_state_ptr>& state_map,
        const engine_nfa::nfa_state_ptr& nfa_accept);
        
    std::set<matchers::matcher_ptr> get_all_matchers(
        const std::set<engine_nfa::nfa_state_ptr>& nfa_states) const;
};

} // namespace engine_dfa

#endif // ENGINE_DFA_HPP