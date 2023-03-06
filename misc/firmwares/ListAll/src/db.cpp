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
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
        NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_rising table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_rising (\
            id      INTEGER   NOT NULL, \
            samples blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_lowering table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_lowering (\
            id      INTEGER   NOT NULL, \
            samples blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_rising table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_rising (\
            id      INTEGER   NOT NULL, \
            samples blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_lowering table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_lowering (\
            id      INTEGER   NOT NULL, \
            samples blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_low_speed_factor table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_low_speed_factors (\
            id      INTEGER   NOT NULL, \
            factors blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the c0_high_speed_factor table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE c0_high_speed_factors (\
            id      INTEGER   NOT NULL, \
            factors blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_low_speed_factor table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_low_speed_factors (\
            id      INTEGER   NOT NULL, \
            factors blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
        );", 
    NULL, NULL, &errMsg);
    ASSERT(rc == SQLITE_OK, errMsg);

    // Create the x1_high_speed_factor table 
    rc = sqlite3_exec(db, "\
        CREATE TABLE x1_high_speed_factors (\
            id      INTEGER   NOT NULL, \
            factors blob NOT NULL, \
            PRIMARY KEY('id'), \
            FOREIGN KEY (id) REFERENCES buckets(id) ON DELETE CASCADE\
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

int Db::query(const char* sql, sqlite3_callback callback, void* data, char** errMsg)
{
   return sqlite3_exec(db, sql, callback, data, errMsg);
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
    Statement stmt(db);
    stmt.prepare("INSERT INTO buckets (id, name) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, name);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

int Db::getBucketId(const String& name)
{
    Statement stmt(db);
    stmt.prepare("SELECT id FROM buckets WHERE name = ?");
    stmt.bind(1, name);
    int rc = stmt.step();
    return rc == SQLITE_DONE ? NoRecordFound : stmt.getColumnInt(1);
}

void Db::updateBucket(const int id, const String& name)
{
    Statement stmt(db);
    stmt.prepare("UPDATE buckets SET name = ? WHERE id = ?");
    stmt.bind(1, name);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

boolean Db::bucketExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM buckets WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::deleteBucket(const int id)
{
    Statement stmt(db);
    stmt.prepare("DELETE FROM buckets WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::listBuckets(LISTBUCKETSCALLBACK callback)
{
    const String sql = String("SELECT id, name FROM buckets ORDER BY name ASC;");
    char* errMsg = NULL;
    int rc = sqlite3_exec(
        db, 
        sql.c_str(), 
        [](void* data, int argc, char** argv, char** azColName)
            {
                // Create an instance of a bucket and return it
                Bucket bucket(String(argv[0]).toInt(), argv[1]);
                return ((LISTBUCKETSCALLBACK)data)(&bucket);
            },
        (void*)callback, 
        &errMsg
    );
    ASSERT(rc == SQLITE_OK, errMsg);
}

boolean Db::limitSettingsExist(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM limit_settings WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addLimitSettings(const int id, const LimitSettings* settings)
{
    Statement stmt(db);
    stmt.prepare("\
        INSERT INTO limit_settings (\
            id,\
            global_correction_factor,\
            min_angle_20x,\
            reset_angle_10x,\
            add_angle_10x,\
            max_angle_10x,\
            c0_weight_kg,\
            x1_weight_kg\
        )\
        VALUES (\
            ?,\
            ?,\
            ?,\
            ?,\
            ?,\
            ?,\
            ?,\
            ?\
        );");
    stmt.bind(1, id);
    stmt.bind(2, settings->global_correction_factor);
    stmt.bind(3, settings->min_angle_20x);
    stmt.bind(4, settings->reset_angle_10x);
    stmt.bind(5, settings->add_angle_10x);
    stmt.bind(6, settings->max_angle_10x);
    stmt.bind(7, settings->c0_weight_kg);
    stmt.bind(8, settings->x1_weight_kg);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateLimitSettings(const int id, const LimitSettings* settings)
{
    Statement stmt(db);
    stmt.prepare("\
        UPDATE limit_settings \
        SET \ 
            global_correction_factor = ?,\
            min_angle_20x = ?,\
            reset_angle_10x = ?,\
            add_angle_10x = ?,\
            max_angle_10x = ?,\
            c0_weight_kg = ?,\
            x1_weight_kg = ?\
        WHERE id = ?");
    stmt.bind(1, settings->global_correction_factor);
    stmt.bind(2, settings->min_angle_20x);
    stmt.bind(3, settings->reset_angle_10x);
    stmt.bind(4, settings->add_angle_10x);
    stmt.bind(5, settings->max_angle_10x);
    stmt.bind(6, settings->c0_weight_kg);
    stmt.bind(7, settings->x1_weight_kg);
    stmt.bind(8, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve 0 rising
boolean Db::c0RisingExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM c0_rising WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addC0Rising(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO c0_rising (id, samples) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateC0Rising(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("UPDATE c0_rising SET samples = ? WHERE id = ?");
    stmt.bind(1, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve 0 lowering
boolean Db::c0LoweringExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM c0_lowering WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addC0Lowering(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO c0_lowering (id, samples) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateC0Lowering(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("UPDATE c0_lowering SET samples = ? WHERE id = ?");
    stmt.bind(1, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve X1 rising
boolean Db::x1RisingExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM x1_rising WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addX1Rising(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO x1_rising (id, samples) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateX1Rising(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("UPDATE x1_rising SET samples = ? WHERE id = ?");
    stmt.bind(1, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve X1 lowering
boolean Db::x1LoweringExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM x1_lowering WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addX1Lowering(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO x1_lowering (id, samples) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateX1Lowering(const int id, const CalibrationSample* samples)
{
    Statement stmt(db);
    stmt.prepare("UPDATE x1_lowering SET samples = ? WHERE id = ?");
    stmt.bind(1, samples, sizeof(CalibrationSample) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve 0 low speed factors
boolean Db::c0LowSpeedFactorExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM c0_low_speed_factors WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addC0LowSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO c0_low_speed_factors (id, factors) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateC0LowSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("UPDATE c0_low_speed_factors SET factors = ? WHERE id = ?");
    stmt.bind(1, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve 0 high speed factors
boolean Db::c0HighSpeedFactorExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM c0_high_speed_factors WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addC0HighSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO c0_high_speed_factors (id, factors) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateC0HighSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("UPDATE c0_high_speed_factors SET factors = ? WHERE id = ?");
    stmt.bind(1, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve X1 low speed factors
boolean Db::x1LowSpeedFactorExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM x1_low_speed_factors WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addX1LowSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO x1_low_speed_factors (id, factors) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateX1LowSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("UPDATE x1_low_speed_factors SET factors = ? WHERE id = ?");
    stmt.bind(1, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

// Curve X1 high speed factors
boolean Db::x1HighSpeedFactorExists(const int id)
{
    Statement stmt(db);
    stmt.prepare("SELECT * FROM x1_high_speed_factors WHERE id = ?");
    stmt.bind(1, id);
    int rc = stmt.step();
    return rc == SQLITE_ROW ? true : false;
}

void Db::addX1HighSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("INSERT INTO x1_high_speed_factors (id, factors) VALUES (?, ?)");
    stmt.bind(1, id);
    stmt.bind(2, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

void Db::updateX1HighSpeedFactor(const int id, const int* factors)
{
    Statement stmt(db);
    stmt.prepare("UPDATE x1_high_speed_factors SET factors = ? WHERE id = ?");
    stmt.bind(1, factors, sizeof(int) * MAX_CALIBRATION_SAMPLES);
    stmt.bind(2, id);
    int rc = stmt.step();
    ASSERT(rc == SQLITE_DONE, sqlite3_errmsg(db));
}

