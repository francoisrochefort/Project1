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
public:
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

class Db {
    sqlite3* db;

    class BucketExistsCmd {
        sqlite3* db;
        static int callback(void *data, int argc, char **argv, char **azColName);
    public:
        BucketExistsCmd(sqlite3* db);
        boolean run(const String& name);
    };

public:
    Db();
    boolean open(boolean init); 
    bool create();
    bool isBucketName(const String& name);
    bool addBucket(const Bucket& bucket);
};

class Bucket {
public:
    int id;
    String name;
    Bucket(const String& name);
    Bucket();
};

class BucketRepository {
public:
    int addBucket(const Bucket& bucket);
    // void updateBucket(const Bucket& bucket);
    // void deleteBucket(const int bucketId);
    // int copyBucket(const int bucketId, const String& name);    
};

enum class Cmd {
    Undefined,
    Awake,
    AddBucket,
    UpdateBucket,
    DeleteBucket,
    CopyBucket,
    SelectBucket
};

class Message {
    String cmd;
    String val;
    int nextChar;
public:
    Message(const String& msg);
    Cmd getCmd();
    String getNextParam();
};

class Android {

    // Messages
    static constexpr const char* ON_AWAKE           = "EV01";
    static constexpr const char* ON_ADD_BUCKET      = "EV02";
    static constexpr const char* ON_UPDATE_BUCKET   = "EV03";
    static constexpr const char* ON_DELETE_BUCKET   = "EV04";
    static constexpr const char* ON_COPY_BUCKET     = "EV05";
    static constexpr const char* ON_SELECT_BUCKET   = "EV06";
    static constexpr const char* ON_CREATE_DATABASE = "EV98";
    static constexpr const char* ON_ERROR           = "EV99";

public:

    // Commands
    static constexpr const char* AWAKE         = "CM01";
    static constexpr const char* ADD_BUCKET    = "CM02";
    static constexpr const char* UPDATE_BUCKET = "CM03";
    static constexpr const char* DELETE_BUCKET = "CM04";
    static constexpr const char* COPY_BUCKET   = "CM05";
    static constexpr const char* SELECT_BUCKET = "CM06";


    // Apis
    boolean isMessagePending();
    Message getMessage();

    // Events
    void onAwake();
    void onAddBucket(int bucketId);
    void onUpdateBucket(int bucketId);
    void onDeleteBucket(int bucketId);
    void onCopyBucket(int bucketId);
    void onCreateDatabase();
    void onError(const String& msg);
    void onError(const char* msg);
};

// Global variables
extern Db db;
extern Android android;

#endif