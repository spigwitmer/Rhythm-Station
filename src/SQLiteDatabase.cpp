#include "SQLiteDatabase.h"
#include "Logger.h"

void SQLiteDatabase::Open(std::string db) {
	if(sqlite3_open(db.c_str(), &db_handle) != SQLITE_OK)
		Log->Print("Error opening SQLite Database: \"" + db + "\"");
}

void SQLiteDatabase::Close() {

	sqlite3_close(db_handle);
}

void SQLiteDatabase::Query(std::string sql_str) {
	sqlite3_finalize(last_query);
	sqlite3_prepare(db_handle, sql_str.c_str(), sql_str.size(), &last_query, NULL);
	sqlite3_sql(last_query);
}
