#include "SQLiteDatabase.h"
#include "Logger.h"
#include <sstream>
#include <map>

using namespace std;

void SQLiteDatabase::Open(string db) {
	// this can be written as simply if (sqlite3_open), but this is more clear.
	if(sqlite3_open(db.c_str(), &db_handle) != SQLITE_OK)
		Log->Print("Error opening SQLite Database: \"%s\"", db.c_str());
}

void SQLiteDatabase::Close() {
	// close the database.
	sqlite3_close(db_handle);
}

void SQLiteDatabase::Query(string sql_str) {
	// reset query state.
	sqlite3_finalize(last_query);

	// prepare the statement and run it
	sqlite3_prepare_v2(db_handle, sql_str.c_str(), sql_str.size(), &last_query, NULL);
	sqlite3_sql(last_query);
}

// returns false until it's done.
bool SQLiteDatabase::Step() {
	int status = sqlite3_step(last_query);
	int cols = 0;
	ostringstream err;

	switch (status) {
		case SQLITE_ERROR:
			err << sqlite3_errmsg(db_handle);
			Log->Print(err.str());
		case SQLITE_DONE:
			return false;
		case SQLITE_ROW:
			return true;
		default:
			break;
	}

	// you shouldn't end up here!
	return false;
}

map<string, string> SQLiteDatabase::GetRow() {
	map<string, string> results;

	int cols = sqlite3_column_count(last_query);
	for (int i = 0; i<cols; i++) {
		string col_name = sqlite3_column_name(last_query, i);
		results[col_name] = (const char*)sqlite3_column_text(last_query, i);
	}

	return results;
}
