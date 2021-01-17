//
// Created by jian.yeung on 2021/1/12.
//

#ifndef BBQVIDEO_DECODESTATEMACHINE_H
#define BBQVIDEO_DECODESTATEMACHINE_H


#include <FiniteStateMachine.h>

#define DECODE_STATE_MACHINE_DEBUG true
#define DECODE_STATE_MACHINE_TAG "DecodeStateMachine"

enum DecodeState {
    IDLE,         // 0
    INITIALIZED,  // 1
    PREPARING,    // 2
    PREPARED,     // 3
    STARTED,      // 4
    PAUSED,       // 5
    COMPLETED,    // 6
    STOPPED,      // 7
    END,          // 8
    ERROR,        // 9
};

class DecodeStateMachine {
public:
    DecodeStateMachine();
    ~DecodeStateMachine();

    bool tryTransitTo(int state);

    void forceTransitTo(int state);

private:
    void runMachine();

private:
    FiniteStateMachine state_machine_;
};


#endif //BBQVIDEO_DECODESTATEMACHINE_H
