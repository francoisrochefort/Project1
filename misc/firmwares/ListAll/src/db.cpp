
#include <mc.h>

Db::Db() : db(nullptr) 
{
}

boolean Db::open() 
{
    // Check if it is the first time
    boolean exists = SPIFFS.exists("/scale.db");

    // Open the SQLite3 database
    sqlite3_initialize();
    if (sqlite3_open("/spiffs/scale.db", &db)) {
        Serial.printf("<%s%s>", Event::ON_ERROR, sqlite3_errmsg(db));
        return false;
    }

    // Create tables if they don't exist
    if (!exists) {
        return create();
    }
    return true;
}

boolean Db::create() 
{
    // Create the bucket table 
    const char sql[] = "\
        CREATE TABLE buckets (\
            id   INTEGER NOT NULL, \
            name TEXT    NOT NULL, \
            PRIMARY KEY('id')\
        );";  
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc!= SQLITE_OK) {
        Serial.printf("<%s%s>", Event::ON_ERROR, errMsg);
        return false;
    }

    // Send the event
    Serial.printf("<%s>", Event::ON_CREATE_DATABASE);
    return true;
}

bool Db::addBucket(const Bucket& bucket) 
{
    // Insert the bucket into the buckets table
    const String sql = String("\
        INSERT INTO buckets (\
            name\
        )\
        VALUES (\
            '") + String(bucket.name) + String("'\
        );"); 
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc!= SQLITE_OK) {
        Serial.printf("<%s%s>", Event::ON_ERROR, errMsg);
        return false;
    }
    return true;    
}