#include "SQLiteDatabase.h"
#include "Logger.h"
#include <sstream>
#include <map>

using namespace std;

void SQLiteDatabase::Open(string db)
{
	// this can be written as simply if (sqlite3_open), but this is more clear.
	if(sqlite3_open(db.c_str(), &db_handle) != SQLITE_OK)
		Log->Print("Error opening SQLite Database: \"%s\"", db.c_str());
}

void SQLiteDatabase::Close()
{
	// close the database.
	sqlite3_close(db_handle);
}

void SQLiteDatabase::Query(string sql_str)
{
	// reset query state.
	sqlite3_finalize(last_query);
	
	// prepare the statement and run it
	sqlite3_prepare_v2(db_handle, sql_str.c_str(), sql_str.size(), &last_query, NULL);
	sqlite3_sql(last_query);
}

// returns false until it's done.
bool SQLiteDatabase::Next()
{
	int status = sqlite3_step(last_query);
	ostringstream err;
	
	switch (status)
	{
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

map<string, string> SQLiteDatabase::GetRow()
{
	map<string, string> results;
	int cols = sqlite3_column_count(last_query);
	
	for (int i = 0; i<cols; i++)
	{
		string col_name = sqlite3_column_name(last_query, i);
		results[col_name] = (const char *)sqlite3_column_text(last_query, i);
	}
	
	return results;
}


/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
