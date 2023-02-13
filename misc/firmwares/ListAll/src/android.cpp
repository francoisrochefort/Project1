
#include <mc.h>

// Apis
boolean Android::isMessagePending() { return Serial.available(); }

Message Android::getMessage()
{
    // Read the message
    Serial.readStringUntil('<');
    String msg = Serial.readStringUntil('>');
    return Message(msg);
}

// Events
void Android::onAwake() { Serial.printf("<%s>", ON_AWAKE); }
void Android::onAddBucket(int bucketId) { Serial.printf("<%s%i>", ON_ADD_BUCKET, bucketId); }
void Android::onUpdateBucket(int bucketId) { Serial.printf("<%s%i>", ON_UPDATE_BUCKET, bucketId); }
void Android::onDeleteBucket(int bucketId) { Serial.printf("<%s%i>", ON_DELETE_BUCKET, bucketId); }
void Android::onCopyBucket(int bucketId) { Serial.printf("<%s%i>", ON_COPY_BUCKET, bucketId); }
void Android::onCreateDatabase() { Serial.printf("<%s>", ON_CREATE_DATABASE); }
void Android::onError(const String& msg) { Serial.printf("<%s%s>", ON_ERROR, msg); }
void Android::onError(const char* msg) { Serial.printf("<%s%s>", ON_ERROR, msg); }
