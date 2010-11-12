#ifndef _SQLITE_DATABASE_H_
#define _SQLITE_DATABASE_H_

#include <sqlite3.h>
#include <string>

class SQLiteDatabase {
public:
	void Open(std::string db);
	void Close();
	void Query(std::string sql_str);
private:
	// Completely guessing at how to implement this. It's 95% likely to be wrong.
	sqlite3* db_handle;
	sqlite3_stmt* last_query;
};

#endif
