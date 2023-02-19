
/**
 * @file db.cpp
 * @author Francois Rochefort (francoisrochefort@hotmail.fr)
 * @brief Abstract the storage media where buckets are saved
 * @version 0.1
 * @date 2023-02-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <mc.h>

/**
 * Implementation of the GetNextBucketIdCmd helper class
 */
Db::GetNextBucketIdCmd::GetNextBucketIdCmd(sqlite3* db) : db(db)
{
}

int Db::GetNextBucketIdCmd::callback(void* data, int argc, char** argv, char** azColName)
{
    // Compute the next identifier and return it back
    int id = argv[0] ? String(argv[0]).toInt() : 1;

    *((int*)data) = id;
    return 0;
}

int Db::GetNextBucketIdCmd::run()
{
    const String sql = String("SELECT MAX(id) + 1 FROM buckets");
    int id = -1;

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &id, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    return id;
}

/**
 * Implementation of the GetBucketIdCmd helper class
 */
Db::GetBucketIdCmd::GetBucketIdCmd(sqlite3* db) : db(db)
{
}

int Db::GetBucketIdCmd::callback(void* data, int argc, char** argv, char** azColName)
{
    // if there is no bucket matching the given name then return -1
    int id = argv[0] ? String(argv[0]).toInt() : -1;

    *((int*)data) = id;
    return 0;
}

int Db::GetBucketIdCmd::run(const String& name)
{
    const String sql = String("SELECT id FROM buckets WHERE name = '") + name + String("'");
    int id = -1;

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &id, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    return id;
}

/**
 * Implementation of the BucketExistsCmd helper class
 */
Db::BucketExistsCmd::BucketExistsCmd(sqlite3* db) : db(db)
{
}

int Db::BucketExistsCmd::callback(void* data, int argc, char** argv, char** azColName)
{
    // If the program gets here then the bucket exists
    *((boolean*)data) = true;
    return 0;
}

boolean Db::BucketExistsCmd::run(const int id)
{
    const String sql = 
        String("SELECT * FROM buckets WHERE id = ") + id + String(";");
    boolean exists = false;

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &exists, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    return exists;
}

/**
 * Implementation of the ListBucketsCmd helper class
 */
Db::ListBucketsCmd::ListBucketsCmd(sqlite3* db) : db(db)
{
}

int Db::ListBucketsCmd::callback(void* data, int argc, char** argv, char** azColName)
{
    // Create an instance of a bucket
    Bucket bucket(String(argv[0]).toInt(), argv[1]);

    // Perform the callback 
    return ((LISTBUCKETSCALLBACK)data)(&bucket);
}

void Db::ListBucketsCmd::run(LISTBUCKETSCALLBACK callback)
{
    const String sql = String("SELECT id, name FROM buckets ORDER BY name ASC;");
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), this->callback, (void*)callback, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

/**
 * Implementation of the Db class
 */
Db::Db() : db(nullptr) 
{
}

void Db::open(bool createDatabase) 
{
    // Open the SQLite3 database
    int rc = sqlite3_open("/spiffs/scale.db", &db);
    ASSERT(rc == SQLITE_OK, sqlite3_errmsg(db));

    // Create tables if they don't exist
    if (createDatabase) {
        this->createDatabase();
    }
}

void Db::createDatabase() 
{
    // Create the bucket table 
    const char sql[] = "\
        CREATE TABLE buckets (\
            id   INTEGER NOT NULL, \
            name TEXT    NOT NULL, \
            PRIMARY KEY('id')\
        );"; 
    char* errMsg = 0; 
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Send the event
    android.onCreateDatabase();
}

int Db::getNextBucketId()
{
    GetNextBucketIdCmd cmd(db);
    return cmd.run();
}

void Db::addBucket(const int id, const String& name) 
{
    // Insert the new bucket
    const String sql = String("\
        INSERT INTO buckets (\
            id, \
            name\
        )\
        VALUES (\
            " + String(id) + ", \
            '") + name + String("'\
        );"
    ); 
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

int Db::getBucketId(const String& name)
{
    GetBucketIdCmd cmd(db);
    return cmd.run(name);
}

void Db::updateBucket(const int id, const String& name)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE buckets \
        SET name = '") + name + String("' \
        WHERE id = ") + id + String(";"
    ); 
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::bucketExists(const int id)
{
    BucketExistsCmd cmd(db);
    return cmd.run(id);
}

void Db::deleteBucket(const int id)
{
    // Delete the whole bucket
    const String sql = 
        String("DELETE FROM buckets WHERE id = ") + id + String(";"); 
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::listAll(LISTBUCKETSCALLBACK callback)
{
    ListBucketsCmd cmd(db);
    cmd.run(callback);
}