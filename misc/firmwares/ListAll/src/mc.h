#if !defined MC_H
#define MC_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <sqlite3.h>
#include <stdarg.h>
#include <assert.h>

// Serial port settings
#define BAUD_RATE 115200

// SPIFFS settings
#define FORMAT_SPIFFS_IF_FAILED true

// Forward declaration
class Context;
class State;
class AsleepState;
class Db;
class Bucket;
class BucketRepository;
class Message;
class Android;

typedef int (*LISTBUCKETSCALLBACK)(Bucket* bucket);

// Machine state
class State
{
public:
    virtual void setContext(Context* context) = 0;
    virtual void doEvents() = 0;
};

// All machine states
class AsleepState : public State 
{
    Context* currentContext;
public:
    virtual void setContext(Context* context);
    virtual void doEvents();
};

class AwakeState : public State 
{
    Context* currentContext;

    // Event handlers
    void onAddBucket(const String& name);
    void onUpdateBucket(const int id, const String& name);
    void onDeleteBucket(const int id);
    void onCopyBucket(const int id, const String& name);
    void onListBuckets();

public:

    // Overides
    virtual void setContext(Context* context);
    virtual void doEvents();
};

class Context {
    State* currentState;
public:
    Context();
    void setState(State* state) ;
    void doEvents();
};

enum Seq {
    BucketId = 1
};

class Db {
    sqlite3* db;

public:
    Db();

    // Database
    void open(boolean createDatabase); 
    void createDatabase();

    // Sequences
    int getNextVal(Seq seq);

    // Buckets
    void addBucket(const int id, const String& name);
    int getBucketId(const String& name);
    void updateBucket(const int id, const String& name);
    boolean bucketExists(const int id);
    void deleteBucket(const int id);
    void listBuckets(LISTBUCKETSCALLBACK callback);
};

class Bucket {
    int id;
    String name;
public:
    Bucket(const int id, char* name);
    int getId();
    String getName();
    virtual ~Bucket();
};

class SequenceRepository {
public:
    int getNextVal(Seq seq);
};

class BucketRepository {

public:
    int addBucket(const String& name);
    boolean updateBucket(const int id, const String& name);
    boolean deleteBucket(const int id);
    // int copyBucket(const int id, const String& name);
    void listAll(LISTBUCKETSCALLBACK callback);
};

enum class Cmd {
    Undefined,
    Awake,
    AddBucket,
    UpdateBucket,
    DeleteBucket,
    CopyBucket,
    SelectBucket,
    ListBuckets
};

class Android {

    // Messages
    static constexpr const char* ON_AWAKE           = "EV01";
    static constexpr const char* ON_ADD_BUCKET      = "EV02";
    static constexpr const char* ON_UPDATE_BUCKET   = "EV03";
    static constexpr const char* ON_DELETE_BUCKET   = "EV04";
    static constexpr const char* ON_COPY_BUCKET     = "EV05";
    static constexpr const char* ON_SELECT_BUCKET   = "EV06";
    static constexpr const char* ON_NEXT_BUCKET     = "EV07";
    static constexpr const char* ON_CREATE_DATABASE = "EV98";
    static constexpr const char* ON_ERROR           = "EV99";

    // Commands
    static constexpr const char* AWAKE         = "CM01";
    static constexpr const char* ADD_BUCKET    = "CM02";
    static constexpr const char* UPDATE_BUCKET = "CM03";
    static constexpr const char* DELETE_BUCKET = "CM04";
    static constexpr const char* COPY_BUCKET   = "CM05";
    static constexpr const char* SELECT_BUCKET = "CM06";
    static constexpr const char* LIST_BUCKETS  = "CM07";

public:

    // Message
    class Message {
        String cmd;
        String val;
        int nextChar;
    public:
        Message(const String& msg);
        Cmd getCmd();
        String getStringParam();
        int getIntParam();
    };

    // Apis
    boolean isMessagePending();
    Message getMessage();

    // Events
    void onAwake();
    void onAddBucket(int bucketId);
    void onUpdateBucket(int bucketId);
    void onDeleteBucket(int bucketId);
    void onCopyBucket(int bucketId);
    void onNextBucket(Bucket* Bucket);
    void onCreateDatabase();
    void onError(const String& msg);
    void onError(const char* msg);
};

// Debug macro
#define ASSERT( __e, msg) \
    if (!__e) { \
        android.onError(msg); \
        assert(__e); \
    }  

// Global variables
extern Db db;
extern Android android;

#endif