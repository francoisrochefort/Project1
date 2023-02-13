#include <mc.h>

void AsleepState::setContext(Context* context) 
{
    currentContext = context;
}

void AsleepState::doEvents() 
{
    if (android.isMessagePending()) {

        // Read and dispatch the message
        Message msg = android.getMessage();
        switch (msg.getCmd()) {

        case Cmd::Awake:
            {
                // Transition to awake state
                currentContext->setState(new AwakeState());

                // Send the event
                android.onAwake();
            }
            break;
        default:
            {
            }
            break;
        }
    }
}
