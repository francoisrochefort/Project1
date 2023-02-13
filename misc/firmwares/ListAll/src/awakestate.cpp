#include <mc.h>

void AwakeState::setContext(Context* context) 
{
    currentContext = context;
}

void AwakeState::doEvents() 
{
    // Any pending message?
    if (android.isMessagePending()) {

        // Read and dispatch the message
        Message msg = android.getMessage();
        switch (msg.getCmd()) {
            
        case Cmd::AddBucket:
            {
                // Insert the new bucket
                String name = msg.getNextParam();
                Bucket bucket(name);
                BucketRepository repo;
                int bucketId = repo.addBucket(bucket);

                // Send the event
                android.onAddBucket(bucketId);
            }
            break;
        case Cmd::UpdateBucket:
            {
                // Update the bucket
                int bucketId = msg.getNextParam().toInt();
                String name =  msg.getNextParam();

                // Send the event
                android.onUpdateBucket(bucketId);
            }
            break;
        case Cmd::DeleteBucket:
            {
                // Delete the bucket
                int bucketId = msg.getNextParam().toInt();

                // Send the event
                android.onDeleteBucket(bucketId);
            }
            break;
        case Cmd::CopyBucket:
            {
                // Copy the bucket
                int src =  msg.getNextParam().toInt();
                int dest = 2;

                // Send the event
                android.onCopyBucket(dest);
            }
            break;
        default:
            {
            }
        }
    }
}
