
#include <mc.h>

Bucket::Bucket(const int id, char* name) : id(id), name(String(name))
{
}

int Bucket::getId()
{
    return id;
}

String Bucket::getName() 
{
    return name;
}

Bucket::~Bucket() 
{
}