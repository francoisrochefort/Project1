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