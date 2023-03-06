
#include <mc.h>

boolean Android::isMessagePending() { return Serial.available(); }

Android::Message Android::getMessage()
{
    Serial.readStringUntil('<');
    String msg = Serial.readStringUntil('>');
    return Android::Message(msg);
}

void Android::onAwake() { Serial.printf("<%s>", ON_AWAKE); }
void Android::onAddBucket(int bucketId) { Serial.printf("<%s%i>", ON_ADD_BUCKET, bucketId); }
void Android::onUpdateBucket(int bucketId) { Serial.printf("<%s%i>", ON_UPDATE_BUCKET, bucketId); }
void Android::onDeleteBucket(int bucketId) { Serial.printf("<%s%i>", ON_DELETE_BUCKET, bucketId); }
void Android::onCopyBucket(int bucketId) { Serial.printf("<%s%i>", ON_COPY_BUCKET, bucketId); }
void Android::onNextBucket(Bucket* bucket) { Serial.printf("<%s%i,%s>", ON_NEXT_BUCKET, bucket->getId(), bucket->getName()); }

void Android::onNextRow(const String& row) { Serial.printf("<%s%s>", ON_QUERY_ROW, row.c_str()); }

void Android::onCreateDatabase() { Serial.printf("<%s>", ON_CREATE_DATABASE); }
void Android::onError(const String& msg) { Serial.printf("<%s%s>", ON_ERROR, msg); }
void Android::onError(const char* msg) { Serial.printf("<%s%s>", ON_ERROR, msg); }
