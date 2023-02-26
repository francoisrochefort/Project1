
#include <mc.h>

void AwakeState::AwakeState::onAddBucket(const String& name)
{
    // Insert the new bucket
    BucketRepository repo;
    int id = repo.addBucket(name);
    if (id == ObjectAlreadyExists) {

        // Throw an error if the bucket exists
        android.onError("Object already exists");
        return;
    }

    // Send the event
    android.onAddBucket(id);
}

void AwakeState::onUpdateBucket(const int id, const String& name)
{
    // Update the bucket
    BucketRepository repo;
    Result result = repo.updateBucket(id, name);
    if (result == ObjectDoesNotExists) {

        // Throw an error if the bucket does not exists
        android.onError("Object does not exists");
        return;
    }
    else if (result == ObjectAlreadyExists) {

        // Throw an error if the bucket already exists
        android.onError("Object already exists");
        return;
    }

    // Send the event
    android.onUpdateBucket(id);
}

void AwakeState::onDeleteBucket(const int id)
{
    // Delete the bucket
    BucketRepository repo;
    Result result = repo.deleteBucket(id);
    if (result == ObjectDoesNotExists) {

        // Throw an error if the bucket does not exists
        android.onError("Object does not exists");
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
    // Query the bucket list
    BucketRepository repo;
    repo.listAll([](Bucket* bucket) 
        {
            // Send each bucket one by one
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
    // Check for any pending message
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
