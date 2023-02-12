#include <mc.h>

void AsleepState::setContext(Context* context) 
{
    currentContext = context;
}

void AsleepState::doEvents() 
{
    // Any pending message?
    int cb = Serial.available();
    if (cb) {

        // Read the message
        Serial.readStringUntil('<');
        String str = Serial.readStringUntil('>');

        // Parse the message
        String cmd = str.substring(0, 4);
        String val = str.substring(4);

        // Dispatch the message
        if (cmd == Command::AWAKE) {

            // Transition to awake state
            currentContext->setState(new AwakeState());

            // Send the event
            Serial.printf("<%s>", Event::ON_AWAKE);
        }
    }
}
