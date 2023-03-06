
#include <mc.h>

Android::Message::Message(const String& msg)
{
    cmd = msg.substring(0, 4);
    val = msg.substring(4);
    nextChar = 0;
}

Cmd Android::Message::getCmd()
{
    if (cmd == Android::AWAKE) return Cmd::Awake;
    else if (cmd == Android::ADD_BUCKET) return Cmd::AddBucket;
    else if (cmd == Android::UPDATE_BUCKET) return Cmd::UpdateBucket;
    else if (cmd == Android::DELETE_BUCKET) return Cmd::DeleteBucket;
    else if (cmd == Android::COPY_BUCKET) return Cmd::CopyBucket;
    else if (cmd == Android::SELECT_BUCKET) return Cmd::SelectBucket;
    else if (cmd == Android::LIST_BUCKETS) return Cmd::ListBuckets;
    else if (cmd == Android::QUERY) return Cmd::Query;
    else return Cmd::Undefined;
}

String Android::Message::getStringParam()
{
    int period = nextChar;
    for(; val[period] != 0 && val[period] != ','; period++);
    String param = val.substring(nextChar, period);
    nextChar = ++period;
    return param;
}   

int Android::Message::getIntParam()
{
    return getStringParam().toInt();
}