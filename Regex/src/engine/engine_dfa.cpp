#include "../../include/engine/engine_dfa.hpp"
#include "../../include/regex/regex_error.hpp"
#include <algorithm>

namespace engine_dfa {

dfa_state_ptr dfa::get_or_create_state(
    const std::set<engine_nfa::nfa_state_ptr>& nfa_states,
    std::map<std::set<engine_nfa::nfa_state_ptr>, dfa_state_ptr>& state_map,
    const engine_nfa::nfa_state_ptr& nfa_accept) {
    if (!state_map.count(nfa_states)) {
        auto new_state = std::make_shared<dfa_state>();
        new_state->nfa_states_ = nfa_states;
        new_state->is_accept_ = nfa_states.count(nfa_accept);
        state_map[nfa_states] = new_state;
        states_.push_back(new_state);
    }
    return state_map[nfa_states];
}

std::set<matchers::matcher_ptr> dfa::get_all_matchers(
    const std::set<engine_nfa::nfa_state_ptr>& nfa_states) const {
    std::set<matchers::matcher_ptr> matchers;
    for (const auto& state : nfa_states) {
        for (const auto& [matcher, _] : state->transitions_) {
            matchers.insert(matcher);
        }
    }
    return matchers;
}

dfa::dfa(const engine_nfa::nfa& nfa) {
    using nfa_state_set = std::set<engine_nfa::nfa_state_ptr>;
    using state_map = std::map<nfa_state_set, dfa_state_ptr>;
    if (!nfa.get_start() || !nfa.get_accept()) {
        throw regex::regex_error("Invalid NFA", 0);
    }
    state_map dfa_state_map;
    std::vector<nfa_state_set> worklist;

    auto epsilon_closure = [&nfa](const nfa_state_set& states) {
        nfa_state_set closure = states;
        std::vector<engine_nfa::nfa_state_ptr> stack(states.begin(), states.end());
        while (!stack.empty()) {
            auto state = stack.back();
            stack.pop_back();
            for (const auto& next : state->epsilon_transitions_) {
                if (closure.insert(next).second) {
                    stack.push_back(next);
                }
            }
        }
        
        return closure;
    };

    start_ = get_or_create_state(epsilon_closure({nfa.get_start()}), dfa_state_map, nfa.get_accept());
    worklist.push_back(start_->nfa_states_);

    while (!worklist.empty()) {
        auto current_nfa_states = worklist.back();
        worklist.pop_back();
        auto current_dfa_state = dfa_state_map[current_nfa_states];

        auto matchers = get_all_matchers(current_nfa_states);
        for (const auto& matcher : matchers) {
            nfa_state_set next_nfa_states;
            for (const auto& nfa_state : current_nfa_states) {
                if (nfa_state->transitions_.count(matcher)) {
                    for (const auto& next : nfa_state->transitions_.at(matcher)) {
                        next_nfa_states.insert(next);
                    }
                }
            }
            if (!next_nfa_states.empty()) {
                auto next_closure = epsilon_closure(next_nfa_states);
                auto next_dfa_state = get_or_create_state(next_closure, dfa_state_map, nfa.get_accept());
                current_dfa_state->transitions_[matcher] = next_dfa_state;
                if (dfa_state_map.find(next_closure) == dfa_state_map.end()) {
                    worklist.push_back(next_closure);
                }
            }
        }
    }
}

match::match dfa::match(const std::string& input) const {
    match::size_t best_pos = 0;
    bool best_accept = false;
    match::size_t start_pos = 0;

    while (start_pos < input.size() || (start_pos == 0 && input.empty())) {
        auto current = start_;
        match::size_t pos = start_pos;
        bool start_anchored = false;

        for (const auto& [matcher, _] : current->transitions_) {
            if (matcher->is_anchor() && dynamic_cast<const matchers::anchor_start_matcher*>(matcher.get())) {
                start_anchored = true;
                break;
            }
        }
        if (start_anchored && start_pos != 0) {
            start_pos++;
            continue;
        }

        while (pos < input.size()) {
            bool transitioned = false;
            for (const auto& [matcher, next_state] : current->transitions_) {
                if (!matcher->is_anchor() && matcher->matches(input[pos])) {
                    current = next_state;
                    pos++;
                    transitioned = true;
                    break;
                }
            }
            if (!transitioned) {
                break;
            }
        }

        bool end_anchored = false;
        for (const auto& [matcher, _] : current->transitions_) {
            if (matcher->is_anchor() && dynamic_cast<const matchers::anchor_end_matcher*>(matcher.get())) {
                end_anchored = true;
                break;
            }
        }
        if (end_anchored && pos != input.size()) {
            start_pos++;
            continue;
        }

        if (current->is_accept_ && (!end_anchored || pos == input.size())) {
            best_accept = true;
            best_pos = pos;
            break;
        }

        start_pos++;
    }

    return match::match(best_accept, 0, best_pos);
}

} // namespace engine_dfa