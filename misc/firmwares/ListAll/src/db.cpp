
#include <mc.h>

Db::Db() : db(nullptr) 
{
}

boolean Db::open(boolean init) 
{
    // Open the SQLite3 database
    if (sqlite3_open("/spiffs/scale.db", &db)) {
        android.onError(sqlite3_errmsg(db));
        return false;
    }

    // Create tables if they don't exist
    if (init) {
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
        android.onError(errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    // Send the event
    android.onCreateDatabase();
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
        android.onError(errMsg);
        sqlite3_free(errMsg);
        return false;
    }
    return true;    
}

Db::BucketExistsCmd::BucketExistsCmd(sqlite3* db) : db(db)
{
}

int Db::BucketExistsCmd::callback(void *data, int argc, char **argv, char **azColName)
{
    *((boolean*)data) = true;
    return 0;
}

boolean Db::BucketExistsCmd::run(const String& name)
{
    const String sql = String("\
        SELECT * \
        FROM buckets \
        WHERE name = '" + name + "'\
        ");
    boolean exists = false;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &exists, NULL);
    assert(rc == SQLITE_OK);
    return exists;
}

bool Db::isBucketName(const String& name)
{
    BucketExistsCmd cmd(db);
    return cmd.run(name);
}