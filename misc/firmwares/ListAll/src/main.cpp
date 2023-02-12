#include <mc.h>

#define DROP_DATABASE

Db db;
Context context;

void setup()
{
    // Init. serial port
    Serial.begin(BAUD_RATE);
    while (!Serial);

    // Mount the file system
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        Serial.printf("<%s%s>", Event::ON_ERROR, "Failed to mount file system");
        return;
    }

    // Open the database
    #ifdef DROP_DATABASE
    SPIFFS.remove("/scale.db");
    #endif
    db.open();
}

void loop()
{
    context.doEvents();
}
