#ifndef _SQLITE_DATABASE_H_
#define _SQLITE_DATABASE_H_

#include <sqlite3.h>
#include <string>
#include <map>

/*
 * Usage:
 * // make database instance and create db in memory.
 * SQLiteDatabase* db = new SQLiteDatabse();
 * db->Open(":memory:");
 * // run an sql statement and get results
 * db->Query("SELECT * FROM foo");
 * while(db->Step()) {
 *	map<std::string, std::string> row = db->GetRow();
 *	Log->Print(row["bar"]);
 * }
 * // finished
 * db->Close();
 */

class SQLiteDatabase {
public:
	void Open(std::string db);
	void Close();
	void Query(std::string sql_str);
	bool Step();
	std::map<std::string, std::string> GetRow();

private:
	// Completely guessing at how to implement this. It's 95% likely to be wrong.
	sqlite3* db_handle;
	sqlite3_stmt* last_query;
};

#endif
