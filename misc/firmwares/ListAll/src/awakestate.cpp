#include <mc.h>

void AwakeState::setContext(Context* context) 
{
    currentContext = context;
}

void AwakeState::doEvents() 
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
        if (cmd == Command::ADD_BUCKET) {

            // Insert the new bucket
            String name = val;
            uint8_t bucketId = 1;

            // Send the event
            Serial.printf("<%s%i>", Event::ON_ADD_BUCKET, bucketId);

        } else if (cmd == Command::UPDATE_BUCKET) {

            // Split the val parameter
            int period = 0;
            for(; val[period] != 0 && val[period] != ','; period++);
            String bucketId = val.substring(0, period);
            String name = val.substring(period + 1);

            // Update the bucket
    

            // Send the event
            Serial.printf("<%s%i>", Event::ON_UPDATE_BUCKET, bucketId);

        } else if (cmd == Command::DELETE_BUCKET) {

            // Delete the bucket
            uint8_t bucketId = val.toInt();

            // Send the event
            Serial.printf("<%s%i>", Event::ON_DELETE_BUCKET, bucketId);

        } else if (cmd == Command::COPY_BUCKET) {
            
            // Copy the bucket
            uint8_t src = val.toInt();
            uint8_t dest = 2;

            // Send the event
            Serial.printf("<%s%i>", Event::ON_COPY_BUCKET, dest);
        }
    }
}
