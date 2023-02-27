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
            c0_rising_t_00 INTEGER, \
            c0_rising_a_00 INTEGER, \
            c0_rising_s_00 INTEGER, \
            c0_rising_p_00 INTEGER, \
            c0_rising_t_01 INTEGER, \
            c0_rising_a_01 INTEGER, \
            c0_rising_s_01 INTEGER, \
            c0_rising_p_01 INTEGER, \
            c0_rising_t_02 INTEGER, \
            c0_rising_a_02 INTEGER, \
            c0_rising_s_02 INTEGER, \
            c0_rising_p_02 INTEGER, \
            c0_rising_t_03 INTEGER, \
            c0_rising_a_03 INTEGER, \
            c0_rising_s_03 INTEGER, \
            c0_rising_p_03 INTEGER, \
            c0_rising_t_04 INTEGER, \
            c0_rising_a_04 INTEGER, \
            c0_rising_s_04 INTEGER, \
            c0_rising_p_04 INTEGER, \
            c0_rising_t_05 INTEGER, \
            c0_rising_a_05 INTEGER, \
            c0_rising_s_05 INTEGER, \
            c0_rising_p_05 INTEGER, \
            c0_rising_t_06 INTEGER, \
            c0_rising_a_06 INTEGER, \
            c0_rising_s_06 INTEGER, \
            c0_rising_p_06 INTEGER, \
            c0_rising_t_07 INTEGER, \
            c0_rising_a_07 INTEGER, \
            c0_rising_s_07 INTEGER, \
            c0_rising_p_07 INTEGER, \
            c0_rising_t_08 INTEGER, \
            c0_rising_a_08 INTEGER, \
            c0_rising_s_08 INTEGER, \
            c0_rising_p_08 INTEGER, \
            c0_rising_t_09 INTEGER, \
            c0_rising_a_09 INTEGER, \
            c0_rising_s_09 INTEGER, \
            c0_rising_p_09 INTEGER, \
            c0_rising_t_10 INTEGER, \
            c0_rising_a_10 INTEGER, \
            c0_rising_s_10 INTEGER, \
            c0_rising_p_10 INTEGER, \
            c0_rising_t_11 INTEGER, \
            c0_rising_a_11 INTEGER, \
            c0_rising_s_11 INTEGER, \
            c0_rising_p_11 INTEGER, \
            c0_rising_t_12 INTEGER, \
            c0_rising_a_12 INTEGER, \
            c0_rising_s_12 INTEGER, \
            c0_rising_p_12 INTEGER, \
            c0_rising_t_13 INTEGER, \
            c0_rising_a_13 INTEGER, \
            c0_rising_s_13 INTEGER, \
            c0_rising_p_13 INTEGER, \
            c0_rising_t_14 INTEGER, \
            c0_rising_a_14 INTEGER, \
            c0_rising_s_14 INTEGER, \
            c0_rising_p_14 INTEGER, \
            c0_rising_t_15 INTEGER, \
            c0_rising_a_15 INTEGER, \
            c0_rising_s_15 INTEGER, \
            c0_rising_p_15 INTEGER, \
            c0_Lowering_t_00 INTEGER, \
            c0_Lowering_a_00 INTEGER, \
            c0_Lowering_s_00 INTEGER, \
            c0_Lowering_p_00 INTEGER, \
            c0_Lowering_t_01 INTEGER, \
            c0_Lowering_a_01 INTEGER, \
            c0_Lowering_s_01 INTEGER, \
            c0_Lowering_p_01 INTEGER, \
            c0_Lowering_t_02 INTEGER, \
            c0_Lowering_a_02 INTEGER, \
            c0_Lowering_s_02 INTEGER, \
            c0_Lowering_p_02 INTEGER, \
            c0_Lowering_t_03 INTEGER, \
            c0_Lowering_a_03 INTEGER, \
            c0_Lowering_s_03 INTEGER, \
            c0_Lowering_p_03 INTEGER, \
            c0_Lowering_t_04 INTEGER, \
            c0_Lowering_a_04 INTEGER, \
            c0_Lowering_s_04 INTEGER, \
            c0_Lowering_p_04 INTEGER, \
            c0_Lowering_t_05 INTEGER, \
            c0_Lowering_a_05 INTEGER, \
            c0_Lowering_s_05 INTEGER, \
            c0_Lowering_p_05 INTEGER, \
            c0_Lowering_t_06 INTEGER, \
            c0_Lowering_a_06 INTEGER, \
            c0_Lowering_s_06 INTEGER, \
            c0_Lowering_p_06 INTEGER, \
            c0_Lowering_t_07 INTEGER, \
            c0_Lowering_a_07 INTEGER, \
            c0_Lowering_s_07 INTEGER, \
            c0_Lowering_p_07 INTEGER, \
            c0_Lowering_t_08 INTEGER, \
            c0_Lowering_a_08 INTEGER, \
            c0_Lowering_s_08 INTEGER, \
            c0_Lowering_p_08 INTEGER, \
            c0_Lowering_t_09 INTEGER, \
            c0_Lowering_a_09 INTEGER, \
            c0_Lowering_s_09 INTEGER, \
            c0_Lowering_p_09 INTEGER, \
            c0_Lowering_t_10 INTEGER, \
            c0_Lowering_a_10 INTEGER, \
            c0_Lowering_s_10 INTEGER, \
            c0_Lowering_p_10 INTEGER, \
            c0_Lowering_t_11 INTEGER, \
            c0_Lowering_a_11 INTEGER, \
            c0_Lowering_s_11 INTEGER, \
            c0_Lowering_p_11 INTEGER, \
            c0_Lowering_t_12 INTEGER, \
            c0_Lowering_a_12 INTEGER, \
            c0_Lowering_s_12 INTEGER, \
            c0_Lowering_p_12 INTEGER, \
            c0_Lowering_t_13 INTEGER, \
            c0_Lowering_a_13 INTEGER, \
            c0_Lowering_s_13 INTEGER, \
            c0_Lowering_p_13 INTEGER, \
            c0_Lowering_t_14 INTEGER, \
            c0_Lowering_a_14 INTEGER, \
            c0_Lowering_s_14 INTEGER, \
            c0_Lowering_p_14 INTEGER, \
            c0_Lowering_t_15 INTEGER, \
            c0_Lowering_a_15 INTEGER, \
            c0_Lowering_s_15 INTEGER, \
            c0_Lowering_p_15 INTEGER, \
            x1_rising_t_00 INTEGER, \
            x1_rising_a_00 INTEGER, \
            x1_rising_s_00 INTEGER, \
            x1_rising_p_00 INTEGER, \
            x1_rising_t_01 INTEGER, \
            x1_rising_a_01 INTEGER, \
            x1_rising_s_01 INTEGER, \
            x1_rising_p_01 INTEGER, \
            x1_rising_t_02 INTEGER, \
            x1_rising_a_02 INTEGER, \
            x1_rising_s_02 INTEGER, \
            x1_rising_p_02 INTEGER, \
            x1_rising_t_03 INTEGER, \
            x1_rising_a_03 INTEGER, \
            x1_rising_s_03 INTEGER, \
            x1_rising_p_03 INTEGER, \
            x1_rising_t_04 INTEGER, \
            x1_rising_a_04 INTEGER, \
            x1_rising_s_04 INTEGER, \
            x1_rising_p_04 INTEGER, \
            x1_rising_t_05 INTEGER, \
            x1_rising_a_05 INTEGER, \
            x1_rising_s_05 INTEGER, \
            x1_rising_p_05 INTEGER, \
            x1_rising_t_06 INTEGER, \
            x1_rising_a_06 INTEGER, \
            x1_rising_s_06 INTEGER, \
            x1_rising_p_06 INTEGER, \
            x1_rising_t_07 INTEGER, \
            x1_rising_a_07 INTEGER, \
            x1_rising_s_07 INTEGER, \
            x1_rising_p_07 INTEGER, \
            x1_rising_t_08 INTEGER, \
            x1_rising_a_08 INTEGER, \
            x1_rising_s_08 INTEGER, \
            x1_rising_p_08 INTEGER, \
            x1_rising_t_09 INTEGER, \
            x1_rising_a_09 INTEGER, \
            x1_rising_s_09 INTEGER, \
            x1_rising_p_09 INTEGER, \
            x1_rising_t_10 INTEGER, \
            x1_rising_a_10 INTEGER, \
            x1_rising_s_10 INTEGER, \
            x1_rising_p_10 INTEGER, \
            x1_rising_t_11 INTEGER, \
            x1_rising_a_11 INTEGER, \
            x1_rising_s_11 INTEGER, \
            x1_rising_p_11 INTEGER, \
            x1_rising_t_12 INTEGER, \
            x1_rising_a_12 INTEGER, \
            x1_rising_s_12 INTEGER, \
            x1_rising_p_12 INTEGER, \
            x1_rising_t_13 INTEGER, \
            x1_rising_a_13 INTEGER, \
            x1_rising_s_13 INTEGER, \
            x1_rising_p_13 INTEGER, \
            x1_rising_t_14 INTEGER, \
            x1_rising_a_14 INTEGER, \
            x1_rising_s_14 INTEGER, \
            x1_rising_p_14 INTEGER, \
            x1_rising_t_15 INTEGER, \
            x1_rising_a_15 INTEGER, \
            x1_rising_s_15 INTEGER, \
            x1_rising_p_15 INTEGER, \
            x1_Lowering_t_00 INTEGER, \
            x1_Lowering_a_00 INTEGER, \
            x1_Lowering_s_00 INTEGER, \
            x1_Lowering_p_00 INTEGER, \
            x1_Lowering_t_01 INTEGER, \
            x1_Lowering_a_01 INTEGER, \
            x1_Lowering_s_01 INTEGER, \
            x1_Lowering_p_01 INTEGER, \
            x1_Lowering_t_02 INTEGER, \
            x1_Lowering_a_02 INTEGER, \
            x1_Lowering_s_02 INTEGER, \
            x1_Lowering_p_02 INTEGER, \
            x1_Lowering_t_03 INTEGER, \
            x1_Lowering_a_03 INTEGER, \
            x1_Lowering_s_03 INTEGER, \
            x1_Lowering_p_03 INTEGER, \
            x1_Lowering_t_04 INTEGER, \
            x1_Lowering_a_04 INTEGER, \
            x1_Lowering_s_04 INTEGER, \
            x1_Lowering_p_04 INTEGER, \
            x1_Lowering_t_05 INTEGER, \
            x1_Lowering_a_05 INTEGER, \
            x1_Lowering_s_05 INTEGER, \
            x1_Lowering_p_05 INTEGER, \
            x1_Lowering_t_06 INTEGER, \
            x1_Lowering_a_06 INTEGER, \
            x1_Lowering_s_06 INTEGER, \
            x1_Lowering_p_06 INTEGER, \
            x1_Lowering_t_07 INTEGER, \
            x1_Lowering_a_07 INTEGER, \
            x1_Lowering_s_07 INTEGER, \
            x1_Lowering_p_07 INTEGER, \
            x1_Lowering_t_08 INTEGER, \
            x1_Lowering_a_08 INTEGER, \
            x1_Lowering_s_08 INTEGER, \
            x1_Lowering_p_08 INTEGER, \
            x1_Lowering_t_09 INTEGER, \
            x1_Lowering_a_09 INTEGER, \
            x1_Lowering_s_09 INTEGER, \
            x1_Lowering_p_09 INTEGER, \
            x1_Lowering_t_10 INTEGER, \
            x1_Lowering_a_10 INTEGER, \
            x1_Lowering_s_10 INTEGER, \
            x1_Lowering_p_10 INTEGER, \
            x1_Lowering_t_11 INTEGER, \
            x1_Lowering_a_11 INTEGER, \
            x1_Lowering_s_11 INTEGER, \
            x1_Lowering_p_11 INTEGER, \
            x1_Lowering_t_12 INTEGER, \
            x1_Lowering_a_12 INTEGER, \
            x1_Lowering_s_12 INTEGER, \
            x1_Lowering_p_12 INTEGER, \
            x1_Lowering_t_13 INTEGER, \
            x1_Lowering_a_13 INTEGER, \
            x1_Lowering_s_13 INTEGER, \
            x1_Lowering_p_13 INTEGER, \
            x1_Lowering_t_14 INTEGER, \
            x1_Lowering_a_14 INTEGER, \
            x1_Lowering_s_14 INTEGER, \
            x1_Lowering_p_14 INTEGER, \
            x1_Lowering_t_15 INTEGER, \
            x1_Lowering_a_15 INTEGER, \
            x1_Lowering_s_15 INTEGER, \
            x1_Lowering_p_15 INTEGER, \
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
    const String sql = String("SELECT * FROM buckets ORDER BY name ASC;");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName)
    {
        int i = 0;

        // Create an instance of a bucket
        Bucket bucket(String(argv[i++]).toInt(), argv[i++]);

        bucket.globalCorrectionFactor = String(argv[i++]).toInt();
        bucket.minAngle20x = String(argv[i++]).toInt();
        bucket.resetAngle10x = String(argv[i++]).toInt();
        bucket.addAngle10x = String(argv[i++]).toInt();
        bucket.maxAngle10x = String(argv[i++]).toInt();
        bucket.curve0WeightKg = String(argv[i++]).toInt();
        bucket.curveX1WeightKg = String(argv[i++]).toInt();

        // Curve0 rising
        for (int j = 0; j < MAX_SAMPLES; j++) {
            bucket.curve0Rising[j].timestamps = String(argv[i++]).toInt();
            bucket.curve0Rising[j].angle = String(argv[i++]).toInt();
            bucket.curve0Rising[j].speed = String(argv[i++]).toInt();
            bucket.curve0Rising[j].pressure = String(argv[i++]).toInt();
        }

        // Curve0 lowering
        for (int k = 0; k < MAX_SAMPLES; k++) {
            bucket.curve0Lowering[k].timestamps = String(argv[i++]).toInt();
            bucket.curve0Lowering[k].angle = String(argv[i++]).toInt();
            bucket.curve0Lowering[k].speed = String(argv[i++]).toInt();
            bucket.curve0Lowering[k].pressure = String(argv[i++]).toInt();
        }

        // CurveX1 rising
        for (int l = 0; l < MAX_SAMPLES; l++) {
            bucket.curveX1Rising[l].timestamps = String(argv[i++]).toInt();
            bucket.curveX1Rising[l].angle = String(argv[i++]).toInt();
            bucket.curveX1Rising[l].speed = String(argv[i++]).toInt();
            bucket.curveX1Rising[l].pressure = String(argv[i++]).toInt();
        }

        // CurveX1 lowering
        for (int m = 0; m < MAX_SAMPLES; m++) {
            bucket.curveX1Lowering[m].timestamps = String(argv[i++]).toInt();
            bucket.curveX1Lowering[m].angle = String(argv[i++]).toInt();
            bucket.curveX1Lowering[m].speed = String(argv[i++]).toInt();
            bucket.curveX1Lowering[m].pressure = String(argv[i++]).toInt();
        }

        // Perform the callback 
        return ((LISTBUCKETSCALLBACK)data)(&bucket);
    },
    (void*)callback, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}