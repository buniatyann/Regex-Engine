#ifndef ENGINE_NFA_HPP
#define ENGINE_NFA_HPP

#include "../match/match.hpp"
#include "../match/matchers.hpp"
#include <memory>
#include <set>
#include <map>

namespace engine_nfa {

class nfa_state;

using nfa_state_ptr = std::shared_ptr<nfa_state>;

class nfa_state {
public:
    std::map<matchers::matcher_ptr, std::set<nfa_state_ptr>> transitions_;
    std::set<nfa_state_ptr> epsilon_transitions_;
};

class nfa {
public:
    nfa(nfa_state_ptr start, nfa_state_ptr accept);
    match::match match(const std::string& input) const;
    nfa_state_ptr get_start() const { return start_; }
    nfa_state_ptr get_accept() const { return accept_; }

private:
    nfa_state_ptr start_;
    nfa_state_ptr accept_;
};

} // namespace engine_nfa

#endif // ENGINE_NFA_HPP