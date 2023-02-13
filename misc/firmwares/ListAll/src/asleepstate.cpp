#include <mc.h>

void AsleepState::setContext(Context* context) 
{
    currentContext = context;
}

void AsleepState::doEvents() 
{
    // Any pending message?
    if (android.getMessage()) {

        // Dispatch the message
        String cmd = android.getCmd();
        if (cmd == Android::AWAKE) {

            // Transition to awake state
            currentContext->setState(new AwakeState());

            // Send the event
            android.onAwake();
        }
    }
}
