#include <mc.h>

#define __ASSERT_USE_STDERR
#define DROP_DATABASE

// Global variables
Db db;
Android android;

// Local variables
Context context;

void setup()
{
    // Init. serial port
    Serial.begin(BAUD_RATE);
    while (!Serial);

    // Init,. the SQLite library
    sqlite3_initialize();

    // Init. the file system
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        android.onError("Failed to mount file system");
        return;
    }

    // Check if the MC is executing for the first time
    boolean init = SPIFFS.exists("/scale.db");

    // Open the database
    #ifdef DROP_DATABASE
    SPIFFS.remove("/scale.db");
    #endif
    db.open(init);
}

void loop()
{
    context.doEvents();
}

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link. 
    // Serial.println(__func);
    // Serial.println(__file);
    // Serial.println(__lineno, DEC);
    // Serial.println(__sexp);
    // Serial.flush();
    android.onError("MC assertion failed");
    // abort program execution.
    abort();
}