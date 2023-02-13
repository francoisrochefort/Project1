#include <mc.h>

void AwakeState::setContext(Context* context) 
{
    currentContext = context;
}

void AwakeState::doEvents() 
{
    // Any pending message?
    if (android.getMessage()) {

        // Dispatch the message
        String cmd = android.getCmd();
        if (cmd == Android::ADD_BUCKET) {

            // Insert the new bucket
            BucketRepository repo;
            Bucket bucket(android.getVal());
            int bucketId = repo.addBucket(bucket);

            // Send the event
            android.onAddBucket(bucketId);

        } else if (cmd == Android::UPDATE_BUCKET) {

            // Split the val parameter
            String val = android.getVal();
            int period = 0;
            for(; val[period] != 0 && val[period] != ','; period++);
            String bucketId = val.substring(0, period);
            String name = val.substring(period + 1);

            // Update the bucket
    

            // Send the event
            android.onUpdateBucket(bucketId.toInt());

        } else if (cmd == Android::DELETE_BUCKET) {

            // Delete the bucket
            String val = android.getVal();
            int bucketId = val.toInt();

            // Send the event
            android.onDeleteBucket(bucketId);

        } else if (cmd == Android::COPY_BUCKET) {
            
            // Copy the bucket
            String val = android.getVal();
            int src = val.toInt();
            int dest = 2;

            // Send the event
            android.onCopyBucket(dest);
        }
    }
}
