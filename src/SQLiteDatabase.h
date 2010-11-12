#ifndef _SQLITE_DATABASE_H_
#define _SQLITE_DATABASE_H_

#include <sqlite3.h>
#include <string>

class SQLiteDatabase {
public:
	void Open();
	void Close();
	void Query(std::string fmt, ...);
private:
	// Completely guessing at how to implement this. It's 95% likely to be wrong.
	sqlite3* sqlite_handle;
	sqlite3_blob** sqlite_blob;
};

#endif
