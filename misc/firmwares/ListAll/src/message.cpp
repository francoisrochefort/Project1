
#include <mc.h>

Message::Message(const String& msg)
{
    cmd = msg.substring(0, 4);
    val = msg.substring(4);
}

Cmd Message::getCmd()
{
    if (cmd == Android::AWAKE) return Cmd::Awake;
    else if (cmd == Android::ADD_BUCKET) return Cmd::AddBucket;
    else if (cmd == Android::UPDATE_BUCKET) return Cmd::UpdateBucket;
    else if (cmd == Android::DELETE_BUCKET) return Cmd::DeleteBucket;
    else if (cmd == Android::COPY_BUCKET) return Cmd::CopyBucket;
    else if (cmd == Android::SELECT_BUCKET) return Cmd::SelectBucket;
    else return Cmd::Undefined;
}

String Message::getNextParam()
{
    int period = nextChar;
    for(; val[period] != 0 && val[period] != ','; period++);
    String param = val.substring(0, period);
    nextChar = ++period;
    return param;
}   
