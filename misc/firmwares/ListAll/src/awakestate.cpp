#include <mc.h>

// int AwakeState::callback(Bucket* bucket) {
// 
//     // Send each bucket one by one to android
//     android.onNextBucket(bucket);
//     return 0;
// }

void AwakeState::setContext(Context* context) 
{
    currentContext = context;
}

void AwakeState::doEvents() 
{
    if (android.isMessagePending()) {

        // Read and dispatch the message
        Android::Message msg = android.getMessage();
        switch (msg.getCmd()) {

        case Cmd::AddBucket:
            {
                // Insert the new bucket
                String name = msg.getStringParam();
                BucketRepository repo;
                int id = repo.addBucket(name);
                if(id == -1) {

                    // Throw an error if the bucket exists
                    android.onError("A bucket with the same name already exists");
                    break;
                }

                // Send the event
                android.onAddBucket(id);
            }
            break;
        case Cmd::UpdateBucket:
            {
                // Update the bucket
                int id = msg.getIntParam();
                String name =  msg.getStringParam();
                BucketRepository repo;
                if (!repo.updateBucket(id, name)) {

                    // Throw an error if the bucket exists
                    android.onError("A bucket with the same name already exists or the specified bucket does not exists");
                    break;
                }

                // Send the event
                android.onUpdateBucket(id);
            }
            break;
        case Cmd::DeleteBucket:
            {
                // Delete the bucket
                int id = msg.getIntParam();
                BucketRepository repo;
                if (!repo.deleteBucket(id)) {

                    // Throw an error if the bucket does not exists
                    android.onError("Bucket does not exists");
                    break;
                }

                // Send the event
                android.onDeleteBucket(id);
            }
            break;
        case Cmd::CopyBucket:
            {
                // Copy the bucket
                int src =  msg.getIntParam();
                int dest = 2;

                // Send the event
                android.onCopyBucket(dest);
            }
            break;
        case Cmd::ListBuckets:
            {
                // Send each bucket one by one to android
                BucketRepository repo;
                repo.listAll([](Bucket* bucket) 
                    {
                        android.onNextBucket(bucket);
                        return 0;
                    }
                );
            }
            break;
        default:
            {
            }
        }
    }
}
