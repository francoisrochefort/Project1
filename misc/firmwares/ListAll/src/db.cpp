#include <mc.h>

Db::Db() : db(NULL) 
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
    char* errMsg = 0;

    // Create the bucket table 
    int rc = sqlite3_exec(db, 
        "\
        CREATE TABLE buckets (\
            id   INTEGER NOT NULL, \
            name TEXT    NOT NULL, \
            PRIMARY KEY('id')\
        );", 
        NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the sequence table 
    rc = sqlite3_exec(db,
        "\
        CREATE TABLE sequences (\
            seq INTEGER NOT NULL, \
            val INTEGER NOT NULL, \
            PRIMARY KEY('seq')\
        );", 
        NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Insert the bucket id sequence
    rc = sqlite3_exec(db, 
        "\
        INSERT INTO sequences (\
            seq, \
            val \
        ) \
        VALUES ( \
            1, \
            1 \
        );", 
        NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Send the event
    android.onCreateDatabase();
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
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

int Db::getBucketId(const String& name)
{
    const String sql = String("SELECT id FROM buckets WHERE name = '") + name + String("'");
    int id = NoRecordFound;
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName)
    {
        // if there is no bucket matching the given name then return NoRecordFound
        *((int*)data) = argv[0] ? String(argv[0]).toInt() : NoRecordFound;;
        return 0;
    }
    , &id, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
    return id;
}

int Db::getNextVal(Seq seq)
{
    // Select the sequence and increment it 
    const String sql = String("\
        SELECT val FROM sequences WHERE seq = ") + seq + String(";\
        UPDATE sequences SET val = val + 1 WHERE seq = ") + seq + String(";");
    char* errMsg = NULL;
    int val = 0;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName)
        {
            // Convert the val to an integer and return it
            *((int*)data) = String(argv[0]).toInt();
            return 0;
        }, 
        &val, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
    return val;
}

void Db::updateBucket(const int id, const String& name)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE buckets \
        SET name = '") + name + String("' \
        WHERE id = ") + id + String(";"
    ); 
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::bucketExists(const int id)
{
    const String sql = 
    String("SELECT * FROM buckets WHERE id = ") + id + String(";");
    boolean exists = false;
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName)
    {
        // If the program gets here then the bucket exists
        *((boolean*)data) = true;
        return 0;
    }, 
    &exists, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
    return exists;
}

void Db::deleteBucket(const int id)
{
    // Delete the whole bucket
    const String sql = 
        String("DELETE FROM buckets WHERE id = ") + id + String(";"); 
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::listBuckets(LISTBUCKETSCALLBACK callback)
{
    const String sql = String("SELECT id, name FROM buckets ORDER BY name ASC;");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName)
    {
        // Create an instance of a bucket
        Bucket bucket(String(argv[0]).toInt(), argv[1]);

        // Perform the callback 
        return ((LISTBUCKETSCALLBACK)data)(&bucket);
    },
    (void*)callback, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}