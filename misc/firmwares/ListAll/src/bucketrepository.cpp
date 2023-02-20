
/**
 * @file bucketrepository.cpp
 * @author Francois Rochefort (francoisrochefort@hotmail.fr)
 * @brief Buckets API
 * @version 0.1
 * @date 2023-02-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <mc.h>

int BucketRepository::addBucket(const String& name)
{
    // Check if the bucket exists
    if (db.getBucketId(name) != -1) {

        // Return the error
        return -1;
    }
    
    // Get the next bucket identifier
    int id = db.getNextVal(BucketId);

    // Insert the new bucket
    db.addBucket(id, name);

    // Return the bucket identifier 
    return id;
}

boolean BucketRepository::updateBucket(const int id, const String& name)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return false;
    }

    // Check if a different bucket with the same name exists
    int other = db.getBucketId(name);
    if (other != -1 && other != id) {

        // Return the error
        return false;
    }

    // Update the bucket name
    db.updateBucket(id, name);
    return true;
}

boolean BucketRepository::deleteBucket(const int id)
{
    // Check if the bucket exists
    if (!db.bucketExists(id)) {

        // Return the error
        return false;
    }

    // Delete the bucket
    db.deleteBucket(id);
    return true;
}

void BucketRepository::listAll(LISTBUCKETSCALLBACK callback)
{
    db.listBuckets(callback);
}