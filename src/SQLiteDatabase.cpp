#include "SQLiteDatabase.h"

void SQLiteDatabase::Open() {
	sqlite3_blob_open(sqlite_handle, ":memory:", "", "", NULL, NULL, sqlite_blob);
}

void SQLiteDatabase::Close() {
	// todo
}

void SQLiteDatabase::Query(std::string fmt, ...) {
	// todo
}
