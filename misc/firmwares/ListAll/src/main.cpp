#include <mc.h>

#define __ASSERT_USE_STDERR
//#define DROP_DATABASE

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

    // Init. the SQLite library
    sqlite3_initialize();

    // Init. the file system
    bool rc = SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    ASSERT(rc, "Failed to mount file system");

    // Open the database
    #ifdef DROP_DATABASE
    SPIFFS.remove("/scale.db");
    #endif
    db.open(!SPIFFS.exists("/scale.db"));
}

void loop()
{
    context.doEvents();
}

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {

    // Abort program execution
    abort();
}