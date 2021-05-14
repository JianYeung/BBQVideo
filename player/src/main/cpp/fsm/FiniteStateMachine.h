//
// Created by jian.yeung on 2021/1/12.
//

#ifndef BBQVIDEO_FINITESTATEMACHINE_H
#define BBQVIDEO_FINITESTATEMACHINE_H

#include <unordered_map>
#include <vector>

#define FiniteStateType int
#define FINITE_STATE_MACHINE_ALL_STATE 0xFFFFFFFF
#define FINITE_STATE_MACHINE_NULL 0xFFFFFFFE

using FiniteStateToDestinations = std::unordered_map<FiniteStateType, std::vector<FiniteStateType>>;

class FiniteStateMachine {
public:
    FiniteStateMachine();
    ~FiniteStateMachine();

    void AddStates(const std::vector<FiniteStateType>& state_vector) {
        states_ = state_vector;
    };

    void DefineTransitions(const FiniteStateToDestinations& transitions);
    bool CanTransitTo(FiniteStateType state);
    void TransitTo(FiniteStateType state);
    bool TryTransitTo(FiniteStateType state);
    void ForceTransitTo(FiniteStateType state);

public:
    std::vector<FiniteStateType> states_;
    FiniteStateType currentState_ = FINITE_STATE_MACHINE_NULL;

private:
    FiniteStateToDestinations transitions_;
};


#endif //BBQVIDEO_FINITESTATEMACHINE_H
