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