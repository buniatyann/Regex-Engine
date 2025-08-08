#include "../../include/engine/engine_dfa.hpp"
#include "../../include/regex/regex_error.hpp"
#include <queue>

namespace engine_nfa {

nfa::nfa(nfa_state_ptr start, nfa_state_ptr accept)
    : start_(start), accept_(accept) {
    if (!start_ || !accept_) {
        throw regex::regex_error("Null NFA state", 0);
    }
}

match::match nfa::match(const std::string& input) const {
    using state_set = std::set<nfa_state_ptr>;
    using state_queue = std::queue<nfa_state_ptr>;
    state_set current_states;
    state_queue to_visit;

    to_visit.push(start_);
    while (!to_visit.empty()) {
        auto state = to_visit.front();
        to_visit.pop();
        if (current_states.insert(state).second) {
            for (const auto& next : state->epsilon_transitions_) {
                to_visit.push(next);
            }
        }
    }

    size_t pos = 0;
    bool start_anchored = false;
    for (const auto& state : current_states) {
        for (const auto& [matcher, _] : state->transitions_) {
            if (matcher->is_anchor() && dynamic_cast<const matchers::anchor_start_matcher*>(matcher.get())) {
                start_anchored = true;
                break;
            }
        }
    }
   
    if (start_anchored && pos != 0) {
        return match::match(false, 0, 0);
    }

    while (pos < input.size()) {
        state_set next_states;
        for (const auto& state : current_states) {
            for (const auto& [matcher, destinations] : state->transitions_) {
                if (!matcher->is_anchor() && matcher->matches(input[pos])) {
                    next_states.insert(destinations.begin(), destinations.end());
                }
            }
        }
   
        current_states.clear();
        to_visit = state_queue();
        for (const auto& state : next_states) {
            to_visit.push(state);
        }
   
        while (!to_visit.empty()) {
            auto state = to_visit.front();
            to_visit.pop();
            if (current_states.insert(state).second) {
                for (const auto& next : state->epsilon_transitions_) {
                    to_visit.push(next);
                }
            }
        }
   
        if (current_states.empty()) {
            break;
        }
   
        pos++;
    }

    bool end_anchored = false;
    for (const auto& state : current_states) {
        for (const auto& [matcher, _] : state->transitions_) {
            if (matcher->is_anchor() && dynamic_cast<const matchers::anchor_end_matcher*>(matcher.get())) {
                end_anchored = true;
                break;
            }
        }
    }
   
    if (end_anchored && pos != input.size()) {
        return match::match(false, 0, 0);
    }

    bool is_accept = current_states.count(accept_);
    return match::match(is_accept, 0, pos);
}

} // namespace engine_nfa   