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

    rc = sqlite3_exec(db, 
        "\
        CREATE TABLE limit_settings (\
            id                       INTEGER NOT NULL, \
            global_correction_factor INTEGER NOT NULL, \
            min_angle_20x            INTEGER NOT NULL, \
            reset_angle_10x          INTEGER NOT NULL, \
            add_angle_10x            INTEGER NOT NULL, \
            max_angle_10x            INTEGER NOT NULL, \
            c0_weight_kg             INTEGER NOT NULL, \
            x1_weight_kg             INTEGER NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE \
        );", 
        NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_rising table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_rising( \
        id  INTEGER NOT NULL, \
        t0  INTEGER NOT NULL, \
        a0  INTEGER NOT NULL, \
        s0  INTEGER NOT NULL, \
        p0  INTEGER NOT NULL, \
        t1  INTEGER NOT NULL, \
        a1  INTEGER NOT NULL, \
        s1  INTEGER NOT NULL, \
        p1  INTEGER NOT NULL, \
        t2  INTEGER NOT NULL, \
        a2  INTEGER NOT NULL, \
        s2  INTEGER NOT NULL, \
        p2  INTEGER NOT NULL, \
        t3  INTEGER NOT NULL, \
        a3  INTEGER NOT NULL, \
        s3  INTEGER NOT NULL, \
        p3  INTEGER NOT NULL, \
        t4  INTEGER NOT NULL, \
        a4  INTEGER NOT NULL, \
        s4  INTEGER NOT NULL, \
        p4  INTEGER NOT NULL, \
        t5  INTEGER NOT NULL, \
        a5  INTEGER NOT NULL, \
        s5  INTEGER NOT NULL, \
        p5  INTEGER NOT NULL, \
        t6  INTEGER NOT NULL, \
        a6  INTEGER NOT NULL, \
        s6  INTEGER NOT NULL, \
        p6  INTEGER NOT NULL, \
        t7  INTEGER NOT NULL, \
        a7  INTEGER NOT NULL, \
        s7  INTEGER NOT NULL, \
        p7  INTEGER NOT NULL, \
        t8  INTEGER NOT NULL, \
        a8  INTEGER NOT NULL, \
        s8  INTEGER NOT NULL, \
        p8  INTEGER NOT NULL, \
        t9  INTEGER NOT NULL, \
        a9  INTEGER NOT NULL, \
        s9  INTEGER NOT NULL, \
        p9  INTEGER NOT NULL, \
        t10  INTEGER NOT NULL, \
        a10  INTEGER NOT NULL, \
        s10  INTEGER NOT NULL, \
        p10  INTEGER NOT NULL, \
        t11  INTEGER NOT NULL, \
        a11  INTEGER NOT NULL, \
        s11  INTEGER NOT NULL, \
        p11  INTEGER NOT NULL, \
        t12  INTEGER NOT NULL, \
        a12  INTEGER NOT NULL, \
        s12  INTEGER NOT NULL, \
        p12  INTEGER NOT NULL, \
        t13  INTEGER NOT NULL, \
        a13  INTEGER NOT NULL, \
        s13  INTEGER NOT NULL, \
        p13  INTEGER NOT NULL, \
        t14  INTEGER NOT NULL, \
        a14  INTEGER NOT NULL, \
        s14  INTEGER NOT NULL, \
        p14  INTEGER NOT NULL, \
        t15  INTEGER NOT NULL, \
        a15  INTEGER NOT NULL, \
        s15  INTEGER NOT NULL, \
        p15  INTEGER NOT NULL, \
        PRIMARY KEY('id'), \
        FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE \
    );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_lowering table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_lowering( \
        id  INTEGER NOT NULL, \
        t0  INTEGER NOT NULL, \
        a0  INTEGER NOT NULL, \
        s0  INTEGER NOT NULL, \
        p0  INTEGER NOT NULL, \
        t1  INTEGER NOT NULL, \
        a1  INTEGER NOT NULL, \
        s1  INTEGER NOT NULL, \
        p1  INTEGER NOT NULL, \
        t2  INTEGER NOT NULL, \
        a2  INTEGER NOT NULL, \
        s2  INTEGER NOT NULL, \
        p2  INTEGER NOT NULL, \
        t3  INTEGER NOT NULL, \
        a3  INTEGER NOT NULL, \
        s3  INTEGER NOT NULL, \
        p3  INTEGER NOT NULL, \
        t4  INTEGER NOT NULL, \
        a4  INTEGER NOT NULL, \
        s4  INTEGER NOT NULL, \
        p4  INTEGER NOT NULL, \
        t5  INTEGER NOT NULL, \
        a5  INTEGER NOT NULL, \
        s5  INTEGER NOT NULL, \
        p5  INTEGER NOT NULL, \
        t6  INTEGER NOT NULL, \
        a6  INTEGER NOT NULL, \
        s6  INTEGER NOT NULL, \
        p6  INTEGER NOT NULL, \
        t7  INTEGER NOT NULL, \
        a7  INTEGER NOT NULL, \
        s7  INTEGER NOT NULL, \
        p7  INTEGER NOT NULL, \
        t8  INTEGER NOT NULL, \
        a8  INTEGER NOT NULL, \
        s8  INTEGER NOT NULL, \
        p8  INTEGER NOT NULL, \
        t9  INTEGER NOT NULL, \
        a9  INTEGER NOT NULL, \
        s9  INTEGER NOT NULL, \
        p9  INTEGER NOT NULL, \
        t10  INTEGER NOT NULL, \
        a10  INTEGER NOT NULL, \
        s10  INTEGER NOT NULL, \
        p10  INTEGER NOT NULL, \
        t11  INTEGER NOT NULL, \
        a11  INTEGER NOT NULL, \
        s11  INTEGER NOT NULL, \
        p11  INTEGER NOT NULL, \
        t12  INTEGER NOT NULL, \
        a12  INTEGER NOT NULL, \
        s12  INTEGER NOT NULL, \
        p12  INTEGER NOT NULL, \
        t13  INTEGER NOT NULL, \
        a13  INTEGER NOT NULL, \
        s13  INTEGER NOT NULL, \
        p13  INTEGER NOT NULL, \
        t14  INTEGER NOT NULL, \
        a14  INTEGER NOT NULL, \
        s14  INTEGER NOT NULL, \
        p14  INTEGER NOT NULL, \
        t15  INTEGER NOT NULL, \
        a15  INTEGER NOT NULL, \
        s15  INTEGER NOT NULL, \
        p15  INTEGER NOT NULL, \
        PRIMARY KEY('id'), \
        FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE \
    );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_rising table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_rising( \
        id  INTEGER NOT NULL, \
        t0  INTEGER NOT NULL, \
        a0  INTEGER NOT NULL, \
        s0  INTEGER NOT NULL, \
        p0  INTEGER NOT NULL, \
        t1  INTEGER NOT NULL, \
        a1  INTEGER NOT NULL, \
        s1  INTEGER NOT NULL, \
        p1  INTEGER NOT NULL, \
        t2  INTEGER NOT NULL, \
        a2  INTEGER NOT NULL, \
        s2  INTEGER NOT NULL, \
        p2  INTEGER NOT NULL, \
        t3  INTEGER NOT NULL, \
        a3  INTEGER NOT NULL, \
        s3  INTEGER NOT NULL, \
        p3  INTEGER NOT NULL, \
        t4  INTEGER NOT NULL, \
        a4  INTEGER NOT NULL, \
        s4  INTEGER NOT NULL, \
        p4  INTEGER NOT NULL, \
        t5  INTEGER NOT NULL, \
        a5  INTEGER NOT NULL, \
        s5  INTEGER NOT NULL, \
        p5  INTEGER NOT NULL, \
        t6  INTEGER NOT NULL, \
        a6  INTEGER NOT NULL, \
        s6  INTEGER NOT NULL, \
        p6  INTEGER NOT NULL, \
        t7  INTEGER NOT NULL, \
        a7  INTEGER NOT NULL, \
        s7  INTEGER NOT NULL, \
        p7  INTEGER NOT NULL, \
        t8  INTEGER NOT NULL, \
        a8  INTEGER NOT NULL, \
        s8  INTEGER NOT NULL, \
        p8  INTEGER NOT NULL, \
        t9  INTEGER NOT NULL, \
        a9  INTEGER NOT NULL, \
        s9  INTEGER NOT NULL, \
        p9  INTEGER NOT NULL, \
        t10  INTEGER NOT NULL, \
        a10  INTEGER NOT NULL, \
        s10  INTEGER NOT NULL, \
        p10  INTEGER NOT NULL, \
        t11  INTEGER NOT NULL, \
        a11  INTEGER NOT NULL, \
        s11  INTEGER NOT NULL, \
        p11  INTEGER NOT NULL, \
        t12  INTEGER NOT NULL, \
        a12  INTEGER NOT NULL, \
        s12  INTEGER NOT NULL, \
        p12  INTEGER NOT NULL, \
        t13  INTEGER NOT NULL, \
        a13  INTEGER NOT NULL, \
        s13  INTEGER NOT NULL, \
        p13  INTEGER NOT NULL, \
        t14  INTEGER NOT NULL, \
        a14  INTEGER NOT NULL, \
        s14  INTEGER NOT NULL, \
        p14  INTEGER NOT NULL, \
        t15  INTEGER NOT NULL, \
        a15  INTEGER NOT NULL, \
        s15  INTEGER NOT NULL, \
        p15  INTEGER NOT NULL, \
        PRIMARY KEY('id'), \
        FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE \
    );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_lowering table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_lowering( \
        id  INTEGER NOT NULL, \
        t0  INTEGER NOT NULL, \
        a0  INTEGER NOT NULL, \
        s0  INTEGER NOT NULL, \
        p0  INTEGER NOT NULL, \
        t1  INTEGER NOT NULL, \
        a1  INTEGER NOT NULL, \
        s1  INTEGER NOT NULL, \
        p1  INTEGER NOT NULL, \
        t2  INTEGER NOT NULL, \
        a2  INTEGER NOT NULL, \
        s2  INTEGER NOT NULL, \
        p2  INTEGER NOT NULL, \
        t3  INTEGER NOT NULL, \
        a3  INTEGER NOT NULL, \
        s3  INTEGER NOT NULL, \
        p3  INTEGER NOT NULL, \
        t4  INTEGER NOT NULL, \
        a4  INTEGER NOT NULL, \
        s4  INTEGER NOT NULL, \
        p4  INTEGER NOT NULL, \
        t5  INTEGER NOT NULL, \
        a5  INTEGER NOT NULL, \
        s5  INTEGER NOT NULL, \
        p5  INTEGER NOT NULL, \
        t6  INTEGER NOT NULL, \
        a6  INTEGER NOT NULL, \
        s6  INTEGER NOT NULL, \
        p6  INTEGER NOT NULL, \
        t7  INTEGER NOT NULL, \
        a7  INTEGER NOT NULL, \
        s7  INTEGER NOT NULL, \
        p7  INTEGER NOT NULL, \
        t8  INTEGER NOT NULL, \
        a8  INTEGER NOT NULL, \
        s8  INTEGER NOT NULL, \
        p8  INTEGER NOT NULL, \
        t9  INTEGER NOT NULL, \
        a9  INTEGER NOT NULL, \
        s9  INTEGER NOT NULL, \
        p9  INTEGER NOT NULL, \
        t10  INTEGER NOT NULL, \
        a10  INTEGER NOT NULL, \
        s10  INTEGER NOT NULL, \
        p10  INTEGER NOT NULL, \
        t11  INTEGER NOT NULL, \
        a11  INTEGER NOT NULL, \
        s11  INTEGER NOT NULL, \
        p11  INTEGER NOT NULL, \
        t12  INTEGER NOT NULL, \
        a12  INTEGER NOT NULL, \
        s12  INTEGER NOT NULL, \
        p12  INTEGER NOT NULL, \
        t13  INTEGER NOT NULL, \
        a13  INTEGER NOT NULL, \
        s13  INTEGER NOT NULL, \
        p13  INTEGER NOT NULL, \
        t14  INTEGER NOT NULL, \
        a14  INTEGER NOT NULL, \
        s14  INTEGER NOT NULL, \
        p14  INTEGER NOT NULL, \
        t15  INTEGER NOT NULL, \
        a15  INTEGER NOT NULL, \
        s15  INTEGER NOT NULL, \
        p15  INTEGER NOT NULL, \
        PRIMARY KEY('id'), \
        FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE \
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

boolean Db::limitSettingsExist(const int id)
{
    const String sql = 
    String("SELECT * FROM limit_settings WHERE id = ") + id + String(";");
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

void Db::addLimitSettings(const int id, const LimitSettings* settings)
{
    const String sql = String("\
        INSERT INTO limit_settings (\
            id,\
            global_correction_factor,\
            min_angle_20x,\
            reset_angle_10x,\
            add_angle_10x,\
            max_angle_10x,\
            c0_weight_kg,\
            x1_weight_kg \
        )\
        VALUES (\
            ") + id + String(",\
            ") + settings->global_correction_factor + String(", \
            ") + settings->min_angle_20x + String(", \
            ") + settings->reset_angle_10x + String(", \
            ") + settings->add_angle_10x + String(", \
            ") + settings->max_angle_10x + String(", \
            ") + settings->c0_weight_kg + String(", \
            ") + settings->x1_weight_kg + String(" \
        );"
    );

    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::updateLimitSettings(const int id, const LimitSettings* settings)
{
    const String sql = String("\
        UPDATE limit_settings \
        SET \ 
            global_correction_factor = ") + settings->global_correction_factor + String(", \
            min_angle_20x = ") + settings->min_angle_20x + String(", \
            reset_angle_10x = ") + settings->reset_angle_10x + String(", \
            add_angle_10x = ") + settings->add_angle_10x + String(", \
            max_angle_10x = ") + settings->max_angle_10x + String(", \
            c0_weight_kg = ") + settings->c0_weight_kg + String(", \
            x1_weight_kg = ") + settings->x1_weight_kg + String(" \
        WHERE id = ") + id + String(";");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::c0RisingExists(const int id)
{
    const String sql = 
        String("SELECT * FROM c0_rising WHERE id = ") + id + String(";");
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

void Db::addC0Rising(const int id, const CalibrationSample* samples)
{
    // Insert the new bucket
    const String sql = String("\
    INSERT INTO c0_rising (\
        id,\
        t0,\
        a0,\
        s0,\
        p0,\
        t1,\
        a1,\
        s1,\
        p1,\
        t2,\
        a2,\
        s2,\
        p2,\
        t3,\
        a3,\
        s3,\
        p3,\
        t4,\
        a4,\
        s4,\
        p4,\
        t5,\
        a5,\
        s5,\
        p5,\
        t6,\
        a6,\
        s6,\
        p6,\
        t7,\
        a7,\
        s7,\
        p7,\
        t8,\
        a8,\
        s8,\
        p8,\
        t9,\
        a9,\
        s9,\
        p9,\
        t10,\
        a10,\
        s10,\
        p10,\
        t11,\
        a11,\
        s11,\
        p11,\
        t12,\
        a12,\
        s12,\
        p12,\
        t13,\
        a13,\
        s13,\
        p13,\
        t14,\
        a14,\
        s14,\
        p14,\
        t15,\
        a15,\
        s15,\
        p15\
    )\
    VALUES (\
        ") + id + String(",\
        ") + samples[0].timestamps + String(", \
        ") + samples[0].angle + String(", \
        ") + samples[0].speed + String(", \
        ") + samples[0].pressure + String(", \
        ") + samples[1].timestamps + String(", \
        ") + samples[1].angle + String(", \
        ") + samples[1].speed + String(", \
        ") + samples[1].pressure + String(", \
        ") + samples[2].timestamps + String(", \
        ") + samples[2].angle + String(", \
        ") + samples[2].speed + String(", \
        ") + samples[2].pressure + String(", \
        ") + samples[3].timestamps + String(", \
        ") + samples[3].angle + String(", \
        ") + samples[3].speed + String(", \
        ") + samples[3].pressure + String(", \
        ") + samples[4].timestamps + String(", \
        ") + samples[4].angle + String(", \
        ") + samples[4].speed + String(", \
        ") + samples[4].pressure + String(", \
        ") + samples[5].timestamps + String(", \
        ") + samples[5].angle + String(", \
        ") + samples[5].speed + String(", \
        ") + samples[5].pressure + String(", \
        ") + samples[6].timestamps + String(", \
        ") + samples[6].angle + String(", \
        ") + samples[6].speed + String(", \
        ") + samples[6].pressure + String(", \
        ") + samples[7].timestamps + String(", \
        ") + samples[7].angle + String(", \
        ") + samples[7].speed + String(", \
        ") + samples[7].pressure + String(", \
        ") + samples[8].timestamps + String(", \
        ") + samples[8].angle + String(", \
        ") + samples[8].speed + String(", \
        ") + samples[8].pressure + String(", \
        ") + samples[9].timestamps + String(", \
        ") + samples[9].angle + String(", \
        ") + samples[9].speed + String(", \
        ") + samples[9].pressure + String(", \
        ") + samples[10].timestamps + String(", \
        ") + samples[10].angle + String(", \
        ") + samples[10].speed + String(", \
        ") + samples[10].pressure + String(", \
        ") + samples[11].timestamps + String(", \
        ") + samples[11].angle + String(", \
        ") + samples[11].speed + String(", \
        ") + samples[11].pressure + String(", \
        ") + samples[12].timestamps + String(", \
        ") + samples[12].angle + String(", \
        ") + samples[12].speed + String(", \
        ") + samples[12].pressure + String(", \
        ") + samples[13].timestamps + String(", \
        ") + samples[13].angle + String(", \
        ") + samples[13].speed + String(", \
        ") + samples[13].pressure + String(", \
        ") + samples[14].timestamps + String(", \
        ") + samples[14].angle + String(", \
        ") + samples[14].speed + String(", \
        ") + samples[14].pressure + String(", \
        ") + samples[15].timestamps + String(", \
        ") + samples[15].angle + String(", \
        ") + samples[15].speed + String(", \
        ") + samples[15].pressure + String(" \
    );");

    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::updateC0Rising(const int id, const CalibrationSample* samples)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE c0_rising \
        SET \ 
        t0 = ") + samples[0].timestamps + String(", \
        a0 = ") + samples[0].angle + String(", \
        s0 = ") + samples[0].speed + String(", \
        p0 = ") + samples[0].pressure + String(", \
        t1 = ") + samples[1].timestamps + String(", \
        a1 = ") + samples[1].angle + String(", \
        s1 = ") + samples[1].speed + String(", \
        p1 = ") + samples[1].pressure + String(", \
        t2 = ") + samples[2].timestamps + String(", \
        a2 = ") + samples[2].angle + String(", \
        s2 = ") + samples[2].speed + String(", \
        p2 = ") + samples[2].pressure + String(", \
        t3 = ") + samples[3].timestamps + String(", \
        a3 = ") + samples[3].angle + String(", \
        s3 = ") + samples[3].speed + String(", \
        p3 = ") + samples[3].pressure + String(", \
        t4 = ") + samples[4].timestamps + String(", \
        a4 = ") + samples[4].angle + String(", \
        s4 = ") + samples[4].speed + String(", \
        p4 = ") + samples[4].pressure + String(", \
        t5 = ") + samples[5].timestamps + String(", \
        a5 = ") + samples[5].angle + String(", \
        s5 = ") + samples[5].speed + String(", \
        p5 = ") + samples[5].pressure + String(", \
        t6 = ") + samples[6].timestamps + String(", \
        a6 = ") + samples[6].angle + String(", \
        s6 = ") + samples[6].speed + String(", \
        p6 = ") + samples[6].pressure + String(", \
        t7 = ") + samples[7].timestamps + String(", \
        a7 = ") + samples[7].angle + String(", \
        s7 = ") + samples[7].speed + String(", \
        p7 = ") + samples[7].pressure + String(", \
        t8 = ") + samples[8].timestamps + String(", \
        a8 = ") + samples[8].angle + String(", \
        s8 = ") + samples[8].speed + String(", \
        p8 = ") + samples[8].pressure + String(", \
        t9 = ") + samples[9].timestamps + String(", \
        a9 = ") + samples[9].angle + String(", \
        s9 = ") + samples[9].speed + String(", \
        p9 = ") + samples[9].pressure + String(", \
        t10 = ") + samples[10].timestamps + String(", \
        a10 = ") + samples[10].angle + String(", \
        s10 = ") + samples[10].speed + String(", \
        p10 = ") + samples[10].pressure + String(", \
        t11 = ") + samples[11].timestamps + String(", \
        a11 = ") + samples[11].angle + String(", \
        s11 = ") + samples[11].speed + String(", \
        p11 = ") + samples[11].pressure + String(", \
        t12 = ") + samples[12].timestamps + String(", \
        a12 = ") + samples[12].angle + String(", \
        s12 = ") + samples[12].speed + String(", \
        p12 = ") + samples[12].pressure + String(", \
        t13 = ") + samples[13].timestamps + String(", \
        a13 = ") + samples[13].angle + String(", \
        s13 = ") + samples[13].speed + String(", \
        p13 = ") + samples[13].pressure + String(", \
        t14 = ") + samples[14].timestamps + String(", \
        a14 = ") + samples[14].angle + String(", \
        s14 = ") + samples[14].speed + String(", \
        p14 = ") + samples[14].pressure + String(", \
        t15 = ") + samples[15].timestamps + String(", \
        a15 = ") + samples[15].angle + String(", \
        s15 = ") + samples[15].speed + String(", \
        p15 = ") + samples[15].pressure + String(" \
        WHERE id = ") + id + String(";");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::c0LoweringExists(const int id)
{
    const String sql = 
    String("SELECT * FROM c0_lowering WHERE id = ") + id + String(";");
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

void Db::addC0Lowering(const int id, const CalibrationSample* samples)
{
    // Insert the new bucket
    const String sql = String("\
    INSERT INTO c0_lowering (\
        id,\
        t0,\
        a0,\
        s0,\
        p0,\
        t1,\
        a1,\
        s1,\
        p1,\
        t2,\
        a2,\
        s2,\
        p2,\
        t3,\
        a3,\
        s3,\
        p3,\
        t4,\
        a4,\
        s4,\
        p4,\
        t5,\
        a5,\
        s5,\
        p5,\
        t6,\
        a6,\
        s6,\
        p6,\
        t7,\
        a7,\
        s7,\
        p7,\
        t8,\
        a8,\
        s8,\
        p8,\
        t9,\
        a9,\
        s9,\
        p9,\
        t10,\
        a10,\
        s10,\
        p10,\
        t11,\
        a11,\
        s11,\
        p11,\
        t12,\
        a12,\
        s12,\
        p12,\
        t13,\
        a13,\
        s13,\
        p13,\
        t14,\
        a14,\
        s14,\
        p14,\
        t15,\
        a15,\
        s15,\
        p15\
    )\
    VALUES (\
        ") + id + String(",\
        ") + samples[0].timestamps + String(", \
        ") + samples[0].angle + String(", \
        ") + samples[0].speed + String(", \
        ") + samples[0].pressure + String(", \
        ") + samples[1].timestamps + String(", \
        ") + samples[1].angle + String(", \
        ") + samples[1].speed + String(", \
        ") + samples[1].pressure + String(", \
        ") + samples[2].timestamps + String(", \
        ") + samples[2].angle + String(", \
        ") + samples[2].speed + String(", \
        ") + samples[2].pressure + String(", \
        ") + samples[3].timestamps + String(", \
        ") + samples[3].angle + String(", \
        ") + samples[3].speed + String(", \
        ") + samples[3].pressure + String(", \
        ") + samples[4].timestamps + String(", \
        ") + samples[4].angle + String(", \
        ") + samples[4].speed + String(", \
        ") + samples[4].pressure + String(", \
        ") + samples[5].timestamps + String(", \
        ") + samples[5].angle + String(", \
        ") + samples[5].speed + String(", \
        ") + samples[5].pressure + String(", \
        ") + samples[6].timestamps + String(", \
        ") + samples[6].angle + String(", \
        ") + samples[6].speed + String(", \
        ") + samples[6].pressure + String(", \
        ") + samples[7].timestamps + String(", \
        ") + samples[7].angle + String(", \
        ") + samples[7].speed + String(", \
        ") + samples[7].pressure + String(", \
        ") + samples[8].timestamps + String(", \
        ") + samples[8].angle + String(", \
        ") + samples[8].speed + String(", \
        ") + samples[8].pressure + String(", \
        ") + samples[9].timestamps + String(", \
        ") + samples[9].angle + String(", \
        ") + samples[9].speed + String(", \
        ") + samples[9].pressure + String(", \
        ") + samples[10].timestamps + String(", \
        ") + samples[10].angle + String(", \
        ") + samples[10].speed + String(", \
        ") + samples[10].pressure + String(", \
        ") + samples[11].timestamps + String(", \
        ") + samples[11].angle + String(", \
        ") + samples[11].speed + String(", \
        ") + samples[11].pressure + String(", \
        ") + samples[12].timestamps + String(", \
        ") + samples[12].angle + String(", \
        ") + samples[12].speed + String(", \
        ") + samples[12].pressure + String(", \
        ") + samples[13].timestamps + String(", \
        ") + samples[13].angle + String(", \
        ") + samples[13].speed + String(", \
        ") + samples[13].pressure + String(", \
        ") + samples[14].timestamps + String(", \
        ") + samples[14].angle + String(", \
        ") + samples[14].speed + String(", \
        ") + samples[14].pressure + String(", \
        ") + samples[15].timestamps + String(", \
        ") + samples[15].angle + String(", \
        ") + samples[15].speed + String(", \
        ") + samples[15].pressure + String(" \
    );");

    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::updateC0Lowering(const int id, const CalibrationSample* samples)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE c0_lowering \
        SET \ 
        t0 = ") + samples[0].timestamps + String(", \
        a0 = ") + samples[0].angle + String(", \
        s0 = ") + samples[0].speed + String(", \
        p0 = ") + samples[0].pressure + String(", \
        t1 = ") + samples[1].timestamps + String(", \
        a1 = ") + samples[1].angle + String(", \
        s1 = ") + samples[1].speed + String(", \
        p1 = ") + samples[1].pressure + String(", \
        t2 = ") + samples[2].timestamps + String(", \
        a2 = ") + samples[2].angle + String(", \
        s2 = ") + samples[2].speed + String(", \
        p2 = ") + samples[2].pressure + String(", \
        t3 = ") + samples[3].timestamps + String(", \
        a3 = ") + samples[3].angle + String(", \
        s3 = ") + samples[3].speed + String(", \
        p3 = ") + samples[3].pressure + String(", \
        t4 = ") + samples[4].timestamps + String(", \
        a4 = ") + samples[4].angle + String(", \
        s4 = ") + samples[4].speed + String(", \
        p4 = ") + samples[4].pressure + String(", \
        t5 = ") + samples[5].timestamps + String(", \
        a5 = ") + samples[5].angle + String(", \
        s5 = ") + samples[5].speed + String(", \
        p5 = ") + samples[5].pressure + String(", \
        t6 = ") + samples[6].timestamps + String(", \
        a6 = ") + samples[6].angle + String(", \
        s6 = ") + samples[6].speed + String(", \
        p6 = ") + samples[6].pressure + String(", \
        t7 = ") + samples[7].timestamps + String(", \
        a7 = ") + samples[7].angle + String(", \
        s7 = ") + samples[7].speed + String(", \
        p7 = ") + samples[7].pressure + String(", \
        t8 = ") + samples[8].timestamps + String(", \
        a8 = ") + samples[8].angle + String(", \
        s8 = ") + samples[8].speed + String(", \
        p8 = ") + samples[8].pressure + String(", \
        t9 = ") + samples[9].timestamps + String(", \
        a9 = ") + samples[9].angle + String(", \
        s9 = ") + samples[9].speed + String(", \
        p9 = ") + samples[9].pressure + String(", \
        t10 = ") + samples[10].timestamps + String(", \
        a10 = ") + samples[10].angle + String(", \
        s10 = ") + samples[10].speed + String(", \
        p10 = ") + samples[10].pressure + String(", \
        t11 = ") + samples[11].timestamps + String(", \
        a11 = ") + samples[11].angle + String(", \
        s11 = ") + samples[11].speed + String(", \
        p11 = ") + samples[11].pressure + String(", \
        t12 = ") + samples[12].timestamps + String(", \
        a12 = ") + samples[12].angle + String(", \
        s12 = ") + samples[12].speed + String(", \
        p12 = ") + samples[12].pressure + String(", \
        t13 = ") + samples[13].timestamps + String(", \
        a13 = ") + samples[13].angle + String(", \
        s13 = ") + samples[13].speed + String(", \
        p13 = ") + samples[13].pressure + String(", \
        t14 = ") + samples[14].timestamps + String(", \
        a14 = ") + samples[14].angle + String(", \
        s14 = ") + samples[14].speed + String(", \
        p14 = ") + samples[14].pressure + String(", \
        t15 = ") + samples[15].timestamps + String(", \
        a15 = ") + samples[15].angle + String(", \
        s15 = ") + samples[15].speed + String(", \
        p15 = ") + samples[15].pressure + String(" \
        WHERE id = ") + id + String(";");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::x1RisingExists(const int id)
{
    const String sql = 
    String("SELECT * FROM x1_rising WHERE id = ") + id + String(";");
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

void Db::addX1Rising(const int id, const CalibrationSample* samples)
{
    // Insert the new bucket
    const String sql = String("\
    INSERT INTO x1_rising (\
        id,\
        t0,\
        a0,\
        s0,\
        p0,\
        t1,\
        a1,\
        s1,\
        p1,\
        t2,\
        a2,\
        s2,\
        p2,\
        t3,\
        a3,\
        s3,\
        p3,\
        t4,\
        a4,\
        s4,\
        p4,\
        t5,\
        a5,\
        s5,\
        p5,\
        t6,\
        a6,\
        s6,\
        p6,\
        t7,\
        a7,\
        s7,\
        p7,\
        t8,\
        a8,\
        s8,\
        p8,\
        t9,\
        a9,\
        s9,\
        p9,\
        t10,\
        a10,\
        s10,\
        p10,\
        t11,\
        a11,\
        s11,\
        p11,\
        t12,\
        a12,\
        s12,\
        p12,\
        t13,\
        a13,\
        s13,\
        p13,\
        t14,\
        a14,\
        s14,\
        p14,\
        t15,\
        a15,\
        s15,\
        p15\
    )\
    VALUES (\
        ") + id + String(",\
        ") + samples[0].timestamps + String(", \
        ") + samples[0].angle + String(", \
        ") + samples[0].speed + String(", \
        ") + samples[0].pressure + String(", \
        ") + samples[1].timestamps + String(", \
        ") + samples[1].angle + String(", \
        ") + samples[1].speed + String(", \
        ") + samples[1].pressure + String(", \
        ") + samples[2].timestamps + String(", \
        ") + samples[2].angle + String(", \
        ") + samples[2].speed + String(", \
        ") + samples[2].pressure + String(", \
        ") + samples[3].timestamps + String(", \
        ") + samples[3].angle + String(", \
        ") + samples[3].speed + String(", \
        ") + samples[3].pressure + String(", \
        ") + samples[4].timestamps + String(", \
        ") + samples[4].angle + String(", \
        ") + samples[4].speed + String(", \
        ") + samples[4].pressure + String(", \
        ") + samples[5].timestamps + String(", \
        ") + samples[5].angle + String(", \
        ") + samples[5].speed + String(", \
        ") + samples[5].pressure + String(", \
        ") + samples[6].timestamps + String(", \
        ") + samples[6].angle + String(", \
        ") + samples[6].speed + String(", \
        ") + samples[6].pressure + String(", \
        ") + samples[7].timestamps + String(", \
        ") + samples[7].angle + String(", \
        ") + samples[7].speed + String(", \
        ") + samples[7].pressure + String(", \
        ") + samples[8].timestamps + String(", \
        ") + samples[8].angle + String(", \
        ") + samples[8].speed + String(", \
        ") + samples[8].pressure + String(", \
        ") + samples[9].timestamps + String(", \
        ") + samples[9].angle + String(", \
        ") + samples[9].speed + String(", \
        ") + samples[9].pressure + String(", \
        ") + samples[10].timestamps + String(", \
        ") + samples[10].angle + String(", \
        ") + samples[10].speed + String(", \
        ") + samples[10].pressure + String(", \
        ") + samples[11].timestamps + String(", \
        ") + samples[11].angle + String(", \
        ") + samples[11].speed + String(", \
        ") + samples[11].pressure + String(", \
        ") + samples[12].timestamps + String(", \
        ") + samples[12].angle + String(", \
        ") + samples[12].speed + String(", \
        ") + samples[12].pressure + String(", \
        ") + samples[13].timestamps + String(", \
        ") + samples[13].angle + String(", \
        ") + samples[13].speed + String(", \
        ") + samples[13].pressure + String(", \
        ") + samples[14].timestamps + String(", \
        ") + samples[14].angle + String(", \
        ") + samples[14].speed + String(", \
        ") + samples[14].pressure + String(", \
        ") + samples[15].timestamps + String(", \
        ") + samples[15].angle + String(", \
        ") + samples[15].speed + String(", \
        ") + samples[15].pressure + String(" \
    );");

    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::updateX1Rising(const int id, const CalibrationSample* samples)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE x1_rising \
        SET \ 
        t0 = ") + samples[0].timestamps + String(", \
        a0 = ") + samples[0].angle + String(", \
        s0 = ") + samples[0].speed + String(", \
        p0 = ") + samples[0].pressure + String(", \
        t1 = ") + samples[1].timestamps + String(", \
        a1 = ") + samples[1].angle + String(", \
        s1 = ") + samples[1].speed + String(", \
        p1 = ") + samples[1].pressure + String(", \
        t2 = ") + samples[2].timestamps + String(", \
        a2 = ") + samples[2].angle + String(", \
        s2 = ") + samples[2].speed + String(", \
        p2 = ") + samples[2].pressure + String(", \
        t3 = ") + samples[3].timestamps + String(", \
        a3 = ") + samples[3].angle + String(", \
        s3 = ") + samples[3].speed + String(", \
        p3 = ") + samples[3].pressure + String(", \
        t4 = ") + samples[4].timestamps + String(", \
        a4 = ") + samples[4].angle + String(", \
        s4 = ") + samples[4].speed + String(", \
        p4 = ") + samples[4].pressure + String(", \
        t5 = ") + samples[5].timestamps + String(", \
        a5 = ") + samples[5].angle + String(", \
        s5 = ") + samples[5].speed + String(", \
        p5 = ") + samples[5].pressure + String(", \
        t6 = ") + samples[6].timestamps + String(", \
        a6 = ") + samples[6].angle + String(", \
        s6 = ") + samples[6].speed + String(", \
        p6 = ") + samples[6].pressure + String(", \
        t7 = ") + samples[7].timestamps + String(", \
        a7 = ") + samples[7].angle + String(", \
        s7 = ") + samples[7].speed + String(", \
        p7 = ") + samples[7].pressure + String(", \
        t8 = ") + samples[8].timestamps + String(", \
        a8 = ") + samples[8].angle + String(", \
        s8 = ") + samples[8].speed + String(", \
        p8 = ") + samples[8].pressure + String(", \
        t9 = ") + samples[9].timestamps + String(", \
        a9 = ") + samples[9].angle + String(", \
        s9 = ") + samples[9].speed + String(", \
        p9 = ") + samples[9].pressure + String(", \
        t10 = ") + samples[10].timestamps + String(", \
        a10 = ") + samples[10].angle + String(", \
        s10 = ") + samples[10].speed + String(", \
        p10 = ") + samples[10].pressure + String(", \
        t11 = ") + samples[11].timestamps + String(", \
        a11 = ") + samples[11].angle + String(", \
        s11 = ") + samples[11].speed + String(", \
        p11 = ") + samples[11].pressure + String(", \
        t12 = ") + samples[12].timestamps + String(", \
        a12 = ") + samples[12].angle + String(", \
        s12 = ") + samples[12].speed + String(", \
        p12 = ") + samples[12].pressure + String(", \
        t13 = ") + samples[13].timestamps + String(", \
        a13 = ") + samples[13].angle + String(", \
        s13 = ") + samples[13].speed + String(", \
        p13 = ") + samples[13].pressure + String(", \
        t14 = ") + samples[14].timestamps + String(", \
        a14 = ") + samples[14].angle + String(", \
        s14 = ") + samples[14].speed + String(", \
        p14 = ") + samples[14].pressure + String(", \
        t15 = ") + samples[15].timestamps + String(", \
        a15 = ") + samples[15].angle + String(", \
        s15 = ") + samples[15].speed + String(", \
        p15 = ") + samples[15].pressure + String(" \
        WHERE id = ") + id + String(";");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::x1LoweringExists(const int id)
{
    const String sql = 
    String("SELECT * FROM x1_lowering WHERE id = ") + id + String(";");
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

void Db::addX1Lowering(const int id, const CalibrationSample* samples)
{
    // Insert the new bucket
    const String sql = String("\
    INSERT INTO x1_lowering (\
        id,\
        t0,\
        a0,\
        s0,\
        p0,\
        t1,\
        a1,\
        s1,\
        p1,\
        t2,\
        a2,\
        s2,\
        p2,\
        t3,\
        a3,\
        s3,\
        p3,\
        t4,\
        a4,\
        s4,\
        p4,\
        t5,\
        a5,\
        s5,\
        p5,\
        t6,\
        a6,\
        s6,\
        p6,\
        t7,\
        a7,\
        s7,\
        p7,\
        t8,\
        a8,\
        s8,\
        p8,\
        t9,\
        a9,\
        s9,\
        p9,\
        t10,\
        a10,\
        s10,\
        p10,\
        t11,\
        a11,\
        s11,\
        p11,\
        t12,\
        a12,\
        s12,\
        p12,\
        t13,\
        a13,\
        s13,\
        p13,\
        t14,\
        a14,\
        s14,\
        p14,\
        t15,\
        a15,\
        s15,\
        p15\
    )\
    VALUES (\
        ") + id + String(",\
        ") + samples[0].timestamps + String(", \
        ") + samples[0].angle + String(", \
        ") + samples[0].speed + String(", \
        ") + samples[0].pressure + String(", \
        ") + samples[1].timestamps + String(", \
        ") + samples[1].angle + String(", \
        ") + samples[1].speed + String(", \
        ") + samples[1].pressure + String(", \
        ") + samples[2].timestamps + String(", \
        ") + samples[2].angle + String(", \
        ") + samples[2].speed + String(", \
        ") + samples[2].pressure + String(", \
        ") + samples[3].timestamps + String(", \
        ") + samples[3].angle + String(", \
        ") + samples[3].speed + String(", \
        ") + samples[3].pressure + String(", \
        ") + samples[4].timestamps + String(", \
        ") + samples[4].angle + String(", \
        ") + samples[4].speed + String(", \
        ") + samples[4].pressure + String(", \
        ") + samples[5].timestamps + String(", \
        ") + samples[5].angle + String(", \
        ") + samples[5].speed + String(", \
        ") + samples[5].pressure + String(", \
        ") + samples[6].timestamps + String(", \
        ") + samples[6].angle + String(", \
        ") + samples[6].speed + String(", \
        ") + samples[6].pressure + String(", \
        ") + samples[7].timestamps + String(", \
        ") + samples[7].angle + String(", \
        ") + samples[7].speed + String(", \
        ") + samples[7].pressure + String(", \
        ") + samples[8].timestamps + String(", \
        ") + samples[8].angle + String(", \
        ") + samples[8].speed + String(", \
        ") + samples[8].pressure + String(", \
        ") + samples[9].timestamps + String(", \
        ") + samples[9].angle + String(", \
        ") + samples[9].speed + String(", \
        ") + samples[9].pressure + String(", \
        ") + samples[10].timestamps + String(", \
        ") + samples[10].angle + String(", \
        ") + samples[10].speed + String(", \
        ") + samples[10].pressure + String(", \
        ") + samples[11].timestamps + String(", \
        ") + samples[11].angle + String(", \
        ") + samples[11].speed + String(", \
        ") + samples[11].pressure + String(", \
        ") + samples[12].timestamps + String(", \
        ") + samples[12].angle + String(", \
        ") + samples[12].speed + String(", \
        ") + samples[12].pressure + String(", \
        ") + samples[13].timestamps + String(", \
        ") + samples[13].angle + String(", \
        ") + samples[13].speed + String(", \
        ") + samples[13].pressure + String(", \
        ") + samples[14].timestamps + String(", \
        ") + samples[14].angle + String(", \
        ") + samples[14].speed + String(", \
        ") + samples[14].pressure + String(", \
        ") + samples[15].timestamps + String(", \
        ") + samples[15].angle + String(", \
        ") + samples[15].speed + String(", \
        ") + samples[15].pressure + String(" \
    );");

    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}

void Db::updateX1Lowering(const int id, const CalibrationSample* samples)
{
    // Update the bucket name
    const String sql = String("\
        UPDATE x1_lowering \
        SET \ 
        t0 = ") + samples[0].timestamps + String(", \
        a0 = ") + samples[0].angle + String(", \
        s0 = ") + samples[0].speed + String(", \
        p0 = ") + samples[0].pressure + String(", \
        t1 = ") + samples[1].timestamps + String(", \
        a1 = ") + samples[1].angle + String(", \
        s1 = ") + samples[1].speed + String(", \
        p1 = ") + samples[1].pressure + String(", \
        t2 = ") + samples[2].timestamps + String(", \
        a2 = ") + samples[2].angle + String(", \
        s2 = ") + samples[2].speed + String(", \
        p2 = ") + samples[2].pressure + String(", \
        t3 = ") + samples[3].timestamps + String(", \
        a3 = ") + samples[3].angle + String(", \
        s3 = ") + samples[3].speed + String(", \
        p3 = ") + samples[3].pressure + String(", \
        t4 = ") + samples[4].timestamps + String(", \
        a4 = ") + samples[4].angle + String(", \
        s4 = ") + samples[4].speed + String(", \
        p4 = ") + samples[4].pressure + String(", \
        t5 = ") + samples[5].timestamps + String(", \
        a5 = ") + samples[5].angle + String(", \
        s5 = ") + samples[5].speed + String(", \
        p5 = ") + samples[5].pressure + String(", \
        t6 = ") + samples[6].timestamps + String(", \
        a6 = ") + samples[6].angle + String(", \
        s6 = ") + samples[6].speed + String(", \
        p6 = ") + samples[6].pressure + String(", \
        t7 = ") + samples[7].timestamps + String(", \
        a7 = ") + samples[7].angle + String(", \
        s7 = ") + samples[7].speed + String(", \
        p7 = ") + samples[7].pressure + String(", \
        t8 = ") + samples[8].timestamps + String(", \
        a8 = ") + samples[8].angle + String(", \
        s8 = ") + samples[8].speed + String(", \
        p8 = ") + samples[8].pressure + String(", \
        t9 = ") + samples[9].timestamps + String(", \
        a9 = ") + samples[9].angle + String(", \
        s9 = ") + samples[9].speed + String(", \
        p9 = ") + samples[9].pressure + String(", \
        t10 = ") + samples[10].timestamps + String(", \
        a10 = ") + samples[10].angle + String(", \
        s10 = ") + samples[10].speed + String(", \
        p10 = ") + samples[10].pressure + String(", \
        t11 = ") + samples[11].timestamps + String(", \
        a11 = ") + samples[11].angle + String(", \
        s11 = ") + samples[11].speed + String(", \
        p11 = ") + samples[11].pressure + String(", \
        t12 = ") + samples[12].timestamps + String(", \
        a12 = ") + samples[12].angle + String(", \
        s12 = ") + samples[12].speed + String(", \
        p12 = ") + samples[12].pressure + String(", \
        t13 = ") + samples[13].timestamps + String(", \
        a13 = ") + samples[13].angle + String(", \
        s13 = ") + samples[13].speed + String(", \
        p13 = ") + samples[13].pressure + String(", \
        t14 = ") + samples[14].timestamps + String(", \
        a14 = ") + samples[14].angle + String(", \
        s14 = ") + samples[14].speed + String(", \
        p14 = ") + samples[14].pressure + String(", \
        t15 = ") + samples[15].timestamps + String(", \
        a15 = ") + samples[15].angle + String(", \
        s15 = ") + samples[15].speed + String(", \
        p15 = ") + samples[15].pressure + String(" \
        WHERE id = ") + id + String(";");
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);
}
