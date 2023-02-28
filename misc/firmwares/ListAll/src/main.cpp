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

    CalibrationSample samples[MAX_CALIBRATION_SAMPLES];
    int i = 1;

    // Test c0_rising table and apis
    for (int j = 0; j < MAX_CALIBRATION_SAMPLES; j++) {
        samples[j].timestamps = i++;
        samples[j].angle = i++;
        samples[j].speed = i++;
        samples[j].pressure = i++;
    }
    repo.setC0Rising(id, samples);

    for (int k = 0; k < MAX_CALIBRATION_SAMPLES; k++) {
        samples[k].timestamps = i++;
        samples[k].angle = i++;
        samples[k].speed = i++;
        samples[k].pressure = i++;
    }
    repo.setC0Rising(id, samples);

    // Test c0_lowering table and apis
    for (int l = 0; l < MAX_CALIBRATION_SAMPLES; l++) {
        samples[l].timestamps = i++;
        samples[l].angle = i++;
        samples[l].speed = i++;
        samples[l].pressure = i++;
    }
    repo.setC0Lowering(id, samples);

        for (int m = 0; m < MAX_CALIBRATION_SAMPLES; m++) {
        samples[m].timestamps = i++;
        samples[m].angle = i++;
        samples[m].speed = i++;
        samples[m].pressure
         = i++;
    }
    repo.setC0Lowering(id, samples);

    // Test x1_rising table and apis
    for (int n = 0; n < MAX_CALIBRATION_SAMPLES; n++) {
        samples[n].timestamps = i++;
        samples[n].angle = i++;
        samples[n].speed = i++;
        samples[n].pressure = i++;
    }
    repo.setX1Rising(id, samples);

    for (int o = 0; o < MAX_CALIBRATION_SAMPLES; o++) {
        samples[o].timestamps = i++;
        samples[o].angle = i++;
        samples[o].speed = i++;
        samples[o].pressure = i++;
    }
    repo.setX1Rising(id, samples);

    // Test x1_lowering table and apis
    for (int p = 0; p < MAX_CALIBRATION_SAMPLES; p++) {
        samples[p].timestamps = i++;
        samples[p].angle = i++;
        samples[p].speed = i++;
        samples[p].pressure = i++;
    }
    repo.setX1Lowering(id, samples);

    for (int q = 0; q < MAX_CALIBRATION_SAMPLES; q++) {
        samples[q].timestamps = i++;
        samples[q].angle = i++;
        samples[q].speed = i++;
        samples[q].pressure = i++;
    }
    repo.setX1Lowering(id, samples);

    Serial.println("C'est tigidoo!");
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