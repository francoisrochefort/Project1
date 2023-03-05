
#include <mc.h>

Statement::Statement(sqlite3* db) : db(db), stmt(NULL)
{
}

Statement::~Statement()
{
    if (stmt) sqlite3_finalize(stmt);
}

void Statement::prepare(const char* sql) 
{
    const char* tail;
    int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &tail);
    ASSERT(rc == SQLITE_OK, sqlite3_errmsg(db));
}

void Statement::bind(int index, int value)
{
    int rc = sqlite3_bind_int(stmt, index, value);
    ASSERT(rc == SQLITE_OK, sqlite3_errmsg(db));
}

void Statement::bind(int index, const void* blob, int cb)
{
    int rc = sqlite3_bind_blob(stmt, index, blob, cb, SQLITE_STATIC);
    ASSERT(rc == SQLITE_OK, sqlite3_errmsg(db));
}

void Statement::bind(int index, const String& text)
{
    int rc = sqlite3_bind_text(stmt, index, text.c_str(), -1, SQLITE_STATIC);
    ASSERT(rc == SQLITE_OK, sqlite3_errmsg(db));
}

int Statement::getColumnInt(int index)
{
    return sqlite3_column_int(stmt, index);
}

int Statement::step()
{
    return sqlite3_step(stmt);
}