#include <mc.h>

// const char* data = "Callback function called";
// static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    Serial.printf("%s: ", (const char*)data);
//    for (i = 0; i<argc; i++){
//        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    Serial.printf("\n");
//    return 0;
// }

// char *zErrMsg = 0;
// int db_exec(sqlite3 *db, const char *sql) {
//    Serial.println(sql);
//    long start = micros();
//    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//    if (rc != SQLITE_OK) {
//        Serial.printf("SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        Serial.printf("Operation done successfully\n");
//    }
//    Serial.print(F("Time taken:"));
//    Serial.println(micros()-start);
//    return rc;
// }

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

void AwakeState::onQuery(const String& sql)
{
    Serial.println();
    Serial.println();

    // Query the database
    char* errMsg = NULL;
    int rc = db.query(sql.c_str(), callback, (void*)this, &errMsg);
    if (rc != SQLITE_OK) {

        // Return the error
        android.onError(errMsg);
        sqlite3_free(errMsg);
    }
}

int AwakeState::callback(void* data, int argc, char** argv, char** azColName)
{
    // Print out query result
    AwakeState* awakeState = (AwakeState*)data;
    for (int i = 0; i < argc; i++){
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
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
        case Cmd::Query:
            onQuery(msg.getStringParam());
            break;
        default:
            break;
        }
    }
}
