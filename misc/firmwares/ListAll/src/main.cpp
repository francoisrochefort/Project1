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

    // Perform Mathieu's test
    BucketRepository repo;
    char name[] = "bucket #1";
    int id = repo.addBucket(name);

    // Testing start here
    int cch = 5000;
    char* samples = (char*) malloc(cch);

    // Test c0_rising table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setC0Rising(id, (CalibrationSample*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'a';
    repo.setC0Rising(id, (CalibrationSample*)samples);

    // Test c0_lowering table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setC0Lowering(id, (CalibrationSample*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'b';
    repo.setC0Lowering(id, (CalibrationSample*)samples);

    // Test x1_rising table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setX1Rising(id, (CalibrationSample*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'c';
    repo.setX1Rising(id, (CalibrationSample*)samples);

    // Test x1_lowering table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setX1Lowering(id, (CalibrationSample*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'd';
    repo.setX1Lowering(id, (CalibrationSample*)samples);

    // Test c0_low_speed_factors table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setC0LowSpeedFactor(id, (int*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'e';
    repo.setC0LowSpeedFactor(id, (int*)samples);

    // Test c0_high_speed_factors table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setC0HighSpeedFactor(id, (int*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'f';
    repo.setC0HighSpeedFactor(id, (int*)samples);

    // Test x1_low_speed_factors table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setX1LowSpeedFactor(id, (int*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'g';
    repo.setX1LowSpeedFactor(id, (int*)samples);

    // Test x1_high_speed_factors table and apis
    for (int i = 0; i < cch; i++)
        samples[i] = 'a' + i % 26;
    repo.setX1HighSpeedFactor(id, (int*)samples);
    for (int i = 0; i < cch; i++)
        samples[i] = 'h';
    repo.setX1HighSpeedFactor(id, (int*)samples);

    // // Test limit settings
    // LimitSettings settings1 = {
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26)
    // };
    // repo.setLimitSettings(id, &settings1);
    // LimitSettings settings2 = {
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26),
    //     'a' + ((i++) % 26)
    // };
    // repo.setLimitSettings(id, &settings2);
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