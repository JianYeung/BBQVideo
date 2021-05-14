//
// Created by jian.yeung on 2021/1/12.
//

#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine() {

}

FiniteStateMachine::~FiniteStateMachine() {

}

void FiniteStateMachine::DefineTransitions(const FiniteStateToDestinations &transitions) {
    transitions_.clear();
    std::vector<FiniteStateType> from_states;
    for (auto transitions_it : transitions) {
        from_states.clear();
        if (transitions_it.first == FINITE_STATE_MACHINE_ALL_STATE) {
            from_states.insert(from_states.begin(), states_.begin(), states_.end());
        } else {
            from_states.push_back(transitions_it.first);
        }

        for (auto from_state_it : from_states) {
            if (transitions_.find(from_state_it) == transitions_.end()) {
                transitions_[from_state_it] = transitions_it.second;
            } else {
                transitions_[from_state_it].insert(transitions_[from_state_it].end(), transitions_it.second.begin(), transitions_it.second.end());
            }
        }
    }
}

bool FiniteStateMachine::CanTransitTo(FiniteStateType state) {
    auto it = transitions_.find(currentState_);
    if (it != transitions_.end()) {
        std::vector<FiniteStateType> next_states = it->second;
        auto canTransit = std::find(next_states.begin(), next_states.end(), state) != next_states.end();
        return canTransit;
    }

    return false;
}

void FiniteStateMachine::TransitTo(FiniteStateType state) {
    if (!CanTransitTo(state)) {
        return;
    }

    currentState_ = state;
}

bool FiniteStateMachine::TryTransitTo(FiniteStateType state) {
    if (CanTransitTo(state)) {
        TransitTo(state);
        return true;
    }

    return false;
}

void FiniteStateMachine::ForceTransitTo(FiniteStateType state) {
    currentState_ = state;
}