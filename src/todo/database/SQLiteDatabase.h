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
 * // Example queries
 * db->Query("CREATE TABLE 'foo' ('bar' 'TEXT')");
 * db->Step();
 * db->Query("INSERT INTO 'foo' ('bar') VALUES ('baz')");
 * db->Step();
 *
 * // run an sql statement and get results
 * db->Query("SELECT * FROM foo");
 * while (db->Next()) {
 * 	std::map<std::string, std::string> row = db->GetRow();
 *	Log->Print(row["bar"]);
 * }
 *
 * // finished
 * db->Close();
 * delete db;
*/

// WARNING: There aren't any safety functions yet. Be careful!
class SQLiteDatabase
{
public:
	// Open/Close a given database file. Creates one if it doesn't exist.
	void Open(std::string db);
	void Close();
	
	// Run an SQL query
	void Query(std::string sql_str);
	
	// Get the next result
	bool Next();
	
	// Get all columns in the current row
	std::map<std::string, std::string> GetRow();
	
private:
	sqlite3 *db_handle;
	sqlite3_stmt *last_query;
};

#endif
