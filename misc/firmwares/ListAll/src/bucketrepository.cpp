#include <mc.h>

int BucketRepository::addBucket(const String& name)
{
    // Check if the bucket exists
    if (db.getBucketId(name) != Db::Result::NoRecordFound) {

        // Return the error
        return ObjectAlreadyExists;
    }
    
    // Get the next bucket identifier
    int id = db.getNextVal(BucketId);

    // Insert the new bucket
    db.addBucket(id, name);

    // Return the bucket identifier 
    return id;
}

Result BucketRepository::updateBucket(const int id, const String& name)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Check if a different bucket with the same name exists
    int other = db.getBucketId(name);
    if (other != Db::Result::NoRecordFound && other != id) {

        // Return the error
        return ObjectAlreadyExists;
    }

    // Update the bucket name
    db.updateBucket(id, name);
    return Succeeded;
}

Result BucketRepository::setLimitSettings(const int id, const LimitSettings* settings)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update limit settings accordingly
    if (!db.limitSettingsExist(id)) {

        // Insert limit settings
        Serial.println("j'ajoute les ti-samples limit_settings");
        db.addLimitSettings(id, settings);
    }
    else {

        // Update limit settings
        Serial.println("j'update les ti-samples limit_settings");
        db.updateLimitSettings(id, settings);
    }
    return Succeeded;
}

Result BucketRepository::setC0Rising(const int id, const CalibrationSample* samples)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update samples accordingly
    if (!db.c0RisingExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-samples c0_rising");
        db.addC0Rising(id, samples);
    }
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-samples c0_rising");
        db.updateC0Rising(id, samples);
    }
    return Succeeded;
}

Result BucketRepository::setC0Lowering(const int id, const CalibrationSample* samples)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update samples accordingly
    if (!db.c0LoweringExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-samples c0_lowering");
        db.addC0Lowering(id, samples);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-samples  c0_lowering");
        db.updateC0Lowering(id, samples);
    }
    return Succeeded;
}

Result BucketRepository::setX1Rising(const int id, const CalibrationSample* samples)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update samples accordingly
    if (!db.x1RisingExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-samples x1_rising");
        db.addX1Rising(id, samples);
    }
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-samples x1_rising");
        db.updateX1Rising(id, samples);
    }
    return Succeeded;
}

Result BucketRepository::setX1Lowering(const int id, const CalibrationSample* samples)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update samples accordingly
    if (!db.x1LoweringExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-samples x1_lowering");
        db.addX1Lowering(id, samples);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-samples  x1_lowering");
        db.updateX1Lowering(id, samples);
    }
    return Succeeded;
}

Result BucketRepository::setC0LowSpeedFactor(const int id, const int* factors)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update factors accordingly
    if (!db.c0LowSpeedFactorExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-factors c0_low_speed_factor");
        db.addC0LowSpeedFactor(id, factors);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-factors  c0_low_speed_factor");
        db.updateC0LowSpeedFactor(id, factors);
    }
    return Succeeded;
}

Result BucketRepository::setC0HighSpeedFactor(const int id, const int* factors)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update factors accordingly
    if (!db.c0HighSpeedFactorExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-factors c0_high_speed_factor");
        db.addC0HighSpeedFactor(id, factors);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-factors  c0_high_speed_factor");
        db.updateC0HighSpeedFactor(id, factors);
    }
    return Succeeded;
}

Result BucketRepository::setX1LowSpeedFactor(const int id, const int* factors)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update factors accordingly
    if (!db.x1LowSpeedFactorExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-factors x1_high_speed_factor");
        db.addX1LowSpeedFactor(id, factors);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-factors  x1_high_speed_factor");
        db.updateX1LowSpeedFactor(id, factors);
    }
    return Succeeded;
}

Result BucketRepository::setX1HighSpeedFactor(const int id, const int* factors)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Add or update factors accordingly
    if (!db.x1HighSpeedFactorExists(id)) {

        // Insert bucket calibration
        Serial.println("j'ajoute les ti-factors x1_high_speed_factor");
        db.addX1HighSpeedFactor(id, factors);
    }  
    else {

        // Update bucket calibration
        Serial.println("j'update les ti-factors  x1_high_speed_factor");
        db.updateX1HighSpeedFactor(id, factors);
    }
    return Succeeded;
}

Result BucketRepository::deleteBucket(const int id)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return ObjectDoesNotExists;
    }

    // Delete the bucket
    db.deleteBucket(id);
    return Succeeded;
}

void BucketRepository::listAll(LISTBUCKETSCALLBACK callback)
{
    db.listBuckets(callback);
}