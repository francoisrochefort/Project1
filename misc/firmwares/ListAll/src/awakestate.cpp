#include <mc.h>

void AwakeState::AwakeState::onAddBucket(const String& name)
{
    // Insert the new bucket
    BucketRepository repo;
    int id = repo.addBucket(name);
    if(id == -1) {

        // Throw an error if the bucket exists
        android.onError("A bucket with the same name already exists");
        return;
    }

    // Send the event
    android.onAddBucket(id);
}

void AwakeState::onUpdateBucket(const int id, const String& name)
{
    // Update the bucket
    BucketRepository repo;
    if (!repo.updateBucket(id, name)) {

        // Throw an error if the bucket exists
        android.onError("A bucket with the same name already exists or the specified bucket does not exists");
        return;
    }

    // Send the event
    android.onUpdateBucket(id);
}

void AwakeState::onDeleteBucket(const int id)
{
    // Delete the bucket
    BucketRepository repo;
    if (!repo.deleteBucket(id)) {

        // Throw an error if the bucket does not exists
        android.onError("Bucket does not exists");
        return;
    }

    // Send the event
    android.onDeleteBucket(id);
}

void AwakeState::onCopyBucket(const int id, const String& name)
{
    // Send the event
    android.onCopyBucket(id);
}

void AwakeState::onListBuckets()
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

void AwakeState::setContext(Context* context) 
{
    currentContext = context;
}

void AwakeState::doEvents() 
{
    // Is there any pending message
    if (android.isMessagePending()) {

        // Read and dispatch the message
        Android::Message msg = android.getMessage();

        // Dispatch the message
        switch (msg.getCmd()) {
        case Cmd::AddBucket:
            onAddBucket(msg.getStringParam());
            break;
        case Cmd::UpdateBucket:
            onUpdateBucket(msg.getIntParam(), msg.getStringParam());
            break;
        case Cmd::DeleteBucket:
            onDeleteBucket(msg.getIntParam());
            break;
        case Cmd::CopyBucket:
            onCopyBucket(msg.getIntParam(), msg.getStringParam());
            break;
        case Cmd::ListBuckets:
            onListBuckets();
            break;
        default:
            break;
        }
    }
}
