
#include <mc.h>

int BucketRepository::addBucket(const Bucket& bucket)
{
    // Bucket name already exists?
    if (db.isBucketName(bucket.name)) {
        android.onError("Bucket name already exists");
        return -1;
    }
    return 1;
}
