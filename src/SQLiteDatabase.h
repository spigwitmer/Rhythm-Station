#ifndef _SQLITE_DATABASE_H_
#define _SQLITE_DATABASE_H_

#include <sqlite3.h>
#include <string>
#include <map>

/*
 * Usage:
 * // make database instance and create db in memory.
 * SQLiteDatabase* db = new SQLiteDatabase();
 * db->Open(":memory:");
 *
 * // run an sql statement and get results
 * db->Query("SELECT * FROM foo");
 * while(db->Step()) {
 * 	map<string, string> row = db->GetRow();
 * 	Log->Print(row["bar"]);
 * }
 *
 * // finished
 * db->Close();
 * delete db;
 */

class SQLiteDatabase {
public:
	void Open(std::string db);
	void Close();

	void Query(std::string sql_str);
	bool Step();
	std::map<std::string, std::string> GetRow();

private:
	sqlite3* db_handle;
	sqlite3_stmt* last_query;
};

#endif
