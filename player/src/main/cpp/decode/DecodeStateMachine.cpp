//
// Created by jian.yeung on 2021/1/12.
//

#include <DLog.h>
#include "DecodeStateMachine.h"

DecodeStateMachine::DecodeStateMachine() {
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::DecodeStateMachine()~~~\n");
    }
    runMachine();
}

DecodeStateMachine::~DecodeStateMachine() {
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::~DecodeStateMachine()~~~\n");
    }
}

void DecodeStateMachine::runMachine() {
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::runMachine()~~~\n");
    }
    state_machine_.AddStates({
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
                             });

    state_machine_.DefineTransitions({
        {IDLE, {
                IDLE,         // 0
                INITIALIZED,  // 1
        }},
        {INITIALIZED,
         {
                INITIALIZED,
                PREPARING,
                STOPPED,
         }},
         {PREPARING,
          {
                PREPARED,
                ERROR,
         }},
         {PREPARED,
          {
                INITIALIZED,
                PREPARING,
                STARTED,
                PAUSED,
                STOPPED,
                END,
                ERROR,
         }},
         {STARTED,
          {
                INITIALIZED,
                PREPARING,
                STARTED,
                PAUSED,
                STOPPED,
                COMPLETED,
                END,
                ERROR,
         }},
         {PAUSED,
          {
                INITIALIZED,
                PREPARING,
                PAUSED,
                STARTED,
                STOPPED,
                END,
                ERROR,
         }},
         {COMPLETED,
          {
                INITIALIZED,
                PREPARING,
                STARTED,
                STOPPED,
                END,
                ERROR,
         }},
         {STOPPED,
          {
                INITIALIZED,
                PREPARING,
                STOPPED,
                END,
                ERROR,
          }},
          { END,
          {
                INITIALIZED,
                PREPARING,
          }},
          {ERROR,
           {
                IDLE,         // 0
                INITIALIZED,  // 1
           }},
    });
    state_machine_.currentState_ = (int) DecodeState::IDLE;
}

bool DecodeStateMachine::tryTransitTo(int state) {
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::tryTransitTo() Start~~~\n");
        DFLOGD(DECODE_STATE_MACHINE_TAG, "DecodeStateMachine::tryTransitTo() preState = %d", state_machine_.currentState_);
    }
    bool result = state_machine_.TryTransitTo(state);
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DFLOGD(DECODE_STATE_MACHINE_TAG, "DecodeStateMachine::tryTransitTo() curState = %d", state_machine_.currentState_);
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::tryTransitTo() End~~~\n");
    }
    return result;
}

void DecodeStateMachine::forceTransitTo(int state) {
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::forceTransitTo() Start~~~\n");
        DFLOGD(DECODE_STATE_MACHINE_TAG, "DecodeStateMachine::forceTransitTo() preState = %d", state_machine_.currentState_);
    }
    state_machine_.ForceTransitTo(state);
    if (DebugEnable && DECODE_STATE_MACHINE_DEBUG) {
        DFLOGD(DECODE_STATE_MACHINE_TAG, "DecodeStateMachine::forceTransitTo() curState = %d", state_machine_.currentState_);
        DLOGI(DECODE_STATE_MACHINE_TAG, "~~~DecodeStateMachine::forceTransitTo() End~~~\n");
    }
}